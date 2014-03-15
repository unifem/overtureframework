#include "Ogen.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "Ogshow.h"

//
// Moving Grid Example: 
//   o read in a grid from a data-base file, rotate a component grid and recompute the overlapping grid.
//
int 
main(int argc, char *argv[]) 
{
  // Mapping::debug=7; 
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  int plotOption=TRUE;
  if( argc > 1 )
  { // look at arguments for "noplot"
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="noplot" )
        plotOption=FALSE;
    }
  }

  aString nameOfOGFile;
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg[2];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file
//cg[0].update();
  cg[1]=cg[0];                                      // copy cg[0] into cg[1]

  enum
  {
    rotate,
    shift
  } moveOption=rotate;
  
  int numberOfSteps=20;
  real deltaAngle=5.*Pi/180.;
  deltaAngle=.5*Pi/180.;0.; // for testing optimized moving grid algorithm

  real xShift=-.01;

  // Rotate component grid 1 (do this by changing the mapping)
  int gridToMove=1;
  Mapping & mappingToMove = *(cg[0][gridToMove].mapping().mapPointer);

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping, keep a transform for each composite grid
  MatrixTransform transform0(mappingToMove); 
  MatrixTransform transform1(mappingToMove); 

/* ----
  if( moveOption==rotate )
  { // specify initial rotation
    real angle = 3.*deltaAngle;
    transform0.rotate(axis3,angle);
    transform1.rotate(axis3,angle);
  }
---- */

  // Replace the mapping of the component grid that we want to move:
  cg[0][gridToMove].reference(transform0); 
  cg[0].update();
  cg[1][gridToMove].reference(transform1); 
  cg[1].update();
  
  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do but it will save space
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  // we tell the grid generator which grids have changed
  LogicalArray hasMoved(2);
  hasMoved    = LogicalFalse;
  hasMoved(gridToMove) = LogicalTrue;  // Only this grid will move.
  char buff[80];

  PlotStuff ps(plotOption,"Moving Grid Example");         // for plotting
  PlotStuffParameters psp;
  // Here is the overlapping grid generator
  Ogen gridGenerator(ps);


  // ---- Move the grid a bunch of times.----
  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    
    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step=%i",i));  // set title
    PlotIt::plot(ps,cg[oldCG],psp);      // plot the current overlapping grid

    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)

    ps.redraw(TRUE);

    // Rotate the grid by rotating the mapping
    // After the first step we must double the angle since we start from the old grid
    if( moveOption==rotate )
    {
      real angle = i==1 ? deltaAngle : deltaAngle*2.; 
      if( newCG==0 )
	transform0.rotate(axis3,angle);
      else
	transform1.rotate(axis3,angle);
    }
    else
    {
      real delta = i==1 ? xShift : xShift*2.; 
      if( newCG==0 )
	transform0.shift(delta,0.,0.);
      else
	transform1.shift(delta,0.,0.);
    }
    // Update the overlapping newCG, starting with and sharing data with oldCG.
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved);

  } 
  return 0;
}

