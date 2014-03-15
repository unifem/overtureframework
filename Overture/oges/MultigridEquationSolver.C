#include "MultigridEquationSolver.h"
#include "SparseRep.h"
#include "MultigridCompositeGrid.h"

MultigridEquationSolver::
MultigridEquationSolver(Oges & oges_)  : EquationSolver(oges_)
{
  name="multigrid";

  ogmg.setGridName(oges.gridName);
  ogmg.setSolverName(oges.solverName);
}

MultigridEquationSolver::
~MultigridEquationSolver()
{
}


real MultigridEquationSolver::
sizeOf( FILE *file /* =NULL */  )
// return number of bytes allocated 
{
  return ogmg.sizeOf(file);
}


int MultigridEquationSolver::
printStatistics(FILE *file /* = stdout */ ) const
//===================================================================================
// /Description:
//   Output any relevant statistics
//\end{>>EquationSolverInclude.tex}
//===================================================================================
{
  ogmg.printStatistics(file);
  return 0;
}

// New: *wdh* 100406
/// \brief Use this function in combination with setGrid(..)
int MultigridEquationSolver::
setCoefficientsAndBoundaryConditions( realCompositeGridFunction & coeff,
				      const IntegerArray & boundaryConditions,
				      const RealArray & bcData )
{
      
  ogmg.setCoefficientArray(coeff,boundaryConditions,bcData); 

  int compatibilityConstraint;
  oges.parameters.get(OgesParameters::THEcompatibilityConstraint,compatibilityConstraint);
  if( compatibilityConstraint )
    printf("MultigridEquationSolver::solve: singular problem\n");
  ogmg.parameters.setProblemIsSingular(compatibilityConstraint);
    
  oges.initialized=true;
  oges.shouldBeInitialized=false;

  return 0;
}

/// \brief Supply a new grid. This will build the multigrid levels 
int MultigridEquationSolver::
setGrid( CompositeGrid & cg )
{
  if( Oges::debug & 1 ) cout << " *** MultigridEquationSolver::setGrid... ****\n";
  
  // set defaults before we copy parameters
  ogmg.parameters.setResidualTolerance(1.e-4);
  ogmg.parameters.setErrorTolerance(1.e-6);

  if( oges.parameters.getOgmgParameters()!=NULL )
    ogmg.setOgmgParameters(*oges.parameters.getOgmgParameters()); // get a copy of the parameters from Oges.

  ogmg.parameters.updateToMatchGrid(cg);

  ogmg.updateToMatchGrid(oges.cg);  // this will build the extra levels.

  return 0;
}

/// \brief Set the MultigridCompositeGrid to use: (for use with Ogmg)
int MultigridEquationSolver::
set( MultigridCompositeGrid & mgcg )
{
  ogmg.set(mgcg);

  return 0;
}


// New: *wdh* 100404
int MultigridEquationSolver::
setCoefficientArray( realCompositeGridFunction & coeff,
		     const IntegerArray & boundaryConditions /* =Overture::nullIntArray() */,
		     const RealArray & bcData /* =Overture::nullRealArray() */ )
{

  return ogmg.setCoefficientArray( coeff,boundaryConditions,bcData );
  
}

int MultigridEquationSolver::
setEquationAndBoundaryConditions( OgesParameters::EquationEnum equation, 
                                  CompositeGridOperators & op,
                                  const IntegerArray & boundaryConditions,
				  const RealArray & bcData, 
				  RealArray & constantCoeff,
                                  realCompositeGridFunction *variableCoeff )
{
  if( !oges.initialized || oges.shouldBeInitialized )
  {
    if( Oges::debug & 1 ) 
      cout << " *** MultigridEquationSolver::setEquationAndBoundaryConditions: initialize... ****\n";
    ogmg.parameters.setResidualTolerance(1.e-4);
    ogmg.parameters.setErrorTolerance(1.e-6);

    if( oges.parameters.getOgmgParameters()!=NULL )
      ogmg.setOgmgParameters(*oges.parameters.getOgmgParameters()); // get a copy of the parameters from Oges.

    ogmg.parameters.updateToMatchGrid(oges.cg); // *wdh* 030831 : always update


    // *wdh* 100408 setGrid will do this: ogmg.updateToMatchGrid(oges.cg);  // this will build the extra levels.

    // CompositeGrid & mgcg = ogmg.getCompositeGrid();
    
    oges.initialized=true;
    oges.shouldBeInitialized=false;
  }
  
  return ogmg.setEquationAndBoundaryConditions(equation,op,boundaryConditions,bcData,constantCoeff,variableCoeff);
}


int MultigridEquationSolver::
solve(realCompositeGridFunction & u,
      realCompositeGridFunction & f)
{

  if( !oges.initialized || oges.shouldBeInitialized )
  {
    if( Oges::debug & 1 ) cout << " *** MultigridEquationSolver::solve: initialize... ****\n";
  
    // set defaults before we copy parameters
    ogmg.parameters.setResidualTolerance(1.e-4);
    ogmg.parameters.setErrorTolerance(1.e-6);

    if( oges.parameters.getOgmgParameters()!=NULL )
      ogmg.setOgmgParameters(*oges.parameters.getOgmgParameters()); // get a copy of the parameters from Oges.

    ogmg.parameters.updateToMatchGrid(oges.cg); // *wdh* 030831 : always update
      
    ogmg.updateToMatchGrid(oges.cg);
    ogmg.setCoefficientArray(oges.coeff); 

    int compatibilityConstraint;
    oges.parameters.get(OgesParameters::THEcompatibilityConstraint,compatibilityConstraint);
    if( compatibilityConstraint )
      printf("MultigridEquationSolver::solve: singular problem\n");
    ogmg.parameters.setProblemIsSingular(compatibilityConstraint);
    
    //  mgSolver.setSmootherType(Ogmg::redBlack);  // default
    // mgSolver.directSolver.setSolverType(Oges::yale);
    //     ogmg.parameters.setResidualTolerance(1.e-4);
    //     ogmg.parameters.setErrorTolerance(1.e-6);

    oges.initialized=TRUE;
    oges.shouldBeInitialized=FALSE;
  }
  
  int returnValue=ogmg.solve(u,f);
  oges.numberOfIterations=ogmg.getNumberOfIterations();
  maximumResidual=ogmg.getMaximumResidual();
  
  return returnValue;
}

