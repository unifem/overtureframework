//===========================================================================================
//  
// This example shows how to determine the time step for a 2D convection diffusion equation
// 
//  Bill Henshaw
//===========================================================================================
#include "Overture.h"  
#include "PlotStuff.h"
#include "SquareMapping.h"
#include "AnnulusMapping.h"
#include "MappedGridOperators.h"
#include "NameList.h"

#define UTRUE(x,y,t) (x)*(1.-(x))*(y)*(1.-(y))*(1.+(t))
#define UTRUEX(x,y,t) (1.-2.*(x))*(y)*(1.-(y))*(1.+(t))
#define UTRUEY(x,y,t) (x)*(1.-(x))*(1.-2.*(y))*(1.+(t))
#define UTRUET(x,y,t) (x)*(1.-(x))*(y)*(1.-(y))
#define UTRUEXX(x,y,t) -2.*(y)*(1.-(y))*(1.+(t))
#define UTRUEYY(x,y,t) -2.*(x)*(1.-(x))*(1.+(t))

#define FORCE(x,y,t) UTRUET(x,y,t)+a*UTRUEX(x,y,t)+b*UTRUEY(x,y,t) \
                     -nu*(UTRUEXX(x,y,t)+UTRUEYY(x,y,t))

real
getDt(const real & cfl, 
      const real & a, 
      const real & b, 
      const real & nu, 
      MappedGrid & mg, 
      MappedGridOperators & op,
      const real alpha0 = -2.,
      const real beta0  = 1. );

int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  printf(" ------------------------------------------------------------------ \n");
  printf(" Solve a convection-diffusion equation on a square or annulus       \n");
  printf(" Determine the correct time step using the getDt function (getDt.C) \n");
  printf(" ------------------------------------------------------------------ \n");

  // Set default values for parameters. These can be optionally changed below
  int numberOfTimeSteps=100;
  real dt=.005, cfl=.5;
  IntegerArray bc(2,3); bc=1;
  IntegerArray gridPoints(3); gridPoints=-1;
  int mapType=0;   // 0=square, 1=annulus
  // The NameList object allows one to read in values by name
  NameList nl;
  aString name(80),answer(80);
  printf(
   " Parameters for Example 3: \n"
   " ------------------------- \n"
   "   name                                                 type    default  \n"
   "numberOfTimeSteps  (nts=)                              (int)      %i     \n"
   "mapType (mt= 0:square, 1=annulus)                      (int)      %i     \n"
   "cfl                                                    (real)     %f     \n"
   "time step (dt=)                                        (real)     %f     \n"
   "gridPoints(axis) (gp(axis)=no. of grid points)         (intArray)        \n"
   "boundary conditions (bc(side,axis)=)                   (intArray)        \n",
      numberOfTimeSteps,mapType,cfl,dt);

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
    else if( name== "cfl" )   
      cfl=nl.realValue(answer);  
    else if( name== "mapType" || name=="mt" )   
      mapType=nl.intValue(answer);  
    else if( name== "bc" )   
      nl.getIntArray( answer,bc );
    else if( name== "gridPoints" || name=="gp")   
      nl.getIntArray( answer,gridPoints );
    else
      cout << "unknown response: [" << name << "]" << endl;

  }

  Mapping *mapping;                                  // keep a pointer to a mapping
  if( mapType==0 )
  {
    mapping = new SquareMapping();                     // create a Square
    mapping->setGridDimensions(axis1,11);              // axis1==0, set no. of grid points
    mapping->setGridDimensions(axis2,11);              // axis2==1, set no. of grid points
  }
  else
  {
    mapping = new AnnulusMapping();                    // create an Annulus
    mapping->setGridDimensions(axis1,41);              // axis1==0, set no. of grid points
    mapping->setGridDimensions(axis2,13);              // axis2==1, set no. of grid points
  }
  for( int axis=0; axis<mapping->getDomainDimension(); axis++ )
  {
    if( gridPoints(axis)>0 )
      mapping->setGridDimensions(axis,gridPoints(axis));
  } 
  MappedGrid mg(*mapping);                           // MappedGrid for a square
  mg.update();                                       // create default variables

  Range all;
  realMappedGridFunction u(mg);
  u.setName("Solution");                          // give names to grid function ...
  u.setName("u",0);                               // ...and components

  Index I1,I2,I3, Ib1,Ib2,Ib3;                                            
  // mg.dimension()(2,3) : all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);               // assign I1,I2,I3 from dimension
  realArray & x= mg.vertex();
  u(I1,I2,I3)=UTRUE(x(I1,I2,I3,0),x(I1,I2,I3,1),0.);       // initial conditions
    
  MappedGridOperators op(mg);                    // operators 
  u.setOperators(op);                            // associate with a grid function

  PlotStuff ps(TRUE,"mappedGridExample6");      // create a PlotStuff object
  PlotStuffParameters psp;         // This object is used to change plotting parameters
  char buffer[80];

  real t=0, a=1., b=1., nu=.1;

  // getDt needs the inverseVertexDerivative 
  mg.update(MappedGrid::THEinverseVertexDerivative);

  dt = getDt( cfl,a,b,nu,mg,op );
  cout << " dt from getDt = " << dt << endl;

  int tStep=numberOfTimeSteps/10;

  for( int step=0; step<numberOfTimeSteps; step++ )
  {
    if( step % tStep == 0 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buffer,"Solution at time t=%e",t));  // set title
      PlotIt::contour(ps, u,psp );
    }

    getIndex(mg.dimension(),I1,I2,I3);
    u+=dt*( (-a)*u.x()+(-b)*u.y()+nu*(u.xx()+u.yy()) +FORCE(x(I1,I2,I3,0),x(I1,I2,I3,1),t) );
    t+=dt;
    // apply Boundary conditions
    // apply Boundary conditions
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( int side=Start; side<=End; side++ )
      { // only assign BC's on sides with a positive boundary condition:
	if( mg.boundaryCondition()(side,axis) > 0 )
	{ // fill in boundary values
	  getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	  u(Ib1,Ib2,Ib3)=UTRUE(x(Ib1,Ib2,Ib3,0),x(Ib1,Ib2,Ib3,1),t);
	}
      }
    u.periodicUpdate();  // swap periodic edges

    getIndex(mg.gridIndexRange(),I1,I2,I3);
    real error = max(abs( u(I1,I2,I3)-UTRUE(x(I1,I2,I3,0),x(I1,I2,I3,1),t) ));  
    cout << "t=" << t << ", error =" << error << endl;    
  }
  
  Overture::finish();          
  return 0;
}

