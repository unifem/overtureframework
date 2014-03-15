#include "DomainSolver.h"
#include "ParallelUtility.h"
#include "App.h"


//\begin{>>CompositeGridSolverInclude.tex}{\subsection{printTimeStepInfo}} 
void DomainSolver::
printTimeStepInfo( const int & step, const real & t, const real & cpuTime )
//=================================================================================
// /Description:
//    Print information about the current solution in a nicely formatted way
//  ** This is a virtual function **
//\end{CompositeGridSolverInclude.tex}  
//=================================================================================
{
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  FILE *debugFile = parameters.dbase.get<FILE* >("debugFile");
  FILE *checkFile = parameters.dbase.get<FILE* >("checkFile");
  const int & debug = parameters.dbase.get<int >("debug");
  const RealArray & checkFileCutoff = parameters.dbase.get<RealArray >("checkFileCutoff");
  
  GridFunction & solution = gf[current];

  if( (false || debug & 4) &&  // *wdh* 060302
      (false || !parameters.dbase.get<bool >("twilightZoneFlow")) )
  {
    if( parameters.dbase.get<int >("myid")==0 ) 
      fprintf(debugFile," ***printTimeStepInfo: Solution at t=%e***\n",t);
    outputSolution( solution.u,t );
  }

  int n;
  RealArray error(numberOfComponents+5);  
  error = 0.;
  // ===Check errors, print results====
  determineErrors( solution.u,solution.gridVelocity,t,0,error );

  // determine the max/min of all components: uMax, uMin, uvMax
  RealArray uMin(numberOfComponents), uMax(numberOfComponents);
  real uvMax;
  // *wdh* 060909 getSolutionBounds(uMin,uMax,uvMax);
  getBounds(solution.u,uMin,uMax,uvMax);

 
  // In Parallel -- compute the max over all statistics
  // ***** For some values it would be better to compute sum(times)/NP
  for( int i=0; i<parameters.dbase.get<RealArray >("statistics").getLength(0); i++ )
  {
    parameters.dbase.get<RealArray>("statistics")(i)=ParallelUtility::getMaxValue(parameters.dbase.get<RealArray>("statistics")(i));
  }

  real maxMem=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage()); 

  if( parameters.dbase.get<int >("myid")!=0 ) return; // only print on processor 0

  aString blanks="                            ";
  bool isTW = parameters.dbase.get<bool >("twilightZoneFlow");
  realCompositeGridFunction* pKnown = parameters.dbase.get<realCompositeGridFunction* >("pKnownSolution");
  if( parameters.dbase.get<bool >("twilightZoneFlow") || parameters.dbase.get<realCompositeGridFunction* >("pKnownSolution") )
  {
    // ****************************************************************
    // *********** twilightzone flow or knownSolution *****************
    // ****************************************************************
    for( int io=0; io<3; io++ )
    {
      FILE *file = io==0 ? stdout : io==1 ? parameters.dbase.get<FILE* >("debugFile") : parameters.dbase.get<FILE* >("logFile");

      // do not put cpu times into the debug file so we can compare serial to parallel for e.g.
      const real cpu = io!=1 ? cpuTime : 0.;  

      if( file==NULL ) continue;   

      if( numberOfComponents<10 )
      {
	if( step<=2 || ((step-1) % 10 == 0) || true ) // always print header *wdh* 080509
	{
	  if( !parameters.isSteadyStateSolver() )
            fprintf(file,"     t ");
          else
            fprintf(file,"     it");

	  for( n=0; n<numberOfComponents; n++)
	    fprintf(file,"   err(%s)",(const char*)parameters.dbase.get<aString* >("componentName")[n]);
	  fprintf(file,"    uMax     dt       cpu    mem (Gb)\n");
	}
	if( !parameters.isSteadyStateSolver() )
          fprintf(file," %7.3f",t);
        else
          fprintf(file,"%10i",parameters.dbase.get<int >("globalStepNumber")+1);
	for( n=0; n<numberOfComponents; n++)
	  fprintf(file," %8.2e",error(n));
	fprintf(file," %8.2e %8.2e %8.2e %8.2g\n",uvMax,dt,cpu,maxMem);
      }
      else
      {
	if( !parameters.isSteadyStateSolver() )
	  fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds, max-mem=%g (Mb) (%i steps)\n",
                  t,dt,cpu,maxMem,parameters.dbase.get<int >("globalStepNumber")+1);
	else
	  fprintf(file," >>> it= %10i, dt =%9.2e, cpu =%9.2e seconds, max-mem=%g (Mb) \n",
                  parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu,maxMem);

	for( n=0; n<numberOfComponents; n++ )
	  fprintf(file," %s%s : (min,max)=(%13.6e,%13.6e), error=%10.3e \n",
		 (const char*)blanks(0,max(0,10-parameters.dbase.get<aString* >("componentName")[n].length())),
		 (const char*)parameters.dbase.get<aString* >("componentName")[n],
		 uMin(n),uMax(n),error(n));
	fprintf(file,"Max errors:");
	for( n=0; n<numberOfComponents; n++ )
	  fprintf(file," %10.3e  &",error(n));
	fprintf(file,"\n");
      }
    }

//      // print errors to the debug file
//      fprintf(debugFile," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds \n",t,dt,cpuTime);
//      for( n=0; n<numberOfComponents; n++ )
//        fprintf(debugFile," %s%s : (min,max)=(%13.6e,%13.6e), error=%16.9e \n",
//                (const char*)blanks(0,max(0,10-parameters.dbase.get<aString* >("componentName")[n].length())),
//  	      (const char*)parameters.dbase.get<aString* >("componentName")[n],
//                   uMin(n),uMax(n),error(n));

    // output results to the check file
    fprintf(checkFile,"%9.2e %i  ",t,numberOfComponents);
    for( n=0; n<numberOfComponents; n++ )
    {
      real err = error(n) > checkFileCutoff(n) ? error(n) : 0.;
      real uc = max(fabs(uMin(n)),fabs(uMax(n)));
      if( uc<checkFileCutoff(n) ) uc=0.;
      fprintf(checkFile,"%i %9.2e %10.3e  ",n,err,uc);
    }
    
    fprintf(checkFile,"\n");

  }
  else
  {
    // **********************************************
    // ************** real run **********************
    // **********************************************
    for( int io=0; io<3; io++ )
    {
      FILE *file = io==0 ? stdout : io==1 ? parameters.dbase.get<FILE* >("debugFile") : parameters.dbase.get<FILE* >("logFile");
      if( file==NULL ) continue;
      
      // do not put cpu times into the debug file so we can compare serial to parallel for e.g.
      const real cpu = io!=1 ? cpuTime : 0.;  

      if( !parameters.isSteadyStateSolver() )
        fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds, max-mem=%g (Mb) (%i steps)\n",
              t,dt,cpu,maxMem,parameters.dbase.get<int >("globalStepNumber")+1);
      else
        fprintf(file," >>> it=   %10i, dt =%9.2e, cpu =%9.2e seconds, max-mem=%g (Mb)\n",
                     parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu,maxMem);
      for( n=0; n<numberOfComponents; n++ )
        fprintf(file," %s%s : (min,max)=(%13.6e,%13.6e) \n",
		(const char*)blanks(0,max(0,10-parameters.dbase.get<aString* >("componentName")[n].length())),
		(const char*)parameters.dbase.get<aString* >("componentName")[n],
		uMin(n),uMax(n));

//        fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds \n",t,dt,cpu);
//        for( n=0; n<numberOfComponents; n++ )
//          fprintf(file," %s%s : (min,max)=(%14.7e,%14.7e) \n",
//  		(const char*)blanks(0,max(0,10-parameters.dbase.get<aString* >("componentName")[n].length())),
//  		(const char*)parameters.dbase.get<aString* >("componentName")[n],
//  		uMin(n),uMax(n));

      // *wdh* 030317 fprintf(file," maximum divergence on all interior points: divMax = %e \n",divMax);
      
    }
    

    // output results to the check file
    fprintf(checkFile,"%8.1e %i  ",t,numberOfComponents);
    for( n=0; n<numberOfComponents; n++ )
    {
      real uMinCheck = fabs(uMin(n))<checkFileCutoff(n) ? 0. : uMin(n);
      real uMaxCheck = fabs(uMax(n))<checkFileCutoff(n) ? 0. : uMax(n);
      fprintf(checkFile,"%i %8.1e %10.3e  ",n,uMinCheck,uMaxCheck);
    }
    fprintf(checkFile,"\n");

  }


  if( true )
    checkArrayIDs(" printTimeStepInfo: done");
 

  checkArrays(sPrintF(" printTimeStepInfo (t=%9.3e)",t));  

  fflush(parameters.dbase.get<FILE* >("logFile"));
  fflush(0);

}
