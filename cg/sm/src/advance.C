// This file automatically generated from advance.bC with bpp.
#include "Cgsm.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "SmParameters.h"
#include "Regrid.h"

// ======================================================================================================
//  This macro will update the grids and grid functions when using AMR
// ======================================================================================================

// fourth order dissipation 2D: ***** NOTE: this is minus of the 4th difference:  -(D+D-)^2 *********
#define FD4_2D(u,i1,i2,i3,c) (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) -12.*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4_3D(u,i1,i2,i3,c) (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) -18.*u(i1,i2,i3,c) )

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FN(m) fn[m+numberOfFunctions*(grid)]

// ===================================================================================================================
// *************** This is OLD **********************
/// \brief Advance one time-step. This function is used by the multi-physics solver Cgmp.
/// \param t (input) : current time.
/// \param dt (input) : time step.
/// \param stepNumber (input) : current counter for the step number.
/// \param numberOfSubSteps (input) : number of sub-steps to take.
// ===================================================================================================================
// void Cgsm::
// takeOneStep( real & t, real & dt, int stepNumber, int & numberOfSubSteps )
// {
//   real time0=getCPU();

//   printF(" Cgsm::takeOneStep: stepNumber=%i : t=%9.3e, dt=%9.3e, current=%i\n",stepNumber,t,dt,current);
//   if( stepNumber<=0 ) // do this for now ****************** fix this *******************************************
//   {
//     printF(" Cgsm::takeOneStep: stepNumber=%i : updateForNewTimeStep, dt=%9.3e\n",stepNumber,dt);
        
//     // For linear-elasticity this next method will compute the solution at t-dt
//     updateForNewTimeStep( gf[current],dt );
//   }
    


//   for( int step=0; step<numberOfSubSteps; step++ )
//   {
//     advance(  current,t,dt );

//     numberOfStepsTaken++;
//     current= (current+1) % numberOfTimeLevels;
//   }
//   RealArray & timing = parameters.dbase.get<RealArray >("timing");
//   timing(SmParameters::totalTime)+=getCPU()-time0;
// }


// =============================================================================
/// \brief Advance the solution one time step.
// =============================================================================
void Cgsm::
advance(  int current, real t, real dt )
{
    checkArrays("advanceLinearElasticity:start");
    int & globalStepNumber = parameters.dbase.get<int >("globalStepNumber");
    globalStepNumber++;

//    printF("advance: t=%e current=%i, numberOfFunctions=%i, numberOfTimeLevels=%i\n",t,
//        current,numberOfFunctions,numberOfTimeLevels);
    FILE *& debugFile  =parameters.dbase.get<FILE* >("debugFile");
    FILE *& logFile    =parameters.dbase.get<FILE* >("logFile");
    FILE *& pDebugFile =parameters.dbase.get<FILE* >("pDebugFile");
    
    const int numberOfDimensions = cg.numberOfDimensions();
    const int numberOfComponentGrids = cg.numberOfComponentGrids();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");
    const int & orderOfAccuracyInSpace = parameters.dbase.get<int>("orderOfAccuracy");
    const int & orderOfAccuracyInTime  = parameters.dbase.get<int>("orderOfTimeAccuracy");

    SmParameters::TimeSteppingMethodSm & timeSteppingMethodSm = 
                                                                      parameters.dbase.get<SmParameters::TimeSteppingMethodSm>("timeSteppingMethodSm");
    RealArray & timing = parameters.dbase.get<RealArray >("timing");

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    Range C=numberOfComponents;
    const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
    const int next = (current+1) % numberOfTimeLevels;

    parameters.dbase.get<int>("currentGF")=current;
    parameters.dbase.get<int>("nextGF")=next;

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    bool & gridHasMaterialInterfaces = parameters.dbase.get<bool>("gridHasMaterialInterfaces");
    int & debug = parameters.dbase.get<int >("debug");

    const real cMax=max(lambdaGrid+muGrid)/rho;


    if( debug & 4 )
    {
        getErrors( prev   ,t-dt,dt,sPrintF("\n ******** advance: Errors in prev    at start, t=%9.3e ********\n",t-dt) );
        getErrors( current,t   ,dt,sPrintF("\n ******** advance: Errors in current at start, t=%9.3e ********\n",t) );
    }

  // ------ AMR -----
  // --- Start AMR ---
    const int regridFrequency = parameters.dbase.get<int >("amrRegridFrequency")>0 ? 
                                                            parameters.dbase.get<int >("amrRegridFrequency") :
                                                            parameters.dbase.get<Regrid* >("regrid")==NULL ? 2 : 
                                                            parameters.dbase.get<Regrid* >("regrid")->getRefinementRatio();
    if( parameters.isAdaptiveGridProblem() && ((globalStepNumber % regridFrequency) == 0) )
    {
    // ****************************************************************************
    // ****************** Adaptive Grid Step  *************************************
    // ****************************************************************************
        if( debug & 2 )
        {
            printP("***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
            fPrintF(debugFile,"***** advance: AMR regrid at step %i t=%e dt=%8.2e***** \n",globalStepNumber,t,dt);
        }
        real timea=getCPU();
        if( debug & 4 )
            fPrintF(debugFile,"\n ***** advance: AMR regrid at step %i ***** \n\n",globalStepNumber);
        if( debug & 8 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
                getErrors( current,t,dt,sPrintF(" advance: errors before regrid, t=%e \n",t) );
            }
            else
            {
                fPrintF(debugFile," ***advance: before regrid: solution ***\n");
                outputSolution( gf[current].u,t );
            }
        }
        int numberToUpdate=0; // we need to update ub to live on the new grid, and interpolate values.
        if( ((SmParameters&)parameters).isSecondOrderSystem() )
        {
            numberToUpdate=1;  // also update and interpolate prev solution to the new grid 
        }
        adaptGrids( gf[current], numberToUpdate,&(gf[prev].u), NULL );  // last arg is for work-space **fix me **
    // *wdh* do this: 090315
        cg.reference(gf[current].cg);
              gf[prev].cg.reference(gf[current].cg);
              gf[next].cg.reference(gf[current].cg);
              gf[next].u.updateToMatchGrid(cg);
    // printF(" After adaptGrids: prev=%i, current=%i, next=%i\n",prev,current,next);
        for( int n=0; n<numberOfTimeLevels; n++ )
        {
            if( n!=current )
            {
          	gf[n].cg.reference(gf[current].cg);  //
          	if( n!=prev ) // this was already done for prev in adaptGrids
            	  gf[n].u.updateToMatchGrid(gf[current].cg);
            }
            gf[n].u.setOperators(*cgop);
      // printF(" After adaptGrids: gf[%i].cg.numberOfComponentGrids = %i\n",n,gf[n].cg.numberOfComponentGrids());
      // printF(" After adaptGrids: gf[%i].u.getCompositeGrid()->numberOfComponentGrids = %i\n",n,gf[n].u.getCompositeGrid()->numberOfComponentGrids());
        }
    // ** do this for now ** fix me **
        if( checkErrors )
        {
            assert( cgerrp!=NULL );
            (*cgerrp).updateToMatchGrid(cg);
        }
    // the next has been moved into adaptGrids 070706
    //     real time1=getCPU();
    //     cgf1.cg.rcData->interpolant->updateToMatchGrid( cgf1.cg ); 
    //     parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForUpdateInterpolant"))+=getCPU()-time1;
        real time1=getCPU();
        if( debug & 8 )
        {
            outputSolution( gf[current].u,t,
                                          sPrintF(" advance:after adaptGrids, before interpAndApplyBC at t=%11.4e \n",t) );
        }
        interpolateAndApplyBoundaryConditions( gf[current] );
    // *wdh* 090829
        if( numberToUpdate==1 )
        {
            interpolateAndApplyBoundaryConditions( gf[prev] );
        }
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrBoundaryConditions"))+=getCPU()-time1;    
        if( debug & 4 )
        {
            if( parameters.dbase.get<bool >("twilightZoneFlow") )
            {
                getErrors( prev   ,t-dt,dt,sPrintF(" advance: errors in prev    after regrid, t=%e \n",t-dt) );
                getErrors( current,t   ,dt,sPrintF(" advance: errors in current after regrid, t=%e \n",t) );
            }
            else
            {
                fPrintF(debugFile," ***after regrid: solution ***\n");
                outputSolution( gf[current].u,t );
            }
        }
        parameters.dbase.get<RealArray>("timing")(parameters.dbase.get<int>("timeForAmrRegrid"))+=getCPU()-timea;
    }
  // --- End AMR ---
    

    if( ((SmParameters&)parameters).isSecondOrderSystem() )
    {
    // advance the solution as a second-order system (do not apply BCs)
        advanceSOS( current,t,dt );
    }
    else
    {
    // advance the solution as a first order system
        advanceFOS( current,t,dt );

    }
    
            
//   if( orderOfAccuracyInTime>=4 )
//   {
//     assert( numberOfFunctions>0 );
//     currentFn=(currentFn+orderOfAccuracyInTime-2)%numberOfFunctions;
//   }
    

    if( true ||   // *wdh* 091205 -- interpolate will call periodicUpdate and updateGhost 
            cg.numberOfComponentGrids()>1 )
    {
        real timei=getCPU();
    
        if( debug & 4 )
            gf[next].u.display(sPrintF("Cgsm::advance: gf[next].u before interpolate, t=%8.2e",t+dt),debugFile,"%8.2e ");

    // Note: interpolate performs a periodicUpdate and updateGhostBoundaries even if there is only one grid

        gf[next].u.interpolate();
  
        if( debug & 4 )
            gf[next].u.display(sPrintF("Cgsm::advance: gf[next].u after interpolate, t=%8.2e",t+dt),debugFile,"%8.2e ");

        if( debug & 4 )
        {
            getErrors( next,t+dt,dt,sPrintF("\n ************** advance Errors after interpolate t=%9.3e ******\n",t+dt));
        }

        timing(parameters.dbase.get<int>("timeForInterpolate"))+=getCPU()-timei;
    }

    gf[next].t=t+dt;


  // ============= Boundary Conditions =============
    int option=0; // not used.
    applyBoundaryConditions( option, dt, next,current ); // apply BC to "next" (current=previous time step)
    

    if( debug & 8 )  // & 64
    {
        gf[next].u.display(sPrintF("Cgsm::advance: gf[next].u after applyBC, t=%8.2e",gf[next].t),debugFile,"%8.2e ");
    }
    

  // ---- assign values at material interfaces ------
  // *** this does nothing currently ***
    assignInterfaceBoundaryConditions( next, t, dt );  // is this the right place to do this?

    

    if( debug & 4 )
    {
        getErrors( next,t+dt,dt,sPrintF("\n *********** advance: Errors at end t=%9.3e ********\n",t+dt) );
    }


    checkArrays("advance:end");
    
}


void Cgsm::
computeDissipation( int current, real t, real dt )
// =====================================================================================
// Compute the dissipation for the structured grid algorithm
// =====================================================================================
{
    if( artificialDissipation<=0. )
        return ;

    real time0=getCPU();
    
    const int numberOfDimensions = cg.numberOfDimensions();
    const int & numberOfComponents = parameters.dbase.get<int >("numberOfComponents");
    const int & uc =  parameters.dbase.get<int >("uc");
    const int & vc =  parameters.dbase.get<int >("vc");
    const int & wc =  parameters.dbase.get<int >("wc");
    const int & rc =  parameters.dbase.get<int >("rc");
    const int & tc =  parameters.dbase.get<int >("tc");

    Index I1,I2,I3;
    Range C=numberOfComponents; 
    const int next = (current+1) % numberOfTimeLevels;

    if( cgdissipation==NULL )
    {
        Range all;
        cgdissipation=new realCompositeGridFunction;
        cgdissipation->updateToMatchGrid(cg,all,all,all,C);

        cgdissipation->setName("u dissipation",uc);
        cgdissipation->setName("v dissipation",vc);
        if( numberOfDimensions==3 )
        {
        cgdissipation->setName("w dissipation",wc);
        }
    }
    
    assert( cgdissipation!=NULL );
    realCompositeGridFunction & cgdiss = (*cgdissipation);

    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    const real cMax=max(muGrid+lambdaGrid);

    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        MappedGrid & mg = cg[grid];

    
        realMappedGridFunction & fieldCurrent =gf[current].u[grid];
        realMappedGridFunction & fieldNext    =gf[next].u[grid];

        realArray & u = fieldCurrent;
        realArray & un =fieldNext;

        realArray & d = cgdiss[grid];

    // const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
        const real adc=artificialDissipation; // do not scale *wdh* 090216

        int extra=2;
        getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

        d(I1,I2,I3,C)=u(I1,I2,I3,C)-un(I1,I2,I3,C);

        getIndex(mg.gridIndexRange(),I1,I2,I3);


        const intArray & mask = mg.mask();
        where( mask(I1,I2,I3)>0 )
        {
            for( int c=C.getBase(); c<=C.getBound(); c++ )
            {
      	if( orderOfArtificialDissipation==4 )
      	{
        	  if( numberOfDimensions==2 )
        	  {
          	    d(I1,I2,I3,c)=(adc*dt)*FD4_2D(d,I1,I2,I3,c);
//	    d(I1,I2,I3,c)=(cd(I1,I2,I3,c)*dt)*FD4_2D(d,I1,I2,I3,c);

        	  }
        	  else
          	    d(I1,I2,I3,c)=(adc*dt)*FD4_3D(d,I1,I2,I3,c);
      	}
      	else if( orderOfArtificialDissipation==8 )
      	{
        	  if( numberOfDimensions==2 )
          	    d(I1,I2,I3,c)=FD4_2D(d,I1,I2,I3,c);
        	  else
          	    d(I1,I2,I3,c)=FD4_3D(d,I1,I2,I3,c);
      	}
      	else
      	{
        	  Overture::abort();
      	}
            }
            
        }
        otherwise()
        {
            for( int c=C.getBase(); c<=C.getBound(); c++ )
                d(I1,I2,I3,c)=0.;
        }
        
    }

    cgdiss.periodicUpdate();  // is this needed?
    
    if( orderOfArtificialDissipation==8 )
    {
    // For this case we interpolate and apply BC's to the 4th order dissipation and
    // then take another 4th order difference


        cgdiss.interpolate();  

    // BC's *** do this for now ***
        assert( cgop!=NULL );
        CompositeGridOperators & operators = (*cgop);

        BoundaryConditionParameters bcParams;

    // *** for now we just set the dissipation to zero at the boundary and two lines in 

//      operators.applyBoundaryCondition(cgdiss, C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,bcParams);
//      bcParams.lineToAssign=-1;
//      operators.applyBoundaryCondition(cgdiss, C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,bcParams);

//    bcParams.orderOfExtrapolation=4;
//    operators.applyBoundaryCondition(cgdiss, C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,bcParams);
//    bcParams.ghostLineToAssign=2;
//    operators.applyBoundaryCondition(cgdiss, C,BCTypes::extrapolate,BCTypes::allBoundaries,0.,t,bcParams);

//    operators.applyBoundaryCondition(cgdiss, C,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.);
//    bcParams.ghostLineToAssign=2;
//    operators.applyBoundaryCondition(cgdiss, C,BCTypes::evenSymmetry,BCTypes::allBoundaries,0.,t,bcParams);

        operators.finishBoundaryConditions(cgdiss);
        
        for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
        {
            MappedGrid & mg = cg[grid];
            getIndex(mg.gridIndexRange(),I1,I2,I3);
            realArray & d = (*cgdissipation)[grid];
      // const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
            const real adc=artificialDissipation; // do not scale *wdh* 090216

            const intArray & mask = mg.mask();
            where( mask(I1,I2,I3)>0 )
            {
      	for( int c=C.getBase(); c<=C.getBound(); c++ )
      	{
          // NOTE: minus sign since FD4 is minus the 4th difference
        	  if( numberOfDimensions==2 )
          	    d(I1,I2,I3,c)=(-adc*dt)*FD4_2D(d,I1,I2,I3,c);
        	  else
          	    d(I1,I2,I3,c)=(-adc*dt)*FD4_3D(d,I1,I2,I3,c);
      	}
            }
            
        }

        bcParams.lineToAssign=0;
        operators.applyBoundaryCondition(cgdiss, C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,bcParams);
        bcParams.lineToAssign=-1;
        operators.applyBoundaryCondition(cgdiss, C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,bcParams);
        bcParams.lineToAssign=-2;
        operators.applyBoundaryCondition(cgdiss, C,BCTypes::dirichlet,BCTypes::allBoundaries,0.,t,bcParams);

        cgdiss.periodicUpdate();


    }
    RealArray & timing = parameters.dbase.get<RealArray >("timing");
    timing(parameters.dbase.get<int>("timeForDissipation"))+=getCPU()-time0;
}


// ***********************************************************************8

#include "Cgsm.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "display.h"


//! Add in an artificial dissipation
/*!
    \param C (input) : apply to these components.
  */
int Cgsm::
addDissipation( int current, real t, real dt, realMappedGridFunction *field, const Range & C )
{
    if( artificialDissipation<=0. ) 
        return 0;
    

    realArray & u = field[current];
    int next = (current+1) % numberOfTimeLevels;
    realArray & un =field[next];

    MappedGrid & mg = *(field[0].getMappedGrid());
    
    Index I1,I2,I3;
    getIndex(mg.gridIndexRange(),I1,I2,I3);

//   assert( dissipation!=NULL );
//   realMappedGridFunction & diss = *dissipation;

    realMappedGridFunction diss(mg);
    realArray & d = diss;

    int n;
    for( n=C.getBase(); n<=C.getBound(); n++ )
    {
    //    d(I1,I2,I3)=-4.*u(I1,I2,I3,n)+u(I1+1,I2,I3,n)+u(I1-1,I2,I3,n)+u(I1,I2+1,I3,n)+u(I1,I2-1,I3,n);

        d(I1,I2,I3)=(-8./3.)*u(I1,I2,I3,n)+
            (1./3.)*(u(I1+1,I2,I3,n)+u(I1-1,I2,I3,n)+u(I1,I2+1,I3,n)+u(I1,I2-1,I3,n)+
             	       u(I1-1,I2-1,I3,n)+u(I1+1,I2-1,I3,n)+u(I1-1,I2+1,I3,n)+u(I1+1,I2+1,I3,n));

    
        if( orderOfArtificialDissipation==2 )
        {
            un(I1,I2,I3,n)+=(artificialDissipation*dt)*d(I1,I2,I3);
        }
        else
        {
            diss.periodicUpdate();

      //       d(I1,I2,I3)=-4.*d(I1,I2,I3)+d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3);
            d(I1,I2,I3)=(-8./3.)*d(I1,I2,I3)+
      	(1./3.)*(d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3)+
             		 d(I1-1,I2-1,I3)+d(I1+1,I2-1,I3)+d(I1-1,I2+1,I3)+d(I1+1,I2+1,I3));
            if( orderOfArtificialDissipation==4 )
            {
	// fourth-order dissipation
      	un(I1,I2,I3,n)+=(-artificialDissipation*dt)*d(I1,I2,I3);
            }
            else if( orderOfArtificialDissipation==6 )
            {
      	diss.periodicUpdate();

	// d(I1,I2,I3)=-4.*d(I1,I2,I3)+d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3);
      	d(I1,I2,I3)=(-8./3.)*d(I1,I2,I3)+
        	  (1./3.)*(d(I1+1,I2,I3)+d(I1-1,I2,I3)+d(I1,I2+1,I3)+d(I1,I2-1,I3)+
               		   d(I1-1,I2-1,I3)+d(I1+1,I2-1,I3)+d(I1-1,I2+1,I3)+d(I1+1,I2+1,I3));

	// sixth-order dissipation
      	un(I1,I2,I3,n)+=(artificialDissipation*dt)*d(I1,I2,I3);
            }
            else
            {
      	Overture::abort();
            }

      //      cout<<"component "<<n<<" min/max diss "<<min(d)<<"  "<<max(d)<<endl;
        }
        
    }
    
//  field[next].periodicUpdate();
//  PlotIt::contour(*Overture::getGraphicsInterface(), diss);

    return 0;
}

