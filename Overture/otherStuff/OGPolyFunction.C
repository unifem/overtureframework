// This file automatically generated from OGPolyFunction.bC with bpp.
#include "OGPolyFunction.h"

#define polyFunction EXTERN_C_NAME(polyfunction)
#define polyEvaluate EXTERN_C_NAME(polyevaluate)
extern "C"
{
    void polyFunction(const int &nd,
                                        const int &ndra,const int &ndrb,const int &ndsa,const int &ndsb,const int &ndta,const int &ndtb,
                		    const int &ndrra,const int &ndrrb,const int &ndrsa,const int &ndrsb,
                                        const int &ndrta,const int &ndrtb,const int &ndrca,const int &ndrcb,
                                        const int &ndc1,const int &ndc2,const int &ndc3,
                		    const int &nra,const int &nrb,const int &nsa,const int &nsb,const int &nta,const int &ntb, 
                                        const int &nca, const int &ncb, const int &nda, 
                                        const int &degree, const int &degreeTime, const real &t,
                		    const real &a, const real &c, real &r,
                                        const real &xa,const real &ya,const real &za, 
                                        const int &dx,const int &dy,const int &dz,const int &dt);

    void polyEvaluate(const int &nd, real &r, const real &x,const real &y,const real &z,const int &n,const real &t, 
                		    const int &ndc1,const int &ndc2,const int &ndc3,const int &nda,
                		    const int &degree, const int &degreeTime, const real &a, const real &c, 
                		    const int &dx,const int &dy,const int &dz,const int &dt);
  
}


//\begin{>OGPolyFunctionInclude.tex}{\subsubsection{Constructor}} 
OGPolyFunction::
OGPolyFunction(const int & degreeOfSpacePolynomial /* =2 */ , 
                              const int & numberOfDimensions0     /* =3 */ ,
                              const int & numberOfComponents0     /* =10 */ ,
                              const int & degreeOfTimePolynomial  /* =1 */  )
//---------------------------------------------------------------------------------------
// /Description: 
// Create a polynomial with the given degree, number Of Space Dimensions and for
// a maximum number of components. The polynomial created is of the form 
//  \begin{alignat*}{3}
//     U &= (1 + x + x^2 + ... + x^{n}) (1+t+...+t^{m})  &\qquad& \mbox{in 1D} \//     U &= (1 + x + x^2 + ... + x^{n} + y + y^2 + ... + y^{n} )(1+t+...+t^{m})  &\qquad& \mbox{in 2D} \//     U &= (1 + x + x^2 + ... + x^{n} + y + y^2 + ... + y^{n} +z + z^2 + ... +z^n)(1+t+...+t^{m})  &\qquad& \mbox{in 3D}
//  \end{alignat*}
//
// /degreeOfSpacePolynomial (input): degree of the polynomial in x,y,z (n in the above formula).
// /numberOfDimensions (input): number of space dimensions, 1,2, or 3.
// /numberOfComponents0 (input): maximum number of components required.
// /degreeOfTimePolynomial (input): degree of the polynomial in t (m in the above formula).
// 
// /Notes:
//  Only polynomials with degreeOfSpacePolynomial $<5$ and degreeOfTimePolynomial $<5$ are supported
// /Author: WDH
//\end{OGPolyFunctionInclude.tex} 
//---------------------------------------------------------------------------------------
{
    numberOfComponents=numberOfComponents0;
    numberOfDimensions=numberOfDimensions0;
    
    assert(numberOfComponents>=0 && degreeOfSpacePolynomial>=0 && degreeOfTimePolynomial>=0 &&
                  numberOfDimensions>=1 && numberOfDimensions<=3 );

    if( degreeOfSpacePolynomial>maximumDegreeX )
    {
        cout << "OGPolyFunction::WARNING: sorry, I can only make a polynomial of degree <=" << maximumDegreeX << "\n";
        cout << " You are trying to make a polynomial with degreeOfSpacePolynomial = " 
            << degreeOfSpacePolynomial << endl;
        cout << " I will use the maximum degree instead...\n";
    }
    if( degreeOfTimePolynomial>maximumDegreeT )
    {
        cout << "OGPolyFunction::WARNING: sorry can only make a polynomial in time of degree <=" << maximumDegreeT << " \n";
        cout << " You are trying to make a polynomial with degreeOfTimePolynomial = " 
            << degreeOfTimePolynomial << endl;
        cout << " I will use the maximum degree instead...\n";
    }

    degreeX=min(maximumDegreeX,degreeOfSpacePolynomial);
    degreeT=min(maximumDegreeT,degreeOfTimePolynomial);

    degreeY = numberOfDimensions>1 ? degreeX : 0;
    degreeZ = numberOfDimensions>2 ? degreeX : 0;
    
    Index N=Range(0,numberOfComponents-1);
    
    int nx=degreeX+1;
    
//  cc.redim(5,5,5,numberOfComponents);
    cc.redim(nx,nx,nx,numberOfComponents);
    cc=0.;
    cc(0,0,0,N)=1.;
    Range X(1,degreeX);
    cc(X,0,0,N)=1.;                 // only set diagonal terms for now
    if( numberOfDimensions>1 )
        cc(0,X,0,N)=1.;
    if( numberOfDimensions>2 )
        cc(0,0,X,N)=1.;

//  a.redim(5,numberOfComponents);

    a.redim(degreeT+1,numberOfComponents);
    a=0.;
    a(Range(0,degreeT),N)=1.;  // default is a linear polynomial in time
}

OGPolyFunction::
OGPolyFunction(const OGPolyFunction & ogp )   // copy constructor is a deep copy
{
    *this=ogp;
}

//--------------------------------------------------------------------------------------
//  operator = is  a deep copy
//--------------------------------------------------------------------------------------
OGPolyFunction & OGPolyFunction::
operator=(const OGPolyFunction & ogp )
{
    numberOfComponents=ogp.numberOfComponents;
    degreeX=ogp.degreeX;
    degreeY=ogp.degreeY;
    degreeZ=ogp.degreeZ;
    degreeT=ogp.degreeT;
    numberOfDimensions=ogp.numberOfDimensions;
    cc=ogp.cc;
    a=ogp.a;
    return *this;
}




//\begin{>>OGPolyFunctionInclude.tex}{\subsubsection{setCoefficients}} 
void OGPolyFunction::
setCoefficients( const RealArray & c, const RealArray & a0 )
// =========================================================================================
// /Description: Use this member function to set the coefficient matrices $c$ and $a$  
// of a {\it general} polynomial up to order 6.
// \[
//    U(x,y,z,n,t) = ( \sum_{i,j,k} c(i,j,k,n) x^i y^j z^k ) \sum_m a(m,n) t^m
// \]
//  Note that the values of {\tt numberOfDimensions} and
// {\tt numberOfComponents} given in the call to the constructor help to determine the polynomial
// created here.
//
// /c (input) : array of dimension $c(0:nx,0:nx,0:nx,0:{\tt numberOfComponents}-1)$ that gives the
//   coefficients of the spatial polynomial of degree nx
//   ({\tt numberOfComponents} is the value given in call to the constructor).
//   Some values in c may be ignored depending on the value for {\tt numberOfDimensions}.
// /a (input) : array of dimension $a(0:nt,0:{\tt numberOfComponents-1})$ that gives the coefficients
//   of the time polynomial ({\tt numberOfComponents} is the value given in call to the constructor).
//
// /Author: WDH
//\end{OGPolyFunctionInclude.tex} 
// ==========================================================================================
{
    numberOfComponents=c.getLength(3);

    if(!(c.getBase(0)==0 && c.getBound(0)<=maximumDegreeX &&
              c.getBase(1)==0 && c.getBound(1)<=maximumDegreeX &&
              c.getBase(2)==0 && c.getBound(2)<=maximumDegreeX &&
              c.getBase(3)==0 && c.getBound(3)<=numberOfComponents-1 &&
              a0.getBase(0)==0 && a0.getBound(0)<=maximumDegreeT &&
              a0.getBase(1)==0 && a0.getBound(1)>=numberOfComponents-1 ) )
    {
        cout << "OGPolyFunction::setCoefficients::ERROR: The arrays c and a are not valid sizes\n";
        cout << " They should be dimensioned at least c(0:nx,0:nx,0:nx,0:n-1) and a(0:nt,0:n-1)\n";
        cout << " Where n is the number of components = " << numberOfComponents << endl;
        printf("  and nx<=%i and nt<=%i\n",maximumDegreeX,maximumDegreeT);
        c.display("Here is c");
        a.display("Here is a");
        Overture::abort("OGPolyFunction::setCoefficients::ERROR: The arrays c and a are not big enough");
    }
    cc.redim(0);
    cc=c;
    a.redim(0);
    a=a0;

    degreeX=min(maximumDegreeX,cc.getBound(0));
    degreeT=min(maximumDegreeT,a.getBound(0));
  // printf(" *** OGPolyFunction: BEFORE degreeX=%i degreeT=%i ****\n",degreeX,degreeT);
  // cc.display("cc");
  // a.display("a");
  // for backward compatibility we check for extra zeroes in the arrays and reduce the order

    Range all;
    int i;
    for( i=degreeX; i>0; i-- )
    {
        Range R(i,i);
        real value=max(fabs(cc(R,all,all,all)));
        if( numberOfDimensions>1 )
            value=max(value, max(fabs(cc(all,R,all,all))));
        if( numberOfDimensions>2 )
            value=max(value,max(fabs(cc(all,all,R,all))));
        if( value!=0. )
        {
            degreeX=i;
            break;
        }
    }
    for( i=degreeT; i>0; i-- )
    {
        Range R(i,i);
        real value=max(fabs(a(R,all)));
        if( value!=0. )
        {
            degreeT=i;
            break;
        }
    }

    degreeY = numberOfDimensions>1 ? degreeX : 0;
    degreeZ = numberOfDimensions>2 ? degreeX : 0;
  // printf(" *** OGPolyFunction: AFTER degreeX=%i degreeT=%i ****\n",degreeX,degreeT);
    
    

}

// \begin{>>OGPolyFunctionInclude.tex}{\subsubsection{setCoefficients}}
void OGPolyFunction::
getCoefficients( RealArray & cx, RealArray & ct ) const
// =========================================================================================
// /Description: 
//    Return the current values of the coefficients. (See setCoeffcients)
//
// /cx (output) : array of dimension $cx(0:nx,0:nx,0:nx,0:{\tt numberOfComponents}-1)$ that gives the
//   coefficients of the spatial polynomial of maximum degree nx
//   ({\tt numberOfComponents} is the value given in call to the constructor).
//   Some values in c may be ignored depending on the value for {\tt numberOfDimensions}.
// /ct (input) : array of dimension $ct(0:nt,0:{\tt numberOfComponents-1})$ that gives the coefficients
//   of the time polynomial ({\tt numberOfComponents} is the value given in call to the constructor).
//
// /Author: WDH
//\end{OGPolyFunctionInclude.tex} 
// ==========================================================================================
{
    cx.redim(0);
    cx=cc;
    ct.redim(0);
    ct=a;
    
}

// This routine is used locally to check some arguments
void OGPolyFunction::
checkArguments(const Index & N)
{
    if( N.getBound() >= numberOfComponents )
    {
        cout << "OGPolyFunction:ERROR: trying to evaluate a function with " << N.getBound()+1 
                  << " components, \n  but the polynomial only supports numberOfComponents = " 
         	   << numberOfComponents << endl;
        cout << "Why not create the OGPolyFunction object with more components :-) \n";
        Overture::abort( "OGPolyFunction:ERROR");
    }
}

// define TIME(n,t)  ( a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n))))) )

#undef X
#undef Y
#undef Z
#define X coord(I[0],I[1],I[2],axis1)
#define Y coord(I[0],I[1],I[2],axis2)
#define Z coord(I[0],I[1],I[2],axis3)


// ================================ new names =======================================

// Here are the non-inlined versions : Use these when OGFunction
// is passed as an argument


#define EVAL(ndx,ndy,ndz,ndt,n,t)real r;polyEvaluate(numberOfDimensions, r, x,y,z,n,t, cc.getLength(0),cc.getLength(1),cc.getLength(2), a.getLength(0),degreeX, degreeT,*a.getDataPointer(), *cc.getDataPointer(), ndx,ndy,ndz,ndt);return r;

//\begin{>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative at a point}} 
real OGPolyFunction::
operator()(const real x, 
         	   const real y, 
         	   const real z, 
         	   const int n /* =0 */ , 
         	   const real t /* =0. */ )
//===================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
    EVAL(0,0,0,0,n,t);
//  return u(x,y,z,n,t);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPolyFunction::
operator()(const real x, const real y, const real z, const int n)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
      EVAL(0,0,0,0,n,0.);
//   return u(x,y,z,n,0.);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPolyFunction::
operator()(const real x, const real y, const real z)
//===================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
    EVAL(0,0,0,0,0,0.);
//  return u(x,y,z,0,0.);
}

//\begin{>>OGFunctionInclude.tex}{}
real OGPolyFunction::
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
    EVAL(1,0,0,0,n,t);
//  return ux(x,y,z,n,t);
}

real OGPolyFunction::
t(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(0,0,0,1,n,t);
//  return ut(x,y,z,n,t);
}
real OGPolyFunction::
y(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(0,1,0,0,n,t);
//  return uy(x,y,z,n,t);
}

real OGPolyFunction::
xx(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(2,0,0,0,n,t);
//  return uxx(x,y,z,n,t);
}

real OGPolyFunction::
xy(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(1,1,0,0,n,t);
//  return uxy(x,y,z,n,t);
}
real OGPolyFunction::
yy(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(0,2,0,0,n,t);
//return uyy(x,y,z,n,t);
}
real OGPolyFunction::
z(const real x, const real y, const real z, const int n, const real t )
//===================================================================================
// ==========================================================================================
{
    EVAL(0,0,1,0,n,t);
//return uz(x,y,z,n,t);
}
real OGPolyFunction::
xz(const real x, const real y, const real z, const int n, const real t )
// ==========================================================================================
{
    EVAL(1,0,1,0,n,t);
//return uxz(x,y,z,n,t);
}
real OGPolyFunction::
yz(const real x, const real y, const real z, const int n, const real t )
{
    EVAL(0,1,1,0,n,t);
//return uyz(x,y,z,n,t);
}
real OGPolyFunction::
zz(const real x, const real y, const real z, const int n, const real t )
{
    EVAL(0,0,2,0,n,t);
//return uzz(x,y,z,n,t);
}
real OGPolyFunction::
xxx(const real x, const real y, const real z, const int n, const real t )
{
    EVAL(3,0,0,0,n,t);
//return (6.*cc(3,0,0,n)+x*24.*cc(4,0,0,n))*TIME(n,t);
}
real OGPolyFunction::
xxxx(const real x, const real y, const real z, const int n, const real t )
{
    EVAL(4,0,0,0,n,t);
//return (24.*cc(4,0,0,n))*TIME(n,t);
}


#define GET_TIME(result)switch (ntd)  {  case 0:  /*    result*=TIME(n,t); */  if( degreeT==0 )  result*=a(0,n);  else if( degreeT==1 )  result*=a(0,n)+t*(a(1,n));  else if( degreeT==2 )  result*=a(0,n)+t*(a(1,n)+t*(a(2,n)));  else if( degreeT==3 )  result*=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n))));  else if( degreeT==4 )  result*=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)))));  else if( degreeT==5 )  result*=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n))))));  else if( degreeT==6 )  result*=a(0,n)+t*(a(1,n)+t*(a(2,n)+t*(a(3,n)+t*(a(4,n)+t*(a(5,n)+t*(a(6,n)))))));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 1:  /* result*=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))); */  if( degreeT==0 )  result*=0.;  else if( degreeT==1 )  result*=a(1,n);  else if( degreeT==2 )  result*=(a(1,n)+t*(2.*a(2,n)));  else if( degreeT==3 )  result*=(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n))));  else if( degreeT==4 )  result*=(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))));  else if( degreeT==5 )  result*=(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n))))));  else if( degreeT==6 )  result*=(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)+t*(5.*a(5,n)+t*(6.*a(6,n)))))));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 2:  /* result*=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n))); */  if( degreeT==0 )  result*=0.;  else if( degreeT==1 )  result*=0.;  else if( degreeT==2 )  result*=(2.*a(2,n));  else if( degreeT==3 )  result*=(2.*a(2,n)+t*(6.*a(3,n)));  else if( degreeT==4 )  result*=(2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n))));  else if( degreeT==5 )  result*=(2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)))));  else if( degreeT==6 )  result*=(2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)+t*(20.*a(5,n)+t*(30.*a(6,n))))));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 3:  /* result*=6.*a(3,n)+t*(24.*a(4,n)); */  if( degreeT<=2 )  result*=0.;  else if( degreeT==3 )  result*=(6.*a(3,n));  else if( degreeT==4 )  result*=(6.*a(3,n)+t*(24.*a(4,n)));  else if( degreeT==5 )  result*=(6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n))));  else if( degreeT==6 )  result*=(6.*a(3,n)+t*(24.*a(4,n)+t*(60.*a(5,n)+t*(120.*a(6,n)))));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 4:  /* result*=24.*a(4,n); */  if( degreeT<=3 )  result*=0.;  else if( degreeT==4 )  result*=(24.*a(4,n));  else if( degreeT==5 )  result*=(24.*a(4,n)+t*(120.*a(5,n)));  else if( degreeT==6 )  result*=(24.*a(4,n)+t*(120.*a(5,n)+t*(360.*a(6,n))));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 5:  /* result*=24.*a(4,n); */  if( degreeT<=4 )  result*=0.;  else if( degreeT==5 )  result*=(120.*a(5,n));  else if( degreeT==6 )  result*=(120.*a(5,n)+t*(720.*a(6,n)));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  case 6:  /* result*=24.*a(4,n); */  if( degreeT<=5 )  result*=0.;  else if( degreeT==6 )  result*=(720.*a(6,n));  else  {  printf("ERROR invalid degreeTime\n");  Overture::abort("error");  }  break;  default:  printf("OGPolyFunction:ERROR: gd\n");  Overture::abort("error");  }




//\begin{>>OGFunctionInclude.tex}{}
real OGPolyFunction::
gd(const int & ntd, 
      const int & nxd, 
      const int & nyd, 
      const int & nzd,
      const real x, 
      const real y, 
      const real z, 
      const int n /* =0 */, 
      const real t /* =0. */ )
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
    {
        return operator()(x,y,z,n,t);
    }

    if( nxd>degreeX || nyd>degreeY || nzd>degreeZ || ntd >degreeT )
    {
        return 0.;
    }
    

    int ix,iy,iz;
    real xFactorial=1., yFactorial=1., zFactorial=1.;
    for( ix=2; ix<=nxd; ix++ )
        xFactorial*=ix; 
    for( iy=2; iy<=nyd; iy++ )
        yFactorial*=iy;
    for( iz=2; iz<=nzd; iz++ )
        zFactorial*=iz;
    
    real xPow, yPow, zPow;

    real result=0.;
    zPow=zFactorial;
    for( iz=nzd; iz<=degreeX; iz++ )
    {
        yPow=yFactorial;
        for( iy=nyd; iy<=degreeX; iy++ )
        {
            const real yPowZpow = yPow*zPow;
            xPow=xFactorial;
            for( ix=nxd; ix<=degreeX; ix++ )
            {
      	result+=cc(ix,iy,iz,n)*xPow*yPowZpow;
      	if( ix<degreeX )
        	  xPow*=(ix+1)*x/(ix-nxd+1);
            }
            if( iy<degreeX )
      	yPow*=(iy+1)*y/(iy-nyd+1);
        }
        if( iz<degreeX )
            zPow*=(iz+1)*z/(iz-nzd+1);;
    }
    
    GET_TIME(result)

    return result;
}





//\begin{>>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative on a MappedGrid}} 
RealDistributedArray OGPolyFunction::
operator()(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
    return operator()(c,I1,I2,I3,N,0.);
}


//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPolyFunction::
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
//    printf(" ++++++++++++++OGPolyFunction::operator(): N=[%i,%i]\n",N.getBase(),N.getBound());
//    cc.display("cc from OGPolyFunction");


  // POLY(0,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,0,0,0 ); 
    
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result;
//   result.partition(x.getPartition());
//   result.redim(Range(I1.getBase(),I1.getBound()),
// 	       Range(I2.getBase(),I2.getBound()),
// 	       Range(I3.getBase(),I3.getBound()),
// 	       Range( N.getBase(), N.getBound()));
                                      
//   bool useNew=true;
//   if( useNew )
//   {
//     int ndra=x.getDataBase(0);
//     int ndsa=x.getDataBase(1);
//     int ndta=x.getDataBase(2);
        
//     int ndrb=x.getRawDataSize(0)+ndra-1;
//     int ndsb=x.getRawDataSize(1)+ndsa-1;
//     int ndtb=x.getRawDataSize(2)+ndta-1;
        
//     // x,y, and z are views we must play around to get the pointer we want
//     real & xp = x(ndra,ndsa,ndta); // these are formally out of bounds!
//     real & yp = y(ndra,ndsa,ndta);
//     real & zp = z(ndra,ndsa,ndta);

//     polyFunction(c.numberOfDimensions(),
//                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,
//                  result.getBase(0),result.getBound(0),result.getBase(1),result.getBound(1),
//                  result.getBase(2),result.getBound(2),
//                  cc.getLength(0),cc.getLength(1),cc.getLength(2),
// 		 I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),
//                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,
// 		 *a.getDataPointer(), *cc.getDataPointer(), *result.getDataPointer(),
//                  xp,yp,zp,
//                  0,0,0,0 ); // dx,dy,dz,dt

//     // result.display("result from new");
//   }
//   else
//   {
//     int n;
//     if( c.numberOfDimensions()==2 )     
//       for( n=N.getBase(); n<=N.getBound(); n++ )
// 	result(I1,I2,I3,n)=
//           (cc(0,0,0,n)+x*(cc(1,0,0,n)+y*cc(1,1,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n))))
// 	   +y*(cc(0,1,0,n)+y*(cc(0,2,0,n)+y*(cc(0,3,0,n)+y*cc(0,4,0,n)))))*TIME(n,t);
//     else if( c.numberOfDimensions()==3 )
//       for( n=N.getBase(); n<=N.getBound(); n++ )
// 	result(I1,I2,I3,n)=
//           (cc(0,0,0,n)
//            +x*(cc(1,0,0,n)+y*cc(1,1,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n))))
//            +y*(cc(0,1,0,n)+z*cc(0,1,1,n)+y*(cc(0,2,0,n)+y*(cc(0,3,0,n)+y*cc(0,4,0,n))))
//   	   +z*(cc(0,0,1,n)+x*cc(1,0,1,n)+z*(cc(0,0,2,n)+z*(cc(0,0,3,n)+z*cc(0,0,4,n)))))*TIME(n,t);
//     else
//       for( n=N.getBase(); n<=N.getBound(); n++ )
// 	result(I1,I2,I3,n)=
// 	  (cc(0,0,0,n)+x*(cc(1,0,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n)))))*TIME(n,t);

//     // result.display("result from old");
//   }
    
    return result;
    
}

//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPolyFunction::
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
  // POLY(1,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  1,0,0,0 ); 

//  result.display("result from new");

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()==2 )     
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(1,0,0,n)+y*cc(1,1,0,n)+x*(2.*cc(2,0,0,n)+x*(3.*cc(3,0,0,n)+x*(4.*cc(4,0,0,n))))
//                          )*TIME(n,t);
//   else if( c.numberOfDimensions()==3 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(1,0,0,n)+y*cc(1,1,0,n)+z*cc(1,0,1,n)+x*(2.*cc(2,0,0,n)+x*(3.*cc(3,0,0,n)
//                           +x*(4.*cc(4,0,0,n)))))*TIME(n,t);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(1,0,0,n)+x*(2.*cc(2,0,0,n)+x*(3.*cc(3,0,0,n)+x*(4.*cc(4,0,0,n))))
//                          )*TIME(n,t);
//   result.display("result from old");
    return result;
}

RealDistributedArray OGPolyFunction::
y(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,1,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,1,0,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()==2 )     
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(0,1,0,n)+x*cc(1,1,0,n)+y*(2.*cc(0,2,0,n)+y*(3.*cc(0,3,0,n)+y*(4.*cc(0,4,0,n))))
//                          )*TIME(n,t);
//   else if( c.numberOfDimensions()==3 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(0,1,0,n)+x*cc(1,1,0,n)+z*cc(0,1,1,n)+y*(2.*cc(0,2,0,n)+y*(3.*cc(0,3,0,n)
//                           +y*(4.*cc(0,4,0,n)))))*TIME(n,t);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;

    return result;
}

RealDistributedArray OGPolyFunction::
z(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,0,1,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,0,1,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()==3 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(cc(0,0,1,n)+x*cc(1,0,1,n)+y*cc(0,1,1,n)+z*(2.*cc(0,0,2,n)+z*(3.*cc(0,0,3,n)
//                           +z*(4.*cc(0,0,4,n)))))*TIME(n,t);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;

    return result;

}
RealDistributedArray OGPolyFunction::
t(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,0,0,1);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,0,0,1 ); 
//result.display("u.t result from new");

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   int n;
//   if( c.numberOfDimensions()==2 )     
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=
//           (cc(0,0,0,n)+x*(cc(1,0,0,n)+y*cc(1,1,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n))))
//                       +y*(cc(0,1,0,n)+y*(cc(0,2,0,n)+y*(cc(0,3,0,n)+y*cc(0,4,0,n)))))*
// 			(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))));
//   else if( c.numberOfDimensions()==3 )
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=
//           (cc(0,0,0,n)
//            +x*(cc(1,0,0,n)+y*cc(1,1,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n))))
//            +y*(cc(0,1,0,n)+z*cc(0,1,1,n)+y*(cc(0,2,0,n)+y*(cc(0,3,0,n)+y*cc(0,4,0,n))))
//   	   +z*(cc(0,0,1,n)+x*cc(1,0,1,n)+z*(cc(0,0,2,n)+z*(cc(0,0,3,n)+z*cc(0,0,4,n)))))*
// 			(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))));
//   else
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=
//           (cc(0,0,0,n)+x*(cc(1,0,0,n)+x*(cc(2,0,0,n)+x*(cc(3,0,0,n)+x*cc(4,0,0,n))))
//                       )*(a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n)))));
//result.display("u.t results from old");

    return result;
}

RealDistributedArray OGPolyFunction::
xx(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(2,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  2,0,0,0 ); 

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   for( int n=N.getBase(); n<=N.getBound(); n++ )
//     result(I1,I2,I3,n)=(2.*cc(2,0,0,n)+x*(6.*cc(3,0,0,n)+x*(12.*cc(4,0,0,n))))*TIME(n,t);
    return result;
}

RealDistributedArray OGPolyFunction::
yy(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,2,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,2,0,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()>1 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(2.*cc(0,2,0,n)+y*(6.*cc(0,3,0,n)+y*(12.*cc(0,4,0,n))))*TIME(n,t);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;
    return result;
}

RealDistributedArray OGPolyFunction::
zz(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,0,2,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,0,2,0 ); 

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()==3 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(2.*cc(0,0,2,n)+z*(6.*cc(0,0,3,n)+z*(12.*cc(0,0,4,n))))*TIME(n,t);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;
    return result;
}

RealDistributedArray OGPolyFunction::
xy(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(1,1,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  1,1,0,0 ); 

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()>1 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=cc(1,1,0,n);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;
    return result;
}

RealDistributedArray OGPolyFunction::
xz(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(1,0,1,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  1,0,1,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()>1 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=cc(1,0,1,n);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;
    return result;
}

RealDistributedArray OGPolyFunction::
yz(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(0,1,1,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  0,1,1,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()>1 )
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=cc(0,1,1,n);
//   else
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=x*0.;
    return result;
}



RealDistributedArray OGPolyFunction::
laplacian(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(-2,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  -2,0,0,0 ); 

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   if( c.numberOfDimensions()==1 )
//   {
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(2.*cc(2,0,0,n)+x*(6.*cc(3,0,0,n)+x*(12.*cc(4,0,0,n))))*TIME(n,t);
//   }
//   else if( c.numberOfDimensions()==2 )
//   {
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(2.*cc(2,0,0,n)+x*(6.*cc(3,0,0,n)+x*(12.*cc(4,0,0,n)))+
//                           2.*cc(0,2,0,n)+y*(6.*cc(0,3,0,n)+y*(12.*cc(0,4,0,n))))*TIME(n,t);
//   }
//   else
//   {
//     for( int n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)=(2.*cc(2,0,0,n)+x*(6.*cc(3,0,0,n)+x*(12.*cc(4,0,0,n)))+
//                           2.*cc(0,2,0,n)+y*(6.*cc(0,3,0,n)+y*(12.*cc(0,4,0,n)))+
//                           2.*cc(0,0,2,n)+z*(6.*cc(0,0,3,n)+z*(12.*cc(0,0,4,n))))*TIME(n,t);
//   }
    
    return result;
}



RealDistributedArray OGPolyFunction::
xxx(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(3,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  3,0,0,0 ); 

//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   for( int n=N.getBase(); n<=N.getBound(); n++ )
//     result(I1,I2,I3,n)=(6.*cc(3,0,0,n)+x*(24.*cc(4,0,0,n)))*TIME(n,t);
    return result;
}

RealDistributedArray OGPolyFunction::
xxxx(const MappedGrid & c, const Index & I1,
                                                        const Index & I2,const Index & I3, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
// ==========================================================================================
// ==========================================================================================
{
  // POLY(4,0,0,0);
        checkArguments( N );  
        RealDistributedArray result;  
        int ndra,ndrb,ndsa,ndsb,ndta,ndtb;
        real *xp,*yp,*zp;
        if( centering==GridFunctionParameters::defaultCentering )
        {
      // printf(" OGPoly: centering is default\n");
            MappedGrid & mg = (MappedGrid &)c;
            result.partition(mg.getPartition());
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
            const realArray & center = c.center();
            #ifdef USE_PPP
                const realSerialArray & xy = center.getLocalArrayWithGhostBoundaries();
            #else
                const realSerialArray & xy = center;
            #endif
            ndra=xy.getDataBase(0);  
            ndsa=xy.getDataBase(1);  
            ndta=xy.getDataBase(2);  
            ndrb=xy.getRawDataSize(0)+ndra-1;  
            ndsb=xy.getRawDataSize(1)+ndsa-1;  
            ndtb=xy.getRawDataSize(2)+ndta-1;
            int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
            real *xyp =xy.getDataPointer();
            xp = xyp;
            yp = (xyp+size);
            zp = (xyp+2*size);
        }
        else
        {
            RealDistributedArray xd,yd,zd;  
            getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,xd,yd,zd);  
            result.partition(xd.getPartition());  
            result.redim(Range(I1.getBase(),I1.getBound()),  
                 		 Range(I2.getBase(),I2.getBound()),  
                 		 Range(I3.getBase(),I3.getBound()),  
                 		 Range( N.getBase(), N.getBound()));  
          #ifndef USE_PPP
            const realSerialArray & x = xd;
            const realSerialArray & y = yd;
            const realSerialArray & z = zd;
          #else
            const realSerialArray & x = xd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & y = yd.getLocalArrayWithGhostBoundaries();
            const realSerialArray & z = zd.getLocalArrayWithGhostBoundaries();
          #endif
            ndra=x.getDataBase(0);  
            ndsa=x.getDataBase(1);  
            ndta=x.getDataBase(2);  
            ndrb=x.getRawDataSize(0)+ndra-1;  
            ndsb=x.getRawDataSize(1)+ndsa-1;  
            ndtb=x.getRawDataSize(2)+ndta-1;  
            /* x,y, and z are views we must play around to get the pointer we want */ 
            xp = &x(ndra,ndsa,ndta); /* these are formally out of bounds! */ 
            yp = &y(ndra,ndsa,ndta);  
            zp = &z(ndra,ndsa,ndta);  
        }
        #ifdef USE_PPP
            const realSerialArray & r = result.getLocalArrayWithGhostBoundaries();
        #else
            const realSerialArray & r = result;
        #endif
        polyFunction(c.numberOfDimensions(),  
                                  ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                                  r.getBase(0),r.getBound(0),r.getBase(1),r.getBound(1),
                                  r.getBase(2),r.getBound(2),r.getBase(3),r.getBound(3),
                                  cc.getLength(0),cc.getLength(1),cc.getLength(2),  
                 	       I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),I3.getBase(),I3.getBound(),  
                                  N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
                 	       *a.getDataPointer(), *cc.getDataPointer(), *r.getDataPointer(),  
                                  *xp,*yp,*zp,  
                                  4,0,0,0 ); 
//   checkArguments( N );
//   RealDistributedArray x,y,z;
//   getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);

//   RealDistributedArray result(Range(I1.getBase(),I1.getBound()),
//                    Range(I2.getBase(),I2.getBound()),
//                    Range(I3.getBase(),I3.getBound()),
//                    Range( N.getBase(), N.getBound()));
//   for( int n=N.getBase(); n<=N.getBound(); n++ )
//     result(I1,I2,I3,n)=(24.*cc(4,0,0,n))*TIME(n,t);
    return result;
}

//\begin{>>OGFunctionInclude.tex}{}
RealDistributedArray OGPolyFunction::
gd(const int & ntd, 
      const int & nxd, 
      const int & nyd, 
      const int & nzd,
      const MappedGrid & c, 
      const Index & I1,
      const Index & I2,
      const Index & I3, 
      const Index & N, 
      const real t /* =0. */,
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
    if( ntd<0 || nxd<0 || nyd<0 || nzd <0 )
    {
        printf("OGPolyFunction::gd: ERROR: invalid derivative requested, ntd=%i, nxd=%i, nyd=%i, nzd=%i\n",
         	   ntd,nxd,nyd,nzd);
        Overture::abort("error");
    }
    if( ntd==0 && nxd==0 && nyd==0 && nzd==0 )
    {
        return operator()(c,I1,I2,I3,N,t,centering);
    }

    RealDistributedArray x,y,z;
    getCoordinates(centering,c.numberOfDimensions(),c,I1,I2,I3,x,y,z);


    RealDistributedArray result;
    #ifdef USE_PPP
        MappedGrid & mg = (MappedGrid &)c;
        result.partition(mg.getPartition());
    #endif
    result.redim(Range(I1.getBase(),I1.getBound()),
             	       Range(I2.getBase(),I2.getBound()),
             	       Range(I3.getBase(),I3.getBound()),
             	       Range( N.getBase(), N.getBound()));

    if( nxd>degreeX || nyd>degreeY || nzd>degreeZ || ntd >degreeT )
    {
        result=0.;
        return result;
    }
    

    result=0.;
    int ix,iy,iz,n;
    real xFactorial=1., yFactorial=1., zFactorial=1.;
    for( ix=2; ix<=nxd; ix++ )
        xFactorial*=ix; 
    for( iy=2; iy<=nyd; iy++ )
        yFactorial*=iy;
    for( iz=2; iz<=nzd; iz++ )
        zFactorial*=iz;
    
    if( c.numberOfDimensions()==1 )
    {
        RealDistributedArray xPow;
        #ifdef USE_PPP
            MappedGrid & mg = (MappedGrid &)c;
            xPow.partition(mg.getPartition());
        #endif
        xPow.redim(I1,I2,I3);
            
        xPow=xFactorial;
        for( ix=nxd; ix<=degreeX; ix++ )
        {
            for( n=N.getBase(); n<=N.getBound(); n++ )
      	result(I1,I2,I3,n)+=cc(ix,0,0,n)*xPow;
            if( ix<degreeX )
                xPow*=((ix+1.)/(ix-nxd+1.))*x;
        }
    }
    else if( c.numberOfDimensions()==2 )
    {
        RealDistributedArray xPow,yPow;
        #ifdef USE_PPP
            MappedGrid & mg = (MappedGrid &)c;
            xPow.partition(mg.getPartition());
            yPow.partition(mg.getPartition());
        #endif
        xPow.redim(I1,I2,I3);
        yPow.redim(I1,I2,I3);

        yPow=yFactorial;
        for( iy=nyd; iy<=degreeX; iy++ )
        {
            xPow=xFactorial;
            for( ix=nxd; ix<=degreeX; ix++ )
            {
      	const RealDistributedArray & temp = evaluate( xPow*yPow );
      	for( n=N.getBase(); n<=N.getBound(); n++ )
        	  result(I1,I2,I3,n)+=cc(ix,iy,0,n)*temp;
      	if( ix<degreeX )
        	  xPow*=((ix+1.)/(ix-nxd+1.))*x;
            }
            if( iy<degreeX )
      	yPow*=((iy+1.)/(iy-nyd+1.))*y;
        }
    }
    else
    {
        RealDistributedArray xPow,yPow,zPow;
        #ifdef USE_PPP
            MappedGrid & mg = (MappedGrid &)c;
            xPow.partition(mg.getPartition());
            yPow.partition(mg.getPartition());
            zPow.partition(mg.getPartition());
        #endif
        xPow.redim(I1,I2,I3);
        yPow.redim(I1,I2,I3);
        zPow.redim(I1,I2,I3);

        zPow=zFactorial;
        for( iz=nzd; iz<=degreeX; iz++ )
        {
            yPow=yFactorial;
            for( iy=nyd; iy<=degreeX; iy++ )
            {
      	const RealDistributedArray & yPowZpow = evaluate( yPow*zPow );
                xPow=xFactorial;
      	for( ix=nxd; ix<=degreeX; ix++ )
      	{
        	  const RealDistributedArray & temp = evaluate( xPow*yPowZpow );
        	  for( n=N.getBase(); n<=N.getBound(); n++ )
          	    result(I1,I2,I3,n)+=cc(ix,iy,iz,n)*temp;
        	  if( ix<degreeX )
          	    xPow*=((ix+1.)/(ix-nxd+1.))*x;
      	}
      	if( iy<degreeX )
        	  yPow*=((iy+1.)/(iy-nyd+1.))*y;
            }
            if( iz<degreeX )
                zPow*=((iz+1.)/(iz-nzd+1.))*z;
        }
    }
    
    GET_TIME(for( n=N.getBase(); n<=N.getBound(); n++ )result(I1,I2,I3,n))
//   switch (ntd)
//   {
//   case 0:
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)*=TIME(n,t);
//     break;
//   case 1:
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)*=a(1,n)+t*(2.*a(2,n)+t*(3.*a(3,n)+t*(4.*a(4,n))));
//     break;
//   case 2:
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)*=2.*a(2,n)+t*(6.*a(3,n)+t*(12.*a(4,n)));
//     break;
//   case 3:
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)*=6.*a(3,n)+t*(24.*a(4,n));
//     break;
//   case 4:
//     for( n=N.getBase(); n<=N.getBound(); n++ )
//       result(I1,I2,I3,n)*=24.*a(4,n);
//     break;
//   default:
//     printf("OGPolyFunction:ERROR: gd\n");
//     throw "error";
//   }
    

    return result;
}

//\begin{>>OGFunctionInclude.tex}{\section{gd}}
realSerialArray&  OGPolyFunction::
gd( realSerialArray & result, 
        const realSerialArray & x,  // coordinates to use if isRectangular==true
        const int numberOfDimensions,
        const bool isRectangular,
        const int & ntd, const int & nxd, const int & nyd, const int & nzd,
        const Index & I1, const Index & I2, 
        const Index & I3, const Index & N, 
        const real t /* =0. */, int option /* =0 */  )
//===================================================================================
// /Description: Evaluate a general derivative. This version was added for the parallel case.
//   It only operates on serial arrays.
// /result (input) : put the result here.
// /xy (input): coordinates to use if isRectangular==true
// /isRectangular (input) : true if the grid is rectangular
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
    
    int i1a=max(I1.getBase(),result.getBase(0)), i1b=min(I1.getBound(),result.getBound(0));
    int i2a=max(I2.getBase(),result.getBase(1)), i2b=min(I2.getBound(),result.getBound(1));
    int i3a=max(I3.getBase(),result.getBase(2)), i3b=min(I3.getBound(),result.getBound(2));

    if( i1a>i1b || i2a>i2b || i3a>i3b )
        return result;  // nothing to do
                		    
    Index J1=Range(i1a,i1b);
    Index J2=Range(i2a,i2b);
    Index J3=Range(i3a,i3b);


    int ndra,ndrb,ndsa,ndsb,ndta,ndtb;

    ndra=x.getDataBase(0);  
    ndsa=x.getDataBase(1);  
    ndta=x.getDataBase(2);  
        
    ndrb=x.getRawDataSize(0)+ndra-1;  
    ndsb=x.getRawDataSize(1)+ndsa-1;  
    ndtb=x.getRawDataSize(2)+ndta-1;

    int size=(ndrb-ndra+1)*(ndsb-ndsa+1)*(ndtb-ndta+1);
    real *xp =x.getDataPointer();
    real *yp = (xp+size);
    real *zp = (xp+2*size);
        

  // if the result has only room for 1 variable and only one component is requested then put the
  // answer in the only component of result. (Pretend that result is dimensioned: result(.,.,.,N))
    int ndrca=result.getBase(3), ndrcb=result.getBound(3);
    if( result.getLength(3)==1 && N.length()==1 )
    {
        ndrca=N.getBase(); 
        ndrcb=N.getBound();
    }

    polyFunction(numberOfDimensions,
                              ndra,ndrb,ndsa,ndsb,ndta,ndtb,  
                              result.getBase(0),result.getBound(0),
                              result.getBase(1),result.getBound(1),
                              result.getBase(2),result.getBound(2),  
                              ndrca,ndrcb,
                              cc.getLength(0),cc.getLength(1),cc.getLength(2),  
             	       J1.getBase(),J1.getBound(),J2.getBase(),J2.getBound(),J3.getBase(),J3.getBound(),  
                              N.getBase(),N.getBound(), a.getLength(0), degreeX, degreeT, t,  
             	       *a.getDataPointer(), *cc.getDataPointer(), *result.getDataPointer(),  
                              *xp,*yp,*zp,  
                              nxd,nyd,nzd,ntd ); 

    return result;
}


//\begin{>>OGFunctionInclude.tex}{\subsection{Evaluate the function or a derivative on a CompositeGrid}} 
realCompositeGridFunction OGPolyFunction::
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
realCompositeGridFunction OGPolyFunction::
operator()(CompositeGrid & cg, const Index & N)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
    return OGFunction::operator()(cg,N,0.);
} 
//\begin{>>OGFunctionInclude.tex}{}
realCompositeGridFunction OGPolyFunction::
operator()(CompositeGrid & cg)
// ==========================================================================================
//\end{OGFunctionInclude.tex} 
// ==========================================================================================
{
    return OGFunction::operator()(cg,0,0.);
} 
//\begin{>>OGFunctionInclude.tex}{}
realCompositeGridFunction OGPolyFunction::
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

realCompositeGridFunction OGPolyFunction::
t(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::t(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
y(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::y(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
z(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::z(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
xx(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::xx(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
xy(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::xy(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
xz(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::xz(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
yy(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::yy(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
yz(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::yz(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
zz(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::zz(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
laplacian(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::laplacian(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
xxx(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::xxx(cg,N,t,centering);
} 
realCompositeGridFunction OGPolyFunction::
xxxx(CompositeGrid & cg, const Index & N, const real t,
                      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::xxxx(cg,N,t,centering);
} 

realCompositeGridFunction OGPolyFunction::
gd(const int & ntd, const int & nxd, const int & nyd, const int & nzd, 
      CompositeGrid & cg, const Index & N, const real t,
      const GridFunctionParameters::GridFunctionType & centering)
{
    return OGFunction::gd(ntd,nxd,nyd,nzd,cg,N,t,centering);
} 

#undef TIME


