#include "Cgins.h"
#include "CompositeGridOperators.h"
#include "Ogshow.h"
#include "Ogen.h"
#include "Ogmg.h"
#include "Insbc4WorkSpace.h"
#include "App.h"
#include "GridStatistics.h"


//=========================================================================================
/// \brief Allocate and initialize grid functions, based on the time-stepping method.
/// \details This function is called by setup 
//=========================================================================================
int Cgins::
setupGridFunctions()
{
  assert( current==0 );
  GridFunction & solution = gf[current];
  CompositeGrid & cg = *solution.u.getCompositeGrid();

  Range all;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    // *wdh* 2011/08/21 - no need to build the center for curvilinear gor moving grids 
    // if( !cg[grid].isRectangular() || twilightZoneFlow() || parameters.isAxisymmetric() ||
    // parameters.gridIsMoving(grid) )
    if( twilightZoneFlow() || parameters.isAxisymmetric() )
    {
      cg[grid].update(MappedGrid::THEcenter | MappedGrid::THEvertex | MappedGrid::THEvertexBoundaryNormal);  
    }
    else
    {
      cg[grid].update(MappedGrid::THEvertexBoundaryNormal);  
    }
    
  }
    

  poisson = new Oges;

  poisson->setSolverName("CginsPressureSolver");  // name used in debug files etc.

  // For multigrid we wish to share the multigrid hierarchy so we create an object here and give it to Oges
  if( !parameters.dbase.has_key("multigridCompositeGrid") ) parameters.dbase.put<MultigridCompositeGrid>("multigridCompositeGrid");
  MultigridCompositeGrid & mgcg = parameters.dbase.get<MultigridCompositeGrid>("multigridCompositeGrid");
  poisson->set(mgcg);


  pressureRightHandSide.updateToMatchGrid(cg,all,all,all); pressureRightHandSide=0.;
  
  if( (parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::implicit || parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateNewton) && parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod")!=Parameters::approximateFactorization )
  {
    buildImplicitSolvers(cg); // This will build the array of implicit solvers
  }
  
  if( parameters.dbase.get<int >("orderOfAccuracy")>=4 ) // kkc 101116 changed == to >= to test 6th order compact operators
  {
    if( parameters.dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer")==NULL )
      parameters.dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer") = new Insbc4WorkSpace;
    parameters.dbase.get<Insbc4WorkSpace* >("bc4workSpacePointer")->init(cg.numberOfComponentGrids());
  }

  if( parameters.dbase.get<InsParameters::PDEModel >("pdeModel")==InsParameters::viscoPlasticModel )
  {
    // use Neumann BC's at outflow for viscoPlasticSolver.
    parameters.dbase.get<int>("outflowOption")=1;
  }
  
  // --- check for negative volumes : this is usually bad news --- *wdh* 2013/09/26
  const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
  const int numberOfGhost = orderOfAccuracyInSpace/2;
  int numberOfNegativeVolumes= GridStatistics::checkForNegativeVolumes( cg,numberOfGhost,stdout ); 
  if( numberOfNegativeVolumes>0 )
  {
    printF("Cgins::FATAL Error: this grid has negative volumes (maybe only in ghost points).\n"
           "  This will normally cause severe or subtle errors. Please remake the grid.\n");
    OV_ABORT("ERROR");
  }
  else
  {
    printF("Cgins:: No negative volumes were found\n.");
  }


  return DomainSolver::setupGridFunctions();
}

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{initializeSolution}} 
int Cgins::
initializeSolution()
//=========================================================================================
// /Description:
//    Initialize the solution, project velocity if required.
//  The initial conditions should have already been assigned in setParameters.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{

  DomainSolver::initializeSolution();

  const bool & twilightZoneFlow = parameters.dbase.get<bool >("twilightZoneFlow");
  if( !twilightZoneFlow )
  {
    // -- For moving grid problems we iterate on the initial conditions since the 
    //    body forces depend on the pressure and the pressure depends on the forces.

    // useMovingGridSubIterations : use multiple sub-iterations per time-step for moving grid problems with light bodies
    const bool & useMovingGridSubIterations = parameters.dbase.get<bool>("useMovingGridSubIterations");

    int numberOfCorrections=1;
    if( movingGridProblem() && useMovingGridSubIterations  )
      numberOfCorrections= parameters.dbase.get<int>("numberOfPCcorrections"); 

    printF("--INS--::initializeSolution:useMovingGridSubIterations=%i numberOfCorrections=%i\n",(int)useMovingGridSubIterations,
	   numberOfCorrections);

    for( int correction=0; correction<numberOfCorrections; correction++ )
    {
      // define initial forces on moving bodies -- we really should iterate here since the 
      // forces depend on the pressure and the pressure depends on the forces.
      if( movingGridProblem() && gf[current].t==0. )
	correctMovingGrids( gf[current].t, gf[current].t,gf[current],gf[current] ); 
      
      // -- compute any body forcing since the pressure may depend on this ---
      const real tForce = gf[current].t; // evaluate the body force at this time
      computeBodyForcing( gf[current], tForce );

      if( !parameters.dbase.get<bool >("projectInitialConditions") ) // TEMP fix for Joel's bug
	updateDivergenceDamping( gf[current].cg,true );
    
      // Evaluate the initial pressure field:
      printF("--INS--::initializeSolution:Solve for the initial pressure field, dt=%9.3e (correction=%i) \n",
	     parameters.dbase.get<real >("dt"),correction);
      solveForTimeIndependentVariables( gf[current] );     

      if( movingGridProblem() && useMovingGridSubIterations )
      {
	if( true || debug() & 2 )
	{
	  bool isConverged = getMovingGridCorrectionHasConverged();
	  real delta = getMovingGridMaximumRelativeCorrection();
	  printF("--INS--: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
		 delta,correction,(int)isConverged);
	}
      }
      

      bool isConverged = getMovingGridCorrectionHasConverged();
      if( isConverged )
	break;
    }
    
  }
  
  dt= getTimeStep( gf[current] ); 
  parameters.dbase.get<real >("dt")=dt;

  return 0;
}

