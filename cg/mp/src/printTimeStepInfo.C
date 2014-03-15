#include "Cgmp.h"


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{printTimeStepInfo}} 
void Cgmp::
printTimeStepInfo( const int & step, const real & t, const real & cpuTime )
//=================================================================================
// /Description:
//    Print information about the current solution in a nicely formatted way
//  ** This is a virtual function **
//\end{CompositeGridSolverInclude.tex}  
//=================================================================================
{
  for( int d=0; d<domainSolver.size(); d++ )
  {
    
    if ( domainSolver[d] )
    {

      printF("============= %s time-step info for domain %i (%s)(%s) steps=%i==================\n",
	     (const char*)domainSolver[d]->getClassName(),d,(const char*)cg.getDomainName(d),
	     (const char*)domainSolver[d]->getName(),numberOfStepsTaken);

      // save checkFile info from all domains to mp.check by changing the checkFile in each DomainSolver:
      FILE *checkFileSave =domainSolver[d]->parameters.dbase.get<FILE* >("checkFile");
      domainSolver[d]->parameters.dbase.get<FILE* >("checkFile")=parameters.dbase.get<FILE* >("checkFile");
	
      domainSolver[d]->printTimeStepInfo(step,t,cpuTime);
	
      domainSolver[d]->parameters.dbase.get<FILE* >("checkFile")=checkFileSave;  // reset
    }
  }

}
