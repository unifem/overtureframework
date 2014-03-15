#include "CgSolver.h"

//! This routine is called when CgSolver is finished with the initial conditions and can 
//!  be used to clean up memory.
void CgSolver::
userDefinedInitialConditionsCleanup()
{
  if( parameters.myid==0 ) 
    printf("***userDefinedInitialConditionsCleanup: delete arrays\n");

  delete uniformPointer;       uniformPointer=NULL;
  delete bubbleCentrePointer;  bubbleCentrePointer=NULL;
  delete bubbleRadiusPointer;  bubbleRadiusPointer=NULL;
  delete bubbleValuesPointer;  bubbleValuesPointer=NULL;
  delete hotSpotDataPointer;   hotSpotDataPointer=NULL;
  delete perturbDataPointer;   perturbDataPointer=NULL;

  delete shockPointer;         shockPointer=NULL;
  delete wallPointer;          wallPointer=NULL;

  delete uLeftPointer;         uLeftPointer=NULL;
  delete uCenterPointer;       uCenterPointer=NULL;
  delete uRightPointer;        uRightPointer=NULL;

  delete uSFPointer;           uSFPointer=NULL;
  delete cgSFPointer;          cgSFPointer=NULL;
}

//! This routine is called when CgSolver is finished and can be used to clean up memory.
void CgSolver::
userDefinedCleanup()
{
  if( parameters.myid==0 ) 
    printf("***userDefinedCleanup: delete arrays\n");

}
