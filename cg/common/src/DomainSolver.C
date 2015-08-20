// ========================================================================================================
/// \class DomainSolver
/// \brief Base class for a generic PDE solver.
/// \details Derive a new CG PDE solver from this base class. 
// ========================================================================================================


#include "DomainSolver.h"
#include "LineSolve.h"
#include "Oges.h"
#include "GridStatistics.h"
#include "EquationDomain.h"
#include "SurfaceEquation.h"
#include "ParallelUtility.h"
#include "gridFunctionNorms.h"
#include "AdvanceOptions.h"
#include <stdarg.h>
#include "OgmgParameters.h"

#include "Interface.h"

//  --------------------------------------------------------------------------
//     Base class functions for the DomainSolver
//  --------------------------------------------------------------------------

int DomainSolver::totalNumberOfArrays=0;



extern DomainSolver *pDomainSolver;  // temporary

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// ===================================================================================================================
/// \brief This is the constructor for the base class DomainSolver. 
///
/// \param par (input) : use this Parameters object.
/// \param cg_ (input) : use this CompositeGrid.
/// \param ps (input) : pointer to a graphics object to use.
/// \param show (input) : pointer to a show file to use.
/// \param plotOption_ (input) : 1=use plotting, 0=run with no plotting.
/// 
// ==================================================================================================================
DomainSolver::
DomainSolver(Parameters & par,
                     CompositeGrid & cg_, 
                     GenericGraphicsInterface *ps /* =NULL */, 
                     Ogshow *show /* =NULL */ , 
                     const int & plotOption_ /* =1 */) : parameters(par), cg(cg_)
{
  className="DomainSolver";
  name="generic";
  pdeName="unknown PDE";


  parameters.dbase.get<GenericGraphicsInterface* >("ps")=ps;
  parameters.dbase.get<Ogshow* >("show")=show;
  if( plotOption_==0 )
  {
    parameters.dbase.get<int >("plotOption")=0;
    parameters.dbase.get<int >("plotMode")=1;  // 1=no plotting => do not allow plotOption to change from no-plotting

    // do not plot anything by default:
    itemsToPlot = 0;  // used by the run time dialog: 1=grid, 2=contour, 4=stream-lines
  }
  else
  {
    // plot contours by default:
   itemsToPlot = 2+8;  // used by the run time dialog: 1=grid, 2=contour, 4=stream-lines, 8=body force regions

  }
  
  parameters.dbase.get<int >("globalStepNumber")=-1;
  numberOfStepsTaken=-1;
  current=0;
  dt=0.;

  poisson=NULL;
  pp=NULL;
  ppx=NULL;
  prL=NULL;
  ppL=NULL;
  prho=NULL;
  pgam=NULL; 
  pvIMS=NULL;
  pwIMS=NULL;
  previousPressure=NULL;
  puLinearized=NULL;
  pGridVelocityLinearized=NULL;
  ui=NULL;
  numberOfImplicitSolvers=0;
  implicitSolver=NULL;
  implicitCoeff=NULL;
  
  pLineSolve=NULL;
  gridHasMaterialInterfaces=true; // true by default unless we find none
  pdtVar=NULL;

  movieFrame=-1;

  restartNumber=-1;  // for multiple restart files

  numberSavedToShowFile=-1; // counts number of solutions saved to the show file 
  
  // pointers to gui dialogs:
  pUniformFlowDialog=NULL;
  pStepFunctionDialog=NULL;
  pShowFileDialog=NULL;
  pTzOptionsDialog=NULL;   

}

// =======================================================================================================
/// \brief Evaluate the min and max grid spacing, total number of grid points etc.
// =======================================================================================================
void DomainSolver::
getGridInfo( real & totalNumberOfGridPoints, 
	     real dsMin[3], real dsAve[3], real dsMax[3], 
	     real & maxMax, real & maxMin, real & minMin )
{

  totalNumberOfGridPoints=0.;
  maxMax=0.,maxMin=0.,minMin=REAL_MAX;

  dtv.resize(cg.numberOfComponentGrids(),-1.);
  hMin.resize(cg.numberOfComponentGrids(),0.);
  hMax.resize(cg.numberOfComponentGrids(),0.);
  numberOfGridPoints.resize(cg.numberOfComponentGrids(),0);

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = cg[grid];

    // *** compute the min and max grid spacing ****
    GridStatistics::getGridSpacing( c,dsMin,dsAve,dsMax ); 

    if( grid==0 && parameters.dbase.get<real>("targetGridSpacing")<=0.  )
    { // If not already assigned, the target grid spacing is taken from grid=0, which is usally a back-ground Cartesian grid
      // This is used to compute performance statistics (TTS)
      parameters.dbase.get<real>("targetGridSpacing")=dsAve[0];
    }
    

    // printF(" ***** From GridStatistics: grid=%i p=%i dsMin=%e,%e \n",grid,parameters.dbase.get<int >("myid"),dsMin[0],dsMin[1]);

    hMin[grid]=REAL_MAX;
    hMax[grid]=0.;
    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
    {
      hMin[grid]=min(hMin[grid],dsMin[axis]);
      hMax[grid]=max(hMax[grid],dsMax[axis]);
    }

    // do this for now:
    maxMax=max(maxMax,hMax[grid]);
    maxMin=max(maxMin,hMin[grid]);
    minMin=min(minMin,hMin[grid]);

    // ******** count the number of active grids points  ********

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    getIndex(extendedGridIndexRange(c),I1,I2,I3);

    // numberOfGridPoints[grid]=sum(evaluate(c.mask()(I1,I2,I3)!=0));

    const intSerialArray & maskLocal= c.mask().getLocalArray();
    bool ok=ParallelUtility::getLocalArrayBounds(c.mask(),maskLocal,I1,I2,I3);

    real numPoints=0.;
    if( ok )
    {
      int i1,i2,i3;
      FOR_3D(i1,i2,i3,I1,I2,I3)
      {
	if( maskLocal(i1,i2,i3)!=0 )
	  numPoints++;
      }
    }
    numberOfGridPoints[grid]=ParallelUtility::getSum(numPoints);

    totalNumberOfGridPoints+=numberOfGridPoints[grid];
  }

}

// =======================================================================================================
/// \brief Output the main header banner that includes info about the grid and parameters.
// =======================================================================================================
void DomainSolver::
outputHeader()
{

  // **** compute the grid spacing and grid point info *****
  real totalNumberOfGridPoints;
  real maxMax,maxMin,minMin;
  real dsMin[3], dsAve[3], dsMax[3];

  getGridInfo(totalNumberOfGridPoints,dsMin,dsAve,dsMax,maxMax,maxMin,minMin );

  const int buffSize=100;
  char buff[buffSize];

  for( int output=0; output<=1; output++ )
  {
    if( parameters.dbase.get<int >("myid")!=0 ) continue;

    FILE *file = output==0 ? stdout : parameters.dbase.get<FILE* >("logFile");

    if( parameters.isMovingGridProblem() )
    { // Output headers for any moving bodies *wdh* 2015/03/06
      fPrintF(file,"\n");
      parameters.dbase.get<MovingGrids >("movingGrids").writeParameterSummary(file);
    }

    aString fullPdeName=pdeName;
    if ( parameters.dbase.has_key("pdeNameModifier") )
      fullPdeName += ", "+parameters.dbase.get<aString>("pdeNameModifier");

    if( parameters.dbase.get<int >("numberOfSpecies")>0 && parameters.dbase.get<aString >("reactionName")!="" )
      fullPdeName+=" ("+parameters.dbase.get<aString >("reactionName")+")";


    fPrintF(file,"\n"
	    "***********************************************************************************\n");
    if( parameters.dbase.get<int>("multiDomainProblem")==0  )
    {
      fPrintF(file,
	      "             %s version 1.0                                 \n"
	      "             -----------------                              \n",
	      (const char*)getClassName()   );
    
    }
    else
    { // multi-domain problem 
      fPrintF(file,
	      "             %s : %s version 1.0                            \n"
	      "             -----------------------------                  \n",
	      (const char*)name,(const char*)getClassName());
    
    }
    
    if( parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")==NULL || parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList")->size()<=1 )
    { 
      // In this case we are solving one set of equations in one domain
      fPrintF(file,
	      " Solving: %s                                                      \n",(const char*)fullPdeName);
    
    }
    else
    {
      // In this case we are solving different sets of equations in different domains
      ListOfEquationDomains & equationDomainList = *(parameters.dbase.get<ListOfEquationDomains* >("pEquationDomainList"));

      // ListOfEquationDomains:iterator iter; 
      for( int domain=0; domain<equationDomainList.size(); domain++ )
      {
	const EquationDomain & equationDomain = equationDomainList[domain];
	aString fullPdeName=equationDomain.getPDE()->pdeName;
	if ( parameters.dbase.has_key("pdeNameModifier") )
	  fullPdeName += ", "+parameters.dbase.get<aString>("pdeNameModifier");

	fPrintF(file,"Equation Domain %i: name=%s, pde=%s\n",domain,
		(const char*)equationDomain.getName(),
		(const char*)fullPdeName); 
	for( int ged=0; ged<equationDomain.gridList.size(); ged++ )
	{
	  int grid=equationDomain.gridList[ged];
	  assert( grid>=0 && grid<cg.numberOfComponentGrids() );
	  fPrintF(file,"    ... contains grid %i (%s)\n",grid,(const char*)cg[grid].getName());
	}
      
      }
    }

    aString nameOfGridFile = parameters.dbase.get<aString>("nameOfGridFile");
    if( nameOfGridFile=="" ) nameOfGridFile="unknown";
    fPrintF(file,"\n The overlapping grid was read from the file=[%s].\n",(const char*)nameOfGridFile);

    const int np= max(1,Communication_Manager::numberOfProcessors());
    fPrintF(file,"Number of processors=%i.\n",np);

    writeParameterSummary(file);

    if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") && 
        (parameters.dbase.get<real >("ad21")>0. || parameters.dbase.get<real >("ad22")>0.) )
      fPrintF(file," second order artificial diffusion is on: ad21=%e, ad22=%e \n",
              parameters.dbase.get<real >("ad21"),parameters.dbase.get<real >("ad22"));
    else
      fPrintF(file," second order artificial diffusion is off.\n");
    if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") && 
        (parameters.dbase.get<real >("ad41")>0. || parameters.dbase.get<real >("ad42")>0.) )
      fPrintF(file," fourth order artificial diffusion is on: ad41=%e, ad42=%e \n",
              parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"));
    else
      fPrintF(file," fourth order artificial diffusion is off.\n");
  
    if( poisson )
    {
      fPrintF(file," pressure equation solver: solver=%s, \n",(const char*)pressureSolverParameters.getSolverName());
      if(  poisson->isSolverIterative() )
      {
	real rtol,atol;
	int maximumNumberOfIterations;
	if( poisson->parameters.getSolverType()!=OgesParameters::multigrid )
	{
	  pressureSolverParameters.get(OgesParameters::THErelativeTolerance,rtol);
	  pressureSolverParameters.get(OgesParameters::THEabsoluteTolerance,atol);
	  pressureSolverParameters.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	}
	else
	{
	  OgmgParameters* ogmgPar = poisson->parameters.getOgmgParameters();
	  assert( ogmgPar!=NULL );
	  ogmgPar->get(OgmgParameters::THEresidualTolerance,rtol);  // note: residual
	  ogmgPar->get(OgmgParameters::THEabsoluteTolerance,atol);
	  ogmgPar->get(OgmgParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
	}
	fPrintF(file,"                         : rel-tol=%8.2e, abs-tol=%8.2e, max iterations=%i (0=choose default)\n",
		rtol,atol,maximumNumberOfIterations);
	if( poisson->parameters.getSolverType()==OgesParameters::multigrid )
	{ // Here is the MG convergence criteria: 
          fPrintF(file,"                         : convergence: max-defect < (rel-tol)*L2NormRHS + abs-tol.\n");
	}
	
      }
    }
  
    // show user defined parameters
    ListOfShowFileParameters & pdeParameters = parameters.dbase.get<ListOfShowFileParameters >("pdeParameters");
    std::list<ShowFileParameter>::iterator iter; 
    for(iter = pdeParameters.begin(); iter!=pdeParameters.end(); iter++ )
    {
      ShowFileParameter & param = *iter;
      aString name; ShowFileParameter::ParameterType type; int ivalue; real rvalue; aString stringValue;
      param.get( name, type, ivalue, rvalue, stringValue );

      fPrintF(file," %s = ",(const char*)name);
      if( type==ShowFileParameter::realParameter )
        fPrintF(file,"%e\n",rvalue);
      else if( type==ShowFileParameter::intParameter )
        fPrintF(file,"%i\n",ivalue);
      else
        fPrintF(file,"%s\n",(const char*)stringValue);

    }

    int maxNameLength=3;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      maxNameLength=max( maxNameLength,cg[grid].mapping().getName(Mapping::mappingName).length());

    aString blanks="                                                                           ";
    fPrintF(file,"               Grid Data\n"
	   "               ---------\n"
	   "grid     name%s  gridIndexRange(0:1,0:2)           gridPoints   hmx      hmn  \n",
	   (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
    
    sPrintF(buff,"%%4i: %%%is   ([%%2i:%%5i],[%%2i:%%5i],[%%2i:%%5i]) %%10g   %%8.2e %%8.2e \n",maxNameLength);
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = cg[grid];
    
      // fPrintF(file,"%4i: %20s ([%2i:%5i],[%2i:%5i],[%2i:%5i])  %8i   %8.2e %8.2e \n",
      fPrintF(file,buff,grid, (const char *)cg[grid].getName(),
	     c.gridIndexRange(Start,axis1),c.gridIndexRange(End,axis1),
	     c.gridIndexRange(Start,axis2),c.gridIndexRange(End,axis2),
	     c.gridIndexRange(Start,axis3),c.gridIndexRange(End,axis3),
	     numberOfGridPoints[grid],hMax[grid],hMin[grid]);
    }
    fPrintF(file," total number of grid points =%g (egir), min(hmn)=%6.2e, max(hmn)=%6.2e, max(hmx)=%6.2e,  \n",
	   totalNumberOfGridPoints,minMin,maxMin,maxMax);

    int mgLevels=cg.numberOfPossibleMultigridLevels();
    fPrintF(file," number of possible multigrid levels=%i.\n\n",mgLevels);
    displayBoundaryConditions(file);

    fPrintF(file,"***********************************************************************************\n\n");

  }  // end for( output )
  
  // kkc 070126 the following is a hack, the overloaded versions of this function check 
  //            to see if the input is the checkFile and do the right thing
  writeParameterSummary(parameters.dbase.get<FILE* >("checkFile"));

  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    if( parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==Parameters::trigonometric )
      fPrintF(parameters.dbase.get<FILE* >("checkFile"),"trigonometric TZ}");
    else
      fPrintF(parameters.dbase.get<FILE* >("checkFile"),"polynomial TZ}");
  }
  fPrintF(parameters.dbase.get<FILE* >("checkFile"),"}\n");

}

// ===================================================================================================================
/// \brief Setup routine.
/// \details The function is called after the parameters have been assigned (called by setParametersInteractively).
///        It will setup the problem and call setupGridFunctions() to allocate grid functions. 
///        This function will output the header information that summarizes the problem being solved and the values of
///        the various parameters.
/// \param time (input) : current time.
// ===================================================================================================================
void DomainSolver::
setup(const real & time /* = 0. */ )
{
  real cpu0 = getCPU();

  if( false )
  {
    printF("**** setParameters START\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  int grid,axis,side;
  const int numberOfDimensions = cg.numberOfDimensions();
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters >("psp");
  real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  Parameters::TimeSteppingMethod & timeSteppingMethod= parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod");
  realCompositeGridFunction & u = gf[current].u;

  // setup the twlightzone function:
  if( parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::noInitialConditionChosen )
    parameters.setTwilightZoneFunction(parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice"),parameters.dbase.get<int >("tzDegreeSpace"),parameters.dbase.get<int >("tzDegreeTime"));


  parameters.updatePDEparameters();
  
//    // *** scLC
//    if( ( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::implicit ) &&
//        (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")!=Parameters::rKutta ))
//      parameters.dbase.get<IntegerArray >("gridIsImplicit")=FALSE;  // make sure these are false for an explicit method
//  // *** ecLC

  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int axis=0; axis<numberOfDimensions; axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( parameters.bcType(side,axis,grid)==Parameters::parabolicInflow )
	  parameters.dbase.get<IntegerArray>("variableBoundaryData")(grid)=true;  // **** why can't this be in setBoundaryConditions ?
      }
    }
  }

  if( timeSteppingMethod==Parameters::implicitAllSpeed )
  {
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    // const int & vc = parameters.dbase.get<int >("vc");
    // const int & wc = parameters.dbase.get<int >("wc");
    const int & pc = parameters.dbase.get<int >("pc");
    const int & tc = parameters.dbase.get<int >("tc");
    
    if( parameters.dbase.get<bool >("computeReactions") && numberOfDimensions==1 )
    {
      IntegerArray componentsToPlot(3);
      componentsToPlot(0)=rc;
      componentsToPlot(1)=uc;
      componentsToPlot(2)=pc;
      psp.set(GI_COMPONENTS_TO_PLOT,componentsToPlot);
    }
    

    // real & machNumber = parameters.dbase.get<real >("machNumber");
    // real & gamma = parameters.dbase.get<real >("gamma");

    //  real r0=1., u0=1., v0=0., w0=0.;
    //    real p0=(SQR(u0)+SQR(v0)+SQR(w0))/( gamma*SQR(machNumber)/r0 );
    RealArray & initialConditions = parameters.dbase.get<RealArray>("initialConditions");
    
    if( !parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      assert( initialConditions(rc)!=(real)Parameters::defaultValue &&
	      initialConditions(tc)!=(real)Parameters::defaultValue );
      // pressureLevel: subtract off a mean value of the pressure
      // p=rho*Rg*T :
      pressureLevel=initialConditions(rc)*parameters.dbase.get<real >("Rg")*initialConditions(tc);
    }
    else if( pressureLevel==(real)Parameters::defaultValue )
    {
      pressureLevel=1./parameters.dbase.get<real >("machNumber");
    }
    printF("DomainSolver: setting pressureLevel = %e \n",pressureLevel);

  }
  
  if( getClassName()=="Cgmp" ) return;  // ************ do this for now ********


  parameters.setUserDefinedParameters(); // update user defined parameters


  if( false )
  {
    printF("**** setParameters II\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  if( parameters.dbase.get<Parameters::InitialConditionOption >("initialConditionOption")==Parameters::noInitialConditionChosen && 
      parameters.dbase.get<bool >("twilightZoneFlow") && parameters.dbase.get<bool >("assignInitialConditionsWithTwilightZoneFlow") )
  {
    // ********* assign initial conditions for twilight zone flow *****************
    
    cg.update(MappedGrid::THEvertex | MappedGrid::THEcenter);
    u.updateToMatchGrid(cg,nullRange,nullRange,nullRange,parameters.dbase.get<int >("numberOfComponents")); 
    parameters.dbase.get<OGFunction* >("exactSolution")->assignGridFunction( u,parameters.dbase.get<real >("tInitial") );   //  Twilight-zone flow

    //    u.display("setParameters: u after assign TZ IC's");

  }

  if( false )
  {
    printF("**** setParameters III\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }

  // **************************************************************************
  // *********************now update the solvers ******************************
  // **************************************************************************

  // kkc 070126 BILL : can we just move this stuff to DomainSolver::initialize and get rid of DomainSolver::setup ?
  if( debug() & 2 )
    printF("inside DomainSolver::setup\n");

  GridFunction & solution = gf[current];

  solution.t=time;
//  solution.updateToMatchGrid(cg);
  solution.setParameters(parameters);
  
  if( parameters.isMovingGridProblem() )
  {
    // do not assign so the gridVelocity will be recomputed:
    // *wdh* solution.gridVelocityTime=solution.t;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( parameters.gridIsMoving(grid) )
        solution.createGridVelocity(grid);
    }
  }

  finiteDifferenceOperators.updateToMatchGrid(cg);
  solution.u.setOperators(finiteDifferenceOperators);

  finiteDifferenceOperators.setTwilightZoneFlow(parameters.dbase.get<bool >("twilightZoneFlow"));
  finiteDifferenceOperators.setTwilightZoneFlowFunction(*(parameters.dbase.get<OGFunction* >("exactSolution")));

  // allocate grid functions gf[] etc. based on the time-stepping method
  setupGridFunctions();

  // kkc 070126 the rest of this stuff should probably sit in setup... 070308 so now it does
  // for AMR computations build the AMR grid structure for the initial conditions.
  if( parameters.dbase.get<bool>("adaptiveGridProblem") )
  {
    buildAmrGridsForInitialConditions();
  }
  

  if( poisson != NULL )
  {
    poisson->setOgesParameters(pressureSolverParameters);
  }
  if( implicitSolver != NULL )
  {
    for( int i=0; i<numberOfImplicitSolvers; i++ )
      implicitSolver[i].setOgesParameters(implicitTimeStepSolverParameters);
  }
  

  // --------------------------------
  // --- output the header banner ---
  // --------------------------------
  outputHeader();
  

  // --- Evaluate variable material properties ---
  setVariableMaterialProperties( solution, solution.t );


  // Initialize the solution (project the solution if required) and updateToMatchGrid
  printF("DomainSolver::setup: initialize the solution: initializeSolution() ...\n");
  initializeSolution();

  cleanupInitialConditions();
  userDefinedInitialConditionsCleanup();  // *wdh* 050514 -- cleanup user defined initial conditions

  real cpu1=getCPU();

  RealArray & timing = parameters.dbase.get<RealArray>("timing");

  timing=0.;  // ********* NOTE: all times up to this point are counted in timing(Parameters::timeForInitialize)
  
  timing(parameters.dbase.get<int>("totalTime"))+=cpu1-cpu0;
  timing(parameters.dbase.get<int>("timeForInitialize"))=cpu1-cpu0;

  // ::display(timing,"timing after setup");

  // **** Look for INFO from a multi-domain problem ---
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    if( TRUE || parameters.gridIsMoving(grid) )  // **DEBUGGING**
    {
      DomainSolver *pCgmp = parameters.dbase.get<DomainSolver*>("multiDomainSolver");
      if( pCgmp!=NULL )
      {
	DomainSolver & cgmp = *pCgmp;
	
	int numberOfDomains = cgmp.domainSolver.size();
	
	printP("--SETUP-- This is a multi-domain problem: multiDomainSolver!=NULL, numberOfDomains=%i \n",numberOfDomains);
        // InterfaceList & interfaceList = pCgmp->parameters.dbase.get<InterfaceList>("interfaceList");

	BoundaryData::BoundaryDataArray & pBoundaryData = parameters.getBoundaryData(grid); // this will create the BDA if it is not there
	std::vector<BoundaryData> & boundaryDataArray = parameters.dbase.get<std::vector<BoundaryData> >("boundaryData");
	BoundaryData & bd = boundaryDataArray[grid];


        IntegerArray & interfaceType = parameters.dbase.get<IntegerArray >("interfaceType");	
	for( int side=0; side<=1; side++ )
	{
	  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	  {
	    if( interfaceType(side,axis,grid)==Parameters::tractionInterface ) 
	    {
	      printP("--SETUP-- (grid,side,axis)=(%i,%i,%i) is a tractionInterface\n",grid,side,axis);
	    }
	  }
	}
	if( bd.dbase.has_key("interfaceDescriptorArray") )
	{
	  typedef InterfaceDescriptor* (InterfaceDescriptorType)[2][3];
	  InterfaceDescriptorType & interfaceDescriptorArray = bd.dbase.get<InterfaceDescriptorType>("interfaceDescriptorArray");
	  for( int side=0; side<=1; side++ )
	  {
	    for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
	    {
	      if( interfaceDescriptorArray[side][axis] !=NULL )
	      {
		InterfaceDescriptor & interfaceDescriptor = *interfaceDescriptorArray[side][axis];
		const int domain1=interfaceDescriptor.domain1, domain2=interfaceDescriptor.domain2;
		printP("--SETUP-- (grid,side,axis)=(%i,%i,%i) has an interfaceDescriptor: domain1=%i, domain2=%i\n",grid,side,axis,domain1,domain2);
		printP("--SETUP-- domain1 : %s, domain2 : %s\n",(const char*)cgmp.domainSolver[domain1]->className, 
                                                                (const char*)cgmp.domainSolver[domain2]->className);


	      }
	    
	    }

	  }
	}
	
      }
      
    }
  }
  

  if( false )
  {
    printF("**** end of setParameters\n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      cg[grid].displayComputedGeometry();
  }
  

}


// ===================================================================================================================
/// \brief Destructor.
// ==================================================================================================================
DomainSolver::
~DomainSolver()
{
  // cout << "~OB_CompositeGridSolver(): numberOfComponentGrids = " << solution.u.numberOfComponentGrids() << endl;
  int grid;

  delete poisson;

  if( ui!=NULL )
  {
    delete [] ui;
  }
  
  delete pp;
  delete ppx;
  delete prL;
  delete ppL;
  delete prho;
  delete pgam; 

  delete pvIMS;
  delete pwIMS;
  delete [] previousPressure;
  delete puLinearized;
  delete [] pGridVelocityLinearized;
  delete [] implicitSolver;
  delete [] implicitCoeff;
 
  delete pLineSolve;

  delete pdtVar;
}

void DomainSolver::
addForcing(realMappedGridFunction & dvdt, const realMappedGridFunction & u, int iparam[], real rparam[],
	   realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
           realMappedGridFunction *referenceFrameVelocity /* =NULL */ )
{
  printF("DomainSolver::addForcing:ERROR: base class function called!\n");
  Overture::abort("error");
}

int DomainSolver::
advanceLineSolve(LineSolve & lineSolve,
		 const int grid, const int direction, 
		 realCompositeGridFunction & u0, 
		 realMappedGridFunction & f, 
		 realMappedGridFunction & residual,
		 const bool refactor,
		 const bool computeTheResidual /*  =false */ )
{
  printF("DomainSolver::advanceLineSolve:ERROR: base class function called!\n");
  Overture::abort("error");
}


void DomainSolver::
allSpeedImplicitTimeStep(GridFunction & gf,              // ** get rid of this **
			 real & t, 
			 real & dt0, 
			 int & numberOfTimeSteps,
			 const real & nextTimeToPrint )
{
  printF("DomainSolver::allSpeedImplicitTimeStep:ERROR: base class function called!\n");
  Overture::abort("error");
}


// Here is where boundary conditions are implemented
int DomainSolver::
applyBoundaryConditions(const real & t, realMappedGridFunction & u, 
			    realMappedGridFunction & gridVelocity,
			    const int & grid,
			    const int & option /* =-1 */,
			    realMappedGridFunction *puOld /* =NULL */, 
			    realMappedGridFunction *pGridVelocityOld /* =NULL */,
			    const real & dt /* =-1. */)
{
  printF("DomainSolver::applyBoundaryConditions:ERROR: base class function called!\n");
  Overture::abort("error");
}

int DomainSolver::
applyBoundaryConditionsForImplicitTimeStepping(realMappedGridFunction & u, 
                                               realMappedGridFunction &uL,
					       realMappedGridFunction & gridVelocity,
					       real t,
					       int scalarSystem,
					       int grid )
{
  printF("DomainSolver::applyBoundaryConditionsForImplicitTimeStepping:ERROR: base class function called!\n");
  Overture::abort("error");
  return 0;
}

void DomainSolver::
assignTestProblem( GridFunction & cgf )
{
  
}


//\begin{>>DomainSolverInclude.tex}{\subsection{checkArrays}} 
void DomainSolver::
checkArrays(const aString & label) 
//==============================================================================
// /Description:
// Output a warning messages if the number of arrays has increased
//\end{DomainSolverInclude.tex}  
//==============================================================================
{
  if(GET_NUMBER_OF_ARRAYS > totalNumberOfArrays ) 
  {
    totalNumberOfArrays=GET_NUMBER_OF_ARRAYS;
    printf("\n**** %s: Number of A++ arrays has increased to %i \n\n",(const char*)label,GET_NUMBER_OF_ARRAYS);
  }
}


// void DomainSolver::
// determineErrors(GridFunction & cgf,
// 		const aString & label /* =nullString */)
// {
//   OB_CompositeGridSolver::determineErrors(cgf,label);
// }


// void DomainSolver::
// determineErrors(realCompositeGridFunction & u,
// 		realMappedGridFunction **gridVelocity,
// 		const real & t, 
// 		const int options,
//                 RealArray & err,
// 		const aString & label /* =nullString */ )
// {
//   OB_CompositeGridSolver::determineErrors(u,gridVelocity,t,options,err,label);
// }

// determine errors if the true solution is known
void DomainSolver:: 
determineErrors(realMappedGridFunction & v, const real & t)
{
  printF("DomainSolver::determineErrors:ERROR: base class function called!\n");
  Overture::abort("error");
}


// int DomainSolver::
// formImplicitTimeSteppingMatrix(realMappedGridFunction & coeff,
// 			       const real & dt0, 
// 			       int scalarSystem,
// 			       realMappedGridFunction & uL,
// 			       const int & grid )
// {
//   printF("DomainSolver::formImplicitTimeSteppingMatrix:ERROR: base class function called!\n");
//   Overture::abort("error"); 
// }




const aString & DomainSolver:: 
getClassName() const
// =========================================================================================
//  /Description:
//    Return the class name of this object
// =========================================================================================
{
  return className;

}

const aString & DomainSolver:: 
getName() const
// =========================================================================================
//  /Description:
//    Return the name given to this object such as "fluid", "air" ...
// =========================================================================================
{
  return name;

}

const aString& DomainSolver::
getPdeName() const
// ========================================================================================
// /Description:
//    Get the name of this domain.
// ========================================================================================
{
  return pdeName;
}

// ======================================================================
/// \brief Domain solver print function with a prefix identifier string.
/// \details implementation of a "printf" function that adds a prefix string to
///    identify the DomainSolver (and only prints on processor 0)
// 
// \param s (input) : fill in this string.
// \param format (input) : use this printf style format.
// \param argument `$\ldots$' (input): variable length argument list.
// 
// ======================================================================
void DomainSolver::
printP(const char *format, ...) const
{
  #ifdef USE_PPP
    if( Communication_Manager::My_Process_Number > 0 )
      return;
  #endif

  printF("%s:",(const char*)getName());

  va_list args;
  va_start(args,format);
  vprintf(format,args);
  va_end(args);
}


// ===================================================================================================================
/// \brief Compute the residual for "steady state" solvers
/// \param t (input): current time 
/// \param dt (input): current global time step -- is this used ?
/// \param cgf (input): holds solution to compute the residual from
/// \param residual (output): residual
///
// ===================================================================================================================
int DomainSolver::
getResidual( real t, real dt, GridFunction & cgf, realCompositeGridFunction & residual)
{
  printF("DomainSolver::getResidual:ERROR: base class function called!\n");
  Overture::abort("error");
  return 0;
}


// ===================================================================================================================
/// \brief Compute the max and l2 residuals and optionally output the info to a file.
/// \param t0 (input) : current time (used in the output to the file).
/// \param residual (input) : holds the residual.
/// \param maximumResidual (output) : maximum residual over all components
/// \param maximuml2 (output) : maximum l2-residual over all components
/// \param file (output) : if file!=NULL then output info about the residuals to this file.
// ===================================================================================================================
int DomainSolver::
getResidualInfo( real t0, const realCompositeGridFunction & residual, real & maximumResidual, real & maximuml2, FILE *file )
{
  maximumResidual=0;
  maximuml2 = 0.;

  Range N = parameters.dbase.get<Range >("Rt");
  RealArray maxRes(N);
  RealArray l2Res(N);
  maxRes=0.;
  l2Res=0;

  int maskOption=1;  // check mask()>0 
  int extra=-1;      // get gridIndexRange minus one layer
  for( int n=N.getBase(); n<=N.getBound(); n++ )
  {
    maxRes(n)=maxNorm(residual,n,maskOption,extra);
    l2Res(n) =l2Norm(residual,n,maskOption,extra);
  }
  
  maximumResidual=max(maxRes);
  maximuml2 = max(l2Res);
  maximumResidual=ParallelUtility::getMaxValue(maximumResidual);
  maximuml2=ParallelUtility::getMaxValue(maximuml2);
   
  //   printF(" $$$$$$$$$ getResidualInfo: t0=%8.2e dt=%8.2e $$$$$$$$$$$\n",t0,dt);

  if( file!=NULL )
  {
    if( parameters.isSteadyStateSolver() )
      fPrintF(file," ----> t=%8.3e: step=%i resid[max, l2]: all=[%5.1e, %5.1e]",
	      t0,parameters.dbase.get<int >("globalStepNumber"),maximumResidual,maximuml2);
    else
      fPrintF(file," ----> t=%8.3e, dt=%7.2e: step=%i |u.t|[max,l2]: all=[%5.1e, %5.1e]",t0,dt,
	      parameters.dbase.get<int >("globalStepNumber"),maximumResidual,maximuml2);
  
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      fPrintF(file," %s=[%8.2e, %8.2e]",(const char*)residual.getName(n),maxRes(n),l2Res(n));
    fPrintF(file,"\n");
  }
  
  return 0;
}

//\begin{>>DomainSolverInclude.tex}{\subsection{getSolutionBounds}} 
void DomainSolver::
getSolutionBounds(const realMappedGridFunction & u, realArray & uMin, realArray & uMax, real & uvMax)
//=========================================================================================
// /Description:
//   Compute min and max of all components plus max of all values
//
//\end{DomainSolverInclude.tex}  
//=========================================================================================
{
  MappedGrid & mg = *u.getMappedGrid();
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");

  uMin=FLT_MAX;
  uMax=-FLT_MAX;
  Index I1,I2,I3;
  getIndex(mg.gridIndexRange(),I1,I2,I3 );
  where( mg.mask()(I1,I2,I3) != 0 )
  {
    for( int n=0; n<numberOfComponents; n++ )
    {
      uMin(n)=min(uMin(n),min(u(I1,I2,I3,n)));
      uMax(n)=max(uMax(n),max(u(I1,I2,I3,n)));
    }
  }
  uvMax=max(max(fabs(uMin)),max(fabs(uMax)));
}

// void DomainSolver:: 
// getSolutionBounds(const realMappedGridFunction & u, realArray & uMin, realArray & uMax, real & uvMax)
// {
//   printF("DomainSolver::getSolutionBounds:ERROR: base class function called!\n");
//   Overture::abort("error");
// }

// // determine the time step based on a given solution
// real  DomainSolver:: 
// getTimeStep( GridFunction & gf)
// {
//   return OB_CompositeGridSolver::getTimeStep(gf);
// }

// // determine the time step based on a given solution
// real DomainSolver:: 
// getTimeStep(MappedGrid & mg,
// 	    realMappedGridFunction & u, 
// 	    realMappedGridFunction & gridVelocity,
// 	    const Parameters::TimeSteppingMethod & timeSteppingMethod,
// 	    const int & grid  )
// {
//   printF("DomainSolver::getTimeStep:ERROR: base class function called!\n");
//   Overture::abort("error");
// }


// // semi-discrete discretization. Lambda is used to determine the time step by requiring
// // lambda*dt to be in the stability region of the particular time stepping method we are
// // using
// void DomainSolver:: 
// getTimeSteppingEigenvalue(MappedGrid & mg, 
// 			  realMappedGridFunction & u, 
// 			  realMappedGridFunction & gridVelocity,  
// 			  real & reLambda,
// 			  real & imLambda, 
// 			  const int & grid)
// {
//   printF("DomainSolver::getTimeSteppingEigenvalue:ERROR: base class function called!\n");
//   Overture::abort("error");
// }



// void DomainSolver::
// getUt( GridFunction & cgf, 
//        const real & t, 
//        RealCompositeGridFunction & ut, 
//        real tForce )
// {
//   OB_CompositeGridSolver::getUt(cgf,t,ut,tForce);
// }


int DomainSolver::
getUt(const realMappedGridFunction & v, 
      const realMappedGridFunction & gridVelocity, 
      realMappedGridFunction & dvdt, 
      int iparam[], real rparam[],
      realMappedGridFunction & dvdtImplicit /* = Overture::nullRealMappedGridFunction() */,
      MappedGrid *pmg2 /* =NULL */,
      const realMappedGridFunction *pGridVelocity2 /* = NULL */)
{
  printF("DomainSolver::getUt:ERROR: base class function called!\n");
  Overture::abort("error");
  return 0;
}

// void DomainSolver::
// outputSolution( realCompositeGridFunction & u, const real & t,
// 		const aString & label /* =nullString */,
//                 int printOption /* = 0 */  )
// {
//   printF("DomainSolver::outputSolution:ERROR: base class function called!\n");
//   Overture::abort("error");
// }


void DomainSolver::
advanceADI( real & t, real & dt, int & numberOfSubSteps, int & init, int initialStep  )
{
  printF("DomainSolver::advanceADI:ERROR: base class function called!\n");
  Overture::abort("error");
}


void DomainSolver::
outputSolution( const realMappedGridFunction & u, const real & t )
{
  printF("DomainSolver::outputSolution(realMappedGridFunction) :ERROR: base class function called!\n");
//  Overture::abort("error");
}

void DomainSolver::
setName(const aString & name_ )
// =========================================================================================
//  /Description:
//    Give a name to this object such as "fluid", "air" ...
// =========================================================================================
{
  name=name_;
}

// =========================================================================================
/// \brief Provide the name of the file from which the overlapping grid was read.
// =========================================================================================
void DomainSolver::
setNameOfGridFile(const aString & name )
{
  parameters.dbase.get<aString>("nameOfGridFile")=name;
}

//=========================================================================================
/// \brief: Solve for the pressure given the velocity.
/// \param updateSolutionDependentEquations (input) : update the equations as needed if they depend
///        on the current solution. 
//=========================================================================================
void DomainSolver::
solveForTimeIndependentVariables( GridFunction & cgf, bool updateSolutionDependentEquations /* =false */ )
{
}


// this should be removed :
int DomainSolver::
initializeInterfaces(GridFunction & cgf)  
{
//   printF("DomainSolver::initializeInterfaces:WARNING: base class function called!\n");
//  Overture::abort("error");
}

//============================================================================================
/// \brief This function is called from applyBoundaryConditions to assign some 
/// interface conditions (e.g. velocity projection for beams) that are not handled by cgmp. 
//============================================================================================
int DomainSolver::
assignInterfaceBoundaryConditions(GridFunction & cgf,
				  const int & option /* =-1 */,
				  int grid_ /* = -1 */,
				  GridFunction *puOld /* =NULL */, 
				  const real & dt /* =-1. */ )
{
//  printF("DomainSolver::assignInterfaceBoundaryConditions:WARNING: base class function called!\n");
//  Overture::abort("error");
}


// int DomainSolver::
// updateGeometryArrays(GridFunction & cgf)
// // Each solver type should update the geometry arrays it needs -- this function is called after 
// // an AMR regrid, for example.
// {
//   printF(" --- DomainSolver::updateGeometryArrays ---\n");

//   return 0;
// }


// int DomainSolver::
// updateToMatchGrid(CompositeGrid & cg)
// {
//   printF("\n $$$$$$$$$$$$$$$ DomainSolver: updateToMatchGrid(CompositeGrid & cg) $$$$$$$$$$$$\n\n");
  
//   if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
//   {
//     // dtVar : dt at each grid point for local time stepping.
//     pdtVar = new realCompositeGridFunction(cg);
//     *pdtVar=0.;
//   }

//   DomainSolver *pDomainSolverSave = pDomainSolver;
//   pDomainSolver=NULL;  // do this for now to prevent a recursive call

//   int returnValue=OB_CompositeGridSolver::updateToMatchGrid(cg);
//   pDomainSolver=pDomainSolverSave;
  
//   return returnValue;
// }

// int DomainSolver::
// userDefinedBoundaryValues(const real & t, 
// 			  realMappedGridFunction & u, 
// 			  realMappedGridFunction & gridVelocity,
// 			  const int & grid,
// 			  int side0  /* = -1 */,
// 			  int axis0  /* = -1 */,
// 			  ForcingTypeEnum forcingType /* =computeForcing */ )
// {
//   printF("DomainSolver::userDefinedBoundaryValues:ERROR: base class function called!\n");
//   Overture::abort("error");
//   return 0;
// }



void DomainSolver::
writeParameterSummary( FILE * file )
// =========================================================================================================
// /Description:
//    Output a summary of the important parameter values. 
// =========================================================================================================
{
  if ( file==parameters.dbase.get<FILE* >("checkFile") )
  {
    return;
  }

  const int buffSize=100;
  char buff[buffSize];

  Parameters::TimeSteppingMethod &timeSteppingMethod = parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");

  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
  {
    fPrintF(file," Turbulence Model : %s\n",
	    (const char*)Parameters::turbulenceModelName[(int)parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")]);
  }
  fPrintF(file,"\n"
	  " cfl = %f, tFinal=%10.4e, tPrint = %10.4e \n"
          " Slow start is %s. (slowStartCFL=%9.3e, slowStartTime=%10.4e, slowStartSteps=%i, slowStartRecomputeDt=%i).\n"
	  " Time stepping method: %s."
	  ,
	  parameters.dbase.get<real >("cfl"),
	  parameters.dbase.get<real >("tFinal"),
	  parameters.dbase.get<real >("tPrint"),
	  ((parameters.dbase.get<real >("slowStartTime")>0. || parameters.dbase.get<int  >("slowStartSteps")>0) ? "on" : "off"),
	  parameters.dbase.get<real >("slowStartCFL"),
	  parameters.dbase.get<real >("slowStartTime"),
	  parameters.dbase.get<int  >("slowStartSteps"),
	  parameters.dbase.get<int  >("slowStartRecomputeDtSteps"),
	  (const char*)parameters.getTimeSteppingName());
  

  if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::approximateFactorization )
    fPrintF(file," Approximate factorization scheme.\n");
  else if( parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")==Parameters::backwardDifferentiationFormula )
    fPrintF(file," Backward differentiation formula scheme, order=%i (BDF%i).\n",
	    parameters.dbase.get<int>("orderOfBDF"), parameters.dbase.get<int>("orderOfBDF"));
  else
    fPrintF(file,".\n");
  

  if( timeSteppingMethod==Parameters::adamsBashforth2 ||
      timeSteppingMethod==Parameters::adamsPredictorCorrector2 ||                
      timeSteppingMethod==Parameters::adamsPredictorCorrector4 ||
      timeSteppingMethod==Parameters::variableTimeStepAdamsPredictorCorrector ||
      timeSteppingMethod==Parameters::implicit )
    fPrintF(file," predictor order = %i (0=use default), useNewImplicitMethod=%i (1=eval RHS with implicit routines).\n"
	    " number of corrections=%i. \n",
            parameters.dbase.get<int >("predictorOrder"),parameters.dbase.get<int>("useNewImplicitMethod"),
            parameters.dbase.get<int>("numberOfPCcorrections") );

  fPrintF(file," recompute dt at least every %i steps.\n",parameters.dbase.get<int >("maximumStepsBetweenComputingDt"));
  int & simulateGridMotion = parameters.dbase.get<int>("simulateGridMotion");
  if( simulateGridMotion!=0 )
  {
    fPrintF(file," ++ simulateGridMotion is on: %s\n",
            (simulateGridMotion==1 ? "move grids and generate overlapping grids (Ogen) but do not solve PDE." : 
             simulateGridMotion==2 ? "move grids but do not generate overlapping grids (Ogen) and do not solve PDE." :
             "unknown!"));
  }
    
  if( parameters.dbase.get<int >("useLocalTimeStepping") )
    fPrintF(file,"  (local time stepping)\n");
  else if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")!=Parameters::noTurbulenceModel )
    fPrintF(file,"  (No local time stepping)\n");
  else
    fPrintF(file,"\n");
    
  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
  {
    fPrintF(file," use 2nd order artificial dissipation, ad21=%8.2e ad22=%8.2e\n",parameters.dbase.get<real >("ad21"),
	    parameters.dbase.get<real >("ad22"));
  }
  if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
  {
    fPrintF(file," use 4th order artificial dissipation, ad41=%8.2e ad42=%8.2e\n",parameters.dbase.get<real >("ad41"),
	    parameters.dbase.get<real >("ad42"));
  }

  fPrintF(file," Interpolation type: %s \n",(
	    parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")==Parameters::defaultInterpolationType ? 
	    "interpolate computational variables" :
	    parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")==Parameters::interpolateConservativeVariables ? 
	    "interpolate conservative variables" :
	    parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")==Parameters::interpolatePrimitiveVariables ? 
	    "interpolate primitive variables" :
	    parameters.dbase.get<Parameters::InterpolationTypeEnum >("interpolationType")==Parameters::interpolatePrimitiveAndPressure ? 
	    "interpolate primitive variables and pressure" :  "unknown"));
  
  if( parameters.dbase.get<Parameters::TurbulenceModel >("turbulenceModel")==Parameters::SpalartAllmaras )
  {
    fPrintF(file," Spalart-Allmaras eddy viscosity `n': use 2nd order artificial dissipation, "
	    "ad21n=%8.2e ad22n=%8.2e\n", parameters.dbase.get<real >("ad21n"),parameters.dbase.get<real >("ad22n"));
  }

  fPrintF(file," Order of accuracy in space = %i\n",parameters.dbase.get<int >("orderOfAccuracy"));
  fPrintF(file," Order of accuracy in time = %i\n",parameters.dbase.get<int >("orderOfTimeAccuracy"));
  fPrintF(file," Order of extrapolation for interpolation neighbours = %i\n",
	  parameters.dbase.get<int >("orderOfExtrapolationForInterpolationNeighbours"));
  fPrintF(file," Order of extrapolation for second ghost line = %i\n",
	  parameters.dbase.get<int >("orderOfExtrapolationForSecondGhostLine"));
  fPrintF(file," Order of extrapolation for outflow = %i\n", 
	  parameters.dbase.get<int >("orderOfExtrapolationForOutflow"));
  const int rwidth=parameters.dbase.get<int >("reducedInterpolationWidth");
  if( rwidth==0 )
    fPrintF(file," The interpolation width is the default value from the grid.\n");
  else
    fPrintF(file," The interpolation width has been reduced to %i.\n",rwidth);
  if( parameters.dbase.get<bool >("advectPassiveScalar") )
  {
    fPrintF(file," Advect a passive scalar with diffussion coefficient = %8.2e\n",
	    parameters.dbase.get<real >("nuPassiveScalar"));
  }
    
  if( parameters.isAxisymmetric() && parameters.dbase.get<int >("numberOfDimensions")==2 )
  {
    fPrintF(file," Solving an axisymmetric problem %sabout the %s axis\n",
	    (parameters.dbase.get<bool >("axisymmetricWithSwirl") ? "with swirl " : "" ),
	    (parameters.dbase.get<int >("radialAxis")==axis1 ? "y" : "x") );
  }
  if( timeSteppingMethod==Parameters::implicit )
  {
    fPrintF(file,"\nImplicit time stepping. Order of predictor corrector=%i\n",parameters.dbase.get<int >("orderOfPredictorCorrector"));
    fPrintF(file,"   implicit factor = %4.2f, (.5=Crank-Nicolson, 1.=Backward Euler)\n",parameters.dbase.get<real >("implicitFactor"));
    if( implicitTimeStepSolverParameters.getSolverName()!="yale" )
    {
      real rtol,atol;
      int maximumNumberOfIterations;
	
      if( implicitTimeStepSolverParameters.getSolverType()!=OgesParameters::multigrid )
      {
	implicitTimeStepSolverParameters.get(OgesParameters::THErelativeTolerance,rtol);
	implicitTimeStepSolverParameters.get(OgesParameters::THEabsoluteTolerance,atol);
	implicitTimeStepSolverParameters.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
      }
      else
      {
	OgmgParameters* ogmgPar = implicitTimeStepSolverParameters.getOgmgParameters();
	assert( ogmgPar!=NULL );
	ogmgPar->get(OgmgParameters::THEresidualTolerance,rtol);
	ogmgPar->get(OgmgParameters::THEabsoluteTolerance,atol);
	ogmgPar->get(OgmgParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
      }

      fPrintF(file,"   Implicit solver =%s, rel-tol=%8.2e, abs-tol=%8.2e max iterations=%s \n",
	      (const char*)implicitTimeStepSolverParameters.getSolverName(),rtol,atol,
	      maximumNumberOfIterations==0 ? "default" : 
	      sPrintF(buff,"%i",maximumNumberOfIterations));
	if( implicitTimeStepSolverParameters.getSolverType()==OgesParameters::multigrid )
	{ // Here is the MG convergence criteria: 
          fPrintF(file,"                         : convergence: max-defect < (rel-tol)*L2NormRHS + abs-tol.\n");
	}

    }
    else  
      fPrintF(file,"   implicit solver: %s\n",(const char *)implicitTimeStepSolverParameters.getSolverName());

  }
  if( timeSteppingMethod==Parameters::implicit || 
      timeSteppingMethod==Parameters::rKutta )
  {
    bool semiImplicitBeingUsed=false;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      if( parameters.getGridIsImplicit(grid)==0 || parameters.getGridIsImplicit(grid)==2 )
      {
	semiImplicitBeingUsed=true;
	break;
      }
    }
    if( semiImplicitBeingUsed )
    {
      fPrintF(file,"   Implicit time stepping with some grids time integrated explicitly or semi-implicitly\n");
      for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	fPrintF(file,"    %20s is time integrated %s \n",(const char *)cg[grid].getName(),
		(parameters.getGridIsImplicit(grid)==1 ? "implicitly" : 
		 (parameters.getGridIsImplicit(grid)==2 ? "semi-implicitly" :
		  "explicitly")));
	if ( timeSteppingMethod==Parameters::rKutta )
	{
	  if ( parameters.getGridIsImplicit(grid)==1 )
	  {
	    parameters.dbase.get<IntegerArray>("timeStepType")(grid)=2;
	  }
	  else if ( parameters.getGridIsImplicit(grid)==2 )
	  {
	    parameters.dbase.get<IntegerArray>("timeStepType")(grid)=1;
	  } 
	  else 
	  {
	    parameters.dbase.get<IntegerArray>("timeStepType")(grid)=0;
	  }
	}
      }
    }
    else
    {
      fPrintF(file,"   Implicit time stepping with all grids time integrated implicitly.\n");
    }
    
  }
  if( parameters.dbase.get<bool >("readRestartFile") )
    fPrintF(file," Read a restart file, restartFileName=%s \n",(const char *)parameters.dbase.get<aString >("restartFileName"));
      
  if( parameters.dbase.get<bool >("twilightZoneFlow") )
  {
    if( parameters.dbase.get<Parameters::TwilightZoneChoice >("twilightZoneChoice")==Parameters::trigonometric )
      fPrintF(file," Twilight zone flow, trigonometric polynomial, fx=%8.2e, fy=%8.2e, fz=%8.2e, ft=%8.2e\n",
	      parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[0],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[1],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[2],parameters.dbase.get<ArraySimpleFixed<real,4,1,1,1> >("omega")[3]);
    else    
      fPrintF(file," Twilight zone flow, polynomial, degree in space=%i, degree in time=%i \n",
	      parameters.dbase.get<int >("tzDegreeSpace"), 
	      parameters.dbase.get<int >("tzDegreeTime"));
  }
  
  if( parameters.dbase.get<real >("advectionCoefficient")!=1. )
    fPrintF(file," advectionCoefficient = %e \n",parameters.dbase.get<real >("advectionCoefficient"));

  if( parameters.isMovingGridProblem() )
  {
    fPrintF(file,"\n Moving grid problem: \n");
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      if( parameters.gridIsMoving(grid) )
      {
	fPrintF(file,"  Grid %15s is moving : %s\n",(const char*)cg[grid].getName(),
		(const char*)parameters.dbase.get<MovingGrids >("movingGrids").movingGridOptionName(
		  parameters.dbase.get<MovingGrids >("movingGrids").movingGridOption(grid)));
      }
    fPrintF(file,"  Frequency for full grid generation update for moving grids = %i. \n",
            parameters.dbase.get<int >("frequencyToUseFullUpdateForMovingGridGeneration"));
     
    fPrintF(file,"  Detect collisions is %s. Collision distance=%g grid-lines.\n",
          (parameters.dbase.get<bool>("detectCollisions") ? "on" : "off"),
           parameters.dbase.get<real >("collisionDistance")  );
  }
  
  Parameters::ReferenceFrameEnum & referenceFrame = 
          parameters.dbase.get<Parameters::ReferenceFrameEnum>("referenceFrame");
  fPrintF(file,"  The equations are solved in %s\n",
	  (referenceFrame==Parameters::fixedReferenceFrame ? "a fixed reference frame." :
	   referenceFrame==Parameters::rigidBodyReferenceFrame ? "a rigid body reference frame." :
	   "some other specified reference frame."));
  fPrintF(file,"\n");

  if( parameters.dbase.get<bool >("adaptiveGridProblem") )
  {
    fPrintF(file," Adaptive mesh refinement is on. ");
    if( parameters.dbase.get<bool >("useDefaultErrorEstimator") )
      fPrintF(file,"Use default error estimator. ");
    if( parameters.dbase.get<bool >("useUserDefinedErrorEstimator") )
      fPrintF(file,"Use the user defined error estimator.");
    fPrintF(file,"\n");
    fPrintF(file," Order of adaptive grid interpolation=%i \n",
            parameters.dbase.get<int >("orderOfAdaptiveGridInterpolation"));
  }

  const bool & useMovingGridSubIterations = parameters.dbase.get<bool>("useMovingGridSubIterations");
  if( useMovingGridSubIterations )
    fPrintF(file," useMovingGridSubIterations=%i (for FSI with light bodies)\n",(int)useMovingGridSubIterations);

}



void 
DomainSolver::
initializeFactorization()
{
  //  printF("DomainSolver::initializeFactorization:ERROR: base class function called!\n");
  //  Overture::abort("error");
  parameters.dbase.put< std::list<int> >("AFLimiterBoundariesToSkip");
  parameters.dbase.put< std::list<int> >("AFComponentsToSkip");
}
