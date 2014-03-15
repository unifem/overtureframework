#include "RadiationBoundaryCondition.h"
#include "RadiationKernel.h"
#include "OGFunction.h"

#define exmax EXTERN_C_NAME(exmax)
#define wpulse EXTERN_C_NAME(wpulse)
#define wdpulse EXTERN_C_NAME(wdpulse)
#define radEval EXTERN_C_NAME(radeval)

extern "C"
{

void exmax(double&Ez,double&Bx,double&By,const int &nsources,const double&xs,const double&ys,
           const double&tau,const double&var,const double&amp, const double&a,
           const double&x,const double&y,const double&time);

void wpulse(double&w,const int &nsources,const double&xs,const double&ys,
           const double&tau,const double&var,const double&amp, const double&a,
           const double&x,const double&y,const double&time);

void wdpulse(double&w,double&wt,double&wx,const int &nsources,const double&xs,const double&ys,
           const double&tau,const double&var,const double&amp, const double&a,
           const double&x,const double&y,const double&time);

void radEval(const int &nd, const int &nd1a,const int &nd1b,
             const int &nd2a,const int &nd2b,const int &nd3a,const int &nd3b,
             const int & gridIndexRange,real& u1,real&  u, real & xy, real & rsxy,
             const int &boundaryCondition, 
             const int &md1a,const int &md1b, const int &md2a,const int &md2b, real& huv, 
             const int &sd1a,const int &sd1b, const int &sd2a,const int &sd2b, 
             const int &sd3a,const int &sd3b, const int &sd4a,const int &sd4b, real& uSave,
             const int &ipar,const real& rpar, const int &ierr );

}

// ****** NOTE: these values should be the same as those in forcing.h
static const int nsources=1;
static double xs[nsources]={0.}, ys[nsources]={1.e-8*1./3.}, tau[nsources]={-.95}, var[nsources]={30.}, 
              amp[nsources]={1.};
static double period= 1.;  // period in y

#define getExactSolution EXTERN_C_NAME(getexactsolution)
extern "C"
{
void getExactSolution( real&x,real&y,real&t,int&ndx,int&ndy,int&ndt, real&wd )
// evaluate a derivative of the exact solution
//  ndx,ndy,ndt : number of x,y,t derivatives, e.g. ndx=1,ndy=0,ndt=1 returns w.xt
{
  real w,wt,wx,wy;
  
  if( ndx==0 && ndy==0 && ndt==0 )
  {
    wpulse(wd,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t);
  }
  else if( ndx==0 && ndy==0 && ndt==1 )
  {
    wdpulse(w,wt,wx,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t);
    wd=wt;
  }
  else if( ndx==1 && ndy==0 && ndt==0 )
  {
    wdpulse(w,wt,wx,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t);
    wd=wx;
  }
  else if( ndx==0 && ndy==0 && ndt==3 )
  {
    const real eps=1.e-3;
    real w1,w2,w3,w4;
    
    wpulse(w1,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t-2*eps);
    wpulse(w2,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t-  eps);
    wpulse(w3,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t+  eps);
    wpulse(w4,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t+2*eps);

    wd = (-w1 +2.*w2-2.*w3+w4)/(2.*eps*eps*eps);
    
  }
  else if( ndx==0 && ndy==0 && ndt==4 )
  {
    const real eps=1.e-2;

    real w1,w2,w3,w4,w5;
    
    wpulse(w1,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t-2*eps);
    wpulse(w2,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t-  eps);
    wpulse(w3,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t      );
    wpulse(w4,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t+  eps);
    wpulse(w5,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x,y,t+2*eps);

    wd = (w1 -4.*w2 +6.*w3 -4.*w4 +w5)/(eps*eps*eps*eps);
    
  }
  else if( ndx==1 && ndy==0 && ndt==1 )
  {
    const real eps=1.e-2;

    real w1,w2,w3,w4;
    
    wpulse(w1,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x-  eps,y,t-  eps);
    wpulse(w2,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x+  eps,y,t-  eps);
    wpulse(w3,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x-  eps,y,t+  eps);
    wpulse(w4,nsources,xs[0],ys[0],tau[0],var[0],amp[0],period,x+  eps,y,t+  eps);

    wd = (w4-w3-w2+w1)/(4.*eps*eps);
    
  }
  else
  {
    printF("getExactSolution:ERROR: ndx,ndy,ndt=%i,%i,%i\n",ndx,ndy,ndt);
  }
}
}



int RadiationBoundaryCondition::debug =0;
real RadiationBoundaryCondition::cpuTime=0.;


RadiationBoundaryCondition::RadiationBoundaryCondition(int orderOfAccuracy_ /* =4 */ )
// =========================================================================================
// /Description:
//     This object can be used to apply a radition boundary condition to one face of a grid.
// 
// =========================================================================================
{
  orderOfAccuracy=orderOfAccuracy_;
  numberOfDerivatives=orderOfAccuracy;
  currentTimeLevel=-1;
  currentTime=0.;
  numberOfTimeLevels=0;
  rside=-1;
  raxis=-1;
  
  tz=NULL;
  radiationKernel=NULL;
  
}



RadiationBoundaryCondition::~RadiationBoundaryCondition()
{
  delete radiationKernel;
}


int RadiationBoundaryCondition::
setOrderOfAccuracy(int orderOfAccuracy_ )
// =========================================================================================
// /Description:
//     Set the order of accuracy.
// 
// =========================================================================================
{
  orderOfAccuracy=orderOfAccuracy_;
  return 0;
}



int RadiationBoundaryCondition::
initialize( MappedGrid & mg, 
	    int side, int axis,
	    int nc1_/* =0 */, int nc2_/* =0 */, 
	    real c_ /* =1. */, real period_ /* =1. */, 
	    int numberOfModes_ /* =-1 */, 
	    int orderOfTimeStepping_ /* =-1 */, int numberOfPoles_ /* =-1 */ )
// ===============================================================================================
// /Description:
//     Initialize the boundary conditions.
//
// /mg (input) : apply BC to a face of this grid. (Fastest results if the number of grid points in 
//   the tangential direction is 2**M+1 )
// /side,axis : apply the radiation BC to this face of the grid.
// /nc1,nc2 : apply BC to these components n=nc1,nc1+1,...,nc1
// /numberOfGridPoints : the number of distinct grid points in the periodic direction. 
//                       Fastest results if this is a power of 2.
// /period (input) : solution is periodic on this interval
// /c (input) : wave speed.
// /numberOfModes (input) : specify the number of Fourier modes to use in the BC. 
// 
// ===============================================================================================
{
  rside=side;
  raxis=axis;

  nc1=nc1_;
  nc2=nc2_;
  assert( nc2-nc1 >=0 );

  const int axisp1=(axis+1) % mg.numberOfDimensions();
  if( !mg.isPeriodic(axisp1) )
  {
    printF("RadiationBoundaryCondition::initialize:ERROR: raxis=%i but mg.isPeriodic(axisp1=%i)=%i\n",
           raxis,axisp1,mg.isPeriodic(axisp1));
    Overture::abort("error");
  }
  
  if( radiationKernel==NULL )
    radiationKernel = new RadiationKernel; 
  

  // This is really the number of points in the Fourier transform:
  numberOfGridPoints=mg.gridIndexRange(1,axisp1)-mg.gridIndexRange(0,axisp1);

  numberOfModes=numberOfModes_;
  const int maxModes=numberOfGridPoints/2-1;  // this is the max allowed
  if( numberOfModes<=0 )
  {
    // guess the number of modes to use
    //  Interior scheme converges like  C1*h**M   where M=orderOfAccuracy
    //  Boundary scheme is spectral:    C2*h2**N   where N=1/h2 and h2=spacing for boundary: h2=L/numberOfModes
    //    Set:  C1*h**M = C2*h2**N  -> Nlog(h2) + log(C2) = M*log(h) + log(C1)
    //             Nlog(N) = M*log(1/h) + log(C1/C2)
//    numberOfModes=min(maxModes,max(8,numberOfGridPoints/4));  
    numberOfModes=min(maxModes,max(8,int(sqrt(numberOfGridPoints))));  
    printF(" **** RadiationBC: numberOfModes=%i (numberOfGridPoints=%i) ****\n",numberOfModes,numberOfGridPoints);
    
  }
  
  period=period_;
  c=c_;
  radius=-1.;
  
  const bool isRectangular = mg.isRectangular();

  if( period<0 )
  {
    if( isRectangular )
    {
      // determine the period (periodic interval)
      real dx[3];
      mg.getDeltaX(dx);
      period=dx[axisp1]*numberOfGridPoints; 
      printF("RaditionBoundaryCondition:INFO the periodic interval was computed to be period=%9.3e\n",period);

      if( orderOfAccuracy==2 )
      {
	numberOfDerivatives = 1 + 1;  // we save u,ux
      }
      else
      {
	numberOfDerivatives = 1 + 1 + 1 + 1;  // we save u,ux,uxx,uxxx
      }

    }
    else
    {
      // determine the radius 
      Mapping & map = mg.mapping().getMapping();
      realArray r(1,3), x(1,3);
      r=0.;
      r(0,axis)=(real)side;
      
      map.map(r,x);
      
      radius = sqrt( x(0,0)*x(0,0) + x(0,1)*x(0,1) );
      
      printF("RaditionBoundaryCondition:INFO kernelType is cylindrical and the radius was computed to be %10.4e\n",
            radius);

      radiationKernel->setKernelType(RadiationKernel::cylindrical);
    }

    if( orderOfAccuracy==2 )
    {
      numberOfDerivatives = 1 + 2;  // we save u,ux,uy
    }
    else
    {
      numberOfDerivatives = 1 + 2 + 3 + 4;  // we save u, ux,uy, uxx,uxy,uyy, uxxx,uxxy,uxyy,uyyy 
    }
    
  }
  
  orderOfTimeStepping=orderOfTimeStepping_;
  if( orderOfTimeStepping==-1 )
    orderOfTimeStepping=orderOfAccuracy+1;
  
  numberOfPoles=numberOfPoles_;
  if( numberOfPoles!=21 || numberOfPoles!=31 )
    numberOfPoles=21;


  const int numberOfComponents=nc2-nc1+1;
  const int numberOfFields=numberOfDerivatives*numberOfComponents;
  
  radiationKernel->initialize( numberOfGridPoints,numberOfFields, 
			       numberOfModes, period, c, 
			       orderOfTimeStepping, numberOfPoles, radius );
  
  
  numberOfTimeLevels=orderOfAccuracy+1;
  uSave.redim(numberOfGridPoints,numberOfTimeLevels,numberOfDerivatives,numberOfComponents);
  uSave=0.;
  
  currentTimeLevel=0;
  currentTime=0.;
  
}



int RadiationBoundaryCondition::
assignBoundaryConditions( realMappedGridFunction & u, real t, real dt,
			  realMappedGridFunction & u2 )
// ==============================================================================================================
// /Description:
//     Assign the radiation boundary conditions.
//
//  u (input/output) : u at time t at interior and boundary points. This routine will assign ghost points of u
//  u2 (input): u at the previous time, u(t-dt)
// ==============================================================================================================
{
  
  if( t<0 ) return 0;  // Do this for now -- assume the solution is zero near the radiation boundary
  if( t==0 && t==currentTime ) return 0;

  real time0=getCPU();
  assert( t>currentTime );  // 

  currentTime=t;
  currentTimeLevel = (currentTimeLevel +1 ) % numberOfTimeLevels;

  if( debug>0 )
    printF("RadiationBoundaryCondition::assignBoundaryConditions t=%9.3e dt=%8.2e rside=%i raxis=%i \n",
            t,dt,rside,raxis);
  

  const RadiationKernel::KernelTypeEnum kernelType = radiationKernel->getKernelType();
  
  MappedGrid & mg = *( u.getMappedGrid() );
  
  const bool isRectangular = mg.isRectangular();
  if( kernelType==RadiationKernel::planar ) 
    assert( isRectangular );

  real dx[3];
  mg.getDeltaX(dx);


  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  int i1,i2,i3;


  const int side=rside, axis=raxis;
  
  RealArray & ub = uSave; 
  
  int n1a,n1b,n2a,n2b,n3a,n3b;
  if( axis==0 )
  {
    n1a=mg.gridIndexRange(side,0); n1b=n1a;
    n2a=mg.gridIndexRange(0,1);    n2b=mg.gridIndexRange(1,1)-1;
    n3a=mg.gridIndexRange(0,2);    n3b=mg.gridIndexRange(1,2);
  }
  else
  {
    n1a=mg.gridIndexRange(0,0);    n1b=mg.gridIndexRange(1,0)-1;
    n2a=mg.gridIndexRange(side,1); n2b=n2a;
    n3a=mg.gridIndexRange(0,2);    n3b=mg.gridIndexRange(1,2);
  }
  
  const int axisp1=(axis+1)%mg.numberOfDimensions();
  Range R(mg.dimension(0,axisp1),mg.dimension(1,axisp1));
  Range I(mg.gridIndexRange(0,axisp1),mg.gridIndexRange(1,axisp1)-1); 
	  
  const int m=currentTimeLevel;
  const int mm1= (m-1+numberOfTimeLevels) % numberOfTimeLevels;
  const int mm2= (m-2+numberOfTimeLevels) % numberOfTimeLevels;
  const int mm3= (m-3+numberOfTimeLevels) % numberOfTimeLevels;
  const int mm4= (m-4+numberOfTimeLevels) % numberOfTimeLevels;

  i3=n3a;
  if( axis==0 )
  {
    i1=side==0 ? n1a : n1b;
    for( int n=nc1; n<=nc2; n++ )
    for( int i2=n2a; i2<=n2b; i2++ )
    {
      ub(i2,m,0,n)=u(i1,i2,i3,n);         // save solution at time t
    }
  }
  else
  {
    i2=side==0 ? n2a : n2b;
    for( int n=nc1; n<=nc2; n++ )
    for( int i1=n1a; i1<=n1b; i1++ )
    {
      ub(i1,m,0,n)=u(i1,i2,i3,n);         // save solution at time t
    }
    
  }
  

  // ***** Evaluate the kernel ***

  const int numberOfComponents=nc2-nc1+1;
  const int numberOfFields=numberOfDerivatives*numberOfComponents;
  
  Range N=numberOfFields; 
  RealArray vv(R,N), huv(R,N);  // do all kernel evaluations at once

  #define vvn(i,m,n) vv(i,m+numberOfDerivatives*(n-nc1))

  const int numberOfSubSteps=1;  // These should no longer be needed with exponential method
  const real dts=dt/numberOfSubSteps;
  for( int subStep=1; subStep<=numberOfSubSteps; subStep++ )
  {
    // *** NOTE final time we reach is t-dt ****

    
    const int interpOrder=3;
	      
    if( numberOfSubSteps )
    {
      for( int n=nc1; n<=nc2; n++ )
      for( int i=0; i<numberOfDerivatives; i++ )
	vvn(I,i,n)=ub(I,mm1,i,n); 
    }
    else if( interpOrder==2 )
    {
      const real beta = subStep/real(numberOfSubSteps);

      for( int n=nc1; n<=nc2; n++ )
      for( int i=0; i<numberOfDerivatives; i++ )
	vvn(I,i,n)=beta*ub(I,mm1,i,n)   +(1.-beta)*ub(I,mm2,i,n); 
    }
    else
    { // beta should be in (1,2]
      const real beta = 1. + subStep/real(numberOfSubSteps);

      real q0=(beta-1.)*(beta-2.)/( 2.);
      real q1=(beta   )*(beta-2.)/(-1.);
      real q2=(beta   )*(beta-1.)/( 2.);

      for( int n=nc1; n<=nc2; n++ )
      for( int i=0; i<numberOfDerivatives; i++ )
	vvn(I,i,n)=q2*ub(I,mm1,i,n)   +q1*ub(I,mm2,i,n)   +q0*ub(I,mm3,i,n);
    }
	      

    // this next call will assign periodic images on hu also.
    if( tz==NULL )
    {
      radiationKernel->evaluateKernel( dts, vv, huv ); 
    }
    else
    {
      huv=0; // do this for now

      // we should set
      // huv(i,0) = -[ u.t + u.r + u/(2r) ]/c

      

    }
    
  }
  #undef vvn 

  // opt version 

  real *uptr = u.getDataPointer();
  real *u2ptr = u2.getDataPointer();
  real *puSave = uSave.getDataPointer();

  real *pxy = uptr;  // set default if not used.
  real *prsxy=uptr;  

  if( !isRectangular )
  {
    mg.update(MappedGrid::THEvertex | MappedGrid::THEinverseVertexDerivative); 
    pxy=mg.vertex().getDataPointer(); 
    prsxy=mg.inverseVertexDerivative().getDataPointer(); 

  }
  else if( debug>0 )
  {
    mg.update(MappedGrid::THEvertex); 
    pxy=mg.vertex().getDataPointer(); 
  }
  
  const int base=huv.getBase(0);
  real *phuv= huv.getDataPointer(); 
    

  const IntegerArray & dim = mg.dimension();
  int ierr=0;
  int ipar[30];
  real rpar[30];
			  
  int grid=0;
  int gridType=0;  // rectangular

  real eps=1., mu=1.; // not used

  real alpha=orderOfAccuracy==2 ? 1. : 1.;  // dissipation term -- 10 is probably too large

  ipar[0] = side;
  ipar[1] = axis;
  ipar[2] = grid;
  ipar[3] = n1a;
  ipar[4] = n1b;
  ipar[5] = n2a;
  ipar[6] = n2b;
  ipar[7] = n3a;
  ipar[8] = n3b;
  ipar[9] = nc1;
  ipar[10]= nc2; 
  ipar[11]= currentTimeLevel;
  ipar[12]= numberOfTimeLevels;

  ipar[13]= gridType;
  ipar[14]= orderOfAccuracy;
  ipar[15]= debug;
  ipar[16]= (int)kernelType;
  
     	      			   
  rpar[0] = dx[0];
  rpar[1] = dx[1];
  rpar[2] = dx[2];
  rpar[3] = mg.gridSpacing(0);
  rpar[4] = mg.gridSpacing(1);
  rpar[5] = mg.gridSpacing(2);
  rpar[6] = alpha;
  // assert( tz!=NULL );
  rpar[7]=(real &)tz;  // twilight zone pointer
     	      			   
  rpar[10]= t;
  rpar[11]= dt;
  rpar[12]= eps;
  rpar[13]= mu;
  rpar[14]= c;



  radEval( mg.numberOfDimensions(), 
	   dim(0,0),dim(1,0),dim(0,1),dim(1,1),dim(0,2),dim(1,2),
	   mg.gridIndexRange(0,0), *uptr, *u2ptr, *pxy, *prsxy,
           mg.boundaryCondition(0,0),
	   huv.getBase(0),huv.getBound(0),0,numberOfDerivatives-1,*phuv, 
           uSave.getBase(0),uSave.getBound(0),
           uSave.getBase(1),uSave.getBound(1),
           uSave.getBase(2),uSave.getBound(2),
           uSave.getBase(3),uSave.getBound(3),*puSave,
           ipar[0], rpar[0], ierr );
	       

  cpuTime+=getCPU()-time0;
  return 0;
  
}

