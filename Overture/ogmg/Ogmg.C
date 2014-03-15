#include "Ogmg.h"
#include "TridiagonalSolver.h"
#include "ParallelUtility.h"
#include "HDF_DataBase.h"
#include "App.h"
#include "LoadBalancer.h"
#include "ParallelGridUtility.h"
#include "InterpolationData.h"

#undef ForBoundary
#define ForBoundary(side,axis)   for( axis=0; axis<mgcg[0].numberOfDimensions(); axis++ ) \
                                 for( side=0; side<=1; side++ )

#define  FOR_3(i1,i2,i3,I1,I2,I3)\
  I1Base=I1.getBase(); I2Base=I2.getBase(); I3Base=I3.getBase();\
  I1Bound=I1.getBound(); I2Bound=I2.getBound(); I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )

#define  FOR_3D(i1,i2,i3,I1,I2,I3)\
  int I1Base=I1.getBase(), I2Base=I2.getBase(), I3Base=I3.getBase(),\
  I1Bound=I1.getBound(), I2Bound=I2.getBound(), I3Bound=I3.getBound();\
  for( int i3=I3Base; i3<=I3Bound; i3++ )  \
  for( int i2=I2Base; i2<=I2Bound; i2++ )  \
  for( int i1=I1Base; i1<=I1Bound; i1++ )

#define adjustSizeMacro(x,n)			\
    while( x.getLength() < n )			\
      x.addElement();				\
    while( x.getLength() > n )			\
      x.deleteElement()

static bool applyFinishBoundaryConditions=false;
  

int Ogmg::debug=0;
int Ogmg::numberOfInstances=0; 
aString Ogmg::infoFileCaption[5]; // for caption on the table
real Ogmg::bogusRealArgument1=0.;
real Ogmg::bogusRealArgument2=0.;



OGFunction *Ogmg::pExactSolution=NULL;


//\begin{>OgmgInclude.tex}{\subsection{constructor}}
Ogmg::
Ogmg()
//==================================================================================
// /Description:
//   Default constructor.
//\end{OgmgInclude.tex} 
//==================================================================================
{
  initialized=false;  // set to true once setup is called
  numberOfInstances++;  // counts the number of instances of Ogmg
  init();
}

//\begin{>>OgmgInclude.tex}{\subsection{constructor}}
Ogmg::
Ogmg( CompositeGrid & mg, 
      GenericGraphicsInterface *ps_ /* = 0 */ ) 
: parameters(mg)
//==================================================================================
// /Description:
//   
//    Build a multigrid solver.
// /mg (input) : grid to use
// /ps\_ (input) : supply an optional GenericGraphicsInterface object for plotting.
// /Notes: 
//    Here are some notes. See the Ogmg User Guide for further details.
//  \begin{description}
//    \item[Boundary Conditions]: Ogmg looks at the coefficient matrix to determine if the ghost line
//     values are extrapolated or not. If not it assumes there is some sort of neumann or mixed boundary
//     conditions and does a few things differently.
//    \item[Interpolants]  The multigrid solver needs to interpolate at the different levels.
//      If no Interpolant is found to be associated with a CompositeGrid at a given level
//      the {\tt updateToMatchGrid} routine will build an Interpolant the first time (and update it
//      on subsequent calls). Thus if you have built an Interpolant for any level then you are responsible
//      to update it if the grid changes.
//  \end{description}
//\end{OgmgInclude.tex} 
//==================================================================================
{
  initialized=false;  // set to true once setup is called
  numberOfInstances++;  // counts the number of instances of Ogmg
  init();
  
  ps=ps_;

  updateToMatchGrid(mg);
}


void Ogmg::
init()
// =================================================================================================
// /Description: Initialize variables -- this is a common routine for all constructors
// =================================================================================================
{  
  ps=NULL;

  myid=max(0,Communication_Manager::My_Process_Number);

  orderOfAccuracy=2;
  useForcingAsBoundaryConditionOnAllLevels=false;
  
  bcSupplied=false;
  bcDataSupplied=false;

  operatorsForExtraLevels=NULL;
  interpolant=NULL;
  
  subSmoothReferenceGrid=0;
  
  numberOfExtraLevels=0;
  assumeSparseStencilForRectangularGrids=true;
  levelZero=0;
  iterationCount=NULL;
  
  v=NULL;
  leftNullVector=NULL;
  leftNullVectorIsComputed=false;
  equationToSolve=OgesParameters::userDefined;
  varCoeff=NULL;
  // for timings
  for( int i=0; i<numberOfThingsToTime; i++ )
    tm[i]=0.;

  timerGranularity=1+2+4;  // To avoid overhead in calls to getCPU, reduce this value to 3 or 1.
  totalNumberOfCoarseGridIterations=0; // counts iterations used to solve coarse grid equations

  const int np= max(1,Communication_Manager::numberOfProcessors());
  // Open debug files
  aString name="ogmg";
  if( numberOfInstances>1 )
  {
    // Give different names to debug files if we have more than 1 instance of Ogmg:
    sPrintF(name,"ogmg%i",numberOfInstances);
  }
  

  char buff[180];
  debugFile = NULL;
  infoFile  = NULL;
  checkFile = NULL;

  #ifndef USE_PPP
    debugFile = fopen(sPrintF(buff,"%s.debug",(const char*)name),"w" );      // Here is the debug file
    pDebugFile=debugFile;
    infoFile  = fopen(sPrintF(buff,"%s.info",(const char*)name),"w" );       // for convergence info
  #else
    if( myid==0 )
    {
      debugFile = fopen(sPrintF(buff,"%sNP%i.debug",(const char*)name,np),"w" );      // Here is the debug file
      infoFile  = fopen(sPrintF(buff,"%sNP%i.info",(const char*)name,np),"w" );       // for convergence info
    }
    pDebugFile= fopen(sPrintF(buff,"%sNP%i.%i.debug",(const char*)name,np,myid),"w");
  #endif


  if( checkFile==NULL && myid==0 )
    checkFile = fopen(sPrintF(buff,"%s.check",(const char*)name),"w" );      // for regression tests.

  // check file for the multigrid levels constructed by Ogmg (to compare serial/parallel results)
  // Note: this file is created in buildExtraLevels
  gridCheckFile=NULL;

  infoFileCaption[0]="";
  infoFileCaption[1]="";
  infoFileCaption[2]="";
  infoFileCaption[3]="";
  infoFileCaption[4]="";


  ogesSmoother=NULL;
  
  nipn=NULL, ndipn=NULL, ipn=NULL;  // for IBS -- could be shared amongst solvers with the same CompositeGrid.
  numberOfIBSArrays=0;

  gridName="unknownGrid";
}


// ======================================================================================================
/// /brief Destructor.
// ======================================================================================================
Ogmg::
~Ogmg()
{
  CompositeGrid & mgcg = multigridCompositeGrid();

  if( initialized )
  {
    delete [] operatorsForExtraLevels;

    // delete all the pointers to the tri-diagonal solvers.
    for( int level=0; level<mgcg.numberOfMultigridLevels(); level++ )
    {
      for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
      {
	for( int dir=0; dir<3; dir++ )
	  delete tridiagonalSolver[level][grid][dir];
      
	delete [] tridiagonalSolver[level][grid];
      }
    
      delete [] tridiagonalSolver[level];

      // *wdh* 091230 -- the grid collections also keep references to the Interpolant so we should be
      // careful about deleting them
      if( interpolant!=NULL && interpolant[level]->decrementReferenceCount()==0 )
	delete interpolant[level];  
    }
    delete [] tridiagonalSolver;

    delete [] interpolant; 
  }

  delete v;
  delete leftNullVector;
  delete varCoeff;
  
  delete [] ogesSmoother;

  // delete arrays used for IBS smoothing
  if( ipn!=NULL )
  {
    for( int i=0; i<numberOfIBSArrays; i++ )
      delete [] ipn[i];

    delete [] ipn;
  }
  delete [] nipn;
  delete [] ndipn;

  if( true )
  { 
    // This should now work even if we have mutiple instances of Ogmg: 
    // OLD: this fails if multiple Ogmg objects are created at the same time.

    if( myid==0 )
    {
      fclose(debugFile);
      fclose(infoFile);
      fclose(checkFile);
    }
    
    #ifdef USE_PPP
      fclose(pDebugFile);
    #endif
  }
  
  if( gridCheckFile!=NULL )
  {
    fclose(gridCheckFile);
  }


}


//\begin{>>OgmgInclude.tex}{\subsection{setPlotStuff}}
void Ogmg::
set( GenericGraphicsInterface *ps_ )
//==================================================================================
// /Description:
//    Supply a GenericGraphicsInterface object to use for plotting.
// /ps\_ (input) : pointer to a GenericGraphicsInterface object.
//\end{OgmgInclude.tex} 
//==================================================================================
{
  ps=ps_;
}

// ======================================================================================================
/// /brief Set the name for the composite grid. 
///  /param name (input) : name for the composite grid (used for labels for e.g.)
// ======================================================================================================
void Ogmg::
setGridName( const aString & name )
{
  gridName=name;
}

// ======================================================================================================
/// /brief Set the name of this instance of Ogmg (for info in debug files etc.)
///  /param name (input) : name for this instnace of Ogmg.
// ======================================================================================================
void Ogmg::
setSolverName( const aString & name )
{
  solverName=name;
  if( gridName == "" ) gridName="unknown";
  if( debugFile!=NULL )
  {
    fPrintF(debugFile,"===== Ogmg is named: %s, Grid: %s =====\n",(const char*)solverName,(const char*)gridName);
  }
  #ifdef USE_PPP
  if( pDebugFile!=NULL )
  {
    fprintf(pDebugFile,"===== Ogmg is named: %s, Grid: %s =====\n",(const char*)solverName,(const char*)gridName);
  }
  #endif
  if( infoFile !=NULL )
  {
    fPrintF(infoFile,"===== Ogmg is named: %s, Grid: %s =====\n",(const char*)solverName,(const char*)gridName);
  }
}


//! Assign an option.
/*!
  /param option==assumeSparseStencilOnRectangularGrids : assume the operator is a 5-point operator (2D)
     or 7-point operator (3D) on a rectangular grid.
 */
int Ogmg::
setOption(OptionEnum option, bool trueOrFalse )
{
  switch (option)
  {
    case assumeSparseStencilOnRectangularGrids:
      assumeSparseStencilForRectangularGrids=trueOrFalse;
      break;
  default:
    cout << "Ogmg::setOption:ERROR: unknown option = " << option << endl;
    return 1;
  }
  return 0;
}


// =======================================================================================
/// \brief  Supply a MultigridCompositeGrid to use.
/// \param mgcg (input) : use this object to hold the multigrid hierarchy. 
/// \details The MultigridCompositeGrid object can be used to share a multigrid hierarchy 
///  amongst different applications and means that the coarse grid levels need only be
/// generated once. 
// =======================================================================================
void Ogmg::
set( MultigridCompositeGrid & mgcg )
{
  multigridCompositeGrid.reference(mgcg);
}

static int numberOfBuildExtraLevelsMessages=0;
static int numberOfDoNotBuildExtraLevelsMessages=0;

//\begin{>>OgmgInclude.tex}{\subsection{updateToMatchGrid}}
void Ogmg::
updateToMatchGrid( CompositeGrid & mg_ )
//==================================================================================
// /Description:
//    Update the solver to match this grid.
// /mg (input) : grid to use
//\end{OgmgInclude.tex} 
//==================================================================================
{
  real time0=getCPU();
  
  // Allocate space for the CompositeGrid: (if not already done)
  multigridCompositeGrid.allocate();

  CompositeGrid & mgcg = multigridCompositeGrid();
  const bool multigridHierachyWasBuiltElseWhere = multigridCompositeGrid.isGridUpToDate();  // the MG hierarchy may have already been built 
  

  if( debug & 2 )
  {
    printF("\n ++++++++++++++ Entering Ogmg::updateToMatchGrid ++++++++++++++\n");
  }

  const int numberOfMultigridLevelsOld = mgcg.numberOfMultigridLevels();
  const int numberOfComponentGridsOld = mgcg.numberOfComponentGrids();

  if( mg_.numberOfComponentGrids()>0 )
  { // choose the order of accuracy automatically
    orderOfAccuracy = mg_[0].discretizationWidth(0)==5 ? 4 : 2;
    parameters.set(OgmgParameters::THEorderOfAccuracy,orderOfAccuracy);
    
    if( debug & 4 && orderOfAccuracy==4 )
      printF("Ogmg::updateToMatchGrid::INFO: Setting orderOfAccuracy=%i\n",orderOfAccuracy);
  }
  
  numberOfExtraLevels=0;
  if( parameters.readMultigridCompositeGrid )
  {
    // read the grid with extra levels from a file
    printF("Reading the multigrid composite grid with extra levels from file=%s\n",
           (const char*)parameters.nameOfMultigridCompositeGrid);
    #ifdef USE_PPP
    
      if( parameters.loadBalancer==NULL )
      {
	if( mg_->pLoadBalancer!=NULL )
	{
	  printF("Ogmg::INFO: using loadBalancer from input CompositeGrid\n");
	  parameters.loadBalancer=mg_->pLoadBalancer;
	}
	else
	{
	  parameters.loadBalancer = new LoadBalancer;
	  // NOTE: when there is only 1 grid then the default load-balancer will always use all-to-all
	  parameters.loadBalancer->setLoadBalancer(LoadBalancer::KernighanLin);
	}
      }

      LoadBalancer & loadBalancer = *parameters.loadBalancer;
      getFromADataBase(mgcg,parameters.nameOfMultigridCompositeGrid,loadBalancer);
    #else
      bool loadBalance=true;
      getFromADataBase(mgcg,parameters.nameOfMultigridCompositeGrid,loadBalance);
    #endif


    if( false )
    {
      fPrintF(debugFile,"\n +++ After reading: mgcg.numberOfMultigridLevels=%i\n",mgcg.numberOfMultigridLevels());
      const int numberOfComponentGrids =mgcg.numberOfComponentGrids();
      for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
      {
	CompositeGrid & cgl = mgcg.multigridLevel[l];
	fPrintF(debugFile,"\n **level=%i : cgl interp data state=%i**\n",l,(int)cgl->localInterpolationDataState);
	::display(cgl.numberOfInterpolationPoints,"cgl.numberOfInterpolationPoints",debugFile);
	for( int grid=0; grid<numberOfComponentGrids; grid++ )
	{
	  ::display(cgl.interpoleeGrid[grid],sPrintF("l=%i, grid=%i, cgl.interpoleeGrid",l,grid),debugFile);
	}
      }
    }
    

    if( debug & 8 && Communication_Manager::Number_Of_Processors >1 )
    { // display the parallel distribution
      mgcg.displayDistribution("Ogmg: read MG with levels: mgcg",stdout);
      // GridDistributionList & gdl0 = mgcg.multigridLevel[0]->gridDistributionList;
      // gdl0.display("Ogmg: after read: mgcg.multigridLevel[0]->gridDistributionList");
    }

    // --- Now reference the MappedGrids to the finest level of the input CompositeGrid mg_ ---
    for( int grid=0; grid<mg_.numberOfComponentGrids(); grid++ )
    {
      mgcg[grid].reference(mg_[grid]);
    }
    
    mgcg.update( CompositeGrid::THEmultigridLevel);
    mgcg.update(MappedGrid::THEmask);
    numberOfExtraLevels=mgcg.numberOfMultigridLevels()-1;

   #ifdef USE_PPP
    GridDistributionList & gdl0 = mgcg.multigridLevel[0]->gridDistributionList;
    // gdl0.resize(mg_.numberOfComponentGrids());
    // gdl0.display("mgcg.multigridLevel[0]->gridDistributionList");
    for( int grid=0; grid<mg_.numberOfComponentGrids(); grid++ )
    {
      // -- update distributions: 
      mgcg->gridDistributionList[grid]=mg_->gridDistributionList[grid];
      gdl0[grid]=mg_->gridDistributionList[grid];
    }

    if( debug & 2 )
    {
      printF("After REFERENCING to MASTER: mgcg.numberOfMultigridLevels=%i\n",mgcg.numberOfMultigridLevels());
      if( Communication_Manager::Number_Of_Processors >1 )
      { // display the parallel distribution
	mgcg.displayDistribution("Ogmg: read MG with levels: mgcg",stdout);
      }
    }
    
    // printF("OGMG: numberOfInterpolationPoints size=%i\n",mgcg.numberOfInterpolationPoints.getLength(0));
    // printF("OGMG: numberOfInterpolationPointsLocal size=%i\n",mgcg->numberOfInterpolationPointsLocal.getLength(0));

    // --- Create the local interpolation arrays ---
    const int numberOfComponentGrids =mgcg.numberOfComponentGrids();
    for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
    {
      CompositeGrid & cgl = mgcg.multigridLevel[l];

      // ---- Get a copy of the interpolation data arrays from current MG level live on this processor:
      InterpolationData *interpData=NULL;
      ParallelGridUtility::getLocalInterpolationData( cgl, interpData );

      if( l==0 )
      {
	// --- is this needed ?

	mgcg->localInterpolationDataState=CompositeGridData::localInterpolationDataForAll;

        // total number of grids over all levels
	const int numGridsAllLevels = numberOfComponentGrids*mgcg.numberOfMultigridLevels();  
	adjustSizeMacro(mgcg->interpolationPointLocal,numGridsAllLevels);
	adjustSizeMacro(mgcg->interpoleeGridLocal,numGridsAllLevels);
	adjustSizeMacro(mgcg->variableInterpolationWidthLocal,numGridsAllLevels);
	adjustSizeMacro(mgcg->interpoleeLocationLocal,numGridsAllLevels);
	adjustSizeMacro(mgcg->interpolationCoordinatesLocal,numGridsAllLevels);

	mgcg->numberOfInterpolationPointsLocal.redim(numGridsAllLevels);
      }

      cgl->localInterpolationDataState=CompositeGridData::localInterpolationDataForAll;
      adjustSizeMacro(cgl->interpolationPointLocal,numberOfComponentGrids);
      adjustSizeMacro(cgl->interpoleeGridLocal,numberOfComponentGrids);
      adjustSizeMacro(cgl->variableInterpolationWidthLocal,numberOfComponentGrids);
      adjustSizeMacro(cgl->interpoleeLocationLocal,numberOfComponentGrids);
      adjustSizeMacro(cgl->interpolationCoordinatesLocal,numberOfComponentGrids);

      cgl->numberOfInterpolationPointsLocal.redim(numberOfComponentGrids);
      for( int grid=0; grid<numberOfComponentGrids; grid++ )
      {
        InterpolationData & ipd = interpData[grid];

        cgl->numberOfInterpolationPointsLocal(grid)=ipd.numberOfInterpolationPoints;
        cgl->interpolationPointLocal[grid].reference(ipd.interpolationPoint);
        cgl->interpoleeGridLocal[grid].reference(ipd.interpoleeGrid);
        cgl->variableInterpolationWidthLocal[grid].reference(ipd.variableInterpolationWidth);
        cgl->interpoleeLocationLocal[grid].reference(ipd.interpoleeLocation);
        cgl->interpolationCoordinatesLocal[grid].reference(ipd.interpolationCoordinates);
      }

      if( false )
      {
	for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
	{
	  fprintf(pDebugFile,"\n -- level=%i, grid=%i, nig=%i\n",l,grid,cgl->numberOfInterpolationPointsLocal(grid));
	  ::display(cgl->interpoleeGridLocal[grid],"cgl->interpoleeGridLocal",pDebugFile);
	  ::display(cgl->interpolationPointLocal[grid],"cgl->interpolationPointLocal",pDebugFile);
          fflush(pDebugFile);
	}
	
      }

      // sort the local interp. arrays and build the interpolationStartEndIndex array: 
      ParallelGridUtility::sortLocalInterpolationPoints(cgl);



      delete [] interpData;
    }
   #endif    

  }
  else 
  {
    if( mg_.numberOfMultigridLevels()==1 )
    {
      if( !multigridCompositeGrid.isGridUpToDate() ) // check if the multigrid hierachy needs to be built
      {
	// *******************************************
	// ***** Automatically build extra levels ****
	// *******************************************

        const int numberOfPossibleMultigridLevels = mg_.numberOfPossibleMultigridLevels();

        numberOfBuildExtraLevelsMessages++;
	if( numberOfBuildExtraLevelsMessages<5 )
	{
	  printF("\n MMMMMMMMMMMMMMMM  Ogmg: call buildExtraLevels: max MG levels = %i (1=no coarser levels) MMMMMMMMMMMMMMMMMMMMMMM\n",
		 numberOfPossibleMultigridLevels );
	}
	else if( numberOfBuildExtraLevelsMessages==5 )
	{
	  printF(" MMMMMMMMMMMMMMMM Ogmg: Too many  Ogmg: call buildExtraLevels ... I will not print anymore\n");
	}
	
	numberOfExtraLevels=1;
	buildExtraLevels(mg_);  // This builds the CompositeGrid mgcg

        multigridCompositeGrid.setGridIsUpToDate(true);  // multigrid hierachy is now up to date.
      }
      else
      {
        // The multigridCompositeGrid is already up to date. Just use the one that is there.
	numberOfDoNotBuildExtraLevelsMessages++;
	if( numberOfDoNotBuildExtraLevelsMessages<5 )
	{
	  printF("\n MMMMMMMMMMMMMMMM  Ogmg: do NOT call buildExtraLevels (grid is up to date) MMMMMMMMMMMMMMMMMM\n");
	}
	else if( numberOfDoNotBuildExtraLevelsMessages==5 )
	{
          printF(" MMMMMMMMMMMMMMMM Ogmg: Too many do NOT call buildExtraLevels ... I will not print anymore\n");
	}
	
        numberOfExtraLevels=mgcg.numberOfMultigridLevels()-1;
      }
      

      if( true && parameters.readMultigridCompositeGrid )
      {
	// read the grid with extra levels from a file
	printF("Reading the multigrid composite grid with extra levels from file=%s\n",
	       (const char*)parameters.nameOfMultigridCompositeGrid);

        CompositeGrid mgcg2;
	getFromADataBase(mgcg2,parameters.nameOfMultigridCompositeGrid);
	printF("After reading: mgcg2.numberOfMultigridLevels=%i\n",mgcg2.numberOfMultigridLevels());
	mgcg2.update(MappedGrid::THEmask);

        assert( mgcg.numberOfGrids()==mgcg2.numberOfGrids() );
	assert( mgcg.numberOfMultigridLevels()==mgcg2.numberOfMultigridLevels() );

	assert( max(abs(mgcg.numberOfInterpolationPoints()-mgcg2.numberOfInterpolationPoints()))==0 );
	assert( max(abs(mgcg.interpolationStartEndIndex-mgcg2.interpolationStartEndIndex))==0 );
	display(mgcg.interpolationStartEndIndex,"mgcg.interpolationStartEndIndex");
	display(mgcg2.interpolationStartEndIndex,"mgcg2.interpolationStartEndIndex");
	
	assert( max(abs(mgcg.multigridCoarseningRatio-mgcg2.multigridCoarseningRatio))==0 );
	
        // mgcg2=mgcg;

	for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
	{
          printF(" *** Check level=%i\n",l);
	  CompositeGrid & cg1 = mgcg.multigridLevel[l];
	  CompositeGrid & cg2 = mgcg2.multigridLevel[l];
	  
          assert( cg1.numberOfGrids()==cg2.numberOfGrids() );
          assert( cg1.numberOfBaseGrids()==cg2.numberOfBaseGrids() );
	  assert( max(abs(cg1.interpolationWidth()-cg2.interpolationWidth()))==0 );
	  assert( max(abs(cg1.interpolationIsImplicit()-cg2.interpolationIsImplicit()))==0 );
	  assert( abs(cg1.interpolationIsAllExplicit()-cg2.interpolationIsAllExplicit())==0 );
	  assert( abs(cg1.interpolationIsAllImplicit()-cg2.interpolationIsAllImplicit())==0 );
          display(cg1.interpolationStartEndIndex,"cg1.interpolationStartEndIndex");
          display(cg2.interpolationStartEndIndex,"cg2.interpolationStartEndIndex");
	  
	  assert( max(abs(cg1.interpolationStartEndIndex-cg2.interpolationStartEndIndex))==0 );
          // cg2.interpolationStartEndIndex=cg1.interpolationStartEndIndex;
	  
          assert( max(abs(cg1.numberOfInterpolationPoints()-cg2.numberOfInterpolationPoints()))==0 );
          ::display(cg1.numberOfInterpolationPoints(),"cg1.numberOfInterpolationPoints()");
	  ::display(cg2.numberOfInterpolationPoints(),"cg2.numberOfInterpolationPoints()");

          ::display(cg1.numberOfImplicitInterpolationPoints(),"cg1.numberOfImplicitInterpolationPoints()");
	  ::display(cg2.numberOfImplicitInterpolationPoints(),"cg2.numberOfImplicitInterpolationPoints()");
          // assert( max(abs(cg1.numberOfImplicitInterpolationPoints()-cg2.numberOfImplicitInterpolationPoints()))==0 );

	  for( int grid=0; grid<cg1.numberOfComponentGrids(); grid++ )
	  {
	    ::display(cg1.interpolationCoordinates[grid],"cg1.interpolationCoordinates[grid]","%4.1f ");
	    ::display(cg1.interpolationCoordinates[grid],"cg2.interpolationCoordinates[grid]","%4.1f ");
	    
	    if( cg1.numberOfInterpolationPoints(grid)>0 )
	    {
	      assert( max(abs(cg1[grid].discretizationWidth()-cg2[grid].discretizationWidth()))==0 );
	      assert( max(abs(cg1.interpolationPoint[grid]-cg2.interpolationPoint[grid]))==0 );
	      assert( max(abs(cg1.interpoleeGrid[grid]-cg2.interpoleeGrid[grid]))==0 );
	      assert( max(abs(cg1.variableInterpolationWidth[grid]-cg2.variableInterpolationWidth[grid]))==0 );
	      assert( max(fabs(cg1.interpolationCoordinates[grid]-cg2.interpolationCoordinates[grid]))==0 );
	      assert( max(abs(cg1[grid].mask()-cg2[grid].mask()))==0 );
	    }
	    
	  }
	}

	// if( true ) Overture::abort("done");
        mgcg=mgcg2;

      }

    }
    else
    {
      mgcg.reference(mg_);
      mgcg.update( CompositeGrid::THEmultigridLevel);
      mgcg.update(MappedGrid::THEmask);
      numberOfExtraLevels=mgcg.numberOfMultigridLevels()-1;
    }
    
  }
  if( parameters.saveMultigridCompositeGrid && !parameters.readMultigridCompositeGrid )
  {
    // save the grid with extra levels to a file
    printF("Saving the multigrid composite grid with extra levels to file=%s\n",
           (const char*)parameters.nameOfMultigridCompositeGrid);

    mgcg.saveGridToAFile(parameters.nameOfMultigridCompositeGrid,"mgcg");

    if( false )
    {
      printF("Ogmg::After saving: mgcg.numberOfMultigridLevels=%i, numberOfGrids=%i\n",mgcg.numberOfMultigridLevels(),
	     mgcg.numberOfGrids());

      mgcg->gridDistributionList.display("Ogmg::After saving");
    }
    

    if( Communication_Manager::Number_Of_Processors >1 )
    { // display the parallel distribution
      mgcg.displayDistribution("Ogmg: save MG with levels: mgcg",stdout);
    }


    if( Ogmg::debug & 4 ) // check that we can read the MG back in
    {
      for( int grid=0; grid<mgcg.numberOfGrids(); grid++ )
      {
	::display(mgcg.interpolationPoint[grid],sPrintF("mgcg.interpolationPoint[%i]",grid));        
      }

      CompositeGrid mgcg2;
      printF("\nReading the multigrid composite grid with extra levels from file=%s\n",
	     (const char*)parameters.nameOfMultigridCompositeGrid);

      getFromADataBase(mgcg2,parameters.nameOfMultigridCompositeGrid);
      
      printF("\n >>>>After reading: mgcg2.numberOfMultigridLevels=%i\n",mgcg2.numberOfMultigridLevels());
      mgcg2.update(MappedGrid::THEmask);
      mgcg2.updateReferences();
      printF("<<< mgcg2.numberOfGrids()=%i, mgcg2.numberOfComponentGrids()=%i, "
	     "mgcg2.interpolationPoint.getLength=%i\n",
	     mgcg.numberOfGrids(),mgcg.numberOfComponentGrids(),mgcg.interpolationPoint.getLength());

      
      for( int grid=0; grid<mgcg2.numberOfGrids(); grid++ )
      {
//	::display(mgcg2.interpolationPoint[grid],sPrintF("mgcg2.interpolationPoint[%i]",grid));        
      } 

      ::display(mgcg.interpolationStartEndIndex(),"mgcg.interpolationStartEndIndex()");
      ::display(mgcg2.interpolationStartEndIndex(),"mgcg2.interpolationStartEndIndex()");

      for( int l=0; l<mgcg2.numberOfMultigridLevels(); l++ )
      {
	CompositeGrid & cgl = mgcg2.multigridLevel[l];
        // cgl.updateReferences();

	printF("*****level=%i cgl.numberOfComponentGrids()=%i, "
	       "cgl.interpolationPoint.getLength=%i\n",
	       l,cgl.numberOfComponentGrids(),cgl.interpolationPoint.getLength());

        ::display(mgcg.multigridLevel[l].interpolationStartEndIndex(),"mgcg[l].interpolationStartEndIndex()");
        ::display(cgl.interpolationStartEndIndex(),"cgl.interpolationStartEndIndex()");

	::display(cgl.numberOfInterpolationPoints(),"cgl.numberOfInterpolationPoints()");
	for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
	{
// 	  ::display(cgl.interpoleeGrid[grid],"cgl.interpoleeGrid[grid]");
// 	  ::display(cgl.variableInterpolationWidth[grid],"cgl.variableInterpolationWidth[grid]");
//	  ::display(cgl.interpolationPoint[grid],"cgl.interpolationPoint[grid]");        
// 	  ::display(cgl.interpoleeLocation[grid],"cgl.interpoleeLocation[grid]");        
// 	  ::display(cgl.interpolationCoordinates[grid],"cgl.interpolationCoordinates[grid]");  
	}
      }
      
    }
    
  }
  

  // *wdh* 100409 CompositeGrid & mg =  mgcg; 

  // displaySmoothers("Ogmg::updateToMatchGrid(cg) before parameters.updateToMatchGrid(mg)");

  parameters.updateToMatchGrid(mgcg);

  if( parameters.numberOfMultigridLevels() < mgcg.numberOfMultigridLevels() ||
      parameters.numberOfComponentGrids()  < mgcg.numberOfComponentGrids()  )
  {
    printF("Ogmg::updateToMatchGrid:ERROR: after update parameters -- parameters are NOT consistent with mgcg\n");
    printF(" parameters.numberOfMultigridLevels()=%i, mgcg.numberOfMultigridLevels()=%i\n",
	   parameters.numberOfMultigridLevels(),mgcg.numberOfMultigridLevels());
    
    OV_ABORT("error");
  }
  // displaySmoothers("Ogmg::updateToMatchGrid(cg) after parameters.updateToMatchGrid(mg)");

  if( !initialized )
    setup(mgcg);
  
  bool numberOfMultigridLevelsHasChanged= 
                          mgcg.numberOfMultigridLevels() > 0 &&
                          (mgcg.numberOfMultigridLevels()!=numberOfMultigridLevelsOld);   // fixed 100331 
   
  bool numberOfComponentGridsHasChanged = 
                          mgcg.numberOfMultigridLevels() > 0 &&
                     (mgcg.numberOfComponentGrids()!=numberOfComponentGridsOld);   // fixed 100331 

  if( numberOfMultigridLevelsHasChanged )
  {
    interpolantWasCreated.resize(mgcg.numberOfMultigridLevels());
    if( mgcg.numberOfMultigridLevels()>numberOfMultigridLevelsOld )
    {
      Range R(numberOfMultigridLevelsOld,mgcg.numberOfMultigridLevels()-1);
      interpolantWasCreated(R)=false;
    }
    
    workUnits.redim(mgcg.numberOfMultigridLevels());
    workUnits=0.;       

    // number of times to iterate on each level (=1 -> V-cycle, 2=W-cycle)
//     numberOfIterations.resize(mgcg.numberOfMultigridLevels());
//     numberOfIterations(R)=1;

  }
  if( numberOfMultigridLevelsHasChanged || numberOfComponentGridsHasChanged )
  {
    lineSmoothIsInitialized.resize(3,mgcg.numberOfComponentGrids(),mgcg.numberOfMultigridLevels());

    // create arrays of pointers for the tridiagonal solvers   ****** delete these *****
    //      tridiagonalSolver[level][grid][axis]
    typedef TridiagonalSolver* TRIP;
    typedef TRIP* TRIPP;
    typedef TRIPP* TRIPPP;
    typedef TRIPPP* TRIPPPP;
  
    tridiagonalSolver = new TRIPPP[mgcg.numberOfMultigridLevels()];
    for( int level=0; level<mgcg.numberOfMultigridLevels(); level++ )
    {
      tridiagonalSolver[level] = new TRIPP[mgcg.numberOfComponentGrids()];
      for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	tridiagonalSolver[level][grid]= new TRIP[3];
        for( int dir=0; dir<3; dir++ )
          tridiagonalSolver[level][grid][dir]=NULL;
      }
    }

  }  

  lineSmoothIsInitialized=false;  // assume that all grids have changed

  // *wdh* 100409 mgcg.reference(mg);


  defectMG.updateToMatchGrid(mgcg);  // allocate space for the defect

  // Holds defect ratios for auto-subSmooth
  defectRatio.redim(mgcg.numberOfComponentGrids(),mgcg.numberOfMultigridLevels());  
  defectRatio=-1.;  // This means the ratios have not been computed yet
  
// *** move   uOld.updateToMatchGrid(mgcg.multigridLevel[0]);  // used in solve **** why is this in the class ? ****
  
  // Make sure there is an interpolant at each level (except the last if we use a Oges)
  // -- the grid collections keep a reference to the Interpolant so we should be careful *wdh* 091230 
  const int mgLevels = mgcg.numberOfMultigridLevels();

  // --- Create the Interpolants as needed -- 
  // *new way 100331*

  if( !multigridHierachyWasBuiltElseWhere )
  {
    // We only need to build Interpolants if we have created the MG hierarchy here. Otherwise some other
    // application (i.e. another instance of Ogmg) has already created Interpolants we can use.

    if( interpolant==NULL )
    {
      interpolant = new Interpolant* [mgLevels];
      for( int level=0; level<mgLevels; level++ )
	interpolant[level]=NULL;
    }
    else if( mgcg.numberOfMultigridLevels() > numberOfMultigridLevelsOld )
    {
      // There are more multigrid levels than before : build a new array of pointers to Interpolants
      // (If there are fewer levels than before we just keep the extra levels of Interpolants so we do need to delete any)
      Interpolant **temp = new Interpolant* [mgLevels];

      for( int level=0; level<mgLevels; level++ )
	temp[level]=NULL;

      for( int level=0; level<numberOfMultigridLevelsOld; level++ )
	temp[level]=interpolant[level];

      // for( int level=mgLevels; level<numberOfMultigridLevelsOld; level++ )
      // { // delete extra interpolants : *WARNING there may be a problem in parallel in deleting POGI objects *wdh* 100331
      //   assert( interpolant[level]!=NULL );
      //   if( interpolant[level]->decrementReferenceCount()==0 )
      //   {
      // 	delete interpolant[level]; interpolant[level]=NULL;
      //   }
      // }
      delete [] interpolant;  // delete old array of pointers
      interpolant=temp;
    }
    for( int level=0; level<mgLevels; level++ )
    {
      if( interpolant[level]==NULL )
      {
	if( level==0 && mg_->interpolant!=NULL )
	{ // The input CompositeGrid already has an Interpolant we can use 
	  interpolant[level]=mg_->interpolant;
	  interpolant[level]->incrementReferenceCount();
	  // We can probably get rid of this next array since we reference count Interpolants: ************
	  interpolantWasCreated(level)=false;    // this means the Interpolant was NOT created by Ogmg
	}
	else
	{
	  // create a new interpolant for this level
	  // assert( mgcg.multigridLevel[level]->interpolant==NULL );  // this must be true I think 
	  if( mgcg.multigridLevel[level]->interpolant!=NULL )
	  {
	    printF("Ogmg:updateToMatchGrid:WARNING:mgcg.multigridLevel[level]->interpolant!=NULL --"
                   "this should only happen when testing\n");
	  }
	  interpolant[level]= new Interpolant;             
	  interpolant[level]->incrementReferenceCount();
	  interpolantWasCreated(level)=true;  // this means the Interpolant WAS created by Ogmg
	}
      
      }
      // Now update the Interpolant *NOTE* This is not needed if only the coefficients change!! *fix me*
      if( true || mgcg.multigridLevel[level]->interpolant==NULL || level>0 )  // ** for testing do this *****************************************
      {
	interpolant[level]->updateToMatchGrid(mgcg.multigridLevel[level]); 
//       if( level==0 )
//       {
//         mgcg->interpolant=interpolant[level];
//         mg_->interpolant=interpolant[level];
//       }
      
      }
    
      // Note if Interpolant is explicit on the top level then we can use explicit *fix me*
      // if( level>0 )
      interpolant[level]->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
      if( false )
      {
	::display(mgcg.multigridLevel[level].numberOfInterpolationPoints(),
		  sPrintF("Ogmg:updateTMG: level=%i cg.numberOfInterpolationPoints()",level));
      }
    


    }
  }
  

//   // ********** there is no need to rebuild the Interpolant's if the number of multigrid levels hasn't changed *fix me*
//   // ********** For time stepping problems there is no need to rebuild the grids if only the coefficients have changed ****
//   if( interpolant!=NULL )
//   {
//     for( int level=0; level<numberOfMultigridLevelsOld; level++ )
//     {
//       if( interpolantWasCreated(level) )
//       {
// 	if( interpolant[level]->decrementReferenceCount()==0 )
// 	{
// 	  delete interpolant[level]; interpolant[level]=NULL;
// 	}
// 	if( mgcg.multigridLevel[level]->interpolant!=NULL && mgcg.multigridLevel[level]->interpolant->decrementReferenceCount()==0 )
// 	{
// 	  delete mgcg.multigridLevel[level]->interpolant;
// 	  mgcg.multigridLevel[level]->interpolant=NULL;
// 	}
//       }
//     }
//     delete [] interpolant;
//     interpolant=NULL;
//   }
  
//   assert( interpolant==NULL );  

//   delete [] interpolant;
//   interpolant = new Interpolant* [mgLevels];
//   for( int level=0; level<mgLevels; level++ )
//   {
//     interpolant[level]= new Interpolant;             
//     interpolant[level]->incrementReferenceCount();  
//     if( mgcg.multigridLevel[level]->interpolant==NULL || interpolantWasCreated(level) )
//     {
//       if( debug & 4 )
//         printF("***********Ogmg: creating the Interpolant at level=%i \n",level);
//       interpolantWasCreated(level)=true;
//       interpolant[level]->updateToMatchGrid(mgcg.multigridLevel[level]);    // object used for interpolation
//       // display(mgcg.interpolationPoint[0],"mgcg.interpolationPoint[0]");
//       // display(mgcg.multigridLevel[level].interpolationPoint[0],"mgcg.multigridLevel[level].interpolationPoint[0]");
      
//       // 40904: bug found with interpolate -- opt implicit iteration was not use the variableInterpolationWidth
//       //   and caused bad values to appear. -- fixed made to gf/interpOpt.bf etc. 
//       if( true ) // ***********************
//         interpolant[level]->setImplicitInterpolationMethod(Interpolant::iterateToInterpolate);
//     }
//   }
  
  // numberOfGridPoints: total number of grid points.
  numberOfGridPoints=0;
//   for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
//     numberOfGridPoints+=mgcg[grid].mask().elementCount();
  
  // --- Determine which grid to use as a "reference" grid for variable sub smooths ---
  //  The reference grid uses 1 sub-smooth (usually) and normally should be a Cartesian
  //  with the most grid points. 

  // == We could compute gridMax once and for all in an initialization step ==
  int gridMax=0;         // grid with the most grid-points
  int maxGridPoints=0;   // number of grid-points in gridMax
  int gridMaxCartesian=-1;         // Cartesian grid with most points
  int maxGridPointsCartesian=0;   // number of grid-points in gridMaxCartesian
  for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = mgcg[grid];
    const IntegerArray & gid = mg.gridIndexRange();
    // int numGridPoints=(gid(1,0)-gid(0,0)+1)*(gid(1,1)-gid(0,1)+1)*(gid(1,2)-gid(0,2)+1);

    int numGridPoints=mg.mask().elementCount();
    numberOfGridPoints +=numGridPoints;

    if( numGridPoints>maxGridPoints )
    {
      maxGridPoints=numGridPoints;
      gridMax=grid;
    }
    if( mg.isRectangular() && numGridPoints>maxGridPointsCartesian )
    {
      maxGridPointsCartesian=numGridPoints;
      gridMaxCartesian=grid;
    }
  }
  if( parameters.subSmoothReferenceGrid>=0 && parameters.subSmoothReferenceGrid<mgcg.numberOfComponentGrids() )
  {
    // user has specified which grid to use for the reference grid:
    subSmoothReferenceGrid=parameters.subSmoothReferenceGrid;
  }
  else
  {
    // sub-smooth reference grid is determined automatically: 
    subSmoothReferenceGrid=gridMax;
    if( gridMaxCartesian>=0 && maxGridPointsCartesian > .25*maxGridPoints )
    {
      subSmoothReferenceGrid=gridMaxCartesian;
    }
  }
  
  if( debug & 4 )
    printF("Ogmg:updateToMatchGrid:INFO: subSmoothReferenceGrid=%i (gridMax=%i, gridMaxCartesian=%i)\n",
	   subSmoothReferenceGrid,gridMax,gridMaxCartesian);


  if( mgcg.numberOfMultigridLevels()==1 && !parameters.useDirectSolverForOneLevel )
  {
    // If there is only one level then iterate with a line smoother
    parameters.useDirectSolverOnCoarseGrid=false;
    parameters.setSmootherType(OgmgParameters::alternatingLineJacobi);
    // parameters.setSmootherType(OgmgParameters::redBlack);
  }
  
  // defectMG.updateToMatchGrid(mgcg);  // allocate space for the defect

  InterpolateParameters interpParams(mgcg.numberOfDimensions());
  interpParams.setInterpolateOrder(parameters.coarseToFineTransferWidth);
  interp.initialize(interpParams);  


  active.redim(mgcg.numberOfComponentGrids());
  active=true;  // set active(grid)=false if we do not need to solve for the solution on this grid.

  tm[timeForInitialize]+=getCPU()-time0;
}


void Ogmg::
setup(CompositeGrid & mg )
// ================================================================================================
// /Description:
//   Initialize Ogmg for the first time a CompositeGrid is available. This routine must only be called
//  once.
// ================================================================================================
{
  assert( !initialized );
  initialized=true;
  
  // Here are guesses at the relative times to perform operations
  timeForAddition=1.;
  timeForMultiplication=1.;
  timeForDivision=4.;
  // workUnit = number of operations for a Jacobi Smooth
  if( mg.numberOfDimensions()==2 )
    workUnit=10*timeForMultiplication+9*timeForAddition+timeForDivision;    // non-rectangular
  else
    workUnit=28*timeForMultiplication+27*timeForAddition+timeForDivision;


  numberOfSolves=0;
  sumTotalWorkUnits=0.;
  totalNumberOfCycles=0;
  averageEffectiveConvergenceRate=0.;

  numberOfIterations=0;
  numberOfCycles=0;
  totalWorkUnits=0.;
  
  workUnits.redim(mg.numberOfMultigridLevels());
  workUnits=0.;       

  width1=orderOfAccuracy+1;
  width2 = mg.numberOfDimensions()>1 ? 3 : 1;
  width3 = mg.numberOfDimensions()>2 ? 3 : 1;
  halfWidth1=width1/2;
  halfWidth2=width2/2;
  halfWidth3=width3/2;

  lineSmoothIsInitialized.redim(3,mg.multigridLevel[0].numberOfComponentGrids(),mg.numberOfMultigridLevels());
  lineSmoothIsInitialized=false;

  // we keep track if we have built Interpolants so that we can update them in updateToMatchGrid
  interpolantWasCreated.redim(mg.numberOfMultigridLevels());
  interpolantWasCreated=false;
  interpolant=NULL;
  
  // create arrays of pointers for the tridiagonal solvers   ****** delete these *****
  //      tridiagonalSolver[level][grid][axis]
  typedef TridiagonalSolver* TRIP;
  typedef TRIP* TRIPP;
  typedef TRIPP* TRIPPP;
  typedef TRIPPP* TRIPPPP;
  
  tridiagonalSolver = new TRIPPP[mg.numberOfMultigridLevels()];
  for( int level=0; level<mg.numberOfMultigridLevels(); level++ )
  {
    tridiagonalSolver[level] = new TRIPP[(int&)mg.multigridLevel[level].numberOfComponentGrids()];
    for( int grid=0; grid<mg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
      tridiagonalSolver[level][grid]= new TRIP[3];
      for( int dir=0; dir<3; dir++ )
        tridiagonalSolver[level][grid][dir]=NULL;
    }
    
  }

  if( mg.numberOfDimensions()==3 )
  {
    // In 3D choose an iterative solver for the coarse grid (if the parameters do not seem to have been changed)
    real relativeTolerance=0.;
    parameters.ogesParameters.get(OgesParameters::THErelativeTolerance,relativeTolerance);
    if( relativeTolerance==0. )
    { 
      // If the relativeTolerance has not been set then we set a reasonable value and also set the solver type.
      relativeTolerance=1.e-3;  // reduce residual by this amount.
      
      if( debug & 1 )
	printF("Ogmg::setup: coarse grid solver: choose THEbestIterativeSolver with relativeTolerance=%e\n",
	       relativeTolerance);
    
      parameters.ogesParameters.set(OgesParameters::THEbestIterativeSolver);

      parameters.ogesParameters.set(OgesParameters::THErelativeTolerance,relativeTolerance);
    }
    
  }

}

// ============================================================================================
/// \brief Return the order of extrapolation to use for a given order of accuracy and level 
// ============================================================================================
int Ogmg::
getOrderOfExtrapolation( const int level ) const
{
  // We have to be careful will some grids (e.g. valvee4.order2.ml3) where there may be only
  // one interior point and then we can only extrapolate to order 2
  if( level==levelZero )
    return orderOfAccuracy==2 ? 3 : 4;
  else
    return orderOfAccuracy;  // try this *wdh* 100118 
}


//\begin{>>OgmgInclude.tex}{\subsection{getMaximumResidual}} 
real Ogmg::
getMaximumResidual() const
//===================================================================================
// /Description: 
//   Return the maximum residual from the last solve
//\end{OgmgInclude.tex}
//===================================================================================
{
  return maximumResidual;
}

//\begin{>>OgmgInclude.tex}{\subsection{getNumberOfIterations}} 
int Ogmg::
getNumberOfIterations() const
//===================================================================================
// /Description: 
//   Return the number of multigrid iterations (cycles).
// /Return value: the number of iterations.
//\end{OgmgInclude.tex}
//===================================================================================
{
  return numberOfIterations;
}



//\begin{>>OgmgInclude.tex}{\subsection{sizeOf}} 
real Ogmg:: 
sizeOf( FILE *file /* =NULL */ ) const 
//===================================================================================
// /Description: 
//   Return number of bytes allocated by Ogmg; Optionally print detailed info to a file
//
// An estimate of space requirements is  $20N$ (2D) or $40N$ (3D) where $N$ is the number of
//  grid points on the finest grid (assumes maximum number of levels so that $.5+.25+.125+...=1$.
// For line smoothers there is addition space required.
//
// /file (input) : optionally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{OgmgInclude.tex}
//===================================================================================
{

  real size=0.;

  size+=sizeof(*this);

  real uMGSize=0., fMGSize=0., cMGSize=0., defectMGSize=0., uOldSize=0., mgcgSize=0., operatorsSize=0., interpolantSize=0.,
    tridSize=0., directSize=0., sizeIBS=0.;
  

  // In some grid functions, don't count finest level since we share
  if( uMG.numberOfComponentGrids()>0 )
  {
    uMGSize=uMG.sizeOf()-uMG.multigridLevel[0].sizeOf(); // 1N
    size+=uMGSize;
  }
  if( fMG.numberOfComponentGrids()>0 )
  {
    fMGSize=fMG.sizeOf()-fMG.multigridLevel[0].sizeOf(); // 2N
    size+=fMGSize;
  }
  if( cMG.numberOfComponentGrids()>0 )
  {
    // If user supplies the coeff matrix then we just keep a ref to the finest level
    if( equationToSolve==OgesParameters::userDefined && cMG.numberOfMultigridLevels()>1 )
      cMGSize=cMG.sizeOf()-cMG.multigridLevel[0].sizeOf(); // 9N or 27N
    else
      cMGSize=cMG.sizeOf();
    size+=cMGSize;
  }
  
  defectMGSize=defectMG.sizeOf();                           // 2N
  size+=defectMGSize;
  uOldSize=uOld.sizeOf();       // uOld only lives on the fine grid     // 1N
  size+=uOldSize;

  if( v!=NULL )
    size+=v->sizeOf();

  int level;
  if( operatorsForExtraLevels!=NULL )
  {
    operatorsSize=0.;
    for( int extraLevel=0; extraLevel<numberOfExtraLevels; extraLevel++ ) // is this correct?
      operatorsSize+=operatorsForExtraLevels[extraLevel].sizeOf();  
    size+=operatorsSize;
    
  }

  if( !multigridCompositeGrid.isNull() )
  {
    const CompositeGrid & mgcg = multigridCompositeGrid();

    if( numberOfExtraLevels>=0 )
    {
      // count the size in mgcg -- the grids on level 0 are referenced.
      real sizeOfReference=0.;
      for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	sizeOfReference+=mgcg[grid].sizeOf();
      }
      mgcgSize=mgcg.sizeOf()-sizeOfReference;  // subtract off the duplicate space
      size+=mgcgSize;  // mask, center, jacobian =    2*7N or 2*13N  // don't count finest level if we can share
      //                             total =  29N  59N
    }
  
    for( level=0; level<mgcg.numberOfMultigridLevels(); level++ )
    {
      if( interpolantWasCreated(level) )
	interpolantSize=interpolant[level]->sizeOf(); 
      size+=interpolantSize;
      tridSize=0.;
      for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	for( int axis=0; axis<mgcg.numberOfDimensions(); axis++ )
	{
	  if( lineSmoothIsInitialized(axis,grid,level) && tridiagonalSolver[level][grid][axis]!=NULL ) 
	    tridSize+=tridiagonalSolver[level][grid][axis]->sizeOf();
	}
      }
      size+=tridSize;
    }
    if( parameters.useDirectSolverOnCoarseGrid )
    {
      directSize=directSolver.sizeOf();
      size+=directSize;
    }
  
    // count arrays used for IBS smoothing
    if( ipn!=NULL )
    {
      for( int i=0; i<numberOfIBSArrays; i++ )
	sizeIBS+=ndipn[i];
      sizeIBS*=mgcg.numberOfDimensions()*sizeof(int);

      size+=sizeIBS;
    }
  }
  
  if( file!=NULL && myid==0 )
  {
    const real meg=1024.*1024.; // 1e6;
    fPrintF(file," Ogmg::sizeOf: uMG=%6.1f M, fMG=%6.1f M, cMG=%6.1f M, defectMG=%6.1f M, uOld=%6.1f M, mgcg=%6.1f M, \n"
	    "                    operators=%6.1f M, interpolant=%6.1f M, trid=%6.1f M, direct=%6.1f M, IBS=%6.1f M\n"
	    "                 ** total = %6.1f M \n",
	    uMGSize/meg,fMGSize/meg,cMGSize/meg,defectMGSize/meg,uOldSize/meg,mgcgSize/meg,
	    operatorsSize/meg,interpolantSize/meg,
	    tridSize/meg,directSize/meg,sizeIBS/meg,
            size/meg);
  }
  
  return size;
}

//\begin{>>OgmgInclude.tex}{\subsection{setOgmgParameters}}
int  Ogmg::
setOgmgParameters(OgmgParameters & parameters_ )
//==================================================================================
// /Description:
// set parameters equal to another parameter object.
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
//  assert( initialized );
//  parameters_.initializeGridDependentParameters(cg);
  parameters=parameters_;
  return 0;
}

//\begin{>>OgmgInclude.tex}{\subsection{fullMultiGrid}}
int Ogmg::
fullMultigrid()
//==================================================================================
// /Description:
//    Perform the first full multigrid cycle (nested iteration) in order to obtain an initial guess.
//
//
//                        X
//                       /
//                  X   X
//                 / \ /
//                X   X
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  real time=getCPU();
//    if( debug & 2 ) 
//    {
//      int level=0;
//      defect(level);
//      real maximumDefect=maxNorm(defectMG.multigridLevel[level]);
//      printF("Entering fullMultigrid(), maximum defect=%8.2e...\n",maximumDefect);
//    }
  
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int iteration=0;
  int level=0;
  fullMultigridWorkUnits=0.;  // count work used in fullMultigrid

  // average the RHS to all coarser levels
  bool transferForcing=true;
  for( level=0; level<parameters.numberOfMultigridLevels()-1; level++ )
  {
    defectMG.multigridLevel[level]=fMG.multigridLevel[level];  // we could avoid this copy with an option in fineToCoarse
    fineToCoarse(level,transferForcing); 

    fullMultigridWorkUnits+=.25/(level==0? 1. : pow(pow(2.,double(mgcg.numberOfDimensions())),double(level)));
  }
  
  if( Ogmg::debug & 4 )
  {
    level=parameters.numberOfMultigridLevels()-1;
    fMG.multigridLevel[level].display(sPrintF(buff,"fullMultigrid rhs for coarse level=%i",level),
        debugFile,"%9.1e");
  }

  real maximumDefect=-1.;
  for( level=parameters.numberOfMultigridLevels()-1; level>0; level-- )
  {
    if( !parameters.useDirectSolverOnCoarseGrid || directSolver.isSolverIterative() )
      uMG.multigridLevel[level]=0.;  // initial guess for iterative solvers on the coarse level.

    const int numberOfSubCycles=1;
    cycle(level,iteration,maximumDefect,numberOfSubCycles);  // solve on this level using MG
    fullMultigridWorkUnits+=workUnits(level)/pow(pow(2.,double(mgcg.numberOfDimensions())),double(level));
    if( false && level==1 )
    {
      cycle(level,iteration,maximumDefect,numberOfSubCycles);  // solve on this level using MG
      fullMultigridWorkUnits+=workUnits(level)/pow(pow(2.,double(mgcg.numberOfDimensions())),double(level));
    }
    
    
    if( ps!=0 && (debug & 8) && ps->isGraphicsWindowOpen() )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"level=%i u during Full MG.",level)); 
      ps->erase();
      PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
      defect(level);
      psp.set(GI_TOP_LABEL,sPrintF(buff,"level=%i defect during Full MG.",level)); 
      ps->erase();
      PlotIt::contour(*ps,defectMG.multigridLevel[level],psp);
      psp.set(GI_TOP_LABEL,sPrintF(buff,"level=%i RHS during Full MG.",level)); 
      ps->erase();
      PlotIt::contour(*ps,fMG.multigridLevel[level],psp);
    }

    uMG.multigridLevel[level-1]=0.;

    levelZero=level-1;          // for boundary conditions at level-1
    const int coarseToFineTransferWidthSave = parameters.coarseToFineTransferWidth;
    // increase the accuracy of the interpolation: 
    parameters.coarseToFineTransferWidth = min(orderOfAccuracy+1,coarseToFineTransferWidthSave+1); 
    coarseToFine(level-1);      // transfer coarse grid to fine grid at level-1
    parameters.coarseToFineTransferWidth=coarseToFineTransferWidthSave;  // reset 

    fullMultigridWorkUnits+=.125/pow(pow(2.,double(mgcg.numberOfDimensions())),double(level));
    

    if( Ogmg::debug & 4 )
    {
      uMG.multigridLevel[level].display(sPrintF(buff,"fullMultigrid u from cycle for level=%i",level),
        debugFile,"%9.1e");

      uMG.multigridLevel[level-1].display(sPrintF(buff,"fullMultigrid u from coarseToFine for level=%i",level-1),
        debugFile,"%9.1e");
    }
  }    
  levelZero=0;
  if( false )
  { // could do a final smooth but is it worth it?
    level=0;
    int numberOfSmoothingSteps=2;
    smooth(level,numberOfSmoothingSteps,iteration);
  }
  if( debug & 2 )
  {
    level=0;
    defect(level);
    real maximumDefect=maxNorm(defectMG.multigridLevel[level]);
    printF("...leaving fullMultigrid(). maximum defect=%8.2e (work units=%8.2e)\n\n",
                maximumDefect,fullMultigridWorkUnits);
  }
  if( ps!=0 && (debug & 8) && ps->isGraphicsWindowOpen() )
  {
    level=0;
    psp.set(GI_TOP_LABEL,sPrintF(buff,"level=%i u after Full MG.",level)); 
    ps->erase();
    PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
    defect(level);
    psp.set(GI_TOP_LABEL,sPrintF(buff,"level=%i defect after Full MG.",level)); 
    ps->erase();
    PlotIt::contour(*ps,defectMG.multigridLevel[level],psp);
  }
  
  tm[timeForFullMultigrid]+=getCPU()-time;
  return 0;
}



//\begin{>>OgmgInclude.tex}{\subsection{solve}}
int Ogmg::
solve( realCompositeGridFunction & u, realCompositeGridFunction & f)
//==================================================================================
// /Description:
//   Solve Au=f with multigrid
//
// /u (input/output) : initial guess on input, answer on output. It is NOT necessary
//    that u be defined on all multigrid levels. u may only live on finest level.
//    It is best if u satisfies the boundary conditions on input although this is not required.
// /f (input) : right hand side for the problem. f should be defined for the finest level.
//    As with u, f can only be defined on the finest level if desired.
//\end{OgmgInclude.tex} 
//==================================================================================
{
  real time=getCPU();

  CompositeGrid & mgcg = multigridCompositeGrid();
  if( debug & 2 )
    printF(" ******** Ogmg::solve, solver=%s, grid=%s********\n",(const char*)solverName,(const char*)gridName);
  
  assert(parameters.gridDependentParametersInitialized);

  directSolver.setOgesParameters(parameters.ogesParameters);  // why is this done here ? do only once?

  CompositeGrid & m = *u.getCompositeGrid();
//   if( true )
//   {
//     printF("Ogmg:solve:  m.numberOfMultigridLevels()=%i\n",m.numberOfMultigridLevels());
//     ::display(m.numberOfInterpolationPoints(),"Ogmg:solve: m.numberOfInterpolationPoints()");
//     ::display(mgcg.numberOfInterpolationPoints(),"Ogmg:solve: mgcg.numberOfInterpolationPoints()");
//     ::display(mgcg.multigridLevel[0].numberOfInterpolationPoints(),"Ogmg:solve: mgcg.multigridLevel[0].numberOfInterpolationPoints()");

//     if( uMG.numberOfMultigridLevels()>0 )
//     {
//       CompositeGrid & cg = *uMG.multigridLevel[0].getCompositeGrid();
//       ::display(cg.numberOfInterpolationPoints(),"Ogmg:solve:(0) uMG.multigridLevel[0].getCompositeGrid().numberOfInterpolationPoints()");
//     }
    
//   }

  if( m.numberOfMultigridLevels()>1 )
  {
    uMG.reference(u);   // reference to local versions
    fMG.reference(f);
  }
  else
  {
    // input grid functions only live on the finest level
    uMG.updateToMatchGrid(mgcg);
    fMG.updateToMatchGrid(mgcg);
 
    // uMG.multigridLevel[0].dataCopy(u);  // **** could avoid a copy *****
    // fMG.multigridLevel[0].dataCopy(f);

    // 100401 -- this fixes the bug in parallel after calling mgSolver.updateToMatchGrid(cg) after a first solve
    //   ---> uMG.multigridLevel[0].getCompositeGrid()->numberOfInterpolationPoints() was bad
    if( true ) 
    {
      for( int level=0; level<mgcg.numberOfMultigridLevels(); level++ )
        uMG.multigridLevel[level].updateToMatchGrid(mgcg.multigridLevel[level]);   // why is this needed?? *fix me*
    }
    

    for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
    {
      uMG[grid].reference(u[grid]);
      uMG.multigridLevel[0][grid].reference(u[grid]);  // need to do this too ?
      fMG[grid].reference(f[grid]);
      fMG.multigridLevel[0][grid].reference(f[grid]);  // need to do this too ?
    }

  }
  
//   if( true )
//   {
//     CompositeGrid & cg0 = *uMG.getCompositeGrid();
//     ::display(cg0.numberOfInterpolationPoints(),"Ogmg:solve:(1) uMG.getCompositeGrid().numberOfInterpolationPoints()");
//     CompositeGrid & cg = *uMG.multigridLevel[0].getCompositeGrid();
//     ::display(cg.numberOfInterpolationPoints(),"Ogmg:solve:(1) uMG.multigridLevel[0].getCompositeGrid().numberOfInterpolationPoints()");
//   }


  if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    uOld.updateToMatchGrid(mgcg.multigridLevel[0]);  // used in solve **** why is this in the class ? ****

  
  if( parameters.numberOfMultigridLevels() < mgcg.numberOfMultigridLevels() ||
      parameters.numberOfComponentGrids()  < mgcg.numberOfComponentGrids()  )
  {
    if( Ogmg::debug & 4 )
      printF("Ogmg::solve: update parameters -- CompositeGrid has apparently changed\n");
    parameters.updateToMatchGrid(mgcg);
  }

  // Check the validity of parameters
  checkParameters();


//   if( true )
//   {
//     CompositeGrid & cg = *uMG.multigridLevel[0].getCompositeGrid();
//     ::display(cg.numberOfInterpolationPoints(),"Ogmg:solve:(2) uMG.multigridLevel[0].getCompositeGrid().numberOfInterpolationPoints()");
//   }

//   if( false && parameters.problemIsSingular )
//   {
//     cout << "Ogmg::solve: problem is singular-- adding 1 to f for testing...\n";
//     f+=1.;
//   }
  if( parameters.problemIsSingular )
  {
    if( debug & 2 )
      printF("***Ogmg::solve: problem is singular, directSolver.setCompatibilityConstraint \n");

    directSolver.set(OgesParameters::THEcompatibilityConstraint,true);
    parameters.ogesParameters.set(OgesParameters::THEcompatibilityConstraint,true);

    createNullVector();

    // alpha and v are used for trying out a different way to project singular problems
    // in addAdjustmentForSingularProblem
    if( alpha.getLength(0)<m.numberOfMultigridLevels() )
    {
      alpha.redim(m.numberOfMultigridLevels());
      alpha=0.;
    }
    
    if( false )
    {

      if( v==NULL )
      {
	// v is used to project the singular problem -- it should approximately satisfy Lv = - r
	CompositeGrid & cg = *u.getCompositeGrid();
      
	v = new realCompositeGridFunction(cg);
	realCompositeGridFunction & vv=*v;
	for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	{
	  realArray & x = cg[grid].center();

	  // this is v for a square: Delta v = -1
	  Range all;
	  vv[grid]= .25*( x(all,all,all,0)*(1.-x(all,all,all,0)) + x(all,all,all,1)*(1.-x(all,all,all,1)) );
	
	}
      }
    }
    
  }

  if( parameters.outputMatlabFile )
   cycleResults.redim(parameters.maximumNumberOfIterations+2,numberOfCycleResults+mgcg.numberOfComponentGrids());
    
  if( parameters.cycleType==OgmgParameters::cycleTypeF )
  { // initialize the iterationCount[level] for an F-cycle
    if( iterationCount==NULL )
    {
      iterationCount= new int[mgcg.numberOfMultigridLevels()];
    }
    
    for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )
    {
      iterationCount[l]=0;
    }
  }

  RealArray error(parameters.maximumNumberOfIterations+2);
  real maximumDefect=-1., maximumDefectOld=1.;

  // Get initial defect here and save to outputResults as "iteration==0"
  if( debug & 1 || parameters.outputMatlabFile ) 
  {
    int level=0;
    defect(level);
    maximumDefect=maxNorm(defectMG.multigridLevel[level]);

    if( !parameters.useFullMultigrid )
      computeDefectRatios(level);
    
    if( debug & 2 )
      printF("Solve: maximum defect at start=%8.2e...\n",maximumDefect);
  }

  // Compute the l2-norm of the RHS for checking convergence
  l2NormRightHandSide=1.;
  if( parameters.convergenceCriteria==OgmgParameters::residualConverged )
  {
    l2NormRightHandSide=l2Norm(f);
  }
  

  if( true )
  {
    // output initial results
    int level=0;
    int iteration=0;
    real defectNew=maximumDefect, defectOld=maximumDefect;
    outputResults(level,iteration,maximumDefect,defectNew,defectOld );
  }

  // For fourth-order we need to initially set some values at ghost points.
  applyInitialConditions();

  int maximumNumberOfIterations = parameters.maximumNumberOfIterations;
  if( mgcg.numberOfMultigridLevels()==1 && parameters.useDirectSolverOnCoarseGrid )
  {
    // -- if there is only one multigrid level and we are using the direct solver then we
    //    only need to use one cycle:
    maximumNumberOfIterations=1;
    // -- we should also set the tolerance on the coarse grid to use the global tolerance (instead of the
    //    coarse grid tol)
    real tol=0.;
    parameters.get(OgmgParameters::THEresidualTolerance,tol);
    if( tol<=0. ) tol=1.e-8; // what should this be?
    directSolver.set(OgesParameters::THErelativeTolerance,tol);
    directSolver.set(OgesParameters::THEmaximumNumberOfIterations,1000);    
    printF("Ogmg:WARNING: There is only one multigrid level on this grid! Using the direct solver instead"
           " with tol=%8.2e\n",tol);
  }
  

  int level=0;
  real defectConvergenceRate,errorConvergenceRate,errorEstimate;
  bool hasConverged=false;
  for( int iteration=0; iteration<maximumNumberOfIterations; iteration++ )
  {
    real timeI=getCPU();

    if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    {
      uOld.dataCopy(uMG.multigridLevel[0]);   // why does uOld have to be in the class ??????????????????????????????
      tm[timeForMiscellaneous]+=getCPU()-timeI;
    }
    
    if( iteration==0 && parameters.useFullMultigrid )
    {
      fullMultigrid();   // obtain an initial guess through a nested iteration from the coarse level
    }
    else
      fullMultigridWorkUnits=0.;  // count work used in fullMultigrid

    cycle(level,iteration,maximumDefect,parameters.maximumNumberOfIterations);

    if( maximumDefect<0. )
    {
      printF("******Ogmg:solve:ERROR: maximumDefect not set by cycle! ******\n");
    }

    real time0=getCPU();
    if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    {
      error(iteration)=l2Error(uMG.multigridLevel[0],uOld);  
      tm[timeForMiscellaneous]+=getCPU()-time0;
    }
    else
    {
      error(iteration)=0.;
    }
    
    numberOfCycles++;
    
    // In order to be able to test the convergence criteria on the first iteration
    // we make a rough guess for the errorConvergenceRate.
    real errorScale=REAL_EPSILON*100.;  // should scale by uNorm
    
    if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    {
      if( iteration>0 )
	errorConvergenceRate=error(iteration)/max(error(iteration-1), errorScale);
      else
	errorConvergenceRate=.9;  // this is a guess. Could do better here.
      errorEstimate=error(iteration)*errorConvergenceRate/(1.-errorConvergenceRate);
    }
    else
    {
      errorEstimate=0.;
    }
    
    if( iteration>0 )
    {
	
      defectConvergenceRate=maximumDefect/maximumDefectOld;
      if( Ogmg::debug & 4 )
      {
        timeI=getCPU()-timeI;  // cpu time for this iteration.
	
        printF("Ogmg:solve: e%i=%7.1e, e%i/e%i=%5.3f, err est=%7.1e, "
               " d%i=%7.1e, d%i/d%i=%6.4f, ECR=%5.2f ***\n",
	       iteration,error(iteration),iteration,iteration-1,errorConvergenceRate,errorEstimate,
	       // -timeI/log(max(DBL_MIN,defectConvergenceRate)),
	       iteration,maximumDefect,iteration,iteration-1,defectConvergenceRate,
	       pow(defectConvergenceRate,1./(max(1.,workUnits(0)))));
      }
    }
    else
    {
      if( Ogmg::debug & 4)
      {
	printF("Ogmg:solve: iteration=%i, maxDefect=%8.2e (tol=%8.2e), errorEstimate=%8.2e (tol=%8.2e)\n",
	       iteration,maximumDefect,parameters.residualTolerance*numberOfGridPoints,
	       fabs(errorEstimate),parameters.errorTolerance);
      }
    }
    

    numberOfIterations=iteration+1;


    // ================================================================
    // ==================== Check for Convergence =====================
    // ================================================================
    
    if( parameters.convergenceCriteria==OgmgParameters::residualConverged )
    {
      // NOTE: Ogmg l2Norm is scaled by the number of points 
      real l2NormResidual = maximumDefect;  // this is an upper bound ******************************** FIX ME **********
      const real resTol = parameters.residualTolerance*l2NormRightHandSide + parameters.absoluteTolerance;

      if( debug & 2 ) 
	printF("   ->solve: (level=0) it=%i, l2NormResidual=%8.2e <? %8.2e = resTol*l2Norm(f) + aTol"
	       "=(%7.1e)*(%7.1e) + %7.1e \n",
	       iteration+1, l2NormResidual, resTol, parameters.residualTolerance,l2NormRightHandSide,parameters.absoluteTolerance);

      if( l2NormResidual < resTol )
      {
        hasConverged=true;
        break;  // we have converged based on the residual convergence criteria
      }

    }
    else if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    {

      if( debug & 2 ) 
	printF("   ->solve: (level=0) it=%i, maxDefect=%8.2e, max(u-uOld)=%8.2e, errorEstimate=%8.2e <? errTol=%8.2e\n",
	       iteration+1, maximumDefect, error(iteration), fabs(errorEstimate), parameters.errorTolerance);
      
      if( fabs(errorEstimate) < parameters.errorTolerance )
      {
        hasConverged=true;
	break;  // we have converged based on the error estimate
      }
      
    }
    else if( parameters.convergenceCriteria==OgmgParameters::residualConvergedOldWay )
    {
      // *** OLD WAY to check for convergence ***

      if( debug & 2 ) 
	printF("   ->solve: (level=0) it=%i, maxDefect=%8.2e <? resTol*pts"
	       "=(%7.1e)*(%i)=%7.1e diff=%7.1e err est=%7.1e <? %7.1e\n",
	       iteration+1, maximumDefect, parameters.residualTolerance,numberOfGridPoints,
	       parameters.residualTolerance*numberOfGridPoints,error(iteration),fabs(errorEstimate),parameters.errorTolerance);

      if( maximumDefect < parameters.residualTolerance*numberOfGridPoints && 
	  ( fabs(errorEstimate) < parameters.errorTolerance || iteration==0 ) )
      {
        hasConverged=true;
	break;
      }
    }
    else
    {
      OV_ABORT("Ogmg:ERROR: unknown value for parameters.convergenceCriteria");
    }
    
    
    maximumDefectOld=maximumDefect;
    
  }

  if( !hasConverged )
  {
    printF("****Ogmg::solve:WARNING: No convergence in %i iterations defect=%8.2e ****\n",
	   parameters.maximumNumberOfIterations,maximumDefect);
  }

  removeAdjustmentForSingularProblem(0,0);

  // For fourth-order we need to set some values on the 2nd ghost line at the end
  applyFinalConditions();

  if( m.numberOfMultigridLevels()==1 )
  {
    // u=uMG.multigridLevel[0];
    // f=fMG.multigridLevel[0];
    assign( u,uMG.multigridLevel[0] );
    assign( f,fMG.multigridLevel[0] );  // why do we set f ??
  }
  maximumResidual=maximumDefect;
  
  outputCycleInfo();

  totalNumberOfCycles+=numberOfCycles;
  sumTotalWorkUnits+=totalWorkUnits;
  averageEffectiveConvergenceRate += pow( totalResidualReduction, 1./max(1.,totalWorkUnits) );
  numberOfSolves++;

  tm[timeForSolve]+=getCPU()-time;
  return 0;
}

//\begin{>>OgmgInclude.tex}{\subsection{cycle}}
int Ogmg::
cycle(const int & level, const int & iteration, real & maximumDefect, const int & numberOfCycleIterations )
//==================================================================================
// /Description:
//   Perform a multigrid cycle. This routine is called recursively.
//
// /maximumDefect (input/output) : on input (if non-negative) 
//     this is the current maximum defect, on output this is the new  maximum defect.
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();

  workUnits(level)=0.;      // counts total work on this level and higher levels for this cycle.
  if( level==0 && iteration==0 )
    workUnits(level)=fullMultigridWorkUnits;
  
  real defectOld,defectNew=REAL_MAX*.1;

  defectOld=maximumDefect;
  if( iteration==0 ) defectRatio(Range(),level)=-1.;  // this means it is not computed
  
  if( defectOld<0. && ( (level==0 && (debug & 2)) || debug & 4 || parameters.showSmoothingRates) )
  {
    // compute the initial defect if we are printing convergence rates:
    defect(level);
    defectOld= maxNorm(defectMG.multigridLevel[level]);
    computeDefectRatios(level);  // for auto-sub-smooth
  }

  addAdjustmentForSingularProblem(level,iteration);
  
  bool useTrueCycle=false;  // if false we sometimes do pre+post smooths all at once
  if( level<mgcg.numberOfMultigridLevels()-1 )
  {


    if( iteration==0 || useTrueCycle )
    {
      // On the first iteration we apply a pre-smooth, otherwise we 
      // combine pre and post smooths in the post smooth.

      // printF("level=%i, iteration=%i, number of presmooths=%i \n",level,iteration,parameters.numberOfSmooths(0,level));
      
      // *wdh* 030429 smooth(level,parameters.numberOfSmooths(0,level),iteration);
      // for testing we sometimes smooth lots of times on the first step:
      int numberOfSmoothingSteps=parameters.numberOfSmooths(0,level);
      if( iteration==0 && level==0 ) 
        numberOfSmoothingSteps = max(numberOfSmoothingSteps,parameters.minimumNumberOfInitialSmooths);
      smooth(level,numberOfSmoothingSteps,iteration);

      // printF(">>>>>>>>>>>level=%i, iteration=%i, number of PRE  smooths=%i, numberOfCycles=%i\n",
      //       level,iteration,numberOfSmoothingSteps,parameters.numberOfCycles(level));

      if( ps!=0 && (debug & 8 ) && ps->isGraphicsWindowOpen() )
      {
	psp.set(GI_TOP_LABEL,sPrintF(buff,"cycle: it=%i, level=%i, after initial smooth.",iteration,level)); 
	ps->erase();
	PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
      }
    }
    

    // uMG.multigridLevel[level].applyBoundaryCondition(0,BCTypes::extrapolateInterpolationNeighbours);
    defect(level);
    workUnits(level)+=1.; 
    computeDefectRatios(level);  // for auto-sub-smooth

    fineToCoarse(level);   // compute f[level+1] = Restriction of the defect
    workUnits(level)+=.25; 

    assign(uMG.multigridLevel[level+1],0.);

    int numberOfSubCycles=parameters.numberOfCycles(level+1);
    if( parameters.cycleType==OgmgParameters::cycleTypeF )
    { // here is an F cycle:
      //  level+1== 1 : perform 2,2,2,2,......  subcycles
      //            2 : perform 2,1,2,1,......  subcycles
      //            3 : perform 2,1,1,2,1,1,... subcycles
      
      numberOfSubCycles=(iterationCount[level+1] % (level+1)) == 0 ? 2 : 1;
      iterationCount[level+1]++;
    }
    if( (level+1)==mgcg.numberOfMultigridLevels()-1 ) 
      numberOfSubCycles=1;  // only 1 iteration on the coarset level

    if( debug & 16 && parameters.cycleType==OgmgParameters::cycleTypeF )
      printF("Before cycle(level+1=%i): iterationCount[level+1]=%i numberOfSubCycles=%i\n",level+1,iterationCount[level+1],numberOfSubCycles);
    
    for( int subIteration=0; subIteration<numberOfSubCycles; subIteration++)
    {
      cycle(level+1,subIteration,maximumDefect,numberOfSubCycles);      // solve for u[level+1] (approximately)
      workUnits(level)+=workUnits(level+1)/pow(2.,double(mgcg.numberOfDimensions()));
    }

    coarseToFine(level);   // correct u[level]
    workUnits(level)+=.125; 

    // ** if this is not the last iteration at this level then do both pre and post smooths at once.
    int numberOfSmoothingSteps=useTrueCycle || (level>0 && iteration>=(numberOfCycleIterations-1)) ? 
      parameters.numberOfSmooths(1,level) :  parameters.numberOfSmooths(0,level)+parameters.numberOfSmooths(1,level);
//      int numberOfSmoothingSteps=useTrueCycle || (level>0 && iteration>=(parameters.numberOfCycles(level)-1)) ? 
//        parameters.numberOfSmooths(1,level) :  parameters.numberOfSmooths(0,level)+parameters.numberOfSmooths(1,level);

    // printF("<<<<<<<<<<<level=%i, iteration=%i, number of post smooths=%i, numberOfCycles=%i\n",
    //        level,iteration,numberOfSmoothingSteps,parameters.numberOfCycles(level));
    
    smooth(level,numberOfSmoothingSteps,iteration+1);

    if( ps!=0 && (debug & 8 ) && ps->isGraphicsWindowOpen() )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"cycle: it=%i, level=%i, after smooth.",iteration,level)); 
      ps->erase();
      PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
    }

  }
  else
  { // Coarse grid solution: 
    real time=getCPU();
    if( debug & 8 )
      fMG.multigridLevel[level].display("RHS for coarse grid solve",debugFile,"%8.1e ");
    
    // Oges::debug=63;
    if( parameters.useDirectSolverOnCoarseGrid )
    {
      if( debug & 4 )
	printF("%*.1s  ***direct solve on level %i, iteration=%i\n",level*4," ",level,iteration);
        
//    if( directSolver.isSolverIterative() ) 
//    {
//      real tol=REAL_EPSILON*100.;
//      directSolver.set(OgesParameters::THEpreconditioner,OgesParameters::incompleteLUPreconditioner);
//      directSolver.set(OgesParameters::THErelativeTolerance,max(tol,REAL_EPSILON*10.));
//      directSolver.set(OgesParameters::THEmaximumNumberOfIterations,10000);
//    } 

//       if( directSolver.isSolverIterative() ) 

      // Oges::debug=63;
      if( false &&  // values at extra equations are set to zero by default
          parameters.problemIsSingular )
      { // Added: *wdh* 100605 
	real value=0.; // what should this be ?
	directSolver.setExtraEquationValues( fMG.multigridLevel[level],&value ); 
      }
      
      directSolver.solve( uMG.multigridLevel[level],fMG.multigridLevel[level] );
      workUnits(level)+=orderOfAccuracy+1;  // estimate work required for back-substitution ?????

      if( directSolver.isSolverIterative() )
      {
	int numIt=directSolver.getNumberOfIterations();
	totalNumberOfCoarseGridIterations+= numIt;
	if( debug & 4 )
	{
	  printF("%*.1s Level=%i, cycle=%i : number of iterations to solve coarse grid equations=%i.\n",
		 level*4," ",level,numberOfCycles,numIt);
	}
      }
      
      if( debug & 8 )
	uMG.multigridLevel[level].display(sPrintF("u after coarse grid solve, level=%i",level),debugFile,"%8.1e ");
    }
    else
    {
      int numberOfSmoothingSteps=max( parameters.numberOfIterationsOnCoarseGrid,
                                      parameters.numberOfSmooths(0,level)+parameters.numberOfSmooths(1,level));
      totalNumberOfCoarseGridIterations+=numberOfSmoothingSteps;
      if( debug & 4 )
	printF(" ***iterate to solve coarse grid problem on level %i. Perform %i smooths\n",
               level,numberOfSmoothingSteps);

      smooth(level,numberOfSmoothingSteps,iteration);
    }
    tm[timeForDirectSolver]+=getCPU()-time;
    

    if( ps!=0 && (debug & 4 ) && ps->isGraphicsWindowOpen() )
    {
      psp.set(GI_TOP_LABEL,sPrintF(buff,"cycle: it=%i, level=%i, solution on coarsest level.",iteration,level)); 
      ps->erase();
      PlotIt::contour(*ps,uMG.multigridLevel[level],psp);
    }

  }

  
  if( parameters.problemIsSingular )
  {
    // realCompositeGridFunction & uu =(realCompositeGridFunction &)uMG.multigridLevel[level];
    if( level==0 )
      setMean(uMG.multigridLevel[level],parameters.meanValueForSingularProblem,level);
    else
      setMean(uMG.multigridLevel[level],0.,level);
  }


//   if( false && parameters.problemIsSingular && level==0 ) // level<mgcg.numberOfMultigridLevels()-1 )
//   {
//     // reset the right hand side
//     if( alpha(level) != 0. )
//       fMG.multigridLevel[level]+=alpha(level)*rightNullVector.multigridLevel[level];
//   }


  if( level==0 )
  { // we need to return the maximum defect for the convergence test -- if we are not close we maybe could 
    // just use the value from the next to last smooth?
    defectNew=0.;
    const int option=1;  // max norm
    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
      defectNew=max(defectNew,defectNorm(level,grid,option));  // this is approximate but fast
    }
    maximumDefect=defectNew;

    // How bad could the guess at the residual be? Assume could be too small by this factor:
    const real safetyFactor=2.;  
    
    bool computeAccurateMaxDefect=false;
    if( parameters.convergenceCriteria==OgmgParameters::residualConverged )
    {
      const real resTol = parameters.residualTolerance*l2NormRightHandSide + parameters.absoluteTolerance;
      real l2NormResidual = maximumDefect;  // this is an upper bound ******************************** FIX ME *******
      computeAccurateMaxDefect = l2NormResidual < resTol*safetyFactor;
    }
    else if( parameters.convergenceCriteria==OgmgParameters::residualConvergedOldWay )
    {
      computeAccurateMaxDefect =maximumDefect < parameters.residualTolerance*numberOfGridPoints;
    }

    if( false )
      printF("---> cycle: level=%i approx-defect = %8.2e, computeAccurateMaxDefect=%i\n",level,maximumDefect,
             (int)computeAccurateMaxDefect);

    if( computeAccurateMaxDefect )
    {
      // If we are close to convergence we compute the true max-defect
  
      defectNew=defectMaximumNorm(level);

      if( debug & 2 )
        printF("@@@@ cycle: level=%i, Compute accurate defect since close to convergence: "
               "maxDefect(approx)=%8.2e , actual=%8.2e\n",level,maximumDefect,defectNew);

    }
    // printF(" cycle: level=%i, maxDefect(approx)=%8.2e , actual=%8.2e\n",level,maximumDefect,defectNew);
    maximumDefect=defectNew;
  }

  // This next function computes the defect somtimes which is needed for some reason ***fix this***
  outputResults(level, iteration+1, maximumDefect, defectNew, defectOld);  // iteration->iteration+1 030710
  

  return 0;
}
  

// =================================================================================================================
/// \brief Check the validity of parameters.
/// \details Not all parameter values are valid or implemented, depending on the equation being solved, order of accuracy, etc.
// =================================================================================================================
void Ogmg::
checkParameters()
{
  if( orderOfAccuracy==4 )
  {
    if( equationToSolve!=OgesParameters::laplaceEquation )
    {
      if( parameters.dirichletFirstGhostLineBC==OgmgParameters::useEquationToSecondOrder )
      {
        printF("Ogmg:INFO: Setting additional Dirichlet BC to extrapolation since equationToSolve!=OgesParameters::laplaceEquation\n");
        parameters.dirichletFirstGhostLineBC           =OgmgParameters::useExtrapolation;
        parameters.lowerLevelDirichletSecondGhostLineBC=OgmgParameters::useExtrapolation;

        // This is the old way of specifying -- this will be removed eventually 
	parameters.fourthOrderBoundaryConditionOption=0;
        parameters.useEquationForDirichletOnLowerLevels=0;

      }

      if( parameters.neumannSecondGhostLineBC==OgmgParameters::useEquationToSecondOrder )
      {
	printF("Ogmg:INFO: Setting additional Neumann BC to extrapolation (l=0), eqn2 (l>0) since equationToSolve!=OgesParameters::laplaceEquation\n");
	parameters.neumannSecondGhostLineBC          =OgmgParameters::useExtrapolation;
	// parameters.lowerLevelNeumannSecondGhostLineBC=OgmgParameters::useExtrapolation;
	parameters.lowerLevelNeumannSecondGhostLineBC=OgmgParameters::useEquationToSecondOrder;
	
	// This is the old way of specifying -- this will be removed eventually
        parameters.useSymmetryForNeumannOnLowerLevels=false;

      }
      else if( parameters.lowerLevelNeumannSecondGhostLineBC==OgmgParameters::useEquationToSecondOrder )
      {
	parameters.lowerLevelNeumannSecondGhostLineBC=OgmgParameters::useExtrapolation;
      }

    }    
  }
}


// =================================================================================================================
//! Output results for this multigrid cycle
// =================================================================================================================
int Ogmg::
outputResults(const int & level, const int & iteration, real & maximumDefect, real & defectNew, real & defectOld )
{
  real time0=getCPU();  // start timeForMiscellaneous

  CompositeGrid & mgcg = multigridCompositeGrid();

  const bool print = myid==0;

  defectNew=maximumDefect;
  if( (level==0 && (debug & 2)) || debug & 4 || parameters.showSmoothingRates )
  {
    real time1=getCPU();

    defect(level);  // is this needed ??? *******************************************************************

    tm[timeForMiscellaneous]-=getCPU()-time1;     // don't count time for defect here 

    defectNew= maxNorm(defectMG.multigridLevel[level]);
    const real defectRatio=defectNew/max(defectOld,REAL_MIN*100.);
    if( iteration>0 )
    {
      printF("%*.1s Ogmg::cycle:level=%i, it=%i, WU=%5.2f, defect=%7.2e, defect/defectOld=%6.3f,"
	     "  ECR%i=%8.3f %s\n",level*4," ",level,iteration,workUnits(level),defectNew,defectRatio,
	     level,pow(defectRatio,1./(max(1.,workUnits(level)))),
	     (level==0 ? "***" : " "));
    }
    else
    {
      printF("%*.1s Ogmg::cycle:level=%i, it=%i, Initial defect =%7.2e\n",level*4," ",level,iteration,defectNew);
    }
  }
  else if( level==0 )
  {
    
/* ----
    // **** this needs to be computed if debug is not turned on *** fix this ***

    real time1=getCPU();
    defect(level);              // is this needed ???  **************************************************************
    tm[timeForMiscellaneous]-=getCPU()-time1;     // don't count time for defect here 


    defectNew= maxNorm(defectMG.multigridLevel[level]);   // ** this may not have been computed!
    ---- */
  }
  maximumDefect=defectNew;

  if( !(level==0 && (Ogmg::debug & 2 || parameters.outputMatlabFile) ) )
  {
    // In this case we only keep track of minimal convergence information.
    if( level==0 )
    {
      // printF(" outputResults: iteration=%i defectNew=%8.2e, defectOld=%8.2e\n",iteration,defectNew,defectOld);
    
      if( defectOld<0. ) 
	defectOld=defectNew*5.;  // do this as an approximation

      if( iteration==0 )
      {
	totalWorkUnits=0.;
	totalResidualReduction=1.;
	numberOfCycles=0;
	totalWorkUnits=0.;
	workUnits=0.;  
      }
      else
      {
	totalWorkUnits+=workUnits(level);
	totalResidualReduction*=defectNew/defectOld;
      }
    }
    
    tm[timeForMiscellaneous]+=getCPU()-time0;
    return 0;
  }
  
  
  RealArray maxDefect(mgcg.numberOfComponentGrids());
  RealArray l2Defect(mgcg.numberOfComponentGrids());
  
  if( parameters.showSmoothingRates )
  {
    // NOTE: These computations require communication
    int grid;
    for( grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
      maxDefect(grid)=maxNorm(defectMG.multigridLevel[level][grid]);
      l2Defect(grid)=l2Norm(defectMG.multigridLevel[level][grid]);
    }
  }
   
  if( !print )
  {
    tm[timeForMiscellaneous]+=getCPU()-time0;
    return 0;
  }
  
  if( level==0 && Ogmg::debug & 2 )
  { 
    // write results to a file for easy insertion into the documentation
    if( iteration==0 )
    {
      fPrintF(infoFile,
	      "\\begin{table}[hbt]\n"
	      "\\begin{center}\n"
	      "\\begin{tabular}{|c|c|c|c|c|} \\hline \n"
              " $i$   & $\\vert\\vert\\mbox{res}\\vert\\vert_\\infty$  &  CR     &  WU    & ECR  \\\\   \\hline \n");
      if( parameters.showSmoothingRates )
      {
        // output the residual on each grid if smoothing rates are being displayed
	fPrintF(infoFile,"\\begin{tabular}{");
        int grid;
	for( grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
          fPrintF(infoFile,"|c");
	fPrintF(infoFile,"|} \\hline \n"
        		 " $i$  &");
	for( grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
	  fPrintF(infoFile," r_%i(i)   &",grid+1);
        fPrintF(infoFile,"	\\\\   \\hline \n");

        // fPrintF(infoFile,"e=[\n  for matlab\n");  
      }
    }
    else 
    {
      if( defectNew/defectOld>1.e-3 )
      {
	fPrintF(infoFile," $%2i$  & $%8.1e$ & $%5.3f$ & $%4.1f$ & $%4.2f$ \\\\ \n",iteration,
		defectNew,defectNew/defectOld,workUnits(level),
		pow(defectNew/defectOld , 1./(max(1.,workUnits(level)))));
	
      }
      else
      {
	fPrintF(infoFile," $%2i$  & $%8.1e$ & $%6.5f$ & $%4.1f$ & $%4.2f$ \\\\ \n",iteration,
		defectNew,defectNew/defectOld,workUnits(level),
		pow(defectNew/defectOld , 1./(max(1.,workUnits(level)))));
      }
      

      if( parameters.showSmoothingRates )
      {
	// output the maximum defect on each grid
	fPrintF(infoFile," $%2i$  &",iteration);
        int grid;
	for( grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
	{
	  fPrintF(infoFile," $%8.1e$ &",maxDefect(grid));
	}
	fPrintF(infoFile," \\\\ %% max defects per grid\n");

	fPrintF(infoFile," $%2i$  &",iteration);
	for( grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
	{
	  fPrintF(infoFile," $%8.1e$ ($n_s=%i$) &",l2Defect(grid),parameters.numberOfSubSmooths(grid,0));
	}
	fPrintF(infoFile," \\\\ %% L2 defects per grid\n");
      }
    }
    
    // write info to a check file for regression testing

    // new check file format:
    //      title
    //      time numberOfComponents [component err norm] [component err norm] ... 

    if( iteration==0 )
    {
      // title: 
      // Old: fPrintF(checkFile,"Grid %s, numberOfMultigridLevels = %i\n",(const char*)gridName, mgcg.numberOfMultigridLevels());
      fPrintF(checkFile,"   time     nc c=0  defect     WU   c=1    CR     ECR (grid=%s, levels=%i)\n",
                     (const char*)gridName, mgcg.numberOfMultigridLevels());

      // fPrintF(checkFile,"numberOfMultigridLevels = %i\n",mgcg.numberOfMultigridLevels());
      // fPrintF(checkFile," iteration  res      CR     WU   ECR\n");
    }
    // fPrintF(checkFile,"   %3i   %8.1e  %5.3f  %4.1f   %4.2f  \n",iteration,
    // 	    defectNew,defectNew/defectOld,workUnits(level),
    // 	    pow(defectNew/defectOld , 1./(max(1.,workUnits(level)))));

    real time=iteration;
    const int numberOfComponents=2, component=0;
    // Output the defect and work units (ogmgt will output the error in the solution as a last line)
    fPrintF(checkFile," %8.1e  %3i  %i %8.2e %8.2e  %i %8.2e %8.2e\n",
	    time,numberOfComponents,component,defectNew,workUnits(level),component+1,
	    defectNew/defectOld, pow(defectNew/defectOld , 1./(max(1.,workUnits(level)))));
    
  }

  // here we keep track of the total work units and total reduction in the residual
  // so we can output an average ECR.
  if( level==0 )
  {
    if( iteration==0 ) // do not count iteration==0
    {
      totalWorkUnits=0.;
      totalResidualReduction=1.;
      numberOfCycles=0;
      totalWorkUnits=0.;
      workUnits=0.;  
    }
    else
    {
      totalWorkUnits+=workUnits(level);
      totalResidualReduction*=defectNew/defectOld;
    }
  }
  
  if( level==0 && parameters.outputMatlabFile )
  {
    // save results for later output to a matlab file
    cycleResults(iteration,defectPerCycle)=maximumDefect;
    cycleResults(iteration,workUnitsPerCycle)=workUnits(level);
    for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    {
      cycleResults(iteration,grid0DefectPerCycle+grid)=l2Defect(grid);
    }
    
  }

  tm[timeForMiscellaneous]+=getCPU()-time0;
  return 0;
}


//! Output results about the cycle convergence rates etc. in a form suitable for matlab
int Ogmg::
outputCycleInfo( )
{

  if( parameters.outputMatlabFile && myid==0 )
  {
    CompositeGrid & mgcg = multigridCompositeGrid();
    const int np= max(1,Communication_Manager::numberOfProcessors());
    const int startCycle=0;

    FILE *matlabFile = fopen("ogmg.m","w" ); 
    int i;
    fPrintF(matlabFile,"t=[");
    for( i=startCycle; i<=numberOfIterations; i++ ) // *note* start at iteration 1
      fPrintF(matlabFile,"%i ",i);
    fPrintF(matlabFile,"];\n");

    fPrintF(matlabFile,"defect=[");
    for( i=startCycle; i<=numberOfIterations; i++ )  // *note* start at iteration 1
      fPrintF(matlabFile,"%9.3e ",cycleResults(i,defectPerCycle));
    fPrintF(matlabFile,"];\n");

    
    fPrintF(matlabFile,"%% clf\n");
    fPrintF(matlabFile,"%% set(gca,'FontSize',16);\n");

    fPrintF(matlabFile,"%% plot(t,defect,'r-o');\n");
    fPrintF(matlabFile,"%% title('Multigrid Convergence','FontSize',18);\n");
    
    fPrintF(matlabFile,"%% ylabel('maximum residual');\n");

    fPrintF(matlabFile,"%% xlabel('multigrid cycle');\n");
    fPrintF(matlabFile,"%% set(gca,'YScale','log');\n");
    fPrintF(matlabFile,"%% grid on\n");

//      real averageCR=pow(totalResidualReduction,1./max(1.,numberOfCycles));
//      real averageECR=pow(totalResidualReduction,1./max(1.,totalWorkUnits));
    // Compute convergence rates without the first cycle -- 
    // the first cycle usually skews the results to be too good.
    int cycleStart=2, cycleEnd=numberOfCycles;
    real residualReduction=cycleResults(cycleEnd,defectPerCycle)/cycleResults(cycleStart-1,defectPerCycle);
    real wu=sum(cycleResults(Range(cycleStart,cycleEnd),workUnitsPerCycle));

    real aveCR=pow(residualReduction,1./(cycleEnd-cycleStart+1));
    real aveECR= pow(residualReduction,1./max(1.,wu));

    aString buffer;

    int level=0;
//     aString cycleLabel = (parameters.cycleType==OgmgParameters::cycleTypeF ? "F" :
// 			  parameters.numberOfCycles(level)==1 ? "V" : 
// 			  parameters.numberOfCycles(level)==2 ? "W" : 
// 			  sPrintF(buffer,"C%i",parameters.numberOfCycles(level)));
    aString cycleLabel;
    if( parameters.cycleType==OgmgParameters::cycleTypeF )
      cycleLabel="F";
    else if( parameters.numberOfCycles(level)==1 )
      cycleLabel = "V"; 
    else if( parameters.numberOfCycles(level)==2 )
      cycleLabel = "W"; 
    else
      cycleLabel = sPrintF(buffer,"C%i",parameters.numberOfCycles(level));
    
    fPrintF(matlabFile,sPrintF(buffer,"%%%% legend('%s','%s','%s','%7.1e points. %i levels.','CR=%3.3f, ECR=%3.2f, %s[%i,%i], N_p=%i');\n",
                               (const char*)infoFileCaption[0],
			       (const char*)infoFileCaption[1],(const char*)infoFileCaption[2],
                               (real)numberOfGridPoints,mgcg.numberOfMultigridLevels(),aveCR,aveECR,
                               (const char*)cycleLabel,parameters.numberOfSmooths(0,level),
                               parameters.numberOfSmooths(1,level),np));


    fPrintF(matlabFile,"%% pause\n");
    
    fPrintF(matlabFile,"%% print -deps2 residual.eps\n\n\n");
    fPrintF(matlabFile,"%% print -depsc2 residual.eps\n\n\n");
    

    if( parameters.showSmoothingRates )
    {
      // output defects for each component grid
      int grid;
      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	fPrintF(matlabFile,"defect%i=[",grid);
	for( i=1; i<=numberOfIterations; i++ )
	  fPrintF(matlabFile,"%9.3e ",cycleResults(i,grid0DefectPerCycle+grid));
	fPrintF(matlabFile,"];\n");
      }
      const int numberOfSymbols=6;
      aString symbol[numberOfSymbols]={"r-o","g-x","b-s","c-<","m->","r-+"}; //

      fPrintF(matlabFile,"plot(");
      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
	fPrintF(matlabFile,sPrintF(buff,"t,defect%i,'%s'",grid,(const char*)symbol[(grid%numberOfSymbols)]));
        if( grid< mgcg.numberOfComponentGrids()-1 )
	  fPrintF(matlabFile,",");
	else
          fPrintF(matlabFile,");\n");
      }
      
      if( parameters.autoSubSmoothDetermination )
        fPrintF(matlabFile,"title('Residuals by component grid, variable smoothing');\n");
      else
	fPrintF(matlabFile,"title('Residuals by component grid, fixed smoothing');\n");
      
      fPrintF(matlabFile,"ylabel('L_2 norm');\n");

      fPrintF(matlabFile,"legend(");
      for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
      {
        fPrintF(matlabFile,"'grid%i'",grid+1);
	if( grid< mgcg.numberOfComponentGrids()-1 )
	  fPrintF(matlabFile,",");
	else
          fPrintF(matlabFile,");\n");
      }
      fPrintF(matlabFile,"xlabel('iteration');\n");
      fPrintF(matlabFile,"set(gca,'YScale','log');\n");
//    fPrintF(matlabFile,"grid on;\n");
      fPrintF(matlabFile,"%% pause\n");
      fPrintF(matlabFile,"%% print -deps2 residualByGrid.eps\n");
      fPrintF(matlabFile,"%% print -depsc2 residual.eps\n\n\n");

    }
    

    fclose(matlabFile);
  }
  return 0;
}


void Ogmg::
displaySmoothers(const aString & label, FILE *file /* =stdout */ )
//==================================================================================
// /Description:
//    Show the smoothers currently being used on each grid and level
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  fPrintF(file,"\n -------- %s: smoothers --------\n",(const char*)label);
  int numberOfComponentGrids=mgcg.numberOfComponentGrids();
  int numberOfMultigridLevels=mgcg.numberOfMultigridLevels();
  if( mgcg.numberOfMultigridLevels()<=1 )
  {
    numberOfComponentGrids=parameters.smootherType.getLength(0);
    numberOfMultigridLevels=parameters.smootherType.getLength(1);
  }
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    fPrintF(file,"grid %i : ",grid);
    for( int level=0; level<numberOfMultigridLevels; level++ )
    {
      int smooth = parameters.smootherType(grid,level);
      fPrintF(file,"%s[%i,%i] ",
	      (const char *)parameters.smootherName[smooth],
	      parameters.numberOfSmooths(0,level),parameters.numberOfSmooths(1,level));
    }
    fPrintF(file,"\n");
  }
  fPrintF(file," -----------------------------------------\n");
}



// extern real timeForApplyBCneumann;  // from neumann.C

// These are from boundaryConditions.C:
extern real timeForNeumannBC;
extern real timeForBC;
extern real timeForFinishBC;
extern real timeForGeneralNeumannBC;
extern real timeForExtrapolationBC;

extern real timeForSetupBC;
extern real timeForBCWhere;
extern real timeForBCOpt;
extern real timeForBC4Extrap;
extern real timeForBCFinal;
extern real timeForBCUpdateGeometry;

//\begin{>>OgmgInclude.tex}{\subsection{printStatistics}}
void Ogmg::
printStatistics(FILE *file_ /* =stdout */) const
//==================================================================================
// /Description:
//   Print performance statistics such as the cpu time required by various routines.
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  const CompositeGrid & mgcg = (CompositeGrid&)multigridCompositeGrid();
  const int numberOfFiles = debug & 2 ? 2 : 1;
  const int cycles = max(1,numberOfCycles);    // number of cycles on level 1

  real *tm2 = (real*)tm;  // remove const
  ParallelUtility::getMaxValues(tm2,tm2,numberOfThingsToTime);
  
  const real total = tm[timeForSolve];
  const int np= max(1,Communication_Manager::numberOfProcessors());
  
  real averageCR=pow(totalResidualReduction,1./max(1.,numberOfCycles));
  real averageECR=pow(totalResidualReduction,1./max(1.,totalWorkUnits));

  real aveCR=averageCR;
  real aveECR=averageECR;
  if( parameters.outputMatlabFile )
  {
    // Compute convergence rates without the first cycle -- 
    // the first cycle usually skews the results to be too good for Dirichlet and too bad for Neumann BCs.
    int cycleStart=2, cycleEnd=numberOfCycles;
    real residualReduction=cycleResults(cycleEnd,defectPerCycle)/cycleResults(cycleStart-1,defectPerCycle);
    real wu=sum(cycleResults(Range(cycleStart,cycleEnd),workUnitsPerCycle));

    aveCR=pow(residualReduction,1./(cycleEnd-cycleStart+1));
    aveECR= pow(residualReduction,1./max(1.,wu));
  }
  
  real aveTR10 = (tm[timeForSolve]/cycles)*(log(.1)/log(max(1.e-50,aveCR)));

  // ave time to reduce residual by a factor of 10 per (Million grid points per processor)
  real aveTR10M = aveTR10/(numberOfGridPoints/(1.e6*np));

//   real *tmnc = (real*) tm; // remove const
//   int i;
//   for(i=0; i<numberOfThingsToTime; i++ )
//   {
//     tmnc[i]=ParallelUtility::getMaxValue(tm[i]);
//   }

  // here is the total time:
  const real sumTotal = tm[timeForSmooth]+tm[timeForDefect]+tm[timeForFineToCoarse]
      +tm[timeForCoarseToFine]+tm[timeForDirectSolver]+tm[timeForMiscellaneous];


  // *************Header label for the info file********************************************

  // Make a label for the info file to define the smoother -- make a list of the different smoothers used.
  aString buffer,smootherLabel;

  int level=0, smooth=-1;
//   aString cycleLabel = (parameters.cycleType==OgmgParameters::cycleTypeF ? "F" :
//                         parameters.numberOfCycles(level)==1 ? "V" : 
// 			parameters.numberOfCycles(level)==2 ? "W" : 
//                         sPrintF(buffer,"C%i",parameters.numberOfCycles(level)));
  aString cycleLabel;
  if( parameters.cycleType==OgmgParameters::cycleTypeF )
    cycleLabel="F";
  else if( parameters.numberOfCycles(level)==1 )
    cycleLabel = "V"; 
  else if( parameters.numberOfCycles(level)==2 )
    cycleLabel = "W"; 
  else
    cycleLabel = sPrintF(buffer,"C%i",parameters.numberOfCycles(level));

  // Label the cycle: (e.g. V[2,1]
  smootherLabel= sPrintF(buffer,"%s[%i,%i]: ",(const char*)cycleLabel,
			     parameters.numberOfSmooths(0,level),parameters.numberOfSmooths(1,level));

  for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
  {
    int newSmooth=parameters.smootherType(grid,level);
    if( grid==0 || min(abs(parameters.smootherType(Range(0,grid-1),level)-newSmooth))!=0 )
    { // this is a new smoother (not equal to any of the previous)
      smooth=parameters.smootherType(grid,level);
      if( grid!=0 ) smootherLabel+=", ";
      smootherLabel+= sPrintF(buffer,"%s",(const char *)parameters.smootherName[smooth]);
      if( smooth==OgmgParameters::Jacobi )
        smootherLabel+= sPrintF(buffer," $\\omega=%3.2f$",parameters.omegaJacobi);
      if( smooth==OgmgParameters::GaussSeidel )
        smootherLabel+= sPrintF(buffer," $\\omega=%3.2f$",parameters.omegaGaussSeidel);
      if( smooth==OgmgParameters::redBlack || smooth==OgmgParameters::redBlackJacobi )
        smootherLabel+= sPrintF(buffer," $\\omega=%3.2f$",parameters.omegaRedBlack);
      if( smooth==OgmgParameters::lineJacobiInDirection1 ||
          smooth==OgmgParameters::lineJacobiInDirection2 ||
          smooth==OgmgParameters::lineJacobiInDirection3 ||
          smooth==OgmgParameters::alternatingLineJacobi )
        smootherLabel+= sPrintF(buffer," $\\omega=%3.2f$",parameters.omegaLineJacobi);
      if( smooth==OgmgParameters::lineZebraInDirection1 ||
          smooth==OgmgParameters::lineZebraInDirection2 ||
          smooth==OgmgParameters::lineZebraInDirection3 ||
          smooth==OgmgParameters::alternatingLineZebra )
        smootherLabel+= sPrintF(buffer," $\\omega=%3.2f$",parameters.omegaLineZebra);

    }
  }

  real mem=Overture::getCurrentMemoryUsage();
  real maxMem=ParallelUtility::getMaxValue(mem);  // max over all processors
  real minMem=ParallelUtility::getMinValue(mem);  // min over all processors
  real totalMem=ParallelUtility::getSum(mem);  // min over all processors
  real aveMem=totalMem/np;
  real maxMemRecorded=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

  const bool print = myid==0;

  if( print )
  {
    fPrintF(infoFile,
	    "\\hline \n");
    for( int n=0; n<5; n++ )
    {
      if( infoFileCaption[n]!="" )
	fPrintF(infoFile,"\\multicolumn{5}{|c|}{%s}  \\\\\n",(const char*)infoFileCaption[n]);
    }

    if( parameters.useFullMultigrid )
    {
      fPrintF(infoFile,"\\multicolumn{5}{|c|}{Full Multigrid.}  \\\\\n");
    }
  
    fPrintF(infoFile,
	    "\\multicolumn{5}{|c|}{%s}  \\\\\n"
	    "\\multicolumn{5}{|c|}{%8.2e grid-points. %i levels. np=%i.}  \\\\\n"
	    "\\multicolumn{5}{|c|}{Average CR=$%4.3f$, ECR=$%3.2f$.}  \\\\\n"
	    "\\multicolumn{5}{|c|}{time/cycle = %6.2e(s), TR10=%6.2e (s), TR10/(M/np)=%6.2e (s).}  \\\\\n"
	    "\\hline \n"
	    "\\end{tabular}\n"
	    "\\end{center}\n"
	    "\\caption{Multigrid convergence rates.}\n"
	    "%% \\label{fig:square} \n"
	    "\\end{table}\n" 
	    "{\\footnotesize\n"
	    "\\begin{verbatim}\n",
	    (const char*)smootherLabel,
	    (real)numberOfGridPoints,
	    mgcg.numberOfMultigridLevels(),np,
	    aveCR,aveECR,tm[timeForSolve]/cycles,aveTR10,aveTR10M);
    
  }
  
  for( int io=0; io<numberOfFiles; io++ )
  {
    // write info to the screen and the info file
    FILE *file = io==0 ? file_ : infoFile;

    // if( file==NULL ) continue;
    
    fPrintF(file,"\n ========================Ogmg Summary=========================\n\n"
			"                       Grid = %s \n",(const char*)gridName);
    

    // if( !print ) continue;
      

    fPrintF(file," Equation: %s.\n",
	    (equationToSolve==OgesParameters::userDefined ? "userDefined" :
	     equationToSolve==OgesParameters::laplaceEquation ? "Laplace" :
	     equationToSolve==OgesParameters::divScalarGradOperator ? "div(s(x) grad)":
	     equationToSolve==OgesParameters::heatEquationOperator ? "I + c0*Delta" :
	     equationToSolve==OgesParameters::variableHeatEquationOperator ? "I + s(x)*Delta" :
	     equationToSolve==OgesParameters::divScalarGradHeatEquationOperator ? "I + div( s(x) grad )" :
	     equationToSolve==OgesParameters::secondOrderConstantCoefficients ? "second-order constant coeff" : 
	     "unknown"));

    fPrintF(file," Boundary conditions explicitly specified = %i.\n",(int)bcSupplied);
    fPrintF(file," Equations are %s.\n",(parameters.problemIsSingular ? "singular" : "are not singular"));
    if( parameters.problemIsSingular )
    {
      fPrintF(file," %s right-hand-side for singular problems.\n",(parameters.projectRightHandSideForSingularProblem ?
								  "Project" : "Do not project"));
      fPrintF(file," %s mean value for singular problems.\n",(parameters.assignMeanValueForSingularProblem ?
								  "Assign" : "Do not assign"));
    }

    if( parameters.convergenceCriteria==OgmgParameters::residualConverged )
    {
      fPrintF(file," Convergence criteria: l2Norm(residual) < residualTolerance*l2Nnorm(f) + absoluteTolerance"
	      " (residualTolerance=%8.2e, absoluteTolerance=%8.2e)\n",parameters.residualTolerance,parameters.absoluteTolerance);
    }
    else if( parameters.convergenceCriteria==OgmgParameters::errorEstimateConverged )
    {
      fPrintF(file,"Convergence criteria: max(error estimate) < errorTolerance (errorTolerance=%8.2e)\n",parameters.errorTolerance);
    }
    else if( parameters.convergenceCriteria==OgmgParameters::residualConvergedOldWay )
    {
      fPrintF(file," Convergence criteria: max-defect < rTol*numGridPoints = %8.2e (rtol=%8.2e)\n",
	      parameters.residualTolerance*numberOfGridPoints,parameters.residualTolerance);
    }

    fPrintF(file," order of accuracy = %i\n",orderOfAccuracy);
    fPrintF(file," number of levels = %i (%i extra levels).\n",mgcg.numberOfMultigridLevels(),
                   mgcg.numberOfMultigridLevels()-1);
    fPrintF(file," interpolate defect = %i\n",(int)parameters.interpolateTheDefect);
    if( parameters.cycleType==OgmgParameters::cycleTypeF )
    {
       fPrintF(file," F-cycle used.\n");
    }
    else
    {
      fPrintF(file," number of cycles per level=");
      for( level=0; level<mgcg.numberOfMultigridLevels(); level++ )
	fPrintF(file," %i ",parameters.numberOfCycles(level));
      fPrintF(file,"\n");
    }
    fPrintF(file," number of smooths (global) per level=");
    for( level=0; level<mgcg.numberOfMultigridLevels(); level++ )
      fPrintF(file," [%i,%i] ",parameters.numberOfSmooths(0,level),parameters.numberOfSmooths(1,level));
    fPrintF(file,"\n");
    fPrintF(file," grid ordering in smooth is %s.\n",(parameters.gridOrderingForSmooth==0 ? "1..ng" :
						    parameters.gridOrderingForSmooth==1 ? "ng..1" : "alternating"));
    fPrintF(file," auto sub-smooth determination is %s (reference grid for sub-smooths=%i).\n",
            (parameters.autoSubSmoothDetermination ? "on" : "off"),subSmoothReferenceGrid);
    fPrintF(file," use new red black smoother=%i\n",(int)parameters.useNewRedBlackSmoother);
  
    fPrintF(file," number of iterations for implicit interpolation is %i\n",
            parameters.maximumNumberOfInterpolationIterations);
    
    fPrintF(file," coarse to fine interpolation width=%i.\n",parameters.coarseToFineTransferWidth);
    fPrintF(file," fine to coarse transfer is %s.\n",
            (parameters.fineToCoarseTransferWidth==1 ? "injection" : "full weighting"));
    
    const OgmgParameters::AveragingOption & averagingOption = parameters.averagingOption;
    fPrintF(file," operator averaging: %s\n",
	    averagingOption==OgmgParameters::averageCoarseGridEquations ? "average coarse grid equations." :
	    averagingOption==OgmgParameters::doNotAverageCoarseGridEquations ? "do not average coarse grid equations." : 
	    averagingOption==OgmgParameters::doNotAverageCoarseCurvilinearGridEquations ? 
	    "average Cartesian grids, do not average curvilinear grids." : "unknown option.");
    


    const aString boundaryAveragingOptionName[] = 
      { "imposeDirichlet", 
	"imposeExtrapolation",
	"injection",
	"partialWeighting",
	"halfWeighting",
	"lumpedPartialWeighting",
	"imposeNeumann",
	"unknown" 
      };
    
    fPrintF(file,"   boundary averaging option is %s (for a 'dirichlet BC') and %s (for a 'Neumann' BC)\n",
	    (const char*)boundaryAveragingOptionName[min(7,max(0,parameters.boundaryAveragingOption[0]))],
	    (const char*)boundaryAveragingOptionName[min(7,max(0,parameters.boundaryAveragingOption[1]))]);

    fPrintF(file,"   ghost line averaging option is %s (for a 'dirichlet BC') and %s (for a 'Neumann' BC)\n",
	    (const char*)boundaryAveragingOptionName[min(7,max(0,parameters.ghostLineAveragingOption[0]))],
	    (const char*)boundaryAveragingOptionName[min(7,max(0,parameters.ghostLineAveragingOption[1]))]);


    fPrintF(file," boundary smoothing: number of layers=%i, iterations=%i, apply on %i levels.\n",
	    parameters.numberOfBoundaryLayersToSmooth,
            parameters.numberOfBoundarySmoothIterations,
            parameters.numberOfLevelsForBoundarySmoothing);

    fPrintF(file," interp. boundary smoothing: layers=%i, iterations=%i, global-its=%i, apply on %i levels, "
            "combine-with-smooths=%i.\n",
            parameters.numberOfInterpolationLayersToSmooth,
            parameters.numberOfInterpolationSmoothIterations,
	    parameters.numberOfIBSIterations,
            parameters.numberOfLevelsForInterpolationSmoothing,
	    (int)parameters.combineSmoothsWithIBS );
    
    fPrintF(file," assumeSparseStencilForRectangularGrids=%i\n",(int)assumeSparseStencilForRectangularGrids);



    fPrintF(file,"\n");
    fPrintF(file," Coarse Grid:\n");
    aString coarseGridSolver;
    if( parameters.useDirectSolverOnCoarseGrid )
      coarseGridSolver=directSolver.parameters.getSolverName();
    else
      coarseGridSolver="smoother";
    int length=coarseGridSolver.length();
    if( length > 70 )
    { // name is too long, split it.
      aString a=coarseGridSolver,b;
      int i;
      for( i=50; i<length; i++ )
      {
	if( coarseGridSolver[i]==' ' )
	{
          a=coarseGridSolver(0,i-1); b=coarseGridSolver(i+1,length-1);
	  break;
	}
        else if( i==(length-1) )
	{ // no split point found at a blank, split at char 60:
          int ii=60;
	  a=coarseGridSolver(0,ii-1)+"-"; b=coarseGridSolver(ii,length-1);
	}
      }
      fPrintF(file,
              "   coarse grid solver : %s\n"
	      "                        %s\n",(const char*)a,(const char*)b);
    }
    else
      fPrintF(file,"   coarse grid solver : %s \n",(const char*)coarseGridSolver);

    real rtol,atol;
    int maximumNumberOfIterations;
    directSolver.get(OgesParameters::THErelativeTolerance,rtol);
    directSolver.get(OgesParameters::THEabsoluteTolerance,atol);
    directSolver.get(OgesParameters::THEmaximumNumberOfIterations,maximumNumberOfIterations);
    fPrintF(file,"   relative tol.=%8.2e, absolute tol.=%8.2e, max number of iterations=%i (0=choose default)\n",
            rtol,atol,maximumNumberOfIterations);


    fPrintF(file,"   average number of iterations per coarse grid solve = %5.1f/cycle\n",
	    real(totalNumberOfCoarseGridIterations)/cycles);
    int numberOfGridPointsOnCoarseGrid=0;
    CompositeGrid & mgc = mgcg.multigridLevel[mgcg.numberOfMultigridLevels()-1];
    for( int grid=0; grid<mgc.numberOfComponentGrids(); grid++ )
      numberOfGridPointsOnCoarseGrid+=mgc[grid].mask().elementCount();
    fPrintF(file,"   coarse grid has %i grid points (%7.1e %% of fine grid)\n",numberOfGridPointsOnCoarseGrid,
	    100.*numberOfGridPointsOnCoarseGrid/real(numberOfGridPoints));

    fPrintF(file,"   coarse grid averaging option: %s\n",
	    (parameters.averagingOption==OgmgParameters::doNotAverageCoarseGridEquations ? "no averaging" :
             parameters.averagingOption==OgmgParameters::averageCoarseGridEquations ? "Galerkin averaging" :
             parameters.averagingOption==OgmgParameters::doNotAverageCoarseCurvilinearGridEquations ? "Galerkin (Cartesian grids only)" :
             "unknown option" ));
    

// 	   "   grid  smoother               iterations  grid name\n"
// 	   "   ----  --------               ----------  ---------\n");
//     level=0;
//     for( grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
//     {
//       int smooth = parameters.smootherType(grid,level);
//       smooth = smooth<0 || smooth>=numberOfSmoothers ? numberOfSmoothers : smooth;
//       fPrintF(file,
//               " %5i %-20s   %8i      %s\n",
// 	     grid,
// 	     (const char *) parameters.smootherName[smooth],
// 	     parameters.numberOfSmooths(level),
// 	     (const char *) mgcg[grid].mapping().getName(Mapping::mappingName));
//     }

    fPrintF(file,"\n");
    for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
    {
      fPrintF(file,"grid %i : ",grid);
      for( level=0; level<mgcg.numberOfMultigridLevels(); level++ )
      {
        int smooth = parameters.smootherType(grid,level);
        fPrintF(file,"%s[%i,%i] ",
                (const char *)parameters.smootherName[smooth],
                 parameters.numberOfSmooths(0,level),parameters.numberOfSmooths(1,level));
      }
      fPrintF(file,
	      " : %s \n"
	      "         bc=",(const char*)mgcg[grid].getName());
      for( int axis=0; axis<mgcg.numberOfDimensions(); axis++ )
      {
	fPrintF(file,"[");
        for( int side=0; side<=1; side++ )
	{
          aString bcName;
	  if( mgcg[grid].boundaryCondition(side,axis)==0 ) 
	    bcName="interp";
	  else if( mgcg[grid].boundaryCondition(side,axis)<0 ) 
	    bcName="periodic";
	  else if( bc(side,axis,grid)==OgesParameters::dirichlet )   // note: check "bc" array
	    bcName="dirichlet";
	  else if( bc(side,axis,grid)==OgesParameters::neumann ) 
	    bcName="neumann";
	  else if( bc(side,axis,grid)==OgesParameters::mixed ) 
	    bcName="mixed";
	  else if( bc(side,axis,grid)==OgesParameters::extrapolate) 
	    bcName="extrap";
	  else
	    bcName="unknown";

// 	  fPrintF(file,"%s",(boundaryCondition(side,axis,grid)==OgmgParameters::extrapolate ? "dirichlet" :
// 				   boundaryCondition(side,axis,grid)==OgmgParameters::equation ? 
// 			     (boundaryConditionData(0,side,axis,grid)==0. ? "neumann" : "mixed") : 
//                                    mgcg[grid].boundaryCondition(side,axis)<0 ? "periodic" :
//                                     mgcg[grid].boundaryCondition(side,axis)==0 ? "interp" :  "other"));
          fPrintF(file,"%s",(const char*)bcName);
          if( side==0 )
            fPrintF(file,",");
          else
            fPrintF(file,"] ");

	}
      }
      fPrintF(file,"\n         ave no. of subSmooths: ");
      for( level=0; level<mgcg.numberOfMultigridLevels(); level++ )
      {
        fPrintF(file,"l%i=%4.1f, ",level,parameters.totalNumberOfSubSmooths(grid,level)/max(1.,parameters.totalNumberOfSmoothsPerLevel(level)));
      }
      
      fPrintF(file,"\n");
      const IntegerArray & gid = mgcg[grid].gridIndexRange();
      int gridPoints = mgcg[grid].mask().elementCount();
      fPrintF(file,"         gid=[%i,%i][%i,%i][%i,%i], gridPoints=%i (%6.2f%%).\n",
              gid(0,0),gid(1,0),gid(0,1),gid(1,1),gid(0,2),gid(1,2),gridPoints,
              100.*real(gridPoints)/numberOfGridPoints);
      
    }  // end for grid
    fPrintF(file,"\n");
    if( parameters.numberOfBoundarySmoothIterations>0 && 
        parameters.numberOfBoundaryLayersToSmooth>0 && parameters.numberOfLevelsForBoundarySmoothing>0 )
    {
      fPrintF(file," Boundary smoothing: local iterations=%i, layers=%i for %i levels.\n",
	      parameters.numberOfBoundarySmoothIterations,
              parameters.numberOfBoundaryLayersToSmooth,parameters.numberOfLevelsForBoundarySmoothing);
    }
    if( parameters.numberOfIBSIterations>0 && parameters.numberOfInterpolationSmoothIterations>0 && 
        parameters.numberOfInterpolationLayersToSmooth>0 && parameters.numberOfLevelsForInterpolationSmoothing>0 )
    {
      fPrintF(file," IBS: interp. bndry smoothing: global its=%i, local its=%i, layers=%i for %i levels, %s.\n",
	      parameters.numberOfIBSIterations,parameters.numberOfInterpolationSmoothIterations,
              parameters.numberOfInterpolationLayersToSmooth,parameters.numberOfLevelsForInterpolationSmoothing,
              (parameters.combineSmoothsWithIBS==1 ? "combine with smooths" : "apply separately from smooths"));
    }
    

    // ************************************************************************8
    // ************Output INFO on Discrete BC's********************************8
    // ************************************************************************8
    if( orderOfAccuracy==2 )
    {
      fPrintF(file,"\nDiscrete boundary conditions: B=boundary, G1=ghost-1, N=Neumann, M=mixed-symmetry extrapN=extrap to order N\n");
      
      aString ghost1;
      int grid=0,ghostLine,orderOfExtrapolation;
      // ***** Dirichlet *****
      for( int level=0; level<=min(1,mgcg.numberOfMultigridLevels()-1); level++ )
      {
	ghostLine=1;
	getGhostLineBoundaryCondition( OgmgParameters::extrapolate, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost1 );
        if( level==0 )
  	  fPrintF(file,"  Dirichlet: l=0 : B=D,    G1=%8s\n",(const char*)ghost1);
        else
          fPrintF(file,"           : l>0 : B=D,    G1=%8s\n",(const char*)ghost1);
      }
      // ***** Neumann or Mixed BC *****
      for( int level=0; level<=min(1,mgcg.numberOfMultigridLevels()-1); level++ )
      {
	ghostLine=1;
	getGhostLineBoundaryCondition( OgmgParameters::equation, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost1 );
        if( level==0 )
	  fPrintF(file,"  Neumann  : l=0 : B=PDE,  G1=%8s\n",(const char*)ghost1);
        else
	  fPrintF(file,"           : l>0 : B=PDE,  G1=%8s\n",(const char*)ghost1);
      }
    }
    else if( orderOfAccuracy==4 )
    {
      fPrintF(file,"\nDiscrete boundary conditions: B=boundary, G1=ghost-1, G2=ghost-2, extrapN=extrap to order N\n");
      
      aString ghost1,ghost2;
      int grid=0,level,ghostLine,orderOfExtrapolation;
      // ***** Dirichlet *****
      for( level=0; level<=min(1,mgcg.numberOfMultigridLevels()-1); level++ )
      {
	ghostLine=1;
	getGhostLineBoundaryCondition( OgmgParameters::extrapolate, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost1 );
	ghostLine=2;
	getGhostLineBoundaryCondition( OgmgParameters::extrapolate, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost2 );
      
        if( level==0 )
  	  fPrintF(file,"  Dirichlet: l=0 : B=D,    G1=%8s, G2=%8s\n",(const char*)ghost1,(const char*)ghost2);
        else
          fPrintF(file,"           : l>0 : B=D,    G1=%8s, G2=%8s\n",(const char*)ghost1,(const char*)ghost2);
      }
      // ***** Neumann or Mixed BC *****
      for( level=0; level<=min(1,mgcg.numberOfMultigridLevels()-1); level++ )
      {
	ghostLine=1;
	getGhostLineBoundaryCondition( OgmgParameters::equation, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost1 );
	ghostLine=2;
	getGhostLineBoundaryCondition( OgmgParameters::equation, ghostLine, grid, level, 
				       orderOfExtrapolation, &ghost2 );
      
        if( level==0 )
	  fPrintF(file,"  Neumann  : l=0 : B=PDE,  G1=%8s, G2=%8s\n",(const char*)ghost1,(const char*)ghost2);
        else
	  fPrintF(file,"           : l>0 : B=PDE,  G1=%8s, G2=%8s\n",(const char*)ghost1,(const char*)ghost2);
      }

    }
    aString corner="extrap  ", lowerLevelCorner="extrap  ";
    if( parameters.useSymmetryCornerBoundaryCondition )
    {
      sPrintF(corner,"taylor%i  ",orderOfAccuracy); 
      lowerLevelCorner=corner;
    }
    
    if( ogesSmoother!=NULL )
      printF("\n Oges smoother: %s\n",(const char*)ogesSmoother[0].parameters.getSolverName());
   
    fPrintF(file," Corner BC: l=0 : %s, l>0 %8s\n",(const char*)corner,(const char*)lowerLevelCorner);
    

    if( np>1 )
    { // output parallel distribution: 
      ((CompositeGrid &)mgcg).displayDistribution("Ogmg",file);
    }

    fPrintF(file,"\n"
	    "    Ogmg, Statistics  %s, grids=%i, cycles=%i, gridPoints=%8i, number of processors=%i\n"
	    "    ----------------                  time (s)  time/cycle  percentage\n"
	    " smooth..(includes bc's)...............%6.2e  %6.2e   %6.2f%% \n"
	    " defect.(excluding those in smooth)....%6.2e  %6.2e   %6.2f%% \n"
	    " fine to coarse........................%6.2e  %6.2e   %6.2f%% \n"
	    " coarse to fine........................%6.2e  %6.2e   %6.2f%% \n"
	    " direct solve on coarsest level........%6.2e  %6.2e   %6.2f%% \n"
	    " miscellaneous.........................%6.2e  %6.2e   %6.2f%% \n"
	    " sum of above..........................%6.2e  %6.2e   %6.2f%%  \n"
            " Details:\n"
            "    defect called from smooth..........%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
            "    relaxation part of smooth..........%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
            "    extra boundary smoothing...........%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
            "    extra interpolation smoothing......%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
            "    tridiagonal factor part of smooth..%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
            "    tridiagonal solve part of smooth...%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    interpolation......................%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    boundary conditions................%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "      (bcOpt=%6.2e extrap=%6.2e setup=%6.2e geom=%6.2e finish=%6.2e total=%6.2e)\n"
	    "    initial guess with FMG.............%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    fine to coarse BC's................%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    interp coarse from fine............%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    compute norms of defect............%6.2e  %6.2e  (%6.2f%%) (already counted)\n"
	    "    ghost boundary update..............%6.2e  %6.2e  (%6.2f%%) (already counted)\n\n"
	    " total.................................%6.2e  %6.2e   %6.2f%% \n\n"
            " initialize............................%6.2e  %6.2e   %6.2f%%  (not counted above)\n"
            "    build extra levels.................%6.2e  %6.2e   %6.2f%%  (already counted)\n"
            "    operator averaging.................%6.2e  %6.2e   %6.2f%%  (already counted)\n"
            "    build predefined equations.........%6.2e  %6.2e   %6.2f%%  (already counted)\n"
            "    oges smoother init.................%6.2e  %6.2e   %6.2f%%  (already counted)\n\n"
            " TOTAL (solve+initialize)..............%6.2e  %6.2e\n"
	    " total number of grid points = %i \n"
	    " total number of cycles      = %i \n",
            (const char*)gridName,
            mgcg.numberOfComponentGrids(),cycles,numberOfGridPoints,np,
	    tm[timeForSmooth],       tm[timeForSmooth]/cycles,       100.*tm[timeForSmooth]/total,
	    tm[timeForDefect],       tm[timeForDefect]/cycles,       100.*tm[timeForDefect]/total,
	    tm[timeForFineToCoarse], tm[timeForFineToCoarse]/cycles, 100.*tm[timeForFineToCoarse]/total,
	    tm[timeForCoarseToFine], tm[timeForCoarseToFine]/cycles, 100.*tm[timeForCoarseToFine]/total,
	    tm[timeForDirectSolver], tm[timeForDirectSolver]/cycles, 100.*tm[timeForDirectSolver]/total,
	    tm[timeForMiscellaneous],tm[timeForMiscellaneous]/cycles,100.*tm[timeForMiscellaneous]/total,
	    sumTotal,                sumTotal/cycles,                100.*sumTotal/total,
	    tm[timeForDefectInSmooth],tm[timeForDefectInSmooth]/cycles,100.*tm[timeForDefectInSmooth]/total,
	    tm[timeForRelaxInSmooth],tm[timeForRelaxInSmooth]/cycles,100.*tm[timeForRelaxInSmooth]/total,
	    tm[timeForBoundarySmooth],tm[timeForBoundarySmooth]/cycles,100.*tm[timeForBoundarySmooth]/total,
	    tm[timeForInterpolationSmooth],tm[timeForInterpolationSmooth]/cycles,100.*tm[timeForInterpolationSmooth]/total,
	    tm[timeForTridiagonalFactorInSmooth],tm[timeForTridiagonalFactorInSmooth]/cycles,
                    100.*tm[timeForTridiagonalFactorInSmooth]/total,
	    tm[timeForTridiagonalSolverInSmooth],tm[timeForTridiagonalSolverInSmooth]/cycles,
                    100.*tm[timeForTridiagonalSolverInSmooth]/total,
	    tm[timeForInterpolation],tm[timeForInterpolation]/cycles,100.*tm[timeForInterpolation]/total,
	    tm[timeForBoundaryConditions],tm[timeForBoundaryConditions]/cycles,100.*tm[timeForBoundaryConditions]/total,
	    timeForBCOpt,timeForExtrapolationBC,timeForSetupBC,timeForBCUpdateGeometry,timeForFinishBC,timeForBC,
	    tm[timeForFullMultigrid],tm[timeForFullMultigrid]/cycles,
	    100.*tm[timeForFullMultigrid]/total,
	    tm[timeForFineToCoarseBC], tm[timeForFineToCoarseBC]/cycles, 100.*tm[timeForFineToCoarseBC]/total,
            tm[timeForInterpolateCoarseFromFine], tm[timeForInterpolateCoarseFromFine]/cycles, 
	                                          tm[timeForInterpolateCoarseFromFine]*100./total,
	    tm[timeForDefectNorm], tm[timeForDefectNorm]/cycles, 100.*tm[timeForDefectNorm]/total,
            tm[timeForGhostBoundaryUpdate],tm[timeForGhostBoundaryUpdate]/cycles,
                 100.*tm[timeForGhostBoundaryUpdate]/total,
	    tm[timeForSolve],        tm[timeForSolve]/cycles,        100.*tm[timeForSolve]/total,
	    tm[timeForInitialize],tm[timeForInitialize]/cycles,100.*tm[timeForInitialize]/total,
	    tm[timeForBuildExtraLevels],tm[timeForBuildExtraLevels]/cycles,100.*tm[timeForBuildExtraLevels]/total,
	    tm[timeForOperatorAveraging],tm[timeForOperatorAveraging]/cycles,100.*tm[timeForOperatorAveraging]/total,
	    tm[timeForBuildPredefinedEquations],tm[timeForBuildPredefinedEquations]/cycles,
                         100.*tm[timeForBuildPredefinedEquations]/total,
	    tm[timeForOgesSmootherInit],tm[timeForOgesSmootherInit]/cycles,100.*tm[timeForOgesSmootherInit]/total,
            tm[timeForSolve]+tm[timeForInitialize],(tm[timeForSolve]+tm[timeForInitialize])/cycles,
	    numberOfGridPoints,cycles
      );
    


    // the average time/cycle for baseLineNumberOfGridPoints should be about 1.
    real baseLineNumberOfGridPoints=100000;   // choose this so the scaleTime is about 1 -- valid for the sun ultra 10
    real gridPointsNormalization=baseLineNumberOfGridPoints/numberOfGridPoints;
    real scaledTime= (tm[timeForSolve]/cycles)*gridPointsNormalization;
    
    if( parameters.outputMatlabFile )
    {
      // Output convergence rates with and without the first cycle -- 
      // the first cycle usually skews the results to be too good.
      for( int ii=1; ii<=2; ii++ )
      {
	int cycleStart=ii, cycleEnd=numberOfCycles;
	real residualReduction=cycleResults(cycleEnd,defectPerCycle)/cycleResults(cycleStart-1,defectPerCycle);
	real aveCR=pow(residualReduction,1./(cycleEnd-cycleStart+1));
	real wu=sum(cycleResults(Range(cycleStart,cycleEnd),workUnitsPerCycle));
	real aveECR= pow(residualReduction,1./max(1.,wu));
	fPrintF(file,"\nIteration=%i..%i : Total WU=%8.2e, total res reduction=%8.2e, ave CR=%5.4f ave ECR=%5.3f\n"
		"  MaxRes=%8.2e, TR10=%8.2e (time to reduce residual by a factor of 10) TR10/(M/np)=%8.2e\n",
		cycleStart,cycleEnd,wu,residualReduction,aveCR,aveECR,maximumResidual,aveTR10,aveTR10M);
      
      }
    }
    else
    {
      fPrintF(file,"\nIteration=%i..%i : Total WU=%8.2e, total res reduction=%8.2e, ave CR=%5.4f ave ECR=%5.3f np=%i\n"
              "  MaxRes=%8.2e, TR10=%8.2e (time to reduce residual by a factor of 10) TR10/(M/np)=%8.2e\n",
	      1,numberOfCycles,totalWorkUnits,totalResidualReduction,averageCR,averageECR,np,maximumResidual,
              aveTR10,aveTR10M);

      if( numberOfSolves>1 )
	fPrintF(file,"\n--Number of solves=%i: total WU=%8.2e, total cycles=%i, cycles/solve=%5.1f, ave. ECR=%5.3f\n",
		numberOfSolves,sumTotalWorkUnits,totalNumberOfCycles,real(totalNumberOfCycles)/numberOfSolves,
                averageEffectiveConvergenceRate/numberOfSolves);
    }


    // Here is the storage required in real's per grid point
    const real realsPerGridPoint = (totalMem*1024.*1024.)/numberOfGridPoints/sizeof(real);
    fPrintF(file,"\n==== memory/proc: [min=%g,ave=%g,max=%g](Mb), max-recorded=%g (Mb), total=%g (Mb), %5.1f reals/(grid-pt)\n",
	    minMem,aveMem,maxMem,maxMemRecorded,totalMem,realsPerGridPoint);
    

    real size=sizeOf(file);
    fPrintF(file," storage allocated = %6.2e MBytes, %7.1f bytes/(grid point) or "
	    "%6.1f reals/(grid point)\n\n",size/(1024.*1024.),
          size/numberOfGridPoints,size/numberOfGridPoints/sizeof(real));


    fPrintF(file,"*** timeForNeumannBC=%8.2e timeForBC=%8.2e "
	    " timeForFinishBC=%8.2e timeForBCFinal=%8.2e\n"
	    "   timeForGeneralNeumannBC=%8.2e timeForExtrapolationBC=%8.2e \n     "
	    " timeForSetupBC=%8.2e, timeForBCWhere=%8.2e, timeForBCOpt=%8.2e timeForBC4Extrap=%8.2e\n",
	    timeForNeumannBC,timeForBC,timeForFinishBC,timeForBCFinal,timeForGeneralNeumannBC,
	    timeForExtrapolationBC,timeForSetupBC,timeForBCWhere,timeForBCOpt,timeForBC4Extrap );

    if( Ogmg::debug & 4 )
    {
      fPrintF(file,"***time timeForNeumannBC=%8.2e timeForBC=%8.2e "
	      " timeForFinishBC=%8.2e timeForGeneralNeumannBC=%8.2e timeForExtrapolationBC=%8.2e "
	      " timeForSetupBC=%8.2e, timeForBCWhere=%8.2e\n",
              timeForNeumannBC,timeForBC,timeForFinishBC,timeForGeneralNeumannBC,
	      timeForExtrapolationBC,timeForSetupBC,timeForBCWhere );
    }
    
    
    fPrintF(file,"\n");
    

//    real averageTCR=pow(averageCR,1./(max(REAL_EPSILON,scaledTime)));
//      fPrintF(file,"\nIteration=1..%i : Total WU=%8.2e, total res reduction=%8.2e, ave CR=%5.4f ave ECR=%5.3f"
//                   " aver TCR=%7.2f (scaledtime=%8.1e)\n",
//  	   numberOfCycles-1,totalWorkUnits,totalResidualReduction,
//                  averageCR,averageECR,averageTCR,scaledTime);

  } // end for io
  fPrintF(infoFile,
          "\\end{verbatim}\n"
          "} %% end footnotesize\n");

}





// ===================================================================================
/// \brief Once the coefficient matrix is known on the finest level we can automatically
///   build the coeff matrices on coarser levels.
// ===================================================================================
int Ogmg::
buildCoefficientArrays()
{
  real time0=getCPU();
  CompositeGrid & mgcg = multigridCompositeGrid();
  
  if( debug & 4 )
    printF("\n ------------ Entering Ogmg::buildCoefficientArrays -----\n\n");

  lineSmoothIsInitialized=false;  // assume that all grids have changed

  realCompositeGridFunction & coeff = cMG.numberOfMultigridLevels()>1 ? cMG.multigridLevel[0] : cMG;

  // This next is no longer needed -- the boundaryConditionArray should already be set *wdh* 070216
  // initializeBoundaryConditions(coeff);

  if( debug & 4 ) printF("**** time for initializeBoundaryConditions=%e\n",getCPU()-time0);

  // initialize arrays used in the case of constant coefficients for predefined equations:
  initializeConstantCoefficients();
  
  real timeForOperators=0.;

  const int width = orderOfAccuracy+1;  // 3 or 5
  const int stencilSize=int( pow(double(width),double(mgcg.numberOfDimensions()))+1.5 );
  const int orderOfExtrapolation = orderOfAccuracy==2 ? 3 : 4;  // 5
  
  if( numberOfExtraLevels>0 )
  {
     // we need operators to apply boundary conditions. *** can we fix this ? ****
    if( false 
        // true  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++ *wdh* 040830 +++++++++++++++
        && operatorsForExtraLevels!=NULL )
    {
      delete [] operatorsForExtraLevels;     // **** don't have to delete if we have the same number of extra levels!
      operatorsForExtraLevels=NULL;
    }
    if( operatorsForExtraLevels==NULL )
      operatorsForExtraLevels= new CompositeGridOperators [numberOfExtraLevels];
 
    int level0=0;
    int l;
    for( l=0; l<numberOfExtraLevels; l++ )
    {
      int level=level0+l;

      // we need operators to apply boundary conditions.
      RealCompositeGridFunction & cl = cMG.multigridLevel[level+1];
      CompositeGrid & cgl = *cl.getCompositeGrid();

      if( debug & 16 )
        printF("+++++buildCoefficientArrays: level+1=%i cgl.rcData=%i \n",level+1,cgl.rcData);

      CompositeGridOperators & op = operatorsForExtraLevels[l];
      op.setStencilSize(stencilSize);
      op.setOrderOfAccuracy(orderOfAccuracy);

      real time1=getCPU();
      op.updateToMatchGrid(cgl);
      cl.setOperators(op);
      timeForOperators+=getCPU()-time1;

      // *************************************
      // **** Build the averaged operator ****
      // *************************************
      operatorAveraging(cMG,level);

      if( false && orderOfAccuracy==4 )
      { 
        // **** for testing *****

        printF("\n ++++++++++++++++ over-write averaged operator with laplacian from operators ++++++++++++++\n\n");
        // cgl.update(MappedGrid::THEinverseVertexDerivative );
	
        int stencilSize=cl[0].getLength(0);
	if( stencilSize==0 )  // for rectangular grids
	{
	   const int width = orderOfAccuracy+1;  // 3 or 5
           stencilSize=int(pow(double(width),double(cgl.numberOfDimensions()))+1.5);
	}
        if( Ogmg::debug & 4 ) cl.display("*************Coefficients from averaging ***********************","%5.1f ");
	
	
	op.setStencilSize(stencilSize);
        cl.destroy();
        cl.updateToMatchGrid(cgl,stencilSize);
        const int numberOfGhostLines=2;
        cl.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines);  
	cl.setOperators(op);

	cl=op.laplacianCoefficients();   // get the coefficients for the Laplace operator 
        // fill in the coefficients for the boundary conditions
	cl.applyBoundaryConditionCoefficients(0,0,BCTypes::dirichlet,BCTypes::allBoundaries);
	cl.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries);
        if( orderOfAccuracy==4 )
	{
	  BoundaryConditionParameters bcParams;
	  bcParams.ghostLineToAssign=2;
          bcParams.orderOfExtrapolation=orderOfExtrapolation; // orderOfAccuracy+1;
	  cl.applyBoundaryConditionCoefficients(0,0,BCTypes::extrapolate,BCTypes::allBoundaries,bcParams); // extrap 2nd ghost line
	}
	
        if( Ogmg::debug & 4 ) 
  	  cl.display(sPrintF(buff,"buildCoefficientArray: coeff from operators on level=%i",level+1),debugFile,"%9.1e");

      }
      
    }

    bool nullVectorNeedsToBeComputed=parameters.problemIsSingular && parameters.projectRightHandSideForSingularProblem;
    if( nullVectorNeedsToBeComputed )
    {
      // if the null vector needs to be computed, check to see if we can read it from a file (i.e. it was
      // saved from a previous computation)
      nullVectorNeedsToBeComputed= readLeftNullVector()!=0;
    }

    // do not finishBoundaryConditions until all levels have been generated since we don't
    // want the interpolation points to be inserted until the end.
    for( l=0; l<numberOfExtraLevels; l++ )
    {
      int level=level0+l;

      RealCompositeGridFunction & cl = cMG.multigridLevel[level+1];
      CompositeGrid & cgl = *cl.getCompositeGrid();

      CompositeGridOperators & opc = *cl.getOperators();
      opc.setStencilSize(stencilSize);
      opc.setOrderOfAccuracy(orderOfAccuracy);

      // We only need to apply finish boundary conditions on the coarsest level if 
      //    we are calling a direct solver *wdh* 011107
      // We need to finish BC's for singular problems if we need to compute the leftNullVectors

      setCornerBoundaryConditions( bcParams, level+1 ); // *wdh* 100713 
      
      if( (l==(numberOfExtraLevels-1) && parameters.useDirectSolverOnCoarseGrid) || 
          applyFinishBoundaryConditions || nullVectorNeedsToBeComputed )
      {
	if( debug & 4 ) 
	{
	  
          printF("*****buildCoefficientArray:  cl.finishBoundaryConditions(); for coarsest level %i\n",level+1);
          printF(" cornerBC: %i\n",bcParams.getCornerBC(0,0,0));
	}
	
        // bcParams.orderOfExtrapolation=2;
	
	if( false && parameters.useSymmetryCornerBoundaryCondition ) // these need to be implemented
	{
	  BoundaryConditionParameters::CornerBoundaryConditionEnum cornerBC=
	    orderOfAccuracy==2 ? BoundaryConditionParameters::taylor2ndOrderEvenCorner :
	    BoundaryConditionParameters::taylor4thOrderEvenCorner;

	  // cornerBC=BoundaryConditionParameters::evenSymmetryCorner; // ********************************
	  // cornerBC=BoundaryConditionParameters::oddSymmetryCorner; // ********************************
    
    
	  bcParams.setCornerBoundaryCondition(cornerBC);  // this sets all corners and edges
	}
	
	cl.finishBoundaryConditions(bcParams);

      }
      else
      {
        // we only call finishBoundaryConditions on each grid to fill in corner points
        // we do not want the interpolation equations filled in
        for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
	{
          // bcParams.orderOfExtrapolation=3; // ***** test for mixed BC ***
          if( !(equationToSolve!=OgesParameters::userDefined && cgl[grid].isRectangular() && 
		(level<mgcg.numberOfMultigridLevels()-1 || !parameters.useDirectSolverOnCoarseGrid)) )
	  {
	    if( debug & 4 ) 
              printF("*****buildCoefficientArray:  cl[grid].finishBoundaryConditions() for level %i\n",level+1);
   	    cl[grid].finishBoundaryConditions(bcParams);
	  }
	  
	}
	
      }
      

      // *** a corner with two adjacent neumann BC's had best results with orderOfExtrap==1 for
      // coarser levels. This didn't seem to hard dirichlet BC's --> probably the
      // best would be u(-1,-1)=u(1,1) ?? 
      if( false )
      {
	bcParams.orderOfExtrapolation=2;
	cl.finishBoundaryConditions(bcParams);
	
	cl[0](0,-1,-1,0)=1.;
	cl[0](1,-1,-1,0)=0.;
	cl[0](2,-1,-1,0)=-1.;

        int n1 = cgl[0].gridIndexRange(End,axis1);
	cl[0](0,n1+1,-1,0)= 1.;
	cl[0](1,n1+1,-1,0)= 0.;
	cl[0](2,n1+1,-1,0)=-1.;

        int n2 = cgl[0].gridIndexRange(End,axis2);
	cl[0](0,-1,n2+1,0)=1.;
	cl[0](1,-1,n2+1,0)=0.;
	cl[0](2,-1,n2+1,0)=-1.;
	cl[0](0,n1+1,n2+1,0)= 1.;
	cl[0](1,n1+1,n2+1,0)= 0.;
	cl[0](2,n1+1,n2+1,0)=-1.;

      }
      
      if( debug & 64 )
      {
	cl.display(sPrintF(buff,"buildCoefficientArray: coeff on level=%i",level+1),debugFile,"%9.1e");
      }
    }
    // ******** do this here after we have averaged  

    if( applyFinishBoundaryConditions )
    {
      if( debug & 4 ) printF("*****buildCoefficientArray:  coeff.finishBoundaryConditions(); for finest level\n");
      coeff.finishBoundaryConditions();
    }
    else
    {
      // printF("*****buildCoefficientArray:  DO NOT call coeff.finishBoundaryConditions() for finest level\n");
      // we only call finishBoundaryConditions on each grid to fill in corner points
      // we do not want the interpolation equations filled in
      if( parameters.problemIsSingular && nullVectorNeedsToBeComputed )
      {
	if( debug & 4 ) printF("*****buildCoefficientArray:problemIsSingular  coeff.finishBoundaryConditions() for finest level\n");
	coeff.finishBoundaryConditions(bcParams);
      }
      else
      {
	for( int grid=0; grid<mgcg.numberOfComponentGrids(); grid++ )
	{
          if( !(equationToSolve!=OgesParameters::userDefined && mgcg[grid].isRectangular()) )
	  {
	    if( debug & 4 ) printF("*****buildCoefficientArray:  coeff[grid=%i].finishBoundaryConditions() "
                           "for finest level\n",grid);
	    coeff[grid].finishBoundaryConditions(bcParams);
	  }
	  
	}
      }
      
    }
    

    if( debug & 64 )
    {
      coeff.display(sPrintF(buff,"buildCoefficientArray: coeff on finest grid"),debugFile,"%9.1e");
    }
    // cMG.multigridLevel[0].finishBoundaryConditions();  // *** ???
    
  }
  else
  {
    // number of extra levels is zero 
    if( parameters.useDirectSolverForOneLevel )
    {
      // *wdh* 040830 cMG.multigridLevel[0].finishBoundaryConditions(bcParams);
      cMG.finishBoundaryConditions(bcParams);
    }
    
  }
  
  if( debug & 4 ) printF(" *** timeForOperators=%8.2e\n",timeForOperators);
  

  if( debug & 64 )
  {
    cMG.multigridLevel[mgcg.numberOfMultigridLevels()-1].display(
                 sPrintF(buff,"After finishBC:coeff on coarsest level=%i",mgcg.numberOfMultigridLevels()-1),
            debugFile,"%8.2e ");
  }

  directSolver.setGrid(mgcg.multigridLevel[mgcg.numberOfMultigridLevels()-1]); // direct solver for coarse grid

  // printF("*** coeff.multigridLevel[mgcg.numberOfMultigridLevels()-1].getIsACoefficientMatrix= %i \n",
  //        coeff.multigridLevel[mgcg.numberOfMultigridLevels()-1].getIsACoefficientMatrix());
  
  // supply coefficients to coarse grid solver
  if( parameters.useDirectSolverOnCoarseGrid )
  {
    if( mgcg.numberOfMultigridLevels()>1 )
      directSolver.setCoefficientArray( cMG.multigridLevel[mgcg.numberOfMultigridLevels()-1] );   
    else
      directSolver.setCoefficientArray( cMG );
  }
  
  if( parameters.problemIsSingular )
  {
    // parameters.setProblemIsSingular(true);
    printF("*** problem is singular, directSolver.setCompatibilityConstraint \n");
    parameters.ogesParameters.set(OgesParameters::THEcompatibilityConstraint,true);
  }
  // directSolver.initialize();                  // initialize oges (assigns classify array used below)

  tm[timeForInitialize]+=getCPU()-time0;
  
  if( debug & 4 ) printF("******time for buildCoefficientArray= %8.2e\n",getCPU()-time0);
  

//    if( true )
//    {
//      for( int l=0; l<numberOfExtraLevels; l++ )
//      {
//        int level=l;

//        // we need operators to apply boundary conditions.
//        RealCompositeGridFunction & cl = cMG.multigridLevel[level+1];
//        CompositeGrid & cgl = *cl.getCompositeGrid();
//        printF("+++++buildCoefficientArrays:END level+1=%i cgl.rcData=%i \n",level+1,cgl.rcData);
      
//      }
//    }
  
  return 0;
}





//\begin{>>OgmgInclude.tex}{\subsection{setCoefficientArray}}
int Ogmg::
setCoefficientArray( realCompositeGridFunction & coeff,
                     const IntegerArray & bc  /* =Overture::nullIntArray() */,
                     const RealArray & bcData /* ==Overture::nullRealArray() */ )
//==================================================================================
// /Description:
//    Supply the coefficient matrix, and optionally supply boundary conditions.
// /matrix (input) : a coefficient matrix defined on all levels.
//
// /bc(0:1,0:2,numberOfComponentGrids) (input): boundary conditions, Ogmg::dirichlet, neumann or mixed.
//    If this array is NOT supplied then the boundary conditions, dirichlet, neumann, mixed should be
//  given in cg[grid].boundaryCondition(side,axis)
// /bcData(0:?,0:1,0:2,numberOfComponentGrids) (input) : data for the boundary conditions.
//    For mixed boundary conditions bcData(0:1,side,axis,grid) = (a0,a1) are the coefficients in the 
//    mixed condition: a0*u + a1*u.n = g
//
// Ogmg::updateToMatchGrid should have already been called at this point so that
// the the multigrid-composite grid with extra levels has already been built.
//\end{OgmgInclude.tex} 
//==================================================================================
{
  real time0=getCPU();
  if( equationToSolve!=OgesParameters::userDefined )
    return 1;

  CompositeGrid & mgcg = multigridCompositeGrid();
  if( numberOfExtraLevels>0 )
  {
    Range all;
    int stencilSize=coeff[0].getLength(0);

    cMG.destroy();  // *wdh* 040830 -- this was needed by moveAndSolve --- ***** FIX THIS RIGHT ****
    
    cMG.updateToMatchGrid(mgcg,stencilSize,all,all,all);
    const int numberOfGhostLines=orderOfAccuracy/2; // *wdh* 2013/11/30
    cMG.setIsACoefficientMatrix(true,stencilSize,numberOfGhostLines); 
    
    cMG.multigridLevel[0].setOperators(*coeff.getOperators()); // *wdh* 011213
    
    if( debug & 4 )
    {
      printF("Ogmg::setCoefficientArray: cMG: numberOfComponentGrids=%i, numberOfGrids=%i, sizeOf=%12.0f\n",
	     cMG.numberOfComponentGrids(),cMG.numberOfGrids(),cMG.sizeOf());
      for( int ll=0; ll<mgcg.numberOfMultigridLevels(); ll++ )
      {
	printf("cMG.multigridLevel[%i].sizeOf()=%12.0f\n",ll,cMG.multigridLevel[ll].sizeOf());
      }
      printF("cMG.sizeOf()=%12.0f\n",cMG.sizeOf());
    }
    
    for( int grid=0; grid<coeff.numberOfComponentGrids(); grid++ )
    {
      if( false )
       cMG[grid]=coeff[grid];  // copy existing levels  *** reference ??? *********** ********** fix this ****
      else
      {
	cMG[grid].reference(coeff[grid]);
	cMG.multigridLevel[0][grid].reference(cMG[grid]);  // need to do this too
      }
      
      cMG[grid].setOperators(*coeff[grid].getOperators());
      // cMG.multigridLevel[0][grid].setOperators(*coeff[grid].getOperators());
    }

  }
  else
  {
    cMG.reference(coeff);
  }

  // assign boundary conditions
  setBoundaryConditions( bc,bcData );

  // ::display(boundaryCondition,"setCoefficientArray: boundaryCondition after setBoundaryConditions( bc );");

  tm[timeForInitialize]+=getCPU()-time0;

  buildCoefficientArrays();
  
  return 0;
}


//\begin{>>OgmgInclude.tex}{\subsection{setBoundaryConditions}}
int Ogmg::
setBoundaryConditions(const IntegerArray & bc_,
		      const RealArray & bcData /* =Overture::nullRealArray() */ )
//==================================================================================
// /Description:
//    Set the boundary conditions. This is a private routine.
//
// This function assigns the internal arrays:
//
//   boundaryCondition(side,axis,grid) = extrapolate
//                                     = equation
//   bc(side,axis,grid) = dirichlet
//                      = neumann
//                      = mixed
// 
//\end{OgmgInclude.tex} 
//==================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  const int numberOfComponentGrids=mgcg.numberOfComponentGrids();
  
  boundaryCondition.redim(2,3,numberOfComponentGrids);
  boundaryCondition=0;
  bc.redim(2,3,numberOfComponentGrids);

  bcSupplied=false;
  if( bc_.getLength(0)>0 )
  {
    bcSupplied=true;
    bc=bc_;
  }

  bcDataSupplied=bcSupplied==true && bcData.getLength(0)>0;
  
  const int numData=2;
  boundaryConditionData.redim(numData,2,3,numberOfComponentGrids);
  boundaryConditionData=0.;
  
  for( int grid=0; grid<numberOfComponentGrids; grid++ )
  {
    MappedGrid & mg = mgcg[grid];  

    for(int axis=axis1; axis<mg.numberOfDimensions(); axis++ )
    {
      for( int side=Start; side<=End; side++ )
      {
	if( !bcSupplied )
	{
	  bc(side,axis,grid)=mg.boundaryCondition(side,axis);
	}
	if( mg.boundaryCondition(side,axis) > 0 )
	{
	  if( Ogmg::debug & 4 )
	    printF("Ogmg::setBoundaryConditions: (side,axis,grid)=(%i,%i,%i) bc(side,axis,grid)=%i (bcDataSupplied=%i)\n",
		   side,axis,grid,bc(side,axis,grid),bcDataSupplied);


	  if( bc(side,axis,grid)==OgmgParameters::dirichlet )
	  {
	    boundaryCondition(side,axis,grid)=OgmgParameters::extrapolate;  // default for now
            // mg.boundaryCondition()(side,axis)=bc(side,axis,grid);
	  }
          else if( bc(side,axis,grid)==OgmgParameters::extrapolate )
	  { // *new* 100412 -- support extrapolation BC
	    boundaryCondition(side,axis,grid)=OgmgParameters::equation;
	    boundaryConditionData(0,side,axis,grid)=bcData(0,side,axis,grid);

            if( Ogmg::debug & 4 )
	      printF(" Ogmg::setBoundaryConditions: (side,axis,grid)=(%i,%i,%i) bc=extrapolate, orderOfExtrap=%i\n",
		     side,axis,grid,int( boundaryConditionData(0,side,axis,grid)+.5) );
	    
            int orderOfExtrap=int( boundaryConditionData(0,side,axis,grid)+.5 );
            if( orderOfExtrap!=2 )
	    {
              // for now we only support orderOfExtrapolation==2 
	      printF("Ogmg::setBoundaryConditions:WARNING: currently only extrapolation of order 2 is \n"
                     "     supported at an extrapolation boundary. I am setting the requested value of %i to 2.\n",
		     orderOfExtrap);
              boundaryConditionData(0,side,axis,grid)=2;  // default order of extrapolation
	    }
	  }
	  else if( bc(side,axis,grid)==OgmgParameters::neumann || 
                   bc(side,axis,grid)==OgmgParameters::mixed )
	  {
//             // -- For now we just use a Neumann BC if the user has requested extrapolate -- *wdh* 100327
// 	    if( bc(side,axis,grid)==OgmgParameters::extrapolate )
// 	    {
//               bc(side,axis,grid)=OgmgParameters::neumann;  // *NOTE*
// 	      if( Ogmg::debug & 2 )
// 		printF("Ogmg:setBoundaryConditions:WARNING: using Neumann BC instead of extrapolate.\n");
// 	    }
	    
	    boundaryCondition(side,axis,grid)=OgmgParameters::equation;
            // mg.boundaryCondition()(side,axis)=bc(side,axis,grid);
            if( bcDataSupplied )
	    {
	      if( bc(side,axis,grid)==OgmgParameters::neumann )
	      {
		boundaryConditionData(0,side,axis,grid)=0.;
		boundaryConditionData(1,side,axis,grid)=1.;
	      }
	      else if( bc(side,axis,grid)==OgmgParameters::mixed )
	      {
		boundaryConditionData(0,side,axis,grid)=bcData(0,side,axis,grid);
		boundaryConditionData(1,side,axis,grid)=bcData(1,side,axis,grid);

                if( Ogmg::debug & 2 )
		  printF("Ogmg::setBoundaryConditions: (side,axis,grid)=(%i,%i,%i) "
			 " mixed BC: a0,a1=%8.2e,%8.2e\n",side,axis,grid,bcData(0,side,axis,grid),
			 bcData(1,side,axis,grid)  );

		if( bcData(0,side,axis,grid)!=0. && bcData(1,side,axis,grid)==0. )
		{
		  // this is really a Dirichlet BC
                  if( Ogmg::debug & 4 )
  		    printF("Ogmg::setBoundaryConditions: (side,axis,grid)=(%i,%i,%i) "
			   " mixed BC is really dirichlet\n",side,axis,grid);
		
		  boundaryCondition(side,axis,grid)=OgmgParameters::extrapolate; 
                  bc(side,axis,grid)=OgmgParameters::dirichlet; // *wdh* 040831
		  
		  // mg.boundaryCondition()(side,axis)=OgesParameters::dirichlet;
		}
		else if( bcData(0,side,axis,grid)==0. && bcData(1,side,axis,grid)==0. )
		{
		  printF("Ogmg::setBoundaryConditions:ERROR: bcData==0 for a mixed bc, bc(%i,%i,%i)=%i\n",
			 side,axis,grid,bc(side,axis,grid));
		  Overture::abort();
		}
	      }
	    } // end if bcDataSupplied
	  }
	  else 
	  {
	    printF("Ogmg::setBoundaryConditions:ERROR: unknown bc(%i,%i,%i)=%i\n",
		   side,axis,grid,bc(side,axis,grid));
	    OV_ABORT("error");
	  }
	}
	else
	{
          bc(side,axis,grid)=mg.boundaryCondition(side,axis);  // set to zero or -1 *wdh* 030605
	  
	  boundaryCondition(side,axis,grid)=0;
	}
      }
    }
  }



  return 0;
}



//\begin{>>OgmgInclude.tex}{\subsection{setOrderOfAccuracy}}
int Ogmg::
setOrderOfAccuracy(const int & orderOfAccuracy_)
//==================================================================================
// /Description:
//    Set the order of accuracy (2 or 4).
// /orderOfAccuracy\_ (input) : 
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  if( orderOfAccuracy_==2 || orderOfAccuracy_==4 )
  {
    orderOfAccuracy=orderOfAccuracy_;
    // no longer needed directSolver.setOrderOfAccuracy(orderOfAccuracy);
  }
  else
  {
    printF("Ogmg::setOrderOfAccuracy: ERROR: orderOfAccuracy must be 2 or 4\n");
    return 1;
  }
  return 0;
}


int Ogmg:: 
createNullVector()
// =====================================================================
// /Description:
//   Create the right null vector. This grid function is 1 at all interior
// and boundary points, but zero at interpolation and unused points.
// The vector is normalized to have l2 norm equal to one.
//
// =====================================================================
{
  const bool useOpt=true;
  
  if( Ogmg::debug & 2 )
    printF("Ogmg: create the right null vectors...\n");

  CompositeGrid & mgcg = multigridCompositeGrid();
  rightNullVector.updateToMatchGrid(mgcg);
  Index I1,I2,I3;
  for( int l=0; l<mgcg.numberOfMultigridLevels(); l++ )  // often not needed on the coarsest level***
  {
    real norm=0.;
    for( int grid=0; grid<mgcg.multigridLevel[l].numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = mgcg.multigridLevel[l][grid];
      realArray & nullVector = rightNullVector.multigridLevel[l][grid];
      const intArray & mask = c.mask();
      getIndex( c.gridIndexRange(),I1,I2,I3 );

      if( useOpt )
      {
	OV_GET_SERIAL_ARRAY(real,nullVector,nullVectorLocal);
	int includeGhost=1; // include ghost 
	bool ok = ParallelUtility::getLocalArrayBounds(nullVector,nullVectorLocal,I1,I2,I3,includeGhost);
	if( ok )
	{
	  real *nullVectorp = nullVectorLocal.Array_Descriptor.Array_View_Pointer2;
	  const int nullVectorDim0=nullVectorLocal.getRawDataSize(0);
	  const int nullVectorDim1=nullVectorLocal.getRawDataSize(1);
#define NULLVECTOR(i0,i1,i2) nullVectorp[i0+nullVectorDim0*(i1+nullVectorDim1*(i2))]

	  OV_GET_SERIAL_ARRAY_CONST(int,mask,maskLocal);
	  const int *maskp = maskLocal.Array_Descriptor.Array_View_Pointer2;
	  const int maskDim0=maskLocal.getRawDataSize(0);
	  const int maskDim1=maskLocal.getRawDataSize(1);
#define MASK(i0,i1,i2) maskp[i0+maskDim0*(i1+maskDim1*(i2))]

          nullVectorLocal=0.;
	  int i1,i2,i3;
	  FOR_3D(i1,i2,i3,I1,I2,I3)
	  {
	    if( MASK(i1,i2,i3)!=0 )
	    {
	      NULLVECTOR(i1,i2,i3)=1.;
	      norm++;
	    }
	  }
#undef NULLVECTOR
#undef MASK
	}

      }
      else
      {
	nullVector=0.;
	where( mask(I1,I2,I3) >0 )
	  nullVector(I1,I2,I3)=1.;
	norm+=sum(nullVector);
      }
      
    }
    norm=ParallelUtility::getSum(norm);
    if( norm==0. )
    {
      printF("Ogmg::createNullVector:WARNING: The norm of the null vector is zero on level=%i\n",l);
      norm=1.;
    }
    // normalize so that l2 norm is 1
    if( false )   // ********************************** turn this off *****
      rightNullVector.multigridLevel[l]/=sqrt(norm);
  }
  return 0;
}

real Ogmg::
rightNullVectorDotU( const int & level, const RealCompositeGridFunction & u )
// =====================================================================
// /Description:
//     Determine the inner product of the right null vector times u
//       ccValue =   (rightNullVector, u ) 
// /level (input) : right null vector and u live on this level.
// =====================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  RealCompositeGridFunction & nullVector = rightNullVector.multigridLevel[level];
  real ccValue=0.;
  for( int grid=0; grid<mgcg.multigridLevel[level].numberOfComponentGrids(); grid++ )
    ccValue+=sum( nullVector[grid]*u[grid] );

  return ccValue;
}




//\begin{>>OgmgInclude.tex}{\subsection{interpolate}}
int Ogmg::
interpolate(realCompositeGridFunction & u, const int & grid /* =-1 */, int level /* =-1 */ )
// =====================================================================================
// /Description:
//
// Interpolate here so we can keep track of the cpu time used.
//
// /grid (input) : interpolate this grid only (if possible)

//\end{OgmgInclude.tex} 
//==================================================================================
{
  real time=getCPU();


  if( parameters.maximumNumberOfInterpolationIterations>0 )
    u.getInterpolant()->setMaximumNumberOfIterations(parameters.maximumNumberOfInterpolationIterations);

  if( !parameters.decoupleCoarseGridEquations || level==0 )
  {
    if( grid==-1 )
      u.interpolate();  // interpolate all grids
    else
    {
      // assert( u.getInterpolant()==interpolant[level] );
      u.getInterpolant()->interpolate(grid,u);
    }
    
    // u.display("u after interpolate",debugFile,"%5.1f ");
    
  }
  else 
  {
    // this is for testing the ICMG algorithm where we do not interpolate coarser levels.
    CompositeGrid & cg = *u.getCompositeGrid();
    const int gridStart = grid==-1 ? 0 : grid;
    const int gridEnd   = grid==-1 ? cg.numberOfComponentGrids()-1 : grid;
    for( int gg=gridStart; gg<=gridEnd; gg++ )
    {
      realArray & uu = u[gg];
      const intArray & ip = cg.interpolationPoint[gg]; 
      Range R=cg.numberOfInterpolationPoints(gg);
      if( R.getLength() > 0 )
      {
	if( cg.numberOfDimensions()==2 )
	  uu(ip(R,0),ip(R,1),0)=0.;
	else
	  uu(ip(R,0),ip(R,1),ip(R,2))=0.;
      }
    }
  }
  
  
  tm[timeForInterpolation]+=getCPU()-time;
  return 0;
}




//\begin{>>OgmgInclude.tex}{\subsection{update}}
int Ogmg::
update( GenericGraphicsInterface & gi )
// =====================================================================================
// /Description:
//   Update parameters interactively.
//\end{OgmgInclude.tex} 
//==================================================================================
{
  CompositeGrid & mgcg = multigridCompositeGrid();
  parameters.update(gi,mgcg);
  directSolver.setOgesParameters(parameters.ogesParameters);
  return 0;
}

//\begin{>>OgmgInclude.tex}{\subsection{update}}
int Ogmg::
update( GenericGraphicsInterface & gi, CompositeGrid & cg  )
// =====================================================================================
// /Description:
//   Update parameters interactively. Use this update if you have not already given a
// CompositeGrid to Ogmg (through a constructor or with the updateToMatchGrid function).
//
//\end{OgmgInclude.tex} 
//==================================================================================
{
  if( multigridCompositeGrid.isNull() ||
      cg.numberOfMultigridLevels()> multigridCompositeGrid().numberOfMultigridLevels() ||
      cg.numberOfComponentGrids()!= multigridCompositeGrid().numberOfComponentGrids() )
  {
    parameters.update(gi,cg);
  }
  
  directSolver.setOgesParameters(parameters.ogesParameters);
  return 0;
}




int Ogmg::
get( const GenericDataBase & dir, const aString & name)
// ======================================================================================
/// \brief Get (read) the Ogmg object to a data-base file.
///
// ======================================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Ogmg");

  aString className;
  subDir.get( className,"className" ); 

  delete &subDir;
  return 0;
}


int Ogmg::
put( GenericDataBase & dir, const aString & name) const
// ======================================================================================
/// \brief Put (save) the Ogmg object to a data-base file.
///
// ======================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Ogmg");                            // create a sub-directory 

  aString className="Ogmg";
  subDir.put( className,"className" );

  subDir.put(orderOfAccuracy,"orderOfAccuracy"); 

  CompositeGrid & mgcg = (CompositeGrid&)multigridCompositeGrid();
  mgcg.destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask ); // should we do this ?
  mgcg.put(subDir,"mgcg");
  

  delete &subDir;
  return 0;  
}

int Ogmg::
loadBalance( CompositeGrid & mg, CompositeGrid & mgcg )
// ============================================================================================
/// \brief Assign work-loads and load balance the CompositeGrid and all multigrid levels.
/// 
/// \param mg (input) : initial CompositeGrid with no levels.
/// \param mgcg (input) : CompositeGrid with multigrid levels.
// ============================================================================================
{

  // From GridCollection.C : 
  if( Ogmg::debug & 8 )
  {
    printF("Ogmg: loadBalance: mgcg.numberOfGrids()=%i\n",mgcg.numberOfGrids());
  }
  

  if( parameters.loadBalancer==NULL )
  {
    parameters.loadBalancer = new LoadBalancer;
    // NOTE: when there is only 1 grid then the default load-balancer will always use all-to-all
    parameters.loadBalancer->setLoadBalancer(LoadBalancer::KernighanLin);
  }
    
  // --- Make the master gridDistributionList (GDL) be consistent with the MG level GDL's -- *wdh* 2013/08/29 
  GridDistributionList & masterGDL = mgcg->gridDistributionList;
  masterGDL.resize(mgcg.numberOfGrids(),GridDistribution());

  LoadBalancer & loadBalancer = *parameters.loadBalancer; // could there already be a load balancer with cg ? 

  // Level 0 : set gridDistribution equal to those in the incoming CompositeGrid
  int level=0;
  CompositeGrid & cg = mgcg.multigridLevel[level];
  GridDistributionList & gridDistributionList = cg->gridDistributionList;
  gridDistributionList.resize(cg.numberOfComponentGrids());
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    gridDistributionList[grid]=mg->gridDistributionList[grid];
  }
    
  // load balance levels ...
  for( int l=1; l<mgcg.numberOfMultigridLevels(); l++ )
  {
    int level=l;
    CompositeGrid & cg = mgcg.multigridLevel[level];

    GridDistributionList & gridDistributionList = cg->gridDistributionList;
    // GridDistributionList gridDistributionList;
    
    // work-loads per grid are based on the number of grid points by default:
    loadBalancer.assignWorkLoads( cg,gridDistributionList );

    loadBalancer.determineLoadBalance( gridDistributionList );

    // From GenericGridCollection.C: get: 
    // Assign parallel distribution (if the info is there)
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      gridDistributionList[grid].setMultigridLevel(l);
      
      int pStart=-1,pEnd=0;
      gridDistributionList[grid].getProcessorRange(pStart,pEnd);
      if( Ogmg::debug & 2  )
      {
	printF("Ogmg:loadBalance: assign level=%i grid=%i to processors=[%i,%i]\n",
	       level,grid,pStart,pEnd);
      }
      if( false )
	printF(" level=%i grid=%i gridNumber=%i componentGridNumber=%i\n",level,grid,cg.gridNumber(grid),
	       cg.componentGridNumber(grid));
	  
      cg[grid].specifyProcesses(Range(pStart,pEnd));

      // Also set the distribution in the master GDL:
      const int masterGridNumber = cg.gridNumber(grid); // here is the grid number in the master list of grids
      masterGDL[masterGridNumber]=gridDistributionList[grid]; 
	
    } // end for grid

  } // end for l 
  
  return 0;
}


