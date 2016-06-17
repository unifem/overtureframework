// This file contains some macros that are shared amongst the different predictor-corrector methods


// ==================================================================================================
// MACRO: This macro saves past values of the pressure and values of the velocity on the ghost lines
// For use with the fourth-order accurate INS solver. 
//
// tp (input) : past value of time
// nab (input) : save results in fn[nab] (NOTE: use the grid from gf[mOld] not the one with fn[nab] !)
// ==================================================================================================
#beginMacro savePressureAndGhostVelocity(tp,nab)
if( orderOfAccuracy==4 )
{
  const int uc = parameters.dbase.get<int >("uc");
  const int pc = parameters.dbase.get<int >("pc");
  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
       
  const int numberOfDimensions=cg.numberOfDimensions();
  const int numberOfGhostLines=2;
  Range V(uc,uc+numberOfDimensions-1);

  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = gf[mOld].cg[grid];

    realArray & fng = fn[nab][grid];
    realArray & uOld = gf[mOld].u[grid];
#ifdef USE_PPP
    realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(fng,fnLocal);
    realSerialArray uOldLocal; getLocalArrayWithGhostBoundaries(uOld,uOldLocal);
#else
    realSerialArray & fnLocal = fng;
    realSerialArray & uOldLocal = uOld;
#endif
    OV_GET_SERIAL_ARRAY_CONST(real,c.vertex(),xLocal);
    const int isRectangular=false; // for e.gd(..)

    const IntegerArray & gridIndexRange = c.gridIndexRange();
    getIndex(c.dimension(),I1,I2,I3);


    // save p for use when extrapolating in time
    //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
    //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
    //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
    if( parameters.dbase.get<bool >("twilightZoneFlow") )
    {
      // *wdh* 050416 fn[nab][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);  
      //  fn[nab][grid](I1,I2,I3,pc)=e(c,I1,I2,I3,pc,tp);
      // e.gd(fn[nab][grid],0,0,0,0,I1,I2,I3,pc,tp);
      e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,pc,tp);
      //  display(fn[nab][grid],"fn[nab][grid] after assigning for fourth order",debugFile,"%5.2f ");
      fprintf(debugFile,"savePressureAndGhostVelocity: Set p at old time for fourth-order: nab=%i, t=%9.3e\n",nab,tp);

      if( debug() & 4 )
      {
	display(xLocal,"savePressureAndGhostVelocity: xLocal from gf[mOld] ",debugFile,"%6.3f ");
	display(fn[nab][grid],"savePressureAndGhostVelocity: fn[nab][grid] after assigning p for fourth order",debugFile,"%6.3f ");
      }
	  
	
    }
    else
    {
      bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
      if( ok )
	fnLocal(I1,I2,I3,pc)=uOldLocal(I1,I2,I3,pc); // *** fix this ****
    }
      
    // We also extrapolate, in time, the ghost values of u -- used in the BC's
    getIndex(gridIndexRange,I1,I2,I3,numberOfGhostLines);
    for( int axis=0; axis<c.numberOfDimensions(); axis++ )
    {
      for( int side=0; side<=1; side++ )
      {
	const int is=1-2*side;
	if( c.boundaryCondition(side,axis)>0 )
	{
	  // set values on the two ghost lines
	  if( side==0 )
	    Iv[axis]=Range(gridIndexRange(side,axis)-2,gridIndexRange(side,axis)-1);
	  else
	    Iv[axis]=Range(gridIndexRange(side,axis)+1,gridIndexRange(side,axis)+2);
 
	  if( parameters.dbase.get<bool >("twilightZoneFlow") )
	  {
	    // *wdh* 050416 fn[nab][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
	    // fn[nab][grid](I1,I2,I3,V)=e(c,I1,I2,I3,V,tp);
	    // display(fn[nab][grid],"fn[nab][grid] before assign V on ghost",debugFile,"%5.2f ");
	    // e.gd(fn[nab][grid],0,0,0,0,I1,I2,I3,V,tp);
	    e.gd(fnLocal,xLocal,numberOfDimensions,isRectangular,0,0,0,0,I1,I2,I3,V,tp);
	    // display(fn[nab][grid],"fn[nab][grid] after assign V on ghost",debugFile,"%5.2f ");

	  }
	  else
	  {
	    bool ok = ParallelUtility::getLocalArrayBounds(fng,fnLocal,I1,I2,I3);
	    if( ok )
	      fnLocal(I1,I2,I3,V)=uOldLocal(I1,I2,I3,V); // ***** fix this ****
	  }
	}
      }
      // set back to gridIndexRange to avoid re-doing corners: *** is this ok for 3D ???
      Iv[axis]=Range(gridIndexRange(0,axis),gridIndexRange(1,axis));
    }
      
  }  // end for grid 
}
#endMacro



// ===============================================================================
///  MACRO:  Perform the initialization step for the PC method
///
///  \METHOD (input) : name of the method: adamsPC or implicitPC
///  Parameters:
///   numberOfPastTimes (input) : method needs u(t-dt), ... u(t-n*dt), n=numberOfPastTimes  
///   numberOfPastTimeDerivatives (input) : method needs u_t(t-dt), ..., u_t(t-m*dt) m=numberOfPastTimeDerivatives
///
// ===============================================================================
#beginMacro initializePredictorCorrector(METHOD,utImplicit)

const int orderOfPredictorCorrector = parameters.dbase.get<int >("orderOfPredictorCorrector");
const int orderOfTimeExtrapolationForPressure = parameters.dbase.get<int >("orderOfTimeExtrapolationForPressure");

printF("--METHOD-- initializePredictorCorrector: mCur=%i, mOld=%i \n",mCur,mOld);

if( movingGridProblem() )
{ 
  getGridVelocity( gf[mCur],t0 );
}
 

if( orderOfTimeExtrapolationForPressure!=-1 )
{

  // if( orderOfPredictorCorrector==2 && orderOfTimeExtrapolationForPressure>1 &&
  //     poisson!=NULL && poisson->isSolverIterative()  )

  // *wdh* 2015/01/26: we may need past time pressure for other reasons:
  const bool & predictedPressureNeeded = parameters.dbase.get<bool>("predictedPressureNeeded");
  const bool predictPressure = predictedPressureNeeded || (poisson!=NULL && poisson->isSolverIterative());
  printF("--METHOD-- orderOfPredictorCorrector=%i, orderOfTimeExtrapolationForPressure=%i, predictPressure=%i\n",
	 orderOfPredictorCorrector,orderOfTimeExtrapolationForPressure,(int)predictPressure);
  
  if( orderOfPredictorCorrector==2 && orderOfTimeExtrapolationForPressure>1 && predictPressure )
  {
    // orderOfTimeExtrapolationForPressure==1 :  p(t+dt) = 2*p(t) - p(t-dt)
    //                                      2 :  p(t+dt) = 3*p(t) - 3*p(t-dt) + p(t-2*dt)
    assert( previousPressure==NULL );
    assert( !parameters.isMovingGridProblem() );  // fix for this case
    
    numberOfExtraPressureTimeLevels = orderOfTimeExtrapolationForPressure - 1;
    printF("--DS-- ***initPC: allocate %i extra grid functions to store the pressure at previous times ****\n",
	   numberOfExtraPressureTimeLevels);
    
    previousPressure = new realCompositeGridFunction [numberOfExtraPressureTimeLevels];
    for( int i=0; i<numberOfExtraPressureTimeLevels; i++ )
    {
      previousPressure[i].updateToMatchGrid(gf[mCur].cg);
    }
  }
  
}

fn[nab0]=0.; 
if( numberOfPastTimeDerivatives>0 )
  fn[nab1]=0.; 
 
 
if( parameters.dbase.get<bool >("twilightZoneFlow") )
{
  OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
 
  if( orderOfAccuracy==4 )
  {
    // For the fourth-order PC method, first compute u.t(t-2*dt) and u.t(t-3*dt)
    // Even for 2nd-order in time methods -- save p and u-ghost at t-2*dt
    const int numberOfPreviousValuesOfPressureToSave= orderOfPredictorCorrector==2 ? 1 : 2;

    int grid;
    for( int m=0; m<numberOfPreviousValuesOfPressureToSave; m++ )
    {

      const int nab=(nab2+m) % 4; // save du/dt in fn[nab] 
      real tp=t0-(m+2)*dt0;       // move grid to this previous time

      if( movingGridProblem() )
      {
	// move gf[mOld] to t-(m+2)*dt
	moveGrids( t0,t0,tp,dt0,gf[mCur],gf[mCur],gf[mOld] );   // Is this correct? dt0?   
 
	gf[mOld].u.updateToMatchGrid(gf[mOld].cg); // *wdh* 040826

        // *wdh* 111125: the vertex is used below for error checking
        fn[nab].updateToMatchGrid(gf[mOld].cg);    


        // *wdh* 090806
	real cpu0=getCPU();
	gf[mOld].u.getOperators()->updateToMatchGrid(gf[mOld].cg); 
	parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;


      }
      gf[mOld].t=tp;
 
      e.assignGridFunction( gf[mOld].u,tp );

      updateStateVariables(gf[mOld]); // *wdh* 080204 
      
      if( parameters.useConservativeVariables() )
	gf[mOld].primitiveToConservative();
 
      if( orderOfPredictorCorrector==4 ) 
      { // we only need du/dt at old times for pc4
	for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
	{
	  rparam[0]=gf[mOld].t;
	  rparam[1]=gf[mOld].t; // tforce
	  rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit  *************** check me 111124 **********************
	  iparam[0]=grid;
	  iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
	  iparam[2]=numberOfStepsTaken;

	  getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab][grid],iparam,rparam,
		utImplicit[grid],&gf[mOld].cg[grid]);
	}
      }
      
      // save past time values of p and ghost u for the 4th order method
      // NOTE: PAST time values are saved in a funny place:
      // save p for use when extrapolating in time
      //    ua(.,.,.,pc)= p(t-2*dt)  (for 2nd/4th order)
      //    ub(.,.,.,pc)= p(t-3*dt)  (for 4th order)
      //    uc(.,.,.,pc)= p(t-4*dt)  (for 4th order)
      assert( nab0==0 );
      const int nabPastTime=(nab0+m);
      savePressureAndGhostVelocity(tp,nabPastTime);
      
      if( debug() & 4 )
      {
	// determineErrors( gf[mOld].u,gf[mOld].gridVelocity, tp, 0, error,
	// 		 sPrintF(" adams:startup: errors in u at t=%e \n",nab,tp) );

	if( movingGridProblem() && debug() & 64 )
	{
          CompositeGrid & cg = *fn[nab].getCompositeGrid();
	  
	  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
	  {
	    if( parameters.gridIsMoving(grid) )
	    {
	      display(cg[grid].vertex()(I1,I2,I3,0),sPrintF("\n *** PC: AFTER moveGrids:  fn[nab] "
                      "grid=%i vertex after move back t=%e",grid,gf[mOld].t),debugFile,"%8.5f ");
	    }
	  }
      
	}
	determineErrors( fn[nab],gf[mOld].gridVelocity, tp, 1, error,
			 sPrintF(" adams:startup: errors in ut (nab=%i) at t=%e \n",nab,tp) );
      }

    }
  }
 
       
  // get solution and derivative at t-dt
  if( movingGridProblem() )
  {
    // move gf[mOld] to t-dt
    if( debug() & 2 )
      fPrintF(debugFile,"--METHOD-- take an initial step backwards\n");
 	
         // display(gf[mOld].cg[0].vertex()(I1,I2,I3,0),sPrintF(" gf[mOld] vertex before move back at t=%e",gf[mOld].t),
         //                  debugFile,"%5.2f ");
 
    moveGrids( t0,t0,t0-dt0,dt0,gf[mCur],gf[mCur],gf[mOld] );          // this will set gf[mOld].t=t-dt
 
    // *wdh* 090806
    if( parameters.isAdaptiveGridProblem() )
    { // both moving and AMR 
      parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(gf[mOld].cg);
    }
    // *wdh* 090806
    real cpu0=getCPU();
    gf[mOld].u.getOperators()->updateToMatchGrid(gf[mOld].cg); 
    parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;

    if( debug() & 64 )
    {
      for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
      {
        if( parameters.gridIsMoving(grid) )
	{
	  display(gf[mOld].cg[grid].vertex()(I1,I2,I3,0),sPrintF("\n --METHOD-- AFTER moveGrids:  gf[mOld] grid=%i vertex after move back t=%e",grid,gf[mOld].t),
		  debugFile,"%10.7f ");
	}
      }
      
    }
    
 
    gf[mOld].u.updateToMatchGrid(gf[mOld].cg); // make sure the grid is correct, vertex used in TZ  *wdh* 040826

    // *wdh* 111125: the vertex is used below for error checking and computing ghost values of u
    if( numberOfPastTimeDerivatives>0 )
      fn[nab1].updateToMatchGrid(gf[mOld].cg);

  }
  else
    gf[mOld].t=t0-dt0; 
 
  // assign u(t-dt) with the TZ solution: 
  e.assignGridFunction( gf[mOld].u,t0-dt0 );
  updateStateVariables(gf[mOld]); // *wdh* 080204 
  if( parameters.useConservativeVariables() )
    gf[mOld].primitiveToConservative();

  // For BDF schemes we need more past solutions
  for( int kgf=2; kgf<=numberOfPastTimes; kgf++ )
  {
     const int mgf = (mCur - kgf + numberOfGridFunctions) % numberOfGridFunctions;
     const real tgf = t0-dt0*kgf;
     if( true )
       printF("--METHOD-- init past time solution at t=%9.3e\n",tgf);
     
     if( movingGridProblem() )
     {
       // **CHECK ME: dt0*kgf ? or -dt0*kgf
       // Note: on input gf[mgf].t=0 indicates the initial grid in gf[mgf] is located at t=0
       moveGrids( t0,t0,tgf,dt0*kgf,gf[mCur],gf[mCur],gf[mgf] );// this will set gf[mgf].t=tgf
     }
     gf[mgf].u.updateToMatchGrid(gf[mgf].cg); 
     e.assignGridFunction( gf[mgf].u,tgf );
     updateStateVariables(gf[mgf]); 
     if( parameters.useConservativeVariables() )
       gf[mgf].primitiveToConservative();
  }
  
  if( numberOfPastTimeDerivatives>0 )
  {
    // -- evaluate du/dt(t-dt) --
    for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
    {
      rparam[0]=gf[mOld].t;
      rparam[1]=gf[mOld].t; // tforce
      rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit  *************** check me 090806 **********************
      iparam[0]=grid;
      iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
      iparam[2]=numberOfStepsTaken;
      getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab1][grid],iparam,rparam,
	    utImplicit[grid],&gf[mOld].cg[grid]);
    }
  }
  
  // display(fn[nab1][0],sPrintF("ut(t-dt) from getUt at t=%e\n",gf[mOld].t),debugFile,"%5.2f ");
 
  if( false ) // for testing assign du/dt(t-dt) from TZ directly
  {
    for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & c = gf[mOld].cg[grid];
      getIndex(c.dimension(),I1,I2,I3);
      Range Na(0,parameters.dbase.get<int >("numberOfComponents")-1);
      fn[nab1][grid](I1,I2,I3,Na)=e.t(c,I1,I2,I3,Na,t0-dt0); 
 
      if( parameters.gridIsMoving(grid) )
      { // add on gDot.grad(u)
	const realArray & gridVelocity = gf[mOld].getGridVelocity(grid);
	const int na=parameters.dbase.get<int >("uc"), nb=na+c.numberOfDimensions()-1;   // ***** watch out ***
	for( int n=na; n<=nb; n++ )
	{
	  fn[nab1][grid](I1,I2,I3,n)+=gridVelocity(I1,I2,I3,0)*e.x(c,I1,I2,I3,n,t0-dt0)+
	    gridVelocity(I1,I2,I3,1)*e.y(c,I1,I2,I3,n,t0-dt0);
	  if( c.numberOfDimensions()>2 )
	    fn[nab1][grid](I1,I2,I3,n)+=gridVelocity(I1,I2,I3,2)*e.z(c,I1,I2,I3,n,t0-dt0);
	}
 	    
        display(fn[nab1][grid],sPrintF("METHOD:init: ut(t-dt) grid=%i from TZ at t=%e\n",grid,gf[mOld].t),debugFile,"%5.2f ");

      }
    }
  }
       
 
  if( numberOfExtraPressureTimeLevels>0 )
  {
    // get extra time levels for extrapolating the pressure in time (as an initial guess for iterative solvers)
    for( int i=0; i<numberOfExtraPressureTimeLevels; i++ )
    {
      // if orderOfPC==2 : we need p(t-2*dt), p(t-3*dt) ...
      //             ==4 : we need p(t-5*dt), ... **check**
      const real tp = t0-dt0*(i+orderOfPredictorCorrector);
      realCompositeGridFunction & pp = previousPressure[i];
      Range all;
      for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
      {
	e.gd( pp[grid],0,0,0,0,all,all,all,parameters.dbase.get<int >("pc"),tp);
      }
    }
  }
  

  if( debug() & 4 || debug() & 64 )
  {
    for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
    {
      aString buff;
      display(gf[mOld].u[grid],sPrintF(buff,"\n--METHOD-- Init:gf[mOld].u grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
              debugFile,"%9.3e ");
      if( numberOfPastTimeDerivatives>0 )
        display(fn[nab1][grid],sPrintF(buff,"\n--METHOD-- Init:fn[nab1] grid=%i : du/dt(t) t=%9.3e",grid,gf[mOld].t),
               debugFile,"%9.3e ");
      if( parameters.isMovingGridProblem() )
      {
        display(gf[mOld].getGridVelocity(grid),sPrintF("--METHOD-- t=-dt: gridVelocity[%i] at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%5.2f ");
        display(gf[mCur].getGridVelocity(grid),sPrintF("--METHOD-- t=0 : gridVelocity[%i] at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%5.2f ");
      }
      if( debug() & 64 && parameters.isMovingGridProblem() )
      {
	display(gf[mOld].cg[grid].vertex(),sPrintF("--METHOD-- gf[mOld].cg[%i].vertex at t=%9.3e\n",grid,gf[mOld].t),debugFile,"%7.4f ");
	display(gf[mCur].cg[grid].vertex(),sPrintF("--METHOD-- gf[mCur].cg[%i].vertex at t=%9.3e\n",grid,gf[mCur].t),debugFile,"%7.4f ");
      }
      
    }
    
  }
  if( debug() & 4 )
  {
    if( parameters.isMovingGridProblem() )
    {
      determineErrors( gf[mOld].u,gf[mOld].gridVelocity, gf[mOld].t, 0, error,
		       sPrintF("--METHOD-- errors in u at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
      if( numberOfPastTimeDerivatives>0 )
      {
        fn[nab1].updateToMatchGrid(gf[mOld].cg);  // for moving grid TZ to get errors correct
        determineErrors( fn[nab1],gf[mOld].gridVelocity, gf[mOld].t, 1, error,
  		       sPrintF("--METHOD-- errors in ut (fn[nab1]) at t=%9.3e (t0-dt0=%9.3e)\n",gf[mOld].t,t0-dt0) );
      }
    }
    
  }
 
       
}
else  
{
  // ****** Initialize for NOT twilightZoneFlow ***********

  // printF(" **************** METHOD: still need correct initial values for du/dt(t-dt)  ****** \n");
  // printF(" **************** use values from du/dt(t)                                  ****** \n");
  printF("--METHOD-- Initialize past time values for scheme ---\n");
  
  if( parameters.useConservativeVariables() )
    gf[mCur].primitiveToConservative();
 
  // if( parameters.isAdaptiveGridProblem() )
  //   gf[mOld].u.updateToMatchGrid(gf[mOld].cg);  // 040928 -- why is this needed here ?
  
  if( debug() & 8 )
  {
    printF(" PC: init: gf[mOld].u.numberOfGrids=%i \n",gf[mOld].u.numberOfGrids());
    printF(" PC: init: gf[mOld].cg.numberOfComponentGrids=%i \n",gf[mOld].cg.numberOfComponentGrids());
  
    printF(" PC: init: gf[mCur].u.numberOfGrids=%i \n",gf[mCur].u.numberOfGrids());
    printF(" PC: init: gf[mCur].cg.numberOfComponentGrids=%i \n",gf[mCur].cg.numberOfComponentGrids());
  }
  
  if( !parameters.dbase.get<bool>("useNewTimeSteppingStartup") )
  {
    assign(gf[mOld].u,gf[mCur].u);  // 990903 give initial values to avoid NAN's at ghost points for CNS
  }
  else
  {
    // *new* way to initialize past time solution  // *wdh* 2014/06/28 
    if( numberOfPastTimes==1 )
    {
      gf[mOld].t=t0-dt0;
      int previous[1]={mOld};  // 
      getPastTimeSolutions( mCur, numberOfPastTimes, previous  ); 

    }
    else
    {
      // For BDF schemes we need more past solutions (NOTE: this does not work for PC since previous[0]!=mOld)
      int *previous = new int[numberOfPastTimes];
      for( int kgf=1; kgf<=numberOfPastTimes; kgf++ )
      {
	const int mgf = (mCur - kgf + numberOfGridFunctions) % numberOfGridFunctions;
        gf[mgf].t=t0-dt0*kgf;
	previous[kgf-1]=mgf;
      }
      getPastTimeSolutions( mCur, numberOfPastTimes, previous  );
      delete [] previous;
    }
    
  }
  
  gf[mOld].form=gf[mCur].form;
 
  if( numberOfPastTimeDerivatives>0 )
  {
    for( int grid=0; grid<gf[mCur].cg.numberOfComponentGrids(); grid++ )
    {
      rparam[0]=gf[mOld].t;
      rparam[1]=gf[mOld].t; // tforce
      // *wdh* 090806 : what was this? rparam[2]=gf[mCur].t-gf[mOld].t; // tImplicit
      rparam[2]=gf[mCur].t; // tImplicit = apply forcing for implicit time stepping at this time
      iparam[0]=grid;
      iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
      iparam[2]=numberOfStepsTaken;
      getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),fn[nab1][grid],iparam,rparam,
	    utImplicit[grid],&gf[mOld].cg[grid]);
    }
  }
  
  if( debug() & 4 )
  {
    determineErrors( fn[nab1],gf[mOld].gridVelocity, gf[mOld].t, 1, error,
   		   sPrintF(" PC:init: du/dt at past time t=%e \n",gf[mOld].t) );
  }
  
  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & c = gf[mOld].cg[grid];
    getIndex(c.dimension(),I1,I2,I3);
    // fn[nab1][grid](I1,I2,I3,N)=fn[nab0][grid](I1,I2,I3,N);
    if( orderOfPredictorCorrector==4 )
    {
      for( int m=0; m<=1; m++ )
      {
	const int nab=(mOld+m+1) % 4;
	// *wdh* 050319 fn[nab][grid](I1,I2,I3,N)=fn[nab0][grid](I1,I2,I3,N);
	assign(fn[nab][grid],fn[nab0][grid],I1,I2,I3,N);
      }
    }
  }
 
}
     
 
dtb=dt0;    // delta t to go from ub to ua
 
dtp[0]=dt0;
dtp[1]=dt0;
dtp[2]=dt0;
dtp[3]=dt0;
dtp[4]=dt0;
 
//       if( debug() & 8 )
//       {
//         fprintf(debugFile," advance Adams PC: ut at t0=%e\n",t0);
//         outputSolution( fn[nab0],t0 );
//         fprintf(debugFile," advance Adams PC: Errors in ut at t0=%e\n",t0);
//         determineErrors( fn[1],t0-dt0 );
//       }
 
init=false;


#endMacro


// =======================================================================================================
//    Macro to move the grids at the start of a PC time step.
// Arguments:
//    METHOD : name of the calling function (for debug output)
//    utImplicit : name of the grid function that holds the explicit part of the implicit operator.
//    
//    predictorOrder : order of the predictor corrector
//    ub,uc,ud : grid functions that hold du/dt at times tb, tc, td
//               If predictorOrder==2 then explosed points are filled in on ub.
//               If predictorOrder==3 then explosed points are filled in on ub and uc.
//               If predictorOrder==4 then explosed points are filled in on ub, uc and ud.
// =======================================================================================================
#beginMacro moveTheGridsMacro(METHOD,utImplicit,predictorOrder,tb,ub,tc,uc,td,ud)

if( movingGridProblem() )
{
  checkArrays(" METHOD : before move grids"); 

  if( debug() & 8 )
    printf(" METHOD: before moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);

  // generate gf[mNew] from gf[mCur] (compute grid velocity on gf[mCur] and gf[mNew]
  moveGrids( t0,t0,t0+dt0,dt0,gf[mCur],gf[mCur],gf[mNew] ); 

  checkArrayIDs(sPrintF(" METHOD : after move grids t=%9.3e",gf[mCur].t));

  if( parameters.isAdaptiveGridProblem() )
  {
    // both moving and AMR 
    parameters.dbase.get<Ogen* >("gridGenerator")->updateRefinement(gf[mNew].cg);
  }
      
  if( debug() & 16 )
  {
    if( twilightZoneFlow() )
    {
      fprintf(debugFile,"\n ---> METHOD : Errors in u after moveGrids t=%e  \n",gf[mCur].t);
      determineErrors( gf[mCur] );
    }
  }


  real cpu0=getCPU();
  gf[mNew].cg.rcData->interpolant->updateToMatchGrid( gf[mNew].cg );  
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-cpu0;

  cpu0=getCPU();
  gf[mNew].u.getOperators()->updateToMatchGrid(gf[mNew].cg); 
  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateOperators"))+=getCPU()-cpu0;

  if( debug() & 4 ) printf("METHOD : step: update gf[mNew] for moving grids, gf[mNew].t=%9.3e,...\n",gf[mNew].t);

  if( debug() & 16 )
  {
    if( twilightZoneFlow() )
    {
      fprintf(debugFile,"\n ---> METHOD: Errors in u before updateForMovingGrids t=%e  \n",gf[mCur].t);
      fprintf(debugFile,"*** mCur=%i mNew=%i numberOfGridFunctions=%i *** \n",
	      mCur,mNew,numberOfGridFunctions);
	  
      determineErrors( gf[mCur] );
    }
  }

  updateForMovingGrids(gf[mNew]);
  // ****      gf[mNew].u.updateToMatchGrid( gf[mNew].cg );  

  checkArrayIDs(sPrintF(" METHOD : after updateForMovingGrids t=%9.3e",gf[mCur].t));

  if( debug() & 16 )
  {
    if( twilightZoneFlow() )
    {
      fprintf(debugFile,"\n ---> METHOD: Errors in u after updateForMovingGrids t=%e  \n",gf[mCur].t);
      determineErrors( gf[mCur] );
    }
  }

  // get values for exposed points on gf[mCur]
  if( parameters.useConservativeVariables() )
    gf[mCur].primitiveToConservative();  // *wdh* 010318

  cpu0=getCPU();
  if( useNewExposedPoints && parameters.dbase.get<int>("simulateGridMotion")==0 )
  {
    if( Parameters::checkForFloatingPointErrors )
      checkSolution(gf[mCur].u,"Before interp exposed points",true);

    if( debug() & 16 )
    {
      if( twilightZoneFlow() )
      {
	fprintf(debugFile,"\n ---> METHOD: Errors in u BEFORE interp exposed t=%e  \n",gf[mCur].t);
	determineErrors( gf[mCur] );
      }
    }


    ExposedPoints exposedPoints;
    exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));
    exposedPoints.initialize(gf[mCur].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
    exposedPoints.interpolate(gf[mCur].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0);

    // Added for BDF: *wdh* 2015/04/05
    for( int kp=1; kp<=numberOfPastTimes; kp++ )
    {
      const int mPast = (mCur -kp + numberOfGridFunctions) % numberOfGridFunctions; 
      exposedPoints.initialize(gf[mPast].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
      exposedPoints.interpolate(gf[mPast].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0);
    }
    

    if( debug() & 16 )
    {
      if( twilightZoneFlow() )
      {
	fprintf(debugFile,"\n ---> METHOD: Errors in u AFTER interp exposed t=%e  \n",gf[mCur].t);
	determineErrors( gf[mCur] );
      }
    }

    if( predictorOrder>=2  )
    {
      // -------------------------
      // --- fixup du/dt(t-dt) ---
      // -------------------------

      // NOTE: we CANNOT directly interpolate points on du/dt since for moving grids
      // du/dt includes the -gDot.grad(u) term 

      // Current procedure: 
      //   1. Interpolate exposed points on u(t-dt)
      //   2. Recompute du/dt(t-dt) 

      // Optimizations: 
      //   - only recompute du/dt(t-dt) on grids with exposed points
      //   - could only compute du/dt(t-dt) on those points where is not already known.

      if( gf[mCur].t<=0. || debug() & 4 )
      {
        printF(" --- INFO: Fixup exposed points of u(t-dt) and recompute du/dt(t-dt) t=%9.3e, tb=%9.3e ----- \n"
               "     The extra work involved in recomputing du/dt(t-dt) can be avoided by using "
               "the option 'first order predictor'.\n",gf[mCur].t,tb);
      }
      
      ExposedPoints exposedPoints;

      //            
      // exposedPoints.setExposedPointType(ExposedPoints::exposedDiscretization);

      exposedPoints.setAssumeInterpolationNeighboursAreAssigned(parameters.dbase.get<int >("extrapolateInterpolationNeighbours"));

      exposedPoints.initialize(gf[mOld].cg,gf[mNew].cg,parameters.dbase.get<int >("stencilWidthForExposedPoints"));
      exposedPoints.interpolate(gf[mOld].u,(twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),gf[mOld].t);
	  
      // For now recompute du/dt(t-dt) using the mask values from cg(t+dt)
      for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
      {
             
	if( gridWasAdapted || exposedPoints.getNumberOfExposedPoints(grid)>0 )
	{
          if( debug() & 2 )
	    printf(" ---- METHOD: recompute du/dt(t-dt) for grid=%i t-dt = %9.3e  (%i exposed)-----\n",grid,gf[mOld].t,
		   exposedPoints.getNumberOfExposedPoints(grid));

	  // This is only necesssary if there are exposed points on this grid
	  rparam[0]=gf[mOld].t;
	  rparam[1]=gf[mOld].t;
	  rparam[2]=gf[mCur].t; // tImplicit
	  iparam[0]=grid;
	  iparam[1]=gf[mOld].cg.refinementLevelNumber(grid);
	  iparam[2]=numberOfStepsTaken;

	  getUt(gf[mOld].u[grid],gf[mOld].getGridVelocity(grid),ub[grid],iparam,rparam,
		utImplicit[grid],&gf[mNew].cg[grid]);
	      
	}
      }
      if( debug() & 4 )
      {	
	if( twilightZoneFlow() )
	{
	  fprintf(debugFile," ***METHOD: gf[mOld] after interp exposed, gf[mOld].t=%e",gf[mOld].t);
	  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
	  {
	    display(gf[mOld].u[grid],sPrintF("\n ****gf[mOld].u[grid=%i]",grid),debugFile,"%7.1e ");
	  }
	  determineErrors( gf[mOld] );

	  fprintf(debugFile," ***METHOD: du/dt(t-dt)  after interp exposed, gf[mOld].t=%e",gf[mOld].t);
	  for( int grid=0; grid<gf[mOld].cg.numberOfComponentGrids(); grid++ )
	  {
	    display(ub[grid],sPrintF("\n ****ub[grid=%i]: du/dt(t-dt)",grid),debugFile,"%7.1e ");
	  }
	  determineErrors( ub,gf[mOld].gridVelocity, gf[mOld].t, 1, error );
	}
      }
      
	    
      if( predictorOrder>=3 )
      {
        OV_ABORT("METHOD: moveTheGridsMacro:Error: finish me for predictorOrder>=3");
      }
      
    } // end if predictorOrder>=2
	
    if( Parameters::checkForFloatingPointErrors )
      checkSolution(gf[mCur].u,"METHOD: After interp exposed points",true);

  }
  else if( parameters.dbase.get<int>("simulateGridMotion")==0  )
  {
    // *old way* 
    interpolateExposedPoints(gf[mCur].cg,gf[mNew].cg,gf[mCur].u, 
			     (twilightZoneFlow() ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t0,
			     false,Overture::nullIntArray(),Overture::nullIntegerDistributedArray(),
			     parameters.dbase.get<int >("stencilWidthForExposedPoints") ); 
  }

    
  if( twilightZoneFlow() && false ) // **** wdh **** 
  {
    // for testing ***
    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));
	
    int grid=0; 
    MappedGrid & c = gf[mNew].cg[grid];
    getIndex(c.dimension(),I1,I2,I3);

    ub[grid](I1,I2,I3,N)=e.t(c,I1,I2,I3,N,t0-dt0); 
	
  }



  parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForInterpolateExposedPoints"))+=getCPU()-cpu0;
  // compute dudt now -- after exposed points have been computed!

  checkArrayIDs(sPrintF(" METHOD : after moving grids update t=%9.3e",gf[mCur].t));


  if( debug() & 16 )
  {
    if( twilightZoneFlow() )
    {
      fprintf(debugFile,"\n ---> METHOD: Errors in u after move grids t=%e  \n",gf[mCur].t);
      determineErrors( gf[mCur] );
    }
  }

  if( debug() & 16 )
    printf(" METHOD: AFTER moveTheGridsMacro: t0=%9.3e, gf[mNew].t=%9.3e, gf[mNew].gridVelocityTime=%9.3e\n",
	   t0,gf[mNew].t,gf[mNew].gridVelocityTime);
}


#endMacro



// =======================================================================================================
//    Macro to correct for moving grids.
// Arguments:
//    METHOD : name of the calling function (for debug output)
// Retrun:
//   movingGridCorrectionsHaveConverged = true if this is a moving grid problem and the sub-iteration
//             corrections have converged.
// =======================================================================================================
#beginMacro correctForMovingGridsMacro(METHOD)

  // Correct for forces on moving bodies if we have more corrections.
  bool movingGridCorrectionsHaveConverged = false;
  real delta =0.; // holds relative correction when we are sub-cycling 
  const bool useMovingGridSubIterations= parameters.dbase.get<bool>("useMovingGridSubIterations");

  // *wdh* 2015/12/16 -- explicitly check for useMovingGridSubIterations, otherwise we can do multiple
  //                     corrections always if requested,
  if( movingGridProblem() && (numberOfCorrections==1  // *wdh* 2015/05/24 -- this case was missing in new version
			      || !useMovingGridSubIterations)  ) // *wdh* 2015/12/16 
  {
    if( numberOfCorrections>10 )
    {
      printF("WARNING: movingGrid problem, useMovingGridSubIterations=false but numberOfCorrections>10\n");
      OV_ABORT("ERROR: this is an error for now");
    }
    
    correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 

  }
// else if( movingGridProblem() && (correction+1)<numberOfCorrections)
  else if( movingGridProblem() )
  {
    // --- we may be iterating on the moving body motion (e.g.for light bodies) ---
    //     After correcting for the motion, check for convergence
    correctMovingGrids( t0,t0+dt0,gf[mCur],gf[mNew] ); 

    // Check if the correction step has converged
    bool isConverged = getMovingGridCorrectionHasConverged();
    delta = getMovingGridMaximumRelativeCorrection();
    if( debug() & 2 )
      printF("METHOD: moving grid correction step : delta =%8.2e (correction=%i, isConverged=%i)\n",
	     delta,correction+1,(int)isConverged);

    if( isConverged && (correction+1) >=minimumNumberOfPCcorrections )  // note correction+1 
    {
      movingGridCorrectionsHaveConverged=true;  // we have converged -- we can break from correction steps
      if( delta!=0. && debug() & 1 )
	printF("METHOD: moving grid correction step : sub-iterations converged after %i corrections, rel-err =%8.2e\n",
	       correction+1,delta);
      // break;  // we have converged -- break from correction steps
    }
    if( (correction+1)>=numberOfCorrections )
    {
      printF("METHOD:ERROR: moving grid corrections have not converged! numberOfCorrections=%i, rel-err =%8.2e\n",
	     correction+1,delta);
    }
    

  }
  else 
  {
  }

#endMacro
