#include "Overture.h"  
#include "PlotStuff.h"
#include "AnnulusMapping.h"
#include "MappedGridFiniteVolumeOperators.h"
#include "NameList.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  printf(" --------------------------------------------------- \n");
  printf(" Solve a convection-diffusion equation on an annulus \n");
  printf(" Input parameters with the NameList class            \n");
  printf(" Show how to apply boundary conditions explicitly    \n");
  printf(" --------------------------------------------------- \n");

  // Set default values for parameters. These can be optionally changed below
  int numberOfTimeSteps=1000;
  real dt=.01;
  IntegerArray bc(2,3); bc=1;
  NameList nl;       // The NameList object allows one to read in values by name
  aString name(80),answer(80);
  printf(
   " Parameters for Example 3: \n"
   " ------------------------- \n"
   "   name                                                 type    default  \n"
   "numberOfTimeSteps  (nts=)                              (int)      %i     \n"
   "time step (dt=)                                        (real)     %f     \n"
   "boundary conditions (bc(side,axis)=)                   (IntegerArray)        \n",
      numberOfTimeSteps,dt);

  // ==========Loop for changing parameters========================
  for( ;; ) 
  {
    cout << "Enter changes to variables, exit to continue" << endl;
    getLine(answer);
    if( answer=="exit" ) break;
    nl.getVariableName( answer, name );   // parse the answer
    if( name== "numberOfTimeSteps" || name=="nts" )   
      numberOfTimeSteps=nl.intValue(answer);  
    else if( name== "dt" )   
      dt=nl.realValue(answer);  
    else if( name== "bc" )   
      nl.getIntArray( answer,bc );
    else
      cout << "unknown response: [" << name << "]" << endl;
  }

  Mapping *mapping;                                  // keep a pointer to a mapping
  mapping = new AnnulusMapping();                    // create an Annulus
  mapping->setGridDimensions(axis1,41);              // axis1==0, set no. of grid points
  mapping->setGridDimensions(axis2,13);              // axis2==1, set no. of grid points
  MappedGrid mg(*mapping);                           // MappedGrid for a square
  mg.changeToAllCellCentered();  
  mg.update();                                       // create default variables

  Range all;
  realMappedGridFunction u;
  u.updateToMatchGrid(mg,all,all,all,1);          // define after declaration (like resize)
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components

  Index I1,I2,I3, Ib1,Ib2,Ib3;
  // The A++ array mg.dimension()(2,3) holds index bounds on all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);               // assign I1,I2,I3 from dimension
  u(I1,I2,I3)=1.;                                // initial conditions
    
  MappedGridFiniteVolumeOperators op(mg);                    // operators 
  u.setOperators(op);                            // associate with a grid function

  PlotStuff ps(TRUE,"mappedGridExample3");      // create a PlotStuff object
  PlotStuffParameters psp;                       // This object is used to change plotting parameters
  char buffer[80];

  real t=0, a=1., b=1., nu=.05;
  for( int step=0; step<numberOfTimeSteps; step++ )
  {
    if( step % 5 == 0 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e",t));  // set title
      ps.erase();
      PlotIt::contour(ps, u,psp );
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
      ps.redraw(TRUE);
    }
    u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.laplacian()) );
    t+=dt;
    // apply Boundary conditions explicitly (just to demonstrate, it is easier to use the operators)
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( int side=Start; side<=End; side++ )
      { // only assign BC's on sides with a positive boundary condition:
	if( mg.boundaryCondition()(side,axis) > 0 )
	{ // fill in boundary values
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  u(Ib1,Ib2,Ib3)=0.; // ***** this is not correct for cell-centred case***
	}
      }
    u.periodicUpdate();  // swap periodic edges
  }
  
  return 0;
}

