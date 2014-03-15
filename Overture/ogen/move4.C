#include "Cgsh.h"
#include "Square.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "Ogshow.h"

void initializeMappingList();   // this allows Mappings to be made with "make"

int interpolateExposedPoints(CompositeGrid & cg1,  
			     CompositeGrid & cg2, 
			     realCompositeGridFunction & u1,
			     OGFunction *TZFlow=NULL,
			     real t=0. );

void assignRightHandSide( Oges & oges, realCompositeGridFunction & f, OGFunction & exactSolution );

//
// In this example we read in an overlapping grid made with Bill's ogen interface
// to the C++ cmpgrd. We then move one of the grids and regenerate the overlapping grid.
//
int 
main() 
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile, nameOfDirectory=".";
  
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;


  // This next call will allow the Mappings to be read in from the data-base file
  initializeMappingList();

  cout << "Create a CompositeGrid..." << endl;
  MultigridCompositeGrid m0(nameOfOGFile,nameOfDirectory);  // keep a copy of the original grid
  CompositeGrid & cg0 = m0[0];

  aString nameOfShowFile = "move2.show";
  Ogshow show( nameOfOGFile,nameOfDirectory,nameOfShowFile );
  show.saveGeneralComment("Moving grid example");
  show.setMovingGridProblem(TRUE);


  CompositeGrid cg[2];                              // use these two grids for moving

  cg0.update(); // m0.update

  // Here is "CMPGRD"
  Cgsh gridGenerator;

//  cg[0]=cg0; cg[0].update(); 
  cg[0].reference(cg0);
  cg[1]=cg0; cg[1].update(); 

  

  PlotStuff ps;
  PlotStuffParameters psp;
    
  psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
  PlotIt::plot(ps,cg0,psp);

  // ---- Move the grid a bunch of times.----
  
  
  // Change the mapping on grid 1:
  Mapping *mapPointer = cg0[1].mapping().mapPointer;

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform *transform[2];
  transform[0]= new MatrixTransform(*mapPointer);
  transform[1]= new MatrixTransform(*mapPointer);

  // Change the mappings that the grids point to:
  // Change the mapping of component grid 1:
  int grid=1;
  cg[0][grid].reference(*transform[0]); 
  cg[1][grid].reference(*transform[1]); 
  
  //m[0][0][1].mapping().mapPointer->display("Here is the mapping on grid 1");
  //MultigridCompositeGrid m3;
  //m3.reference(m[0]);
  //m3[0][1].mapping().mapPointer->display("m3 after reference: Here is the mapping on grid 1");

  // ***** testing *****
//  cout << "Enter Mapping::debug \n";
//  cin >> Mapping::debug;
  // Mapping::debug=7; 
// 1  CompositeGrid cg1;
// 1  cg1.reference(cg[0]);
// 1  cg1.update();    
// 1  mapPointer = cg1[grid].mapping().mapPointer;
// 1  ((MatrixTransform *)mapPointer)->shift(0.,0.,0.);
  // ***** testing *****
    

  // Here are some grid functions that we will use to interpolate exposed points
  realCompositeGridFunction u[2];
    
  Range all;
  u[0].updateToMatchGrid(cg[0],all,all,all,2); 
  u[1].updateToMatchGrid(cg[1],all,all,all,2); 
  u[0].setName("u");
  u[0].setName("u0",0);
  u[0].setName("u1",1);
  u[1].setName("u");
  u[1].setName("u0",0);
  u[1].setName("u1",1);
  // use this twilight-zone function so we can compute errors in interpolating exposed points
// 1  int degreeX=1;
// 1  OGPolyFunction TZFlow(degreeX,cg0.numberOfDimensions,2,1);   
// 1  TZFlow.assignGridFunction(u[0]);
// 1  TZFlow.assignGridFunction(u[1]);

//  Interpolant interpolant;

  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  LogicalArray hasMoved(2);
  hasMoved    = LogicalFalse;
  hasMoved(1) = LogicalTrue;  // Only this grid will move.

  real deltaAngle;
  cout << "Enter the rotation angle in degrees \n";
  cin >> deltaAngle;
  deltaAngle*=Pi/180.;
    
  // ---- Create a Oges solver ------
/* ---
  Oges::debug=1; 
  Oges solver(cg0);                    // create a solver
  // Assign parameters 
  if( (int) cg0[0].isAllVertexCentered() )
    solver.setEquationType( Oges::LaplaceDirichlet ); // Use one of the predefined equations 
  else
    solver.setEquationType(Oges::Interpolation);      // no predefined cell-centred equations

  realCompositeGridFunction f(cg0,all,all,all);     // hold rhs
  solver.setFillinRatio(20.);
--- */

  char buff[80];
  aString showFileTitle[2];
  const int numberOfSteps = 3;
  for (Int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    // Draw the overlapping grid

    psp.set(GI_TOP_LABEL,sprintf(buff,"Grid at step %i",i));  // set title
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    ps.erase();
    PlotIt::plot(ps,cg[oldCG],psp);
    ps.redraw(TRUE);   // force a redraw

    //  Rotate the grid...
    // After the first step we must double the angle since we start from the old grid
    real angle = i==1 ? deltaAngle : deltaAngle*2.; 
    transform[newCG]->rotate(axis3,angle);
    
    //      Update the overlapping newCG, starting with and sharing data with oldCG.
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved);

// **    gridGenerator.updateOverlap(cg[newCG]);

    // cg[newCG][1].inverseVertexDerivative().display("Here is the inverseVertexDerivative");
    // cg[newCG][1].vertexDerivative().display("Here is the vertexDerivative on the new grid");
    
    u[newCG].updateToMatchGrid(cg[newCG]);
    // Interpolate any exposed points on the old grid function
    // (pass a TwilightZone function and the routine will compute errors)
//    interpolateExposedPoints(cg[oldCG],cg[newCG],u[oldCG],&TZFlow);

    // re-evaluate the grid function on the moved grid
//    TZFlow.assignGridFunction(u[newCG]);

    // interpolate the new grid function, first update interpolant
//    interpolant.updateToMatchGrid(cg[newCG]);
//    u[newCG].interpolate();

/* ---
    if( Mapping::debug & 4 )
    {
      // solve Laplace's equation on the new grid
      cout << "Solve a problem with Oges on the new grid...\n";
      f.updateToMatchGrid(cg[newCG]);
      // This next call will cause the matrix to be recreated and refactored
      solver.updateToMatchGrid(cg[newCG]);

      assignRightHandSide( solver,f,TZFlow );  // assign f so that the true solution is known

      solver.solve( u[newCG],f );   // solve the equations

      // ...Calculate the maximum error  (for Twilight-zone flow )
      int printOptions = 1;  // bitflag: 1=print max errors, 8=print errors, 16=print solution
      solver.determineErrors(  u[newCG], TZFlow, printOptions );
    }
---- */
    // save results in a show file:
    if( TRUE || i % 2 == 0 )
    {
      show.startFrame();
      sprintf(buff,"Moving Example, step=%i",i);
      show.saveComment(0,buff);
      show.saveSolution(u[newCG]);
    }
  } // end for
  cout << "Done! ...\n";

  return 0;
}

