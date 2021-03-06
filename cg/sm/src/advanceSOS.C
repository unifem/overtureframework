// This file automatically generated from advanceSOS.bC with bpp.
#include "Cgsm.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelOverlappingGridInterpolator.h"
#include "SmParameters.h"
#include "GridFunctionFilter.h"
#include "GridMaterialProperties.h"

// ===================================================================================
//   This macro extracts the boundary data arrays
// ===================================================================================

// ===============================================================================================
// This macro determines the pointers to the variable material properties that are
// used when calling fortran routines.
// ===============================================================================================

#define advSM EXTERN_C_NAME(advsm)
extern "C"
{
    void advSM(const int&nd,
           	     const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
           	     const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
           	     const int&nd4a,const int&nd4b,
           	     const int&mask,const real&rx, const real&xy, 
           	     const real&um, const real&u, real&un, const real&f,
                          const int & ndMatProp, const int& matIndex, const real& matValpc, const real& matVal,
           	     const int&bc, const real &dis, const real &varDis, const int&ipar, const real&rpar, int&ierr );

}


// fourth order dissipation 2D: ***** NOTE: this is minus of the 4th difference:  -(D+D-)^2 *********
#define FD4_2D(u,i1,i2,i3,c) (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) -12.*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4_3D(u,i1,i2,i3,c) (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) -18.*u(i1,i2,i3,c) )

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FN(m) fn[m+numberOfFunctions*(grid)]


// ============================================================================================
/// \brief Compute u.tt for the second-order-system
// ============================================================================================
void Cgsm::
getUtSOS(GridFunction & cgf, 
       	 const real & t, 
       	 RealCompositeGridFunction & ut, 
       	 real tForce )
{
    real & dt= deltaT;

  // --- do this for now ---
    int cur=-1;
    for( int i=0; i<numberOfGridFunctionsToUse; i++ )
    {
        if( &cgf == &gf[i] )
        {
            cur=i;
            break;
        }
    }
    assert( cur>=0 );
    advanceSOS( cur,t,dt, &ut, tForce );
}



// =============================================================================
/// \brief Advance the solution as a second-order system. This function will
//     update the interior points; boundary conditions and interpolation are left
//     to advance(..)
// \param ut : if not NULL, compute u.tt and return in this grid-function. Otherwise
//             return the solution gf[next] at t=t+dt.
// =============================================================================
void Cgsm::
advanceSOS(  int current, real t, real dt,
                          RealCompositeGridFunction *ut /* = NULL */, 
           	     real tForce /* = 0. */ )
{
    checkArrays("advanceSOS:end");


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

    real & rho=parameters.dbase.get<real>("rho");
    real & mu = parameters.dbase.get<real>("mu");
    real & lambda = parameters.dbase.get<real>("lambda");
    RealArray & muGrid = parameters.dbase.get<RealArray>("muGrid");
    RealArray & lambdaGrid = parameters.dbase.get<RealArray>("lambdaGrid");
    bool & gridHasMaterialInterfaces = parameters.dbase.get<bool>("gridHasMaterialInterfaces");
    int & debug = parameters.dbase.get<int >("debug");

    const real cMax=max(lambdaGrid+muGrid)/rho;

    const int computeUt = ut != NULL;  // compute u.tt

  // *** add higher order dissipation that requires interpolation:
    bool useComputeArtificialDissipation=artificialDissipation>0. && 
          orderOfArtificialDissipation > orderOfAccuracyInSpace && orderOfArtificialDissipation>6;

    if( useComputeArtificialDissipation )
    {
        computeDissipation( current,t,dt );
    }



  // -- Apply the high-order filter -- 090823 
    const bool applyFilter = parameters.dbase.get<bool >("applyFilter");
    #ifdef USE_PPP
   // *wdh* 091123 == this should no longer be needed ==
   // in parallel we need to extrap. interpolation neighbours again to get some points that
   // are invalidated when updateGhostBoundaries is called. (rsise2 example, -N4 -n32)
   // *wdh* 091123 const bool extrapInterpolationNeighbours = true;
        const bool extrapInterpolationNeighbours = !parameters.dbase.get<int>("useNewExtrapInterpNeighbours");
    #else
        const bool extrapInterpolationNeighbours=false;
    #endif
    if( applyFilter )
    {
        real time0=getCPU();
        
        GridFunctionFilter *& gridFunctionFilter =parameters.dbase.get<GridFunctionFilter*>("gridFunctionFilter");
        assert( gridFunctionFilter!=NULL );
        GridFunctionFilter & filter = *gridFunctionFilter;
        
        const int filterFrequency = filter.filterFrequency;
        
    // High-order filters may need values assigned at interpolation neighbours and a second ghost line: 
        if( filter.filterType==GridFunctionFilter::explicitFilter &&
      	filter.orderOfFilter> orderOfAccuracyInSpace &&
	!parameters.dbase.get<int >("extrapolateInterpolationNeighbours") )
        {
            printF("advanceSOS:ERROR:extrapolateInterpolationNeighbours should be true for this filter\n");
            OV_ABORT("error");
        }
        if( extrapInterpolationNeighbours && filter.filterType==GridFunctionFilter::explicitFilter &&
      	filter.orderOfFilter> orderOfAccuracyInSpace )
        {
      // -- Extrapolate interpolation neighbours for the high-order filter and artificial dissipation ---
      //   Do this here since in parallel we can extrap interp. neighbours with no communication BUT
      //   the values may be wrong if afterward we perform an updateGhostBoundaries !
      // -- We could fix this but this would require a re-write to extrap. interp neighbours. 
            printF("advSOS: Extrapolate interpolation neighbours before the filter, t=%9.3e\n",t);
            extrapolateInterpolationNeighbours( gf[current], C );

        }
        

        if( (numberOfStepsTaken % filterFrequency) ==0  )
        {
            if( debug & 4 )
      	printF("advanceSOS: apply filter at step=%i, t=%9.3e\n",numberOfStepsTaken,t);
            
      // apply high order filter to u[current]
            gridFunctionFilter->applyFilter( gf[current].u, C, gf[next].u /* work space */ );
        }

        timing(parameters.dbase.get<int>("timeForFilter"))+=getCPU()-time0;
        
    }
    if( extrapInterpolationNeighbours && 
            artificialDissipation>0. && orderOfArtificialDissipation > orderOfAccuracyInSpace )
    {
    // -- Extrapolate interpolation neighbours for the artificial dissipation ---
        printF("advSOS: Extrapolate interpolation neighbours for artificial dissipation, t=%9.3e\n",t);
        extrapolateInterpolationNeighbours( gf[current], C );
    }
    


    sizeOfLocalArraysForAdvance=0.;
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
        real time0=getCPU();

        MappedGrid & mg = cg[grid];
        MappedGridOperators & mgop = (*cgop)[grid];

        getIndex(mg.gridIndexRange(),I1,I2,I3);
    
        getBoundsForPML( mg,Iv );

        realMappedGridFunction & fieldPrev    =gf[prev].u[grid];
        realMappedGridFunction & fieldCurrent =gf[current].u[grid];
        realMappedGridFunction & fieldNext    =gf[next].u[grid];


        realArray & um = fieldPrev;
        realArray & u  = fieldCurrent;
        realArray & un = fieldNext;

        if( debug & 4 )
        {
            fPrintF(debugFile," **** start of advance, t=%8.2e\n",t);
            fprintf(pDebugFile," **** start of advance, t=%8.2e\n",t);
            
            if( debug & 8 )
            {
      	display(um,sPrintF("um start of advance, t=%8.2e",t),debugFile,"%8.2e ");
      	display(u,sPrintF("u start of advance, t=%8.2e",t),debugFile,"%8.2e ");
            }
        }

        lambda = lambdaGrid(grid);
        mu = muGrid(grid);
        c1=(mu+lambda)/rho, c2= mu/rho;
        
        if( numberOfStepsTaken<1 ) 
            printF(" advance:INFO lambda,mu=%8.2e %8.2e for grid=%i (%s) \n",lambda,mu,grid,
                  (const char*)cg[grid].getName());
        
        const real dtsq=dt*dt; 
    // const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
        const real adc=artificialDissipation; // do not scale *wdh* 090216

        bool useOpt=true; // true;
        const bool isRectangular=mg.isRectangular();
        if( !isRectangular )
        {
            real timea=getCPU();
            if( false && useConservative )
            {
	// The conservative operators need the jacobian  --> no longer <--
      	mg.update( MappedGrid::THEinverseVertexDerivative | 
                                      MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
            }
            else
            {
      	mg.update( MappedGrid::THEinverseVertexDerivative | 
                                      MappedGrid::THEinverseCenterDerivative );
            }
          timea=getCPU()-timea;
          timing(parameters.dbase.get<int>("timeForInitialize"))+=timea;

          time0+=timea;  // do not include with time for curvilinear
        }

    //  ::display(lap,"lap","%8.1e ");


        realArray f;
        const bool addForcing = forcingOption!=noForcing && forcingOption!=planeWaveBoundaryForcing;

        if( addForcing )
        {
            Index D1,D2,D3;
            getIndex(mg.dimension(),D1,D2,D3);
            f.partition(mg.getPartition());
            f.redim(D1,D2,D3,C);  // could use some other array for work space ??

            int option=1;  
            if( addForcing )
            {
	//kkc getForcing is called from advance but is also called from assignIC and getErrors.
	//    we have to add the timing in external to getForcing to avoid double counting the time
	//    in assignIC and getErrors
      	real timef = getCPU();
      	getForcing( next, grid,f,t,dt,option );
      	timing(parameters.dbase.get<int>("timeForForcing")) += getCPU()-timef;

            }
        }

    // --- We always call advOpt ---
        real timeAdv=getCPU();
            
    // In some cases we combine the artificial dissipation loop with the main loop
        int combineDissipationWithAdvance = isRectangular && 
      !useVariableDissipation &&
            timeSteppingMethodSm==SmParameters::modifiedEquationTimeStepping &&
            orderOfAccuracyInSpace==4 && orderOfAccuracyInTime==4;

    // combineDissipationWithAdvance=0;

        const int useWhereMask = numberOfComponentGrids>1;
            
        int gridType = isRectangular? 0 : 1;
    // int option=(isRectangular || useCurvilinearOpt) ? 0 : 1;   // 0=SolidMechanics+AD, 1=AD



        int ierr=0;

#ifdef USE_PPP
        realSerialArray umLocal;  getLocalArrayWithGhostBoundaries(um,umLocal);
        realSerialArray uLocal;   getLocalArrayWithGhostBoundaries(u,uLocal);
        realSerialArray unLocal;  getLocalArrayWithGhostBoundaries(un,unLocal);
        realSerialArray fLocal;   getLocalArrayWithGhostBoundaries(f,fLocal);
        realSerialArray varDis; 
        if( useVariableDissipation ) getLocalArrayWithGhostBoundaries((*variableDissipation)[grid],varDis);
#else
        const realSerialArray & umLocal = um;
        const realSerialArray & uLocal  =  u;
        const realSerialArray & unLocal = un;
        const realSerialArray & fLocal = f;
        const realSerialArray & varDis = useVariableDissipation ? (*variableDissipation)[grid] : uLocal;
#endif

        real *umptr=umLocal.getDataPointer();
        real *uptr =uLocal.getDataPointer();
        real *unptr=unLocal.getDataPointer();
            
        if( computeUt )
        { // in this case we save u.tt in *ut 
            #ifdef USE_PPP
              realSerialArray utLocal;  getLocalArrayWithGhostBoundaries((*ut)[grid],utLocal);
              unptr=utLocal.getDataPointer();
            #else
              unptr=(*ut)[grid].getDataPointer();
            #endif      
        }
        

        real *ut1ptr = uptr; 
        real *ut2ptr = uptr; 
        real *ut3ptr = uptr; 
        real *ut4ptr = uptr; 
        real *ut5ptr = uptr; 
        real *ut6ptr = uptr; 
        real *ut7ptr = uptr; 
        real *fptr   = addForcing ? fLocal.getDataPointer() : uptr;

        assert( !useVariableDissipation || variableDissipation!=NULL );
        real *pVarDis = useVariableDissipation ? varDis.getDataPointer() : uptr;
            
            
        const intArray & mask = mg.mask();
#ifdef USE_PPP
        intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
        const intSerialArray & maskLocal = mask; 
#endif

        real *rxptr;
        if( isRectangular )
        {
            rxptr=uptr;
        }
        else
        {
#ifdef USE_PPP
            realSerialArray rxLocal; getLocalArrayWithGhostBoundaries(mg.inverseVertexDerivative(),rxLocal);
#else
            const realSerialArray & rxLocal=mg.inverseVertexDerivative();
#endif
            rxptr = rxLocal.getDataPointer();
        }
            
        const bool centerNeeded=forcingOption==twilightZoneForcing || (forcingOption==planeWaveBoundaryForcing); // **************** fix this 
        #ifdef USE_PPP
            realSerialArray xy;
            if( centerNeeded ) getLocalArrayWithGhostBoundaries(mg.center(),xy);
        #else
            const realSerialArray & xy = centerNeeded ? mg.center() : umLocal;
        #endif
        real *xyptr=xy.getDataPointer();

        int maskNull[1];
        int *maskptr = useWhereMask ? maskLocal.getDataPointer() : maskNull;

        realSerialArray *dis = NULL;
        real *pdis=uptr;

    // Macro to extract the pointers to the variable material property arrays
     // --- Variable material properies ---
          GridMaterialProperties::MaterialFormatEnum materialFormat = GridMaterialProperties::constantMaterialProperties;
          int ndMatProp=1;  // for piecewise constant materials, this is the leading dimension of the matVal array
          int *matIndexPtr=maskptr;  // if not used, point to mask
          real*matValPtr=uptr;       // if not used, point to u
          if( parameters.dbase.get<int>("variableMaterialPropertiesOption")!=0 )
          {
       // Material properties do vary 
              std::vector<GridMaterialProperties> & materialProperties = 
              	parameters.dbase.get<std::vector<GridMaterialProperties> >("materialProperties");
              GridMaterialProperties & matProp = materialProperties[grid];
              materialFormat = matProp.getMaterialFormat();
              if( materialFormat==GridMaterialProperties::piecewiseConstantMaterialProperties )
              {
              	IntegerArray & matIndex = matProp.getMaterialIndexArray();
                  matIndexPtr = matIndex.getDataPointer();
              }
              RealArray & matVal = matProp.getMaterialValuesArray();
              matValPtr = matVal.getDataPointer();
              ndMatProp = matVal.getLength(0);  
       // ::display(matVal,"matVal");
          }

        int option=0; // (isRectangular || useCurvilinearOpt) ? 0 : 1;   // 0=Maxwell+AD, 1=AD
        int ipar[]={option,
            		gridType,
            		orderOfAccuracyInSpace,
            		orderOfAccuracyInTime,
            		(int)addForcing,
            		orderOfArtificialDissipation,   // 5
            		uc,
                                vc,
                                wc,
            		useWhereMask,
            		(int)timeSteppingMethodSm,
            		(int)useVariableDissipation,
            		(int)useConservative,           // 12 
            		combineDissipationWithAdvance,
                                debug,
                                computeUt,
                                materialFormat,        // 16 
                                myid
                                };                      


        real dx[3]={1.,1.,1.};
        if( isRectangular )
            mg.getDeltaX(dx);
        	  
        real rpar[30];
        rpar[ 0]=dt;
        rpar[ 1]=dx[0];
        rpar[ 2]=dx[1];
        rpar[ 3]=dx[2];
        rpar[ 4]=adc;
        rpar[ 5]=mg.gridSpacing(0);
        rpar[ 6]=mg.gridSpacing(1);
        rpar[ 7]=mg.gridSpacing(2);
        rpar[ 8]=c1;
        rpar[ 9]=c2;
        rpar[10]=kx; // for plane wave scattering
        rpar[11]=ky;
        rpar[12]=kz;

        
        rpar[13]=(real &)parameters.dbase.get<OGFunction* >("exactSolution");  // twilight zone pointer, ep
        rpar[14]=t;
        rpar[15]=parameters.dbase.get<real>("dtOld");  // dt used on the previous step

        rpar[16]=rho;
        rpar[17]=mu;
        rpar[18]=lambda;

        rpar[20]=0.;  // return cpu for dissipation


    // printF("** AdvanceSOS: gridType=%i, isRectangular=%i\n",gridType,(int)isRectangular);

        bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);

        if( ok )
        {
            if( adc>0. && !combineDissipationWithAdvance )
            {
	// create a temp array to hold the artificial dissipation
      	dis = new realSerialArray(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),uLocal.dimension(3));
      	pdis = dis->getDataPointer();
      	assert( pdis!=NULL );

      	sizeOfLocalArraysForAdvance=max(sizeOfLocalArraysForAdvance,(double)(dis->elementCount()*sizeof(real)));
            }


      // real timeAdv=getCPU();
            if( combineDissipationWithAdvance )
            {
      	assert( umptr!=unptr );
            }
            advSM(mg.numberOfDimensions(),
          	    I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
          	    uLocal.getBase(0),uLocal.getBound(0),
          	    uLocal.getBase(1),uLocal.getBound(1),
          	    uLocal.getBase(2),uLocal.getBound(2),
          	    uLocal.getBase(3),uLocal.getBound(3),
          	    *maskptr,*rxptr,*xyptr,  
          	    *umptr,*uptr,*unptr, *fptr, ndMatProp,*matIndexPtr,*matValPtr,*matValPtr, 
          	    mg.boundaryCondition(0,0), *pdis, *pVarDis, ipar[0], rpar[0], ierr );
            
        }
        timeAdv=getCPU()-timeAdv;
        timing(parameters.dbase.get<int>("timeForAdvOpt"))+=timeAdv;
            
        if( debug & 8 )
        {
            display(unLocal,sPrintF("unLocal after advSolidMechanics, processor=%i before BC's t=%8.2e",
                        			      Communication_Manager::My_Process_Number,t),pDebugFile,"%8.2e ");

            display(un,sPrintF("un after advSolidMechanics, before BC's t=%8.2e",t),debugFile,"%8.2e ");
        }

        if( dis!=NULL )
        {
            delete dis;
        }
            
        timing(parameters.dbase.get<int>("timeForDissipation"))+=rpar[20];
        


        
        if( useComputeArtificialDissipation && ok )
        {
      // *** add higher order dissipation that requires interpolation
            real timed=getCPU();
            assert( cgdissipation!=NULL );
            #ifdef USE_PPP
                realSerialArray dLocal; getLocalArrayWithGhostBoundaries((*cgdissipation)[grid],dLocal);
            #else
                const realSerialArray & dLocal = (*cgdissipation)[grid];
            #endif      

            unLocal(I1,I2,I3,C)+=dLocal(I1,I2,I3,C); 
            timing(parameters.dbase.get<int>("timeForDissipation"))+=getCPU()-timed;

        }
        

        if( isRectangular )   
            timing(parameters.dbase.get<int>("timeForAdvanceRectangularGrids"))+=getCPU()-time0;
        else
            timing(parameters.dbase.get<int>("timeForAdvanceCurvilinearGrids"))+=getCPU()-time0;

    // Is this the right place?
        #ifdef USE_PPP
        if( false ) // *wdh* 091205 -- this is not needed -- we call interpolate next which does this 
        {
            real timea=getCPU();

            if( debug & 16 )
            {
      	Communication_Manager::Sync();
      	display(unLocal,sPrintF(" Before updateGhostBoundaries: t=%e",t),pDebugFile,"%8.2e ");
            }
            
      // **** at this point we really only need to update interior-ghost points needed for
      //      interpolation or boundary conditions
            un.updateGhostBoundaries();

            if( debug & 16 )
            {
      	display(unLocal,sPrintF(" After updateGhostBoundaries: processor=%i t=%e",
                        				Communication_Manager::My_Process_Number,t),pDebugFile,"%8.2e ");
            }
            
            timing(parameters.dbase.get<int>("timeForUpdateGhostBoundaries"))+=getCPU()-timea;
        }
        #endif

        if( debug & 8 )
        {
            display(u,sPrintF("u after advOpt and updateGhost, t=%8.2e",t),debugFile,"%8.2e ");
        }


        if( debug & 16 )
        {
            if( addForcing ) 
                ::display(f,sPrintF("  *** advanceStructured: Here is the forcing f grid=%i t=%9.3e ********\n",grid,t),
              		  pDebugFile,"%7.4f ");
        }
    } // end grid
    
    if( debug & 4 )
    {
        getErrors( next,t+dt,dt,sPrintF("\n *** advanceSOS: Errors after advance, before BC, t=%9.3e ******\n",t+dt) );
    }

#undef FN

    parameters.dbase.get<real>("dtOld")=dt;  // set dtOld 

    checkArrays("advanceSOS:end");
    
}
