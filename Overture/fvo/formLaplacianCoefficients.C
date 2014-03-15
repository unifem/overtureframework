#include "Projection.h"
// ===============================================================================
void Projection::
formLaplacianCoefficients ()
{
  
  cout << "Projection::formLaplacianCoefficients ... " << endl;
  
    // =============================
    // Form Laplacian coefficients
    // =============================

  bool isVolumeScaled;
  isVolumeScaled = opPointer->getIsVolumeScaled();
  assert (!isVolumeScaled);

  coefficients = 0.;
  
  if (isVariableDensityProjection)
  {
    coefficients = opPointer->divInverseScalarGradCoefficients (density);
  }
  else
  {
    coefficients = opPointer->laplacianCoefficients();
  }

  if (projectionDebug) projectionDisplay.display (coefficients, "Projection Laplacian coefficients before BCs:");    

  applyLaplacianBoundaryConditions ();
  
  if (projectionDebug) projectionDisplay.display (coefficients, "Projection Laplacian coefficients:");    

//...User controlled Oges options; (defaults set in initializeNonGridItems)
  if (iterativeImprovementReset)      ellipticSolver.setIterativeImprovement (iterativeImprovementValue);
  if (preconditionBoundaryReset)      ellipticSolver.setPreconditionBoundary (preconditionBoundaryValue);
  if (conjugateGradientPreconditionerReset) 
    ellipticSolver.setConjugateGradientPreconditioner (conjugateGradientPreconditionerValue);
  if (conjugateGradientTypeReset)     ellipticSolver.setConjugateGradientType (conjugateGradientTypeValue);
  if (conjugateGradientNumberOfIterationsReset) 
    ellipticSolver.setConjugateGradientNumberOfIterations (conjugateGradientNumberOfIterationsValue);
  if (conjugateGradientNumberOfSaveVectorsReset)
    ellipticSolver.setConjugateGradientNumberOfSaveVectors (conjugateGradientNumberOfSaveVectorsValue);
  if (sorNumberOfIterationsReset)     ellipticSolver.setSorNumberOfIterations (sorNumberOfIterationsValue);
  if (fillinRatioReset)               ellipticSolver.setFillinRatio (fillinRatioValue);
  if (zeroRatioReset)                 ellipticSolver.setZeroRatio (zeroRatioValue);
  if (fillinRatio2Reset)              ellipticSolver.setFillinRatio2 (fillinRatio2Value);
  if (harwellToleranceReset)          ellipticSolver.setHarwellTolerance (harwellToleranceValue);
  if (matrixCutoffReset)              ellipticSolver.setMatrixCutoff (matrixCutoffValue);
  if (sorOmegaReset)                  ellipticSolver.setSorOmega (sorOmegaValue);
  if (solverTypeReset)                ellipticSolver.setSolverType (solverTypeValue);

//...need to call initialize to recompute the right null vector

//...970925 this shouldn't be called here.
//  ellipticSolver.initialize();

//...coefficients have changed so we have to refactor the matrix

  ellipticSolver.setRefactor (TRUE);

  updateEllipticSolverToMatchGrid (compositeGrid);

}

// ===============================================================================
void Projection::
applyLaplacianBoundaryConditions ()
//
// /Purpose:
//   This routine sets the LHS of boundary conditions for the elliptic
//   solution by modifying the coefficients of the coefficient array.
//   It does not set up the forcing for the boundary conditons since
//   this may be time dependent while the elliptic operator might not be.
//   use applyRightHandSideBoundaryConditions() to set the forcing.
//
// ===============================================================================
{

// ========================================
// declarations:
// If exact velocity used, allocate space in boundaryConditionRightHandSide
// ========================================
  
  REAL ZERO = 0.0;


  Index I1ghost, I2ghost, I3ghost, I1boundary, I2boundary, I3boundary;

  REALCompositeGridFunction boundaryConditionRightHandSide;
  if (useExactVelocity)
  {
    boundaryConditionRightHandSide.updateToMatchGrid (compositeGrid,  cellCentered);
    boundaryConditionRightHandSide = ZERO;
  }
  

// ========================================
// First set the BC types: loop over all boundaries, translate the CMPGRD boundaryCondition code
// to a boundary condition for the projection, and then call the appropriate setBoundaryCondition... routines
// if an exact velocity has been specified, then force the BCs with the exact phi
// ========================================

//  setEllipticCompatibilityConstraint (TRUE); // this will be reset to FALSE if a dirichlet condition is specified
  ellipticCompatibilityConstraintSet = TRUE;
  Index Component, Equation;
  Component = Range(0,0);
  Equation  = Range(0,0);

  int cmpgrdBC;
  
  for (cmpgrdBC=1; cmpgrdBC<=maximumNumberOfOvertureBCs; cmpgrdBC++)
  {
    
    switch (listOfBCs[cmpgrdBC])
    {
    case cheapNeumann:
    case homogeneousNeumann:
    case fixedWallNeumann:
    case movingWallNeumann:
      opPointer->applyBoundaryConditionCoefficients (coefficients, Equation, Component, BCTypes::neumann, cmpgrdBC);
      break;
      
    case normalDerivativeGiven:
      opPointer->applyBoundaryConditionCoefficients (coefficients, Equation, Component, BCTypes::neumann, cmpgrdBC);
      break;
      
    case valueGiven:
      opPointer->applyBoundaryConditionCoefficients (coefficients, Equation, Component, BCTypes::dirichlet, cmpgrdBC);
      break;
      
    case normalValueGiven: //not a reasonable choice
      cout << "Projection::operators.applyLaplacianBoundaryConditions: normalValueGiven is not a reasonable choice for an elliptic BC" << endl;
      assert (listOfBCs[cmpgrdBC] != normalValueGiven);
      break;   

    case extrapolateAllComponents:
      opPointer->applyBoundaryConditionCoefficients (coefficients, Equation, Component, BCTypes::extrapolate, cmpgrdBC);
      break;
      
    default:
      break;

    }
  }

  if (projectionDebug)
    projectionDisplay.display (coefficients, "Projection Laplacian coefficients after BCs but before finishBoundaryConditions:");    

  opPointer->finishBoundaryConditions (coefficients);
  

}
