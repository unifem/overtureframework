// ========================================================================================================
/// \class Parameters
/// \brief A class to hold PDE parameters for a DomainSolver.
/// \details This base class object holds generic parameters. 
///
//============================================================================================================



// \var RealArray userBoundaryConditionParameters
//    \brief an array to hold parameters for user defined boundary conditions.
// \var RealArray timeSequence
//    \brief array to hold time sequences such as the residual norm over time.
// ========================================================================================================

#include "Parameters.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "OGPulseFunction.h"
#include "Ogshow.h"
#include "PlotStuff.h"
#include "conversion.h"
#include "FileOutput.h"
#include "Regrid.h"
#include "InterpolateRefinements.h"
#include "ErrorEstimator.h"
#include "Ogen.h"
#include "HDF_DataBase.h"
#include "Reactions.h"
#include "Insbc4WorkSpace.h"
#include "FlowSolutions.h"
#include "GridFunction.h"

#include "EquationDomain.h"
#include "SurfaceEquation.h"

#include "GenericGraphicsInterface.h"
#include "ParallelUtility.h"

#include "ProbeInfo.h"
#include "ShowFileReader.h"
#include "InterpolatePointsOnAGrid.h"

#include "GridFunctionFilter.h"

#include "CompositeGridOperators.h"
#include "ExternalBoundaryData.h"
#include "BodyForce.h"

aString Parameters::timeSteppingName[Parameters::numberOfTimeSteppingMethods+1];
aString Parameters::turbulenceModelName[Parameters::numberOfTurbulenceModels+1];

real Parameters::spalartAllmarasScaleFactor=1.;
real Parameters::spalartAllmarasDistanceScale=1.e-10;

int Parameters::checkForFloatingPointErrors=0;  // =1 : check for floating point errors such as nan's and inf's

class DomainSolver; // forward declaration

//===================================================================================
//\begin{>ParametersInclude.tex}{\subsection{Variables in Parameters}} 
//\no function header:
//
// /int numberOfDimensions: number of spacial dimensions.
// /PDE pde: one of
//
// /int numberOfComponents: number of components in the equations. 
// /real cfl, cflMin, cflOpt, cflMax: parameters to determine the time step.
// /int rc: if rc$>$0 then the density is {\tt u(all,all,all,rc)}.
// /int uc: if uc$>$0 then the x component of the velocity is {\tt u(all,all,all,uc)}.
// /int vc: if vc$>$0 then the y component of the velocity is {\tt u(all,all,all,vc)}.
// /int wc: if wc$>$0 then the z component of the velocity is {\tt u(all,all,all,wc)}.
// /int pc: if pc$>$0 then the pressure is {\tt u(all,all,all,pc)}.
// /int tc:   temperature
// /int sc:   position of first species, species m is located at sc+m
// /int kc, epsc: for k-epsilon model
//
// /real machNumber, reynoldsNumber, prandtlNumber: PDE parameters CNS and ASF
// /real mu, kThermal, Rg, gamma, avr, anu: for CNS, ASF
// /real pressureLevel, nuRho: for ASF
//
// 
// /enum TurbulenceModel turbulenceModel: One of
//  \begin{verbatim}
//   enum TurbulenceModel
//   {
//     noTurbulenceModel,
//     BaldwinLomax,
//     kEpsilon,
//     kOmega,
//     SpalartAllmaras,
//     numberOfTurbulenceModels
//   };
// \end{verbatim}
//
// {\bf Boundary condition parameters:}
// /IntegerArray bcInfo(0:2,side,axis,grid): Array holding info about the parameters. The values are accessed
//    through member functions {\tt bcType(side,axis,grid)}, {\tt variableBoundaryData(side,axis,grid)}
//    and {\tt bcIsTimeDependent(side,axis,grid)}
//   \begin{description}
//      \item[bcInfo(0,side,axis,grid)] : values from enum BoundaryConditionType, e.g. uniformInflow
//      \item[bcInfo(1,side,axis,grid)] : bit flag, bit 1=BC is spatially dependent, bit 2=BC is time dependent.
//      \item[bcInfo(2,side,axis,grid)] : keys into the map bcModifiers that specify the kind modified bc to use (i.e. penalty methods, wall models, etc)
//   \end{description}
// /RealArray bcData: bcData(.,side,axis,grid) : data for the boundary condition
//
// % IntegerArray bcType  bcType(side,axis,grid) : values from enum BoundaryConditionType
// /real inflowPressure:
// /RealArray bcParameters:  arrays for boundary condition parameters
// /IntegerArray variableBoundaryData: variableBoundaryData(grid) is true if variable BC data is required.
//
//\end{ParametersInclude.tex}
//===================================================================================



// ===================================================================================================================
/// \brief constructor.
/// \param numberOfDimensions0 (input) : number of space dimensions for the problem.
///
// ===================================================================================================================
Parameters::
Parameters(const int & numberOfDimensions0) : pdeName("unknown"), numberOfBCNames(0)
{


/// \anchor dbaseDescription
/// The \b dbase member data is a DataBase that contains most parameters in this class. By creating parameters
/// dynamically (rather than declaring them directly in the class) it is easier to add new variables and easier to 
/// automatically save the values to a file. 
/// 
/// Here is a list of values in the \b dbase (classes that derive from Parameters will define more entries):

  // add dbase entries in alphabetical order: 

  /// \li <b> allowUserDefinedOutput (int) </b> : if true call the userDefinedOutput function.
  if (!dbase.has_key("allowUserDefinedOutput")) dbase.put<int>("allowUserDefinedOutput");
  /// \li <b> detectCollisions (bool) </b> : if true detect collisions between bodies.
  if (!dbase.has_key("detectCollisions")) dbase.put<bool>("detectCollisions");

  /// \li <b> globalStepNumber (int) </b> : global step number for time-stepping or iterations
  if (!dbase.has_key("globalStepNumber")) dbase.put<int>("globalStepNumber",-1);

  /// \li <b> outputStepNumber (int) </b> : holds the step number of the last time DomainSolver::output was called (to avoid multiple calls)
  if (!dbase.has_key("outputStepNumber")) dbase.put<int>("outputStepNumber",-2);

  /// \li <b> showResdiuals (int) </b> : if true show the residuals
  if (!dbase.has_key("showResiduals")) dbase.put<int>("showResiduals");
  /// \li <b> pResidual (realCompositeGridFunction*) </b> : pointer to the current residual grid function (if not NULL)
  if (!dbase.has_key("pResidual")) dbase.put<realCompositeGridFunction*>("pResidual")=NULL;
  /// \li <b> statistics (RealArray) </b> : an array to hold statistics.
  if (!dbase.has_key("statistics")) dbase.put<RealArray>("statistics");
  /// \li <b> timeSequence (RealArray) </b> : array to hold time sequences such as the residual norm over time.
  if (!dbase.has_key("timeSequence")) dbase.put<RealArray>("timeSequence");
  /// \li <b> twilightZoneFlow (bool) </b> : true if we are testing solutions with twilightzone flow.
  if (!dbase.has_key("twilightZoneFlow")) dbase.put<bool>("twilightZoneFlow");
  // \li <b> userBoundaryConditionParameters (RealArray) </b> : an array to hold parameters for user defined boundary conditions.
  if (!dbase.has_key("userBoundaryConditionParameters")) dbase.put<RealArray>("userBoundaryConditionParameters");

// \li <b>  </b> :
// \li <b>  </b> :
// \li <b>  </b> :
// \li <b>  </b> :

  /// \li <b> pdeParameters (int) </b> : sub-directory of user defined pde parameters.
  if (!dbase.has_key("PdeParameters")) dbase.put<DataBase>("PdeParameters");

  // nameOfGridFile = the name of the file from which the overlapping grid was read (if any).
  if (!dbase.has_key("nameOfGridFile")) dbase.put<aString>("nameOfGridFile","");


  // these next should go in CnsParameters:

  /// \li <b> reciprocalActivationEnergyB (real) </b> : parameter for the ignition and growth model.
  if (!dbase.has_key("reciprocalActivationEnergyB")) dbase.put<real>("reciprocalActivationEnergyB");


  if (!dbase.has_key("a2")) dbase.put<real>("a2");
  if (!dbase.has_key("pSurfaceEquation")) dbase.put<SurfaceEquation*>("pSurfaceEquation");
  if (!dbase.has_key("ad41n")) dbase.put<real>("ad41n");
  if (!dbase.has_key("showVariableName")) dbase.put<aString*>("showVariableName");
  if (!dbase.has_key("betaK")) dbase.put<real>("betaK");
  if (!dbase.has_key("numberOfSolvesForConstraints")) dbase.put<int>("numberOfSolvesForConstraints");
  if (!dbase.has_key("useCharacteristicInterpolation")) dbase.put<bool>("useCharacteristicInterpolation");
  if (!dbase.has_key("orderOfExtrapolationForOutflow")) dbase.put<int>("orderOfExtrapolationForOutflow");
  if (!dbase.has_key("tc")) dbase.put<int>("tc");
  if (!dbase.has_key("numberOfIterationsForConstraints")) dbase.put<int>("numberOfIterationsForConstraints");
  if (!dbase.has_key("extrapolateInterpolationNeighbours")) dbase.put<int>("extrapolateInterpolationNeighbours");
  if (!dbase.has_key("interpolationType")) dbase.put<Parameters::InterpolationTypeEnum>("interpolationType");
  if (!dbase.has_key("nuPassiveScalar")) dbase.put<real>("nuPassiveScalar");
  if (!dbase.has_key("epsc")) dbase.put<int>("epsc");
  if (!dbase.has_key("gravity")) dbase.put<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  if (!dbase.has_key("dampingDt")) dbase.put<real>("dampingDt");
  if (!dbase.has_key("outputFormat")) dbase.put<aString>("outputFormat");
  if (!dbase.has_key("numberOfSpecies")) dbase.put<int>("numberOfSpecies",0);
  if (!dbase.has_key("twilightZoneChoice")) dbase.put<Parameters::TwilightZoneChoice>("twilightZoneChoice");
  if (!dbase.has_key("divDampingImplicitTimeStepReductionFactor")) dbase.put<real>("divDampingImplicitTimeStepReductionFactor");
  if (!dbase.has_key("nuRho")) dbase.put<real>("nuRho");

  // predictedPressureNeeded: sometimes we need a predicted value for the pressure at the new time which can
  //  be obtained by extrapolation in time.
  if (!dbase.has_key("predictedPressureNeeded")) dbase.put<bool>("predictedPressureNeeded")=false;
  if (!dbase.has_key("orderOfTimeExtrapolationForPressure")) dbase.put<int>("orderOfTimeExtrapolationForPressure");

  if (!dbase.has_key("reducedInterpolationWidth")) dbase.put<int>("reducedInterpolationWidth");
  if (!dbase.has_key("bcInfo")) dbase.put<IntegerArray>("bcInfo");
  if (!dbase.has_key("bcData")) dbase.put<RealArray>("bcData");
  if (!dbase.has_key("wc")) dbase.put<int>("wc");
  if (!dbase.has_key("a1")) dbase.put<real>("a1");
  if (!dbase.has_key("extrapolationOption")) dbase.put<BoundaryConditionParameters::ExtrapolationOptionEnum>("extrapolationOption");
  if (!dbase.has_key("initializeImplicitTimeStepping")) dbase.put<int>("initializeImplicitTimeStepping");
  if (!dbase.has_key("compare3Dto2D")) dbase.put<int>("compare3Dto2D");
  if (!dbase.has_key("machNumber")) dbase.put<real>("machNumber");
  if (!dbase.has_key("anu")) dbase.put<real>("anu");
  if (!dbase.has_key("reciprocalActivationEnergy")) dbase.put<real>("reciprocalActivationEnergy");
  if (!dbase.has_key("trackingFrequency")) dbase.put<int>("trackingFrequency");
  if (!dbase.has_key("tzDegreeTime")) dbase.put<int>("tzDegreeTime");
  if (!dbase.has_key("te0")) dbase.put<real>("te0");
  if (!dbase.has_key("restartFileName")) dbase.put<aString>("restartFileName");

  if (!dbase.has_key("timeStepType")) dbase.put<IntegerArray>("timeStepType");
  if (!dbase.has_key("orderOfExtrapolationForSecondGhostLine")) dbase.put<int>("orderOfExtrapolationForSecondGhostLine");
  if (!dbase.has_key("prandtlNumber")) dbase.put<real>("prandtlNumber");
  if (!dbase.has_key("truncationErrorCoefficient")) dbase.put<real>("truncationErrorCoefficient");


  if (!dbase.has_key("minimumNumberOfGrids")) dbase.put<real>("minimumNumberOfGrids");
  if (!dbase.has_key("maximumNumberOfGrids")) dbase.put<real>("maximumNumberOfGrids");
  if (!dbase.has_key("totalNumberOfGrids")) dbase.put<real>("totalNumberOfGrids");
  if (!dbase.has_key("minimumNumberOfGridPoints")) dbase.put<real>("minimumNumberOfGridPoints");
  if (!dbase.has_key("maximumNumberOfGridPoints")) dbase.put<real>("maximumNumberOfGridPoints");
  if (!dbase.has_key("sumTotalNumberOfGridPoints")) dbase.put<real>("sumTotalNumberOfGridPoints");
  if (!dbase.has_key("numberOfRegrids")) dbase.put<real>("numberOfRegrids");


  if (!dbase.has_key("showVariable")) dbase.put<IntegerArray>("showVariable");
  if (!dbase.has_key("fluidDensity")) dbase.put<real>("fluidDensity");
  if (!dbase.has_key("checkFileCutoff")) dbase.put<RealArray>("checkFileCutoff");
  if (!dbase.has_key("useFullSystemForImplicitTimeStepping")) dbase.put<bool>("useFullSystemForImplicitTimeStepping");
  if (!dbase.has_key("pParallelInterpolator")) dbase.put<ParallelOverlappingGridInterpolator*>("pParallelInterpolator");
  if (!dbase.has_key("exactSolution")) dbase.put<OGFunction*>("exactSolution");
  if (!dbase.has_key("numberOfSequences")) dbase.put<int>("numberOfSequences");
  if (!dbase.has_key("gridIsImplicit")) dbase.put<IntegerArray>("gridIsImplicit");
  if (!dbase.has_key("inflowPressure")) dbase.put<real>("inflowPressure");
  if (!dbase.has_key("implicitMethod")) dbase.put<Parameters::ImplicitMethod>("implicitMethod");
  if (!dbase.has_key("linearizeImplicitMethod")) dbase.put<int>("linearizeImplicitMethod");
  if (!dbase.has_key("cDt")) dbase.put<real>("cDt");
  if (!dbase.has_key("pDebugFile")) dbase.put<FILE*>("pDebugFile");
  if (!dbase.has_key("initializeImplicitMethod")) dbase.put<int>("initializeImplicitMethod");
  if (!dbase.has_key("frequencyToSaveInShowFile")) dbase.put<int>("frequencyToSaveInShowFile");
  if (!dbase.has_key("probeFileFrequency")) dbase.put<int>("probeFileFrequency",1);
  if (!dbase.has_key("av2")) dbase.put<real>("av2");
  if (!dbase.has_key("pEquationDomainList")) dbase.put<ListOfEquationDomains*>("pEquationDomainList");
  if (!dbase.has_key("userDefinedTwilightZoneCoefficients")) dbase.put<bool>("userDefinedTwilightZoneCoefficients");
  if (!dbase.has_key("ad62")) dbase.put<real>("ad62");

  if (!dbase.has_key("assignInitialConditionsWithTwilightZoneFlow")) dbase.put<bool>("assignInitialConditionsWithTwilightZoneFlow");
  // Use the new way to compute starting values for PC schemes:
  if (!dbase.has_key("useNewTimeSteppingStartup")) dbase.put<bool>("useNewTimeSteppingStartup")=false;

  if (!dbase.has_key("plotMode")) dbase.put<int>("plotMode");
  if (!dbase.has_key("timeDependenceBoundaryConditionParameters")) dbase.put<RealArray>("timeDependenceBoundaryConditionParameters");
  if (!dbase.has_key("plotOption")) dbase.put<int>("plotOption");
  if (!dbase.has_key("tzDegreeSpace")) dbase.put<int>("tzDegreeSpace");
  if (!dbase.has_key("trigonometricTwilightZoneScaleFactor")) dbase.put<real>("trigonometricTwilightZoneScaleFactor");
  if (!dbase.has_key("explicitMethod")) dbase.put<int>("explicitMethod");
  if (!dbase.has_key("useLocalTimeStepping")) dbase.put<int>("useLocalTimeStepping");
  if (!dbase.has_key("modelParameters")) dbase.put<DataBase>("modelParameters");
  if (!dbase.has_key("alwaysUseCurvilinearBoundaryConditions")) dbase.put<bool>("alwaysUseCurvilinearBoundaryConditions");
  if (!dbase.has_key("maxIterations")) dbase.put<int>("maxIterations");
  if (!dbase.has_key("bc4workSpacePointer")) dbase.put<Insbc4WorkSpace*>("bc4workSpacePointer");
  if (!dbase.has_key("implicitFactor")) dbase.put<real>("implicitFactor");
  if (!dbase.has_key("collisionDistance")) dbase.put<real>("collisionDistance");
  if (!dbase.has_key("Rg")) dbase.put<real>("Rg");
  if (!dbase.has_key("forcingFunction")) dbase.put<realCompositeGridFunction*>("forcingFunction");
  if (!dbase.has_key("timeSteppingMethod")) dbase.put<Parameters::TimeSteppingMethod>("timeSteppingMethod");
  if (!dbase.has_key("gamma")) dbase.put<real>("gamma");
  if (!dbase.has_key("initialConditions")) dbase.put<RealArray>("initialConditions");
  if (!dbase.has_key("useStreamMode")) dbase.put<int>("useStreamMode");
  if (!dbase.has_key("pc")) dbase.put<int>("pc");
  if (!dbase.has_key("useImplicitFourthArtificialDiffusion")) dbase.put<bool>("useImplicitFourthArtificialDiffusion");
  if (!dbase.has_key("info")) dbase.put<int>("info");
  if (!dbase.has_key("turbulenceTripPoint")) dbase.put<IntegerArray>("turbulenceTripPoint");
  if (!dbase.has_key("numberOfSurfaceEquationVariables")) dbase.put<int>("numberOfSurfaceEquationVariables");
  if (!dbase.has_key("truncationError")) dbase.put<realCompositeGridFunction*>("truncationError");
  if (!dbase.has_key("av4")) dbase.put<real>("av4");
  if (!dbase.has_key("readRestartFile")) dbase.put<bool>("readRestartFile");
  if (!dbase.has_key("u0")) dbase.put<real>("u0");
  MovingGrids mvTmp(*this);
  if (!dbase.has_key("movingGrids")) dbase.put<MovingGrids>("movingGrids",mvTmp);
  if (!dbase.has_key("infoFile")) dbase.put<FILE*>("infoFile");
  if (!dbase.has_key("trackingIsOn")) dbase.put<int>("trackingIsOn");
  if (!dbase.has_key("reactions")) dbase.put<Reactions*>("reactions",NULL);
  if (!dbase.has_key("Ru")) dbase.put<Range>("Ru");
  if (!dbase.has_key("debugFile")) dbase.put<FILE*>("debugFile");
  if (!dbase.has_key("tFinal")) dbase.put<real>("tFinal");
  if (!dbase.has_key("slowStartTime")) dbase.put<real>("slowStartTime");
  if (!dbase.has_key("slowStartSteps")) dbase.put<int>("slowStartSteps");
  if (!dbase.has_key("slowStartRecomputeDtSteps")) dbase.put<int>("slowStartRecomputeDtSteps");
  if (!dbase.has_key("saveSequencesEveryTime")) dbase.put<bool>("saveSequencesEveryTime");
  if (!dbase.has_key("show")) dbase.put<Ogshow*>("show");
  if (!dbase.has_key("useUserDefinedErrorEstimator")) dbase.put<bool>("useUserDefinedErrorEstimator");
  if (!dbase.has_key("crossOverTemperatureI")) dbase.put<real>("crossOverTemperatureI");
  if (!dbase.has_key("variableBoundaryData")) dbase.put<IntegerArray>("variableBoundaryData");
  if (!dbase.has_key("anl")) dbase.put<real>("anl");
  if (!dbase.has_key("artificialDiffusion")) dbase.put<RealArray>("artificialDiffusion");
  if (!dbase.has_key("saveGridInShowFile")) dbase.put<int>("saveGridInShowFile");
  if (!dbase.has_key("fixupFrequency")) dbase.put<int>("fixupFrequency");
  if (!dbase.has_key("advectPassiveScalar")) dbase.put<bool>("advectPassiveScalar");
  if (!dbase.has_key("orderOfExtrapolationForInterpolationNeighbours")) dbase.put<int>("orderOfExtrapolationForInterpolationNeighbours");
  if (!dbase.has_key("interpolateRefinements")) dbase.put<InterpolateRefinements*>("interpolateRefinements");
  if (!dbase.has_key("cdv")) dbase.put<real>("cdv");
  if (!dbase.has_key("sec")) dbase.put<int>("sec");
  if (!dbase.has_key("useSelfAdjointDiffusion")) dbase.put<bool>("useSelfAdjointDiffusion");
  if (!dbase.has_key("errorThreshold")) dbase.put<real>("errorThreshold");
  if (!dbase.has_key("nu")) dbase.put<real>("nu");
  if (!dbase.has_key("forcing")) dbase.put<bool>("forcing");
  if (!dbase.has_key("useSecondOrderArtificialDiffusion")) dbase.put<bool>("useSecondOrderArtificialDiffusion");
  if (!dbase.has_key("showFileFrameForGrid")) dbase.put<int>("showFileFrameForGrid");
  if (!dbase.has_key("omega")) dbase.put<ArraySimpleFixed<real,4,1,1,1> >("omega");
  if (!dbase.has_key("pulseData")) dbase.put<ArraySimpleFixed<real,9,1,1,1> >("pulseData");
  if (!dbase.has_key("dbase")) dbase.put<DataBase>("dbase");
  if (!dbase.has_key("l0")) dbase.put<real>("l0");
  if (!dbase.has_key("ad21n")) dbase.put<real>("ad21n");
  if (!dbase.has_key("absorbedEnergy")) dbase.put<real>("absorbedEnergy");
  if (!dbase.has_key("stencilWidthForExposedPoints")) dbase.put<int>("stencilWidthForExposedPoints");
  if (!dbase.has_key("useSixthOrderArtificialDiffusion")) dbase.put<bool>("useSixthOrderArtificialDiffusion");
  if (!dbase.has_key("adaptiveGridProblem")) dbase.put<bool>("adaptiveGridProblem");
  if (!dbase.has_key("rateConstant")) dbase.put<real>("rateConstant");
  if (!dbase.has_key("nextTimeToPrint")) dbase.put<real>("nextTimeToPrint");
  if (!dbase.has_key("heatRelease")) dbase.put<real>("heatRelease");
  if (!dbase.has_key("ps")) dbase.put<GenericGraphicsInterface*>("ps");
  if (!dbase.has_key("errorNorm")) dbase.put<int>("errorNorm");
  if (!dbase.has_key("projectInitialConditions")) dbase.put<bool>("projectInitialConditions");
  if (!dbase.has_key("outputFile")) dbase.put<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile");
  if (!dbase.has_key("implicitOption")) dbase.put<Parameters::ImplicitOption>("implicitOption");
  if (!dbase.has_key("cfl")) dbase.put<real>("cfl");
  if (!dbase.has_key("numberOfComponents")) dbase.put<int>("numberOfComponents");
  if (!dbase.has_key("uc")) dbase.put<int>("uc");
  if (!dbase.has_key("frequencyToSaveSequenceInfo")) dbase.put<int>("frequencyToSaveSequenceInfo");
  if (!dbase.has_key("forcingType")) dbase.put<Parameters::ForcingType>("forcingType");
  if (!dbase.has_key("rc")) dbase.put<int>("rc");
  if (!dbase.has_key("pressureLevel")) dbase.put<real>("pressureLevel");
  if (!dbase.has_key("fileOutputFrequency")) dbase.put<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency");
  if (!dbase.has_key("ad61")) dbase.put<real>("ad61");
  if (!dbase.has_key("printArray")) dbase.put<RealArray>("printArray");
  if (!dbase.has_key("orderOfAccuracy")) dbase.put<int>("orderOfAccuracy");
  if (!dbase.has_key("advectionCoefficient")) dbase.put<real>("advectionCoefficient");
  if (!dbase.has_key("computeReactions")) dbase.put<bool>("computeReactions",false);
  if (!dbase.has_key("a0")) dbase.put<real>("a0");
  if (!dbase.has_key("numberOfDimensions")) dbase.put<int>("numberOfDimensions");
  if (!dbase.has_key("sequenceCount")) dbase.put<int>("sequenceCount");
  if (!dbase.has_key("avr")) dbase.put<real>("avr");
  if (!dbase.has_key("ad41")) dbase.put<real>("ad41");
  if (!dbase.has_key("debug")) dbase.put<int>("debug");
  if (!dbase.has_key("betaT")) dbase.put<real>("betaT");
  if (!dbase.has_key("useDefaultErrorEstimator")) dbase.put<bool>("useDefaultErrorEstimator");
  if (!dbase.has_key("useDimensionalParameters")) dbase.put<bool>("useDimensionalParameters");
  if (!dbase.has_key("bcParameters")) dbase.put<RealArray>("bcParameters");
  if (!dbase.has_key("gridGenerator")) dbase.put<Ogen*>("gridGenerator");
  if (!dbase.has_key("axisymmetricProblem")) dbase.put<bool>("axisymmetricProblem");
  if (!dbase.has_key("slipWallBoundaryConditionOption")) dbase.put<int>("slipWallBoundaryConditionOption");
  if (!dbase.has_key("scalarSystemForImplicitTimeStepping")) dbase.put<int>("scalarSystemForImplicitTimeStepping");
  if (!dbase.has_key("b1")) dbase.put<real>("b1");
  if (!dbase.has_key("maximumStepsBetweenComputingDt")) dbase.put<int>("maximumStepsBetweenComputingDt");
  if (!dbase.has_key("initialConditionOption")) dbase.put<Parameters::InitialConditionOption>("initialConditionOption");
  if (!dbase.has_key("removeFastPressureWaves")) dbase.put<int>("removeFastPressureWaves");
  if (!dbase.has_key("p0")) dbase.put<real>("p0");
  if (!dbase.has_key("turbulenceModel")) dbase.put<Parameters::TurbulenceModel>("turbulenceModel");
  if (!dbase.has_key("outputYplus")) dbase.put<bool>("outputYplus",false);
  if (!dbase.has_key("rho0")) dbase.put<real>("rho0");
  if (!dbase.has_key("frequencyToUseFullUpdateForMovingGridGeneration")) dbase.put<int>("frequencyToUseFullUpdateForMovingGridGeneration");
  if (!dbase.has_key("ad42n")) dbase.put<real>("ad42n");
  if (!dbase.has_key("useNewFourthOrderBoundaryConditions")) dbase.put<int>("useNewFourthOrderBoundaryConditions");
  if (!dbase.has_key("orderOfPredictorCorrector")) dbase.put<int>("orderOfPredictorCorrector");
  if (!dbase.has_key("orderOfBDF")) dbase.put<int>("orderOfBDF")=2;
  if (!dbase.has_key("numberOfShowVariables")) dbase.put<int>("numberOfShowVariables");
  if (!dbase.has_key("cornerExtrapolationOption")) dbase.put<int>("cornerExtrapolationOption");
  if (!dbase.has_key("cflOpt")) dbase.put<real>("cflOpt");
  if (!dbase.has_key("moveFile")) dbase.put<FILE*>("moveFile");
  if (!dbase.has_key("includeArtificialDiffusionInPressureEquation")) dbase.put<bool>("includeArtificialDiffusionInPressureEquation");
  if (!dbase.has_key("slowStartCFL")) dbase.put<real>("slowStartCFL");
  if (!dbase.has_key("artVisc")) dbase.put<real>("artVisc");
  if (!dbase.has_key("timing")) dbase.put<RealArray>("timing");
  if (!dbase.has_key("cflMin")) dbase.put<real>("cflMin");
  if (!dbase.has_key("runTimeDialog")) dbase.put<GUIState*>("runTimeDialog");
  if (!dbase.has_key("pressureBoundaryCondition")) dbase.put<int>("pressureBoundaryCondition");
  if (!dbase.has_key("sc")) dbase.put<int>("sc");
  if (!dbase.has_key("axisymmetricWithSwirl")) dbase.put<bool>("axisymmetricWithSwirl");
  if (!dbase.has_key("sequence")) dbase.put<RealArray>("sequence");
  if (!dbase.has_key("regrid")) dbase.put<Regrid*>("regrid");
  if (!dbase.has_key("myid")) dbase.put<int>("myid");
  if (!dbase.has_key("saveRestartFile")) dbase.put<bool>("saveRestartFile");
  if (!dbase.has_key("modelData")) dbase.put<DataBase>("modelData");
  if (!dbase.has_key("logFile")) dbase.put<FILE*>("logFile");
  if (!dbase.has_key("R0")) dbase.put<real>("R0");
  if (!dbase.has_key("aw4")) dbase.put<real>("aw4");
  if (!dbase.has_key("useSmagorinskyEddyViscosity")) dbase.put<bool>("useSmagorinskyEddyViscosity");
  if (!dbase.has_key("checkErrorsAtGhostPoints")) dbase.put<int>("checkErrorsAtGhostPoints");
  if (!dbase.has_key("ec")) dbase.put<int>("ec");
  if (!dbase.has_key("orderOfTimeAccuracy")) dbase.put<int>("orderOfTimeAccuracy");
  if (!dbase.has_key("maxNumberOfShowVariables")) dbase.put<int>("maxNumberOfShowVariables");
  if (!dbase.has_key("numberOfIterationsForImplicitTimeStepping")) dbase.put<int>("numberOfIterationsForImplicitTimeStepping");
  if (!dbase.has_key("adjustTimeStepForMovingBodies")) dbase.put<bool>("adjustTimeStepForMovingBodies");
  if (!dbase.has_key("numberOfOutputFiles")) dbase.put<int>("numberOfOutputFiles");
  if (!dbase.has_key("crossOverTemperatureB")) dbase.put<real>("crossOverTemperatureB");
  if (!dbase.has_key("cflMax")) dbase.put<real>("cflMax");
  if (!dbase.has_key("numberOfExtraVariables")) dbase.put<int>("numberOfExtraVariables");
  if (!dbase.has_key("aw2")) dbase.put<real>("aw2");
  if (!dbase.has_key("kThermal")) dbase.put<real>("kThermal");
  if (!dbase.has_key("enforceAbsoluteToleranceForIterativeSolvers")) dbase.put<int>("enforceAbsoluteToleranceForIterativeSolvers");
  if (!dbase.has_key("maximumNumberOfIterationsForImplicitInterpolation")) dbase.put<int>("maximumNumberOfIterationsForImplicitInterpolation");
  if (!dbase.has_key("showAmrErrorFunction")) dbase.put<int>("showAmrErrorFunction");
  if (!dbase.has_key("amrErrorFunctionOption")) dbase.put<int>("amrErrorFunctionOption");
  if (!dbase.has_key("tInitial")) dbase.put<real>("tInitial");
  if (!dbase.has_key("rT0")) dbase.put<real>("rT0");
  if (!dbase.has_key("smagorinskyCoefficient")) dbase.put<real>("smagorinskyCoefficient");
  if (!dbase.has_key("reactionType")) dbase.put<Parameters::ReactionTypeEnum>("reactionType");
  if (!dbase.has_key("mu")) dbase.put<real>("mu");
  if (!dbase.has_key("tPrint")) dbase.put<real>("tPrint");
  if (!dbase.has_key("pStatic")) dbase.put<real>("pStatic");
  if (!dbase.has_key("ad22")) dbase.put<real>("ad22");
  if (!dbase.has_key("namesOfStatistics")) dbase.put<aString*>("namesOfStatistics");
  if (!dbase.has_key("useFourthOrderArtificialDiffusion")) dbase.put<bool>("useFourthOrderArtificialDiffusion");
  if (!dbase.has_key("refactorFrequency")) dbase.put<int>("refactorFrequency");
  if (!dbase.has_key("reactionName")) dbase.put<aString>("reactionName");
  if (!dbase.has_key("improveQualityOfInterpolation")) dbase.put<bool>("improveQualityOfInterpolation");
  if (!dbase.has_key("dimensionOfTZFunction")) dbase.put<int>("dimensionOfTZFunction");
  if (!dbase.has_key("b0")) dbase.put<real>("b0");
  if (!dbase.has_key("amrRegridFrequency")) dbase.put<int>("amrRegridFrequency");

  if (!dbase.has_key("Rt")) dbase.put<Range>("Rt");  // time dependent components
  if (!dbase.has_key("Rtimp")) dbase.put<Range>("Rtimp");  // time dependent components that may be treated implicitly 


  if (!dbase.has_key("componentName")) dbase.put<aString*>("componentName",NULL);
  if (!dbase.has_key("pdeParameters")) dbase.put<ListOfShowFileParameters>("pdeParameters");
  if (!dbase.has_key("pDistanceToBoundary")) dbase.put<realCompositeGridFunction*>("pDistanceToBoundary");
  if (!dbase.has_key("reynoldsNumber")) dbase.put<real>("reynoldsNumber");
  if (!dbase.has_key("ad21")) dbase.put<real>("ad21");
  if (!dbase.has_key("b2")) dbase.put<real>("b2");
  if (!dbase.has_key("useSplitStepImplicitArtificialDiffusion")) dbase.put<int>("useSplitStepImplicitArtificialDiffusion");
  if (!dbase.has_key("pVariableCoefficients")) dbase.put<realCompositeGridFunction*>("pVariableCoefficients");
  if (!dbase.has_key("radialAxis")) dbase.put<int>("radialAxis");
  if (!dbase.has_key("vc")) dbase.put<int>("vc");
  if (!dbase.has_key("kc")) dbase.put<int>("kc");
  if (!dbase.has_key("interpolationQualityBound")) dbase.put<real>("interpolationQualityBound");
  if (!dbase.has_key("ad22n")) dbase.put<real>("ad22n");
  if (!dbase.has_key("dt")) dbase.put<real>("dt",0.);
  if (!dbase.has_key("orderOfAdaptiveGridInterpolation")) dbase.put<int>("orderOfAdaptiveGridInterpolation");
  if (!dbase.has_key("checkForInflowAtOutFlow")) dbase.put<int>("checkForInflowAtOutFlow");
  if (!dbase.has_key("ad42")) dbase.put<real>("ad42");
  if (!dbase.has_key("dtMax")) dbase.put<real>("dtMax");
  if (!dbase.has_key("reciprocalActivationEnergyI")) dbase.put<real>("reciprocalActivationEnergyI");
  if (!dbase.has_key("psp")) dbase.put<GraphicsParameters>("psp");
  if (!dbase.has_key("errorEstimator")) dbase.put<ErrorEstimator*>("errorEstimator");
  if (!dbase.has_key("plotIterations")) dbase.put<int>("plotIterations");
  if (!dbase.has_key("checkFile")) dbase.put<FILE*>("checkFile");
  if (!dbase.has_key("applyExplicitBCsToImplicitGrids")) dbase.put<bool>("applyExplicitBCsToImplicitGrids");
  if (!dbase.has_key("pKnownSolution")) dbase.put<realCompositeGridFunction*>("pKnownSolution");
  if (!dbase.has_key("recomputeDTEveryStep")) dbase.put<bool>("recomputeDTEveryStep",false);
  if (!dbase.has_key("timeStepDataIsPrecomputed")) dbase.put<bool>("timeStepDataIsPrecomputed",false);
  if (!dbase.has_key("useLineSolver")) dbase.put<bool>("useLineSolver",false);
  if (!dbase.has_key("preconditionerFrequency")) dbase.put<int>("preconditionerFrequency",1);

  if (!dbase.has_key("targetGridSpacing")) dbase.put<real>("targetGridSpacing"); // used to compute TTS for output
  dbase.get<real>("targetGridSpacing")=-1.; // this is assigned in getGridInfo (or by the user in "General Options")
  if (!dbase.has_key("velocityScale")) dbase.put<real>("velocityScale");         // used to compute TTS for output
  dbase.get<real>("velocityScale")=1.;      // this is optionally changed by the user


  if (!dbase.has_key("plotGridVelocity")) dbase.put<int>("plotGridVelocity",false);

  if (!dbase.has_key("knownSolution"))
  {
    dbase.put<Parameters::KnownSolutionsEnum>("knownSolution");
    dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")=noKnownSolution;
  }
  if( !dbase.has_key("knownSolutionIsTimeDependent") )  dbase.put<bool>("knownSolutionIsTimeDependent",true);

  if(!dbase.has_key("useGridFromShowFile") ) dbase.put<bool>("useGridFromShowFile",true);

  if (!dbase.has_key("initialConditionsAreBeingProjected")) dbase.put<int>("initialConditionsAreBeingProjected",0);

  if (!dbase.has_key("useNewImplicitMethod")) dbase.put<int>("useNewImplicitMethod");
  dbase.get<int>("useNewImplicitMethod")=0;

  // By default we adjust the (TZ) forcing for implicit time stepping:
  if (!dbase.has_key("adjustForcingForImplicit")) dbase.put<int>("adjustForcingForImplicit");
  dbase.get<int>("adjustForcingForImplicit")=1;

  if (!dbase.has_key("predictorOrder")) dbase.put<int>("predictorOrder");
  dbase.get<int>("predictorOrder")=0; // 0 means use default order

  // number of correction steps for the predictor corrector method: (maximum number allowed)
  if (!dbase.has_key("numberOfPCcorrections")) dbase.put<int>("numberOfPCcorrections");
  dbase.get<int>("numberOfPCcorrections")=1; 

  // minium number of correction steps for the predictor corrector method (we may have a convergence criteria):
  if (!dbase.has_key("minimumNumberOfPCcorrections")) dbase.put<int>("minimumNumberOfPCcorrections",1);

  // keep count of the total number of PC corrections (the number per step can vary with some options)
  if (!dbase.has_key("totalNumberOfPCcorrections")) dbase.put<int>("totalNumberOfPCcorrections",0);

  // max number of correction steps for the approximate factorization method:
  if (!dbase.has_key("numberOfAFcorrections")) dbase.put<int>("numberOfAFcorrections");
  dbase.get<int>("numberOfAFcorrections")=10; 

  // relative tolerance for the approximate factorization method's interpolation point iteration
  if (!dbase.has_key("AFcorrectionRelTol")) dbase.put<real>("AFcorrectionRelTol");
  dbase.get<real>("AFcorrectionRelTol")=1e-2; 

  // width of parallel ghost points for the approximate factorization method
  if (!dbase.has_key("AFparallelGhostWidth")) dbase.put<int>("AFparallelGhostWidth");
  dbase.get<int>("AFparallelGhostWidth")=2; 

  // multiDomainProblem=1 if this DomainSolver is part of a large multi-domain problem
  if (!dbase.has_key("multiDomainProblem")) dbase.put<int>("multiDomainProblem");
  dbase.get<int>("multiDomainProblem")=0; 

  // multiDomainSolver - pointer to the multiDomainSolver Cgmp (for multi-domain solvers)
  if (!dbase.has_key("multiDomainSolver")){ dbase.put<DomainSolver*>("multiDomainSolver")=NULL; } // 

  if (!dbase.has_key("referenceFrame")) dbase.put<ReferenceFrameEnum>("referenceFrame");
  dbase.get<ReferenceFrameEnum>("referenceFrame")=fixedReferenceFrame; 

  // Set updateTimeIndependentVariables=true if time indepenent variables need updated (such as the pressure)
  if (!dbase.has_key("updateTimeIndependentVariables")) dbase.put<int>("updateTimeIndependentVariables");
  dbase.get<int>("updateTimeIndependentVariables")=true; 

  if (!dbase.has_key("originalBoundaryCondition")) dbase.put<IntegerArray>("originalBoundaryCondition");

  if (!dbase.has_key("useNewAdvanceStepsVersions")) dbase.put<int >("useNewAdvanceStepsVersions");
  dbase.get<int >("useNewAdvanceStepsVersions")=false;

  // The interfaceType(side,axis,grid) : defines the type of interface condition
  if( !dbase.has_key("interfaceType") ) dbase.put<IntegerArray>("interfaceType");

  // We sometimes need to turn off application of the interface boundary conditions
  if( !dbase.has_key("applyInterfaceBoundaryConditions") ) dbase.put<int>("applyInterfaceBoundaryConditions",1);

  // Apply a projection to the interface values
  if (!dbase.has_key("projectInterface")) dbase.put<bool>("projectInterface",false);

  // Apply a projection to the rigid body interface values
  if (!dbase.has_key("projectRigidBodyInterface")) dbase.put<bool>("projectRigidBodyInterface",false);

  if( !dbase.has_key("printMovingBodyInfo") ) dbase.put<bool>("printMovingBodyInfo")=false;

  // simulateGridMotionOnly : 
  //            0 = normal solve and move 
  //            1 = move grids and generate overlapping grids
  //            2 = move grids (do not generate overlapping grids)    
  if (!dbase.has_key("simulateGridMotion")) dbase.put<int>("simulateGridMotion",0);

  if (!dbase.has_key("adjustGridForDisplacement")) dbase.put<int>("adjustGridForDisplacement",0);

  if (!dbase.has_key("showFileParams")) dbase.put<ListOfShowFileParameters>("showFileParams");

  if( !dbase.has_key("saveAugmentedSolutionToShowFile") ) dbase.put<bool>("saveAugmentedSolutionToShowFile")=false;

  // turn on the interactive grid generator (Ogen) for moving grids.
  if (!dbase.has_key("useInteractiveGridGenerator")) dbase.put<bool >("useInteractiveGridGenerator",false);

   // boundaryData holds the RHS for BC's 
   // std::vector<BoundaryData> boundaryData;

  if (!dbase.has_key("boundaryData")) dbase.put<std::vector<BoundaryData> >("boundaryData");


  
  // *wdh* 100907:  -- we need to make this more flexible, but do this for testing:
  // movingBodyPressureBC = 0 : use default Neumann pressure BC on moving bodies
  //                      = 1 : use mixed BC for pressure on moving bodies (for "light" bodies)
  // 
  if( !dbase.has_key("movingBodyPressureBC")) dbase.put<int>("movingBodyPressureBC",0);
  if( !dbase.has_key("movingBodyPressureCoefficient")) dbase.put<real>("movingBodyPressureCoefficient",0.);


  // -- body forcing objects ---
  if( !dbase.has_key("turnOnBodyForcing") ) dbase.put<bool >("turnOnBodyForcing",false);
  if( !dbase.has_key("bodyForce") ) dbase.put<realCompositeGridFunction* >("bodyForce");
  dbase.get<realCompositeGridFunction* >("bodyForce")=NULL;

  if( !dbase.has_key("plotBodyForceMaskSurface") ) dbase.put<bool>("plotBodyForceMaskSurface",false);  

  // plot the body force:
  if( !dbase.has_key("plotBodyForce") ) dbase.put<bool>("plotBodyForce",false);

  // plot beams and shells
  if( !dbase.has_key("plotStructures") ) dbase.put<bool>("plotStructures",true);

  // -- boundary forcing objects --
  if( !dbase.has_key("turnOnBoundaryForcing") ) dbase.put<bool >("turnOnBoundaryForcing",false);

  // -- User defined forcing objects --
  if( !dbase.has_key("turnOnUserDefinedForcing") ) dbase.put<bool >("turnOnUserDefinedForcing",false);
  if( !dbase.has_key("userDefinedForcingIsTimeDependent") ) dbase.put<bool >("userDefinedForcingIsTimeDependent",true);

  // -- Variable Material properties option --
  //   0 = constant material properties
  //   1 = piece-wise constant material properties
  //   2 = variable material properties
  if( !dbase.has_key("variableMaterialPropertiesOption") ) dbase.put<int>("variableMaterialPropertiesOption",0);

  // Names of material properties go here: (Each name should be an entry in the dbase of type real)
  // These are coefficients that can vary over the grid (e.g. rho, mu, lambda for elasticity)
  if( !dbase.has_key("materialPropertyNames") ) dbase.put<std::vector<aString> >("materialPropertyNames");

  // Here is where we save the names of things for which we keep track of CPU time for:
  if( !dbase.has_key("timingName") ) dbase.put<std::vector<aString> >("timingName");
  if( !dbase.has_key("maximumNumberOfTimings") ) dbase.put<int>("maximumNumberOfTimings",0);  


  // -- Controls --
  if( !dbase.has_key("turnOnController") ) dbase.put<bool >("turnOnController",false);

  // -- added mass parameters --
  //  useAddedMassAlgorithm : turn on the added mass algorithm
  //  projectAddedMassVelocity : perform the added mass velocity projection (if useAddedMassAlgorithm=true)
  // *** ALL these parameters should be in the DeformingBody ****  FIX ME **

  if( !dbase.has_key("useAddedMassAlgorithm") ) dbase.put<bool>("useAddedMassAlgorithm")=false;
  if( !dbase.has_key("useApproximateAMPcondition") ) dbase.put<bool>("useApproximateAMPcondition")=false;
  if( !dbase.has_key("projectAddedMassVelocity") ) dbase.put<bool>("projectAddedMassVelocity")=true;
  if( !dbase.has_key("projectNormalComponentOfAddedMassVelocity") )
     dbase.put<bool>("projectNormalComponentOfAddedMassVelocity")=false;
  if( !dbase.has_key("projectBeamVelocity") ) dbase.put<bool>("projectBeamVelocity")=true;
  if( !dbase.has_key("projectVelocityOnBeamEnds") ) dbase.put<bool>("projectVelocityOnBeamEnds")=true;
  if( !dbase.has_key("smoothInterfaceVelocity") ) dbase.put<bool>("smoothInterfaceVelocity")=true;
  if( !dbase.has_key("numberOfInterfaceVelocitySmooths") ) dbase.put<int>("numberOfInterfaceVelocitySmooths")=1;
  if( !dbase.has_key("fluidAddedMassLengthScale") ) dbase.put<real>("fluidAddedMassLengthScale")=1.;

  // For the traditional FSI scheme we sometimes perform sub-iterations for FSI problems
  if( !dbase.has_key("useMovingGridSubIterations") ) dbase.put<bool>("useMovingGridSubIterations")=false;


  // -----------------------------------------------------------------
  // ---- Assign initial values to those variables not already set ----
  // -----------------------------------------------------------------


  dbase.get<realCompositeGridFunction* >("pKnownSolution")=NULL;              // holds any known solution 

  dbase.get<int >("numberOfDimensions") = numberOfDimensions0;

  dbase.get<int >("myid")=max(0,Communication_Manager::My_Process_Number);

  dbase.get<DataBase >("modelParameters").put<int>("fixupUnusedPointsFrequency",4);
  

  dbase.get<int >("debug")=0;
  dbase.get<int >("info")=3;              // level of informational output

  dbase.get<FILE* >("infoFile")=stdout;      // information messages sent here (by default stdout)
  dbase.get<FILE* >("debugFile")=NULL;       // pointer to  dbase.get<int >("debug") file for fprintf
  dbase.get<FILE* >("pDebugFile")=NULL;      //  dbase.get<FILE* >("pDebugFile") =  dbase.get<int >("debug") file for each processor.
  dbase.get<FILE* >("logFile")=NULL;         // log file.
  dbase.get<FILE* >("moveFile")=NULL;        // file to hold  dbase.get<int >("info") from moving grids
  dbase.get<FILE* >("checkFile")=NULL;       // file to hold output for regression tests

  dbase.get<OGFunction* >("exactSolution")=NULL;
  dbase.get<aString* >("componentName")=NULL;
  dbase.get<GenericGraphicsInterface* >("ps")=NULL;
  dbase.get<Ogshow* >("show")=NULL;
  dbase.get<Parameters::ImplicitMethod >("implicitMethod")=backwardEuler;

  dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=adamsPredictorCorrector2; // default time stepper
  dbase.get<int >("orderOfPredictorCorrector")=2;

  dbase.get<Parameters::InitialConditionOption >("initialConditionOption")=noInitialConditionChosen;
  
  dbase.get<bool >("advectPassiveScalar")=false;
  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=noTurbulenceModel;
  dbase.get<int >("numberOfExtraVariables")=0;
  dbase.get<int >("numberOfSurfaceEquationVariables")=0;
  
  
  dbase.get<int >("useNewFourthOrderBoundaryConditions")=true;
  dbase.get<int >("slipWallBoundaryConditionOption")=0;  // use default slip wall BC
  dbase.get<bool >("alwaysUseCurvilinearBoundaryConditions")=false;
  
  dbase.get<aString >("reactionName")="none";
  dbase.get<Reactions* >("reactions")=NULL;
  
  dbase.get<int >("orderOfAccuracy")=2;
  dbase.get<int >("orderOfTimeAccuracy")=2;
  
  dbase.get<int >("orderOfTimeExtrapolationForPressure")=-1;

  dbase.get<bool >("readRestartFile")=false;
  dbase.get<bool >("saveRestartFile")=false;
  dbase.get<aString >("restartFileName")="cg.restart";
  dbase.get<aString* >("showVariableName")=NULL;
  dbase.get<int >("numberOfShowVariables")=0;
  dbase.get<int >("maxNumberOfShowVariables")=0;
  dbase.get<int >("useStreamMode")=false;  // *wdh* 000521 : now save uncompressed by default
  dbase.get<bool >("adaptiveGridProblem")=false;
  dbase.get<int >("orderOfAdaptiveGridInterpolation")=3;
  dbase.get<int >("amrRegridFrequency")=-1;  // this means use a default equal to the refinement ratio.
  dbase.get<int>("amrErrorFunctionOption")=0; // 1 means use top-hat function for error estimate
  dbase.get<real >("truncationErrorCoefficient")=0.;

  dbase.get<realCompositeGridFunction* >("truncationError")=NULL;  // put here for now
  
  dbase.get<Regrid* >("regrid")=NULL;
  dbase.get<InterpolateRefinements* >("interpolateRefinements")=NULL;
  dbase.get<ErrorEstimator* >("errorEstimator")=NULL;
  dbase.get<real >("errorThreshold")=.2;
  dbase.get<int >("showAmrErrorFunction")=false;
  dbase.get<int >("showResiduals")=false;

  dbase.get<bool >("useDefaultErrorEstimator")=true;
  dbase.get<bool >("useUserDefinedErrorEstimator")=false;

  dbase.get<int >("allowUserDefinedOutput")=false;
  dbase.get<int >("trackingIsOn")=false;         // if true, turn on calls to the tracking function (front tracking etc.)
  dbase.get<int >("trackingFrequency")=10;
  
  dbase.get<aString >("outputFormat")="%9.2e ";   // output format for outputSolution
  
  dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")=NULL;
  dbase.get<realCompositeGridFunction* >("pVariableCoefficients")=NULL;
  
  dbase.get<Ogen* >("gridGenerator")=NULL;
  dbase.get<ParallelOverlappingGridInterpolator* >("pParallelInterpolator")=NULL;
  
  // **fix me : these Ogen parameters should be saved with the CompositeGrid when it is made **
  dbase.get<bool >("improveQualityOfInterpolation")=false;  // for moving grids - grid generation option 
  dbase.get<real >("interpolationQualityBound")=-1;         // -1. => use default

  if (!dbase.has_key("maximumAngleDifferenceForNormalsOnSharedBoundaries")) dbase.put<real>("maximumAngleDifferenceForNormalsOnSharedBoundaries");
  dbase.get<real >("maximumAngleDifferenceForNormalsOnSharedBoundaries")=-1.;  // -1 : use default

  dbase.get<GUIState* >("runTimeDialog")=NULL;

  dbase.get<aString* >("namesOfStatistics")=NULL;
  
  dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration")=5;

  // some methods have a stencil width of 5 -- these require more exposed points to be set
  dbase.get<int >("stencilWidthForExposedPoints")=3;
  // For some methods we extrapolate interpolation neighbours to allow for  a wider stencil
  dbase.get<int >("extrapolateInterpolationNeighbours")=false;
  
  dbase.get<bool >("detectCollisions")=false;
  dbase.get<real >("collisionDistance")=2.5;
  dbase.get<bool >("adjustTimeStepForMovingBodies")=false; // adjust  dbase.get<real >("dt") for the equations of motion of the bodies.
  
  dbase.get<int >("numberOfOutputFiles")=0;
  for( int n=0; n<maximumNumberOfOutputFiles; n++ )
    {
      dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n]=NULL;
      dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency")[n]=1;
    }

  dbase.get<int >("pressureBoundaryCondition")=0;  // 0=new, 1=old
  dbase.get<bool >("useCharacteristicInterpolation")=false;

  dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")= defaultInterpolationType;
  
  dbase.get<int >("numberOfSequences")=2; // save max(residual) and l2(residual)
  dbase.get<int >("sequenceCount")=0;  
  dbase.get<RealArray >("timeSequence").redim(20);
  dbase.get<RealArray >("sequence").redim(20, dbase.get<int >("numberOfSequences"));
  dbase.get<bool >("saveSequencesEveryTime")=true; // if true save current  dbase.get<RealArray >("sequence")  dbase.get<int >("info") to  dbase.get<Ogshow* >("show") file at each output step. 
  dbase.get<int >("frequencyToSaveSequenceInfo")=10;
  
  dbase.get<real >("a0")=0.;  dbase.get<real >("a1")=0.;  dbase.get<real >("a2")=0.;  dbase.get<real >("b0")=0.;  dbase.get<real >("b1")=0.;  dbase.get<real >("b2")=0.;   // parameters for time stepping methods


  dbase.get<bool >("axisymmetricProblem")=false;
  dbase.get<bool >("axisymmetricWithSwirl")=false;
  dbase.get<int >("radialAxis")=axis2;    // =axis2 if y=0 is axis of symmetry, =axis1 if x=0 is axis of symmetry

  dbase.get<int >("useLocalTimeStepping")=false;
  
  dbase.get<real >("u0")=1.;   // velocity scale
  dbase.get<real >("l0")=1.;   // length scale 
  dbase.get<real >("rho0")=1.; // density scale 
  dbase.get<real >("p0")=1.;   // pressure
  dbase.get<real >("te0")=1.;  // temperature

  // kkc 101005 added the filter stuff in the next three lines
  if( !dbase.has_key("applyFilter") ) dbase.put<bool >("applyFilter",false);
  if( !dbase.has_key("gridFunctionFilter") ) dbase.put<GridFunctionFilter*>("gridFunctionFilter");
  dbase.get<GridFunctionFilter*>("gridFunctionFilter")=NULL;
  if ( !dbase.has_key("applyAFBCLimiter") ) dbase.put<bool >("applyAFBCLimiter",false);

  timeSteppingName[forwardEuler]=                 "forwardEuler";
  timeSteppingName[adamsBashforth2]=              "adamsBashforth (2nd order)";
  timeSteppingName[adamsPredictorCorrector2]=     "adams predictor corrector (2nd order)";
  timeSteppingName[adamsPredictorCorrector4]=     "adams predictor corrector (4th order)";
  timeSteppingName[variableTimeStepAdamsPredictorCorrector]="variable time step, predictor-corrector";
  timeSteppingName[laxFriedrichs]=                "laxFriedrichs";
  timeSteppingName[implicit]=                     "implicit";
  timeSteppingName[midPoint]=                     "midPoint";
  timeSteppingName[implicitAllSpeed ]=            "implicitAllSpeed";
  timeSteppingName[nonMethodOfLines]=             "nonMethodOfLines";
  timeSteppingName[steadyStateRungeKutta]=        "steady state Runge Kutta";
  timeSteppingName[steadyStateNewton]=            "steady-state Newton";
  timeSteppingName[secondOrderSystemTimeStepping]="second order system time stepping";
  timeSteppingName[adi]=                          "adi";
  timeSteppingName[numberOfTimeSteppingMethods]="";

  /* ----
     movingGridOptionName[rotate]="rotate";
     movingGridOptionName[shift]="shift";
     movingGridOptionName[oscillate]="oscillate";
     movingGridOptionName[scale]="scale";
     movingGridOptionName[userDefinedMovingGrid]="userDefined";
     movingGridOptionName[numberOfMovingGridOptions]=""; // null terminated
     ---- */
  
  turbulenceModelName[0]="noTurbulenceModel";
  turbulenceModelName[1]="Baldwin-Lomax";
  turbulenceModelName[2]="kEpsilon";
  turbulenceModelName[3]="kOmega";
  turbulenceModelName[4]="SpalartAllmaras";
  turbulenceModelName[5]="LargeEddySimulation";
  turbulenceModelName[6]="";

  dbase.get<int >("compare3Dto2D")=false;       // if true we are comparing  a 3D run to  a 2D 
  dbase.get<int >("dimensionOfTZFunction")=-1;
  
  dbase.get<int >("maximumStepsBetweenComputingDt")=100; // recompute  dbase.get<real >("dt") at least this often
  dbase.get<int >("fixupFrequency")=4;
  
  dbase.get<real >("dtMax")=REAL_MAX*.1;
  dbase.get<real >("cfl")=.9;

  dbase.get<real >("cflMin")=.8;  // used by implicit time stepping to indicate when to increase the time step
  dbase.get<real >("cflMax")=.95;  // used by implicit time stepping to indicate when to decrease the time step
  

  dbase.get<real >("tFinal")=1.;
  dbase.get<real >("tInitial")=0.;
  dbase.get<real >("tPrint")=.1;
  dbase.get<int >("maxIterations")=10000;  // for steady state solvers
  dbase.get<int >("plotIterations")=100;   // number of iterations between plotting
  
  dbase.get<int >("plotMode")=0;           // 0=normal, 1=no plotting => do not allow  dbase.get<int >("plotOption") to change from no-plotting
  dbase.get<int >("plotOption")=3;  

  dbase.get<int >("frequencyToSaveInShowFile")=1;  // this means save every  dbase.get<real >("tPrint") steps
  dbase.get<int >("saveGridInShowFile")=true;
  dbase.get<int >("showFileFrameForGrid")=Ogshow::useDefaultLocation;
  
  
  dbase.get<bool >("forcing")=true;
  dbase.get<bool >("twilightZoneFlow")=true;
  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=polynomial;
  dbase.get<int >("tzDegreeSpace")=2;
  dbase.get<int >("tzDegreeTime")=2;
  dbase.get<int >("errorNorm")=INTEGER_MAX;  // by default use the emax norm error
  
  dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow")=true;
  dbase.get<bool >("userDefinedTwilightZoneCoefficients")=false;        // set to true if the user has assigned the coefficients
  
  dbase.get<real>("trigonometricTwilightZoneScaleFactor")=1;  // scale factor for Trigonometric TZ

  dbase.get<int >("reducedInterpolationWidth")=0;  // set to  a positive value if width has been reduced.
  
  dbase.get<bool >("projectInitialConditions")=false;
  // iterativePressureSolve=false;  // true if the pressure equation is solved by iteration

  dbase.get<int >("checkErrorsAtGhostPoints")=0;   // check errors on these many ghost lines
  
  dbase.get<Parameters::ImplicitMethod >("implicitMethod")=notImplicit;  // default 
  dbase.get<Parameters::ImplicitOption >("implicitOption")=computeAllTerms;

  dbase.get<int >("initializeImplicitMethod")=true;          // is this used?
  dbase.get<int >("initializeImplicitTimeStepping")=true;  
  dbase.get<int >("scalarSystemForImplicitTimeStepping")=true;
  dbase.get<bool >("useFullSystemForImplicitTimeStepping")=false; // prevent  a scalat system being solved even if it could be
  dbase.get<bool >("applyExplicitBCsToImplicitGrids")=true;

  dbase.get<int >("numberOfIterationsForConstraints")=0;
  dbase.get<int >("numberOfSolvesForConstraints")=0;
  
  dbase.get<int >("numberOfIterationsForImplicitTimeStepping")=0;
  
  dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation")=-1; // -1 : use default

  dbase.get<int >("enforceAbsoluteToleranceForIterativeSolvers")=false;
  
  dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours")=-1; // -1 -> use default
  dbase.get<int >("orderOfExtrapolationForSecondGhostLine")=-1;         // -1 -> use default.
  dbase.get<int >("orderOfExtrapolationForOutflow")=-1;                 // -1 -> use default.
  dbase.get<int >("cornerExtrapolationOption")=0;  // by default extrap corners along diagonals
  dbase.get<BoundaryConditionParameters::ExtrapolationOptionEnum >("extrapolationOption")=  BoundaryConditionParameters::polynomialExtrapolation;
  
  dbase.get<int >("checkForInflowAtOutFlow")=0;  // for insBC -- check for inflow at an outflow BC and adjust the BC


  // --- PDE parameters : some of these should be moved to the appropriate DomainSolver -----

  dbase.get<real >("nu")=.01;

  dbase.get<real >("advectionCoefficient")=1.;        // coefficient of advection terms in the NS (default=1)
  dbase.get<real >("implicitFactor")=.5;              // .5=CN, 1=BE, 0=FE

  dbase.get<real >("slowStartCFL")=.25;
  dbase.get<real >("slowStartTime")=-1.;              // if positive, ramp up the time step during this time interval.
  dbase.get<int  >("slowStartSteps")=-1;              // if positive, ramp up the time step over this many time steps
  dbase.get<int  >("slowStartRecomputeDtSteps")=100;  // recompute dt during slow start every this many steps
  
  dbase.get<bool >("useDimensionalParameters")=true;
  dbase.get<real >("mu")=.1;
  dbase.get<real >("kThermal")=.1;
  dbase.get<real >("Rg")=2.;
  dbase.get<real >("gamma")=1.4;
  dbase.get<real >("avr")=0.;

  ArraySimpleFixed<real,3,1,1,1> & gravity = dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");

  gravity[0]= gravity[1]= gravity[2]=0.;   //  gravity is off by default
  dbase.get<real >("fluidDensity")=0.;   // use zero for backward compatibility of rigid body computations

  dbase.get<int >("removeFastPressureWaves")=false;
  
  dbase.get<real >("nuPassiveScalar")=.0;
  
  // for conservative CNS with artificial viscosity:
  dbase.get<real >("artVisc")=.0001;
  dbase.get<real >("av2")=.25;
  dbase.get<real >("aw2")=.008333; 
  dbase.get<real >("av4")=.1; 
  dbase.get<real >("aw4")=.01; 
  dbase.get<real >("betaT")=0.;  // 0=no temperature dependence.
  dbase.get<real >("betaK")=0.; 
  dbase.get<real >("rT0")=1.;
  
  dbase.get<bool >("useSmagorinskyEddyViscosity")=false;
  dbase.get<real >("smagorinskyCoefficient")=.2;

  // spalartAllmarasScaleFactor=1.; // factor to artificially increase the SA production term

  dbase.get<bool >("useSelfAdjointDiffusion")=false;
  
  
  dbase.get<real >("anu")=0.;  // nonlinear artificial viscosity
  dbase.get<real >("pressureLevel")=0.;
  dbase.get<real >("machNumber")= dbase.get<real >("reynoldsNumber")=defaultValue;
  dbase.get<real >("prandtlNumber")=.72;
  
  dbase.get<Parameters::ReactionTypeEnum >("reactionType")=noReactions;
  
  // for Don's Godnunov reaction:
  dbase.get<real >("heatRelease")=0.;
  dbase.get<real >("reciprocalActivationEnergy")=.1;
  dbase.get<real >("rateConstant")=1.;

  dbase.get<real >("reciprocalActivationEnergyI")=.1;
  dbase.get<real >("reciprocalActivationEnergyB")=.1;
  dbase.get<real >("crossOverTemperatureI")=1.;
  dbase.get<real >("crossOverTemperatureB")=1.;
  dbase.get<real >("absorbedEnergy")=0.;

  dbase.get<real >("nuRho")=.0;
  dbase.get<int >("explicitMethod")=true;
  dbase.get<int >("linearizeImplicitMethod")=false;
  dbase.get<int >("refactorFrequency")=100;

  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0]= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1]= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2]= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]=1.;

  // Default values for the pulse TZ function: 
  //    U &=  a_0 \exp( - a_1 | \xv-\bv(t) |^{2p} )
  //    \bv(t) &= \cv_0 + \vv t
  ArraySimpleFixed<real,9,1,1,1> & pulseData = dbase.get<ArraySimpleFixed<real,9,1,1,1> >("pulseData");
  pulseData[0]=1.;  // amplitude
  pulseData[1]=5.;  // exponent
  pulseData[2]=0.;  // center x0
  pulseData[3]=0.;  // center x1
  pulseData[4]=0.;  // center x2
  pulseData[5]=1.;  // velocity v0
  pulseData[6]=1.;  // velocity v1
  pulseData[7]=1.;  // velocity v2
  pulseData[8]=1.;  // power
  


  // incompressible NS:
  dbase.get<bool >("useImplicitFourthArtificialDiffusion")=false;
  dbase.get<bool >("includeArtificialDiffusionInPressureEquation")=false;

  dbase.get<bool >("useSecondOrderArtificialDiffusion")=false;  
  dbase.get<bool >("useFourthOrderArtificialDiffusion")=false;
  dbase.get<bool >("useSixthOrderArtificialDiffusion")=false;

  dbase.get<int >("useSplitStepImplicitArtificialDiffusion")=false;
  
  
  dbase.get<real >("anl")=1.;      // coefficient of advection terms
  dbase.get<real >("ad21")=2.;     // coeff's for 2nd order artificial viscosity based
  dbase.get<real >("ad22")=2.;     // on the minimum scale result
  dbase.get<real >("ad41")=2.;     // coeff's for 4th order artificial viscosity based
  dbase.get<real >("ad42")=2.;     // on the minimum scale result
  dbase.get<real >("ad61")=2.;     // coeff's for 6th order artificial viscosity based
  dbase.get<real >("ad62")=2.;     // on the minimum scale result

  dbase.get<real >("ad21n")=2.;    // for SPAL turbulence model
  dbase.get<real >("ad22n")=2.;
  dbase.get<real >("ad41n")=0.;
  dbase.get<real >("ad42n")=0.;

  dbase.get<real >("cdv")=1.;      // cell volume factor for divergence damping
  dbase.get<real >("cDt")=.25;     // time step factor for divergence damping
  dbase.get<real >("divDampingImplicitTimeStepReductionFactor")=1.;  // reduce  dbase.get<real >("dt") for implicit time stepping to account for div-damping
  
  dbase.get<real >("dampingDt")=-1.;  //  dbase.get<real >("dt") when the divergence damping was last computed.

  dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer")=NULL;  // for 4th order INS BC's

  // kkc 070126 BILL : where should we move this documentation ?
  //\begin{>cgBoundaryConditionsInclude.tex}{}
  //\no function header:
  // {\footnotesize
  // \begin{verbatim}
  //   enum BoundaryCondition
  //   {
  //     interpolation=0,
  //     noSlipWall,
  //     inflowWithVelocityGiven,
  //     inflowWithPressureAndTangentialVelocityGiven,
  //     slipWall,
  //     outflow,
  //     superSonicInflow,
  //     superSonicOutflow,
  //     subSonicInflow,
  //     subSonicInflow2,
  //     subSonicOutflow,
  //     symmetry,
  //     dirichletBoundaryCondition,
  //     axisymmetric,
  //     convectiveOutflow,
  //     tractionFree,
  //     numberOfBCNames     // counts number of entries
  //   };
  // \end{verbatim}
  // }
  //\end{cgBoundaryConditionsInclude.tex}

  registerBC((int)dirichletBoundaryCondition,"dirichletBoundaryCondition");
  registerBC((int)neumannBoundaryCondition,"neumannBoundaryCondition");
  registerBC((int)symmetry,"symmetry");
  registerBC((int)interfaceBoundaryCondition,"interfaceBoundaryCondition");
  registerBC((int)axisymmetric,"axisymmetric");
  // kkc 070126 BILL : the following 3 bc integers would have been "invalid" in the original code
  registerBC((int)interpolation,"interpolation");
  registerBC((int)dirichletInterface,"dirichletInterface");
  registerBC((int)neumannInterface,"neumannInterface");

  registerBC((int)noSlipWall,"noSlipWall");
  registerBC((int)slipWall,"slipWall");
  registerBC((int)noSlipWallInterface,"noSlipWallInterface");
  registerBC((int)slipWallInterface,"slipWallInterface");

  registerBC((int)penaltyBoundaryCondition,"penaltyBoundaryCondition");

  registerBC((int)freeSurfaceBoundaryCondition,"freeSurfaceBoundaryCondition");

  // here are the interface types
  registerInterfaceType((int)noInterface,"noInterface");
  registerInterfaceType((int)heatFluxInterface,"heatFluxInterface");
  registerInterfaceType((int)tractionInterface,"tractionInterface");
  registerInterfaceType((int)tractionAndHeatFluxInterface,"tractionAndHeatFluxInterface");

  dbase.get<realCompositeGridFunction* >("forcingFunction")=0;
  dbase.get<Parameters::ForcingType >("forcingType")=noForcing;

  dbase.get<real >("minimumNumberOfGrids")=REAL_MAX;
  dbase.get<real >("maximumNumberOfGrids")=0.;
  dbase.get<real >("totalNumberOfGrids")=0.;
  dbase.get<real >("minimumNumberOfGridPoints")=REAL_MAX;
  dbase.get<real >("maximumNumberOfGridPoints")=0.;
  dbase.get<real >("sumTotalNumberOfGridPoints")=0.;
  dbase.get<real >("numberOfRegrids")=0.;
  
  dbase.get<ListOfEquationDomains* >("pEquationDomainList")=NULL; 
  dbase.get<SurfaceEquation* >("pSurfaceEquation")=NULL;
}

// ===================================================================================================================
/// \brief Add a new item to be timed.
/// \timeVariableName : name of the variable in the dbase
/// \timeLabel : label to be used on output.
/// \return value: the item's index in the timing array.
// ===================================================================================================================
int Parameters::
addTiming( const aString & timeVariableName, const aString & timeLabel )
{
  if( !dbase.has_key(timeVariableName) ) dbase.put<int>(timeVariableName,0);  
  
  std::vector<aString> & timingName = dbase.get<std::vector<aString> >("timingName");
  timingName.push_back(timeLabel);

  dbase.get<int>(timeVariableName)=timingName.size()-1;

  return timingName.size()-1;
}

// ===================================================================================================================
/// \brief Define the items that will be timed (this is a virtual function that may be overloaded by derived classes)
// ===================================================================================================================
int Parameters::
initializeTimings()
{
  addTiming("totalTime",                         "total time");
  addTiming("timeForInitialize",                 "setup and initialize");
  addTiming("timeForAdvance",                    "advance");
  addTiming("timeForGetUt",                      "  compute du/dt");
  addTiming("timeForAddUt",                      "  add on du/dt");
  addTiming("timeForAFSrhs",                     "  AFS right hand side");
  addTiming("timeForAFSforcing",                 "  AFS forcing");
  addTiming("timeForAFSlhs",                     "  AFS left hand side");
  addTiming("timeForForcing",                    "  add forcing");
  addTiming("timeForBoundaryConditions",         "  boundary conditions");
  addTiming("timeForInterpolate",                "  interpolation");
  addTiming("timeForUpdateGhostBoundaries",      "  update ghost boundaries");
  addTiming("timeForMovingGrids",                "  moving grids");
  addTiming("timeForGridGeneration",             "    grid generation (ogen)");
  addTiming("timeForMovingUpdate",               "  moving update grids,gf");
  addTiming("timeForUpdateOperators",            "  moving update operators");
  addTiming("timeForUpdateInterpolant",          "  moving update Interpolant");
  addTiming("timeForInterpolateExposedPoints",   "  interpolate exposed pts");
  addTiming("timeForImplicitSolve",              "  implicit time stepping");
  addTiming("timeForComputingDeltaT","  compute dt");
  addTiming("timeForLineImplicit","  line implicit");
  addTiming("timeForLineImplicitSolve","    factor tridiagonal");
  addTiming("timeForLineImplicitFactor","    solve tridiagonal");
  addTiming("timeForLineImplicitResidual","    compute residual");
  addTiming("timeForLineImplicitJacobian","      setup jacobian");
  addTiming("timeForLineImplicitSetupA","      setup a");
  addTiming("timeForTimeIndependentVariables","  time independent variables");
  addTiming("timeForUpdatePressureEquation","    update pressure equation");
  addTiming("timeForPressureSolve","    pressure solve");
  addTiming("timeForAssignPressureRHS","    assign pressure rhs");
  addTiming("timeForAmrRegrid","  AMR regrid");
  addTiming("timeForAmrErrorFunction","    compute error function");
  addTiming("timeForAmrRegridBaseGrids","    regrid base grids");
  addTiming("timeForAmrRegridOverlap","    regrid overlap");
  addTiming("timeForAmrUpdate","    update grids and functions");
  addTiming("timeForAmrInterpolateRefinements","    interpolate refinements");
  addTiming("timeForAmrBoundaryConditions","    boundary conditions");
  addTiming("timeForInterfaces","  interfaces");
  addTiming("timeForPlotting","  plotting");
  addTiming("timeForShowFile","  showFile");
  addTiming("timeForOther","other");
  addTiming("timeForWaiting"," waiting (not counted)");            

  printF("*** Parameters::initializeTimings: totalTime=%i timeForAdvance=%i timeForMovingGrids=%i\n",
           dbase.get<int>("totalTime"), dbase.get<int>("timeForAdvance"), dbase.get<int>("timeForMovingGrids"));

  int & maximumNumberOfTimings = dbase.get<int>("maximumNumberOfTimings");
  maximumNumberOfTimings = dbase.get<std::vector<aString> >("timingName").size();

  dbase.get<RealArray >("timing").redim(maximumNumberOfTimings);
  dbase.get<RealArray >("timing")=0.;

  return 0;
}



// ===================================================================================================================
/// \brief destructor.
// ===================================================================================================================
Parameters::
~Parameters()
{
  fclose( dbase.get<FILE* >("debugFile"));
  if( Communication_Manager::numberOfProcessors()>1 )
    fclose( dbase.get<FILE* >("pDebugFile"));
  fclose( dbase.get<FILE* >("checkFile"));
  fclose( dbase.get<FILE* >("logFile"));
  fclose( dbase.get<FILE* >("moveFile"));
  
  delete  dbase.get<OGFunction* >("exactSolution");
  delete []  dbase.get<aString* >("componentName");
  delete []  dbase.get<aString* >("showVariableName");

  delete []  dbase.get<aString* >("namesOfStatistics");
  
  delete  dbase.get<Ogshow* >("show");

  for( int n=0; n<maximumNumberOfOutputFiles; n++ )
    delete  dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[n];

  delete  dbase.get<Regrid* >("regrid");
  delete  dbase.get<InterpolateRefinements* >("interpolateRefinements");
  delete  dbase.get<ErrorEstimator* >("errorEstimator");
  delete  dbase.get<realCompositeGridFunction* >("truncationError");
  
  delete  dbase.get<realCompositeGridFunction* >("bodyForce");

  delete  dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer");
  
  delete  dbase.get<Ogen* >("gridGenerator");
#ifdef USE_PPP
  delete  dbase.get<ParallelOverlappingGridInterpolator* >("pParallelInterpolator");
#endif
  delete  dbase.get<GUIState* >("runTimeDialog");
  
  delete  dbase.get<realCompositeGridFunction* >("pDistanceToBoundary");
  delete  dbase.get<realCompositeGridFunction* >("pVariableCoefficients");
  
  delete  dbase.get<ListOfEquationDomains* >("pEquationDomainList"); 
  delete  dbase.get<SurfaceEquation* >("pSurfaceEquation");

  if ( dbase.get<GridFunctionFilter*>("gridFunctionFilter") ) delete dbase.get<GridFunctionFilter*>("gridFunctionFilter");

  if( dbase.has_key("probeList") )
  {
    // delete probes (this will close the files)
    // printF("Parameters:: deleting the probes...\n");
    std::vector<ProbeInfo*> & probeList = dbase.get<std::vector<ProbeInfo*> >("probeList");
    for( int i=0; i<probeList.size(); i++ )
      delete probeList[i];
  }

  // moved from ~CnsParameters *wdh* 100808
  if ( dbase.get<realCompositeGridFunction* >("pKnownSolution") )
    delete  dbase.get<realCompositeGridFunction* >("pKnownSolution");

  if( dbase.has_key("externalBoundaryData") ) 
  {
    delete dbase.get<ExternalBoundaryData*>("externalBoundaryData");
  }
  
  for (  BCModifierIterator i=bcModifiers.begin(); i!=bcModifiers.end(); i++ )
  {
    delete (i->second);
  }

  if( dbase.has_key("bodyForceMaskGridFunction") )
  {
    delete dbase.get<realCompositeGridFunction*>("bodyForceMaskGridFunction");
  }
  if( dbase.has_key("bodyForceMaskCompositeGrid") )
  {
    delete dbase.get<realCompositeGridFunction*>("bodyForceMaskCompositeGrid");
  }
  
  if( dbase.has_key("bodyForcings") )
  {
    // Here is the array of body forcings:
    std::vector<BodyForce*> & bodyForcings =  dbase.get<std::vector<BodyForce*> >("bodyForcings"); 
    for( int bf=0; bf<bodyForcings.size(); bf++ )
      delete bodyForcings[bf];
  }
  

}

// real Parameters::
// getMaxValue(real value, int processor /* = -1 */)
// {
//   real maxValue=value;
//   #ifdef USE_PPP 
//   if( processor==-1 )
//     MPI_Allreduce(&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
//   else
//     MPI_Reduce        (&value, &maxValue, 1, MPI_DOUBLE, MPI_MAX, processor, MPI_COMM_WORLD);
//   #endif
//   return maxValue;
// }

// int Parameters::
// getMaxValue(int value, int processor /* = -1 */)
// {
//   int maxValue=value;
//   #ifdef USE_PPP 
//   if( processor==-1 )
//     MPI_Allreduce(&value, &maxValue, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
//   else
//     MPI_Reduce        (&value, &maxValue, 1, MPI_INT, MPI_MAX, processor, MPI_COMM_WORLD);
//   #endif
//   return maxValue;
// }

// real Parameters::
// getMinValue(real value, int processor /* = -1 */ )
// {
//   real minValue=value;
//   #ifdef USE_PPP 
//   if( processor==-1 )
//     MPI_Allreduce(&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
//   else
//     MPI_Reduce        (&value, &minValue, 1, MPI_DOUBLE, MPI_MIN, processor, MPI_COMM_WORLD);
//   #endif
//   return minValue;
// }

// int Parameters::
// getMinValue(int value, int processor /* = -1 */)
// {
//   int minValue=value;
//   #ifdef USE_PPP 
//   if( processor==-1 )
//     MPI_Allreduce(&value, &minValue, 1, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
//   else
//     MPI_Reduce        (&value, &minValue, 1, MPI_INT, MPI_MIN, processor, MPI_COMM_WORLD);
//   #endif
//   return minValue;
// }

// ===================================================================================================================
/// \brief Open the log files.
/// \param name (input) : name for the prefix of the log files: name.debug, name.check, name.log, name.move.
///
// ===================================================================================================================
int Parameters::
openLogFiles(const aString & name)
{
  FILE *& debugFile =dbase.get<FILE* >("debugFile");
  FILE *& checkFile =dbase.get<FILE* >("checkFile");
  FILE *& logFile =dbase.get<FILE* >("logFile");
  FILE *& moveFile =dbase.get<FILE* >("moveFile");
  FILE *& pDebugFile =dbase.get<FILE* >("pDebugFile");

  assert(  debugFile==NULL &&  checkFile==NULL &&  logFile==NULL && moveFile==NULL );
  aString buff;
  const int np= max(1,Communication_Manager::numberOfProcessors());
#ifndef USE_PPP
  debugFile = fopen(sPrintF(buff,"%s.debug",(const char*)name),"w" );      // Here is the debug file
  pDebugFile= debugFile;
#else
  debugFile = fopen(sPrintF(buff,"%sNP%i.debug",(const char*)name,np),"w" );  // Here is the debug file
  dbase.get<FILE* >("pDebugFile")= fopen(sPrintF(buff,"%s%i.debug",(const char*)name, dbase.get<int >("myid")),"w");
#endif

  checkFile = fopen(sPrintF(buff,"%s.check",(const char*)name),"w" );   // Here is the check file for regression tests
  logFile   = fopen(sPrintF(buff,"%s.log",(const char*)name),"w" );     // Here is the log file
  moveFile  = fopen(sPrintF(buff,"%s.move",(const char*)name),"w");     // Info from moving grids

  return 0;
}


// ===================================================================================================================
/// \brief specify which file to write informational messages to.
/// \details Normally informational messages are sent to stdout. This can be changed by supplying a file to write to.
///
/// \param file (input) : file to write info messages to.
// ==================================================================================================================
int Parameters::
setInfoFile(FILE *file)
{
  assert( file!=NULL );
   dbase.get<FILE* >("infoFile")=file;

  return 0;
}


// ===================================================================================================================
/// \brief build the AMR error estimator.
// ==================================================================================================================
int Parameters::
buildErrorEstimator()
{
  if(  dbase.get<ErrorEstimator* >("errorEstimator")==NULL )
  {
    if(  dbase.get<InterpolateRefinements* >("interpolateRefinements")==NULL )
    {
       dbase.get<InterpolateRefinements* >("interpolateRefinements") = new InterpolateRefinements( dbase.get<int >("numberOfDimensions"));
      assert(  dbase.get<InterpolateRefinements* >("interpolateRefinements")!=NULL );
    }
     dbase.get<ErrorEstimator* >("errorEstimator") = new ErrorEstimator(* dbase.get<InterpolateRefinements* >("interpolateRefinements"));
    assert(  dbase.get<ErrorEstimator* >("errorEstimator")!=NULL );
  }
  return 0;
}


int 
boundaryDistance(CompositeGrid & cg, realCompositeGridFunction & d, const IntegerArray & wall );

// ===================================================================================================================
/// \brief Update the parameters when the grid has changed.
/// \param cg (input) : new CompositeGrid
/// \param sharedBoundaryCondition (input) : sharedBoundaryCondition(side,axis,grid) : = side2+2*(axis2+3*grid2) : match to (side2,axis2,grid2)
// ==================================================================================================================
int Parameters::
updateToMatchGrid( CompositeGrid & cg, 
		   IntegerArray & sharedBoundaryCondition /* = Overture::nullIntArray() */ )
{
  //  dbase.get<IntegerArray >("variableBoundaryData")(grid) is true if variable BC data is required; such as 
  //  a parabolic inflow profile
  // kkc 070130 this had to be changed since DomainSolver::setParametersInteractively had a reorg that made this break  int oldNumber= dbase.get<IntegerArray >("variableBoundaryData").getLength(0);
  int oldNumber= dbase.get<IntegerArray >("bcInfo").getLength(3);
  int newNumber=cg.numberOfComponentGrids();
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  const int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  
  
  RealArray & bcData = dbase.get<RealArray >("bcData");
  RealArray & bcParameters = dbase.get<RealArray >("bcParameters");
  IntegerArray & bcInfo = dbase.get<IntegerArray >("bcInfo");
  IntegerArray & interfaceType = dbase.get<IntegerArray >("interfaceType");

  dbase.get<IntegerArray >("variableBoundaryData").resize(cg.numberOfComponentGrids());

  Range R(oldNumber,max(oldNumber,newNumber-1));
  if( newNumber>oldNumber )
     dbase.get<IntegerArray >("variableBoundaryData")(R)=false;  // *** this may not be correct for refinements added!
  
   dbase.get<IntegerArray >("gridIsImplicit").resize(cg.numberOfComponentGrids());
  if( newNumber>oldNumber )
     dbase.get<IntegerArray >("gridIsImplicit")(R)=0;  // 1,2 if  a grid is time stepped with an implicit method.

  // todo: bcType(side,axis,grid),  bcData(n,side,axis,grid)


  Range all;
  // The  bcData array holds the data for each bc on each side of each grid
  // The  bcParameters array holds extra parameters for each bc on each side of each grid
  if( oldNumber==0 )
  {
     // bcData : allocate space for mixed-derivative BC's which means we need numberOfComponents*3 values
     //    Mixed-derivative BC for component i: 
     //          mixedCoeff(i)*u(i) + mixedNormalCoeff(i)*u_n(i) = mixedRHS(i)
#define mixedRHS(component,side,axis,grid)         bcData(component+numberOfComponents*(0),side,axis,grid)
#define mixedCoeff(component,side,axis,grid)       bcData(component+numberOfComponents*(1),side,axis,grid)
#define mixedNormalCoeff(component,side,axis,grid) bcData(component+numberOfComponents*(2),side,axis,grid)
     bcData.redim(max(5, numberOfComponents*3),2, numberOfDimensions,cg.numberOfComponentGrids());
     bcData=0.;
     for( int grid=0; grid<cg.numberOfComponentGrids(); grid++) 
       for( int axis=0; axis<numberOfDimensions; axis++ ) for( int side=0; side<=1; side++ )
	 for( int n=0; n<numberOfComponents; n++ )
	 { // Default is a Dirichlet BC: 
	   mixedRHS(n,side,axis,grid)=0.;
	   mixedCoeff(n,side,axis,grid)=1.;
	   mixedNormalCoeff(n,side,axis,grid)=0.;
           if( dbase.get<int >("debug") & 8 )
	     printF("*** Parameters::updateToMatchGrid: Set bcData for mixed BC (side,axis,grid)=(%i,%i,%i)"
		    " n=%i (%g,%g,%g) (Dirichlet BC)******\n",
                    side,axis,grid,n,mixedRHS(n,side,axis,grid),mixedCoeff(n,side,axis,grid),mixedNormalCoeff(n,side,axis,grid));
	 }

     bcParameters.redim(max(9, numberOfComponents),2, numberOfDimensions,cg.numberOfComponentGrids());
     bcParameters=0.;

     //kkc added one for bc modifiers     bcInfo.redim(2,2,3,cg.numberOfComponentGrids()); 
     bcInfo.redim(3,2,3,cg.numberOfComponentGrids()); 
     bcInfo=0;

     interfaceType.redim(2,3,cg.numberOfComponentGrids());
     interfaceType=noInterface;
  }
  else
  {
    Range Rold(0,min(oldNumber-1,newNumber-1));
    RealArray bcDataOld= bcData(all,all,all,Rold);

    bcData.resize( bcData.dimension(0), bcData.dimension(1), bcData.dimension(2),newNumber);
    if( newNumber>oldNumber )
    {
      bcData(all,all,all,R)=0.;
      for( int grid=oldNumber; grid<cg.numberOfComponentGrids(); grid++) 
	for( int axis=0; axis<numberOfDimensions; axis++ ) for( int side=0; side<=1; side++ )
	  for( int n=0; n<numberOfComponents; n++ )
	  { // Default is a Dirichlet BC: 
	    mixedRHS(n,side,axis,grid)=0.;
	    mixedCoeff(n,side,axis,grid)=1.;
	    mixedNormalCoeff(n,side,axis,grid)=0.;
	  }
    }
     
    bcData(all,all,all,Rold)=bcDataOld;
    RealArray bcParametersOld= bcParameters(all,all,all,Rold);
    bcParameters.resize( bcParameters.dimension(0), bcParameters.dimension(1), bcParameters.dimension(2),newNumber);
    if( newNumber>oldNumber )
      bcParameters(all,all,all,R)=0.;
    bcParameters(all,all,all,Rold)=bcParametersOld;

    IntegerArray bcInfoOld= bcInfo(all,all,all,Rold);
    bcInfo.redim( bcInfo.dimension(0), bcInfo.dimension(1), bcInfo.dimension(2),newNumber);
    if( newNumber>oldNumber )
      bcInfo(all,all,all,R)=0;
    bcInfo(all,all,all,Rold)=bcInfoOld;

    IntegerArray interfaceTypeOld= interfaceType(all,all,Rold);
    interfaceType.redim( interfaceType.dimension(0), interfaceType.dimension(1), newNumber);
    if( newNumber>oldNumber )
      interfaceType(all,all,R)=noInterface;
    interfaceType(all,all,Rold)=interfaceTypeOld;


    // we need to copy parameter values from base grids to new refinement grids
    // ** assign ALL refinement grids ***
    for( int grid=0; grid<newNumber; grid++ )  
    {
      if( cg.refinementLevelNumber(grid)>0 ) // this is a refinement grid 
      {
	int baseGrid = cg.baseGridNumber(grid);
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ ) 
	{
	  for( int side=0; side<=1; side++ )
	  {
	    if( cg[grid].boundaryCondition(side,axis) > 0 )
	    {
              assert( cg[grid].boundaryCondition(side,axis)==cg[baseGrid].boundaryCondition(side,axis) );
	      
	      bcData(all,side,axis,grid)= bcData(all,side,axis,baseGrid);
	      bcParameters(all,side,axis,grid)= bcParameters(all,side,axis,baseGrid);
	      bcInfo(all,side,axis,grid)= bcInfo(all,side,axis,baseGrid);
	      interfaceType(side,axis,grid)= interfaceType(side,axis,baseGrid);
	    }
	    else
	    {  // *wdh* 100803 -- reset the interface type
              interfaceType(side,axis,grid)=noInterface;
	    }
	    
	  }
	}
      }
    }

    // display(sharedBoundaryCondition,"Parameters: sharedBoundaryCondition");
    // **** we need to copy parameters to new grids that hit the boundary *****
    //  ??? how do we know which boundaries to use ?? ****
    if( sharedBoundaryCondition.getLength(2)>= cg.numberOfComponentGrids() )
    {
      for( int grid=oldNumber; grid<newNumber; grid++ )
      {
	for( int axis=0; axis<cg.numberOfDimensions(); axis++ ) 
	{
	  for( int side=0; side<=1; side++ )
	  {
	    if( cg[grid].boundaryCondition(side,axis) > 0 && sharedBoundaryCondition(side,axis,grid)>=0 )
	    {
              // sharedBoundaryCondition(side,axis,grid) = side2+2*(axis2+3*grid2) : match to (side2,axis2,grid2)

	      
	      int grid2=sharedBoundaryCondition(side,axis,grid)/6;
              int dir2=(sharedBoundaryCondition(side,axis,grid)-6*grid2)/2;
              int side2=sharedBoundaryCondition(side,axis,grid) % 2;
	      assert( grid2>=0 && grid2<oldNumber );
	      assert(dir2>=0 && dir2<=2 );
	      
              printF("Parameters: assign BC for new grid %i from old grid %i\n",grid,grid2);

	       bcData(all,side,axis,grid)= bcData(all,side2,dir2,grid2);
	       bcParameters(all,side,axis,grid)= bcParameters(all,side2,dir2,grid2);
	       bcInfo(all,side,axis,grid)= bcInfo(all,side2,dir2,grid2);
	       interfaceType(side,axis,grid)= interfaceType(side2,dir2,grid2);
	    }
	  }
	}
      }
    }
    
  }

  // Now update the parameters for moving grids
  dbase.get<MovingGrids >("movingGrids").updateToMatchGrid(cg);
  
  // kkc 070131 we should only call this if we actually have a known solution  updateKnownSolutionToMatchGrid(cg);
  updateKnownSolutionToMatchGrid(cg); // *wdh* 090412 -- add this back --

  return 0;
}


// ===================================================================================================================
/// \brief Update turbulence models.
/// \details This function will compute the distance to the wall for certain turbulence models..
/// \note Call this routine after the boundary conditions have been defined
// ==================================================================================================================
int Parameters::
updateTurbulenceModels(CompositeGrid & cg)
{

  const Parameters::TurbulenceModel & turbulenceModel = dbase.get<Parameters::TurbulenceModel >("turbulenceModel");
  
  printF("Parameters::updateTurbulenceModels: turbulenceModel=%i\n",turbulenceModel);

  if(  turbulenceModel==SpalartAllmaras ||  turbulenceModel==BaldwinLomax )
  {
    if(  dbase.get<realCompositeGridFunction* >("pVariableCoefficients")==NULL )
       dbase.get<realCompositeGridFunction* >("pVariableCoefficients") = new realCompositeGridFunction(cg);

    (* dbase.get<realCompositeGridFunction* >("pVariableCoefficients")).updateToMatchGrid(cg);
    
    // **** compute the distance to solid walls *****

    if(  dbase.get<realCompositeGridFunction* >("pDistanceToBoundary")==NULL )
       dbase.get<realCompositeGridFunction* >("pDistanceToBoundary") = new realCompositeGridFunction(cg);

    IntegerArray wall(cg.numberOfComponentGrids()*cg.numberOfDimensions()*2,3);
    int nw=0;
    int grid;
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
      {
	for( int side=0; side<=1; side++ )
	{
          int bc=cg[grid].boundaryCondition(side,axis);
	  if( bc==noSlipWall )
	  {
	    wall(nw,0)=grid;
	    wall(nw,1)=side;
	    wall(nw,2)=axis;
	    nw++;
	  }
	}
      }
    }
    if( nw>0 )
      wall.resize(nw,3);
    else
      wall.redim(0);

    boundaryDistance(cg,* dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"), wall );

    if( true )
    {
      GraphicsParameters & psp = dbase.get<GraphicsParameters >("psp");
      GenericGraphicsInterface & ps = *dbase.get<GenericGraphicsInterface* >("ps");

      ps.erase();
      psp.set(GI_TOP_LABEL,"distance to walls");
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      PlotIt::contour(ps,*dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"),psp);
      psp.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    

    // we don't let d get too small at the boundary
    const real dMin=pow(REAL_MIN,.25);
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const intArray & mask = cg[grid].mask();
      
      realArray & d = (* dbase.get<realCompositeGridFunction* >("pDistanceToBoundary"))[grid];
      d=max(dMin,d);
      where( mask==0 )
      {
        d=1.;
      }
    }
    
  }
  return 0;
}


// ===================================================================================================================
/// \brief return true if this is a moving grid problem.
// ==================================================================================================================
bool Parameters::
isMovingGridProblem() const
{
  return  dbase.get<MovingGrids >("movingGrids").isMovingGridProblem();
}

// ===================================================================================================================
/// \brief return the reference frame for the PDEs in a domain.
/// \details 
/// The frame of reference is needed so that we know how to transform the PDE when the grids move. 
/// Often the PDEs are defined in a fixed reference frame (even if some boundaries are moving). 
/// If we are solving for a PDE inside a moving rigid body, then the PDE (e.g. the heat equation) 
/// may be defined in the frame of reference of the rigid body. 
// ==================================================================================================================
const Parameters::ReferenceFrameEnum Parameters::
getReferenceFrame()
{
  return dbase.get<ReferenceFrameEnum>("referenceFrame");
}

// ===================================================================================================================
/// \brief return the name of the time-stepping method
// ==================================================================================================================
aString Parameters::
getTimeSteppingName() const
{
  TimeSteppingMethod & timeSteppingMethod = dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  if( timeSteppingMethod>=0 && timeSteppingMethod<Parameters::numberOfTimeSteppingMethods )
    return timeSteppingName[timeSteppingMethod];
  else
    return "unknown";
}



// ===================================================================================================================
/// \brief return true if this grid is moving.
/// \param grid (input) : grid number.
// ==================================================================================================================
bool Parameters::
gridIsMoving(int grid) const
{
  return  dbase.get<MovingGrids >("movingGrids").gridIsMoving(grid);
}


// ===================================================================================================================
/// \brief Return the boundary condition type, a value from the enum Parameters::BoundaryConditionType
/// \param side (input) : side 
/// \param axis (input) : axis
/// \param grid (input) : grid number.
/// \return return the boundary condition type for a given face of a grid.
// ==================================================================================================================
int Parameters::
bcType(int side, int axis, int grid) const
{
  IntegerArray &bcInfo = dbase.get<IntegerArray >("bcInfo");
  return  bcInfo(0,side,axis,grid);
}

// ===================================================================================================================
/// \brief Return the number of faces where there is a boundary condition of type "bc", from the specified faces.
/// \param side (input) : side 
/// \param axis (input) : axis
/// \param grid (input) : grid number.
/// \param bc (input) : check for this boundary condition.
/// \return return the number of faces where the boundary condition is \a bc
// ==================================================================================================================
int Parameters::
howManyBcTypes(const Index & side, 
	       const Index & axis, 
	       const Index & grid, 
	       BoundaryConditionType bc) const
{
  int numberOfFaces=sum( ( dbase.get<IntegerArray >("bcInfo")(0,side,axis,grid)-(int)bc)==0 );
  return numberOfFaces;
}

// ===================================================================================================================
/// \brief Return true if there are time dependent user boundary condition.
/// \param Side (input) : side(s) 
/// \param Axis (input) : axis(s)
/// \param Grid (input) : grid(s)
// ==================================================================================================================
int Parameters::
thereAreTimeDependentUserBoundaryConditions(const Index & Side, 
					    const Index & Axis, 
					    const Index & Grid ) const  
{
  const int numberOfGrids= dbase.get<IntegerArray >("bcInfo").getLength(3);
  Index S = Side.getLength()<=0 ? Index(0,2) : Side;
  Index A = Axis.getLength()<=0 ? Index(0,3) : Axis;
  Index G = Grid.getLength()<=0 ? Index(0,numberOfGrids) : Grid;
  
  for(int side=S.getBase(); side<=S.getBound(); side++ )
  {
    for(int axis=A.getBase(); axis<=A.getBound(); axis++ )
    {
      for(int grid=G.getBase(); grid<=G.getBound(); grid++ )
      {
	// if(  dbase.get<IntegerArray >("bcInfo")(0,side,axis,grid)>=numberOfPredefinedBoundaryConditionTypes && bcIsTimeDependent(side,axis,grid) )
        // *wdh* 2015/03/27 -- do this for now *fix me*
	if( bcIsTimeDependent(side,axis,grid) )
	{
	  return true;
	}
      }
    }
  }
  return false;
}


// ===================================================================================================================
/// \brief Set the boundary condition type for a particular side.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param bc (input) : boundary condition type.
// ==================================================================================================================
int Parameters::
setBcType(int side, int axis, int grid, BoundaryConditionType bc)
{
   dbase.get<IntegerArray >("bcInfo")(0,side,axis,grid)=bc;
  return 0;
}


// OLD *wdh* 2015/03/27 
// // ===================================================================================================================
// /// \brief Return the user defined boundary condition type.
// /// \param side (input) : side
// /// \param axis (input) : axis
// /// \param grid (input) : grid
// /// \return return the boundary condition type for a given face of a grid.
// // ===================================================================================================================
// int Parameters::
// userBcType(int side, int axis, int grid) const
// {
//   return  dbase.get<IntegerArray >("bcInfo")(0,side,axis,grid)-numberOfPredefinedBoundaryConditionTypes;
// }


// OLD *wdh* 2015/03/27 
// // ===================================================================================================================
// /// \brief Set the user defined boundary condition type for a particular side.
// /// \param side (input) : side
// /// \param axis (input) : axis
// /// \param grid (input) : grid
// /// \param bc (input) : boundary condition number.
// // ==================================================================================================================
// int Parameters::
// setUserBcType(int side, int axis,int grid, int bc)
// {
  
//   dbase.get<IntegerArray >("bcInfo")(0,side,axis,grid)=bc+numberOfPredefinedBoundaryConditionTypes;
//   return 0;
// }

// ===================================================================================================================
/// \brief Return true if the boundary face has a boundary condition that varies in space.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \return return true if the boundary face has a boundary condition that varies in space.
// ===================================================================================================================
int Parameters::
bcVariesInSpace(int side, int axis, int grid) const
{
  return  dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) & 2;
}

// ===================================================================================================================
/// \brief Return true if any of a set of boundary faces has a boundary condition that varies in space.
/// \param side (input) : side(s)
/// \param axis (input) : axis(s)
/// \param grid (input) : grid(s)
/// \return return true if any of a set of boundary faces has a boundary condition that varies in space.
// ===================================================================================================================
int Parameters::
bcVariesInSpace(const Index & side /*= nullIndex */, 
		const Index & axis /*= nullIndex */, 
		const Index & grid /*= nullIndex */ ) const
{
  return max(  dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) & 2 ); 
}
  
// ===================================================================================================================
/// \brief Specify whether a boundary face has a boundary condition that varies in space.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param trueOrFalse (input) : true if the boundary face has a boundary condition that varies in space.
// ===================================================================================================================
int Parameters:: 
setBcVariesInSpace(int side, int axis, int grid, bool trueOrFalse /* = true */ )
{
//  bcInfo(1,side,axis,grid) -= (bcInfo(1,side,axis,grid) % 2)*2;
//  bcInfo(1,side,axis,grid) += 2*trueOrFalse;

  if( trueOrFalse )
    dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) |= 2;
  else
    dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) &= ~2;
  
  dbase.get<IntegerArray >("variableBoundaryData")(grid)=trueOrFalse;  // do this while converting
  
  return 0;
}

// ===================================================================================================================
/// \brief Return true if the bc value corresponds to an interface boundary condition.
/// \param bc (input) : boundary condition.
/// \return Return true if the bc value corresponds to an interface boundary condition.
// ===================================================================================================================
int Parameters::
bcIsAnInterface(int bc) const
{
  return bc==interfaceBoundaryCondition ||
         bc==noSlipWallInterface        ||  
         bc==slipWallInterface          ||
         bc==dirichletInterface         ||
         bc==neumannInterface;
}


// ===================================================================================================================
/// \brief Return true if the boundary face has a boundary condition that varies in time.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \return return true if the boundary face has a boundary condition that varies in time.
// ===================================================================================================================
int Parameters::
bcIsTimeDependent(int side, int axis, int grid  ) const
{
  const IntegerArray & bcInfo = dbase.get<IntegerArray >("bcInfo");
  return bcInfo(1,side,axis,grid) & 4;
}
// ===================================================================================================================
/// \brief Return true if ANY boundary face on a grid has a boundary condition that varies in time.
/// \param grid (input) : check this grid 
/// \return return true if there is ANY boundary face that has a boundary condition that varies in time.
// ===================================================================================================================
int Parameters::
bcIsTimeDependent( int grid ) const
{
  const IntegerArray & bcInfo = dbase.get<IntegerArray >("bcInfo");
  const int numberOfDimensions= dbase.get<int>("numberOfDimensions");
  for( int side=0; side<=1; side++ )
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      if( bcInfo(1,side,axis,grid) & 4 )
	return true; // there is at least one face on this grid with a time-dependent BC
    }
  }
  
  return false;  // there are no faces  with a time-dependent BC
}



// ===================================================================================================================
/// \brief Specify whether a boundary face has a boundary condition that varies in time.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param trueOrFalse (input) : true if the boundary face has a boundary condition that varies in time.
// ===================================================================================================================
int Parameters::
setBcIsTimeDependent(int side, int axis, int grid, bool trueOrFalse /* = true */)
{
  if( trueOrFalse )
     dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) |= 4;
  else
     dbase.get<IntegerArray >("bcInfo")(1,side,axis,grid) &= ~4;
  
  return 0;
}


// ===================================================================================================================
/// \brief Assign user defined boundary values.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param values (input) : boundary condition values.
// ===================================================================================================================
int Parameters::
setUserBoundaryConditionParameters(int side, int axis, int grid, RealArray & values)
{
  if( values.getLength(0)==0 )
    return 0;
  
  Range R=values.getLength(0);
  if( values.getLength(0) >  dbase.get<RealArray >("userBoundaryConditionParameters").getLength(0) )
  {
    int numberOfGrids= dbase.get<RealArray >("bcParameters").getLength(3);
    dbase.get<RealArray >("userBoundaryConditionParameters").resize(R,2,3,numberOfGrids);
  }
  dbase.get<RealArray >("userBoundaryConditionParameters")(R,side,axis,grid)=values;

  return 0;
}

// ===================================================================================================================
/// \brief Retrieve user defined boundary values.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param values (output) : boundary condition values (on input this array must be the correct length).
// ===================================================================================================================
int Parameters::
getUserBoundaryConditionParameters(int side, int axis, int grid, RealArray & values) const
{
  if( values.getLength(0)==0 )
    return 0;
  
  Range R=values.getLength(0);
  if( values.getLength(0)> dbase.get<RealArray >("userBoundaryConditionParameters").getLength(0) )
  {
    printF("Parameters::getUserBoundaryConditionParameters:ERROR: requesting too many parameters\n");
    Overture::abort("error");
  }
  if ( grid>= dbase.get<RealArray >("userBoundaryConditionParameters").getLength(3) ) 
    values= dbase.get<RealArray >("userBoundaryConditionParameters")(R,side,axis,0); // adaptive grids  mess this up
  else
    values= dbase.get<RealArray >("userBoundaryConditionParameters")(R,side,axis,grid);
  return 0;
}

// ===================================================================================================================
/// \brief Assign time dependent boundary values.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param values (input) : boundary condition values.
// ===================================================================================================================
int Parameters::
setTimeDependenceBoundaryConditionParameters(int side, int axis, int grid, RealArray & values)
{
  if( values.getLength(0)==0 )
    return 0;
  
   Range R=values.getLength(0);
   if( values.getLength(0) >  dbase.get<RealArray >("timeDependenceBoundaryConditionParameters").getLength(0) )
   {
     int numberOfGrids= dbase.get<RealArray >("bcParameters").getLength(3);
      dbase.get<RealArray >("timeDependenceBoundaryConditionParameters").resize(R,2,3,numberOfGrids);
   }
    dbase.get<RealArray >("timeDependenceBoundaryConditionParameters")(R,side,axis,grid)=values;

   return 0;
}

// ===================================================================================================================
/// \brief Retrieve time dependent boundary values.
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param values (output) : boundary condition values (on input this array must be the correct length).
// ===================================================================================================================
int Parameters::
getTimeDependenceBoundaryConditionParameters(int side, int axis, int grid, RealArray & values) const
{
  if( values.getLength(0)==0 )
    return 0;
  
  Range R=values.getLength(0);
  if( values.getLength(0)> dbase.get<RealArray >("timeDependenceBoundaryConditionParameters").getLength(0) )
  {
    printF("Parameters::getTimeDependenceBoundaryConditionParameters:ERROR: requesting too many parameters\n");
    printF("Requesting %i values but there are only %i available\n",values.getLength(0),
	    dbase.get<RealArray >("timeDependenceBoundaryConditionParameters").getLength(0));
    
    Overture::abort("error");
  }
  values= dbase.get<RealArray >("timeDependenceBoundaryConditionParameters")(R,side,axis,grid);
  return 0;
}

// ===================================================================================================================
/// \brief Add a show variable name to the list of possible show file variables.
/// \param name (input) : name to give the show variable.
/// \param component (input) : the component number of this variable (if it is a computational variable), otherwise
///   a positive integer larger than any component number.
/// \param variableIsOn (input) : if true this variable will be saved in the show file. If false
/// the variable will not be saved by default but the user can change this.
/// \note
///    showVariable(i) >0 if we are to save showVariableName[i], <0 we do not save.
//     showVariableName[] names of possible variables to save in the show file, NULL terminated.
// ===================================================================================================================
int Parameters::
addShowVariable( const aString & name, int component, bool variableIsOn /* = true */ )
{
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  int & numberOfShowVariables = dbase.get<int >("numberOfShowVariables");
  
  if(  numberOfShowVariables >=  dbase.get<int >("maxNumberOfShowVariables")-1 )
  {
    dbase.get<int >("maxNumberOfShowVariables")=max( numberOfComponents, numberOfShowVariables)+10;
    aString *temp = new aString[ dbase.get<int >("maxNumberOfShowVariables")]; 
    int i;
    for( i=0; i< numberOfShowVariables; i++ )   // copy any existing names
      temp[i]= dbase.get<aString* >("showVariableName")[i];
    for( i= numberOfShowVariables; i< dbase.get<int >("maxNumberOfShowVariables"); i++ )
      temp[i]="";
    
    dbase.get<IntegerArray >("showVariable").resize( dbase.get<int >("maxNumberOfShowVariables")); 
    dbase.get<IntegerArray >("showVariable")(Range( numberOfShowVariables, dbase.get<int >("maxNumberOfShowVariables")-1))=-1;
    
    delete []  dbase.get<aString* >("showVariableName");
    dbase.get<aString* >("showVariableName")=temp;
  }
  dbase.get<aString* >("showVariableName")[ numberOfShowVariables]=name;
  dbase.get<IntegerArray >("showVariable")( numberOfShowVariables)=variableIsOn ? component : -component;
  numberOfShowVariables++;
  
  return 0;
}

// ===================================================================================================================
/// \brief Return the component number of a show variable with a given name. Also return whether the variable is
///     saved in the show file.
/// \param name (input) : name of the show variable to lookup.
/// \param component (output) : the component number of this show variable (return -1 if the name is not found)
/// \param variableIsOn (output) : if true this variable will be saved in the show file. If false
/// the variable will not be saved by default but the user can change this.
// ===================================================================================================================
int Parameters::
getShowVariable( const aString & name, int & component, bool & variableIsOn ) const 
{
  int & numberOfShowVariables = dbase.get<int >("numberOfShowVariables");
  
  component=-1;
  variableIsOn=false;
  const aString *showVariableName = dbase.get<aString* >("showVariableName");
  const IntegerArray & showVariable = dbase.get<IntegerArray >("showVariable");
  for( int i=0; i< numberOfShowVariables; i++ )
  {
    if( showVariableName[i] == name )
    {
      component = showVariable(i);
      variableIsOn = component >=0 ;
      component =abs(component);
      break;
    }
  }
  
  return 0;
}

// ===================================================================================================================
/// \brief Turn on or off the saving of a variable in the show file.
///     saved in the show file.
/// \param name (input) : name of the show variable to lookup.
/// \param variableIsOn (input) : if true this variable will be saved in the show file. If false
/// the variable will not be saved by default but the user can change this.
// ===================================================================================================================
int Parameters::
setShowVariable( const aString & name, const bool variableIsOn )
{
  int & numberOfShowVariables = dbase.get<int >("numberOfShowVariables");
  
  const aString *showVariableName = dbase.get<aString* >("showVariableName");
  const IntegerArray & showVariable = dbase.get<IntegerArray >("showVariable");
  bool found=false;
  for( int i=0; i< numberOfShowVariables; i++ )
  {
    if( showVariableName[i] == name )
    {
      showVariable(i) = variableIsOn ? abs(showVariable(i)) : -abs(showVariable(i));
      found=true;
      break;
    }
  }
  if( !found )
  {
    printF("Parameters::setShowVariable:ERROR: name=[%s] not found as a show variable\n",(const char*)name);
    return 1;
  }
  else  
    return 0;
}


// ===================================================================================================================
/// \brief Return 1 or 2 if the grid is integrated implicitity.
/// \details This requires that both the time stepping method is an implicit one and that the grid was chosen to
///      be implicit.
/// \param grid (input) : grid
/// \return return 1=implicit, 2= semi-implicit
// ===================================================================================================================
int Parameters::
getGridIsImplicit(int grid) const
{
  return ( dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==implicit || 
           dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==implicitAllSpeed || 
           dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==implicitAllSpeed ||
           dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==steadyStateRungeKutta ||
	   dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==steadyStateNewton ) &&  dbase.get<IntegerArray >("gridIsImplicit")(grid);
}


// ===================================================================================================================
/// \brief Specify if this grid should be integrated implicitly when an implicit time stepping  method is used.
///
/// \param grid (input) : grid to set, -1=set all
/// \param value (input) : 1=implicit, 2=semi-implicit, 0 = not-implicit
// ==================================================================================================================
int Parameters::
setGridIsImplicit(int grid /* =-1 */, int value /* =1 */ ) 
{
  if( grid<0 )
  {
     dbase.get<IntegerArray >("gridIsImplicit")=value;
  }
  else
  {
    assert( grid>= dbase.get<IntegerArray >("gridIsImplicit").getBase(0) && grid<= dbase.get<IntegerArray >("gridIsImplicit").getBound(0) );
     dbase.get<IntegerArray >("gridIsImplicit")(grid)=value;
  }
  
  return 0;
}


// ===================================================================================================================
/// \brief initialize the parameters.
///
/// \param numberOfDimensions0 (input) : number of dimensions.
/// \param reactionName (input) : optional name of a reaction oe a reaction 
///     file that defines the chemical reactions, such as a Chemkin binary file. 
// ==================================================================================================================
int Parameters::
setParameters(const int & numberOfDimensions0 /* =2 */ , 
              const aString & reactionName /* =nullString */ )
{
  dbase.get<int >("numberOfDimensions")=numberOfDimensions0;

  dbase.get<int >("rc")= dbase.get<int >("uc")= dbase.get<int >("vc")= dbase.get<int >("wc")= dbase.get<int >("pc")= 
   dbase.get<int >("tc")= dbase.get<int >("sc")= dbase.get<int >("kc")= dbase.get<int >("epsc")= dbase.get<int >("sec")=-1;
  
   dbase.get<aString >("reactionName")=reactionName;
  if(  dbase.get<aString >("reactionName")!=nullString &&  dbase.get<aString >("reactionName")!="" )
  {
     dbase.get<bool >("computeReactions")=true;
    // This next function will assign the number of species and build  a reaction object.
    buildReactions();
  }
  else
  {
     dbase.get<bool >("computeReactions")=false;
     dbase.get<Reactions* >("reactions")=NULL;
     dbase.get<int >("numberOfSpecies")=0;
    
  }

  // kkc set some reasonable defaults (note this is what the default constructor for Parameters would have caused
  dbase.get<int >("numberOfComponents")= dbase.get<int >("numberOfDimensions");
  dbase.get<int >("uc")=0;    
  if(  dbase.get<int >("numberOfDimensions")>1 )  dbase.get<int >("vc")=1;
  if(  dbase.get<int >("numberOfDimensions")>2 )  dbase.get<int >("wc")=2;
  dbase.get<Range >("Ru")=Range( dbase.get<int >("uc"),dbase.get<int >("uc")+ dbase.get<int >("numberOfDimensions")-1);    // velocity components
  dbase.get<Range >("Rt")= dbase.get<Range >("Ru");              // time dependent components
  
  //     equationNumber.redim(numberOfComponents);
  //     for (i=0; i<numberOfComponents; i++) equationNumber(i) = i;
  
  addShowVariable( "u", dbase.get<int >("uc") );
  if(  dbase.get<int >("numberOfDimensions")>1 )
    addShowVariable( "v", dbase.get<int >("vc") );
  if(  dbase.get<int >("numberOfDimensions")>2 )
    addShowVariable( "w", dbase.get<int >("wc") );
  
  addShowVariable( "speed", dbase.get<int >("numberOfComponents")+1,false ); // false=turned off by default
  
  if (dbase.get<aString* >("componentName")) delete [] dbase.get<aString* >("componentName");
  dbase.get<aString* >("componentName")= new aString [ dbase.get<int >("numberOfComponents")];

  if(  dbase.get<int >("rc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("rc")]="r";
  if(  dbase.get<int >("uc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("uc")]="u";
  if(  dbase.get<int >("vc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("vc")]="v";
  if(  dbase.get<int >("wc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("wc")]="w";
  if(  dbase.get<int >("pc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("pc")]="p";
  if(  dbase.get<int >("tc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("tc")]="T";
  if(  dbase.get<int >("kc")>=0 )
  {
    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==SpalartAllmaras )
       dbase.get<aString* >("componentName")[ dbase.get<int >("kc")]="n";  // for nuT
    else
       dbase.get<aString* >("componentName")[ dbase.get<int >("kc")]="k";
  }
  
  if(  dbase.get<int >("epsc")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("epsc")]="epsilon";

  int scp =  dbase.get<int >("sc");
  if(  dbase.get<bool >("advectPassiveScalar") )
  {
     dbase.get<aString* >("componentName")[scp]="s";   // use "s" as  a name for now, "passive";
  }
  else if(  dbase.get<int >("numberOfSpecies")>0 )
  {
    int numberOfActiveSpecies=  dbase.get<int >("numberOfSpecies");
    if( numberOfActiveSpecies>0 )
    {
      assert(  dbase.get<Reactions* >("reactions")!=NULL );
      for( int s=0; s<numberOfActiveSpecies; s++ )
	 dbase.get<aString* >("componentName")[scp+s]= dbase.get<Reactions* >("reactions")->getName(s);
    }
    
  }
  
  if(  dbase.get<int >("sec")>=0 )  dbase.get<aString* >("componentName")[ dbase.get<int >("sec")]="Tb";

  if(  dbase.get<int >("numberOfExtraVariables")>0 )
  {
    aString buff;
    for( int e=0; e< dbase.get<int >("numberOfExtraVariables"); e++ )
    {
      int n= dbase.get<int >("numberOfComponents")- dbase.get<int >("numberOfExtraVariables")+e;
       dbase.get<aString* >("componentName")[n]=sPrintF(buff,"Var%i",e);
      addShowVariable(  dbase.get<aString* >("componentName")[n],n );
    }

  }
  

   dbase.get<int >("stencilWidthForExposedPoints")=3;
   dbase.get<int >("extrapolateInterpolationNeighbours")=false;

   dbase.get<real >("inflowPressure")=1.;
   dbase.get<RealArray >("initialConditions").redim( dbase.get<int >("numberOfComponents"));  dbase.get<RealArray >("initialConditions")=defaultValue;
  
   dbase.get<RealArray >("checkFileCutoff").redim( dbase.get<int >("numberOfComponents")+1);  // cutoff's for errors in checkfile
   dbase.get<RealArray >("checkFileCutoff")=REAL_EPSILON*500.;
  //  dbase.get<RealArray >("checkFileCutoff").display("checkFileCutOff");
  
 return 0;
}


// ===================================================================================================================
/// \brief return true if the PDE solver uses conservative variables.
///
/// \param grid (input): if grid!=-1 then check if this grid uses conservative variables, otherwise 
///               check if by default we use conservative variables
// ==================================================================================================================
bool Parameters::
useConservativeVariables(int grid /* =-1 */) const
{
  return false;
}

  
// ===================================================================================================================
/// \brief return true if this is an axisymmetric problem on a 2D grid 
///
// ==================================================================================================================
bool Parameters::
isAxisymmetric() const
{
  return  dbase.get<bool >("axisymmetricProblem");
}


// ===================================================================================================================
/// \brief return true if this is a steady state problem.
///
// ==================================================================================================================
bool Parameters::
isSteadyStateSolver() const
{
  return (dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==steadyStateRungeKutta ||
          dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==steadyStateNewton);
}


// ===================================================================================================================
/// \brief return the number of ghost points needed by this method.
///
// ==================================================================================================================
int Parameters::
numberOfGhostPointsNeeded() const  
{
  int numGhost= dbase.get<int >("orderOfAccuracy")/2;

  return numGhost;
}

// ===================================================================================================================
/// \brief return the number of ghost points needed by this method for the implicit matrix.
///
// ==================================================================================================================
int Parameters::
numberOfGhostPointsNeededForImplicitMatrix() const  
{
  return numberOfGhostPointsNeeded();
}


// ===================================================================================================================
/// \brief set the twilight-zone function.
///
/// \param choice_ (input) : the twilight-zone option.
/// \param degreeSpace (input) : the degree in space for a polynomial function.
/// \param degreeTime (input) : the degree in time for a polynomial function.
///
// ==================================================================================================================
int Parameters::
setTwilightZoneFunction(const TwilightZoneChoice & choice_,
                        const int & degreeSpace /* =2 */ , 
                        const int & degreeTime /* =1 */ )
{
  TwilightZoneChoice choice=choice_;
  
  printF("setTwilightZoneFunction:choice=%i\n",(int)choice); // **************
  

  //TODO: add TZ for passive scalar=passivec
  if( choice!=polynomial && choice!=trigonometric && choice!=pulse )
  {
    printF("Parameters:: setTwilightZoneFunction: TwilightZoneChoice=%i not recognized\n"
           "  TwilightZoneChoice=trigonometric will be used instead\n",choice);
  }

  const int numberOfDimensions = dbase.get<int >("numberOfDimensions");
  const int numberOfComponents = dbase.get<int >("numberOfComponents");
  

  delete  dbase.get<OGFunction* >("exactSolution");
  if( choice==polynomial )
  {
    // ******* polynomial twilight zone function ******
     dbase.get<OGFunction* >("exactSolution") = new OGPolyFunction(degreeSpace, numberOfDimensions, numberOfComponents,degreeTime);

    Range R5(0,4);
    RealArray spatialCoefficientsForTZ(5,5,5, numberOfComponents);  
    spatialCoefficientsForTZ=0.;
    RealArray timeCoefficientsForTZ(5, numberOfComponents);      
    timeCoefficientsForTZ=0.;


    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==kEpsilon ||  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==kOmega )
    {
      // k, eps and  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega") should remain positive: nuT = cMu*k*k/eps
      spatialCoefficientsForTZ(0,0,0, dbase.get<int >("kc"))=.5;
      spatialCoefficientsForTZ(0,0,0, dbase.get<int >("epsc"))=2.;
      
      if(  numberOfDimensions>1 )
      {
        // spatialCoefficientsForTZ(0,1,0, dbase.get<int >("kc"))=.2;
        // spatialCoefficientsForTZ(0,1,0, dbase.get<int >("epsc"))=.1;
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0, dbase.get<int >("kc"))=.2;
	  spatialCoefficientsForTZ(0,2,0, dbase.get<int >("kc"))=.3;
	  spatialCoefficientsForTZ(2,0,0, dbase.get<int >("epsc"))=.8;
	  spatialCoefficientsForTZ(0,2,0, dbase.get<int >("epsc"))=.6;
	}
      }
      if(  numberOfDimensions>2 )
      {
        // spatialCoefficientsForTZ(0,0,1, dbase.get<int >("kc"))=.15;
        // spatialCoefficientsForTZ(0,0,1, dbase.get<int >("epsc"))=.25;
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(0,0,2, dbase.get<int >("kc"))=.5;
	  spatialCoefficientsForTZ(0,0,2, dbase.get<int >("epsc"))=.5;
	}
      }
    }
    else if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==SpalartAllmaras )
    {
      // nuT should remain positive
      const int nc= dbase.get<int >("kc");
      spatialCoefficientsForTZ(0,0,0,nc)=.5;
      spatialCoefficientsForTZ(1,0,0,nc)=0.;
      spatialCoefficientsForTZ(0,1,0,nc)=0.;
      spatialCoefficientsForTZ(0,0,1,nc)=0.;
      
      if(  numberOfDimensions>1 )
      {
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(2,0,0,nc)=.25; // .4;
	  spatialCoefficientsForTZ(0,2,0,nc)=.15; // .6;
	}
        // Do no add  a linear term since this could cause the viscosity coeff to be negative
//      else if( degreeSpace==1 )
//   	{
//   	  spatialCoefficientsForTZ(1,0,0,nc)=.1;
//   	  spatialCoefficientsForTZ(0,1,0,nc)=.1;
//   	}
      }
      if(  numberOfDimensions>2 )
      {
        if( degreeSpace==2 )
	{
	  spatialCoefficientsForTZ(0,0,2,nc)=.5;
	}
//          else if( degreeSpace==1 )
//  	{
//  	  spatialCoefficientsForTZ(0,0,2,nc)=.5;
//  	}
      }
    }

    if(  dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==noTurbulenceModel )
    {
      // default case:
      for( int n=0; n< numberOfComponents; n++ )
      {
	real ni =1./(n+1);
    
	spatialCoefficientsForTZ(0,0,0,n)=2.+n;      
	if( degreeSpace>0 )
	{
	  spatialCoefficientsForTZ(1,0,0,n)=1.*ni;
	  spatialCoefficientsForTZ(0,1,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,0,1,n)=  numberOfDimensions==3 ? .25*ni : 0.;
	}
	if( degreeSpace>1 )
	{
	  spatialCoefficientsForTZ(2,0,0,n)=.5*ni;
	  spatialCoefficientsForTZ(0,2,0,n)=.25*ni;
	  spatialCoefficientsForTZ(0,0,2,n)=  numberOfDimensions==3 ? .125*ni : 0.;

          if( false ) // *wdh* 050610
	  {
	    // add cross terms
            printF("\n\n ************* add cross terms to TZ ************** \n\n");
	    

            spatialCoefficientsForTZ(1,1,0,n)=.125*ni;
            if(  numberOfDimensions>2 )
	    {
	      spatialCoefficientsForTZ(1,0,1,n)=.1*ni;
	      spatialCoefficientsForTZ(0,1,1,n)=-.15*ni;
	    }
	    
          }
	  
	}
      }
    }

    for( int n=0; n< numberOfComponents; n++ )
    {
      for( int i=0; i<=4; i++ )
      {
	timeCoefficientsForTZ(i,n)= i<=degreeTime ? 1./(i+1) : 0. ;
      }
	  
    }
  
    // ::display(spatialCoefficientsForTZ,"spatialCoefficientsForTZ","%6.2f ");
    
    ((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( spatialCoefficientsForTZ,timeCoefficientsForTZ );  // for u
  
  }
  else if( choice==trigonometric ) // ******* Trigonometric function chosen ******
  {
    RealArray fx( numberOfComponents),fy( numberOfComponents),fz( numberOfComponents),ft( numberOfComponents);
    RealArray gx( numberOfComponents),gy( numberOfComponents),gz( numberOfComponents),gt( numberOfComponents);
    gx=0.;
    gy=0.;
    gz=0.;
    gt=0.;
    RealArray amplitude( numberOfComponents), cc( numberOfComponents);
    amplitude=1.;
    cc=0.;

    

    fx= dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0];
    fy =  numberOfDimensions>1 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1] : 0.;
    fz =  numberOfDimensions>2 ?  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2] : 0.;
    ft =  dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3];

     dbase.get<OGFunction* >("exactSolution") = new OGTrigFunction(fx,fy,fz,ft);
    
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setShifts(gx,gy,gz,gt);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setAmplitudes(amplitude);
    ((OGTrigFunction*) dbase.get<OGFunction* >("exactSolution"))->setConstants(cc);
      
  }
  else if( choice==pulse ) 
  {
    // ******* Pulse function chosen ******
     ArraySimpleFixed<real,9,1,1,1> & pulseData = dbase.get<ArraySimpleFixed<real,9,1,1,1> >("pulseData");

     printF("setTwilightZoneFunction:INFO: create the OGPulseFunction pulseData="
            "[%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g,%.2g\n",pulseData[0],pulseData[1],pulseData[2],
            pulseData[3],pulseData[4],pulseData[5],pulseData[6],pulseData[7],pulseData[8]);

     dbase.get<OGFunction* >("exactSolution") =  new OGPulseFunction( numberOfDimensions, numberOfComponents, pulseData[0],pulseData[1],pulseData[2],pulseData[3],pulseData[4],pulseData[5],
         pulseData[6],pulseData[7],pulseData[8]); 

    // this pulse function is not divergence free!

  }
    
  
  return 0;
}


// ===================================================================================================================
/// \brief Add a prefix string to the start of every label.
///
/// /param label (input) : null terminated array of strings.
/// /param prefix (input) : all this string as a prefix.
/// /param cmd (input/output): on output cmd[i]=prefix+label[i];
/// /param maxCommands (input): maximum number of strings in the cmd array.
///
// ==================================================================================================================
int
addPrefix(const aString label[], const aString & prefix, aString cmd[], const int maxCommands)
{
    
  int i;
  for( i=0; i<maxCommands && label[i]!=""; i++ )
    cmd[i]=prefix+label[i];
  if( i<maxCommands )
    cmd[i]="";
  else
  {
    printF("ERROR:addPrefix: maxCommands=%i is too small\n",maxCommands);
    assert( maxCommands>0 );
    cmd[maxCommands-1];
    return 1;
  }
  return 0;
}



// ===================================================================================================================
/// \brief Open or close show files, set variables that appear in the show file.
///
/// \param command (input) : optionally supply a command to execute. Attempt to execute the command
///    and then return. The return value is 0 if the command was executed, 1 otherwise.
/// \param interface (input) : use this dialog. If command=="build dialog", fill in the dialog and return.
///
/// \anchor ParameterShowFileOptions
/// Here are the show file options:
/// 
///  - <b> open </b> : open a new show file.
///  - <b> close </b> : close any open show file.
///  - <b> show file variables </b> : specify extra derived quantities, such as the divergence or vorticity, that
///      should be saved in the show file in addition to the standard variables.
///  - <b> frequency to save </b> : By default the solution is saved in the show file
///          as often as it is plotted according to "times to plot". To save the solution less
///          often set this integer value to be greater than 1. A value of 2 for example will save solutions
///          every 2nd time the solution is plot.
///  - <b> frequency to flush </b> : Save this many solutions in each sub-show file so that no file gets too big.
///        This will result in multiple show files being created (these are automatically handled by plotStuff). 
///  - <b> uncompressed </b> : save the show file uncompressed. This is a more portable format
///       that can be read by future versions of Overture.
///  - <b> compressed </b> : save the show file compressed. This is a less portable format.
// ==================================================================================================================
int Parameters::
updateShowFile(const aString & command /* = nullString */,
               DialogData *interface /* =NULL */ )
{
  int returnValue=0;
  
  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");

  Ogshow *& show = dbase.get<Ogshow* >("show");

  aString prefix = "OBPSF:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;


  GUIState gui;
  gui.setWindowTitle("Show File Options");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  if( interface==NULL || command=="build dialog" )
  {

    const int maxCommands=40;
    aString cmd[maxCommands];

    aString label[] = {"compressed", "uncompressed","" }; //
    addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("mode", cmd, label, ( dbase.get<int >("useStreamMode")? 0 : 1));

    aString tbCommands[] = {"save augmented variables",""};
    int tbState[10];
    tbState[0] = dbase.get<bool>("saveAugmentedSolutionToShowFile");
    int numColumns=1;
    dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

    aString pbLabels[] = {"open","close",""};
    addPrefix(pbLabels,prefix,cmd,maxCommands);
    int numRows=1;
    dialog.setPushButtons( cmd, pbLabels, numRows ); 

    const int numberOfTextStrings=40;
    aString textLabels[numberOfTextStrings];
    aString textStrings[numberOfTextStrings];

    int nt=0;
    
    textLabels[nt] = "frequency to save";  sPrintF(textStrings[nt], "%i",  dbase.get<int >("frequencyToSaveInShowFile"));  nt++; 

    int flushFrequency=  show!=NULL ?  show->getFlushFrequency() : 5;
    textLabels[nt] = "frequency to flush"; sPrintF(textStrings[nt], "%i", flushFrequency);  nt++; 

    textLabels[nt] = "frequency to save sequences"; 
    sPrintF(textStrings[nt], "%i",  dbase.get<int >("frequencyToSaveSequenceInfo"));  nt++; 

    const int np = Communication_Manager::numberOfProcessors();
    textLabels[nt] = "maximum number of parallel sub-files"; 
    sPrintF(textStrings[nt], "%i",np);  nt++; 

    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);

    //  show file variables
    const int maximumNumberOfNames= dbase.get<int >("numberOfComponents")+40;
    aString *showLabel= new aString[maximumNumberOfNames];
    aString *& showVariableName = dbase.get<aString* >("showVariableName");
    int *onOff = new int [maximumNumberOfNames];
    int i=0;
    for( int n=0;  showVariableName!=NULL && dbase.get<aString* >("showVariableName")[n]!=""; n++ )
    {
      showLabel[i]= showVariableName[n];
      onOff[i]= dbase.get<IntegerArray >("showVariable")(i)>=0;
      i++;
      assert( i+2 < maximumNumberOfNames );
    }
    showLabel[i]="";

    addPrefix(showLabel,prefix+"show variable: ",cmd,maxCommands);
    dialog.addPulldownMenu("show variables", cmd, showLabel, GI_TOGGLEBUTTON,onOff);
    delete [] showLabel;
    delete [] onOff;

    if( executeCommand ) return 0;
  }
  


  aString answer,answer2;
  char buff[100];

  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("showFile>");  
  }
  int len=0;
  for(int it=0; ; it++)
  {
    if( !executeCommand )
      gi.getAnswer(answer,"");
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);

    if( answer=="exit" || answer=="done" )
    {
      break;
    }
    else if( answer=="open" )
    {
      gi.inputFileName(answer2,"Enter the name of the show file (e.g. ob.show)");
      // gi.inputString(answer2,"Enter the name of the show file (e.g. ob.show)");
      if( answer2!="" )
      {
        if(  show!=NULL )
	{
          printF("INFO:closing the currently open show file\n");
	   show->close();
	}
	printF("Opening the show file %s\n",(const char*)answer2);
	 show = new Ogshow( answer2,".", dbase.get<int >("useStreamMode") );      

	 // -- saveParametersToShowFile();

      }
    }
    else if( answer=="close" )
    {
      if(  show!=NULL )
      {
	 show->close();
        delete  show;
         show=NULL;
      }
      else
      {
	printF("ERROR:There is no open show file\n");
      }
    }
    else if( answer=="uncompressed" )
    {
       dbase.get<int >("useStreamMode")=false;
      printF("Any newly opened show files will be saved in uncompressed format\n");
    }
    else if( answer=="compressed" )
    {
       dbase.get<int >("useStreamMode")=true;
      printF("Any newly opened show files will be saved in compressed format\n");
    }
    else if( answer(0,13)=="show variable:" )
    {
      // answer looks like: "show file variable: vorticity 1"
      answer2=answer(15,answer.length()-3);   //
      int onOff=0; sScanF(answer(answer.length()-2,answer.length()-1),"%i",&onOff);
      
      int i=-1;
      for( int n=0;  dbase.get<aString* >("showVariableName")[n]!=""; n++ )
      {
	if( answer2== dbase.get<aString* >("showVariableName")[n] )
	{
           dbase.get<IntegerArray >("showVariable")(n)= onOff==1 ? abs( dbase.get<IntegerArray >("showVariable")(n)) : -abs( dbase.get<IntegerArray >("showVariable")(n)); // - dbase.get<IntegerArray >("showVariable")(n);
          i=n;
          break;
	}
      }
      if( i<0 )
        printF("ERROR: unknown response: answer=[%s], answer2=[%s]\n",(const char*)answer,(const char*)answer2);
      else
      {
        printF("  ..show file variable %s is now %s.\n",(const char*) dbase.get<aString* >("showVariableName")[i],
                 ( dbase.get<IntegerArray >("showVariable")(i)>0 ? "on" : "off"));
      }
      
    }
    else if( answer=="show file variables" )
    {
      const int maximumNumberOfNames= dbase.get<int >("numberOfComponents")+40;
      aString *showMenu= new aString[maximumNumberOfNames];
      for( ;; )
      {
	int i=0;
	for( int n=0;  dbase.get<aString* >("showVariableName")[n]!=""; n++ )
	{
	  showMenu[i]= dbase.get<aString* >("showVariableName")[n] + ( dbase.get<IntegerArray >("showVariable")(i)>0 ? " (on)" : " (off)");
          i++;
          assert( i+2 < maximumNumberOfNames );
	}
	showMenu[i++]="done";
	showMenu[i]="";

	int response=gi.getMenuItem(showMenu,answer2,"toggle variables to save in the show file");
        if( answer2=="done" || answer2=="exit" )
	  break;
	else if( response>=0 && response<i-1 )
	   dbase.get<IntegerArray >("showVariable")(response)=- dbase.get<IntegerArray >("showVariable")(response);
	else
	{
	  printF("Unknown response: [%s]\n",(const char*)answer2);
	  gi.stopReadingCommandFile();
	}
	
      }
      delete [] showMenu;
    }
    else if( dialog.getToggleValue(answer,"save augmented variables",dbase.get<bool>("saveAugmentedSolutionToShowFile")) )
    {
      if( dbase.get<bool>("saveAugmentedSolutionToShowFile") )
        printF("INFO: Saving all augmented solution variables (those plotted when running interactively) to the show file\n");
    }

    else if( len=answer.matches("frequency to save sequences") )
    {
      sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("frequencyToSaveSequenceInfo"));
      printF(" frequencyToSaveSequenceInfo=%i\n", dbase.get<int >("frequencyToSaveSequenceInfo"));
      dialog.setTextLabel("frequency to save sequences",sPrintF(answer2,"%i",  dbase.get<int >("frequencyToSaveSequenceInfo")));  
    }
    else if( len=answer.matches("maximum number of parallel sub-files") )
    {
      if( show!=NULL )
      {
	printF("WARNING: The option 'maximum number of parallel sub-files' will only apply to a show file\n"
               "         that is subsequently opened, not to an already opened show file.\n");
      }
      const int np = Communication_Manager::numberOfProcessors();
      int maxFiles=np;
      sScanF(answer(len,answer.length()-1),"%i",&maxFiles);
      printF("maximum number of parallel sub-files =%i\n",maxFiles);
      dialog.setTextLabel("maximum number of parallel sub-files",sPrintF(answer2,"%i",maxFiles));
      GenericDataBase::setMaximumNumberOfFilesForWriting(maxFiles);
    }
    else if( answer=="frequency to save" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the frequencyToSaveInShowFile (default value=%i)",
              dbase.get<int >("frequencyToSaveInShowFile")));
      if( answer2!="" )
	sScanF(answer2,"%i",& dbase.get<int >("frequencyToSaveInShowFile"));
      printF(" frequencyToSaveInShowFile=%i\n", dbase.get<int >("frequencyToSaveInShowFile"));
    }
    else if( answer(0,16)=="frequency to save" )
    {
      sScanF(answer(17,answer.length()-1),"%i",& dbase.get<int >("frequencyToSaveInShowFile"));
      printF(" frequencyToSaveInShowFile=%i\n", dbase.get<int >("frequencyToSaveInShowFile"));
      dialog.setTextLabel("frequency to save",sPrintF(answer2,"%i",  dbase.get<int >("frequencyToSaveInShowFile")));  
    }
    else if( answer=="frequency to flush" )
    {
      int flushFrequency;
      gi.inputString(answer2,"Enter the frequency to flush the show file");
      if( answer2!="" )
	sScanF(answer2,"%i",&flushFrequency);
      flushFrequency=max(1,flushFrequency);
      if(  show!=NULL )
         show->setFlushFrequency( flushFrequency );
      printF("flushFrequency=%i\n",flushFrequency); 
    }
    else if( answer(0,17)=="frequency to flush" )
    {
      int flushFrequency=  show!=NULL ?  show->getFlushFrequency() : 5;
      sScanF(answer(18,answer.length()-1),"%i",&flushFrequency);
      flushFrequency=max(1,flushFrequency);
      if(  show!=NULL )
         show->setFlushFrequency( flushFrequency );
      printF("flushFrequency=%i\n",flushFrequency); 
      dialog.setTextLabel("frequency to flush",sPrintF(answer2,"%i", flushFrequency));  
    }
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing  a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	printF("Unknown response: [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
       
    }
    
  }

  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }
  
  return returnValue;
}

// ===================================================================================================================
/// \brief Return true if we should save the linearized solution for implicit methods.
/// \details If implicit operator is non-linear then we may need to save the solution we
///           linearize about.
// ==================================================================================================================
bool Parameters::
saveLinearizedSolution()
{
  return false;
}

// ===================================================================================================================
/// \brief Save PDE specific parameters in the show file.
/// \details These parameters can be used for a restart. They can also be used, for example,
///     by the user defined derived functions (when viewing the show file with plotStuff).
// ==================================================================================================================
int Parameters::
saveParametersToShowFile()
{
  assert(  dbase.get<Ogshow* >("show")!=NULL );

  ListOfShowFileParameters & showFileParams = dbase.get<ListOfShowFileParameters>("showFileParams");

  // Save the show variable components *wdh* 080627
  const int & numberOfComponents = dbase.get<int >("numberOfComponents");
  const int & numberOfShowVariables = dbase.get<int >("numberOfShowVariables");
  const aString *showVariableName = dbase.get<aString* >("showVariableName");
  const IntegerArray & showVariable = dbase.get<IntegerArray >("showVariable");
  int cc=0;  // counts show file variables that are actually saved 
  for( int i=0; i< numberOfShowVariables; i++ )
  {
    aString name = showVariableName[i];
    // We use special name for some variables:
    if( name=="r" || name=="rho" )
      name = "densityComponent";  
    else if( name=="T" )
      name = "temperatureComponent";
    else if( name=="p" )
      name = "pressureComponent";
    else
      name = name + "Component";

    int component=showVariable(i);  // component number (negative means this variable is turned off in the show file)
    if( showVariable(i)==numberOfComponents+1 )  // this is an additional "derived" variable
      component=cc;
    if( showVariable(i) >=0 )
      cc++;

    showFileParams.push_back(ShowFileParameter(name, component));
  }

  // Add on user defined parameters
  std::list<ShowFileParameter>::iterator iter; 
  for(iter =  dbase.get<ListOfShowFileParameters >("pdeParameters").begin(); iter!= dbase.get<ListOfShowFileParameters >("pdeParameters").end(); iter++ )
  {
    showFileParams.push_back(*iter);
  }

  dbase.get<Ogshow* >("show")->saveGeneralParameters(showFileParams);
  
  showFileParams.clear();
  
  return 0;
}



// int Parameters::
// getEquationNumber (const int component)
// //added by DLB 001020
// {
//   return equationNumber(component);

// }



#define GET_OR_PUT \
\
  subDir.GP( dbase.get<real >("dt"),"dt");\
  subDir.GP( dbase.get<real >("a0"),"a0");\
  subDir.GP( dbase.get<real >("a1"),"a1");\
  subDir.GP( dbase.get<real >("a2"),"a2");\
  subDir.GP( dbase.get<real >("b0"),"b0");\
  subDir.GP( dbase.get<real >("b1"),"b1");\
  subDir.GP( dbase.get<real >("b2"),"b2");\
     \
  subDir.GP( dbase.get<RealArray >("initialConditions"),"initialConditions");\
  subDir.GP( dbase.get<int >("numberOfDimensions"),"numberOfDimensions");\
  subDir.GP( dbase.get<int >("numberOfComponents"),"numberOfComponents");\
  subDir.GP( dbase.get<int >("orderOfAccuracy"),"orderOfAccuracy");\
\
  subDir.GP( dbase.get<real >("cfl"),"cfl");\
  subDir.GP( dbase.get<real >("cflMin"),"cflMin"); subDir.GP( dbase.get<real >("cflOpt"),"cflOpt"); subDir.GP( dbase.get<real >("cflMax"),"cflMax");\
  subDir.GP( dbase.get<real >("dtMax"),"dtMax");\
  subDir.GP( dbase.get<bool >("axisymmetricProblem"),"axisymmetricProblem");\
  subDir.GP( dbase.get<bool >("axisymmetricWithSwirl"),"axisymmetricWithSwirl");\
  subDir.GP( dbase.get<int >("useLocalTimeStepping"),"useLocalTimeStepping");\
  subDir.GP( dbase.get<int >("radialAxis"),"radialAxis");\
  \
  subDir.GP( dbase.get<int >("rc"),"rc");\
  subDir.GP( dbase.get<int >("uc"),"uc");\
  subDir.GP( dbase.get<int >("vc"),"vc");\
  subDir.GP( dbase.get<int >("wc"),"wc");\
  subDir.GP( dbase.get<int >("pc"),"pc");\
  subDir.GP( dbase.get<int >("tc"),"tc");\
  subDir.GP( dbase.get<int >("sc"),"sc");\
  subDir.GP( dbase.get<int >("kc"),"kc"); subDir.GP( dbase.get<int >("epsc"),"epsc");\
\
  subDir.GP( dbase.get<int >("ec"),"ec");\
\
  subDir.GP( dbase.get<real >("u0"),"u0");  \
  subDir.GP( dbase.get<real >("l0"),"l0");  \
  subDir.GP( dbase.get<real >("rho0"),"rho0"); \
  subDir.GP( dbase.get<real >("pStatic"),"pStatic");\
  subDir.GP( dbase.get<real >("p0"),"p0");  \
  subDir.GP( dbase.get<real >("te0"),"te0");  \
  subDir.GP( dbase.get<real >("R0"),"R0");\
  subDir.GP( dbase.get<real >("nu"),"nu");\
  subDir.GP( dbase.get<real >("advectionCoefficient"),"advectionCoefficient");\
  \
  subDir.GP( dbase.get<real >("machNumber"),"machNumber"); subDir.GP( dbase.get<real >("reynoldsNumber"),"reynoldsNumber"); \
  subDir.GP( dbase.get<real >("prandtlNumber"),"prandtlNumber");\
  subDir.GP( dbase.get<real >("mu"),"mu"); subDir.GP( dbase.get<real >("kThermal"),"kThermal"); subDir.GP( dbase.get<real >("Rg"),"Rg"); \
  subDir.GP( dbase.get<real >("gamma"),"gamma"); subDir.GP( dbase.get<real >("avr"),"avr"); subDir.GP( dbase.get<real >("anu"),"anu");\
  subDir.GP( dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"),"orderOfExtrapolationForInterpolationNeighbours");\
  \
  subDir.GP( dbase.get<real >("pressureLevel"),"pressureLevel"); subDir.GP( dbase.get<real >("nuRho"),"nuRho");\
  subDir.GP( dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity").ptr(),"gravity",3);\
  subDir.GP( dbase.get<real >("fluidDensity"),"fluidDensity");\
  subDir.GP( dbase.get<int >("explicitMethod"),"explicitMethod"); subDir.GP( dbase.get<int >("linearizeImplicitMethod"),"linearizeImplicitMethod");\
  subDir.GP( dbase.get<int >("refactorFrequency"),"refactorFrequency");\
\
  subDir.GP( dbase.get<int >("removeFastPressureWaves"),"removeFastPressureWaves");\
\
  subDir.GP( dbase.get<real >("av2"),"av2"); subDir.GP( dbase.get<real >("aw2"),"aw2"); subDir.GP( dbase.get<real >("av4"),"av4"); subDir.GP( dbase.get<real >("aw4"),"aw4");\
  subDir.GP( dbase.get<real >("betaT"),"betaT"); subDir.GP( dbase.get<real >("betaK"),"betaK"); subDir.GP( dbase.get<real >("rT0"),"rT0"); \
  subDir.GP( dbase.get<real >("artVisc"),"artVisc");\
  subDir.GP( dbase.get<RealArray >("artificialDiffusion"),"artificialDiffusion");\
  subDir.GP( dbase.get<bool >("useSmagorinskyEddyViscosity"),"useSmagorinskyEddyViscosity");\
  subDir.GP( dbase.get<real >("smagorinskyCoefficient"),"smagorinskyCoefficient");\
\
  subDir.GP( dbase.get<int >("initializeImplicitMethod"),"initializeImplicitMethod");\
\
  subDir.GP( dbase.get<IntegerArray >("gridIsImplicit"),"gridIsImplicit");\
  subDir.GP( dbase.get<IntegerArray >("timeStepType"),"timeStepType");\
  subDir.GP( dbase.get<int >("orderOfPredictorCorrector"),"orderOfPredictorCorrector");\
  subDir.GP( dbase.get<int >("orderOfBDF"),"orderOfBDF");\
\
  subDir.GP( dbase.get<int >("initializeImplicitTimeStepping"),"initializeImplicitTimeStepping");  \
  subDir.GP( dbase.get<int >("scalarSystemForImplicitTimeStepping"),"scalarSystemForImplicitTimeStepping");\
  \
  subDir.GP( dbase.get<real >("anl"),"anl"); subDir.GP( dbase.get<real >("ad21"),"ad21"); subDir.GP( dbase.get<real >("ad22"),"ad22");\
  subDir.GP( dbase.get<real >("ad41"),"ad41"); subDir.GP( dbase.get<real >("ad42"),"ad42"); subDir.GP( dbase.get<real >("cdv"),"cdv"); subDir.GP( dbase.get<real >("cDt"),"cDt"); \
  subDir.GP( dbase.get<real >("dampingDt"),"dampingDt");\
\
  subDir.GP( dbase.get<real >("heatRelease"),"heatRelease"); subDir.GP( dbase.get<real >("reciprocalActivationEnergy"),"reciprocalActivationEnergy");\
  subDir.GP( dbase.get<real >("rateConstant"),"rateConstant");\
\
  subDir.GP( dbase.get<real >("implicitFactor"),"implicitFactor");\
  subDir.GP( dbase.get<real >("slowStartCFL"),"slowStartCFL");\
  subDir.GP( dbase.get<real >("slowStartTime"),"slowStartTime");\
  subDir.GP( dbase.get<int  >("slowStartSteps"),"slowStartSteps");\
  subDir.GP( dbase.get<int  >("slowStartRecomputeDtSteps"),"slowStartRecomputeDtSteps");\
\
  subDir.GP( dbase.get<bool >("useCharacteristicInterpolation"),"useCharacteristicInterpolation"); /* bool */\
  subDir.GP( dbase.get<int >("reducedInterpolationWidth"),"reducedInterpolationWidth");\
  \
  /*  dbase.get<MovingGrids >("movingGrids");   */ \
\
  subDir.GP( dbase.get<bool >("detectCollisions"),"detectCollisions"); /* bool */\
  subDir.GP( dbase.get<bool >("adjustTimeStepForMovingBodies"),"adjustTimeStepForMovingBodies"); /* bool */\
  subDir.GP( dbase.get<real >("collisionDistance"),"collisionDistance");\
  subDir.GP( dbase.get<int >("stencilWidthForExposedPoints"),"stencilWidthForExposedPoints");\
  subDir.GP( dbase.get<int >("extrapolateInterpolationNeighbours"),"extrapolateInterpolationNeighbours");\
  subDir.GP( dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"),"frequencyToUseFullUpdateForMovingGridGeneration"); \
  \
  subDir.GP( dbase.get<bool >("forcing"),"forcing"); /* bool */\
  subDir.GP( dbase.get<bool >("twilightZoneFlow"),"twilightZoneFlow"); /* bool */\
\
  subDir.GP( dbase.get<int >("tzDegreeSpace"),"tzDegreeSpace"); subDir.GP( dbase.get<int >("tzDegreeTime"),"tzDegreeTime");\
  \
  subDir.GP( dbase.get<bool >("readRestartFile"),"readRestartFile"); /* bool */\
  subDir.GP( dbase.get<bool >("saveRestartFile"),"saveRestartFile"); /* bool */\
  subDir.GP( dbase.get<aString >("restartFileName"),"restartFileName");\
  /* OGFunction * dbase.get<OGFunction* >("exactSolution");  */ \
  subDir.GP( dbase.get<bool >("projectInitialConditions"),"projectInitialConditions");\
\
  subDir.GP( dbase.get<bool >("adaptiveGridProblem"),"adaptiveGridProblem"); /* bool */\
\
  subDir.GP( dbase.get<real >("errorThreshold"),"errorThreshold");\
  subDir.GP( dbase.get<int >("orderOfAdaptiveGridInterpolation"),"orderOfAdaptiveGridInterpolation");\
  subDir.GP( dbase.get<int >("showAmrErrorFunction"),"showAmrErrorFunction");\
  subDir.GP( dbase.get<int >("amrErrorFunctionOption"),"amrErrorFunctionOption");\
  subDir.GP( dbase.get<int >("showResiduals"),"showResiduals");\
\
  /* Ogen * dbase.get<Ogen* >("gridGenerator");  */ \
\
  subDir.GP( dbase.get<bool >("computeReactions"),"computeReactions"); /* bool */\
  /*  Reactions * dbase.get<Reactions* >("reactions"); */ \
  subDir.GP( dbase.get<int >("numberOfSpecies"),"numberOfSpecies");\
  \
  subDir.GP( dbase.get<int >("maximumStepsBetweenComputingDt"),"maximumStepsBetweenComputingDt");\
\
  subDir.GP( dbase.get<int >("debug"),"debug");\
  subDir.GP( dbase.get<int >("info"),"info");\
/*  FILE * dbase.get<FILE* >("debugFile");       */ \
/*  FILE * dbase.get<FILE* >("checkFile");       */ \
/*  FILE * dbase.get<FILE* >("logFile");         */ \
  \
\
  subDir.GP( dbase.get<int >("plotMode"),"plotMode");\
  subDir.GP( dbase.get<int >("plotOption"),"plotOption");\
  /* Ogshow * dbase.get<Ogshow* >("show"); */ \
\
  /* aString * dbase.get<aString* >("showVariableName");  */ \
\
  subDir.GP( dbase.get<IntegerArray >("showVariable"),"showVariable");\
  subDir.GP( dbase.get<int >("numberOfShowVariables"),"numberOfShowVariables");\
  subDir.GP( dbase.get<int >("maxNumberOfShowVariables"),"maxNumberOfShowVariables");\
  subDir.GP( dbase.get<int >("frequencyToSaveInShowFile"),"frequencyToSaveInShowFile");\
  subDir.GP( dbase.get<int >("saveGridInShowFile"),"saveGridInShowFile");\
  subDir.GP( dbase.get<int >("allowUserDefinedOutput"),"allowUserDefinedOutput");\
  subDir.GP( dbase.get<int >("trackingIsOn"),"trackingIsOn");\
  subDir.GP( dbase.get<bool >("useDefaultErrorEstimator"),"useDefaultErrorEstimator");\
  subDir.GP( dbase.get<bool >("useUserDefinedErrorEstimator"),"useUserDefinedErrorEstimator");\
  \
  subDir.GP( dbase.get<int >("useStreamMode"),"useStreamMode");\
\
  subDir.GP( dbase.get<int >("numberOfSequences"),"numberOfSequences");\
  subDir.GP( dbase.get<int >("sequenceCount"),"sequenceCount");  \
  subDir.GP( dbase.get<RealArray >("timeSequence"),"timeSequence"); subDir.GP( dbase.get<RealArray >("sequence"),"sequence");\
\
  subDir.GP( dbase.get<real >("tFinal"),"tFinal");\
  subDir.GP( dbase.get<real >("tInitial"),"tInitial");\
  subDir.GP( dbase.get<real >("tPrint"),"tPrint");\
  subDir.GP( dbase.get<int >("maxIterations"),"maxIterations");\
  subDir.GP( dbase.get<int >("plotIterations"),"plotIterations");\
  subDir.GP( dbase.get<RealArray >("printArray"),"printArray");\
  \
  subDir.GP( dbase.get<int >("compare3Dto2D"),"compare3Dto2D");\
  subDir.GP( dbase.get<int >("dimensionOfTZFunction"),"dimensionOfTZFunction");\
\
  subDir.GP( dbase.get<RealArray >("bcData"),"bcData");\
  subDir.GP( dbase.get<real >("inflowPressure"),"inflowPressure");\
  subDir.GP( dbase.get<RealArray >("bcParameters"),"bcParameters");\
\
  subDir.GP( dbase.get<IntegerArray >("bcInfo"),"bcInfo");\
  \
  subDir.GP( dbase.get<RealArray >("userBoundaryConditionParameters"),"userBoundaryConditionParameters");\
\
  subDir.GP( dbase.get<RealArray >("timeDependenceBoundaryConditionParameters"),"timeDependenceBoundaryConditionParameters");\
\
  subDir.GP( dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega").ptr(),"omega",4);\
\
  subDir.GP( dbase.get<int >("checkErrorsAtGhostPoints"),"checkErrorsAtGhostPoints");\
\
  subDir.GP( dbase.get<int >("numberOfIterationsForConstraints"),"numberOfIterationsForConstraints");\
  subDir.GP( dbase.get<int >("numberOfSolvesForConstraints"),"numberOfSolvesForConstraints");\
\
  subDir.GP( dbase.get<int >("numberOfIterationsForImplicitTimeStepping"),"numberOfIterationsForImplicitTimeStepping");\
  subDir.GP( dbase.get<int >("orderOfTimeExtrapolationForPressure"),"orderOfTimeExtrapolationForPressure");\
  subDir.GP( dbase.get<int >("pressureBoundaryCondition"),"pressureBoundaryCondition");\
  subDir.GP( dbase.get<IntegerArray >("variableBoundaryData"),"variableBoundaryData");\
\
  subDir.GP( dbase.get<int >("numberOfOutputFiles"),"numberOfOutputFiles");\
/*  FileOutput * dbase.get<ArraySimpleFixed<FileOutput*,Parameters::maximumNumberOfOutputFiles,1,1,1> >("outputFile")[maximumNumberOfOutputFiles]; */ \
  subDir.GP( dbase.get<ArraySimpleFixed<int,Parameters::maximumNumberOfOutputFiles,1,1,1> >("fileOutputFrequency").ptr(),"fileOutputFrequency",maximumNumberOfOutputFiles); \
  subDir.GP( dbase.get<bool >("improveQualityOfInterpolation"),"improveQualityOfInterpolation"); \
  subDir.GP( dbase.get<real >("interpolationQualityBound"),"interpolationQualityBound"); \
  subDir.GP( dbase.get<real >("maximumAngleDifferenceForNormalsOnSharedBoundaries"),"maximumAngleDifferenceForNormalsOnSharedBoundaries"); 

int Parameters::
get( const GenericDataBase & dir, const aString & name)
// /Description: 
//    Read parameters to a data-base file
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Parameters");

  aString className;
  subDir.get( className,"className" ); 
  if( className != "Parameters" )
  {
    printF("Parameters::get ERROR in className! \n");
  }
#define GP get
#define GET
  GET_OR_PUT
#undef GP
#undef GET

  printF("****Parameters::get: numberOfShowVariables = %i\n", dbase.get<int >("numberOfShowVariables"));


  int temp; 
  subDir.get(temp,"turbulenceModel");   dbase.get<Parameters::TurbulenceModel >("turbulenceModel")=(TurbulenceModel)temp; 
  subDir.get(temp,"advectPassiveScalar");   dbase.get<bool >("advectPassiveScalar")=(bool)temp;
  subDir.get(temp,"timeSteppingMethod");  dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")=(TimeSteppingMethod)temp; 
  subDir.get(temp,"implicitMethod");   dbase.get<Parameters::ImplicitMethod >("implicitMethod")=(ImplicitMethod)temp; 
  subDir.get(temp,"implicitOption");   dbase.get<Parameters::ImplicitOption >("implicitOption")=(ImplicitOption)temp; 
  subDir.get(temp,"initialConditionOption");   dbase.get<Parameters::InitialConditionOption >("initialConditionOption")=(InitialConditionOption)temp; 

  subDir.get(temp,"twilightZoneChoice");  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=(TwilightZoneChoice)temp; 

  subDir.get(temp,"interpolationType");  dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")=(InterpolationTypeEnum)temp;

  int base,bound; 
  subDir.get(base,"Ru.base"); subDir.get(bound,"Ru.bound");  dbase.get<Range >("Ru")=Range(base,bound); 
  subDir.get(base,"Rt.base"); subDir.get(bound,"Rt.bound");  dbase.get<Range >("Rt")=Range(base,bound); 
  assert(  dbase.get<int >("numberOfComponents")>0 ); 
   dbase.get<aString* >("componentName") = new aString[ dbase.get<int >("numberOfComponents")]; 
  /* HDF_DataBase:: dbase.get<int >("debug")=3; */ 
  subDir.get( dbase.get<aString* >("componentName"),"componentName", dbase.get<int >("numberOfComponents")); 
  printF("Parameters::get:componentName[0]=[%s]\n",(const char*) dbase.get<aString* >("componentName")[0]);
  /* HDF_DataBase:: dbase.get<int >("debug")=0; */

  int regridExists=false;
  subDir.get(regridExists,"regridExists"); 
  if( regridExists ) 
  {
     dbase.get<Regrid* >("regrid") = new Regrid;
     dbase.get<Regrid* >("regrid")->get(subDir,"regrid");  
  }

  int errorEstimatorExists=false;
  subDir.get(errorEstimatorExists,"errorEstimatorExists"); 
  if( errorEstimatorExists )
  {
    buildErrorEstimator();  // this will also build InterpolateRefinements
     dbase.get<ErrorEstimator* >("errorEstimator")->get(subDir,"errorEstimator");
  }

  int interpolateRefinementsExists=false;
  subDir.get(interpolateRefinementsExists,"interpolateRefinementsExists"); 
  if( interpolateRefinementsExists )
  {
     dbase.get<InterpolateRefinements* >("interpolateRefinements")->get(subDir,"interpolateRefinements");
  }
  char buff[80];
  aString showName;
  const int numShowVariables= dbase.get<int >("numberOfShowVariables");
   dbase.get<int >("numberOfShowVariables")=0;  // this will be incremented by addShowVariable
  for( int i=0; i< dbase.get<int >("numberOfShowVariables"); i++ )
  {
    subDir.get(showName,sPrintF(buff,"showVariableName%i",i));
    addShowVariable(showName,i, dbase.get<IntegerArray >("showVariable")(i));
  }
  
  delete &subDir;
  return 0;
}

int Parameters::
put( GenericDataBase & dir, const aString & name) const
// ======================================================================================
// /Description: 
//    Save parameters to a data-base file
// ======================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create  a derived data-base object
  dir.create(subDir,name,"Parameters");                   // create  a sub-directory 

  subDir.put( "Parameters","className" );

#define GP put
  GET_OR_PUT
#undef GP
#undef GET_OR_PUT
  
  printF("****Parameters::put: numberOfShowVariables = %i\n", dbase.get<int >("numberOfShowVariables"));
  

  subDir.put((int) dbase.get<Parameters::TurbulenceModel >("turbulenceModel"),"turbulenceModel"); 
  subDir.put((int) dbase.get<bool >("advectPassiveScalar"),"advectPassiveScalar"); 
  subDir.put((int) dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod"),"timeSteppingMethod"); 
  subDir.put((int) dbase.get<Parameters::ImplicitMethod >("implicitMethod"),"implicitMethod");
  subDir.put((int) dbase.get<Parameters::ImplicitOption >("implicitOption"),"implicitOption");
  subDir.put((int) dbase.get<Parameters::InitialConditionOption >("initialConditionOption"),"initialConditionOption");
  subDir.put((int) dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"),"twilightZoneChoice"); 
  subDir.put((int) dbase.get<Parameters::InterpolationTypeEnum >("interpolationType"),"interpolationType");

  subDir.put( dbase.get<Range >("Ru").getBase(),"Ru.base"); subDir.put( dbase.get<Range >("Ru").getBound(),"Ru.bound"); 
  subDir.put( dbase.get<Range >("Rt").getBase(),"Rt.base"); subDir.put( dbase.get<Range >("Rt").getBound(),"Rt.bound"); 
  subDir.put( dbase.get<aString* >("componentName"),"componentName", dbase.get<int >("numberOfComponents")); 

  int regridExists= dbase.get<Regrid* >("regrid")!=NULL; 
  subDir.put(regridExists,"regridExists"); 
  if(  dbase.get<Regrid* >("regrid")!=NULL ) 
     dbase.get<Regrid* >("regrid")->put(subDir,"regrid");  

  int errorEstimatorExists= dbase.get<ErrorEstimator* >("errorEstimator")!=NULL;
  subDir.put(errorEstimatorExists,"errorEstimatorExists"); 
  if( errorEstimatorExists )
     dbase.get<ErrorEstimator* >("errorEstimator")->put(subDir,"errorEstimator");
  
  int interpolateRefinementsExists= dbase.get<InterpolateRefinements* >("interpolateRefinements")!=NULL;
  subDir.put(interpolateRefinementsExists,"interpolateRefinementsExists"); 
  if( interpolateRefinementsExists )
     dbase.get<InterpolateRefinements* >("interpolateRefinements")->put(subDir,"interpolateRefinements");
  
  char buff[80];
  for( int i=0; i< dbase.get<int >("numberOfShowVariables"); i++ )
    subDir.put( dbase.get<aString* >("showVariableName")[i],sPrintF(buff,"showVariableName%i",i));
  

  delete &subDir;
  return 0;
}


//\begin{>>ParametersInclude.tex}{\subsection{assignParameterValues}}   
int Parameters::
assignParameterValues(const aString & label, RealArray & values,
                      const int & numRead, char c[][10], real val[],
                      char *extraName1 /* = 0 */, const int & extraValue1Location /* = 0 */, 
                      char *extraName2 /* = 0 */, const int & extraValue2Location /* = 0 */, 
                      char *extraName3 /* = 0 */, const int & extraValue3Location /* = 0 */ )
// ==============================================================================================
//  /Description:
//     Assign parameter values with names in the array $c$ and values in the array {\tt val}.
// The entries in {\tt c} correspond to one of componentName[n] or to one of the extra names.
// /label (input) : print this label when showing the responses. For example label="initial conditions".
// /numRead (input) :
// /c (input) : names of components
// /val (input) : value to assign to the component.
// /values (output) : return values in this array. For example the assignment `u=1.' will result in
//   values(uc)=1.
// /Return value: The number of variables assigned.
//\end{ParametersInclude.tex}  
// ==============================================================================================
{
  const int numberOfExtraNames=3;
  char *extraName[numberOfExtraNames] = {extraName1,extraName2,extraName3};
  const int extraValueLocation[numberOfExtraNames]={extraValue1Location,extraValue2Location,extraValue3Location};

  aString name;
  int i,n;
  real energy=-1.;
  int ie=-1;
  for( i=0; i<numRead; i++ )
  {
    bool found=false;
    name =  c[i];	
    for( n=0; n< dbase.get<int >("numberOfComponents"); n++ )
    {
      if( name== dbase.get<aString* >("componentName")[n] )
      {
	values(n)=val[i];
	printF("assigning %s: %s=%e \n",(const char *)label,c[i],val[i]);
	found=true;
	break;
      }
    }
    if( !found )
    {
      for( int n=0; n<numberOfExtraNames; n++ )
      {
	if( extraName[n]!=0 && name==*extraName[n] )
	{
	  values(extraValueLocation[n])=val[i];
	  found=true;
          break;
	}
      }
      if( !found )
      {
	if( name=="e" )
	{
	  energy = val[i];
	  ie=i;
	  printF("Parameters::assignParameterValues:assigning %s: %s=%e \n",(const char *)label,c[i],val[i]);
	  found=true;
	}
      }
    }
    if( !found )
    {
      printF("Parameters::assignParameterValues:ERROR: unknown parameter being assigned: name=%s, value=%e \n", 
      c[i],val[i]);
      // printF("     : input string=[%s]\n",(const char*)answer);
    }
  }
  if( ie>=0 )
  {
    // determine the temperature from the energy
    const real & rho = values( dbase.get<int >("rc"));

    if( values( dbase.get<int >("uc"))==(real)defaultValue )
      values( dbase.get<int >("uc"))=0.;
    if(  dbase.get<int >("numberOfDimensions")>1 && values( dbase.get<int >("vc"))==(real)defaultValue )
      values( dbase.get<int >("vc"))=0.;
    if(  dbase.get<int >("numberOfDimensions")>2 && values( dbase.get<int >("wc"))==(real)defaultValue )
      values( dbase.get<int >("wc"))=0.;
    
      
    real uSq = SQR(values( dbase.get<int >("uc")));
    if(  dbase.get<int >("numberOfDimensions")>1 )
      uSq+=SQR(values( dbase.get<int >("vc")));
    if(  dbase.get<int >("numberOfDimensions")>2 )
      uSq+=SQR(values( dbase.get<int >("wc")));
    
    if(  dbase.get<real >("Rg")<=0. ||  dbase.get<real >("Rg")>1.e10 )
    {
      printF("assignParameterValues:ERROR: Rg=%e seems to be invalid. I am unable to set the temperature\n"
             "from the other variables\n", dbase.get<real >("Rg"));
    }
    else
    {
      values( dbase.get<int >("tc"))=(( dbase.get<real >("gamma")-1.)/ dbase.get<real >("Rg"))*(energy/rho-.5*uSq);
      printF("assignParameterValues:assigning the temperature=%7.3e from e=%7.3e, rho=%7.3e, Rg=%7.3e, |u|^2=%7.3e"
             " gamma=%7.3e\n",values( dbase.get<int >("tc")),energy,rho, dbase.get<real >("Rg"),uSq, dbase.get<real >("gamma"));
    }
  }

  // Print summary of values assigned:
  printF(" %s: (",(const char *)label);
  for( n=0; n< dbase.get<int >("numberOfComponents"); n++ )
  {
    if( values(n)!=(real)defaultValue )
      printF("%s=%f,",(const char *) dbase.get<aString* >("componentName")[n],values(n));
  }
  printF(")\n");

  return 0;
  
}

//\begin{>>ParametersInclude.tex}{\subsection{assignParameterValues}}   
int Parameters::
assignParameterValues(const aString & label, RealArray & values,
                      const int & numRead, aString *c, real val[],
                      char *extraName1 /* = 0 */, const int & extraValue1Location /* = 0 */, 
                      char *extraName2 /* = 0 */, const int & extraValue2Location /* = 0 */, 
                      char *extraName3 /* = 0 */, const int & extraValue3Location /* = 0 */ )
// ==============================================================================================
//  /Description:
//     Assign parameter values with names in the array $c$ and values in the array {\tt val}.
// The entries in {\tt c} correspond to one of componentName[n] or to one of the extra names.
// /label (input) : print this label when showing the responses. For example label="initial conditions".
// /numRead (input) :
// /c (input) : names of components
// /val (input) : value to assign to the component.
// /values (output) : return values in this array. For example the assignment `u=1.' will result in
//   values(uc)=1.
// /Return value: The number of variables assigned.
//\end{ParametersInclude.tex}  
// ==============================================================================================
{
  const int numberOfExtraNames=3;
  char *extraName[numberOfExtraNames] = {extraName1,extraName2,extraName3};
  const int extraValueLocation[numberOfExtraNames]={extraValue1Location,extraValue2Location,extraValue3Location};

  aString name;
  int i,n;
  real energy=-1.;
  int ie=-1;
  for( i=0; i<numRead; i++ )
  {
    bool found=false;
    name = c[i];	
    for( n=0; n< dbase.get<int >("numberOfComponents"); n++ )
    {
      if( name== dbase.get<aString* >("componentName")[n] )
      {
	values(n)=val[i];
	printF("Parameters::assignParameterValues:assigning %s: %s=%e \n",
                (const char *)label,(const char *)c[i],val[i]);
	found=true;
	break;
      }
    }
    if( !found )
    {
      for( int n=0; n<numberOfExtraNames; n++ )
      {
	if( extraName[n]!=0 && name==*extraName[n] )
	{
	  values(extraValueLocation[n])=val[i];
	  found=true;
          break;
	}
      }
      if( !found )
      {
	if( name=="e" )
	{
	  energy = val[i];
	  ie=i;
	  printF("Parameters::assignParameterValues:assigning %s: %s=%e \n",
                 (const char *)label,(const char *)c[i],val[i]);
	  found=true;
	}
      }
    }
    if( !found )
    {
      printF("ERROR: unknown parameter being assigned: name=%s, value=%e \n",(const char *)c[i],val[i]);
      // printF("     : input string=[%s]\n",(const char*)answer);
    }
  }
  if( ie>=0 )
  {
    // determine the temperature from the energy
    const real & rho = values(dbase.get<int >("rc"));

    if( values( dbase.get<int >("uc"))==(real)defaultValue )
      values( dbase.get<int >("uc"))=0.;
    if(  dbase.get<int >("numberOfDimensions")>1 && values( dbase.get<int >("vc"))==(real)defaultValue )
      values( dbase.get<int >("vc"))=0.;
    if(  dbase.get<int >("numberOfDimensions")>2 && values( dbase.get<int >("wc"))==(real)defaultValue )
      values( dbase.get<int >("wc"))=0.;
  }

  printF(" %s: (",(const char *)label);
  for( n=0; n< dbase.get<int >("numberOfComponents"); n++ )
  {
    if( values(n)!=(real)defaultValue )
      printF("%s=%f,",(const char *) dbase.get<aString* >("componentName")[n],values(n));
  }
  
  printF(")\n");

  return 0;
  
}

// ============================================================================
// /Description:
//    Given a string of the form
//       answer = "name1=value1, name2=value2 name3 = value, name4=value4"
//   This function will parse the string to determine the names and values and return as
//       name[i]="namei"
//       value[i]=valuei
// /name,value (input) : arrays of strings and reals that are at least maxNumber long.
// /Return value:
//    Number of names found. 
// ============================================================================
int Parameters::
parseValues( const aString & answer, aString *name, real *value, int maxNumber )
{

  int numNames=0;
  
  int ia,ib,j=0;
  int len=answer.length();
  while( j<len )
  {
    while( j<len && answer[j]==' ' || answer[j]==',' ) // skip blanks and ','
      j++;
    if( j==len ) break;
    ia=j;  // start of name
    while( j<len && answer[j]!='=' )  // search for '='
      j++;
    if( j==len ) break;
    ib=j-1; // end of name
    while( ib>ia && answer[ib]==' ' )  // remove trailing blanks
      ib--;
    
    if( numNames>=maxNumber ) break;
    
    name[numNames]=answer(ia,ib);

    j++;
    ia=j; // start of the number
    while( j<len && answer[j]!=' ' && answer[j]!=',' )
      j++;
    ib=j-1; // end of the number
    
    sScanF(answer(ia,ib),"%e",&value[numNames]);
    numNames++;
  }
  if( dbase.get<int >("debug") & 4 &&  dbase.get<int >("myid")==0 )
  {
    for( int i=0; i<numNames; i++ )
    {
      printF(" parseForValues : i=%i name=[%s] value=%8.2e\n",i,(const char*)name[i],value[i]);
    }
  }
  
  return numNames;
}


// ===================================================================================================================
/// \brief Parse an input string "answer" that assigns values to components.
/// \details Input parameter values from a string of the form: `u=1., v=2., ...' where the names on the
///      left-hand-side of the equal operator should correspond to one of 
//       parameters.dbase.get<aString* >("componentName")[n].
/// \param answer (input) : string containing the parameter value specification of the form: `u=1., v=2., ...'
/// \param label (input) : print this label when showing the responses. For example label="initial conditions".
/// \param values (output) : return values in this array. For example the assignment `u=1.' will result in
///   values(parameters.dbase.get<int >("uc"))=1.
/// \return return the number of variables assigned.
// ===================================================================================================================
int Parameters::
inputParameterValues(const aString & answer, const aString & label, RealArray & values )
{
  int maxNumberOfParameters = max( values.getLength(0), 10);
  aString * name = new aString [maxNumberOfParameters];
  real *value = new real [maxNumberOfParameters];

  int numNames=parseValues( answer, name, value, maxNumberOfParameters);

  
  assignParameterValues(label,values,numNames,name,value);
  
  delete [] name;
  delete [] value;
  
  return numNames;
}


// ===================================================================================================================
/// \brief Output the form of the polynomial TZ solution .
/// \param cx (input) : spatial coefficients
/// \param ct (input) : time coefficients.
/// \param componentName (input) : array of component names.
/// \param numberOfComponents (input) : number of components.
/// \param file (input) : write output to this file.
// ===================================================================================================================
void Parameters::
displayPolynomialCoefficients(RealArray & cx, RealArray & ct, aString * componentName, 
                              int numberOfComponents, FILE *file)
{
  const int ndp=cx.getBound(0)-cx.getBase(0)+1;
  
  fprintf(file,"---------------------------------------------------------------------\n");
  fprintf(file,"Here are the current polynomial twilight-zone solutions\n");
  for( int n=0; n< numberOfComponents; n++ )
  {
    fprintf(file,"%s(x,y,z,t)=[",(const char*)componentName[n]);
    for( int mz=0; mz<ndp; mz++ )
      for( int my=0; my<ndp; my++ )
	for( int mx=0; mx<ndp; mx++ )
	{
	  if( cx(mx,my,mz,n)!=0. )
	  {
	    fprintf(file," %+6.4f ",cx(mx,my,mz,n));
	    if( mx!=0 )
	      fprintf(file,"x^%i",mx); 
	    if( my!=0 )
	      fprintf(file,"y^%i",my);
	    if( mz!=0 )
	      fprintf(file,"z^%i\n",mz);
	  }
	}
    fprintf(file,"][ ",(const char*) componentName[n]);
    for( int mt=0; mt<ndp; mt++ )
    {
      if( ct(mt,n)!=0. )
      {
	fprintf(file," %+6.4f t^%i",ct(mt,n),mt);
      }
    }
    fprintf(file,"]\n");
  }
  fprintf(file,"---------------------------------------------------------------------\n");
}



// ===================================================================================================================
/// \brief Prompt for changes to the twilight zone parameters.
/// \param command (input) : if non-null, parse this string for a command.
/// \param interface (input) : use this graphics dialog.
// ===================================================================================================================
int Parameters::
setTwilightZoneParameters(CompositeGrid & cg, 
                          const aString & command /* = nullString */,
			  DialogData *interface /* =NULL */ )
{
  int returnValue=0;
  
  assert(  dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = * dbase.get<GenericGraphicsInterface* >("ps");
  aString answer2;
  char buff[80];
  
  aString prefix = "OBTZ:"; // prefix for commands to make them unique.

  const bool executeCommand = command!=nullString;
  if( false &&   // don't check prefix for now
      executeCommand && command(0,prefix.length()-1)!=prefix && command!="build dialog" )
    return 1;

  KnownSolutionsEnum & knownSolution = dbase.get<Parameters::KnownSolutionsEnum >("knownSolution");
  real & trigonometricTwilightZoneScaleFactor=
         dbase.get<real>("trigonometricTwilightZoneScaleFactor");  // scale factor for Trigonometric TZ

  GUIState gui;
  gui.setWindowTitle("Twilight Zone Options");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = interface!=NULL ? *interface : (DialogData &)gui;

  ArraySimpleFixed<real,4,1,1,1> & omega = dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");
  ArraySimpleFixed<real,9,1,1,1> & pulseData = dbase.get<ArraySimpleFixed<real,9,1,1,1> >("pulseData");

  if( interface==NULL || command=="build dialog" )
  {
    const int maxCommands=40;
    aString cmd[maxCommands];
    aString pushButtonCommands[maxCommands];

    int n,numRows;
    n=0;
    pushButtonCommands[n]="assign polynomial coefficients"; n++;
    pushButtonCommands[n]=""; n++;
    assert( n<maxCommands );

    numRows=n;
    addPrefix(pushButtonCommands,prefix,cmd,maxCommands);
    dialog.setPushButtons( cmd, pushButtonCommands, numRows );


    dialog.setOptionMenuColumns(1);

    aString label[] = {"polynomial","trigonometric","pulse",""}; //
    addPrefix(label,prefix,cmd,maxCommands);
    dialog.addOptionMenu("type", cmd,label, (int) dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"));

    aString label2[] = {"no known solution",
			// "axisymmetric rigid body rotation",
			"user defined known solution",
                        "known solution from a show file",
                        ""}; //
    addPrefix(label2,prefix,cmd,maxCommands);
    dialog.addOptionMenu("known solution", cmd,label2, (int) knownSolution);

    aString label3[] = {"maximum norm",
			"l1 norm",
			"l2 norm",""}; //
    addPrefix(label3,prefix,cmd,maxCommands);
    dialog.addOptionMenu("Error Norm", cmd,label3, ( dbase.get<int >("errorNorm")>2 ? 0 :  dbase.get<int >("errorNorm")));

    aString tbLabel[] = {"twilight zone flow",
                         "use 2D function in 3D",
                         "compare 3D run to 2D",
                         "assign TZ initial conditions",
                         ""};
    int tbState[5];
    tbState[0] =  dbase.get<bool >("twilightZoneFlow");
    tbState[1] =  dbase.get<int >("dimensionOfTZFunction")==2;
    tbState[2] =  dbase.get<int >("compare3Dto2D"); 
    tbState[3] =  dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow");
    addPrefix(tbLabel,prefix,cmd,maxCommands);

    int numColumns=1;
    dialog.setToggleButtons(cmd, tbLabel, tbState, numColumns); 

    const int numberOfTextStrings=8;
    aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

    int nt=0;
    textLabels[nt] = "degree in space"; 
    sPrintF(textStrings[nt], "%i",  dbase.get<int >("tzDegreeSpace")); nt++; 

    textLabels[nt] = "degree in time"; 
    sPrintF(textStrings[nt], "%i",  dbase.get<int >("tzDegreeTime")); nt++; 

    textLabels[nt] = "frequencies (x,y,z,t)"; 
    sPrintF(textStrings[nt], "%g, %g, %g, %g", omega[0], omega[1], omega[2],
	     omega[3]); 
    nt++; 

    // Pulse TZ function: 
    //    U &=  a_0 \exp( - a_1 | \xv-\bv(t) |^{2p} )
    //    \bv(t) &= \cv_0 + \vv t

    textLabels[nt] = "pulse amplitude, exponent, power"; 
    sPrintF(textStrings[nt], "%g %g %g",pulseData[0],pulseData[1],pulseData[8]); nt++; 

    textLabels[nt] = "pulse center"; 
    sPrintF(textStrings[nt], "%g %g %g",pulseData[2],pulseData[3],pulseData[4]); nt++; 

    textLabels[nt] = "pulse velocity";
    sPrintF(textStrings[nt], "%g %g %g",pulseData[5],pulseData[6],pulseData[7]); nt++; 

    textLabels[nt] = "trigonometric scale factor";
    sPrintF(textStrings[nt], "%g",trigonometricTwilightZoneScaleFactor); nt++; 


    // null strings terminal list
    textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );

    addPrefix(textLabels,prefix,cmd,maxCommands);
    dialog.setTextBoxes(cmd, textLabels, textStrings);
  }

  aString answer;
  
  if( !executeCommand  )
  {
    gi.pushGUI(gui);
    gi.appendToTheDefaultPrompt("TZ parameters>");
  }

  int len;
  for(int it=0; ; it++)
  {
    if( !executeCommand )
      gi.getAnswer(answer,"");
    else
    {
      if( it==0 ) 
        answer=command;
      else
        break;
    }
  
    if( answer(0,prefix.length()-1)==prefix )
      answer=answer(prefix.length(),answer.length()-1);

    if( answer.matches("polynomial") )
    {
      dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::polynomial;
      printF("use polynomial\n");
    }

    else if( dialog.getTextValue(answer,"trigonometric scale factor","%e",trigonometricTwilightZoneScaleFactor) )
    {
      printF("Setting the scale factor for the trigonmetric TZ function to %9.3e\n",
             trigonometricTwilightZoneScaleFactor);
    }

    else if( answer.matches("trigonometric") )
    {
      dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::trigonometric;
      printF("use trigonometric\n");

      dbase.get<bool >("userDefinedTwilightZoneCoefficients")=false;
    }
    else if( answer=="pulse" )
    {
      dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::pulse;
      printF("use pulse\n");

      dbase.get<bool >("userDefinedTwilightZoneCoefficients")=false;
    }
    else if( answer=="turn on twilight zone" ||
             answer=="turn on twilight" )
    {
      dbase.get<bool >("twilightZoneFlow")=true;
      printF("Setting twilightZoneFlow=%i\n", dbase.get<bool >("twilightZoneFlow"));
    }
    else if( answer=="turn off twilight zone" ||
             answer=="turn off twilight" )
    {
      dbase.get<bool >("twilightZoneFlow")=false;
      printF("Setting twilightZoneFlow=%i\n", dbase.get<bool >("twilightZoneFlow"));
    }
    else if( answer=="turn on polynomial" )
    {
      dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::polynomial;
      dbase.get<bool >("twilightZoneFlow")=true;
      printF("turn on polynomial\n");
    }
    else if( answer=="turn on trigonometric" )
    {
      dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")=Parameters::trigonometric;
      dbase.get<bool >("twilightZoneFlow")=true;
      printF("turn on trigonometric\n");
    }
    else if( answer=="frequencies" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter the x,y,z,t frequencies (default =%f,%f,%f,%f)",
				      omega[0], omega[1], omega[2], omega[3]));
      if( answer2!="" )
	sScanF(answer2,"%e %e %e %e",& omega[0],& omega[1],& omega[2],& omega[3]);
      printF("(omegaX,omegaY,omegaZ,omegaT)=(%e,%e,%e,%e)\n", omega[0], omega[1], omega[2], omega[3]);
    }
    else if( answer=="degree in space" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter degree in space (default =%i)", dbase.get<int >("tzDegreeSpace")));
      if( answer2!="" )
	sScanF(answer2,"%i",& dbase.get<int >("tzDegreeSpace"));
      printF(" tzDegreeSpace= %i\n", dbase.get<int >("tzDegreeSpace"));
    }
    else if( answer=="degree in time" )
    {
      gi.inputString(answer2,sPrintF(buff,"Enter degree in time (default =%i)", dbase.get<int >("tzDegreeTime")));
      if( answer2!="" )
	sScanF(answer2,"%i",& dbase.get<int >("tzDegreeTime"));
      printF(" tzDegreeTime=%i \n", dbase.get<int >("tzDegreeTime"));
    }
    else if( answer=="use 2D function in 3D" )
    {
       dbase.get<int >("dimensionOfTZFunction")=2;
    }
    else if( answer=="compare 3D run to 2D" )
    {
       dbase.get<int >("compare3Dto2D")=true;
       dbase.get<int >("dimensionOfTZFunction")=2;
    }
// ------- new versions
    else if( dialog.getToggleValue(answer,"twilight zone flow",dbase.get<bool >("twilightZoneFlow")) ){}//
//     else if( len=answer.matches("twilight zone flow") )
//     {
//       sScanF(answer(len,answer.length()-1),"%i",& dbase.get<bool >("twilightZoneFlow"));
//       printF(" twilightZoneFlow=%i \n", dbase.get<bool >("twilightZoneFlow"));
//       dialog.setToggleState("twilight zone flow", dbase.get<bool >("twilightZoneFlow"));
//     }
    else if( answer.matches("frequencies (x,y,z,t)") )
    {
      answer2=answer(21,answer.length()-1);
      sScanF(answer2,"%e %e %e %e",& omega[0],& omega[1],& omega[2],& omega[3]);
      printF("(omegaX,omegaY,omegaZ,omegaT)=(%e,%e,%e,%e)\n", omega[0], omega[1], omega[2], omega[3]);

      dialog.setTextLabel("frequencies (x,y,z,t)",
               sPrintF(answer2, "%g, %g, %g, %g", omega[0], omega[1], omega[2],  omega[3])); 
    }
    else if( len=answer.matches("degree in space") )
    {
      sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("tzDegreeSpace"));
      printF(" tzDegreeSpace= %i\n", dbase.get<int >("tzDegreeSpace"));
      dialog.setTextLabel("degree in space",sPrintF(answer2, "%i", dbase.get<int >("tzDegreeSpace")));
    }
    else if( len=answer.matches("degree in time") )
    {
      sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("tzDegreeTime"));
      printF(" tzDegreeTime=%i \n", dbase.get<int >("tzDegreeTime"));
      dialog.setTextLabel("degree in time",sPrintF(answer2, "%i", dbase.get<int >("tzDegreeTime")));
    }
    else if( len=answer.matches("pulse amplitude, exponent, power") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&pulseData[0],&pulseData[1],&pulseData[8]);
      printF(" Pulse: amplitude=%e, exponent=%e, power=%e\n",pulseData[0],pulseData[1],pulseData[8]);
      dialog.setTextLabel("pulse amplitude, exponent, power",
                          sPrintF(answer2, "%g %g %g",pulseData[0],pulseData[1],pulseData[8]));
    }
    else if( len=answer.matches("pulse center") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&pulseData[2],&pulseData[3],&pulseData[4]);
      printF(" Pulse: center=(%e,%e,%e)\n",pulseData[2],pulseData[3],pulseData[4]);
      dialog.setTextLabel("pulse center",
                          sPrintF(answer2, "%g %g %g",pulseData[2],pulseData[3],pulseData[4]));
    }
    else if( len=answer.matches("pulse velocity") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e",&pulseData[5],&pulseData[6],&pulseData[7]);
      printF(" Pulse: velocity=(%e,%e,%e)\n",pulseData[5],pulseData[6],pulseData[7]);
      dialog.setTextLabel("pulse velocity",
                          sPrintF(answer2, "%g %g %g",pulseData[5],pulseData[6],pulseData[7]));
    }


    else if( answer=="assign polynomial coefficients" )
    {
      // printf(
      const int ndp=5;
      RealArray cx(ndp,ndp,ndp, dbase.get<int >("numberOfComponents"));   
      RealArray ct(ndp, dbase.get<int >("numberOfComponents")); 
      NameList nl;       

      if(  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==polynomial )
      {
        if(  dbase.get<OGFunction* >("exactSolution")==NULL )
	{
	  setTwilightZoneFunction( dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"), dbase.get<int >("tzDegreeSpace"), dbase.get<int >("tzDegreeTime"));
	}
	
        // get the current values of the coefficients
	((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->getCoefficients( cx,ct );  // for u

	displayPolynomialCoefficients(cx,ct, dbase.get<aString* >("componentName"), dbase.get<int >("numberOfComponents"),stdout);

      }
      else
      { // we allow changes to the coefficients even if we don't have  a polynomial TZ function -- this means we
        // can keep these changes in the command file even if they are not used.
	cx=0.;
	ct=0.;
      }
      
      printF("Make changes to the current coefficients of the polynomial twilight-zone function.\n"
             "Enter cx(mx,my,mz,mc)=value to set the coefficient of x^{mx} y^{my} z^mz for component mc \n"
	     "Enter ct(mt,mc)=value to set the coefficient of t^{mt} for component mc \n"
	     "Enter `done' to finish\n");
    
      int i0,i1,i2,i3;
      aString name;
      // ==========Loop for changing coefficients========================
      for( ;; ) 
      {
	gi.inputString(answer,"Enter changes to cx or ct or `done' to finish\n"); 
	if( answer=="done" || answer=="continue" || answer=="exit" ) break;
	nl.getVariableName( answer, name );   // parse the answer

	if( name== "cx" )   
	{
	  nl.getRealArray( answer,cx,i0,i1,i2,i3 );
          printF(" Setting cx(%i,%i,%i,%i)=%9.3e\n",i0,i1,i2,i3,cx(i0,i1,i2,i3));

	   dbase.get<bool >("userDefinedTwilightZoneCoefficients")=true;
	}
	else if( name== "ct" )   
	{
	  nl.getRealArray( answer,ct,i0,i1 );
          printF(" Setting ct(%i,%i)=%9.3e\n",i0,i1,ct(i0,i1));

	   dbase.get<bool >("userDefinedTwilightZoneCoefficients")=true;
	}
	else
	  printF("unknown response: answer=[%s]\n",(const char*)answer);
      }

      if(  dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==polynomial )
      {
	((OGPolyFunction*) dbase.get<OGFunction* >("exactSolution"))->setCoefficients( cx,ct );  // for u
	displayPolynomialCoefficients(cx,ct, dbase.get<aString* >("componentName"), dbase.get<int >("numberOfComponents"),stdout);
      }
      else
      {
	printF("WARNING: To set the polynomial coefficients th twilightzone function must be a polynomial\n"
               " The coefficients have not been changed");
      }
      

    }
    else if( len=answer.matches("use 2D function in 3D") )
    {
      int state=0;
      sScanF(answer(len,answer.length()-1),"%i",&state);
      if( state==1 )
	 dbase.get<int >("dimensionOfTZFunction")=2;
      else
	 dbase.get<int >("dimensionOfTZFunction")= dbase.get<int >("numberOfDimensions"); // is this set?
      dialog.setToggleState("use 2D function in 3D",state);
    }
    else if( len=answer.matches("compare 3D run to 2D") )
    {
      sScanF(answer(len,answer.length()-1),"%i",& dbase.get<int >("compare3Dto2D"));
      if(  dbase.get<int >("compare3Dto2D") )
	 dbase.get<int >("dimensionOfTZFunction")=2;
      else
	 dbase.get<int >("dimensionOfTZFunction")= dbase.get<int >("numberOfDimensions");
      dialog.setToggleState("compare 3D run to 2D", dbase.get<int >("compare3Dto2D"));
    }
    else if( len=answer.matches("assign TZ initial conditions") )
    {
      int state=0;
      sScanF(&answer[len],"%i",&state);
       dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow")=state;
      dialog.setToggleState("assign TZ initial conditions",state);
    }
    else if( answer=="no known solution" || 
             answer=="known solution from a show file" ||
             answer=="user defined known solution" )
    {
      knownSolution=(answer=="no known solution" ? noKnownSolution : 
                     answer=="known solution from a show file" ? knownSolutionFromAShowFile :
                     userDefinedKnownSolution );

      if( knownSolution==userDefinedKnownSolution )
      { // choose a user defined known solution:

	int returnValue=updateUserDefinedKnownSolution(gi,cg);
	
        if( returnValue==0 )
	{
           knownSolution=noKnownSolution; // reset -- no known solution chosen.
	}
      }
      else if( knownSolution==knownSolutionFromAShowFile )
      {
        // new: 100808
	ShowFileReader showFileReader;
	CompositeGrid cgSF;
        realCompositeGridFunction uSF;
	int solutionNumber=-1;

        dbase.get<bool>("knownSolutionIsTimeDependent")=false;  // known solution does NOT depend on time

 	readFromAShowFile(showFileReader,cg,cgSF,uSF,solutionNumber);

        // Interpolate the show file known solution onto current grid 
	printF("Transfer the known solution from the show file to the current grid...\n");
	
	realCompositeGridFunction *& pKnownSolution = dbase.get<realCompositeGridFunction* >("pKnownSolution");
	if( pKnownSolution==NULL )
	{
	  pKnownSolution = new realCompositeGridFunction;
	}
	realCompositeGridFunction & uKnown = *pKnownSolution;
	Range all;
        uKnown.updateToMatchGrid(cg,all,all,all,dbase.get<int >("numberOfComponents"));
	uKnown=0.;
	
        InterpolatePointsOnAGrid interp;
        int interpWidth=3;    // default is 2  // *wdh* 101117
        interp.setInterpolationWidth( interpWidth );  // *wdh* 101117
	interp.setAssignAllPoints(true);
	interp.interpolateAllPoints(uSF,uKnown);


	  if( false  ) // ********************************************************* TEMP
	  {
	    printF("\n **** INFO: extrap ghost on uKnown ****");
	    
	    // for testing extrap boundary values
            CompositeGridOperators cgop(cg);
	    uKnown.setOperators(cgop);
	    Range C=dbase.get<int >("numberOfComponents");

	    BoundaryConditionParameters extrapParams;
	    extrapParams.orderOfExtrapolation=3;  // what should this be? 

	    extrapParams.ghostLineToAssign=1;  // extrap 1st ghost
	    uKnown.applyBoundaryCondition(C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,0.,extrapParams); 

	  }




      }

      printF(" Setting the known solution to %i (%s) \n",(int) knownSolution,(const char*)answer);
      if( knownSolution!=noKnownSolution )
      {
	 dbase.get<Parameters::InitialConditionOption >("initialConditionOption")=knownSolutionInitialCondition;
      }
      
    }
    else if( answer=="maximum norm" || 
             answer=="l1 norm" ||
             answer=="l2 norm" )
    {
       dbase.get<int >("errorNorm")= (answer=="maximum norm" ? INTEGER_MAX :
                  answer=="l1 norm" ? 1 :
                  answer=="l2 norm" ? 2 : INTEGER_MAX);
    }
    else
    {
      if( executeCommand )
      {
	returnValue= 1;  // when executing a single command, return 1 if the command was not recognised.
        break;
      }
      else
      {
	printF("Unknown response: [%s]\n",(const char*)answer);
	gi.stopReadingCommandFile();
      }
    }
  }
  
  if( !executeCommand  )
  {
    gi.popGUI();
    gi.unAppendTheDefaultPrompt();
  }

  return returnValue;
}



// ===================================================================================================================
/// \brief Prompt for changes in the PDE parameters.
/// \param cg (input) : Composite grid to use. 
/// \param command (input) : if non-null, parse this string for a command.
/// \param interface (input) : use this graphics dialog.
// ===================================================================================================================
int Parameters::
setPdeParameters(CompositeGrid & cg,
                 const aString & command /* = nullString */,
                 DialogData *interface /* =NULL */ )
{
  int returnValue=1;

  return returnValue;
}



// ===================================================================================================================
/// \brief Display PDE parameters.
/// \param file (input) : output information to this file.
// ===================================================================================================================
int Parameters::
displayPdeParameters(FILE *file /* = stdout */ )
{
  // display parameters help in the  dbase.get<DataBase >("modelParameters") data-base
  for( DataBase::iterator e= dbase.get<DataBase >("modelParameters").begin(); e!= dbase.get<DataBase >("modelParameters").end(); e++ )
  {
    string name=(*e).first;
    printf("modelParameters: %s=",name.c_str());
    DBase::Entry &entry = *((*e).second);
    if( DBase::can_cast_entry<real>(entry) )
    {
      real value=cast_entry<real>(entry);  
      printf("%9.3e\n",value);
    }
    else if( DBase::can_cast_entry<int>(entry) )
    {
      printf("%i\n",cast_entry<int>(entry));
    }
    else if( DBase::can_cast_entry<string>(entry) )
    {
      const string & s = cast_entry<string>(entry);
      printf("%s\n",s.c_str());
    }
    else
    {
      printf("? (unknown type)\n");
    }
  }

  return 0;
}

// ===================================================================================================================
/// \brief Update the PDE parameters to be consistent after some values have changed.
/// \details Update the dimensional PDE parameters such as mu if the non-dimensional
/// parameters (Reynolds number, mach number etc) were specified.
// ===================================================================================================================
int Parameters::
updatePDEparameters()
{

  return 1;
}

// ===================================================================================================================
/// \brief Update the known solution to match the grid. 
/// \details This routine will update the
///  grid function that holds the known solution to be the correct dimensions. It will only evaluate
/// the known solution if the knownSolution is being allocated for the first time.
/// \param cg (input) : match to this grid.
// ===================================================================================================================
int Parameters::
updateKnownSolutionToMatchGrid(CompositeGrid & cg )
{
  if( dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")==noKnownSolution )
  {
    return 0;
  }

  realCompositeGridFunction *& pKnownSolution = dbase.get<realCompositeGridFunction* >("pKnownSolution");

  Range all;
  if( pKnownSolution==NULL )
  {
     pKnownSolution = new realCompositeGridFunction(cg,all,all,all, dbase.get<int >("numberOfComponents"));
    const real t=0.;
    getKnownSolution( cg,t );
  }
  else 
  {
     pKnownSolution->updateToMatchGrid(cg,all,all,all, dbase.get<int >("numberOfComponents"));
  }
  return 0;
}



// ===================================================================================================================
/// \brief Return a known solution.
/// \param cg (input) : match to this grid.
/// \param t (input) : time to evaluate the known solution.
/// \param numberOfTimeDerivatives (input) : number of time derivatives to evaluate (0 means evaluate the solution).
/// \return Return a reference to the known solution.
// ===================================================================================================================
realCompositeGridFunction& Parameters::
getKnownSolution( CompositeGrid & cg, real t, int numberOfTimeDerivatives /* =0 */ )
{
  bool initialCall=false;
  realCompositeGridFunction *& pKnownSolution = dbase.get<realCompositeGridFunction* >("pKnownSolution");
  if( pKnownSolution==NULL )
  {
    initialCall=true;
    Range all;
    pKnownSolution = new realCompositeGridFunction(cg,all,all,all, dbase.get<int >("numberOfComponents"));
  }
  else // if( cg.numberOfComponentGrids()!=pKnownSolution->numberOfComponentGrids() )
  {
    // ??? Is this correct to always update this ?? *wdh* 100809
    Range all;
    pKnownSolution->updateToMatchGrid(cg,all,all,all,dbase.get<int >("numberOfComponents"));
  }
  

  Index I1,I2,I3;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    getIndex(mg.dimension(),I1,I2,I3);
    getKnownSolution( t,grid,I1,I2,I3,initialCall,numberOfTimeDerivatives);
  }
  return *pKnownSolution;
}

// ===================================================================================================================
/// \brief Return a known solution on a component grid.
/// \param t (input) : time to evaluate the known solution.
/// \param grid (input) : match to this grid.
/// \param I1 (input) :
/// \param I2 (input) :
/// \param I3 (input) :
/// \param initialCall (input) : true if this is the initial call. 
/// \param numberOfTimeDerivatives (input) : number of time derivatives to evaluate (0 means evaluate the solution).
/// \return Return a reference to the known solution.
/// \note This routine assumes that getKnownSolution(cg,t) has been intially called to allocate space. 
// ===================================================================================================================
realMappedGridFunction& Parameters::
getKnownSolution(real t, int grid, const Index & I1, const Index &I2, const Index &I3, 
                 bool initialCall /* =false */, 
                 int numberOfTimeDerivatives /* = 0 */  )
{
  realCompositeGridFunction *& pKnownSolution = dbase.get<realCompositeGridFunction* >("pKnownSolution");
  if( pKnownSolution==NULL )
  {
    printF("Parameters::getKnownSolution(grid):ERROR: you should call getKnownSolution(cg,t) first\n");
    OV_ABORT("error");
  }
  
  int & numberOfDimensions = dbase.get<int >("numberOfDimensions");
  realCompositeGridFunction & uKnown = *pKnownSolution;
  CompositeGrid & cg = *uKnown.getCompositeGrid();

  // For moving grids or time dependent problems, or for AMR refinement grids we always re-evaluate the known solution
  initialCall = initialCall || gridIsMoving(grid) ||  dbase.get<bool>("knownSolutionIsTimeDependent") || 
                (  dbase.get<bool >("adaptiveGridProblem") && cg.refinementLevelNumber(grid)>0);  

  if( dbase.get<int >("debug") & 16 )
  {
    printF("getKnownSolution: t=%9.3e : grid=%i gridIsMoving=%i knownSolutionIsTimeDependent=%i "
	   "evaluate known solution=%i\n",t,grid,
	   (int)gridIsMoving(grid),(int)dbase.get<bool>("knownSolutionIsTimeDependent"),(int)initialCall);
  }
  
  if( !initialCall ) // *wdh* 110103 
   return uKnown[grid];

  assert( grid < uKnown.numberOfComponentGrids() );

  MappedGrid & mg = cg[grid];
  realArray & ua = uKnown[grid]; 
  OV_GET_SERIAL_ARRAY(real,ua,uaLocal);

  const KnownSolutionsEnum & knownSolution = dbase.get<KnownSolutionsEnum >("knownSolution");

  if( knownSolution==userDefinedKnownSolution )
  {
    getUserDefinedKnownSolution(t,cg,grid,uaLocal,I1,I2,I3,numberOfTimeDerivatives);

    if( dbase.get<int >("debug") & 16 ) 
      ::display(uaLocal,sPrintF("getKnownSolution: grid=%i, t=%9.3e",grid,t),dbase.get<FILE* >("pDebugFile"),"%4.2f ");
  }
  else if( knownSolution==knownSolutionFromAShowFile )
  {
    if( initialCall )
    {
      printF("Parameters::getKnownSolution:ERROR: FINISH ME - "
             "re-eval known solution from a show file on a new grid\n");
      // OV_ABORT("ERROR")
    }
    
  }
  else
  {
    printF("Parameters::getKnownSolution:ERROR: unknown knownSolution=%i\n", (int)knownSolution);
    OV_ABORT("ERROR");
  }

  return uKnown[grid];
}


// ===================================================================================================================
/// \brief Set user defined parameters.
// ===================================================================================================================
int Parameters::
setUserDefinedParameters()
{
  return 1;
}

// ===================================================================================================================
/// \brief Convert primitive variables to conservative.
/// \param gf (input) : convert this grid function.
/// \param gridToConvert (input) : (grid==-1) convert all grids, otherwise convert this grid.
/// \param fixupUnsedPoints (input) : if true, fixup unused points.
/// \note 
///     - primitive : rho, u,v,w, T, species
///     - conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
// ===================================================================================================================
int Parameters::
primitiveToConservative(GridFunction & gf,
                        int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */)
{
  abort();
  return 1;
}

// ===================================================================================================================
/// \brief Convert conservative variables to primitive.
/// \param gf (input) : convert this grid function.
/// \param gridToConvert (input) : (grid==-1) convert all grids, otherwise convert this grid.
/// \param fixupUnsedPoints (input) : if true, fixup unused points.
/// \note 
///     - primitive : rho, u,v,w, T, species
///     - conservative rho, (rho*u), (rho*v), (rho*w), E, (rho*species)
// ===================================================================================================================
int Parameters::
conservativeToPrimitive(GridFunction & gf,
                        int gridToConvert  /* =-1 */, 
                        int fixupUnsedPoints /* =false */ )
{
  abort();
  return 1;
}


// ===================================================================================================================
/// \brief Assign the values of a derived quantity.
/// \details This function knows how to compute some "derived" quantities. For example for the 
///    compressible Navier-Stokes, the pressure can be computed.
/// \param name (input) the name of the derived quantity.
/// \param u (input) : evaluate the derived function using this grid function.
/// \param v (input) : fill in a component of this grid function. 
/// \param component (input) : component index to fill, i.e. fill v(all,all,all,component)
/// \param parameters (input) : 
// ===================================================================================================================
int Parameters::
getDerivedFunction(const aString & name, 
                   const realCompositeGridFunction & u,
                   realCompositeGridFunction & v, 
                   const int component, const real t, 
                   Parameters & parameters)
{
  CompositeGrid & cg = *v.getCompositeGrid();
  
  int ok=0;
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    ok = getDerivedFunction(name,u[grid],v[grid],grid,component,t,parameters);
    if ( ok!=0 ) break;
  }
  return ok;
}


// ===================================================================================================================
/// \brief Assign the values of a derived quantity.
/// \details This function knows how to compute some "derived" quantities. For example for the 
///    compressible Navier-Stokes, the pressure can be computed.
/// \param name (input) : the name of the derived quantity.
/// \param uIn (input) : evaluate the derived function using this grid function.
/// \param vIn (input) : fill in a component of this grid function. 
/// \param component (input) : component index to fill, i.e. fill v(all,all,all,component)
/// \param parameters (input) : 
// ===================================================================================================================
int Parameters::
getDerivedFunction( const aString & name, const 
                    realMappedGridFunction & uIn, 
                    realMappedGridFunction & vIn, 
                    const int grid, const int component, const real t, 
                    Parameters & parameters)
{
  MappedGrid & mg = *vIn.getMappedGrid();


  printf("getDerivedFunction:ERROR: unknown derived function! name=%s\n",(const char*)name);
  return 1;
}

// ===================================================================================================================
/// \brief Get an array of component indices.  IS THIS USED ANYMORE? YES, BY THE RAMP BC
/// \param components (output): the list of component indices.
// ===================================================================================================================
int Parameters::
getComponents( IntegerArray &components )
{
  int numberToSet=dbase.get<int >("numberOfDimensions");  // this may not be correct 
  if ( dbase.get<int >("tc")>=0 ) numberToSet++;
  components.redim(numberToSet);
  int n=0;
  components(n++)=dbase.get<int >("uc");
  components(n++)=dbase.get<int >("vc");
  if( dbase.get<int >("wc")>=0 ) components(n++)=dbase.get<int >("wc");
  if( dbase.get<int >("tc")>=0 ) components(n++)=dbase.get<int >("tc");

  return 0;
}

// ===================================================================================================================
/// \brief Add a bc with an integer identifier and a string name.
/// \param id (input) : identifier.
/// \param name (input) : name of the boundary condition.
/// \param replace (input) : if true, replace a boundary condition with the same id. If false do not replace. 
/// \return
///  This method returns true if the addition was ok, false if there is an existing bc with id and replace==false.
// ===================================================================================================================
bool Parameters::
registerBC(int id, const aString & name, bool replace /*= false */ )
{
  if( !replace && bcNames.count(id) ) return false;
  bcNames[id] = name;
  numberOfBCNames++;
  return true;
}

// ===================================================================================================================
/// \brief Add a interface type with an integer identifier and a string name.
/// \param id (input) : identifier.
/// \param name (input) : name of the interface type
/// \param replace (input) : if true, replace a boundary interface with the same id. If false do not replace. 
/// \return
///  This method returns true if the addition was ok, false if there is an existing bc with id and replace==false.
// ===================================================================================================================
bool Parameters::
registerInterfaceType(int id, const aString & name, bool replace /*= false */ )
{
  if( !replace && icNames.count(id) ) return false;
  icNames[id] = name;
  // numberOfICNames++;
  return true;
}

// ===================================================================================================================
/// \brief Add a bc modifier with an integer identifier and a string name.
/// \param id (input) : identifier.
/// \param name (input) : name of the boundary condition.
/// \param createBCMod (input) : a function pointer to a function that create's BCModifier instances with the name "name"
/// \param replace (input) : if true, replace a boundary condition with the same id. If false do not replace. 
/// \return
///  This method returns true if the addition was ok, false if there is an existing bc with id and replace==false.
// ===================================================================================================================
bool Parameters::
registerBCModifier(const aString &name, Parameters::CreateBCModifierFromName createBCMod, bool replace/*=false*/)
{
  if( !replace && bcModCreators.count(name.c_str()) ) return false;
  bcModCreators[name.c_str()] = createBCMod;
  return true;
}

// ===================================================================================================================
/// \brief Return the normal force on a boundary.
/// \details This routine is called, for example, by MovingGrids::rigidBodyMotion to determine 
///       the motion of a rigid body.
/// \param u (input): solution to compute the force from.
/// \param normalForce (output) : fill in the components of the normal force. 
/// \param ipar (input) : integer parameters. The boundary is defined by grid=ipar[0], side=ipar[1], axis=ipar[2] 
/// \param rpar (input) : real parameters. The current time is t=rpar[0]
// ===================================================================================================================
int Parameters::
getNormalForce( realCompositeGridFunction & u, realSerialArray & normalForce, int *ipar, real *rpar )
{
  printF("Parameters::getNormalForce:ERROR base class called!\n");
  Overture::abort("error");
  return 0;
}



// ===================================================================================
/// \brief Allocate the boundary data for a given side of a grid.
/// \details Some boundary data may have so be saved since it is too expensive to recompute. e.g. parabolicInfow
/// Return an array to use on a given face (allocate it if necessary)
/// \param side,axis,grid (input) : face
/// \param mg (input) : the MappedGrid
// ===================================================================================
RealArray & Parameters::
getBoundaryData(int side, int axis, int grid, MappedGrid & mg )
{

  std::vector<BoundaryData> & boundaryData = dbase.get<std::vector<BoundaryData> >("boundaryData");

  assert( grid>=0 );
  if( grid >= boundaryData.size() )
    boundaryData.resize(grid+1,BoundaryData());


  Range C = dbase.get<int >("numberOfComponents"); 
  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  // *wdh* 091020 -- allocate the boundary-data using the dimension
  if( false )
  {
    int extra=1;
    getBoundaryIndex(mg.extendedIndexRange(),side,axis,I1,I2,I3,extra);
  }
  else
  { // *new* 
    getBoundaryIndex(mg.dimension(),side,axis,I1,I2,I3);
    Iv[axis]=Range(mg.gridIndexRange(side,axis),mg.gridIndexRange(side,axis));
  }
  
  bool ok=true;
  #ifdef USE_PPP
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mg.mask(),maskLocal);
    int includeGhost=1;
    ok=ParallelUtility::getLocalArrayBounds(mg.mask(),maskLocal,I1,I2,I3,includeGhost);
  #endif


  RealArray *&pBoundaryData = boundaryData[grid].boundaryData[side][axis];
  if( pBoundaryData!=NULL )
  { // check if the boundary data array is the correct size (this check may be needed for AMR) *wdh* 090819
    // **but note that sometimes we allocate space for the ghost point ***
    RealArray & bd = *pBoundaryData;
    if( !ok ||
        ( (bd.getBase(0)!=I1.getBase()  &&  bd.getBase(0)!=I1.getBase()-1) ||       // *wdh* 100201 fixed && to ||
         (bd.getBound(0)!=I1.getBound() && bd.getBound(0)!=I1.getBound()+1) ) || 
        ( (bd.getBase(1)!=I2.getBase()  &&  bd.getBase(1)!=I2.getBase()-1) || 
         (bd.getBound(1)!=I2.getBound() && bd.getBound(1)!=I2.getBound()+1) ) ||
        ( (bd.getBase(2)!=I3.getBase()  &&  bd.getBase(2)!=I3.getBase()-1) || 
         (bd.getBound(2)!=I3.getBound() && bd.getBound(2)!=I3.getBound()+1) ) ||
        bd.dimension(3)!=C  )
    {
      if( false )
      {
	printF("******* getBoundaryData:INFO: re-allocate boundary data for (grid,side,axis)=(%i,%i,%i)"
	       " as the dimensions have changed\n",grid,side,axis);
	printF(" bd=[%i,%i][%i,%i][%i,%i][%i,%i]  I1=[%i,%i] I2=[%i,%i] I3=[%i,%i]\n",
	       bd.getBase(0),bd.getBound(0),bd.getBase(1),bd.getBound(1),bd.getBase(2),bd.getBound(2),
               bd.getBase(3),bd.getBound(3),
               I1.getBase(),I1.getBound(), I2.getBase(),I2.getBound(), I3.getBase(),I3.getBound());
	
      }
      

      delete pBoundaryData;
      pBoundaryData=NULL;
    }
  }
  if( pBoundaryData==NULL )
  {
    if( ok )
    {
      pBoundaryData = new RealArray(I1,I2,I3,C);
      RealArray & bd = *pBoundaryData;
      bd=0.;  //  *wdh* 090819
      return *pBoundaryData;
    }
    else
    {
      return Overture::nullRealArray();
    }
    
  }
  else
  {
    return *pBoundaryData;
  }
}

// ===================================================================================
/// \brief return the boundary data for a grid
// ===================================================================================
BoundaryData::BoundaryDataArray& Parameters::
getBoundaryData( int grid )
{
  std::vector<BoundaryData> & boundaryData = dbase.get<std::vector<BoundaryData> >("boundaryData");

  assert( grid>=0 );
  if( grid >= boundaryData.size() )
    boundaryData.resize(grid+1,BoundaryData());

  return boundaryData[grid].boundaryData;

}


// ===================================================================================================================
/// \brief Set the boundary condition midifier id 
/// \param side (input) : side
/// \param axis (input) : axis
/// \param grid (input) : grid
/// \param bcm (input) : boundary condition modifier id.
// ==================================================================================================================
int Parameters::
setBcModifier(int side, int axis, int grid, int bcm)
{
   dbase.get<IntegerArray >("bcInfo")(2,side,axis,grid)=bcm;
  return 0;
}

const bool 
Parameters::BCModifier::
isPenaltyBC() const {return false;}


int Parameters::
updateUserDefinedEOS(GenericGraphicsInterface & gi)
{
  printF("Parameters::WARNING: There are no user defined equations of state.\n");
  return 0;
}
