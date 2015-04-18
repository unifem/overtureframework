#include "DomainSolver.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
// #include "turbulenceModels.h"


//=========================================================================================
/// \brief Allocate and initialize grid functions, based on the time-stepping method.
/// \details This function is called by DomainSolver::setup (DomainSolver.C)
//=========================================================================================
int DomainSolver::
setupGridFunctions()
{
  totalNumberOfArrays=0;   // keep a count of the number of A++ arrays; used to check for leaks
  // count total number of times steps, for statistics:
  numberOfStepsTaken=-1;    // numberOfStepsTaken==-1 : for initialization steps
  dt=0.;
  movieFrame=-1;

  assert( current==0 );
  GridFunction & solution = gf[current];
  
  CompositeGrid & cg = *solution.u.getCompositeGrid();
  const int numberOfDimensions = cg.numberOfDimensions();
  
  variableDt.redim(cg.numberOfComponentGrids());
  variableDt=0.;
  int numberOfTimeLevels=3;  // *** fix this ****
  variableTime.redim(cg.numberOfComponentGrids(),numberOfTimeLevels);
  variableTime=0.;
  
  Range all;
  CompositeGridOperators & operators = *gf[current].u.getOperators();

  //kkc 100216 !!!fix to test compact ops  operators.setOrderOfAccuracy(parameters.dbase.get<int >("orderOfAccuracy"));  // *wdh* 020901
  operators.setOrderOfAccuracy(min(4,parameters.dbase.get<int >("orderOfAccuracy")));  // *wdh* 020901
  
  updateWorkSpace(solution);

  int grid;
  
  numberOfGridFunctionsToUse=2;  // ********
  numberOfExtraFunctionsToUse=0;

  const Parameters::ImplicitMethod & implicitMethod = parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
  const int & orderOfBDF= parameters.dbase.get<int>("orderOfBDF");
  

  switch (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod"))
  {
  case Parameters::trapezoidal:
  case Parameters::midPoint:
  case Parameters::forwardEuler:
    numberOfGridFunctionsToUse=2; // *wdh* 010923 Only 2 needed,  3;
    numberOfExtraFunctionsToUse=1;
    // these are used for temporary space in the time steppers: 
    fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);  
    break;
  case Parameters::adamsBashforth2:
  case Parameters::adamsPredictorCorrector2:
  case Parameters::variableTimeStepAdamsPredictorCorrector:
    numberOfGridFunctionsToUse=2; 
    if( parameters.isMovingGridProblem() )
      numberOfGridFunctionsToUse=3;  // use one extra for moving grids *wdh* 040827

    numberOfExtraFunctionsToUse=2;
    fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);   // work space
    fn[1].updateToMatchGridFunction(solution.u); assign(fn[1],0.); 
    break;
  case Parameters::adamsPredictorCorrector4:
    numberOfGridFunctionsToUse=2; 
    if( parameters.isMovingGridProblem() )
      numberOfGridFunctionsToUse=3;  // use one extra for moving grids *wdh* 040827
    numberOfExtraFunctionsToUse=4;
    for( int m=0; m<numberOfExtraFunctionsToUse; m++ )
    {
      fn[m].updateToMatchGridFunction(solution.u); assign(fn[m],0.);   // work space
    }
    break;
  case Parameters::implicit:
  case Parameters::steadyStateNewton:

    numberOfGridFunctionsToUse=2; 

    if( implicitMethod==Parameters::backwardDifferentiationFormula )
    {
      numberOfGridFunctionsToUse=orderOfBDF+1;  // check me 
    }
    else if( parameters.isMovingGridProblem() || 
	     implicitMethod==Parameters::approximateFactorization )
      // use one extra for moving grids *wdh* 040827 and one extra of factored scheme kkc 100104
      numberOfGridFunctionsToUse=3;  

    if( implicitMethod==Parameters::backwardDifferentiationFormula )
    {  
      numberOfExtraFunctionsToUse=1; 
      fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);  // work space
    }
    else if( implicitMethod==Parameters::approximateFactorization )
    {
      numberOfExtraFunctionsToUse=2;
      fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);  // work space
      fn[1].updateToMatchGridFunction(solution.u); assign(fn[1],0.);  // work space
    }
    else
    {
      numberOfExtraFunctionsToUse=3;
      fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);  // work space
      fn[1].updateToMatchGridFunction(solution.u); assign(fn[1],0.); 
      fn[2].updateToMatchGridFunction(solution.u); assign(fn[2],0.);   // holds explicit part of implicit terms
    }
    break;
  case Parameters::steadyStateRungeKutta:
    numberOfGridFunctionsToUse=2; 
    numberOfExtraFunctionsToUse=1;
    fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);
    break;
  case Parameters::rKutta:
    numberOfGridFunctionsToUse=3; 
    numberOfExtraFunctionsToUse=2;
    fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);
    fn[1].updateToMatchGridFunction(solution.u); assign(fn[1],0.); 
    break;
  case Parameters::implicitAllSpeed:
    numberOfGridFunctionsToUse=2; 
    numberOfExtraFunctionsToUse=3;
    fn[0].updateToMatchGridFunction(solution.u); assign(fn[0],0.);  // work space
    fn[1].updateToMatchGridFunction(solution.u); assign(fn[1],0.); 
    fn[2].updateToMatchGridFunction(solution.u); assign(fn[2],0.); 

    fn[3].updateToMatchGridFunction(solution.u); assign(fn[3],0.); // for implicit time stepping

    // 010710 : (but where should this be put?)
    if( prho==NULL )
      prho = new realCompositeGridFunction;  // used as link in formAllSpeedPressureEquation


    break;
  case Parameters::adi:
    numberOfGridFunctionsToUse=1; 
    break;
  default:
    printf("DomainSolver::initialize:ERROR: unknown time stepping method %i\n",
          parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod"));
    Overture::abort();
    break;
  }


  printF(" DomainSolver::setupGridFunctions:  numberOfGridFunctionsToUse= %i, numberOfExtraFunctionsToUse=%i \n",
           numberOfGridFunctionsToUse,numberOfExtraFunctionsToUse);

  printF(" +++++DomainSolver::setupGridFunctions: cg.numberOfComponentGrids=%i ++++++++++++++\n",
         cg.numberOfComponentGrids());

  assert( numberOfGridFunctionsToUse<=maximumNumberOfGridFunctionsToUse );

  solution.u.setName("u");
  solution.setParameters(parameters);
  int c;
  for( c=0; c<parameters.dbase.get<int >("numberOfComponents"); c++ )
    solution.u.setName(parameters.dbase.get<aString* >("componentName")[c],c);

  for( int i=0; i<maximumNumberOfGridFunctionsToUse; i++ )
    gf[i].transform=NULL;

  // holds grid velocity for moving grids:
  if( parameters.isMovingGridProblem() )
  {
    // *** fix me: Ogen parameters should be saved with the CompositeGrid ****

    if( parameters.dbase.get<Ogen* >("gridGenerator")==NULL );
      parameters.dbase.get<Ogen* >("gridGenerator") = new Ogen(*parameters.dbase.get<GenericGraphicsInterface* >("ps"));

    parameters.dbase.get<Ogen* >("gridGenerator")->set(Ogen::THEimproveQualityOfInterpolation,
                                 parameters.dbase.get<bool >("improveQualityOfInterpolation"));
    if( parameters.dbase.get<real >("interpolationQualityBound")>1. )
      parameters.dbase.get<Ogen* >("gridGenerator")->set(Ogen::THEqualityBound,parameters.dbase.get<real >("interpolationQualityBound"));
    
    const real & tol = parameters.dbase.get<real >("maximumAngleDifferenceForNormalsOnSharedBoundaries");
    if( tol>=0. )
    {
      parameters.dbase.get<Ogen* >("gridGenerator")->set(Ogen::THEmaximumAngleDifferenceForNormalsOnSharedBoundaries,tol);
    }
    

    if( parameters.dbase.get<Ogshow* >("show")!=NULL )
    {
      if( debug() & 2 ) 
        printF(" ++++ ->setMovingGridProblem(true); +++\n");
      parameters.dbase.get<Ogshow* >("show")->setIsMovingGridProblem(true);
    }
    
    // CompositeGrid & cgu = (CompositeGrid &) (*u.gridCollection);
    // cgu[0].center().display("+++++ 1: Here is u.center()");

    // ** gf[0].cg.reference(cg);
  
    for( int i=1; i<numberOfGridFunctionsToUse; i++ )
      gf[i].cg=cg; 
    // holds grid velocity for moving grids:
    for( int i=0; i<numberOfGridFunctionsToUse; i++ )
    {
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	if( parameters.gridIsMoving(grid) )
	{
	  assign(gf[i].createGridVelocity(grid),0.);
	}
      }
    }
  }
  else
  {
    for( int i=1; i<numberOfGridFunctionsToUse; i++ )
      gf[i].cg.reference(cg);
  }
  
  for( int i=1; i<numberOfGridFunctionsToUse; i++ )
  {
    gf[i].updateToMatchGrid(gf[i].cg);  
  }
  

  for( int i=1; i<numberOfGridFunctionsToUse; i++ )
  {
    gf[i].u.updateToMatchGrid(gf[i].cg,nullRange,nullRange,nullRange,parameters.dbase.get<int >("numberOfComponents")); 
    gf[i].u.setOperators( *solution.u.getOperators() ); 
    assign(gf[i].u,0.);
    gf[i].t=0.;
    gf[i].setParameters(parameters);
    // set names for interactive plotting, debug output
    gf[i].u.setName("u");
    for( int c=0; c<parameters.dbase.get<int >("numberOfComponents"); c++ )
    {
      gf[i].u.setName(parameters.dbase.get<aString* >("componentName")[c],c);
    }
  }

  // -- Allocate the grid function where we save the body forcing (including any user defined forcing) ---
  if( parameters.dbase.get<bool >("turnOnBodyForcing") )
  {
    realCompositeGridFunction *&bodyForce = parameters.dbase.get<realCompositeGridFunction* >("bodyForce");
    if( bodyForce==NULL )
    {
      bodyForce= new realCompositeGridFunction;
      // we could probably save some space here as not all components will have forcing added:
      (*bodyForce).updateToMatchGridFunction(solution.u); 

      assign((*bodyForce),0.);
      
      printF("\n ++++++++++++++++ allocate bodyForce +++++++++++\n\n");
      
    }
  }
  

  if( gf[current].cg.numberOfDimensions()==1 )
    parameters.dbase.get<GraphicsParameters >("psp").set(GI_COLOUR_LINE_CONTOURS,TRUE);

  if( false )
  {
    GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
    PlotStuffParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

    gi.erase();
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    psp.set(GI_TOP_LABEL,"initialize: gf[current].cg.refinementLevel[1]");
    PlotIt::plot(gi,gf[current].cg.refinementLevel[1],psp);
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  }

  return 0;
}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateWorkSpace}} 
int DomainSolver::
updateWorkSpace(GridFunction & gf0)
//=========================================================================================
// /Description:
//    Update the workSpace.
//
//   Each grid has a work space. Work spaces may be shared between grids.
// Work spaces consist of a number of arrays, a(I1,I2,I3). For two grids to share a work space
// the lengths of I1,I2,I3 must be bigger than or equal to the grid dimensions along
// each axis. We will increase any of I1,I2,I3 to permit sharing, but only if the result
// is smaller than having two separate work spaces.
// 
// Examples:
//  a) grid 1 : (10,3), grid 2 : (3,10) --> do not share since merged size of 10x10 > 10x3 + 3x10  
//  b) grid 1 : (10,3), grid 2 : (9,4) --> do share since  since merged size of 10x4 < 10x3 + 9x4
// 
//   pWorkSpace[i] = pointer to work space i (may be NULL)
//   workSpaceIndex(grid) : index into pWorkSpace[]
//   pWorkSpace[workSpaceIndex(grid)] = work space to use for grid.
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  CompositeGrid & cg = *gf0.u.getCompositeGrid();

  int grid;
  
  return 0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{initializeSolution}} 
int DomainSolver::
initializeSolution()
//=========================================================================================
// /Description:
//    Initialize the solution, project velocity if required.
//  The initial conditions should have already been assigned in setParameters.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  if( false )
  {
    printF("**** DomainSolver::initializeSolution START\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  
  if( !twilightZoneFlow && movingGridProblem() && gf[current].t==0. )
  {
    // --- Initialize current and past time grids for moving and deforming grids ---*NEW* 2014/06/28 
    // The original grid from the file may have been changed (e.g. to match an initial deformation)
    // and we need to regenerate the grid.

    if( TRUE )
    {
      // Initialize moving grids -- put this here since deforming grids may depend on the
      // initial conditions and/or known solution *wdh* 2014/07/11: 
      parameters.dbase.get<MovingGrids >("movingGrids").assignInitialConditions( gf[current] );

      if( FALSE )
      {
	GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
	printF("*********** Grid after movingGrids.assignInitialConditions\n");
	ps.erase();
	PlotIt::plot(ps,gf[current].cg);
      }
      
      // int numberOfPast=1;
      // int previous[1]={current};  // 
      // printF("\n ---DomainSolver::initializeSolution: REGENERATE THE INITIAL OVERLAPPING GRID  ---\n\n");
      // getPastTimeSolutions( current, numberOfPast, previous  ); 
    }

    if( true )
    {
      printF("\n ---DomainSolver::initializeSolution: REGENERATE THE INITIAL OVERLAPPING GRID  ---\n\n");
      parameters.regenerateOverlappingGrid( gf[current].cg , gf[current].cg, true );
    }
    
    // Note: below the initial conditions will be interpolated and BC's applied.

    // int numberOfPast=1;
    // for( int i=0; i<
    //  getPastTimeSolutions( int current, int numberOfPast, int *previous  )

  }
      

  
  // --- Evaluate variable material properties --- *wdh* 2011/10/26 -- is this the right place
  printF("**** DomainSolver::initializeSolution setVariableMaterialProperties****\n");
  setVariableMaterialProperties( gf[current], gf[current].t );

  if( parameters.isMovingGridProblem() )
    getGridVelocity(gf[current],gf[current].t);  // needed for BC's and time step


  if( debug() & 4 )
    gf[current].u.display("Here is gf[current].u after getInitial",parameters.dbase.get<FILE* >("debugFile"),"%7.1e ");


  if( debug() & 256 )
  {
    RealArray error(20);  // *****
    fprintf(parameters.dbase.get<FILE* >("debugFile")," CGSolver:initialize: errors in gf[current] at t=%e \n",gf[current].t);
    determineErrors( gf[current].u,gf[current].gridVelocity, gf[current].t, 0, error );
  }
  
  // The initial time may be set by the initial condition routines above
  for( int i=0; i<numberOfGridFunctionsToUse; i++ )
    if( i!=current )
      gf[i].t=gf[current].t;

  
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  GenericGraphicsInterface & ps = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  bool turnOnDebugPlotting=false;
  if( turnOnDebugPlotting )
  {
    psp.set(GI_TOP_LABEL,"Solution before BC t=0");
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    PlotIt::contour(ps, gf[current].u,psp);
  }
  
  // Determine the time independent and spatially varying BC's such as the parabolic inflow BC profile
  if( parameters.bcVariesInSpace() )  
  {
    timeIndependentBoundaryConditions(gf[current]); 
  }

  if( false )
  {
    printF("**** DomainSolver::initializeSolution I\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }



  // Compute the distance to the wall for turbulence models 
  parameters.updateTurbulenceModels(gf[current].cg);

  if( debug() & 32 )
    gf[current].u.display("Initial conditions before assign BC's: u",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");

  // assign bc's 
  // applyBoundaryConditions(gf[current]);
  interpolateAndApplyBoundaryConditions(gf[current]);  // *wdh* 020523

  if( turnOnDebugPlotting )
  {
    psp.set(GI_TOP_LABEL,"Solution after BC t=0");
    psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    PlotIt::contour(ps, gf[current].u,psp);
  }

  if( debug() & 32 )
    gf[current].u.display("Initial conditions after assign BC's (I): u",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");

  if( debug () & 128 )
  {
    realCompositeGridFunction up,uc;
    gf[current].u.display(sPrintF("initializeSolution: gf[current] (form=%i): u",(int)gf[current].form),
                    parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
    up=gf[current].u;
    gf[current].primitiveToConservative();
    uc=gf[current].u;
    gf[current].u.display(sPrintF("initializeSolution: gf[current] (form=%i): u",(int)gf[current].form),
                    parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
    gf[current].conservativeToPrimitive();
    gf[current].u.display(sPrintF("initializeSolution: gf[current] (form=%i): u",(int)gf[current].form),
                    parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
    fprintf(parameters.dbase.get<FILE* >("debugFile")," **** diff in up-gf[current](primitive)= %9.2e\n",max(fabs(gf[current].u-up)));
  }
  

  // in some cases (twoBlock) we may have to interpolate and reapply the BC's to get values
  // near the boundaries correct: *wdh* 991106

  interpolateAndApplyBoundaryConditions(gf[current]);  // *wdh* 020523

  if( debug() & 32 )
    gf[current].u.display("Initial conditions after assign BC's (II): u",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");
  
    
  if( parameters.dbase.get<bool >("projectInitialConditions") )
  {
    project(gf[current]);   // project initial conditions 
  }
  else
  {
    updateToMatchGrid(gf[current].cg); 
  }
  
  initializeTurbulenceModels(gf[current]);

  if( debug() & 32 )
    gf[current].u.display("Solution after project initial conditions: u",parameters.dbase.get<FILE* >("debugFile"),"%8.5f ");


  dt= getTimeStep( gf[current] ); 
  parameters.dbase.get<real >("dt")=dt;

  return 0;
}

int DomainSolver::
initializeTurbulenceModels(GridFunction & cgf)
{

  return 0;
}
