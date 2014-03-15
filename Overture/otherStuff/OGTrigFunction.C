#include "OGTrigFunction.h"

#if 0
namespace { 
  inline double pow(double x, const int p) { return ::pow(x,double(p)); }
  inline double pow(double &x, const int &p) { return ::pow(x,double(p)); }
}
#endif

//\begin{>OGTrigFunctionInclude.tex}{\subsubsection{Constructors}} 
OGTrigFunction::
OGTrigFunction(const real & fx_ /* =1. */, 
	       const real & fy_ /* =1. */, 
	       const real & fz_ /* =0. */, 
	       const real & ft_ /* =0. */, 
	       const int & maximumNumberOfComponents /* =10 */)
//---------------------------------------------------------------------------------------
// /Description: 
// 
// This class is derived from the {\ff OGFunction} class and defines a function
// and defines a function that is a trigonometric polynomial:
// \[
//      u_n(x,y,z,t) = a(n) \cos(f_x(n) \pi (x-g_x(n)))
//                          \cos(f_y(n) \pi (y-g_y(n)))
//                          \cos(f_z(n) \pi (z-g_z(n)))
//                          \cos(f_t(n) \pi (t-g_t(n)))  + c(n)
// \]
//  where $a(n)$, $f_x(n)$, $f_y(n)$ etc. can be given values for each component n.
//
// /fx\_, fy\_, fz\_, ft\_ (input): give frequencies (constant for all components).
// /maximumNumberOfComponents (input): maximum number of components.
//
// /Notes:
//    By default $a(n)=1$ and $g_x(n)=g_y(n)=g_z(n)=g_t(n)=0$.
// /Author: WDH
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{ 
  fx.redim(maximumNumberOfComponents); 
  fy.redim(maximumNumberOfComponents);
  fz.redim(maximumNumberOfComponents);
  ft.redim(maximumNumberOfComponents);
  fx=fx_*Pi;       // for efficiency, store this way
  fy=fy_*Pi; 
  fz=fz_*Pi; 
  ft=ft_*Pi;
  gx.redim(fx); 
  gy.redim(fx);
  gz.redim(fx);
  gt.redim(fx);
  gx=0.;
  gy=0.;
  gz=0.;
  gt=0.;
  a.redim(fx);
  a=1.;
  cc.redim(fx);
  cc=0.;
}

//\begin{>>OGTrigFunctionInclude.tex}{}
OGTrigFunction::
OGTrigFunction(const RealArray & fx_, 
	       const RealArray & fy_, 
	       const RealArray & fz_, 
	       const RealArray & ft_)
//---------------------------------------------------------------------------------------
// /Description: 
// 
// Use this constructor to supply different frequencies for different components.
//
// /fx\_, fy\_, fz\_, ft\_ (input): give frequencies for different components. The dimension of fx\_
//        will determine the maximumNumberOfComponents.
//
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  setFrequencies(fx_,fy_,fz_,ft_);
  gx.redim(fx); 
  gy.redim(fx);
  gz.redim(fx);
  gt.redim(fx);
  gx=0.;
  gy=0.;
  gz=0.;
  gt=0.;
  a.redim(fx);
  a=1.;
  cc.redim(fx);
  cc=0.;
}

	       
//\begin{>>OGTrigFunctionInclude.tex}{\subsubsection{setAmplitudes}}
int OGTrigFunction::
setAmplitudes(const RealArray & a_ )
//---------------------------------------------------------------------------------------
// /Description: 
// 
// Use this function to supply different amplitudes for different components.
//
// /a\_ (input): give amplitudes for different components. The dimension of a\_
//   should be equal to the maximumNumberOfComponents as determined by the call to the constructor.
//
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  a=a_; 

  return 0;
}

//\begin{>>OGTrigFunctionInclude.tex}{\subsubsection{setConstants}}
int OGTrigFunction::
setConstants(const RealArray & c_ )
//---------------------------------------------------------------------------------------
// /Description: 
// 
// Use this function to supply different constants for different components.
//
// /c\_ (input): give constants for different components. The dimension of c\_
//   should be equal to the maximumNumberOfComponents as determined by the call to the constructor.
//
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  cc=c_; 

  return 0;
}


//\begin{>>OGTrigFunctionInclude.tex}{\subsubsection{setFrequencies}}
int OGTrigFunction::
setFrequencies(const RealArray & fx_, 
	       const RealArray & fy_, 
	       const RealArray & fz_, 
	       const RealArray & ft_)
//---------------------------------------------------------------------------------------
// /Description: 
// 
// Use this function to supply different frequencies for different components.
//
// /fx\_, fy\_, fz\_, ft\_ (input): give frequencies for different components. The dimension of fx\_
//        will determine the maximumNumberOfComponents.
//
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  int maximumNumberOfComponents=fx_.getLength(0);
  fx.redim(maximumNumberOfComponents);
  fy.redim(maximumNumberOfComponents);
  fz.redim(maximumNumberOfComponents);
  ft.redim(maximumNumberOfComponents);
  fx=fx_*Pi; 
  fy=fy_*Pi; 
  fz=fz_*Pi; 
  ft=ft_*Pi;

  return 0;
}

//\begin{>>OGTrigFunctionInclude.tex}{\subsubsection{setShifts}}
int OGTrigFunction::
setShifts(const RealArray & gx_, 
	  const RealArray & gy_, 
	  const RealArray & gz_, 
	  const RealArray & gt_)
//---------------------------------------------------------------------------------------
// /Description: 
// 
// Use this function to supply different shifts for different components.
//
// /gx\_, gy\_, gz\_, gt\_ (input): give shifts for different components. The dimensions of gx\_, gy\_,...
//   should be equal to the maximumNumberOfComponents as determined by the call to the constructor.
//
//\end{OGTrigFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
  gx=gx_; 
  gy=gy_; 
  gz=gz_; 
  gt=gt_;

  return 0;
}

  

OGTrigFunction::
OGTrigFunction(const OGTrigFunction & ogp )   // copy constructor is a deep copy
{
  *this=ogp;
}

//--------------------------------------------------------------------------------------
//  operator = is  a deep copy
//--------------------------------------------------------------------------------------
OGTrigFunction & OGTrigFunction::
operator=(const OGTrigFunction & ogp )
{
  fx=ogp.fx;
  fy=ogp.fy;
  fz=ogp.fz;
  ft=ogp.ft;
  gx=ogp.gx;
  gy=ogp.gy;
  gz=ogp.gz;
  gt=ogp.gt;
  a=ogp.a;
  cc=ogp.cc;
  
  return *this;
}

// Here are the non-inlined versions : Use these when OGFunction
// is passed as an argument


// Here are the non-inlined versions : Use these when OGFunction
// is passed as an argument


real OGTrigFunction::
operator()(const real x, const real y, const real z, const int n, const real t )
{
  return u(x,y,z,n,t);
}
real OGTrigFunction::
operator()(const real x, const real y, const real z, const int n)
{
  return u(x,y,z,n,0.);
}
real OGTrigFunction::
operator()(const real x, const real y, const real z)
{
  return u(x,y,z,0,0.);
}
real OGTrigFunction::
t(const real x, const real y, const real z, const int n, const real t )
{
  return ut(x,y,z,n,t);
}
real OGTrigFunction::
x(const real x, const real y, const real z, const int n, const real t )
{
  return ux(x,y,z,n,t);
}
real OGTrigFunction::
y(const real x, const real y, const real z, const int n, const real t )
{
  return uy(x,y,z,n,t);
}
real OGTrigFunction::
xx(const real x, const real y, const real z, const int n, const real t )
{
  return uxx(x,y,z,n,t);
}
real OGTrigFunction::
xy(const real x, const real y, const real z, const int n, const real t )
{
  return uxy(x,y,z,n,t);
}
real OGTrigFunction::
yy(const real x, const real y, const real z, const int n, const real t )
{
  return uyy(x,y,z,n,t);
}
real OGTrigFunction::
z(const real x, const real y, const real z, const int n, const real t )
{
  return uz(x,y,z,n,t);
}
real OGTrigFunction::
xz(const real x, const real y, const real z, const int n, const real t )
{
  return uxz(x,y,z,n,t);
}
real OGTrigFunction::
yz(const real x, const real y, const real z, const int n, const real t )
{
  return uyz(x,y,z,n,t);
}
real OGTrigFunction::
zz(const real x, const real y, const real z, const int n, const real t )
{
  return uzz(x,y,z,n,t);
}
real OGTrigFunction::
xxx(const real x, const real y, const real z, const int n, const real t )
{
  return fx(n)*fx(n)*fx(n)*sin(fx(n)*(x-gx(n)))*cos(fy(n)*(y-gy(n)))*cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
}
real OGTrigFunction::
xxxx(const real x, const real y, const real z, const int n, const real t )
{
  return pow(fx(n),4.)*cos(fx(n)*(x-gx(n)))*cos(fy(n)*(y-gy(n)))*cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
}

real OGTrigFunction::
gd(const int & ntd, 
   const int & nxd, 
   const int & nyd, 
   const int & nzd,
   const real x, const real y, const real z, const int n, const real t )
{
  if( ntd<0 || nxd<0 || nyd<0 || nzd <0 )
  {
    printf("OGTrigFunction::gd: ERROR: invalid derivative requested, ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);
    Overture::abort("error");
  }
  
  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
  {
    return operator()(x,y,z,n,t);
  }
  

  real result=1.;

  real pm = (ntd%2 == 0) ? (ntd/2 %2 ==0 ? 1 : -1) : (ntd+1)/2 %2 ==0 ? 1 : -1;
    
  result*= (ntd%2 == 0) ? pm*pow(ft(n),double(ntd))*cos(ft(n)*(t-gt(n)))*a(n)  : pm*pow(ft(n),double(ntd))*sin(ft(n)*(t-gt(n)))*a(n);

  pm = (nxd%2 == 0) ? (nxd/2 %2 ==0 ? 1 : -1) : (nxd+1)/2 %2 ==0 ? 1 : -1;
    
  result*= (nxd%2 == 0) ? pm*pow(fx(n),double(nxd))*cos(fx(n)*(x-gx(n)))  : pm*pow(fx(n),double(nxd))*sin(fx(n)*(x-gx(n)));

  pm = (nyd%2 == 0) ? (nyd/2 %2 ==0 ? 1 : -1) : (nyd+1)/2 %2 ==0 ? 1 : -1;
    
  result*= (nyd%2 == 0) ? pm*pow(fy(n),double(nyd))*cos(fy(n)*(y-gy(n)))  : pm*pow(fy(n),double(nyd))*sin(fy(n)*(y-gy(n)));

  pm = (nzd%2 == 0) ? (nzd/2 %2 ==0 ? 1 : -1) : (nzd+1)/2 %2 ==0 ? 1 : -1;
    
  result*= (nzd%2 == 0) ? pm*pow(fz(n),double(nzd))*cos(fz(n)*(z-gz(n))) : pm*pow(fz(n),double(nzd))*sin(fz(n)*(z-gz(n)));
  

  return result;
}



RealDistributedArray OGTrigFunction::
operator()(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=cos(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=cos(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
  return result;
}
RealDistributedArray OGTrigFunction::
operator()(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N)
{
  return operator()(c,I1,I2,I3,N,0.);
}

RealDistributedArray OGTrigFunction::
t(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-ft(n)*
                         cos(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*sin(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-ft(n)*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*sin(ft(n)*(t-gt(n)))*a(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-ft(n)* 
                         cos(fx(n)*(x-gx(n)))
                        *sin(ft(n)*(t-gt(n)))*a(n);
  return result;
}
RealDistributedArray OGTrigFunction::
x(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fx(n)*
                         sin(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fx(n)*
                         sin(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fx(n)*
                         sin(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n);
  return result;
}
RealDistributedArray OGTrigFunction::
y(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fy(n)*
                         cos(fx(n)*(x-gx(n)))
                        *sin(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fy(n)*
                         cos(fx(n)*(x-gx(n)))    
                        *sin(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}
RealDistributedArray OGTrigFunction::
z(const MappedGrid & c, const Index & I1,
                           const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=0.;
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-fz(n)*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *sin(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;



}
RealDistributedArray OGTrigFunction::
xx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fx(n))*
                         cos(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fx(n))*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fx(n))*
                         cos(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n);
  return result;
}

RealDistributedArray OGTrigFunction::
yy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fy(n))*
                         cos(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fy(n))*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}

RealDistributedArray OGTrigFunction::
zz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=0.;
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fz(n))*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}

RealDistributedArray OGTrigFunction::
xy(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=fx(n)*fy(n)*
                         sin(fx(n)*(x-gx(n)))
                        *sin(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=fx(n)*fy(n)*
                         sin(fx(n)*(x-gx(n)))    
                        *sin(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}

RealDistributedArray OGTrigFunction::
xz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=0.;
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=fx(n)*fz(n)*
                         sin(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *sin(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}

RealDistributedArray OGTrigFunction::
yz(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=0.;
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=fy(n)*fz(n)*
                         cos(fx(n)*(x-gx(n)))    
                        *sin(fy(n)*(y-gy(n)))
                        *sin(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    result(I1,I2,I3,N)=0.;
  return result;
}





RealDistributedArray OGTrigFunction::
laplacian(const MappedGrid & c, const Index & I1,
	  const Index & I2,const Index & I3, const Index & N, const real t,
	  const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=((-SQR(fx(n))-SQR(fy(n)))*cos(ft(n)*(t-gt(n)))*a(n))
	                *cos(fx(n)*(x-gx(n)))
	                *cos(fy(n)*(y-gy(n)));
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=((-SQR(fx(n))-SQR(fy(n))-SQR(fz(n)))*cos(ft(n)*(t-gt(n)))*a(n))
                        *cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)));
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=-SQR(fx(n))*
                         cos(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n);
  return result;
}



RealDistributedArray OGTrigFunction::
xxx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),3.)*
                         sin(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),3.)*
                         sin(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),3.)*
                         sin(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n);
  return result;
}

RealDistributedArray OGTrigFunction::
xxxx(const MappedGrid & c, const Index & I1,
                            const Index & I2,const Index & I3, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));
  if( c.numberOfDimensions()==2 )     
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),4.)*
                         cos(fx(n)*(x-gx(n)))
                        *cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else if(c.numberOfDimensions()==3 )
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),4.)*
                         cos(fx(n)*(x-gx(n)))    
                        *cos(fy(n)*(y-gy(n)))
                        *cos(fz(n)*(z-gz(n)))*cos(ft(n)*(t-gt(n)))*a(n);
  else
    for( int n=N.getBase(); n<=N.getBound(); n++ )
      result(I1,I2,I3,n)=pow(fx(n),4.)*
                         cos(fx(n)*(x-gx(n)))
                        *cos(ft(n)*(t-gt(n)))*a(n);
  return result;
}

RealDistributedArray OGTrigFunction::
gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd,
   const MappedGrid & c, const Index & I1,
   const Index & I2,const Index & I3, const Index & N, const real t,
   const GridFunctionParameters::GridFunctionType & centering)
{
  if( ntd<0 || nxd<0 || nyd<0 || nzd <0 )
  {
    printf("OGTrigFunction::gd: ERROR: invalid derivative requested, ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);
    Overture::abort("error");
  }
  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
  {
    return operator()(c,I1,I2,I3,N,t,centering);
  }

  RealDistributedArray x,y,z;
  getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

  RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
                   Range(I2.getBase(),I2.getBound()),
                   Range(I3.getBase(),I3.getBound()),
                   Range( N.getBase(), N.getBound()));

  result=1.;
  for( int n=N.getBase(); n<=N.getBound(); n++ )
  {
    real pm = (ntd%2 == 0) ? (ntd/2 %2 ==0 ? 1 : -1) : (ntd+1)/2 %2 ==0 ? 1 : -1;
    
    result(I1,I2,I3,n)*= (ntd%2 == 0) ? pm*pow(ft(n),double(ntd))*cos(ft(n)*(t-gt(n)))*a(n) 
      : pm*pow(ft(n),double(ntd))*sin(ft(n)*(t-gt(n)))*a(n);

    pm = (nxd%2 == 0) ? (nxd/2 %2 ==0 ? 1 : -1) : (nxd+1)/2 %2 ==0 ? 1 : -1;
    
    result(I1,I2,I3,n)*= (nxd%2 == 0) ? pm*pow(fx(n),double(nxd))*cos(fx(n)*(x-gx(n))) 
      : pm*pow(fx(n),double(nxd))*sin(fx(n)*(x-gx(n)));

    if( c.numberOfDimensions()>1 )
    {
      pm = (nyd%2 == 0) ? (nyd/2 %2 ==0 ? 1 : -1) : (nyd+1)/2 %2 ==0 ? 1 : -1;
    
      result(I1,I2,I3,n)*= (nyd%2 == 0) ? pm*pow(fy(n),double(nyd))*cos(fy(n)*(y-gy(n))) 
	: pm*pow(fy(n),double(nyd))*sin(fy(n)*(y-gy(n)));

      if( c.numberOfDimensions()>2 )
      {
	pm = (nzd%2 == 0) ? (nzd/2 %2 ==0 ? 1 : -1) : (nzd+1)/2 %2 ==0 ? 1 : -1;
    
	result(I1,I2,I3,n)*= (nzd%2 == 0) ? pm*pow(fz(n),double(nzd))*cos(fz(n)*(z-gz(n))) 
	  : pm*pow(fz(n),double(nzd))*sin(fz(n)*(z-gz(n)));
      }
    }
    
  }

  return result;
}


// obtain a general derivative using serial arrays
realSerialArray& OGTrigFunction::
gd( realSerialArray & result,   // put result here
    const realSerialArray & xy,  // coordinates to use if isRectangular==true
    const int numberOfDimensions,
    const bool isRectangular,
    const int & ntd, const int & nxd, const int & nyd, const int & nzd,
    const Index & J1, const Index & J2, 
    const Index & J3, const Index & N, 
    const real t /* =0. */, int option /* =0 */ )
{
  if( ntd<0 || nxd<0 || nyd<0 || nzd <0 )
  {
    printf("OGTrigFunction::gd: ERROR: invalid derivative requested, ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
	   ntd,nxd,nyd,nzd);
    Overture::abort("error");
  }

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

  // if the result has only room for 1 variable and only one component is requested then put the
  // answer in result.
  int n0=0;
  if( result.getLength(3)==1 && N.length()==1 )
  {
    n0=N.getBase()-result.getBase(3);
  }
  

  if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
  {
    if( numberOfDimensions==2 )     
      for( int n=N.getBase(); n<=N.getBound(); n++ )
	result(I1,I2,I3,n-n0)=cos(fx(n)*(x-gx(n)))*cos(fy(n)*(y-gy(n)))*cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
    else if( numberOfDimensions==3 )
      for( int n=N.getBase(); n<=N.getBound(); n++ )
	result(I1,I2,I3,n-n0)=cos(fx(n)*(x-gx(n)))*cos(fy(n)*(y-gy(n)))*cos(fz(n)*(z-gz(n)))
                          *cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
    else
      for( int n=N.getBase(); n<=N.getBound(); n++ )
	result(I1,I2,I3,n-n0)=cos(fx(n)*(x-gx(n)))*cos(ft(n)*(t-gt(n)))*a(n) +cc(n);
    return result;
  }

  

  result=1.;
  for( int n=N.getBase(); n<=N.getBound(); n++ )
  {
    real pm = (ntd%2 == 0) ? (ntd/2 %2 ==0 ? 1 : -1) : (ntd+1)/2 %2 ==0 ? 1 : -1;
    
    result(I1,I2,I3,n-n0)*= (ntd%2 == 0) ? pm*pow(ft(n),double(ntd))*cos(ft(n)*(t-gt(n)))*a(n) 
      : pm*pow(ft(n),double(ntd))*sin(ft(n)*(t-gt(n)))*a(n);

    pm = (nxd%2 == 0) ? (nxd/2 %2 ==0 ? 1 : -1) : (nxd+1)/2 %2 ==0 ? 1 : -1;
    
    result(I1,I2,I3,n-n0)*= (nxd%2 == 0) ? pm*pow(fx(n),double(nxd))*cos(fx(n)*(x-gx(n))) 
      : pm*pow(fx(n),double(nxd))*sin(fx(n)*(x-gx(n)));

    if( numberOfDimensions>1 )
    {
      pm = (nyd%2 == 0) ? (nyd/2 %2 ==0 ? 1 : -1) : (nyd+1)/2 %2 ==0 ? 1 : -1;
    
      result(I1,I2,I3,n-n0)*= (nyd%2 == 0) ? pm*pow(fy(n),double(nyd))*cos(fy(n)*(y-gy(n))) 
	: pm*pow(fy(n),double(nyd))*sin(fy(n)*(y-gy(n)));

      if( numberOfDimensions>2 )
      {
	pm = (nzd%2 == 0) ? (nzd/2 %2 ==0 ? 1 : -1) : (nzd+1)/2 %2 ==0 ? 1 : -1;
    
	result(I1,I2,I3,n-n0)*= (nzd%2 == 0) ? pm*pow(fz(n),double(nzd))*cos(fz(n)*(z-gz(n))) 
	  : pm*pow(fz(n),double(nzd))*sin(fz(n)*(z-gz(n)));
      }
    }
    
  }


  return result;
}


realCompositeGridFunction OGTrigFunction::
operator()(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::operator()(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
operator()(CompositeGrid & cg, const Index & N)
{
  return OGFunction::operator()(cg,N,0.);
} 
realCompositeGridFunction OGTrigFunction::
operator()(CompositeGrid & cg)
{
  return OGFunction::operator()(cg,0,0.);
} 
realCompositeGridFunction OGTrigFunction::
t(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::t(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
x(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::x(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
y(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::y(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
z(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::z(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
xx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xx(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
xy(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xy(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
xz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xz(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
yy(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::yy(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
yz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::yz(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
zz(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::zz(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
xxx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering)
{
  return OGFunction::xxx(cg,N,t,centering);
} 
realCompositeGridFunction OGTrigFunction::
xxxx(CompositeGrid & cg, const Index & N, const real t,
           const GridFunctionParameters::GridFunctionType & centering )
{
  return OGFunction::xxxx(cg,N,t,centering);
} 

realCompositeGridFunction OGTrigFunction::
gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd,
   CompositeGrid & cg, const Index & N, const real t,
   const GridFunctionParameters::GridFunctionType & centering )
{
  return OGFunction::gd(ntd,nxd,nyd,nzd,cg,N,t,centering);
} 

