#include "Projection.h"
#include "OgesEnums.h"

const Index Projection::nullIndex;	//this creates an Index with nothing in it
const Range Projection::all;		//also nullIndex; used to default an index to "all"
bool Projection::projectionDebug = LogicalFalse; 
real Projection::undefinedRealValue = -.9e-9;


//=================================================================================
//\begin{>projectionDoc.tex}{\subsection{Projection default constructor}}   
Projection::
Projection()
//
// /Purpose: This is the default constructor for the Projection class.
// /Author: D. L. Brown
// /Date documentation last modified: 951023
//\end{projectionDoc.tex} 
//=================================================================================
{
  initializeNonGridItems();
}

//=================================================================================
//\begin{>projectionDoc.tex}{\subsection{Projection  constructor}}   
Projection::
Projection(CompositeGridFiniteVolumeOperators *operators_)
//
// /Purpose: This constructor is used to set operators, but not the grid
// /Author: D. L. Brown
// /Date documentation last modified: 951023
//\end{projectionDoc.tex} 
//=================================================================================
{
  initializeNonGridItems();
  setOperators (operators_);
}
//=================================================================================
Projection::
Projection(CompositeGrid & cg)
//
// /Purpose:
//    This constructor makes a projection object given a CompositeGrid. 
//    Calling this constructor is equivalent to calling the default constructor
//    followed by a call to the public class member function {\tt updateToMatchGrid}.
//
// /cg: a valid CompositeGrid
// 
// /Author: D. L. Brown
// /Date documentation last modified: 951023
// /981111: obsolete
//=================================================================================
{
  cout << "Projection(CompositeGrid&) is an obsolete constructor. Use " << endl;
  cout << "Projection(CompositeGrid&, CompositeGridFiniteVolumeOperators*, DynamicMovingGrids*) instead" << endl;
  exit (-1);
  
//  initializeNonGridItems();
//  updateToMatchGrid(cg);
}

//==============================================================================
//\begin{>>projectionDoc.tex}{\subsection{Projection constructor from a CompositeGrid and MovingGrids object}}
Projection::
Projection (CompositeGrid & cg, 
	    CompositeGridFiniteVolumeOperators * operators_,
	    MovingGrids * movingGridsPointer_ // =NULL
  )
// /Purpose:
//    This is the main constructor for the Projection class.
//    It can be used when there are moving boundaries as 
//    described by a MovingGrids object, in which case a pointer to
//    the MovingGrids object is passed here. The forcing function for
//    the Neumann condition on the pressure (actually on phi) depends
//    on the boundary velocity, which will be computed from the MovingGrids
//    object.
//
//   /cg: a valid Composite Grid
//   /operators\_: CompositeGridFiniteVolumeOperators to use.
//   /movingGridsPointer\_: (optional) pointer to the MovingGrids object
//
//\end{projectionDoc.tex}
//==============================================================================
{
  initializeNonGridItems();
  setOperators (operators_);
  updateToMatchGrid (cg);
  if (movingGridsPointer_) setMovingGridsPointer (movingGridsPointer_);
}

//================================================================================
void Projection::
setOperators (CompositeGridFiniteVolumeOperators *operators_)
{
  opPointer = operators_;
  bool isVolumeScaled = opPointer->getIsVolumeScaled();
  assert (!isVolumeScaled);
  operatorsAreSet = LogicalTrue;
}

//==============================================================================
void Projection::
setMovingGridsPointer (MovingGrids * movingGridsPointer_)
{
  movingGridsPointer = movingGridsPointer_;
  movingGridsPointerIsSet = LogicalTrue;
}

  //
//=================================================================================
Projection::
~Projection()
//=================================================================================
{
  cout << "Projection destructor :):)" << endl;
  cleanup();

}
//=================================================================================
void Projection::
initializeNonGridItems()
//
// Private class function for initializing the class. This is called by
// all constructors (whether or not a CompositeGrid is specified)
//
//=================================================================================
{
  operatorsAreSet = LogicalFalse;
  Oges::debug = 0;
  firstCallToUTMG = LogicalTrue;
  timestep = 0.;
  timestepIsSet = LogicalFalse;
  twilightZoneFlow = LogicalFalse;
  twilightZoneFlowFunction = NULL;
  ellipticCompatibilityConstraintSet = LogicalTrue;
  isVariableDensityProjection = LogicalFalse;
  reinitializeCoefficients = LogicalTrue;   //initially set to LogicalTrue so that the coefficients will always be initialized at least once
  useExactVelocity = LogicalFalse;          // this is a debugging parameter set in getPerturbedVelocity
  previousTime = undefinedRealValue;
  
  
  if (projectionDebug) projectionDisplay.interactivelySetInteractiveDisplay ("projectionDisplay initialization");

   general           = GridFunctionParameters::general;
   vertexCentered    = GridFunctionParameters::vertexCentered;
   cellCentered      = GridFunctionParameters::cellCentered;
   faceCenteredAll   = GridFunctionParameters::faceCenteredAll;
   faceCenteredAxis1 = GridFunctionParameters::faceCenteredAxis1;
   faceCenteredAxis2 = GridFunctionParameters::faceCenteredAxis2;
   faceCenteredAxis3 = GridFunctionParameters::faceCenteredAxis3;
   defaultCentering  = GridFunctionParameters::defaultCentering;
  
   cellCenteredWith0Components = GridFunctionParameters::cellCenteredWith0Components;
   cellCenteredWith1Component  = GridFunctionParameters::cellCenteredWith1Component;

   cellCenteredWith2Components = GridFunctionParameters::cellCenteredWith2Components;

   faceCenteredAllWith0Components = GridFunctionParameters::faceCenteredAllWith0Components;
   faceCenteredAllWith1Component  = GridFunctionParameters::faceCenteredAllWith1Component;
   faceCenteredAllWith2Components = GridFunctionParameters::faceCenteredAllWith2Components;

// ... Elliptic solver defaults

  iterativeImprovementReset = LogicalFalse;
  preconditionBoundaryReset = LogicalFalse;
  conjugateGradientPreconditionerReset = LogicalFalse;
  conjugateGradientTypeReset = LogicalFalse;
  conjugateGradientNumberOfIterationsReset = LogicalFalse;
  conjugateGradientNumberOfSaveVectorsReset = LogicalFalse;
  sorNumberOfIterationsReset = LogicalFalse;
  zeroRatioReset = LogicalFalse;
  fillinRatio2Reset = LogicalFalse;
  harwellToleranceReset = LogicalFalse;
  matrixCutoffReset = LogicalFalse;
  sorOmegaReset = LogicalFalse;
  
  solverTypeValue = yale;      solverTypeReset  = LogicalTrue;
  fillinRatioValue = 20;       fillinRatioReset = LogicalTrue;
  
}
//=================================================================================
void Projection::
cleanup()
//
// This private function is called by the destructor.
//=================================================================================
{
  if (!firstCallToUTMG) //note: this will only be LogicalTrue if updateToMatchGrid has been called, and listOf*BCs have been new'ed
  {
    delete [] listOfBCs;
    delete [] listOfVelocityBCs;
  }
}

//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{updateToMatchGrid}}  
void Projection::
updateToMatchGrid(CompositeGrid & cg)
//=================================================================================
//
// /Purpose:
//   update the Projection class to match a composite grid.
//   Internal storage and parameters are reset to correspond
//   to the new grid.
// 
// /cg: CompositeGrid to update it to.
//
// /951208: this routine is called each time the grid changes, and will

//          result in the elliptic solver being set up again.
//
// /Author:				D.L.Brown
// /Date Doc Last Modified:	951208
//
//\end{projectionDoc.tex} 
//========================================

{
  cout << "Projection::updateToMatchGrid called..." << endl;
//  operators.updateToMatchGrid            (cg);
//  operatorsForVelocity.updateToMatchGrid (cg);

  updateToMatchGridOnly (cg);
}

/*
//==============================================================================
//\begin{>>projectionDoc.tex}{\subsection{updateToMatchGrid}}  
void Projection::
updateToMatchGridAndOperators (CompositeGrid & cg,
			       CompositeGridFiniteVolumeOperators * operators_)
//
// /Purpose:
//   This routine allows the operators to be set for the Projection class.
//   Otherwise, a new copy of the FiniteVolumeOperators is made for use by
//   the class internally.
//
// 
// /cg: CompositeGrid to update it to.
// /operators\_: operators to use
//
//\end{projectionDoc.tex}
//==============================================================================
{


  cout << "Projection::updateToMatchGridAndOperators called... " << endl;
  opPointer = &operators_;

  operatorsForVelocity.updateToMatchGrid (cg);  // *** fix this: once the new boundary conditions are used, these operators
                                                //             should be the same as the regular operators.

  updateToMatchGridOnly (cg);


    cout << "Projection::updateToMatchGridAndOperators not yet implemented" << endl;
    cout << "  Operators will not be set" << endl;
    cout << "  Calling Projection::updateToMatchGrid instead..." << endl;
    updateToMatchGrid (cg);

}

*/  

//==============================================================================

//
//==============================================================================
//
void Projection::
updateToMatchGridOnly (CompositeGrid &cg)
//==============================================================================
{

  if (!operatorsAreSet)
  {
    cout << "ERROR: Projection::updateToMatchGrid called before setting operators" << endl;
    cout << "   call Projection::setOperators before updateToMatchGrid " << endl;
    assert (!operatorsAreSet);
  }
  
// ***981112 I dont think this needs to be here?
//  opPointer->setTwilightZoneFlow (twilightZoneFlow);
  
 
  compositeGrid.reference (cg);

// ... the grid must have changed if this is being called so...

  reinitializeCoefficients = LogicalTrue;
  
  numberOfDimensions = cg.numberOfDimensions();
  numberOfComponentGrids = cg.numberOfComponentGrids();
  int stencilSize = numberOfDimensions == 2 ? 10 : 28;

  if (firstCallToUTMG)
  {


//    int maxBC = getMaximumCMPGRDBC ();
//    maximumNumberOfOvertureBCs = maxBC;
//961206: usage of maximumNumberOfOvertureBCs appears to be different than it was originally
//    its OK to associate an unused boundary type with a BC, so pick an arbitrary maximum value

    int maxBC = 50;
    maximumNumberOfOvertureBCs = maxBC;
    
    listOfBCs = new BoundaryConditionType [maxBC+1];
    listOfVelocityBCs = new BoundaryConditionType [maxBC+1];
    int i;
    for (i=0; i<=maxBC; i++){
      listOfBCs[i] = noBoundaryConditionDefined;
      listOfVelocityBCs[i]= noBoundaryConditionDefined;
    }

    int numberOfSides = 2;
    int numberOfVelocityComponents = 3;
    velocityBoundaryConditionValueGiven.redim 
      (numberOfComponentGrids, numberOfSides, numberOfDimensions, numberOfVelocityComponents);
    velocityBoundaryConditionValue.redim
      (numberOfComponentGrids, numberOfSides, numberOfDimensions, numberOfVelocityComponents);      
    if (LogicalFalse)
    {
      cout << "listOfBCs: " << endl;
      for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfBCs[i] << " ";
      cout << endl << "listOfVelocityBCs:" << endl;
      for (i=0; i<= maximumNumberOfOvertureBCs; i++) cout << listOfVelocityBCs[i] << " ";
      cout << endl;
    }

    firstCallToUTMG = LogicalFalse;
  }
  

  coefficients.updateToMatchGrid         (cg, stencilSize, all, all, all);  
  

// ...960725 try adding this

  int numberOfCC = 1;

  opPointer->setStencilSize (stencilSize);
  opPointer->setNumberOfComponentsForCoefficients (numberOfCC);

  coefficients.setIsACoefficientMatrix (LogicalTrue, stencilSize);

// ...960725
  coefficients = 0.;
  
  phi.updateToMatchGrid                  (cg, all, all, all);
  GridFunctionParameters::GridFunctionTypeWithComponents phiType = phi.getGridFunctionTypeWithComponents();  
  phi.setOperators (*opPointer);
  
  phi = 0.;

//  if (!isVariableDensityProjection) updateEllipticSolverToMatchGrid        (cg);
// 951208: no, always do this:
// 981105: it appears that this should be done AFTER setting up the Laplacian coefficients instead
//  updateEllipticSolverToMatchGrid        (cg);
  
    
}
//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{setTwilightZoneFlow}}
void Projection::
setTwilightZoneFlow (const bool TrueOrFalse //=LogicalTrue
  )
//
// /Purpose: ``TwilightZoneFlow'' is used for debugging purposes.
//           A TwilightZoneFlowFunction must be set, as must the timestep 
//           (see documentation for setTimestep). If the result of the projection
//           is to compute an incremental pressure, the user must also call
//           setIsIncrementalPressureFormulation(), and provide the time level
//           of the previously computed pressure through a call to 
//           setPreviousPressureTimeLevel().
//           NOTE: It is assumed that the ``exact'' pressure is stored in the TwilightZone
//           function in component {\tt numberOfDimensions}.
//\end{projectionDoc.tex}
//=================================================================================
{
  twilightZoneFlow = TrueOrFalse;

  if (operatorsAreSet)
  {
    opPointer->setTwilightZoneFlow (twilightZoneFlow);
  }
  else
  {
    cout << "Projection::setTwilightZoneFlow: Must call Projection::setOperators before setting TwilightZoneFlow" << endl;
    assert (operatorsAreSet);
  }
  
}
//=================================================================================
void Projection::
setTwilightZoneFlowFunction (OGFunction & TwilightZoneFlowFunction_)
//
// /Purpose: set the ``TwilightZoneFlowFunction''.
//           It is assumed that
//           the first numberOfDimensions components of the TwilightZoneFlowFunction
//           contain the velocity components of the exact solution, and that component
//           numberOfDimensions+1 contains the exact pressure. 
//
// /981123: This version is obsolete, use the pointer version instead
//
//\end{projectionDoc.tex}
//=================================================================================
{
  cout << "WARNING: Projection::setTwilightZoneFlowFunction (OGFunction&) is obsolete " << endl;
  cout << " Please use Projection::setTwilightZoneFlowFunction (OGFunction*) instead " << endl;
  
  if (!twilightZoneFlow)
  {
    cout << "Projection:setTwilightZoneFlowFunction: ERROR: Can't set TwilightZoneFunction" << endl;
    cout << "   before you set TwilightZoneFlow. Call Projection::setTwilightZoneFlow first." << endl;
    assert (twilightZoneFlow);
  }
  twilightZoneFlowFunction = &TwilightZoneFlowFunction_;
  opPointer->setTwilightZoneFlowFunction (*twilightZoneFlowFunction);
}
//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{setTwilightZoneFlowFunction}}
void Projection::
setTwilightZoneFlowFunction (OGFunction * TwilightZoneFlowFunction_)
//
// /Purpose: set the ``TwilightZoneFlowFunction''.
//           It is assumed that
//           the first numberOfDimensions components of the TwilightZoneFlowFunction
//           contain the velocity components of the exact solution, and that component
//           numberOfDimensions+1 contains the exact pressure. 
//
// 
//\end{projectionDoc.tex}
//=================================================================================
{
  if (!TwilightZoneFlowFunction_) return;
  
  if (!twilightZoneFlow)
  {
    cout << "Projection:setTwilightZoneFlowFunction: ERROR: Can't set nontrivial TwilightZoneFunction" << endl;
    cout << "   before you set TwilightZoneFlow. Call Projection::setTwilightZoneFlow first." << endl;
    assert (twilightZoneFlow);
  }
  twilightZoneFlowFunction = TwilightZoneFlowFunction_;
  opPointer->setTwilightZoneFlowFunction (*twilightZoneFlowFunction);
}
//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{setTimestep}}
void Projection::
setTimestep ( const real & dt)
//=================================================================================
//
// /Purpose:
//    Let the Projection class know what the value of the timestep is. This
//    must be called before a call to {\bf getPressure} since P = $\phi$/$\Delta t$.
//    This is also used for TwilightZoneFlow problems. Since the given "exact" solution
//    is for pressure and not $\phi$, the Projection class needs to know what the
//    conversion factor is between the two. Note that for a multistep or
//    Runge-Kutta method, the intermediate pressure may not be related to $\phi$ 
//    by the full timestep.
//
// /dt: set the timestep to this value
//
// /Author: D. L. Brown
// /Date documentation last modified: 951019
//\end{projectionDoc.tex}
//================================================================================ 
{
  if (dt > 0)
  {
    
    timestep = dt;
    timestepIsSet = LogicalTrue;
  }
  else
  {
    timestep = dt;
    timestepIsSet = LogicalFalse;
    cout << "Projection::setTimestep: WARNING: unable to set timestep; non-positive input value" << endl;
  }
  
}

//==============================================================================
//\begin{>>projectionDoc.tex}{\subsection{setIsIncrementalPressureFormulation}}
void Projection::
setIsIncrementalPressureFormulation (
  const bool & trueOrFalse // = LogicalFalse
  )
//
// /Purpose:
//    For the TwilightZone case only, indicate that the projection is
//    being used to compute an incremental pressure. It is not necessary
//    to call this function except for the TwilightZoneFlow case.
//
//\end{projection.Doc.tex}
//==============================================================================
{
  isIncrementalPressureFormulation = trueOrFalse;
}
//================================================================================
//\begin{>>projectionDoc.tex}{\subsection{setPreviousPressureTimeLevel}} 
void Projection::
setPreviousPressureTimeLevel (const real & time)
//
// /Purpose:
//   For the TwilightZone case and incrementalPressureFormulation case only,
//   indicates at what value of time the previously computed pressure is 
//   evaluated.
//   It is not necessary to call this function except for the TwilightZone case.
//\end{projectionDoc.tex}
//==============================================================================
{
  previousTime = time;
}


//================================================================================
//\begin{>>projectionDoc.tex}{\subsection{setDensity}} 
void Projection::
setDensity (REALCompositeGridFunction & rho0)
//===============================================================================
//
// /Purpose:
//    This function sets the density function for a variable density
//    projection. It must be called every time the density changes.
//    The elliptic coefficients will be reinitialized in the next call
//    to {\bf project}
// /rho0(all,all,all): set the Projetion density to this function.
//  rho0 should be a cellCentered REALCompositeGridFunction.
//\end{projectionDoc.tex}
//=============================================================================== 
{
  if (!density.getIsCellCentered())
  {
    throw "ERROR: Expecting cellCentered density for Projection class";
  }
  
  if (!isVariableDensityProjection)
  {
    isVariableDensityProjection = LogicalTrue;
    cout << "WARNING: since a density is being set, it is assumed that a variableDensity projection is desired" << endl;
  }

  density = rho0;   //if we set a reference, the user might delete rho0, and then we'd be in trouble
  reinitializeCoefficients = LogicalTrue;
}

//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{getPressure}} 
REALCompositeGridFunction Projection::
getPressure ()
//
// /Purpose: Returns the pressure following a projection. The function setTimestep
//           must be called first, since $p = \phi/{\Delta t}$. 
//   
//   /getPressure: returns a REALCompositeGridFunction of dimension (all,all,all)
//                 containing cellCentered pressure.
//
// /Author: D. L. Brown
// /Date documentation last modified: 951019
//\end{projectionDoc.tex} 
//=================================================================================
{
  REALCompositeGridFunction returnedValue;
  returnedValue.updateToMatchGrid (compositeGrid, all, all, all);
  returnedValue.setOperators (*opPointer);
  
  if (timestepIsSet) 
    returnedValue = phi/timestep;
  else
  {
    returnedValue = 0.;
    cout << "Projection::getPressure: WARNING: cannot compute pressure, returning zero" << endl;
  }
  return (returnedValue);
}
//=================================================================================
//\begin{>>projectionDoc.tex}{\subsection{getPhi}} 
REALCompositeGridFunction Projection::
getPhi()
//
// /Purpose: Return the projection potential $\phi$ following a call to one of the
//           {\bf project} functions. 
//
//   /getPhi: returns a REALCompositeGridFunction of dimension (all,all,all) containing
//            the (cellCentered) projection potential.
//
// /Author: D. L. Brown
// /Date documentation last modified: 951019
//\end{projectionDoc.tex} 
//=================================================================================
{
  return phi;
}


// ========================================================================================
void Projection::
updateEllipticSolverToMatchGrid (CompositeGrid & cg)
//
// private function: this is used to initialize the elliptic solver.
// ========================================================================================
{    

  cout << "Projection::updateEllipticSolverToMatchGrid called ... " << endl;
  
//  ellipticSolver.setCompositeGrid (cg);
  
  // ==============================
  // Set parameters for the elliptic solver
  // ==============================

  
  if (ellipticCompatibilityConstraintSet)
  {
    ellipticSolver.setCompatibilityConstraint (LogicalTrue);
  }
  else
  {
    ellipticSolver.setCompatibilityConstraint (LogicalFalse);
  }
  


//...hardwired Oges options
  ellipticSolver.setCoefficientArray (coefficients);

  ellipticSolver.setOrderOfAccuracy (2); //...shouldn't be necessary


  // ========================================
  // if its not variable density, then setup the matrix and refactor
  //  no, don't do this; instead, set reinitializeCoefficients to true and let project do it
  // ========================================

  cout << "Projection::updateEllipticSolverToMatchGrid: reinitializing coefficients and updateToMatchGrid..." << endl;
  reinitializeCoefficients = LogicalTrue;  
  ellipticSolver.updateToMatchGrid (cg);

// ... should no longer be necessary
//  ellipticSolver.setNumberOfGhostLines (1);
//  ellipticSolver.setGhostLineOption (ghostLine1, Oges::useGhostLineExceptCorner);

//  cout << "   :::::::calling Oges::updateToMatchGrid again" << endl;
//  ellipticSolver.updateToMatchGrid (cg);
  

//  ellipticSolver.initialize();  //...need to call this because the classify array must be reset--951208 this is called in
                                  // Oges::updateToMatchGrid now.

//  if (!isVariableDensityProjection) formLaplacianCoefficients ();
    
}  
// ========================================================================================
void Projection::
setEllipticCompatibilityConstraint(const bool trueOrFalse)
//
// /Purpose:
//   THIS FUNCTION IS OBSOLETE; included for compatibility,
//   with older versions of the class. However it doesn't do anything.
//
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951026
//========================================

{
  cout << "*****" << endl;
  cout << "Projection::setEllipticCompatibilityConstraint: WARNING: This function is obsolete" << endl;
  cout << "  the elliptic compatibility constraint will be reset by the boundary condition routines. " << endl;
  cout << "*****" << endl;

  //ellipticCompatibilityConstraintSet = trueOrFalse;
}
// ========================================================================
//\begin{>>projectionDoc.tex}{\subsection{setIsVariableDensityProjection}}  
void Projection::
setIsVariableDensityProjection (const bool trueOrFalse)
//
// /Purpose:
//   let the class know that the projections will be 
//   variable density projections or not. It should be
//   unneccesary to call this function, as it is called
//   by {\bf setDensity}, but I'm leaving it in here for now just in case. 
//
// /Interface: (inputs)
//   trueOrFalse (default LogicalTrue) indicates whether this is
//               is a variable density projection (LogicalTrue)
//               or a constant coefficient projection (LogicalFalse)
// /Author:				D.L.Brown
// /Date Documentation Last Modified:	951019
//
//\end{projectionDoc.tex} 
//========================================
{
  isVariableDensityProjection = trueOrFalse;
  cout << "Projection::setIsVariableDensity = " << trueOrFalse << endl;
}


// ===============================================================================
Projection::ProjectionType Projection:: 
getProjectionType (const REALCompositeGridFunction & uStar)
//
// figure out the projection type from the centering and dimensions of input velocity uStar
//
// ===============================================================================
{
  
  ProjectionType projectionType;
  
  projectionType = noProjection;
  GridFunctionParameters::GridFunctionTypeWithComponents gfType = 
    uStar.getGridFunctionTypeWithComponents();  

  switch (gfType) 
  {
  case GridFunctionParameters::cellCenteredWith1Component:
    projectionType = approximateProjection;
    break;

  case GridFunctionParameters::faceCenteredAllWith0Components:
    projectionType = macProjectionOfNormalVelocity;
    break;

  case GridFunctionParameters::faceCenteredAllWith1Component:
    projectionType = macProjection;
    break;

  default:
    projectionType = noProjection;
  
// ========================================
// Backup checks
// ======================================== 

    if (uStar.getFaceCentering() == GridFunctionParameters::all && uStar.getComponentDimension(1) == numberOfDimensions)
      projectionType = macProjection;
    
    if (uStar.getFaceCentering() == GridFunctionParameters::all && uStar.getComponentDimension(1) == 1)
      projectionType = macProjectionOfNormalVelocity;
    
    if (uStar.getIsCellCentered()) projectionType = approximateProjection;
    break;


};
  if (projectionType == noProjection)
    cout << "Warning: input velocity doesn't define the projection type..." << endl;

  return (projectionType);
    
}

// ==============================================================================
void Projection::
setPerturbedVelocity (ExactVelocityType & exactVelocityType, real & perturbationSize)
//
// /Purpose:
//   This routine is used for debugging. It sets an exact divergence free velocity
//   internally to the class, and also a perturbed velocity with perturbation
//   size perturbationSize. It also sets the flag useExactVelocity, which tells
//   Projection::project to force the velocity boundary conditions with the exact
//   velocity. To get the exact or perturbed velocities, use the functions
//   getExactVelocity() and getPerturbedVelocity.
//
// /exactVelocityType: choices are periodicVelocity and polynomialVelocity
// /perturbationSize:  the size of the perturbation from a divergence-free velocity
// ==============================================================================
{
// ========================================
// if 3D, exit with error since not implemented
// ========================================

  if (numberOfDimensions == 3)
  {
    cout << "Projection:setPerturbedVelocity: not implemented for 3D" <<endl;
    assert (numberOfDimensions == 2);
  }

// =====================
// declarations
// =====================

  int grid;
  REAL pi = 3.1415927, HALF = 0.5, ZERO = 0.0, TWO = 2.0, THIRD = 1./3., FOURTH = 0.25;

  Index I1,I2,I3;

  perturbedVelocity.updateToMatchGrid (compositeGrid,  defaultCentering, numberOfDimensions);
  exactVelocity.updateToMatchGrid     (compositeGrid,  defaultCentering, numberOfDimensions);
  exactGradient.updateToMatchGrid     (compositeGrid,  defaultCentering, numberOfDimensions);
  exactPhi.updateToMatchGrid          (compositeGrid,  defaultCentering);

// ========================================
// set useExactVelocity to LogicalTrue to alert the projection boundary conditions routines
// ========================================

  useExactVelocity = LogicalTrue;

// ========================================
// Loop over grids:
//   Make links to x,y, and individual velocities and gradients for convenience
// ========================================

  ForAllGrids (grid)
  {
    MappedGrid & mg = compositeGrid[grid];
// wdh  REALMappedGridFunction x, y, u, v, uPerturbed, vPerturbed, gradx, grady;

    REALMappedGridFunction u, v, uPerturbed, vPerturbed, gradx, grady;

    const realArray & x = mg.center()(all,all,all,0);
    const realArray & y = mg.center()(all,all,all,1);
    
//    x.link (mg.center(), Range (xComponent, xComponent));
//    y.link (mg.center(), Range (yComponent, yComponent));

    u.link (exactVelocity[grid], Range (uComponent, uComponent));
    v.link (exactVelocity[grid], Range (vComponent, vComponent));
    
    uPerturbed.link (perturbedVelocity[grid], Range (uComponent, uComponent));
    vPerturbed.link (perturbedVelocity[grid], Range (vComponent, vComponent));

    gradx.link (exactGradient[grid], Range (xComponent, xComponent));
    grady.link (exactGradient[grid], Range (yComponent, yComponent));
    
// ========================================
//     Depending on the exactVelocityType, assign both the
//      exactVelocity class function and returnedValue, the 
//      perturbed velocity function
// ========================================

    switch (exactVelocityType)
    {
    case noExactVelocity:

      u = ZERO;
      v = ZERO;

      uPerturbed = ZERO;
      vPerturbed = ZERO;

      gradx = ZERO;
      grady = ZERO;

      exactPhi[grid] = ZERO;

      break;

    case zeroExactVelocity:

      u = ZERO;
      v = ZERO;

      uPerturbed = u - perturbationSize * x*x;
      vPerturbed = v + perturbationSize * y*y;

      gradx =  perturbationSize * x*x;
      grady = -perturbationSize * y*y;

      exactPhi[grid] = THIRD*perturbationSize * (x*x*x - y*y*y);

      break;
      
    case periodicVelocity:

      u = cos(pi*y) + HALF*perturbationSize * (cos(pi*x)*cos(pi*y) + sin(pi*x)*sin(pi*y));
      v = cos(pi*x) + HALF*perturbationSize * (cos(pi*x)*cos(pi*y) + sin(pi*x)*sin(pi*y));
      
      uPerturbed = cos(pi*y) + perturbationSize * cos(pi*x)*cos(pi*y);
      vPerturbed = cos(pi*x) + perturbationSize * cos(pi*x)*cos(pi*y);

      gradx = HALF*perturbationSize * (cos(pi*x)*cos(pi*y) - sin(pi*x)*sin(pi*y));
      grady = HALF*perturbationSize * (cos(pi*x)*cos(pi*y) - sin(pi*x)*sin(pi*y));

      exactPhi[grid] = HALF*perturbationSize/pi * (cos(pi*x)*sin(pi*y) + sin(pi*x)*cos(pi*y));
          

      break;

    case polynomialVelocity:

      u = y + perturbationSize * x*(x + TWO*y);
      v = x - perturbationSize * y*(y + TWO*x);
            
      uPerturbed = u - perturbationSize * x*x;
      vPerturbed = v + perturbationSize * y*y;

      gradx =  perturbationSize * x*x;
      grady = -perturbationSize * y*y;

      exactPhi[grid] = THIRD*perturbationSize * (x*x*x - y*y*y);
    
      break;

    case shearLayers:

    //... set x-velocity
    
    where(y >= -0.25 && y <= 0.25)
    {
      u = tanh(30.0*y);
    }
    elsewhere(y > 0.25)
    {
      u = tanh(30.0*(HALF - y));
    }
    elsewhere(y < -0.25)

    {
// ... what WDH originally gave me:
//      u = tanh(30.0*(y - HALF));
        u = -tanh(30.0*(y + HALF));
    }

    //... set y-velocity
        
      v = 0.05 * sin(TWO*pi*x);

   // ... perturbed velocities

      uPerturbed = u + perturbationSize * HALF * (cos(TWO*pi*x)*cos(TWO*pi*y) - sin(TWO*pi*x)*sin(TWO*pi*y));
      vPerturbed = v + perturbationSize * HALF * (cos(TWO*pi*x)*cos(TWO*pi*y) - sin(TWO*pi*x)*sin(TWO*pi*y));

   // ... phi and grad(phi) for these functions

      exactPhi[grid] = 
	perturbationSize*(FOURTH/pi)*(sin(TWO*pi*x)*cos(TWO*pi*y) + cos(TWO*pi*x)*sin(TWO*pi*y));
      gradx    = perturbationSize * HALF * (cos(TWO*pi*x)*cos(TWO*pi*y) - sin(TWO*pi*x)*sin(TWO*pi*y));
      grady    = perturbationSize * HALF * (cos(TWO*pi*x)*cos(TWO*pi*y) - sin(TWO*pi*x)*sin(TWO*pi*y));

      break;
      
    default:
      cout << "Projection::setPerturbedVelocity: unknown ExactVelocityType: " << exactVelocityType << endl;
      exit (-1);
    
      break;
    }; 

// ========================================
// If TZ flow, add in the TZ function
// ========================================

    if (twilightZoneFlow)
    {
      cout << "Projection::setPerturbedVelocity: adding TZflow function " << endl;
      
      getIndex (mg.dimension(), I1, I2, I3);
    
      real t = ZERO;
      OGFunction *e = twilightZoneFlowFunction;
      assert (e != NULL);

      u += (*e)(mg, I1, I2, I3, xComponent, t);
      v += (*e)(mg, I1, I2, I3, yComponent, t);
    
      uPerturbed += (*e)(mg, I1, I2, I3, xComponent, t);
      vPerturbed += (*e)(mg, I1, I2, I3, yComponent, t); 

      //...981130: phi doesn't change since we still have u = u* - grad(phi)


    }
  }  // End loop over grids

// ========================================
// Return the perturbed velocities
// ========================================
  if (projectionDebug)
  {
    realCompositeGridFunction perturbation (compositeGrid, defaultCentering, numberOfDimensions);
    perturbation = perturbedVelocity - exactVelocity;
    
    projectionDisplay.display (perturbedVelocity, "setPerturbedVelocity: perturbedVelocity");
    projectionDisplay.display (exactVelocity,     "setPerturbedVelocity: exactVelocity");
    projectionDisplay.display (perturbation,      "setPerturbedVelocity: perturbation");
    projectionDisplay.display (exactGradient,     "setPerturbedVelocity: exactGradient");
    projectionDisplay.display (exactPhi,          "setPerturbedVelocity: exactPhi");
  }
}

  

// ==============================================================================
REALCompositeGridFunction Projection::
getPerturbedVelocity ()
//
// /Purpose:
//   This routine is used for debugging. 
//   It returns a the perturbed velocity. 
//   See setPerturbedVelocity(). 
// ==============================================================================
{
  if (!useExactVelocity)
  {
    cout << "Projection::getPerturbedVelocity: ERROR: no perturbed velocity has been set" << endl;
    assert (useExactVelocity == LogicalTrue);
  }

  return (perturbedVelocity);
}

// ==============================================================================
REALCompositeGridFunction Projection::   
getExactVelocity()
//
// /Purpose:
//   This is a debug routine. It returns the exact velocity that is being used,
//   provided that one has been set.
// ==============================================================================
{
  // ========================================
  // Return with an error if an exact velocity has not been set
  // ========================================

  if (!useExactVelocity)
  {
    cout << "Projection::getExactVelocity: ERROR: no perturbed velocity has been set" << endl;
    assert (useExactVelocity == LogicalTrue);
  }

  // ========================================
  // Return the exact velocity function
  // ========================================

  return (exactVelocity);
  
}

// ==============================================================================
REALCompositeGridFunction Projection::
getExactPhi ()
//
// /Purpose:
//   This is a debug routine. It returns the exact projection potential (phi) that
//   is being used.
// ==============================================================================
{
  // ========================================
  // Return with an error if an exact velocity has not been set
  // ========================================

  if (!useExactVelocity)
  {
    cout << "Projection::getExactPhi: ERROR: no exact velocity has been set" << endl;
    assert (useExactVelocity == LogicalTrue);
  }

  // ========================================
  // Return the exact phi
  // ========================================

  return (exactPhi);
  
}

// ==============================================================================
REALCompositeGridFunction Projection::
getExactGradient ()
//
// /Purpose:
//   This is a debug routine. It returns the exact projection potential gradient
//   is being used.
// ==============================================================================
{
  // ========================================
  // Return with an error if an exact velocity has not been set
  // ========================================

  if (!useExactVelocity)
  {
    cout << "Projection::getExactGradient: ERROR: no exact velocity has been set" << endl;
    assert (useExactVelocity == LogicalTrue);
  }

  // ========================================
  // Return the exact gradient
  // ========================================

  return (exactGradient);
  
}


 
#include "project.C"
#include "applyVelocityBoundaryConditions.C"
#include "formLaplacianCoefficients.C"
#include "applyRightHandSideBoundaryConditions.C"
#include "associateCMPGRDBC.C"
