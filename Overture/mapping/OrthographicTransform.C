#include "OrthographicTransform.h"
#include "MappingInformation.h"
#include <float.h>


OrthographicTransform::
OrthographicTransform( const real sa_   /* = 1. */, 
		       const real sb_   /* = 1. */ , 
		       const Pole pole_ /* = northPole */ )
//===========================================================================
/// \brief  
///     The {\tt OrthographicTransform} is used by the {\tt ReparameterizationTransform}
///     to remove a polar singularity.
/// \param sa_, sb_ (input) : parameters that specify the dimensions of the plane that is projected
///    onto the sphere in the orthographic transform.
/// \param pole (input) : reparameterize the {\tt northPole} or the {\tt southPole}. 
//===========================================================================
: Mapping(3,3,parameterSpace,parameterSpace)   
{ 
  OrthographicTransform::className="OrthographicTransform";
  setName( Mapping::mappingName,"orthographic transform");
  setGridDimensions( axis1,11 );
  setGridDimensions( axis2,11 );
  setGridDimensions( axis3,7  );
  sa=sa_;
  sb=sb_;
  pole=(int)pole_;   // +1=north pole, -1=south pole
  tAxis=axis2;
  setBasicInverseOption(canInvert);  // basicInverse is available
  mappingHasChanged();
}

// Copy constructor is deep by default
OrthographicTransform::
OrthographicTransform( const OrthographicTransform & map, const CopyType copyType )
{
  OrthographicTransform::className="OrthographicTransform";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "OrthographicTransform:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

OrthographicTransform::
~OrthographicTransform()
{ if( debug & 4 )
  cout << " OrthographicTransform::Desctructor called" << endl;
}

OrthographicTransform & OrthographicTransform::
operator=( const OrthographicTransform & X )
{
  if( OrthographicTransform::className != X.getClassName() )
  {
    cout << "OrthographicTransform::operator= ERROR trying to set a OrthographicTransform = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  sa=X.sa;
  sb=X.sb;
  pole=X.pole; 
  tAxis=X.tAxis;
  return *this;
}

int OrthographicTransform::
setAngularAxis( const int & tAxis_ )
//===========================================================================
/// \brief  
///     Specify which axis (axis1 or axis2) corresponds to the angular ($\theta$) direction
///    of the mapping that will have an orthographic patch on it. The $\phi$ direction will
///    be axis1 if tAxis=axis2 or axis2 if tAxis=axis1.
/// \param tAxis_ (input) : axis1 (0) or axis2 (1).
//===========================================================================
{
  if( tAxis_!=0 && tAxis_!=1 )
  {
    printf("OrthographicTransform::setAngularAxis:ERROR: tAxis_ must be 0 or 1! \n");
    return 1;
  }
  tAxis=tAxis_;
  return 0;
}

int OrthographicTransform::
setPole( const Pole & pole_ )
//===========================================================================
/// \brief  
///     Specify which pole to reparameterize.
/// \param pole (input) : reparameterize the {\tt northPole} or the {\tt southPole}. 
//===========================================================================
{
  pole=(int)pole_;
  return 0;
}


int OrthographicTransform::
setSize( const real & sa_, 
         const real & sb_ )
//===========================================================================
/// \brief  
///    Specify the size of the orthographic patch.
/// \param sa_, sb_ (input) : parameters that specify the dimensions of the plane that is projected
///    onto the sphere in the orthographic transform.
//===========================================================================
{
  sa=sa_;
  sb=sb_;
  return 0;
}


void OrthographicTransform::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  real PiInverse = 1./Pi;
  real twoPiInverse = 1./twoPi;
  const int sAxis = 1-tAxis;

  // *** add back *** if( s.getBase(0)>base || s.getBound(0)<bound )
  realArray s(I,2);

  s(I,axis1)=(r(I,axis1)-.5)*sa;
  s(I,axis2)=(r(I,axis2)-.5)*sb;

  const realArray & s1=s(I,axis1);
  const realArray & s2=s(I,axis2);
  const realArray & sNormSquared = evaluate( min(1.e10,SQR(s1)+SQR(s2)) ); // 1.e10 : prevent overflows

  #ifdef USE_PPP 
    const realSerialArray & xLocal =x.getLocalArray();
    const realSerialArray & xrLocal=xr.getLocalArray();
    const realSerialArray & sLocal =s.getLocalArray();
    const realSerialArray & sNormSquaredLocal=sNormSquared.getLocalArray();

  #else
    const realSerialArray & xLocal =x;
    const realSerialArray & xrLocal=xr;
    const realSerialArray & sLocal =s;
    const realSerialArray & sNormSquaredLocal=sNormSquared;
  #endif

  real * xp = xLocal.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=xLocal.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
  real * xrp = xrLocal.Array_Descriptor.Array_View_Pointer2;
  const int xrDim0=xrLocal.getRawDataSize(0);
  const int xrDim1=xrLocal.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]

  real * sp = sLocal.Array_Descriptor.Array_View_Pointer1;
  const int sDim0=sLocal.getRawDataSize(0);
#undef S
#define S(i0,i1) sp[i0+sDim0*(i1)]

#undef S1
#define S1(i0) S(i0,0)
#undef S2
#define S2(i0) S(i0,1)

  const bool useOpt=true;

  const int baseLocal = max(base,sLocal.getBase(0));
  const int boundLocal = min(bound,sLocal.getBound(0));

  if( computeMap )
  { 
    if( params.coordinateType==cartesian || params.coordinateType==spherical )
    {
      x(I,sAxis)=acos(evaluate( pole*(1.-sNormSquared)/(1.+sNormSquared) ))  *PiInverse;   // phi/Pi
    }
    else
    {  // cylindrical x_1 = +- (1-s_1^2-s_2^2)/(1+s_1^2+s_2^2)
      x(I,sAxis)= (.5*pole)*(1.-sNormSquared)/(1.+sNormSquared) +.5;
    }
    if( useOpt ) // avoid where for P++
    {
      for( int i=baseLocal; i<=boundLocal; i++ )
      {
	if( fabs(S1(i))+fabs(S2(i)) >  REAL_EPSILON )
	{ // compute theta, map onto [0,2pi] instead of [-pi,pi] from atan2
	  X(i,tAxis)=fmod( ( atan2((double)pole*S2(i),(double)S1(i)) +twoPi)*twoPiInverse ,1.);
	}
	else
	{
	  X(i,tAxis)=0.;
	}
      }
    }
    else
    {
      where( fabs(s1)+fabs(s2) >  REAL_EPSILON )
      { // compute theta, map onto [0,2pi] instead of [-pi,pi] from atan2
	x(I,tAxis)=fmod(
	  evaluate( (atan2(evaluate(pole*s2),evaluate(s1))+twoPi)*twoPiInverse ),1.);
      }
      otherwise()
      {
	x(I,tAxis)=0.;
      }
    }
    
    if( domainDimension==3 )
      x(I,axis3)=r(I,axis3);
  }
  if( computeMapDerivative )
  {
    // *** add back *** if( a.getBase(0)>base || a.getBound(0)<bound )
  real * sNormSquaredp = sNormSquaredLocal.Array_Descriptor.Array_View_Pointer0;
#undef SNORMSQUARED
#define SNORMSQUARED(i0) sNormSquaredp[i0]


    if( params.coordinateType==cartesian )
    {
      if( useOpt )
      {
	for( int i=baseLocal; i<=boundLocal; i++ )
	{
          real a = SNORMSQUARED(i);
	  if( a==0. ) a=1;
	  const real denom = 1./( (1.+a )*SQR(a)) ;
	  XR(i,sAxis,axis1)=S1(i)*denom*(2.*sa*PiInverse*pole);
	  XR(i,sAxis,axis2)=S2(i)*denom*(2.*sb*PiInverse*pole);

	  a=1./a;
	  XR(i,tAxis,axis1)=-S2(i)*a*(sa*twoPiInverse*pole);
	  XR(i,tAxis,axis2)=+S1(i)*a*(sa*twoPiInverse*pole);
	  
	}
      }
      else
      {
	realArray a(I);
	a=sNormSquared;
	where( a==0. )
	{
	  a=1.;
	}
	const realArray & denom = evaluate( 1./( (1.+sNormSquared)*SQR(a)) );
	xr(I,sAxis,axis1)=s1*denom*(2.*sa*PiInverse*pole);
	xr(I,sAxis,axis2)=s2*denom*(2.*sb*PiInverse*pole);

	a=1./a;
	xr(I,tAxis,axis1)=-s2*a*(sa*twoPiInverse*pole);
	xr(I,tAxis,axis2)=+s1*a*(sa*twoPiInverse*pole);
      }
      
    }
    else if( params.coordinateType==spherical ||  params.coordinateType==cylindrical )
    { // The spherical and cylindrical cases are the same except for multiplicative constants
      real c11,c12,c21,c22;
      if( params.coordinateType==spherical )
      {
	// return sin(phi)*d(x(axis2)/d(*)  *********
	//  watch out when s(axis1,I) and s(axis2,I) both are near zero
	//  Note that the eventual result is independent of s1/s2 as s1 && s2 -> 0
        c11=2.*sa*PiInverse*pole;
        c12=2.*sb*PiInverse*pole;
        c21=-.5*sa/sb;
	c22=+.5*sb/sa;
      }
      else
      { // return (-1./r)*d(x(axis1)/d(*)   r=2s/(1+s_1^2+s_2^2)
        c11=pole*sa;
	c12=pole*sb;
        c21=-PiInverse*sa/sb; 
        c22=+PiInverse*sb/sa;
      }

      if( useOpt )
      {
	for( int i=baseLocal; i<=boundLocal; i++ )
	{
          real a = SNORMSQUARED(i);
	  
	  if( fabs(a) > REAL_EPSILON )
	  {
	    a=1./( (1.+a)*sqrt(a) );
	    XR(i,sAxis,axis1)=S1(i)*a*c11;
	    XR(i,sAxis,axis2)=S2(i)*a*c12;
	  }
	  else if(  fabs(S1(i))+fabs(S2(i)) <=  REAL_EPSILON )
	  { // take theta=0 when s1=s2=0 since this is what we choose above for x(I,axis2)
	    // ***this is required in order to get the correct answers ****
	    XR(i,sAxis,axis1)=c11;
	    XR(i,sAxis,axis2)=0.;
	  }
	  else if( fabs(S1(i)) < fabs(S2(i)) )
	  { // ignore the term 1./(1+sigma)    .. sigma = s1^2+s2^2
	    // *** it is not clear that we need to be so careful in these cases ??
	    a=S1(i)/S2(i);
	    const real denom = 1./sqrt(1.+SQR(a));
	    XR(i,sAxis,axis1)= a*denom*c11;
	    XR(i,sAxis,axis2)= denom*c12;
	
	  }
	  else if(  fabs(S2(i)) < fabs(S1(i)) )
	  {
	    a=S2(i)/S1(i);
	    const real denom = 1./sqrt(1.+SQR(a));
	    XR(i,sAxis,axis1)=denom*c11;
	    XR(i,sAxis,axis2)=a*denom*c12;
	  }
	  else
	  { 
	    XR(i,sAxis,axis1)=c11/sqrt(2.);
	    XR(i,sAxis,axis2)=c12/sqrt(2.);
	  }
	  XR(i,tAxis,axis1)=XR(i,sAxis,axis2)*c21;
	  XR(i,tAxis,axis2)=XR(i,sAxis,axis1)*c22;
	}
      
      }
      else // old way
      {
        realArray a(I);
        a=sNormSquared;
	where( fabs(a) > REAL_EPSILON )
	{
	  a=1./( (1.+sNormSquared)*SQRT(a) );
	  xr(I,sAxis,axis1)=s1*a*c11;
	  xr(I,sAxis,axis2)=s2*a*c12;
	}
	elsewhere(  fabs(s1)+fabs(s2) <=  REAL_EPSILON )
	{ // take theta=0 when s1=s2=0 since this is what we choose above for x(I,axis2)
	  // ***this is required in order to get the correct answers ****
	  xr(I,sAxis,axis1)=c11;
	  xr(I,sAxis,axis2)=0.;
	}
	elsewhere( fabs(s1) < fabs(s2) )
	{ // ignore the term 1./(1+sigma)    .. sigma = s1^2+s2^2
	  // *** it is not clear that we need to be so careful in these cases ??
	  a=s1/s2;
	  const realArray & denom = evaluate( 1./SQRT(evaluate(1.+SQR(a))) );
	  xr(I,sAxis,axis1)= a*denom*c11;
	  xr(I,sAxis,axis2)= denom*c12;
	
	}
	elsewhere(  fabs(s2) < fabs(s1) )
	{
	  a=s2/s1;
	  const realArray & denom = evaluate( 1./SQRT(evaluate(1.+SQR(a))) );
	  xr(I,sAxis,axis1)=denom*c11;
	  xr(I,sAxis,axis2)=a*denom*c12;
	}
	otherwise()
	{ 
	  xr(I,sAxis,axis1)=c11/SQRT(2.);
	  xr(I,sAxis,axis2)=c12/SQRT(2.);
	}
	xr(I,tAxis,axis1)=xr(I,sAxis,axis2)*c21;
	xr(I,tAxis,axis2)=xr(I,sAxis,axis1)*c22;
      }
      
    }
    else
    {
      cout << "OrthographicTransform::map::ERROR unknown coordinateType = " << params.coordinateType << endl;
      return;
    }
    if( domainDimension==3 )
    {
      xr(I,sAxis,axis3)=0.;
      xr(I,tAxis,axis3)=0.;

      xr(I,axis3,axis1)=0.;
      xr(I,axis3,axis2)=0.;
      xr(I,axis3,axis3)=1.;
    }
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void OrthographicTransform::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  real PiInverse = 1./Pi;
  // real twoPiInverse = 1./twoPi;
  real eps=SQRT(REAL_MIN*100.);  // make sqrt as map will take the square
  real epsInverse  = .99/eps;
  const int sAxis = 1-tAxis;

  // *** add back ** if( s.getBase(0)>base || s.getBound(0)<bound )
  // ** s.redim(Range(base,bound),Range(0,1));
  realArray sI(I);

  const realArray & phi = evaluate( Pi*x(I,sAxis) );
  realArray sinp;
  if( params.coordinateType==cartesian || params.coordinateType==spherical )
    sinp=sin(phi);

  const realArray & theta = evaluate( twoPi*x(I,tAxis) );
  const realArray & cost  = evaluate( cos(theta) );
  const realArray & sint  = evaluate( sin(theta) );
  

  if( params.coordinateType==cartesian )
    sI=1./max(eps,1.+pole*cos(phi));
  else if( params.coordinateType==spherical )
    sI=1./max(eps,1.+pole*cos(phi));
  else if( params.coordinateType==cylindrical )
    sI=1./max(eps,1.+pole*(2.*x(I,sAxis)-1.));
  else
  {
    cout << "OrthographicTransform::map::ERROR unknown coordinateType = " << params.coordinateType << endl;
    return;
  }

  // ** s(I,axis2)=theta;

  if( computeMapDerivative )
  {
    if( params.coordinateType==cartesian )
    {
      rx(I,axis1,tAxis)=sI*sinp*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*sinp*cost*(+twoPi*pole/sb);
      rx(I,axis1,sAxis)=sI*cost*(pole*Pi/sa);
      rx(I,axis2,sAxis)=sI*sint*(     Pi/sb);

    }
    else if( params.coordinateType==spherical )
    { // return (1/sinp * d()/d(tAxis)
      rx(I,axis1,tAxis)=sI*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*cost*(+twoPi*pole/sb);

      rx(I,axis1,sAxis)=rx(I,axis2,tAxis)*( .5*sb/sa); //    sI*cost*(Pi*pole/sa);
      rx(I,axis2,sAxis)=rx(I,axis1,tAxis)*(-.5*sa/sb); // sI*sint*(Pi/sb);
    }
    else if( params.coordinateType==cylindrical )
    {  // return -r*d()/dt_0  (1/r)*d()/dt_1

      rx(I,axis1,tAxis)=sI*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*cost*(+twoPi*pole/sb);

      rx(I,axis1,sAxis)=rx(I,axis2,tAxis)*( PiInverse*sb/sa);
      rx(I,axis2,sAxis)=rx(I,axis1,tAxis)*(-PiInverse*sa/sb);
    }

    if( domainDimension==3 )
    {
      rx(I,axis1,axis3)=0.;
      rx(I,axis2,axis3)=0.;

      rx(I,axis3,sAxis)=0.;
      rx(I,axis3,tAxis)=0.;
      rx(I,axis3,axis3)=1.;
    }
  }
  if( computeMap )
  { 
    if( params.coordinateType==cartesian )
    {
      where( sI < epsInverse )  
        sI*=sinp;  // if s is large do not accidently make it small again
    }
    else if( params.coordinateType==spherical )
    {
      where( sI < epsInverse )
	sI*=sinp;
    }
    else if( params.coordinateType==cylindrical )
    {
      where( sI < epsInverse )
	sI*=SQRT(1.-min(1.,SQR(2.*x(I,sAxis)-1.)));
      // *wdh* 991120 sI*=SQRT(1.-SQR(2.*x(I,sAxis)-1.));
    }
    r(I,axis1)=sI*cost*(1./sa)  +.5;
    r(I,axis2)=sI*sint*(pole/sb)+.5;
    if( domainDimension==3 )
      r(I,axis3)=x(I,axis3);
  }
}
  


void OrthographicTransform::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  real PiInverse = 1./Pi;
  real twoPiInverse = 1./twoPi;
  const int sAxis = 1-tAxis;

  // *** add back *** if( s.getBase(0)>base || s.getBound(0)<bound )
  RealArray s(I,2);

  s(I,axis1)=(r(I,axis1)-.5)*sa;
  s(I,axis2)=(r(I,axis2)-.5)*sb;

  const RealArray & s1=s(I,axis1);
  const RealArray & s2=s(I,axis2);
  const RealArray & sNormSquared = evaluate( min(1.e10,SQR(s1)+SQR(s2)) ); // 1.e10 : prevent overflows

  const realSerialArray & xLocal =x;
  const realSerialArray & xrLocal=xr;
  const realSerialArray & sLocal =s;
  const realSerialArray & sNormSquaredLocal=sNormSquared;

  real * xp = xLocal.Array_Descriptor.Array_View_Pointer1;
  const int xDim0=xLocal.getRawDataSize(0);
#undef X
#define X(i0,i1) xp[i0+xDim0*(i1)]
  real * xrp = xrLocal.Array_Descriptor.Array_View_Pointer2;
  const int xrDim0=xrLocal.getRawDataSize(0);
  const int xrDim1=xrLocal.getRawDataSize(1);
#undef XR
#define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]

  real * sp = sLocal.Array_Descriptor.Array_View_Pointer1;
  const int sDim0=sLocal.getRawDataSize(0);
#undef S
#define S(i0,i1) sp[i0+sDim0*(i1)]

#undef S1
#define S1(i0) S(i0,0)
#undef S2
#define S2(i0) S(i0,1)

  const bool useOpt=true;

  const int baseLocal = max(base,sLocal.getBase(0));
  const int boundLocal = min(bound,sLocal.getBound(0));

  if( computeMap )
  { 
    if( params.coordinateType==cartesian || params.coordinateType==spherical )
    {
      x(I,sAxis)=acos(evaluate( pole*(1.-sNormSquared)/(1.+sNormSquared) ))  *PiInverse;   // phi/Pi
    }
    else
    {  // cylindrical x_1 = +- (1-s_1^2-s_2^2)/(1+s_1^2+s_2^2)
      x(I,sAxis)= (.5*pole)*(1.-sNormSquared)/(1.+sNormSquared) +.5;
    }
    if( useOpt ) // avoid where for P++
    {
      for( int i=baseLocal; i<=boundLocal; i++ )
      {
	if( fabs(S1(i))+fabs(S2(i)) >  REAL_EPSILON )
	{ // compute theta, map onto [0,2pi] instead of [-pi,pi] from atan2
	  X(i,tAxis)=fmod( ( atan2((double)pole*S2(i),(double)S1(i)) +twoPi)*twoPiInverse ,1.);
	}
	else
	{
	  X(i,tAxis)=0.;
	}
      }
    }
    else
    {
      where( fabs(s1)+fabs(s2) >  REAL_EPSILON )
      { // compute theta, map onto [0,2pi] instead of [-pi,pi] from atan2
	x(I,tAxis)=fmod(
	  evaluate( (atan2(evaluate(pole*s2),evaluate(s1))+twoPi)*twoPiInverse ),1.);
      }
      otherwise()
      {
	x(I,tAxis)=0.;
      }
    }
    
    if( domainDimension==3 )
      x(I,axis3)=r(I,axis3);
  }
  if( computeMapDerivative )
  {
    // *** add back *** if( a.getBase(0)>base || a.getBound(0)<bound )
  real * sNormSquaredp = sNormSquaredLocal.Array_Descriptor.Array_View_Pointer0;
#undef SNORMSQUARED
#define SNORMSQUARED(i0) sNormSquaredp[i0]


    if( params.coordinateType==cartesian )
    {
      if( useOpt )
      {
	for( int i=baseLocal; i<=boundLocal; i++ )
	{
          real a = SNORMSQUARED(i);
	  if( a==0. ) a=1;
	  const real denom = 1./( (1.+a )*SQR(a)) ;
	  XR(i,sAxis,axis1)=S1(i)*denom*(2.*sa*PiInverse*pole);
	  XR(i,sAxis,axis2)=S2(i)*denom*(2.*sb*PiInverse*pole);

	  a=1./a;
	  XR(i,tAxis,axis1)=-S2(i)*a*(sa*twoPiInverse*pole);
	  XR(i,tAxis,axis2)=+S1(i)*a*(sa*twoPiInverse*pole);
	  
	}
      }
      else
      {
	RealArray a(I);
	a=sNormSquared;
	where( a==0. )
	{
	  a=1.;
	}
	const RealArray & denom = evaluate( 1./( (1.+sNormSquared)*SQR(a)) );
	xr(I,sAxis,axis1)=s1*denom*(2.*sa*PiInverse*pole);
	xr(I,sAxis,axis2)=s2*denom*(2.*sb*PiInverse*pole);

	a=1./a;
	xr(I,tAxis,axis1)=-s2*a*(sa*twoPiInverse*pole);
	xr(I,tAxis,axis2)=+s1*a*(sa*twoPiInverse*pole);
      }
      
    }
    else if( params.coordinateType==spherical ||  params.coordinateType==cylindrical )
    { // The spherical and cylindrical cases are the same except for multiplicative constants
      real c11,c12,c21,c22;
      if( params.coordinateType==spherical )
      {
	// return sin(phi)*d(x(axis2)/d(*)  *********
	//  watch out when s(axis1,I) and s(axis2,I) both are near zero
	//  Note that the eventual result is independent of s1/s2 as s1 && s2 -> 0
        c11=2.*sa*PiInverse*pole;
        c12=2.*sb*PiInverse*pole;
        c21=-.5*sa/sb;
	c22=+.5*sb/sa;
      }
      else
      { // return (-1./r)*d(x(axis1)/d(*)   r=2s/(1+s_1^2+s_2^2)
        c11=pole*sa;
	c12=pole*sb;
        c21=-PiInverse*sa/sb; 
        c22=+PiInverse*sb/sa;
      }

      if( useOpt )
      {
	for( int i=baseLocal; i<=boundLocal; i++ )
	{
          real a = SNORMSQUARED(i);
	  
	  if( fabs(a) > REAL_EPSILON )
	  {
	    a=1./( (1.+a)*sqrt(a) );
	    XR(i,sAxis,axis1)=S1(i)*a*c11;
	    XR(i,sAxis,axis2)=S2(i)*a*c12;
	  }
	  else if(  fabs(S1(i))+fabs(S2(i)) <=  REAL_EPSILON )
	  { // take theta=0 when s1=s2=0 since this is what we choose above for x(I,axis2)
	    // ***this is required in order to get the correct answers ****
	    XR(i,sAxis,axis1)=c11;
	    XR(i,sAxis,axis2)=0.;
	  }
	  else if( fabs(S1(i)) < fabs(S2(i)) )
	  { // ignore the term 1./(1+sigma)    .. sigma = s1^2+s2^2
	    // *** it is not clear that we need to be so careful in these cases ??
	    a=S1(i)/S2(i);
	    const real denom = 1./sqrt(1.+SQR(a));
	    XR(i,sAxis,axis1)= a*denom*c11;
	    XR(i,sAxis,axis2)= denom*c12;
	
	  }
	  else if(  fabs(S2(i)) < fabs(S1(i)) )
	  {
	    a=S2(i)/S1(i);
	    const real denom = 1./sqrt(1.+SQR(a));
	    XR(i,sAxis,axis1)=denom*c11;
	    XR(i,sAxis,axis2)=a*denom*c12;
	  }
	  else
	  { 
	    XR(i,sAxis,axis1)=c11/sqrt(2.);
	    XR(i,sAxis,axis2)=c12/sqrt(2.);
	  }
	  XR(i,tAxis,axis1)=XR(i,sAxis,axis2)*c21;
	  XR(i,tAxis,axis2)=XR(i,sAxis,axis1)*c22;
	}
      
      }
      else // old way
      {
        RealArray a(I);
        a=sNormSquared;
	where( fabs(a) > REAL_EPSILON )
	{
	  a=1./( (1.+sNormSquared)*SQRT(a) );
	  xr(I,sAxis,axis1)=s1*a*c11;
	  xr(I,sAxis,axis2)=s2*a*c12;
	}
	elsewhere(  fabs(s1)+fabs(s2) <=  REAL_EPSILON )
	{ // take theta=0 when s1=s2=0 since this is what we choose above for x(I,axis2)
	  // ***this is required in order to get the correct answers ****
	  xr(I,sAxis,axis1)=c11;
	  xr(I,sAxis,axis2)=0.;
	}
	elsewhere( fabs(s1) < fabs(s2) )
	{ // ignore the term 1./(1+sigma)    .. sigma = s1^2+s2^2
	  // *** it is not clear that we need to be so careful in these cases ??
	  a=s1/s2;
	  const RealArray & denom = evaluate( 1./SQRT(evaluate(1.+SQR(a))) );
	  xr(I,sAxis,axis1)= a*denom*c11;
	  xr(I,sAxis,axis2)= denom*c12;
	
	}
	elsewhere(  fabs(s2) < fabs(s1) )
	{
	  a=s2/s1;
	  const RealArray & denom = evaluate( 1./SQRT(evaluate(1.+SQR(a))) );
	  xr(I,sAxis,axis1)=denom*c11;
	  xr(I,sAxis,axis2)=a*denom*c12;
	}
	otherwise()
	{ 
	  xr(I,sAxis,axis1)=c11/SQRT(2.);
	  xr(I,sAxis,axis2)=c12/SQRT(2.);
	}
	xr(I,tAxis,axis1)=xr(I,sAxis,axis2)*c21;
	xr(I,tAxis,axis2)=xr(I,sAxis,axis1)*c22;
      }
      
    }
    else
    {
      cout << "OrthographicTransform::map::ERROR unknown coordinateType = " << params.coordinateType << endl;
      return;
    }
    if( domainDimension==3 )
    {
      xr(I,sAxis,axis3)=0.;
      xr(I,tAxis,axis3)=0.;

      xr(I,axis3,axis1)=0.;
      xr(I,axis3,axis2)=0.;
      xr(I,axis3,axis3)=1.;
    }
  }
}

//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//=================================================================================
void OrthographicTransform::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  real PiInverse = 1./Pi;
  // real twoPiInverse = 1./twoPi;
  real eps=SQRT(REAL_MIN*100.);  // make sqrt as map will take the square
  real epsInverse  = .99/eps;
  const int sAxis = 1-tAxis;

  // *** add back ** if( s.getBase(0)>base || s.getBound(0)<bound )
  // ** s.redim(Range(base,bound),Range(0,1));
  RealArray sI(I);

  const RealArray & phi = evaluate( Pi*x(I,sAxis) );
  RealArray sinp;
  if( params.coordinateType==cartesian || params.coordinateType==spherical )
    sinp=sin(phi);

  const RealArray & theta = evaluate( twoPi*x(I,tAxis) );
  const RealArray & cost  = evaluate( cos(theta) );
  const RealArray & sint  = evaluate( sin(theta) );
  

  if( params.coordinateType==cartesian )
    sI=1./max(eps,1.+pole*cos(phi));
  else if( params.coordinateType==spherical )
    sI=1./max(eps,1.+pole*cos(phi));
  else if( params.coordinateType==cylindrical )
    sI=1./max(eps,1.+pole*(2.*x(I,sAxis)-1.));
  else
  {
    cout << "OrthographicTransform::map::ERROR unknown coordinateType = " << params.coordinateType << endl;
    return;
  }

  // ** s(I,axis2)=theta;

  if( computeMapDerivative )
  {
    if( params.coordinateType==cartesian )
    {
      rx(I,axis1,tAxis)=sI*sinp*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*sinp*cost*(+twoPi*pole/sb);
      rx(I,axis1,sAxis)=sI*cost*(pole*Pi/sa);
      rx(I,axis2,sAxis)=sI*sint*(     Pi/sb);

    }
    else if( params.coordinateType==spherical )
    { // return (1/sinp * d()/d(tAxis)
      rx(I,axis1,tAxis)=sI*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*cost*(+twoPi*pole/sb);

      rx(I,axis1,sAxis)=rx(I,axis2,tAxis)*( .5*sb/sa); //    sI*cost*(Pi*pole/sa);
      rx(I,axis2,sAxis)=rx(I,axis1,tAxis)*(-.5*sa/sb); // sI*sint*(Pi/sb);
    }
    else if( params.coordinateType==cylindrical )
    {  // return -r*d()/dt_0  (1/r)*d()/dt_1

      rx(I,axis1,tAxis)=sI*sint*(-twoPi/sa);   
      rx(I,axis2,tAxis)=sI*cost*(+twoPi*pole/sb);

      rx(I,axis1,sAxis)=rx(I,axis2,tAxis)*( PiInverse*sb/sa);
      rx(I,axis2,sAxis)=rx(I,axis1,tAxis)*(-PiInverse*sa/sb);
    }

    if( domainDimension==3 )
    {
      rx(I,axis1,axis3)=0.;
      rx(I,axis2,axis3)=0.;

      rx(I,axis3,sAxis)=0.;
      rx(I,axis3,tAxis)=0.;
      rx(I,axis3,axis3)=1.;
    }
  }
  if( computeMap )
  { 
    if( params.coordinateType==cartesian )
    {
      where( sI < epsInverse )  
        sI*=sinp;  // if s is large do not accidently make it small again
    }
    else if( params.coordinateType==spherical )
    {
      where( sI < epsInverse )
	sI*=sinp;
    }
    else if( params.coordinateType==cylindrical )
    {
      where( sI < epsInverse )
	sI*=SQRT(1.-min(1.,SQR(2.*x(I,sAxis)-1.)));
      // *wdh* 991120 sI*=SQRT(1.-SQR(2.*x(I,sAxis)-1.));
    }
    r(I,axis1)=sI*cost*(1./sa)  +.5;
    r(I,axis2)=sI*sint*(pole/sb)+.5;
    if( domainDimension==3 )
      r(I,axis3)=x(I,axis3);
  }
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int OrthographicTransform::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering OrthographicTransform::get" << endl;

  subDir.get( OrthographicTransform::className,"className" ); 
  if( OrthographicTransform::className != "OrthographicTransform" )
  {
    cout << "OrthographicTransform::get ERROR in className!" << endl;
  }
  subDir.get( sa,"sa" );
  subDir.get( sb,"sb" );
  subDir.get( pole,"pole" );
  subDir.get( tAxis,"tAxis" );
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}
int OrthographicTransform::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( OrthographicTransform::className,"className" );
  subDir.put( sa,"sa" );            
  subDir.put( sb,"sb" );
  subDir.put( pole,"pole" );
  subDir.put( tAxis,"tAxis" );
  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *OrthographicTransform::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==OrthographicTransform::className )
    retval = new OrthographicTransform();
  return retval;
}

    

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int OrthographicTransform::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!OrthographicTransform",
      "specify sa,sb",
      "choose north or south pole",
      "angular axis = axis1",
      "angular axis = axis2",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "specify sa,sb      : ",
      "choose north or south pole: reparameterize which pole",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  parameters.set(GI_TOP_LABEL,"Orthographic Mapping");

  gi.appendToTheDefaultPrompt("Orthographic>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="specify sa,sb" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter sa,sb (default=(%e,%e)): ",
          sa,sb));
      if( line!="" ) sScanF(line,"%e %e",&sa,&sb);
      mappingHasChanged();
    }
    else if( answer=="choose north or south pole" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter pole, +1=north, -1=south (default=(%i)): ",pole));
      if( line!="" ) sScanF(line,"%i",&pole);
      if( pole!=+1 && pole!=-1 )
      {
	cout << "Error, pole must be +1 or -1, setting to +1=north pole\n";
	pole=1;
      }
      mappingHasChanged();
    }
    else if( answer=="angular axis = axis1" )
    {
      tAxis=axis1;
    }
    else if( answer=="angular axis = axis2" )
    {
      tAxis=axis2;
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" ||
             answer=="check" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }

    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}
