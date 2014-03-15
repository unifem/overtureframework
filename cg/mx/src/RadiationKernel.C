#include "RadiationKernel.h"

//       subroutine bcperq21(p,f,ploc,c,len,dt,n,m,md,ns,ord,fold,phi,amc,
//      &                    fftsave,bcinit)
// c
// c  this routine uses an adams-moulton formula to compute -
// c  in Fourier variables - 21-pole approximation to the
// c  planar kernel
// c
// c     (d/dt - c beta_j w) phi_j = c alpha_j w^2 phat 
// c
// c     fhat = sum phi_j
// c
// c     w = k*scl , k=1, ... 
// c      
// c  double precision: p(n,m) - m fields to which the operator should be applied
// c
// c  double precision: f(n,m) - the m results
// c
// c  double precision: ploc(n) - workspace 
// c
// c  double precision: c - the wave speed
// c
// c  double precision: len - the period
// c
// c  double precision: dt - the time step
// c
// c  integer: n the number of grid points - most efficient if it has small
// c           prime factors, preferably even
// c
// c  integer: m the number of fields
// c
// c  integer: md the maximum mode used in the bc md < n/2
// c
// c  integer: ns>=2*n+15 
// c
// c  integer: ord - time-stepping order - note that the stability domain for
// c                 Adams-Moulton methods gets small if this is too big
// c 
// c  complex*16: fold(0:ord-2,md,21,m) - stored values for time-stepping
// c
// c  complex*16: phi(md,21,m) - the auxiliary functions computed here
// c
// c  double precision: amc(-1:ord-2) - Adams-Moulton coefficients (computed here)
// c                              use amcof.f
// c
// c  double precision: fftsave(ns) - used by fftpack - link to rffti,rfftf,rfftb
// c
// c  integer bcinit: initialize to zero 
// c

#define bcperq21d EXTERN_C_NAME(bcperq21d)
#define bcperq31d EXTERN_C_NAME(bcperq31d)
#define bccyld EXTERN_C_NAME(bccyld)
extern "C"
{
  void bcperq21d( const int&nda,const int&ndb,double & p,double &f,double &ploc,double &c,double &len,double &dt,
                int&n,int&m,int&md,int&ns,int&ord,double &fold,double &phi,double &amc,
		double &fftsave,int&bcinit);

  void bcperq31d( const int&nda,const int&ndb,double & p,double &f,double &ploc,double &c,double &len,double &dt,
                int&n,int&m,int&md,int&ns,int&ord,double &fold,double &phi,double &amc,
		double &fftsave,int&bcinit);

  void bccyld(const int&nda,const int&ndb,
             double &p,double &f,double &ploc,double &c,double &r,double &dt,int&n,int&m,int&md,int&ns,int&ord,
             double &fold, double &phi,
             int&npoles,double &alpha,double &beta,double &amc,double &fftsave,int&bcinit);
}

real RadiationKernel::cpuTime=0.;

RadiationKernel::RadiationKernel()
// ======================================================================================
// This class is used to evaluate the Kernel of a Radiation boundary condition.
// ======================================================================================
{
  kernelType=planar;
  
  ploc=NULL;
  fold=NULL;
  phi=NULL;
  amc=NULL;
  fftsave=NULL;

  radius=1.;
  alpha=NULL;
  beta=NULL;
  npoles=NULL;
  
  bcinit=-1;
  
}



RadiationKernel::~RadiationKernel()
{
  delete [] ploc;
  delete [] fold;
  delete [] phi;
  delete [] amc;
  delete [] fftsave;
  delete [] alpha;
  delete [] beta;
  delete [] npoles;
}

int RadiationKernel::setKernelType( KernelTypeEnum type )
// ============================================================================
// /Description:
//   Set the kernel type. Call this before initialize.
// ============================================================================
{
  kernelType=type;
  return 0;
}

RadiationKernel::KernelTypeEnum RadiationKernel::
getKernelType() const
{
  return kernelType;
}



int RadiationKernel::initialize( int numberOfGridPoints_, int numberOfFields_, 
				 int numberOfModes_, real period_, real c_, 
				 int orderOfTimeStepping_, int numberOfPoles_,
                                 real radius_ /* =1. */ )
{
  numberOfGridPoints=numberOfGridPoints_;
  numberOfFields=numberOfFields_;
  numberOfModes=numberOfModes_;
  period=period_;
  c=c_;
  orderOfTimeStepping=orderOfTimeStepping_;
  numberOfPoles=numberOfPoles_;
  
  delete [] ploc;
  ploc = new double [numberOfGridPoints];
  
  ns=2*numberOfGridPoints+15;
  
  if( kernelType==planar )
  {

    delete [] fold;
    fold= new double [(orderOfTimeStepping-1)*numberOfModes*numberOfPoles*numberOfFields*2]; // complex
  
    delete [] phi;
    phi = new double [numberOfModes*numberOfPoles*numberOfFields*2];  // complex

    delete [] amc;
    amc = new double[numberOfModes];
    
    delete [] fftsave;
    fftsave = new double [ns];

  }

//  double precision: p(n,m) - m fields to which the operator should be applied
//
//  double precision: f(n,m) - the m results
//
//  double precision: ploc(n) - workspace 
//
//  double precision: c - the wave speed
//
//  double precision: r - the radius
//
//  double precision: dt - the time step
//
//  integer: n the number of grid points - most efficient if it has small
//           prime factors, preferably even
//
//  integer: m the number of fields
//
//  integer: md the maximum mode used in the bc md < n/2
//
//  integer: ns>=2*n+15 
//
//  integer: ord - time-stepping order - note that the stability domain for
//                 Adams-Moulton methods gets small if this is too big
// 
//  complex*16: fold(0:ord-2,0:md,44,m) - stored values for time-stepping
//
//  complex*16: phi(0:md,44,m) - the auxiliary functions computed here
//
//  integer: npoles(0:md) - #poles - computed here
//
//  complex*16: alpha(0:md,44) - the amplitudes computed here
//
//  complex*16: beta(0:md,44) - the poles computed here
//
//  double precision: amc(-1:ord-2) - Adams-Moulton coefficients (computed here)
//                              use amcof.f
//
//  double precision: fftsave(ns) - used by fftpack - link to rffti,rfftf,rfftb

  else if( kernelType==cylindrical )
  {
    const int mdp1=numberOfModes+1;

    numberOfPoles=44;  // for now all modes get 44 poles

    delete [] fold;
    fold= new double [(orderOfTimeStepping-1)*mdp1*numberOfPoles*numberOfFields*2]; // complex
  
    delete [] phi;
    phi = new double [mdp1*numberOfPoles*numberOfFields*2];  // complex

    delete [] amc;
    amc = new double[mdp1];
    
    delete [] fftsave;
    fftsave = new double [ns];
    
    delete [] npoles;
    npoles = new int [mdp1];
    
    delete [] alpha;
    alpha = new double [mdp1*numberOfPoles*2];  // complex

    delete [] beta;
    beta = new double [mdp1*numberOfPoles*2];  // complex

    
  }
  
  bcinit=0;
  
}


int RadiationKernel::evaluateKernel( double dt, RealArray & u, RealArray & hu )
// ========================================================================================
//
//  Assign the radiation kernel. 
//
//  This routine assumes that the input values run from u(0),...,u(numberOfGridPoints-1)
//
// 
//   The output hu(i) is defined for all i (with periodic images assigned too)
// ========================================================================================
{
  real time=getCPU();
  
  assert( ploc!=NULL );
  
  double *pu = u.getDataPointer();
  double *pf = hu.getDataPointer();
  
  assert( u.getBase(0)==hu.getBase(0) && u.getBound(0)==hu.getBound(0) );
  

  const int nda=u.getBase(0)+1, ndb=u.getBound(0)+1;  // add one (base one assumed in bcperq21d)
  
  assert( bcinit>=0 );
  
  if( kernelType==planar )
  {
    if( numberOfPoles==21 )
    {
      // 21 -pole approximation: advance the "numberOfModes" auxillary variables in time and
      // evaluate the Kernel 
      bcperq21d( nda,ndb,
		 *pu, *pf, *ploc, c,period,dt,numberOfGridPoints,numberOfFields,numberOfModes,ns,
		 orderOfTimeStepping,*fold,*phi,*amc, *fftsave,bcinit);
    }
    else if( numberOfPoles==31 )
    {
      // here is the 31 pole approximation
      bcperq31d( nda,ndb,
		 *pu, *pf, *ploc, c,period,dt,numberOfGridPoints,numberOfFields,numberOfModes,ns,
		 orderOfTimeStepping,*fold,*phi,*amc, *fftsave,bcinit);
    }
    else
    {
      Overture::abort("error");
    }
    
  }
  else if( kernelType==cylindrical )
  {
    bccyld(nda,ndb,
           *pu, *pf, *ploc, c,radius, dt,numberOfGridPoints,numberOfFields,numberOfModes,ns,
           orderOfTimeStepping, *fold,*phi,
	   *npoles, *alpha, *beta,*amc,*fftsave,bcinit);
  }
  else
  {
    printF("ERROR: un-implemented kernelType=%i\n",kernelType);
    Overture::abort("error");
  }
  

  // assign periodic images
  for( int i=hu.getBase(0); i<0; i++ )
  {
    for( int j=0; j<numberOfFields; j++ )
      hu(i,j)=hu(i+numberOfGridPoints,j);
  }
  for( int i=numberOfGridPoints; i<=hu.getBound(0); i++ )
  {
    for( int j=0; j<numberOfFields; j++ )
      hu(i,j)=hu(i-numberOfGridPoints,j);
  }
  
  cpuTime+=getCPU()-time;
  return 0;
}

