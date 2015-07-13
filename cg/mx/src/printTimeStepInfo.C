#include "Maxwell.h"
#include "ParallelUtility.h"
#include "App.h"


void Maxwell::
printTimeStepInfo( const int current, const int & step, const real & t, const real & dt, const real & cpuTime )
//=================================================================================
/// \brief  Print information about the current solution in a nicely formatted way
//=================================================================================
{
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

  if ( method==nfdtd || method==sosup )
    getMaxDivergence( current,t );

  aString label;
  getTimeSteppingLabel( dt,label );

  real maxMem=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage()); 

  for( int fileio=0; fileio<2; fileio++ )
  {
    FILE *output = fileio==0 ? logFile : stdout;
    fPrintF(output,">>> Cgmx:%s: t=%6.2e %s |div(E)|/|grad(E)|=%8.2e (=%8.2e/%8.2e) "
                   "%i steps mem=%.3g(Mb) cpu=%8.2e(s)\n",
	   (const char *)methodName,t,(const char*)label,
	    divEMax/max(REAL_MIN*100.,gradEMax),divEMax,gradEMax,step,maxMem,cpuTime);
    // fPrintF(output,">>> Cgmx:%s: t=%6.2e %s |div(E)|=%8.2e, |div(E)|/|grad(E)|=%8.2e, |grad(E)|=%8.2e, "
    //                "%i steps max-mem=%g (Mb) cpu=%8.2e(s)\n",
    // 	   (const char *)methodName,t,(const char*)label,
    // 	    divEMax,divEMax/max(REAL_MIN*100.,gradEMax),gradEMax,step,maxMem,cpuTime);
    if( solveForMagneticField && cg.numberOfDimensions()==3 )
    {
      fPrintF(output,
	     "|div(H)|/|grad(H)|=%8.2e (=%8.2e/%8.2e) (%i steps)\n",
	     divHMax/max(REAL_MIN*100.,gradHMax),divHMax,gradHMax,step);
      // fPrintF(output,
      // 	     "|div(H)|=%8.2e, |div(H)|/|grad(H)|=%8.2e, |grad(H)|=%8.2e (%i steps)\n",
      // 	     divHMax,divHMax/max(REAL_MIN*100.,gradHMax),gradHMax,step);
    }
  }
  
}
