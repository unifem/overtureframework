#include "Maxwell.h"
#include "PlotStuff.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "ParallelUtility.h"
#include "App.h"
#include "Ogshow.h"
#include "DomainSolver.h"


void Maxwell::
displayBoundaryConditions(FILE *file /* = stdout */)
{
//===================================================================================
// /Description:
//   Print names for boundary conditions
//
// /file (input) : write to this file.
//===================================================================================
  assert( file!=NULL );
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

  int maxNameLength=3;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    maxNameLength=max( maxNameLength,cg[grid].getName().length());

  char buff[80];
  sPrintF(buff," %%4i: %%%is     %%i    %%i    %%3i :",maxNameLength); // build a format string


  aString blanks="                                                                           ";
  fPrintF(file," grid   name%s side axis    boundary condition and name\n",
           (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
  fPrintF(file," ----   ----%s ---- ----    ---------------------------\n",
           (const char *)blanks(0,min(maxNameLength-3,blanks.length()-1)));
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    for( int axis=axis1; axis<cg.numberOfDimensions(); axis++ )
    for( int side=Start; side<=End; side++ )
    {
      int bc=cg[grid].boundaryCondition()(side,axis);
      fPrintF(file,buff,grid,(const char *)cg[grid].getName(),side,axis,bc);

      if( bc > 0 && bc<numberOfBCNames)
        fPrintF(file," %s \n",(const char*)bcName[bc]);
      else if( bc==0 )
        fPrintF(file," %s \n","none");
      else if( bc<0 )
        fPrintF(file," %s \n","periodic");
      else
        fPrintF(file," %s \n","unknown");
    }
  }
}

void Maxwell::
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
  if ( dtNew<=REAL_MIN && tFinal<=REAL_MIN )
    { // kkc I use dt=0 to test the interpolation in the dsi scheme
      if ( adjustTimeStep ) dtNew=0.;
       return;
    }
  else if( dtNew<=0. )
  {
    printF("\n\ncomputeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e\n",dtNew);
    printF(" t=%e, tFinal=%e, nextTimeToPlot=%e, tPlot=%e \n",t,tFinal,nextTimeToPlot,tPlot);
    Overture::abort("error");
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
    Overture::abort("error");
  }
  
  numberOfSubSteps=max(1,int(timeInterval/dtNew+.9999));   // used to be +.5 or +1.

  const int maximumStepsBetweenComputingDt=INT_MAX;
    
  if( numberOfSubSteps > maximumStepsBetweenComputingDt )
  {
    // no need to adjust dt in this case since we will recompute dt anyway
    numberOfSubSteps=maximumStepsBetweenComputingDt;
  }
  else if( adjustTimeStep )
  {
    dtNew=timeInterval/numberOfSubSteps;
    if( true || debug & 2 )
      printF(" ---- adjust time step: timeInterval=%e, numberOfSubSteps=%i, dtNew=%e\n",
	     timeInterval,numberOfSubSteps,dtNew);
  }

  if( dtNew<0. )
  {
    printF("computeNumberOfStepsAndAdjustTheTimeStep:ERROR: dtNew<=0., dtNew=%e, numberOfSubSteps=%i\n",dtNew,
           numberOfSubSteps);
    printF(" t=%e, tFinal=%e, nextTimeToPlot=%e, tPrint=%e timeInterval=%e\n",t,tFinal,nextTimeToPlot,
            tPlot,timeInterval);
    Overture::abort("error");
  }

}

int Maxwell::
outputResultsAfterEachTimeStep( int current, real t, real dt, int stepNumber, real nextTimeToPlot )
// ===================================================================================
// /Description:
//     Save any results that need to be output at every time step:
//          (1) values at probes.
// ===================================================================================
{
  const bool okToOutput = myid<=0;
  
  // **NEW WAY**
  // output to any probe files
  DomainSolver::outputProbes( parameters, gf[current], stepNumber );

  // **OLD WAY FOR PROBES: 
  const int & probeFileFrequency = parameters.dbase.get<int>("probeFileFrequency");
  if( probes.size(1)>0 && (stepNumber % probeFileFrequency)==0 )
  {
    assert( cgp!=NULL );
    CompositeGrid & cg= *cgp;

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
      realMappedGridFunction & u =mgp!=NULL ? fields[current] : getCGField(HField,current)[grid];//cgfields[current][grid];

      const int i1=probeGridLocation(0,i);
      const int i2=probeGridLocation(1,i);
      const int i3=probeGridLocation(2,i);
      
      if ( cg[grid].getGridType()==MappedGrid::structuredGrid )
      {
	if( cg.numberOfDimensions()==2 )
	{
	  real uex=u(i1,i2,i3,ex), uey=u(i1,i2,i3,ey), uhz=u(i1,i2,i3,hz);
	  if( okToOutput ) fPrintF(probeFile,"%e %e %e ",uex,uey,uhz);
	}
	else
	{
	  real uex=u(i1,i2,i3,ex), uey=u(i1,i2,i3,ey), uez=u(i1,i2,i3,ez);
	  if( okToOutput ) fPrintF(probeFile,"%e %e %e ",uex,uey,uez);
	}
      }
      else
      {
	uprobeLoc(0) = i1;
	reconstructDSIAtEntities(t, HField, uprobeLoc, u, uprobeVal);
	if ( cg.numberOfDimensions()==2 && okToOutput)
	  fPrintF(probeFile,"%e ",uprobeVal(0));
	else if ( okToOutput )
	  fPrintF(probeFile,"%e %e %e ",uprobeVal(0),uprobeVal(1),uprobeVal(2));
	      
      }
      
      
    }
    if( okToOutput ) fPrintF(probeFile,"\n");
    
  }


  if( plotIntensity || plotHarmonicElectricFieldComponents )
  {
    computeIntensity(current,t,dt,stepNumber,nextTimeToPlot);
  }


  return 0;
}



int Maxwell::
outputResults( int current, real t, real dt )
// ===================================================================================
// /Description:
//     Save any results after time intervals of tPlot
// 
// ===================================================================================
{
  // *************************************************************
  // write to the check file for regression and convergence tests
  // *************************************************************

  // kkc fix range to work with staggered schemes
  int base = ex;
  int bound = (method==nfdtd  || method==yee ) ? hz : max(ey,ez) + hz + 1;
  if( method==sosup ) 
    bound=cgfields[0][0].getBound(3);
  Range C(base,bound);  // fix this 

  int numberOfComponents= C.length(); 
  int numberToOutput= numberOfComponents+1; // include div(E)
//    printF("\n ***** outputResults: maximumError.getLength(0)=%i numberOfComponents=%i\n",
//  	 maximumError.getLength(0),numberOfComponents);
  
  if( maximumError.getLength(0)>= (numberToOutput-1) && myid==0 )
  {
    fPrintF(checkFile,"%9.2e %i  ",t,numberToOutput);
    int c;
    real err,uc;
    for( c=0; c<numberToOutput-1; c++ )
    {
      err = maximumError(c); // error(c) > checkFileCutoff(c) ? error(c) : 0.;
      uc = solutionNorm(c); 
      // if( uc<checkFileCutoff(c) ) uc=0.;
      fPrintF(checkFile,"%i %9.2e %10.3e  ",c,err,uc);
    }

    c=numberToOutput-1;
    err=divEMax/max(REAL_MIN*100.,gradEMax);
    uc=gradEMax;
    if ( method==nfdtd || method==yee )
      fPrintF(checkFile,"%i %9.2e %10.3e  ",c,err,uc);
    else
      fPrintF(checkFile,"%i %9.2e %10.3e  ",c,divEMax,gradEMax); // kkc this is actually divEMax and divHMax (divH stored in gradE)
    fPrintF(checkFile,"\n");

    // *************************************************************
    if( computeEnergy )
    {
      maximumError(numberOfComponents )=totalEnergy;
      maximumError(numberOfComponents+1)=totalEnergy-initialTotalEnergy;
    }
    
    // saveSequenceInfo( t, maximumError );

  }
  else if( solutionNorm.getLength(0)>= (numberToOutput-1) && myid<=0 )
  {
    for( int fileio=0; fileio<2; fileio++ )
    {
      FILE *output = fileio==0 ? logFile : stdout;

      if ( method==nfdtd || method==yee || method==sosup )
	fPrintF(output,"-->t=%10.4e dt=%8.2e |div(E)|=%8.2e |div(E)|/|grad(E)|=%8.2e, |grad(E)|=%8.2e, max(u):[",
                t,dt,divEMax,divEMax/max(REAL_MIN*100.,gradEMax),gradEMax);
      else
	fPrintF(output,"-->t=%10.4e dt=%8.2e |div(E)|=%8.2e |div(H)|=%8.2e, max(u):[",
                t,dt,divEMax,gradEMax);
      for( int c=C.getBase(); c<=C.getBound(); c++ )
	fPrintF(output,"%8.2e,",solutionNorm(c));

      fPrintF(output,"]\n");
	  
    }
  }

  if( maximumError.getLength(0)>= (numberToOutput-1) )
  {
    saveSequenceInfo( t, maximumError );
  }
  

  // compute the current energy (if computeEnergy==true)
  getEnergy( current, t, dt );

  return 0;
}


// =========================================================================================
/// \brief Time-step Maxwell's equations.
/// 
// =========================================================================================
int Maxwell::
solve(GL_GraphicsInterface &gi )
{
  real time0=getCPU();
  
  Range all;
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;

  if( mgp!=NULL )
  {

    MappedGrid & mg = *mgp;

    if( errp==NULL && checkErrors ) 
    {
      // Create a grid function to hold the errors for plotting
      if ( method==nfdtd )
      {
	int numberOfComponents = fields[0].getLength(3);
	errp = new realMappedGridFunction[1];
	errp->updateToMatchGrid(mg,all,all,all,numberOfComponents);
	errp->setName("Ex error",ex);
	errp->setName("Ey error",ey);
	errp->setName("Hz error",hz);
      }
      else
      {
	errp = new realMappedGridFunction[2];
	if ( cg.numberOfDimensions()==2 )
	{
	  errp[0].updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,2);
	  errp[1].updateToMatchGrid(mg,GridFunctionParameters::cellCentered);
	  errp[0].setName("Ex error",ex);
	  errp[0].setName("Ey error",ey);
	  errp[1].setName("Hz error",hz);
	}
	else
	{
	  errp[0].updateToMatchGrid(mg,GridFunctionParameters::edgeCentered,3);
	  errp[0].setName("Ex error",ex);
	  errp[0].setName("Ey error",ey);
	  errp[0].setName("Ez error",ez);
	  errp[1].updateToMatchGrid(mg,GridFunctionParameters::faceCenteredAll,3);
	  errp[1].setName("Hx error",hx);
	  errp[1].setName("Hy error",hy);
	  errp[1].setName("Hz error",hz);
	}
      }
    }
  }
  else
  {
    if( checkErrors )//&& cg[0].getGridType()==MappedGrid::structuredGrid )
    {
      if( method==nfdtd || method==yee )
      {
	cgerrp = new realCompositeGridFunction [1];
	int numberOfComponents = cgfields[0][0].getLength(3);
	cgerrp->updateToMatchGrid(*cgp,all,all,all,numberOfComponents);
	
	if( cg.numberOfDimensions()==2 )
	{
	  cgerrp->setName("Ex error",ex);
	  cgerrp->setName("Ey error",ey);
	  cgerrp->setName("Hz error",hz);
	  if( dispersionModel==noDispersion )
	  {
   	    assert( numberOfComponents==3 );
	  }
	  else
	  {
            // -- dispersion components --
	    cgerrp->setName("Px error",pxc);
	    cgerrp->setName("Py error",pyc);
	    if( orderOfAccuracyInSpace==4 )
	    {
	      cgerrp->setName("Qx error",qxc);
	      cgerrp->setName("Qy error",qyc);
	    }
	    else if( orderOfAccuracyInSpace==6 )
	    {
	      cgerrp->setName("Rx error",rxc);
	      cgerrp->setName("Ry error",ryc);
	    }
	  }
	  
	}
	else
	{
	  if( solveForElectricField )
	  {
	    cgerrp->setName("Ex error",ex);
	    cgerrp->setName("Ey error",ey);
	    cgerrp->setName("Ez error",ez);
	  }
	  if( solveForMagneticField )
	  {
	    cgerrp->setName("Hx error",hx);
	    cgerrp->setName("Hy error",hy);
	    cgerrp->setName("Hz error",hz);
	  }
	  if( dispersionModel!=noDispersion )
	  {
            // -- dispersion components --
	    cgerrp->setName("Px error",pxc);
	    cgerrp->setName("Py error",pyc);
	    cgerrp->setName("Pz error",pzc);
	    if( orderOfAccuracyInSpace==4 )
	    {
	      cgerrp->setName("Qx error",qxc);
	      cgerrp->setName("Qy error",qyc);
	      cgerrp->setName("Qz error",qzc);
	    }
	    else if( orderOfAccuracyInSpace==6 )
	    {
	      cgerrp->setName("Rx error",rxc);
	      cgerrp->setName("Ry error",ryc);
	      cgerrp->setName("Rz error",rzc);
	    }
	  }
	      
	}
      }
      else if( method==sosup )
      {
	cgerrp = new realCompositeGridFunction [1];
	int numberOfComponents = cgfields[0][0].getLength(3);
	cgerrp->updateToMatchGrid(*cgp,all,all,all,numberOfComponents);
	
	if( cg.numberOfDimensions()==2 )
	{
	  assert( numberOfComponents==6 );
	  cgerrp->setName("Ex error",ex);
	  cgerrp->setName("Ey error",ey);
	  cgerrp->setName("Hz  error",hz );
	  cgerrp->setName("Ext error",ext);
	  cgerrp->setName("Eyt error",eyt);
	  cgerrp->setName("Hzt error",hzt);
	}
	else
	{
	  if( solveForElectricField )
	  {
	    cgerrp->setName("Ex error",ex);
	    cgerrp->setName("Ey error",ey);
	    cgerrp->setName("Ez error",ez);
	    cgerrp->setName("Ext error",ext);
	    cgerrp->setName("Eyt error",eyt);
	    cgerrp->setName("Ezt error",ezt);
	  }
	  if( solveForMagneticField )
	  {
	    cgerrp->setName("Hx error",hx);
	    cgerrp->setName("Hy error",hy);
	    cgerrp->setName("Hz error",hz);
	    cgerrp->setName("Hxt error",hxt);
	    cgerrp->setName("Hyt error",hyt);
	    cgerrp->setName("Hzt error",hzt);
	  }
	      
	}
      }
      else
      {
	cgerrp = new realCompositeGridFunction [2];
	if ( cg.numberOfDimensions()==2 )
	{
	  cgerrp[0].updateToMatchGrid(*cgp,GridFunctionParameters::edgeCentered,2);
	  cgerrp[1].updateToMatchGrid(*cgp,GridFunctionParameters::cellCentered);
	  cgerrp[0].setName("Ex error",ex);
	  cgerrp[0].setName("Ey error",ey);
	  cgerrp[1].setName("Hz error",hz);	  
	}
	else
	{
	  cgerrp[0].updateToMatchGrid(*cgp,GridFunctionParameters::edgeCentered,3);
	  cgerrp[1].updateToMatchGrid(*cgp,GridFunctionParameters::faceCentered,3);

	  cgerrp[0].setName("Ex error",ex);
	  cgerrp[0].setName("Ey error",ey);
	  cgerrp[0].setName("Ez error",ez);
	      
	  cgerrp[1].setName("Hx error",hx);
	  cgerrp[1].setName("Hy error",hy);
	  cgerrp[1].setName("Hz error",hz);

	}
      }
    }
    
  }
    
  
  // realMappedGridFunction & err = *errp;

  real t = 0.;
  real dtb2,dtNew;
  real & dt= deltaT;
  int maxNumberOfTimeSteps=INT_MAX;
  int numberOfSubSteps;
  
  int current=0;
  int next=1;

  // adjust the time step so we reach tPlot exactly
  //   dt=deltaT is computed in computeTimeStep in setup

  if ( tPlot<=0 ) //kkc 040304
    tPlot = tFinal;


  if( true )
  {
    // sanity check for dt 
    real dtmx = ParallelUtility::getMaxValue(dt);
    real dtmn = ParallelUtility::getMinValue(dt);
    if( dtmx!=dtmn )
    {
      printf(" ERROR: dt=%10.4e, dtmx=%10.4e dtmn=%10.4e, myid=%i \n",dt,dtmx,dtmn,myid);
      Overture::abort("Error");
    }
  }
  
  // We want dt*numStepsToPlot = tPlot
  const real dt0=dt;
  int numStepsToPlot = int(std::ceil( tPlot/dt ));// kkc is this what you mean?int(tPlot/dt+.999999);
  // int numSteps = int(tFinal/dt+1.);
  // numSteps = (numSteps/numStepsToPlot)*numStepsToPlot;

  // *wdh int numSteps= int(std::ceil( tFinal/dt ));//kkc is this what you meant?int(tFinal/tPlot+.999999)*numStepsToPlot;
  // we choose the number of steps so that we exactly reach times that are multiples of tPlot
  int numSteps= int(ceil(tFinal/tPlot))*numStepsToPlot; 

  dt= numSteps ? tFinal/numSteps : 0 ;
  
  printF("--- dt0=%9.3e dt=%9.3e tPlot=%f, tPlot/dt=%f numStepsToPlot=%i numSteps=%i \n",dt0,dt,tPlot,tPlot/dt,numStepsToPlot,numSteps);
  if( debug & 1 )
  {
    fprintf(pDebugFile,"--- dt0=%9.3e dt=%9.3e tPlot=%f, tPlot/dt=%f numStepsToPlot=%i numSteps=%i \n",
	    dt0,dt,tPlot,tPlot/dt,numStepsToPlot,numSteps);
    fflush(0);
  }
  assert( dt<= dt0 );
  

  // Initial conditions
  assignInitialConditions( current,t,dt ); 
  dtNew=dt;

  real nextTimeToPlot=0.;
//    nextTimeToPlot=min(int(t/tPlot+.5)*tPlot+tPlot,tFinal);   //  ...new time to print:
//    computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPlot,numberOfSubSteps,dtNew );
//    dt=dtNew;

   
  // Output the header banner with parameters and grid info.
  outputHeader();

  plotOptions=1;
  if( !gi.graphicsIsOn() && !gi.readingFromCommandFile() )
    plotOptions=0;
  
  char buff[80];           // buffer for sprintf
  numberOfStepsTaken=0;
  int iPrint=0;
  
  outputResultsAfterEachTimeStep( current,t,dt,0,nextTimeToPlot );
    
  for( int stepNumber=0; dt>REAL_MIN && stepNumber<maxNumberOfTimeSteps; stepNumber++ )                // take some time steps
  {

    if( t+.5*dt>nextTimeToPlot )  // plot solution 
    {
      const real cpuTime=getCPU()-time0;

      if( frequencyToSaveInShowFile>0 && (iPrint % frequencyToSaveInShowFile == 0) )
        saveShow( current,t,dt );  // save the current solution in the show file
      
      // --- print time step info ---
      printTimeStepInfo( current, numberOfStepsTaken,t,dt,cpuTime );

      // compute errors
      getErrors( current,t,dt );

      // save results to check file etc.
      outputResults(current,t,dt);


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
      iPrint++;
    }

    nextTimeToPlot=min(int(t/tPlot+.5)*tPlot+tPlot,tFinal);   //  ...new time to print:
    // ** we cannot change the time step now: 
    bool adjustTimeStep=false;

    computeNumberOfStepsAndAdjustTheTimeStep(t,tFinal,nextTimeToPlot,numberOfSubSteps,dtNew,adjustTimeStep );

    // no: dt=dtNew;  

    real time1=getCPU();

    for( int subStep=0; subStep<numberOfSubSteps; subStep++)
    {
      if( method==yee )
      {
	// advance on a rectangular grid
        advanceFDTD( numberOfStepsTaken,current,t,dt );
      }
      else if( method==dsi || method==dsiMatVec )      
      {
        advanceDSI(current,t,dt);
      }
      else if( method==nfdtd )
      {
	advanceNFDTD( numberOfStepsTaken,current,t,dt );
      }
      else if( method==dsiNew )
      {
	advanceNew( current,t,dt, fields );
      }
      else if( method==sosup )
      { // advance using the second-order-system upwind scheme:
	advanceSOSUP( numberOfStepsTaken,current,t,dt );
      }
      else
      {
	OV_ABORT("CGMX: ERROR: unknown method");
      }

      gf[current].t=t;

      t+=dt;
      current = next;
      next= (next+1) % numberOfTimeLevels;

      outputResultsAfterEachTimeStep( current,t,dt,stepNumber,nextTimeToPlot );

      numberOfStepsTaken++;
    }
    
    timing(timeForAdvance)+=getCPU()-time1;
    
  }

  if ( !numberOfStepsTaken && dt<REAL_MIN )
  {
    getErrors(current,t,dt );
    outputResults(current,t,dt);
  }
    

  timing(totalTime)+=getCPU()-time0;
  
  printStatistics();
  
  return 0;
}
