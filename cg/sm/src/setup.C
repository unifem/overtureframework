#include "Cgsm.h"
#include "SmParameters.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "AnnulusMapping.h"
#include "MatrixTransform.h"
#include "DataPointMapping.h"
#include "CompositeGridOperators.h"
#include "display.h"
#include "UnstructuredMapping.h"
#include "ParallelUtility.h"
#include "GridStatistics.h"

#include "ULink.h"

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

#define FOR_3D(i1,i2,i3,I1,I2,I3) \
int I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
int I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)

#define FOR_3(i1,i2,i3,I1,I2,I3) \
I1Base =I1.getBase(),   I2Base =I2.getBase(),  I3Base =I3.getBase();  \
I1Bound=I1.getBound(),  I2Bound=I2.getBound(), I3Bound=I3.getBound(); \
for(i3=I3Base; i3<=I3Bound; i3++) \
for(i2=I2Base; i2<=I2Bound; i2++) \
for(i1=I1Base; i1<=I1Bound; i1++)


// ===================================================================================================================
// \brief Setup routine.
/// \details The function is called after the parameters have been assigned (called by setParametersInteractively).
///        This function will output the header information that summarizes the problem being solved and the values of
///        the various parameters.
/// \param time (input) : current time.
// ===================================================================================================================
void Cgsm::
setup(const real & time /* = 0. */ )
{
  // -- these are used in adaptGrids -- (add to time step)
  if( realPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  if( imaginaryPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);

  // ---- For nonlinear solvers we need to adjust the time step ----
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  int & maximumStepsBetweenComputingDt= parameters.dbase.get<int>("maximumStepsBetweenComputingDt");
  if( pdeVariation==SmParameters::godunov )
  {
    const int pdeTypeForGodunovMethod = parameters.dbase.get<int >("pdeTypeForGodunovMethod");
    if( pdeTypeForGodunovMethod!=0 )
    {
      // nonlinear models we recompute dt every this many steps 
      maximumStepsBetweenComputingDt=2;
    }
  }
  else
  {
    maximumStepsBetweenComputingDt=INT_MAX;
  }
  

  // **** now build grid functions *****
  setupGridFunctions();

  // *wdh* 090314
  // For AMR computations build the AMR grid structure for the initial conditions.
  if( parameters.dbase.get<bool>("adaptiveGridProblem") )
  {

    // *wdh* 090829  -- for AMR we extrapolate interpolation neighbours -- is this right?
    parameters.dbase.get<int >("extrapolateInterpolationNeighbours")=true; 

    buildAmrGridsForInitialConditions();

    // cg.reference(gf[0].cg);  // *** is this correct ? 090314
    // cg.getInterpolant()->updateToMatchGrid(cg);

    // should this be put somewhere else ? 
    for( int n=0; n<numberOfTimeLevels; n++ )
    {
      gf[n].u.setOperators(*cgop);
    }

  }

  // --- output the main header ----
  outputHeader();
  
  // --- Evaluate variable material properties ---
  setVariableMaterialProperties( gf[current], gf[current].t );

  // printF("Cgsm::setup:INFO: initialize the solution...\n");
  // initializeSolution();

  // ---- this next is from initializeSolution: 

  // Determine the time independent and spatially varying BC's such as the parabolic inflow BC profile
  if( parameters.bcVariesInSpace() )  
  {
    timeIndependentBoundaryConditions(gf[current]); 
  }


  // for FOS check the agreement between the initial stress components and the values computed from u 
  checkDisplacementAndStress( current, time );

}


//==============================================================================================
/// \brief Update geometry arrays, solution at old times etc. after the time step has changed.
//==============================================================================================
int Cgsm::
updateForNewTimeStep(GridFunction & cgf, real & dt )
{
  SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  int & debug = parameters.dbase.get<int >("debug");
  
  if( pdeModel==SmParameters::linearElasticity && 
      (pdeVariation==SmParameters::nonConservative || 
       pdeVariation==SmParameters::conservative) )
  {
    // Initial conditions
    printF("Cgsm::updateForNewTimeStep:Assign solution at t-dt: t-dt=%9.3e dt=%9.3e\n",cgf.t-dt,dt);

    assert( cgf.t==parameters.dbase.get<real>("tInitial") );
    assert( numberOfTimeLevels>0 );
    int prev = (current -1 + numberOfTimeLevels) % numberOfTimeLevels;
    gf[prev].t=gf[current].t-dt;
    assignInitialConditions( prev );   // is this right to put this here ?

    if( debug & 4 )
    {
      getErrors( current, gf[current].t,dt,sPrintF("\n ********updateForNewTimeStep Errors current at t=%9.3e ******\n", gf[current].t));
      getErrors( prev, gf[prev].t,dt,sPrintF("\n ********updateForNewTimeStep Errors prev at t=%9.3e ******\n", gf[prev].t));
    }

    parameters.dbase.get<real>("dtOld")=dt;  // set dtOld 
  }
  
  return 0;
}


// ===================================================================================
///\brief Setup and initialization. Build the grid and solution fields. 
// ===================================================================================
int Cgsm::
setupGrids()
{
  real time0=getCPU();

  // *********************************************
  // *********** CompositeGrid *******************
  // *********************************************

  if ( cg[0].getGridType()==MappedGrid::unstructuredGrid )
  {
    UnstructuredMapping &umap = (UnstructuredMapping &) cg[0].mapping().getMapping();
    umap.expandGhostBoundary();
    verifyUnstructuredConnectivity(umap,true);
    //	umap.expandGhostBoundary();
    //	verifyUnstructuredConnectivity(umap,true);

    cg.destroy( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask |
		MappedGrid::THEcorner | MappedGrid::THEcellVolume | MappedGrid::THEcenterNormal |
		MappedGrid::THEfaceArea | MappedGrid::THEfaceNormal | 
		MappedGrid::THEcellVolume  | MappedGrid::THEcenterArea );	

    cg.update( MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEmask |
	       MappedGrid::THEcorner | MappedGrid::THEcellVolume | MappedGrid::THEcenterNormal |
	       MappedGrid::THEfaceArea | MappedGrid::THEfaceNormal | 
	       MappedGrid::THEcellVolume  | MappedGrid::THEcenterArea );	

  }
  else
  {
    // cg.update(MappedGrid::THEmask );
      
    // *wdh* 031202 cg.update(MappedGrid::THEcenter | MappedGrid::THEvertex );  
  }

    
  // Set the default order of accuracy from the grid parameters
  int minDiscretizationWidth=INT_MAX;
  int minInterpolationWidth=INT_MAX;
  Range R=cg.numberOfDimensions();
  const IntegerArray & iw = cg.interpolationWidth;
  // iw.display("iw");
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];
    const IntegerArray & dw = mg.discretizationWidth();
      
    // dw.display("dw");
      
    minDiscretizationWidth=min(minDiscretizationWidth,min(dw(R)));
      
    for( int grid2=0; grid2<cg.numberOfComponentGrids(); grid2++ )
    {
      if( grid!=grid2 )
	minInterpolationWidth=min( minInterpolationWidth,min(iw(R,grid,grid2)));
    }
  }
  if( minInterpolationWidth==INT_MAX ) minInterpolationWidth=minDiscretizationWidth;
  printF(" *** minDiscretizationWidth=%i, minInterpolationWidth=%i ****\n",minDiscretizationWidth,
	 minInterpolationWidth);

  const int maxOrderOfAccuracy=8;  // *************
    
  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
  

  orderOfAccuracyInSpace=min(maxOrderOfAccuracy,minDiscretizationWidth-1,minInterpolationWidth-1);
  if( orderOfAccuracyInSpace%2 ==1 )
    orderOfAccuracyInSpace--;   // must be even
    
  orderOfAccuracyInTime =orderOfAccuracyInSpace;
  orderOfArtificialDissipation=orderOfAccuracyInSpace;
    
  printF("***Setting orderOfAccuracyInSpace=%i, orderOfAccuracyInTime=%i, orderOfArtificialDissipation=%i\n",
	 orderOfAccuracyInSpace,orderOfAccuracyInTime,orderOfArtificialDissipation);

  if( orderOfAccuracyInSpace>4 )
  {
    printF("***Setting useConservative=false by default for order of accuracy >4.\n");
    useConservative=false;
  }
    

  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  timing(parameters.dbase.get<int>("timeForInitialize"))+=getCPU()-time0;

  if( cg.numberOfDimensions()==3 )
  {
  }
  else
  {
    kz=0; // *wdh* 040626 
  }
  
  // These next arrays hold (lambda,mu,c) in the case when they are constant on each grid but
  // may vary from grid to grid
  real & mu = parameters.dbase.get<real>("mu");
  real & lambda = parameters.dbase.get<real>("lambda");
  RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
  RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");

  const int numberOfComponentGrids = cg.numberOfComponentGrids();
  lambdaGrid.redim(numberOfComponentGrids); lambdaGrid=lambda;
  muGrid.redim(numberOfComponentGrids);  muGrid=mu;

  return 0;
}

//! Setup and initialization. Build the solution fields. 
int Cgsm::
setupGridFunctions()
// ===================================================================================
//    Build grid functions
// ===================================================================================
{
  real time0=getCPU();

  const int numberOfDimensions = cg.numberOfDimensions();
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");

  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");


  numberOfExtraFunctionsToUse=0;  // number of "fn" functions to use

  assert( current==0 );

  SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                   parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");
  aString buff;
  Range all;
  
  if( pdeModel==SmParameters::linearElasticity )
  {
      
    numberOfTimeLevels=2;  // keep this many levels of u

    if( pdeVariation==SmParameters::nonConservative ||
        pdeVariation==SmParameters::conservative )
    {
      if( usingPMLBoundaryConditions() ||
	  true )  // for combined dissipation with advance
	numberOfTimeLevels=3;  // needed for PML 
    }
    numberOfGridFunctionsToUse=numberOfTimeLevels;

    if( timeSteppingMethodSm==SmParameters::modifiedEquationTimeStepping )
    {
    }
    else if( timeSteppingMethodSm==SmParameters::forwardEuler ||
	     timeSteppingMethodSm==SmParameters::improvedEuler ||
	     timeSteppingMethodSm==SmParameters::adamsBashforth2 ||
	     timeSteppingMethodSm==SmParameters::adamsPredictorCorrector2 ||
	     timeSteppingMethodSm==SmParameters::adamsPredictorCorrector4 )
    {
      numberOfExtraFunctionsToUse=2;
    }
    else
    {
      printF("Cgsm::setupGridFunctions: unepected time-stepping method : timeSteppingMethodSm=%i\n",(int)timeSteppingMethodSm);
      Overture::abort("error");
    }
    
   

    for( int n=0; n<numberOfGridFunctionsToUse; n++ )
      gf[n].transform=NULL;

    for( int n=0; n<numberOfGridFunctionsToUse; n++ )
    {
      gf[n].cg.reference(cg);
      if( n>0 || gf[n].u.numberOfComponentGrids()==0 )  // gf[0] may already be assigned IC's
      {
	gf[n].u.updateToMatchGrid(cg,all,all,all,numberOfComponents);
	gf[n].u=0.;
      }

      aString *& componentName = parameters.dbase.get<aString* >("componentName");
      for( int c=0; c<numberOfComponents; c++ )
      {
	gf[n].u.setName(componentName[c],c);
      }
      
    }
	
    cgop = new CompositeGridOperators(cg);
    cgop->useConservativeApproximations(useConservative);
    cgop->setOrderOfAccuracy(orderOfAccuracyInSpace);
    for( int n=0; n<numberOfTimeLevels; n++ )
    {
      gf[n].u.setOperators(*cgop);
    }
    cgop->setTwilightZoneFlow(parameters.dbase.get<bool >("twilightZoneFlow"));
    cgop->setTwilightZoneFlowFunction(*parameters.dbase.get<OGFunction* >("exactSolution") );
  }
  else
  {
    printF("SolidMechanics:setup:ERROR: unknown pdeModel=%i\n",(int)pdeModel);
    throw "error";
  }

  for( int m=0; m<numberOfExtraFunctionsToUse; m++ )
  {
    fn[m].updateToMatchGridFunction(gf[0].u); fn[m]=0.;   // work space
  }


  dxMinMax.redim(cg.numberOfComponentGrids(),2);
  dxMinMax=0.;
  
  if( useVariableDissipation )
    buildVariableDissipation();

  if( initialConditionOption==annulusEigenfunctionInitialCondition )
  {
    // try to guess the cylinder radius and length

    if( cg[0].isRectangular() )
    {
      // assume grid 0 is the inner core 
      MappedGrid & mg = cg[0];
      real dx[3]={1.,1.,1.}, xab[2][3]={0.,0.,0.,0.,0.,0.};
      mg.getRectangularGridParameters( dx, xab );

      cylinderAxisStart=min(xab[0][2],xab[1][2]);
      cylinderAxisEnd  =max(xab[0][2],xab[1][2]);
 
      printF(" setupGridFunctions: I guess that the cylinder extends from [%8.2e,%8.2e] in the axial direction\n",
             cylinderAxisStart,cylinderAxisEnd    );
	
    }
      
  }

  if( pdeVariation==SmParameters::hemp )
  {
    // we need to keep (mass,density,energy) at t=0 
//       numberOfExtraFunctionsToUse=1;
//       fn[0].updateToMatchGrid(gf[0].cg,all,all,all,3); 
//       fn[0]=0.;  

    realCompositeGridFunction *& initialState = 
      parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction");
    if( initialState == NULL )
    {
      initialState = new realCompositeGridFunction(gf[0].cg,all,all,all,3);
      (*initialState)=1.;
    }
  }


  // For plotting displacement and contours (adjusted for displacement) specify which variables to use:
  GraphicsParameters & psp = parameters.dbase.get<GraphicsParameters>("psp");
  const int & u1c =  parameters.dbase.get<int >("u1c"); // hemp code uses this for the displacement 
  if( u1c<0 )
  {
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    psp.set(GI_DISPLACEMENT_U_COMPONENT,uc);
    psp.set(GI_DISPLACEMENT_V_COMPONENT,vc);
    psp.set(GI_DISPLACEMENT_W_COMPONENT,wc);
  }
  else
  {
    const int & u2c =  parameters.dbase.get<int >("u2c");
    const int & u3c =  parameters.dbase.get<int >("u3c");
    psp.set(GI_DISPLACEMENT_U_COMPONENT,u1c);
    psp.set(GI_DISPLACEMENT_V_COMPONENT,u2c);
    psp.set(GI_DISPLACEMENT_W_COMPONENT,u3c);
  }


  // Hemp: here is where we store the initial state (mass,density,energy)
  if(  parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction") == NULL && pdeVariation == SmParameters::hemp )
  {
    realCompositeGridFunction *& pInitialState = 
      parameters.dbase.get<realCompositeGridFunction*>("initialStateGridFunction");
    Range all;
    pInitialState = new realCompositeGridFunction(gf[0].cg,all,all,all,3);
    (*pInitialState)=1.;
  }

//  rc=numberOfComponents;  // position of density in TZ functions

//   if( useChargeDensity && pRho==NULL )
//   {
//     // allocate the grid function that holds the charge density
//     pRho=new realCompositeGridFunction(cg);
//   }

//  printF(">>>>setupGridFunctions: numberOfComponents=%i\n",numberOfComponents);

  numberOfSequences=numberOfComponents;
  if( computeEnergy )
    numberOfSequences+=2; // save the energy and delta(energy)

  // --- check for negative volumes : this is usually bad news --- *wdh* 2013/09/26
  const int numberOfGhost = orderOfAccuracyInSpace/2;
  int numberOfNegativeVolumes= GridStatistics::checkForNegativeVolumes( cg,numberOfGhost,stdout ); 
  if( numberOfNegativeVolumes>0 )
  {
    printF("Cgsm::FATAL Error: this grid has negative volumes (maybe only in ghost points).\n"
           "  This will normally cause severe or subtle errors. Please remake the grid.\n");
    OV_ABORT("ERROR");
  }
  else
  {
    printF("Cgsm:: No negative volumes were found\n.");
  }


  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  timing(parameters.dbase.get<int>("timeForInitialize"))+=getCPU()-time0;

  
  return 0;
}

