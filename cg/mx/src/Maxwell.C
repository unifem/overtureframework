#include "Maxwell.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "OGPolyFunction.h"
#include "OGTrigFunction.h"
#include "InterpolatePoints.h"
#include "Ogshow.h"
#include "ShowFileReader.h"
#include "RadiationBoundaryCondition.h"
#include "RadiationKernel.h"
#include "Oges.h"
#include "ParallelUtility.h"

aString Maxwell::
bcName[numberOfBCNames]={
  "periodic",
  "dirichlet",
  "perfectElectricalConductor",
  "perfectMagneticConductor",
  "planeWaveBoundaryCondition",
  "symmetry",
  "interfaceBoundaryCondition", // for the interface between two regions with different properties
  "abcEM2",     // absorbing BC, Engquist-Majda order 2 
  "abcPML",     // perfectly matched layer
  "abc3",           // future absorbing BC
  "abc4",           // future absorbing BC
  "abc5",           // future absorbing BC
  "rbcNonLocal",    // radiation BC, non-local
  "rbcLocal"        // radiation BC, local
};
  

// =====================================================================================
/// \brief Constructor for the Maxwell solver.
// =====================================================================================
Maxwell::
Maxwell()
{
  myid=max(0,Communication_Manager::My_Process_Number);


  /// \li <b> allowUserDefinedOutput (int) </b> : if true call the userDefinedOutput function.
  if( !dbase.has_key("allowUserDefinedOutput") ) dbase.put<int>("allowUserDefinedOutput");  // this is a test -- not used.


  gip=NULL;

  debug=0;
  debugFile   = fopen("mx.debug","w" );        // Here is the log file

  // *** open multiple debug files for each processor ****
  const int np= max(1,Communication_Manager::numberOfProcessors());
  aString buff;
#ifndef USE_PPP
  debugFile   = fopen("mx.debug","w" );        // Here is the log file
  pDebugFile= debugFile;
#else
  debugFile = fopen(sPrintF(buff,"mxNP%i.debug",np),"w" );  // Here is the debug file
  pDebugFile = fopen(sPrintF(buff,"mx%i.debug",myid),"w");
#endif

  checkFile   = fopen("mx.check","w" );        // for regression and convergence tests
  
  method=defaultMethod;
  bcOption=useGeneralBoundaryConditions; // *wdh* 040109 useAllPeriodicBoundaryConditions;

  initialConditionOption=defaultInitialCondition;
  forcingOption=noForcing;
  knownSolutionOption=noKnownSolution;
  
  gridHasMaterialInterfaces=false;
  useNewInterfaceRoutines=true; // if true: 2D AND 3D interface routines found in interface3d.bd. old=interface.bf
  
  frequency=5.;
  checkErrors=true;
  radiusForCheckingErrors=-1;  // if >0 only check errors in a disk (sphere) of this radius  
  computeEnergy=false;
  totalEnergy=0.;
  initialTotalEnergy=-1.;  // energy at time 0 (<0 : means has not been assigned)
  
  maximumNumberOfIterationsForImplicitInterpolation=-1; // -1 : use default

  numberOfIterationsForInterfaceBC=3;
  
  plotDivergence=true;
  plotErrors=true;
  plotDissipation=false;

  plotScatteredField=false;   // set to true to subtract off the plane wave
  plotTotalField=false;       // set to true to add on the plane wave
  plotRho=false;         
  plotEnergyDensity=false;

  plotIntensity=false;
  pIntensity=NULL;
  intensityOption=1;  // 0=compute from time average, 1=compute from just two solutions

  omegaTimeHarmonic=-1.;  // the intensity computation needs to know the frequency in time,  omegaTimeHarmonic = omega/(2 pi)
  intensityAveragingInterval=1.; // average intensity over this many periods
  
  plotHarmonicElectricFieldComponents=false;  // plot Er and Ei assuming : E(x,t) = Er(x)*cos(w*t) + Ei(x)*sin(w*t) 
  pHarmonicElectricField=NULL;

  numberOfMaterialRegions=1; // for variable coefficients -- number of piecewise constant regions
  maskBodies=false;          // if true then PEC bodies with stair-stepping are defined
  pBodyMask=NULL;
  useTwilightZoneMaterials=false;          // if true define eps, mu, .. using the twilight-zone functions

  compareToReferenceShowFile=false;
  plotDSIPoints = false;
  plotDSIMaxVertVals = false;

  eps=1.;
  mu=1.;
  c = 1./sqrt(eps*mu);
  kx=1.; ky=kz=0.;
  pwc[0]=pwc[1]=pwc[2]=pwc[3]=pwc[4]=pwc[5]=0.;  // Plane wave coefficients

  omegaForInterfaceIteration=1.;
  materialInterfaceOption=1;  // 1=extrapolate as initial guess for material interface ghost values

  interfaceEquationsOption=0;  // 0=use extrapolation, 1=use centered approx. for 2nd ghost line (4th order)
  
  solveForElectricField=true;
  solveForMagneticField=true;

  // defaults for 2D
  ex=0;
  ey=1;
  hz=2;
  ez=hx=hy=-1;
  ext=eyt=ezt=hxt=hyt=hzt=-1;     // time derivatives (sosup)
  epsc=muc=sigmaEc=sigmaHc=-1;    // components for eps, mu, sigmaE and sigmaH in TZ functions
  numberOfComponentsRectangularGrid=3;

  ex10=0;   // ex10 : Ex(i1+1/2,i2)
  ey10=1; 
  ex01=2;   // ex01 : Ex(i1,i2+1/2)
  ey01=3; 
  hz11=4;   
  numberOfComponentsCurvilinearGrid=5;


  artificialDissipation=0.;
  artificialDissipationCurvilinear=-1.; // set to non-negative to use this instead of the above

  artificialDissipationInterval=1;
  orderOfArtificialDissipation=4;
  divergenceDamping=0.;
  
  applyFilter=false;          // true : apply the high order filter
  orderOfFilter=-1;           // this means use default order
  filterFrequency=2;          // apply filter every this many steps 
  numberOfFilterIterations=1; // number of iterations in the filter  
  filterCoefficient=1.;       // coefficient in the filter


  useDivergenceCleaning=false;
  divergenceCleaningCoefficient=1.;

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


  initialConditionBoundingBox.redim(2,3);

  Range all;
  initialConditionBoundingBox(0,all)= REAL_MAX*.1;
  initialConditionBoundingBox(1,all)=-REAL_MAX*.1;
  
  boundingBoxDecayExponent=2.;  // initial condition has a smooth transition outside the bounding box
  

  nx[0]=21;   nx[1]=21;   nx[2]=21;
  xab[0][0]=0.;  xab[1][0]=1.;
  xab[0][1]=0.;  xab[1][1]=1.;
  xab[0][2]=0.;  xab[1][2]=1.;
  
  
  cfl=.9;
  tFinal=1.;
  tPlot=-1.;

  gridType=unknown; // square;
  elementType=structuredElements;
  
  chevronFrequency = 1;
  chevronAmplitude = .1;

  cylinderRadius=1.;
  cylinderAxisStart=0.; cylinderAxisEnd=1.; // for eigenfunctions of the cylinder

  dbase.put<real>("scatteringRadius")=.5; // radius od cylinder or sphere for scattering solution

  orderOfAccuracyInSpace=2;
  orderOfAccuracyInTime=2;
  useConservative=true;

  useVariableDissipation=false;
  variableDissipation=NULL;
  numberOfVariableDissipationSmooths=10;  // number of times to smooth the variable dissipation function 

  useChargeDensity=false;
  pRho=NULL;
  poisson=NULL;
  pPhi=NULL;
  pF=NULL;
  projectFields=false;                // if true, project the fields to have the correct divergence
  frequencyToProjectFields=1;         // apply the project every this many steps
  numberOfConsecutiveStepsToProject=2; // project this many consecutive fields
  numberOfProjectionIterations=5;     // number of iterations in the projection solve
  numberOfInitialProjectionSteps=10;  // always project this first number of steps
  numberOfDivergenceSmooths=0;

  initializeProjection=true;
  useConservativeDivergence=false;
  projectInitialConditions=false;
  projectInterpolation=false;   // if true, project interp. pts so that div(E)=0


  degreeSpace=2;  // for TZ polynomial
  degreeSpaceX=degreeSpaceY=degreeSpaceZ=degreeSpace;
  
  degreeTime=2;
  omega[0]=omega[1]=omega[2]=omega[3]=2.;  // for trig TZ
  
  for( int i=0; i<10; i++ )
    initialConditionParameters[i]=0.;
  
  timeSteppingMethod=defaultTimeStepping;

  divEMax=0.;
  gradEMax=0.; // kkc this will be used to store divHMax in the dsi algorithm
  
  errorNorm=0; // set to 1 or 2 to compute L1 and L2 error norms too

  slowStartInterval=-1.;  // negative means don't use

  fields=NULL;
  dissipation=NULL;
  errp=NULL;
  
  op=NULL;
  mgp=NULL;
  cgp=NULL;
  
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
  cgfields=NULL;
  cgdissipation=NULL;
  e_cgdissipation=NULL;
  cgerrp=NULL;
  pinterpolant=NULL;
  knownSolution=NULL;
  
  show=NULL;
  referenceShowFileReader=NULL;  // for comparing to a reference solution

  sequenceCount=0; 
  numberOfSequences=0;
  
  // for radiation BC's
  radbcGrid[0]=radbcGrid[1]=-1; radbcSide[0]=radbcSide[1]=-1; radbcAxis[0]=radbcAxis[1]=-1;
  radiationBoundaryCondition=NULL;

  useStreamMode=true;
  saveGridInShowFile=true;
  frequencyToSaveInShowFile=1;
  showFileFrameForGrid=-1;
  nameOfReferenceShowFile="ref.show";
  
  numberOfFields=2; // 2 levels saved by default
  // These next variables are for higher order time stepping
  numberOfFunctions=0;
  currentFn=0;
  fn=NULL;
  cgfn=NULL;
  
  runTimeDialog=NULL;
  movieFrame=-1;
  plotOptions=0;
  plotChoices=2; // plot contours by default
  totalNumberOfArrays=0;
  
  useTwilightZone=false;
  twilightZoneOption=polynomialTwilightZone;
  tz=NULL;

  if( !dbase.has_key("extrapolateInterpolationNeighbours") ) 
    dbase.put<int>("extrapolateInterpolationNeighbours"); 
  dbase.get<int>("extrapolateInterpolationNeighbours")=false;
  
  if( !dbase.has_key("orderOfExtrapolationForInterpolationNeighbours") ) 
    dbase.put<int>("orderOfExtrapolationForInterpolationNeighbours"); 
  dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours")=-1; // -1 : use default

  dbase.put<aString>("knownSolutionName")="noKnownSolution";
  dbase.put<bool>("knownSolutionIsTimeDependent")=true;

  // Time history of the forcing is stored here (when needed)
  //    forcingArray[numberOfForcingFunctions] 
  //    forcingArray[fCurrent]  : current forcing
  dbase.put<bool>("useNewForcingMethod")=false;  // set to true to use new way for forcing
  dbase.put<realArray*>("forcingArray")=NULL;
  dbase.put<int>("numberOfForcingFunctions")=0;  // number of elements in forcingArray
  dbase.put<int>("fCurrent")=0;                  // forcingArray[fCurrent] : current forcing

  timing.redim(maximumNumberOfTimings);
  timing=0.;
  for( int i=0; i<maximumNumberOfTimings; i++ )
    timingName[i]="";

  // only name the things that will be really timed in this run
  timingName[totalTime]                          ="total time";
  timingName[timeForInitialize]                  ="setup and initialize";
  timingName[timeForDSIMatrix]                   ="  dsi matrix construction";
  timingName[timeForInitialConditions]           ="initial conditions";
  timingName[timeForAdvance]                     ="advance";
  timingName[timeForAdvanceRectangularGrids]     ="  advance rectangular grids";
  timingName[timeForAdvanceCurvilinearGrids]     ="  advance curvilinear grids";
  timingName[timeForAdvanceUnstructuredGrids]    ="  advance unstructured grids";
  timingName[timeForAdvOpt]                      ="   (advOpt)";
  timingName[timeForForcing]                     ="  add forcing";
  timingName[timeForProject]                     ="  project    ";
  timingName[timeForDissipation]                 ="  add dissipation";
  timingName[timeForBoundaryConditions]          ="  boundary conditions";
  timingName[timeForInterfaceBC]                 ="  interface bc";
  timingName[timeForRadiationBC]                 ="  radiation bc";
  timingName[timeForRaditionKernel]              ="  radiationKernel";
  timingName[timeForUpdateGhostBoundaries]       ="  update ghost (parallel)";
  timingName[timeForInterpolate]                 ="  interpolation";
  timingName[timeForIntensity]                   ="compute intensity";
  timingName[timeForComputingDeltaT]             ="compute dt";
  timingName[timeForGetError]                    ="get errors";
  timingName[timeForPlotting]                    ="plotting";
  timingName[timeForShowFile]                    ="showFile";
  timingName[timeForWaiting]                     ="waiting (not counted)";
  logFile   = fopen("mx.log","w" );        // Here is the log file

  probeFileName="probeFile.dat";
  probeFile = NULL;

  sizeOfLocalArraysForAdvance=0.;
}

Maxwell::
~Maxwell()
{
  #ifndef USE_PPP
  // fix me for parallel -- adding this causes the program to hang when closing the show file.
    saveSequencesToShowFile();
  #endif

  if( show!=NULL )
  { // 090201
    show->endFrame();  
  }
  delete show;
  delete referenceShowFileReader;

  // assert( cgp!=NULL );
  // CompositeGrid & cg= *cgp;
  // const int numberOfComponentGrids=cg.numberOfComponentGrids();
  // for( int m=0; m<numberOfFunctions*numberOfComponentGrids; m++ )
  // {
  //   printF(" Workspace: fn[%i] elementCount=%i\n",m,fn[m].elementCount());
  // }
  
  delete [] fn;  

  if ( mgp!=NULL )
  {
    delete [] fields;
    delete dissipation;
    delete [] errp;
  }
  else
  {
    delete [] cgfields;
    delete cgdissipation;
    delete e_cgdissipation;
    delete [] cgerrp;
    delete [] cgfn;
  }

  delete variableDissipation;

  delete mgp;
  delete runTimeDialog;
  delete op;

  delete vpml;

  delete cgp;
  delete cgop;
  if( pinterpolant!=NULL && pinterpolant->decrementReferenceCount()==0 )
    delete pinterpolant;

  delete knownSolution;
  
  delete tz;

  delete [] radiationBoundaryCondition;
  
  delete pRho;
  delete poisson;
  delete pPhi;
  delete pF;

  delete pIntensity; // 110609
  delete pHarmonicElectricField;

  delete [] dbase.get<realArray*>("forcingArray");  // 2015/05/18

  userDefinedForcingCleanup();
  userDefinedInitialConditionsCleanup();
  
  fclose(debugFile);
  if( Communication_Manager::numberOfProcessors()>1 )
    fclose(pDebugFile);

  fclose(logFile);
  fclose(checkFile);
  if( probeFile!=NULL )
    fclose(probeFile);
}

// =====================================================================================
/// \brief Return true if the equations are forced (external forcing)
// =====================================================================================
bool Maxwell::
forcingIsOn() const
{
  if( forcingOption==twilightZoneForcing || 
      forcingOption==gaussianSource ||
      forcingOption==magneticSinusoidalPointSource ||
      forcingOption==gaussianChargeSource||
      forcingOption==userDefinedForcingOption )
  {
    return true;
  }
  else
  {
    return false;
  }
  
}

// =====================================================================================
/// \brief Return true if we should construct the array of grid vertices (THEvertex)
/// \param grid (input) : check this grid.
// =====================================================================================
bool Maxwell::
vertexArrayIsNeeded( int grid ) const
{

  // from assignBC: 
  const int useForcing = forcingOption==twilightZoneForcing;
  const bool centerNeeded=(useForcing || 
                           forcingOption==planeWaveBoundaryForcing ||  // **************** fix this 
                           initialConditionOption==gaussianPlaneWave || 
                           (initialConditionOption==planeWaveInitialCondition 
			      && method!=nfdtd  && method!=sosup  ) ||  // for ABC + incident field fix 
                           initialConditionOption==planeMaterialInterfaceInitialCondition ||
                           initialConditionOption==annulusEigenfunctionInitialCondition  ||
                           method==yee || 
                           method==dsi );
  if( centerNeeded )
    return true;

  // -- now check for cases that depend on whether the grid is rectangular (i.e. Cartesian)
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const bool isRectangular = cg[grid].isRectangular();

  // from forcing: 
  const bool buildCenter = !( isRectangular &&
			      ( initialConditionOption==squareEigenfunctionInitialCondition ||
				initialConditionOption==gaussianPulseInitialCondition ||
				(forcingOption==gaussianChargeSource && initialConditionOption==defaultInitialCondition)
				|| initialConditionOption==userDefinedKnownSolutionInitialCondition 
				|| initialConditionOption==userDefinedInitialConditionsOption
                                || initialConditionOption==planeWaveInitialCondition 
				// || initialConditionOption==planeMaterialInterfaceInitialCondition
				// ||  initialConditionOption==annulusEigenfunctionInitialCondition
				) 
    ); // fix this 

  return centerNeeded || buildCenter;
}


// ===================================================================================
// /Description:
//    Return true if any grid has PML boundary conditions
// ===================================================================================
bool Maxwell::
usingPMLBoundaryConditions() const
{
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & bc = cg[grid].boundaryCondition();
    if( bc(0,0)==abcPML || bc(1,0)==abcPML ||
	bc(0,1)==abcPML || bc(1,1)==abcPML ||
	bc(0,2)==abcPML || bc(1,2)==abcPML )
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
int Maxwell::
initializeRadiationBoundaryConditions()
{
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  
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
	  if( bc(side,axis)==rbcNonLocal )
	  {
            radbcGrid[numberOfNonLocal]=grid;
	    radbcSide[numberOfNonLocal]=side;
	    radbcAxis[numberOfNonLocal]=axis;
	    
	    numberOfNonLocal++;
            if( numberOfNonLocal>2 )
	    {
	      printF("Maxwell::initializeRadiationBoundaryConditions:ERROR: there are too many sides with\n"
                     "  radiation boundary conditions -- there should be at most 2\n");
	      Overture::abort("error");
	    }
	  }
	}
    }
  }
  if( numberOfNonLocal>0 )
  {
    assert( radiationBoundaryCondition==NULL );
    radiationBoundaryCondition = new RadiationBoundaryCondition [numberOfNonLocal];
    
    for( int i=0; i<numberOfNonLocal; i++ )
    {
      radiationBoundaryCondition[i].setOrderOfAccuracy(orderOfAccuracyInSpace);
      
      int nc1=0, nc2=0;      // component range
      if( cg.numberOfDimensions()==2 )
      {
	nc1=ex; nc2=hz;
      }
      else
      {
	nc1=ex; nc2=ez;
      }
      MappedGrid & mg = cg[radbcGrid[i]];
      radiationBoundaryCondition[i].initialize(mg,radbcSide[i],radbcAxis[i],nc1,nc2,c);
    }
    
  }
  
  return 0;
}


void Maxwell::
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

//=============================================================================================
/// /brief: print the memory usage that cgmx thinks that it is using 
//=============================================================================================
int Maxwell::
printMemoryUsage(FILE *file /* = stdout */)
{
  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;

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
    memoryForDSIArrays,
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
    "DSI Arrays",
    "total (of above items)"
  };

  real memory[numberOfMemoryItems]={0.,0.,0.,0.,0.,0.,0.};
  memory[memoryForCompositeGrid]=cg.sizeOf();
  
  memory[memoryForOperators]= cgop!=NULL ? cgop->sizeOf() : op!=NULL ? op->sizeOf() : 0.;

  if( cg.rcData->interpolant!=NULL )
  {
    memory[memoryForInterpolant]=cg.rcData->interpolant->sizeOf();
  }

  int debugs=1;

  memory[memoryForGridFunctions]=0.;
  int i;
  real cgfieldsSize=0., fieldsSize=0., fnSize=0., cgdissSize=0., errSize=0.;
  
  if( cgfields!=NULL )
  {
    for( int i=0; i<numberOfTimeLevels; i++ )
      memory[memoryForGridFunctions]+=cgfields[i].sizeOf();
    cgfieldsSize=memory[memoryForGridFunctions];
  }
  else if ( dsi_cgfieldsH !=NULL && dsi_cgfieldsE0!= NULL )
  {
    for( int i=0; i<numberOfTimeLevels; i++ )
    {
      memory[memoryForGridFunctions]+=dsi_cgfieldsH[i].sizeOf();
      memory[memoryForGridFunctions]+=dsi_cgfieldsE0[i].sizeOf();
    }
    cgfieldsSize=memory[memoryForGridFunctions];
  }
  else
  {
    for( int i=0; i<numberOfTimeLevels; i++ )
      memory[memoryForGridFunctions]+=fields[i].sizeOf();

    if ( Ecoeff.size() )
      for( int i=0; i<numberOfTimeLevels; i++ )
	memory[memoryForGridFunctions]+=fields[i+1].sizeOf();
      
    fieldsSize=memory[memoryForGridFunctions];
  }
  
  int & numberOfForcingFunctions= dbase.get<int>("numberOfForcingFunctions"); // number of elements in forcingArray
  if( numberOfForcingFunctions>0 )
  {
    const int & fCurrent = dbase.get<int>("fCurrent");          // forcingArray[fCurrent] : current forcing
    realArray *& forcingArray = dbase.get<realArray*>("forcingArray");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      memory[memoryForGridFunctions]+= forcingArray[grid].elementCount()*sizeof(real);
    }
  }
  

  if ( Ecoeff.size() )
  {
    memory[memoryForDSIArrays] += sizeof(real)*( Ecoeff.size() + Hcoeff.size() +
						 Dcoeff.size() );

    memory[memoryForDSIArrays] += sizeof(int)*( Eindex.size() + Eoffset.size() +
						Hindex.size() + Hoffset.size() +
						Dindex.size() + Doffset.size() );

    memory[memoryForDSIArrays] += 5*sizeof(ArraySimple<real>);
    memory[memoryForDSIArrays] += 10*sizeof(ArraySimple<int>);
      
    memory[memoryForDSIArrays] += faceAreaNormals.elementCount()*sizeof(real);
    memory[memoryForDSIArrays] += edgeAreaNormals.elementCount()*sizeof(real);

    if ( REcoeff.size() )
    {
      memory[memoryForDSIArrays] += REcoeff.size()*sizeof(ArraySimple<real>) + sizeof(ArraySimple<ArraySimple<real> >);
      memory[memoryForDSIArrays] += REindex.size()*sizeof(ArraySimple<int>) + sizeof(ArraySimple<ArraySimple<int> >);
      memory[memoryForDSIArrays] += SEindex.size()*sizeof(ArraySimple<int>) + sizeof(ArraySimple<ArraySimple<int> >);
      for ( int i=0; i<REcoeff.size(); i++ )
	memory[memoryForDSIArrays] += REcoeff[i].size()*sizeof(real) + REindex[i].size()*sizeof(int);
      for ( int i=0; i<SEindex.size(); i++ )
	memory[memoryForDSIArrays] += SEindex[i].size()*sizeof(int);


      if ( RHcoeff.size() )
      {
	memory[memoryForDSIArrays] += RHcoeff.size()*sizeof(ArraySimple<real>) + sizeof(ArraySimple<ArraySimple<real> >);
	memory[memoryForDSIArrays] += RHindex.size()*sizeof(ArraySimple<int>) + sizeof(ArraySimple<ArraySimple<int> >);
	memory[memoryForDSIArrays] += SHindex.size()*sizeof(ArraySimple<int>) + sizeof(ArraySimple<ArraySimple<int> >);
	for ( int i=0; i<RHcoeff.size(); i++ )
	  memory[memoryForDSIArrays] += RHcoeff[i].size()*sizeof(real) + RHindex[i].size()*sizeof(int);
	for ( int i=0; i<SHindex.size(); i++ )
	  memory[memoryForDSIArrays] += SHindex[i].size()*sizeof(int);
      }
    }

  }

#define FN(m,grid) fn[m+numberOfFunctions*(grid)]
  fnSize=-memory[memoryForGridFunctions];
  for( i=0; i<numberOfFunctions; i++ )
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      memory[memoryForGridFunctions]+=FN(i,grid).elementCount()*sizeof(real);
  fnSize+=memory[memoryForGridFunctions];
#undef FN

  if( cgdissipation!=NULL )
  {
    cgdissSize=-memory[memoryForGridFunctions];
    memory[memoryForGridFunctions]+=cgdissipation->sizeOf();  
    cgdissSize+=memory[memoryForGridFunctions];
  }

  if( e_cgdissipation!=NULL )
  {
    cgdissSize=-memory[memoryForGridFunctions];
    memory[memoryForGridFunctions]+=e_cgdissipation->sizeOf();  
    cgdissSize+=memory[memoryForGridFunctions];
  }
  
  if( cgerrp!=NULL )
  {
    if ( method==nfdtd || method==yee || method==sosup )
    {
      errSize=-memory[memoryForGridFunctions];
      memory[memoryForGridFunctions]+=cgerrp->sizeOf();  
      errSize+=memory[memoryForGridFunctions];
    }
    else if( method==dsi || method==dsiNew || method==dsiMatVec )
    {
      errSize=-memory[memoryForGridFunctions];
      memory[memoryForGridFunctions]+=cgerrp[0].sizeOf()+cgerrp[1].sizeOf();  
      errSize+=memory[memoryForGridFunctions];
    }
    else
    {
      printF("Maxwell::printMemoryUsage:ERROR: unknown method=%i\n",(int)method);
      OV_ABORT("error");
    }
    
  }
  
  if( pRho!=NULL )
    memory[memoryForGridFunctions]+=pRho->sizeOf();
  if( pPhi!=NULL )
    memory[memoryForGridFunctions]+=pPhi->sizeOf();
  if( pF!=NULL )
    memory[memoryForGridFunctions]+=pF->sizeOf();
  
  // memory for local arrays -- arrays that are allocated and de-allocated locally
  memory[memoryForLocalArrays]=0.;
  memory[memoryForLocalArrays]+=sizeOfLocalArraysForAdvance;

  const real megaByte=1024.*1024;
  if( debugs ) fPrintF(file," size of: cgfields=%9.2f, fields=%9.2f, fn=%9.2f, cgdiss=%9.2f, err=%9.2f, dsi=%9.2f (MB)\n",
		      cgfieldsSize/megaByte,fieldsSize/megaByte,fnSize/megaByte,cgdissSize/megaByte,errSize/megaByte,memory[memoryForDSIArrays]/megaByte);

  memory[memoryTotal]=0.;
  for( i=0; i<numberOfMemoryItems-1; i++ )
    memory[memoryTotal]+=memory[i];

  const int numberOfComponents=cgfields!=NULL ? cgfields[0][0].getLength(3) : (dsi_cgfieldsH ? 
									       dsi_cgfieldsH[0][0].getLength(3)+
									       dsi_cgfieldsE0[0][0].getLength(3) : 
									       fields[0].getLength(3));

  if( Communication_Manager::My_Process_Number<=0 )
  {
    for( i=0; i<numberOfMemoryItems; i++ )
    {
      fPrintF(file," %s%s%9.2f  %9.2f    %9.2f       %5.1f%%  \n",
	      (const char*)memoryName[i],(const char*)dots(0,max(0,nSpace-memoryName[i].length())),
	      memory[i]/megaByte,memory[i]/sizeof(real)/numberOfGridPoints,
	      memory[i]/sizeof(real)/numberOfGridPoints/numberOfComponents, 100.*memory[i]/memory[memoryTotal]);
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

int Maxwell::
printStatistics(FILE *file /* = stdout */)
//===================================================================================
// /Description:
// Output timing statistics
//
//\end{OverBlownInclude.tex}  
//===================================================================================
{
  fflush(0);
  Communication_Manager::Sync();
  const int np= max(1,Communication_Manager::numberOfProcessors());

  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;

  // count the total number of grid points
  numberOfGridPoints=0.;
  int numberOfInterpolationPoints=0;
  int grid;
  if( method==nfdtd || method==yee || method==sosup )
  {
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      numberOfGridPoints+=cg[grid].mask().elementCount();
    }
  }
  else
  {
    numberOfGridPoints = Eoffset.size() + Hoffset.size() - 2;
  }

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    numberOfInterpolationPoints+=cg.numberOfInterpolationPoints(grid);

  // output timings from Interpolant.
  if( cg.numberOfComponentGrids()>1  && pinterpolant!=NULL )
    pinterpolant->printMyStatistics(logFile);
  printMemoryUsage(logFile);
  // printMemoryUsage(file);

  GenericMappedGridOperators::printBoundaryConditionStatistics(logFile);
  
  if( poisson!=NULL )
    poisson->printStatistics(logFile);

  timing(timeForRadiationBC)=RadiationBoundaryCondition::cpuTime; 
  timing(timeForRaditionKernel)=RadiationKernel::cpuTime;

  int i;
  for( i=0; i<maximumNumberOfTimings; i++ )
    timing(i) =getMaxValue(timing(i),0);  // get max over processors -- results only go to processor=0

  // adjust times for waiting
  real timeWaiting=timing(timeForWaiting);
  timing(timeForPlotting)-=timeWaiting;
  timing(totalTime)-=timeWaiting;

  timing(totalTime)=max(timing(totalTime),REAL_MIN*10.);

  // Get max/ave times 
  RealArray maxTiming(timing.dimension(0)),minTiming(timing.dimension(0)),aveTiming(timing.dimension(0));

//   for( int i=0; i<Parameters::maximumNumberOfTimings; i++ )
//     maxTiming(i)=ParallelUtility::getMaxValue(timing(i));   // max over all processors  -- is this the right thing to do?
  
  ParallelUtility::getMaxValues(&timing(0),&maxTiming(0),maximumNumberOfTimings);
  ParallelUtility::getMinValues(&timing(0),&minTiming(0),maximumNumberOfTimings);
  ParallelUtility::getSums(&timing(0),&aveTiming(0),maximumNumberOfTimings);
  aveTiming/=np;

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors
  real totalMem=ParallelUtility::getSum(mem);  // min over all processors
  real aveMem=totalMem/np;
  real maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

  const real realsPerGridPoint = (totalMem*1024.*1024.)/numberOfGridPoints/sizeof(real);

  // Get the current date
  time_t *tp= new time_t;
  time(tp);
  // tm *ptm=localtime(tp);
  const char *dateString = ctime(tp);

  for( int fileio=0; fileio<2; fileio++ )
  {
    FILE *output = fileio==0 ? logFile : file;

    fPrintF(output,"\n"
            "              ---------Maxwell Summary------- \n"
            "                       %s" 
            "               Grid:   %s \n" 
	    "  ==== numberOfStepsTaken =%9i, grids=%i, gridpts =%g, interp pts=%i, processors=%i ==== \n"
	    "  ==== memory per-proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), total=%g (Mb)\n"
	    "   Timings:         (ave-sec/proc:)   seconds    sec/step   sec/step/pt     %%     [max-s/proc] [min-s/proc]\n",
	    dateString,(const char*)nameOfGridFile,
            numberOfStepsTaken,cg.numberOfComponentGrids(),numberOfGridPoints,numberOfInterpolationPoints,
            np,minMem,aveMem,maxMem,maxMemRecorded,totalMem);
    
  
    int nSpace=35;
    aString dots="........................................................................";
    if( timing(0)==0. )
      timing(0)=REAL_MIN;
    for( i=0; i<maximumNumberOfTimings; i++ )
    {
      if( timingName[i]!="" && timing(i)>0. )    
	fPrintF(output,"%s%s%10.2e  %10.2e  %10.2e   %7.3f  %10.3e  %10.3e\n",(const char*)timingName[i],
		(const char*)dots(0,max(0,nSpace-timingName[i].length())),
		aveTiming(i),aveTiming(i)/numberOfStepsTaken,aveTiming(i)/numberOfStepsTaken/numberOfGridPoints,
		100.*aveTiming(i)/aveTiming(0),maxTiming(i),minTiming(i));
      
    }

    fPrintF(output,"-----------------------------------------------------------------------------------------\n");
    fPrintF(output," Memory usage: reals/grid-point = %6.2f.\n",realsPerGridPoint);
    fPrintF(output,"-----------------------------------------------------------------------------------------\n");

    cg.displayDistribution("Maxwell",output);

    if( fileio==0 )
    {
      // Output results as a LaTeX table
      fPrintF(output,"\n\n%% ------------ Table for LaTeX -------------------------------\n");
      fPrintF(output,
	      "\\begin{table}[hbt]\n"
	      "\\begin{center}\\footnotesize\n"
	      "\\begin{tabular}{|l|r|r|r|r|} \\hline\n"
	      "  Timings:   &  seconds &    sec/step  &  sec/step/pt &  \\%%    \\\\ \\hline\n");
      for( i=0; i<maximumNumberOfTimings; i++ )
      {
        aString name=timingName[i];
	int len=name.length();
	for( int j=0; j<len; j++)
	{
	  if( name[j]==' ' ) name[j]='~';   // replace indentation space with ~
	}
	if( timingName[i]!="" && timing(i)>0. )    
	  fPrintF(output,"%s\\dotfill & %10.2e & %10.2e & %10.2e & %7.3f \\\\ \n",(const char*)name,
		  timing(i),timing(i)/numberOfStepsTaken,timing(i)/numberOfStepsTaken/numberOfGridPoints,
		  100.*timing(i)/timing(0));
      
      }     
      fPrintF(output,
              " \\hline \n"
              "\\end{tabular}\n"
              "\\end{center}\n"
              "\\caption{grid=%s, %g grid points, %i interp points, %i steps taken, %i processors.}\n"
              "\\label{tab:%s}\n"
              "\\end{table}\n", (const char*)nameOfGridFile,numberOfGridPoints,numberOfInterpolationPoints,
	      numberOfStepsTaken,Communication_Manager::numberOfProcessors(),(const char*)nameOfGridFile );
    }
    
    
  }

  delete tp;

  if( np>1 )
  {
    // In parallel we print some timings for each processor
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : file;
      fflush(output);
      fPrintF(output,"\n"
	      " ------- Summary: Timings per processor -----------\n"
	      "   p   ");
      for( int i=0; i<maximumNumberOfTimings; i++ )
      { // output a short-form name (7 chars)
	if( timingName[i]!="" && maxTiming(i)!=0. )  
	{
          aString shortName="       ";
          int m=0;
	  for( int s=0; m<7 && s<timingName[i].length(); s++ ) 
	  { // strip off blanks
	    if( timingName[i][s]!=' ' ) {shortName[m]=timingName[i][s]; m++;} //
	  }
          fPrintF(output,"%7.7s ",(const char*)shortName);
	}
      }
      fPrintF(output,"\n");
      fflush(output);
      RealArray timingLocal(timing.dimension(0));
      for( int p=0; p<np; p++ )
      {
	// Note -- it did not work very well to have processor p try to write results, so instead
        // we copy results to processor 0 to print 
        timingLocal=timing;
        broadCast(timingLocal,p);  // send timing info from processor p   -- don't need a broad cast here **fix**
	fPrintF(output,"%4i : ",p);
	for( int i=0; i<maximumNumberOfTimings; i++ )
	{
	  if( timingName[i]!="" && maxTiming(i)!=0. )    
	    fPrintF(output,"%7.1e ",timingLocal(i));
	}
	fflush(output);
	fPrintF(output,"\n");
      }
      fPrintF(output,"\n");
      fflush(output);
    }
  
  }

  printF("\n >>>> See the file mx.log for further timings, memory usage and other statistics <<<< \n\n");
  
  
  // reset times
  timing(timeForPlotting)+=timeWaiting;
  timing(totalTime)+=timeWaiting;

  return 0;
}


int Maxwell::
buildTimeSteppingOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the time stepping options dialog.
// ==========================================================================================
{
  dialog.setOptionMenuColumns(1);

  aString methodCommands[] = {"default", "Yee", "DSI", "new DSI", "DSI-MatVec", "NFDTD", "SOSUP", "" };
  dialog.addOptionMenu("method:", methodCommands, methodCommands, (int)method );

  aString timeSteppingMethodCommands[] = {"defaultTimeStepping", 
					  "adamsBashforthSymmetricThirdOrder",
					  "rungeKuttaFourthOrder",
					  "stoermerTimeStepping",
					  "modifiedEquationTimeStepping",
					  "" };

  dialog.addOptionMenu("time stepping:", timeSteppingMethodCommands, timeSteppingMethodCommands, 
                       (int)timeSteppingMethod );

  aString pushButtonCommands[] = {"projection solver parameters...",
				  ""};
  int numRows=1;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  aString tbCommands[] = {"use conservative difference",
                          "solve for electric field",
                          "solve for magnetic field",
                          "use variable dissipation",
                          "project fields",
                          "use charge density",
                          "use conservative divergence",
                          "project initial conditions",
                          "use new interface routines",
                          "apply filter",
                          "use divergence cleaning",
                          "project interpolation points",
                          "use new forcing method",
 			  ""};
  int tbState[15];
  tbState[0] = useConservative;
  tbState[1] = solveForElectricField;
  tbState[2] = solveForMagneticField;
  tbState[3] = useVariableDissipation;
  tbState[4] = projectFields;
  tbState[5] = useChargeDensity;
  tbState[6] = useConservativeDivergence;
  tbState[7] = projectInitialConditions;
  tbState[8] = useNewInterfaceRoutines; 
  tbState[9] = applyFilter;
  tbState[10]= useDivergenceCleaning;
  tbState[11]= projectInterpolation;
  tbState[12]= dbase.get<bool>("useNewForcingMethod");

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 

  // ----- Text strings ------
  const int numberOfTextStrings=50;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
  textCommands[nt] = "nx,ny";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i %i",nx[0],nx[1]);  nt++; 

  textCommands[nt] = "cfl";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%g",cfl);  nt++; 

   textCommands[nt] = "dissipation";  textLabels[nt]=textCommands[nt];
   sPrintF(textStrings[nt], "%g",artificialDissipation);  nt++; 

   textCommands[nt] = "dissipation (curvilinear)";  textLabels[nt]=textCommands[nt];
   sPrintF(textStrings[nt], "%g",artificialDissipationCurvilinear);  nt++; 

   textCommands[nt] = "order of dissipation";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfArtificialDissipation); nt++; 

   textCommands[nt] = "filter order";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",orderOfFilter); nt++; 

   textCommands[nt] = "filter frequency";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",filterFrequency); nt++; 

   textCommands[nt] = "filter iterations";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfFilterIterations); nt++; 

   textCommands[nt] = "filter coefficient";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",filterCoefficient); nt++; 

   // for unstructured grids: 
   textCommands[nt] = "dissipation interval";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",artificialDissipationInterval); nt++; 

   textCommands[nt] = "number of variable dissipation smooths";
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfVariableDissipationSmooths); nt++; 

   textCommands[nt] = "divergence damping";  textLabels[nt]=textCommands[nt];
   sPrintF(textStrings[nt], "%g",divergenceDamping);  nt++; 

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
  
   textCommands[nt] = "interface equations option";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",interfaceEquationsOption); nt++; 
  
   textCommands[nt] = "projection frequency";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",frequencyToProjectFields); nt++; 

   textCommands[nt] = "consecutive projection steps";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfConsecutiveStepsToProject); nt++; 

   textCommands[nt] = "initial projection steps";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfInitialProjectionSteps); nt++; 

   textCommands[nt] = "number of divergence smooths";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",numberOfDivergenceSmooths); nt++; 

   textCommands[nt] = "div cleaning coefficient";  
   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",divergenceCleaningCoefficient); nt++; 

  
  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


int Maxwell::
buildForcingOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the forcing options dialog.
// ==========================================================================================
{

  // ************** PUSH BUTTONS *****************
  aString pushButtonCommands[] = {"set pml error checking offset",
                                  "define embedded bodies",
				  ""};
  int numRows=2;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  dialog.setOptionMenuColumns(1);

  aString initialConditionOptionCommands[] = {"defaultInitialCondition", 
					      "planeWaveInitialCondition",
					      "gaussianPlaneWave",
                                              "gaussianPulseInitialCondition",
					      "squareEigenfunctionInitialCondition",  
					      "annulusEigenfunctionInitialCondition",
                                              "zeroInitialCondition",
                                              "planeWaveScatteredFieldInitialCondition",
                                              "planeMaterialInterfaceInitialCondition",
                                              "gaussianIntegralInitialCondition",
                                              "twilightZoneInitialCondition",
                                              "userDefinedInitialConditions",
                                              "userDefinedKnownSolutionInitialCondition",
					      "" };

  dialog.addOptionMenu("initial conditions:", initialConditionOptionCommands, initialConditionOptionCommands, 
                            (int)initialConditionOption );

  aString forcingOptionCommands[] = {"noForcing", 
                                     "magneticSinusoidalPointSource",
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

  dialog.addOptionMenu("TZ option:", twilightZoneOptionCommands, twilightZoneOptionCommands, 
                       (int)twilightZoneOption );


  aString knownSolutionOptionCommands[] = {"noKnownSolution",
                                           "twilightZoneKnownSolution",
					   "planeWaveKnownSolution",
                                           "gaussianPlaneWaveKnownSolution",
                                           "gaussianIntegralKnownSolution",
                                           "planeMaterialInterfaceKnownSolution",
					   "scatteringFromADiskKnownSolution",
					   "scatteringFromADielectricDiskKnownSolution",
					   "scatteringFromASphereKnownSolution",
                                           "scatteringFromADielectricSphereKnownSolution",
					   "squareEigenfunctionKnownSolution",
					   "annulusEigenfunctionKnownSolution",
					   "eigenfunctionsOfACylinderKnownSolution",
					   "eigenfunctionsOfASphereKnownSolution ",   // not implemented yet 
                                           "user defined known solution",
					   "" };

  dialog.addOptionMenu("known solution:", knownSolutionOptionCommands, knownSolutionOptionCommands, 
                            (int)knownSolutionOption );

  aString tbCommands[] = {"use twilightZone materials",
 			  ""};
  int tbState[10];
  tbState[0] = useTwilightZoneMaterials;
  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 


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

  textCommands[nt] = "kx,ky,kz";  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g,%g,%g",kx,ky,kz);  nt++; 
  textCommands[nt] = "plane wave coefficients";  textLabels[nt]=textCommands[nt]; 
  const real epsPW=0., muPW=0.;  // eps and mu that define the plane wave
  sPrintF(textStrings[nt], "%g,%g,%g, %g,%g",pwc[0],pwc[1],pwc[2],epsPW,muPW);  nt++; 

  textCommands[nt] = "frequency";  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",frequency); nt++; 

  textCommands[nt] = "slow start interval";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",slowStartInterval); nt++; 

  textCommands[nt] = "Gaussian plane wave:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g %g %g %g (beta,x0,y0,z0)",
                   betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave); nt++; 

  textCommands[nt] = "Gaussian source:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g %g (beta,omega,x0,y0,z0)",
	  gaussianSourceParameters[0],gaussianSourceParameters[1],
          gaussianSourceParameters[2],gaussianSourceParameters[3],gaussianSourceParameters[4]); nt++;

  textCommands[nt] = "Gaussian pulse:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g %g %g (beta,scale,exponent,x0,y0,z0)",
	  gaussianPulseParameters[0][0],gaussianPulseParameters[0][1],gaussianPulseParameters[0][2],
          gaussianPulseParameters[0][3],gaussianPulseParameters[0][4],gaussianPulseParameters[0][5]); nt++;

  textCommands[nt] = "Gaussian charge source:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g %g %g %g %g %g (amp,beta,p,x0,y0,z0,v0,v1,v2)",
	  gaussianChargeSourceParameters[0][0],gaussianChargeSourceParameters[0][1],
          gaussianChargeSourceParameters[0][2],gaussianChargeSourceParameters[0][3],
          gaussianChargeSourceParameters[0][4],gaussianChargeSourceParameters[0][5],
          gaussianChargeSourceParameters[0][6],gaussianChargeSourceParameters[0][7],
          gaussianChargeSourceParameters[0][8]); nt++;

  textCommands[nt] = "TZ omega:";
  textLabels[nt]=textCommands[nt]; 
  sPrintF(textStrings[nt], "%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]); nt++;


  textCommands[nt] = "degreeSpace, degreeTime";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i, %i",degreeSpace,degreeTime); nt++; 

  textCommands[nt] = "degreeSpaceX, degreeSpaceY, degreeSpaceZ";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i, %i, %i",
                                           degreeSpaceX, degreeSpaceY, degreeSpaceZ); nt++; 

  textCommands[nt] = "material interface normal";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%5.3f, %5.3f, %5.3f",
                                           normalPlaneMaterialInterface[0], normalPlaneMaterialInterface[1],
                                           normalPlaneMaterialInterface[2]); nt++; 
  textCommands[nt] = "material interface point";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%5.3f, %5.3f, %5.3f",
                                           x0PlaneMaterialInterface[0], x0PlaneMaterialInterface[1],
                                           x0PlaneMaterialInterface[2]); nt++; 

  RealArray & icBox = initialConditionBoundingBox;
  textCommands[nt] = "initial condition bounding box";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%8.2e,%8.2e, %8.2e,%8.2e, %8.2e,%8.2e (xa,xb,ya,yb...)",
                                           icBox(0,0),icBox(1,0),
                                           icBox(0,1),icBox(1,1),
                                           icBox(0,2),icBox(1,2)); nt++; 

  textCommands[nt] = "bounding box decay exponent";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%8.2e",boundingBoxDecayExponent); nt++;


  textCommands[nt] = "adjust boundaries for incident field";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "[0|1] [gridName|all]"); nt++; 

  textCommands[nt] = "scattering radius";  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",
           dbase.get<real>("scatteringRadius")); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}


int Maxwell::
buildPlotOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the plot options dialog.
// ==========================================================================================
{

  // ************** PUSH BUTTONS *****************
  dialog.setOptionMenuColumns(1);

  aString tbCommands[] = {"plot errors",
                          "plot divergence",
                          "plot dissipation",
                          "plot scattered field",
                          "plot total field",
                          "plot energy density",
                          "plot intensity",
                          "plot harmomic E field",
                          "check errors",
                          "compare to show file",
                          "compute energy",
                          "plot rho",
			  "plot dsi vertex max",
 			  ""};
  int tbState[15];
  tbState[0] = plotErrors;
  tbState[1] = plotDivergence;
  tbState[2] = plotDissipation;
  tbState[3] = plotScatteredField;
  tbState[4] = plotTotalField;
  tbState[5] = plotEnergyDensity;
  tbState[6] = plotIntensity;
  tbState[7] = plotHarmonicElectricFieldComponents;
  tbState[7] = checkErrors;
  tbState[8] = compareToReferenceShowFile;
  tbState[9] = computeEnergy;
  tbState[10]= plotRho;
  tbState[11]= plotDSIMaxVertVals;

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns); 



  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "radius for checking errors";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f",radiusForCheckingErrors); nt++; 

  textCommands[nt] = "pml error offset";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",pmlErrorOffset); nt++; 

  textCommands[nt] = "reference show file:";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)nameOfReferenceShowFile); nt++; 

  textCommands[nt] = "intensity option";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",intensityOption); nt++; 

  textCommands[nt] = "intensity averaging interval";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f (in periods))",intensityAveragingInterval); nt++; 

  // We no longer need to specify this: 
//   textCommands[nt] = "time harmonic omega";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%f (omega/(2pi), normally c*|k|)",omegaTimeHarmonic); nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

int Maxwell::
buildInputOutputOptionsDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the input-output options dialog.
// ==========================================================================================
{

  aString gridTypeCommands[] = {"square", 
				"rotatedSquare", 
				"sineSquare",
				"skewedSquare", 
				"chevron", 
				"squareByTriangles", 
				"squareByQuads", 
				"sineByTriangles", 
				"annulus", 
				"box",
				"chevbox",
				"perturbedSquare",     //  square with random perturbations
				"perturbedBox",        //  box with random perturbations
				"compositeGrid", 
				"grid", 
				"" };
  dialog.addOptionMenu("grid:", gridTypeCommands, gridTypeCommands, (int)gridType );

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

  textCommands[nt] = "probe frequency";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",frequencyToSaveProbes); nt++; 

  textCommands[nt] = "probe file:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s",(const char*)probeFileName); nt++; 

  textCommands[nt] = "grid file:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%s","none"); nt++; 

  textCommands[nt] = "error norm:";
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",errorNorm); nt++; 

  const int np = Communication_Manager::numberOfProcessors();
  textLabels[nt] = "maximum number of parallel sub-files";  sPrintF(textStrings[nt], "%i",np);  nt++;

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}

int Maxwell::
buildPdeParametersDialog(DialogData & dialog )
// ==========================================================================================
// /Description:
//   Build the pde parameters dialog.
// ==========================================================================================
{

  // ************** PUSH BUTTONS *****************
  // aString pushButtonCommands[] = {"specify coefficients per grid",
  //			  ""};
  // int numRows=3;
  // dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 

  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

//   textCommands[nt] = "eps";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",eps); nt++; 

//   textCommands[nt] = "mu";  
//   textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g",mu); nt++; 

  textCommands[nt] = "coefficients";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%g %g %s (eps,mu,grid-name)",eps,mu,"all"); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  return 0;
}



// ====================================================================================================
// ====================================================================================================
int Maxwell::
interactiveUpdate(GL_GraphicsInterface &gi )
{
  gip=&gi;
  
  if( !gi.graphicsIsOn() && !gi.readingFromCommandFile() )
    return 0;

  // we should know the grid by the time we get here 
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();


  GUIState gui;

  DialogData & dialog=gui;

  dialog.setWindowTitle("Maxwell Equation Solver");
  dialog.setExitCommand("continue", "continue");

  dialog.setOptionMenuColumns(1);



  // ************** PUSH BUTTONS *****************
  aString pushButtonCommands[] = {"time stepping options...",
                                  "forcing options...",
                                  "plot options...",
                                  "input-output options...",
                                  "pde parameters...",
				  ""};
  int numRows=3;
  dialog.setPushButtons(pushButtonCommands,  pushButtonCommands, numRows ); 


  // ----- Text strings ------
  const int numberOfTextStrings=30;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;
//  textCommands[nt] = "nx,ny";  textLabels[nt]=textCommands[nt];
//  sPrintF(textStrings[nt], "%i %i",nx[0],nx[1]);  nt++; 

  textCommands[nt] = "tFinal";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%e",tFinal);  nt++; 
  textCommands[nt] = "tPlot";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%e",tPlot);  nt++; 


  textCommands[nt] = "debug";  
  textLabels[nt]=textCommands[nt]; sPrintF(textStrings[nt], "%i",debug); nt++; 


  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  dialog.setTextBoxes(textCommands, textLabels, textStrings);

  
  // --- Build the sibling dialog for time stepping options ---
  DialogData &timeSteppingOptionsDialog = gui.getDialogSibling();
  timeSteppingOptionsDialog.setWindowTitle("MX Time-stepping Options");
  timeSteppingOptionsDialog.setExitCommand("close time stepping options", "close");
  buildTimeSteppingOptionsDialog(timeSteppingOptionsDialog);

  // --- Build the sibling dialog for forcing options ---
  DialogData &forcingOptionsDialog = gui.getDialogSibling();
  forcingOptionsDialog.setWindowTitle("MX Forcing Options");
  forcingOptionsDialog.setExitCommand("close forcing options", "close");
  buildForcingOptionsDialog(forcingOptionsDialog);

  // --- Build the sibling dialog for plot options ---
  DialogData &plotOptionsDialog = gui.getDialogSibling();
  plotOptionsDialog.setWindowTitle("MX Plot Options");
  plotOptionsDialog.setExitCommand("close plot options", "close");
  buildPlotOptionsDialog(plotOptionsDialog);

  // --- Build the sibling dialog for inputOutput options ---
  DialogData &inputOutputOptionsDialog = gui.getDialogSibling();
  inputOutputOptionsDialog.setWindowTitle("MX InputOutput Options");
  inputOutputOptionsDialog.setExitCommand("close input-output options", "close");
  buildInputOutputOptionsDialog(inputOutputOptionsDialog);

  // --- Build the sibling dialog for pdeParameters options ---
  DialogData &pdeParametersDialog = gui.getDialogSibling();
  pdeParametersDialog.setWindowTitle("MX PDE Parameters Options");
  pdeParametersDialog.setExitCommand("close pde parameters", "close");
  buildPdeParametersDialog(pdeParametersDialog);


  IntegerArray originalBoundaryCondition(2,3,cg.numberOfComponentGrids());
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )  
      {
	originalBoundaryCondition(side,axis,grid)=cg[grid].boundaryCondition(side,axis);
      }
    }
  }
  real epsPW=0.,muPW=0.;
  
  gi.pushGUI(gui);
  aString answer,line;
  int len=0;
  for(;;) 
  {
    gi.getAnswer(answer,"");      
    // printF("Start: answer=[%s]\n",(const char*) answer);
    
    if( answer=="continue" || answer=="exit" )
    {
      break;
    }
    else if( answer=="time stepping options..." )
    {
      timeSteppingOptionsDialog.showSibling();
    }
    else if( answer=="close time stepping options" )
    {
      timeSteppingOptionsDialog.hideSibling();
    }
    else if( answer=="forcing options..." )
    {
      forcingOptionsDialog.showSibling();
    }
    else if( answer=="close forcing options" )
    {
      forcingOptionsDialog.hideSibling();
    }
    else if( answer=="plot options..." )
    {
      plotOptionsDialog.showSibling();
    }
    else if( answer=="close plot options" )
    {
      plotOptionsDialog.hideSibling();
    }
    else if( answer=="input-output options..." )
    {
      inputOutputOptionsDialog.showSibling();
    }
    else if( answer=="close input-output options" )
    {
      inputOutputOptionsDialog.hideSibling();
    }
    else if( answer=="pde parameters..." )
    {
      pdeParametersDialog.showSibling();
    }
    else if( answer=="close pde parameters" )
    {
      pdeParametersDialog.hideSibling();
    }
    else if( answer=="square" || answer=="rotatedSquare" || answer=="sineSquare"|| answer=="skewedSquare" || 
             answer=="chevron" || answer=="squareByTriangles" || answer=="squareByQuads" || answer=="sineByTriangles" ||
             answer=="annulus" || 
             answer=="compositeGrid" )
    {
      gridType=(answer=="square" ? square : answer=="rotatedSquare" ? rotatedSquare : 
		answer=="sineSquare" ? sineSquare : answer=="skewedSquare" ? skewedSquare : 
		answer=="chevron" ? chevron : answer=="squareByTriangles" ? squareByTriangles :
		answer=="squareByQuads" ? squareByQuads : answer=="sineByTriangles" ? sineByTriangles :
		answer=="annulus" ? annulus : answer=="chevbox" ? chevbox : answer=="grid" ? compositeGrid:
                compositeGrid);
      inputOutputOptionsDialog.getOptionMenu("grid:").setCurrentChoice((int)gridType);
    }
    else if( len=answer.matches("grid file: ") )
    {
      nameOfGridFile=answer(len,answer.length()-1);
      gridType=Maxwell::compositeGrid;
      printF(" Setting gridType=compositeGrid, file=%s\n",(const char*)nameOfGridFile);
      dialog.setTextLabel("grid file:",sPrintF(line,"%s",(const char*)nameOfGridFile));
    }
    else if( answer.matches("grid") )
    {
      gridType=compositeGrid;
      
    }
    else if( answer=="structured" || answer=="triangles" || answer=="quadrilaterals" )
    {
      elementType=answer=="structured" ? structuredElements : answer=="triangles" ? triangles : quadrilaterals;
      inputOutputOptionsDialog.getOptionMenu("elements:").setCurrentChoice((int)elementType);
    }
    else if( answer=="default" || 
             answer=="Yee" || answer=="yee" || 
             answer=="DSI" || answer=="DSI-MatVec" || answer=="new DSI" || 
             answer=="NFDTD" || answer=="nfdtd" ||
             answer=="SOSUP" || answer=="sosup" )
    {
      method= (answer=="default" ? defaultMethod :  
               (answer=="Yee" || answer=="yee" ) ? yee : 
               answer=="DSI" ? dsi :
	       answer=="new DSI" ? dsiNew : 
               answer=="DSI-MatVec" ? dsiMatVec : 
               (answer=="NFDTD" || answer=="nfdtd") ? nfdtd : 
               (answer=="SOSUP" || answer=="sosup") ? sosup : defaultMethod);

      timeSteppingOptionsDialog.getOptionMenu("method:").setCurrentChoice((int)method);
      if( timeSteppingMethod==defaultTimeStepping && (method==nfdtd || method==sosup) )
      {
        timeSteppingMethod=modifiedEquationTimeStepping;
	timeSteppingOptionsDialog.getOptionMenu("time stepping:").setCurrentChoice((int)timeSteppingMethod);
      }
    }
    else if( answer=="defaultInitialCondition" ||
             answer=="planeWaveInitialCondition" ||
             answer=="gaussianPlaneWave" ||
             answer=="gaussianPulseInitialCondition" ||
	     answer=="squareEigenfunctionInitialCondition" ||  
	     answer=="annulusEigenfunctionInitialCondition" ||
             answer=="zeroInitialCondition" ||
             answer=="planeWaveScatteredFieldInitialCondition" ||
             answer=="planeMaterialInterfaceInitialCondition" ||
             answer=="gaussianIntegralInitialCondition" ||
             answer=="twilightZoneInitialCondition" ||
             answer=="userDefinedInitialConditions" ||
             answer=="userDefinedKnownSolutionInitialCondition" )
    {
      initialConditionOption=
	(answer=="planeWaveInitialCondition" ? planeWaveInitialCondition :
	 answer=="gaussianPlaneWave"         ? gaussianPlaneWave : 
	 answer=="gaussianPulseInitialCondition" ? gaussianPulseInitialCondition :
	 answer=="squareEigenfunctionInitialCondition" ? squareEigenfunctionInitialCondition :
	 answer=="annulusEigenfunctionInitialCondition" ? annulusEigenfunctionInitialCondition :
	 answer=="planeWaveScatteredFieldInitialCondition" ? planeWaveScatteredFieldInitialCondition :
	 answer=="zeroInitialCondition" ?  zeroInitialCondition :
         answer=="planeMaterialInterfaceInitialCondition" ? planeMaterialInterfaceInitialCondition :
         answer=="gaussianIntegralInitialCondition" ? gaussianIntegralInitialCondition :
         answer=="twilightZoneInitialCondition" ? twilightZoneInitialCondition :
         answer=="userDefinedInitialConditions" ? userDefinedInitialConditionsOption :
         answer=="userDefinedKnownSolutionInitialCondition" ? userDefinedKnownSolutionInitialCondition :
	 defaultInitialCondition);

      if( initialConditionOption==userDefinedInitialConditionsOption )
      {
	setupUserDefinedInitialConditions();
	forcingOption=noForcing;
      }
      else if( initialConditionOption==planeWaveInitialCondition )
      {
        // By default adjust the far-field BC's on all grids for a plane wave initial condition
	adjustFarFieldBoundariesForIncidentField=1;
      }
      else if( initialConditionOption==gaussianPlaneWave )
      {
        knownSolutionOption=gaussianPlaneWaveKnownSolution;
      }
      else if( initialConditionOption==gaussianIntegralInitialCondition )
      {
        knownSolutionOption=gaussianIntegralKnownSolution;
      }
      else if( initialConditionOption==planeMaterialInterfaceInitialCondition )
      {
	knownSolutionOption=planeMaterialInterfaceKnownSolution;
      }
      else if( initialConditionOption==squareEigenfunctionInitialCondition )
      {
	gi.inputString(line,"Enter the frequencies and offset: omegax,omegay,omegaz, x0,y0,z0");
	sScanF(line,"%e %e %e %e %e %e ",&initialConditionParameters[0],&initialConditionParameters[1],
	       &initialConditionParameters[2],&initialConditionParameters[3],&initialConditionParameters[4],
               &initialConditionParameters[5]);
        printF("Using omegax=%f ,omegay=%f ,omegaz=%f, (x0,y0,z0)=(%g,%g,%g)\n",
               initialConditionParameters[0],initialConditionParameters[1],initialConditionParameters[2],
               initialConditionParameters[3],initialConditionParameters[4],initialConditionParameters[5]);

	forcingOption=noForcing;

	knownSolutionOption=squareEigenfunctionKnownSolution;
      }
      else if( initialConditionOption==annulusEigenfunctionInitialCondition )
      {
        knownSolutionOption=annulusEigenfunctionKnownSolution;
	
        if( cg.numberOfDimensions()==2 )
	{
	  gi.inputString(line,"Enter the m,n (n-> Jn, cos(n*theta), m=radial");
	  sScanF(line,"%e %e",&initialConditionParameters[0],&initialConditionParameters[1]);
	  printF("Using m=%i , n=%i\n",int(initialConditionParameters[0]+.5),int(initialConditionParameters[1]+.5));
	}
	else
	{
	  gi.inputString(line,"Enter the m,n,k (n-> Jn, cos(n*theta), m=radial, k=axial (k>=1)");
	  sScanF(line,"%e %e %e",&initialConditionParameters[0],&initialConditionParameters[1],
		 &initialConditionParameters[2]);
	  printF("Using m=%i , n=%i, k=%i\n",int(initialConditionParameters[0]+.5),
		 int(initialConditionParameters[1]+.5),
		 int(initialConditionParameters[2]+.5));
	}
	
	forcingOption=noForcing;
      }
      else if( initialConditionOption==userDefinedKnownSolutionInitialCondition )
      {
	// knownSolutionOption=userDefinedKnownSolution;
      }
      
      
      forcingOptionsDialog.getOptionMenu("known solution:").setCurrentChoice((int)knownSolutionOption);
      forcingOptionsDialog.getOptionMenu("initial conditions:").setCurrentChoice((int)initialConditionOption);
    }

    else if( answer=="noForcing" ||
             answer=="magneticSinusoidalPointSource" ||
             answer=="gaussianSource" ||
             answer=="twilightZone" ||
             answer=="planeWaveBoundaryForcing" ||
             answer=="gaussianChargeSource" ||
             answer=="userDefinedForcing" )
    {
      forcingOption=(answer=="magneticSinusoidalPointSource" ? magneticSinusoidalPointSource :
		     answer=="gaussianSource"                ? gaussianSource : 
                     answer=="twilightZone"                  ? twilightZoneForcing :
                     answer=="planeWaveBoundaryForcing"      ? planeWaveBoundaryForcing :
                     answer=="gaussianChargeSource"          ? gaussianChargeSource :
                     answer=="userDefinedForcing"            ? userDefinedForcingOption :
		     noForcing);

      if( forcingOption==twilightZoneForcing )
      {
	knownSolutionOption=twilightZoneKnownSolution;
	forcingOptionsDialog.getOptionMenu("known solution:").setCurrentChoice((int)knownSolutionOption);
      }
      
      if( forcingOption==magneticSinusoidalPointSource ||
          forcingOption==gaussianSource )
      {
        plotErrors=false;
        dialog.setToggleState("plot errors",(int)plotErrors);
      }
      if( forcingOption==userDefinedForcingOption )
      {
        // -- initialize the user defined forcing:
        setupUserDefinedForcing();
      }

      forcingOptionsDialog.getOptionMenu("forcing:").setCurrentChoice((int)forcingOption);
    }
    else if( answer=="noKnownSolution" ||
             answer=="twilightZoneKnownSolution" ||
	     answer=="planeWaveKnownSolution" ||
             answer=="gaussianPlaneWaveKnownSolution" ||
             answer=="gaussianIntegralKnownSolution" ||
             answer=="planeMaterialInterfaceKnownSolution" ||
	     answer=="scatteringFromADiskKnownSolution" ||
	     answer=="scatteringFromADielectricDiskKnownSolution" ||
	     answer=="scatteringFromASphereKnownSolution" ||
	     answer=="scatteringFromADielectricSphereKnownSolution" ||
	     answer=="squareEigenfunctionKnownSolution" ||
	     answer=="annulusEigenfunctionKnownSolution" ||
	     answer=="eigenfunctionsOfASphereKnownSolution " ||
             answer=="user defined known solution" )
    {
      knownSolutionOption = 
	(answer=="noKnownSolution" ? noKnownSolution :
         answer=="twilightZoneKnownSolution" ? twilightZoneKnownSolution : 
	 answer=="planeWaveKnownSolution" ? planeWaveKnownSolution :
         answer=="gaussianPlaneWaveKnownSolution" ? gaussianPlaneWaveKnownSolution :
         answer=="gaussianIntegralKnownSolution" ? gaussianIntegralKnownSolution :
         answer=="planeMaterialInterfaceKnownSolution" ? planeMaterialInterfaceKnownSolution :
	 answer=="scatteringFromADiskKnownSolution" ? scatteringFromADiskKnownSolution :
	 answer=="scatteringFromADielectricDiskKnownSolution" ? scatteringFromADielectricDiskKnownSolution :
	 answer=="scatteringFromASphereKnownSolution" ? scatteringFromASphereKnownSolution :
	 answer=="scatteringFromADielectricSphereKnownSolution" ? scatteringFromADielectricSphereKnownSolution :
	 answer=="squareEigenfunctionKnownSolution" ? squareEigenfunctionKnownSolution :
	 answer=="annulusEigenfunctionKnownSolution" ? annulusEigenfunctionKnownSolution :
	 answer=="eigenfunctionsOfASphereKnownSolution " ? eigenfunctionsOfASphereKnownSolution : 
         answer=="user defined known solution" ? userDefinedKnownSolution : 
	 noKnownSolution );

      aString & knownSolutionName=dbase.get<aString>("knownSolutionName");
      knownSolutionName=answer;
      
      forcingOptionsDialog.getOptionMenu("known solution:").setCurrentChoice((int)knownSolutionOption);

      if( knownSolutionOption==userDefinedKnownSolution )
      {
        // -- choose the user defined known solution ---
	updateUserDefinedKnownSolution(gi,cg);
      }
      

    }
    else if( answer=="defaultTimeStepping" ||
	     answer=="adamsBashforthSymmetricThirdOrder" ||
	     answer=="rungeKuttaFourthOrder" ||
	     answer=="stoermerTimeStepping" ||
	     answer=="modifiedEquationTimeStepping" )
    {
      timeSteppingMethod = answer=="defaultTimeStepping"  ? defaultTimeStepping :
	answer=="adamsBashforthSymmetricThirdOrder" ? adamsBashforthSymmetricThirdOrder : 
	answer=="rungeKuttaFourthOrder" ? rungeKuttaFourthOrder :
	answer=="stoermerTimeStepping" ? stoermerTimeStepping : 
	answer=="modifiedEquationTimeStepping" ? modifiedEquationTimeStepping : defaultTimeStepping;

      timeSteppingOptionsDialog.getOptionMenu("time stepping:").setCurrentChoice((int)timeSteppingMethod);
    }
    else if( answer=="polynomial" ||
             answer=="trigonometric" ||
             answer=="pulse" )
    {
      twilightZoneOption= answer=="polynomial" ? polynomialTwilightZone :
	answer=="trigonometric" ? trigonometricTwilightZone : pulseTwilightZone;
      forcingOptionsDialog.getOptionMenu("TZ option:").setCurrentChoice((int)twilightZoneOption);
    }
    else if( len=answer.matches("nx,ny") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i",&nx[0],&nx[1]);
      cout << " nx=" << nx[0] << " ny=" << nx[1] << endl;
      timeSteppingOptionsDialog.setTextLabel("nx,ny",sPrintF(line, "%i %i",nx[0],nx[1]));
    }
    else if( answer.matches("set pml error checking offset") )
    {
      gi.inputString(line,"Enter the pml error checking offset");
      sScanF(line,"%i",&pmlErrorOffset);
    }
    else if( answer=="define embedded bodies" )
    {
      if( method!=yee )
      {
	printF("ERROR: 'define embedded bodies' is currently only valid for the Yee method.\n");
      }
      else
      {
	defineRegionsAndBodies();
      }
    }
    else if( len=answer.matches("pml width,strength,power") )
    {
      sScanF(answer(len,answer.length()-1),"%i %e %i ",&numberLinesForPML,&pmlLayerStrength,&pmlPower);
      forcingOptionsDialog.setTextLabel("pml width,strength,power",sPrintF(line, "%i %4.1f %i",numberLinesForPML,
									   pmlLayerStrength,pmlPower));
    }
    else if( len=answer.matches("kx,ky,kz") )
    {
      printF(" kx,ky,kz are used to define the plane wave and other true solutions.\n");
	  
      sScanF(answer(len,answer.length()-1),"%e %e %e",&kx,&ky,&kz);
      forcingOptionsDialog.setTextLabel("kx,ky,kz",sPrintF(line, "%g,%g,%g",kx,ky,kz));
    }
    else if( len=answer.matches("plane wave coefficients") )
    {
      printF(" These 5 values, a1,a2,a3, eps,mu define the plane wave solution:\n"
             " (Ex,Ey,Ez) = sin(twoPi*(kx*x+ky*y+kz*z-cc*t)) (a1,a2,a3),\n"
             " These coefficients should satisfy (since div(E)=0)\n"
             "      a1*kx + a2*ky + a3*kz = 0  ( a.k=0 )\n");

      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e",&pwc[0],&pwc[1],&pwc[2],&epsPW,&muPW);
      forcingOptionsDialog.setTextLabel("plane wave coefficients",sPrintF(line, "%g,%g,%g, %g,%g",pwc[0],pwc[1],pwc[2],epsPW,muPW));
    }
    else if( len=answer.matches("degreeSpace, degreeTime") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i",&degreeSpace,&degreeTime);
      forcingOptionsDialog.setTextLabel("degreeSpace, degreeTime",sPrintF(line,"%i, %i",degreeSpace,degreeTime));

      degreeSpaceX=degreeSpaceY=degreeSpaceZ=degreeSpace;
    }
    else if( len=answer.matches("degreeSpaceX, degreeSpaceY, degreeSpaceZ") )
    {
      sScanF(answer(len,answer.length()-1),"%i %i %i %i",&degreeSpaceX,&degreeSpaceY,&degreeSpaceZ);
      forcingOptionsDialog.setTextLabel("degreeSpaceX, degreeSpaceY, degreeSpaceZ",sPrintF(line,"%i, %i %i",
											   degreeSpaceX,degreeSpaceY,degreeSpaceZ));
    }
    else if( len=answer.matches("material interface normal") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e ",&normalPlaneMaterialInterface[0], 
	     &normalPlaneMaterialInterface[1],&normalPlaneMaterialInterface[2]);
      forcingOptionsDialog.setTextLabel("material interface normal",
                                        sPrintF("%5.3f %5.3f %5.3f",
						normalPlaneMaterialInterface[0], normalPlaneMaterialInterface[1],
						normalPlaneMaterialInterface[2]));
    }
    else if( len=answer.matches("material interface point") )
    {
      sScanF(answer(len,answer.length()-1),"%e %e %e ",&x0PlaneMaterialInterface[0], 
	     &x0PlaneMaterialInterface[1],&x0PlaneMaterialInterface[2]);
      forcingOptionsDialog.setTextLabel("material interface point",
                                        sPrintF("%5.3f %5.3f %5.3f",
						x0PlaneMaterialInterface[0], x0PlaneMaterialInterface[1],
						x0PlaneMaterialInterface[2]));
    }
    else if( len=answer.matches("initial condition bounding box") )
    {
      RealArray & icBox = initialConditionBoundingBox;
      
      sScanF(answer(len,answer.length()-1),"%e %e %e %e %e %e ",&icBox(0,0),&icBox(1,0),
	     &icBox(0,1),&icBox(1,1),
	     &icBox(0,2),&icBox(1,2));
      forcingOptionsDialog.setTextLabel("initial condition bounding box",
                                        sPrintF("%8.2e,%8.2e, %8.2e,%8.2e, %8.2e,%8.2e (xa,xb,ya,yb...)",
						icBox(0,0),icBox(1,0),
						icBox(0,1),icBox(1,1),
						icBox(0,2),icBox(1,2)));
    }
    else if( len=answer.matches("adjust boundaries for incident field") )
    {
      char *buff = new char [answer.length()];
      int trueOrFalse=0;
      sScanF(answer(len,answer.length()),"%i %s",&trueOrFalse,buff);
      aString gridName=buff;
      delete [] buff;
      printF("adjust boundaries for incident field: %i, grid-name=[%s]\n",trueOrFalse,(const char*)gridName);
      
      if( gridName=="all" )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          printF("adjust boundaries for incident field is %s for grid %s\n",(trueOrFalse ? "true" : "false"),(const char*)cg[grid].getName());
	  adjustFarFieldBoundariesForIncidentField(grid)=trueOrFalse;
	}
      }
      else
      {
        bool found=false;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( cg[grid].getName().matches(gridName) )
	  {
            printF("adjust boundaries for incident field is %s for grid %s\n",(trueOrFalse ? "true" : "false"),(const char*)gridName);
	    adjustFarFieldBoundariesForIncidentField(grid)=trueOrFalse;
	    found=true;
	    break;
	  }
	}
        if( !found )
	{
	  printF("WARNING:adjust boundaries for incident field :  No match for grid-name [%s*]\n",(const char*)gridName);
	  continue;
	}
      }      
    }
    
    else if( timeSteppingOptionsDialog.getTextValue(answer,"cfl","%g",cfl) ){}//
    else if( dialog.getTextValue(answer,"tFinal","%g",tFinal) ){}//
    else if( dialog.getTextValue(answer,"tPlot","%g",tPlot) ){}//

    else if( dialog.getTextValue(answer,"frequency","%g",frequency) ){}//

    else if( forcingOptionsDialog.getTextValue(answer,"slow start interval","%f",slowStartInterval) ){}//
    else if( forcingOptionsDialog.getTextValue(answer,"bounding box decay exponent","%f",boundingBoxDecayExponent) ){}//

    else if( forcingOptionsDialog.getTextValue(answer,"scattering radius","%f",dbase.get<real>("scatteringRadius")) ){}//

    else if( forcingOptionsDialog.getToggleValue(answer,"use twilightZone materials",useTwilightZoneMaterials) ){}//

    else if( plotOptionsDialog.getTextValue(answer,"radius for checking errors","%f",radiusForCheckingErrors) ){}//
    else if( plotOptionsDialog.getTextValue(answer,"intensity option","%i",intensityOption) ){}//
    else if( plotOptionsDialog.getTextValue(answer,"intensity averaging interval","%e (periods)",intensityAveragingInterval) ){}//
    else if( plotOptionsDialog.getTextValue(answer,"pml error offset","%i",pmlErrorOffset) ){}//
    else if( plotOptionsDialog.getTextValue(answer,"reference show file:","%s",nameOfReferenceShowFile) ){}//

    else if( inputOutputOptionsDialog.getTextValue(answer,"probe file:","%s",probeFileName) ){}//
    else if( inputOutputOptionsDialog.getTextValue(answer,"probe frequency","%i",frequencyToSaveProbes) ){}//
    else if( inputOutputOptionsDialog.getTextValue(answer,"error norm","%i",errorNorm) )
    { 
      printF("cgmx:INFO: errorNorm=0 : print max norm errors, \n"
             "           errorNorm=1 : also print L1 norm errors,\n"
             "           errorNorm=2 : also print L2 norm errors\n");
    }
    else if( dialog.getTextValue(answer,"debug","%i",debug) ){}//
//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use conservative difference",useConservative) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"solve for electric field",solveForElectricField) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"solve for magnetic field",solveForMagneticField) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use variable dissipation",useVariableDissipation) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"project fields",projectFields) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"project initial conditions",
						      projectInitialConditions) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"project interpolation points",
						      projectInterpolation) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use charge density",useChargeDensity) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use new interface routines",useNewInterfaceRoutines) ){}//

    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use conservative divergence",
						      useConservativeDivergence) ){}//

    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use new forcing method",
						      dbase.get<bool>("useNewForcingMethod")) ){}//


    else if( timeSteppingOptionsDialog.getTextValue(answer,"order of dissipation","%i",orderOfArtificialDissipation) )
    {
      if( orderOfArtificialDissipation<0 ) orderOfArtificialDissipation=orderOfAccuracyInSpace;
    }//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"use divergence cleaning",useDivergenceCleaning) ){}//
    else if( timeSteppingOptionsDialog.getToggleValue(answer,"apply filter",applyFilter) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"filter order","%i",orderOfFilter) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"filter frequency","%i",filterFrequency) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"filter iterations","%i",numberOfFilterIterations) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"filter coefficient","%e",filterCoefficient) ){}//
    // 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"dissipation interval","%i",artificialDissipationInterval) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"number of variable dissipation smooths","%i",
                                                    numberOfVariableDissipationSmooths) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"divergence damping","%g",divergenceDamping) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"accuracy in space","%i",orderOfAccuracyInSpace) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"accuracy in time","%i",orderOfAccuracyInTime) ){}//
    //kkc moved plot dissipation here since matches will think the answer is "dissipation" otherwise
    else if( plotOptionsDialog.getToggleValue(answer,"plot dissipation",plotDissipation) ){}//

    else if( timeSteppingOptionsDialog.getTextValue(answer,"dissipation (curvilinear)","%g",artificialDissipationCurvilinear) ){}//
    else if( timeSteppingOptionsDialog.getTextValue(answer,"dissipation","%g",artificialDissipation) ){}//

    else if( timeSteppingOptionsDialog.getTextValue(answer,"max iterations for interpolation","%i",
						    maximumNumberOfIterationsForImplicitInterpolation) )
    {
      cg.getInterpolant()->setMaximumNumberOfIterations(maximumNumberOfIterationsForImplicitInterpolation);
    }
    else if( timeSteppingOptionsDialog.getTextValue(answer,"interface BC iterations","%i",
						    numberOfIterationsForInterfaceBC) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"omega for interface iterations","%e",
						    omegaForInterfaceIteration) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"interface equations option","%i",
						    interfaceEquationsOption) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"interface option","%i",
						    materialInterfaceOption) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"projection frequency","%i",
						    frequencyToProjectFields) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"consecutive projection steps","%i",
						    numberOfConsecutiveStepsToProject) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"initial projection steps","%i",
						    numberOfInitialProjectionSteps) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"number of divergence smooths","%i",
						    numberOfDivergenceSmooths) ){}// 
    else if( timeSteppingOptionsDialog.getTextValue(answer,"div cleaning coefficient","%e",
						    divergenceCleaningCoefficient) ){}// 
    else if( answer=="projection solver parameters..." )
    { // Specify parameters for the Elliptic solver used to project the fields, div(eps*E)=rho
      if( poisson==NULL )
      {
	poisson = new Oges();
      }
      assert( cgp!=NULL );
      poisson->parameters.update(gi,*cgp);
    }

    else if( plotOptionsDialog.getToggleValue(answer,"plot energy density",plotEnergyDensity) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot intensity",plotIntensity) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot harmonic E field",plotHarmonicElectricFieldComponents) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot errors",plotErrors) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot scattered field",plotScatteredField) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot total field",plotTotalField) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot dissipation",plotDissipation) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot divergence",plotDivergence) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"check errors",checkErrors) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"compute energy",computeEnergy) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"plot rho",plotRho) ){}//
    else if( plotOptionsDialog.getToggleValue(answer,"compare to show file",compareToReferenceShowFile) ){}//
    //    else if( plotOptionsDialog.getToggleValue(answer,"plot dsi points",plotDSIPoints) ){}//
    else if( len=answer.matches("bc: ") )
    {
      line=answer(len,answer.length()-1);
      printF("answer=[%s] line=[%s]\n",(const char*)answer,(const char*)line);
      
      setBoundaryCondition( line,gi,originalBoundaryCondition );
      
      forcingOptionsDialog.setTextLabel("bc:",sPrintF(line,"gridName(side,axis)=bcName",0));

    }
    else if( len=answer.matches("Gaussian plane wave:") )
    {
      sScanF(&answer[len],"%e %e %e %e %e",&betaGaussianPlaneWave,&x0GaussianPlaneWave,&y0GaussianPlaneWave,&z0GaussianPlaneWave);
      
      forcingOptionsDialog.setTextLabel("Gaussian plane wave:",sPrintF(line,"%g %g %g %g (beta,x0,y0,z0)",
								       betaGaussianPlaneWave,x0GaussianPlaneWave,y0GaussianPlaneWave,z0GaussianPlaneWave));
  
    }
    else if( len=answer.matches("Gaussian source:") )
    {
      sScanF(&answer[len],"%e %e %e %e %e",&gaussianSourceParameters[0],&gaussianSourceParameters[1],
             &gaussianSourceParameters[2],&gaussianSourceParameters[3],&gaussianSourceParameters[4]);
      
      forcingOptionsDialog.setTextLabel("Gaussian source:",sPrintF(line,"%g %g %g %g %g (beta,omega,x0,y0,z0)",
								   gaussianSourceParameters[0],gaussianSourceParameters[1],gaussianSourceParameters[2],
								   gaussianSourceParameters[3],gaussianSourceParameters[4]));  
    }
    else if( len=answer.matches("Gaussian pulse:") )
    {
      if( numberOfGaussianPulses>=maxNumberOfGaussianPulses )
      {
	printf(" ERROR: there are too many Gaussian pulses. At most %i are allowed\n",maxNumberOfGaussianPulses);
	continue;
      }
      real *gpp = gaussianPulseParameters[numberOfGaussianPulses];
      sScanF(&answer[len],"%e %e %e %e %e %e",&gpp[0],&gpp[1],&gpp[2],&gpp[3],&gpp[4],&gpp[5]);
      
      forcingOptionsDialog.setTextLabel("Gaussian pulse:",sPrintF(line,"%g %g %g %g %g %g (beta,scale,exponent,x0,y0,z0)",
								  gpp[0],gpp[1],gpp[2],gpp[3],gpp[4],gpp[5]));  

      printF(" Setting pulse %i parameters:  beta=%g scale=%g exponent=%g x0=%g y0=%g z0=%g\n",
	     numberOfGaussianPulses,gpp[0],gpp[1],gpp[2],gpp[3],gpp[4],gpp[5]);
      numberOfGaussianPulses++;

    }
    else if( len=answer.matches("Gaussian charge source:") )
    {
      if( numberOfGaussianChargeSources>=maxNumberOfGaussianChargeSources )
      {
	printf(" ERROR: there are too many Gaussian charge sources. At most %i are allowed\n",
	       maxNumberOfGaussianChargeSources);
	continue;
      }
      real *gcs = gaussianChargeSourceParameters[numberOfGaussianChargeSources];
      sScanF(&answer[len],"%e %e %e %e %e %e %e %e %e",&gcs[0],&gcs[1],&gcs[2],&gcs[3],&gcs[4],&gcs[5],
	     &gcs[6],&gcs[7],&gcs[8]);
      
      printF(" Setting charge source %i parameters:  amplitude=%g beta=%g p=%g x0=%g x1=%g x2=%g v0=%g v1=%g v2=%g\n",
	     numberOfGaussianChargeSources,gcs[0],gcs[1],gcs[2],gcs[3],gcs[4],gcs[5],gcs[6],gcs[7],gcs[8]);
      numberOfGaussianChargeSources++;

      forcingOptionsDialog.setTextLabel("Gaussian charge source:",
					sPrintF(line,"%g %g %g %g %g %g %g %g  (amp,beta,p,x0,y0,z0,v0,v1,v2)",
						gcs[0],gcs[1],gcs[2],gcs[3],gcs[4],gcs[5],gcs[6],gcs[7],gcs[8])); 
    }
    else if( len=answer.matches("TZ omega:") )
    {
      sScanF(&answer[len],"%e %e %e %e",&omega[0],&omega[1],&omega[2],&omega[3]);
      
      forcingOptionsDialog.setTextLabel("TZ omega:",sPrintF(line,"%g %g %g %g (fx,fy,fz,ft)",omega[0],omega[1],omega[2],omega[3]));
    }
    else if( len=answer.matches("coefficients") )
    {
      char *buff = new char [answer.length()];
      sScanF(answer(len,answer.length()),"%e %e %s",&eps,&mu,buff);
      aString gridName=buff;
      delete [] buff;
      printF("coefficients:  eps=%e, mu=%e, grid-name=[%s]\n",eps,mu,(const char*)gridName);
      
      // ** Range G=cg.numberOfComponentGrids();

      // New: Allow for wild cards of the file name*
      
      std::vector<int> gridsToCheck;
      if( gridName=="all" )
      {
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  gridsToCheck.push_back(grid);
	}
      }
      else if( gridName[gridName.length()-1]=='*' )
      {
	// wild card: final char is a '*'
        printF(" INFO: looking for a wild card match since the final character is a '*' ...\n");
        bool found=false;
        gridName=gridName(0,gridName.length()-2); // remove trailing '*'
	
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
          // printF(" Check [%s] matches [%s] \n",(const char*)gridName,(const char*)cg[grid].getName());
	  if( cg[grid].getName().matches(gridName) )
	  {
	    gridsToCheck.push_back(grid);
	    printF(" -- (wild card match) Set coefficients for grid=%i (%s) to eps=%8.2e mu=%8.2e\n",grid,
		   (const char*)cg[grid].getName(),eps,mu);
	    
            found=true;
	  }
	}
        if( !found )
	{
	  printF("WARNING: No match for the wildcard name [%s*]\n",(const char*)gridName);
	  continue;
	}
      }
      else
      {
        bool found=false;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  if( cg[grid].getName()==gridName )
	  {
	    gridsToCheck.push_back(grid);
            found=true;
	    break;
	  }
	}
        if( !found )
	{
	  printF("ERROR looking for the grid named [%s]\n",(const char*)gridName);
	  gi.stopReadingCommandFile();
	  continue;
	}
      }
      
      if( gridsToCheck.size()>=1 )
        gridHasMaterialInterfaces=true;
      printF(" **** setting gridHasMaterialInterfaces=true ****\n");
      for( int g=0; g<gridsToCheck.size(); g++ )
      {
        int grid=gridsToCheck[g];
	
	MappedGrid & mg = cg[grid];
	const IntegerArray & bc = mg.boundaryCondition();
	const IntegerArray & share = mg.sharedBoundaryFlag();
	for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
	{
	  for( int side=0; side<=1; side++ )
	  {
	    if( share(side,axis)>=100 ) // **** for now -- material interfaces have share > 100
	    {
	      bc(side,axis)=interfaceBoundaryCondition;

	      printF(" ++++ setting bc(%i,%i) on grid=%i = interfaceBoundaryCondition\n",side,axis,grid);
		
	    }
	  }
	}
	
	epsGrid(grid)=eps;
	muGrid(grid)=mu;
	cGrid(grid)=1./sqrt(eps*mu);

      }

      pdeParametersDialog.setTextLabel("coefficients",sPrintF(textStrings[nt], "%g %g %s (eps,mu,grid-name)",
							      eps,mu,"all"));

    }
    else if( answer=="show file options..." )
    {
      updateShowFile();
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
      inputOutputOptionsDialog.setTextLabel("maximum number of parallel sub-files",sPrintF(line,"%i",maxFiles));
      GenericDataBase::setMaximumNumberOfFilesForWriting(maxFiles);
    }
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
            mg.update(MappedGrid::THEmask);
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
#ifndef USE_PPP
		  if( ok && mask(i1,i2,i3)>0 ) // *** fix this -- P++ problem --
#else
		    if( ok ) 
#endif
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
      cout << "Unknown command = [" << answer << "]\n";
      gi.stopReadingCommandFile();
       
    }
  }

  gi.popGUI();  // pop dialog

  // If artificialDissipationCurvilinear was not set then use artificialDissipation.
  if( artificialDissipationCurvilinear<0. )
  {
    artificialDissipationCurvilinear=artificialDissipation;
  }
  if( orderOfFilter<0 )
  { // set the default order for the filter
    orderOfFilter=orderOfAccuracyInSpace*2;
  }
  

  // *** now we can compute the full plane wave solution ***
  real &ax=pwc[0], &ay=pwc[1], &az=pwc[2];
  real &bx=pwc[3], &by=pwc[4], &bz=pwc[5];
  const real kNorm = sqrt(kx*kx+ky*ky+kz*kz);
  if( fabs(ax)+fabs(ay)+fabs(az) == 0. )
  {
    // Coefficients of the plane wave solution were not set. Here are the default values:
    epsPW=eps;
    muPW=mu;
    const real c = 1./sqrt(epsPW*muPW);
    const real cc = c*kNorm;
    if( fabs(kx)+fabs(ky)>0. )
    {
      ax = -ky/(epsPW*cc);
      ay =  kx/(epsPW*cc);
      az =0.;
    }
    else
    {
      ay = -kz/(epsPW*cc);
      az =  ky/(epsPW*cc);
      ax =0.;
    }
  }
  assert( epsPW>0. && muPW>0. );
  const real c = 1./sqrt(epsPW*muPW);
  omegaTimeHarmonic = c*kNorm; // here is the frequency associated with the plane wave solution

  const real aNorm = sqrt( ax*ax+ay*ay+az*az );
  real aDotk = ax*kx+ay*ky+az*kz;
  if( (aDotk > 100.*REAL_EPSILON*kNorm*aNorm) || aNorm==0. || kNorm==0. )
  {
    printF("Maxwell:ERROR: plane wave coefficients are not valid: \n"
           "    (ax,ay,az)=(%9.3e,%9.3e,%9.3e), (kx,ky,kz)=(%9.3e,%9.3e,%9.3e), \n",
           ax,ay,az,kx,ky,kz,aDotk);
    if( aDotk > 100.*REAL_EPSILON*kNorm*aNorm )
      printF(" but vector a is not orthogonal to vector k, a.k = %8.2e\n");
    else
      printF(" but vector a or vector k is length zero: aNorm=%8.2e, kNorm=%8.2e.\n",aNorm,kNorm);

    OV_ABORT("error");
  }
  
  //   Coefficients of H are (b1,b2,b3) and given by
  //      b = sqrt(eps/mu) (k X a )/|k|
  const real bc = sqrt(epsPW/muPW)/kNorm;
  bx = (ky*az-kz*ay)*bc;
  by = (kz*ax-kx*az)*bc;
  bz = (kx*ay-ky*ax)*bc;
  
  // **** now build grid functions *****
  setupGridFunctions();

  initializePlaneMaterialInterface();

  initializeRadiationBoundaryConditions();
  
  return 0;
}

int Maxwell::
setBoundaryCondition( aString & answer, GL_GraphicsInterface & gi, IntegerArray & originalBoundaryCondition )
//  ================================================================================================================
//   Parse an answer to set boundary conditions.
//  ================================================================================================================
{
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfComponentGrids = cg.numberOfComponentGrids();


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
    for( int i=1; i<numberOfBCNames; i++ )
    {
      if( nameOfBC==bcName[i] )
      {
	bc=i;
	break;
      }
    }

    if( gridName=="all")
    {
      printF("Setting boundary condition to %s (=%i) on all grids.\n",(const char*)bcName[bc],bc);

      if( bc==periodic )
      {
	bcOption=useAllPeriodicBoundaryConditions;
      }
      
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
	    
//              if( mgp!=NULL )
//  	    {
//  	      MappedGrid & mg = *mgp;
//  	      mg.setBoundaryCondition(side,axis,bc);
//  	      // set underlying mapping too (for moving grids)
//  	      mg.mapping().getMapping().setBoundaryCondition(side,axis,bc);
//  	    }
	    
	  }
	}
      }
    }
  }
  
  return 0;
}

realCompositeGridFunction &
Maxwell::getCGField(Maxwell::FieldEnum f, int tn)
{
  switch(method) {
  case dsi:
  case dsiMatVec:
    switch(f) {
    case EField:
      return dsi_cgfieldsE0[tn];
      break;
    case E100:
      return dsi_cgfieldsE0[tn];
      break;
    case E010:
      return dsi_cgfieldsE1[tn];
      break;
      //    case E001:
      //      break;
    case HField:
    case H100:
      return dsi_cgfieldsH[tn];
      break;
      //    case H010:
      //      break;
      //    case H001:
      //      break;
    }
    break;
  default:
    return cgfields[tn];
  }
  
  return cgfields[tn];
}
