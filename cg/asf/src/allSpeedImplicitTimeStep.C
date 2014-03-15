// This file automatically generated from allSpeedImplicitTimeStep.bC with bpp.
#include "Cgasf.h"
#include "AsfParameters.h"
#include "MappedGridOperators.h"
#include "interpPoints.h"
#include "Reactions.h"
#include "CompositeGridOperators.h"
#include "Oges.h"
#include "AdamsPCData.h"
#include "ParallelUtility.h"
#include "App.h"

// Macros for extracting local arrays



#define asfAddGradP EXTERN_C_NAME(asfaddgradp)
extern "C"
{
    void asfAddGradP(const int&nd,const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,
               		   const int&nd3a,const int&nd3b,const int&nd4a,const int&nd4b,		   const int&ipar, const real&rpar, 
               		   real&u, real&rho, const int& mask, const real&rsxy, const DataBase *pdb, const int&ierr );
}



// This macro is used to loop over the boundaries
#define ForBoundary(side,axis)   for( axis=0; axis<c.numberOfDimensions(); axis++ ) for( side=0; side<=1; side++ )

static int
smoothNearBoundary( realCompositeGridFunction & u, Range & R)
{
    CompositeGrid & cg = *u.getCompositeGrid();
    Index I1b,I2b,I3b;
    const int numberOfIterations =2;
    for( int it=0; it<numberOfIterations; it++ )
    {
    // smooth points on lines near the boundary
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mg = cg[grid];
        
            realArray & v = u[grid];
            const real omega=1./8.;
            for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
            {
      	for( int side=Start; side<=End; side++ )
      	{
        	  if( mg.boundaryCondition(side,axis)>0 )
        	  {
          	    getGhostIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b,-1,-2); // first line in
          	    v(I1b,I2b,I3b,R)+=omega*( v(I1b-1,I2b,I3b,R)+v(I1b+1,I2b,I3b,R)+v(I1b,I2b+1,I3b,R)+v(I1b,I2b-1,I3b,R)
                              				      -4.*v(I1b,I2b,I3b,R) );
          	    getGhostIndex(mg.gridIndexRange(),side,axis,I1b,I2b,I3b,-2,-2); 
          	    v(I1b,I2b,I3b,R)+=omega*( v(I1b-1,I2b,I3b,R)+v(I1b+1,I2b,I3b,R)+v(I1b,I2b+1,I3b,R)+v(I1b,I2b-1,I3b,R)
                              				      -4.*v(I1b,I2b,I3b,R) );
        	  }
      	}
            }
        }
        u.periodicUpdate();
    }
    return 0;
}

// =======================================================================================
//   Initialization steps for the all-speed time stepping 
// =======================================================================================


// =====================================================================================
//   Compute the desnity rho from the equation of state
// =====================================================================================

// =========================================================================================
//  Smooth the velocity rhs for the pressure to remove large oscillations in the divergence
//   -- this was a test ---
// =========================================================================================


// static real deltaT=-1., oldDeltaT, olderDeltaT;
// int n0,n1,n2, m0,m1;  // fix this (also appears in asfp.C)

void Cgasf::
allSpeedImplicitTimeStep( GridFunction & gf0,  
                    			  real & t, 
                    			  real & dt0, 
                    			  int & numberOfSubSteps,
                    			  const real & nextTimeToPrint )
// ==================================================================================================
// /Purpose: Integrate from the current time to time=tFinal using an implicit
//   time-stepping method.
// /Remarks:
//   
// ==================================================================================================
{

    const bool newWay = FALSE;

    CompositeGrid & cg = gf0.cg;

    realCompositeGridFunction & u = gf0.u;
    realCompositeGridFunction & f = pressureRightHandSide;
    RealCompositeGridFunction & uti = fn[3];  // save implicit (viscous) terms here

    OGFunction & e = *(parameters.dbase.get<OGFunction* >("exactSolution"));

    CompositeGridOperators & operators = *(u.getOperators());   // ***** be careful with op's for moving grids
    
    parameters.dbase.get<int >("explicitMethod")=FALSE;  // **** ??
    real cImplicit=0.;  // this will cause some terms to be zeroed out in du/dt
    
    AsfParameters & asfParameters = (AsfParameters &)parameters;
    const AsfParameters::AlgorithmVariation & algorithmVariation = 
                                                parameters.dbase.get<AsfParameters::AlgorithmVariation>("algorithmVariation");

    int & initializeImplicitMethod = parameters.dbase.get<int >("initializeImplicitMethod");
    const int & linearizeImplicitMethod = parameters.dbase.get<int >("linearizeImplicitMethod");
    const Parameters::ImplicitMethod & implicitMethod = parameters.dbase.get<Parameters::ImplicitMethod >("implicitMethod");
    
  // Save some time-stepping state information in the data-base: 
    DataBase & modelData = parameters.dbase.get<DataBase >("modelData");
    if( !modelData.has_key("asfImplicitData") )
        modelData.put<AdamsPCData>("asfImplicitData");
    AdamsPCData & adamsData = modelData.get<AdamsPCData>("asfImplicitData");
    real & deltaT= adamsData.dtp[0], &oldDeltaT=adamsData.dtp[1], &olderDeltaT=adamsData.dtp[2];
    int &n0=adamsData.nab0, &n1=adamsData.nab1, &n2=adamsData.nab2, &m0=adamsData.mab0, &m1=adamsData.mab1; 

    int grid;
    Index Iv[3];
    Index & I1 = Iv[0], & I2=Iv[1], & I3=Iv[2];
    Index I1g,I2g,I3g;
    int iparam[10];
    real rparam[10];

    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & numberOfSpecies    = parameters.dbase.get<int >("numberOfSpecies");
    const int & rc = parameters.dbase.get<int >("rc");
    const int & uc = parameters.dbase.get<int >("uc");
    const int & vc = parameters.dbase.get<int >("vc");
    const int & wc = parameters.dbase.get<int >("wc");
    const int & pc = parameters.dbase.get<int >("pc");
    const int & tc = parameters.dbase.get<int >("tc");
    const int & sc = parameters.dbase.get<int >("sc");
  // const real & mu = parameters.dbase.get<real >("mu");
    const real & gamma = parameters.dbase.get<real >("gamma");
  // const real & kThermal = parameters.dbase.get<real >("kThermal");
    const real & Rg = parameters.dbase.get<real >("Rg");
  // const real & avr = parameters.dbase.get<real >("avr");
    const real & pressureLevel = parameters.dbase.get<real >("pressureLevel");
  // const real & nuRho = parameters.dbase.get<real >("nuRho");
  // const real & nu = parameters.dbase.get<real >("nu");
  // const real & anu = parameters.dbase.get<real >("anu");
    
    const bool & computeReactions = parameters.dbase.get<bool>("computeReactions");
    const bool & twilightZoneFlow = parameters.dbase.get<bool>("twilightZoneFlow");
    
    RealArray & timing = parameters.dbase.get<RealArray>("timing");
    

    real & a0 = parameters.dbase.get<real >("a0");
    real & a1 = parameters.dbase.get<real >("a1");
    real & a2 = parameters.dbase.get<real >("a2");
    real & b0 = parameters.dbase.get<real >("b0");
    real & b1 = parameters.dbase.get<real >("b1");
    real & b2 = parameters.dbase.get<real >("b2");
    
    FILE *&debugFile = parameters.dbase.get<FILE* >("debugFile");

    Index N(0,numberOfComponents);
    Range V(uc,uc+cg.numberOfDimensions()-1);
    Range all;

    bool useOpt=true;  // use new optimized versions

  // --- initialization steps ---
        if( initializeImplicitMethod )
        {
      // initialize linearized state before computing the time step
            if( linearizeImplicitMethod )
            { // Assign the linearized state:
                cout << "\n allSpeed implicit time : reset the linearized state, rL and pL (OLD WAY)............... \n";
                for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                {
                    realMappedGridFunction & r0 = rL()[grid];     // *************** fix this ******
                    realMappedGridFunction & p0 = pL()[grid];
          	r0.updateToMatchGrid(cg[grid],all,all,all);
          	p0.updateToMatchGrid(cg[grid],all,all,all);
          // r0=u[grid](all,all,all,rc);
          // p0=u[grid](all,all,all,pc);   // leave off pressureLevel
                    assign(r0,all,all,all,0, u[grid],all,all,all,rc);
                    assign(p0,all,all,all,0, u[grid],all,all,all,pc);
                }
            }
            if( computeReactions )
            {
                for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                    gam()[grid].updateToMatchGrid(cg[grid],all,all,all);
            }
        }
    // compute the potential new time step
        dt= getTimeStep( gf[current] ); 
        if( initializeImplicitMethod ||  movingGridProblem() || fabs(deltaT/dt-1.) > .05 )
        {
            refactorImplicitMatrix=TRUE;
        }
        else
        {
      // do not change the time step
            dt=deltaT;  
            refactorImplicitMatrix=FALSE;
        }
        numberOfSubSteps=max(1,int((nextTimeToPrint-t)/dt+.9999));   // used to be +.5 or +1.
        if( numberOfSubSteps <= parameters.dbase.get<int >("maximumStepsBetweenComputingDt") )
        {
      // alter dt slightly so we exactly reach the next time to print (or tFinal)
            if( refactorImplicitMatrix )
                dt=(nextTimeToPrint-t)/numberOfSubSteps;
        }
        else
            numberOfSubSteps=parameters.dbase.get<int >("maximumStepsBetweenComputingDt");
        if( fabs(dt-deltaT) < REAL_EPSILON*10.*dt )
        {
            dt=deltaT;
            refactorImplicitMatrix=FALSE;
        }
        else
        {
            if( debug() & 1 )
                printf("-----------AllSpeed: new time step: dt = %e -----------\n",dt);
        }
        if( initializeImplicitMethod )
        {
            olderDeltaT=dt;
            oldDeltaT=dt;   // this holds the previous value of deltaT, needed when we change time step
        }
        else
        {
            olderDeltaT=oldDeltaT;
            oldDeltaT=deltaT;   // this holds the previous value of deltaT, needed when we change time step
        }
        deltaT=dt;
        if( implicitMethod==Parameters::secondOrderBDF ) 
        {
      // a0=2./3., a1=4./3., a2=-1./3., b0=16./9., b1=-14./9., b2=4./9.;
            real oldRatio=oldDeltaT/deltaT;
            real olderRatio=(oldDeltaT+olderDeltaT)/deltaT;
            a0=(oldRatio+1)/(oldRatio+2.);
            a1=SQR(oldRatio+1.)/(oldRatio*(oldRatio+2));
            a2=-1./(oldRatio*(oldRatio+2.));
            b1=(olderRatio/4+1./6+a2*SQR(oldRatio)*.5*(oldRatio/3.-olderRatio*.5))/(oldRatio*.5*(oldRatio-olderRatio));
            b2=(a2*SQR(oldRatio)-1.)/(2.*olderRatio)-b1*oldRatio/olderRatio;
            b0=1.+oldRatio*a2-b1-b2;
            if( debug() & 8 )
                printf(" a0=%f (2/3), a1=%f (4/3), a2=%f (-1/3), b0=%f (16/9), b1=%f (-14/9), b2=%f (4/9)\n",
             	   a0,a1,a2,b0,b1,b2);
        }
        else
        { // Backward Euler, Adams Basthforth  
      // a0=1., a1=1., a2=0., b0=1.5, b1=-.5, b2=0.; 
            real oldRatio=oldDeltaT/deltaT;
            a0=1., a1=1., a2=0., b0=1.+.5/oldRatio, b1=-.5/oldRatio, b2=0.;  
            if( debug() & 2 )
                printf(" a0=%f (1), a1=%f (1), a2=%f (0), b0=%f (1.5), b1=%f (-.5), b2=%f (0)\n",
             	   a0,a1,a2,b0,b1,b2);
        }
        if( initializeImplicitMethod )
        {
      // cout << "allSpeedImplicitTimeStep: time step = " << dt << " (explicit dt= " << dtExplicit << ", ratio=" 
      //      << dt/dtExplicit << ") gridSpacing=" << cg[0].gridSpacing()(0) << endl;    
            n0=0;  n1=1; n2=2;            // initial values
            m0=0;  m1=1;                  // initial values
            gf[m0].u = u;
      // get values needed to start
            if( movingGridProblem() )
                getGridVelocity( gf[m0],t);
            if( implicitMethod==Parameters::secondOrderBDF ) 
            {
        // *** we need fn[n2] = du/dt(t-2dt) *******
                gf[m1].t=t-2.*dt;
                if( movingGridProblem() )
          	getGridVelocity( gf[m1],t-2.*dt);  // ****** fix this ****** move the grid
                if( twilightZoneFlow )
                    e.assignGridFunction(gf[m1].u,t-2.*dt);
                else
                    assign(gf[m1].u,gf[m0].u); // ******** fix this ****
                for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                {
          	MappedGrid & c = gf[m1].cg[grid];
          	getIndex(c.dimension(),I1,I2,I3);
  	// if( twilightZoneFlow )
  	//   gf[m1].u[grid](I1,I2,I3,N)=e(c,I1,I2,I3,N,t-2.*dt);
  	// else
  	//   gf[m1].u[grid](I1,I2,I3,N)=gf[m0].u[grid](I1,I2,I3,N);   // ******** fix this ****
  	// getUt(grid, gf[m1][grid],t-2.*dt,fn[n2][grid] );
          	rparam[0]=t-2.*dt;
          	rparam[1]=t-2.*dt; // tforce
          	rparam[2]=t-dt;    // tImplicit
          	iparam[0]=grid;
                    iparam[1]=gf[m1].cg.refinementLevelNumber(grid);
          	getUt(gf[m1].u[grid],gf[m1].getGridVelocity(grid), fn[n2][grid],iparam,rparam,uti[grid]);
                }
            }
      // get solution and derivative at t-dt
            if( movingGridProblem() )
            {
        // move gf[m1] to t-dt
                moveGrids( t,t,t-dt,dt,gf[m0],gf[m0],gf[m1] );          // this will set gf[m1].t=t-dt
                gf[m1].u.getOperators()->updateToMatchGrid(gf[m1].cg); 
            }
            else
                gf[m1].t=t-dt; 
            if( twilightZoneFlow )
                e.assignGridFunction(gf[m1].u,t-dt);
            else
                assign(gf[m1].u,gf[m0].u); // just set solution at time t-dt to solution at time t -- fix this --
  //     for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  //     {
  //       MappedGrid & c = gf[m1].cg[grid];
  //       getIndex(c.dimension(),I1,I2,I3);
  //       if( twilightZoneFlow )
  // 	gf[m1].u[grid](I1,I2,I3,N)=e(c,I1,I2,I3,N,t-dt);
  //       else
  //       {
  // 	// just set solution at time t-dt to solution at time t
  // 	gf[m1].u[grid](I1,I2,I3,N)=gf[m0].u[grid](I1,I2,I3,N);   
  //       }
  //     }
      // get values for exposed points on gf[m1]  ** remember: you must compute du/dt after doing this ***
              if( movingGridProblem() )
                  interpolateExposedPoints(gf[m1].cg,gf[m0].cg,gf[m1].u,
                                                                    (twilightZoneFlow ? parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t-dt);
            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
                MappedGrid & c = gf[m1].cg[grid];
                getIndex(c.dimension(),I1,I2,I3);
                rparam[0]=t-dt;
                rparam[1]=t-dt; // tforce
                rparam[2]=t;    // tImplicit
                iparam[0]=grid;
                iparam[1]=gf[m1].cg.refinementLevelNumber(grid);
                getUt(gf[m1].u[grid],gf[m1].getGridVelocity(grid),fn[n1][grid],iparam,rparam, uti[grid]);
            }
            if( debug() & 16 )
            { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
                fprintf(debugFile,
                	      "allSpeedTimeStep: Initialize: after getUt t=%e dt=%e n0=%i, n1=%i\n",t+dt,dt,n0,n1);
                fn[n1].display("fn[n1]",debugFile);
            }
  // ---------------------
      // extrapolate du/dt to the ghostline here
      // --needed for rhs to p equation (div(f))  [ note: tz flow fixes the  "u" part]
            if( newWay )
            {
                if( twilightZoneFlow )
                    fn[n1].getOperators()->setTwilightZoneFlow( FALSE );  // turn off tz forcing in extrapolation!
                fn[n1].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t-dt);
        // extrapolate rhs for p equation too -- neumann BC's ??
                fn[n1].applyBoundaryCondition(pc,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t-dt);
                fn[n1].finishBoundaryConditions();  
                if( twilightZoneFlow )
                    fn[n1].getOperators()->setTwilightZoneFlow( TRUE );
            }
  // ------------------------
            initializeImplicitMethod=FALSE;
        }
        int mInitial=m0;  // save initial value


  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%%%%%%%%%%%%%%%%%%%% Time Step %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  // %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    for( int step=0; step<numberOfSubSteps; step++ )
    {
        const real dtRatio = step==0 ? deltaT/oldDeltaT : 1.;

        realCompositeGridFunction & u0 = gf[m0].u;
        realCompositeGridFunction & u1 = gf[m1].u;

        if( movingGridProblem() )
        {
      // generate gf[m1] from gf[m0] (compute grid velocity on gf[m0] and gf[m1]
            moveGrids( t,t,t+dt,dt,gf[m0],gf[m0],gf[m1] ); 
            gf[m1].cg.rcData->interpolant->updateToMatchGrid( gf[m1].cg );  
            gf[m1].u.getOperators()->updateToMatchGrid(gf[m1].cg); 

            p().updateToMatchGrid(gf[m1].cg,all,all,all);   // *** added ***
            p().setOperators(*gf[m1].u.getOperators());
            px().setOperators(*gf[m1].u.getOperators());

            updateForMovingGrids(gf[m1]);
      // ****      gf[m1].u.updateToMatchGrid( gf[m1].cg );  

      // get values for exposed points on gf[m0]  ** remember: you must compute du/dt after doing this ***
            interpolateExposedPoints(gf[m0].cg,gf[m1].cg,gf[m0].u,(twilightZoneFlow ? 
                                                              parameters.dbase.get<OGFunction* >("exactSolution") : NULL),t);  
        }

    // This next check should be the same as in solveForPressure, except numberOfImplicitSolves is 1 bigger
        if( refactorImplicitMatrix || (numberOfImplicitSolves % parameters.dbase.get<int >("refactorFrequency")) == 0 )
        {
            if( linearizeImplicitMethod )
            {
                cout << "\n allSpeedTimeStep:linearizeImplicitMethod  reset rL and pL............... \n";

      	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      	{
                    realMappedGridFunction & r0 = rL()[grid];
                    realMappedGridFunction & p0 = pL()[grid];
        	  if( useOpt )
        	  {
                        assign(r0,all,all,all,0, u0[grid],all,all,all,rc);
          	    assign(p0,all,all,all,0, u0[grid],all,all,all,pc);
        	  }
        	  else
        	  {
          	    r0=u0[grid](all,all,all,rc);
          	    p0=u0[grid](all,all,all,pc);  // leave off pressureLevel
        	  }
        	  
      	}
            }
        }
        
        if( false )
        {
            printf("...smooth u near the boundary\n");
            smoothNearBoundary( gf[m0].u,V );
        }
        
        if( false )
        {
            if( !twilightZoneFlow )
            {
        // smooth the velocity rhs for the pressure to remove large oscillations in the divergence
        // probably only need to do this near the boundary
                realCompositeGridFunction & vv = gf[m0].u;
                if( parameters.dbase.get<real >("ad41")>0. )
                {
          // fourth order
                    printf("...smooth u (4th-order)\n");
                    const real omega=1./32.;
                    for( int it=0; it<2; it++ )
                    {
            //	  vv.applyBoundaryCondition(V,BCTypes::extrapolateInterpolationNeighbours);
                        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                        {
      	// smooth interior points and periodic edges
                  	MappedGrid & mg = cg[grid];
                  	getIndex(mg.indexRange(),I1,I2,I3,-1); 
                  	for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
                  	{
                    	  if( mg.boundaryCondition(Start,axis)<0 )
                      	    Iv[axis]=Range(Iv[axis].getBase()-1,Iv[axis].getBound()+1); // +++++++++++++++++++++
                  	}
                  	realMappedGridFunction & v = vv[grid];
                  	v(I1,I2,I3,V)+=omega*(  -(v(I1+2,I2,I3,V)+v(I1-2,I2,I3,V)+v(I1,I2+2,I3,V)+v(I1,I2-2,I3,V))
                                    				+4.*(v(I1+1,I2,I3,V)+v(I1-1,I2,I3,V)+v(I1,I2+1,I3,V)+v(I1,I2-1,I3,V))
                                    				-12.*v(I1,I2,I3,V) );
                        }
                        interpolate(gf[m0],V);
                        vv.periodicUpdate(V);   // only need to do velocity components **********************h
                    }
                }
                else if( parameters.dbase.get<real >("ad21")>0. )
                {
                    printf("...smooth u (2nd-order)\n");
                    const real omega=1./8.;
                    for( int it=0; it<2; it++ )
                    {
                        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                        {
      	// smooth interior points and periodic edges
                  	MappedGrid & mg = cg[grid];
                  	getIndex(mg.extendedIndexRange(),I1,I2,I3); 
                  	if( TRUE )
                  	{
                    	  for( int axis=0; axis<cg.numberOfDimensions(); axis++ )
                    	  {
                      	    if( mg.boundaryCondition(Start,axis)>0 )
                        	      Iv[axis]=Range(Iv[axis].getBase()+1,Iv[axis].getBound());
                      	    if( mg.boundaryCondition(End  ,axis)>0 )
                        	      Iv[axis]=Range(Iv[axis].getBase(),Iv[axis].getBound()-1);
                    	  }
                  	}
                  	realMappedGridFunction & v = vv[grid];
                  	v(I1,I2,I3,V)+=omega*( 
                    	  v(I1+1,I2,I3,V)+v(I1-1,I2,I3,V)+v(I1,I2+1,I3,V)+v(I1,I2-1,I3,V)-4.*v(I1,I2,I3,V) );
                        }
            // vv.interpolate(V);
                        interpolate(gf[m0],V);
            // vv.applyBoundaryCondition(V,BCTypes::generalizedDivergence);
                        vv.periodicUpdate(V);   // only need to do velocity components **********************h
                    }
                }
            }
        }


        if( debug() & 16 )
        { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
            fprintf(debugFile,
            	      "allSpeedTimeStep: Before getUt t=%e dt=%e dtRatio=%e, n0=%i, n1=%i\n",t+dt,dt,dtRatio,n0,n1);
            fn[n0].display("fn[n0]",debugFile);
            fn[n1].display("fn[n1]",debugFile);
        }

    // AB is wrong if pL and rL are changed !

        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
      // getUt(grid, gf[m0][grid],t, fn[n0][grid]);
            rparam[0]=t;
            rparam[1]=t; // tforce
            rparam[2]=t+dt;    // tImplicit
            iparam[0]=grid;
            iparam[1]=gf[m0].cg.refinementLevelNumber(grid);
            getUt(gf[m0].u[grid],gf[m0].getGridVelocity(grid),fn[n0][grid],iparam,rparam,uti[grid]);
        }

        if( debug() & 16 )
        { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
            fprintf(debugFile,
            	      "allSpeedTimeStep: After getUt t=%e dt=%e dtRatio=%e\n",t+dt,dt,dtRatio);
            fn[n0].display("fn[n0]",debugFile);
            fn[n1].display("fn[n1]",debugFile);
        }

// ---------------------

    // extrapolate du/dt to the ghostline here
    // --needed for rhs to p equation (div(f))  [ note: tz flow fixes the  "u" part]
        if( newWay )
        {
            if( twilightZoneFlow )
                fn[n0].getOperators()->setTwilightZoneFlow( FALSE );  // turn off tz forcing in extrapolation!
        
      //    BoundaryConditionParameters extrapParams;
      //    extrapParams.dbase.get< >("orderOfExtrapolation")=4;
      //    fn[n0].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt,extrapParams);

            fn[n0].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt);

      // extrapolate rhs for p equation too -- neumann BC's ??
            fn[n0].applyBoundaryCondition(pc,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt);
            fn[n0].finishBoundaryConditions();  

            if( twilightZoneFlow )
                fn[n0].getOperators()->setTwilightZoneFlow( TRUE );
        }

// ------------------------

        real cpu0=getCPU();
        
        if( computeReactions )
        {
            for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	getIndex(cg[grid].dimension(),I1,I2,I3);
                if( useOpt )
      	{
                    #ifdef USE_PPP
                        realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0[grid],u0Local);
                    #else
                        realSerialArray & u0Local=u0[grid];
                    #endif
                    #ifdef USE_PPP
                        realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                    #else
                        realSerialArray & u1Local=u1[grid];
                    #endif
                    #ifdef USE_PPP
                        realSerialArray fnLocal; getLocalArrayWithGhostBoundaries(fn[n0][grid],fnLocal);
                    #else
                        realSerialArray & fnLocal=fn[n0][grid];
                    #endif
                    int includeGhost=1;
                    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
                    if( !ok ) continue;
        	  fnLocal(I1,I2,I3,rc)=(u0Local(I1,I2,I3,rc)-u1Local(I1,I2,I3,rc))*(1./dt);   // estimate rho.t
                }
      	else
      	{
        	  fn[n0][grid](I1,I2,I3,rc)=(u0[grid](I1,I2,I3,rc)-u1[grid](I1,I2,I3,rc))*(1./dt);   // estimate rho.t
      	}
      	
            }
        }

        if( debug() & 16 )
        { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
            fprintf(debugFile,
            	      "allSpeedTimeStep: Before increment t=%e dt=%e dtRatio=%e\n",t+dt,dt,dtRatio);
            u0.display("u0",debugFile);
            u1.display("u1",debugFile);
            fn[n0].display("fn[n0]",debugFile);
            fn[n1].display("fn[n1]",debugFile);
            
        }

        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            getIndex(gf[m1].cg[grid].gridIndexRange(),I1,I2,I3,1);  // include a ghost point for p

            if( useOpt )
            {
            #ifdef USE_PPP
                realSerialArray u0Local; getLocalArrayWithGhostBoundaries(u0[grid],u0Local);
            #else
                realSerialArray & u0Local=u0[grid];
            #endif
                int includeGhost=1;
      	bool ok = ParallelUtility::getLocalArrayBounds(u0[grid],u0Local,I1,I2,I3,includeGhost);
      	if( !ok ) continue;

            #ifdef USE_PPP
                realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
            #else
                realSerialArray & u1Local=u1[grid];
            #endif
            #ifdef USE_PPP
                realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
            #else
                realSerialArray & pLocal=p()[grid];
            #endif
            #ifdef USE_PPP
                realSerialArray fn0Local; getLocalArrayWithGhostBoundaries(fn[n0][grid],fn0Local);
            #else
                realSerialArray & fn0Local=fn[n0][grid];
            #endif
            #ifdef USE_PPP
                realSerialArray fn1Local; getLocalArrayWithGhostBoundaries(fn[n1][grid],fn1Local);
            #else
                realSerialArray & fn1Local=fn[n1][grid];
            #endif
            #ifdef USE_PPP
                realSerialArray fn2Local; getLocalArrayWithGhostBoundaries(fn[n2][grid],fn2Local);
            #else
                realSerialArray & fn2Local=fn[n2][grid];
            #endif


	// guess for p: This extrapolation is exact for linear functions in time but not quadratic.
      	pLocal(I1,I2,I3)=(1.+dtRatio)*u0Local(I1,I2,I3,pc)-dtRatio*u1Local(I1,I2,I3,pc);   // error O(dt^2)

	// advance momentum equations without grad(p) term. Advance T equation
      	u1Local(I1,I2,I3,N)=a1*u0Local(I1,I2,I3,N)+a2*u1Local(I1,I2,I3,N)
        	  + (b0*dt)*fn0Local(I1,I2,I3,N)+(b1*dt)*fn1Local(I1,I2,I3,N)+(b2*dt)*fn2Local(I1,I2,I3,N) ;

            }
            else
            {
	// guess for p: This extrapolation is exact for linear functions in time but not quadratic.
      	p()[grid](I1,I2,I3)=(1.+dtRatio)*u0[grid](I1,I2,I3,pc)-dtRatio*u1[grid](I1,I2,I3,pc);   // error O(dt^2)

	// **      fn[3][grid](I1,I2,I3,N)=b0*fn[n0][grid](I1,I2,I3,N)+b1*fn[n1][grid](I1,I2,I3,N);
            
	// advance momentum equations without grad(p) term. Advance T equation

      	u1[grid](I1,I2,I3,N)=a1*u0[grid](I1,I2,I3,N)+a2*u1[grid](I1,I2,I3,N)
        	  + (b0*dt)*fn[n0][grid](I1,I2,I3,N)+(b1*dt)*fn[n1][grid](I1,I2,I3,N)+(b2*dt)*fn[n2][grid](I1,I2,I3,N) ;

	// **   fn[n1][grid](I1,I2,I3,rc)=(b0*dt)*fn[n0][grid](I1,I2,I3,rc)+
        //                                (b1*dt)*fn[n1][grid](I1,I2,I3,rc); // holds div(f_1)
      	
            }
            
        }
        gf[m1].t=t+dt; // ***

        timing(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu0;


        if( debug() & 16 )
        { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
            fprintf(debugFile,
            	      "allSpeedTimeStep: After increment Errors at t+dt=%e dtRatio=%e (a1=%e,a2=%e,b0=%e,b1=%e,b2=%e)\n",
                              t+dt,dtRatio,a1,a2,b0,b1,b2);
            determineErrors( gf[m1] ); 
        }
    
        real cpu1=getCPU();
        if( algorithmVariation==AsfParameters::densityFromGasLawAlgorithm )
        {
      // interpolate T *** note *** interpolate sets unused points to ZERO
      // u1.interpolate(Range(tc,tc));
            interpolate(gf[m1],Range(tc,tc));

      // apply the BC for T 
            applyBoundaryConditions(gf[m1],tc);
        

      // ****************************************
      // *****  compute rho from p and T *******
      // ****************************************
              for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
              {
                  MappedGrid & mg = gf[m1].cg[grid];
                  getIndex(mg.dimension(),I1,I2,I3);
                  if( useOpt )
                  {
                      #ifdef USE_PPP
                          realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                      #else
                          realSerialArray & u1Local=u1[grid];
                      #endif
                      int includeGhost=1;
                      bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
                      if( !ok ) continue;
                      #ifdef USE_PPP
                          realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
                      #else
                          realSerialArray & pLocal=p()[grid];
                      #endif
                      #ifdef USE_PPP
                          intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
                      #else
                          intSerialArray & maskLocal=cg[grid].mask();
                      #endif
                      where( maskLocal(I1,I2,I3)!=0 )
                          u1Local(I1,I2,I3,rc)=(pLocal(I1,I2,I3)+pressureLevel)/(Rg*u1Local(I1,I2,I3,tc));
                      if( twilightZoneFlow )
                      {
                          const bool isRectangular = false;  // do this for now
                          realArray & x= mg.center();
            #ifdef USE_PPP
                          realSerialArray xLocal; 
                          if( !isRectangular ) 
                   	 getLocalArrayWithGhostBoundaries(x,xLocal);
            #else
                          const realSerialArray & xLocal = x;
            #endif
                          realSerialArray r0(I1,I2,I3), T0(I1,I2,I3), p0(I1,I2,I3);
                          e.gd(r0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,rc,t+dt);
                          e.gd(T0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,tc,t+dt);
                          e.gd(p0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,pc,t+dt);
             // this seemed to work best:
                          u1Local(I1,I2,I3,rc)*=r0*Rg*T0/(p0+pressureLevel);
                      }
                  }
                  else
                  {
                      where( mg.mask()(I1,I2,I3)!=0 )
                          u1[grid](I1,I2,I3,rc)=(p()[grid](I1,I2,I3)+pressureLevel)/(Rg*u1[grid](I1,I2,I3,tc));
                      if( twilightZoneFlow )
                      {
             //	  u1[grid](I1,I2,I3,rc)+=e(c,I1,I2,I3,rc,t+dt)
             //	    -(e(c,I1,I2,I3,pc,t+dt)+pressureLevel)/(Rg*e(c,I1,I2,I3,tc,t+dt));
             //     	  u1[grid](I1,I2,I3,rc)=e(c,I1,I2,I3,rc,t+dt);
                          u1[grid](I1,I2,I3,rc)*=e(mg,I1,I2,I3,rc,t+dt)*(Rg*e(mg,I1,I2,I3,tc,t+dt))/
                   	 (e(mg,I1,I2,I3,pc,t+dt)+pressureLevel);
                      }
                  }
              }

        }
        else
        {
      // *wdh* 070109
      // values needed for rho on ghost lines here -- do this for now
            u1.applyBoundaryCondition(rc,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt);
        }
        timing(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-cpu1;    

        if( debug() & 16 )
        { // errors should be zero since both grad(p)/rho and forcing grad(P)/R are missing
            fprintf(debugFile,
            	      "allSpeedTimeStep: SUBSTEP Errors at t+dt=%e (note: r,T errors should be zero here, "
            	      " others zero at interior)\n",t+dt);
            determineErrors( gf[m1] ); 
        }

        for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            addAllSpeedImplicitForcing( u1[grid],t+dt,dt,grid );

    // interpolate rhs functions so we can differentiate them
    // u1.interpolate();  
        interpolate(gf[m1]);  

        if( !newWay )
        {
            cpu1=getCPU();
      // extrapolate the values held in the velocity components ( f = u(t+dt)-a0*dt)p.x/r )
      // --needed for rhs to p equation (div(f))  [ note: tz flow fixes the  "u" part]
            if( twilightZoneFlow )
      	u1.getOperators()->setTwilightZoneFlow( FALSE );  // turn off tz forcing in extrapolation!
        
//    BoundaryConditionParameters extrapParams;
//    extrapParams.dbase.get< >("orderOfExtrapolation")=4;
//    u1.applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt,extrapParams);

            u1.applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt);

      // extrapolate rhs for p equation too -- neumann BC's ??
            u1.applyBoundaryCondition(pc,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt);
            u1.finishBoundaryConditions();  

//    fn[3].setOperators(*gf[m1].u.getOperators());
//    fn[3].applyBoundaryCondition(V,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t+dt); // ?? is this needed?

            if( twilightZoneFlow )
      	u1.getOperators()->setTwilightZoneFlow( TRUE );

            timing(parameters.dbase.get<int>("timeForBoundaryConditions"))+=getCPU()-cpu1;    

        }
        


    // ******************************************************
    // *** solve for the pressure, compute p at time t+dt ***
    // ******************************************************
        if( cg.numberOfDimensions()==1 && computeReactions )  // ************************ fix this *********
        {
      // *** laminar flame: compute u and p from analytical formulae for now:
            int i1=cg[0].gridIndexRange()(Start,axis1);
            int i2=cg[0].gridIndexRange()(Start,axis2);
            int i3=cg[0].gridIndexRange()(Start,axis3);
            real dxb2=cg[0].gridSpacing()(axis1)*.5;     // only valid for the unit line
            for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	getIndex(cg[grid].dimension(),I1,I2,I3);
	// (rho*u).x = -rho.t  -> u(x) = rho(0)*u(0)/rho(x) - 1/rho(x) * integral_0^x{ rho.t dx }
                i1=cg[0].gridIndexRange()(Start,axis1);
      	u1[grid](I1,I2,I3,uc)=u0[0](i1,i2,i3,rc)*u1[0](i1,i2,i3,uc)/u0[grid](I1,I2,I3,rc);
                real sum=0; // holds the integral of rho.t
      	for( i1=I1.getBase()+1; i1<=I1.getBound(); i1++ )
      	{
        	  sum+=( fn[n0][grid](i1,i2,i3,rc)+fn[n0][grid](i1-1,i2,i3,rc) )*dxb2; // mid point rule
        	  u1[grid](i1,i2,i3,uc)-=sum/u0[0](i1,i2,i3,rc);
      	}
	// p = - rho u^2 in steady inviscid
	// p()[grid](I1,I2,I3)=-u[0](I1,I2,I3,rc)*SQR(u[grid](I1,I2,I3,uc));   // *** outflow value should be zero ??
      	p()[grid](I1,I2,I3)=0.;
      	u1[grid](I1,I2,I3,pc)=p()[grid](I1,I2,I3);
            }
      // **** add grad(p) terms *******??
            printf("\n\n ***********************  Check this in asfp. ******************** \n\n");
      // throw "error";
            
        }
        else
        {
            
            solveForAllSpeedPressure(t+dt,dt,dtRatio); 
            
            cpu1=getCPU();
            
            if( parameters.dbase.get<bool>("useDivergenceBoundaryCondition") )
            {
        // >>> save in f the rhs for the BC: div(u)=( f_2 - p(n+1) )/( a0*dt*gamma*p(n+1) ) 
      	f=0.;   // ***** only needed for max below ******
	// **** should use gam() here if there are species.
      	for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      	{ 
	  // This is really only needed on true boundaries: **********************************
        	  getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
                    #ifdef USE_PPP
                        realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                    #else
                        realSerialArray & u1Local=u1[grid];
                    #endif
                    int includeGhost=1;
                    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
                    if( !ok ) continue;

                    #ifdef USE_PPP
                        realSerialArray fLocal; getLocalArrayWithGhostBoundaries(f[grid],fLocal);
                    #else
                        realSerialArray & fLocal=f[grid];
                    #endif
                    #ifdef USE_PPP
                        realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
                    #else
                        realSerialArray & pLocal=p()[grid];
                    #endif

        	  fLocal(I1,I2,I3)=(u1Local(I1,I2,I3,pc)-pLocal(I1,I2,I3))/(a0*deltaT*gamma*(pLocal(I1,I2,I3)+pressureLevel));
          
      	}
	// cout << "RHS for div(u) BC : max(abs(rhs)) = " << max(fabs(f)) << endl;
            }
            
            if( !useOpt )
            {
      	px()=p().x();
            }

            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	getIndex(cg[grid].extendedIndexRange(),I1g,I2g,I3g,1);
                if( useOpt )
      	{
                    #ifdef USE_PPP
                        realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                    #else
                        realSerialArray & u1Local=u1[grid];
                    #endif
                    #ifdef USE_PPP
                        realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
                    #else
                        realSerialArray & pLocal=p()[grid];
                    #endif
                    int includeGhost=1;
                    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1g,I2g,I3g,includeGhost);
                    if( !ok ) continue;
                    u1Local(I1g,I2g,I3g,pc)=pLocal(I1g,I2g,I3g);
        	  
      	}
      	else
      	{
        	  u1[grid](I1g,I2g,I3g,pc)=p()[grid](I1g,I2g,I3g);
      	}
      	
            }
        
            if( algorithmVariation==AsfParameters::densityFromGasLawAlgorithm )
            {
	// *****  compute a corrected value for rho from p and T *******
                  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
                  {
                      MappedGrid & mg = gf[m1].cg[grid];
                      getIndex(mg.dimension(),I1,I2,I3);
                      if( useOpt )
                      {
                          #ifdef USE_PPP
                              realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                          #else
                              realSerialArray & u1Local=u1[grid];
                          #endif
                          int includeGhost=1;
                          bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
                          if( !ok ) continue;
                          #ifdef USE_PPP
                              realSerialArray pLocal; getLocalArrayWithGhostBoundaries(p()[grid],pLocal);
                          #else
                              realSerialArray & pLocal=p()[grid];
                          #endif
                          #ifdef USE_PPP
                              intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
                          #else
                              intSerialArray & maskLocal=cg[grid].mask();
                          #endif
                          where( maskLocal(I1,I2,I3)!=0 )
                              u1Local(I1,I2,I3,rc)=(pLocal(I1,I2,I3)+pressureLevel)/(Rg*u1Local(I1,I2,I3,tc));
                          if( twilightZoneFlow )
                          {
                              const bool isRectangular = false;  // do this for now
                              realArray & x= mg.center();
                #ifdef USE_PPP
                              realSerialArray xLocal; 
                              if( !isRectangular ) 
                       	 getLocalArrayWithGhostBoundaries(x,xLocal);
                #else
                              const realSerialArray & xLocal = x;
                #endif
                              realSerialArray r0(I1,I2,I3), T0(I1,I2,I3), p0(I1,I2,I3);
                              e.gd(r0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,rc,t+dt);
                              e.gd(T0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,tc,t+dt);
                              e.gd(p0 ,xLocal,mg.numberOfDimensions(),isRectangular,0,0,0,0,I1,I2,I3,pc,t+dt);
               // this seemed to work best:
                              u1Local(I1,I2,I3,rc)*=r0*Rg*T0/(p0+pressureLevel);
                          }
                      }
                      else
                      {
                          where( mg.mask()(I1,I2,I3)!=0 )
                              u1[grid](I1,I2,I3,rc)=(p()[grid](I1,I2,I3)+pressureLevel)/(Rg*u1[grid](I1,I2,I3,tc));
                          if( twilightZoneFlow )
                          {
               //	  u1[grid](I1,I2,I3,rc)+=e(c,I1,I2,I3,rc,t+dt)
               //	    -(e(c,I1,I2,I3,pc,t+dt)+pressureLevel)/(Rg*e(c,I1,I2,I3,tc,t+dt));
               //     	  u1[grid](I1,I2,I3,rc)=e(c,I1,I2,I3,rc,t+dt);
                              u1[grid](I1,I2,I3,rc)*=e(mg,I1,I2,I3,rc,t+dt)*(Rg*e(mg,I1,I2,I3,tc,t+dt))/
                       	 (e(mg,I1,I2,I3,pc,t+dt)+pressureLevel);
                          }
                      }
                  }
            }
            
      // correct the velocities 
            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
      	
                if( useOpt )
      	{
                    MappedGrid & mg =cg[grid];
    
                    #ifdef USE_PPP
                        realSerialArray u1Local; getLocalArrayWithGhostBoundaries(u1[grid],u1Local);
                    #else
                        realSerialArray & u1Local=u1[grid];
                    #endif
                    #ifdef USE_PPP
                        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(cg[grid].mask(),maskLocal);
                    #else
                        intSerialArray & maskLocal=cg[grid].mask();
                    #endif

                    int includeGhost=1;
                    bool ok = ParallelUtility::getLocalArrayBounds(u1[grid],u1Local,I1,I2,I3,includeGhost);
                    if( !ok ) continue;

                    bool isRectangular=mg.isRectangular();
        	  real dx[3]={1.,1.,1.};
        	  if( isRectangular )
          	    mg.getDeltaX(dx);

                    real *pu=u1Local.getDataPointer();
        	  int *pmask=maskLocal.getDataPointer();

          // rho is either the linearized version or the current value
                    real *prho= linearizeImplicitMethod ? rL()[grid].getLocalArray().getDataPointer() :
                                	          &u1Local(u1Local.getBase(0),u1Local.getBase(1),u1Local.getBase(2),rc);
                    real *prsxy = isRectangular ? pu : cg[grid].inverseVertexDerivative().getLocalArray().getDataPointer();

                    int gridType = isRectangular? 0 : 1;
                    int orderOfAccuracy=parameters.dbase.get<int >("orderOfAccuracy");
        	  const int gridIsMoving = parameters.gridIsMoving(grid);
        	  int useWhereMask=true;
        	  int ipar[]={rc,uc,vc,wc,tc,pc,grid,gridType,orderOfAccuracy,gridIsMoving,useWhereMask,
                                            I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound()};  //
                      
                    const real alpha = (1.-cImplicit)*(-dt*a0);
        	  real rpar[]={dx[0],dx[1],dx[2],mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2),t,dt,alpha};  //

                    DataBase *pdb = &parameters.dbase;
                    int ierr=0;

          // Add the (grad(p)/rho term to the velocity equations:
          //       u(i1,i2,i3,uc..) += alpha*grad(p)/rho
          // where rho is either the current density or the linearized density
                    asfAddGradP(cg[grid].numberOfDimensions(),
                                            u1Local.getBase(0),u1Local.getBound(0),
                                            u1Local.getBase(1),u1Local.getBound(1),
                                            u1Local.getBase(2),u1Local.getBound(2),
                                            u1Local.getBase(3),u1Local.getBound(3),
                  		      ipar[0],rpar[0],*pu,*prho,*pmask,*prsxy,pdb,ierr);

      	}
      	else
      	{
        	  if( !linearizeImplicitMethod )
        	  {
          	    const realArray & dtOverRho = evaluate( ((1.-cImplicit)*(-dt*a0))/u1[grid](I1,I2,I3,rc) );

          	    u1[grid](I1,I2,I3,uc)+=px()[grid](I1,I2,I3)*dtOverRho;
          	    if( cg[grid].numberOfDimensions()>1 )    
            	      u1[grid](I1,I2,I3,vc)+=p()[grid].y(I1,I2,I3)(I1,I2,I3)*dtOverRho;
          	    if( cg[grid].numberOfDimensions()>2 )    
            	      u1[grid](I1,I2,I3,wc)+=p()[grid].z(I1,I2,I3)(I1,I2,I3)*dtOverRho;
        	  }
        	  else
        	  { // linearize the implicit method
          	    realMappedGridFunction & r0 = rL()[grid];
          	    const realArray & dtOverRho = evaluate( ((1.-cImplicit)*(-dt*a0))/r0(I1,I2,I3) );

          	    u1[grid](I1,I2,I3,uc)+=px()[grid](I1,I2,I3)*dtOverRho;
          	    if( cg[grid].numberOfDimensions()>1 )    
            	      u1[grid](I1,I2,I3,vc)+=p()[grid].y(I1,I2,I3)(I1,I2,I3)*dtOverRho;
          	    if( cg[grid].numberOfDimensions()>2 )    
            	      u1[grid](I1,I2,I3,wc)+=p()[grid].z(I1,I2,I3)(I1,I2,I3)*dtOverRho;
        	  }
      	}
            }
            
        }
        
        if( cg.numberOfDimensions()==1 && computeReactions )  // ****** fix this
        {
#ifdef USE_PPP
        Overture::abort("Error- fix this Bill");
#else
      // Solve the reaction equations
            Range S0(0,numberOfSpecies-1);
            Range S(sc,sc+numberOfSpecies-1);
            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
                getIndex(cg[grid].extendedIndexRange(),I1,I2,I3);
      	realArray rhs(I1,I2,I3,Range(0,numberOfSpecies));  // ****** fix this
      	rhs(I1,I2,I3,0)=u1[grid](I1,I2,I3,tc);
      	rhs(I1,I2,I3,S0+1)=u1[grid](I1,I2,I3,S);
      	
      	parameters.dbase.get<Reactions* >("reactions")->solveImplicitForRTYGivenP(u1[grid],rhs,rc,pc,tc,sc,
            		I1,I2,I3,dt*parameters.dbase.get<real >("l0")/parameters.dbase.get<real >("u0"));
            }
            if( debug() & 16 )
            {
                fprintf(debugFile," allSpeed: After solveImplicitForRTYGivenP, t=%e \n",gf[m1].t);
        // determineErrors( cg,gf[m1],gf.gridVelocity, t+dt, 0, error );   // *********
                gf[m1].u.display("gf[m1].u",debugFile) ;
            }
#endif      
        }

        timing(parameters.dbase.get<int>("timeForAddUt"))+=getCPU()-cpu1;
        
    // u1.interpolate();
        interpolate(gf[m1]);

        t+=dt;
        gf[m1].t=t;

        if( parameters.dbase.get<bool>("useDivergenceBoundaryCondition") )
        {
      // fill in the rhs for the divergence boundary condition  **** fix this ********************************
      //     div(u) = div(u)=(-1\r)[ r.t+u.grad(rho) ]

            for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
            {
      	MappedGrid & c = cg[grid];
      	int side,axis;
      	ForBoundary(side,axis)
      	{
        	  Index I1b,I2b,I3b;
        	  getBoundaryIndex(c.gridIndexRange(),side,axis,I1b,I2b,I3b);
        	  if( c.boundaryCondition()(side,axis) > 0 )
        	  {
          	    f[grid](I1b,I2b,I3b)=0.;
        	  }
      	}
            }
        }
        

        applyBoundaryConditions(gf[m1]);


        if( debug() & 16 )
        {
            fprintf(debugFile,"allSpeedTimeStep: After applyBC, Errors at t=%e \n",t);
      // determineErrors( cg,gf[m1],gf.gridVelocity, t+dt, 0, error );   // *********
            determineErrors( gf[m1] ) ;
        }

    // permute (m0<->m1) (n2->n1->n0->n2)
        m0=(m0-1+2) % 2;
        m1=(m1-1+2) % 2;
        
        n0=(n0-1 +3) % 3;
        n1=(n1-1 +3) % 3;
        n2=(n2-1 +3) % 3;
    }
  // set current solution 
    current = m0;
    
}

