#include "Maxwell.h"
#include "display.h"
#include "CompositeGridOperators.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#define advSOSUP EXTERN_C_NAME(advsosup)

extern "C"
{
      void advSOSUP(const int&nd,
      const int&n1a,const int&n1b,const int&n2a,const int&n2b,const int&n3a,const int&n3b,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&nd4a,const int&nd4b, 
      const int&ndf4a,const int&ndf4b,  // dimensions for forcing f
      const int&mask,const real&rx,  
      const real&u, real&un, const real&f,
      const int&bc, const int&ipar, const real&rpar, int&ierr );

}

#define FOR_3(i1,i2,i3,I1,I2,I3) for( i3=I3.getBase(); i3<=I3.getBound(); i3++ )  for( i2=I2.getBase(); i2<=I2.getBound(); i2++ )  for( i1=I1.getBase(); i1<=I1.getBound(); i1++ )  

#define FN(m) fn[m+numberOfFunctions*(grid)]

// =============================================================================
//! Advance using the second-order-system upwind scheme
// =============================================================================
void Maxwell::
advanceSOSUP(  int numberOfStepsTaken, int current, real t, real dt )
{
  checkArrays("advanceSOSUP:start");

//    printF("advanceNFDTD: t=%e current=%i, numberOfFunctions=%i, numberOfTimeLevels=%i\n",t,
//        current,numberOfFunctions,numberOfTimeLevels);
  
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Range C(ex,hz); // these components do not include [ext,eyt,ezt]
  const int prev = (current-1+numberOfTimeLevels) % numberOfTimeLevels;
  const int next = (current+1) % numberOfTimeLevels;

  const real cMax=max(cGrid);


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

    // realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
    realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
    realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];


    // realArray & um = fieldPrev;
    realArray & u  = fieldCurrent;
    realArray & un = fieldNext;

    if( debug & 8 )
    {
      Communication_Manager::Sync();
      fPrintF(debugFile," **** start of advanceSOSUP t=%8.2e\n",t);
      // display(um,sPrintF("um start of advanceSOSUP, t=%8.2e",t),debugFile,"%8.2e ");
      display(u,sPrintF("u start of advanceSOSUP, t=%8.2e",t),debugFile,"%8.2e ");
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
      printF(" advanceSOSUP:INFO eps,mu,c=%8.2e %8.2e %8.2e for grid=%i (%s) \n",eps,mu,c,grid,
         (const char*)cg[grid].getName());
    
    const bool isRectangular=mg.isRectangular();

    const real dtsq=dt*dt; 
    const real csq=c*c;
    const real cdtsq=c*c*dt*dt;


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

    realArray f;
    const bool addForcing = forcingOption!=noForcing && forcingOption!=planeWaveBoundaryForcing;

    if( addForcing )
    {
      // Compute the forcing at the times needed for SOSUP

      // f(i1,i2,i3,C,m) :
      //    2nd-order: m=0,1      :   t,t+g11*dt
      //    4th-order: m=0,1,2,3  :   t-dt,t,t+g21*dt,t+g22*dt,t+dt   g11=(1-sqrt(3))/2, g22=(1+sqrt(3))/2 (Gauss points)
      //    6th-order: m=0,1,..,4 :   t-2*dt,t-dt,t,t+g31*dt,t+g32*dt,t+g33*dt,t+dt,t+2*dt
      const int numTimes = orderOfAccuracyInSpace/2 + 1 + orderOfAccuracyInSpace - 2;
      real tv[10];
      if( orderOfAccuracyInSpace==2 )
      {
        const real g11=.5;  // Gauss point
	tv[0]=t; tv[1]=t+g11*dt;
      }
      else if( orderOfAccuracyInSpace==4 )      
      {
        const real g21=.5*(1.-sqrt(1./3.)), g22=.5*(1.+sqrt(1./3.)); // Gauss points
	tv[0]=t-dt; tv[1]=t; tv[2]=t+dt; tv[3]=t+g21*dt; tv[4]=t+g22*dt; 
      }
      else if( orderOfAccuracyInSpace==6 )      
      {
        const real g31=.5*(1.-sqrt(3./5.)), g32=.5*(1.+.0), g33=.5*(1.+sqrt(3./5.)); // Gauss points
	tv[0]=t-2*dt; tv[1]=t-dt; tv[2]=t; tv[3]=t+dt; tv[4]=t+2*dt; tv[5]=t+g31*dt; tv[6]=t+g32*dt; tv[7]=t+g33*dt;
      }
      else
      {
        OV_ABORT("ERROR: finish me for orderOfAccuracyInSpace");
      }
      
      
      Index D1,D2,D3;
      getIndex(mg.dimension(),D1,D2,D3);
      f.partition(mg.getPartition());
      f.redim(D1,D2,D3,C,numTimes);  // could use some other array for work space ??

      realArray f0;  // if getForcing could take a local array we wouldn't need this extra copy
      f0.partition(mg.getPartition());
      f0.redim(D1,D2,D3,C);

      int option=1;  
      if( addForcing )
      {
	//kkc getForcing is called from advance but is also called from assignIC and getErrors.
	//    we have to add the timing in external to getForcing to avoid double counting the time
	//    in assignIC and getErrors
	real timef = getCPU();

	OV_GET_SERIAL_ARRAY(real,f,fLocal);
	OV_GET_SERIAL_ARRAY(real,f0,f0Local);
	

	for( int m=0; m<numTimes; m++ )
	{
          // evaluate the forcing at time tv[m]
	  if( t<=0 )
	    printF("advSOSUP: eval forcing at time t=%8.2e\n",tv[m]);
	  
	  getForcing( next, grid,f0,tv[m],dt,option );

          fLocal(D1,D2,D3,C,m)=f0Local;  
	}
	
	timing(timeForForcing) += getCPU()-timef;
      }
    }

#ifdef USE_PPP
      // realSerialArray umLocal;  getLocalArrayWithGhostBoundaries(um,umLocal);
      realSerialArray uLocal;   getLocalArrayWithGhostBoundaries(u,uLocal);
      realSerialArray unLocal;  getLocalArrayWithGhostBoundaries(un,unLocal);
      realSerialArray fLocal;   getLocalArrayWithGhostBoundaries(f,fLocal);
#else
      // const realSerialArray & umLocal = um;
      const realSerialArray & uLocal  =  u;
      const realSerialArray & unLocal = un;
      const realSerialArray & fLocal = f;
#endif

    bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3);

    if( useOpt ) 
    {
      // --- We always call advOpt ---
      real timeAdv=getCPU();
      
      const int useWhereMask = numberOfComponentGrids>1;
      int gridType = isRectangular? 0 : 1;
      int option=0; //  0=Maxwell+AD, 1=AD
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
                  0, // (int)useVariableDissipation,
                  0, // (int)useCurvilinearOptNew,
                  (int)useConservative,
                  0, // combineDissipationWithAdvance,
                  (int)useDivergenceCleaning,
                  ext,eyt,ezt,hxt,hyt,hzt};  //

      real dx[3]={1.,1.,1.};
      if( isRectangular )
	mg.getDeltaX(dx);
	  
      real rpar[30];
      rpar[ 0]=c;
      rpar[ 1]=dt;
      rpar[ 2]=dx[0];
      rpar[ 3]=dx[1];
      rpar[ 4]=dx[2];
      rpar[ 5]=0.; // adc;
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


      // real *umptr=umLocal.getDataPointer();
      real *uptr =uLocal.getDataPointer();
      real *unptr=unLocal.getDataPointer();
      
      real *fptr   = addForcing ? fLocal.getDataPointer() : uptr;

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
      if( ok )
      {
	advSOSUP(mg.numberOfDimensions(),
		   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
		   uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
		   uLocal.getBase(2),uLocal.getBound(2),
		   uLocal.getBase(3),uLocal.getBound(3),
  		   C.getBase(),C.getBound(), // bounds on component "4" of f 
		   *maskptr,*rxptr, *uptr,*unptr, *fptr, 
		   mg.boundaryCondition(0,0), ipar[0], rpar[0], ierr );
      }
      
      timeAdv=getCPU()-timeAdv;
      timing(timeForAdvOpt)+=timeAdv;
      
      if( debug & 8 )
      {
        display(unLocal,sPrintF("unLocal after advSOSUP, processor=%i before BC's t=%8.2e",
                         Communication_Manager::My_Process_Number,t),pDebugFile,"%8.2e ");

        display(un,sPrintF("un after advSOSUP, before BC's t=%8.2e",t),debugFile,"%8.2e ");
      }

//        printF(" p=%i time for advMaxwell=%e I1,I2,I3=[%i,%i][%i,%i][%i,%i]\n",Communication_Manager::My_Process_Number,timeAdv,
//                   I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound());
      
    } // end if( use Opt )
    


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
        ::display(f,sPrintF("  *** advanceSOSUP: Here is the forcing f grid=%i t=%9.3e ********\n",grid,t),
                    pDebugFile,"%7.4f ");
      fprintf(pDebugFile," *** advanceSOSUP: After advance, before BC, grid=%i t=%9.3e ********\n",grid,t);
      getErrors( next,t+dt,dt );
    }

  } // end grid
  
#undef FN

//    printF(" **** advanceSOSUP t=%e processor=%i\n",t,
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

  // -- for now we always extrapolate interpolation neighbours for the upwind schemes
  dbase.get<int>("extrapolateInterpolationNeighbours")=true;
  dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours")=orderOfAccuracyInSpace+1; // what should this be?
  // We need to increase the maximum allowable width to extrap interp neighbours
  GenericMappedGridOperators::setDefaultMaximumWidthForExtrapolateInterpolationNeighbours(
                                       dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours")+1);
  

  BoundaryConditionParameters extrapParams;
  Range Ca = cgfields[0][0].getLength(3); // all components
  for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
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

    // Extrapolate neighbours of interpolation points for the wider upwind stencil
    if( dbase.get<int>("extrapolateInterpolationNeighbours") ) 
    {
      extrapParams.orderOfExtrapolation=dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours");
      if( debug & 4 )
	printf("***advSOSUP: orderOfExtrapolationForInterpolationNeighbours=%i\n",
	       dbase.get<int>("orderOfExtrapolationForInterpolationNeighbours"));
    
      MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];
      fieldNext.setOperators(mgop);
      fieldNext.applyBoundaryCondition(Ca,BCTypes::extrapolateInterpolationNeighbours,BCTypes::allBoundaries,0.,t+dt,
			       extrapParams,grid);
    }

    if( true )
    {
       real timeBC=getCPU();
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
      display(fields[next],sPrintF("fields[next] after advanceSOSUP, t=%8.2e",t+dt),debugFile,"%8.2e ");
    else
      cgfields[next].display(sPrintF("cgfields[next] after advanceSOSUP, t=%8.2e",t+dt),debugFile,"%8.2e ");
  }
  

  if( debug & 4 )
  {
    fPrintF(debugFile,"\n ******************* advanceSOSUP Errors BEFORE assignInterface t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile,"\n ******************* advanceSOSUP Errors BEFORE assignInterface t=%9.3e ********\n",t+dt);
    getErrors( next,t+dt,dt );
  }


  // ---- assign values at material interfaces ------
  assignInterfaceBoundaryConditions( current, t+dt, dt );  // is this the right place to do this?
  if( debug & 4 )
  {
    fPrintF(debugFile,"\n ******************* advanceSOSUP Errors after assignInterface t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile,"\n ******************* advanceSOSUP Errors after assignInterface t=%9.3e ********\n",t+dt);
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
    fPrintF(debugFile," ******************* advanceSOSUP: Errors at end t=%9.3e ********\n",t+dt);
    fprintf(pDebugFile," ******************* advanceSOSUP: Errors at end t=%9.3e ********\n",t+dt);
    getErrors( next,t+dt,dt );
  }


  checkArrays("advanceSOSUP:end");
  
}

