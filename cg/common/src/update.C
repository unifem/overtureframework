#include "DomainSolver.h"
#include "CompositeGridOperators.h"

//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateToMatchGrid}} 
int DomainSolver::
updateToMatchGrid(CompositeGrid & cg)
//=========================================================================================
// /Description:
//    Update the CompositeGridSolver for a new CompositeGrid.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  // should allocate space for ux,uy,... once they are in a work space class
  // printF("\n $$$$$$$$$$$$$$$ DomainSolver: updateToMatchGrid(CompositeGrid & cg) $$$$$$$$$$$$\n\n");
  
  if( parameters.dbase.get<Parameters::TimeSteppingMethod >("timeSteppingMethod")==Parameters::steadyStateRungeKutta )
  {
    // dtVar : dt at each grid point for local time stepping.
    pdtVar = new realCompositeGridFunction(cg);
    *pdtVar=0.;
  }

  return 0;
}


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{updateForAdaptiveGrids}} 
int DomainSolver::
updateForAdaptiveGrids(CompositeGrid & cg)
//=========================================================================================
// /Description:
//    Update the CompositeGridSolver after an AMR grid has been generated. Currently this
// is only called after the initial conditions have been adapted.
//
//\end{CompositeGridSolverInclude.tex}  
//=========================================================================================
{
  if( parameters.dbase.get<bool >("adaptiveGridProblem") )
  {
    int i;
    if( parameters.isMovingGridProblem() )
    {
      gf[current].updateToMatchGrid(cg);
      gf[current].u.updateToMatchGrid(cg);  // *wdh* 040829
      for( i=0; i<numberOfGridFunctionsToUse; i++ )
      {
        if( i!=current )
  	  gf[i].cg=cg;   // make a copy of cg for moving grid problems
      }
    }
    else
    {
      for( i=0; i<numberOfGridFunctionsToUse; i++ )
      {
	gf[i].updateToMatchGrid(cg);
      }
    }
    for( i=0; i<numberOfGridFunctionsToUse; i++ )
    {
      gf[i].u.updateToMatchGrid(gf[i].cg);
    }
    
    for( i=0; i<numberOfExtraFunctionsToUse; i++ )
    {
      fn[i].updateToMatchGrid(cg);
      fn[i]=0.;  // *wdh* 040316 - to avoid UMR's
    }

    //  update the body forcing *wdh* 2013/08/30
    if( parameters.dbase.get<bool >("turnOnBodyForcing") )
    {
      realCompositeGridFunction *&bodyForce = parameters.dbase.get<realCompositeGridFunction* >("bodyForce");
      (*bodyForce).updateToMatchGrid(cg);
    }

  }
  
  return 0;
  
}

//==============================================================================================
/// \brief Update the DomainSolver after grids have moved or have been adapted.
//==============================================================================================
int DomainSolver::
updateForMovingGrids(GridFunction & cgf)
{
  real cpu0,timeFourUpdatePressure=0.;
  if( movingGridProblem() )
  {
    cpu0=getCPU();
    cgf.u.updateToMatchGrid( cgf.cg );    
    updateGeometryArrays( cgf );
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForMovingUpdate"))+=getCPU()-cpu0;

  }

  if( parameters.isAdaptiveGridProblem() )
  {
  }
  if( parameters.isMovingGridProblem()  && parameters.isAdaptiveGridProblem() )
  {
    // update cgf.gridVelocity arrays
    cgf.updateGridVelocityArrays();
    // don't do this here if we have just moved the grids and not adapted!
    // *no* cgf.gridVelocityTime=cgf.t -1.e10;  // this will force a recomputation of the grid velocity the next time...
  }
  
  return 0;
}


//==============================================================================================
/// \brief Update the geometry arrays.
//==============================================================================================
int DomainSolver::
updateForNewTimeStep(GridFunction & cgf, const real & dt)
{

  return 0;
}


//==============================================================================================
/// \brief Update geometry arrays, solution at old times etc. after the time step has changed.
//==============================================================================================
int DomainSolver::
updateGeometryArrays(GridFunction & cgf)
{

  return 0;
}



void DomainSolver::
updateTimeIndependentVariables(CompositeGrid & cg0, GridFunction & cgf )
// ==========================================================================================
// This function is called after grids have been added or removed
// ==========================================================================================
{
}
