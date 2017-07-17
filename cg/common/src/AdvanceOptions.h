#ifndef ADVANCE_OPTIONS_H
#define ADVANCE_OPTIONS_H

#include "Overture.h"

// This class holds options that are passed to the DomainSolver time stepping functions
// such as takeTimeStep, takeTimeStepFE, takeTimeStepPC, ...

class AdvanceOptions
{

public:

enum TakeTimeStepOptionEnum
{
  takeStepAndApplyBoundaryConditions,
  takeStepButDoNotApplyBoundaryConditions,
  applyBoundaryConditionsOnly
};

enum GridChangesEnum
{
  noChangeToGrid,
  newAmrGrid,
};

      


AdvanceOptions();
~AdvanceOptions();

AdvanceOptions(const AdvanceOptions & x);
AdvanceOptions& operator=(const AdvanceOptions & x);

TakeTimeStepOptionEnum takeTimeStepOption;

int numberOfCorrectorSteps;  // this value is returned by startTimeStep
GridChangesEnum gridChanges;  // returned by startTimeStep to indicate if the grid has changed on this step (e.g. AMR)

bool correctionIterationsHaveConverged;

};

// This class holds info about each stage in a multi-stage algorithm (used by Cgmp)
class StageInfo
{
public:
  AdvanceOptions::TakeTimeStepOptionEnum action;
  std::vector<int> domainList;
};


#endif
