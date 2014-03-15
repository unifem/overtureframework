// ====================================================================================
//      Test moving grids II
//
// Examples:
// 
//  srun -N1 -n1 -ppdebug move2 -grid=sise2.order2 -rotate=5  
//  srun -N1 -n1 -ppdebug move2 -grid=sise2.order2 -rotate=5 -numSteps=5 -interpolate=1
//  srun -N1 -n1 -ppdebug move2 -grid=sise2.order2 -rotate=5 -numSteps=5 -solvePoisson=1
//  mpirun -np 1 move2 -grid=sise2.order2 -rotate=5 -numSteps=5 -solvePoisson=1
//  mpirun -np 4 move2 -grid=sise2.order2 -rotate=5 -numSteps=5 -solvePoisson=1     [ok]
//  mpirun -np 1 move2 -grid=joukowsky2de1.order2.ml3 -rotate=5 -numSteps=5 -solvePoisson=1  [ok]
//  mpirun -np 2 move2 -grid=joukowsky2de1.order2.ml3 -rotate=5 -numSteps=5 -solvePoisson=1  -solver=mg [ok
//  mpirun -np 2 move2 -grid=joukowsky2de2.order2.ml3 -rotate=5 -numSteps=5 -solvePoisson=1  -solver=mg [ok
// 
// Check for leak:
//   mpirun -np 1 move2 -noplot -grid=sise2.order2 -rotate=5 -numSteps=5 -interpolate=0
//   move2 -noplot -grid=sise2.order2 -rotate=5 -numSteps=5 -interpolate=0 [ serial ok
// 
// Exposed points:
//  mpirun -np 2 move2 -grid=sise2.order2 -rotate=5 -numSteps=3 -interpExposed=1 -debug=1
//  mpirun -np 2 move2 -grid=cice2.order2 -shift=.025 -numSteps=3 -interpExposed=1 -debug=1
//  srun -N1 -n4 -ppdebug move2 -grid=stir -rotate=5 -numSteps=5 -interpExposed=1 -saveShow=0 -debug=1
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=sibe2.order2 -shift=.02 -numSteps=3 -interpExposed=1 -saveShow=0 -debug=1
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=ellipsoid -shift=.02 -numSteps=3 -interpExposed=1 -saveShow=0 -debug=1
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=cice2.order2 -shift=.025 -numSteps=3 -interpExposed=1 -saveShow=0 -debug=1
// 
//  srun -N1 -n2 -ppdebug move2 -noplot -grid=sise2.order2 -rotate=5 -numSteps=3 -interpExposed=1  -saveShow=0 -debug=1
// 
//  totalview srun -a -N1 -n1 -ppdebug move2  -grid=sise2.order2 -rotate
//  totalview srun -a -N1 -n1 -ppdebug move2  -grid=sise2.order2 -rotate
//
// -- solver in parallel
//  srun -N1 -n1 -ppdebug move2 -noplot -grid=sise2.order2 -rotate=5 -numSteps=2 -solvePoisson=1 -saveShow=0 -debug=3
//  srun -N1 -n1 -ppdebug move2 -noplot -grid=square5 -shift=.1 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=3
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=sise1.order2 -shift=.1 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=3 [OK]
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=cice2.order2 -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=1 [OK]
//  srun -N1 -n4 -ppdebug move2 -noplot -grid=sibe2.order2 -shift=.02 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=1
//
//  srun -N1 -n2 -ppdebug move2 -noplot -grid=cice1.order2 -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=1
// 
// -- Fourth-order grids
//  mpirun -np 2 move2 -grid=cice2.order4 -shift=.025 -numSteps=3 -debug=1 -numParallelGhost=4
// -- multigrid
//  mpirun -np 2 move2 -noplot -grid=cice1.order2.ml2 -solver=mg -shift=.1 -numSteps=2 -solvePoisson=1 -interpExposed=1  -saveShow=0 [OK]
//  mpirun -np 2 move2 -noplot -grid=cice2.order2.ml2 -solver=mg -shift=.1 -numSteps=3 -solvePoisson=1 -interpExposed=1  -saveShow=0 -debug=1 [OK 
//  srun -N1 -n2 -ppdebug move2 -grid=cice1.order2 -solver=mg -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 -debug=1
// 
//  mpirun -np 4 move2 -grid=square16 -solver=mg -shift=.1 -numSteps=2 -solvePoisson=1 -interpExposed=1 -saveShow=0 [ok
//  mpirun -np 2 move2 -grid=sise1.order2.ml2 -solver=mg -shift=.1 -numSteps=2 -solvePoisson=1 -interpExposed=1 -saveShow=0  [ok
//  mpirun -np 4 move2 -grid=sise1.order2.ml2 -solver=mg -rotate=5 -numSteps=2 -solvePoisson=1 -interpExposed=1 -saveShow=0  [ok
//
//  mpirun -np 1 move2 -grid=sise2.order2.ml3 -solver=mg -rotate=5 -numSteps=50 -solvePoisson=1 -interpExposed=1 -saveShow=0 -odebug=3 
// 
// ***** 100414:
//   srun -N1 -n2 -ppdebug move2 -noplot -grid=sise1.order2.ml2 -solver=mg -rotate=5 -numSteps=5 -solvePoisson=1 -saveShow=0 -odebug=1 [OK
//   srun -N1 -n2 -ppdebug move2 -noplot -grid=sise1.order2.ml3 -solver=mg -rotate=5 -numSteps=1 -solvePoisson=1 -saveShow=0 -odebug=3 >! junk [ OK 
//   srun -N1 -n2 -ppdebug move2 -noplot -grid=cice1.order2.ml2 -solver=mg -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 [OK
//   srun -N1 -n2 -ppdebug move2 -noplot -grid=cice2.order2.ml3 -solver=mg -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 [OK
//   mpirun -np 2 move2 -noplot -grid=cice2.order2.ml3 -solver=mg -shift=.025 -numSteps=3 -solvePoisson=1 -saveShow=0 [OK]
// *************** track down ogmg bug:
// mpirun -np 2 move2 -noplot -grid=sise2.order2.ml3 -solver=mg -rotate=10 -numSteps=2 -solvePoisson=1 -interpExposed=1 -saveShow=0 -odebug=7 -reuseSolver=0 > ! junk1
// mpirun -np 2 move2 -noplot -grid=sise2.order2.ml3 -solver=mg -rotate=10 -numSteps=2 -solvePoisson=1 -interpExposed=1 -saveShow=0 -odebug=7 -reuseSolver=1 > ! junk2

// ===================================================================================

#include "Ogen.h"
#include "SquareMapping.h"
#include "PlotStuff.h"
#include "mogl.h"
#include "MatrixTransform.h"
#include "OGPolyFunction.h"
#include "Oges.h"
#include "Ogshow.h"
#include "HDF_DataBase.h"
#include "interpPoints.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ExposedPoints.h"
#include "Ogmg.h"
#include "App.h"

#define ForBoundary(side,axis)   for( axis=0; axis<mg.numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )


// NOTE: leak of array ID's was found in using getLocalArrayWithGhostBoundaries(u,uLocal); (which uses an adopt)
//       + uLocal.reshape() (in Mapping::mapGrid()) --> fix made to Mapping.C : uLocal2.reference(uLocal); uLocal2.reshape(..) ..
int 
leakCheck( CompositeGrid & cg )
{
  checkArrayIDs("leakCheck:start");
  for( int ii=0; ii<50; ii++ )
  {
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & g = cg[grid];

      g.setShareGridWithMapping(false);  // still leaks
      
      g.update(MappedGrid::THEmask);  // *wdh* 081026 -- needed for parallel for some reason --

      // this next section has an array leak in parallel
      // We invalidate the geometry arrays if the grid has moved (or the base grid for an AMR grid has moved)
      g.geometryHasChanged(~MappedGrid::THEmask);   // **** this invalidates all geometry except the mask ***
      
      if( false )
      {
	// The grid generator needs the following arrays
	g.update( MappedGrid::THEcenter               |
		  MappedGrid::THEvertex               |   // we need this even for cell centred grids
		  // MappedGrid::THEvertexDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		  // MappedGrid::THEcenterDerivative     |   // This was needed to prevent an error in 3D with ghost=2
		  MappedGrid::THEvertexBoundaryNormal |
		  MappedGrid::THEboundingBox      );  
	  
      }
      else
      {
	// The grid generator needs the following arrays
	g.update( MappedGrid::THEvertex );  // leak
	// g.update( MappedGrid::THEcenter );  // leak
	// g.update( MappedGrid::THEinverseVertexDerivative );  // leak
	// g.update( MappedGrid::THEvertexBoundaryNormal |  // OK
        // 		  MappedGrid::THEboundingBox     );
	
      }
    }
    checkArrayIDs(sPrintF("leakCheck: ii=%i",ii));

  }
  
  checkArrayIDs("leakCheck:end");
  printF("*** Done leak check\n");

  return 0;
  
}


int 
main(int argc, char *argv[])
{
  Overture::start(argc,argv);  // initialize Overture
  // real memoryUsageScaleFactor=1.1;
  real memoryUsageScaleFactor=1.0;
  Overture::turnOnMemoryChecking(true,memoryUsageScaleFactor);

  // Mapping::debug=7;
  int debug=0;
  
  printF(" moving grid demo II: \n"
         " move2 -grid=<name> -numSteps=<> -shift=<> -rotate=<degrees> -debug=<> -odebug=<Oges::debug> -interpolate=[0|1] -saveShow=[0|1] -solvePoisson=[0|1] -interpExposed=[0|1] -solver=[mg|petsc] -width=[] -numParallelGhost=<>\n");

  enum
  {
    rotate,
    shift
  } moveOption=shift;

  int numberOfSteps=20;
  real deltaAngle=5.*Pi/180.;
  real deltaShift=-.01;
  int width=-1;
  int useFullAlgorithmInterval=10; // 10000;
  #ifdef USE_PPP
    useFullAlgorithmInterval=1;  // for now always use full algorithm for ogen
  #endif

  // aString nameOfOGFile = "cice2.order2.hdf";
  aString nameOfOGFile = "cic.hdf";

  int plotOption=true;
  int interpolate=false;
  int saveShow=false;
  int solvePoisson=false;
  int interpExposed=false;
  int reuseSolver=true;
  
  int solverType=OgesParameters::yale;
  // solverType=OgesParameters::PETSc;
  #ifdef USE_PPP
    solverType=OgesParameters::PETScNew;
  #endif

  int numParallelGhost=2;  // for second-order accurate (1 is good enough for implicit)
  // int numParallelGhost=4;  // Fourth order : parallel ghost = (IW + DW -2 )/2 ??

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
      else if( len=line.matches("-shift=") )
      {
        sScanF(line(len,line.length()-1),"%e",&deltaShift);
	moveOption=shift;
        printF(" Shift: deltaShift=%8.2e\n",deltaShift);
      }
      else if( line=="-solver=mg" )
      {
        solverType=OgesParameters::multigrid;
        printF(" Setting solverType=multigrid\n");
      }
      else if( line=="-solver=petsc" )
      {
        #ifndef USE_PPP
          solverType=OgesParameters::PETSc;
        #else
          solverType=OgesParameters::PETScNew;
        #endif
        printF(" Setting solverType=PETSc\n");
      }
      else if( len=line.matches("-rotate=") )
      {
        sScanF(line(len,line.length()-1),"%e",&deltaAngle);
	moveOption=rotate;
        printF(" Rotate: deltaAngle=%8.2e (degrees)\n",deltaAngle);
        deltaAngle*=Pi/180.; // convert degrees to radians
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
      else if( len=line.matches("-solvePoisson=") )
      {
	sScanF(line(len,line.length()-1),"%i",&solvePoisson);
      }
      else if( len=line.matches("-reuseSolver=") )
      {
	sScanF(line(len,line.length()-1),"%i",&reuseSolver);
	printF("reuseSolver=%i\n",reuseSolver);
      }
      else if( len=line.matches("-interpExposed=") )
      {
	sScanF(line(len,line.length()-1),"%i",&interpExposed);
      }
      else if( len=line.matches("-debug=") )
      {
	sScanF(line(len,line.length()-1),"%i",&debug);
	printF(" Setting debug=%i \n",debug);
      }
      else if( len=line.matches("-width=") )
      {
	sScanF(line(len,line.length()-1),"%i",&width);
	printF(" Setting width=%i \n",width);
      }
      else if( len=line.matches("-useFullAlgorithmInterval=") )
      {
	sScanF(line(len,line.length()-1),"%i",&useFullAlgorithmInterval);
	printF(" Setting useFullAlgorithmInterval=%i \n",useFullAlgorithmInterval);
      }
      else if( len=line.matches("-numSteps=") )
      {
	sScanF(line(len,line.length()-1),"%i",&numberOfSteps);
      }
      else if( len=line.matches("-numParallelGhost=") )
      {
	sScanF(line(len,line.length()-1),"%i",&numParallelGhost);
	printF(" Setting numParallelGhost=%i \n",numParallelGhost);
      }
      else
      {
	printF("Unknown option=[%s]\n",(const char*)line);
      }
      
      
    }
  }

  // Ogmg::debug=15;
  
  // Oges::debug=7;  // *********

  MappedGrid::setMinimumNumberOfDistributedGhostLines(numParallelGhost);


  // Create two CompositeGrid objects, cg[0] and cg[1]
  CompositeGrid cg[2];                             
  getFromADataBase(cg[0],nameOfOGFile);             // read cg[0] from a data-base file

  if( width>0 )
  { // change the interpolation width
    int oldWidth=max(cg[0].interpolationWidth);
    printF("Changing the interpolation width to %i (old width=%i)\n",width,oldWidth);
    cg[0].changeInterpolationWidth(width);

    // TEST:
    // cg[0].interpolationOverlap=1.;   // (IW+DW-3)*.5 
  }
  

  cg[1]=cg[0];                                      // copy cg[0] into cg[1]
  const int numberOfDimensions = cg[0].numberOfDimensions();

  PlotStuff ps(plotOption,"Moving Grid Example II");         // for plotting
  PlotStuffParameters psp;
  if( numberOfDimensions==2 )
    psp.set(GI_PLOT_INTERPOLATION_POINTS,true);
  

  if( debug & 4  )
  {
    psp.set(GI_TOP_LABEL,"initial grid");  // set title
    PlotIt::plot(ps,cg[0],psp);
  }

  aString nameOfShowFile = "move2.show";
  Ogshow show( nameOfShowFile );
  show.saveGeneralComment("Moving grid example");
  show.setIsMovingGridProblem(true);
  show.setFlushFrequency(2);

  // -- Here is the grid generator --
  Ogen gridGenerator(ps);

  const int numberOfComponentGrids = cg[0].numberOfComponentGrids();

  // For now we move all grids but the first:
  int numberOfGridsToMove=numberOfComponentGrids-1;
  int *gridsToMove = new int [numberOfGridsToMove];
  for( int grid=0; grid<numberOfGridsToMove; grid++ )
    gridsToMove[grid]=grid+1;
  // special case of 1 grid -- it will move
  if( numberOfComponentGrids==1 )
    gridsToMove[0]=0;

  // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
  // can rotate/scale and shift any Mapping
  MatrixTransform **transform[2];
  transform[0]= new MatrixTransform* [numberOfGridsToMove];
  transform[1]= new MatrixTransform* [numberOfGridsToMove];
  
  for( int g=0; g<numberOfGridsToMove; g++ )
  {
    int grid=gridsToMove[g];

    // Use this MatrixTransform to change the existing Mapping, the MatrixTransform
    // can rotate/scale and shift any Mapping, keep a transform for each composite grid
    Mapping & mappingToMove = *(cg[0][grid].mapping().mapPointer);
    transform[0][g] = new MatrixTransform(mappingToMove);
    transform[1][g] = new MatrixTransform(mappingToMove);
    transform[0][g]->incrementReferenceCount();
    transform[1][g]->incrementReferenceCount();
    
    cg[0][grid].reference(*transform[0][g]); 
    cg[1][grid].reference(*transform[1][g]); 
  }
  cg[0].updateReferences();
  cg[1].updateReferences();

  // For Twilight-zone we will need the grid point arrays
  cg[0].update(MappedGrid::THEvertex | MappedGrid::THEcenter);
  cg[1].update(MappedGrid::THEvertex | MappedGrid::THEcenter);

  // update the initial grid, since the above reference destroys the mask
  gridGenerator.updateOverlap(cg[0]);

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
  int degreeX=1;
  OGPolyFunction exact(degreeX,numberOfDimensions,2,1);   
  realCompositeGridFunction exactSolution;
  exact.assignGridFunction(u[0]);
  exact.assignGridFunction(u[1]);

  Interpolant & interpolant = *new Interpolant; interpolant.incrementReferenceCount();
  interpolant.setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);

  const int orderOfAccuracy=2;

  // make a grid function to hold the coefficients
  int stencilSize=int( pow(3,numberOfDimensions)+1.5);  // add 1 for interpolation equations
  const int numberOfGhostLines=orderOfAccuracy/2;
  // realCompositeGridFunction coeff(cg[0],stencilSize,all,all,all); 
  realCompositeGridFunction coeff;
  if( solvePoisson )
  {
    coeff.updateToMatchGrid(cg[0],stencilSize,all,all,all);
    coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
    coeff=0.;
  }
  
  realCompositeGridFunction w(cg[0]),f(cg[0]);
  w=0.; // for iterative solvers

  CompositeGridOperators op(cg[0]);                            // create some differential operators
  op.setStencilSize(stencilSize);
  op.setOrderOfAccuracy(orderOfAccuracy);

  
  Oges & solver = *new Oges( cg[0] );    // create a solver 
//   if( solvePoisson )
//    solver.setCoefficientArray( coeff );   // supply coefficients
  solver.set(OgesParameters::THEsolverType,solverType); 
//   if( solverType==OgesParameters::SLAP ||  solverType==OgesParameters::PETSc )
//   {
//     solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
//     solver.set(OgesParameters::THErelativeTolerance,max(1.e-8,REAL_EPSILON*10.));
//   }    
  real tol=1.e-5;
  int iluLevels=-1; // -1 : use default
  if( solver.isSolverIterative() ) 
  {
    solver.setCommandLineArguments( argc,argv );
    if( solverType==OgesParameters::PETSc )
      solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
    else if( solverType==OgesParameters::PETScNew )
    { // parallel: -- NOTE: in parallel the solveMethod should be preonly and the parallelSolverMethod bicgs etc.
      solver.set(OgesParameters::THEbestIterativeSolver);
    }
    else
      solver.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradient);
	
    solver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
    solver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
    solver.set(OgesParameters::THEmaximumNumberOfIterations,10000);
    if( iluLevels>=0 )
      solver.set(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
  }    

  printF("\n === Solver:\n %s\n =====\n",(const char*)solver.parameters.getSolverName());



  // now we destroy all the data on the new grid -- it will be shared with the old grid
  // this is not necessary to do
  cg[1].destroy(CompositeGrid::EVERYTHING);  

  ExposedPoints exposedPoints;  // for interpolating exposed points
  ExposedPoints::debug=debug;
  

  LogicalArray hasMoved(numberOfComponentGrids);
  hasMoved    = true;
  hasMoved(0) = false;  // Only this grid will NOT move.

  // special case of 1 grid -- it will move
  if( numberOfComponentGrids==1 )
    hasMoved=true;
  

  if( false )
  {
    leakCheck(cg[0]);
    leakCheck(cg[1]);
    return 0;
  }
  


  char buff[80];
  aString showFileTitle[2];

  real matrixSetUpTime=0.;
  real matrixSolveTime=0.;

  Index I1,I2,I3;
  real angle=0., xShift=0.;
  // ----------------------------------------
  // ---- Move the grid a bunch of times.----
  // ----------------------------------------
  for (int i=1; i<=numberOfSteps; i++) 
  {
    int newCG = i % 2;        // new grid
    int oldCG = (i+1) % 2;    // old grid
    // Draw the overlapping grid

    if( plotOption )
    {
      if( moveOption==rotate )
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i, angle=%6.2e",i,angle*180./Pi));  // set title
      else
	psp.set(GI_TOP_LABEL,sPrintF(buff,"Grid at step %i",i));  // set title
      if( true || i==1 )
	psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      ps.erase();
      PlotIt::plot(ps,cg[oldCG],psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

      ps.redraw(true);   // force a redraw
    }
    
    // Move the grids by changing the Mapping (rotate or shift)
    for( int g=0; g<numberOfGridsToMove; g++ )
    {
      MatrixTransform & mtrans = *transform[newCG][g];
      if( moveOption==rotate )
      {
	angle += deltaAngle;
	mtrans.reset();  // reset transform since otherwise rotate is incremental
	mtrans.rotate(axis3,angle);
      }
      else
      {
	xShift += deltaShift;
	// printF(" xShift=%9.3e\n",xShift);
	mtrans.reset();  // reset transform since otherwise shift is incremental
	mtrans.shift(xShift,0.,0.);
      }
    }

    
    // Update the overlapping newCG, starting with and sharing data with oldCG.    
    Ogen::MovingGridOption option = Ogen::useOptimalAlgorithm;
    if( i% useFullAlgorithmInterval == useFullAlgorithmInterval-1  )
    {
      printF(" +++++++++++ use full algorithm in updateOverlap step=%i +++++++++++++++ \n",i);
      option=Ogen::useFullAlgorithm;
    }
    // gridGenerator.debug=7;
    checkArrayIDs("move2: before gridGenerator.updateOverlap");

    // gridGenerator.info=3; // *wdh* 2012/06/17 
    
    Overture::printMemoryUsage(sPrintF("move2:Before gridGenerator step=%i",i));
    // for( int itgg=0; itgg<=10; itgg++ ) // *wdh* 2012/06/17 
    {
      gridGenerator.updateOverlap(cg[newCG], cg[oldCG], hasMoved, option);
    }
    Overture::printMemoryUsage(sPrintF("move2:After gridGenerator step=%i",i));
    
    checkArrayIDs("move2: after gridGenerator.updateOverlap");

    if( Mapping::debug > 0 ) 
      ApproximateGlobalInverse::printStatistics();

    u[newCG].updateToMatchGrid(cg[newCG]);

    // Interpolate any exposed points on the old grid function
    // (pass a TwilightZone function and the routine will compute errors)
    if( interpExposed )
    {
      // Interpolate any exposed points on the old grid function
      // (pass a TwilightZone function and the routine will compute errors)
      printF("Interpolate exposed points...\n");
      Overture::printMemoryUsage(sPrintF("move2:Before Interpolate exposed points step=%i",i));

      real time0=getCPU();
      for( int itgg=0; itgg<=10; itgg++ ) 
      {
	exposedPoints.initialize(cg[oldCG],cg[newCG]);
	exposedPoints.interpolate(u[oldCG],&exact);
      }
      
      real time=getCPU()-time0;
      printF(" Time for interpolateExposedPoints=%8.2e\n",time);
      Overture::printMemoryUsage(sPrintF("move2:After Interpolate exposed points step=%i",i));

    }

    // re-evaluate the exact solution on the moved grid
    cg[newCG].update(MappedGrid::THEvertex | MappedGrid::THEcenter );
    exactSolution.updateToMatchGrid(cg[newCG]);
    exact.assignGridFunction(exactSolution);

    exact.assignGridFunction(u[newCG]);
    // u[newCG]=exactSolution;

    if( interpolate )
    {
      // cg[newCG][1].mask().display("Here is cg[newCG][1]");
      cg[newCG].update(MappedGrid::THEvertex | MappedGrid::THEcenter);
      // cg[newCG][1].mask().display("Here is cg[newCG][1] after cg[newCG][1].update");
      

      // interpolate the new grid function
      // first put bogus values in the interpolation and unused points
      for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[newCG][grid];
	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[newCG][grid],uLocal);

        getIndex(mg.indexRange(),I1,I2,I3); 
	bool ok = ParallelUtility::getLocalArrayBounds(u[newCG][grid],uLocal,I1,I2,I3,1);
	if( ok ) // this processor has grid points
	{
	  where( maskLocal(I1,I2,I3)<=0 )
	    uLocal(I1,I2,I3,0)=1.e5;
	}
      }
      for( int it=0; it<100; it++ )
      {
	checkArrayIDs("Before interpolant.updateToMatchGrid");
	interpolant.updateToMatchGrid(cg[newCG]);
	// u[newCG].display("u before interpolate");
	checkArrayIDs("After interpolant.updateToMatchGrid");

	u[newCG].interpolate();
	checkArrayIDs(sPrintF("After u[newCG].interpolate(): it=%i",it));
      }
      
      // u[newCG].display("u after interpolate");
      

      real error=0.;
      for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[newCG][grid];

	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
	realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u[newCG][grid],uLocal);
	realSerialArray exactLocal; getLocalArrayWithGhostBoundaries(exactSolution[grid],exactLocal);

        getIndex(mg.indexRange(),I1,I2,I3); 
	bool ok = ParallelUtility::getLocalArrayBounds(u[newCG][grid],uLocal,I1,I2,I3,1);
	if( ok ) // this processor has grid points
	{
	  where( maskLocal(I1,I2,I3)!=0 )
	    error=max(error,max(abs(uLocal(I1,I2,I3,0)-exactLocal(I1,I2,I3))));
	}
	
      }
      error=ParallelUtility::getMaxValue(error);  // get max error over all processors
      printF(">>>>Maximum error in interpolating = %e <<<<<<\n",error);  
    }
    
    if( solvePoisson )
    {
      // solve Laplace's equation on the new grid
      printF("Solve a problem with Oges on the new grid...\n");

      cg[newCG].update(MappedGrid::THEmask | MappedGrid::THEvertex | MappedGrid::THEcenter | MappedGrid::THEinverseVertexDerivative);


      real time0=getCPU();
      
      op.updateToMatchGrid(cg[newCG]);
      coeff.updateToMatchGrid(cg[newCG]);
      coeff.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
      coeff.setOperators(op);
      coeff=0.;
      
      if( false )
      {
	coeff=op.laplacianCoefficients();       // get the coefficients for the Laplace operator
      }
      else
      { // new way for parallel -- this avoids all communication
	for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
	{
          MappedGrid & mg = cg[newCG][grid];
	  getIndex(mg.gridIndexRange(),I1,I2,I3);
	  op[grid].coefficients(MappedGridOperators::laplacianOperator,coeff[grid],I1,I2,I3);
	}
      }

      // fill in the coefficients for the boundary conditions
      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,  BCTypes::allBoundaries);
      coeff.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries);

      coeff.finishBoundaryConditions();
      if( true )
      {  // *wdh* 100414 -- Is this needed? ---> YES .. FIX ME
	for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
	{
          coeff[grid].updateGhostBoundaries();
	}
      }
      

      matrixSetUpTime+=getCPU()-time0;
      time0=getCPU();
      
      if( false )
      {
	for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
	{
	  displayCoeff(coeff[grid],sPrintF("Coeff matrix for grid %i",grid));
	}
      }

      // new way: 
      IntegerArray bc(2,3,cg[newCG].numberOfComponentGrids());
      bc=OgesParameters::dirichlet;  // set the boundary conditions
      RealArray bcData(2,2,3,cg[newCG].numberOfComponentGrids()); 
      bcData=0.;

      // solver.mgcg.setGridIsUpToDate(false);  // *************************************** do this for now -- fix me ---

// ****************************************
      if( !reuseSolver )
      {
	delete &solver;
	Oges & solver = *new Oges;    // create a solver 
//   if( solvePoisson )
//    solver.setCoefficientArray( coeff );   // supply coefficients
	

	solver.set(OgesParameters::THEsolverType,solverType); 
	solver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
      }
      
// ****************************************

      solver.setGrid(cg[newCG]);  

      // Ogmg::debug=15;
      // OgmgParameters & ogmgParameters = solver.parameters.buildOgmgParameters();
      // ogmgParameters.setSmootherType(OgmgParameters::redBlackJacobi);

      // solver.parameters.update(ps,cg[newCG]);
      

      solver.setCoefficientsAndBoundaryConditions( coeff,bc,bcData );


      // old way: 
      // solver.setCoefficientArray( coeff );   // supply coefficients
      // This next call will cause the matrix to be recreated and refactored
      // solver.updateToMatchGrid(cg[newCG]);


      f.updateToMatchGrid(cg[newCG]);
      w.updateToMatchGrid(cg[newCG]);
      // assign the rhs: Laplacian(u)=f, u=exact on the boundary
      Index Ia1,Ia2,Ia3;
      int side,axis;
      Index Ib1,Ib2,Ib3;
      for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
	MappedGrid & mg = cg[newCG][grid];
	realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
	realSerialArray xLocal; getLocalArrayWithGhostBoundaries(mg.vertex(),xLocal);
	realSerialArray exactLocal; getLocalArrayWithGhostBoundaries(exactSolution[grid],exactLocal);
	realSerialArray wLocal; getLocalArrayWithGhostBoundaries(w[grid],wLocal);

	// fLocal=0.;
	// wLocal=0.;

        getIndex(mg.indexRange(),I1,I2,I3); 
	bool ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,I1,I2,I3,1);
	if( ok ) // this processor has grid points
	{
          // Here is how we eval derivatives of the TZ function in parallel: 
          realSerialArray uedd(I1,I2,I3);
          bool isRectangular=false;
          real t=0.;
          exact.gd( uedd ,xLocal,numberOfDimensions,isRectangular,0,2,0,0,I1,I2,I3,0,t);   // eval exact.xx
          wLocal(I1,I2,I3)=uedd;
          fLocal(I1,I2,I3)=uedd;
	  if( numberOfDimensions>1 )
	  {
            exact.gd( uedd ,xLocal,numberOfDimensions,isRectangular,0,0,2,0,I1,I2,I3,0,t); // eval exact.yy
            fLocal(I1,I2,I3)+=uedd;
	  }
	  if( numberOfDimensions>2 )
	  {
            exact.gd( uedd ,xLocal,numberOfDimensions,isRectangular,0,0,0,2,I1,I2,I3,0,t); // eval exact.zz
            fLocal(I1,I2,I3)+=uedd;
	  }
	  ForBoundary(side,axis)
	  {
	    if( mg.boundaryCondition(side,axis) > 0 )
	    {
	      getBoundaryIndex(mg.gridIndexRange(),side,axis,Ib1,Ib2,Ib3);
	      ok = ParallelUtility::getLocalArrayBounds(f[grid],fLocal,Ib1,Ib2,Ib3,1);
	      if( ok )
		fLocal(Ib1,Ib2,Ib3)=exactLocal(Ib1,Ib2,Ib3);
	    }
	  }
	}
	
      }
  
      time0=getCPU();
      solver.solve( w,f );   // solve the equations
      matrixSolveTime+=getCPU()-time0;

      Overture::checkMemoryUsage(sPrintF("move2:After poisson solve i=%i",i));

      // ...Calculate the maximum error  (for Twilight-zone flow )
      real error=0.;
      for( int grid=0; grid<cg[newCG].numberOfComponentGrids(); grid++ )
      {
        MappedGrid & mg = cg[newCG][grid];

	realSerialArray wLocal; getLocalArrayWithGhostBoundaries(w[grid],wLocal);
	realSerialArray exactLocal; getLocalArrayWithGhostBoundaries(exactSolution[grid],exactLocal);
	intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
	
	getIndex(mg.indexRange(),I1,I2,I3,1);  
	bool ok = ParallelUtility::getLocalArrayBounds(w[grid],wLocal,I1,I2,I3,1);
	if( ok ) // this processor has grid points
	{
	  where( maskLocal(I1,I2,I3)!=0 )
	    error=max(error, max(abs(wLocal(I1,I2,I3)-exactLocal(I1,I2,I3)))/max(abs(exactLocal(I1,I2,I3))) );
	  if( Oges::debug & 8 )
	  {
	    realSerialArray err(I1,I2,I3);
	    err(I1,I2,I3)=abs(wLocal(I1,I2,I3)-exactLocal(I1,I2,I3))/max(abs(exactLocal(I1,I2,I3)));
	    where( maskLocal(I1,I2,I3)==0 )
	      err(I1,I2,I3)=0.;
	    ::display(err,"abs(error on indexRange +1)","%8.2e ");
	  }
	}
      }
      error=ParallelUtility::getMaxValue(error);  // get max error over all processors
      printF("Maximum relative error with dirichlet bc's= %e\n",error);  
    }

    // save results in a show file:
    if( saveShow )
    {
      show.startFrame();
      sPrintF(buff,"Moving Example, step=%i",i);
      show.saveComment(0,buff);
      show.saveSolution(u[newCG]);
    }

    checkArrayIDs("move2: after saveShow");

  } // end for

  checkArrayIDs("*** **** move2:info: At end ***",true);

  if( solvePoisson )
  {
    printF(" Average time for matrix setup..................%7.2e \n",matrixSetUpTime/numberOfSteps);
    printF(" Average time for matrix solve..................%7.2e \n",matrixSolveTime/numberOfSteps);
    if( Oges::debug & 2 )
    {
      solver.printStatistics();
    }

  }
  
  printF("move2: done! ...\n");

  if( saveShow )
    printF("Results saved in move2.show, use Overture/bin/plotStuff to view this file\n");
  show.close();  // in parallel we need to explicitly close the show here while MPI is still valid.

  delete &solver; // We need to destroy the solver here too while MPI is valid.

  if( plotOption )
  {
    ps.erase();
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    psp.set(GI_TOP_LABEL,"final grid");  // set title
    PlotIt::plot(ps,cg[(numberOfSteps%2)],psp);
  }

  // clean up: 
  delete [] gridsToMove;
  for( int g=0; g<numberOfGridsToMove; g++ )
  {
    if( transform[0][g]->decrementReferenceCount()==0 )
      delete transform[0][g];
    if( transform[1][g]->decrementReferenceCount()==0 )
      delete transform[1][g];
  }
  delete [] transform[0];
  delete [] transform[1];


  printF("move2: interpolant.getReferenceCount=%i\n",interpolant.getReferenceCount());
  if( interpolant.decrementReferenceCount()==0 )
  {
    printF("move2: delete Interpolant\n");
    delete &interpolant;
  }

  Overture::finish();          
  return 0;
}

