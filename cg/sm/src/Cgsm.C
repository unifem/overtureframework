#include "Cgsm.h"
#include "SmParameters.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "InterpolatePoints.h"
#include "Ogshow.h"
#include "ShowFileReader.h"
#include "Oges.h"
#include "ParallelUtility.h"
#include "GridFunctionFilter.h"

// ===================================================================================================================
/// \brief Constructor for the Cgsm class.
///
/// \param cg_ (input) : use this CompositeGrid.
/// \param ps (input) : pointer to a graphics object to use.
/// \param show (input) : pointer to a show file to use.
/// \param plotOption_ (input) : plot option 
/// 
///  \note SmParameters (passed to the DomainSolver constructor above) replaces the base class Parameters
// ==================================================================================================================
// Cgsm::
// SolidMechanics(CompositeGrid & cg_, 
//       GenericGraphicsInterface *ps /* =NULL */, 
//       Ogshow *show /* =NULL */ , 
//       const int & plotOption_ /* =1 */) 
//     : DomainSolver(*(new Parameters),cg_,ps,show,plotOption_)
// {


Cgsm::
Cgsm(CompositeGrid & cg_, 
     GenericGraphicsInterface *ps_ /* =NULL */, 
     Ogshow *show /* =NULL */ , 
     const int & plotOption_ /* =1 */ ) : DomainSolver(*(new SmParameters),cg_,ps_,show,plotOption_)
{
   className="Cgsm";
   name="sm";


  myid=max(0,Communication_Manager::My_Process_Number);

//   debug=0;

  aString buff;
  const int np= max(1,Communication_Manager::numberOfProcessors());
  
  initialConditionOption=defaultInitialCondition;
  forcingOption=noForcing;
  knownSolutionOption=noKnownSolution;
  
  // frequency=5.;
  checkErrors=false;
  radiusForCheckingErrors=-1;  // if >0 only check errors in a disk (sphere) of this radius  
  computeEnergy=false;
  totalEnergy=0.;
  initialTotalEnergy=-1.;  // energy at time 0 (<0 : means has not been assigned)
  dScale=.1; // displacement scale factor (for plotting the displacement)
  
  int & maximumNumberOfIterationsForImplicitInterpolation =
          parameters.dbase.get<int>("maximumNumberOfIterationsForImplicitInterpolation");
  maximumNumberOfIterationsForImplicitInterpolation=-1; // -1 : use default

  numberOfIterationsForInterfaceBC=3;
    
  numberOfStepsTaken=0;

  plotDivergence=false;
  plotVorticity=false;
  plotErrors=false;
  plotDissipation=false;

  plotVelocity=false;
  plotStress=false;

  plotScatteredField=false;   // set to true to subtract off the plane wave
  plotTotalField=false;       // set to true to add on the plane wave
  plotRho=false;         
  
  compareToReferenceShowFile=false;

  real & rho=parameters.dbase.get<real>("rho");
  real & mu = parameters.dbase.get<real>("mu");
  real & lambda = parameters.dbase.get<real>("lambda");
  RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
  bool & gridHasMaterialInterfaces = parameters.dbase.get<bool>("gridHasMaterialInterfaces");

  gridHasMaterialInterfaces=false;

  lambda=1.;
  mu=1.;
  rho=1.;
  c1=(mu+lambda)/rho, c2= mu/rho;
  kx=ky=kz=1;

  omegaForInterfaceIteration=1.;
  materialInterfaceOption=1;  // 1=extrapolate as initial guess for material interface ghost values
  
  artificialDissipation=0.;
  artificialDissipationInterval=1;
  orderOfArtificialDissipation=4;
  
  divergenceDamping=0.;

  betaGaussianPlaneWave=50.;
  x0GaussianPlaneWave=.5;
  y0GaussianPlaneWave=0.;
  z0GaussianPlaneWave=0.;

  gaussianSourceParameters[0]=100.; // gamma,omega,x0,y0,z0
  gaussianSourceParameters[1]=5.;
  gaussianSourceParameters[2]=.0;
  gaussianSourceParameters[3]=.0;
  gaussianSourceParameters[4]=.0;
  
  numberOfGaussianPulses=0;
  gaussianPulseParameters[0][0]=10.;   // beta scale, exponent, x0,y0,z0
  gaussianPulseParameters[0][1]=2.;   
  gaussianPulseParameters[0][2]=10.;   
  gaussianPulseParameters[0][3]=0.;   
  gaussianPulseParameters[0][4]=0.;   
  gaussianPulseParameters[0][5]=0.;   
  
  //   rho = a*exp( - [beta* | xpv - vpv*t |]^p ) )
  numberOfGaussianChargeSources=0;
  gaussianChargeSourceParameters[0][0]=1.;  // amplitude
  gaussianChargeSourceParameters[0][1]=10.; // beta     
  gaussianChargeSourceParameters[0][2]=2.;  // p        
  gaussianChargeSourceParameters[0][3]=0.;  // xp0      
  gaussianChargeSourceParameters[0][4]=0.;  // xp1      
  gaussianChargeSourceParameters[0][5]=0.;  // xp2      
  gaussianChargeSourceParameters[0][6]=1.;  // vp0      
  gaussianChargeSourceParameters[0][7]=0.;  // vp1      
  gaussianChargeSourceParameters[0][8]=0.;  // vp2      


//   nx[0]=21;   nx[1]=21;   nx[2]=21;
//   xab[0][0]=0.;  xab[1][0]=1.;
//   xab[0][1]=0.;  xab[1][1]=1.;
//   xab[0][2]=0.;  xab[1][2]=1.;
  
  real & cfl = parameters.dbase.get<real>("cfl");
  real & tFinal = parameters.dbase.get<real>("tFinal");
  real & tPlot = parameters.dbase.get<real>("tPrint");
  
  cfl=.9;
  tFinal=1.;
  tPlot=-1.;

  
  elementType=structuredElements;
  
//   chevronFrequency = 1;
//   chevronAmplitude = .1;

  cylinderRadius=1.;
  cylinderAxisStart=0.; cylinderAxisEnd=1.; // for eigenfunctions of the cylinder

  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
  orderOfAccuracyInSpace=2;
  orderOfAccuracyInTime=2;
  useConservative=false;

  useVariableDissipation=false;
  variableDissipation=NULL;

//  pRho=NULL;
//  poisson=NULL;
//  pPhi=NULL;
//  pF=NULL;

//   degreeSpace=2;  // for TZ polynomial
//   degreeSpaceX=degreeSpaceY=degreeSpaceZ=degreeSpace;
  
//   degreeTime=2;
//   omega[0]=omega[1]=omega[2]=omega[3]=2.;  // for trig TZ
  
//  timeSteppingMethod=defaultTimeStepping;

  divUMax=0.;
  vorUMax=0.;
  gradUMax=0.; 
  
  slowStartInterval=-1.;  // negative means don't use

//   fields=NULL;
//   dissipation=NULL;
//   errp=NULL;
  
//   op=NULL;
//   mgp=NULL;

  numberLinesForPML=15;
  pmlPower=4;
  pmlLayerStrength=30.;
  pmlErrorOffset=0;  // only check errors within this many lines of the pml

  frequencyToSaveProbes=1;
  
  vpml=NULL;
  
  normalPlaneMaterialInterface[0]=1.;
  normalPlaneMaterialInterface[1]=0.;
  normalPlaneMaterialInterface[2]=0.;

  x0PlaneMaterialInterface[0]=0.;
  x0PlaneMaterialInterface[1]=0.;
  x0PlaneMaterialInterface[2]=0.;

  cgop=NULL;
  cgdissipation=NULL;
  cgerrp=NULL;
  knownSolution=NULL;
  
  show=NULL;
  referenceShowFileReader=NULL;  // for comparing to a reference solution

  sequenceCount=0; 
  numberOfSequences=0;
  
  // for radiation BC's
  radbcGrid[0]=radbcGrid[1]=-1; radbcSide[0]=radbcSide[1]=-1; radbcAxis[0]=radbcAxis[1]=-1;
//  radiationBoundaryCondition=NULL;

  useStreamMode=true;
  saveDivergenceInShowFile=false;
  saveErrorsInShowFile=false;
  saveVelocityInShowFile=false;
  saveStressInShowFile=false; 
  
  frequencyToSaveInShowFile=1;
  showFileFrameForGrid=-1;
  
  // These next variables are for higher order time stepping
  numberOfFunctions=0;
  currentFn=0;
  cgfn=NULL;
  
  runTimeDialog=NULL;
  movieFrame=-1;
  plotOptions=0;
  plotChoices=2; // plot contours by default
  totalNumberOfArrays=0;
  
//  useTwilightZone=false;
//  twilightZoneOption=polynomialTwilightZone;
//  tz=NULL;

//   timing.redim(maximumNumberOfTimings);
//   timing=0.;
//   for( int i=0; i<maximumNumberOfTimings; i++ )
//     timingName[i]="";

//   // only name the things that will be really timed in this run
//   timingName[totalTime]                          ="total time";
//   timingName[timeForInitialize]                  ="setup and initialize";
//   timingName[timeForInitialConditions]           ="initial conditions";
//   timingName[timeForAdvance]                     ="advance";
//   timingName[timeForAdvanceRectangularGrids]     ="  advance rectangular grids";
//   timingName[timeForAdvanceCurvilinearGrids]     ="  advance curvilinear grids";
//   timingName[timeForAdvanceUnstructuredGrids]    ="  advance unstructured grids";
//   timingName[timeForAdvOpt]                      ="   (advOpt)";
//   timingName[timeForForcing]                     ="  add forcing";
//   timingName[timeForProject]                     ="  project    ";
//   timingName[timeForDissipation]                 ="  add dissipation";
//   timingName[timeForBoundaryConditions]          ="  boundary conditions";
//   timingName[timeForInterfaceBC]                 ="  interface bc";
//   timingName[timeForRadiationBC]                 ="  radiation bc";
//   timingName[timeForRaditionKernel]              ="  radiationKernel";
//   timingName[timeForUpdateGhostBoundaries]       ="  update ghost (parallel)";
//   timingName[timeForInterpolate]                 ="  interpolation";
//   timingName[timeForComputingDeltaT]             ="compute dt";
//   timingName[timeForGetError]                    ="get errors";
//   timingName[timeForPlotting]                    ="plotting";
//   timingName[timeForShowFile]                    ="showFile";
//   timingName[timeForWaiting]                     ="waiting (not counted)";

  probeFileName="probeFile.dat";
  probeFile = NULL;

  sizeOfLocalArraysForAdvance=0.;


  // grid setup 
  setupGrids();

}

Cgsm::
~Cgsm()
{
  //  saveSequencesToShowFile(); // *wdh* 091124 

  delete &parameters;

  delete cgdissipation;
  delete cgerrp;
  delete [] cgfn;

  delete variableDissipation;

  delete runTimeDialog;

  delete vpml;

  delete cgop;
  delete knownSolution;
  
//  delete tz;

  delete referenceShowFileReader;
  
//  delete [] radiationBoundaryCondition;
  
//   delete pRho;
//   delete poisson;
//   delete pPhi;
//   delete pF;

  if( probeFile!=NULL )
    fclose(probeFile);
}

// ===================================================================================
// /Description:
//    Return true if any grid has PML boundary conditions
// ===================================================================================
bool Cgsm::
usingPMLBoundaryConditions() const
{
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & bc = cg[grid].boundaryCondition();
    if( bc(0,0)==SmParameters::abcPML || bc(1,0)==SmParameters::abcPML ||
	bc(0,1)==SmParameters::abcPML || bc(1,1)==SmParameters::abcPML ||
	bc(0,2)==SmParameters::abcPML || bc(1,2)==SmParameters::abcPML )
    {
      return true;
    }
  }
  return false;
}

// ===================================================================================
// /Description:
//    Initialize the radition boundary conditions
// ===================================================================================
int Cgsm::
initializeRadiationBoundaryConditions()
{
  // first count the number of faces where we apply a non-local BC -- at most 2 for now
  int numberOfNonLocal=0;
  
  int grid,side,axis;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    if( mg.getGridType()==MappedGrid::structuredGrid )
    {
      const IntegerArray & bc = cg[grid].boundaryCondition();
      for( axis=0; axis<mg.numberOfDimensions(); axis++ )
	for( side=0; side<=1; side++ )
	{
	  if( bc(side,axis)==SmParameters::rbcNonLocal )
	  {
            radbcGrid[numberOfNonLocal]=grid;
	    radbcSide[numberOfNonLocal]=side;
	    radbcAxis[numberOfNonLocal]=axis;
	    
	    numberOfNonLocal++;
            if( numberOfNonLocal>2 )
	    {
	      printF("Cgsm::initializeRadiationBoundaryConditions:ERROR: there are too many sides with\n"
                     "  radiation boundary conditions -- there should be at most 2\n");
	      Overture::abort("error");
	    }
	  }
	}
    }
  }
//   if( numberOfNonLocal>0 )
//   {
//     assert( radiationBoundaryCondition==NULL );
//     radiationBoundaryCondition = new RadiationBoundaryCondition [numberOfNonLocal];
    
//     for( int i=0; i<numberOfNonLocal; i++ )
//     {
//       radiationBoundaryCondition[i].setOrderOfAccuracy(orderOfAccuracyInSpace);
      
//       int nc1=0, nc2=0;      // component range
//       if( cg.numberOfDimensions()==2 )
//       {
// 	nc1=ex; nc2=hz;
//       }
//       else
//       {
// 	nc1=ex; nc2=ez;
//       }
//       MappedGrid & mg = cg[radbcGrid[i]];
//       radiationBoundaryCondition[i].initialize(mg,radbcSide[i],radbcAxis[i],nc1,nc2,c);
//     }
    
//   }
  
  return 0;
}


void Cgsm::
checkArrays(const aString & label) 
//==============================================================================
// /Description:
// Output a warning messages if the number of arrays has increased
//\end{CompositeGridSolverInclude.tex}  
//==============================================================================
{
   if(GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
   {
     totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
     printF("**** %s: Number of A++ arrays has increased to %i \n",(const char*)label,GET_NUMBER_OF_ARRAYS);
   }
}

int Cgsm::
printMemoryUsage(FILE *file /* = stdout */)
{
  int & debug = parameters.dbase.get<int >("debug");
  int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  
  real totalNumberOfGridPoints;
  real maxMax,maxMin,minMin;
  real dsMin[3], dsAve[3], dsMax[3];

  // *wdh* 110416 -- compute totalNumberOfGridPoints (in DomainSolver we do this in outputHeader).
  getGridInfo(totalNumberOfGridPoints,dsMin,dsAve,dsMax,maxMax,maxMin,minMin );


  int nSpace=30;
  aString dots="...................................................................";

  fPrintF(file,"\n\n"
         " ------------------------------------------------------------------------------ \n"
         "   Memory usage                   Mbytes    real/point  real/component  percent   \n");
  enum
  {
    memoryForCompositeGrid,
    memoryForOperators,
    memoryForInterpolant,
    memoryForGridFunctions,
    memoryForLocalArrays,     // includes dissipation arrays created locally in advanceStructured
    memoryTotal,
    numberOfMemoryItems
  };

  aString memoryName[numberOfMemoryItems]=
  {
    "CompositeGrid",
    "finite difference operators",
    "Interpolant",
    "Grid functions",
    "local arrays",
    "total (of above items)"
  };

  real memory[numberOfMemoryItems]={0.,0.,0.,0.,0.,0.};
  memory[memoryForCompositeGrid]=cg.sizeOf();
  
  memory[memoryForOperators]= cgop!=NULL ? cgop->sizeOf() : 0.;

  if( cg.rcData->interpolant!=NULL )
  {
    memory[memoryForInterpolant]=cg.rcData->interpolant->sizeOf();
  }

  int debugs=1;

  memory[memoryForGridFunctions]=0.;
  int i;
  real cgfieldsSize=0., fnSize=0., cgdissSize=0., errSize=0.;
  
  for( i=0; i<numberOfTimeLevels; i++ )
    memory[memoryForGridFunctions]+=gf[i].sizeOf();
  cgfieldsSize=memory[memoryForGridFunctions];
  
  if( cgdissipation!=NULL )
  {
    cgdissSize=-memory[memoryForGridFunctions];
    memory[memoryForGridFunctions]+=cgdissipation->sizeOf();  
    cgdissSize+=memory[memoryForGridFunctions];
  }

  if( cgerrp!=NULL )
  {
    errSize=-memory[memoryForGridFunctions];
    memory[memoryForGridFunctions]+=cgerrp->sizeOf();  
    errSize+=memory[memoryForGridFunctions];
  }
  
//   if( pRho!=NULL )
//     memory[memoryForGridFunctions]+=pRho->sizeOf();
//   if( pPhi!=NULL )
//     memory[memoryForGridFunctions]+=pPhi->sizeOf();
//   if( pF!=NULL )
//     memory[memoryForGridFunctions]+=pF->sizeOf();
  
  // memory for local arrays -- arrays that are allocated and de-allocated locally
  memory[memoryForLocalArrays]=0.;
  memory[memoryForLocalArrays]+=sizeOfLocalArraysForAdvance;

  const real megaByte=1024.*1024;
  if( debugs ) fPrintF(file," size of: cgfields=%9.2f, fn=%9.2f, cgdiss=%9.2f, err=%9.2f, (MB)\n",
		      cgfieldsSize/megaByte,fnSize/megaByte,cgdissSize/megaByte,
                      errSize/megaByte);

  memory[memoryTotal]=0.;
  for( i=0; i<numberOfMemoryItems-1; i++ )
    memory[memoryTotal]+=memory[i];

  if( Communication_Manager::My_Process_Number<=0 )
  {
    for( i=0; i<numberOfMemoryItems; i++ )
    {
      fPrintF(file," %s%s%9.2f  %9.2f    %9.2f       %5.1f%%  \n",
	      (const char*)memoryName[i],(const char*)dots(0,max(0,nSpace-memoryName[i].length())),
	      memory[i]/megaByte,memory[i]/sizeof(real)/totalNumberOfGridPoints,
	      memory[i]/sizeof(real)/totalNumberOfGridPoints/numberOfComponents, 100.*memory[i]/memory[memoryTotal]);
    }
//    fPrintF(file,"\n **Bytes per grid point = %9.3e/%i = %9.3e\n",
//                     memory[memoryTotal],numberOfGridPoints,memory[memoryTotal]/numberOfGridPoints);
//    fPrintF(file," **number of reals per grid point = %9.3e\n\n",memory[memoryTotal]/sizeof(real)/numberOfGridPoints);

    fPrintF(file," ------------------------------------------------------------------------------ \n\n");
  
    if( true || debug & 2 )
    {
      fPrintF(file,"\n +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	cg[grid].displayComputedGeometry(file);
      fPrintF(file," +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    }
    
  }
 
//    double residentSetSize;
//    getResidentSetSize(residentSetSize);
//    fPrintF(file,"resident set size = %9.2e \n",residentSetSize);

//    fPrintF(file,"************************************************************************* \n"
//           " Here is what memory is used by A++ arrays \n"
//           " total array memory in use = %10.3fM, (%10.3fM including overhead)\n"
//           " (This info is obtained by running overBlown with the `memory' option)\n"
//           "*************************************************************************\n\n",
//           Diagnostic_Manager::getTotalArrayMemoryInUse()/1.e6,Diagnostic_Manager::getTotalMemoryInUse()/1.e6);


  return 0;
}

// int Cgsm::
// printStatistics(FILE *file /* = stdout */)
// //===================================================================================
// // /Description:
// // Output timing statistics
// //
// //\end{OverBlownInclude.tex}  
// //===================================================================================
// {
//   FILE *& logFile = parameters.dbase.get<FILE* >("logFile");

//   // count the total number of grid points
//   numberOfGridPoints=0;
//   int numberOfInterpolationPoints=0;
//   int grid;
//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//   {
//     numberOfGridPoints+=cg[grid].mask().elementCount();
//   }

//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     numberOfInterpolationPoints+=cg.numberOfInterpolationPoints(grid);

//   // output timings from Interpolant.
//   if( cg.numberOfComponentGrids()>1  )
//     cg.getInterpolant()->printMyStatistics(logFile);

//   printMemoryUsage(logFile);
//   // printMemoryUsage(file);

//   GenericMappedGridOperators::printBoundaryConditionStatistics(logFile);
  
//   RealArray & timing = parameters.dbase.get<RealArray >("timing");
//   const std::vector<aString> & timingName = parameters.dbase.get<std::vector<aString> >("timingName");
//   const int & maximumNumberOfTimings = Parameters::maximumNumberOfTimings;

// //   if( poisson!=NULL )
// //     poisson->printStatistics(logFile);

// //  timing(SmParameters::timeForRadiationBC)=RadiationBoundaryCondition::cpuTime; 
// //  timing(SmParameters::timeForRaditionKernel)=RadiationKernel::cpuTime;

//   printF(" printStat: totalTime=%8.2e, timeForWaiting=%8.2e \n",timing(SmParameters::totalTime),timing(SmParameters::timeForWaiting));

//   int i;
//   for( i=0; i<maximumNumberOfTimings; i++ )
//     timing(i) =ParallelUtility::getMaxValue(timing(i),0);  // get max over processors -- results only for processor=0

//   // adjust times for waiting
//   real timeWaiting=timing(SmParameters::timeForWaiting);
//   timing(SmParameters::timeForPlotting)-=timeWaiting;
//   // ** timing(SmParameters::totalTime)-=timeWaiting;

//   printF(" printStat: totalTime=%8.2e, timeForWaiting=%8.2e \n",timing(SmParameters::totalTime),timeWaiting);

//   timing(SmParameters::totalTime)=max(timing(SmParameters::totalTime),REAL_MIN*10.);

//   if( Communication_Manager::My_Process_Number<=0 )
//   {

//   for( int fileio=0; fileio<2; fileio++ )
//   {
//     FILE *output = fileio==0 ? logFile : file;

//     fPrintF(output,"\n         ---SolidMechanics Summary--- \n"
// 	    "  ==== numberOfStepsTaken =%9i, grids=%i, gridpts =%i, interp pts=%i, processors=%i ==== \n"
// 	    "   Timings:                           seconds    sec/step   sec/step/pt     %%   \n",
// 	    numberOfStepsTaken,cg.numberOfComponentGrids(),numberOfGridPoints,numberOfInterpolationPoints,
//             Communication_Manager::numberOfProcessors());
    
  
//     int nSpace=35;
//     aString dots="........................................................................";
//     if( timing(0)==0. )
//       timing(0)=REAL_MIN;
//     for( i=0; i<maximumNumberOfTimings; i++ )
//     {
//       if( timingName[i]!="" && timing(i)>0. )    
// 	fPrintF(output,"%s%s%10.2e  %10.2e  %10.2e   %7.3f\n",(const char*)timingName[i],
// 		(const char*)dots(0,max(0,nSpace-timingName[i].length())),
// 		timing(i),timing(i)/numberOfStepsTaken,timing(i)/numberOfStepsTaken/numberOfGridPoints,
// 		100.*timing(i)/timing(SmParameters::totalTime));
      
//     }

//     if( fileio==0 )
//     {
//       // Output results as a LaTeX table
//       fPrintF(output,"\n\n%% ------------ Table for LaTeX -------------------------------\n");
//       fPrintF(output,
// 	      "\\begin{table}[hbt]\n"
// 	      "\\begin{center}\\footnotesize\n"
// 	      "\\begin{tabular}{|l|r|r|r|r|} \\hline\n"
// 	      "  Timings:   &  seconds &    sec/step  &  sec/step/pt &  \\%%    \\\\ \\hline\n");
//       for( i=0; i<maximumNumberOfTimings; i++ )
//       {
//         aString name=timingName[i];
// 	int len=name.length();
// 	for( int j=0; j<len; j++)
// 	{
// 	  if( name[j]==' ' ) name[j]='~';   // replace indentation space with ~
// 	}
// 	if( timingName[i]!="" && timing(i)>0. )    
// 	  fPrintF(output,"%s\\dotfill & %10.2e & %10.2e & %10.2e & %7.3f \\\\ \n",(const char*)name,
// 		  timing(i),timing(i)/numberOfStepsTaken,timing(i)/numberOfStepsTaken/numberOfGridPoints,
// 		  100.*timing(i)/timing(0));
      
//       }     
//       fPrintF(output,
//               " \\hline \n"
//               "\\end{tabular}\n"
//               "\\end{center}\n"
//               "\\caption{grid=%s, %i grid points, %i interp points, %i steps taken, %i processors.}\n"
//               "\\label{tab:%s}\n"
//               "\\end{table}\n", (const char*)nameOfGridFile,numberOfGridPoints,numberOfInterpolationPoints,
// 	      numberOfStepsTaken,Communication_Manager::numberOfProcessors(),(const char*)nameOfGridFile );
//     }
    

//   }
//   printF("\n >>>> See the file sm.log for further timings, memory usage and other statistics <<<< \n\n");
  
//   }  // end if myProcessor==0
  
//   // reset times
//   timing(SmParameters::timeForPlotting)+=timeWaiting;
//   timing(SmParameters::totalTime)+=timeWaiting;

//   return 0;
// }


//================================================================================
/// \brief: Build the time stepping options dialog.
///
//================================================================================
int Cgsm::
buildTimeSteppingDialog(DialogData & dialog )
{
  real & cfl = parameters.dbase.get<real>("cfl");
  real & tFinal = parameters.dbase.get<real>("tFinal");
  real & tPlot = parameters.dbase.get<real>("tPrint");
  int & maximumNumberOfIterationsForImplicitInterpolation =
          parameters.dbase.get<int>("maximumNumberOfIterationsForImplicitInterpolation");
  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                   parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");

  dialog.setOptionMenuColumns(1);

  aString timeSteppingMethodCommands[] = {"defaultTimeStepping", 
					  "adamsBashforthSymmetricThirdOrder",
					  "rungeKuttaFourthOrder",
					  "stoermerTimeStepping",
					  "modifiedEquationTimeStepping",
                                          "forwardEuler",
                                          "improvedEuler",
                                          "adamsBashforth2",
                                          "adamsPredictorCorrector2",
                                          "adamsPredictorCorrector4",
					  "" };

  dialog.addOptionMenu("time stepping:", timeSteppingMethodCommands, timeSteppingMethodCommands, 
                       (int)timeSteppingMethodSm );

  aString pushButtonCommands[] = {"projection solver parameters...",
				  ""};
  int numRows=1;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"use conservative difference",
                          "use variable dissipation",
                          "apply filter",
 			  ""};
  int tbState[10];
  tbState[0] = useConservative;
  tbState[1] = useVariableDissipation;
  tbState[2] = (int)parameters.dbase.get<bool >("applyFilter");

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // ----- Text strings ------
  const int numberOfTextStrings=33;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "final time";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",tFinal);  nt++; 

  textCommands[nt] = "cfl";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",cfl);  nt++; 

  textCommands[nt] = "dissipation";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",artificialDissipation);  nt++; 

  textCommands[nt] = "order of dissipation";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfArtificialDissipation); nt++; 

  textCommands[nt] = "dissipation interval";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",artificialDissipationInterval); nt++; 

  textCommands[nt] = "accuracy in space";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfAccuracyInSpace); nt++; 

  textCommands[nt] = "accuracy in time";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfAccuracyInTime); nt++; 

  textCommands[nt] = "max iterations for interpolation";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",
                   maximumNumberOfIterationsForImplicitInterpolation); nt++; 

  textCommands[nt] = "interface BC iterations";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfIterationsForInterfaceBC); nt++; 

  textCommands[nt] = "omega for interface iterations";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%5.3f",omegaForInterfaceIteration); nt++; 

  textCommands[nt] = "interface option";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",materialInterfaceOption); nt++; 
  
  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


int Cgsm::
getTimeSteppingOption(const aString & answer,
		      DialogData & dialog )
{
  assert( parameters.dbase.get<GenericGraphicsInterface* >("ps") !=NULL );
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  real & tFinal = parameters.dbase.get<real>("tFinal");
  real & cfl = parameters.dbase.get<real>("cfl");
  int & maximumNumberOfIterationsForImplicitInterpolation =
          parameters.dbase.get<int>("maximumNumberOfIterationsForImplicitInterpolation");
  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
  bool & applyFilter = parameters.dbase.get<bool >("applyFilter");

  int found=true; 
  char buff[180];
  aString answer2;
  int len=0;

  if( dialog.getTextValue(answer,"final time","%g",tFinal) ){}//
  else if( dialog.getTextValue(answer,"tFinal","%g",tFinal) ){}//  for backward compatibility
  else if( dialog.getTextValue(answer,"cfl","%g",cfl) ){}//
  else if( dialog.getTextValue(answer,"dissipation","%g",artificialDissipation) ){}//
  else if( dialog.getTextValue(answer,"order of dissipation","%i",orderOfArtificialDissipation) )
  {
    if( orderOfArtificialDissipation > orderOfAccuracyInSpace ) // *wdh* 090823
    {
      printF("INFO: I will extrapolate interpolation neighbours for the high order dissipation.\n");
      parameters.dbase.get<int >("extrapolateInterpolationNeighbours")=true;
    }
  }//
  else if( dialog.getTextValue(answer,"dissipation interval","%i",artificialDissipationInterval) ){}//
  else if( dialog.getTextValue(answer,"accuracy in space","%i",orderOfAccuracyInSpace) ){}//
  else if( dialog.getTextValue(answer,"accuracy in time","%i",orderOfAccuracyInTime) ){}//
  else if( dialog.getTextValue(answer,"max iterations for interpolation","%i",
						  maximumNumberOfIterationsForImplicitInterpolation) )
  {
    cg.getInterpolant()->setMaximumNumberOfIterations(maximumNumberOfIterationsForImplicitInterpolation);
  }
  else if( dialog.getTextValue(answer,"interface BC iterations","%i",
						  numberOfIterationsForInterfaceBC) ){}// 
  else if( dialog.getTextValue(answer,"omega for interface iterations","%e",
						  omegaForInterfaceIteration) ){}// 
  else if( dialog.getTextValue(answer,"interface option","%i",
						  materialInterfaceOption) ){}// 
  else if( dialog.getToggleValue(answer,"use conservative difference",useConservative) ){}//
  else if( dialog.getToggleValue(answer,"use variable dissipation",useVariableDissipation) ){}//
  else if( dialog.getToggleValue(answer,"apply filter",applyFilter) )
  {
    if( applyFilter )
    {
      GridFunctionFilter *& gridFunctionFilter =parameters.dbase.get<GridFunctionFilter*>("gridFunctionFilter");
      if( gridFunctionFilter==NULL )
      {
        gridFunctionFilter = new GridFunctionFilter();
      }
      GridFunctionFilter & filter = *gridFunctionFilter;
      filter.update( gi ); // make changes to any filter parameters

      const int orderOfFilter = filter.orderOfFilter;
      const int filterFrequency = filter.filterFrequency;
      const int numberOfFilterIterations = filter.numberOfFilterIterations;

      const real filterCoefficient = filter.filterCoefficient;
      if( filter.filterType==GridFunctionFilter::explicitFilter &&
          orderOfFilter> orderOfAccuracyInSpace )
      {
	printF("INFO: I will extrapolate interpolation neighbours for the explicit filter.\n");
	parameters.dbase.get<int >("extrapolateInterpolationNeighbours")=true;
      }
    }
  }//
  else if( dialog.getTextValue(answer,"divergence damping","%g",divergenceDamping) ){}//
  else if( answer=="defaultTimeStepping" ||
	   answer=="adamsBashforthSymmetricThirdOrder" ||
	   answer=="rungeKuttaFourthOrder" ||
	   answer=="stoermerTimeStepping" ||
	   answer=="modifiedEquationTimeStepping" ||
	   answer=="forwardEuler"  ||
	   answer=="improvedEuler"  ||
	   answer=="adamsBashforth2"  ||
	   answer=="adamsPredictorCorrector2"  ||
	   answer=="adamsPredictorCorrector4" )
  {
    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                   parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");
    timeSteppingMethodSm = answer=="defaultTimeStepping"  ? SmParameters::defaultTimeStepping :
      answer=="adamsBashforthSymmetricThirdOrder" ? SmParameters::adamsBashforthSymmetricThirdOrder : 
      answer=="rungeKuttaFourthOrder" ? SmParameters::rungeKuttaFourthOrder :
      answer=="stoermerTimeStepping" ? SmParameters::stoermerTimeStepping : 
      answer=="modifiedEquationTimeStepping" ? SmParameters::modifiedEquationTimeStepping : 
      answer=="forwardEuler" ? SmParameters::forwardEuler :
      answer=="improvedEuler" ? SmParameters::improvedEuler :
      answer=="adamsBashforth2" ? SmParameters::adamsBashforth2 :
      answer=="adamsPredictorCorrector2" ? SmParameters::adamsPredictorCorrector2 :
      answer=="adamsPredictorCorrector4" ? SmParameters::adamsPredictorCorrector4 :
                                               SmParameters::defaultTimeStepping;

    Parameters::TimeSteppingMethod & timeSteppingMethod = 
      parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");
    if( timeSteppingMethodSm==SmParameters::adamsBashforth2 )
      timeSteppingMethod = Parameters::adamsBashforth2;
    if( timeSteppingMethodSm==SmParameters::adamsPredictorCorrector2 )
      timeSteppingMethod = Parameters::adamsPredictorCorrector2;
    if( timeSteppingMethodSm==SmParameters::adamsPredictorCorrector4 )
      timeSteppingMethod = Parameters::adamsPredictorCorrector4;

    dialog.getOptionMenu("time stepping:").setCurrentChoice((int)timeSteppingMethodSm);
  }
  else
  {
    found=false;
  }
  
  
  return found;
}

int Cgsm::
buildForcingOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the forcing options dialog.
// ==========================================================================================
{

  dialog.setWindowTitle("Cgsm forcing options");

  // ************** PUSH BUTTONS *****************
  aString pushButtonCommands[] = {"set pml error checking offset",
                                  "user defined material properties...",
				  ""};
  int numRows=2;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  dialog.setOptionMenuColumns(1);

  aString forcingOptionCommands[] = {"noForcing", 
                                     "gaussianSource",
                                     "twilightZone",
                                     "planeWaveBoundaryForcing",
                                     "gaussianChargeSource",
                                     "userDefinedForcing",
                                     "" };

  dialog.addOptionMenu("forcing:", forcingOptionCommands, forcingOptionCommands, (int)forcingOption );

  aString twilightZoneOptionCommands[] = {"polynomial", 
					  "trigonometric",
					  "pulse",
					  "" };

  Parameters::TwilightZoneChoice & twilightZoneChoice = 
    parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
  dialog.addOptionMenu("TZ option:", twilightZoneOptionCommands, twilightZoneOptionCommands, 
                       (int)twilightZoneChoice );


  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "bc:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "gridName(side,axis)=bcName",0); nt++; 

  textCommands[nt] = "pml width,strength,power";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i %4.1f %i ",numberLinesForPML,pmlLayerStrength,pmlPower); nt++; 

  textCommands[nt] = "slow start interval";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",slowStartInterval); nt++; 

  textCommands[nt] = "Gaussian source:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g %g (beta,omega,x0,y0,z0)",
	  gaussianSourceParameters[0],gaussianSourceParameters[1],
          gaussianSourceParameters[2],gaussianSourceParameters[3],gaussianSourceParameters[4]); nt++;

  textCommands[nt] = "Gaussian charge source:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g %g %g %g %g %g (amp,beta,p,x0,y0,z0,v0,v1,v2)",
	  gaussianChargeSourceParameters[0][0],gaussianChargeSourceParameters[0][1],
          gaussianChargeSourceParameters[0][2],gaussianChargeSourceParameters[0][3],
          gaussianChargeSourceParameters[0][4],gaussianChargeSourceParameters[0][5],
          gaussianChargeSourceParameters[0][6],gaussianChargeSourceParameters[0][7],
          gaussianChargeSourceParameters[0][8]); nt++;

  ArraySimpleFixed<real,4,1,1,1> & omega = parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");
  textCommands[nt] = "TZ omega:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]); nt++;

  const int & tzDegreeSpace= parameters.dbase.get<int >("tzDegreeSpace");
  const int & tzDegreeTime = parameters.dbase.get<int >("tzDegreeTime");
  textCommands[nt] = "degreeSpace, degreeTime";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i, %i",tzDegreeSpace,tzDegreeTime); nt++; 

//   textCommands[nt] = "degreeSpaceX, degreeSpaceY, degreeSpaceZ";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i, %i, %i",
//                                            degreeSpaceX, degreeSpaceY, degreeSpaceZ); nt++; 

  textCommands[nt] = "material interface normal";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%5.3f, %5.3f, %5.3f",
                                           normalPlaneMaterialInterface[0], normalPlaneMaterialInterface[1],
                                           normalPlaneMaterialInterface[2]); nt++; 
  textCommands[nt] = "material interface point";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%5.3f, %5.3f, %5.3f",
                                           x0PlaneMaterialInterface[0], x0PlaneMaterialInterface[1],
                                           x0PlaneMaterialInterface[2]); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

//================================================================================
/// \brief: Look for a forcing option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//================================================================================
int Cgsm::
getForcingOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( answer=="noForcing" ||
	   answer=="gaussianSource" ||
	   answer=="twilightZone" ||
	   answer=="planeWaveBoundaryForcing" ||
	   answer=="gaussianChargeSource" ||
           answer=="userDefinedForcing" )
  {
    forcingOption=(answer=="gaussianSource"                ? gaussianSource : 
		   answer=="twilightZone"                  ? twilightZoneForcing :
		   answer=="planeWaveBoundaryForcing"      ? planeWaveBoundaryForcing :
		   answer=="gaussianChargeSource"          ? gaussianChargeSource :
                   answer=="userDefinedForcing"            ? userDefinedForcingOption :
		   noForcing);

    if( forcingOption==gaussianSource )
    {
      plotErrors=false;
      dialog.setToggleState("plot errors",(int)plotErrors);
    }
    if( forcingOption==userDefinedForcingOption )
    {
      // choose the user defined forcing
      setupUserDefinedForcing();
    }
    
    dialog.getOptionMenu("forcing:").setCurrentChoice((int)forcingOption);
  }
  else if( answer=="polynomial" ||
	   answer=="trigonometric" ||
	   answer=="pulse" )
  {
    Parameters::TwilightZoneChoice & twilightZoneChoice = 
      parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
    twilightZoneChoice =  answer=="polynomial" ? Parameters::polynomial :
      answer=="trigonometric" ? Parameters::trigonometric : Parameters::pulse;
    dialog.getOptionMenu("TZ option:").setCurrentChoice((int)twilightZoneChoice);
  }
  else if( len=answer.matches("pml width,strength,power") )
  {
    sScanF(answer(len,answer.length()-1),"%i %e %i ",&numberLinesForPML,&pmlLayerStrength,&pmlPower);
    dialog.setTextLabel("pml width,strength,power",sPrintF(line, "%i %4.1f %i",numberLinesForPML,
									 pmlLayerStrength,pmlPower));
  }
  else if( len=answer.matches("degreeSpace, degreeTime") )
  {
    int & tzDegreeSpace= parameters.dbase.get<int >("tzDegreeSpace");
    int & tzDegreeTime = parameters.dbase.get<int >("tzDegreeTime");

    sScanF(answer(len,answer.length()-1),"%i %i",&tzDegreeSpace,&tzDegreeTime);
    dialog.setTextLabel("degreeSpace, degreeTime",sPrintF(line,"%i, %i",tzDegreeSpace,tzDegreeTime));

    // degreeSpaceX=degreeSpaceY=degreeSpaceZ=degreeSpace;
  }
//   else if( len=answer.matches("degreeSpaceX, degreeSpaceY, degreeSpaceZ") )
//   {
//     sScanF(answer(len,answer.length()-1),"%i %i %i %i",&degreeSpaceX,&degreeSpaceY,&degreeSpaceZ);
//     dialog.setTextLabel("degreeSpaceX, degreeSpaceY, degreeSpaceZ",sPrintF(line,"%i, %i %i",
// 											 degreeSpaceX,degreeSpaceY,degreeSpaceZ));
//   }
  else if( len=answer.matches("material interface normal") )
  {
    sScanF(answer(len,answer.length()-1),"%e %e %e ",&normalPlaneMaterialInterface[0], 
	   &normalPlaneMaterialInterface[1],&normalPlaneMaterialInterface[2]);
    dialog.setTextLabel("material interface normal",
				      sPrintF("%5.3f %5.3f %5.3f",
					      normalPlaneMaterialInterface[0], normalPlaneMaterialInterface[1],
					      normalPlaneMaterialInterface[2]));
  }
  else if( len=answer.matches("material interface point") )
  {
    sScanF(answer(len,answer.length()-1),"%e %e %e ",&x0PlaneMaterialInterface[0], 
	   &x0PlaneMaterialInterface[1],&x0PlaneMaterialInterface[2]);
    dialog.setTextLabel("material interface point",
				      sPrintF("%5.3f %5.3f %5.3f",
					      x0PlaneMaterialInterface[0], x0PlaneMaterialInterface[1],
					      x0PlaneMaterialInterface[2]));
  }
  else if( dialog.getTextValue(answer,"slow start interval","%e",slowStartInterval) ){}//
  else if( len=answer.matches("bc: ") )
  {
    line=answer(len,answer.length()-1);
    printF("answer=[%s] line=[%s]\n",(const char*)answer,(const char*)line);
      
    IntegerArray & originalBoundaryCondition = parameters.dbase.get<IntegerArray>("originalBoundaryCondition");
    setBoundaryCondition( line,originalBoundaryCondition );
      
    dialog.setTextLabel("bc:",sPrintF(line,"gridName(side,axis)=bcName",0));

  }
  else if( len=answer.matches("Gaussian source:") )
  {
    sScanF(answer(len,answer.length()-1),"%e %e %e %e %e",&gaussianSourceParameters[0],&gaussianSourceParameters[1],
	   &gaussianSourceParameters[2],&gaussianSourceParameters[3],&gaussianSourceParameters[4]);
      
    dialog.setTextLabel("Gaussian source:",sPrintF(line,"%g %g %g %g %g (beta,omega,x0,y0,z0)",
								 gaussianSourceParameters[0],gaussianSourceParameters[1],gaussianSourceParameters[2],
								 gaussianSourceParameters[3],gaussianSourceParameters[4]));  
  }
  else if( len=answer.matches("Gaussian charge source:") )
  {
    if( numberOfGaussianChargeSources>=maxNumberOfGaussianChargeSources )
    {
      printf(" ERROR: there are too many Gaussian charge sources. At most %i are allowed\n",
	     maxNumberOfGaussianChargeSources);
    }
    real *gcs = gaussianChargeSourceParameters[numberOfGaussianChargeSources];
    sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e %e %e %e",&gcs[0],&gcs[1],&gcs[2],&gcs[3],&gcs[4],&gcs[5],
	   &gcs[6],&gcs[7],&gcs[8]);
      
    printF(" Setting charge source %i parameters:  amplitude=%g beta=%g p=%g x0=%g x1=%g x2=%g v0=%g v1=%g v2=%g\n",
	   numberOfGaussianChargeSources,gcs[0],gcs[1],gcs[2],gcs[3],gcs[4],gcs[5],gcs[6],gcs[7],gcs[8]);
    numberOfGaussianChargeSources++;

    dialog.setTextLabel("Gaussian charge source:",
				      sPrintF(line,"%g %g %g %g %g %g %g %g  (amp,beta,p,x0,y0,z0,v0,v1,v2)",
					      gcs[0],gcs[1],gcs[2],gcs[3],gcs[4],gcs[5],gcs[6],gcs[7],gcs[8])); 
  }
  else if( len=answer.matches("TZ omega:") )
  {
    ArraySimpleFixed<real,4,1,1,1> & omega = parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega");
    sScanF(answer(len,answer.length()-1),"%e %e %e %e",&omega[0],&omega[1],&omega[2],&omega[3]);
      
    dialog.setTextLabel("TZ omega:",sPrintF(line,"%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]));
  }
  else
  {
    found=false;
  }
  

  return found;
}

// ===================================================================================================================
/// \brief Build the dialog that shows the various general options
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int Cgsm::
buildGeneralOptionsDialog(DialogData & dialog )
{
  aString pbCommands[] = {"pin corners or edges",
			  ""};
  aString *pbLabels = pbCommands;
  int numRows=2;
  dialog.setPushButtons( pbCommands, pbLabels, numRows ); 


  aString tbCommands[] = {"axisymmetric flow",
                          "iterative implicit interpolation",
                          "check for floating point errors",
                          "use interactive grid generator",
			  ""};
  int tbState[10];
  tbState[0] = parameters.dbase.get<bool >("axisymmetricProblem"); 
  tbState[1] = cg.rcData->interpolant!=NULL ? 
               cg.rcData->interpolant->getImplicitInterpolationMethod()==Interpolant::iterateToInterpolate  : 0;
  tbState[2] = Parameters::checkForFloatingPointErrors;
  tbState[3] = parameters.dbase.get<bool >("useInteractiveGridGenerator");
  
  int numColumns=1;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


  // ----- Text strings ------
  const int numberOfTextStrings=5;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "maximum iterations for implicit interpolation";
  sPrintF(textStrings[nt], "%i",parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation"));  nt++;

  int width=max(cg.interpolationWidth);
  textCommands[nt] = "reduce interpolation width";
  sPrintF(textStrings[nt], "%i",width);  nt++;

  // null strings terminal list
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  assert( nt<numberOfTextStrings );
  dialog.setTextBoxes(textCommands, textCommands, textStrings);
}

//================================================================================
/// \brief: Look for a general option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int Cgsm::
getGeneralOption(const aString & answer,
		 DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  bool iterativeImplicitInterpolation=true;
  int width=-1;

  int found=true; 
  char buff[180];
  aString answer2,line;
  int len=0;

  if( answer=="pressure solver options" )
  {
    pressureSolverParameters.update(gi,cg);
  }
  else if( answer=="implicit time step solver options" )
  {
    implicitTimeStepSolverParameters.update(gi,cg);
  }
  else if( dialog.getToggleValue(answer,"axisymmetric flow",parameters.dbase.get<bool >("axisymmetricProblem") ) ){}//
  else if( dialog.getToggleValue(answer,"iterative implicit interpolation",iterativeImplicitInterpolation) )
  {
    if( cg.rcData->interpolant!=NULL )
    {
      if( iterativeImplicitInterpolation )
	cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      else
	cg.rcData->interpolant->setImplicitInterpolationMethod(Interpolant::directSolve);
    }
  }
  else if( dialog.getToggleValue(answer,"check for floating point errors",Parameters::checkForFloatingPointErrors ) ){}//
  else if( dialog.getToggleValue(answer,"use interactive grid generator",
                                 parameters.dbase.get<bool >("useInteractiveGridGenerator") ) )
  {
    if(  parameters.dbase.get<bool >("useInteractiveGridGenerator") )
    printF("For moving grids I will use the interactive Ogen grid generator so that you\n"
           "can interactively step through the grid generation steps.\n");
  }
  else if( dialog.getTextValue(answer,"maximum iterations for implicit interpolation","%i",
                               parameters.dbase.get<int >("maximumNumberOfIterationsForImplicitInterpolation")) ){}//
  else if( answer=="reduce interpolation width" ) // for backward compat.
  {
    int width, oldWidth=max(cg.interpolationWidth);
    gi.inputString(answer2,sPrintF(buff,"Enter new interpolation width (should <= %i)",oldWidth));
    if( answer2!="" )
    {
      sScanF(answer2,"%i",&width);
      if( width<oldWidth )
      {
	printF("Changing width to %i\n",width);
	cg.changeInterpolationWidth(width);
	parameters.dbase.get<int >("reducedInterpolationWidth")=width;
      }
      else
      {
	printF("Sorry, the requested width=%i should be <= %i\n",width,oldWidth);
      }
    }
  }
  else if( dialog.getTextValue(answer,"reduce interpolation width","%i",width) )
  {
    int oldWidth=max(cg.interpolationWidth);
    if( width<oldWidth && width>0 )
    {
      printF("Changing the interpolation width to %i\n",width);
      cg.changeInterpolationWidth(width);
      parameters.dbase.get<int >("reducedInterpolationWidth")=width;
    }
    else
    {
      printF("Sorry, the requested width=%i should be <= %i and > 0 \n"
             "  No change was made.\n", width,oldWidth);
    }
  }
  else if( answer=="pin corners or edges" )
  {
    printF("To pin (i.e. fix the values) at a corner or edge first specify the 5 values:\n"
           "   grid, side1, side2, side3, option \n"
           " where \n"
           "   grid = valid grid number,\n"
           "   side1 = 0,1 or -1 to pin r1=side1 (-1 : pin this edge),\n"
           "   side2 = 0,1 or -1 to pin r2=side2 (-1 : pin this edge),\n"
           "   side3 = 0,1 or -1 to pin r3=side3 (-1 : pin this edge),\n"
           "   option = 0,1 : 1=pin, 0=do not pin.\n"
           "Examples: \n"
           "   grid=0, side1= 0, side2=0, side3=0, option=1 : pin the point r=(0,0,0) of grid 0.\n"
           "   grid=1, side1=-1, side2=0, side3=0, option=1 : pin the edge r=([0,1],0,0) of grid 1.\n");


    IntegerArray & pinBoundaryCondition = parameters.dbase.get<IntegerArray>("pinBoundaryCondition");
    int numberToPin=pinBoundaryCondition.getLength(1);
    RealArray & pinValues = parameters.dbase.get<RealArray>("pinValues");

    for( ;; )
    {

      int grid,side1,side2,side3,option;
      gi.inputString(answer2,"Enter grid, side1, side2, side3, option ('done' to finish)");
      sScanF(answer2,"%i %i %i %i %i",&grid,&side1,&side2,&side3,&option);
      if( answer2=="done" ) break;
    
      real u1=0., u2=0., u3=0., v1=0., v2=0., v3=0., s11=0., s12=0., s13=0., s22=0., s23=0., s33=0.;
      if( cg.numberOfDimensions()==2 )
      {
	gi.inputString(answer2,"Enter values: u1,u2, v1,v2, s11, s12, s22");
	sScanF(answer2,"%e %e %e %e %e %e %e",&u1,&u2,&v1,&v2,&s11,&s12,&s22);
      }
      else
      {
	gi.inputString(answer2,"Enter values: u1,u2,u3, v1,v2,v3, s11, s12, s13, s22, s23, s33");
	sScanF(answer2,"%e %e %e %e %e %e %e %e %e %e %e %e ",&u1,&u2,&u3, &v1,&v2,&v3,  &s11,&s12,&s13,&s22,&s23,&s33);
      }
    
      const int n=numberToPin;
      numberToPin++;
      pinBoundaryCondition.resize(5,numberToPin);


      pinBoundaryCondition(0,n)=grid;
      pinBoundaryCondition(1,n)=side1;
      pinBoundaryCondition(2,n)=side2;
      pinBoundaryCondition(3,n)=side3;
      pinBoundaryCondition(4,n)=option;

      pinValues.resize(12,numberToPin);

      pinValues( 0,n)=u1;
      pinValues( 1,n)=u2;
      pinValues( 2,n)=u3;
      pinValues( 3,n)=v1;
      pinValues( 4,n)=v2;
      pinValues( 5,n)=v3;
      pinValues( 6,n)=s11;
      pinValues( 7,n)=s12;
      pinValues( 8,n)=s13;
      pinValues( 9,n)=s22;
      pinValues(10,n)=s23;
      pinValues(11,n)=s33;
      
    }
    
    printF("INFO: Here are the corners or edges that will be pinned:\n");
    for( int n=0; n<numberToPin; n++ )
    {
      int grid =pinBoundaryCondition(0,n);
      if( grid>=0 && grid<cg.numberOfComponentGrids() )
      {
	printF(" grid=%i (%s) r=(%i,%i,%i) option=%i ",grid,(const char*)cg[grid].getName(),
	       pinBoundaryCondition(1,n),pinBoundaryCondition(2,n),pinBoundaryCondition(3,n),
	       pinBoundaryCondition(4,n));
	if( cg.numberOfDimensions()==2 )
	{
	  printF(" : (u1,u2)=(%9.3e,%9.3e) (v1,v2)=(%9.3e,%9.3e) (s11,s12,s22)=(%9.3e,%9.3e,%9.3e)\n",
		 pinValues( 0,n),pinValues( 1,n),
		 pinValues( 3,n),pinValues( 4,n),
		 pinValues( 6,n),pinValues( 7,n),pinValues( 9,n));
	}
	else
	{
	  printF(" : (u1,u2,u3)=(%9.3e,%9.3e) (v1,v2,v3)=(%9.3e,%9.3e) (s11,s12,s13,s22,s23,s33)=(%9.3e,%9.3e,%9.3e,%9.3e,%9.3e,%9.3e)\n",
		 pinValues( 0,n),pinValues( 1,n),pinValues( 2,n),
		 pinValues( 3,n),pinValues( 4,n),pinValues( 5,n),
		 pinValues( 6,n),pinValues( 7,n),pinValues( 8,n),pinValues( 9,n),pinValues(10,n),pinValues(11,n));
	}
	  
      }
      else
      {
	printF(" grid=%i **WARNING** invalid value for grid!\n",grid);
      }
    }
    
  }
  
  else
  {
    found=false;
  }
  

  return found;
}



int Cgsm::
buildPlotOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the plot options dialog.
// ==========================================================================================
{

  real & tPlot = parameters.dbase.get<real>("tPrint");

  // ************** PUSH BUTTONS *****************


  // ************** TOGGLE BUTTONS *****************
  dialog.setOptionMenuColumns(1);

  aString tbCommands[] = {"plot errors",
                          "plot divergence",
                          "plot vorticity",
                          "plot dissipation",
                          "plot scattered field",
                          "plot total field",
                          "check errors",
                          "compare to show file",
                          "compute energy",
                          "plot velocity",
                          "plot stress",
                          "adjust grid for displacement",
 			  ""};
  int tbState[15];
  tbState[0] = plotErrors;
  tbState[1] = plotDivergence;
  tbState[2] = plotVorticity;
  tbState[3] = plotDissipation;
  tbState[4] = plotScatteredField;
  tbState[5] = plotTotalField;
  tbState[6] = checkErrors;
  tbState[7] = compareToReferenceShowFile;
  tbState[8] = computeEnergy;
  tbState[9] = plotVelocity; 
  tbState[10]= plotStress; 
  tbState[11]= parameters.dbase.get<int>("adjustGridForDisplacement");

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 



  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "times to plot";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",tPlot);  nt++; 

  textCommands[nt] = "displacement scale factor";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",dScale); nt++; 

  textCommands[nt] = "radius for checking errors";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",radiusForCheckingErrors); nt++; 

  textCommands[nt] = "reference show file:";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)nameOfReferenceShowFile); nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


//================================================================================
/// \brief: Look for a plot option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int Cgsm::
getPlotOption(const aString & answer,
		 DialogData & dialog )
{
  real & tPlot = parameters.dbase.get<real>("tPrint");

  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");

  int found=true; 
  char buff[180];
  aString answer2;
  int len=0;

  // *** NOTE: if you change these you should also change the ones in plot.bC ***

  if( dialog.getTextValue(answer,"times to plot","%g",tPlot) ){}  // 
  // if( dialog.getTextValue(answer,"tPlot","%g",tPlot) ){}  // for backward compatibility 
  else if( dialog.getTextValue(answer,"displacement scale factor","%g",dScale) )
  { 
    psp.set(GI_DISPLACEMENT_SCALE_FACTOR,dScale);
  }
  else if( dialog.getTextValue(answer,"radius for checking errors","%f",radiusForCheckingErrors) ){}//
  else if( dialog.getTextValue(answer,"reference show file:","%s",nameOfReferenceShowFile) ){}//
  else if( dialog.getToggleValue(answer,"plot dissipation",plotDissipation) ){}//
  else if( dialog.getToggleValue(answer,"plot errors",plotErrors) ){}//
  else if( dialog.getToggleValue(answer,"plot scattered field",plotScatteredField) ){}//
  else if( dialog.getToggleValue(answer,"plot total field",plotTotalField) ){}//
  else if( dialog.getToggleValue(answer,"plot dissipation",plotDissipation) ){}//
  else if( dialog.getToggleValue(answer,"plot divergence",plotDivergence) )
  {
    parameters.setShowVariable( "div",plotDivergence );
  }
  else if( dialog.getToggleValue(answer,"plot vorticity",plotVorticity) )
  {
    parameters.setShowVariable( "vor",plotVorticity );
  }
  else if( dialog.getToggleValue(answer,"check errors",checkErrors) ){}//
  else if( dialog.getToggleValue(answer,"compute energy",computeEnergy) ){}//
  else if( dialog.getToggleValue(answer,"compare to show file",compareToReferenceShowFile) ){}//
  else if( dialog.getToggleValue(answer,"plot velocity",plotVelocity) )
  {
    const SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    if( pdeVariation==SmParameters::nonConservative ||
        pdeVariation==SmParameters::conservative )
    {
      parameters.setShowVariable( "v1",plotVelocity );
      parameters.setShowVariable( "v2",plotVelocity );
      if( cg.numberOfDimensions()==3 )
	parameters.setShowVariable( "v3",plotVelocity );

    }
  }
  else if( dialog.getToggleValue(answer,"plot stress",plotStress) )
  {
    const SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
    if( pdeVariation==SmParameters::nonConservative ||
        pdeVariation==SmParameters::conservative )
    {
      parameters.setShowVariable( "s11",plotStress );
      parameters.setShowVariable( "s12",plotStress );
      parameters.setShowVariable( "s21",plotStress );
      parameters.setShowVariable( "s22",plotStress );
      if( cg.numberOfDimensions()==3 )
      {
	parameters.setShowVariable( "s13",plotStress );
	parameters.setShowVariable( "s23",plotStress );
	parameters.setShowVariable( "s31",plotStress );
	parameters.setShowVariable( "s32",plotStress );
	parameters.setShowVariable( "s33",plotStress );
      }
    }
    
  }//
  else if( dialog.getToggleValue(answer,"adjust grid for displacement",parameters.dbase.get<int>("adjustGridForDisplacement")) )
  {
    psp.set(GI_ADJUST_GRID_FOR_DISPLACEMENT,parameters.dbase.get<int>("adjustGridForDisplacement"));
  }
  else
  {
    found=false;
  }

  return found;

}


// ===================================================================================================================
/// \brief Build the input-output options dialog.
/// \param dialog (input) : graphics dialog to use.
///
// ==================================================================================================================
int Cgsm::
buildInputOutputOptionsDialog(DialogData & dialog )
{

  aString elementTypeCommands[] = {"structured", "triangles", "quadrilaterals", "" };
  dialog.addOptionMenu("elements:", elementTypeCommands, elementTypeCommands, (int)elementType );


  // ************** PUSH BUTTONS *****************
  aString pushButtonCommands[] = {"specify probes",
                                  "show file options...",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 


  dialog.setOptionMenuColumns(1);


  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  int & debug = parameters.dbase.get<int >("debug");
  textCommands[nt] = "debug";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",debug); nt++; 

  textCommands[nt] = "probe frequency";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",frequencyToSaveProbes); nt++; 

  textCommands[nt] = "probe file:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)probeFileName); nt++; 

  textCommands[nt] = "grid file:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s","none"); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


//================================================================================
/// \brief: Look for an input/output option in the string "answer"
///
/// \param answer (input) : check this command 
///
/// \return return 1 if the command was found, 0 otherwise.
//====================================================================
int Cgsm::
getInputOutputOption(const aString & answer,
		     DialogData & dialog )
{
  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  int & debug = parameters.dbase.get<int >("debug");
  
  int found=true; 
  char buff[180];
  aString answer2;
  int len=0;

  if( answer=="structured" || answer=="triangles" || answer=="quadrilaterals" )
  {
    elementType=answer=="structured" ? structuredElements : answer=="triangles" ? triangles : quadrilaterals;
    dialog.getOptionMenu("elements:").setCurrentChoice((int)elementType);
  }
  else if( dialog.getTextValue(answer,"debug:","%i",debug) ){}//
  else if( dialog.getTextValue(answer,"probe file:","%s",probeFileName) ){}//
  else if( dialog.getTextValue(answer,"probe frequency","%i",frequencyToSaveProbes) ){}//
  else if( answer=="specify probes" )
  {
    // aString probeFileName;
    // gi.inputString(probeFileName,"Enter the name of the file for saving probe data");
      
    int numberOfProbes=0;
    RealArray values;
    numberOfProbes=gi.getValues("Enter a list of probe positions (x,y,z) (done to finish)",values);
    // values.display("values");
      
    numberOfProbes/=3;
    if( numberOfProbes>0 )
    {
      probes.redim(3,numberOfProbes);
      probeGridLocation.redim(4,numberOfProbes);

      // *** find closest grid and grid point to save in probes ****

      RealArray positionToInterpolate(numberOfProbes,3);
      IntegerArray indexValues, interpoleeGrid;
      int i,j;
      for( i=0,j=0; i<numberOfProbes; i++ )
      {

	positionToInterpolate(i,0)=values(j); j++;
	positionToInterpolate(i,1)=values(j); j++;
	positionToInterpolate(i,2)=values(j); j++;
      }
	

      if( false )
      {
	// ***fix**** this requires the center array I think
	InterpolatePoints interp;
	interp.buildInterpolationInfo(positionToInterpolate,cg );
	interp.getInterpolationInfo(cg, indexValues, interpoleeGrid);
      }
      else
      {
	// locate the nearest grid point
          
	const int numberOfDimensions = cg.numberOfDimensions();
	Range I=numberOfProbes;
	realArray r(I,numberOfDimensions), x(I,numberOfDimensions);
	int axis;
	for( i=0; i<numberOfProbes; i++ )
	{
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    x(i,axis)=positionToInterpolate(i,axis);
	  }
	}
	  
	indexValues.redim(I,numberOfDimensions);
	interpoleeGrid.redim(I);
	interpoleeGrid=-1;

	int iv[3], &i1=iv[0], &i2=iv[1], &i3=iv[2];
	int numFound=0;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  MappedGrid & mg = cg[grid];
	  Mapping & map = mg.mapping().getMapping();
	  const intArray & mask = mg.mask();
	  i3=mg.gridIndexRange(0,2);

	  if ( mg.getGridType()==MappedGrid::structuredGrid )
	  {
	    r=-1.;
	    map.inverseMap(x,r);
	    for( i=0; i<numberOfProbes; i++ )
	    {
	      if( interpoleeGrid(i)<0 ) // this point not yet found
	      {
		bool ok=true;
		for( axis=0; axis<numberOfDimensions; axis++ )
		{
		  // closest point:
		  iv[axis] = int( r(i,axis)/mg.gridSpacing(axis)+ mg.gridIndexRange(0,axis) +.5);
		  if( iv[axis]< mg.gridIndexRange(0,axis) ||
		      iv[axis]> mg.gridIndexRange(1,axis) )
		  {
		    ok=false;
		    break;
		  }
		}
		// *** if( ok && mask(i1,i2,i3)>0 ) // *** fix this -- P++ problem --
		if( ok ) 
		{
		  for( axis=0; axis<numberOfDimensions; axis++ )
		    indexValues(i,axis)=iv[axis];
		  interpoleeGrid(i)=grid;
		  numFound++;
		}
	      }
		
	    } // end for i
	  }
	  else
	  {
	    for( i=0; i<numberOfProbes; i++ )
	    {
	      real xx[3];
	      xx[2] = 0.;
	      for ( int a=0; a<mg.numberOfDimensions(); a++ )
		xx[a] = x(i,a);

	      UnstructuredMapping & umap = (UnstructuredMapping &)mg.mapping().getMapping();
		    
	      int ent = umap.findClosestEntity(UnstructuredMapping::Face, xx[0],xx[1],xx[2]);
	      assert(ent!=-1);

	      interpoleeGrid(i) = grid;
	      indexValues(i,0) = ent;
	      for ( int a=1; a<mg.numberOfDimensions(); a++ )
		indexValues(i,a) = 0;
	      numFound++;
	    }
	  }

	  if( numFound==numberOfProbes ) break;
	} // end for grid
      }
	
	
      for( i=0,j=0; i<numberOfProbes; i++ )
      {
	probes(0,i)=values(j); j++;
	probes(1,i)=values(j); j++;
	probes(2,i)=values(j); j++;
	probeGridLocation(0,i)=indexValues(i,0);
	probeGridLocation(1,i)=indexValues(i,1);
	if( cg.numberOfDimensions()==3 )
	  probeGridLocation(2,i)=indexValues(i,2);
	else
	  probeGridLocation(2,i)=0;
	  
	probeGridLocation(3,i)=interpoleeGrid(i);
	if( interpoleeGrid(i)<0 )
	{
	  printF(" probe: error location probe %i: x=(%9.3e,%9.3e,%9.3e)\n",i,
		 probes(0,i),probes(1,i),probes(2,i));
	  Overture::abort();
	}

	printF(" probe %i: x=(%9.3e,%9.3e,%9.3e), closest grid=%i, pt i=(%i,%i,%i)\n",i,
	       probes(0,i),probes(1,i),probes(2,i),
	       probeGridLocation(3,i),probeGridLocation(0,i),probeGridLocation(1,i),probeGridLocation(2,i)   );
      }
    }
    else
    {
      printf("INFO: No probes were specified\n");
    }
  }
  else
  {
    found=false;
  }
  return found;
}

// int Cgsm::
// buildPdeParametersDialog(DialogData & dialog )
// // ==========================================================================================
// // /Description:
// //   Build the pde parameters dialog.
// // ==========================================================================================
// {

//   real & rho = parameters.dbase.get<real>("rho");
//   real & mu = parameters.dbase.get<real>("mu");
//   real & lambda = parameters.dbase.get<real>("lambda");
//   RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
//   RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");

//   // ----- Text strings ------
//   const int numberOfTextStrings=30;
//   aString textCommands[numberOfTextStrings];
//   aString textLabels[numberOfTextStrings];
//   aString textStrings[numberOfTextStrings];

//   int nt=0;

//   textCommands[nt] = "lambda";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",lambda); nt++; 

//   textCommands[nt] = "mu";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",mu); nt++; 

//   textCommands[nt] = "rho";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",rho); nt++; 

//   textCommands[nt] = "coefficients";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g %g %s (lambda,mu,grid-name)",lambda,mu,"all"); nt++; 

//   SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

//   if( true || pdeVariation==SmParameters::godunov )
//   {

//     textCommands[nt] = "Godunov order of accuracy";  
//     textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",
// 					     parameters.dbase.get<int >("orderOfAccuracyForGodunovMethod")); nt++; 
//   }

//   // null strings terminal list
//   assert( nt<numberOfTextStrings );
//   textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
//   dialog.setTextBoxes(textCommands, textLabels, textStrings);

//   return 0;
// }


// ====================================================================================================
/// \brief Set parameters, boundary conditions, forcings, initial conditions etc.
///        that define the problem
// ====================================================================================================
int Cgsm::
setParametersInteractively()
{

  DomainSolver::setParametersInteractively();


  initializeRadiationBoundaryConditions();
  
  return 0;
}

int Cgsm::
setBoundaryCondition( aString & answer, IntegerArray & originalBoundaryCondition )
//  ================================================================================================================
//   Parse an answer to set boundary conditions.
//  ================================================================================================================
{
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  GenericGraphicsInterface & gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");

  int length=answer.length();
  int i,mark=-1;
  for( i=0; i<length; i++ )
  {
    if( answer[i]=='(' || answer[i]=='=' )
    {
      mark=i-1;
      break;
    }
  }
  if( mark<0 )
  {
    printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer);
    gi.stopReadingCommandFile();
    return 1;
  }
  else
  {
    Range G,S,A;
    aString gridName;

    gridName=answer(0,mark);  // this is the name of the grid or `all'
    S=Range(-1,-1);
    A=Range(-1,-1);
    if( answer[mark+1]=='(' )
    { // determine which side and axis to assign
      int side=-1,axis=-1;
      int numRead=sscanf(answer(mark+1,length-1),"(%i,%i)",&side,&axis);
      if( numRead==2 )
      {
	if( side>=0 && side<=1 && axis>=0 && axis<=cg.numberOfDimensions()-1 )
	{
	  S=Range(side,side);
	  A=Range(axis,axis);
	  for( i=mark+1; i<length; i++ )
	  {
	    if( answer[i]=='=' )
	    {
	      mark=i-1;
	      break;
	    }
	  }
	}
	else
	{
	  printF("invalid values for side=%i or axis=%i, 0<=side<=1, 0<=axis<=%i \n",side,axis,
		 cg.numberOfDimensions()-1);

	  gi.stopReadingCommandFile();
	  return 1;
	}
      }
    }
    else
    { // assign all sides:
      S=Range(0,1);
      A=Range(0,cg.numberOfDimensions()-1);
    }
    if( S.getBase()==-1 || A.getBase()==-1 || mark+2>length-1 )
    {
      printF("unknown form of answer=[%s]. Try again or type `help' for examples.\n",(const char *)answer);
      gi.stopReadingCommandFile();
      return 1;
    }
    // search for a blank separating the bc name from any options
    int endOfName=length-1;
    for( i=mark+3; i<length; i++ )
    {
      if( answer[i]==' ' || answer[i]==',' )
      {
	endOfName=i-1;
	break;
      }
    }

    aString nameOfBC=answer(mark+2,endOfName);
    mark=endOfName+1;

    G=Range(-1,-1);
    int changeBoundaryConditionNumber=-1;  // for bcNumber#=nameOfBC
    int len,grid;
    
    if( gridName=="all" )
    {
      G=Range(0,cg.numberOfComponentGrids()-1);
    }
    else if( len=gridName.matches("bcNumber") )
    {
      // BC of the form
      //    bcNumber3=noSlipWall
      G=Range(0,cg.numberOfComponentGrids()-1); // check all grids

      sScanF(gridName(len,gridName.length()-1),"%i",&changeBoundaryConditionNumber);
      printF("setting BC number %i to be %s\n",changeBoundaryConditionNumber,(const char*)nameOfBC);

    }
    else
    { // search for the name of the grid
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	if( gridName==cg[grid].getName() )
	{
	  G=Range(grid,grid);
	  break;
	}
      }
    }
    if( G.getBase()==-1  )
    {
      printF("Unknown grid name = <%s> \n",(const char *)gridName);
      gi.stopReadingCommandFile();
      return 1;
    }
    // search for the name of the boundary condition
    int bc=-1;
    for( int i=1; i<SmParameters::numberOfBCNames; i++ )
    {
      if( nameOfBC==SmParameters::bcName[i] )
      {
	bc=i;
	break;
      }
    }
    if( bc==-1 )
    {
      printF("ERROR: unable to find bc with name=[%s]\n",(const char*)nameOfBC);
      return -1;
    }
    
    if( gridName=="all")
    {
      printF("Setting boundary condition to %s (=%i) on all grids.\n",(const char*)SmParameters::bcName[bc],bc);
    }

    for( grid=G.getBase(); grid<=G.getBound(); grid++ )
    {
      for( int axis=A.getBase(); axis<=A.getBound(); axis++ )
      {
	for( int side=S.getBase(); side<=S.getBound(); side++ )
	{
	  if( cg[grid].boundaryCondition(side,axis) > 0 && 
	      (changeBoundaryConditionNumber==-1 || 
	       originalBoundaryCondition(side,axis,grid)==changeBoundaryConditionNumber) )
	  {
            printF("Setting grid=%i (side,axis)=(%i,%i) to bc=%i\n",grid,side,axis,bc);
	    
	    cg[grid].setBoundaryCondition(side,axis,bc);
	    // set underlying mapping too (for moving grids)
	    cg[grid].mapping().getMapping().setBoundaryCondition(side,axis,bc);
	    
	  }
	}
      }
    }
  }
  
  return 0;
}

// ===================================================================================================================
/// \brief Output run-time parameters for the header.
/// \param file (input) : write values to this file.
///
// ===================================================================================================================
void
Cgsm::
writeParameterSummary( FILE * file )
{

  DomainSolver::writeParameterSummary( file );

  if ( file==parameters.dbase.get<FILE* >("checkFile") )
  {
    fprintf(parameters.dbase.get<FILE* >("checkFile"),"\\caption{SolidMechanics, gridName, $\\mu=%3.2f$, "
            "$\\lambda=%3.2f$, $\\rho=%3.2f$, $t=%2.1f$, ",
	    parameters.dbase.get<real >("mu"),parameters.dbase.get<real >("lambda"),parameters.dbase.get<real >("rho"),
            parameters.dbase.get<real >("tFinal"));

    return;
    
  }
  fPrintF(file,"\n");
  // Do this for now:
  // pdeTypeForGodunovMethod==2 : SVK 
  const int pdeTypeForGodunovMethod = parameters.dbase.get<int >("pdeTypeForGodunovMethod");
  if( pdeTypeForGodunovMethod==2 )
  {
    fPrintF(file," Solving the St.-Venant Kirchoff model.\n");
  }
  else if( pdeTypeForGodunovMethod==3 )
  {
    fPrintF(file," Solving the St.-Venant Kirchoff model with rotated-linear stress-strain.\n");
  }
  

  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  fPrintF(file," PDE Variation = %s\n\n",(const char*)SmParameters::PDEVariationName[pdeVariation]);
  

  SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");
  real & cfl = parameters.dbase.get<real>("cfl");
  real & tFinal = parameters.dbase.get<real>("tFinal");
  real & tPlot = parameters.dbase.get<real>("tPrint");
  real & dt= deltaT;
  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

  // fPrintF(file," Using pde model: %s\n",(const char *)SmParameters::PDEModelName[pdeModel]);

//   if( timeSteppingMethod==modifiedEquationTimeStepping )
//     fPrintF(file," Time stepping method is modifiedEquation\n");
    
  if( parameters.dbase.get<int>("variableMaterialPropertiesOption")==0 )
  {
    fPrintF(file," Material parameters are constant:\n");
    fPrintF(file," lambda=%9.3e, mu=%9.3e,  rho=%9.3e\n",
	    parameters.dbase.get<real >("lambda"),parameters.dbase.get<real >("mu"),
	    parameters.dbase.get<real >("rho"));
  }
  else
  {
    fPrintF(file," Material parameters are variable.\n");
  }
  
  // fPrintF(file," order of accuracy: space=%i, time=%i\n",orderOfAccuracyInSpace,orderOfAccuracyInTime);
  fPrintF(file," artificial diffusion: coefficient=%8.2e, order=%i\n",artificialDissipation,
	  orderOfArtificialDissipation);
            
  fPrintF(file," Stress relaxation is %s, order=%i, alpha=%8.2e, delta=%8.2e\n",
	  (parameters.dbase.get<int>( "stressRelaxation" )==0 ? "off" : "on" ),
	  parameters.dbase.get<int>( "stressRelaxation" ), parameters.dbase.get<real>( "relaxAlpha" ),
	  parameters.dbase.get<real>( "relaxAlpha" ));

  if( parameters.dbase.get<bool >("applyFilter") )
  {
    GridFunctionFilter *& gridFunctionFilter =parameters.dbase.get<GridFunctionFilter*>("gridFunctionFilter");
    assert( gridFunctionFilter!=NULL );
    GridFunctionFilter & filter = *gridFunctionFilter;
    const int orderOfFilter = filter.orderOfFilter;
    const int filterStages = filter.numberOfFilterStages;
    const int filterFrequency = filter.filterFrequency;
    const int numberOfFilterIterations = filter.numberOfFilterIterations;

    const real filterCoefficient = filter.filterCoefficient;

    fPrintF(file,
           " apply high order filter, type=%s, order=%i, stages=%i, frequency=%i, iterations=%i, coefficient=%g\n",
            (filter.filterType==GridFunctionFilter::explicitFilter ? "explicit" : "implicit"),
	    orderOfFilter,filterStages,filterFrequency,numberOfFilterIterations,filterCoefficient);
  }
  else
    fPrintF(file," do not apply the high order filter\n");

  if( pdeVariation==SmParameters::godunov )
  {
    const RealArray & artificialDiffusion = parameters.dbase.get<RealArray >("artificialDiffusion");
    const RealArray & artificialDiffusion4 = parameters.dbase.get<RealArray >("artificialDiffusion4");
    if( max(artificialDiffusion)==0. && max(artificialDiffusion4)==0. )
    {
      fPrintF(file," Godunov: constant-coefficient artificial diffusion is OFF.\n");
    }
    else
    {
      fPrintF(file," Godunov: constant-coefficient artificial diffusion: \n");
      for( int m=0; m<parameters.dbase.get<int >("numberOfComponents"); m++ )
      {
	fPrintF(file,"  ad2=%8.2e, ad4=%8.2e (%s) \n",
		artificialDiffusion(m),artificialDiffusion4(m),(const char*) parameters.dbase.get<aString* >("componentName")[m]);
      }
    }
    
  }


  fPrintF(file," extrapolateInterpolationNeighbours = %i, orderOfExtrapolation=%i \n",
          (int)parameters.dbase.get<int >("extrapolateInterpolationNeighbours"),
          parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"));

  Parameters::TwilightZoneChoice & twilightZoneChoice = 
    parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice");
  const int & tzDegreeSpace= parameters.dbase.get<int >("tzDegreeSpace");
  const int & tzDegreeTime = parameters.dbase.get<int >("tzDegreeTime");

  fPrintF(file," Forcing option:");
  if( forcingOption==noForcing )
    fPrintF(file," no forcing.\n");
  else if( forcingOption==twilightZoneForcing )
  {
    fPrintF(file," twilightzone forcing, ");
    if( twilightZoneChoice==Parameters::polynomial )
      fPrintF(file," polynomial solution, degreeSpace=%i, degreeTime=%i\n",tzDegreeSpace,tzDegreeTime);
    else if( twilightZoneChoice==Parameters::trigonometric )
      fPrintF(file," trigonometric solution.\n");
    else if( twilightZoneChoice==Parameters::pulse )
      fPrintF(file," pulse solution.\n");
    else
     fPrintF(file," UNKNOWN !\n"); 
  }
  else if( forcingOption==gaussianSource )
    fPrintF(file," Gaussian source.\n");
  else if( forcingOption==userDefinedForcingOption )
    fPrintF(file," user defined forcing.\n");
  else
    fPrintF(file,"UNKNOWN! \n");

//   const IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");
//   interfaceType.display("interfaceType");
  
//   if( pdeModel==SmParameters::linearElasticity )
//   {
//     MappedGridOperators & mgop = (*cgop)[0];
//     fPrintF(file," Using the %s difference approximation\n",
// 	    mgop.usingConservativeApproximations() ? "conservative" : "non-conservative" );
      
//   }

}

// ======================================================================================================
/// \brief:  Set the titles and labels that go on the show file output 
// ======================================================================================================
void Cgsm::
saveShowFileComments( Ogshow & show )
{
  // save comments that go at the top of each plot

  aString timeLine="";
  if(  parameters.dbase.has_key("timeLine") )
    timeLine=parameters.dbase.get<aString>("timeLine");
  aString showFileTitle[5];

  aString methodName,buff;
  getMethodName( methodName );
  
  showFileTitle[0]=sPrintF(buff,"Cgsm %s",(const char *)methodName);
  showFileTitle[1]=timeLine;
  showFileTitle[2]="";  // marks end of titles


  for( int i=0; showFileTitle[i]!=""; i++ )
    show.saveComment(i,showFileTitle[i]);

}  


// ===================================================================================================================
/// \brief Update geometry arrays when the grid has changed (called by adaptGrids for example).
/// \param cgf (input) : 
// ===================================================================================================================
int Cgsm::
updateGeometryArrays(GridFunction & cgf)
{
  if( debug() & 8 ) printP(" --- updateGeometryArrays ---\n");
  
  CompositeGrid & cg = cgf.cg;

  // These next are used in adaptGrids: 
  if( realPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  if( imaginaryPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  
  dxMinMax.redim(cg.numberOfComponentGrids(),2);
  dxMinMax=0.;

// from Cgcns.C: 

//   if( parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes ||
//       parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleMultiphase )
//   {
//     if( parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeWithArtificialDissipation )
//     {

//       cgf.cg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEinverseCenterDerivative | 
// 		    MappedGrid::THEcenterJacobian |
// 		    MappedGrid::THEvertex | MappedGrid::THEcenter );
//     }
//     else
//     {
//       int grid;
//       for( grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
//       {
// 	MappedGrid & mg = cgf.cg[grid];
// 	if( mg.isRectangular() )
// 	{
// 	  mg.update(MappedGrid::THEmask);
// 	}
// 	else
// 	{
// 	  mg.update(MappedGrid::THEinverseVertexDerivative | MappedGrid::THEinverseCenterDerivative | 
// 		    MappedGrid::THEcenterJacobian );
// 	}
//       }
//     }

//   }

  return DomainSolver::updateGeometryArrays(cgf);
}



//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateForAdaptiveGrids}} 
int Cgsm::
updateForAdaptiveGrids(CompositeGrid & cg)
//=========================================================================================
// /Description:
//    Update the CompositeGridSolver after an AMR grid has been generated. Currently this
// is only called after the initial conditions have been adapted.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  if( pdeVariation==SmParameters::hemp )
  {
    assert(  parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction")!=NULL );
    realCompositeGridFunction & initialState = 
      *parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction");

    initialState.updateToMatchGrid(cg);
  }

  return DomainSolver::updateForAdaptiveGrids(cg);
}
