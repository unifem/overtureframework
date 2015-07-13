#include "Cgsm.h"
#include "SmParameters.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "Oges.h"

// void Cgsm::
// displayBoundaryConditions(FILE *file /* = stdout */)
// {
// //===================================================================================
// // /Description:
// //   Print names for boundary conditions
// //
// // /file (input) : write to this file.
// //===================================================================================
//   assert( file!=NULL );

//   int maxNameLength=3;
//   int grid;
//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//     maxNameLength=max( maxNameLength,cg[grid].getName().length());

//   char buff[80];
//   sPrintF(buff," %%4i: %%%is     %%i    %%i    %%3i :",maxNameLength); // build a format string


//   aString blanks="                                                                           ";
//   fPrintF(file," grid   name%s side axis    boundary condition and name\n",
//            (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
//   fPrintF(file," ----   ----%s ---- ----    ---------------------------\n",
//            (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
//   for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
//   {
//     for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
//     for( int side=Start; side<=End; side++ )
//     {
//       int bc=cg[grid].boundaryCondition()(side,axis);
//       fPrintF(file,buff,grid,(const char *)cg[grid].getName(),side,axis,bc);

//       if( bc > 0 && bc<SmParameters::numberOfBCNames)
//         fPrintF(file," %s \n",(const char*)SmParameters::bcName[bc]);
//       else if( bc==0 )
//         fPrintF(file," %s \n","none");
//       else if( bc<0 )
//         fPrintF(file," %s \n","periodic");
//       else
//         fPrintF(file," %s \n","unknown");
//     }
//   }
// }

void Cgsm::
computeNumberOfStepsAndAdjustTheTimeStep(const real & t,
                                         const real & tFinal,
					 const real & nextTimeToPlot, 
					 int & numberOfSubSteps, 
					 real & dtNew,
                                         const bool & adjustTimeStep /* = TRUE */ )
// =====================================================================================
// /Description:
// determine how many steps we should take to reach the next output time, but do
// not take more than `maximumStepsBetweenComputingDt'
//
// /nextTimeToPlot (input):  
// /numberOfSteps (output): Take this many sub-steps
// /dtNew (input/output) : On input this is the time step. On output this value may be changed
//     if adjustTimeStep=TRUE.
// /adjustTimeStep (input) : if TRUE alter the time step to exactly reach the nexTimeToPrint.
//\end{CompositeGridSolverInclude.tex}  
// =====================================================================================
{
  real & cfl = parameters.dbase.get<real>("cfl");
  real & tPlot = parameters.dbase.get<real>("tPrint");
  int & debug = parameters.dbase.get<int >("debug");

  // printF("--SM-- computeNumberOfStepsAndAdjustTheTimeStep: t=%9.3e nextTimeToPlot=%9.3e dt=%9.3e dtNew=%9.3e"
  //        " adjustTimeStep=%i\n", t,nextTimeToPlot,dt,dtNew,(int)adjustTimeStep);

  if ( dtNew<=REAL_MIN && tFinal<=REAL_MIN )
  { // kkc I use dt=0 to test the interpolation in the dsi scheme
    if ( adjustTimeStep ) dtNew=0.;
    return;
  }
  else if( dtNew<=0. )
  {
    printF("\n\ncomputeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e\n",dtNew);
    printF(" t=%e, tFinal=%e, nextTimeToPlot=%e, tPlot=%e \n",t,tFinal,nextTimeToPlot,tPlot);
    OV_ABORT("error");
  }
  
  real timeInterval=min(tFinal,nextTimeToPlot)-t;
  if(  timeInterval<=0. )
    timeInterval+=tPlot;
  assert( timeInterval>0. );

  if( timeInterval/dtNew > INT_MAX ) 
  {
    printF("computeNumberOfStepsAndAdjustTheTimeStep:ERROR: time step too small? dtNew=%e, timeInterval=%e \n",dtNew,
           timeInterval);
    printF(" t=%e, tFinal=%e, nextTimeToPlot=%e, tPrint=%e \n",t,tFinal,nextTimeToPlot,tPlot);
    OV_ABORT("error");
  }
  
  numberOfSubSteps=max(1,int(timeInterval/dtNew+.9999));   // used to be +.5 or +1.

  if( adjustTimeStep )
  {
    //   const int maximumStepsBetweenComputingDt=INT_MAX;
    int & maximumStepsBetweenComputingDt= parameters.dbase.get<int>("maximumStepsBetweenComputingDt");
    
    if( numberOfSubSteps > maximumStepsBetweenComputingDt )
    {
      // no need to adjust dt in this case since we will recompute dt anyway
      numberOfSubSteps=maximumStepsBetweenComputingDt;
    }
    else 
    {
      dtNew=timeInterval/numberOfSubSteps;
      if( debug & 2 )
	printF("--SM-- computeNum... adjust time step: timeInterval=%e, numberOfSubSteps=%i, dtNew=%e\n",
	       timeInterval,numberOfSubSteps,dtNew);
    }
  
    if( debug & 4 )
      printF("--SM-- compute..Step: numberOfSubSteps=%i maximumStepsBetweenComputingDt=%i dtNew=%9.3e\n",
	     numberOfSubSteps,maximumStepsBetweenComputingDt,dtNew);
  }

  if( dtNew<0. )
  {
    printF("computeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e, numberOfSubSteps=%i\n",dtNew,
           numberOfSubSteps);
    printF(" t=%e, tFinal=%e, nextTimeToPlot=%e, tPrint=%e timeInterval=%e\n",t,tFinal,nextTimeToPlot,
            tPlot,timeInterval);
    OV_ABORT("error");
  }

}

int Cgsm::
outputResultsAfterEachTimeStep( int current, real t, real dt, int stepNumber )
// ===================================================================================
// /Description:
//     Save any results that need to be output at every time step:
//          (1) values at probes.
// ===================================================================================
{
  const bool okToOutput = myid<=0;
  
  if( parameters.dbase.get<int >("allowUserDefinedOutput") )
    userDefinedOutput( gf[current], stepNumber );

  // output to any probe files *new* way 
  outputProbes( gf[current], stepNumber );


  if( probes.size(1)>0 && (stepNumber % frequencyToSaveProbes)==0 )
  {
    // output probe values
    if( probeFile==NULL && okToOutput )
    {
      // remove leading blanks in name
      int i=0, len=probeFileName.length();
      while( i<len && probeFileName[i]==' ' ) i++;
      probeFileName=probeFileName(i,len-1);
      if( okToOutput ) printF("Opening the probe file named [%s]\n",(const char*)probeFileName);
      probeFile=fopen((const char*)probeFileName,"w" ); 
    }
    
    // Write probe data in the format
    //     t_1 ex_1 ey_1 hz_1
    //     t_2 ex_2 ey_2 hz_2
    //     t_3 ex_3 ey_3 hz_3
    //       ...
    const int numberOfProbes=probes.size(1);
    if( okToOutput ) fPrintF(probeFile,"%e ",t);
    IntegerArray uprobeLoc(1);
    RealArray uprobeVal(1,3);

    for( int i=0; i<numberOfProbes; i++ )
    {
      int grid=probeGridLocation(3,i);
      assert( grid>=0 && grid<cg.numberOfComponentGrids() );
      realMappedGridFunction & u =gf[current].u[grid];

      const int i1=probeGridLocation(0,i);
      const int i2=probeGridLocation(1,i);
      const int i3=probeGridLocation(2,i);
      
      if ( cg[grid].getGridType()==MappedGrid::structuredGrid )
      {
// 	if( cg.numberOfDimensions()==2 )
// 	{
// 	  real uex=u(i1,i2,i3,ex), uey=u(i1,i2,i3,ey), uhz=u(i1,i2,i3,hz);
// 	  if( okToOutput ) fPrintF(probeFile,"%e %e %e ",uex,uey,uhz);
// 	}
// 	else
// 	{
// 	  real uex=u(i1,i2,i3,ex), uey=u(i1,i2,i3,ey), uez=u(i1,i2,i3,ez);
// 	  if( okToOutput ) fPrintF(probeFile,"%e %e %e ",uex,uey,uez);
// 	}
      }
      else
      {
	uprobeLoc(0) = i1;
// 	reconstructDSIAtEntities(t, HField, uprobeLoc, u, uprobeVal);
	if ( cg.numberOfDimensions()==2 && okToOutput)
	  fPrintF(probeFile,"%e ",uprobeVal(0));
	else if ( okToOutput )
	  fPrintF(probeFile,"%e %e %e ",uprobeVal(0),uprobeVal(1),uprobeVal(2));
	      
      }
      
      
    }
    if( okToOutput ) fPrintF(probeFile,"\n");
    
  }

  return 0;
}

int Cgsm::
outputResults( int current, real t, real dt )
// ===================================================================================
// /Description:
//     Save any results after time intervals of tPlot
// *************************************************************
// write to the check file for regression and convergence tests
// *************************************************************
// 
// ===================================================================================
{

  FILE *& checkFile =parameters.dbase.get<FILE* >("checkFile");
  FILE *& logFile   =parameters.dbase.get<FILE* >("logFile");

  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");

  const int numberOfDimensions=cg.numberOfDimensions();
  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");

  const int & u1c = parameters.dbase.get<int >("u1c");
  const int & u2c = parameters.dbase.get<int >("u2c");
  const int & u3c = parameters.dbase.get<int >("u3c");

  const int & v1c =  parameters.dbase.get<int >("v1c");
  const int & v2c =  parameters.dbase.get<int >("v2c");
  const int & v3c =  parameters.dbase.get<int >("v3c");
  const bool saveVelocities= v1c>=0 ;

  const int s11c = parameters.dbase.get<int >("s11c");
  const int s12c = parameters.dbase.get<int >("s12c");
  const int s13c = parameters.dbase.get<int >("s13c");
  const int s21c = parameters.dbase.get<int >("s21c");
  const int s22c = parameters.dbase.get<int >("s22c");
  const int s23c = parameters.dbase.get<int >("s23c");
  const int s31c = parameters.dbase.get<int >("s31c");
  const int s32c = parameters.dbase.get<int >("s32c");
  const int s33c = parameters.dbase.get<int >("s33c");
  const bool saveStress = s11c >=0 ;

  Range C=numberOfComponents; 

  const int numberToOutput= numberOfComponents + 1 + int(saveVelocities) + int(saveStress); 
  
  if( maximumError.getLength(0)>= numberOfComponents && myid==0 )
  {
    fPrintF(checkFile,"%9.2e %i  ",t,numberToOutput);
    int c,cc=0;
    real err,uNorm;

    // first output :   *wdh* 090904
    //   1. max error in all displacements
    //   2. max error in all velocities (FOS)
    //   3. max error in all stresses   (FOS)
    err = (numberOfDimensions == 2 ? max(maximumError(uc),maximumError(vc)) : 
                                      max(maximumError(uc),maximumError(vc),maximumError(wc)) );
    uNorm= (numberOfDimensions == 2 ? max(solutionNorm(uc),solutionNorm(vc)) : 
                                      max(solutionNorm(uc),solutionNorm(vc),solutionNorm(wc)) );

    fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,err,uNorm); cc++;
    if( saveVelocities )
    {
      err = (numberOfDimensions == 2 ? max(maximumError(v1c),maximumError(v2c)) : 
	                                max(maximumError(v1c),maximumError(v2c),maximumError(v3c)) );
      uNorm= (numberOfDimensions == 2 ? max(solutionNorm(v1c),solutionNorm(v2c)) : 
	                                max(solutionNorm(v1c),solutionNorm(v2c),solutionNorm(v3c)) );
      fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,err,uNorm); cc++;
    }
    if( saveStress )
    {
      if( numberOfDimensions == 2 )
      {
        err = max( maximumError(s11c),maximumError(s12c),maximumError(s21c),maximumError(s22c) );
        uNorm= max( solutionNorm(s11c),solutionNorm(s12c),solutionNorm(s21c),solutionNorm(s22c) );
      }
      else
      {
        err = max( 
	  max(maximumError(s11c),maximumError(s12c),maximumError(s13c)),
	  max(maximumError(s21c),maximumError(s22c),maximumError(s23c)),
	  max(maximumError(s31c),maximumError(s32c),maximumError(s33c)) );
	
        uNorm = max( 
	  max(solutionNorm(s11c),solutionNorm(s12c),solutionNorm(s13c)),
	  max(solutionNorm(s21c),solutionNorm(s22c),solutionNorm(s23c)),
	  max(solutionNorm(s31c),solutionNorm(s32c),solutionNorm(s33c)) );
      }
      fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,err,uNorm); cc++;
    }
    

    for( c=0; c<numberOfComponents; c++ )
    {
      err = maximumError(c); // error(c) > checkFileCutoff(c) ? error(c) : 0.;
      uNorm = solutionNorm(c); 
      // if( uNorm<checkFileCutoff(c) ) uNorm=0.;
      fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,err,uNorm); cc++;
    }

//     err=divUMax/max(REAL_MIN*100.,gradUMax);
//     uNorm=gradUMax;
//     fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,err,uNorm); cc++;
    fPrintF(checkFile,"\n");

    // *************************************************************
    if( computeEnergy )
    {
      maximumError(numberOfComponents )=totalEnergy;
      maximumError(numberOfComponents+1)=totalEnergy-initialTotalEnergy;
    }
    
    saveSequenceInfo( t, maximumError );

  }
  else if( solutionNorm.getLength(0)>= (numberOfComponents-1) && myid<=0 )
  {
    // Errors are not being computed. 

    // Save solution norms to the check file *wdh* 101017
    fPrintF(checkFile,"%9.2e %i  ",t,numberOfComponents);
    int cc=0;
    for( int c=C.getBase(); c<=C.getBound(); c++ )
    {
      real uNorm = solutionNorm(c); 
      fPrintF(checkFile,"%i %9.2e %10.3e  ",cc,uNorm,uNorm); cc++;
    }
    fPrintF(checkFile,"\n");

    // output results to stdio and the logFile
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : stdout;

      fPrintF(output,"-->t=%10.4e dt=%8.2e |div(U)|=%8.2e |div(U)|/|grad(U)|=%8.2e, |grad(U)|=%8.2e, max(u):[",
	      t,dt,divUMax,divUMax/max(REAL_MIN*100.,gradUMax),gradUMax);
      for( int c=C.getBase(); c<=C.getBound(); c++ )
	fPrintF(output,"%8.2e,",solutionNorm(c));

      fPrintF(output,"]\n");
	  
    }
  }
  else
  {
    printP(" ******* outputResults WARNING: solutionNorm not defined! solutionNorm.getLength(0)=%i ***********\n",
           solutionNorm.getLength(0));
  }
  


  // compute the current energy (if computeEnergy==true)
  getEnergy( current, t, dt );

  return 0;
}

//======================================================================================================
///\brief Solve the equations
//======================================================================================================
int Cgsm::
solve()
{
  real time0=getCPU();
  parameters.dbase.get<real>("cpuInitial")=time0;
  
  GenericGraphicsInterface &gi = *parameters.dbase.get<GenericGraphicsInterface* >("ps");
  
  FILE *& logFile   =parameters.dbase.get<FILE* >("logFile");
  const int & debug = parameters.dbase.get<int >("debug");

  SmParameters::PDEModel & pdeModel = parameters.dbase.get<SmParameters::PDEModel>("pdeModel");
  SmParameters::PDEVariation & pdeVariation = parameters.dbase.get<SmParameters::PDEVariation>("pdeVariation");
  SmParameters::TimeSteppingMethodSm & timeSteppingMethod = 
                parameters.dbase.get< SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");
  RealArray & timing = parameters.dbase.get<RealArray >("timing");
  const int pdeTypeForGodunovMethod = parameters.dbase.get<int>("pdeTypeForGodunovMethod");   // 0=linear, 2=SVK ? 

  const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
  const int & uc =  parameters.dbase.get<int >("uc");
  const int & vc =  parameters.dbase.get<int >("vc");
  const int & wc =  parameters.dbase.get<int >("wc");
  const int & rc =  parameters.dbase.get<int >("rc");
  const int & tc =  parameters.dbase.get<int >("tc");

  Range all;
  if( checkErrors )//&& cg[0].getGridType()==MappedGrid::structuredGrid )
  {
    cgerrp = new realCompositeGridFunction;
    cgerrp->updateToMatchGrid(cg,all,all,all,numberOfComponents);

//     cgerrp->setName("u error",uc);
//     cgerrp->setName("v error",vc);
//     if( cg.numberOfDimensions()==3)
//       cgerrp->setName("w error",wc);
    for( int n=0; n<numberOfComponents; n++ )
    {
      cgerrp->setName(gf[current].u.getName(n)+"Error",n);
    }
    
  }
  
  real & cfl = parameters.dbase.get<real>("cfl");
  real & tFinal = parameters.dbase.get<real>("tFinal");
  real & tPlot = parameters.dbase.get<real>("tPrint");

  real t = parameters.dbase.get<real>("tInitial");
  real dtb2,dtNew;
  real & dt= deltaT;
  int maxNumberOfTimeSteps=INT_MAX;
  int numberOfSubSteps;
  
  current=0;

  int next=1;
  real nextTimeToPlot=0.;
  if ( tPlot<=0 ) 
    tPlot = tFinal;

  // get the initial time step 
  getTimeStep( gf[current] );

  // *new* way *wdh* 2015/07/11
  bool adjustTimeStep=true;
  nextTimeToPlot=min(int(t/tPlot+.5)*tPlot+tPlot,tFinal);   //  ...new time to print:
  dtNew=dt;
  computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPlot,numberOfSubSteps,dtNew,adjustTimeStep );
  dt=dtNew;
  adjustTimeStep=false;
    
  nextTimeToPlot=0.; // reset so we plot at t=0
    
    // // *old way*
    // // adjust the time step so we reach tPlot exactly
    // //   dt=deltaT is computed in computeTimeStep in setup

    // // We want dt*numStepsToPlot = tPlot
    // const real dt0=dt;
    // int numStepsToPlot = int(std::ceil( tPlot/dt ));// kkc is this what you mean?int(tPlot/dt+.999999);
    // // we choose the number of steps so that we exactly reach times that are multiples of tPlot
    // int numSteps= int(ceil(tFinal/tPlot))*numStepsToPlot; 

    // dt= numSteps ? tFinal/numSteps : 0 ;
    // assert( dt<= dt0 );
    // printF("--solve-- dt=%9.3e, tPlot=%f, tPlot/dt=%f numStepsToPlot=%i numSteps=%i \n",dt,
    // 	   tPlot,tPlot/dt,numStepsToPlot,numSteps);


  // For linear-elasticity this next method will compute the solution at t-dt
  updateForNewTimeStep( gf[current],dt );

  dtNew=dt;

  int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");
  if( globalStepNumber<0 )
  { // we need to apply the BC's at the start for some cases (e.g. hemp, boundary-data )
    printF(" >>>> solve: applyBC's at start, globalStepNumber=%i, t=%8.2e\n",globalStepNumber,t);

    if( debug & 4 )
      getErrors( current, gf[current].t,dt,
                 sPrintF("\n ******** Solve : errors BEFORE applyBC at start, t=%9.3e ******\n", gf[current].t));

    int option=0;
    applyBoundaryConditions( option, dt, current, current );
    // interpolateAndApplyBoundaryConditions( gf[current] );

    if( debug & 4 )
      getErrors( current, gf[current].t,dt,
                 sPrintF("\n ******** Solve : errors AFTER applyBC at start, t=%9.3e ******\n", gf[current].t));
  }


  plotOptions=1;
  if( !gi.graphicsIsOn() && !gi.readingFromCommandFile() )
    plotOptions=0;
  
  char buff[80];           // buffer for sprintf
  numberOfStepsTaken=0;
  int iPrint=0;
  
  outputResultsAfterEachTimeStep( current,t,dt,0 );

  if( !parameters.dbase.get<DataBase >("modelData").has_key("initializeAdvance") )
    parameters.dbase.get<DataBase >("modelData").put<int>("initializeAdvance",true);
  int & init=parameters.dbase.get<DataBase >("modelData").get<int>("initializeAdvance");
  
  for( int stepNumber=0; dt>REAL_MIN && stepNumber<maxNumberOfTimeSteps; stepNumber++ )   // take some time steps
  {

    if( t+.5*dt>nextTimeToPlot )  // plot solution 
    {

      // output results, compute errors, save check file:
      real cpuTime =getCPU();
      printTimeStepInfo( stepNumber, t, cpuTime );

      if( frequencyToSaveInShowFile>0 && (iPrint % frequencyToSaveInShowFile == 0) )
        saveShow( gf[current] );  // save the current solution in the show file
      // saveShow( current,t,dt );  // save the current solution in the show file
      
      int finished = plot(current, t, dt );
      if( finished ) break;
      if( t >tFinal-.5*dt )
      {
	// we are done (unless tFinal is increased in the next call to plot). plot solution at final time
	//	if( plotOptions & 1 )
	{
	  plotOptions=1;  // plot and wait 
	  plot(current, t, dt );
	}
        if( t >tFinal-.5*dt ) // tFinal may have been increased, so check again
	{ 
	  finished=true;
	  break;
	}
      }

      nextTimeToPlot=min(int(t/tPlot+.5)*tPlot+tPlot,tFinal);   //  ...new time to print:

      iPrint++;
    }

    // NOTE: adjustTimeStep is set to true if the cfl number or tPlot  is changed in plot()
    bool & adjustTimeStep= parameters.dbase.get<bool>("adjustTimeStep");
    if( parameters.isAdaptiveGridProblem() )
       adjustTimeStep=true;
    else if( stepNumber>0  && pdeVariation==SmParameters::hemp ) 
       adjustTimeStep=true;
    else if( stepNumber>0  && pdeVariation==SmParameters::godunov && pdeTypeForGodunovMethod>0  ) 
       adjustTimeStep=true;
    
    
    const real dtOld = dt;
    if( adjustTimeStep )
    {
      parameters.dbase.get<bool>("recomputeDt")=true;  // why is this needed?

      getTimeStep( gf[current] );
      dtNew = dt;
      dt = dtOld;  // we only change dt below if it differs enough from dtNew
    
      computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPlot,numberOfSubSteps,dtNew,adjustTimeStep );

      if( fabs( dtOld - dtNew) > dtOld*REAL_EPSILON*100. )
      {
	if( debug & 1 )
	  printF(" *new* time step: step=%i, dt=%9.3e (old=%9.3e) numberOfSubSteps=%i, dt*numberOfSubSteps=%9.3e\n",
		 globalStepNumber,dtNew,dtOld, numberOfSubSteps,numberOfSubSteps*dtNew );

	dt=dtNew;  
      }
      else
      {
	printF("--SOLVE-- adjustTimeStep: dt has NOT changed: dtOld=%9.3e dtNew=%9.3e\n",dt,dtOld,dtNew);
      }
    }
    
    
    adjustTimeStep=false;
    

    real time1=getCPU();

      
    if( timeSteppingMethod==SmParameters::defaultTimeStepping ||
        timeSteppingMethod==SmParameters::modifiedEquationTimeStepping)
    {
      for( int subStep=0; subStep<numberOfSubSteps; subStep++)
      {

	if( pdeModel==SmParameters::linearElasticity )
	{
	  advance( current,t,dt );
	}
	else
	{
	  OV_ABORT("Cgsm::solve:ERROR: unknown pdeModel");
	}
	t+=dt;
	current = next;
	next= (next+1) % numberOfTimeLevels;

	outputResultsAfterEachTimeStep( current,t,dt,stepNumber );

	numberOfStepsTaken++;
      }
    }
    else if( timeSteppingMethod==SmParameters::forwardEuler ||
             timeSteppingMethod==SmParameters::improvedEuler ||
             timeSteppingMethod==SmParameters::adamsBashforth2 ||
	     timeSteppingMethod==SmParameters::adamsPredictorCorrector2 ||
	     timeSteppingMethod==SmParameters::adamsPredictorCorrector4 )
    {
      for( int subStep=0; subStep<numberOfSubSteps; subStep++)
      {
	advanceMethodOfLines(  current, t, dt );
	t+=dt;
	current = next;
	next= (next+1) % numberOfTimeLevels;

	outputResultsAfterEachTimeStep( current,t,dt,stepNumber );
      
	numberOfStepsTaken++;
      
	// advanceAdamsPredictorCorrectorNew( t,dt, numberOfSubSteps,init,stepNumber );
	// stepNumber+=numberOfSubSteps; numberOfStepsTaken+=numberOfSubSteps; 
	// printF("Cgsm:solve: after advanceAdamsPredictorCorrectorNew: t=%8.2e\n",t);
	// Overture::abort("done");
      }
    }
    else
    {
      printF("Cgsm:advance:ERROR: unknown timeSteppingMethod=%i\n",(int)timeSteppingMethod);
    }
    

    
    
  }

  if ( !numberOfStepsTaken && dt<REAL_MIN )
  {
    real cpuTime =getCPU();
    printTimeStepInfo( globalStepNumber, t, cpuTime );

    // *wdh* 101017 getErrors(current,t,dt,sPrintF("\n *** solve: errors at t=%9.3e ****\n",t) );
    // *wdh* 101017outputResults(current,t,dt);
  }
    
  saveSequencesToShowFile(); // 100126 

  timing(parameters.dbase.get<int>("timeForAdvance"))+=getCPU()-time0;
  timing(parameters.dbase.get<int>("totalTime"))+=getCPU()-time0;
  printF(" solve: timing(SmParameters::totalTime)=%8.2e\n",timing(parameters.dbase.get<int>("totalTime")));
  
  
  printStatistics();
  
  return 0;
}
