//
// In this example we read in an overlapping grid and then
// move one of the grids and regenerate the overlapping grid.
//
// This file was used to move a 3d grid for the two-stroke-engine example
//
// 990510: valve
//    11 1 .1 0
// 980131: tse.hdf
//    11 3 .5 0
//  960216 (sis.dat, no Cgsh)
//    options 2steps, 2=show, 0=shift, debug=3 
//       MIU : 0,   MLK : 0
// 960922
//   options 11steps 3=plot .5=deltaShift 0=debug
// 961115
//   options 21steps 3=plot .5=deltaShift 0=debug
#include "Ogen.h"
#include "Square.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "Ogshow.h"

// * MemoryManagerType memoryManager;  // This will delete A++ allocated memory at the end

int 
main() 
{
  ios::sync_with_stdio(); // Synchronize C++ and C I/O subsystems
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  aString nameOfOGFile;
  
  cout << "Enter the name of the (old) overlapping grid file:" << endl;
  cin >> nameOfOGFile;


  cout << "Read in a CompositeGrid..." << endl;
  CompositeGrid cg0;
  getFromADataBase(cg0,nameOfOGFile);

  enum moveOptions
  {
    twoStrokeEngine=0,
    valve=1
  } moveOption;
  cout << "Enter the movement option: 0=two stroke engine, 1=valve \n";
  int temp;
  cin >> temp;
  moveOption=(moveOptions)temp;

  real deltaShift;
  int plotOption=1;
  int numberOfSteps = 100;
  
  cout << "Enter the numberOfSteps, plotOption (1=plot,2=show), shift, Mapping::debug\n";
  cin >> numberOfSteps >> plotOption >> deltaShift >> Mapping::debug;

  aString nameOfShowFile = "move2.show";
  Ogshow show(nameOfShowFile );
  show.saveGeneralComment("Moving grid example");
  show.setMovingGridProblem(TRUE);
  show.setFlushFrequency(2);

  CompositeGrid cg[2];                              // use these two grids for moving

  cg0.update(); // m0.update

//  cg[0]=cg0; cg[0].update(); 
  cg[0].reference(cg0);
  cg[1]=cg0; cg[1].update(); 

  
  PlotStuff ps( plotOption & 1 );
  PlotStuffParameters psp;
    
  // Here is "CMPGRD"
  Ogen gridGenerator(ps);

  if( plotOption & 1 )
  {
    // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    psp.set(GI_TOP_LABEL,"Original grid, cg0");  // set title
    PlotIt::plot(ps,cg0,psp);
  }
  // ---- Move the grid a bunch of times.----
  
  
  const int maxNumberOfGrids=10;
  LogicalArray hasMoved(maxNumberOfGrids); // **** watch this ****
  hasMoved    = LogicalFalse;

  
  MatrixTransform *transform[2][maxNumberOfGrids];
  for( int i=0; i<maxNumberOfGrids; i++ )
  {
    transform[0][i]=NULL;
    transform[1][i]=NULL;
  }
  
  // Change the mapping on these component grids (-1 marks the end of the list)
  int gridToMove[maxNumberOfGrids] = { 0, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  if( moveOption==twoStrokeEngine )
  { // two-stroke engine
    gridToMove[0]=2;
    gridToMove[1]=3;
  }    
  else
  { // valve: move grid 2
    gridToMove[0]=2;
  }
  
  for( i=0; gridToMove[i] >= 0; i++ )
  {
    int grid=gridToMove[i];
    hasMoved(grid) = LogicalTrue;  // this grid will move

    Mapping *mapPointer = cg0[grid].mapping().mapPointer;
    // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
    // can rotate/scale and shift any Mapping
    transform[0][i]= new MatrixTransform(*mapPointer);
    transform[1][i]= new MatrixTransform(*mapPointer);

    // Change the mappings that the grids point to:
    IntegerArray mask;
    mask=cg[0][grid].mask();
    
    cg[0][grid].reference(*transform[0][i]); 
    cg[0][grid].update(MappedGrid::THEmask,MappedGrid::COMPUTEnothing);
    cg[0][grid]->computedGeometry |= MappedGrid::THEmask;
    cg[0]->computedGeometry |= CompositeGrid::THEmask;
    cg[0][grid].mask()=mask;
    
    cg[1][grid].reference(*transform[1][i]); 
    // cg[1][grid]->computedGeometry |= MappedGrid::THEmask;
    // cg[1]->computedGeometry |= CompositeGrid::THEmask;


    // cg[0][grid].update(MappedGrid::THEmask,MappedGrid::COMPUTEnothing);
    // cg[1][grid].update(MappedGrid::THEmask,MappedGrid::COMPUTEnothing);

    // cg[0][grid].update();
    // cg[1][grid].update();


  }
  // The grid is invalid now, regerate it.
  // gridGenerator.updateOverlap(cg[0], cg[oldCG], hasMoved);
  
  Range all;
  realCompositeGridFunction u[2];
  u[0].updateToMatchGrid(cg[0],all,all,all,2);
  u[1].updateToMatchGrid(cg[1],all,all,all,2);
  u[0]=1.; u[1]=2.;
  u[0].setName("u0");
  u[0].setName("u0.0",0);
  u[0].setName("u0.1",1);
  u[1].setName("u1");
  u[1].setName("u1.1",0);
  u[1].setName("u1.2",1);

  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

    
  real t=0.;
  real dt=1./max(8,numberOfSteps);
    
  char buff[80];
  aString showFileTitle[2];
  for (int step=1; step<=numberOfSteps; step++) 
  {
    int newCG = step % 2;        // new grid
    int oldCG = (step+1) % 2;    // old grid
    // Draw the overlapping grid

    if( plotOption & 1 )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",step));  // set title
      // psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      ps.erase();
      PlotIt::plot(ps,cg[oldCG],psp);
      ps.redraw(TRUE);   // force a redraw
    }
    // save results in a show file:
    if( plotOption & 2 )
    {
      cout << "save solution in the show file, t=" << t << endl;
      u[oldCG].updateToMatchGrid(cg[oldCG]);
      show.startFrame();
      sPrintF(buff,"Moving Example, t=%e",t);
      show.saveComment(0,buff);
      show.saveSolution(u[oldCG]);
    }
    
    t+=dt;
    //  Move the grid...
    // The grid is located at is base position plus:
#define POSITION(t) (deltaShift*.5*(1.-cos(twoPi*(t))))

    if( moveOption==twoStrokeEngine )
    {
      real yShift;
      if( step==1 )
	 yShift=POSITION(t)-POSITION(0.);
      else      
	 yShift=POSITION(t)-POSITION(t-2.*dt);

      cout << "shift grid by yShift = " << yShift << endl;
      // if( (step-1) % 8 > 4 )
      //  yShift*=-1.;
      for( int i=0; gridToMove[i] >= 0; i++ )
	 transform[newCG][i]->shift(0.,yShift,0.);
    }
    else
    {
      real xShift;
      if( step==1 )
	 xShift=-( POSITION(t)-POSITION(0.) );
      else      
	 xShift=-( POSITION(t)-POSITION(t-2.*dt));

      cout << "shift grid by xShift = " << xShift << endl;
      for( int i=0; gridToMove[i] >= 0; i++ )
	 transform[newCG][i]->shift(xShift,0.,0.);
    }

    //      Update the overlapping newCG, starting with and sharing data with oldCG.
    if( fabs(deltaShift)!=0. )
      gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved);

    if( Mapping::debug > 0 ) 
      ApproximateGlobalInverse::printStatistics();


  } // end for
  cout << "Done! ...\n";

  for( i=0; i<maxNumberOfGrids; i++ )
  {
    delete transform[0][i];
    delete transform[1][i];
  }

  return 0;
}

