// ========================================================================================================
/// \class Cgins
/// \brief Solve the incompressible Navier-Stokes equations.
/// \details Cgins can be used to solve the incompressible Navier-Stokes with heat transfer.
// ========================================================================================================

#include "Cgins.h"
#include "Oges.h"
#include "Ogshow.h"
#include "ParallelUtility.h"
#include "viscoPlasticMacrosCpp.h"
#include "insFactors.h"

// CG equation-domain solver for the Incompressible Navier-Stokes Equations

// ===================================================================================================================
/// \brief Constructor for the Cgcns class.
///
/// \param cg_ (input) : use this CompositeGrid.
/// \param ps (input) : pointer to a graphics object to use.
/// \param show (input) : pointer to a show file to use.
/// \param plotOption_ (input) : plot option 
/// 
///  \note InsParameters (passed to the DomainSolver constructor above) replaces the base class Parameters
// ==================================================================================================================
Cgins::
Cgins(CompositeGrid & cg_, 
      GenericGraphicsInterface *ps /* =NULL */, 
      Ogshow *show /* =NULL */ , 
      const int & plotOption_ /* =1 */) 
    : DomainSolver(*(new InsParameters),cg_,ps,show,plotOption_)
{
  className="Cgins";
  name="ins";

  insSetup();
}



// ===================================================================================================================
/// \brief Destructor for the Cgins class.
// ==================================================================================================================
Cgins::
~Cgins()
{
  delete &parameters;
  
  for( int i=0; i<tzForcingVector.size(); i++)
    delete [] tzForcingVector[i];
}


//===============================================================================================
/// \brief Perform setup operations. Determine the default order of accuracy. 
//===============================================================================================
int Cgins::
insSetup()
{

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
  printF("Cgins::setup: minDiscretizationWidth=%i, minInterpolationWidth=%i.\n",minDiscretizationWidth,
	 minInterpolationWidth);

  const int maxOrderOfAccuracy=8;  // *************
    
  int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");
  

  orderOfAccuracyInSpace=min(maxOrderOfAccuracy,minDiscretizationWidth-1,minInterpolationWidth-1);
  if( orderOfAccuracyInSpace%2 ==1 )
    orderOfAccuracyInSpace--;   // must be even
 

  printP("Cgins::setup: INFO: setting default order of accuracy=%i based on the input grid parameters\n",
	 orderOfAccuracyInSpace);
  
  
  return 0;
}



//===============================================================================================
/// \brief Update the solver to match a new grid.
/// \param cg (input): composite grid.
//===============================================================================================
int Cgins::
updateToMatchGrid(CompositeGrid & cg)
{
  printF("\n $$$$$$$$$$$$$$$ Cgins: updateToMatchGrid(CompositeGrid & cg) $$$$$$$$$$$$\n\n");
  
  if( pp==NULL )
    pp=new realCompositeGridFunction;
  p().updateToMatchGrid(cg);
  // create the pressure equation
  updatePressureEquation(cg,gf[current]);

  return DomainSolver::updateToMatchGrid(cg);

}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateForMovingGrids}} 
int Cgins::
updateForMovingGrids(GridFunction & cgf)
//=========================================================================================
// /Description:
//    Update the CompositeGridSolver after grids have moved or have been adapted.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  real cpu0,timeForUpdatePressure=0.;

  if( parameters.dbase.has_key("multigridCompositeGrid") )
  {
    // For multigrid we share the multigrid hierarchy amongst the pressure and implicit solvers.
    // Here we mark that the multigrid hierarchy is out of date: 
    parameters.dbase.get<MultigridCompositeGrid>("multigridCompositeGrid").setGridIsUpToDate(false);
   
  }
  
  if( movingGridProblem() )
  {
    cpu0=getCPU();
    cgf.u.updateToMatchGrid( cgf.cg );    
    updateGeometryArrays( cgf );
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForMovingUpdate"))+=getCPU()-cpu0;

    if( false )  // do this below, once for moving and AMR *wdh* 100408
    {
      cpu0=getCPU();
      // update the pressure equation
      updatePressureEquation(cgf.cg, cgf);
      timeForUpdatePressure=getCPU()-cpu0;
    }
    
  }

  if( movingGridProblem() || parameters.isAdaptiveGridProblem() )
  {
    real cpu0=getCPU();
    // update the pressure equation
    updatePressureEquation(cgf.cg, cgf);
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdatePressureEquation"))+=getCPU()-cpu0;
    timeForUpdatePressure=getCPU()-cpu0;
  }
  if( parameters.isMovingGridProblem()  && parameters.isAdaptiveGridProblem() )
  {
    // update cgf.gridVelocity arrays
    cgf.updateGridVelocityArrays();
    // don't do this here if we have just moved the grids and not adapted!
    // *no* cgf.gridVelocityTime=cgf.t -1.e10;  // this will force a recomputation of the grid velocity the next time...
  }
  

  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdatePressureEquation"))+=timeForUpdatePressure;
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForTimeIndependentVariables"))+=timeForUpdatePressure;
  return 0;
}


int Cgins::
updateForNewTimeStep(GridFunction & cgf, const real & dt)
//=========================================================================================
// /Description:
//    Update geometry arrays after the time step has changed.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  const int geometryHasChanged=false;
  updateDivergenceDamping( cgf.cg,geometryHasChanged ); 
  return 0;
}

int Cgins::
updateGeometryArrays(GridFunction & cgf)
{
  if( debug() & 4 ) printF(" --- Cgins::updateGeometryArrays ---\n");

  real cpu0=getCPU();

  // These next are used in adaptGrids: 
  if( realPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    realPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  if( imaginaryPartOfEigenvalue.size() != cg.numberOfComponentGrids() )
    imaginaryPartOfEigenvalue.resize(cg.numberOfComponentGrids(),-1.);
  

  for( int grid=0; grid<cgf.cg.numberOfComponentGrids(); grid++ )
  {
    if( !cgf.cg[grid].isRectangular() || twilightZoneFlow() || parameters.isAxisymmetric() ||
	parameters.gridIsMoving(grid) )
      cgf.cg[grid].update(MappedGrid::THEcenter | MappedGrid::THEvertex );  
  }
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdatePressureEquation"))+=getCPU()-cpu0;

  return DomainSolver::updateGeometryArrays(cgf);
}

void Cgins::
updateTimeIndependentVariables( CompositeGrid & cg0, GridFunction & cgf )
// ==========================================================================================
// This function is called after grids have been added or removed
// ==========================================================================================
{
  updatePressureEquation(cg0,cgf);
}

// ======================================================================================================
/// \brief:  Set the titles and labels that go on the show file output 
// ======================================================================================================
void Cgins::
saveShowFileComments( Ogshow &show )
{
  char buffer[200]; 

  // save comments that go at the top of each plot

  aString timeLine="";
  if(  parameters.dbase.has_key("timeLine") )
    timeLine=parameters.dbase.get<aString>("timeLine");

  aString showFileTitle[5];
  if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::BoussinesqModel )
    showFileTitle[0]=sPrintF(buffer,"Incompressible NS (Boussinesq), nu=%8.2e, k=%8.2e",
			     parameters.dbase.get<real >("nu"),parameters.dbase.get<real >("kThermal"));
  else if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::viscoPlasticModel )
  {
    // declare and lookup visco-plastic parameters (macro)
    declareViscoPlasticParameters;
    showFileTitle[0]=sPrintF(buffer,"INS-VP (eta,yield,exp)=(%.4g,%.4g,%.4g), k=%8.2e",
			     etaViscoPlastic,yieldStressViscoPlastic,exponentViscoPlastic,
                             parameters.dbase.get<real >("kThermal"));
  }
  else 
    showFileTitle[0]=sPrintF(buffer,"Incompressible NS, nu=%8.2e",parameters.dbase.get<real >("nu"));

  if( parameters.isAxisymmetric() )
    showFileTitle[1]=sPrintF(buffer,"axisymmetric, %s",(const char*)timeLine);
  else
    showFileTitle[1]=timeLine;

  if( parameters.dbase.get<bool >("useSecondOrderArtificialDiffusion") )
    showFileTitle[1]+=sPrintF(buffer,", ad2=(%3.1f,%3.1f)",parameters.dbase.get<real >("ad21"),parameters.dbase.get<real >("ad22"));
  if( parameters.dbase.get<bool >("useFourthOrderArtificialDiffusion") )
    showFileTitle[1]+=sPrintF(buffer,", ad4=(%3.1f,%3.1f)",parameters.dbase.get<real >("ad41"),parameters.dbase.get<real >("ad42"));

  showFileTitle[2]="";  // marks end of titles

  for( int i=0; showFileTitle[i]!=""; i++ )
    show.saveComment(i,showFileTitle[i]);

}  

// ===================================================================================================================
/// \brief Output run-time parameters for the header.
/// \param file (input) : write values to this file.
///
// ===================================================================================================================
void
Cgins::
writeParameterSummary( FILE * file )
{
  DomainSolver::writeParameterSummary( file );

  const InsParameters::PDEModel & pdeModel = parameters.dbase.get<InsParameters::PDEModel>("pdeModel");

  if ( file==parameters.dbase.get<FILE* >("checkFile") )
  {
    fPrintF(parameters.dbase.get<FILE* >("checkFile"),"\\caption{Incompressible Navier Stokes, gridName, $\\nu=%3.2f$, $t=%2.1f$, ",
	    parameters.dbase.get<real >("nu"),parameters.dbase.get<real >("tFinal"));

    return;
    
  }

  Parameters::TimeSteppingMethod &timeSteppingMethod = 
             parameters.dbase.get<Parameters::TimeSteppingMethod>("timeSteppingMethod");

  fPrintF(file,"\n");
  fPrintF(file," nu=%e, cdv=%g, cDt=%g, dtMax=%g\n",parameters.dbase.get<real >("nu"),
	  parameters.dbase.get<real >("cdv"),parameters.dbase.get<real >("cDt"),
          parameters.dbase.get<real >("dtMax"));
      
  if( pdeModel==InsParameters::BoussinesqModel || 
      pdeModel==InsParameters::twoPhaseFlowModel || 
      pdeModel==InsParameters::viscoPlasticModel )
  {

    if( parameters.dbase.get<int>("variableMaterialPropertiesOption")==0 )
    {
      fPrintF(file," Boussinesq: kThermal=%g, thermalConductivity=%g (constant material properties)\n",
	      parameters.dbase.get<real >("kThermal"),
	      parameters.dbase.get<real >("thermalConductivity"));
    }
    else
    {
      fPrintF(file," Boussinesq: material properties rho, Cp and thermalConductivity are variable.\n");
    }


    ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
    if( gravity[0]!=0. || gravity[1]!=0. || gravity[2]!=0. )
    {
      real thermalExpansivity=1.;
      parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("thermalExpansivity",thermalExpansivity);
      fPrintF(file," Coefficient of thermal expansivity =%8.2e \n",thermalExpansivity);
    }

    if( parameters.dbase.get<real >("kThermal") <0. )
    {
      printF("Cgins::FATAL ERROR: kThermal<0 : kThermal=%12.4e\n",parameters.dbase.get<real >("kThermal"));
      OV_ABORT("ERROR");
    }
    

  }
  else if( pdeModel==InsParameters::twoPhaseFlowModel )
  {
    fPrintF(file," twoPhaseFlowModel: kThermal=%g\n",parameters.dbase.get<real >("kThermal"));
  }


  if( true ||  // gravity is used with rigid bodies for all models
      pdeModel==InsParameters::BoussinesqModel || 
      pdeModel==InsParameters::twoPhaseFlowModel || 
      pdeModel==InsParameters::viscoPlasticModel )
  {
    ArraySimpleFixed<real,3,1,1,1> & gravity = parameters.dbase.get<ArraySimpleFixed<real,3,1,1,1> >("gravity");
    if( gravity[0]!=0. || gravity[1]!=0. || gravity[2]!=0. )
    {
      fPrintF(file," gravity is on, acceleration due to gravity = (%8.2e,%8.2e,%8.2e) \n",
                     gravity[0],gravity[1],gravity[2]);
    }
    else
    {
      fPrintF(file," gravity is off.\n");
    }
    
  }
  real surfaceTension=-1.;
  parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("surfaceTension",surfaceTension);
  if( surfaceTension>=0. )
  {
    real pAtmosphere=0.;  // atmosphere pressure for free surface
    parameters.dbase.get<ListOfShowFileParameters >("pdeParameters").getParameter("pAtmosphere",pAtmosphere);
    fPrintF(file," surfaceTension = %9.3e. pAtmosphere=%9.3e.\n",surfaceTension,pAtmosphere);
  }
  

  if( parameters.dbase.get<bool >("projectInitialConditions") )
    fPrintF(file," Project the initial conditions.\n");
  else
    fPrintF(file," Do NOT project the initial conditions.\n");

  if( timeSteppingMethod==Parameters::implicit )
  {
    const InsParameters::ImplicitVariation & implicitVariation = 
             parameters.dbase.get<InsParameters::ImplicitVariation>("implicitVariation");
    const Parameters::ImplicitMethod & implicitMethod = 
             parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    const InsParameters::DiscretizationOptions & discretizationOption =
                   parameters.dbase.get<InsParameters::DiscretizationOptions>("discretizationOption") = InsParameters::compactDifference;

    fPrintF(file,"\n");
    fPrintF(file,"INS implicit time stepping: \n");
    if( implicitMethod==Parameters::approximateFactorization )
    {
      // -- output options for the approximate factorization scheme. --      
      fPrintF(file," method = approximate factorization scheme,");
      if( discretizationOption==InsParameters::standardFiniteDifference )
	fPrintF(file," standard finite difference.\n");
      else if( discretizationOption==InsParameters::compactDifference )
        fPrintF(file," compact finite difference.\n");

      fPrintF(file,"  AFS: max correction relative tol=%8.2e\n",parameters.dbase.get<real>("AFcorrectionRelTol"));
      fPrintF(file,"  AFS: max number of corrections=%i\n",parameters.dbase.get<int>("numberOfAFcorrections"));
      

      fPrintF(file,"  useBoundaryDissipationInAFScheme=%s\n", (parameters.dbase.get<bool >("useBoundaryDissipationInAFScheme") ? "true" : "false"));
    }
    else
    {
      // -- "old" semi-implicit method
      fPrintF(file," implicit variation = %s\n",(implicitVariation==InsParameters::implicitViscous ? "implicitViscous" :
						 implicitVariation==InsParameters::implicitAdvectionAndViscous ? "implicitAdvectionAndViscous" :
						 implicitVariation==InsParameters::implicitFullLinearized ? "implicitFullLinearized" : "unknown"));
      fPrintF(file," useNewImplicitMethod=%i (1=use implicit RHS evaluation for dudt)\n",
	      parameters.dbase.get<int>("useNewImplicitMethod"));
    
      fPrintF(file," refactor frequency=%i\n",parameters.dbase.get<int>("refactorFrequency"));

      int & numberOfImplicitVelocitySolvers = parameters.dbase.get<int>("numberOfImplicitVelocitySolvers");
      int & implicitSolverForTemperature = parameters.dbase.get<int>("implicitSolverForTemperature");
      bool & useFullSystemForImplicitTimeStepping = parameters.dbase.get<bool >("useFullSystemForImplicitTimeStepping");
      int & scalarSystemForImplicitTimeStepping = parameters.dbase.get<int >("scalarSystemForImplicitTimeStepping");
      fPrintF(file," number of implicit solvers for velocity=%i. \n"
	      " useFullSystemForImplicitTimeStepping=%i. scalarSystemForImplicitTimeStepping=%i.\n", numberOfImplicitVelocitySolvers,
	      (int)useFullSystemForImplicitTimeStepping,scalarSystemForImplicitTimeStepping);
      fPrintF(file,"\n");
    }
    
  }
  
  
  const int outflowOption=parameters.dbase.get<int>("outflowOption");
  const int checkForInflowAtOutFlow = parameters.dbase.get<int >("checkForInflowAtOutFlow");
  // NOTE: checkForInflowAtOutFlow=2: means expect inflow at outflow
  // fPrintF(file," outflowOption=%i (0=extrap,1=neumann), checkForInflowAtOutFlow=%i (1=check, 2=expect)\n",outflowOption,checkForInflowAtOutFlow);
  
  if( outflowOption==0 ) // && checkForInflowAtOutFlow!=2 )
  {
    assert( checkForInflowAtOutFlow!=2 );
    fPrintF(file," Outflow boundary condition is extrapolation (order of extrap.=%i). (Other options: Neumann)\n",
	    parameters.dbase.get<int >("orderOfExtrapolationForOutflow"));
  }
  else
    fPrintF(file," Outflow boundary condition is Neumann. (Other options: extrapolation)\n");

  fPrintF(file," checkForInflowAtOutFlow=%i. 0=use default outflow, 1=check locally for inflow, "
                "2=expect inflow everywhere.\n",checkForInflowAtOutFlow);

  if( parameters.dbase.get<int>("orderOfAccuracy") ==4 )
  {
    if( parameters.dbase.get<bool >("stabilizeHighOrderBoundaryConditions") )
      fPrintF(file," Stabilize fourth order boundary conditions with second order dissipation.\n");
    else
      fPrintF(file," Do NOT stabilize fourth order boundary conditions with second order dissipation.\n");
    
  }

}


// ===================================================================================================================
/// \brief Create the factors for the approximate factorization time stepping method.  
// ===================================================================================================================
void 
Cgins::initializeFactorization()
{

  DomainSolver::initializeFactorization();
  parameters.dbase.get< std::list<int> >("AFLimiterBoundariesToSkip").push_back(InsParameters::outflow);
  parameters.dbase.get<std::list<int> >("AFComponentsToSkip").push_back(parameters.dbase.get<int>("pc"));

  int ndim = gf[current].cg.numberOfDimensions();

  parameters.dbase.get<int>("AFparallelGhostWidth") = 2;//(parameters.dbase.get<int>("orderOfAccuracy")==2 ? 1 : 2); 

#ifdef USE_COMBINED_FACTORS


  //    for ( int d=ndim-1; d>=0; d-- )
  for ( int d=0; d<ndim; d++ )
    factors.push_back(new CGINS_ApproximateFactorization::INS_Factor(d,CGINS_ApproximateFactorization::Merged_Factor,(InsParameters&)parameters));

#else
  for (int type=CGINS_ApproximateFactorization::R_Factor;
       type<CGINS_ApproximateFactorization::Diagonal_Factor;
       type++)
    {
      for ( int d=0; d<ndim; d++ )
	factors.push_back(new CGINS_ApproximateFactorization::INS_Factor(d,CGINS_ApproximateFactorization::FactorTypes(type),(InsParameters&)parameters));
    }
#endif
  factors.push_back(new CGINS_ApproximateFactorization::INS_Factor(0,CGINS_ApproximateFactorization::Diagonal_Factor,(InsParameters&)parameters));
}
