#include "FourierOperators.h"

#define RFFTI EXTERN_C_NAME(rffti)
#define RFFTF EXTERN_C_NAME(rfftf)
#define RFFTB EXTERN_C_NAME(rfftb)

extern "C"
{
  void RFFTI( int & nx, real & wsave );
  void RFFTF( int & nx, real & u, real & wsave );
  void RFFTB( int & nx, real & u, real & wsave );
}


// ================ General info  ========

//\begin{>FourierOperatorsInclude.tex}{\subsection{General Info}} 
//\no function header:
// 
//  Use this class to perform various operations on the Fourier transform of a
//  real valued function such as
// \begin{itemize}
//   \item forward and reverse transforms
//   \item derivatives and integrals in fourier space 
// \end{itemize}
//
//  This class can be used to implement (pseudo) spectral appoximations to PDEs.
//  The Overture class MappedGridOperators uses this class to compute spectral derivatives.
// 
//  The fourier transform is represented as a real transform (sine-cosine).
//   
//   By default the elements of the arrays that we operate on are 
//      u(0:nx-1,0:ny-1,0:nz-1,C) where $C$ is a Range that species which components to operate on.
//    (The array dimensions can be different from 0:nx-1, etc.).
//   This can be changed to the form
//      $u(R1,R2,R3,C)$ where $R1$ has length $nx$, $R2$ has length $ny$ and $R3$ has length $nz$.
//
// In practice you may keep a duplicate point in the array. You may declare
//   an array to be u(0:nx,0:ny,0:nz) where u(0,all,all) == u(nx,all,all)
// These routines only change the values u(0:nx-1,0:ny-1,0:nz-1,C).
//  
//\end{MappedGridFunctionInclude.tex}


//\begin{>>FourierOperatorsInclude.tex}{\subsection{Constructors}} 
FourierOperators::
FourierOperators(const int & numberOfDimensions_, 
                 const int & nx_,  
                 const int & ny_ /* =1 */ , 
                 const int & nz_ /* =1 */ )
//-----------------------------------------------------------------------------------------
// /Description: Define the number of space dimensions and the number of grid points.
// /numberOfDimensions\_: The number of space dimensions (1,2, or 3)
// /nx\_, ny\_, nz\_: The number of grid points (minus one) in each dimension (nx,ny,nz should
//   be a power of two or a product of small primes for efficiency). 
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  initialized=FALSE;
  xPeriod=yPeriod=zPeriod=twoPi;
  setDimensions(numberOfDimensions_,nx_,ny_,nz_);
}

FourierOperators::
~FourierOperators()
{
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{fourierLaplacian}} 
void FourierOperators::
fourierLaplacian(const RealArray & uHat, 
                 RealArray & uLaplacianHat, 
                 const int & power /* =1 */,
                 const Range & Components0 /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description: 
//  Apply the Laplacian operator (or powers of the Laplacian operator) in fourier space.
//  The  power can be positive or negative. 
//
// /uHat (input) : the fourier transform
// /uLaplacianHat (output) : uHat multiplied by "$[-(k_x^2+k_y^2+k_z^2)]^{\tt power}$". Note that
//    the mean (i.e. the constant mode) is set to zero in all cases.
// /power (input): The power of the operator to apply. 
// /Components0 (input) : optional components to operate on (default is all components)
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( !initialized )
    initialize();

  Range C0 = Components0==nullRange ? Range(uHat.getBase(3),uHat.getBase(3)) : Components0;

  if( power==1 )
  {
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      uLaplacianHat(R1,R2,R3,c0)=uHat(R1,R2,R3,c0)*kSquared(R1,R2,R3);
  }
  else if( power==-1 )  
  {
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      uLaplacianHat(R1,R2,R3,C0)=uHat(R1,R2,R3,C0)/kSquared(R1,R2,R3,C0);
  }
  else
  {
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      uLaplacianHat(R1,R2,R3,C0)=uHat(R1,R2,R3,C0)*pow(kSquared(R1,R2,R3,C0),power);
  }
  // set zero mode to zero
  uLaplacianHat(R1.getBase(),R2.getBase(),R3.getBase())=0.;
  
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{fourierDerivative}} 
void FourierOperators:: 
fourierDerivative(const RealArray & uHat, 
                  RealArray & uHatDerivative, 
		  const int & xDerivative  /* =1 */, 
		  const int & yDerivative  /* =0 */, 
		  const int & zDerivative  /* =0 */, 
		  const Range & Components0 /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description: 
//  Compute a derivative in Fourier space.
//  The order of the derivative can be be positive or negative. 
//
// /uHat (input) : the fourier transform
// /uHatDerivative (output) : The derivative in fourier space.
// /xDerivative (input): The order of the x-derivative
// /yDerivative (input): The order of the y-derivative
// /zDerivative (input): The order of the z-derivative
// /Components0 (input) : optional components to operate on (default is all components)
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( !initialized )
    initialize();

  if( yDerivative!=0 && numberOfDimensions<2 )
  {
    cout << "FourierOperators::fourierDerivative: ERROR: attempting a y-derivative but numberOfDimensions=" 
         << numberOfDimensions << endl;
    return;
  }
  if( zDerivative!=0 && numberOfDimensions<3 )
  {
    cout << "FourierOperators::fourierDerivative: ERROR: attempting a z-Derivative but numberOfDimensions=" 
         << numberOfDimensions << endl;
    return;
  }
  

  Range C0 = Components0==nullRange ? Range(uHat.getBase(3),uHat.getBase(3)) : Components0;


  if( xDerivative==1 )
  { // 1 x-derivative (do as special case for efficiency)
    Range R(R1.getBase()+1,R1.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
    {
      uHatDerivative(R1.getBase() ,R2,R3,c0)=0.;
      uHatDerivative(R1.getBound(),R2,R3,c0)=0.;  // last point not used
      uHatDerivative(R  ,R2,R3,c0)=uHat(R+1,R2,R3,c0)*kxDerivative(R  ,R2,R3);  // sin -> cos
      uHatDerivative(R+1,R2,R3,c0)=uHat(R  ,R2,R3,c0)*kxDerivative(R+1,R2,R3);  // cos -> sin
    }
  }
  else if( xDerivative!=0 )
  {
    if( xDerivative % 2 == 0 )
    { // even derivatives:
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1,R2,R3,c0)=uHat(R1,R2,R3,c0)*pow(kxDerivative(R1,R2,R3),xDerivative)*pow(-1.,xDerivative/2.);
	uHatDerivative(R1.getBase(),R2,R3,c0)=0.;
      }
    }
    else
    {
      Range R(R1.getBase()+1,R1.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1.getBase() ,R2,R3,c0)=0.;
        uHatDerivative(R1.getBound(),R2,R3,c0)=0.;
	uHatDerivative(R  ,R2,R3,c0)=uHat(R+1,R2,R3,c0)*pow(kxDerivative(R  ,R2,R3),xDerivative)*pow(-1.,xDerivative/2.);
	uHatDerivative(R+1,R2,R3,c0)=uHat(R  ,R2,R3,c0)*pow(kxDerivative(R+1,R2,R3),xDerivative)*pow(-1.,xDerivative/2.);
      }
    }
  }
  
  // operate next on v
  RealArray uH;
  if( xDerivative!=0 )
    uH=uHatDerivative;  // copy
  const RealArray & v = xDerivative!=0 ? uH : uHat;
  
  if( yDerivative==1 )
  { // y-derivative
    Range R(R2.getBase()+1,R2.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
    {
      uHatDerivative(R1,R2.getBase() ,R3,c0)=0.;
      uHatDerivative(R1,R2.getBound(),R3,c0)=0.;
      uHatDerivative(R1,R  ,R3,c0)=v(R1,R+1,R3,c0)*kyDerivative(R1,R  ,R3);
      uHatDerivative(R1,R+1,R3,c0)=v(R1,R  ,R3,c0)*kyDerivative(R1,R+1,R3);
    }
  }
  else if( yDerivative!=0 )
  {
    if( yDerivative % 2 == 0 )
    {
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1,R2,R3,c0)=v(R1,R2,R3,c0)*pow(kyDerivative(R1,R2,R3),yDerivative)*pow(-1.,yDerivative/2.);
        uHatDerivative(R1,R2.getBase() ,R3,c0)=0.;
      }
    }
    else
    {
      Range R(R2.getBase()+1,R2.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1,R2.getBase() ,R3,c0)=0.;
	uHatDerivative(R1,R2.getBound(),R3,c0)=0.;
	uHatDerivative(R1,R  ,R3,c0)=v(R1,R+1,R3,c0)*pow(kyDerivative(R1,R  ,R3),yDerivative)*pow(-1.,yDerivative/2.);
	uHatDerivative(R1,R+1,R3,c0)=v(R1,R  ,R3,c0)*pow(kyDerivative(R1,R+1,R3),yDerivative)*pow(-1.,yDerivative/2.);
      }
    }
  }

  if( yDerivative!=0 )
    uH=uHatDerivative;
  const RealArray & w = (xDerivative!=0 || yDerivative!=0) ? uH : uHat;

  if( zDerivative==1 )
  { // z-derivative
    Range R(R3.getBase()+1,R3.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
    for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
    {
      uHatDerivative(R1,R2,R3.getBase() ,c0)=0.;
      uHatDerivative(R1,R2,R3.getBound(),c0)=0.;
      uHatDerivative(R1,R2,R  ,c0)=w(R1,R2,R+1,c0)*kzDerivative(R1,R2,R  );
      uHatDerivative(R1,R2,R+1,c0)=w(R1,R2,R  ,c0)*kzDerivative(R1,R2,R+1);
    }
  }
  else if( zDerivative!=0 )
  {
    if( zDerivative % 2 == 0 )
    {
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1,R2,R3,c0)=w(R1,R2,R3,c0)*pow(kzDerivative(R1,R2,R3),zDerivative)*pow(-1.,zDerivative/2.);
	uHatDerivative(R1,R2,R3.getBase(),c0)=0.;
      }
    }
    else
    {
      Range R(R3.getBase()+1,R3.getBound()-2,2); // start from 1  ( 0 c1 s1 c1 s1 ... )
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
	uHatDerivative(R1,R2,R3.getBase() ,c0)=0.;
        uHatDerivative(R1,R2,R3.getBound(),c0)=0.;
	uHatDerivative(R1,R2,R  ,c0)=w(R1,R2,R+1,c0)*pow(kzDerivative(R1,R2,R  ),zDerivative)*pow(-1.,zDerivative/2.);
	uHatDerivative(R1,R2,R+1,c0)=w(R1,R2,R  ,c0)*pow(kzDerivative(R1,R2,R+1),zDerivative)*pow(-1.,zDerivative/2.);
      }
    }
  }
  

}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{fourierToReal}} 
void FourierOperators::
fourierToReal(const RealArray & uHat, 
              RealArray & u, 
              const Range & Components0 /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description: 
//   Perform a transform from fourier space to real space (backward transform)
// /uHat (input) : the fourier transform
// /u (output) : The array to be assigned the backward fourier transform.
// /Components0 (input) : optional components to operate on (default is all components)
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  transform(1,uHat,u,Components0);
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{realToFourier}} 
void FourierOperators::
realToFourier(const RealArray & u, 
              RealArray & uHat, 
              const Range & Components0 /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description: 
//     Real space to fourier space (forward transform)
// /u (input) : The array to fourier transform.
// /uHat (output) : the fourier transform
// /Components0 (input) : optional components to operate on (default is all components)
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  transform(0,u,uHat,Components0);
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{setDefaultRanges}} 
void  FourierOperators::
setDefaultRanges(const Range & R1_, 
		 const Range & R2_ /* =nullRange */, 
		 const Range & R3_ /* =nullRange */ )
//-----------------------------------------------------------------------------------------
// /Description:
// Change the Ranges over which the transforms are performed. This may also change
// the number of points. The operations will then be applied to u(R1\_,R2\_,R3\_,C)
// /R1\_,R2\_,R3\_ : new ranges 
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  R1=R1_;
  R2= R2_==nullRange ? Range(0,0) : R2_;
  R3= R3_==nullRange ? Range(0,0) : R3_;
  bool numberOfPointsHasChanged = R1.length()!=nx || R2.length()!=ny || R3.length()!=nz;
  nx=R1.length();
  ny=R2.length();
  nz=R3.length();

  if( nx<= 0 )
  {
    nx=1;
    R1=Range(R1.getBase(),R1.getBase());
  }
  if( ny<= 0 )
  {
    ny=1;
    R2=Range(R2.getBase(),R2.getBase());
  }
  if( nz<= 0 )
  {
    nz=1;
    R3=Range(R3.getBase(),R3.getBase());
  }
  
    
  if( numberOfPointsHasChanged )
    initialized=FALSE;
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{setDimensions}} 
void FourierOperators::
setDimensions(const int & numberOfDimensions_, 
	      const int & nx_, 
	      const int & ny_ /* =1 */, 
	      const int & nz_ /* =1 */)
//-----------------------------------------------------------------------------------------
// /Description: Define the number of space dimensions and the number of grid points.
// /numberOfDimensions\_: The number of space dimensions (1,2, or 3)
// /nx\_, ny\_, nz\_: The number of grid points (minus one) in each dimension (nx,ny,nz should
//   be a power of two or a product of small primes for efficiency). 
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( numberOfDimensions_<1 || numberOfDimensions_>3 )
  {
    cout << "FourierOperators::setDimensions: ERROR invalid value for numberOfDimensions_ ="
         << numberOfDimensions_ << endl;
    return;
  }
  if( nx_<=0 || ny_<=0 || nz_<=0 )
  {
    cout << "FourierOperators::setDimensions: ERROR, nx_,ny_, and nz_ must all be positive \n";
    cout << " nx_= " << nx_ << ", ny_ = " << ny_ << ", nz_ = " << nz_ << endl;
    return;
  }
  
  numberOfDimensions=numberOfDimensions_;
  nx=nx_;
  ny=ny_;
  nz=nz_;
  R1=Range(0,nx-1);
  R2=Range(0,ny-1);
  R3=Range(0,nz-1);
  initialized=FALSE;
}

//\begin{>>FourierOperatorsInclude.tex}{\subsection{setPeriod}} 
void FourierOperators::
setPeriod(const real & xPeriod_,
          const real & yPeriod_ /* = twoPi */,
          const real & zPeriod_ /* = twoPi */ )
//-----------------------------------------------------------------------------------------
// /Description: Set the period, default is 2*pi.
// /xPeriod\_, yPeriod\_, zPeriod\_ (input) : The length of the periodic interval in each direction
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
{
  if( xPeriod != xPeriod_ || yPeriod != yPeriod_ || zPeriod != zPeriod_ )
  {
    xPeriod=xPeriod_;
    yPeriod=yPeriod_;
    zPeriod=zPeriod_;
    initialized=FALSE;
  }
}


void FourierOperators::
initialize()
// private routine
{
  initialized=TRUE;

  assert( nx>0 && ny>0 && nz>0 );

  wsavex.redim(2*nx+15);
  wsavey.redim(2*ny+15);
  wsavez.redim(2*nz+15);
  // initialize fft routines
  RFFTI( nx,wsavex(0) );
  RFFTI( ny,wsavey(0) );
  RFFTI( nz,wsavez(0) );

  uTemp.redim(max(nx,max(ny,nz)));

  //    kSquared : multiplication by this matrix is applying the  Laplacian in Fourier space
  kSquared.redim(R1,R2,R3);
  int i1,i2,i3,k1,k2,k3;
  real k22,k32;
  for( i3=R3.getBase(), k3=1; i3<=R3.getBound(); i3++,k3++ )
  {
    k32=SQR(twoPi/zPeriod*(k3/2.));
    for( i2=R2.getBase(), k2=1; i2<=R2.getBound(); i2++,k2++ )
    {
      k22=SQR(twoPi/yPeriod*(k2/2));
      for( i1=R1.getBase(), k1=1; i1<=R1.getBound(); i1++,k1++ )
      {
	kSquared(i1,i2,i3)=-( SQR((twoPi/xPeriod)*(k1/2))+k22+k32 );
      }
    }
  }
  kSquared(R1.getBase(),R2.getBase(),R3.getBase())=1.;  // set zero mode to 1 to avoid division by zero

  //    kxDerivative : multiplication by this matrix is an x-derivative in Fourier space
  kxDerivative.redim(R1,R2,R3);
  for( i1=R1.getBase(), k1=1; i1<R1.getBound(); i1+=2,k1+=2 )
  {
    kxDerivative(i1,R2,R3)  =k1/2;
    kxDerivative(i1+1,R2,R3)=-(k1+1)/2;
  }
  kxDerivative*=twoPi/xPeriod;
  kxDerivative(R1.getBase(),R2,R3)=1.;  // set zero mode to 1 to avoid division by zero

  if( numberOfDimensions>1 )
  {
    //       y derivative
    kyDerivative.redim(R1,R2,R3);
    for( i2=R2.getBase(), k2=1; i2<R2.getBound(); i2+=2,k2+=2 )
    {
      kyDerivative(R1,i2  ,R3)  =k2/2;
      kyDerivative(R2,i2+1,R3)=-(k2+1)/2;
    }
    kyDerivative*=twoPi/yPeriod;
    kyDerivative(R1,R2.getBase(),R3)=1.;  // set zero mode to 1 to avoid division by zero
  }

  if( numberOfDimensions>2 )
  {
    // z -derivative
    kzDerivative.redim(R1,R2,R3);
    for( i3=R3.getBase(), k3=1; i3<R3.getBound(); i3+=2,k3+=2 )
    {
      kzDerivative(R1,R2,i3  )  =k3/2;
      kzDerivative(R2,R2,i3+1)=-(k3+1)/2;
    }
    kzDerivative*=twoPi/zPeriod;
    kzDerivative(R1,R2,R3.getBase())=1.;  // set zero mode to 1 to avoid division by zero
  }
  
}


//\begin{>>FourierOperatorsInclude.tex}{\subsection{transform}} 
void FourierOperators::
transform(const int & forwardOrBackward,
          const RealArray & u, 
	  RealArray & uHat, 
	  const Range & Components0 )
//-----------------------------------------------------------------------------------------
// /Description: 
//  Perform a forward or backward fourier transform. (This routine is called by
//   {\tt realToFourier} and {\tt fourierToReal}.
// /forwardOrBackward (input): 0=forward, 1=backward
// /u (input) : The array to fourier transform.
// /uHat (output) : the fourier transform
// /Components0 (input) : optional components to operate on (default is all components)
// /Author: WDH
//\end{FourierOperatorsInclude.tex} 
//-----------------------------------------------------------------------------------------
// /u (input) : before transform
// /uHat (output) : after transform
{
  if( !initialized )
    initialize();
  
  int ndx=u.getLength(0);
  int ndy=u.getLength(1);
  int ndz=u.getLength(2);
  int ndTemp=uTemp.elementCount();
  Range C0 = Components0==nullRange ? Range(u.getBase(3),u.getBase(3)) : Components0;

  // choose the appropriate FFT routine to call
  typedef void (*fftFunctionPointer)(int & , real & , real & );
  fftFunctionPointer forwardOrBackwardFFT;
  forwardOrBackwardFFT = forwardOrBackward==0 ? &RFFTF : &RFFTB;

//  FFT2DF( u(0,0),uHat(0,0) , ndx,nx,ny,wsavex(0),wsavey(0),uTemp(0));

  int i1,i2,i3;
  if( numberOfDimensions>2 )
  {
    int i3a=R3.getBase();
    uTemp.reshape(1,1,Range(i3a,i3a+ndTemp-1));
    for( i1=R1.getBase(); i1<=R1.getBound(); i1++ )
    {
      for( i2=R2.getBase(); i2<=R2.getBound(); i2++ )
      {
        for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
	{
          uTemp(0,0,R3)=u(i1,i2,R3,c0);
  	  forwardOrBackwardFFT( nz,uTemp(0,0,i3a),wsavez(0) );
          uHat(i1,i2,R3,c0)=uTemp(0,0,R3);
	}
      }
    }
  }
  if(  numberOfDimensions>1 )
  {
    const RealArray & u2 = numberOfDimensions==2 ? u : uHat;
    
    int i2a=R2.getBase();
    uTemp.reshape(1,Range(i2a,i2a+ndTemp-1),1);
    for( i3=R3.getBase(); i3<=R3.getBound(); i3++ )
    {
      for( i1=R1.getBase(); i1<=R1.getBound(); i1++ )
      {
        for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
	{
	  uTemp(0,R2,0)=u2(i1,R2,i3,c0);
	  forwardOrBackwardFFT( ny,uTemp(0,i2a,0),wsavey(0) );
	  uHat(i1,R2,i3,c0)=uTemp(0,R2,0);
	}
      }
    }
  }
  
  if( numberOfDimensions==1 && (&uHat != &u) )
    uHat=u;

  int i1a=R1.getBase();
  for( i3=R3.getBase(); i3<=R3.getBound(); i3++ )
  {
    for( i2=R2.getBase(); i2<=R2.getBound(); i2++ )
    {
      for( int c0=C0.getBase(); c0<=C0.getBound(); c0++ )  // loop over component 0
      {
        forwardOrBackwardFFT( nx,uHat(i1a,i2,i3,c0),wsavex(0) );
      }
    }
  }
  if( forwardOrBackward==1 )
  { // scale backward transform
    uHat*=(1./(nx*ny*nz));
  }

}

// --- define versions taking distributed arrays ---
#ifdef USE_PPP
// real space to fourier space (forward transform)
void FourierOperators:: 
realToFourier( const RealDistributedArray & u, RealDistributedArray & uHat, const Range & Components )
{
  OV_ABORT("FourierOperators::ERROR: not implemented in parallel");
}


// fourier space to real space (backward transform)
void FourierOperators:: 
fourierToReal(const RealDistributedArray & uHat, 
	      RealDistributedArray & u, 
	      const Range & Components )
{
  OV_ABORT("FourierOperators::ERROR: not implemented in parallel");
}

// compute a derivative (or integral) in Fourier space
void FourierOperators:: 
fourierDerivative(const RealDistributedArray & uHat, 
		  RealDistributedArray & uHatDerivative, 
		  const int & xDerivative, 
		  const int & yDerivative, 
		  const int & zDerivative, 
		  const Range & Components )
{
  OV_ABORT("FourierOperators::ERROR: not implemented in parallel");
}  
// compute 
//       (Laplacian)^power uHat
// where power can be positive or negative
void FourierOperators:: 
fourierLaplacian(const RealDistributedArray & uHat, 
		 RealDistributedArray & uHatLaplacian, 
		 const int & power,
		 const Range & Components )
{
  OV_ABORT("FourierOperators::ERROR: not implemented in parallel");
}  

void FourierOperators:: 
transform(const int & forwardOrBackward,
	  const RealDistributedArray & u, 
	  RealDistributedArray & uHat, 
	  const Range & Components )
{
  OV_ABORT("FourierOperators::ERROR: not implemented in parallel");
}
#endif 



/* -------
void FourierOperators:: 
complexMagnitude( RealArray & uhat, RealArray & uComplexMagnitude )
{
//===================================================================
// /Description:
//   Compute the Squares of the Magnitudes of the Complex Spectrum
// Input
//  what  - NCAR FFT spectrum ( ~cosine-sine transform )
//  w2    - work space
// OUTPUT
//  wh    -  squares of the magnitudes of the complex spectrum
//
// ===================================================================
c
      real what(nd,nd)
      real wh(-nd/2:nd/2,-nd/2:nd/2),w2(nd,nd)
c
c First renormalize the spectrum
c
      p=1./(nx*ny)
      do 100 j=1,ny
        do 100 i=1,nx
          w2(i,j)=what(i,j)*p
 100  continue
c
      do 300 j=2,ny-2,2
        ky=j/2
        do 200 i=2,nx-2,2
          kx=i/2
          wh( kx,ky)= (w2(i  ,j)-w2(i+1,j+1))**2+
     +                (w2(i+1,j)+w2(i  ,j+1))**2
          wh(-kx,-ky)=wh(kx,ky)
          wh(-kx,ky)= (w2(i  ,j)+w2(i+1,j+1))**2+
     +                (w2(i+1,j)-w2(i  ,j+1))**2
          wh(kx,-ky)=wh(-kx,ky)
 200    continue
 300  continue
c
      wh(0,0)=w2(1,1)**2
      i=1
      do 400 j=2,ny-2,2
        ky=j/2
        wh(0,ky)= w2(i,j)**2+w2(i,j+1)**2
        wh(0,-ky)=wh(0,ky)
 400  continue
c
      j=1
      do 500 i=2,nx-2,2
        kx=i/2
        wh(kx,0)= w2(i,j)**2+w2(i+1,j)**2
        wh(-kx,0)=wh(kx,0)
 500  continue
      return
      end
----- */
