#include "Ogen.h"



Ogen::
Ogen()
{
  initialize();
}


Ogen::
Ogen(GenericGraphicsInterface & ps_)
{
  ps=&ps_;
  initialize();
}

Ogen::
~Ogen()
{
  if( logFile!=NULL )
    fclose(logFile);
  #ifdef USE_PPP
    fclose(plogFile);
  #endif
  if( checkFile!=NULL )
    fclose(checkFile);
}


int Ogen::
initialize()
{
  myid=max(0,Communication_Manager::My_Process_Number);
  const int np=Communication_Manager::numberOfProcessors();
  
  printF("Ogen: initialize start\n");


  debug=0;
  info=0;
  logFile=NULL;
  #ifdef USE_PPP
    if( myid==0 )
    {
      logFile = fopen(sPrintF("ogenNP%i.log",np),"w" ); 
    }
    plogFile = fopen(sPrintF("ogenNP%i.p%i.log",np,myid),"w" ); 
    fprintf(plogFile,
            " ********************************************************************************** \n"
	    " ************** Ogen log file, processor=%i, number of processors=%i *************** \n"
	    " ********************************************************************************** \n\n",
                                 myid,np);
  #else
    logFile = fopen("ogen.log","w" ); 
    plogFile=logFile;
  #endif

  fprintf(plogFile,"Ogen: myid=%i initialize start\n",myid);


  fPrintF(logFile,
	  " ********************************************* \n"
	  " ************** Ogen log file **************** \n"
	  " ********************************************* \n\n");
  
  checkFile=NULL;
  if( myid==0 )
  {
    // checkFile: contains data on the grid that we can use to check when we change the code.
    checkFile = fopen("ogen.check","w");  
  }
  
  fPrintF(checkFile," ********************************************* \n"
                    " ************** Ogen check file ************** \n"
                    " ********************************************* \n\n");
  fPrintF(checkFile," This grid data is used to make sure the grid generator is giving \n"
                    " the same answer after changes are made. Compare this file to a the \n"
                    " check file from previous version. \n\n");

  // by default we invalidate and recompute the geometry for grids that move: 
  computeGeometryForMovingGrids=true;

  plotTitles=true;  // label intermediate steps if plotted

  boundaryEps=0.;  // this is set in updateOverlap since it comes from cg.epsilon()

  makeAdjustmentsForNearbyBoundaries=false; // make fixes when two boundaries approach each other with little overlap

  checkForOneSided=false;  // check for one side interp from non-conforming grids *******
  maskRatio[0]=maskRatio[1]=maskRatio[2]=1;  // for multigrid: ratio of coarse to fine grid spacing for ray tracing

  #ifdef USE_PPP
    holeCuttingOption=2;  // 2=new way (parallel version)
  #else
    holeCuttingOption=1;  // 0=old way, 1=current
  #endif  
  useBoundaryAdjustment=true;

  maximumAngleDifferenceForNormalsOnSharedBoundaries=.1; //    1-cos(angle)

  maximumNumberOfPointsToInvertAtOneTime=200000; // limit the number of pts we invert at a time to save memory
  
  improveQualityOfInterpolation=false;
  qualityBound=2.;
  minimizeTheOverlap=true;
  allowHangingInterpolation=false;
  allowBackupRules=false; // leave false for better error messages.
  useNewMovingUpdate=true; // fix variableInterpolationWidth *** true;
  isMovingGridProblem=false;
  useLocalBoundingBoxes=true;  // new  parallel option
  loadBalanceGrids=false;      // load balance cg when it is created
  doubleCheckInterpolation=false;
  
  backupValues=NULL;

  numberOfArrays=0;
  numberOfHolePoints=0;
  numberOfOrphanPoints=0;
  
  numberOfMixedBoundaries=0;
  numberOfManualHoles=0;
  numberOfManualSharedBoundaries=0;
  numberOfSharedBoundaryTolerances=0;
  
  numberOfNonCuttingBoundaries=0;
  
  plotExplicitHoleCutters=true;

  incrementalHoleSweep=0;
  
  // Here is the default number of ghost lines to use when building grids. *wdh* 070401 -- changed to 2 
  defaultNumberOfGhostPoints=2;

  totalTime=0.;
  timeUpdateGeometry=0.;
  timeInterpolateBoundaries=0.;
  timePreInterpolate=0.;
  timeCutHoles=0.;
  timeCheckHoleCutting=0.;
  timeFindTrueBoundary=0.;
  timeRemoveExteriorPoints=0.;
  timeImproperInterpolation=0.;
  timeProperInterpolation=0.;
  timeAllInterpolation=0.;
  timeRemoveRedundant=0.;
  timeImproveQuality=0.;

  classifyHolesForHybrid = false;
  defaultInterpolationIsImplicit=true; // false; // true; // false;
  
  // these are normally on set when not running interactively
  outputGridOnFailure=false;  // save the grid in a file if the algorithm fails
  abortOnAlgorithmFailure=false;

  fflush(0);
  fprintf(plogFile,"Ogen: myid=%i initialize end\n",myid);
  printF("Ogen: initialize end\n");
  Communication_Manager::Sync();

  return 0;
}

  
void Ogen::
getSharedBoundaryTolerances( CompositeGrid & cg, int grid1, int side1, int dir1, int grid2, int side2, int  dir2,
			     real & rTol, real & xTol, real & nTol ) const
// ==============================================================================================================
// Return the tolerances for determining which points interpolate on shared boundaries.
//
// 
// ==============================================================================================================
{
  // first check values in manual shared boundaries which take precedence
  int n;
  for( n=0; n<numberOfManualSharedBoundaries; n++ )
  {
    if( manualSharedBoundary(n,0)==grid1 && manualSharedBoundary(n,3)==grid2 &&
	manualSharedBoundary(n,1)==side1 && manualSharedBoundary(n,2)==dir1 &&
	manualSharedBoundary(n,4)==side2 && manualSharedBoundary(n,5)==dir2   )
    {
      rTol=manualSharedBoundaryValue(n,0);
      xTol=manualSharedBoundaryValue(n,1);
      nTol=manualSharedBoundaryValue(n,2);
      return;
    }
  }

  // now check for any changes to the default
  for( n=0; n<numberOfSharedBoundaryTolerances; n++ )
  {
    if( sharedBoundaryTolerances(n,0)==grid1 && sharedBoundaryTolerances(n,3)==grid2 &&
	sharedBoundaryTolerances(n,1)==side1 && sharedBoundaryTolerances(n,2)==dir1 &&
	sharedBoundaryTolerances(n,4)==side2 && sharedBoundaryTolerances(n,5)==dir2   )
    {
      rTol=sharedBoundaryTolerancesValue(n,0);
      xTol=sharedBoundaryTolerancesValue(n,1);
      nTol=sharedBoundaryTolerancesValue(n,2);
      return;
    }
  }
  rTol=.8;
  xTol=REAL_MAX*.1;
  nTol=maximumAngleDifferenceForNormalsOnSharedBoundaries;
}


//\begin{>>ogenInclude.tex}{\subsubsection{set(OgenParameter::option, bool)}}
void Ogen::
set(const OgenParameterEnum option, const bool value)
// =========================================================================================
// /Description:
// set the values of some selected parameters
//  
// /option (input): change this option 
// /value (input) : 
//
// /THEcomputeGeometryForMovingGrids : If true, the moving grid update function will
//    invalidate and recompute the geometry arrays for grids that move. If false, the
//    geometry arrays will not be invalidated (it will be assumed that this has already been done). 
//\end{ogenInclude.tex}
// =========================================================================================
{
  switch (option)
  {
  case THEimproveQualityOfInterpolation:
    improveQualityOfInterpolation=value;
    if( improveQualityOfInterpolation )
      printF("Improve quality of interpolation is now ON, qualityBound=%5.2e\n",qualityBound);
    else
      printF("Improve quality of interpolation is now OFF\n");
    break;
//    case THEqualityBound:
//      qualityBound=value;
//      break;
  case THEoutputGridOnFailure: 
    outputGridOnFailure=value;
    if( Ogen::debug & 4 ) printF("Ogen:set: outputGridOnFailure=%i\n",outputGridOnFailure);
    break;
  case THEabortOnAlgorithmFailure:
    abortOnAlgorithmFailure=value;
    if( Ogen::debug & 4 ) printF("Ogen:set: abortOnAlgorithmFailure=%i\n",abortOnAlgorithmFailure);
    break;
  case THEcomputeGeometryForMovingGrids:
    computeGeometryForMovingGrids=value;
    if( Ogen::debug & 4 ) printF("Ogen:set: computeGeometryForMovingGrids=%i\n",computeGeometryForMovingGrids);
    break;
  default:
    printf("Ogen:set:ERROR unexpected parameter to set\n");
    break;
  };
}

//\begin{>>ogenInclude.tex}{\subsubsection{set(OgenParameter::option, int)}}
void Ogen::
set(const OgenParameterEnum option, const int value)
// =========================================================================================
// /Description:
// set the values of some selected parameters
//  
// /option (input): change this option 
// /value (input) : 
//\end{ogenInclude.tex}
// =========================================================================================
{
  switch (option)
  {
  case THEimproveQualityOfInterpolation:
    improveQualityOfInterpolation=value;
    if( improveQualityOfInterpolation )
      printf("Improve quality of interpolation is now ON, qualityBound=%5.2e\n",qualityBound);
    else
      printf("Improve quality of interpolation is now OFF\n");
    break;
  case THEqualityBound:
    qualityBound=value;
    if( qualityBound<=1. )
    {
      qualityBound=2.;
      printf("Ogen:set:ERROR: invalide value for THEqualityBound=%e, setting to the default value=%e\n",
	     value,qualityBound);
    }
    break;
  case THEcomputeGeometryForMovingGrids:
    computeGeometryForMovingGrids=value;
    if( Ogen::debug & 4 ) printF("Ogen:set: computeGeometryForMovingGrids=%i\n",computeGeometryForMovingGrids);
    break;
  default:
    printf("Ogen:set:ERROR unexpected parameter to set\n");
    break;
  };
}

//\begin{>>ogenInclude.tex}{\subsubsection{set(OgenParameter::option, int)}}
void Ogen::
set(const OgenParameterEnum option, const real value)
// =========================================================================================
// /Description:
// set the values of some selected parameters
//  
// /option (input): change this option 
// /value (input) : 
//\end{ogenInclude.tex}
// =========================================================================================
{
  switch (option)
  {
  case THEimproveQualityOfInterpolation:
    improveQualityOfInterpolation=value;
    if( improveQualityOfInterpolation )
      printf("Improve quality of interpolation is now ON, qualityBound=%5.2e\n",qualityBound);
    else
      printf("Improve quality of interpolation is now OFF\n");
    break;
  case THEqualityBound:
    qualityBound=value;
    if( qualityBound<=1. )
    {
      qualityBound=2.;
      printf("Ogen:set:ERROR: invalide value for THEqualityBound=%e, setting to the default value=%e\n",
	     value,qualityBound);
    }
    break;
  case THEmaximumAngleDifferenceForNormalsOnSharedBoundaries:
    maximumAngleDifferenceForNormalsOnSharedBoundaries=value;
    printF("Ogen:set:maximumAngleDifferenceForNormalsOnSharedBoundaries = %9.3e\n",
             maximumAngleDifferenceForNormalsOnSharedBoundaries);
    break;
  default:
    printf("Ogen:set:ERROR unexpected parameter to set\n");
    break;
  };

}
