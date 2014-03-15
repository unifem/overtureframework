#include "AdvanceOptions.h"


// ========================================================================
/// \brief Constructor for the AdvanceStepsOptions class.
/// This class holds options that are passed to the DomainSolver time stepping functions
/// such as takeTimeStep, takeTimeStepFE, takeTimeStepPC, ...
// ========================================================================
AdvanceOptions::
AdvanceOptions()
{
  takeTimeStepOption=takeStepAndApplyBoundaryConditions;
  numberOfCorrectorSteps=0; // this value is returned by startTimeStep
  gridChanges=noChangeToGrid;   // returned by startTimeStep to indicate if the grid has changed on this step (e.g. AMR)
  correctionIterationsHaveConverged=false;
}
// ========================================================================
/// \brief Destructor for the AdvanceStepsOptions class.
// ========================================================================
AdvanceOptions::
~AdvanceOptions()
{
}


// ========================================================================
/// \brief Copy constructor.
// ========================================================================
AdvanceOptions::
AdvanceOptions(const AdvanceOptions & x)
{
  *this=x;
}


// ========================================================================
/// \brief Equals operator.
// ========================================================================
AdvanceOptions& AdvanceOptions::
operator=(const AdvanceOptions & x)
{
  takeTimeStepOption     =x.takeTimeStepOption;
  numberOfCorrectorSteps =x.numberOfCorrectorSteps;
  gridChanges            =x.gridChanges;

  return *this;
}

