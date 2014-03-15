#include "SphereMapping.h"
#include "MappingInformation.h"
#include <float.h>
#include "GenericDataBase.h"


SphereMapping::
SphereMapping(const real & innerRadius_ /* .5 */, 
	      const real & outerRadius_ /* 1. */, 
	      const real & x0_ /* .0 */, 
	      const real & y0_ /* .0 */, 
	      const real & z0_ /* .0 */,
              const real & startTheta_ /* .0 */,
              const real & endTheta_ /* 1. */,
              const real & startPhi_ /* .0 */,
              const real & endPhi_ /* 1. */)
: Mapping(3,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  
///      Define a spherical shell or spherical surface.
/// \param innerRadius_,outerRadius_ (input): bounds on the radius.
/// \param x0_,y0_,z0_ (input) : center.
/// \param startTheta_,endTheta_ (input) : bounds on normalized $\theta$, in the range $[0,1]$.
/// \param startPhi_,endPhi_ (input): bounds on the normalized $\phi$, in the range $[0,1]$.
//===========================================================================
{ 
  SphereMapping::className="SphereMapping";

  setName( Mapping::mappingName,"sphere");
  setName(Mapping::domainAxis1Name,"phi");
  setName(Mapping::domainAxis2Name,"theta");
  setName(Mapping::domainAxis3Name,"radius");

  setGridDimensions( axis1,15 );
  setGridDimensions( axis2,15 );
  setGridDimensions( axis3,5 );
  innerRadius=innerRadius_;
  outerRadius=outerRadius_;
  x0=x0_;
  y0=y0_;
  z0=z0_;
  startTheta=startTheta_;
  endTheta=endTheta_;
  startPhi=startPhi_;
  endPhi=endPhi_;


  setBasicInverseOption(canInvert);
  inverseIsDistributed=false;

  setBoundaryCondition(Start,0,0);  // phi  -- singular side , by default interpolatio
  setBoundaryCondition(End  ,0,0);
  setBoundaryCondition(Start,2,1);  // r : inner shell
  setBoundaryCondition(End  ,2,0);  // r : outer shell, interpolation
  
  if( fabs(endTheta-startTheta-1.)<REAL_EPSILON*10. ) 
  {
    setIsPeriodic(axis2,functionPeriodic );  
    setBoundaryCondition( Start,axis2,-1 );
    setBoundaryCondition(   End,axis2,-1 );
  }	
  else
  {
    setIsPeriodic(axis2,notPeriodic );  
    if( getBoundaryCondition(Start,axis2)<0 )
    {
      setBoundaryCondition(Start,axis2,0);
      setBoundaryCondition(  End,axis2,0);
    }
  }
  if( startPhi==0. )
    setTypeOfCoordinateSingularity( Start,axis1,polarSingularity ); // phi has a "polar" singularity
  if( endPhi==1. )
    setTypeOfCoordinateSingularity( End  ,axis1,polarSingularity ); // at both ends
  setCoordinateEvaluationType( spherical,TRUE );  // Mapping can be evaluated in spherical coordinates
                                                  // (in addition to cartesian)  
  mappingHasChanged();
}

// Copy constructor is deep by default
SphereMapping::
SphereMapping( const SphereMapping & map, const CopyType copyType )
{
  SphereMapping::className="SphereMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "SphereMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

SphereMapping::
~SphereMapping()
{ if( debug & 4 )
  cout << " SphereMapping::Destructor called" << endl;
}

    
SphereMapping & SphereMapping::
operator =( const SphereMapping & X )
{
  if( SphereMapping::className != X.getClassName() )
  {
    cout << "SphereMapping::operator= ERROR trying to set a SphereMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  x0=X.x0;
  y0=X.y0;
  z0=X.z0;
  innerRadius=X.innerRadius;
  outerRadius=X.outerRadius;
  startTheta=X.startTheta;
  endTheta=X.endTheta;
  startPhi=X.startPhi;
  endPhi=X.endPhi;
  
  return *this;
}


int SphereMapping::
setOrigin(const real & x0_ /* =.0 */, 
	  const real & y0_ /* =.0 */, 
	  const real & z0_ /* =.0 */)
//===========================================================================
/// \details  
///     Specify parameters for the sphere.
/// \param x0_,y0_,z0_ (input) : center.
//===========================================================================
{ 
  x0=x0_;
  y0=y0_;
  z0=z0_;
  return 0;
}
int SphereMapping::
setPhi(const real & startPhi_ /* =.0 */,
       const real & endPhi_   /* =1. */)
//===========================================================================
/// \details  
///     Specify parameters for the sphere.
/// \param startPhi_,endPhi_ (input): bounds on the normalized $\phi$, in the range $[0,1]$.
//===========================================================================
{ 
  startPhi=startPhi_;
  endPhi=endPhi_;
  return 0;
}

int SphereMapping::
setRadii(const real & innerRadius_ /* =.5 */, 
	 const real & outerRadius_ /* =1. */)
//===========================================================================
/// \details  
///     Specify parameters for the sphere.
/// \param innerRadius_,outerRadius_ (input): bounds on the radius.
//===========================================================================
{ 
  innerRadius=innerRadius_;
  outerRadius=outerRadius_;
  return 0;
}

int SphereMapping::
setTheta( const real & startTheta_ /* =.0 */,
	  const real & endTheta_   /* =1. */)
//===========================================================================
/// \details  
///     Specify parameters for the sphere.
/// \param startTheta_,endTheta_ (input) : bounds on normalized $\theta$, in the range $[0,1]$.
//===========================================================================
{ 
  startTheta=startTheta_;
  endTheta=endTheta_;
  return 0;
}

#define RADIUS(x) (rad*(x)+innerRadius)
void SphereMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  real rad=outerRadius-innerRadius;
  real phiFactor=Pi*(endPhi-startPhi);
  real thetaFactor=twoPi*(endTheta-startTheta);

  realArray sinPhi(I), cosPhi(I), cosTheta(I),sinTheta(I);    
  sinPhi=sin(phiFactor*r(I,axis1)+startPhi*Pi);
  cosPhi=cos(phiFactor*r(I,axis1)+startPhi*Pi);
  sinTheta=sin(thetaFactor*r(I,axis2)+startTheta*twoPi);
  cosTheta=cos(thetaFactor*r(I,axis2)+startTheta*twoPi);

  realArray sinPhiOrtho, sinRatio;
  sinPhiOrtho=sin(Pi*r(I,axis1)); // this is the sinPhi used in the orthographic scaling
  const bool useOpt=true;
  if( useOpt )
  {
    sinRatio.redim(I);
    #ifdef USE_PPP 
     const realSerialArray & sinPhiOrthoLocal =sinPhiOrtho.getLocalArray();
     const realSerialArray & sinRatioLocal=sinRatio.getLocalArray();
     const realSerialArray & sinPhiLocal =sinPhi.getLocalArray();

    #else
     const realSerialArray & sinPhiOrthoLocal =sinPhiOrtho;
     const realSerialArray & sinRatioLocal=sinRatio;
     const realSerialArray & sinPhiLocal =sinPhi;
    #endif

    real * sinPhiOrthop = sinPhiOrthoLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINPHIORTHO(i0) sinPhiOrthop[i0]
    
    real * sinPhip = sinPhiLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINPHI(i0) sinPhip[i0]
    
    real * sinRatiop = sinRatioLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINRATIO(i0) sinRatiop[i0]

    const int baseLocal = max(base,sinPhiLocal.getBase(0));
    const int boundLocal = min(bound,sinPhiLocal.getBound(0));
    
    for( int i=baseLocal; i<=boundLocal; i++ )
    {
      if( SINPHIORTHO(i)>REAL_EPSILON*100. )
      {
	SINRATIO(i)=SINPHI(i)/SINPHIORTHO(i);
      }
      else
      {
	SINRATIO(i)=endPhi-startPhi;
      }
    }
  }
  else
  {
    where( sinPhiOrtho>REAL_EPSILON*100. )
    {
      sinRatio=sinPhi/sinPhiOrtho;
    }
    otherwise()
    {
      sinRatio=endPhi-startPhi;
    }
  }
  

  if( computeMap )
  {
    if( domainDimension==2 )
    {  // spherical surface
      x(I,axis1)=innerRadius*cosTheta*sinPhi+x0; 
      x(I,axis2)=innerRadius*sinTheta*sinPhi+y0;
      x(I,axis3)=innerRadius*cosPhi+z0;
    }
    else
    {
      x(I,axis1)=RADIUS(r(I,axis3))*cosTheta*sinPhi+x0; 
      x(I,axis2)=RADIUS(r(I,axis3))*sinTheta*sinPhi+y0;
      x(I,axis3)=RADIUS(r(I,axis3))*cosPhi+z0;
    }
  }

  if( computeMapDerivative )
  {
    if( domainDimension==2 )
    {  // spherical surface
      xr(I,axis1,axis1)=innerRadius*         cosTheta*phiFactor*cosPhi;
      // xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta   *sinPhi;

      xr(I,axis2,axis1)=innerRadius*sinTheta*phiFactor*cosPhi;
      // xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta*sinPhi;

      xr(I,axis3,axis1)=innerRadius*(-phiFactor)*sinPhi;
      xr(I,axis3,axis2)=0.;

    }
    else
    {
      xr(I,axis1,axis1)=RADIUS(r(I,axis3))*         cosTheta*phiFactor*cosPhi;
      // xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta   *sinPhi;
      xr(I,axis1,axis3)=rad*                    cosTheta   *sinPhi;

      xr(I,axis2,axis1)=RADIUS(r(I,axis3))*sinTheta*phiFactor*cosPhi;
      // xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta*sinPhi;
      xr(I,axis2,axis3)=rad*sinTheta*sinPhi;

      xr(I,axis3,axis1)=RADIUS(r(I,axis3))*(-phiFactor)*sinPhi;
      xr(I,axis3,axis2)=0.;
      xr(I,axis3,axis3)=rad*cosPhi;
    }
    switch (params.coordinateType)
    {
      
    case cartesian:  // mapping returned in cartesian form
      // derivatives: ( d/d(r1), d/d(r2), d/d(r3) )
      if( domainDimension==2 )
      {  // spherical surface
	xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta   *sinPhi;
	xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta*sinPhi;
      }
      else
      {
	xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta   *sinPhi;
	xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta*sinPhi;
      }
      break;

    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d/d(phi), (1/sin(phi))d/d(theta), d/d(r) )

      if( domainDimension==2 )
      {  // spherical surface
	xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta*sinRatio;
	xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta   *sinRatio;
      }
      else
      {
	xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta*sinRatio;
	xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta   *sinRatio;
      }
      break;
    default:
      cerr << "Sphere::map: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      exit(1);
    }
  }
}
#undef RADIUS

//======================================================================
// Here is the inverse for the Sphere mapping
//======================================================================
void SphereMapping::
basicInverse(const realArray & x, realArray & r, realArray & rx, MappingParameters & params)
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  real inverseScale=1./(twoPi*(endTheta-startTheta));
  real rad=outerRadius-innerRadius;
  real inverseRad=1./rad;

  realArray radius(I);
  real phiFactor = Pi*(endPhi-startPhi);
  real inversePhiFactor = 1./phiFactor;
  real eps = REAL_MIN*100.;

  if( computeMap || computeMapDerivative )
  {
    // This next line was 4000 times slower than using sqrt() ! on 64bit tux231 gcc 3.4.4 
    //  radius=pow(pow(x(I,axis1)-x0,2)+pow(x(I,axis2)-y0,2)+pow(x(I,axis3)-z0,2),.5);  // VERY slow tux231 gcc 3.4.4
    // radius=sqrt(pow(x(I,axis1)-x0,2)+pow(x(I,axis2)-y0,2)+pow(x(I,axis3)-z0,2));  

    radius=sqrt(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0)+SQR(x(I,axis3)-z0));      // fastest for tux231

    where( radius==0. )
      radius=eps;
  }
  
  if( computeMap )
  {
    if( getIsPeriodic(axis2) )
    {
      r(I,axis2)=(atan2(evaluate(y0-x(I,axis2)),evaluate(x0-x(I,axis1)))+(Pi-twoPi*startTheta))*inverseScale;
      r(I,axis2)=fmod(r(I,axis2)+1.,1.);  // map back to [0,1]
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      real theta0=twoPi*startTheta, theta1=twoPi*endTheta;

      r(I,axis2)=atan2(evaluate(x(I,axis2)-y0),evaluate(x(I,axis1)-x0));  // **NOTE** +theta : result in [-pi,pi]

      real delta = (1.-(endTheta-startTheta))*Pi;
      where ( r(I,axis2) < theta0 - delta )
      {
	r(I,axis2)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,axis2)=(r(I,axis2)-theta0)*inverseScale;
    }


    r(I,axis1)=(acos((x(I,axis3)-z0)/radius)-startPhi*Pi)*inversePhiFactor;   // acos() -> [0,pi]
    if( domainDimension==3 )
      r(I,axis3)=(radius-innerRadius)*inverseRad;
  }

  if( computeMapDerivative )
  {
    realArray theta;
    realArray sinPhi,sinPhiOrtho, sinRatio;
    const realArray & rI = computeMap ? r(I,axis1) : 
                           evaluate( (acos((x(I,axis3)-z0)/radius)-startPhi*Pi)*inversePhiFactor );
    sinPhi=sin(phiFactor*rI+startPhi*Pi);
    sinPhiOrtho=sin(Pi*rI); // this is the sinPhi used in the orthographic scaling
    where( sinPhi>REAL_EPSILON*100. )
    {
      sinRatio=sinPhiOrtho/sinPhi;
    }
    otherwise()
    {
      sinRatio=1./(endPhi-startPhi);
    }
    if( domainDimension==2 )
    {  // spherical surface
      // rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis3)=0.;

      theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
      rx(I,axis1,axis3)=-(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))*cos(theta(I))*inversePhiFactor
	/(SQR(radius)*(x(I,axis1)-x0));
      rx(I,axis1,axis1)=(x(I,axis3)-z0)*cos(theta(I))*inversePhiFactor/(SQR(radius));
      rx(I,axis1,axis2)=(x(I,axis3)-z0)*sin(theta(I))*inversePhiFactor/(SQR(radius));
    }
    else
    {
      rx(I,axis3,axis1)=(x(I,axis1)-x0)/(rad*radius);
      rx(I,axis3,axis2)=(x(I,axis2)-y0)/(rad*radius);
      rx(I,axis3,axis3)=(x(I,axis3)-z0)/(rad*radius);

      // rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      rx(I,axis2,axis3)=0.;

      theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
      rx(I,axis1,axis3)=-(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))*cos(theta(I))*inversePhiFactor
	/(SQR(radius)*(x(I,axis1)-x0));
      rx(I,axis1,axis1)=(x(I,axis3)-z0)*cos(theta(I))*inversePhiFactor/(SQR(radius));
      rx(I,axis1,axis2)=(x(I,axis3)-z0)*sin(theta(I))*inversePhiFactor/(SQR(radius));
    }

    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
      // derivatives: ( d/d(r1), d/d(r2), d/d(r3) )
      if( domainDimension==2 )
      {  // spherical surface
	rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
	rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      }
      else
      {
	rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
	rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      }
      break;

    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d(phi)/d(x) sin(phi)*d(theta)/dx dr/dx)

      if( domainDimension==2 )
      {  // spherical surface
	theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;

	rx(I,axis2,axis1)=-sin(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
	rx(I,axis2,axis2)= cos(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
      }
      else
      {
	theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
	rx(I,axis2,axis1)=-sin(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
	rx(I,axis2,axis2)= cos(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
      }

      break;
    default:
      cerr << "Sphere::map: ERROR not implemented for coordinateType = " 
	   << params.coordinateType << endl;
      exit(1);
    }
  }
}

#define RADIUS(x) (rad*(x)+innerRadius)
void SphereMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params)
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  real rad=outerRadius-innerRadius;
  real phiFactor=Pi*(endPhi-startPhi);
  real thetaFactor=twoPi*(endTheta-startTheta);

  RealArray sinPhi(I), cosPhi(I), cosTheta(I),sinTheta(I);    
  sinPhi=sin(phiFactor*r(I,axis1)+startPhi*Pi);
  cosPhi=cos(phiFactor*r(I,axis1)+startPhi*Pi);
  sinTheta=sin(thetaFactor*r(I,axis2)+startTheta*twoPi);
  cosTheta=cos(thetaFactor*r(I,axis2)+startTheta*twoPi);

  RealArray sinPhiOrtho, sinRatio;
  sinPhiOrtho=sin(Pi*r(I,axis1)); // this is the sinPhi used in the orthographic scaling
  const bool useOpt=true;
  if( useOpt )
  {
    sinRatio.redim(I);
    const realSerialArray & sinPhiOrthoLocal =sinPhiOrtho;
    const realSerialArray & sinRatioLocal=sinRatio;
    const realSerialArray & sinPhiLocal =sinPhi;

    real * sinPhiOrthop = sinPhiOrthoLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINPHIORTHO(i0) sinPhiOrthop[i0]
    
    real * sinPhip = sinPhiLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINPHI(i0) sinPhip[i0]
    
    real * sinRatiop = sinRatioLocal.Array_Descriptor.Array_View_Pointer0;
    #define SINRATIO(i0) sinRatiop[i0]

    const int baseLocal = max(base,sinPhiLocal.getBase(0));
    const int boundLocal = min(bound,sinPhiLocal.getBound(0));
    
    for( int i=baseLocal; i<=boundLocal; i++ )
    {
      if( SINPHIORTHO(i)>REAL_EPSILON*100. )
      {
	SINRATIO(i)=SINPHI(i)/SINPHIORTHO(i);
      }
      else
      {
	SINRATIO(i)=endPhi-startPhi;
      }
    }
  }
  else
  {
    where( sinPhiOrtho>REAL_EPSILON*100. )
    {
      sinRatio=sinPhi/sinPhiOrtho;
    }
    otherwise()
    {
      sinRatio=endPhi-startPhi;
    }
  }
  

  if( computeMap )
  {
    if( domainDimension==2 )
    {  // spherical surface
      x(I,axis1)=innerRadius*cosTheta*sinPhi+x0; 
      x(I,axis2)=innerRadius*sinTheta*sinPhi+y0;
      x(I,axis3)=innerRadius*cosPhi+z0;
    }
    else
    {
      x(I,axis1)=RADIUS(r(I,axis3))*cosTheta*sinPhi+x0; 
      x(I,axis2)=RADIUS(r(I,axis3))*sinTheta*sinPhi+y0;
      x(I,axis3)=RADIUS(r(I,axis3))*cosPhi+z0;
    }
  }

  if( computeMapDerivative )
  {
    if( domainDimension==2 )
    {  // spherical surface
      xr(I,axis1,axis1)=innerRadius*         cosTheta*phiFactor*cosPhi;
      // xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta   *sinPhi;

      xr(I,axis2,axis1)=innerRadius*sinTheta*phiFactor*cosPhi;
      // xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta*sinPhi;

      xr(I,axis3,axis1)=innerRadius*(-phiFactor)*sinPhi;
      xr(I,axis3,axis2)=0.;

    }
    else
    {
      xr(I,axis1,axis1)=RADIUS(r(I,axis3))*         cosTheta*phiFactor*cosPhi;
      // xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta   *sinPhi;
      xr(I,axis1,axis3)=rad*                    cosTheta   *sinPhi;

      xr(I,axis2,axis1)=RADIUS(r(I,axis3))*sinTheta*phiFactor*cosPhi;
      // xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta*sinPhi;
      xr(I,axis2,axis3)=rad*sinTheta*sinPhi;

      xr(I,axis3,axis1)=RADIUS(r(I,axis3))*(-phiFactor)*sinPhi;
      xr(I,axis3,axis2)=0.;
      xr(I,axis3,axis3)=rad*cosPhi;
    }
    switch (params.coordinateType)
    {
      
    case cartesian:  // mapping returned in cartesian form
      // derivatives: ( d/d(r1), d/d(r2), d/d(r3) )
      if( domainDimension==2 )
      {  // spherical surface
	xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta   *sinPhi;
	xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta*sinPhi;
      }
      else
      {
	xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta   *sinPhi;
	xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta*sinPhi;
      }
      break;

    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d/d(phi), (1/sin(phi))d/d(theta), d/d(r) )

      if( domainDimension==2 )
      {  // spherical surface
	xr(I,axis1,axis2)=innerRadius*(-thetaFactor)*sinTheta*sinRatio;
	xr(I,axis2,axis2)=innerRadius*thetaFactor*cosTheta   *sinRatio;
      }
      else
      {
	xr(I,axis1,axis2)=RADIUS(r(I,axis3))*(-thetaFactor)*sinTheta*sinRatio;
	xr(I,axis2,axis2)=RADIUS(r(I,axis3))*thetaFactor*cosTheta   *sinRatio;
      }
      break;
    default:
      cerr << "Sphere::map: ERROR not implemented for coordinateType = " 
	<< params.coordinateType << endl;
      exit(1);
    }
  }
}
#undef RADIUS


void SphereMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
// =========================================================================================
//   new *serial-array version*
// =========================================================================================
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  real inverseScale=1./(twoPi*(endTheta-startTheta));
  real rad=outerRadius-innerRadius;
  real inverseRad=1./rad;

  RealArray radius(I);
  real phiFactor = Pi*(endPhi-startPhi);
  real inversePhiFactor = 1./phiFactor;
  real eps = REAL_MIN*100.;

  if( computeMap || computeMapDerivative )
  {
    // This next line was 4000 times slower than using sqrt() ! on 64bit tux231 gcc 3.4.4 
    // radius=pow(pow(x(I,axis1)-x0,2)+pow(x(I,axis2)-y0,2)+pow(x(I,axis3)-z0,2),.5);

    radius=sqrt(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0)+SQR(x(I,axis3)-z0));      // fastest for tux231

    where( radius==0. )
      radius=eps;
  }
  
  if( computeMap )
  {
    if( getIsPeriodic(axis2) )
    {
      r(I,axis2)=(atan2(evaluate(y0-x(I,axis2)),evaluate(x0-x(I,axis1)))+(Pi-twoPi*startTheta))*inverseScale;
      r(I,axis2)=fmod(r(I,axis2)+1.,1.);  // map back to [0,1]
    }
    else
    {
      // if we are not periodic then we shift the angle to [theta0-delta,theta1+delta] where
      // delta is the gap in the angle between the start and end of the annulus
      real theta0=twoPi*startTheta, theta1=twoPi*endTheta;

      r(I,axis2)=atan2(evaluate(x(I,axis2)-y0),evaluate(x(I,axis1)-x0));  // **NOTE** +theta : result in [-pi,pi]

      real delta = (1.-(endTheta-startTheta))*Pi;
      where ( r(I,axis2) < theta0 - delta )
      {
	r(I,axis2)+=twoPi;    // now the angle in is the range [theta0-delta,theta1+delta]
      }
      r(I,axis2)=(r(I,axis2)-theta0)*inverseScale;
    }


    r(I,axis1)=(acos((x(I,axis3)-z0)/radius)-startPhi*Pi)*inversePhiFactor;   // acos() -> [0,pi]
    if( domainDimension==3 )
      r(I,axis3)=(radius-innerRadius)*inverseRad;
  }

  if( computeMapDerivative )
  {
    RealArray theta;
    RealArray sinPhi,sinPhiOrtho, sinRatio;
    const RealArray & rI = computeMap ? r(I,axis1) : 
                           evaluate( (acos((x(I,axis3)-z0)/radius)-startPhi*Pi)*inversePhiFactor );
    sinPhi=sin(phiFactor*rI+startPhi*Pi);
    sinPhiOrtho=sin(Pi*rI); // this is the sinPhi used in the orthographic scaling
    where( sinPhi>REAL_EPSILON*100. )
    {
      sinRatio=sinPhiOrtho/sinPhi;
    }
    otherwise()
    {
      sinRatio=1./(endPhi-startPhi);
    }
    if( domainDimension==2 )
    {  // spherical surface
      // rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis3)=0.;

      theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
      rx(I,axis1,axis3)=-(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))*cos(theta(I))*inversePhiFactor
	/(SQR(radius)*(x(I,axis1)-x0));
      rx(I,axis1,axis1)=(x(I,axis3)-z0)*cos(theta(I))*inversePhiFactor/(SQR(radius));
      rx(I,axis1,axis2)=(x(I,axis3)-z0)*sin(theta(I))*inversePhiFactor/(SQR(radius));
    }
    else
    {
      rx(I,axis3,axis1)=(x(I,axis1)-x0)/(rad*radius);
      rx(I,axis3,axis2)=(x(I,axis2)-y0)/(rad*radius);
      rx(I,axis3,axis3)=(x(I,axis3)-z0)/(rad*radius);

      // rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      // rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      rx(I,axis2,axis3)=0.;

      theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
      rx(I,axis1,axis3)=-(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))*cos(theta(I))*inversePhiFactor
	/(SQR(radius)*(x(I,axis1)-x0));
      rx(I,axis1,axis1)=(x(I,axis3)-z0)*cos(theta(I))*inversePhiFactor/(SQR(radius));
      rx(I,axis1,axis2)=(x(I,axis3)-z0)*sin(theta(I))*inversePhiFactor/(SQR(radius));
    }

    switch (params.coordinateType)
    {
    case cartesian:  // mapping returned in cartesian form
      // derivatives: ( d/d(r1), d/d(r2), d/d(r3) )
      if( domainDimension==2 )
      {  // spherical surface
	rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
	rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      }
      else
      {
	rx(I,axis2,axis1)=-(x(I,axis2)-y0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
	rx(I,axis2,axis2)= (x(I,axis1)-x0)*inverseScale/((SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))); 
      }
      break;

    case spherical: // Mapping returned in spherical form : (phi,theta,r) 
      // derivatives: ( d(phi)/d(x) sin(phi)*d(theta)/dx dr/dx)

      if( domainDimension==2 )
      {  // spherical surface
	theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;

	rx(I,axis2,axis1)=-sin(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
	rx(I,axis2,axis2)= cos(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
      }
      else
      {
	theta=atan2(y0-x(I,axis2),x0-x(I,axis1))+Pi;
	rx(I,axis2,axis1)=-sin(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
	rx(I,axis2,axis2)= cos(theta(I))*sinRatio*inverseScale/(radius);  // *sin(phi)
      }

      break;
    default:
      cerr << "Sphere::map: ERROR not implemented for coordinateType = " 
	   << params.coordinateType << endl;
      exit(1);
    }
  }
}


//=================================================================================
// Is the point x outside the range of the mapping
// TRUE : point is definitely outside
// FALSE : don't know if the point is outside
//=================================================================================
int SphereMapping::outside( const realArray & x )
{
  const real safetyFactor=1.1;
  real distanceSquared=SQR(x(axis1)-x0)+SQR(x(axis2)-y0)+SQR(x(axis3)-z0);
  return (distanceSquared > SQR(outerRadius)*safetyFactor) || 
         (distanceSquared < SQR(innerRadius)/safetyFactor);
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int SphereMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  subDir.setMode(GenericDataBase::streamInputMode);

  if( debug & 4 )
    cout << "Entering SphereMapping::get" << endl;

  subDir.get( SphereMapping::className,"className" ); 
  if( SphereMapping::className != "SphereMapping" )
  {
    cout << "SphereMapping::get ERROR in className!" << endl;
  }

  subDir.get( x0,"x0" );
  subDir.get( y0,"y0" );
  subDir.get( z0,"z0" );
  subDir.get( innerRadius,"innerRadius" );
  subDir.get( outerRadius,"outerRadius" );
  subDir.get( startTheta,"startTheta");
  subDir.get( endTheta,"endTheta");
  subDir.get( startPhi,"startPhi");
  subDir.get( endPhi,"endPhi");
  
  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  mappingHasChanged();
  return 0;
}

int SphereMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( SphereMapping::className,"className" );
  subDir.put( x0,"x0" );
  subDir.put( y0,"y0" );
  subDir.put( z0,"z0" );
  subDir.put( innerRadius,"innerRadius" );
  subDir.put( outerRadius,"outerRadius" );
  subDir.put( startTheta,"startTheta");
  subDir.put( endTheta,"endTheta");
  subDir.put( startPhi,"startPhi");
  subDir.put( endPhi,"endPhi");

  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}

Mapping *SphereMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==SphereMapping::className )
    retval = new SphereMapping();
  return retval;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int SphereMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!SphereMapping",
      "centre for sphere",
      "inner radius",
      "outer radius",
      "inner and outer radii",
      "bounds on phi (latitude)",
      "bounds on theta (longitude)",
      "surface or volume (toggle)",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "check",
      "check inverse",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "centre for sphere  : Specify (x0,y0,z0) for the centre",
      "inner radius       : Specify the inner radius",
      "outer radius       : Specify the outer radius",
      "bounds on phi (latitude) : define bounds on phi (normalized to [0,1])",
      "bounds on theta (longitude) : define bounds on theta (normalized to [0,1])",
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

  gi.appendToTheDefaultPrompt("Sphere>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="centre for sphere" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter (x0,y0,z0) for centre (default=(%e,%e,%e)): ",x0,y0,z0));
      if( line!="" ) sScanF(line,"%e %e %e",&x0,&y0,&z0);
      mappingHasChanged();
    }
    else if( answer=="inner and outer radii" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the inner and outer radii (default=%e,%e): ",innerRadius,outerRadius));
      if( line!="" ) sScanF( line,"%e %e",&innerRadius,&outerRadius);
      mappingHasChanged();
    }
    else if( answer=="inner radius" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the inner radius (default=%e): ",innerRadius));
      if( line!="" ) sScanF( line,"%e",&innerRadius);
      mappingHasChanged();
    }
    else if( answer=="outer radius" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the outer radius (default=%e): ",outerRadius));
      if( line!="" ) sScanF( line,"%e",&outerRadius);
      mappingHasChanged();
    }
    else if(answer=="surface or volume (toggle)")
    {
      if( domainDimension==3 )
        setDomainDimension(2);
      else
        setDomainDimension(3);
      mappingHasChanged();
    }
    else if( answer=="bounds on phi (latitude)" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the bounds on `phi' in [0,1] (default=(%e,%e)): ",
         startPhi,endPhi));
      if( line!="" ) sScanF( line,"%e %e",&startPhi,&endPhi);

      if( startPhi==0. )
      {
        setTypeOfCoordinateSingularity( Start,axis1,polarSingularity ); // phi has a "polar" singularity
        if( getBoundaryCondition(Start,axis1)>0 )
	{
          printf("SphereMapping:Info: setting boundary condition in phi direction to 0\n");
	  setBoundaryCondition(Start,axis1,0);
	}
      }
      else
        setTypeOfCoordinateSingularity( Start,axis1,noCoordinateSingularity );
      if( endPhi==1. )
      {
        setTypeOfCoordinateSingularity( End,axis1,polarSingularity ); // phi has a "polar" singularity
        if( getBoundaryCondition(End,axis1)>0 )
	{
          printf("SphereMapping:Info: setting boundary condition in phi direction to 0\n");
	  setBoundaryCondition(End,axis1,0);
	}
      }
      else
        setTypeOfCoordinateSingularity( End,axis1,noCoordinateSingularity );
      mappingHasChanged();
    }
    else if( answer=="bounds on theta (longitude)" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the bounds on `theta' in [0,1] (default=(%e,%e)): ",
         startTheta,endTheta));
      if( line!="" ) sScanF( line,"%e %e",&startTheta,&endTheta);
      // fix up boundary conditions and periodicity
      if( fabs(endTheta-startTheta-1.)<REAL_EPSILON*10. ) 
      {
	setIsPeriodic(axis2,functionPeriodic );  
	setBoundaryCondition( Start,axis2,-1 );
	setBoundaryCondition(   End,axis2,-1 );
      }	
      else
      {
	setIsPeriodic(axis2,notPeriodic );  
	if( getBoundaryCondition(Start,axis2)<0 )
	{
	  setBoundaryCondition(Start,axis2,0);
	  setBoundaryCondition(  End,axis2,0);
	}
      }
      mappingHasChanged();
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" || 
             answer=="check" ||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="show parameters" )
    {
      printf(" (innerRadius,outerRadius=(%e,%e)\n centre: (x0,y0,z0)=(%e,%e,%e)\n",
         innerRadius,outerRadius,x0,y0,z0);
      Mapping::display();
      // printf(" (startTheta,endTheta)=(%e,%e)\n",startTheta,endTheta);
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
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
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

