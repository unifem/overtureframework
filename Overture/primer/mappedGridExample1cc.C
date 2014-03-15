#include "Overture.h"  
#include "PlotStuff.h"
#include "Square.h"
#include "MappedGridFiniteVolumeOperators.h"

int 
main()
{
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  printf(" ------------------------------------------------------------------ \n");
  printf(" Demonstrate mappings, grids, gridfunctions, operators and plotting \n");
  printf("                Cell Centered Version                               \n");
  printf(" ------------------------------------------------------------------ \n");

  SquareMapping square(0.,1.,0.,1.);                   // Make a mapping, unit square
  square.setGridDimensions(axis1,11);                  // axis1==0, set no. of grid points
  square.setGridDimensions(axis2,11);                  // axis2==1, set no. of grid points

  square.setIsPeriodic(axis1,Mapping::derivativePeriodic);  // make periodic for fun
  square.setIsPeriodic(axis2,Mapping::derivativePeriodic);
  
  MappedGrid mg(square);                               // MappedGrid for a square

  mg.changeToAllCellCentered();                        // make a cell centered grid

  // add extra ghost lines, just for fun
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
    for( int side=Start; side<=End; side++ )
      mg.setNumberOfGhostPoints(side,axis,2);
  
  mg.update(MappedGrid::THEusualSuspects, MappedGrid::COMPUTEtheUsual | MappedGrid::USEdifferenceApproximation);
  // use next line for difference approximations to some geometry arrays
  // mg.update(MappedGrid::THEusualSuspects, MappedGrid::COMPUTEtheUsual | MappedGrid::USEdifferenceApproximation);

  mg.dimension().display("dimension");
  mg.indexRange().display("indexRange");
  mg.indexRange().display("extendedIndexRange");
  mg.gridIndexRange().display("gridIndexRange");
  // mg.center().display("center");
  
  
  Range all;
  realMappedGridFunction u(mg,all,all,all,2);           // create a grid function with 2 components
  u.setName("Velocity Stuff");                          // give names to grid function ...
  u.setName("u Stuff",0);                               // ...and components
  u.setName("v Stuff",1);
  Index I1,I2,I3;                                            

  // mg.dimension()(2,3) : all points on the grid, including ghost-points
  getIndex(mg.dimension(),I1,I2,I3);                        // assign I1,I2,I3 from dimension
  u(I1,I2,I3,0)=sin(Pi*mg.center()(I1,I2,I3,axis1))         // component 0 : sin(pi*x)*cos(pi*y)
               *cos(Pi*mg.center()(I1,I2,I3,axis2));        
  u(I1,I2,I3,1)=cos(Pi*mg.center()(I1,I2,I3,axis1))         // component 1 : cos(pi*x)*sin(pi*y)
               *sin(Pi*mg.center()(I1,I2,I3,axis2));
    
  MappedGridFiniteVolumeOperators op(mg);                             // operators 
  u.setOperators(op);                                     // associate with a grid function
  u.x().display("here is u.x");                           // x derivative

  getIndex(mg.indexRange(),I1,I2,I3);                   // interior and boundary points
  // compute the error in component 0 of u.x, the notation u.x(I1,I2,I3,0) means only evaluate
  // the derivative for component 0 and at the points (I1,I2,I3) (done for efficiency only)
  real error = max(abs( u.x(I1,I2,I3,0)(I1,I2,I3,0)-
            Pi*cos(Pi*mg.center()(I1,I2,I3,axis1))*cos(Pi*mg.center()(I1,I2,I3,axis2)) ));
  cout << "Maximum error in component 0 of u.x = " << error << endl;

  PlotStuff ps(TRUE,"mappedGridExample1cc");      // create a PlotStuff object
  PlotStuffParameters psp;                      // This object is used to change plotting parameters
    
  aString answer;
  aString menu[] = { "contour",                  // Make some menu items
		    "stream lines",
		    "grid",
		    "read command file",
		    "save command file",
		    "erase",
		    "exit",
                    "" };                       // empty string denotes the end of the menu
  for(;;)
  {
    ps.getMenuItem(menu,answer);                // put up a menu and wait for a response
    if( answer=="contour" )
    {
      psp.set(GI_TOP_LABEL,"My Contour Plot");  // set title
      PlotIt::contour(ps,u,psp);                        // contour/surface plots
    }
    else if( answer=="grid" )
      PlotIt::plot(ps,mg,psp);                          // plot the composite grid
    else if( answer=="stream lines" )
      PlotIt::streamLines(ps,u);                        // streamlines
    else if( answer=="read command file" )
      ps.readCommandFile(); 
    else if( answer=="save command file" )
      ps.saveCommandFile(); 
    else if( answer=="erase" )
      ps.erase();
    else if( answer=="exit" )
      break;
  }
  return 0;
}
