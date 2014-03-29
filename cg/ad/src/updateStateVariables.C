#include "Cgad.h"
#include "CompositeGridOperators.h"
#include "GridCollectionOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"
#include "AdParameters.h"


// ===================================================================================================================
/// \brief Ths function is used to update state-variables. For example, variable coefficients that depend
///  on time or the solution.
/// 
/// \param cgf (input/output)
/// \param stage (input) : -1, 0 or 1 
///
/// \details 
/// If stage equals -1 then update state variables at all points. 
/// 
/// This function is used at two different stages for each time step.  In the first
/// stage, (stage=0) the function is called after the solution has been advanced (but before boundary
/// conditions have been applied) to update any equilibrium state variables (and to limit any 
/// reacting species variables). Update all points of state variables 
/// that may be needed to apply the boundary conditions. 
///
/// In the second stage, (stage=1) the function is called after the boundary conditions have
/// been applied. Make sure that the state variables have been updated at all points after this step.
// ===================================================================================================================
int Cgad::
updateStateVariables(GridFunction & cgf, int stage /* = -1 */ )
{
  FILE *& debugFile =parameters.dbase.get<FILE* >("debugFile");
  FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");

  const bool variableDiffusivity = parameters.dbase.get<bool >("variableDiffusivity");
  const bool variableAdvection = parameters.dbase.get<bool >("variableAdvection");
  
  if( variableDiffusivity || variableAdvection )
  {
    if( stage==-1 || stage==1 )
    {
      if( variableDiffusivity )
	getDiffusionCoefficients( cgf );

      if( variableAdvection )
	getAdvectionCoefficients( cgf );
    }
  }

  return 0;
}
