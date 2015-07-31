#include "OgmgParameters.h"
#include "GenericGraphicsInterface.h"
#include "DialogData.h"
#include "Ogmg.h"
#include "LoadBalancer.h"
#include "GridStatistics.h"

//\begin{>OgmgParametersInclude.tex}{\subsection{constructor}} 
OgmgParameters::
OgmgParameters()
//==================================================================================
// /Description:
//   Constructor for an OgmgParameters object. Use this class to set
// parameters for Ogmg.
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  cgPointer=NULL;
  smootherName=NULL;
  gridDependentParametersInitialized=false;
  loadBalancer=NULL;
  init();
}

//\begin{>OgmgParametersInclude.tex}{\subsection{constructor}} 
OgmgParameters::
OgmgParameters(CompositeGrid & cg )
//==================================================================================
// /Description:
//   Constructor for an OgmgParameters object. Use this class to set
// parameters for Ogmg.
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  cgPointer=NULL;
  smootherName=NULL;
  gridDependentParametersInitialized=false;
  loadBalancer=NULL;
  
  set(cg);
  init();
  initializeGridDependentParameters(cg);
}

OgmgParameters::
~OgmgParameters()
{
  // printF("OgmgParameters destructor called -- smootherName= %i\n",smootherName);
  delete [] smootherName;
  delete nullVectorParameters;
  delete loadBalancer;
}

void OgmgParameters::
init()
// ========================================================================================
//  /Description:
//    Initialize parameters that are independent of the grid. This function is called by all constructors.
// ========================================================================================
{
  cycleType=cycleTypeC;  // standard "gamma" cycle
  
  assert( smootherName==NULL );

  smootherName = new aString [numberOfSmoothers+1];
  // printF("OgmgParameters::init smootherName= %i\n",smootherName);

  smootherName[Jacobi]="ja"; // "Jacobi";
  smootherName[GaussSeidel]="gs"; // "GaussSeidel";
  smootherName[redBlack]="rb"; // "redBlack";
  smootherName[redBlackJacobi]="rbj"; // "redBlackJacobi";
  smootherName[lineJacobiInDirection1]="lj1"; // "lineJacobiInDirection1";
  smootherName[lineJacobiInDirection2]="lj2"; // "lineJacobiInDirection2";
  smootherName[lineJacobiInDirection3]="lj3"; // "lineJacobiInDirection3";
  smootherName[lineZebraInDirection1]="lz1";  // "lineZebraInDirection1";
  smootherName[lineZebraInDirection2]="lz2";  // "lineZebraInDirection2";
  smootherName[lineZebraInDirection3]="lz3";  // "lineZebraInDirection3";
  smootherName[alternatingLineJacobi]="alj"; // "alternatingLine";
  smootherName[alternatingLineZebra]="alz"; // "alternatingLine";
  smootherName[ogesSmoother]="oges"; 
  smootherName[numberOfSmoothers]="unknown";
  
  useDirectSolverOnCoarseGrid=true;
  numberOfIterationsOnCoarseGrid=10;  // Number of smooths when using the smoother to solve the coarse grid equations
  useDirectSolverForOneLevel=true;    // if true call direct solver if there is only 1 level, otherwise use smoother
  interpolateAfterSmoothing=true;
  

  averagingOption=averageCoarseGridEquations;

  // relaxation parameters: a value of -1 means use default
  omegaJacobi=-1.;                     // relaxation parameter for Jacobi smoother
  omegaGaussSeidel=-1.;                // relaxation parameter for Gauss-Seidel smoother
  omegaRedBlack=-1.;                   // relaxation parameter for red-black smoother
  omegaLineJacobi=-1.;
  omegaLineZebra=-1.;
  variableOmegaScaleFactor=1.; // .98; // .95;
  
  useNewRedBlackSmoother=true; // make true the default, 100417 
  
  defectRatioLowerBound=-1.; // -1 : use default
  defectRatioUpperBound=-1.; // -1 : use default
  defectRatioLowerBoundLineSmooth=-1.; // -1 : use default
  defectRatioUpperBoundLineSmooth=-1.; // -1 : use default

  useLocallyOptimalOmega=true;
  useLocallyOptimalLineOmega=true;
  useSplitStepLineSolver=true;  // if true then only do one direction per smooth, alternating through the directions
  
  maximumNumberOfSubSmooths=10;      
  maximumNumberOfLineSubSmooths=8; // 4; *wdh* 2012/03/16
  subSmoothReferenceGrid=-1;       // -1 : use default grid (determined in Ogmg::updateToMatchGrid)
  
  useFullMultigrid=false; // use a full MG cycle (i.e. start solution process from the coarse grid)
  minimumNumberOfInitialSmooths=-1;       // smooth at least this many times on the first iteration.

  numberOfBoundaryLayersToSmooth=0;
  numberOfBoundarySmoothIterations=1;
  numberOfLevelsForBoundarySmoothing=1;

  combineSmoothsWithIBS=false;
  numberOfInterpolationLayersToSmooth=0; // for smoothing points near interpolation points
  numberOfInterpolationSmoothIterations=1;  // for smoothing interpolation neighbours
  numberOfLevelsForInterpolationSmoothing=1;
  numberOfIBSIterations=2;                  // global iterations of interp. boundary smoothing
  
  gridOrderingForSmooth=2;  // 0= 1...ng  1= ng...1 2=alternate
  
  coarseGridInterpolationWidth=-1;  // -1 = use default

  alternateSmoothingDirections=0;
  
  convergenceCriteria=residualConverged;

  residualTolerance=REAL_EPSILON*1000.;  // this will be scaled by the norm of the RHS f
  absoluteTolerance=REAL_EPSILON*1000.;

  errorTolerance=REAL_EPSILON*100.;
  // *wdh* 110310 useErrorEstimateForConvergence=true;
  
  problemIsSingular=FALSE;
  projectRightHandSideForSingularProblem=true;
  assignMeanValueForSingularProblem=true;
  adjustEquationsForSingularProblem=false;

  nullVectorOption=computeNullVector;
  nullVectorFileName="leftNullVector.hdf";
  nullVectorParameters=NULL;  // Oges parameters when solving for the null vector

  meanValueForSingularProblem=0.;
  maximumNumberOfIterations=15;

  interpolateTheDefect=TRUE;
  maximumNumberOfExtraLevels=3; // 5;

  // we can read/save the multigrid composite grid instead of generating it: 
  readMultigridCompositeGrid=false;
  saveMultigridCompositeGrid=false;
  nameOfMultigridCompositeGrid="mgcg.hdf";

  autoSubSmoothDetermination=true;
  useNewAutoSubSmooth=false; 
  
  showSmoothingRates=false;
  outputMatlabFile=false;
  useOptimizedVersion=true;
  decoupleCoarseGridEquations=false;
  
  maximumNumberOfLevels=100;
  boundaryAveragingOption[0]=imposeDirichlet;     // for extrapolation BC
  boundaryAveragingOption[1]=partialWeighting;           // for equation BC

  ghostLineAveragingOption[0]=imposeExtrapolation; // for extrapolation BC
  ghostLineAveragingOption[1]=partialWeighting;    // imposeNeumann; // partialWeighting;           // for equation BC
  
  fineToCoarseTransferWidth=3;  // 1=injection, 3= full weighting
  coarseToFineTransferWidth=2;  // 2=linear interpolation, 4=cubic interpolation

  // >>>>>>>>>>>>>>>>>>>>
  // These next set of values should be replaced by the boundary condition parameters below
  fourthOrderBoundaryConditionOption=1;  // 0:extrapolate-ghost 1:use equation
  useEquationForDirichletOnLowerLevels=2; // (0=no, 1=yes, 2=only for rectangular)
  useEquationForNeumannOnLowerLevels=0; // (0=no, 1=yes, 2=only for rectangular)
  
  // symmetry is not good for a mixed BC: but is better for the annulus
  useSymmetryForNeumannOnLowerLevels=true;    // replace Neumann BC by even symmetry for l>0
  useSymmetryForDirichletOnLowerLevels=false; // use an odd symmetry condition for the first ghost line on lower levels
  
  // <<<<<<<<<<<<<<<<<<<<<<


  solveEquationWithBoundaryConditions=0;  // for 4th order, solve PDE on boundary with BC's

  // Boundary conditions for the 1st and 2nd ghost lines for fourth-order Neumann BC's
  //      useSymmetry,
  //      useEquationToFourthOrder,
  //      useEquationToSecondOrder,
  //      useExtrapolation

  dirichletFirstGhostLineBC=useEquationToSecondOrder; // assumes 4th order
  // dirichletFirstGhostLineBC=useExtrapolation;
  dirichletSecondGhostLineBC=useExtrapolation;  

  lowerLevelDirichletFirstGhostLineBC=useExtrapolation; // useEquationToSecondOrder; // useSymmetry;
  lowerLevelDirichletSecondGhostLineBC=useExtrapolation;
  orderOfExtrapolationForDirichlet=4;
  orderOfExtrapolationForDirichletOnLowerLevels=4;

  neumannFirstGhostLineBC=useEquationToFourthOrder;

  neumannSecondGhostLineBC=useEquationToSecondOrder;
  // neumannSecondGhostLineBC=useExtrapolation;

  lowerLevelNeumannFirstGhostLineBC=useEquationToSecondOrder; // This means use the mixed-BC --> reduces to symmetry

  // lowerLevelNeumannFirstGhostLineBC=useEquationToFourthOrder;  // **** TEMP ***

  lowerLevelNeumannSecondGhostLineBC=useEquationToSecondOrder; // This means used the mixed-BC with a "wide" formula --> reduces to symmetry

  // line smooths can only do order 4 extrap: 1 4 6 4 1 , *wdh* 110223 (was 5)
  orderOfExtrapolationForNeumann=5;   // 4 *wdh* 110309 - 4 not good enough?
  orderOfExtrapolationForNeumannOnLowerLevels=4; 

  useSymmetryCornerBoundaryCondition=true; // FALSE;  true means use symmetry BC when two equation BC's meet

  useNewFineToCoarseBC=true; // *wdh* 040515 false;

  ogesSmoothParameters= NULL;   // for Oges solvers used as smoothers

  maximumNumberOfInterpolationIterations=-1;
//   #ifdef USE_PPP
//     maximumNumberOfInterpolationIterations=2;
//   #else
//     maximumNumberOfInterpolationIterations=5;
//   #endif

  allowInterpolationFromGhostPoints=false;  // for coarse level interpolation points that are outside a grid
  allowExtrapolationOfInterpolationPoints=false;  // for coarse level interpolation points that are outside a grid

  saveGridCheckFile=false;
  
  chooseGoodParametersOption=0;  // choose good parameters has different levels of robustness, 0=OFF

  orderOfAccuracy=-1;  // -1 means that the order has not been assigned yet
  
}

// =================================================================================
/// \brief Automatically choose good parameters (smoothers etc.) for this grid
// =================================================================================
int OgmgParameters::
chooseGoodMultigridParameters(CompositeGrid & cg, int maxLevels /* =useLevelsInGrid */, int robustnessLevel /* =1 */ )
{

  const real spacingFactor=4.;  // choose a line smoother if ratio of grid spacings exceeds this value

  if( orderOfAccuracy==-1 )
  {
    orderOfAccuracy =  cg[0].discretizationWidth(0)-1;
    if( orderOfAccuracy!=2 && orderOfAccuracy!=4 )
    {
      printF("OgmgParameters::WARNING:orderOfAccuracy=%i is unexpected! Expecting 2 or 4.\n");
      OV_ABORT("error");
    }
  }
  
  int info=3;
  
  if( info & 1 )
    printF(" ------------- Automatically Choosing Good Multigrid Parameters -----------------\n");


  #ifdef USE_PPP
  bool isParallel=true;
  #else
  bool isParallel=false;
  #endif

  // Here is the default smoother for nearly square aspect-ratio grids: 
  //  -- in parallel red-black jacobi is cheaper in terms of communication
  const SmootherTypeEnum defaultSmoother = isParallel ? redBlackJacobi : redBlack;

  const int numberOfDimensions=cg.numberOfDimensions();
  const int numberOfMultigridLevels = maxLevels==useLevelsInGrid ? cg.numberOfMultigridLevels() : maxLevels;
  Range L=numberOfMultigridLevels;

  if( info & 1 )
    printF(" Choosing a V(2,1) cycle.\n");
  // V(2,1) : cycle is default: 
  numberOfCycles=1;        // V-cycle
  numberOfSmooths(0,L)=2;  // pre-smooths
  numberOfSmooths(1,L)=1;  // post-smooths

  useNewRedBlackSmoother=true;

  if( cg.numberOfComponentGrids()>0 )
  {
    // Turn on IBS: 

    combineSmoothsWithIBS=true;

    if( chooseGoodParametersOption<=1 )
    {
      numberOfInterpolationLayersToSmooth=orderOfAccuracy; // =2;  *wdh* 2012/06/08 increased for fourth-order
      numberOfInterpolationSmoothIterations=2;
      numberOfLevelsForInterpolationSmoothing=1;
      maximumNumberOfInterpolationIterations=3;
    }
    else
    {
      // robust parameters:
      numberOfInterpolationLayersToSmooth=orderOfAccuracy+1; // 3; // *wdh* 2012/06/08 increased for fourth-order
      numberOfInterpolationSmoothIterations=3;
      numberOfLevelsForInterpolationSmoothing=2;
      maximumNumberOfInterpolationIterations=4;

    }

    if( info & 1 )
    {
      printF(" Turning on IBS (interpolation boundary smoothing), orderOfAccuracy=%i.\n",orderOfAccuracy);
      printF(" IBS: interp. bndry smoothing: global its=%i, local its=%i, layers=%i for %i levels, %s.\n",
	      numberOfIBSIterations,numberOfInterpolationSmoothIterations,
              numberOfInterpolationLayersToSmooth,numberOfLevelsForInterpolationSmoothing,
	     (combineSmoothsWithIBS==1 ? "combine with smooths" : "apply separately from smooths"));
    }
    
  }

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const bool isRectangular = mg.isRectangular();
    if( isRectangular )
    {
      smootherType(grid,L)=defaultSmoother;
    }
    else
    {
      // If this looks like a curvilinear boundary layer grid then use a line smoother

      SmootherTypeEnum smoother=defaultSmoother;
      // SmootherTypeEnum sm[3]={defaultSmoother,defaultSmoother,defaultSmoother}; //

      real dsMin[3],dsAve[3],dsMax[3];
      GridStatistics::getGridSpacing( mg,dsMin,dsAve,dsMax );

      real minDsMin = numberOfDimensions==2 ? min(dsMin[0],dsMin[1]) : min(dsMin[0],dsMin[1],dsMin[2]);
      real maxDsMax = numberOfDimensions==2 ? max(dsMax[0],dsMax[1]) : max(dsMax[0],dsMax[1],dsMax[2]);
      

      const IntegerArray & gid = mg.gridIndexRange();
      for( int axis=0; axis<numberOfDimensions; axis++ )
      {
        // check the ratio of the max to min spacing in this direction:
        real ratio1 = dsMax[axis]/dsMin[axis];
	
        // check the ratio of the min spacing on this axis to the max spacing in all directions: 
        real ratio2 = maxDsMax/dsMin[axis];

	if( info & 2 )
	  printF(" grid=%i : axis=%i : grid-pts=%i, spacing: (min,ave,max)=(%8.2e,%8.2e,%8.2e), dsMax/dsMin=%8.2e, "
                 " max(dsMax)/dsMin = %8.2e\n",
                 grid,axis,gid(1,axis)-gid(0,axis)+1,dsMin[axis],dsAve[axis],dsMax[axis],ratio1,ratio2);

        if( max(ratio1,ratio2) > spacingFactor )
	{
	  // choose a line smoother in this direction 
	  if( smoother==defaultSmoother )
            smoother = SmootherTypeEnum(lineZebraInDirection1 + axis);
          else
	    smoother = alternatingLineZebra;
	}
      }
      if( info & 1 )
	printF(" grid =%i : smoother = %s\n",grid,
	       (smoother==lineZebraInDirection1 ? "lineZebraInDirection1" :
		smoother==lineZebraInDirection2 ? "lineZebraInDirection2" :
		smoother==lineZebraInDirection3 ? "lineZebraInDirection3" :
		smoother==alternatingLineZebra ? "alternatingLineZebra" :
		smoother==redBlack ? "redBlack" :
		smoother==redBlackJacobi ? "redBlackJacobi" : "unknown"));

      smootherType(grid,L)=smoother;
    }
  } // end for grid


  // -- coarse grid solver ---
  ogesParameters.set(OgesParameters::THEbestIterativeSolver);
  // Note: For nearly singular problems (Neumann + mixed BC's) we need to make sure we solve the coarse grid equations 
  // accurately enough. 
  // Do not over-ride tolerance if it has been set (default value is 0)
  real tol = 0.;
  ogesParameters.get(OgesParameters::THErelativeTolerance,tol);
  if( tol <= 0. ) tol=1.e-3;
  ogesParameters.set(OgesParameters::THErelativeTolerance,tol);
  int maxit=100;  // this should depend on the number of points on the coarse grid!
  ogesParameters.set(OgesParameters::THEmaximumNumberOfIterations,maxit);

  // *wdh* 2012/06/04 -- increase ILU levels on coarse grid solve
  ogesParameters.set(OgesParameters::THEnumberOfIncompleteLULevels,3);
  if( chooseGoodParametersOption>1 )
  { // more robust: 
    ogesParameters.set(OgesParameters::THEnumberOfIncompleteLULevels,5);
  }
  

  if( info & 1 )
  {
    printF(" Choosing coarse grid solver: `best iterative', tol=%9.3e, maxit=%i\n",
	   tol,maxit);
  }

  if( info & 1 )
    printF(" ------------- Done Automatically Choosing Good Multigrid Parameters -----------------\n");

  return 0;
}


//       // --- Do better here : check for stretched grids ---
//       // Do this for now: 

//       const IntegerArray & gid = mg.gridIndexRange();
//       int gridDims[3]={1,1,1}; // 
//       int gridMin=INT_MAX, gridMax=-INT_MAX, minAxis=-1, maxAxis=-1, midAxis=-1;
//       for( int axis=0; axis<numberOfDimensions; axis++ )
//       {
// 	gridDims[axis]=gid(1,axis)-gid(0,axis);
// 	if( gridDims[axis]<gridMin )
// 	{
// 	  gridMin=gridDims[axis]; minAxis=axis;
// 	}
// 	if( gridDims[axis]>gridMax )
// 	{
// 	  gridMax=gridDims[axis]; maxAxis=axis;
// 	} 
//       }
//       assert( minAxis>=0 && maxAxis>=0 );
//       if( numberOfDimensions==2 )
//       {
//         midAxis=maxAxis;
//       }
//       else
//       {
// 	midAxis = (minAxis+maxAxis) == 1 ? 2 : (minAxis+maxAxis) == 2 ? 1 : 0;
//         assert( midAxis!=minAxis && midAxis!=maxAxis );
//       }
//       real ratio1 = real(gridMax)/gridMin;
//       real ratio2 = real(gridDims[midAxis])/gridMin;
//       if( ratio1>5. )
//       {
//         smootherType(grid,L)= lineZebraInDirection1 + minAxis;
//       }
//     }
//   } // end for grid 
  


  // Coarse grid solver: use "best" iterative, set convergence tol's
  // OV_ABORT("chooseGoodMultigridParameters: finish me" );

int OgmgParameters:: 
initializeGridDependentParameters(CompositeGrid & cg, int maxLevels /* =useLevelsInGrid */ )
// ========================================================================================
//  /Description:
//      Initialize parameters that depend on the grid, such as the number of component grids.
// 
// /maxLevels (input): optionally specify the maximum number of multigrid levels that are expected.
//    Use this to update parameters before the extra multigrid levels have actually been built.
// ========================================================================================
{ 
  gridDependentParametersInitialized=true;
 
  if( Ogmg::debug & 4 )
    printF("\n\n $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"
               " *********  OgmgParameters::initializeGridDependentParameters ****\n"
               " *********     grids=%3i levels=%3i                           ****\n"
               " $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n",
              cg.numberOfComponentGrids(),cg.numberOfMultigridLevels() );
  
  const int numberOfMultigridLevels = maxLevels==useLevelsInGrid ? cg.numberOfMultigridLevels() : maxLevels;

  numberOfSmooths.redim(2,numberOfMultigridLevels);
  Range L=numberOfMultigridLevels;
  numberOfSmooths(0,L)=2;  // pre-smooths
  numberOfSmooths(1,L)=1;  // post-smooths

  numberOfSubSmooths.redim(cg.numberOfComponentGrids(),numberOfMultigridLevels);
  numberOfSubSmooths=1;

  totalNumberOfSmooths=0;  // counts smooths
  totalNumberOfSubSmooths.redim(cg.numberOfComponentGrids(),numberOfMultigridLevels);
  totalNumberOfSubSmooths=0;
  totalNumberOfSmoothsPerLevel.redim(numberOfMultigridLevels);
  totalNumberOfSmoothsPerLevel=0;

  smootherType.redim(cg.numberOfComponentGrids(),numberOfMultigridLevels);
  smootherType=redBlack; // default

  smoothingRateCutoff=.6;      // continue smoothing until smoothing rate is bigger than this
  useDirectSolverOnCoarseGrid=true;

  // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
  numberOfCycles.redim(numberOfMultigridLevels);
  numberOfCycles=1;  // V cycle by default

  return 0;
}

int OgmgParameters:: 
numberOfMultigridLevels() const
// this is how many levels the parameters thinks exists
{
  if( cgPointer!=NULL )
    return cgPointer->numberOfMultigridLevels();
  else
    return numberOfSubSmooths.getLength(1);
}

int OgmgParameters:: 
numberOfComponentGrids() const  
// this is how many grids the parameters thinks exists
{
  if( cgPointer!=NULL )
    return cgPointer->numberOfComponentGrids();
  else
    return numberOfSubSmooths.getLength(0);
}


//\begin{>OgmgParametersInclude.tex}{\subsection{operator=}} 
OgmgParameters& OgmgParameters:: 
operator=(const OgmgParameters& x)
//==================================================================================
// /Description:
//   deep copy of data.
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  // ****************************** this may not be up to date -- check ****************
  cgPointer=x.cgPointer;
  ogesParameters=x.ogesParameters;

  smootherName=NULL;

  // for now we do not copy the LoadBalancer. 
  delete loadBalancer;
  loadBalancer=NULL;

  init();

  gridDependentParametersInitialized=x.gridDependentParametersInitialized;
  
  numberOfSmooths.redim(0);
  numberOfSmooths=x.numberOfSmooths;
  smoothingRateCutoff=x.smoothingRateCutoff;
  useDirectSolverOnCoarseGrid=x.useDirectSolverOnCoarseGrid;
  numberOfCycles.redim(0);
  numberOfCycles=x.numberOfCycles;

  convergenceCriteria=x.convergenceCriteria;
  residualTolerance=x.residualTolerance;
  absoluteTolerance=x.absoluteTolerance;
  errorTolerance=x.errorTolerance;

  problemIsSingular=x.problemIsSingular;
  projectRightHandSideForSingularProblem=x.projectRightHandSideForSingularProblem;
  assignMeanValueForSingularProblem=x.assignMeanValueForSingularProblem;

  nullVectorOption=x.nullVectorOption;
  nullVectorFileName=x.nullVectorFileName;
  // *wdh* 100605 nullVectorParameters=x.nullVectorParameters; 
  if( x.nullVectorParameters!=NULL )
  {
    nullVectorParameters = new OgesParameters;
    *nullVectorParameters=*x.nullVectorParameters;  
  }
  else
  {
    nullVectorParameters=NULL;
  }
  
  meanValueForSingularProblem=x.meanValueForSingularProblem;
  maximumNumberOfIterations=x.maximumNumberOfIterations;

  subSmoothReferenceGrid=x.subSmoothReferenceGrid;
  
  numberOfSubSmooths.redim(0);
  numberOfSubSmooths=x.numberOfSubSmooths;

  totalNumberOfSubSmooths.redim(0);
  totalNumberOfSubSmooths=x.totalNumberOfSubSmooths;

  totalNumberOfSmoothsPerLevel.redim(0);
  totalNumberOfSmoothsPerLevel=x.totalNumberOfSmoothsPerLevel;
  
  smootherType.redim(0);
  smootherType=x.smootherType;
  
  useLocallyOptimalOmega=x.useLocallyOptimalOmega;
  useLocallyOptimalLineOmega=x.useLocallyOptimalLineOmega;
  useSplitStepLineSolver=x.useSplitStepLineSolver;
  interpolateAfterSmoothing=x.interpolateAfterSmoothing;
  useNewRedBlackSmoother=x.useNewRedBlackSmoother;
  
  interpolateTheDefect=x.interpolateTheDefect;
  maximumNumberOfExtraLevels=x.maximumNumberOfExtraLevels;
  saveMultigridCompositeGrid=x.saveMultigridCompositeGrid;
  readMultigridCompositeGrid=x.readMultigridCompositeGrid;
  nameOfMultigridCompositeGrid=x.nameOfMultigridCompositeGrid;
  autoSubSmoothDetermination=x.autoSubSmoothDetermination;
  showSmoothingRates=x.showSmoothingRates;
  maximumNumberOfLevels=x.maximumNumberOfLevels;
  boundaryAveragingOption[0]=x.boundaryAveragingOption[0];
  boundaryAveragingOption[1]=x.boundaryAveragingOption[1];

  ghostLineAveragingOption[0]=x.ghostLineAveragingOption[0];
  ghostLineAveragingOption[1]=x.ghostLineAveragingOption[1];

  outputMatlabFile=x.outputMatlabFile;
  useOptimizedVersion=x.useOptimizedVersion;
  decoupleCoarseGridEquations=x.decoupleCoarseGridEquations;
  numberOfIterationsOnCoarseGrid=x.numberOfIterationsOnCoarseGrid;
  fineToCoarseTransferWidth=x.fineToCoarseTransferWidth;
  coarseToFineTransferWidth=x.coarseToFineTransferWidth;

  averagingOption=x.averagingOption;

  useSymmetryForNeumannOnLowerLevels=x.useSymmetryForNeumannOnLowerLevels;  
  useSymmetryForDirichletOnLowerLevels=x.useSymmetryForDirichletOnLowerLevels;
  useSymmetryCornerBoundaryCondition=x.useSymmetryCornerBoundaryCondition;
  useEquationForDirichletOnLowerLevels=x.useEquationForDirichletOnLowerLevels;
  useEquationForNeumannOnLowerLevels=x.useEquationForNeumannOnLowerLevels;  
  solveEquationWithBoundaryConditions=x.solveEquationWithBoundaryConditions;

  dirichletFirstGhostLineBC=x.dirichletFirstGhostLineBC;
  dirichletSecondGhostLineBC=x.dirichletSecondGhostLineBC;
  lowerLevelDirichletFirstGhostLineBC=x.lowerLevelDirichletFirstGhostLineBC;
  lowerLevelDirichletSecondGhostLineBC=x.lowerLevelDirichletSecondGhostLineBC;
  orderOfExtrapolationForDirichlet=x.orderOfExtrapolationForDirichlet;
  orderOfExtrapolationForDirichletOnLowerLevels=x.orderOfExtrapolationForDirichletOnLowerLevels;

  neumannFirstGhostLineBC=x.neumannFirstGhostLineBC;
  neumannSecondGhostLineBC=x.neumannSecondGhostLineBC;
  lowerLevelNeumannFirstGhostLineBC=x.lowerLevelNeumannFirstGhostLineBC;
  lowerLevelNeumannSecondGhostLineBC=x.lowerLevelNeumannSecondGhostLineBC;
  orderOfExtrapolationForNeumann=x.orderOfExtrapolationForNeumann;
  orderOfExtrapolationForNeumannOnLowerLevels=x.orderOfExtrapolationForNeumannOnLowerLevels;

  fourthOrderBoundaryConditionOption=x.fourthOrderBoundaryConditionOption;

  numberOfBoundaryLayersToSmooth=x.numberOfBoundaryLayersToSmooth; 
  numberOfBoundarySmoothIterations=x.numberOfBoundarySmoothIterations;
  numberOfLevelsForBoundarySmoothing=x.numberOfLevelsForBoundarySmoothing;
  
  combineSmoothsWithIBS = x.combineSmoothsWithIBS;
  numberOfInterpolationLayersToSmooth=x.numberOfInterpolationLayersToSmooth; 
  numberOfInterpolationSmoothIterations=x.numberOfInterpolationSmoothIterations; 
  numberOfLevelsForInterpolationSmoothing=x.numberOfLevelsForInterpolationSmoothing;
  numberOfIBSIterations=x.numberOfIBSIterations;

  gridOrderingForSmooth=x.gridOrderingForSmooth;
  totalNumberOfSmooths=x.totalNumberOfSmooths;
  coarseGridInterpolationWidth=x.coarseGridInterpolationWidth;
  alternateSmoothingDirections=x.alternateSmoothingDirections;
  useNewFineToCoarseBC=x.useNewFineToCoarseBC;
  maximumNumberOfInterpolationIterations=x.maximumNumberOfInterpolationIterations;
  
  ogesSmoothParameters=x.ogesSmoothParameters;

  return *this;
}


/* --------- finish me 
// ======================================================================================
/// \brief Return the smoother acceleration parameter (omega) for a given grid and level
// ======================================================================================
real OgmgParameters::
getSmootherAccelerationParameter( int grid, int level, MappedGrid & mg )
{
//   enum SmootherTypeEnum
//   {
//     Jacobi=0,
//     GaussSeidel,
//     redBlack,                // red-black Gauss-Seidel
//     redBlackJacobi,
//     lineJacobiInDirection1,
//     lineJacobiInDirection2,
//     lineJacobiInDirection3,
//     lineZebraInDirection1,
//     lineZebraInDirection2,
//     lineZebraInDirection3,
//     alternatingLineJacobi,
//     alternatingLineZebra,
//     ogesSmoother,
//     numberOfSmoothers
//   };
  const SmootherTypeEnum smootherType = smootherType(grid,level);

  // averagingOption=averageCoarseGridEquations;
  // averagingOption=doNotAverageCoarseGridEquations;

  if( smootherType==redBlack || smootherType==redBlackJacobi )
  {
  }
  else if( smootherType==lineZebraInDirection1 ||
           smootherType==lineZebraInDirection2 ||
           smootherType==lineZebraInDirection3 ||
  if( smootherType==Jacobi )



  real omegaJacobi;                     // relaxation parameter for Jacobi smoother
  real omegaGaussSeidel;                // relaxation parameter for Gauss-Seidel smoother
  real omegaRedBlack;                   // relaxation parameter for red-black smoother
  real omegaLineJacobi;
  real omegaLineZebra;
  real variableOmegaScaleFactor;        // for scaling automatically chosen omega
  bool useLocallyOptimalOmega;                // use locally optimal omega on non-uniform grids
  bool useLocallyOptimalLineOmega;            // use locally optimal omega on non-uniform grids (line solvers)

  // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
  numberOfCycles.resize(newNumberOfMultigridLevels);

}
---------- */

//\begin{>OgmgParametersInclude.tex}{\subsection{constructor}} 
int OgmgParameters::
set( CompositeGrid & cg)  // apply parameters to this grid.
//==================================================================================
// /Description:
//   Apply parameters to this grid.
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  cgPointer=&cg;
  initializeGridDependentParameters(cg);
  return 0;
  
}

int OgmgParameters::
updateToMatchGrid( CompositeGrid & mg, int maxLevels /* =useLevelsInGrid */ )
// ==============================================================================================
//  /Description:
//     Update multigrid parameters such as those that depend on the number of grids and number
//  of multigrid levels.
//     
// /maxLevels (input): optionally specify the maximum number of multigrid levels that are expected.
//    Use this to update parameters before the extra multigrid levels have actually been built.
// ==============================================================================================
{
  if( cgPointer==NULL )
  {
    cgPointer=&mg;
    initializeGridDependentParameters(mg,maxLevels);
  }
  else
  {
    CompositeGrid & cg = *cgPointer;

    if( Ogmg::debug & 2 )
    {
      printF("*** OgmgParameters::updateToMatchGrid: maxLevels=%i, old=cg.numberOfMultigridLevels=%i"
	     " new=mg.numberOfMultigridLevels=%i (new)\n",
	     maxLevels,cg.numberOfMultigridLevels(),mg.numberOfMultigridLevels());
    }
    
    if( orderOfAccuracy==-1 )  // order has not been set yet
    {
      orderOfAccuracy = cg[0].discretizationWidth(0)-1;  
      if( orderOfAccuracy!=2 && orderOfAccuracy!=4 )
      {
	printF("OgmgParameters::WARNING:orderOfAccuracy=%i is unexpected! Expecting 2 or 4.\n");
	OV_ABORT("error");
      }
    }

    if( !gridDependentParametersInitialized )
      initializeGridDependentParameters(cg,maxLevels);

    const int oldNumberOfMultigridLevels=cg.numberOfMultigridLevels();
    const int oldNumberOfComponentGrids =cg.numberOfComponentGrids();
    const int newNumberOfMultigridLevels=maxLevels==useLevelsInGrid ? mg.numberOfMultigridLevels() :
                                         maxLevels;
    const int newNumberOfComponentGrids =mg.numberOfComponentGrids();

    bool numberOfMultigridLevelsHasChanged= 
      oldNumberOfMultigridLevels > 0 &&
      newNumberOfMultigridLevels > 0 &&
      (oldNumberOfMultigridLevels!=newNumberOfMultigridLevels);
  
    bool numberOfComponentGridsHasChanged = 
      oldNumberOfMultigridLevels > 0 &&
      newNumberOfMultigridLevels > 0 &&
      (oldNumberOfComponentGrids!=newNumberOfComponentGrids);

    if( newNumberOfComponentGrids <=smootherType.getLength(0) &&
        newNumberOfMultigridLevels<=smootherType.getLength(1) )
    { 
      cgPointer=&mg;  // *wdh* 100410
      return 0; 
    }
    

    if( numberOfMultigridLevelsHasChanged )
    {
      if( newNumberOfMultigridLevels>oldNumberOfMultigridLevels )
      {
	Range R(oldNumberOfMultigridLevels,newNumberOfMultigridLevels-1);
	numberOfSmooths.resize(2,newNumberOfMultigridLevels);
	numberOfSmooths(0,R)=2;
	numberOfSmooths(1,R)=1;

        // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
	numberOfCycles.resize(newNumberOfMultigridLevels);
	if( oldNumberOfMultigridLevels>0 )
	  numberOfCycles(R)=numberOfCycles(oldNumberOfMultigridLevels-1);
        else
    	  numberOfCycles(R)=1;

	totalNumberOfSmoothsPerLevel.resize(newNumberOfMultigridLevels);
	totalNumberOfSmoothsPerLevel(R)=0;
      }
      else
      {
	numberOfSmooths.resize(2,newNumberOfMultigridLevels);
	numberOfCycles.resize(newNumberOfMultigridLevels);
	totalNumberOfSmoothsPerLevel.resize(newNumberOfMultigridLevels);
	totalNumberOfSmoothsPerLevel=0;
      }
      
    }
    if( numberOfMultigridLevelsHasChanged || numberOfComponentGridsHasChanged )
    {
      // smootherType.display("OgmgParameters::updateToMatchGrid smootherType BEFORE");
      if( numberOfSubSmooths.getLength(0)==0 )
      {
	numberOfSubSmooths.redim(newNumberOfComponentGrids,newNumberOfMultigridLevels);
	smootherType.redim(newNumberOfComponentGrids,newNumberOfMultigridLevels);
	numberOfSubSmooths=1;
	smootherType=redBlack;

        totalNumberOfSubSmooths.redim(newNumberOfComponentGrids,newNumberOfMultigridLevels);
        totalNumberOfSubSmooths=0;
	
      }
      else
      {
	numberOfSubSmooths.resize(newNumberOfComponentGrids,newNumberOfMultigridLevels);
	smootherType.resize(newNumberOfComponentGrids,newNumberOfMultigridLevels);
        totalNumberOfSubSmooths.resize(newNumberOfComponentGrids,newNumberOfMultigridLevels);

	for( int level=0; level<newNumberOfMultigridLevels; level++ )
	{
	  for( int grid=0; grid<newNumberOfComponentGrids; grid++ )
	  {
	    if( grid>=oldNumberOfComponentGrids || level>=oldNumberOfMultigridLevels )
	    {
	      numberOfSubSmooths(grid,level)= (grid>0 || level>0) ? numberOfSubSmooths(0,0) : 1;
	      // smootherType(grid,level)=(grid>0 || level>0) ? smootherType(0,0) : redBlack; // *wdh* 100607
              if( grid>0 || level>0 )
	      {
                // new grid or new level: 
		if( grid<oldNumberOfComponentGrids )
		{  // old grid, new level: keep same smoother as level 0
		  smootherType(grid,level)= smootherType(grid,0);
		}
		else 
		{ // new grid
		  smootherType(grid,level)= smootherType(0,0);
		}
	      }
	      else
	      {
                smootherType(grid,level)= redBlack;
	      }
		
	      totalNumberOfSubSmooths(grid,level)=0;
	      
	    }
	  }
	}
      }
      
      // smootherType.display("OgmgParameters::updateToMatchGrid smootherType AFTER");
      
    }  

    cgPointer=&mg;
  }
  
  return 0;
}




//\begin{>>OgmgParametersInclude.tex}{\subsection{set( OptionEnum , int )}} 
int OgmgParameters::
set( OptionEnum option, int value /* = 0 */ )
//==================================================================================
// /Description:
//   Set an int option from the {\tt OptionEnum}.
// \begin{verbatim}
//   enum OptionEnum
//   {
//     THEnumberOfCycles,                  // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
//     THEnumberOfSmooths,                 // (minimum) number of smooths (level)
//     THEnumberOfSubSmooths,              // number of sub smooths (grid,level)
//     THEsmootherType,                    // type of smooth (grid,level)
//     THEsmoothingRateCutoff,             // continue smoothing until smoothing rate is bigger than this
//     THEuseDirectSolverOnCoarseGrid;     // if true use Oges, if false use a 'smoother' on the coarse grid.
//     THEresidualTolerance, 
//     THEabsoluteTolerance, 
//     THEerrorTolerance, 
//     THEmeanValueForSingularProblem,
//     THEmaximumNumberOfIterations
//     THEfineToCoarseTransferWidth,
//     THEcoarseToFineTransferWidth
//     THEnumberOfInitialSmooths,
//     THEuseFullMultigrid
//     THEorderOfAccuracy
//   };
// \end{verbatim}
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  return set(option,value,(real)value);
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{set( OptionEnum , float )}} 
int OgmgParameters::
set( OptionEnum option, float value )
//==================================================================================
// /Description:
//    Set a real valued option from the {\tt OptionEnum}.
// \begin{verbatim}
// \end{verbatim}
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  return set(option,(int)value,(real)value);
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{set( OptionEnum , double )}} 
int OgmgParameters::
set( OptionEnum option, double value )
//==================================================================================
// /Description:
//    Set a real valued option from the {\tt OptionEnum}.
// \begin{verbatim}
// \end{verbatim}
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  return set(option,(int)value,(real)value);
}


int OgmgParameters::
set( OptionEnum option, int value, real rvalue )
//==================================================================================
//  /Description:
//    generic set for int and real valued options.
//==================================================================================
{
  Range all;
  switch (option) 
  {
  case THEnumberOfCycles:              // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
    numberOfCycles=value;
    break;
  case THEnumberOfPreSmooths:                 // (minimum) number of pre-smooths (level)
    numberOfSmooths(0,all)=value;
    break;
  case THEnumberOfPostSmooths:                 // (minimum) number of post-smooths (level)
    numberOfSmooths(1,all)=value;
    break;
  case THEnumberOfSmooths:                 // for backward compatibility
    numberOfSmooths(0,all)=(value+1)/2;
    numberOfSmooths(1,all)=value/2;
    break;
  case THEnumberOfSubSmooths:              // number of sub smooths (grid,level)
    numberOfSubSmooths=value;
    break;
  case THEsmootherType:                    // type of smooth (grid,level)
    smootherType=(SmootherTypeEnum)value;
    break;
  case THEsmoothingRateCutoff:             // continue smoothing until smoothing rate is bigger than this
    smoothingRateCutoff=rvalue;
    break;
  case THEuseDirectSolverOnCoarseGrid:     // if true use Oges: if false use a 'smoother' on the coarse grid.
    useDirectSolverOnCoarseGrid=(bool)value;
    break;
  case THEresidualTolerance: 
    residualTolerance=rvalue;
    break;
  case THEabsoluteTolerance: 
    absoluteTolerance=rvalue;
    break;
  case THEerrorTolerance: 
    errorTolerance=rvalue;
    break;
  case THEprojectRightHandSideForSingularProblem:
    projectRightHandSideForSingularProblem=(bool)value;
    break;
  case THEassignMeanValueForSingularProblem:
    assignMeanValueForSingularProblem=(bool)value;
    break;
  case THEmeanValueForSingularProblem:
    meanValueForSingularProblem=rvalue;
    break;
  case THEmaximumNumberOfIterations:
    maximumNumberOfIterations=value;
    break;
  case THEmaximumNumberOfExtraLevels:
    maximumNumberOfExtraLevels=value;
    break;
  case THEfineToCoarseTransferWidth:
    fineToCoarseTransferWidth=value;
    break;
  case THEcoarseToFineTransferWidth:
    coarseToFineTransferWidth=value;
    break;
  case THEnumberOfInitialSmooths:
    minimumNumberOfInitialSmooths=int(value+.5);
    break;
  case THEuseFullMultigrid:
    useFullMultigrid=value!=0.;
    break;
  case THEorderOfAccuracy:
    orderOfAccuracy=value;
    break;
  default:
    printF("OgmgParameters::set: Unknown option=%i! This should not happen\n",option);
    Overture::abort();
  }
  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setErrorTolerance}}
int OgmgParameters:: 
setErrorTolerance(const real errorTolerance_ )
//==================================================================================
// /Description:
//    Set the tolerance for convergence in terms of an estimate for the error.
// The iteration is deemed to converge if the error estimate is less than this
// error tolerance.
// 
// The estimate $E^n$ for the error at iteration $n$ is computed as
// \begin{align*}
//     \delta^n = { \| u^{n+1} - u^n \| \over \| u^n - u^{n-1} \| } \\
//     E^n = {\delta\over 1-\delta} \| u^n - u^{n-1} \| 
// \end{align*}
//  where we use the maximum norm.
//
// /errorTolerance\_ (input):
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  errorTolerance=errorTolerance_;
  return 0;
}


//\begin{>>OgmgParametersInclude.tex}{\subsection{setMaximumNumberOfIterations}}
int OgmgParameters:: 
setMaximumNumberOfIterations( const int max )
//==================================================================================
// /Description:
//    Specify the maximum number of multigrid iterations (cycles)
// /max (input) : 
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  maximumNumberOfIterations=max;
  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setNumberOfCycles}}
int OgmgParameters:: 
setNumberOfCycles(  const int & number, const int & level /* =allLevels */ )
//==================================================================================
// /Description:
//    Specify the number of iterations on each level for pre and post smooths.
// The pre-smooth is only done the first iteration of a level solve.
// /max (input) : 
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  assert( cgPointer!=NULL );
  CompositeGrid & cg = *cgPointer;

  if( level==allLevels )
    numberOfCycles=number;
  else
   numberOfCycles(level)=number; 

  // *wdh* 100725 numberOfCycles(cg.numberOfMultigridLevels()-1)=1;  // coarse grid value should be one.
  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setNumberOfSubSmooths}}
int OgmgParameters:: 
setNumberOfSubSmooths( const int & number, const int & grid, const int & level /* =allLevels */)
//==================================================================================
// /Description:
//    Specify the number of sub-smoothing steps to take on a particular grid.
//  **** This value is currently not used since an automatic algorithm is used to determine
//   the number of sub-smooths. *****
// /number (input):
// /grid (input): if grid<0 on input then assign all grids
// /level (input):
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  Range G;
  if( grid>=0 )
    G=Range(grid,grid);
  if( level==allLevels )
    numberOfSubSmooths(G,nullRange)=number;
  else
   numberOfSubSmooths(G,level)=number;

  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setMeanValueForSingularProblem}}
int OgmgParameters:: 
setMeanValueForSingularProblem( const real meanValue )
//==================================================================================
// /Description:
//   Set the mean value of the solution for a singular problem.
//  /meanValue (input):
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  meanValueForSingularProblem=meanValue;
  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setNumberOfSmooths}}
int OgmgParameters:: 
setNumberOfSmooths(const int numberOfPreSmooths, const int numberOfPostSmooths, const int level)
//==================================================================================
// /Description:
//   Set the number of (composite smooths) to use on each level
// /numberOfPreSmooths (input):
// /numberOfPostSmooths (input):
// /level (input):
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  assert( cgPointer!=NULL );
  CompositeGrid & cg = *cgPointer;
  const int numberOfMultigridLevels=numberOfSmooths.getLength(1);
  if( level<0 || level>=numberOfMultigridLevels )
  {
    printF("OgmgParameters::setNumberOfSmooths:ERROR: Invalid argument level = %i"
           " maximum number of numberOfMultigridLevels=%i \n",level,numberOfMultigridLevels);
    return 1;
  }
  numberOfSmooths(0,level)=numberOfPreSmooths;
  numberOfSmooths(1,level)=numberOfPostSmooths;
  return 0;
}


//\begin{>>OgmgParametersInclude.tex}{\subsection{setProblemIsSingular}}
int OgmgParameters:: 
setProblemIsSingular( const bool trueOrFalse )
//==================================================================================
// /Description:
//   Indicate if the problem is singular. (Neumann problem or periodic square, for example).
// /trueOrFalse (input) : if true then the problem is singular.
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  problemIsSingular=trueOrFalse;
  return 0;
}


// ===================================================================================================
// Set the absolute tolerance for the residual based convergence criteria.
//    max(residual) < residualTolerance*l2Nnorm(f) + absoluteTolerance
// ===================================================================================================
int OgmgParameters::
setAbsoluteTolerance(const real absoluteTolerance_ )
{
  absoluteTolerance=absoluteTolerance_;
  return 0;
}



//\begin{>>OgmgParametersInclude.tex}{\subsection{setResidualTolerance}}
int OgmgParameters:: 
setResidualTolerance(const real residualTolerance_ )
//==================================================================================
// /Description:
//    Set the relative tolerance for convergence in terms of the residual.
//
//     l2Norm(residual) < residualTolerance*l2Nnorm(f) + absoluteTolerance
//
// OLD COMMENTS:
// 
// This tolerance will be scaled by the number of grid points since for second order
// elliptic problems this will very roughly mean that for a given value of {\tt  residualTolerance\_}
// the error achieved in the solution will be approximately the same as the grid is refined.
//
// The solution will be deemed to have converged if both the maximum residual is less
// than this tolerance times the number of grid points and the error tolerance is also satisfied. 
// /residualTolerance\_ (input): tolerance for the residual which will be multiplied by the number
// of grid points before comparing to the maximum residuial.  
//
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  residualTolerance=residualTolerance_;
  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setSmootherType}}
int OgmgParameters::
setSmootherType(const SmootherTypeEnum & smoother, 
		const int & grid /* =allGrids */, 
		const int & level /* =allLevels */ )
//==================================================================================
// /Description:
//   Set the type of smoother.
//
// /smoother (input) : use this smoother, one of
// {\footnotesize
// \begin{verbatim} 
// enum smootherTypes
//  {
//    Jacobi,
//    GaussSeidel,
//    redBlack,
//    redBlackJacobi,
//    lineJacobiInDirection1,
//    lineJacobiInDirection2,
//    lineJacobiInDirection3,
//    lineZebraInDirection1,
//    lineZebraInDirection2,
//    lineZebraInDirection3,
//    alternatingLineJacobi,
//    alternatingLineZebra,
//    ogesSmoother
//  };
// \end{verbatim}
// }
// /grid (input) : use the smoother on this grid, by default use on all grids.
// /level (input) : use the smoother on this level, by default use on all levels.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  Range G,L;
  if( grid!=allGrids )
    G=Range(grid,grid);
  if( level!=allLevels )
    L=Range(level,level);

  smootherType(G,L)=smoother;
  return 0;
}









//\begin{>>OgmgParametersInclude.tex}{\subsection{get( OptionEnum , int \& )}} 
int OgmgParameters::
get( OptionEnum option, int & value ) const
//==================================================================================
// /Description:
//  Get the value of an `int' valued option.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  real rvalue=0.;
  return get(option,value,rvalue);
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{get( OptionEnum , real \&  )}} 
int OgmgParameters::
get( OptionEnum option, real & value ) const
//==================================================================================
// /Description:
//  Get the value of an `real' valued option.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  int ivalue=0;
  return get(option,ivalue,value);
}

int OgmgParameters::
get( OptionEnum option, int & value, real & rvalue ) const
//==================================================================================
// /Description:
//   Generic get.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  switch (option) 
  {
  case THEsmoothingRateCutoff:             // continue smoothing until smoothing rate is bigger than this
    rvalue=smoothingRateCutoff;
    break;
  case THEuseDirectSolverOnCoarseGrid:     // if true use Oges: if false use a 'smoother' on the coarse grid.
    value=useDirectSolverOnCoarseGrid;
    break;
  case THEresidualTolerance: 
    rvalue=residualTolerance;
    break;
  case THEabsoluteTolerance: 
    rvalue=absoluteTolerance;
    break;
  case THEerrorTolerance: 
    rvalue=errorTolerance;
    break;
  case THEprojectRightHandSideForSingularProblem:
    value=projectRightHandSideForSingularProblem;
    break;
  case THEassignMeanValueForSingularProblem:
    value=assignMeanValueForSingularProblem;
    break;
  case THEmeanValueForSingularProblem:
    rvalue=meanValueForSingularProblem;
    break;
  case THEmaximumNumberOfIterations:
    value=maximumNumberOfIterations;
    break;
  case THEfineToCoarseTransferWidth:
    value=fineToCoarseTransferWidth;
    break;
  case THEcoarseToFineTransferWidth:
    value=coarseToFineTransferWidth;
    break;
  case THEorderOfAccuracy:
    value=orderOfAccuracy;
    break;
  default:
    printF("OgmgParameters::get: Unknown option=%i! This should not happen\n",option);
    Overture::abort();
  }
  return 0;
}


// This next macro can be used to get or put stuff to the dataBase.
#define GET_PUT_LIST(getPut)  \
  subDir.getPut(numberOfCycles,"numberOfCycles" );  \



//\begin{>>OgmgParametersInclude.tex}{\subsection{get from a data base}} 
int OgmgParameters::
get( const GenericDataBase & dir, const aString & name)
//==================================================================================
// /Description:
//   Get a copy of the OgmgParameters from a database file
// /dir (input): get from this directory of the database.
// /name (input): the name of Ogmg on the database.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"OgmgParameters");

  subDir.setMode(GenericDataBase::streamInputMode);

  GET_PUT_LIST(get);

  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{put to a data base}} 
int OgmgParameters::
put( GenericDataBase & dir, const aString & name) const   
//==================================================================================
// /Description:
//   Output an image of OgmgParameters to a data base. 
// /dir (input): put onto this directory of the database.
// /name (input): the name of Ogmg on the database.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Ogmg");                      // create a sub-directory 

  subDir.setMode(GenericDataBase::streamOutputMode);

  GET_PUT_LIST(put);
  
  return 0;
}


//\begin{>>OgmgParametersInclude.tex}{\subsection{display}} 
int OgmgParameters::
display(FILE *file /* = stdout */ )
//==================================================================================
// /Description:
//   Print out current values of parameters
// /file (input) : print to this file (standard output by default).
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  // fprintF(file,"name = %s\n",(const char*)getSolverName());


  return 0;
}

//\begin{>>OgmgParametersInclude.tex}{\subsection{setNullVectorOption}}
int OgmgParameters::
setNullVectorOption( NullVectorOptionsEnum option, const aString & fileName )
//==================================================================================
// /Description:
//   Set option concerning the null vector for singular problems
// 
// /option (input) : indicate whether to save/read the null vector from a file or to compute it
//    if no file exists.
// /fileName (input): save the null vector in the file with this name.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  nullVectorOption=option;
  nullVectorFileName=fileName;
  return 0;
}


static int numberOfPushButtons=0;
static int numberOfTextBoxes=0;

int OgmgParameters::
buildOptionsDialog( DialogData & dialog )
// =============================================================================================
// =============================================================================================
{
  dialog.setWindowTitle("OgmgParameters");
  dialog.setExitCommand("exit", "exit");

  dialog.addInfoLabel("See popup menu for more options.");

  // option menus
  dialog.setOptionMenuColumns(1);

  const int maxCommands=20;
  // create a new menu with options for choosing a component.
  aString *label = new aString[maxCommands+1];
  aString *cmd   = new aString[maxCommands+1];

  int n=0;
  label[n]="jacobi"; n++;
  label[n]="gauss-seidel"; n++;
  label[n]="red black"; n++;
  label[n]="red black jacobi"; n++;
  label[n]="line jacobi direction 1"; n++;
  label[n]="line jacobi direction 2"; n++;
  label[n]="line jacobi direction 3"; n++;
  label[n]="line zebra direction 1"; n++;
  label[n]="line zebra direction 2"; n++;
  label[n]="line zebra direction 3"; n++;
  label[n]="alternating"; n++;
  label[n]="alternating jacobi"; n++;
  label[n]="alternating zebra"; n++;
  for( int i=0; i<n; i++ )
    cmd[i]=label[i];
  SmootherTypeEnum smoother=OgmgParameters::redBlack; // default 
  dialog.addOptionMenu("smoother:", cmd,label,smoother);

  n=0;
  label[n]="residual converged"; n++;
  label[n]="error estimate converged"; n++;
  label[n]="residual converged old-way"; n++;
  label[n]=""; cmd[n]="";
  for( int i=0; i<n; i++ )
    cmd[i]="Convergence criteria: " + label[i];

  dialog.addOptionMenu("Convergence criteria:", cmd,label,convergenceCriteria);


  n=0;
  label[n]="compute"; n++;
  label[n]="computeAndSave"; n++;
  label[n]="readOrCompute"; n++;
  label[n]="readOrComputeAndSave"; n++;
  label[n]=""; cmd[n]="";
  for( int i=0; i<n; i++ )
    cmd[i]="null vector option:"+label[i];

  dialog.addOptionMenu("null vector option:", cmd,label,nullVectorOption);


  delete [] label;
  delete [] cmd;


  aString pbcmds[] = {"null vector solve options...",
		      "oges smoother",
		      "oges smoother parameters",
		      ""};
  numberOfPushButtons=3;  // number of entries in cmds
  int numRows=(numberOfPushButtons+1)/2;
  dialog.setPushButtons( pbcmds, pbcmds, numRows ); 


  aString tbCommands[] = {"problem is singular",
                          "project right hand side for singular problems",
                          "set mean value for singular problems",
                          "adjust equations for singular problems",
			  ""};
  int tbState[10];
  tbState[0] = problemIsSingular; 
  tbState[1] = projectRightHandSideForSingularProblem; 
  tbState[2] = assignMeanValueForSingularProblem;
  tbState[3] = adjustEquationsForSingularProblem;
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=8;
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "choose good parameters:"; textStrings[nt]=sPrintF("i",chooseGoodParametersOption); nt++; 

  textLabels[nt] = "null vector file name:";  textStrings[nt]=nullVectorFileName;  nt++; 

  textLabels[nt] = "residual tolerance"; textStrings[nt]=sPrintF("%8.2e",residualTolerance); nt++; 
  textLabels[nt] = "absolute tolerance"; textStrings[nt]=sPrintF("%8.2e",absoluteTolerance); nt++; 
  textLabels[nt] = "error tolerance";    textStrings[nt]=sPrintF("%8.2e",errorTolerance);    nt++; 

  textLabels[nt] = "sub-smooth reference grid:";  
  sPrintF(textStrings[nt],"%i (-1=use default)",subSmoothReferenceGrid);  nt++; 

  textLabels[nt] = "order of accuracy:";  
  sPrintF(textStrings[nt],"%i",orderOfAccuracy);  nt++; 

  // null strings terminal list
  textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textLabels, textLabels, textStrings);
  numberOfTextBoxes=nt;
    
//   aString infoLabel ="number of solutions:";
//   for( int fs=0; fs<numberOfFrameSeries; fs++ )
//   {
//     infoLabel+=sPrintF(" %i",numberOfSolutions[fs]);
//     if( fs<numberOfFrameSeries-1 ) infoLabel+=",";
//   }
//   dialog.addInfoLabel(infoLabel);

  return 0;

}


//\begin{>>OgmgParametersInclude.tex}{\subsection{update}}
int OgmgParameters::
update( GenericGraphicsInterface & gi, CompositeGrid & cg )
// =====================================================================================
// /Description:
//   Update parameters interactively.
//\end{OgmgParametersInclude.tex} 
//==================================================================================
{
  updateToMatchGrid(cg);

  const int numberOfMultigridLevels = numberOfSubSmooths.getLength(1); // use this


  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
    "!Ogmg parameters",
    "choose good parameters",
    ">smoother",
//       "jacobi",
//       "gauss-seidel",
//       "red black",
//       "red black jacobi",
//       "line jacobi direction 1",
//       "line jacobi direction 2",
//       "line jacobi direction 3",
//       "line zebra direction 1",
//       "line zebra direction 2",
//       "line zebra direction 3",
//       "alternating",
//       "alternating jacobi",
//       "alternating zebra",
      "smoother(grid)=[ja][rb][ld1][ld2][ld3][alj][alz]",
      "smoother(grid,level)=[ja][rb][ld1][ld2][ld3][alj][alz]",
//      "oges smoother",
//      "oges smoother parameters",
    "<number of smooths",
    "maximum number of iterations",
    "number of cycles",
    "number of smooths",
    "number of smooths per level",
    "number of sub-smooths",
    "residual tolerance",
    "error tolerance",
    "fine to coarse transfer width",
    "coarse to fine transfer width",
    ">boundary conditions",
      "use symmetry for Neumann BC on lower levels",
      "do not use symmetry for Neumann BC on lower levels",
      "use symmetry corner boundary condition",
      "do not use symmetry corner boundary condition",
      "boundary averaging option",
      "ghost line averaging option",
      "extrapolate fourth order boundary conditions",
      "use equation for fourth order boundary conditions",
      "useEquationForDirichletOnLowerLevels",
      "solve equation with boundary conditions",
      "do not equation with boundary conditions",
    "<>miscellaneous",
      "maximum number of levels",
      "maximum number of extra levels",
      "smoothing rate cutoff",
      "number of iterations on coarse grid",
      "minimum number of initial smooths",
      "interpolate the defect",
      "do not interpolate the defect",
      "use automatic sub-smooth determination",
      "do not use automatic sub-smooth determination",
      "show smoothing rates",
      "output a matlab file",
      "do not show smoothing rates",
      "use optimized version",
      "do not use optimized version",
      "do not couple coarse grid equations",
      "average coarse grid equations",
      "do not average coarse grid equations",
      "do not average coarse curvilinear grid equations",
      "omega Jacobi",
      "omega Gauss-Seidel",
      "omega red-black",
      "omega line-Jacobi",
      "omega line-zebra",
      "variable omega scale factor",
      "use locally optimal omega",
      "do not use locally optimal omega",
      "use locally optimal line omega",
      "do not use locally optimal line omega",
      "use split step line solver",
      "do not use split step line solver",
      "maximum number of sub-smooths",
      "maximum number of line sub-smooths",
      "use full multigrid",
      "do not use full multigrid",
      "interpolate after smoothing",
      "do not interpolate after smoothing",
      "order of extrapolation for Dirichlet on lower levels",
      "forward ordering of grids in smooth",
      "reverse ordering of grids in smooth",
      "alternate ordering of grids in smooth",
      "alternate smoothing directions",
      "fully alternate smoothing directions",
      "do not alternate smoothing directions",
      "number of boundary layers to smooth",
      "number of boundary smooth iterations",
      "number of levels for boundary smooths",
      "combine smooths with IBS",
      "do not combine smooths with IBS",
      "number of interpolation smooth global iterations",
      "number of interpolation layers to smooth",
      "number of interpolation smooth iterations",
      "number of levels for interpolation smooths",
      "use an F cycle",
      "use new fine to coarse BC",
      "do not use new fine to coarse BC",
      "use new auto sub-smooth",
      "do not use new auto sub-smooth",
      "defect ratio lower bound",
      "defect ratio upper bound",
      "defect ratio lower bound for line smooths",
      "defect ratio upper bound for line smooths",
      "use error estimate in convergence test",
      "do not use error estimate in convergence test",
      "maximum number of interpolation iterations",
      "allow interpolation from ghost points", 
      "allow extrapolation of interpolation points",
      "use new red-black smoother",
      "do not use new red-black smoother",
      "save the multigrid composite grid",
      "read the multigrid composite grid",
      "save coarse grid check file",
      "set load balancing options",
    "<debug",
    "Oges::debug",
    ">coarse grid options",
      "Oges parameters",
      "iterate on coarse grid",
      "number of coarse grid iterations",
//      "coarse grid interpolation width",
    "<exit",
    ""
     };
  aString help[] = 
    {
     ""
    };


  GUIState dialog;

  // --- create dialog here: 
  buildOptionsDialog(dialog);

  dialog.buildPopup(menu);
  gi.pushGUI(dialog);


  aString answer,answer2,line; 
  
  int level, len=0;
  gi.appendToTheDefaultPrompt("Ogmg>"); // set the default prompt

  for( int it=0;; it++ )
  {
    // gi.getMenuItem(menu,answer);
    gi.getAnswer(answer,""); 


    if( answer=="choose good parameters" ) // old way 
    {
      chooseGoodParametersOption=1;
      chooseGoodMultigridParameters(cg,numberOfMultigridLevels);
    }
    else if( dialog.getTextValue(answer,"choose good parameters:","%i",chooseGoodParametersOption) ) // new way
    {
      printF("choose good parameters: 0=OFF, 1=ON, 2=Use more robust parameters. Setting value to %i\n",chooseGoodParametersOption);
      if( chooseGoodParametersOption>0 )
      {
	chooseGoodMultigridParameters(cg,numberOfMultigridLevels,chooseGoodParametersOption);
      }
      
    }
    else if( answer(0,8)=="smoother(" )
    {
      int grid=-1, level=-1;
      int length=answer.length();
      sScanF(answer(9,length-1),"%i",&grid);
      for( int i=10; i<length; i++ )
      {
        if( answer[i]=='=' )	  
	{
	  line=answer(i+1,length);
	  break;
	}
	if( answer[i]==',' )
	  sScanF(answer(i+1,length),"%i",&level);
      }
      SmootherTypeEnum smoother=OgmgParameters::redBlack;
      if( line=="rb" )
        smoother=redBlack;
      else if( line=="rbj" )
        smoother=redBlackJacobi;
      else if( line=="ja" || line=="jacobi" )      
        smoother=Jacobi;
      else if( line=="gauss-seidel" || line=="gs" )      
        smoother=GaussSeidel;
      else if( line=="lj1" || line=="line jacobi direction 1")
        smoother=lineJacobiInDirection1;
      else if( line=="lj2" || line=="line jacobi direction 2")
        smoother=lineJacobiInDirection2;
      else if( line=="lj3" || line=="line jacobi direction 3")
        smoother=lineJacobiInDirection3;
      else if( line=="lz1" || line=="line zebra direction 1" )
        smoother=lineZebraInDirection1;
      else if( line=="lz2" || line=="line zebra direction 2" )
        smoother=lineZebraInDirection2;
      else if( line=="lz3" || line=="line zebra direction 3" )
        smoother=lineZebraInDirection3;
      else if( line=="a" || line=="alj" || line=="alternating" || line=="alternating jacobi" )
        smoother=alternatingLineJacobi;
      else if( line=="alz" || line=="alternating zebra" )
        smoother=alternatingLineZebra;
      else if( line=="oges" || line=="ogesSmoother" )
      {
        smoother=ogesSmoother;
      }
      else
      {
	printF("Unknown smoother specified on answer=[%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
        continue;
      }
      printF("set smoother=%s : cg.numberOfComponentGrids()=%i numberOfMultigridLevels=%i\n",
	     (const char*)smootherName[smoother],cg.numberOfComponentGrids(),numberOfMultigridLevels);

      Range G=cg.numberOfComponentGrids(),
            L=numberOfMultigridLevels;
      if( grid>=0 && grid<=G.getBound() )
        G=Range(grid,grid);
      if( level>=0 && level<=L.getBound() )
        L=Range(level,level);
      for( level=L.getBase(); level<=L.getBound(); level++ )
      {
	for( grid=G.getBase(); grid<=G.getBound(); grid++ )
	{
          printF("set smoother(grid=%i,level=%i)=%s\n",grid,level,(const char*)smootherName[smoother]);
	  setSmootherType(smoother,grid,level);
	}
      }
      // cout << "Choosing: " << getSolverName() << endl;
    }
    else if( answer=="oges smoother" )
    {
      int num=gi.getValues("Enter grids to use oges smoother on (`done' to finish)",activeGrids,0,cg.numberOfComponentGrids()-1);
      if( ogesSmoothParameters==NULL )
      {
        ogesSmoothParameters=new OgesParameters(); // who will delete this ?
        // set some defaults
	OgesParameters & par = *ogesSmoothParameters;
        int solverType=solverType=OgesParameters::PETSc;
	par.set(OgesParameters::THEsolverType,solverType); 
	par.set(OgesParameters::THEsolverMethod,OgesParameters::biConjugateGradientStabilized);
	par.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
        real tol=1.e-10;                                          // ************************
	par.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
	par.set(OgesParameters::THEmaximumNumberOfIterations,1);  // ************************
        int iluLevels=cg.numberOfDimensions()==2 ? 5 : 3;
        if( iluLevels>=0 )
          par.set(OgesParameters::THEnumberOfIncompleteLULevels,iluLevels);
      }
      for( int g=0; g<=activeGrids.getBound(0); g++ )
      {
        int grid=activeGrids(g);
	setSmootherType(ogesSmoother,grid);
      }
      
	
    }
    else if( answer=="oges smoother parameters" )
    {
      // update oges smoother parameters
      if( ogesSmoothParameters==NULL )
        ogesSmoothParameters=new OgesParameters(); // who will delete this ?
      ogesSmoothParameters->update(gi,cg);
    }
    else if( answer=="jacobi" )
      setSmootherType(OgmgParameters::Jacobi);
    else if( answer=="gauss-seidel" || answer=="gs" )      
      setSmootherType(OgmgParameters::GaussSeidel); 
    else if( answer=="red black" )
      setSmootherType(OgmgParameters::redBlack);
    else if( answer=="red black jacobi" )
      setSmootherType(OgmgParameters::redBlackJacobi);
    else if( answer=="line jacobi direction 1" || answer=="ld1" )
      setSmootherType(OgmgParameters::lineJacobiInDirection1);
    else if( answer=="line jacobi direction 2" || answer=="ld2" )
      setSmootherType(OgmgParameters::lineJacobiInDirection2);
    else if( answer=="line jacobi direction 3" || answer=="ld3" )
      setSmootherType(OgmgParameters::lineJacobiInDirection3);
    else if( answer=="line zebra direction 1" )
      setSmootherType(OgmgParameters::lineZebraInDirection1);
    else if( answer=="line zebra direction 2" )
      setSmootherType(OgmgParameters::lineZebraInDirection2);
    else if( answer=="line zebra direction 3" )
      setSmootherType(OgmgParameters::lineZebraInDirection3);
    else if( answer=="alternating" || answer=="al" || answer=="alternating jacobi" )
      setSmootherType(OgmgParameters::alternatingLineJacobi);
    else if( answer=="alternating zebra" )
      setSmootherType(OgmgParameters::alternatingLineZebra);
    else if( answer=="number of smooths" )
    {
      printF("Enter the number of pre-smooths and post-smooths (applied to all levels)\n");

      int preSmooths=2;
      int postSmooths=1;
      gi.inputString(answer2,sPrintF(line,"Enter the number of pre-smooths and post-smooths (default=2,1)"));
      if( answer2!="" )
	sScanF(answer2,"%i %i",&preSmooths,&postSmooths);

      for(level=0; level<numberOfMultigridLevels; level++ )
      {
	setNumberOfSmooths(preSmooths,postSmooths,level);
	printF("level %i: preSmooths=%i, postSmooths=%i\n",level,preSmooths,postSmooths);
      }
    } 
    else if( answer=="number of smooths per level" )
    {
      int n[8];
//      printF("Enter the maximum number of smooths per cycle. Smoothing may also stop if the smoothing\n"
//             "  rate becomes larger than the smooth rate cutoff (currently=%6.2f)\n",smoothingRateCutoff);

      printF("Enter the number of pre-smooths and post-smooths per level\n");
      printF("If you get tired of entering values enter a blank line and all remaining levels will get the\n"
             "last values entered\n");

      int preSmooths=2;
      int postSmooths=1;
      answer2="start";
      for(level=0; level<numberOfMultigridLevels; level++ )
      {
	if( answer2!="" )
	{
	  gi.inputString(answer2,sPrintF(line,"Enter the number of pre-smooths and post-smooths for level %i",level));
          if( answer2!="" )
	    sScanF(answer2,"%i %i",&preSmooths,&postSmooths);

	}
	setNumberOfSmooths(preSmooths,postSmooths,level);
	printF("level %i: preSmooths=%i, postSmooths=%i\n",level,preSmooths,postSmooths);
      }
    } 
    else if( answer=="maximum number of iterations" )
    {
      int n=10;
      gi.inputString(answer2,sPrintF(buff,"Enter the maximum number of iterations"));
      if( answer2!="" )
      {
	sScanF(answer2,"%i",&n);
        setMaximumNumberOfIterations(n);
      }
    }
    else if( answer=="maximum number of interpolation iterations" )
    {
      int n=10;
      gi.inputString(answer2,"Enter the maximum number of iterations for interpolation (-1=use default)");
      sScanF(answer2,"%i",&maximumNumberOfInterpolationIterations);
      if( maximumNumberOfInterpolationIterations>=0 )
        printF("Setting maximumNumberOfInterpolationIterations=%i\n",maximumNumberOfInterpolationIterations);
      
    }
    else if( answer=="allow interpolation from ghost points" )
    {
      allowInterpolationFromGhostPoints=true;  // for coarse level interpolation points that are outside a grid
      printF(" allowInterpolationFromGhostPoints=%i\n",allowInterpolationFromGhostPoints);
    }
    else if( answer=="allow extrapolation of interpolation points" )
    {
      allowExtrapolationOfInterpolationPoints=true;
      printF(" allowExtrapolationOfInterpolationPoints=%i\n",allowExtrapolationOfInterpolationPoints);
    }
    else if( answer=="number of cycles" )
    {
//       printF(" number of iterations for a level solve: 1=V cycle, 2=W cycle, 3,..\n");
//       int n=1;
//       gi.inputString(answer2,sPrintF(buff,"Enter the number of iterations for a level solve"));
//       if( answer2!="" )
//       {
// 	sScanF(answer2,"%i",&n);
//         setNumberOfCycles(n);
//       }

      int n[8];
      printF("Enter the number of iterations for a level solve: 1=V cycle, 2=W cycle, 3,..\n");
      printF("Enter multiple values, one for each level. The last value entered will apply to all levels not given\n");
      
      gi.inputString(answer2,sPrintF(buff,"Enter the number of iterations for a level solve"));
      if( answer2!="" )
      {
	int numRead=0;
	numRead=sScanF(answer2,"%i %i %i %i %i %i %i %i",&n[0],&n[1],&n[2],&n[3],&n[4],&n[5],&n[6],&n[7]);
	// printF("numRead = %i \n",numRead);
	for(int level=0; level<numberOfMultigridLevels; level++ )
	{
	  setNumberOfCycles((level<numRead ? n[level] : n[numRead-1]),level);
          printF("numberOfCycles(level=%i)=%i\n",level,numberOfCycles(level));
	}
      }

    }
    else if( answer=="number of sub-smooths" )
    {
      IntegerArray values(cg.numberOfComponentGrids());
      int numRead=gi.getValues("Enter sub-smooths per grid (last value is repeated for remaining grids)",
	           values);
//   gi.inputString(answer2,sPrintF(buff,"Enter sub-smooths per grid (last value is repeated for remaining grids)"));
      if( numRead>0 )
      {
	int grid;
	for( int grid=0; grid<min(numRead,cg.numberOfComponentGrids()); grid++ )
	  setNumberOfSubSmooths(values(grid),grid);
	for( grid=numRead; grid<cg.numberOfComponentGrids(); grid++ )
	  setNumberOfSubSmooths(values(numRead-1),grid);
      }
      // numberOfSubSmooths.display("numberOfSubSmooths");
    }
    else if( answer=="minimum number of initial smooths" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the minimum number of initial smooths"));
      if( answer2!="" )
      {
	sScanF(answer2,"%i",&minimumNumberOfInitialSmooths);
        printF("Setting the minimum number of initial smooths to %i\n",minimumNumberOfInitialSmooths);
      }
    }
    else if( answer=="residual tolerance" )
    {
      real tol;
      gi.inputString(answer2,sPrintF(buff,"Enter the residual tolerance"));
      if( answer2!="" )
      {
	sScanF(answer2,"%e",&tol);
        // setResidualTolerance(tol);
        set(THEresidualTolerance,tol);
        printF("residualTolerance=%e\n",residualTolerance);
      }
      else
      {
	printF("ERROR: answer2=[%s]\n",(const char*)answer2);
      }
    }
    else if( answer=="error tolerance" )
    {
      real tol;
      gi.inputString(answer2,sPrintF(buff,"Enter the error tolerance"));
      if( answer2!="" )
      {
	sScanF(answer2,"%e",&tol);
        set(THEerrorTolerance,tol);
      }
    }
    else if( answer=="smoothing rate cutoff" )
    {
      printF("The smoothing rate cutoff determines how many smooths are performed.\n"
             "Smoothing continues until the smoothing rate exceeds this value\n"
             "Typical values lie in the range [.5,.9] \n");
      real tol;
      gi.inputString(answer2,sPrintF(buff,"Enter the smoothing rate cutoff (current=%5.1f)",smoothingRateCutoff));
      if( answer2!="" )
      {
	sScanF(answer2,"%e",&tol);
        set(THEsmoothingRateCutoff,tol);
      }
    }
    else if( answer=="boundary averaging option" )
    {
      printF("Enter the boundary averaging option for `extrapolation' and `equation' type boundary conditions\n");
      printF("0 imposeDirichlet          : impose a dirichlet BC\n"
	     "1 imposeExtrapolation    : explicitly impose an extrapolation equation\n"
	     "2 injection             : \n"
	     "3 partialWeighting   : full weighting in the tangential directions\n"
	     "4 halfWeighting      : full weighting but exclude ghost points.\n"
	     "5 lumpedPartialWeighting : lump partial weighting coefficients in tangential directions.\n"
             "6 imposeNeumann : ");
     gi.inputString(answer2,sPrintF(buff,"Enter the boundary averaging options (current=%i, %i)",
				    boundaryAveragingOption[0],boundaryAveragingOption[1]));
     if( answer2!="" )
     {
       sScanF(answer2,"%i %i",&boundaryAveragingOption[0],&boundaryAveragingOption[1]);
     }
    }
    else if( answer=="ghost line averaging option" )
    {
      printF("Enter the ghost line averaging option for `extrapolation' and `equation' type boundary conditions\n");
      printF("0 imposeDirichlet          : impose a dirichlet BC\n"
	     "1 imposeExtrapolation    : explicitly impose an extrapolation equation\n"
	     "2 injection             : \n"
	     "3 partialWeighting   : full weighting in the tangential directions\n"
	     "4 halfWeighting      : full weighting but exclude ghost points.\n"
	     "5 lumpedPartialWeighting : lump partial weighting coefficients in tangential directions.\n"
             "6 imposeNeumann : \n");
     gi.inputString(answer2,sPrintF(buff,"Enter the ghost line averaging options (current=%i, %i)",
				    ghostLineAveragingOption[0],ghostLineAveragingOption[1]));
     if( answer2!="" )
     {
       sScanF(answer2,"%i %i",&ghostLineAveragingOption[0],&ghostLineAveragingOption[1]);
     }
    }
    else if( answer=="Oges parameters" )
    {
      // update Oges coarse grid solver
      ogesParameters.update(gi,cg);
    }
    else if( answer=="interpolate the defect" )
    {
      interpolateTheDefect=TRUE;
    }
    else if( answer=="do not interpolate the defect" )
    {
      interpolateTheDefect=FALSE;
    }
    else if( answer=="maximum number of levels" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter maximumNumberOfLevels (current=%i)",maximumNumberOfLevels));
      if( answer2!="" )
	sScanF(answer2,"%i",&maximumNumberOfLevels);
      printF("maximumNumberOfLevels = %i \n",maximumNumberOfLevels);
      
    }
    else if( answer=="maximum number of extra levels" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter maximumNumberOfExtraLevels (current=%i)",maximumNumberOfExtraLevels));
      if( answer2!="" )
	sScanF(answer2,"%i",&maximumNumberOfExtraLevels);
      
    }
    else if( answer=="fine to coarse transfer width" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter fineToCoarseTransferWidth (current=%i)",fineToCoarseTransferWidth));
      if( answer2!="" )
	sScanF(answer2,"%i",&fineToCoarseTransferWidth);
    }
    else if( answer=="coarse to fine transfer width" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter coarseToFineTransferWidth (current=%i)",coarseToFineTransferWidth));
      if( answer2!="" )
	sScanF(answer2,"%i",&coarseToFineTransferWidth);
    }
    else if( answer=="debug" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter Ogmg::debug (current=%i)",Ogmg::debug));
      if( answer2!="" )
	sScanF(answer2,"%i",&Ogmg::debug);

      if( Ogmg::debug>6 )
      {
	showSmoothingRates=true;
      }
    }
    else if( answer=="Oges::debug" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter Oges:debug (current=%i)",Oges::debug));
      if( answer2!="" )
	sScanF(answer2,"%i",&Oges::debug);
    }
    else if( answer=="iterate on coarse grid" )
    {
      useDirectSolverOnCoarseGrid=!useDirectSolverOnCoarseGrid;
    }
    else if( answer=="number of iterations on coarse grid" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter number of iterations on coarse grid (current=%i)",numberOfIterationsOnCoarseGrid));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfIterationsOnCoarseGrid);
    }
    // this next answer is deprecated: 
    else if( answer=="use error estimate in convergence test" ||
	     answer=="do not use error estimate in convergence test" )
    {
      // useErrorEstimateForConvergence=answer=="use error estimate in convergence test";

      // -convert to new way: *wdh* 110310
      if( answer=="use error estimate in convergence test" )
	convergenceCriteria=errorEstimateConverged;
      else
	convergenceCriteria=residualConverged;
    }
    else if( answer=="use automatic sub-smooth determination" )
      autoSubSmoothDetermination=TRUE;
    else if( answer=="use new auto sub-smooth" || answer=="do not use new auto sub-smooth" )
      useNewAutoSubSmooth=answer=="use new auto sub-smooth";
    else if( answer=="do not use automatic sub-smooth determination" )
      autoSubSmoothDetermination=FALSE;
    else if( answer=="show smoothing rates" )
      showSmoothingRates=true;
    else if( answer=="do not show smoothing rates" )
      showSmoothingRates=false;
    else if( answer=="output a matlab file" )
      outputMatlabFile=true;
    else if( answer=="use optimized version" )
      useOptimizedVersion=true;
    else if( answer=="do not use optimized version" )
      useOptimizedVersion=false;
    else if( answer=="do not couple coarse grid equations" )
    {
      decoupleCoarseGridEquations=true;
      printF("do not couple coarse grid equations\n");
    }
    else if( answer=="use symmetry for Neumann BC on lower levels" ||
             answer=="do not use symmetry for Neumann BC on lower levels" )
    {
      useSymmetryForNeumannOnLowerLevels= answer=="use symmetry for Neumann BC on lower levels";
      printF(" useSymmetryForNeumannOnLowerLevels=%i\n",useSymmetryForNeumannOnLowerLevels);
    }
    else if( answer=="use symmetry corner boundary condition" )
    {
      useSymmetryCornerBoundaryCondition=TRUE;
      printF(" useSymmetryCornerBoundaryCondition=%i\n",useSymmetryCornerBoundaryCondition);
    }
    else if( answer=="do not use symmetry corner boundary condition" )
    {
      useSymmetryCornerBoundaryCondition=FALSE;
      printF(" useSymmetryCornerBoundaryCondition=%i\n",useSymmetryCornerBoundaryCondition);
    }
//     else if( answer== "number of coarse grid iterations" )
//     {
//       gi.inputString(answer2,sPrintF(buff,"Number of interations for a coarse grid solve (default =%i)",
//                      conjugateGradientNumberOfIterations));
//       if( answer2!="" )
//       {
// 	sScanF(answer2,"%i",&conjugateGradientNumberOfIterations);
//         mgSolver.directSolver.setConjugateGradientNumberOfIterations(conjugateGradientNumberOfIterations);
//       }
//     }
    else if( answer=="average coarse grid equations" )
    {
      averagingOption=averageCoarseGridEquations;
      printF(" average coarse grid equations.\n");
    }
    else if( answer=="do not average coarse grid equations" )
    {
      averagingOption=doNotAverageCoarseGridEquations;
      printF(" do NOT average coarse grid equations.\n");
    }
    else if( answer=="do not average coarse curvilinear grid equations" )
    {
      averagingOption=doNotAverageCoarseCurvilinearGridEquations;
      printF(" do NOT average coarse curvilinear grid equations.\n");
    }
    else if( answer=="extrapolate fourth order boundary conditions" )
    {
      fourthOrderBoundaryConditionOption=0;

      dirichletFirstGhostLineBC=OgmgParameters::useExtrapolation;
      lowerLevelDirichletFirstGhostLineBC=OgmgParameters::useExtrapolation;
      useEquationForDirichletOnLowerLevels=0;

      neumannSecondGhostLineBC=OgmgParameters::useExtrapolation;
      lowerLevelNeumannSecondGhostLineBC=OgmgParameters::useExtrapolation;
    }
    else if( answer=="use equation for fourth order boundary conditions" )
    {
      fourthOrderBoundaryConditionOption=1;

      dirichletFirstGhostLineBC=OgmgParameters::useEquationToSecondOrder;
      lowerLevelDirichletFirstGhostLineBC=OgmgParameters::useExtrapolation; // useSymmetry; // check this 

      neumannSecondGhostLineBC=OgmgParameters::useEquationToSecondOrder;
      lowerLevelNeumannSecondGhostLineBC=OgmgParameters::useEquationToSecondOrder; // check this 
    }
    else if( answer=="use full multigrid" || answer=="do not use full multigrid" )
    {
      useFullMultigrid= answer=="use full multigrid";
      printF(" useFullMultigrid=%i\n",useFullMultigrid);
    }
    else if( answer=="interpolate after smoothing" || answer=="do not interpolate after smoothing" )
    {
      interpolateAfterSmoothing= answer=="interpolate after smoothing";
      printF(" interpolateAfterSmoothing=%i\n",interpolateAfterSmoothing);
    }
    else if( answer=="omega Jacobi" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the relaxation parameter for Jacobi smoothing"));
      if( answer2!="" )
	sScanF(answer2,"%e",&omegaJacobi);
      printF("omegaJacobi=%e\n",omegaJacobi);
    }
    else if( answer=="omega Gauss-Seidel" || answer=="omega gauss-seidel" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the relaxation parameter for Gauss-Seidel smoothing"));
      if( answer2!="" )
	sScanF(answer2,"%e",&omegaGaussSeidel);
      printF("omegaGaussSeidel=%e\n",omegaGaussSeidel);
    }
    else if( answer=="omega red-black" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the relaxation parameter for red-black smoothing"));
      if( answer2!="" )
	sScanF(answer2,"%e",&omegaRedBlack);
      printF("omegaRedBlack=%e\n",omegaRedBlack);
    }
    else if( answer=="omega line-Jacobi" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the relaxation parameter for line-Jacobi smoothing"));
      if( answer2!="" )
	sScanF(answer2,"%e",&omegaLineJacobi);
      printF("omegaJacobi=%e\n",omegaLineJacobi);
    }
    else if( answer=="omega line-zebra" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the relaxation parameter for line-zebra smoothing"));
      if( answer2!="" )
	sScanF(answer2,"%e",&omegaLineZebra);
      printF("omegaJacobi=%e\n",omegaLineZebra);
    }
    else if( answer=="variable omega scale factor" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter variable omega scale factor (current=%7.3f)",
                  variableOmegaScaleFactor));
      if( answer2!="" )
	sScanF(answer2,"%e",&variableOmegaScaleFactor);
      printF("variableOmegaScaleFactor=%e\n",variableOmegaScaleFactor);
    }
    else if( answer=="defect ratio lower bound" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the defect ratio lower bound"));
      if( answer2!="" )
	sScanF(answer2,"%e",&defectRatioLowerBound);
      printF("defectRatioLowerBound=%e\n",defectRatioLowerBound);
    }
    else if( answer=="defect ratio upper bound" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the defect ratio upper bound"));
      if( answer2!="" )
	sScanF(answer2,"%e",&defectRatioUpperBound);
      printF("defectRatioUpperBound=%e\n",defectRatioUpperBound);
    }
    else if( answer=="defect ratio lower bound for line smooths" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the defect ratio lower bound for line smooths"));
      if( answer2!="" )
	sScanF(answer2,"%e",&defectRatioLowerBoundLineSmooth);
      printF("defectRatioLowerBoundLineSmooth=%e\n",defectRatioLowerBoundLineSmooth);
    }
    else if( answer=="defect ratio upper bound for line smooths" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the defect ratio upper bound for line smooths"));
      if( answer2!="" )
	sScanF(answer2,"%e",&defectRatioUpperBoundLineSmooth);
      printF("defectRatioUpperBoundLineSmooth=%e\n",defectRatioUpperBoundLineSmooth);
    }
    else if( answer=="use locally optimal omega" || answer=="do not use locally optimal omega" )
    {
      useLocallyOptimalOmega=answer=="use locally optimal omega";
      printF("useLocallyOptimalOmega=%i\n",(int)useLocallyOptimalOmega);     
    }
    else if( answer=="use locally optimal line omega" || answer=="do not use locally optimal line omega" )
    {
      useLocallyOptimalLineOmega=answer=="use locally optimal line omega";
      printF("useLocallyOptimalLineOmega=%i\n",(int)useLocallyOptimalLineOmega);     
    }
    else if( answer=="use split step line solver" )
    {
      useSplitStepLineSolver=true;
      printF("useSplitStepLineSolver=%i\n",(int)useSplitStepLineSolver);     
    }
    else if( answer=="do not use split step line solver" )
    {
      useSplitStepLineSolver=false;
      printF("useSplitStepLineSolver=%i\n",(int)useSplitStepLineSolver);     
    }
    else if( answer=="order of extrapolation for Dirichlet on lower levels" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the order (current=%i)",orderOfExtrapolationForDirichletOnLowerLevels));
      if( answer2!="" )
	sScanF(answer2,"%i",&orderOfExtrapolationForDirichletOnLowerLevels);
      printF(" orderOfExtrapolationForDirichletOnLowerLevels=%i\n",orderOfExtrapolationForDirichletOnLowerLevels);
      
    }
    else if( answer=="number of boundary layers to smooth" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of boundary layers to smooth"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfBoundaryLayersToSmooth);
      printF("numberOfBoundaryLayersToSmooth=%i\n",numberOfBoundaryLayersToSmooth);
    } 
    else if( answer=="number of boundary smooth iterations" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of boundary smooth iterations"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfBoundarySmoothIterations);
      printF("numberOfBoundarySmoothIterations=%i\n",numberOfBoundarySmoothIterations);
    } 
    else if( answer=="number of levels for boundary smooths" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of levels for boundary smooths"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfLevelsForBoundarySmoothing);
      printF("numberOfLevelsForBoundarySmoothing=%i\n",numberOfLevelsForBoundarySmoothing);
    } 
    else if( answer=="combine smooths with IBS" )
    {
      combineSmoothsWithIBS=true;
    }
    else if( answer=="do not combine smooths with IBS" )
    {
      combineSmoothsWithIBS=false;
    }
    else if( answer=="number of interpolation layers to smooth" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of interpolation layers to smooth"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfInterpolationLayersToSmooth);
      printF("numberOfInterpolationLayersToSmooth=%i\n",numberOfInterpolationLayersToSmooth);
    } 
    else if( answer=="number of interpolation smooth iterations" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of interpolation smooth iterations"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfInterpolationSmoothIterations);
      printF("numberOfInterpolationSmoothIterations=%i\n",numberOfInterpolationSmoothIterations);
    } 
    else if( answer=="number of levels for interpolation smooths" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of levels for interpolation smooths"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfLevelsForInterpolationSmoothing);
      printF("numberOfLevelsForInterpolationSmoothing=%i\n",numberOfLevelsForInterpolationSmoothing);
    } 
    else if( answer=="number of interpolation smooth global iterations" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the number of interpolation smooth global iterations"));
      if( answer2!="" )
	sScanF(answer2,"%i",&numberOfIBSIterations);
      printF("numberOfIBSIterations=%i\n",numberOfIBSIterations);
    } 
    else if( answer=="maximum number of sub-smooths" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the maximum number of sub-smooths for point smoothers"));
      if( answer2!="" )
	sScanF(answer2,"%i",&maximumNumberOfSubSmooths);
      printF("maximumNumberOfSubSmooths=%i\n",maximumNumberOfSubSmooths);
    } 
    else if( answer=="maximum number of line sub-smooths" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter the maximum number of sub-smooths for line smoothers"));
      if( answer2!="" )
	sScanF(answer2,"%i",&maximumNumberOfLineSubSmooths);
      printF("maximumNumberOfSubSmooths=%i\n",maximumNumberOfLineSubSmooths);
    } 
//      else if( answer=="coarse grid interpolation width" ) // this is used before it can be set
//      {
//        gi.inputString(answer2,
//              sPrintF(buff,"Enter the coarse grid interpolation width (-1=use default)"));
//        if( answer2!="" )
//  	sScanF(answer2,"%i",&coarseGridInterpolationWidth);
//        printF("coarseGridInterpolationWidth=%i\n",coarseGridInterpolationWidth);
//      } 
    else if( answer=="useEquationForDirichletOnLowerLevels" )
    {
      gi.inputString(answer2,
            sPrintF(buff,"Enter useEquationForDirichletOnLowerLevels: 0=no, 1=yes, 2=rectangular only"));
      if( answer2!="" )
	sScanF(answer2,"%i",&useEquationForDirichletOnLowerLevels);
      printF("useEquationForDirichletOnLowerLevels=%i\n",useEquationForDirichletOnLowerLevels);
    }
    else if( answer=="solve equation with boundary conditions" ||
             answer=="do not equation with boundary conditions" )
    {
      solveEquationWithBoundaryConditions= answer=="solve equation with boundary conditions";
      printF("solveEquationWithBoundaryConditions=%i\n",solveEquationWithBoundaryConditions);
    }
    else if( answer=="use an F cycle" )
    {
      cycleType=cycleTypeF;
      printF("Using an F cycle...\n");
    }
    else if( answer=="forward ordering of grids in smooth" )
      gridOrderingForSmooth=0;
    else if( answer=="reverse ordering of grids in smooth" )
      gridOrderingForSmooth=1;
    else if( answer=="alternate ordering of grids in smooth" )
      gridOrderingForSmooth=2;
    else if( answer=="alternate smoothing directions" || answer=="do not alternate smoothing directions" ||
             answer=="fully alternate smoothing directions" )
    {
      alternateSmoothingDirections=(answer=="alternate smoothing directions" ? 1 :
                                   answer=="fully alternate smoothing directions" ? 2 : 0);
      printF("alternateSmoothingDirections=%i (0=no, 1=yes, 2=full)\n",alternateSmoothingDirections);
    }
    else if( answer=="use new fine to coarse BC" )
    {
      useNewFineToCoarseBC=true;
    }
    else if( answer=="use new red-black smoother" )
    {
      useNewRedBlackSmoother=true;
      printF("Use the new red-black smoother.\n");
    }
    else if( answer=="do not use new red-black smoother" )
    {
      useNewRedBlackSmoother=false;
      printF("Do not use the new red-black smoother.\n");
    }
    else if( answer=="do not use new fine to coarse BC" )
    {
      useNewFineToCoarseBC=false;
    }
    else if( answer=="save the multigrid composite grid" )
    {
      saveMultigridCompositeGrid=true;
      gi.inputString(nameOfMultigridCompositeGrid,"Enter the name of the file to save (e.g. mgcg.hdf)");
    }
    else if( answer=="read the multigrid composite grid" )
    {
      readMultigridCompositeGrid=true;
      gi.inputString(nameOfMultigridCompositeGrid,"Enter the name of the file to read (e.g. mgcg.hdf)");
    }
    else if( dialog.getToggleValue(answer,"problem is singular",problemIsSingular) ){}//
    else if( dialog.getToggleValue(answer,"project right hand side for singular problems",
                                   projectRightHandSideForSingularProblem) ){}//
    else if( dialog.getToggleValue(answer,"set mean value for singular problems",
                                   assignMeanValueForSingularProblem) ){}//
    else if( dialog.getToggleValue(answer,"adjust equations for singular problems",
                                   adjustEquationsForSingularProblem) )
    { 
      problemIsSingular=false;
    }//
    else if( (len=answer.matches("null vector option:")) )
    {
      aString cmd = answer(len,answer.length()-1);
      if( cmd=="compute" ) nullVectorOption=computeNullVector;
      else if( cmd=="computeAndSave" ) nullVectorOption=computeAndSaveNullVector;
      else if( cmd=="readOrCompute" ) nullVectorOption=readOrComputeNullVector;
      else if( cmd=="readOrComputeAndSave" ) nullVectorOption=readOrComputeAndSaveNullVector;
      else
      {
        printf("ERROR: unknown null vector option=[%s]\n",(const char*)cmd);
        nullVectorOption=computeNullVector;   
      }

      dialog.getOptionMenu("null vector option:").setCurrentChoice(nullVectorOption);
      
    }
    else if( dialog.getTextValue(answer,"null vector file name:","%s",nullVectorFileName) ){}//

    else if( answer=="null vector solve options..." )
    {
      if( nullVectorParameters==NULL )
      {
	nullVectorParameters = new OgesParameters;
      }
      nullVectorParameters->update(gi,cg);
      
    }
    else if( answer=="save coarse grid check file" )
    {
      saveGridCheckFile=true;
      printF(" ...saveGridCheckFile=true\n");
    }
    else if( answer=="set load balancing options" )
    {
      if( loadBalancer==NULL )
      {
	loadBalancer = new LoadBalancer;
	// NOTE: when there is only 1 grid then the default load-balancer will always use all-to-all
	loadBalancer->setLoadBalancer(LoadBalancer::KernighanLin);
      }
      loadBalancer->update(gi);
    }
    else if( answer=="Convergence criteria: residual converged" ||
             answer=="Convergence criteria: error estimate converged" ||
             answer=="Convergence criteria: residual converged old-way" )
    {
      convergenceCriteria = ( answer=="Convergence criteria: residual converged" ? convergenceCriteria=residualConverged :
                             answer=="Convergence criteria: error estimate converged" ? convergenceCriteria=errorEstimateConverged : 
                             convergenceCriteria=residualConvergedOldWay );
      if( convergenceCriteria==residualConverged )
      {
	printF("Ogmg:INFO: Setting convergence criteria to : l2Norm(residual) < residualTolerance*l2Nnorm(f) + absoluteTolerance\n");
      }
      else if( convergenceCriteria==errorEstimateConverged )
      {
	printF("Ogmg:INFO: Setting convergence criteria to : max(error estimate) < errorTolerance\n");
      }
      else if( convergenceCriteria==residualConvergedOldWay )
      {
        printF("Ogmg:INFO: Setting convergence criteria to : max(residual) < residualTolerance*numberOfGridPoints (OLD-WAY)\n");
      }
      
    }
    else if( dialog.getTextValue(answer,"residual tolerance","%e",residualTolerance) ){}//
    else if( dialog.getTextValue(answer,"absolute tolerance","%e",absoluteTolerance) ){}//
    else if( dialog.getTextValue(answer,"error tolerance","%e",errorTolerance) ){}//
    else if( dialog.getTextValue(answer,"sub-smooth reference grid:","%i",subSmoothReferenceGrid) )
    {
      printF("Setting the reference grid for determining variable sub-smooths to grid=%i.\n"
             " This grid will use 1 sub-smooth and should normally be a large Cartesian grid.\n"
             " Set this value to -1 to have Ogmg determine this grid automatically.\n",
	     subSmoothReferenceGrid);
    }
    else if( dialog.getTextValue(answer,"order of accuracy:","%i",orderOfAccuracy) )
    {
      if( orderOfAccuracy!=2 && orderOfAccuracy!=4 )
      {
        orderOfAccuracy =  cg[0].discretizationWidth(0)-1;
	printF("OgmgParameters::WARNING: orderOfAccuracy=%i is unexpected! Expecting 2 or 4. Setting to %i.\n",orderOfAccuracy);
      }
    }
    
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="exit" )
      break;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
  }
  

  gi.unAppendTheDefaultPrompt();  // reset
  gi.popGUI(); // restore the GUI

  return 0;
}


