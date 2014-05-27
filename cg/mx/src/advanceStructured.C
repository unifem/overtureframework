#include "Maxwell.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#define advMaxwell EXTERN_C_NAME(advmaxwell)
#define mxFilter EXTERN_C_NAME(mxfilter)

extern "C"
{
      void advMaxwell(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b,
      const int&mask,const real&rx,  
      const real&um, const real&u, real&un, const real&f,
      const real&ut1,const real&ut2,const real&ut3, const real&ut4,const real&ut5,const real&ut6,const real&ut7,
      const int&bc, const real &dis, const real &varDis, const int&ipar, const real&rpar, int&ierr );

 void mxFilter(const int&nd,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int & gridIndexRange, const real & u, const real & d, 
      const int&mask, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );
}

// fourth order dissipation 2D: ***** NOTE: this is minus of the 4th difference:  -(D+D-)^2 *********
#define FD4_2D(u,i1,i2,i3,c) \
      (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
        +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       -12.*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4_3D(u,i1,i2,i3,c) \
      (    -( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   \
        +4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
       -18.*u(i1,i2,i3,c) )

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FN(m) fn[m+numberOfFunctions*(grid)]

// =================================================================================
/// \brief Advance a time-step on a structured grid (curvilinear or Cartesian grid)
// =================================================================================
void Maxwell::
advanceNFDTD(  int numberOfStepsTaken, int current, real t, real dt )
{
  checkArrays("advanceStructured:start");

//    printF("advanceNFDTD: t=%e current=%i, numberOfFunctions=%i, numberOfTimeLevels=%i\n",t,
//        current,numberOfFunctions,numberOfTimeLevels);
  
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range C(ex,hz);
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  const int next = (current+1) % numberOfTimeLevels;

  const real cMax=max(cGrid);

 // *** add higher order dissipation that requires interpolation:
//  bool useComputeArtificialDissipation=(artificialDissipation>0. || artificialDissipationCurvilinear>0. ) && 
//    orderOfArtificialDissipation > orderOfAccuracyInSpace && orderOfArtificialDissipation>6;

//   if( useComputeArtificialDissipation )
//   {
//     computeDissipation( current,t,dt );
//   }

  if( applyFilter &&  (numberOfStepsTaken % filterFrequency) ==0  )
  {
    // apply high order filter to u[current]
    addFilter( current,t,dt );
  }
  

  sizeOfLocalArraysForAdvance=0.;
  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    real time0=getCPU();

    MappedGrid & mg = cg[grid];
    assert( mgp==NULL || op!=NULL );
    MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];

    getIndex(mg.gridIndexRange(),I1,I2,I3);

  
    getBoundsForPML( mg,Iv );

    realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
    realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];


    realArray & um = fieldPrev;
    realArray & u  = fieldCurrent;
    realArray & un = fieldNext;

    if( debug & 8 )
    {
      Communication_Manager::Sync();
      fPrintF(debugFile," **** start of advanceNFDTD t=%8.2e\n",t);
      display(um,sPrintF("um start of advanceNFDTD, t=%8.2e",t),debugFile,"%8.2e ");
      display(u,sPrintF("u start of advanceNFDTD, t=%8.2e",t),debugFile,"%8.2e ");
      Communication_Manager::Sync();
    }

    // 2D TEz mode:
    //   (Ex).t = (1/eps)*[  (Hz).y ]
    //   (Ey).t = (1/eps)*[ -(Hz).x ]
    //   (Hz).t = (1/mu) *[ (Ex).y - (Ey).x ]

    c = cGrid(grid);
    eps = epsGrid(grid);
    mu = muGrid(grid);
    if( numberOfStepsTaken<1 ) 
      printF(" advanceNFDTD:INFO eps,mu,c=%8.2e %8.2e %8.2e for grid=%i (%s) \n",eps,mu,c,grid,
         (const char*)cg[grid].getName());
    
    const bool isRectangular=mg.isRectangular();

    const real dtsq=dt*dt; 
    const real csq=c*c;
    const real cdtsq=c*c*dt*dt;



    // *NO* const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
    real adc = isRectangular ? c*artificialDissipation : c*artificialDissipationCurvilinear;  // Do this *wdh* 090602 

    bool useOpt=true; // true;
    if( !isRectangular )
    {
      real timea=getCPU();
      if( useConservative )
      {
	// The conservative operators need the jacobian
	mg.update( MappedGrid::THEinverseVertexDerivative | 
                   MappedGrid::THEinverseCenterDerivative | MappedGrid::THEcenterJacobian );
      }
      else
      {
	mg.update( MappedGrid::THEinverseVertexDerivative | 
                   MappedGrid::THEinverseCenterDerivative );
      }
     timea=getCPU()-timea;
     timing(timeForInitialize)+=timea;

     time0+=timea;  // do not include with time for curvilinear
    }

    //  ::display(lap,"lap","%8.1e ");

//      real ad4dt=ad4*dt;
//      // add a 'fourth' order dissipation  ad4 h^4 dt (u.xxxx).t to (c*dt)^2*laplacian(u)
//      LAP(i1,i2,i3)=cdtsq*LAP(i1,i2,i3)+ad4dt*( FD4_2D(U1G,i1,i2,i3)-FD4_2D(U2G,i1,i2,i3) );


    

    realArray f;
    const bool addForcing = forcingOption!=noForcing && forcingOption!=planeWaveBoundaryForcing;

    bool useCurvilinearOpt=true && !isRectangular;   // use advOpt to advance curvilinear grids given the RHS

    // useCurvilinearOptNew: if true, use adopt to advance full equations in curvilinear case
    //                       if false, evaluate RHS for curvilinear below and then use advOpt to update solution
    // bool useCurvilinearOptNew = !isRectangular && !useConservative && numberOfDimensions==2 ;
    bool useCurvilinearOptNew = !isRectangular && !useConservative && 
                                (orderOfAccuracyInSpace==4 || orderOfAccuracyInSpace==6);

    if( addForcing || !useCurvilinearOptNew )
    {
      Index D1,D2,D3;
      getIndex(mg.dimension(),D1,D2,D3);
      f.partition(mg.getPartition());
      f.redim(D1,D2,D3,C);  // could use some other array for work space ??

      int option=1;  
      if( !isRectangular && !useCurvilinearOptNew )
      {
        if( timeSteppingMethod == modifiedEquationTimeStepping && orderOfAccuracyInTime>=4 )
	{
          // Compute the square of the spatial operator
          assert( numberOfFunctions>=1 && fn!=NULL );
    	  const int m0=currentFn;
          realArray & lapSq = FN(m0);  // this is passed to advOpt ***              *** FIX for PARALLEL**
          
//            #ifdef USE_PPP
//  	  u.updateGhostBoundaries();   // *** this is needed for some reason ****
//            #endif
	  // display(u,sPrintF("u before lapSq t=%e processor=%i",t,myid),debugFile,"%6.2f ");

          Index J1,J2,J3;
          const int extra=1; // orderOfAccuracyInSpace/2-1;
          getIndex(mg.gridIndexRange(),J1,J2,J3,extra);

          mgop.setOrderOfAccuracy(orderOfAccuracyInSpace-2);
	  
	  mgop.derivative(MappedGridOperators::laplacianOperator,u,f,J1,J2,J3);  // *** use f as a temporary
          #ifdef USE_PPP
  	    f.updateGhostBoundaries();
          #endif
	  // display(f,sPrintF("f=lap(order=2) t=%e processor=%i",t,myid),debugFile,"%6.2f ");

	  mgop.derivative(MappedGridOperators::laplacianOperator,f,lapSq,I1,I2,I3);

          mgop.setOrderOfAccuracy(orderOfAccuracyInSpace);
          
          lapSq(I1,I2,I3,C)*=csq*csq;
	  // display(lapSq,sPrintF("lapSq t=%e processor=%i",t,myid),debugFile,"%6.2f ");
          // printF(" max(fabs(lapSq))=%8.2e min=%8.2e\n",max(fabs(lapSq(I1,I2,I3,C))),min(fabs(lapSq(I1,I2,I3,C))));
	  
	  
	}

	// compute laplacian for curvilinear grids
        mgop.derivative(MappedGridOperators::laplacianOperator,u,f,I1,I2,I3);
        f(I1,I2,I3,C)*=csq;

        // display(f,sPrintF("lap*csq t=%e processor=%i",t,myid),debugFile,"%6.2f ");

	//f = csq*f + (csq*cdt*cdt/12.)*lapSq;  // put all into f

        option=0;  // append any forcing below
      }
      else
      {
        option=1;
      }
      if( addForcing )
      {
	//kkc getForcing is called from advance but is also called from assignIC and getErrors.
	//    we have to add the timing in external to getForcing to avoid double counting the time
	//    in assignIC and getErrors
	real timef = getCPU();
	getForcing( next, grid,f,t,dt,option );
	timing(timeForForcing) += getCPU()-timef;
      }
    }

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

    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);


    if( useOpt ) 
    {
      // --- We always call advOpt ---
      real timeAdv=getCPU();
      
      // In some cases we combine the artificial dissipation loop with the main loop
      int combineDissipationWithAdvance = adc>0. && isRectangular && 
                                          !useVariableDissipation &&
                                          timeSteppingMethod==modifiedEquationTimeStepping &&
                                          orderOfAccuracyInSpace==4 && orderOfAccuracyInTime==4;

      // combineDissipationWithAdvance=0;

      const int useWhereMask = numberOfComponentGrids>1;
      
      int gridType = isRectangular? 0 : 1;
      int option=(isRectangular || useCurvilinearOpt) ? 0 : 1;   // 0=Maxwell+AD, 1=AD
      int ipar[]={option,
                  gridType,
		  orderOfAccuracyInSpace,
		  orderOfAccuracyInTime,
                  (int)addForcing,
                  orderOfArtificialDissipation,
		  ex,ey,ez,hx,hy,hz,
                  int(solveForElectricField),
                  int(solveForMagneticField),
                  useWhereMask,
                  (int)timeSteppingMethod,
                  (int)useVariableDissipation,
                  (int)useCurvilinearOptNew,
                  (int)useConservative,
                  combineDissipationWithAdvance,
                  (int)useDivergenceCleaning};  //

      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);
	  
      real rpar[30];
      rpar[ 0]=c;
      rpar[ 1]=dt;
      rpar[ 2]=dx[0];
      rpar[ 3]=dx[1];
      rpar[ 4]=dx[2];
      rpar[ 5]=adc;
      rpar[ 6]=divergenceDamping;
      rpar[ 7]=mg.gridSpacing(0);
      rpar[ 8]=mg.gridSpacing(1);
      rpar[ 9]=mg.gridSpacing(2);
      rpar[10]=eps;
      rpar[11]=mu;
      rpar[12]=kx; // for plane wave scattering
      rpar[13]=ky;
      rpar[14]=kz;
      rpar[15]=sigmaEGrid(grid);
      rpar[16]=sigmaHGrid(grid);
      rpar[17]=divergenceCleaningCoefficient;
      rpar[18]=t;

      rpar[20]=0.;  // return cpu for dissipation

      int ierr=0;


      real *umptr=umLocal.getDataPointer();
      real *uptr =uLocal.getDataPointer();
      real *unptr=unLocal.getDataPointer();
      
      real *ut1ptr = uptr; 
      real *ut2ptr = uptr; 
      real *ut3ptr = uptr; 
      real *ut4ptr = uptr; 
      real *ut5ptr = uptr; 
      real *ut6ptr = uptr; 
      real *ut7ptr = uptr; 
      real *fptr   = (addForcing || useCurvilinearOpt) ? fLocal.getDataPointer() : uptr;

      assert( !useVariableDissipation || variableDissipation!=NULL );
      real *pVarDis = useVariableDissipation ? varDis.getDataPointer() : uptr;
      
      if( true || method!=nfdtd )
      {
        if( orderOfAccuracyInTime>=4 )
        {
  	  assert( numberOfFunctions>=3 && fn!=NULL );
  	  const int m0=currentFn, m1=(m0+1)%numberOfFunctions, m2=(m1+1)%numberOfFunctions;
  
          #ifdef USE_PPP
    	  realSerialArray f0; getLocalArrayWithGhostBoundaries(FN(m0),f0); ut1ptr=f0.getDataPointer();
    	  realSerialArray f1; getLocalArrayWithGhostBoundaries(FN(m1),f1); ut2ptr=f1.getDataPointer();
    	  realSerialArray f2; getLocalArrayWithGhostBoundaries(FN(m2),f2); ut3ptr=f2.getDataPointer();
          #else
    	  ut1ptr=FN(m0).getDataPointer();
  	  ut2ptr=FN(m1).getDataPointer();
  	  ut3ptr=FN(m2).getDataPointer();
          #endif
  
  	if( orderOfAccuracyInTime>=6 && timeSteppingMethod==stoermerTimeStepping )
  	{
  	  assert( numberOfFunctions>=5 );
  	  const int m3=(m2+1)%numberOfFunctions, m4=(m3+1)%numberOfFunctions;
            #ifdef USE_PPP
    	    realSerialArray f3; getLocalArrayWithGhostBoundaries(FN(m3),f3); ut4ptr=f3.getDataPointer();
    	    realSerialArray f4; getLocalArrayWithGhostBoundaries(FN(m4),f4); ut5ptr=f4.getDataPointer();
            #else
    	    ut4ptr=FN(m3).getDataPointer();
  	    ut5ptr=FN(m4).getDataPointer();
            #endif
  
  	  if( orderOfAccuracyInTime>=8 && timeSteppingMethod==stoermerTimeStepping )
  	  {
  	    assert( numberOfFunctions>=7 );
  	    const int m5=(m4+1)%numberOfFunctions, m6=(m5+1)%numberOfFunctions;
              #ifdef USE_PPP
    	      realSerialArray f5; getLocalArrayWithGhostBoundaries(FN(m5),f5); ut6ptr=f5.getDataPointer();
    	      realSerialArray f6; getLocalArrayWithGhostBoundaries(FN(m6),f6); ut7ptr=f6.getDataPointer();
              #else
    	      ut6ptr=FN(m5).getDataPointer();
  	      ut7ptr=FN(m6).getDataPointer();
              #endif
  	  }
  	}
        }
      }

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
      
      int *maskptr = useWhereMask ? maskLocal.getDataPointer() : ipar;

      realSerialArray *dis = NULL;
      real *pdis=uptr;
      if( adc>0. && !combineDissipationWithAdvance && ok )
      {
        // create a temp array to hold the artificial dissipation
	dis = new realSerialArray(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2),uLocal.dimension(3));
        pdis = dis->getDataPointer();
        assert( pdis!=NULL );

        sizeOfLocalArraysForAdvance=max(sizeOfLocalArraysForAdvance,(double)(dis->elementCount()*sizeof(real)));
      }

      if( ok )
      {
	if( combineDissipationWithAdvance )
	{
	  assert( umptr!=unptr );
	}
      
	advMaxwell(mg.numberOfDimensions(),
		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		   uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
		   uLocal.getBase(2),uLocal.getBound(2),
		   uLocal.getBase(3),uLocal.getBound(3),
		   *maskptr,*rxptr,  
		   *umptr,*uptr,*unptr, *fptr, *ut1ptr,*ut2ptr,*ut3ptr,*ut4ptr,*ut5ptr,*ut6ptr,*ut7ptr,   
		   mg.boundaryCondition(0,0), *pdis, *pVarDis, ipar[0], rpar[0], ierr );
      }
      
      timeAdv=getCPU()-timeAdv;
      timing(timeForAdvOpt)+=timeAdv;
      
      if( debug & 8 )
      {
        display(unLocal,sPrintF("unLocal after advMaxwell, processor=%i before BC's t=%8.2e",
                         Communication_Manager::My_Process_Number,t),pDebugFile,"%8.2e ");

        display(un,sPrintF("un after advMaxwell, before BC's t=%8.2e",t),debugFile,"%8.2e ");
      }

//        printF(" p=%i time for advMaxwell=%e I1,I2,I3=[%i,%i][%i,%i][%i,%i]\n",Communication_Manager::My_Process_Number,timeAdv,
//                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
      
      if( dis!=NULL )
      {
        delete dis;
      }
      
      timing(timeForDissipation)+=rpar[20];
    } // end if( use Opt )
    


//     if( !useOpt ) 
//     {
//       // *** old way ***

//       // take a time step

//       if( orderOfAccuracyInTime==2 )
//       {

// 	realArray lap(I1,I2,I3,C);  // ** fix this **
// 	mgop.derivative(MappedGridOperators::laplacianOperator,u,lap,I1,I2,I3);

//         if( addForcing )
//   	  un(I1,I2,I3,C)=2.*u(I1,I2,I3,C)-un(I1,I2,I3,C)+cdtsq*lap(I1,I2,I3,C)+dtsq*f(I1,I2,I3,C);
//         else
//   	  un(I1,I2,I3,C)=2.*u(I1,I2,I3,C)-un(I1,I2,I3,C)+cdtsq*lap(I1,I2,I3,C);
//       }
//       else if( orderOfAccuracyInTime==4 )
//       {
// 	// 4th order Stoermer method

// 	const real c0=(7./6.)*dtsq, c1=(-5./12.)*dtsq, c2=(1./3.)*dtsq, c3=(-1./12.)*dtsq;

// 	assert( numberOfFunctions>=3 && fn!=NULL );

// 	const int m0=currentFn, m1=(m0+1)%numberOfFunctions, m2=(m1+1)%numberOfFunctions;
// 	// printF("4th order Stoermer method: t=%8.2e, m0,m1,m2=%i,%i,%i\n",t,m0,m1,m2);

// 	un(I1,I2,I3,C)=2.*u(I1,I2,I3,C)-un(I1,I2,I3,C)+
// 	  ( c1*FN(m0)(I1,I2,I3,C)+c2*FN(m1)(I1,I2,I3,C)+c3*FN(m2)(I1,I2,I3,C) );

// 	// over-write FN(m2) with F(t)
// 	mgop.derivative(MappedGridOperators::laplacianOperator,u,FN(m2),I1,I2,I3);

// 	// add body forcing (and TZ forcing)
// 	if( addForcing )
// 	  FN(m2)= FN(m2)*csq+f(I1,I2,I3,C);
// 	else
// 	  FN(m2)*=csq;

// 	un(I1,I2,I3,C)+=c0*FN(m2)(I1,I2,I3,C);
//       }
//       else if( orderOfAccuracyInTime==6 )
//       {
// 	// 6th order Stoermer method

//         const real c60=( 317./240.)*dtsq;    // from stoermer.maple
//         const real c61=(-266./240.)*dtsq;
//         const real c62=( 374./240.)*dtsq;
//         const real c63=(-276./240.)*dtsq;
//         const real c64=( 109./240.)*dtsq;
//         const real c65=( -18./240.)*dtsq;

// 	assert( numberOfFunctions>=5 && fn!=NULL );

// 	const int m0=currentFn, m1=(m0+1)%numberOfFunctions, m2=(m1+1)%numberOfFunctions,
// 	          m3=(m2+1)%numberOfFunctions, m4=(m3+1)%numberOfFunctions;
// 	const int mm=m4; // Here is the last work space where we will store the current RHS below
	
// 	un(I1,I2,I3,C)=2.*u(I1,I2,I3,C)-un(I1,I2,I3,C)+
// 	  ( c61*FN(m0)(I1,I2,I3,C)+c62*FN(m1)(I1,I2,I3,C)+c63*FN(m2)(I1,I2,I3,C)
//            +c64*FN(m3)(I1,I2,I3,C)+c65*FN(m4)(I1,I2,I3,C) );

// 	// over-write FN(m2) with F(t)
// 	mgop.derivative(MappedGridOperators::laplacianOperator,u,FN(mm),I1,I2,I3);

// 	// add body forcing (and TZ forcing)
// 	if( addForcing )
// 	  FN(mm)= FN(mm)*csq+f(I1,I2,I3,C);
// 	else
// 	  FN(mm)*=csq;

// 	un(I1,I2,I3,C)+=c60*FN(mm)(I1,I2,I3,C);
//       }
//       else if( orderOfAccuracyInTime==8 )
//       {
// 	// 8th order Stoermer method

//         const real c80=(  88324./60480.)*dtsq; // from stoermer.maple
//         const real c81=(-121797./60480.)*dtsq;
//         const real c82=( 245598./60480.)*dtsq;
//         const real c83=(-300227./60480.)*dtsq;
//         const real c84=( 236568./60480.)*dtsq;
//         const real c85=(-117051./60480.)*dtsq;
//         const real c86=(  33190./60480.)*dtsq;
//         const real c87=(  -4125./60480.)*dtsq;

// 	assert( numberOfFunctions>=7 && fn!=NULL );

// 	const int m0=currentFn, m1=(m0+1)%numberOfFunctions, m2=(m1+1)%numberOfFunctions,
// 	          m3=(m2+1)%numberOfFunctions, m4=(m3+1)%numberOfFunctions,
// 	          m5=(m4+1)%numberOfFunctions, m6=(m5+1)%numberOfFunctions;
// 	const int mm=m6; // Here is the last work space where we will store the current RHS below
	
// 	un(I1,I2,I3,C)=2.*u(I1,I2,I3,C)-un(I1,I2,I3,C)+
// 	  ( c81*FN(m0)(I1,I2,I3,C)+c82*FN(m1)(I1,I2,I3,C)+c83*FN(m2)(I1,I2,I3,C)
//            +c84*FN(m3)(I1,I2,I3,C)+c85*FN(m4)(I1,I2,I3,C) 
//            +c86*FN(m5)(I1,I2,I3,C)+c87*FN(m6)(I1,I2,I3,C) );

// 	// over-write FN(m2) with F(t)
// 	mgop.derivative(MappedGridOperators::laplacianOperator,u,FN(mm),I1,I2,I3);

// 	// add body forcing (and TZ forcing)
// 	if( addForcing )
// 	  FN(mm)= FN(mm)*csq+f(I1,I2,I3,C);
// 	else
// 	  FN(mm)*=csq;

// 	un(I1,I2,I3,C)+=c80*FN(mm)(I1,I2,I3,C);
//       }
//       else
//       {
// 	printf("advanceNFDTD:ERROR: orderOfAccuracyInTime=%i is not implemented=%i\n",orderOfAccuracyInTime);
//       }

//     }  // end -- not use opt
    
//     if( useComputeArtificialDissipation )
//     {
//       // *** add higher order dissipation that requires interpolation
//       real timed=getCPU();
//       assert( cgdissipation!=NULL );
//       #ifdef USE_PPP
//         realSerialArray dLocal; getLocalArrayWithGhostBoundaries((*cgdissipation)[grid],dLocal);
//       #else
//         const realSerialArray & dLocal = (*cgdissipation)[grid];
//       #endif      

//       unLocal(I1,I2,I3,C)+=dLocal(I1,I2,I3,C); 

//       timing(timeForDissipation)+=getCPU()-timed;
//     }
    

    if( isRectangular )   
      timing(timeForAdvanceRectangularGrids)+=getCPU()-time0;
    else
      timing(timeForAdvanceCurvilinearGrids)+=getCPU()-time0;

    // Is this the right place?
    #ifdef USE_PPP
      real timea=getCPU();

      if( debug & 8 )
      {
	Communication_Manager::Sync();
	display(unLocal,sPrintF(" Before updateGhostBoundaries: t=%e",t),pDebugFile,"%8.2e ");
      }
      
      // **** at this point we really only need to update interior-ghost points needed for
      //      interpolation or boundary conditions
      un.updateGhostBoundaries();

      if( debug & 8 )
      {
	display(unLocal,sPrintF(" After updateGhostBoundaries: t=%e",t),pDebugFile,"%8.2e ");
      }
      
      timing(timeForUpdateGhostBoundaries)+=getCPU()-timea;
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
      fprintf(pDebugFile," *** advanceStructured: After advance, before BC, grid=%i t=%9.3e ********\n",grid,t);
      getErrors( next,t+dt,dt );
    }

  } // end grid
  
#undef FN

//    printF(" **** advanceStructured t=%e processor=%i\n",t,
//               Communication_Manager::My_Process_Number);
//    fflush(stdout);
//    Communication_Manager::Sync();
  

  if( orderOfAccuracyInTime>=4 )
  {
    currentFn=(currentFn+orderOfAccuracyInTime-2)%numberOfFunctions;
  }
  
  
  if( cg.numberOfComponentGrids()>1 )
  {
    real timei=getCPU();
    
    if( debug & 8 )
      cgfields[next].display(sPrintF("cgfields[next] before interpolate, t=%8.2e",t),debugFile,"%8.2e ");

    cgfields[next].interpolate();
 
    if( debug & 8 )
      cgfields[next].display(sPrintF("cgfields[next] after interpolate, t=%8.2e",t),debugFile,"%8.2e ");


    if( projectInterpolation )
      projectInterpolationPoints( numberOfStepsTaken, next, t+dt, dt );

    timing(timeForInterpolate)+=getCPU()-timei;

  }


  // ================= Project Fields =================================

  // Here we project the field to satsify Gauss's Law  div(E)=0 or div(E) = stuff
  // It seems important to project two steps in a row

  // We project fields here before the BC's -- to do this we use:
  //    --> Boundary values of the field should be accurate
  //    --> the divergence on the boundary is not needed for the projection
  //    --> for 4th-order we do need values on the first ghost line

  const bool projectBeforeBCs=false;
  const bool projectThisStep = projectFields && (
            ( (numberOfStepsTaken % frequencyToProjectFields) < numberOfConsecutiveStepsToProject ) ||
	    numberOfStepsTaken < numberOfInitialProjectionSteps );

  if( projectBeforeBCs && projectThisStep )
  {

    if( cg.numberOfComponentGrids()==1 )
      cgfields[next].periodicUpdate();   // this seems to be needed

    project( numberOfStepsTaken, next, t+dt , dt );

    if( false )
    {
      // re-apply the BC;s
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];
	realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];

	int option=0;
	assignBoundaryConditions( option, grid, t+dt, dt, fieldNext, fieldCurrent,current );
      }
    }
    
  }


  // ============= Boundary Conditions =============

  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];
    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];


    if( true )
    {
      // ** 050402 wdh: This next line seems to be needed for Hz (annulus.rbc) -- could fix in PEC
      fieldNext.periodicUpdate(); 
    }
    
    int option=0; // not used.
    assignBoundaryConditions( option, grid, t+dt, dt, fieldNext, fieldCurrent,current );

    if( true )
    {
       real timeBC=getCPU();

//        MappedGrid & mg = cg[grid];
      
//        MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];
//        BoundaryConditionParameters bcParams;
//        bcParams.orderOfExtrapolation=orderOfAccuracyInSpace+1;

     
//        if( mg.numberOfDimensions()==2 )
//        {
//  	for( int side2=0; side2<=1; side2++ )
//  	{
//  	  for( int side1=0; side1<=1; side1++ )
//  	  {
//  	    if( mg.boundaryCondition(side1,0)==perfectElectricalConductor &&
//  		mg.boundaryCondition(side2,1)==perfectElectricalConductor )
//  	    {
//  	      bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::doNothingCorner,side1,side2);
//  	    }
//  	  }
//  	}
//        }
//        else
//        {
//          int side1,side2,side3;
//          // edges parallel to i3:
//  	side3=-1;
//  	for( side2=0; side2<=1; side2++ )
//  	for( side1=0; side1<=1; side1++ )
//  	  if( mg.boundaryCondition(side1,0)==perfectElectricalConductor &&
//  	      mg.boundaryCondition(side2,1)==perfectElectricalConductor )
//  	    bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::doNothingCorner,side1,side2,side3);

//          // edges parallel to i2:
//  	side2=-1;
//  	for( side3=0; side3<=1; side3++ )
//  	for( side1=0; side1<=1; side1++ )
//  	  if( mg.boundaryCondition(side1,0)==perfectElectricalConductor &&
//  	      mg.boundaryCondition(side3,2)==perfectElectricalConductor )
//  	    bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::doNothingCorner,side1,side2,side3);

//          // edges parallel to i1:
//  	side1=-1;
//  	for( side3=0; side3<=1; side3++ )
//  	for( side2=0; side2<=1; side2++ )
//  	  if( mg.boundaryCondition(side2,1)==perfectElectricalConductor &&
//  	      mg.boundaryCondition(side3,2)==perfectElectricalConductor )
//  	    bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::doNothingCorner,side1,side2,side3);

//          // corners
//  	for( side3=0; side3<=1; side3++ )
//  	for( side2=0; side2<=1; side2++ )
//  	for( side1=0; side1<=1; side1++ )
//  	  if( mg.boundaryCondition(side1,0)==perfectElectricalConductor &&
//                mg.boundaryCondition(side2,1)==perfectElectricalConductor &&
//  	      mg.boundaryCondition(side3,2)==perfectElectricalConductor )
//  	    bcParams.setCornerBoundaryCondition(BoundaryConditionParameters::doNothingCorner,side1,side2,side3);

//        }
      
      // *** mgop.finishBoundaryConditions(fieldNext,bcParams);
      // ** No need to do any corners ***

      #ifdef USE_PPP
       if( orderOfAccuracyInSpace>2 )  // this doesn't seem to be needed for 2nd order ?
       {
	 real timea=getCPU();
	 fieldNext.updateGhostBoundaries();
	 timing(timeForUpdateGhostBoundaries)+=getCPU()-timea;
       }
      #endif

      fieldNext.periodicUpdate();
      
      timing(timeForBoundaryConditions)+=getCPU()-timeBC;

      // display(fieldNext,"fieldNext after finishBoundaryConditions","%7.4f ");
    }
    else if( cg.numberOfComponentGrids()==1 )
    {
      real timeBC=getCPU();
      fieldNext.periodicUpdate(C);
      timing(timeForBoundaryConditions)+=getCPU()-timeBC;
    }
    

  }
  if( debug & 4 )
  {
    if( mgp!=NULL )
      display(fields[next],sPrintF("fields[next] after advanceNFDTD, t=%8.2e",t+dt),debugFile,"%8.2e ");
    else
      cgfields[next].display(sPrintF("cgfields[next] after advanceNFDTD, t=%8.2e",t+dt),debugFile,"%8.2e ");
  }
  

  if( debug & 4 )
  {
    fPrintF(debugFile,"\n ******************* advanceStructured Errors BEFORE assignInterface t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile,"\n ******************* advanceStructured Errors BEFORE assignInterface t=%9.3e ********\n",t+dt);
    getErrors( next,t+dt,dt );
  }


  // ---- assign values at material interfaces ------
  assignInterfaceBoundaryConditions( current, t+dt, dt );  // is this the right place to do this?
  if( debug & 4 )
  {
    fPrintF(debugFile,"\n ******************* advanceStructured Errors after assignInterface t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile,"\n ******************* advanceStructured Errors after assignInterface t=%9.3e ********\n",t+dt);
    getErrors( next,t+dt,dt );
  }
  

  // Here we project the field to satsify Gauss's Law  div(E)=0 or div(E) = stuff
  // It seems important to project two steps in a row
  if( !projectBeforeBCs && projectThisStep )
  {
    
    project( numberOfStepsTaken, next, t+dt , dt );

    if( true )
    {
      // re-apply the BC;s
      for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
      {
	realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];
	realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];

	int option=0;
	assignBoundaryConditions( option, grid, t+dt, dt, fieldNext, fieldCurrent,current );
      }
    }

    const bool computeMaxNorms = numberOfStepsTaken<10  || (debug & 1);
    if( computeMaxNorms )
    {
      getMaxDivergence(next,t+dt);

      printF("===>> project: After project and BC's: |div(E)-rho|=%8.2e, |div(E)-rho|/|grad(E)|=%8.2e, step=%i, t=%9.3e\n",
	       divEMax,divEMax/gradEMax,numberOfStepsTaken,t+dt);
    
    }    

  }

  if( debug & 16 )
  {
    fPrintF(debugFile," ******************* advanceStructured: Errors at end t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile," ******************* advanceStructured: Errors at end t=%9.3e ********\n",t+dt);
    getErrors( next,t+dt,dt );
  }


  checkArrays("advanceStructured:end");
  
}


void Maxwell::
computeDissipation( int current, real t, real dt )
// =====================================================================================
// Compute the dissipation for the structured grid algorithm
// =====================================================================================
{
  OV_ABORT("Error"); // this function should not be called anymore

  if( artificialDissipation<=0. && artificialDissipationCurvilinear<=0. )
    return ;

  real time0=getCPU();
  
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();

  Index I1,I2,I3;
  Range C(ex,hz);
  const int next = (current+1) % numberOfTimeLevels;

  if( cgdissipation==NULL )
  {
    Range all;
    cgdissipation=new realCompositeGridFunction;
    cgdissipation->updateToMatchGrid(cg,all,all,all,C);
    if( numberOfDimensions==2 )
    {
      cgdissipation->setName("Ex dissipation",ex);
      cgdissipation->setName("Ey dissipation",ey);
      cgdissipation->setName("Hz dissipation",hz);
    }
    else
    {
      cgdissipation->setName("Ex dissipation",ex);
      cgdissipation->setName("Ey dissipation",ey);
      cgdissipation->setName("Ez dissipation",ez);
    }
  }
  
  assert( cgdissipation!=NULL );
  realCompositeGridFunction & cgdiss = (*cgdissipation);

  const real cMax=max(cGrid);

  int grid;
  for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
  {
    MappedGrid & mg = cg[grid];

  
    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
    realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];

    realArray & u = fieldCurrent;
    realArray & un =fieldNext;

    realArray & d = cgdiss[grid];


    // *NO* const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
    const bool isRectangular=mg.isRectangular();
    const real c = cGrid(grid);
    const real adc = isRectangular ? c*artificialDissipation : c*artificialDissipationCurvilinear;  // Do this *wdh* 090602 

    int extra=2;
    getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

    d(I1,I2,I3,C)=u(I1,I2,I3,C)-un(I1,I2,I3,C);

    getIndex(mg.gridIndexRange(),I1,I2,I3);

//      // make the coefficient depend on the solution:
//      realArray cd(I1,I2,I3,C);
//      cd = fabs(u(I1-1,I2,I3,C)+u(I1+1,I2,I3,C)+u(I1,I2-1,I3,C)+u(I1,I2+1,I3,C)-4.*u(I1,I2,I3,C))/
//               max(eps,(fabs( u(I1-1,I2,I3,C)+u(I1+1,I2,I3,C)+u(I1,I2-1,I3,C)+u(I1,I2+1,I3,C)+4.*u(I1,I2,I3,C))));
    
//      cd =min(artificialDissipation,cd);

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
    
    for( grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      realArray & d = (*cgdissipation)[grid];

      // const real adc=artificialDissipation*SQR(cMax); // scale dissipation by c^2 *wdh* 041103
      const bool isRectangular=mg.isRectangular();
      const real c = cGrid(grid);
      const real adc = isRectangular ? c*artificialDissipation : c*artificialDissipationCurvilinear;  // Do this *wdh* 090602 

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
  timing(timeForDissipation)+=getCPU()-time0;
}

// fourth order dissipation 2D: (D+D-)^2 *********
#define FD4A_2D(u,i1,i2,i3,c) \
      (     ( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
        -4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       +12.*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4A_3D(u,i1,i2,i3,c) \
      (     ( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c)+u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c)+u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) )   \
        -4.*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c)+u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c)+u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
       +18.*u(i1,i2,i3,c) )


// fourth order dissipation 2D: 
#define FD4V_2D(u,i1,i2,i3,c) \
      (     ax*( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c) )  \
           +ay*( u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )   \
          -ax4*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c) ) \
          -ay4*( u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
       + axy12*u(i1,i2,i3,c) )

// fourth order dissipation 3D:
#define FD4V_3D(u,i1,i2,i3,c) \
      (     ax*( u(i1-2,i2,i3,c)+u(i1+2,i2,i3,c) )\
           +ay*( u(i1,i2-2,i3,c)+u(i1,i2+2,i3,c) )\
           +az*( u(i1,i2,i3-2,c)+u(i1,i2,i3+2,c) ) \
          -ax4*( u(i1-1,i2,i3,c)+u(i1+1,i2,i3,c) ) \
          -ay4*( u(i1,i2-1,i3,c)+u(i1,i2+1,i3,c) ) \
          -az4*( u(i1,i2,i3-1,c)+u(i1,i2,i3+1,c) ) \
          +axyz18*u(i1,i2,i3,c) )

void Maxwell::
addFilter( int current, real t, real dt )
// =====================================================================================
//  Add the higher-order filter that requires two steps since the stencil is so wide.
//
//  Add to u[current] at the START of the step.
// =====================================================================================
{
  if( !applyFilter )
    return ;

  if( t<dt )
    printF(" addFilter: t=%9.3e, orderOfFilter=%i, frequency=%i, numberOfFilterIterations=%i, filterCoefficient=%g\n",
             t,orderOfFilter,filterFrequency,numberOfFilterIterations,filterCoefficient);

  real time0=getCPU();
  
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();

  Index I1,I2,I3;
  Range C(ex,hz);
  if( numberOfDimensions==3 )
    C=Range(ex,ez);
  
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  const int next = (current+1) % numberOfTimeLevels;

  bool useNextTimeLevelForDissipation =  numberOfTimeLevels==3 && timeSteppingMethod==modifiedEquationTimeStepping;

  if( !useNextTimeLevelForDissipation && cgdissipation==NULL )
  {
    // allocate extra space for the dissipation
    Range all;
    cgdissipation=new realCompositeGridFunction;
    cgdissipation->updateToMatchGrid(cg,all,all,all,C);
    if( numberOfDimensions==2 )
    {
      cgdissipation->setName("Ex dissipation",ex);
      cgdissipation->setName("Ey dissipation",ey);
      cgdissipation->setName("Hz dissipation",hz);
    }
    else
    {
      cgdissipation->setName("Ex dissipation",ex);
      cgdissipation->setName("Ey dissipation",ey);
      cgdissipation->setName("Ez dissipation",ez);
    }
  }
  
  assert( useNextTimeLevelForDissipation || cgdissipation!=NULL );
  realCompositeGridFunction & cgdiss = useNextTimeLevelForDissipation ? cgfields[next] : (*cgdissipation);

  if( useNextTimeLevelForDissipation && t<dt )
  {
    printF("*** addHigherOrderDissipation **** Use next time level as work space for the dissipation\n");
  }
  

  // Stage I
  //    D4 =  alpha*(D+xD-x)^2 + beta*(D+yD-y)^2
  //    D4^2 = alpha^2 (D+xD-x)^4 + 2*alpha*beta(D+xD-x)^2*(D+yD-y)^2 + beta^2 *(D+yD-y)^2
  //
  // We choose alpha^2 =1 for weak instabilities and alpha^2=1/dx for strong 


  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  const bool useOpt=true;

  for( int it=0; it<numberOfFilterIterations; it++ )
  {

    int option=0;  // Stage I 

    int ipar[] = {option, ex,ey,ez,hx,hy,hz, orderOfFilter,debug,myid };
    real ad=1.;   // changed below 
    real rpar[] = { ad,dt,t }; // 
    int ierr=0;
  
    option=0;   // Stage I 
    ipar[0]=option;
    
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
  
      realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
      realArray & u = fieldCurrent;
      realArray & d = cgdiss[grid];
      const intArray & mask = mg.mask();

#ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray dLocal; getLocalArrayWithGhostBoundaries(d,dLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
      realSerialArray & uLocal = u;
      realSerialArray & dLocal = d;
      const intSerialArray & maskLocal = mask;
#endif    
    
      getIndex(mg.gridIndexRange(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
      if( !ok ) continue;

      dLocal=0.;


      // int extra=2;
      // getIndex(mg.gridIndexRange(),I1,I2,I3,extra);
      // d(I1,I2,I3,C)=u(I1,I2,I3,C)-up(I1,I2,I3,C);

      //  const real dr[3]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2)}; //
      // const real ax = 1./sqrt(dr[0]), ay=1./sqrt(dr[1]), az=1./sqrt(dr[2]);
      // const real ax = 1., ay=1., az=1.;
      // const real ax4= 4.*ax, ay4=4.*ay, az4=4.*az;
      // const real axy12= 6.*(ax + ay);
      // const real axyz18=6.*(ax + ay + az);

      if( useOpt )
      {
	ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );
	mxFilter( numberOfDimensions,
		       uLocal.getBase(0),uLocal.getBound(0),
		       uLocal.getBase(1),uLocal.getBound(1),
		       uLocal.getBase(2),uLocal.getBound(2),
		       gidLocal(0,0),*uLocal.getDataPointer(),*dLocal.getDataPointer(),*maskLocal.getDataPointer(),
		       bcLocal(0,0), ipar[0],rpar[0],ierr );

      }
      else
      {
	getIndex(mg.gridIndexRange(),I1,I2,I3);
	where( mask(I1,I2,I3)>0 )
	{
	  for( int n=C.getBase(); n<=C.getBound(); n++ )
	  {
	    if( numberOfDimensions==2 )
	    {
	      d(I1,I2,I3,n)=FD4A_2D(u,I1,I2,I3,n);
	      // d(I1,I2,I3,n)=FD4V_2D(u,I1,I2,I3,n);
	    }
	    else
	    {
	      d(I1,I2,I3,n)=FD4A_3D(u,I1,I2,I3,n);
	    }
	  }
      
	}
      }
    
      cgdiss[grid].periodicUpdate();  
      cgdiss[grid].updateGhostBoundaries();  
    
    }

    // cgdiss.interpolate();
  
    // STAGE II


    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = cg[grid];
  
      realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
      realArray & u = fieldCurrent;

      realArray & d = cgdiss[grid];
      const intArray & mask = mg.mask();

#ifdef USE_PPP
      realSerialArray uLocal; getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray dLocal; getLocalArrayWithGhostBoundaries(d,dLocal);
      intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
      realSerialArray & uLocal = u;
      realSerialArray & dLocal = d;
      const intSerialArray & maskLocal = mask;
#endif  

      getIndex(mg.gridIndexRange(),I1,I2,I3);
      bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);
      if( !ok ) continue;


      const bool isRectangular=mg.isRectangular();

      // const real c = cGrid(grid);
      // const real adc = isRectangular ? c*artificialDissipation : c*artificialDissipationCurvilinear;  // Do this *wdh* 090602 

      // const real ad8 = -adc*dt/(256.*numberOfDimensions);  // scale by 2^8 = (D+D-)^4 (-1)^i = 4^4 
      // const real ad4 = -adc*dt/(16.*numberOfDimensions);  // scale by 2^8 = (D+D-)^4 (-1)^i = 4^4 
 
      // real ad8 = -adc*dt/(256.*numberOfDimensions);  

 
      // Here is the average dr for the grid : 
      // assert( numberOfGridPoints>0. );
      //const real drAve = 1./pow(numberOfGridPoints,1./numberOfDimensions);

      // scale the coeff. of art. diffusion as 1/dr 
      // real ad8 = ad8/drAve;
    

      // -------------------------------------------------------------------------
      // choose dissipation so that the mode (-1)^i has a damping factor of zero: 
      // -------------------------------------------------------------------------

      if( orderOfFilter==8 )
      {
	// Diss =[ (D+xD-x)^2 + (D+yD-y)^2 +(D+zD-z)^2 ]^2 
	//  (D+D-)^2 (-1)^i = 16 * (-1)^i 
	ad =  -filterCoefficient/SQR(16.*numberOfDimensions); 
      }
      else if( orderOfFilter==4 )
      {
	// Diss =[ (D+xD-x) + (D+yD-y) +(D+zD-z) ]^2 
	//  (D+D-) (-1)^i = 4 * (-1)^i 
	ad =  -filterCoefficient/SQR(4.*numberOfDimensions); 
      }
      else
      {
	OV_ABORT("Error : finish me");
      }
    


      if( t<dt )
	printF("addFilter: it=%i : grid=%i dt=%9.3e ad=%8.2e ad/dt=%8.2e \n",it,grid,dt,ad,ad/dt );
      //  printF(" grid=%i dt=%9.3e drAve=%8.2e ad8=%8.2e ad8/dt=%8.2e \n",grid,dt,drAve,ad8,ad8/dt );

      // ::display(d," d ","%6.2f ");
    
      // const real dr[3]={mg.gridSpacing(0),mg.gridSpacing(1),mg.gridSpacing(2)}; //
      // const real ax = 1./sqrt(dr[0]), ay=1./sqrt(dr[1]), az=1./sqrt(dr[2]);
      // const real ax = 1., ay=1., az=1.;

      // const real ax4= 4.*ax, ay4=4.*ay, az4=4.*az;
      // const real axy12= 6.*(ax + ay);
      // const real axyz18=6.*(ax + ay + az);

      // real ad8d = ad8*( 256.+2*36. + 256. );  // 2D 
      // assert( numberOfDimensions==2 );
    
      if( useOpt )
      {
	ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );
	option =1;  // stage II
	ipar[0] = option;
	rpar[0] = ad;  // coefficient of the dissipation
	mxFilter( numberOfDimensions,
		       uLocal.getBase(0),uLocal.getBound(0),
		       uLocal.getBase(1),uLocal.getBound(1),
		       uLocal.getBase(2),uLocal.getBound(2),
		       gidLocal(0,0),*uLocal.getDataPointer(),*dLocal.getDataPointer(),*maskLocal.getDataPointer(),
		       bcLocal(0,0), ipar[0],rpar[0],ierr );

      }
      else
      {
	int extra=0;
	getIndex(mg.gridIndexRange(),I1,I2,I3,extra);

	where( mask(I1,I2,I3)>0 )
	{
	  for( int n=C.getBase(); n<=C.getBound(); n++ )
	  {
	    if( numberOfDimensions==2 )
	    {
	      if( orderOfFilter==8 )
	      {
		u(I1,I2,I3,n) +=  ad * FD4A_2D(d,I1,I2,I3,n);

		// implicit diagonal term:
		// u(I1,I2,I3,n) = (u(I1,I2,I3,n) + ad8 * FD4A_2D(d,I1,I2,I3,n) - ad8d*u(I1,I2,I3,n))/( 1.-ad8d );

		// u(I1,I2,I3,n) +=  ad8 * FD4V_2D(d,I1,I2,I3,n);
		// u(I1,I2,I3,n) +=  ad4 * d(I1,I2,I3,n);
	      }
	      else 
	      {
		OV_ABORT("Error : finish me");
	      }
	
	    }
	    else
	    {
	      if( orderOfFilter==8 )
	      {
		u(I1,I2,I3,n) +=  ad * FD4A_3D(d,I1,I2,I3,n);
	      }
	      else
	      {
		OV_ABORT("Error : finish me");
	      }
	 
	    }
	  }
	}
      }
    
      fieldCurrent.periodicUpdate();  
      fieldCurrent.updateGhostBoundaries();  
    
      // ::display(u," u ","%6.2f ");
    }

    // ********************************************************
    if( false ) // *wdh* 2014/05/21 -- is this needed when grids overlap on a boundary ?
    {
      cgfields[current].interpolate();
    }
    
  }
  
  timing(timeForDissipation)+=getCPU()-time0;
}
