#ifndef EQUATION_DOMAIN_SOLVER_H
#define EQUATION_DOMAIN_SOLVER_H

#include "Overture.h"
#include "GridFunction.h"
#include "OgesParameters.h"
#include "CompositeGridOperators.h"
#include "ApproximateFactorization.h"

int
checkForSymmetry(realCompositeGridFunction & u, Parameters & parameters, const aString & label,
                 int numberOfGhostLinesToCheck );


// --- forward declarations of classes --
class Ogen;
class Ogmg;
class LineSolve;
class GridFaceDescriptor;
class DialogState;
class AdvanceOptions;
class AdamsPCData;

/// ===================================================================
/// \brief Here is the base class for all domain solvers.
/// ===================================================================
class DomainSolver 
{
public:

enum
{
  defaultValue=-1234567
};

enum ForcingTypeEnum
{
  computeForcing,
  computeTimeDerivativeOfForcing
};

// Here are options for the DomainSolver::boundaryConditionPredictor
enum BoundaryConditionPredictorEnum
{
  predictPressure,
  predictPressureAndVelocity
};
  

DomainSolver(Parameters & par,
	     CompositeGrid & cg, GenericGraphicsInterface *ps=NULL, 
	     Ogshow *show=NULL, const int & plotOption=1 );



virtual ~DomainSolver();


virtual int 
adaptGrids( GridFunction & gf0, 
	    int numberOfGridFunctionsToUpdate=0,
	    realCompositeGridFunction *cgf=NULL,
	    realCompositeGridFunction *uWork =NULL );


virtual int
addArtificialDissipation( realCompositeGridFunction & u, real dt );
virtual int
addArtificialDissipation( realMappedGridFunction & u, real dt, int grid );

virtual int 
addConstraintEquation( Parameters &parameters, Oges& solver, 
		       realCompositeGridFunction &coeff, 
		       realCompositeGridFunction &ucur, 
		       realCompositeGridFunction &rhs, const int &numberOfComponents); 

virtual void 
addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, int iparam[], real rparam[],
	   realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
           realMappedGridFunction *referenceFrameVelocity=NULL);

virtual int 
addGrids();

virtual int
advance(real & tFinal );

virtual void 
advanceAdamsPredictorCorrector( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );
virtual void 
advanceAdamsPredictorCorrectorNew( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void
advanceADI( real & t, real & dt, int & numberOfSubSteps, int & init, int initialStep  );

void 
advanceForwardEulerNew( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void 
advanceImplicitMultiStep( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void 
advanceImplicitMultiStepNew( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual int 
advanceLineSolve(LineSolve & lineSolve,
		 const int grid, const int direction, 
		 realCompositeGridFunction & u0, 
		 realMappedGridFunction & f, 
		 realMappedGridFunction & residual,
		 const bool refactor,
		 const bool computeTheResidual  =false );

virtual void 
advanceMidPoint( real & t0, real & dt0, int & numberOfSubSteps, int initialStep  );

virtual void 
advanceNewton( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void
advanceSecondOrderSystem( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void 
advanceSteadyStateRungeKutta( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );


virtual void
advanceTrapezoidal( real & t0, real & dt0, int & numberOfSubSteps, int & init, int initialStep  );

virtual void
advanceVariableTimeStepAdamsPredictorCorrector( real & t0, real & dt0, int & numberOfSubSteps, int & init, 
						int initialStep  );

virtual void 
allSpeedImplicitTimeStep(GridFunction & gf,              // ** get rid of this **
			 real & t, 
			 real & dt0, 
			 int & numberOfTimeSteps,
			 const real & nextTimeToPrint );

// Here is where boundary conditions are implemented
virtual int
applyBoundaryConditions(GridFunction & cgf,
                        const int & option =-1,
                        int grid_= -1,
                        GridFunction *puOld=NULL, 
                        const real & dt =-1. );

virtual int 
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			realMappedGridFunction & gridVelocity,
			const int & grid,
			const int & option=-1,
			realMappedGridFunction *puOld=NULL, 
			realMappedGridFunction *pGridVelocityOld=NULL,
			const real & dt=-1.);

virtual int
applyBoundaryConditionsForImplicitTimeStepping(GridFunction & cgf );

virtual int
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & u, 
                                               realMappedGridFunction &uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid );
virtual int
assignInitialConditions(int gfIndex);

// kkc 101005, added a function to compute the filter
virtual int applyFilter(int gfIndex);

virtual int 
assignInterfaceBoundaryConditions(GridFunction & cgf, 
				  const int & option=-1,
				  int grid_ =-1,
				  GridFunction *puOld =NULL, 
				  const real & dt=-1.);
  
// virtual void 
// assignStandardInitialConditions( GridFunction & cgf );

virtual void assignTestProblem( GridFunction & gf );

virtual void 
bodyForcingCleanup();

virtual int 
boundaryConditionPredictor( const BoundaryConditionPredictorEnum bcpOption,
                            const AdamsPCData & adamsData, 
                            const int orderOfExtrapolation,
                            const int mNew, 
                            const int mCur, 
                            const int mOld, 
                            realCompositeGridFunction *puga=NULL, 
                            realCompositeGridFunction *pugb=NULL, 
                            realCompositeGridFunction *pugc=NULL, 
                            realCompositeGridFunction *pugd=NULL );

virtual int
buildAdaptiveGridOptionsDialog(DialogData & dialog );

virtual int 
buildAmrGridsForInitialConditions();

virtual int
buildForcingOptionsDialog(DialogData & dialog );

virtual int
buildGeneralOptionsDialog(DialogData & dialog );

virtual int 
buildGrid( Mapping *&newMapping, int newGridNumber, IntegerArray & sharedBoundaryCondition );

virtual int 
buildOutputOptionsDialog(DialogData & dialog );

virtual void 
buildImplicitSolvers(CompositeGrid & cg);

virtual int
buildMovingGridOptionsDialog(DialogData & dialog );

virtual int 
buildPlotOptionsDialog(DialogData & dialog );


virtual int
buildRunTimeDialog();

virtual int
buildTimeSteppingDialog(DialogData & dialog );


static void checkArrays(const aString & label);

int checkProbes();

void
checkSolution(const realGridCollectionFunction & u, const aString & title, bool printResults=false );

int 
checkSolution(realMappedGridFunction & u, const aString & title, bool printResults, int grid,
              real & maxVal, bool printResultsOnFailure=false );

virtual void
cleanupInitialConditions();

virtual int 
computeBodyForcing( GridFunction *gfa, int *gfIndex, real *times, int numberOfTimeLevels, const real & tForce );

// *OLD WAY*
virtual int
computeBodyForcing( GridFunction & gf, const real & tForce );

virtual void 
computeNumberOfStepsAndAdjustTheTimeStep(const real & t,
                                         const real & tFinal,
					 const real & nextTimeToPrint, 
					 int & numberOfSubSteps, 
					 real & dtNew,
                                         const bool & adjustTimeStep = true );

virtual void
correctMovingGrids(const real t1,
                   const real t2, 
                   GridFunction & cgf1,
		   GridFunction & cgf2 );

const int & debug() const { return parameters.dbase.get<int>("debug");}


virtual void 
determineErrors(GridFunction & cgf,
		const aString & label =nullString );

virtual void
determineErrors(realCompositeGridFunction & u,
		realMappedGridFunction **gridVelocity,
		const real & t, 
		const int options,
                RealArray & err,
		const aString & label =nullString );

// determine errors if the true solution is known
virtual void
determineErrors(realMappedGridFunction & v, const real & t);

virtual void
displayBoundaryConditions(FILE *file = stdout);

virtual int
displayParameters(FILE *file = stdout );

virtual int
endTimeStep( real & t0, real & dt0, AdvanceOptions & advanceOptions );

//  different implementations of endTimeStep:
virtual int
endTimeStepAF( real & t0, real & dt0, AdvanceOptions & advanceOptions );
virtual int
endTimeStepBDF( real & t0, real & dt0, AdvanceOptions & advanceOptions );
virtual int
endTimeStepFE( real & t0, real & dt0, AdvanceOptions & advanceOptions );
virtual int
endTimeStepIM( real & t0, real & dt0, AdvanceOptions & advanceOptions );
virtual int
endTimeStepPC( real & t0, real & dt0, AdvanceOptions & advanceOptions );


virtual void 
eulerStep(const real & t1, const real & t2, const real & t3, const real & dt0,
	  GridFunction & cgf1,  
	  GridFunction & cgf2,  
	  GridFunction & cgf3,  
	  realCompositeGridFunction & ut, 
	  realCompositeGridFunction & uti,
	  int stepNumber,
	  int & numberOfSubSteps );

void 
extrapolateInterpolationNeighbours( GridFunction & gf, const Range & C );

virtual int
fixupUnusedPoints( realCompositeGridFunction & u );

virtual void 
formMatrixForImplicitSolve(const real & dt0,
			   GridFunction & cgf1,
			   GridFunction & cgf0 );

// virtual int 
// formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
// 			       const real & dt0, 
// 			       int scalarSystem,
// 			       realMappedGridFunction & uL,
// 			       const int & grid );


virtual int 
getAmrErrorFunction(realCompositeGridFunction & u, 
                    real t,
                    intCompositeGridFunction & errorFlag,
                    realCompositeGridFunction & error );

virtual int 
getAmrErrorFunction(realCompositeGridFunction & u, 
                    real t,
                    realCompositeGridFunction & error,
                    bool computeOnFinestLevel=false );

virtual realCompositeGridFunction & 
getAugmentedSolution( GridFunction & gf0, realCompositeGridFunction & v );

static int
getBounds(const realCompositeGridFunction & u, 
	  RealArray & uMin,
	  RealArray & uMax, 
	  real & uvMax);

const aString & 
getClassName() const;

virtual int 
getAdaptiveGridOption(const aString & answer, DialogData & dialog );

virtual int 
getForcingOption(const aString & command, DialogData & dialog );

virtual int 
getGeneralOption(const aString & answer, DialogData & dialog );

virtual void 
getGridInfo( real & totalNumberOfGridPoints, 
	     real dsMin[3], real dsAve[3], real dsMax[3], 
	     real & maxMax, real & maxMin, real & minMin );

virtual void
getGridVelocity( GridFunction & gf0, const real & tGV );


virtual int
getInitialConditions(const aString & command = nullString,
		     DialogData *interface =NULL,
                     GUIState *guiState = NULL,
                     DialogState *dialogState = NULL );

// Return the list of interface data needed by a given interface:
virtual int
getInterfaceDataOptions( GridFaceDescriptor & info, int & interfaceDataOptions ) const;


int
getMaterialProperties( GridFunction & solution, realCompositeGridFunction & matProp );

real 
getMovingGridMaximumRelativeCorrection();

bool 
getMovingGridCorrectionHasConverged();

virtual int 
getMovingGridOption(const aString & answer, DialogData & dialog );

// name of the DomainSolver (e.g. Solid)
const aString & 
getName() const;

static int
getOriginalBoundaryConditions(CompositeGrid & cg, IntegerArray & originalBoundaryCondition );

virtual int 
getOutputOption(const aString & command, DialogData & dialog );

virtual int
getPastTimeSolutions( int current, int numberOfPast, int *previous );

// name of the PDE (e.g. advection-diffusion)
const aString & 
getPdeName() const;

virtual int 
getPlotOption(const aString & answer, DialogData & dialog );

virtual int
getResidual( real t, real dt, GridFunction & cgf, realCompositeGridFunction & residual);

int 
getResidualInfo( real t0, const realCompositeGridFunction & residual, real & maximumResidual, real & maximuml2, FILE *file=NULL );

virtual void
getSolutionBounds(const realMappedGridFunction & u, realArray & uMin, realArray & uMax, real & uvMax);


virtual int 
getTimeDependentBoundaryConditions( MappedGrid & mg,
				    real t, 
				    int grid = 0, 
				    int side0  = -1,
				    int axis0  = -1,
				    ForcingTypeEnum forcingType =computeForcing );

virtual int 
getTimeDerivativeOfBoundaryValues(GridFunction & gf0,
                                  const real & t, 
				  const int & grid,
				  int side=-1,
				  int axis=-1 );


//       ===Choose time step====
virtual real 
getTimeStep( GridFunction & gf); 

// determine the time step based on a given solution
virtual real
getTimeStep(MappedGrid & mg,
	    realMappedGridFunction & u, 
	    realMappedGridFunction & gridVelocity,
	    const Parameters::TimeSteppingMethod & timeSteppingMethod,
	    const int & grid  );

// Re-compute the time-step and number of sub-steps to take
int 
getTimeStepAndNumberOfSubSteps( GridFunction & cgf, int stepNumber, int & numberOfSubSteps, real & dt );

// semi-discrete discretization. Lambda is used to determine the time step by requiring
// lambda*dt to be in the stability region of the particular time stepping method we are
// using
virtual void
getTimeSteppingEigenvalue(MappedGrid & mg, 
			  realMappedGridFunction & u, 
			  realMappedGridFunction & gridVelocity,  
			  real & reLambda,
			  real & imLambda, 
			  const int & grid);

virtual int 
getTimeSteppingOption(const aString & command,
		      DialogData & dialog ) ;

virtual void 
getUt( GridFunction & cgf, 
       const real & t, 
       RealCompositeGridFunction & ut, 
       real tForce );

virtual int
getUt(const realMappedGridFunction & v, 
      const realMappedGridFunction & gridVelocity, 
      realMappedGridFunction & dvdt, 
      int iparam[], real rparam[],
      realMappedGridFunction & dvdtImplicit = Overture::nullRealMappedGridFunction(),
      MappedGrid *pmg2=NULL,
      const realMappedGridFunction *pGridVelocity2= NULL);


virtual void 
implicitSolve(const real & dt0,
	      GridFunction & cgf1,
              GridFunction & cgf0);


virtual int 
initializeInterfaces(GridFunction & cgf);

virtual int
initializeSolution();

virtual int 
initializeTimeStepping( real & t0, real & dt0 );

//  different implementations of initializeTimeStepping: 
virtual int 
initializeTimeSteppingAF( real & t0, real & dt0 );
virtual int 
initializeTimeSteppingBDF( real & t0, real & dt0 );
virtual int 
initializeTimeSteppingFE( real & t0, real & dt0 );
virtual int 
initializeTimeSteppingIM( real & t0, real & dt0 );
virtual int 
initializeTimeSteppingPC( real & t0, real & dt0 );


virtual int 
initializeTurbulenceModels(GridFunction & cgf);

enum InterfaceOptionsEnum
{
  getInterfaceRightHandSide,
  setInterfaceRightHandSide
};

virtual int
interfaceRightHandSide( InterfaceOptionsEnum option, 
                        int interfaceDataOptions,
                        GridFaceDescriptor & info, 
                        GridFaceDescriptor & gfd,
			int gfIndex, real t );

virtual int 
interpolate( GridFunction & cgf, const Range & R = nullRange );

virtual int 
interpolateAndApplyBoundaryConditions( GridFunction & cgf,
                                       GridFunction *uOld =NULL, 
                                       const real & dt =-1. );

virtual bool 
isImplicitMatrixSingular( realCompositeGridFunction &uL );


virtual int
jetInflow(GridFunction & cgf );

virtual void
moveGrids(const real & t1, 
	  const real & t2, 
	  const real & t3,
	  const real & dt0,
	  GridFunction & cgf1,  
	  GridFunction & cgf2,
	  GridFunction & cgf3 );

virtual bool 
movingGridProblem() const { return parameters.isMovingGridProblem();}


virtual int
newAdaptiveGridBuilt(CompositeGrid & cg, realCompositeGridFunction & u, bool updateSolution );

const int & numberOfComponents() const { return parameters.dbase.get<int>("numberOfComponents");}

virtual int
output( GridFunction & gf0, int stepNumber );

virtual void 
outputHeader();

virtual int 
outputProbes( GridFunction & gf0, int stepNumber, std::vector<real> *regionProbeValues=NULL );

virtual void 
outputSolution( realCompositeGridFunction & u, const real & t,
		const aString & label =nullString,
                int printOption = 0 );

virtual void
outputSolution( const realMappedGridFunction & u, const real & t );

virtual int 
parabolicInflow(GridFunction & cgf );

virtual int
plot(const real & t, const int & optionIn, real & tFinal, int solutionToPlot=-1 );


virtual int
predictTimeIndependentVariables( const int numberOfTimeLevels, const int *gfIndex );

virtual int 
printMemoryUsage(FILE *file = stdout );

// printF with the DomainSolver name as a prefix (to identify the output).
void 
printP(const char *format, ...) const;

virtual int 
printStatistics(FILE *file = stdout );              // print timing statistics

virtual void 
printTimeStepInfo( const int & step, const real & t, const real & cpuTime );

virtual int
project(GridFunction & cgf);

virtual int
projectInitialConditionsForMovingGrids(int gfIndex);

virtual int 
readRestartFile(realCompositeGridFunction & v,   // this version came from CgSolver
		real & t,
		const aString & restartFileName =nullString );

virtual int
readRestartFile(GridFunction & cgf, const aString & restartFileName =nullString );

virtual int
saveSequenceInfo( real t0, const realCompositeGridFunction & residual );

virtual int
saveSequencesToShowFile();

virtual int 
saveRestartFile(const GridFunction & cgf, const aString & restartFileName  );

virtual void
saveShow( GridFunction & gf0 );

virtual void
saveShowFileComments( Ogshow &show );

virtual int
setBoundaryConditionsInteractively(const aString & answer,
				   const IntegerArray & originalBoundaryCondition );

virtual int
setDefaultDataForBoundaryConditions();

virtual int 
setFinalTime(const real & tFinal);

virtual void
setInterfacesAtPastTimes(const real & t1, 
			 const real & t2, 
			 const real & t3,
			 const real & dt0,
			 GridFunction & cgf1,  
			 GridFunction & cgf2,
			 GridFunction & cgf3 );

virtual int
setInterfaceBoundaryCondition( GridFaceDescriptor & info );


void 
setName(const aString & name );

void
setNameOfGridFile( const aString & name ); 

virtual
int setOgesBoundaryConditions( GridFunction &cgf, IntegerArray & boundaryConditions, RealArray &boundaryConditionData,
                               const int imp );

virtual int
setParametersInteractively(bool runSetupOnExit=true);

virtual int
setPlotTitle(const real &t, const real &dt);

virtual void
setSensitivity( GUIState & dialog, 
                bool trueOrFalse );

virtual int 
setSolverParameters(const aString & command = nullString,
		    DialogData *interface =NULL );

virtual void 
setup(const real & time = 0.);

virtual int 
setupGridFunctions();

virtual int 
setupPde(aString & reactionName, bool restartChosen, IntegerArray & originalBoundaryCondition);

virtual int 
setupUserDefinedForcing();

virtual int 
setupUserDefinedInitialConditions();

virtual int 
setupUserDefinedMaterialProperties();

virtual int
setVariableBoundaryValues(const real & t, 
			  GridFunction & gf0,
			  const int & grid,
			  int side0 = -1,
			  int axis0 = -1,
			  ForcingTypeEnum forcingType=computeForcing );

virtual int 
setVariableMaterialProperties( GridFunction & gf, const real & t );

virtual real 
sizeOf(FILE *file = NULL ) const;

virtual void 
smoothVelocity(GridFunction & cgf,const int numberOfSmooths );

virtual int
solve();

virtual void  
solveForTimeIndependentVariables( GridFunction & cgf, bool updateSolutionDependentEquations=false );

virtual int 
startTimeStep( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );

//  different implementations of startTimeStep: 
virtual int 
startTimeStepAF( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );
virtual int 
startTimeStepBDF( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );
virtual int 
startTimeStepFE( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );
virtual int 
startTimeStepIM( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );
virtual int 
startTimeStepPC( real & t0, real & dt0, int & currentGF, int & nextGF, AdvanceOptions & advanceOptions );

// 
virtual void 
takeOneStep( real & t, real & dt, int stepNumber, int & numberOfSubSteps );

// 
virtual int 
takeTimeStep( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );

//  different implementations of takeTimeStep: 
virtual int 
takeTimeStepAF( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );
virtual int 
takeTimeStepBDF( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );
virtual int 
takeTimeStepFE( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );
virtual int 
takeTimeStepIM( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );
virtual int 
takeTimeStepPC( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );

// routine called by takeTimeStepBDF
virtual int 
implicitTimeStep( real & t0, real & dt0, int correction, AdvanceOptions & advanceOptions );


virtual int 
timeIndependentBoundaryConditions( GridFunction & cgf );

virtual void initializeFactorization();

virtual int
tracking( GridFunction & gf0, int stepNumber  );

const bool & twilightZoneFlow() const { return parameters.dbase.get<bool>("twilightZoneFlow");}


virtual int
userDefinedBoundaryValues(const real & t, 
			  GridFunction & gf0,
			  const int & grid,
			  int side0 = -1,
			  int axis0 = -1,
			  ForcingTypeEnum forcingType=computeForcing );

void virtual 
userDefinedCleanup();

virtual int
userDefinedGrid( GridFunction & gfct,  
                 Mapping *&newMapping, 
                 int newGridNumber, 
                 IntegerArray & sharedBoundaryCondition );

virtual int
userDefinedForcing( realCompositeGridFunction & f, GridFunction *gfa, int *gfIndex, real *times, 
                    int numberOfTimeLevels, const real & tForce );

virtual void 
userDefinedForcingCleanup();

virtual int
userDefinedInitialConditions(CompositeGrid & cg, realCompositeGridFunction & u );

virtual void 
userDefinedInitialConditionsCleanup();

virtual int
userDefinedMaterialProperties(GridFunction & gf );

virtual void 
userDefinedMaterialPropertiesCleanup();

virtual int
userDefinedOutput( GridFunction & gf, int stepNumber  );

virtual int
updateForAdaptiveGrids(CompositeGrid & cg);

virtual int
updateForMovingGrids(GridFunction & cgf);

virtual int
updateForNewTimeStep(GridFunction & gf, const real & dt );

virtual int 
updateGeometryArrays(GridFunction & cgf);

virtual int
updateToMatchGrid(CompositeGrid & cg);

virtual int 
updateToMatchNewGrid(CompositeGrid & cgNew,  
                     IntegerArray & changes,
                     IntegerArray & sharedBoundaryCondition,
                     GridFunction & gf0 );

virtual int
updateStateVariables(GridFunction & cgf, int stage=-1 );

virtual void 
updateTimeIndependentVariables(CompositeGrid & cg0, GridFunction & cgf );

virtual int
updateVariableTimeInterpolation( int newGrid, GridFunction & cgf );

virtual int
updateWorkSpace(GridFunction & gf0);


virtual int 
variableTimeStepBoundaryInterpolation( int grid, GridFunction & cgf );

virtual
void writeParameterSummary( FILE * file );

// protected:


  CompositeGrid & cg;         
  // These next objects hold parameters for the two different Oges solvers that may be used.
  OgesParameters pressureSolverParameters,          // for the pressure solve, INS or ASF
    implicitTimeStepSolverParameters; 
  CompositeGridOperators finiteDifferenceOperators;


enum Dimensions
{
  maximumNumberOfGridFunctionsToUse=5,
  maximumNumberOfExtraFunctionsToUse=5
};    

real dt;
int numberOfStepsTaken;  // count total number of times steps, for statistics
  
Parameters & parameters;

GridFunction gf[maximumNumberOfGridFunctionsToUse];  // grid functions for time stepping
int current;  // gf[current] is the current solution

// int globalStepNumber; 
  
int movieFrame;
aString movieFileName;

int numberOfGridFunctionsToUse;   // number of entries in gf[] that we use
int numberOfExtraFunctionsToUse;  // number of entries in fn[] that we use
static int totalNumberOfArrays; // keep a count of the number of A++ arrays; used to check for leaks
realCompositeGridFunction fn[maximumNumberOfExtraFunctionsToUse];  // work arrays for time stepping

// These next arrays are for variable time stepping
RealArray variableDt;  // holds variable dt for each grid.
RealArray variableTime;  // holds times for different grids
realArray *ui;           // holds values on interpolation points at different time levels 
RealArray tv, tvb, tv0;


enum ChangesEnum
{
  gridWasAdded=1,
  gridWasChanged,
  gridWasRemoved,
  refinementWasAdded,
  refinementWasRemoved
};


realCompositeGridFunction coeff;

int numberOfImplicitSolvers;
Oges *implicitSolver;                      // array of implicit solvers
realCompositeGridFunction *implicitCoeff;  // coeff matricies for implicit solvers

Oges *poisson;  // for pressure solve

realCompositeGridFunction pressureRightHandSide;
realCompositeGridFunction poissonCoefficients;
  
realCompositeGridFunction & p() const{ return *pp;} // 
realCompositeGridFunction & px() const{ return *ppx;} // 
realCompositeGridFunction & rL() const{ return *prL;} // 
realCompositeGridFunction & pL() const{ return *ppL;} // 
realCompositeGridFunction & rho() const{ return *prho;} // 
realCompositeGridFunction & gam() const{ return *pgam;} // 

realCompositeGridFunction *pp,*ppx;  // for all-speed
realCompositeGridFunction *prL, *ppL, *prho;  // for all speed
realCompositeGridFunction *pgam;  // variable gamma for reactions
realCompositeGridFunction *pvIMS, *pwIMS;  // for implicit time stepping solve
realCompositeGridFunction *previousPressure; // for INS
realCompositeGridFunction *puLinearized;  // holds linearized solution for implicit time-stepping
realMappedGridFunction *pGridVelocityLinearized;  // holds linearized grid-velocity for implicit time-stepping

LineSolve *pLineSolve;  // holds line solver 

bool gridHasMaterialInterfaces;


realCompositeGridFunction *pdtVar;  // for local time stepping

std::vector<real> hMin,hMax;
std::vector<real> numberOfGridPoints;
std::vector<real> dtv;   // dt on each grid

std::vector<real> realPartOfEigenvalue;
std::vector<real> imaginaryPartOfEigenvalue;

int numberSavedToShowFile; // counts number of solutions saved to the show file 

CG_ApproximateFactorization::FactorList factors; // if there was an "Advance" class it would go there...

std::vector<DomainSolver*> domainSolver;  // For Cgmp: holds PDE solvers for each domain 

protected:

aString name;       // a name given to this object (e.g. "fluid" or "gas")
aString className;  // class name of this or a derived class
aString pdeName;    // name of the PDE being solved (e.q. "incompressible Navier Stokes")

int restartNumber;

// pointers to gui dialogs:
DialogData *pUniformFlowDialog,*pStepFunctionDialog,*pShowFileDialog,*pTzOptionsDialog;
// for the run-time dialog:
int chooseAComponentMenuItem, numberOfPushButtons, numberOfTextBoxes;
int itemsToPlot;

private:


friend class CgSolver;

};

#endif
