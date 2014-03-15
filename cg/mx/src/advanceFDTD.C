#include "Maxwell.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ParallelGridUtility.h"

#define mxYee EXTERN_C_NAME(mxyee)
#define mxYeeIcErr EXTERN_C_NAME(mxyeeicerr)

extern "C"
{
 void mxYee(const int&nd,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int&ndf1a,const int&ndf1b,const int&ndf2a,const int&ndf2b,const int&ndf3a,const int&ndf3b,
      const int & gridIndexRange, const real & um, const real & u, real & un, real & f, const int & media, 
      const real & epsv, const real & muv, const real & sigmaEv, const real & sigmaHv,
      const int&mask, const real & uKnown, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );

 void mxYeeIcErr(const int&nd,
      const int&nd1a,const int&nd1b,const int&nd2a,const int&nd2b,const int&nd3a,const int&nd3b,
      const int & gridIndexRange, real & u, real & v,
      const int&mask, const real & uKnown, const int&boundaryCondition, const int&ipar, const real&rpar, int&ierr );
}

// =============================================================================
//! Advance a step with the Yee scheme. -- Cartesian grids only ---
// =============================================================================
void Maxwell::
advanceFDTD(  int numberOfStepsTaken, int current, real t, real dt )
{
 
  assert( cgp!=NULL );
  CompositeGrid & cg= *cgp;
  const int numberOfDimensions = cg.numberOfDimensions();
  const int numberOfComponentGrids = cg.numberOfComponentGrids();

  assert( numberOfComponentGrids==1 );

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  const int prev = (current-1 +numberOfTimeLevels) % numberOfTimeLevels;
  const int next = (current+1) % numberOfTimeLevels;

  int grid=0;
  real time0=getCPU();

  MappedGrid & mg = cg[grid];
//   assert( mgp==NULL || op!=NULL );
//   MappedGridOperators & mgop = mgp!=NULL ? *op : (*cgop)[grid];

  realMappedGridFunction & fieldPrev    =mgp!=NULL ? fields[prev]    : cgfields[prev][grid];
  realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
  realMappedGridFunction & fieldNext    =mgp!=NULL ? fields[next]    : cgfields[next][grid];

  realArray & um = fieldPrev;
  realArray & u  = fieldCurrent;
  realArray & un = fieldNext;

  const bool isRectangular=mg.isRectangular();
  assert( isRectangular );

  realArray f; // fix me !
  int addSourceTerm = false; // add an addition source term (e.g. current J)

//   if(  addSourceTerm )
//   {
//     Index D1,D2,D3;
//     getIndex(mg.dimension(),D1,D2,D3);
//     f.partition(mg.getPartition());
//     f.redim(D1,D2,D3,C);  // could use some other array for work space ??
//   }
  
#ifdef USE_PPP
  realSerialArray uLocal;   getLocalArrayWithGhostBoundaries(u,uLocal);
  realSerialArray unLocal;  getLocalArrayWithGhostBoundaries(un,unLocal);
  realSerialArray umLocal;  getLocalArrayWithGhostBoundaries(um,umLocal);
  realSerialArray fLocal;   getLocalArrayWithGhostBoundaries(f,fLocal);
#else
  const realSerialArray & uLocal  =  u;
  const realSerialArray & unLocal = un;
  const realSerialArray & umLocal = um;
  const realSerialArray & fLocal = f;
#endif

  getIndex(mg.gridIndexRange(),I1,I2,I3);
  int includeGhost=0;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);

  const int useForcing = false; // Set to true if we have a current force
  const bool centerNeeded=false;

  const IntegerArray & gridIndexRange = mg.gridIndexRange();
  const IntegerArray & boundaryCondition = mg.boundaryCondition();
  
  IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  
  // ** fix for parallel: 
  //gidLocal=gridIndexRange;
  //bcLocal=boundaryCondition;

  // The next routine wants the indexRange -- is this a problem? 
  ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldNext,gidLocal,dimLocal,bcLocal );

  if( debug & 2 )
  {
    ::display(bcLocal,"bcLocal",pDebugFile,"%i ");
  }
  

  // --- We assume that we have two parallel ghost points and that we will update the fields
  //     on the first parallel ghostline : thus we only need to communicate once at the end of the Yee step ---
  // *** We may not actually need 2 ghost lines ***
  gidLocal=gridIndexRange;
  for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    gidLocal(0,axis)=max(uLocal.getBase(axis)+1,gidLocal(0,axis));
    gidLocal(1,axis)=min(uLocal.getBound(axis)-1,gidLocal(1,axis));
  }
  


  if( ok && media.getLength(0)==0 )
  {
    // -- set default values for the media if they have not already been set --

    media.redim(uLocal.dimension(0),uLocal.dimension(1),uLocal.dimension(2));
    media=0;

    epsv.redim(1); muv.redim(1); sigmaEv.redim(1); sigmaHv.redim(1);

    epsv=eps;
    muv=mu;
    sigmaEv=0.;
    sigmaHv=0.;
  }
  
  int option=0;
  if( ok )
  {
    int ipar[]={option,
		ex,ey,ez,hx,hy,hz,epsc,muc,sigmaEc,sigmaHc,
		grid,
		debug,
                (int)addSourceTerm,
		(int)useForcing,
		forcingOption,
                gridIndexRange(0,0),   // for computing x,y,z coordinates on a Cartesian grid
                gridIndexRange(0,1),
                gridIndexRange(0,2),
                (int)initialConditionOption,
                maskBodies,
                (int)knownSolutionOption,
                (int)useTwilightZoneMaterials,
                myid,
                numberOfPlaneMaterialInterfaceCoefficients };

    assert( isRectangular );
    real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    mg.getRectangularGridParameters( dx, xab );

	  
    real rpar[40+numberOfPlaneMaterialInterfaceCoefficients];
    rpar[ 0]=dx[0];
    rpar[ 1]=dx[1];
    rpar[ 2]=dx[2];
    rpar[ 3]=t;
    rpar[ 4]=(real &)tz;  // twilight zone pointer
    rpar[ 5]=dt;
    rpar[ 6]=c;
    rpar[ 7]=kx; // for plane wave scattering
    rpar[ 8]=ky;
    rpar[ 9]=kz;
    rpar[10]=slowStartInterval;
    rpar[11]=xab[0][0];  // for computing x,y,z coordinates on a Cartesian grid
    rpar[12]=xab[0][1];
    rpar[13]=xab[0][2];
    rpar[14]=pwc[0];
    rpar[15]=pwc[1];
    rpar[16]=pwc[2];
    rpar[17]=pwc[3];
    rpar[18]=pwc[4];
    rpar[19]=pwc[5];
      
    //plane material interface:
    for( int n=0; n<numberOfPlaneMaterialInterfaceCoefficients; n++ )
      rpar[20+n]=pmc[n];

    real *uptr =uLocal.getDataPointer();
    real *unptr=unLocal.getDataPointer();
    real *umptr=umLocal.getDataPointer();
      
    real *fptr   = addSourceTerm ? fLocal.getDataPointer() : uptr;


    const intArray & mask = mg.mask();
#ifdef USE_PPP
    intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
#else
    const intSerialArray & maskLocal = mask; 
#endif
    // ***NOTE*** pmask points to the bodyMask
    if( maskBodies )
    {
      assert( pBodyMask!=NULL );
    }
    int *pmask = maskBodies ? pBodyMask->getDataPointer() : maskLocal.getDataPointer();

    const realArray & ug = knownSolution!=NULL ? (*knownSolution)[grid] : u;
#ifdef USE_PPP
    realSerialArray ugLocal;  getLocalArrayWithGhostBoundaries(ug,ugLocal);
#else
    const realSerialArray & ugLocal = ug; 
#endif
    int ierr=0;
    
    mxYee(mg.numberOfDimensions(),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
	  uLocal.getBase(2),uLocal.getBound(2),
	  uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),
	  uLocal.getBase(2),uLocal.getBound(2),
	  gidLocal(0,0), *umptr, *uptr, *unptr, *fptr, *media.getDataPointer(), 
	  epsv(0), muv(0), sigmaEv(0), sigmaHv(0),
	  *pmask, *ugLocal.getDataPointer(), bcLocal(0,0), ipar[0], rpar[0], ierr );

      
  } // end if ok 
    
  timing(timeForAdvOpt)+=getCPU()-time0;
    
  
  // *** update periodic points and parallel ghost ****    
  time0=getCPU();
  fieldNext.periodicUpdate(); 
  fieldNext.updateGhostBoundaries(); 
  timing(timeForUpdateGhostBoundaries)+=getCPU()-time0;

//   int option=1;
//   int grid=0;  // **********
//   assignBoundaryConditions( option, grid, t, dt, field[next], field[current],current );
//   field[next].periodicUpdate(Range(ex,ey));

//   un(I1,I2,I3,hz)=u(I1,I2,I3,hz) + ((dt/(mu*dx[1]))*( un(I1,I2+1,I3,ex)-un(I1,I2,I3,ex) )-
// 				    (dt/(mu*dx[0]))*( un(I1+1,I2,I3,ey)-un(I1,I2,I3,ey) ));

//   option=2;
//   assignBoundaryConditions( option, grid, t, dt, field[next], field[current], current );
//   field[next].periodicUpdate(Range(hz,hz));

}


// ===================================================================================================
/// \brief compute various quantities for the FDTD "Yee" scheme
/// \param option (input) : 
///           option=0 : compute initial conditions, 
///           option=1 : compute errors
///           option=2 : compute div(E) , div(H)
///           option=3 : compute node centered fields for plotting
/// \param ipar (input) :
///          iparam[0] = nDivE : save div(E) in this component of v 
///          iparam[1] = nDivH : save div(H) in this component of v 
// ===================================================================================================
int Maxwell::
getValuesFDTD(int option, int *iparam, int current, real t, real dt, realCompositeGridFunction *pv /* = NULL */ )
{

  assert( cgp!=NULL );
  CompositeGrid & cg = *cgp;
  const int numberOfDimensions=cg.numberOfDimensions();

  if( option==2 )
  {
    divEMax=0.;
    divHMax=0.;
    gradEMax=0.;  
    gradHMax=0.;  
  }

  cg.update(MappedGrid::THEmask );

  const int grid=0;
  MappedGrid & mg = cg[grid];
  const intArray & mask = mg.mask();

  realMappedGridFunction & fieldCurrent =mgp!=NULL ? fields[current] : cgfields[current][grid];
  realArray & u  = fieldCurrent;
    
  realMappedGridFunction & v = pv!=NULL ? (*pv)[grid] : fieldCurrent;  

#ifdef USE_PPP
  realSerialArray uLocal;   getLocalArrayWithGhostBoundaries(u,uLocal);
  intSerialArray maskLocal;  getLocalArrayWithGhostBoundaries(mask,maskLocal);
  realSerialArray vLocal; getLocalArrayWithGhostBoundaries(v,vLocal);
#else
  const realSerialArray & uLocal  =  u;
  const intSerialArray & maskLocal = mask; 
  realSerialArray & vLocal = v;
#endif
  int extra=0;
  Index I1,I2,I3;  // these are not used below
  getIndex(mg.dimension(),I1,I2,I3,extra);
  int includeGhost=0;
  bool ok = ParallelUtility::getLocalArrayBounds(u,uLocal,I1,I2,I3,includeGhost);
  if( ok )
  {
    real *uptr = uLocal.getDataPointer();

    const realArray & ug = knownSolution!=NULL ? (*knownSolution)[grid] : u;
    #ifdef USE_PPP
      realSerialArray ugLocal;  getLocalArrayWithGhostBoundaries(ug,ugLocal);
    #else
      const realSerialArray & ugLocal = ug; 
    #endif

    if( maskBodies )
    {
      assert( pBodyMask!=NULL );
    }
    int *pmask = maskBodies ? pBodyMask->getDataPointer() : maskLocal.getDataPointer();

    const IntegerArray & gridIndexRange = mg.gridIndexRange();
    const IntegerArray & boundaryCondition = mg.boundaryCondition();
  
    IntegerArray gidLocal(2,3), dimLocal(2,3), bcLocal(2,3);
  
    // ** fix for parallel: 
    // gidLocal=gridIndexRange;
    // bcLocal=boundaryCondition;
    // The next routine wants the indexRange -- is this a problem? 
    ParallelGridUtility::getLocalIndexBoundsAndBoundaryConditions( fieldCurrent,gidLocal,dimLocal,bcLocal );
    if( option==0 )
    {
      // fill in ghost values on the initial conditions -- these are used by the symmetry BC
      gidLocal=mg.dimension();
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	gidLocal(0,axis)=max(uLocal.getBase(axis),gidLocal(0,axis));
	gidLocal(1,axis)=min(uLocal.getBound(axis),gidLocal(1,axis));
      }
    }
    else if( option==1 )
    {
      // compute errors
      vLocal=0.;
      gidLocal=gridIndexRange;
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	gidLocal(0,axis)=max(uLocal.getBase(axis)+1,gidLocal(0,axis));
	gidLocal(1,axis)=min(uLocal.getBound(axis)-1,gidLocal(1,axis));
      }
    }
    else if( option==3 )
    {
      // compute node centered values for plotting
      gidLocal=gridIndexRange;
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	gidLocal(0,axis)=max(uLocal.getBase(axis)+1,gidLocal(0,axis));
	gidLocal(1,axis)=min(uLocal.getBound(axis),gidLocal(1,axis));
      }
    }
  


    real dx[3]={1.,1.,1.}, xab[2][3]={{0.,0.,0.},{0.,0.,0.}};
    mg.getRectangularGridParameters( dx, xab );

    const int nDivE=iparam[0];  // save div(E) in this component of v 
    const int nDivH=iparam[1];  // save div(H) in this component of v 
    int optionYee = option; 
    int ipar[]={optionYee,
		ex,ey,ez,hx,hy,hz,
		grid,
		debug,
		(int)initialConditionOption,
		forcingOption,
		gridIndexRange(0,0),   // for computing x,y,z coordinates on a Cartesian grid
		gridIndexRange(0,1),
		gridIndexRange(0,2),
		nDivE,  // save div(E) here 
                nDivH,  // save div(H) here 
                (int)knownSolutionOption,
                maskBodies,
                myid,
                numberOfPlaneMaterialInterfaceCoefficients };

    real rpar[40+numberOfPlaneMaterialInterfaceCoefficients];
    rpar[ 0]=dx[0];
    rpar[ 1]=dx[1];
    rpar[ 2]=dx[2];
    rpar[ 3]=t;
    rpar[ 4]=(real &)tz;  // twilight zone pointer
    rpar[ 5]=dt;
    rpar[ 6]=c;
    rpar[ 7]=kx; // for plane wave scattering
    rpar[ 8]=ky;
    rpar[ 9]=kz;
    rpar[10]=slowStartInterval;
    rpar[11]=xab[0][0];  // for computing x,y,z coordinates on a Cartesian grid
    rpar[12]=xab[0][1];
    rpar[13]=xab[0][2];
    rpar[14]=pwc[0];
    rpar[15]=pwc[1];
    rpar[16]=pwc[2];
    rpar[17]=pwc[3];
    rpar[18]=pwc[4];
    rpar[19]=pwc[5];

    //plane material interface:
    for( int n=0; n<numberOfPlaneMaterialInterfaceCoefficients; n++ )
      rpar[20+n]=pmc[n];
    

    int ierr=0;
    mxYeeIcErr(mg.numberOfDimensions(),
	       uLocal.getBase(0),uLocal.getBound(0),uLocal.getBase(1),uLocal.getBound(1),uLocal.getBase(2),uLocal.getBound(2),
	       gidLocal(0,0), *uptr, *vLocal.getDataPointer(), 
	       *pmask, *ugLocal.getDataPointer(), bcLocal(0,0), ipar[0], rpar[0], ierr );
    
    if( option==1 )
    {
      // fill in errors and solution norms

      const int numberOfComponents=3*(numberOfDimensions-1);
      if( debug & 8 )
	printF(" Return from mxYeeIcErr rpar[20],...=%e %e %e %e %e %e\n",rpar[20],rpar[21],rpar[22],
	       rpar[23],rpar[24],rpar[25]);
      
      
      for( int n=0; n<numberOfComponents; n++ )
      {
	maximumError(ex+n)=rpar[20+n];
        solutionNorm(ex+n)=rpar[20+n+numberOfComponents];
      }
    }
    
    if( option==2 )
    {
      divEMax =rpar[30];
      divHMax =rpar[31];
      gradEMax=rpar[32]; 
      gradHMax=rpar[33]; 
      // printF("Return from mxYeeIcErr: divEMax=%e, gradEMax=%e\n",divEMax,gradEMax);
      
   }
    
  }
  if( option==2 )
  {
    divEMax=getMaxValue(divEMax);
    divHMax=getMaxValue(divHMax);
    gradEMax=getMaxValue(gradEMax);
    gradHMax=getMaxValue(gradHMax);
    
  }
  

  
  return 0;
}
