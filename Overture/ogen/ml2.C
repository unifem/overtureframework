#include "Cgsh.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "Ogshow.h"

//
// Check for memoery leaks
//
int 
main() 
{
  // Mapping::debug=7; 
  ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile;
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;

  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg[2];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file
//cg[0].update();
  cg[1]=cg[0];                                      // copy cg[0] into cg[1]

  // Rotate component grid 1 (do this by changing the mapping)
  int gridToMove=1;
  Mapping & mappingToMove = *(cg[0][gridToMove].mapping().mapPointer);

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping, keep a transform for each composite grid

  MatrixTransform transform0(mappingToMove); 

//  cout << "mappingToMove.uncountedReferencesMayExist()=" << mappingToMove.uncountedReferencesMayExist() << endl;
//  MatrixTransform & transform0= *(new MatrixTransform(mappingToMove));
  transform0.setName(Mapping::mappingName,"transform0");
//  transform0.incrementReferenceCount(); 
//  cout << "transform0: ref count = " << transform0.getReferenceCount() << endl;


  MatrixTransform transform1(mappingToMove); 
//  MatrixTransform & transform1= *(new MatrixTransform(mappingToMove));
  transform1.setName(Mapping::mappingName,"transform1");
//  transform1.incrementReferenceCount(); 

  // Replace the mapping of the component grid that we want to move:
  cg[0].update();
  cg[1].update();
  cg[0][gridToMove].reference(transform0); 
  cg[0].updateReferences();
  cg[1][gridToMove].reference(transform1); 
  cg[1].updateReferences();
  
  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do but it will save space
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  // we tell the grid generator which grids have changed
  LogicalArray hasMoved(2);
  hasMoved    = LogicalFalse;
  hasMoved(gridToMove) = LogicalTrue;  // Only this grid will move.
  char buff[80];

  PlotStuff ps(FALSE,"Moving Grid Example");         // for plotting
  PlotStuffParameters psp;
  // Here is the overlapping grid generator
  Cgsh gridGenerator(ps);

  enum
  {
    rotate,
    shift
  } moveOption=shift;
  

  // Here is an interpolant
  Interpolant interpolant(cg[0]);
  realCompositeGridFunction u(cg[0]);
  // Here is a show file
  Ogshow show("move1.show");
  show.setMovingGridProblem(TRUE);

  int numberOfSteps=100;
  real deltaAngle=5.*Pi/180.;
  real xShift=-.01;

  // ---- Move the grid a bunch of times.----
  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    
/* ----
    ps.erase();
    psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step=%i",i));  // set title
    PlotIt::plot(ps,cg[oldCG],psp);      // plot the current overlapping grid
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);     // set this to run in "movie" mode (after first plot)
    ps.redraw(TRUE);
---- */

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
    interpolant.updateToMatchGrid(cg[newCG]);
    u.updateToMatchGrid(cg[newCG]);
    // assign values to u
    Index I1,I2,I3;
    for( int grid=0; grid<cg[newCG].numberOfComponentGrids; grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      getIndex(mg.dimension(),I1,I2,I3);
      real freq = 2.*i/numberOfSteps;
      u[grid](I1,I2,I3)=cos(freq*mg.vertex()(I1,I2,I3,axis1))*sin(freq*mg.vertex()(I1,I2,I3,axis2));  
    }

    u.interpolate();
    // save the result in a show file, every fourth step
    if( TRUE || (i % 4) == 1 )
    {
      show.startFrame();
      show.saveComment(0,sPrintF(buff,"Solution form move1 ar step = %i",i));
      show.saveSolution(u);
    }
    if( i % 5 == 0 )
      printf("**** number of A++ arrays = %i \n",Array_Descriptor_Type::getMaxNumberOfArrays());
  } 
  cout << "Results saved in move1.show, use Overture/bin/plotStuff to view this file" << endl;

//  transform0.decrementReferenceCount(); 
//  cout << "transform0 ref count = " << transform0.getReferenceCount() << endl;
//  transform1.decrementReferenceCount(); 
//  cout << "transform1 ref count = " << transform1.getReferenceCount() << endl;
  
  return 0;

}
