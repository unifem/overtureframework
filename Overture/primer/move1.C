#include "Ogen.h"
#include "PlotStuff.h"
#include "MatrixTransform.h"
#include "Ogshow.h"
#include "ParallelUtility.h"

// ========================================================================================================
// Moving Grid Example: 
//   o read in a grid from a data-base file, rotate a component grid and recompute the overlapping grid.
//   o interpolate a grid function, update the interpolate for the new grid
//   o save solutions in a show file
// 
// Usage:
//   move1 [-grid=<name>] [-numSteps=<>] [-shift] [-rotate] [-interpolate=[0|1]] [-saveShow=[0|1]]
// 
// Examples:
//   move1 -grid=cic
//   move1 -grid=sib
//   move1 -grid=ellipsoid1
// 
//  mpirun -np 1 move1 -grid=cic -numSteps=5 
//  mpirun -np 1 move1 -grid=cic -numSteps=5 -interpolate=1
//  srun -N1 -n2 -ppdebug move1 -grid=cic -numSteps=5
//  srun -N1 -n4 -ppdebug move1 -grid=cice -numSteps=5 -interpolate=1 -saveShow=0
//  totalview srun -a -N1 -n1 -ppdebug move1 -grid=cic
// ========================================================================================================
int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture

  Mapping::debug=0; 
  int debug=0;
  
  int numGhost=2;  // for second-order accurate (1 is good enough for implicit)
  MappedGrid::setMinimumNumberOfDistributedGhostLines(numGhost);

  printF(" moving grid demo: \n"
         " move1 [-grid=<name>] [-numSteps=<>] [-shift] [-rotate] [-interpolate=[0|1]] [-saveShow=[0|1]]\n");

  enum
  {
    rotate,
    shift
  } moveOption=shift;

  int numberOfSteps=20;
  real deltaAngle=5.*Pi/180.;
  real deltaShift=-.01;
  int useFullAlgorithmInterval=10; // 10000;
  #ifdef USE_PPP
    useFullAlgorithmInterval=1;  // for now always use full algorithm for ogen
  #endif

  // aString nameOfOGFile = "cice2.order2.hdf";
  aString nameOfOGFile = "cic.hdf";

  int plotOption=true;
  int interpolate=false;
  int saveShow=true;
  if( argc > 1 )
  { // look at arguments for "noplot"
    aString line;
    int len=0;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line=="-noplot" || line=="noplot" )
        plotOption=false;
      else if( len=line.matches("-grid=") )
      {
	nameOfOGFile=line(len,line.length()-1);
      }
      else if( line=="-shift" )
      {
	moveOption=shift;
      }
      else if( line=="-rotate" )
      {
	moveOption=rotate;
      }
      else if( len=line.matches("-numSteps=") )
      {
	sScanF(line(len,line.length()-1),"%i",&numberOfSteps);
      }
      else if( len=line.matches("-saveShow=") )
      {
	sScanF(line(len,line.length()-1),"%i",&saveShow);
      }
      else if( len=line.matches("-interpolate=") )
      {
	sScanF(line(len,line.length()-1),"%i",&interpolate);
      }
      else
      {
	printF("Unknown option=[%s]\n",(const char*)line);
      }
      
      
    }
  }


  PlotStuff ps(plotOption,"Moving Grid Example");         // for plotting
  PlotStuffParameters psp;

  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg[2];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file
  cg[1]=cg[0];                                      // copy cg[0] into cg[1]

  if( cg[0].numberOfDimensions()==2 )
    psp.set(GI_PLOT_INTERPOLATION_POINTS,true);

  if( debug & 2  )
  {
    psp.set(GI_TOP_LABEL,"initial grid");  // set title
    PlotIt::plot(ps,cg[0],psp);
  }
  

  // Move some component grids (do this by changing the mapping)
  int gridsToMove[5]={1,2,3,4,5};  // move at most 5 grids
  MatrixTransform *transform0[5]={NULL,NULL,NULL,NULL,NULL}; //
  MatrixTransform *transform1[5]={NULL,NULL,NULL,NULL,NULL};
  
  // By default we move all the grids but grid=0 
  int numberOfGridsToMove=cg[0].numberOfComponentGrids()-1;  // number of grids to move
  assert( numberOfGridsToMove<=5 );
  
  for( int g=0; g<numberOfGridsToMove; g++ )
  {
    int grid=gridsToMove[g];
    // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
    // can rotate/scale and shift any Mapping, keep a transform for each composite grid

    Mapping & mappingToMove = *(cg[0][grid].mapping().mapPointer);
    transform0[g] = new MatrixTransform(mappingToMove);
    transform1[g] = new MatrixTransform(mappingToMove);
    cg[0][grid].reference(*transform0[g]); 
    cg[1][grid].reference(*transform1[g]); 
  }
  cg[0].updateReferences();
  cg[1].updateReferences();
  

//   // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
//   // can rotate/scale and shift any Mapping, keep a transform for each composite grid
//   MatrixTransform transform0(mappingToMove); 
//   MatrixTransform transform1(mappingToMove); 

//   // Replace the mapping of the component grid that we want to move:
//   cg[0][gridToMove].reference(transform0); 
//   cg[0].updateReferences();
//   cg[1][gridToMove].reference(transform1); 
//   cg[1].updateReferences();
  
  if( debug & 2 )
  {
    ps.erase();
    psp.set(GI_TOP_LABEL,"cg[0] after reference to transform");  // set title
    PlotIt::plot(ps,cg[0],psp);
  }


  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do but it will save space
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  // we tell the grid generator which grids have changed
  LogicalArray hasMoved(cg[0].numberOfComponentGrids());
  hasMoved    = true;
  hasMoved(0) = false;

  char buff[80];

  // Here is the overlapping grid generator
  Ogen gridGenerator(ps);

  // update the initial grid, since the above reference destroys the mask
  gridGenerator.updateOverlap(cg[0]);

  if( debug & 2 )
  {
    ps.erase();
    psp.set(GI_TOP_LABEL,"cg[0] gridGenerator.updateOverlap");  // set title
    PlotIt::plot(ps,cg[0],psp);
  }
  

  // Here is an interpolant
  // Interpolant interpolant(cg[0]);
  Interpolant & interpolant = *new Interpolant(cg[0]);               // do this instead for now. 
  realCompositeGridFunction u(cg[0]);
  // Here is a show file
  Ogshow show("move1.show");
  show.setIsMovingGridProblem(true);


  // ---- Move the grid a bunch of times.----
  real angle=0.; // total angle rotated so far
  real xShift=0;  // cummulative shift
  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    
    printF("--- take step %i\n",i);

    if( plotOption && i>0 )
    {
      ps.erase();
      psp.set(GI_TOP_LABEL,sPrintF(buff,"Solution at step=%i",i));  // set title
      PlotIt::plot(ps,cg[oldCG],psp);      // plot the current overlapping grid
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);     // set this to run in "movie" mode (after first plot)
      ps.redraw(true);
    }
    
    // Move the grids by changing the Mapping (rotate/shift)
    for( int g=0; g<numberOfGridsToMove; g++ )
    {
      MatrixTransform & transform = newCG==0 ? *transform0[g] : *transform1[g];
      if( moveOption==rotate )
      {
	angle += deltaAngle;
	transform.reset();  // reset transform since otherwise rotate is incremental
	transform.rotate(axis3,angle);
      }
      else
      {
	xShift += deltaShift;
	// printF(" xShift=%9.3e\n",xShift);
	transform.reset();  // reset transform since otherwise shift is incremental
	transform.shift(xShift,0.,0.);
      }
    }
    
    // Update the overlapping newCG, starting with and sharing data with oldCG.
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    if( i% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      printF("\n +++++++++++ use full algorithm in updateOverlap +++++++++++++++ \n");
      option=Ogen::useFullAlgorithm;
    }
    gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option );

    if( interpolate )
    {
      interpolant.updateToMatchGrid(cg[newCG]);
      u.updateToMatchGrid(cg[newCG]);
    }
    
    // assign values to u
    Index I1,I2,I3;
    for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[newCG][grid];
      getIndex(mg.dimension(),I1,I2,I3);
      realSerialArray vertexLocal; getLocalArrayWithGhostBoundaries(mg.vertex(),vertexLocal);
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[grid],uLocal);
      bool ok = ParallelUtility::getLocalArrayBounds(u[grid],uLocal,I1,I2,I3,1);
      if( ok ) // this processor has grid points
      {
	real freq = 2.*i/numberOfSteps;
	uLocal(I1,I2,I3)=cos(freq*vertexLocal(I1,I2,I3,axis1))*sin(freq*vertexLocal(I1,I2,I3,axis2));  
      }
      
    }
    if( interpolate )
      u.interpolate();
    // save the result in a show file, every fourth step
    if( saveShow && (i % 4) == 1 )
    {
      show.startFrame();
      show.saveComment(0,sPrintF(buff,"Solution form move1 ar step = %i",i));
      show.saveSolution(u);
    }
  } 
  if( saveShow )
    printF("Results saved in move1.show, use Overture/bin/plotStuff to view this file\n");
  show.close();  // in parallel we need to explicitly close the show here while MPI is still valid.

  if( plotOption )
  {
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    psp.set(GI_TOP_LABEL,"final grid");  // set title
    PlotIt::plot(ps,cg[(numberOfSteps%2)],psp);
  }

  // clean up : 
  for( int g=0; g<numberOfGridsToMove; g++ )
  {
    delete transform0[g];
    delete transform1[g];
  }


  Overture::finish();          
  return 0;
}

