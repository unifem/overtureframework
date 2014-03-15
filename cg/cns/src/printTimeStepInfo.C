#include "Cgcns.h"
#include "ParallelUtility.h"
#include "App.h"
#include "CnsParameters.h"

// ===================================================================================================================
/// \brief Print time-step information about the current solution in a nicely formatted way.
///
/// \param step (input) : current step number.
/// \param t (input) : time.
/// \param cpuTime (input) : current cpu time.
/// 
// ==================================================================================================================
void Cgcns::
printTimeStepInfo( const int & step, const real & t, const real & cpuTime )
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

  // printP("printTimeStepInfo: step=%i, dt=%e\n",step,dt);

  int n;
  RealArray error(numberOfComponents+5);  

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

  real curMem=ParallelUtility::getMaxValue(Overture::getCurrentMemoryUsage());
  real maxMem=ParallelUtility::getMaxValue(Overture::getMaximumMemoryUsage());

  if( parameters.dbase.get<int >("myid")!=0 ) return; // only print on processor 0

  aString blanks="                            ";
  if( parameters.dbase.get<bool >("twilightZoneFlow") || parameters.dbase.get<Parameters::KnownSolutionsEnum >("knownSolution")!=CnsParameters::noKnownSolution )
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

      if( numberOfComponents<5 )
      {
	if( step<=2 || ((step-1) % 10 == 0) )
	{
	  if( !parameters.isSteadyStateSolver() )
            fprintf(file,"     t ");
          else
            fprintf(file,"     it");

	  for( n=0; n<numberOfComponents; n++)
	    fprintf(file,"   err(%s)",(const char*)parameters.dbase.get<aString* >("componentName")[n]);
	  fprintf(file,"    uMax     dt       cpu  (cur,max)-mem (Gb)\n");
	}
	if( !parameters.isSteadyStateSolver() )
          fprintf(file," %7.3f",t);
        else
          fprintf(file,"%10i",parameters.dbase.get<int >("globalStepNumber")+1);
	for( n=0; n<numberOfComponents; n++)
	  fprintf(file," %8.2e",error(n));
	fprintf(file," %8.2e %8.2e %8.2e (%8.2g,%8.2g)\n",uvMax,dt,cpu,curMem,maxMem);
      }
      else
      {
	if( !parameters.isSteadyStateSolver() )
	  fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds, (cur,max)-mem=(%g,%g) (Mb) (%i steps)\n",
                  t,dt,cpu,curMem,maxMem,parameters.dbase.get<int >("globalStepNumber")+1);
	else
	  fprintf(file," >>> it= %10i, dt =%9.2e, cpu =%9.2e seconds, (cur,max)-mem=(%g,%g) (Mb) \n",
                  parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu,curMem,maxMem);

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
        fprintf(file," >>> t = %10.3e, dt =%9.2e, cpu =%9.2e seconds, (cur,max)-mem=(%g,%g) (Mb) (%i steps)\n",
                t,dt,cpu,curMem,maxMem,parameters.dbase.get<int >("globalStepNumber")+1);
      else
        fprintf(file," >>> it=   %10i, dt =%9.2e, cpu =%9.2e seconds, (cur,max)-mem=(%g,%g) (Mb) \n",
                parameters.dbase.get<int >("globalStepNumber")+1,dt,cpu,curMem,maxMem);
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

      if( parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleNavierStokes &&
	  parameters.dbase.get<CnsParameters::PDEVariation >("pdeVariation")==CnsParameters::conservativeGodunov )
      {
	if(parameters.dbase.get<RealArray >("statistics").getLength(0)>0 )
	{
	  fprintf(file,"Max sub-cycles: ");
	  for( int l=0; l<min(parameters.dbase.get<RealArray >("statistics").getLength(0),solution.cg.numberOfRefinementLevels()); l++ )
	  {
            int maxCycles=(int)(parameters.dbase.get<RealArray>("statistics")(l+10)+.5);
	    fprintf(file," level %i =%i, ",l,maxCycles);
	  }
	  fprintf(file,"\n");

          if( io!=1 )
	  { // no cpu times in file 1
            real tflux  =parameters.dbase.get<RealArray>("statistics")(0);
	    real tslope =parameters.dbase.get<RealArray>("statistics")(1);
	    real tsource=parameters.dbase.get<RealArray>("statistics")(2);
	    fprintf(file,"time for flux=%8.2e, slope=%8.2e, source = %8.2e \n",tflux,tslope,tsource);
	  }
	  
	}
      
      }

      if( parameters.dbase.get<CnsParameters::PDE >("pde")==CnsParameters::compressibleMultiphase )
      {
	if(parameters.dbase.get<RealArray >("statistics").getLength(0)>0 )
	{
// 	  fprintf(file,"Max sub-cycles: ");
// 	  for( int l=0; l<min(parameters.dbase.get<RealArray >("statistics").getLength(0),solution.cg.numberOfRefinementLevels()); l++ )
// 	  {
//             int maxCycles=(int)(parameters.dbase.get<RealArray>("statistics")(l+10)+.5);
// 	    fprintf(file," level %i =%i, ",l,maxCycles);
// 	  }
// 	  fprintf(file,"\n");

          if( io!=1 ) // no cpu times in file 1
	  { 
            real tflux   =parameters.dbase.get<RealArray>("statistics")(0);
	    real tslope  =parameters.dbase.get<RealArray>("statistics")(1);
	    real tsource =parameters.dbase.get<RealArray>("statistics")(2);
            real tmiddles=parameters.dbase.get<RealArray>("statistics")(3);
            real tmiddleg=parameters.dbase.get<RealArray>("statistics")(4);
            real tcouple =parameters.dbase.get<RealArray>("statistics")(5);
            real tfluxes =parameters.dbase.get<RealArray>("statistics")(5);
	    fprintf(file,"time for flux=%8.2e, slope=%8.2e, source = %8.2e \n",tflux,tslope,tsource);
	    fprintf(file,"time for middle(solid)=%8.2e, middle(gas)=%8.2e \n",tmiddles,tmiddleg);
	    fprintf(file,"time for couple=%8.2e, fluxes=%8.2e \n",tcouple,tfluxes);
	  }
	  
	}
      
      }


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
}
