#include "Cgsm.h"
#include "SmParameters.h"

// ===================================================================================================================
/// \brief Print time-step information about the current solution in a nicely formatted way.
///
/// \param step (input) : current step number.
/// \param t (input) : time.
/// \param cpuTime (input) : current cpu time.
/// 
// ==================================================================================================================
void Cgsm::
printTimeStepInfo( const int & step, const real & t, const real & cpuTime )
{
  //const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
 // FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
 // FILE *checkFile = parameters.dbase.get<FILE* >("checkFile");
 // const int & debug = parameters.dbase.get<int >("debug");
 // const RealArray & checkFileCutoff = parameters.dbase.get<RealArray >("checkFileCutoff");
  
 // GridFunction & solution = gf[current];

  // compute errors
  // getErrors( current,t,deltaT,sPrintF("\n****printTimeStepInfo: errors at t=%9.3e, dt=%9.3e *****\n",t,dt) );  
  getErrors( current,t,deltaT,"" );  


  // printP(" ********** printTimeStepInfo CALLED t=%9.3e ***************\n",t);

  // Save info to the check file: *wdh* 101017
  outputResults(current,t,deltaT);


  return;
}

