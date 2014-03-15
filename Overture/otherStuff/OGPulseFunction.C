#include "OGPulseFunction.h"

//\begin{>OGPulseFunctionInclude.tex}{\subsubsection{Constructor}} 
OGPulseFunction::
OGPulseFunction(int numberOfDimensions_ /* = 2 */,
                int numberOfComponents_ /* =1 */,
                real a0_ /* =1. */ , 
                real a1_ /* =5. */ ,
                real c0_ /* =0. */ ,
                real c1_ /* =0. */ ,
                real c2_ /* =0. */ ,
                real v0_ /* =1. */ ,
                real v1_ /* =1. */ ,
                real v2_ /* =1. */ ,
                real p_  /* =1. */ )
//---------------------------------------------------------------------------------------
// /Description: 
//    
//    Define a pulse.
//
//  \begin{align*}
//    U &=  a_0 \exp( - a_1 | \xv-\bv(t) |^{2p} )  \\
//    \bv(t) &= \cv_0 + \vv t
//  \end{align*}
//
// /numberOfDimensions\_ (input): number of space dimensions, 1,2, or 3.
// /numberOfComponents\_ (input): maximum number of components required.
// /a0\_,a1\_,...p\_ (input): pulse parameters
// 
//
//\end{OGPulseFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  setParameters(numberOfDimensions_,numberOfComponents_,a0_,a1_,c0_,c1_,c2_,v0_,v1_,v2_,p_ );

}

OGPulseFunction::
OGPulseFunction(const OGPulseFunction & ogp )   // copy constructor is a deep copy
{
  *this=ogp;
}

//--------------------------------------------------------------------------------------
//  operator = is  a deep copy
//--------------------------------------------------------------------------------------
OGPulseFunction & OGPulseFunction::
operator=(const OGPulseFunction & ogp )
{
  numberOfComponents=ogp.numberOfComponents;
  numberOfDimensions=ogp.numberOfDimensions;
  
  assert(numberOfComponents>=0 && numberOfDimensions>=1 && numberOfDimensions<=3 );

  a0=ogp.a0;
  a1=ogp.a1;
  c0=ogp.c0;
  c1=ogp.c1;
  c2=ogp.c2;
  v0=ogp.v0;
  v1=ogp.v1;
  v2=ogp.v2;
  p =ogp.p;
  
  return *this;
}


//\begin{>>OGPulseFunctionInclude.tex}{\subsubsection{setRadius}} 
int OGPulseFunction::
setRadius( real radius )
// =========================================================================================
// /Description:
//  Set the approximte radius of the pulse. This will set the parameter $a_1$
//   according to the formula $\mbox{radius} = 1/\sqrt{a_1}$.
//  
// /radius (input): approximate radius.
// 
// /Author: WDH
//\end{OGPulseFunctionInclude.tex} 
// ==========================================================================================
{
  a1=1./SQR(radius);
  return 0;
}

//\begin{>>OGPulseFunctionInclude.tex}{\subsubsection{setRadius}} 
int OGPulseFunction::
setShape( real p_ )
// =========================================================================================
// /Description:
//  Set the {\it shape} parameter p. $p=1$ gives a Gaussian pulse, choosing a larger value of $p$
//  will cause the pulse to flatten on the top and approach a top-hat function as $p$ tends to infinity.
//  
// /p\_ (input): shape parameter, $p>{1\over2}$.
// 
// /Author: WDH
//\end{OGPulseFunctionInclude.tex} 
// ==========================================================================================
{
  p=p_;
  return 0;
}

//\begin{>>OGPulseFunctionInclude.tex}{\subsubsection{setCentre}} 
int OGPulseFunction::
setCentre( real c0_  /* =0. */,
	   real c1_  /* =0. */,
	   real c2_  /* =0. */ )
// =========================================================================================
// /Description:
//  Set the pulse centre.
//  
// /c0\_,c1\_,c2\_ (input): centre.
// 
// /Author: WDH
//\end{OGPulseFunctionInclude.tex} 
// ==========================================================================================
{
  c0=c0_;
  c1=c1_;
  c2=c2_;
  return 0;
}

//\begin{>>OGPulseFunctionInclude.tex}{\subsubsection{setVelocity}} 
int OGPulseFunction::
setVelocity( real v0_  /* =1. */,
	     real v1_  /* =1. */,
	     real v2_  /* =1. */ )
// =========================================================================================
// /Description:
//  Set the pulse velocity.
//  
// /v0\_,v1\_,v2\_ (input): velocity.
// 
// /Author: WDH
//\end{OGPulseFunctionInclude.tex} 
// ==========================================================================================
{
  v0=v0_;
  v1=v1_;
  v2=v2_;
  return 0;
}



//\begin{>>OGPulseFunctionInclude.tex}{\subsubsection{setCoefficients}} 
void OGPulseFunction::
setParameters( int numberOfDimensions_ /* = 2 */,
                int numberOfComponents_ /* =1 */,
                real a0_ /* =1. */ , 
                real a1_ /* =5. */ ,
                real c0_ /* =0. */ ,
                real c1_ /* =0. */ ,
                real c2_ /* =0. */ ,
                real v0_ /* =1. */ ,
                real v1_ /* =1. */ ,
                real v2_ /* =1. */ ,
                real p_  /* =1. */ )
// =========================================================================================
// /Description: Use this member function to set parameters.
//
//    Define a pulse.
//  \begin{align*}
//    U &=  a_0 \exp( - a_1 | \xv-\cv(t) |^p )  \qquad p>{1\over2}\\
//    \cv(t) &= \cv_0 + \vv t
//  \end{align*}
//
// /numberOfDimensions\_ (input): number of space dimensions, 1,2, or 3.
// /numberOfComponents\_ (input): maximum number of components required.
// /a0\_,a1\_,...p\_ (input): pulse parameters. 
// 
// /Author: WDH
//\end{OGPulseFunctionInclude.tex} 
// ==========================================================================================
{
  numberOfComponents=numberOfComponents_;
  numberOfDimensions=numberOfDimensions_;
  
  assert(numberOfComponents>=0 && numberOfDimensions>=1 && numberOfDimensions<=3 );
  assert( p_>0. );

  a0=a0_;
  a1=a1_;
  c0=c0_;
  c1=c1_;
  c2=c2_;
  v0=v0_;
  v1=v1_;
  v2=v2_;
  p =p_;

}


// This routine is used locally to check some arguments
void OGPulseFunction::
checkArguments(const Index & N)
{
  if( N.getBound() >= numberOfComponents )
  {
    cout << "OGPulseFunction:ERROR: trying to evaluate a function with " << N.getBound()+1 
         << " components, \n  but the polynomial only supports numberOfComponents = " 
	   << numberOfComponents << endl;
    cout << "Why not create the OGPulseFunction object with more components :-) \n";
    Overture::abort( "OGPulseFunction:ERROR");
  }
}

// ================================ new names =======================================

// Here are the non-inlined versions : Use these when OGFunction
// is passed as an argument


//\begin{>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative at a point}} 
real OGPulseFunction::
operator()(const real x, 
	   const real y, 
	   const real z, 
	   const int n /* =0 */ , 
	   const real t /* =0. */ )
//===================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return u(x,y,z,n,t);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPulseFunction::
operator()(const real x, const real y, const real z, const int n)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return u(x,y,z,n,0.);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPulseFunction::
operator()(const real x, const real y, const real z)
//===================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return u(x,y,z,0,0.);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPulseFunction::
x(const real x, 
  const real y, 
  const real z, 
  const int n /* =0 */, 
  const real t /* =0. */ )
//===================================================================================
// /Description: Evaluate the function or a derivative of the function at a point
//    The function name {\bf x} can be replaced by any one of {\bf t}, {\bf x}, {\bf y}, {\bf z}, {\bf xx},
//     {\bf yy}, {\bf zz}, {\bf xy}, {\bf xz}, {\bf yz}, {\bf xxx} or {\bf xxxx}.
// /x,y,z (input) : coordinates
// /n (input) : component number (starting from 0)
// /t (input) : time
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return ux(x,y,z,n,t);
}

real OGPulseFunction::
t(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return ut(x,y,z,n,t);
}
real OGPulseFunction::
y(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return uy(x,y,z,n,t);
}

real OGPulseFunction::
xx(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return uxx(x,y,z,n,t);
}

real OGPulseFunction::
xy(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return uxy(x,y,z,n,t);
}
real OGPulseFunction::
yy(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return uyy(x,y,z,n,t);
}
real OGPulseFunction::
z(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
  return uz(x,y,z,n,t);
}
real OGPulseFunction::
xz(const real x, const real y, const real z, const int n, const real t )
// ==========================================================================================
{
  return uxz(x,y,z,n,t);
}
real OGPulseFunction::
yz(const real x, const real y, const real z, const int n, const real t )
{
  return uyz(x,y,z,n,t);
}
real OGPulseFunction::
zz(const real x, const real y, const real z, const int n, const real t )
{
  return uzz(x,y,z,n,t);
}
real OGPulseFunction::
xxx(const real x, const real y, const real z, const int n, const real t )
{
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  real r;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
      
  real rp=pow(r,p);
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  real g = 2.*a1*p*rp/r*(x-b0);
  real gx = 2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0));
  real gxx = a1*4.*p*(p-1.)*rp/(r*r)*(x-b0)*( 2.*(p-2.)*SQR(x-b0)/r+3. );

  return a0*exp(-a1*rp)*( -g*g*g +3.*g*gx -gxx );
}
real OGPulseFunction::
xxxx(const real x, const real y, const real z, const int n, const real t )
{
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  real r;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
      
  real rp=pow(r,p);
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5

  real g = 2.*a1*p*rp/r*(x-b0);
  real gx = 2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0));
  real gxx = a1*4.*p*(p-1.)*rp/(r*r)*(x-b0)*( 2.*(p-2.)*SQR(x-b0)/r+3. );
  real gxxx = a1*4.*p*(p-1.)*rp/(r*r)*( 4.*(p-2.)*SQR(x-b0)/r*( (p-3.)*SQR(x-b0)/r +3.) +3. );

  return a0*exp(-a1*rp)*( g*g*g*g +3.*gx*gx -6.*g*g*gx +4.*g*gxx -gxxx );

}


//\begin{>>OGFunctionInclude.tex}{}
real OGPulseFunction::
gd(const int & ntd, 
   const int & nxd, 
   const int & nyd, 
   const int & nzd,
   const real x0, 
   const real y0, 
   const real z0, 
   const int n /* =0 */, 
   const real t0 /* =0. */ )
//===================================================================================
// /Description: Evaluate a general derivative. The arguments are the same as in the
// corresponding {\tt x} function except that the first 4 arguments specify the derivative
// to compute.
// /ntd,nxd,nyd,nzd (input): Specify the derivative to compute by indicating the order
//   of each partial derivative. 
//    \begin{description}
//       \item[ntd] : number of time derivatives (order of the time derivative).
//       \item[nxd] : number of x derivatives (order of the x derivative).
//       \item[nyd] : number of y derivatives (order of the y derivative).
//       \item[nzd] : number of z derivatives (order of the z derivative).
//    \end{description}
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
    return operator()(x0,y0,z0,n,t0);
  else if( ntd==1 && nxd==0 && nyd==0 && nzd==0 )
   return t(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==0 )
   return x(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==0 )
   return y(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==1 )
   return z(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==2 && nyd==0 && nzd==0 )
   return xx(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==1 && nyd==1 && nzd==0 )
   return xy(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==1 )
   return xz(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==0 && nyd==2 && nzd==0 )
   return yy(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==1 )
   return yz(x0,y0,z0,n,t0);
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==2 )
   return zz(x0,y0,z0,n,t0);
  else if( ntd==2 && nxd==0 && nyd==0 && nzd==0 )
  {
    // *wdh* add u.tt 090828
    // U = a0*exp( -a1*r^p )
    // r = ( x-b0(t))^2 + (y-b1(t))^2 + (z-b2(t))^2
    
    // r.t = 2*( x-b0(t) )*v0 + 2*( y-b1(t))*v1 + 2*( z-b2(t))*v2

    // U.t = a0*exp( -a1*f^p )*( -a1*p*r^{p-1} * f.t )
    //     = a0*exp( -a1*f^p )*( g )
    //   g = -2.*a1*p*r^{p-1}*[ ( x-b0(t) )*v0 + ( y-b1(t))*v1 + ( z-b2(t))*v2 ]

    // U.tt = a0*exp( -a1*f^p )*( g*g + g.t )
    //  g.t = -2.*a1*p*(  2.*(p-1)*r^{p-2}*[ ( x-b0(t) )*v0 + ( y-b1(t))*v1 + ( z-b2(t))*v2 ]^2
    //                    r^{p-1}*[ v0^2 + v1^2 + v2^2 ]

    real b0=c0+v0*t0;
    real b1=c1+v1*t0;
    real b2=c2+v2*t0;
    real r,rp,rpm1,g,h,utt=0.;

    if( numberOfDimensions==2 )
    {
      r= SQR(x0-b0)+SQR(y0-b1);
      rp=pow(r,p);  
      r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
      rpm1=rp/r;
      h = (x0-b0)*v0 + (y0-b1)*v1;
      g  = -2.*a1*p*rpm1*h;
      utt= a0*exp(-a1*rp)*( g*g -2.*a1*p*rpm1*( v0*v0+v1*v1 + 2.*(p-1.)/r*( h*h ) ) );

      if( utt != utt )
      {
        printF("ERROR: OGPulse: utt=%e for (x,y,z,t)=(%8.2e,%8.2e,%8.2e,%8.2e)\n",x0,y0,z0,t0);
        OV_ABORT("error");
      }
      
    }
    else if( numberOfDimensions==3 )
    {
      r= SQR(x0-b0)+SQR(y0-b1)+SQR(z0-b2);
      rp=pow(r,p);  
      r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
      rpm1=rp/r;
      h = (x0-b0)*v0 + (y0-b1)*v1 + (z0-b2)*v2;
      g  = -2.*a1*p*rpm1*h;
      utt= a0*exp(-a1*rp)*( g*g -2.*a1*p*rpm1*( v0*v0+v1*v1+v2*v2 + 2.*(p-1.)/r*( h*h ) ) );
    }
    else
    {
      r= SQR(x0-b0);
      rp=pow(r,p);  
      r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
      rpm1=rp/r;
      h = (x0-b0)*v0;
      g  = -2.*a1*p*rpm1*h;
      utt= a0*exp(-a1*rp)*( g*g -2.*a1*p*rpm1*( v0*v0 + 2.*(p-1.)/r*( h*h ) ) );
    }
    
    return utt;
      
  }
  else
    printf("OGPulseFunction::ERROR: general derivative not implemented for ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);
  return 0;
}

//\begin{>>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative on a MappedGrid}} 
RealDistributedArray OGPulseFunction::
operator()(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return operator()(c,I1,I2,I3,N,0.);
}

//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPulseFunction::
operator()(const MappedGrid & c,
	   const Index & I1,
	   const Index & I2,
	   const Index & I3, 
	   const Index & N, 
	   const real t /* =0. */ ,
	   const GridFunctionParameters::GridFunctionType & centering 
           /* =defaultCentering */ )
//===================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
                   
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;

  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp( -a1*rp );
  return result;
  
}

//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPulseFunction::
x(const MappedGrid & c, 
  const Index & I1,
  const Index & I2,const
  Index & I3, 
  const Index & N, 
  const real t,
  const GridFunctionParameters::GridFunctionType & centering 
  /* = defaultCentering */)
// ==========================================================================================
// /Description: Evaluate the function or a derivative of the function at points on a MappedGrid
//    The function name {\bf x} can be replaced by any one of {\bf t}, {\bf x}, {\bf y}, {\bf z}, {\bf xx},
//     {\bf yy}, {\bf zz}, {\bf xy}, {\bf xz}, {\bf yz}, {\bf laplacian}, {\bf xxx} or {\bf xxxx}.
// /I1,I2,I3 (input) : Ranges that indicate points to use, for example by default the
//     points 
//    \[ {\tt c.center()(I1,I2,I3,0:numberOfDimensions-1)} \]
//     are used.
// /N (input) : component indicies to assign
// /t (input) : time
// /centering (input): This enum is found in {\tt GridFunctionParameters}.
//       It indicates the positions of the coordinates, one of 
//   \begin{description} 
//    \item[defaultCentering] use the {\tt c.center()} array (vertices for a vertex centered grid
//       and cell centers for a cell-centered grid).
//    \item[vertexCentered] grid vertices, {\tt c.vertex()}.
//    \item[cellCentered] {\tt c.center()} for a cell-centered grid or else {\tt c.corner()} for
//        a vertex centered grid (the cell centers).
//    \item[faceCenteredAxis1] use the center of the cell face in the axis1 direction (defined
//       by averaging the {\tt c.vertex() values} for the y,z coordinates).
//    \item[faceCenteredAxis2] use the center of the cell face in the axis2 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,z coordinates).
//    \item[faceCenteredAxis3] use the center of the cell face in the axis3 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,y coordinates).
//  \end{description}
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  

  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (x-b0)) );

  return result;
  
}

RealDistributedArray OGPulseFunction::
y(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));

  if( numberOfDimensions==1 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (y-b1)) );

  return result;
}

RealDistributedArray OGPulseFunction::
z(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=2 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (z-b2)) );

  return result;
}

RealDistributedArray OGPulseFunction::
t(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  for( int n=N.getBase(); n<=N.getBound(); n++ )
  {
    if( numberOfDimensions==2 )     
      result(I1,I2,I3,n)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0+(y-b1)*v1 ) );
    else if( numberOfDimensions==3 )
      result(I1,I2,I3,n)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0+(y-b1)*v1+(z-b2)*v2 ) );
    else
      result(I1,I2,I3,n)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0 ) );
  }
  return result;
}

RealDistributedArray OGPulseFunction::
xx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  RealDistributedArray g; g = -2.*a1*p*rp/r*(x-b0);

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0)));

  return result;
}

RealDistributedArray OGPulseFunction::
yy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=1 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  RealDistributedArray g; g = -2.*a1*p*rp/r*(y-b1);

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(y-b1)));

  return result;
}

RealDistributedArray OGPulseFunction::
zz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=2 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  RealDistributedArray g; g = -2.*a1*p*rp/r*(z-b2);

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(z-b2)));

  return result;
}

RealDistributedArray OGPulseFunction::
xy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=1 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  RealDistributedArray gx; gx = -2.*a1*p*rp/r*(x-b0); 
  RealDistributedArray gy; gy = -2.*a1*p*rp/r*(y-b1);

  // N.B. write this as rp/r/r not rp/(r*r) to avoid nan's
  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(gx*gy-4.*a1*p*(p-1.)*((rp/r)/r)*(x-b0)*(y-b1));

  return result;
}

RealDistributedArray OGPulseFunction::
xz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=2 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  
  r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
  RealDistributedArray gx; gx = -2.*a1*p*rp/r*(x-b0); 
  RealDistributedArray gz; gz = -2.*a1*p*rp/r*(z-b2);

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(gx*gz-4.*a1*p*(p-1.)*rp/(r*r)*(x-b0)*(z-b2));

  return result;
}

RealDistributedArray OGPulseFunction::
yz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  checkArguments( N );
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( numberOfDimensions<=2 )
  {
    result=0.;
    return result;
  }
  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  RealDistributedArray r,rp;
  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  


  RealDistributedArray gy; gy = -2.*a1*p*rp/r*(y-b1); 
  RealDistributedArray gz; gz = -2.*a1*p*rp/r*(z-b2);

  for( int n=N.getBase(); n<=N.getBound(); n++ )
    result(I1,I2,I3,n)= a0*exp(-a1*rp)*(gy*gz-4.*a1*p*(p-1.)*rp/(r*r)*(y-b1)*(z-b2));

  return result;
}



RealDistributedArray OGPulseFunction::
laplacian(const MappedGrid & c, const Index & I1,
	  const Index & I2,const Index & I3, const Index & N, const real t,
	  const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  if( numberOfDimensions==2 )
    return evaluate(xx(c,I1,I2,I3,N,t,centering)+yy(c,I1,I2,I3,N,t,centering));
  else if( numberOfDimensions==3 )
    return evaluate(xx(c,I1,I2,I3,N,t,centering)+yy(c,I1,I2,I3,N,t,centering)+zz(c,I1,I2,I3,N,t,centering));
  else
    return xx(c,I1,I2,I3,N,t,centering);
}



RealDistributedArray OGPulseFunction::
xxx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  return x(c,I1,I2,I3,N,t,centering);
}

RealDistributedArray OGPulseFunction::
xxxx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  return x(c,I1,I2,I3,N,t,centering);
}

//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPulseFunction::
gd(const int & ntd, 
   const int & nxd, 
   const int & nyd, 
   const int & nzd,
   const MappedGrid & c, 
   const Index & I1,
   const Index & I2,
   const Index & I3, 
   const Index & N, 
   const real t0 /* =0. */,
   const GridFunctionParameters::GridFunctionType & centering /* = defaultCentering */ )
//===================================================================================
// /Description: Evaluate a general derivative. The arguments are the same as in the
// corresponding {\tt x} function except that the first 4 arguments specify the derivative
// to compute.
// /ntd,nxd,nyd,nzd (input): Specify the derivative to compute by indicating the order
//   of each partial derivative. 
//    \begin{description}
//       \item[ntd] : number of time derivatives (order of the time derivative).
//       \item[nxd] : number of x derivatives (order of the x derivative).
//       \item[nyd] : number of y derivatives (order of the y derivative).
//       \item[nzd] : number of z derivatives (order of the z derivative).
//    \end{description}
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  checkArguments( N );

  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
    return operator()(c,I1,I2,I3,N,t0,centering);
  else if( ntd==1 && nxd==0 && nyd==0 && nzd==0 )
   return t(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==0 )
   return x(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==0 )
   return y(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==1 )
   return z(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==2 && nyd==0 && nzd==0 )
   return xx(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==1 && nyd==1 && nzd==0 )
   return xy(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==1 )
   return xz(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==0 && nyd==2 && nzd==0 )
   return yy(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==1 )
   return yz(c,I1,I2,I3,N,t0,centering);
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==2 )
   return zz(c,I1,I2,I3,N,t0,centering);
  else
    printf("OGPulseFunction::ERROR: general derivative not implemented for ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);

  return x(c,I1,I2,I3,N,t0,centering);

}

// obtain a general derivative using serial arrays
realSerialArray& OGPulseFunction::
gd( realSerialArray & result,   // put result here
    const realSerialArray & xy,  // coordinates to use if isRectangular==true
    const int numberOfDimensions,
    const bool isRectangular,
    const int & ntd, const int & nxd, const int & nyd, const int & nzd,
    const Index & J1, const Index & J2, 
    const Index & J3, const Index & N, 
    const real t /* =0. */, int option /* =0 */ )
{

  int i1a=max(J1.getBase(),result.getBase(0)), i1b=min(J1.getBound(),result.getBound(0));
  int i2a=max(J2.getBase(),result.getBase(1)), i2b=min(J2.getBound(),result.getBound(1));
  int i3a=max(J3.getBase(),result.getBase(2)), i3b=min(J3.getBound(),result.getBound(2));

  if( i1a>i1b || i2a>i2b || i3a>i3b )
    return result;  // nothing to do
		    
  Index I1=Range(i1a,i1b);
  Index I2=Range(i2a,i2b);
  Index I3=Range(i3a,i3b);

  const int nd2 = numberOfDimensions>=2 ? 1 : 0;
  const int nd3 = numberOfDimensions==3 ? 2 : 0;
  
  const realSerialArray & x = xy(I1,I2,I3,0);
  const realSerialArray & y = xy(I1,I2,I3,nd2);
  const realSerialArray & z = xy(I1,I2,I3,nd3);

  real b0=c0+v0*t;
  real b1=c1+v1*t;
  real b2=c2+v2*t;
  
  realSerialArray r,rp;

  // if the result has only room for 1 variable and only one component is requested then put the
  // answer in result.
  int n0=0;
  if( result.getLength(3)==1 && N.length()==1 )
  {
    n0=N.getBase()-result.getBase(3);
  }

  if( numberOfDimensions==2 )     
    r= SQR(x-b0)+SQR(y-b1);
  else if( numberOfDimensions==3 )
    r= SQR(x-b0)+SQR(y-b1)+SQR(z-b2);
  else
    r=SQR(x-b0);
  rp=pow(r,p);  

  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
  {
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp( -a1*rp );
  }
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (x-b0)) );
  }
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (y-b1)) );
  }
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==1 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( -2.*a1*p*rp/r*( (z-b2)) );
  }
  else if( ntd==1 && nxd==0 && nyd==0 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    for( int n=N.getBase(); n<=N.getBound(); n++ )
    {
      if( numberOfDimensions==2 )     
	result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0+(y-b1)*v1 ) );
      else if( numberOfDimensions==3 )
	result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0+(y-b1)*v1+(z-b2)*v2 ) );
      else
	result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*( 2.*a1*p*rp/r*( (x-b0)*v0 ) );
    }
  }
  else if( ntd==0 && nxd==2 && nyd==0 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray g; g = -2.*a1*p*rp/r*(x-b0);
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0)));
  }
  else if( ntd==0 && nxd==0 && nyd==2 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray g; g = -2.*a1*p*rp/r*(y-b1);
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(y-b1)));  
  }
  else if( ntd==0 && nxd==0 && nyd==0 && nzd==2 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray g; g = -2.*a1*p*rp/r*(z-b2);
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(g*g-2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(z-b2)));
  }
  else if( ntd==0 && nxd==1 && nyd==1 && nzd==0 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray gx; gx = -2.*a1*p*rp/r*(x-b0); 
    realSerialArray gy; gy = -2.*a1*p*rp/r*(y-b1);

    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(gx*gy-4.*a1*p*(p-1.)*rp/(r*r)*(x-b0)*(y-b1));
  }
  else if( ntd==0 && nxd==1 && nyd==0 && nzd==1 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray gx; gx = -2.*a1*p*rp/r*(x-b0); 
    realSerialArray gz; gz = -2.*a1*p*rp/r*(z-b2);

    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(gx*gz-4.*a1*p*(p-1.)*rp/(r*r)*(x-b0)*(z-b2));
  }
  else if( ntd==0 && nxd==0 && nyd==1 && nzd==1 )
  {
    r=max(r,REAL_MIN*100.);  // avoid division by zero, note that rp/r*(x-b0) == 0 if r==0, p>.5
    realSerialArray gy; gy = -2.*a1*p*rp/r*(y-b1); 
    realSerialArray gz; gz = -2.*a1*p*rp/r*(z-b2);

    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n-n0)= a0*exp(-a1*rp)*(gy*gz-4.*a1*p*(p-1.)*rp/(r*r)*(y-b1)*(z-b2));
  }
//    else if( ntd==0 && nxd==2 && nyd==2 && nzd==0 )
//    {
//      // 2D laplacian

//      realSerialArray gx; gx = -2.*a1*p*rp/r*(x-b0);
//      realSerialArray gy; gy = -2.*a1*p*rp/r*(y-b1);

//      for( int n=N.getBase(); n<=N.getBound(); n++ )
//      {
//        result(I1,I2,I3,n)= a0*exp(-a1*rp)*( gx*gx -2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0)) +
//  					   gy*gy -2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(y-b1)) );
//      }
    
//    }
//    else if( ntd==0 && nxd==2 && nyd==2 && nzd==2 )
//    {
//      // 3D laplacian

//      realSerialArray gx; gx = -2.*a1*p*rp/r*(x-b0);
//      realSerialArray gy; gy = -2.*a1*p*rp/r*(y-b1);
//      realSerialArray gz; gz = -2.*a1*p*rp/r*(z-b2);

//      for( int n=N.getBase(); n<=N.getBound(); n++ )
//      {
//        result(I1,I2,I3,n)= a0*exp(-a1*rp)*( gx*gx -2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(x-b0)) +
//  					   gy*gy -2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(y-b1)) +  
//  					   gz*gz -2.*a1*p*rp/r*(1.+2.*(p-1.)/r*SQR(z-b2)) );
//      }
    
//    }
  else
  {
    printf("OGPulseFunction::ERROR: general derivative not implemented for ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);
  }
  
  return result;
}

//\begin{>>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative on a CompositeGrid}} 
realCompositeGridFunction OGPulseFunction::
operator()(CompositeGrid & cg, 
	   const Index & N, 
	   const real t,
           const GridFunctionParameters::
           GridFunctionType & centering  /* = defaultCentering */ )
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return OGFunction::operator()(cg,N,t,centering);
} 
//\begin{>>OGFunctionInclude.tex}{}
realCompositeGridFunction OGPulseFunction::
operator()(CompositeGrid & cg, const Index & N)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return OGFunction::operator()(cg,N,0.);
} 
//\begin{>>OGFunctionInclude.tex}{}
realCompositeGridFunction OGPulseFunction::
operator()(CompositeGrid & cg)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return OGFunction::operator()(cg,0,0.);
} 
//\begin{>>OGFunctionInclude.tex}{}
realCompositeGridFunction OGPulseFunction::
x(CompositeGrid & cg, 
  const Index & N, 
  const real t,
  const GridFunctionParameters::GridFunctionType & centering
      /* = defaultCentering */)
// ==========================================================================================
// /Description: Evaluate the function or a derivative of the function at points on a CompositeGrid
//    The function name {\bf x} can be replaced by any one of {\bf t}, {\bf x}, {\bf y}, {\bf z}, {\bf xx},
//     {\bf yy}, {\bf zz}, {\bf xy}, {\bf xz}, {\bf yz}, {\bf laplacian}, {\bf xxx} or {\bf xxxx}.
// /cg (input) : use this grid.
//     By default the points \[ {\tt c.center()(i1,I2,I3,0:numberOfDimensions-1)}\] are used.
// /N (input) : evaluate these components
// /t (input) : time
// /centering (input): This enum is found in {\tt GridFunctionParameters}.
//       It indicates the positions of the coordinates, one of 
//   \begin{description} 
//    \item[defaultCentering] use the {\tt c.center()} array (vertices for a vertex centered grid
//       and cell centers for a cell-centered grid).
//    \item[vertexCentered] grid vertices, {\tt c.vertex()}.
//    \item[cellCentered] {\tt c.center()} for a cell-centered grid or else {\tt c.corner()} for
//        a vertex centered grid (the cell centers).
//    \item[faceCenteredAxis1] use the center of the cell face in the axis1 direction (defined
//       by averaging the {\tt c.vertex() values} for the y,z coordinates).
//    \item[faceCenteredAxis2] use the center of the cell face in the axis1 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,z coordinates).
//    \item[faceCenteredAxis3] use the center of the cell face in the axis1 direction (defined
//       by averaging the {\tt c.vertex() values} for the x,y coordinates).
//  \end{description}
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
  return OGFunction::x(cg,N,t,centering);
} 

realCompositeGridFunction OGPulseFunction::
t(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::t(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
y(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::y(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
z(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::z(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
xx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xx(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
xy(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xy(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
xz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xz(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
yy(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::yy(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
yz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::yz(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
zz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::zz(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
laplacian(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::laplacian(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
xxx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xxx(cg,N,t,centering);
} 
realCompositeGridFunction OGPulseFunction::
xxxx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xxxx(cg,N,t,centering);
} 

realCompositeGridFunction OGPulseFunction::
gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
   CompositeGrid & cg, const Index & N, const real t,
   const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::gd(ntd,nxd,nyd,nzd,cg,N,t,centering);
} 



