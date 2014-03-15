#include "Cgmp.h"


real Cgmp::
getTimeStep( GridFunction & gf)
{
  dt=REAL_MAX;
  for( int d=0; d<domainSolver.size(); d++ )
  {

    real dtd = REAL_MAX;
     if (domainSolver[d] )
      {
	dtd = domainSolver[d]->getTimeStep(domainSolver[d]->gf[domainSolver[d]->current]); // note we ignore gf

        if( debug() & 2 )
	{
	  printF(" ====== Cgmp::getTimeStep: t=%9.3e dt=%9.3e for domain %i (%s)(%s)  =======\n",gf.t,dtd,d,
		 (const char*)cg.getDomainName(d),(const char*)domainSolver[d]->getName());
	}
      }
    
    dt=min(dt,dtd);
  }
  if( debug() & 1 )
  {
    printF(" ------ Cgmp::getTimeStep: t=%9.3e, global dt=%9.3e  (min value from all domains)-------\n",gf.t,dt);
  }

  return dt;
}
