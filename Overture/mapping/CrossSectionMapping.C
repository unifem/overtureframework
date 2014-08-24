#include "CrossSectionMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "SplineMapping.h"
#include "display.h"
#include <float.h>

//--------------------------------------------------------------------------
//  Define a mapping from cross-sections
//--------------------------------------------------------------------------

CrossSectionMapping::
CrossSectionMapping(
		    const real startS_,
		    const real endS_,
		    const real startAngle_,
		    const real endAngle_,
		    const real innerRadius_ ,
		    const real outerRadius_,
		    const real x0_ ,
		    const real y0_ ,
		    const real z0_ ,
		    const real length_,
		    const int domainDimension_)
  : Mapping(domainDimension_,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \details  Default Constructor, define a mapping from cross-sections.
/// 
///  Build a mapping defined by cross sections. In the {\tt general} case the cross-sections
///   are defined by other Mapping's. One can also build an ellipsoid when the cross-section
///  type is {\tt ellipse} or a Joukowsky wing.
///  enum CrossSectionType:
///  <ul>
///   <li> general
///   <li> ellipse
///   <li> joukowsky
///  </ul>
/// 
///   enum Parameterization:
///  <ul>
///   <li> <B>arcLength</B> : parameterize by the arc length distance between the centroids of the cross sectional curves.
///   <li> <B>index</B> : parameterize by the index of the cross section.
///   <li> <B>userDefined</B> : supply a parameterization.
///  </ul>
//===========================================================================
{ 
  CrossSectionMapping::className="CrossSectionMapping";
  setName( Mapping::mappingName,"CrossSection");

  tAxis=axis1;  // first tangential (angular direction).

  // the default cross section is the ellipse
  crossSectionType=ellipse;
  sAxis=axis2;  // axial direction for ellipse
  setBasicInverseOption(canInvert);  // only the ellipse is invertible

  setGridDimensions( tAxis,21 );  
  setGridDimensions( sAxis,11); 
  setGridDimensions( axis3,5 );  // radius
 
  axialApproximation=linear;

  numberOfCrossSections=0;
  crossSection=NULL;
  parameterMap=NULL;
  radiusSpline=NULL;
  parameterization=arcLength;
  

  innerRadius=innerRadius_;
  outerRadius=outerRadius_;
  x0=x0_;
  y0=y0_;
  z0=z0_;
  length=length_;
  startAngle=startAngle_;
  endAngle=endAngle_;
  startS=startS_;
  endS=endS_;

  polarSingularityFactor=5.;

  a=1.; b=1., c=2.;   // default is an ellipse
  joukowskyDelta=15.*twoPi/360.;
  joukowskyD=.15;
  joukowskyA=.85;
  joukowskyLength=10.;
  joukowskyBeta=1.;

  
  if( startS==0. )
    setTypeOfCoordinateSingularity( Start,sAxis,polarSingularity ); // s has a "polar" singularity
  if(  fabs(endS-1.)<REAL_EPSILON*5. )
    setTypeOfCoordinateSingularity( End  ,sAxis,polarSingularity ); // at both ends
  
  setCoordinateEvaluationType( cylindrical,TRUE );  // Mapping can be evaluated in cylindrical coordinates
                                                    // (in addition to cartesian)  

  setBoundaryCondition( Start,sAxis,0 );  // singular sides are interpolation by default.
  setBoundaryCondition( End  ,sAxis,0 );

  setBoundaryCondition( Start,axis3,1 );
  setBoundaryCondition( End  ,axis3,0 );
  if( fabs(endAngle-startAngle)-1. < REAL_EPSILON*10. )
  {
    setIsPeriodic(tAxis, functionPeriodic );  
    setBoundaryCondition( Start,tAxis,-1);
    setBoundaryCondition( End  ,tAxis,-1);
  }
  else
  {
    setBoundaryCondition( Start,tAxis,0 );
    setBoundaryCondition( End  ,tAxis,0 );
  }
  mappingHasChanged();
}


  // Copy constructor is deep by default
CrossSectionMapping::
CrossSectionMapping( const CrossSectionMapping & map, const CopyType copyType )
{
  CrossSectionMapping::className="CrossSectionMapping";
  numberOfCrossSections=0;
  crossSection=NULL;
  parameterMap=NULL;
  radiusSpline=NULL;

  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "CrossSectionMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

CrossSectionMapping::
~CrossSectionMapping()
{ 
  if( (debug/4) % 2 )
   cout << " CrossSectionMapping::Destructor called" << endl;

  delete parameterMap;
  delete radiusSpline;
  
  for( int i=0; i<numberOfCrossSections; i++ )
  {
    if( crossSection[i]!=NULL && crossSection[i]->decrementReferenceCount() == 0)
    {
      delete crossSection[i];
    }
  }
  delete [] crossSection;
}

CrossSectionMapping & CrossSectionMapping::
operator =( const CrossSectionMapping & x )
{
  if( CrossSectionMapping::className != x.getClassName() )
  {
    cout << "CrossSectionMapping::operator= ERROR trying to set a CrossSectionMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class

  if( crossSection!=NULL )
  {
    for( int i=0; i<numberOfCrossSections; i++ )
    {
      if( crossSection[i]!=NULL && crossSection[i]->decrementReferenceCount() == 0)
      {
	delete crossSection[i];
      }
    }
    delete [] crossSection;
    crossSection=NULL;
  }

  tAxis                 =x.tAxis;
  sAxis                 =x.sAxis;
  crossSectionType      =x.crossSectionType;
  numberOfCrossSections =x.numberOfCrossSections;

  // *wdh* 050617  crossSection          =x.crossSection;
  if( numberOfCrossSections>0 )
  {
    crossSection = new Mapping* [numberOfCrossSections];
    for( int i=0; i<numberOfCrossSections; i++ )
    {
      crossSection[i]=x. crossSection[i];
      crossSection[i]->incrementReferenceCount();
    }
  }
  
  axialApproximation    =x.axialApproximation;
  parameterization      =x.parameterization;
  s.redim(0);  
  s                     =x.s;
  if( parameterMap==0 && x.parameterMap!=0 )
    parameterMap = new SplineMapping;
  else if( parameterMap!=0 && x.parameterMap==0 )
  {
    delete parameterMap;
    parameterMap=0;
  }
  if( parameterMap!=0 && x.parameterMap!=0 )
  {
    //    *parameterMap         =*x.parameterMap;  // This only calls the base calls operator =  *wdh* 050617

    SplineMapping & xSpline = *((SplineMapping*)x.parameterMap);
    SplineMapping & pSpline = *((SplineMapping*)parameterMap);

    pSpline=xSpline;// note : deep copy since the Spline may have internal parameters set.
  }
  
  centroid.redim(0);
  centroid              =x.centroid;
  csRadius.redim(0);
  csRadius              =x.csRadius;

  if( radiusSpline==0 && x.radiusSpline!=0 )
    radiusSpline = new SplineMapping;
  else if( radiusSpline!=0 && x.radiusSpline==0 )
  {
    delete radiusSpline;
    radiusSpline=0;
  }
  if( radiusSpline!=0 && x.radiusSpline!=0 )
    *radiusSpline         =*x.radiusSpline;  // note : deep copy since the Spline may have internal parameters set.

  polarSingularityFactor=x.polarSingularityFactor;
  
  innerRadius           =x.innerRadius;
  outerRadius           =x.outerRadius;
  x0                    =x.x0;
  y0                    =x.y0;
  z0                    =x.z0;
  length                =x.length;
  startAngle            =x.startAngle;
  endAngle              =x.endAngle;
  a                     =x.a;
  b                     =x.b;
  c                     =x.c;
  startS                =x.startS;
  endS                  =x.endS;
  joukowskyDelta        =x.joukowskyDelta;
  joukowskyD            =x.joukowskyD;
  joukowskyA            =x.joukowskyA;
  joukowskyLength       =x.joukowskyLength;
  joukowskyBeta         =x.joukowskyBeta;

  return *this;
}

int CrossSectionMapping::
setCrossSectionType(CrossSectionTypes type)
//===========================================================================
/// \details 
///    Define the cross-section type.  *this is not finished yet*
/// \param type (input):
/// 
//===========================================================================
{

  crossSectionType=type;
  // *** finish this ***
  return 0;
}




int CrossSectionMapping::
initialize()
//===========================================================================
/// \details  private routine. Initialize the parameterization for the
///   cross sections.
/// 
//===========================================================================
{
  if( crossSectionType==general )
    sAxis=domainDimension-1;
  else
    sAxis=axis2;
  
  if( crossSectionType==ellipse )
  {
    setBasicInverseOption(canInvert);
  }
  else
  {
    setBasicInverseOption(canDoNothing);
  }

  if( crossSectionType!=general || numberOfCrossSections<=0 )
    return 0;
  
  // For general cross-sections we compute
  //     o centroid for each cross section
  //     o average radius of each cross section 
  //     o arclength between centroids
  int i,axis;
  Range I1,I2,I3;
  realArray r,x,x1;
  centroid.redim(3,Range(-1,numberOfCrossSections));  // holds centroid
  csRadius.redim(numberOfCrossSections);    // holds cross-section radius

  if( parameterization==arcLength || parameterization==userDefined )
  {
    s.redim(Range(-1,numberOfCrossSections)); // include ghost points
    s(0)=0.;
  }

  const realArray & xc0 = crossSection[0]->getGrid();
  Range Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  
  J1=xc0.dimension(0);
  J2=xc0.dimension(1);
  J3=xc0.dimension(2);
  
  for( i=0; i<numberOfCrossSections; i++ )
  {
    x.reference(crossSection[i]->getGrid());
    Range I1=x.dimension(0);
    Range I2=x.dimension(1);
    Range I3=x.dimension(2);

    if( I1!=J1 || I2!=J2 )
    {
      // printf("grid for cross section %i not the same size at x-section 0\n",i);

      I1=J1; I2=J2; I3=J3;
      
      if( r.getLength(0)!=J1.length() )
      {
	const int sectionDomainDimension=crossSection[i]->getDomainDimension();
	r.redim(J1,J2,J3,sectionDomainDimension);
	real dr[3];
	for( axis=axis1; axis<=axis3; axis++ )
	  dr[axis]=1./max(Jv[axis].getBound()-Jv[axis].getBase(),1);

	int i1,i2,i3;
	for( i3=J3.getBase(); i3<=J3.getBound(); i3++ )
	{
     	  for( i2=J2.getBase(); i2<=J2.getBound(); i2++ )
	    r(J1,i2,i3,0).seqAdd(0.,dr[axis1]);
	  if( sectionDomainDimension>1 )
	  {
	    for( i1=J1.getBase(); i1<=J1.getBound(); i1++ )
	      r(i1,J2,i3,1).seqAdd(0.,dr[axis2]);
	  }
	}
      }
      x.breakReference();
      x.redim(J1,J2,J3,rangeDimension);
      crossSection[i]->mapGrid(r,x);
    }

    //  do not include last point which equals the first if periodic
    if( crossSection[i]->getIsPeriodic(axis1) )
      I1=Range(I1.getBase(),I1.getBound()-1);
    if( domainDimension==3 && (bool)crossSection[i]->getIsPeriodic(axis2) )
      I2=Range(I2.getBase(),I2.getBound()-1);

    const int n=I1.getLength()*I2.getLength();
    for( axis=axis1; axis<rangeDimension; axis++ )
      centroid(axis,i)=sum(x(I1,I2,I3,axis))/n;

    csRadius(i)=sum(SQRT( SQR(x(I1,I2,I3,0)-centroid(0,i))+
			  SQR(x(I1,I2,I3,1)-centroid(1,i))+
			  SQR(x(I1,I2,I3,2)-centroid(2,i)) ) )/n;
    // compute the average distance between the curves --- this assumes that the parameterizations
    // on the curves are reasonably compatible
    if( (parameterization==arcLength || parameterization==userDefined) && i>0 )
    {
      s(i)=s(i-1)+sum( SQRT( SQR(x(I1,I2,I3,0)-x1(I1,I2,I3,0))+
			     SQR(x(I1,I2,I3,1)-x1(I1,I2,I3,1))+
			     SQR(x(I1,I2,I3,2)-x1(I1,I2,I3,2)) ) )/n;
    }
    x1.redim(0);
    x1=x;
  }
  if( parameterization==arcLength || parameterization==userDefined )
  {
    // if there are polar singularities on the ends we increase the arclength at the ends.
    if( getTypeOfCoordinateSingularity(Start,sAxis)==polarSingularity )
    {
      s(Range(1,numberOfCrossSections-1))+=csRadius(0);
    }
    if( getTypeOfCoordinateSingularity(  End,sAxis)==polarSingularity )
      s(numberOfCrossSections-1)+=csRadius(numberOfCrossSections-1);
       

    real totalLength=s(numberOfCrossSections-1);
    if( totalLength<=0. )
    {
      printf("CrossSectionMapping::initialize:ERROR: the cross sections all appear to have the same centroid!\n");
      totalLength=1.;
    }

    s(Range(0,(numberOfCrossSections-1)))/=totalLength;

    // assign ghost values:
    s(-1)=2.*s(0)-s(1);
    s(numberOfCrossSections)=2.*s(numberOfCrossSections-1)-s(numberOfCrossSections-2);

    Range Rx(0,rangeDimension-1);
    
    real alpha=(s(-1)-s(0))/(s(1)-s(0));
    centroid(Rx,-1)=(1.-alpha)*centroid(Rx,0)+alpha*centroid(Rx,1);
    alpha=(s(numberOfCrossSections  )-s(numberOfCrossSections-2))/
          (s(numberOfCrossSections-1)-s(numberOfCrossSections-2));
    centroid(Rx,numberOfCrossSections)=(1.-alpha)*centroid(Rx,numberOfCrossSections-2)+
                                           alpha *centroid(Rx,numberOfCrossSections-1);

    if( Mapping::debug & 2 )
      ::display(s,"CrossSectionMapping::initialize: arclength s");

    if( parameterization==arcLength )
    {
      if( parameterMap==NULL )
	parameterMap = new SplineMapping;
      SplineMapping & spline = (SplineMapping&) *parameterMap;


      spline.setParameterization(s);
      realArray rr(numberOfCrossSections+2);
      real dr=1./(numberOfCrossSections-1);
      rr.seqAdd(-dr,dr);
      spline.setPoints(rr);  // make a 1D spline
      real splineTension=numberOfCrossSections<5 ? 40. : 20;
      spline.setTension(splineTension);

      // set the end conditions to be x''(0)=x''(1)=0
      RealArray endValues(2,rangeDimension);
      endValues=0.;
      spline.setEndConditions(SplineMapping::secondDerivative,endValues);

      // check the spline derivatives to make sure that it is montone.
      // *** we need to be able to invert the parameter map at the ghost points ****
      const int m=max(11,numberOfCrossSections*2);
      Range I(0,m);
      realArray sr(I,1,1), r(I,1), x(2,1);

      const real ra=-.25, rb=1.25;
      dr =(rb-ra)/m;
      r.seqAdd(ra,dr);
      for( int it=0; it<2; it++ )
      {
	spline.map(r,Overture::nullRealDistributedArray(),sr);

        if( Mapping::debug & 2 )
  	  ::display(sr,"Here are the derivatives of the axial spline. These should be positive","%8.2e ");

	const real srMin = min(sr);
	if( srMin<.01 )
	{
          if( it==0 )
	  {
            if( srMin<.01 )
	    {
	      printf("CrossSectionMapping::INFO: the default axial spline has a small derivative = %e\n"
		     "  The spline may not be invertible so I am increasing the tension to %5.1f\n"
		     "  You may want to adjust this further with the `change arclength spline parameters' option\n",
		     srMin,splineTension);

	      ::display(sr,"Here are the derivatives of the axial spline. These should be positive","%8.2e ");
	    }
	  }
	  // set the tension and the end conditions to x''=0
          splineTension+=20.;
          printf("CrossSectionMapping::INFO:Setting the tension of the spline to be %5.1f\n",splineTension);
	  spline.setTension(splineTension);
	}
        else
          break;
      }
    }
  }
      
  if( Mapping::debug & 4 )
  {
    ::display(centroid,"Here are the centroid's");
    ::display(csRadius,"Here are the cross-section radii");
  }
  // normalize by the last radius
  if( Mapping::debug & 4 )
    ::display(csRadius,"csRadius before normalizing");
  
  csRadius*=(1./csRadius(numberOfCrossSections-1));
  
  if( radiusSpline==NULL )
    radiusSpline = new SplineMapping;

  radiusSpline->setPoints(csRadius);  // make a 1D spline parameterize by normalized index, [0,1]
  RealArray endValues(2,rangeDimension);
  endValues=0.;
  radiusSpline->setEndConditions(SplineMapping::secondDerivative,endValues);


  return 0;
}




#define RADIUS(x) (rad*(x)+innerRadius)
// ******************
// cubic interpolant: nodes -1, 0, 1, 2
// ******************
#define q03(z)  ( oneSixth           *((z))*((z)-1.)*(2.-(z)))
#define q13(z)  ( .5        *((z)+1.)      *((z)-1.)*((z)-2.))
#define q23(z)  ( .5        *((z)+1.)*((z))         *(2.-(z)))
#define q33(z)  ( oneSixth  *((z)+1.)*((z))*((z)-1.)         )
// derivatives of the cubic
#define q03d(z) ( -oneSixth*(2.+(z)*(-6.+(z)*3.)) )
#define q13d(z) ( -.5+(z)*(-2.+(z)*1.5) )
#define q23d(z) (  1.+(z)*( 1.-(z)*1.5) )
#define q33d(z) ( -oneSixth+(z)*(z)*.5 )

// quadratic: nodes 0,1,2  to use at the left boundary
#define q02a(z)  (.5      *((z)-1.)*((z)-2.))
#define q12a(z)  (   ((z))         *(2.-(z)))
#define q22a(z)  (.5*((z))*((z)-1.))
// derivatives
#define q02ad(z) (-1.5+(z) )
#define q12ad(z) (2.-2.*(z) )
#define q22ad(z) (-.5+(z) )

// quadratic: nodes -1 0 1 to use at the right boundary
#define q02b(z)  (.5         *((z))*((z)-1.))
#define q12b(z)  (   ((z)+1.)      *(1.-(z)))
#define q22b(z)  (.5*((z)+1.)*((z))         )

#define q02bd(z) (-.5+(z) )
#define q12bd(z) (-2.*(z) )
#define q22bd(z) (.5+(z) )

//--------------------------------------------------------------------------------------------
// Define cross-sections in cylindrical coordinates, with possible polar singularities
// at one or more ends
//
//  (r1,r2,r3) = (s,theta/2pi,r) -> (x,y,z)
//
//  zeta = 2*s-1 in [-1,1]  rho =sqrt(1-zeta^2)
//
//  For "cylindrical" coordinates return the jacobian derivatives as  ...
//
//--------------------------------------------------------------------------------------------

void CrossSectionMapping::
map(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
{
  switch ( crossSectionType )
  {
  case general:
    if( getTypeOfCoordinateSingularity( Start,sAxis)==polarSingularity ||
        getTypeOfCoordinateSingularity( End  ,sAxis)==polarSingularity )
    {
      mapGeneralWithPolarSingularity(r,x,xr,params);
    }
    else
    {
      mapGeneral(r,x,xr,params);
    }
    break;
  default:
    mapBuiltin(r,x,xr,params);
  }
}

int CrossSectionMapping::
crossSectionMap( const int & cs, const realArray & r, realArray & x, realArray & xr)
// =======================================================================================================
// /Description:
//  Evaluate a cross section cs (also evaluate ghost cross-sections cs=-1 and cs=numberOfCrossSections.)
// =======================================================================================================
{
  if( cs>=0 && cs<numberOfCrossSections )
  {
    crossSection[cs]->map(r,x,xr);
  }
  else if( cs==-1 )
  {
/* ---
    real alpha=(s(-1)-s(0))/(s(1)-s(0));
    centroid(Rx,-1)=(1.-alpha)*centroid(Rx,0)+alpha*centroid(Rx,1);
    alpha=(s(numberOfCrossSections)-s(numberOfCrossSections-2))/
          (s(numberOfCrossSections-1)-s(numberOfCrossSections-2));
    centroid(Rx,-1)=(1.-alpha)*centroid(Rx,numberOfCrossSections-2)+alpha*centroid(Rx,numberOfCrossSections-1);
--- */
    crossSection[0]->map(r,x,xr);
    Range all;
    for( int axis=0; axis<rangeDimension; axis++ )
      x(all,axis)+=(centroid(axis,-1)-centroid(axis,0));
  }
  else if( cs==numberOfCrossSections )
  {
    crossSection[numberOfCrossSections-1]->map(r,x,xr);
    Range all;
    for( int axis=0; axis<rangeDimension; axis++ )
      x(all,axis)+=(centroid(axis,numberOfCrossSections)-centroid(axis,numberOfCrossSections-1));
  }
  else
  {
    {throw "error";}
  }
  return 0;
}


void CrossSectionMapping::
mapGeneral(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
// ========================================================================================
// General Cross section, no axial polar singularity
// ========================================================================================
{
  assert( crossSectionType==general );
  
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  int i,axis,dir;
   
  // ********************************
  // **** general cross section *****
  // ********************************
  const real oneSixth=1./6.;
  const bool useLinearInterpolation = axialApproximation==linear || numberOfCrossSections <=2 ;
  const bool useCubicInterpolation = !useLinearInterpolation;
    
  Range T(tAxis,tAxis+domainDimension-2);   // tangential directions

  realArray rI(I,T), rS(I,1); 
  rI=r(I,T);      // tangential direction(s)
  rS=r(I,sAxis);  // axial directions
    
  realArray dr(I);
  realArray section(I);  // point i lies between x-section section(i) and section(i)+1
  realArray sI(I,1), srI(I,1,1);
  if( parameterization==index )
  {
    sI.reference(rS);
    srI=1.;
  }
  else
  {
    parameterMap->mapC(rS,sI(I,0),srI(I,0,0));
  }

  section = floor(sI*(numberOfCrossSections-1));    // point i lies between x-section section(i) and section(i)+1)
  section = min(max(section,0.),numberOfCrossSections-2.);
      
  dr = sI*(numberOfCrossSections-1.) - section;

  // ::display(section,"*** section ***");
  // ::display(dr,"*** dr ***");

  // num(cs) = number of points that lie between cross-section cs and cs+1
  IntegerArray num(Range(-2,numberOfCrossSections+1)); // extend on ends for convenince below
  num=0;
    
#define IA(cs) ia[(cs)+1]

   // collect up all points between x-section cs and cs+1 into the intArray IA(cs)
  int cs;
  IntegerArray *ia = new IntegerArray [numberOfCrossSections+2];
  for( cs=0; cs<numberOfCrossSections; cs++ )
    IA(cs).redim(Range(I));
    
  for( i=base; i<=bound; i++ )
  {
    cs=int( floor(section(i)+.5) ); // ***
    IA(cs)(base+num(cs))=i;
    num(cs)++;
  }
    
  realArray xc0,xc1,xc2,xc3, xcr0,xcr1,xcr2,xcr3;

  // evaluate all points between x-section cs and cs+1
  for( cs=0; cs<numberOfCrossSections-1; cs++ )
  {
    if( num(cs)>0 )
    {
      const IntegerArray & ib = evaluate(IA(cs)(Range(base,num(cs)-1+base)));
      const realArray & dri = dr(ib);
      Range R=ib.dimension(0);
      realArray rT(R,T);
      for( dir=0; dir<domainDimension-1; dir++ )
	rT(R,dir) = r(ib,tAxis+dir);
	  
      xc1.redim(R,rangeDimension);
      xc2.redim(R,rangeDimension);
      if( computeMapDerivative )
      {
	xcr1.redim(R,rangeDimension,domainDimension);
	xcr2.redim(R,rangeDimension,domainDimension);
      }
      // evaluate the cross sections:
      crossSectionMap(cs  ,rT,xc1,xcr1); 
      crossSectionMap(cs+1,rT,xc2,xcr2); 
      if( useCubicInterpolation )
      {
	// 2 more cross-sections needed for cubic case
	if( cs!=0 )
	{
	  xc0.redim(R,rangeDimension);
	  if( computeMapDerivative )
	    xcr0.redim(R,rangeDimension,domainDimension);
	  crossSectionMap(cs-1,rT,xc0,xcr0); 
	}
	if( cs < numberOfCrossSections-1 )
	{
	  xc3.redim(R,rangeDimension);
	  if( computeMapDerivative )
	    xcr3.redim(R,rangeDimension,domainDimension);
	  crossSectionMap(cs+2,rT,xc3,xcr3); 
	}
      }
	
      if( computeMap )
      {
	for( axis=0; axis<rangeDimension; axis++ )
	{
	  if( useLinearInterpolation )
	    x(ib,axis) = (1.-dri)*xc1(R,axis) + dri*xc2(R,axis);
	  else
	  {
	    // cubic interpolation
	    // Use quadratic approximations at the ends.
	    if( cs>0 && cs<numberOfCrossSections-2 )
	      x(ib,axis)=q03(dri)*xc0(R,axis)+q13(dri)*xc1(R,axis)+q23(dri)*xc2(R,axis)+q33(dri)*xc3(R,axis);
	    else if( cs==0 )
	      x(ib,axis)=q02a(dri)*xc1(R,axis)+q12a(dri)*xc2(R,axis)+q22a(dri)*xc3(R,axis);
	    else
	      x(ib,axis)=q02b(dri)*xc0(R,axis)+q12b(dri)*xc1(R,axis)+q22b(dri)*xc2(R,axis);
	  }
	}
      }
      if( computeMapDerivative )
      {
	for( axis=0; axis<rangeDimension; axis++ )
	{
	  if( useLinearInterpolation )
	  {
	    xr(ib,axis,sAxis) = (xc2(R,axis)-xc1(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.);
	    for( dir=0; dir<domainDimension-1; dir++ )
	      xr(ib,axis,tAxis+dir) = (1.-dr(ib))*xcr1(R,axis,dir) + dr(ib)*xcr2(R,axis,dir);
	  }
	  else
	  {
	    // cubic 
	    if( cs>0 && cs<numberOfCrossSections-2 )
	    {
	      xr(ib,axis,sAxis) = 
		(q03d(dri)*xc0(R,axis) + 
		 q13d(dri)*xc1(R,axis) + 
		 q23d(dri)*xc2(R,axis) +
		 q33d(dri)*xc3(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.);
	      for( dir=0; dir<domainDimension-1; dir++ )
		xr(ib,axis,tAxis+dir)=
		  q03(dri)*xcr0(R,axis,dir) + 
		  q13(dri)*xcr1(R,axis,dir) + 
		  q23(dri)*xcr2(R,axis,dir) +
		  q33(dri)*xcr3(R,axis,dir);
	    }
	    else if( cs==0 )
	    {
	      xr(ib,axis,sAxis) = 
		(q02ad(dri)*xc1(R,axis) + 
		 q12ad(dri)*xc2(R,axis) +
		 q22ad(dri)*xc3(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.);
	      for( dir=0; dir<domainDimension-1; dir++ )
		xr(ib,axis,tAxis+dir) = 
		  q02a(dri)*xcr1(R,axis,dir) + 
		  q12a(dri)*xcr2(R,axis,dir) +
		  q22a(dri)*xcr3(R,axis,dir);
	    }
	    else 
	    {
	      xr(ib,axis,sAxis) = 
		(q02bd(dri)*xc0(R,axis) + 
		 q12bd(dri)*xc1(R,axis) + 
		 q22bd(dri)*xc2(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.);
	      for( dir=0; dir<domainDimension-1; dir++ )
		xr(ib,axis,tAxis+dir) = 
		  q02b(dri)*xcr0(R,axis,dir) + 
		  q12b(dri)*xcr1(R,axis,dir) + 
		  q22b(dri)*xcr2(R,axis,dir);
	    }
	  }
	}
      }
    }
  }
  delete [] ia;
}

void CrossSectionMapping::
mapGeneralWithPolarSingularity(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
// ========================================================================================
// General Cross section, with an axial polar singularity
// ========================================================================================
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  int i,axis,dir;
   
  // ********************************
  // **** general cross section *****
  // ********************************
  const real oneSixth=1./6.;
  const bool useLinearInterpolation = axialApproximation==linear || numberOfCrossSections <=2 ;
  const bool useCubicInterpolation = !useLinearInterpolation;
    
  Range T(tAxis,tAxis+domainDimension-2);   // tangential directions

  realArray rI(I,T), rS(I,1); 
  rI=r(I,T);      // tangential direction(s)
  rS=r(I,sAxis);  // axial directions
    
  realArray dr(I);
  realArray section(I);  // point i lies between x-section section(i) and section(i)+1
  realArray sI(I,1), srI(I,1,1);
  if( parameterization==index )
  {
    sI.reference(rS);
    srI=1.;
  }
  else
  {
    parameterMap->mapC(rS,sI(I,0),srI(I,0,0));
  }

  section = floor(sI*(numberOfCrossSections-1));    // point i lies between x-section section(i) and section(i)+1)
  section = min(max(section,0.),numberOfCrossSections-2.);
      
  dr = sI*(numberOfCrossSections-1.) - section;

  // num(cs) = number of points that lie between cross-section cs and cs+1
  IntegerArray num(Range(-2,numberOfCrossSections)); // extend on ends for convenience below
  num=0;
    
  // collect up all points between x-section cs and cs+1 into the intArray ia[cs]
  int cs;
  IntegerArray *ia = new IntegerArray [numberOfCrossSections+2];
  for( cs=0; cs<numberOfCrossSections; cs++ )
    ia[cs].redim(I);
    
  for( i=base; i<=bound; i++ )
  {
    cs=int( floor(section(i)+.5) );
    ia[cs](base+num(cs))=i;
    num(cs)++;
  }
  realArray tanhbr, tanhbrr;
  realArray csRad(I),csRadr(I,1);
    
  bool mappingHasAPolarSingularity, singular[2];
  singular[0]=getTypeOfCoordinateSingularity( Start,sAxis)==polarSingularity; 
  singular[1]=getTypeOfCoordinateSingularity( End  ,sAxis)==polarSingularity; 
  mappingHasAPolarSingularity = singular[0] || singular[1];
    
  realArray rho(I);
  if( singular[0] && singular[1] )
    rho=SQRT(fabs(1.-SQR(2.*rS-1.)));            // note fabs in case zeta > 1
  else if( singular[0] )
    rho=SQRT(fabs(1.-SQR(rS-1.)));
  else 
    rho=SQRT(fabs(1.-SQR(rS)));
      
  radiusSpline->map(sI,csRad,csRadr);
  tanhbr.redim(I);
  tanhbr=tanh(polarSingularityFactor*rho/csRad);
  rho=max(rho,REAL_EPSILON*10.);

  if( computeMapDerivative && params.coordinateType==cartesian )
  {
    // d(tanh)/dr 
    tanhbrr.redim(I);
    if( singular[0] && singular[1] )
      tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( (2.-4.*rS)/rho - rho*csRadr/csRad );
    else if( singular[0] )
      tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( (1. - rS )/rho - rho*csRadr/csRad );
    else if( singular[1] )
      tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( (    - rS)/rho - rho*csRadr/csRad );
  }

  realArray rhoIOverRho(I), rhoI(I), tanhbrOverRho(I);
  if( params.coordinateType==cylindrical )
  {
    // rhoI is the "rho" for the cylindrical coordinates.
    rhoI = (singular[0] && singular[1]) ? rho : SQRT(fabs(1.-SQR(2.*rS-1.)));   

    if( mappingHasAPolarSingularity )
    { 
      if( singular[0] && singular[1] )
	rhoIOverRho=1.;
	    
      where( rho>100.*REAL_EPSILON )
      {
	if( !(singular[0] && singular[1]) )
	  rhoIOverRho=rhoI/rho;
	tanhbrOverRho=tanhbr/rhoI;
      }
      otherwise()
      {
	if( !(singular[0] && singular[1]) )
	  rhoIOverRho=SQRT(2.);
	tanhbrOverRho=polarSingularityFactor/(csRad*rhoIOverRho);  // tanh(polarSingularityFactor*rho/csRad)/rhoI
      }
      // d(tanh)/dr * (-rhoI )
      tanhbrr.redim(I); // *wdh* 100214
      if( singular[0] && singular[1] )
	tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( (4.*rS-2.) + rho*rho*csRadr/csRad )*rhoIOverRho;
      else if( singular[0] )
	tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( ( rS-1.  ) + rho*rho*csRadr/csRad )*rhoIOverRho;
      else if( singular[1] )
	tanhbrr=(1.-tanhbr*tanhbr)*(polarSingularityFactor/csRad)*( (   rS)    + rho*rho*csRadr/csRad )*rhoIOverRho;
    }
  }
    
  realArray xc0,xc1,xc2,xc3, xcr0,xcr1,xcr2,xcr3;
  realArray xct0,xct1,xct2,xct3;

  // evaluate all points between x-section cs and cs+1
  for( cs=0; cs<numberOfCrossSections-1; cs++ )
  {
    if( num(cs)>0 )
    {
      const IntegerArray & ib = evaluate(ia[cs](Range(base,num(cs)-1+base)));
      const realArray & dri = dr(ib);
      Range R=ib.dimension(0);
      realArray rT(R,T);
      const realArray & tanhbrrIb = tanhbrr(ib);
      
      for( dir=0; dir<domainDimension-1; dir++ )
	rT(R,dir) = r(ib,tAxis+dir);
	  
      xc1.redim(R,rangeDimension);
      xc2.redim(R,rangeDimension);
      if( computeMapDerivative )
      {
	xcr1.redim(R,rangeDimension,domainDimension);
	xcr2.redim(R,rangeDimension,domainDimension);
      }
      // evaluate the cross sections:
      crossSection[cs  ]->map(rT,xc1,xcr1); 
      crossSection[cs+1]->map(rT,xc2,xcr2); 
      if( useCubicInterpolation )
      {
	// 2 more cross-sections needed for cubic case
	if( cs!=0 )
	{
	  xc0.redim(R,rangeDimension);
	  if( computeMapDerivative )
	    xcr0.redim(R,rangeDimension,domainDimension);
	  crossSection[cs-1]->map(rT,xc0,xcr0); 
	}
	if( cs < numberOfCrossSections-2 )
	{
	  xc3.redim(R,rangeDimension);
	  if( computeMapDerivative )
	    xcr3.redim(R,rangeDimension,domainDimension);
	  crossSection[cs+2]->map(rT,xc3,xcr3); 
	}
      }
      // adjust the cross-sections to approach a point on the axial ends:
      xct1.redim(R,rangeDimension);
      xct2.redim(R,rangeDimension);
      if( useCubicInterpolation )
      {
	xct0.redim(R,rangeDimension);
	xct3.redim(R,rangeDimension);
      }
      for( axis=0; axis<rangeDimension; axis++ )
      {
	if( computeMapDerivative )
	{
	  xct1(R,axis) = (xc1(R,axis)-centroid(axis,cs  ))*tanhbrr(ib);
	  xct2(R,axis) = (xc2(R,axis)-centroid(axis,cs+1))*tanhbrr(ib);
	  if( useCubicInterpolation )
	  {
	    xct0(R,axis) = (xc0(R,axis)-centroid(axis,cs-1))*tanhbrr(ib);
	    xct3(R,axis) = (xc3(R,axis)-centroid(axis,cs+2))*tanhbrr(ib);
	  }
	  if( params.coordinateType==cartesian )
	  {
	    for( dir=0; dir<domainDimension-1; dir++ )
	    {
	      xcr1(R,axis,dir)=xcr1(R,axis,dir)*tanhbr(ib);
	      xcr2(R,axis,dir)=xcr2(R,axis,dir)*tanhbr(ib);
	      if( useCubicInterpolation )
	      {
		xcr0(R,axis,dir)=xcr0(R,axis,dir)*tanhbr(ib);
		xcr3(R,axis,dir)=xcr3(R,axis,dir)*tanhbr(ib);
	      }
	    }
	  }
	}
	xc1(R,axis)=(xc1(R,axis)-centroid(axis,cs  ))*tanhbr(ib) + centroid(axis,cs);
	xc2(R,axis)=(xc2(R,axis)-centroid(axis,cs+1))*tanhbr(ib) + centroid(axis,cs+1);
	if( useCubicInterpolation )
	{
	  xc0(R,axis)=(xc0(R,axis)-centroid(axis,cs-1))*tanhbr(ib) + centroid(axis,cs-1);
	  xc3(R,axis)=(xc3(R,axis)-centroid(axis,cs+2))*tanhbr(ib) + centroid(axis,cs+2);
	}
      }
	
      if( computeMap )
      {
	for( axis=0; axis<rangeDimension; axis++ )
	{
	  if( useLinearInterpolation )
	    x(ib,axis) = (1.-dri)*xc1(R,axis) + dri*xc2(R,axis);
	  else
	  {
	    // cubic interpolation
	    // Use quadratic approximations at the ends.
	    if( cs>0 && cs<numberOfCrossSections-2 )
	      x(ib,axis)=q03(dri)*xc0(R,axis)+q13(dri)*xc1(R,axis)+q23(dri)*xc2(R,axis)+q33(dri)*xc3(R,axis);
	    else if( cs==0 )
	      x(ib,axis)=q02a(dri)*xc1(R,axis)+q12a(dri)*xc2(R,axis)+q22a(dri)*xc3(R,axis);
	    else
	      x(ib,axis)=q02b(dri)*xc0(R,axis)+q12b(dri)*xc1(R,axis)+q22b(dri)*xc2(R,axis);
	  }
	}
      }
      if( computeMapDerivative )
      {
	switch (params.coordinateType)
	{
	case cartesian:  // mapping returned in cartesian form
	{
	  for( axis=0; axis<rangeDimension; axis++ )
	  {
	    if( useLinearInterpolation )
	    {
	      xr(ib,axis,sAxis) = (xc2(R,axis)-xc1(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.)+
                      (1.-dr(ib))*xct1(R,axis) + dr(ib)*xct2(R,axis);
	      for( dir=0; dir<domainDimension-1; dir++ )
		xr(ib,axis,tAxis+dir) = (1.-dr(ib))*xcr1(R,axis,dir) + dr(ib)*xcr2(R,axis,dir);
	    }
	    else
	    {
	      // cubic 
	      if( cs>0 && cs<numberOfCrossSections-2 )
	      {
		xr(ib,axis,sAxis) = 
		  (q03d(dri)*xc0(R,axis) + 
		   q13d(dri)*xc1(R,axis) + 
		   q23d(dri)*xc2(R,axis) +
		   q33d(dri)*xc3(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.) +
		  q03(dri)*xct0(R,axis)+q13(dri)*xct1(R,axis)+
		  q23(dri)*xct2(R,axis)+q33(dri)*xct3(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		  xr(ib,axis,tAxis+dir)=
		    q03(dri)*xcr0(R,axis,dir) + 
		    q13(dri)*xcr1(R,axis,dir) + 
		    q23(dri)*xcr2(R,axis,dir) +
		    q33(dri)*xcr3(R,axis,dir);
	      }
	      else if( cs==0 )
	      {
		xr(ib,axis,sAxis) = 
		  (q02ad(dri)*xc1(R,axis) + 
		   q12ad(dri)*xc2(R,axis) +
		   q22ad(dri)*xc3(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.) +
                     q02a(dri)*xct1(R,axis)+q12a(dri)*xct2(R,axis)+q22a(dri)*xct3(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		  xr(ib,axis,tAxis+dir) = 
		    q02a(dri)*xcr1(R,axis,dir) + 
		    q12a(dri)*xcr2(R,axis,dir) +
		    q22a(dri)*xcr3(R,axis,dir);
	      }
	      else
	      {
		xr(ib,axis,sAxis) = 
		  (q02bd(dri)*xc0(R,axis) + 
		   q12bd(dri)*xc1(R,axis) + 
		   q22bd(dri)*xc2(R,axis))*srI(ib,0,0)*(numberOfCrossSections-1.) +
                    q02b(dri)*xct0(R,axis)+q12b(dri)*xct1(R,axis)+q22b(dri)*xct2(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		  xr(ib,axis,tAxis+dir) = 
		    q02b(dri)*xcr0(R,axis,dir) + 
		    q12b(dri)*xcr1(R,axis,dir) + 
		    q22b(dri)*xcr2(R,axis,dir);
	      }
	    }
	  }
	  break;
	}  // end case cartesian
	case cylindrical: // return -rhoI*d()/ds and (1/rhoI)*d()/d(theta) [and d()/dr]
	{
	  for( axis=0; axis<rangeDimension; axis++ )
	  {
	    if( useLinearInterpolation )
	    {
	      xr(ib,axis,sAxis) = (xc2(R,axis)-xc1(R,axis))*srI(ib,0,0)*rhoI*(-(numberOfCrossSections-1.)) +
		(1.-dr(ib))*xct1(R,axis) + dr(ib)*xct2(R,axis);
	      for( dir=0; dir<domainDimension-1; dir++ )
		xr(ib,axis,tAxis+dir)=( (1.-dr(ib))*xcr1(R,axis,dir)+dr(ib)*xcr2(R,axis,dir) )*tanhbrOverRho(ib);
	    }
	    else
	    {
	      // cubic
	      const realArray factor = mappingHasAPolarSingularity ? tanhbrOverRho(ib) : evaluate(1./rhoI);
	      if( cs>0 && cs<numberOfCrossSections-2 )
	      {
		xr(ib,axis,sAxis) = 
		  (q03d(dri)*xc0(R,axis) + 
		   q13d(dri)*xc1(R,axis) + 
		   q23d(dri)*xc2(R,axis) +
		   q33d(dri)*xc3(R,axis))*srI(ib,0,0)*rhoI*(-(numberOfCrossSections-1.)) +
                     q03(dri)*xct0(R,axis)+q13(dri)*xct1(R,axis)+q23(dri)*xct2(R,axis)+q33(dri)*xct3(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		{
		  xr(ib,axis,tAxis+dir)=
		    (q03(dri)*xcr0(R,axis,dir) + 
		     q13(dri)*xcr1(R,axis,dir) + 
		     q23(dri)*xcr2(R,axis,dir) +
		     q33(dri)*xcr3(R,axis,dir))*factor;
		}
	      }
	      else if( cs==0 )
	      {
		xr(ib,axis,sAxis) = 
		  (q02ad(dri)*xc1(R,axis) + 
		   q12ad(dri)*xc2(R,axis) +
		   q22ad(dri)*xc3(R,axis))*srI(ib,0,0)*rhoI*(-(numberOfCrossSections-1.)) +
                       q02a(dri)*xct1(R,axis)+q12a(dri)*xct2(R,axis)+q22a(dri)*xct3(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		{
		  xr(ib,axis,tAxis+dir) = 
		    (q02a(dri)*xcr1(R,axis,dir) + 
		     q12a(dri)*xcr2(R,axis,dir) +
		     q22a(dri)*xcr3(R,axis,dir))*factor;
		}
	      }
	      else
	      {
		xr(ib,axis,sAxis) = 
		  (q02bd(dri)*xc0(R,axis) + 
		   q12bd(dri)*xc1(R,axis) + 
		   q22bd(dri)*xc2(R,axis))*srI(ib,0,0)*rhoI*(-(numberOfCrossSections-1.)) + 
                     q02b(dri)*xct0(R,axis)+q12b(dri)*xct1(R,axis)+q22b(dri)*xct2(R,axis);
		for( dir=0; dir<domainDimension-1; dir++ )
		{
		  xr(ib,axis,tAxis+dir) = 
		    (q02b(dri)*xcr0(R,axis,dir) + 
		     q12b(dri)*xcr1(R,axis,dir) + 
		     q22b(dri)*xcr2(R,axis,dir))*factor;
		}
	      }
	    }
	  }
	  break;
	}  // end case cylindrical
	}  // end switch
      }
    }
  }
  delete [] ia;

}
void CrossSectionMapping::
mapBuiltin(const realArray & r, realArray & x, realArray & xr, MappingParameters & params)
// ===========================================================================================
//   Built-in cross section types : ellipse, joukowsky
// ===========================================================================================
{
  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  // real scale=twoPi*(endAngle-startAngle);
  real rad=outerRadius-innerRadius;
  real sScale=2.*(endS-startS);



  realArray rI(I), rS(I); 
  rI=r(I,tAxis);
  rS=r(I,sAxis);  // axial directions
   
  if( crossSectionType==ellipse )
  {

    realArray zeta(I), rho(I);

    zeta=sScale*rS-(1.-2.*startS);        // 2*( (endS-startS)*r+startS ) -1
    rho=SQRT(fabs(1.-SQR(zeta)));            // note fabs in case zeta > 1
    const realArray radius = domainDimension==3 ? evaluate( RADIUS(r(I,axis3)) ) : rho;
    const realArray & cos2 = evaluate( cos(twoPi*rI) );
    const realArray & sin2 = evaluate( sin(twoPi*rI) );
    if( computeMap )
    {
      if( domainDimension==2 )
      {
	x(I,axis1)=(a*innerRadius)*rho*cos2+x0;
	x(I,axis2)=(b*innerRadius)*rho*sin2+y0;
	x(I,axis3)=(c*innerRadius)*zeta+z0;
      }
      else
      {
	x(I,axis1)=a*radius*rho*cos2+x0;
	x(I,axis2)=b*radius*rho*sin2+y0;
	x(I,axis3)=c*radius*zeta+z0;
      }
    }
    if( computeMapDerivative )
    {
      switch (params.coordinateType)
      {
      case cartesian:  // mapping returned in cartesian form
        rho=max(rho,REAL_EPSILON);   // prevent division by zero
	if( domainDimension==2 )
	{
	  xr(I,axis1,sAxis)=(-sScale*a*innerRadius)*zeta/rho*cos2;
	  xr(I,axis2,sAxis)=(-sScale*b*innerRadius)*zeta/rho*sin2;
	  xr(I,axis3,sAxis)=(sScale*c*innerRadius);

	  xr(I,axis1,tAxis)=(-twoPi*a*innerRadius)*rho*sin2;
	  xr(I,axis2,tAxis)=(+twoPi*b*innerRadius)*rho*cos2;
	  xr(I,axis3,tAxis)=0;
	}
	else
	{
	  xr(I,axis1,sAxis)=(-sScale*a)*radius*zeta/rho*cos2;
	  xr(I,axis2,sAxis)=(-sScale*b)*radius*zeta/rho*sin2;
	  xr(I,axis3,sAxis)=(+sScale*c)*radius;

	  xr(I,axis1,tAxis)=(-twoPi*a)*radius*rho*sin2;
	  xr(I,axis2,tAxis)=(+twoPi*b)*radius*rho*cos2;
	  xr(I,axis3,tAxis)=0.;

	  xr(I,axis1,axis3)=(a*rad)*rho*cos2;
	  xr(I,axis2,axis3)=(b*rad)*rho*sin2;
	  xr(I,axis3,axis3)=(c*rad)*zeta;
	}
	break;
      case cylindrical:  // return -rho*d()/ds and (1/rho)*d()/d(theta) and d()/dr
	if( domainDimension==2 )
	{

	  xr(I,axis1,sAxis)=(+sScale*a*innerRadius)*zeta*cos2;
	  xr(I,axis2,sAxis)=(+sScale*b*innerRadius)*zeta*sin2;
	  xr(I,axis3,sAxis)=(-sScale*c*innerRadius)*rho;

	  xr(I,axis1,tAxis)=(-twoPi*a*innerRadius)*sin2;
	  xr(I,axis2,tAxis)=(+twoPi*b*innerRadius)*cos2;
	  xr(I,axis3,tAxis)=0;
	}
	else
	{
	  xr(I,axis1,sAxis)=(+sScale*a)*radius*zeta*cos2;
	  xr(I,axis2,sAxis)=(+sScale*b)*radius*zeta*sin2;
	  xr(I,axis3,sAxis)=(-sScale*c)*radius*rho;

	  xr(I,axis1,tAxis)=(-twoPi*a)*radius*sin2;
	  xr(I,axis2,tAxis)=(+twoPi*b)*radius*cos2;
	  xr(I,axis3,tAxis)=0.;

	  xr(I,axis1,axis3)=(a*rad)*rho*cos2;
	  xr(I,axis2,axis3)=(b*rad)*rho*sin2;
	  xr(I,axis3,axis3)=(c*rad)*zeta;
	}
	break;
      default:
	cerr << "CrossSectionMapping::map: ERROR not implemented for coordinateType = " 
	  << params.coordinateType << endl;
        return;
      }
    }
  }
  else if( crossSectionType==joukowsky )
  {
    real thb; thb=tanh(joukowskyBeta*1.);
    real thbp; thbp=joukowskyBeta*(1.-thb*thb);
    
    // am=i*d*cexp(i*delta);
    real amRe; amRe=-joukowskyD*sin(joukowskyDelta);
    real amIm; amIm= joukowskyD*cos(joukowskyDelta);


    const realArray & cos2 = evaluate( cos(twoPi*rI) );
    const realArray & sin2 = evaluate( sin(twoPi*rI) );

    // cexpt=cos2-i*sin2;

    const realArray & zeta= evaluate( sScale*rS-(1.-2.*startS) );        // 2*( (endS-startS)*r+startS ) -1
    const realArray & rho= evaluate( SQRT(fabs(1.-SQR(zeta))) );            // note fabs in case zeta > 1

    const realArray & th  = evaluate( tanh(joukowskyBeta*rho) );
    const realArray & thp = evaluate( joukowskyBeta*(1.-th*th) );
    //  thro=tanh(beta*ro)/ro
    realArray thro;
    where( rho>REAL_EPSILON*100. )
    {
      thro=th/rho;
    }
    otherwise()
    {
      thro=joukowskyBeta;
    }
    //........fz(ro) = ( g(ro) - ro * g'(1) )**2
    //        fzp = fz.zeta = fz.ro (-zeta/ro)
    //        g(ro) = tanh( beta*ro )
    realArray f,fz,fzp;
    f=evaluate( th-thbp*rho );
    fz=evaluate( f*f );
    fzp=evaluate( (-2.)*(thp-thbp)*(thro-thbp)*(zeta) );
    if( false )
    {
      f=1.;
      fz=1.;
      fzp=0.;
    }
    
    // printf(" amRe=%e, amIm=%e \n",amRe,amIm);
    // fz.display("Here is fz");

    // zpz=a*cexpt+am*fz;  cexpt=cos-isin
    // const realArray & zpzRe=evaluate(   joukowskyA *cos2+amRe*fz );
    // const realArray & zpzIm=evaluate( (-joukowskyA)*sin2+amIm*fz );
    
    realArray zpzRe,zpzIm;
    zpzRe=joukowskyA *cos2+amRe*fz;
    zpzIm=(-joukowskyA)*sin2+amIm*fz;
    
    // z=zpz+1./zpz;
    const realArray & zpzNormI=evaluate( 1./(zpzRe*zpzRe+zpzIm*zpzIm) );
    const realArray & zRe=evaluate( zpzRe+zpzRe*zpzNormI );
    const realArray & zIm=evaluate( zpzIm-zpzIm*zpzNormI );
    if( computeMap )
    {
      x(I,axis1)=joukowskyLength*zeta;
      x(I,axis2)=zRe*th;
      x(I,axis3)=zIm*th;
    }
    if( computeMapDerivative )
    {
      // dz=1.-1./(zpz*zpz);
      const realArray & dzRe=evaluate( 1.-( zpzRe*zpzRe-zpzIm*zpzIm )*zpzNormI*zpzNormI );
      const realArray & dzIm=evaluate( ( 2.*zpzRe*zpzIm )*zpzNormI*zpzNormI );
      // *wdh* z1=a*i*cexpt*dz;
      // z1=-a*i*cexpt*dz;
      const realArray & z1Re=evaluate( ( joukowskyA*twoPi)*(cos2*dzIm-sin2*dzRe) );
      const realArray & z1Im=evaluate( (-joukowskyA*twoPi)*(cos2*dzRe+sin2*dzIm) );
      // z2=fzp*am*dz;
      const realArray & z2Re=evaluate( fzp*(amRe*dzRe-amIm*dzIm) );
      const realArray & z2Im=evaluate( fzp*(amRe*dzIm+amIm*dzRe) );

      if( params.coordinateType==cartesian )
      {
	xr(I,axis1,sAxis)=joukowskyLength*sScale;
	xr(I,axis1,tAxis)=0.;

        const realArray & rhoM = max(rho,REAL_MIN*100.);
	xr(I,axis2,sAxis)=z2Re*th+(-sScale)*zRe*thp*zeta/rho;
	xr(I,axis2,tAxis)=z1Re*th;

	xr(I,axis3,sAxis)=z2Im*th+(-sScale)*zIm*thp*zeta/rho;
	xr(I,axis3,tAxis)=z1Im*th;
      }
      else if( params.coordinateType==cylindrical )  // return -rho*d()/ds and (1/rho)*d()/d(theta) and d()/dr
      {
	xr(I,axis1,sAxis)=0.;
	xr(I,axis2,sAxis)=z1Re*thro;
	xr(I,axis3,sAxis)=z1Im*thro;
	xr(I,axis1,tAxis)=joukowskyLength*rho;
	xr(I,axis2,tAxis)=z2Re*th*rho-zRe*thp*zeta;
	xr(I,axis3,tAxis)=z2Im*th*rho-zIm*thp*zeta;
      }
      else
      {
	cerr << "CrossSectionMapping::map: ERROR not implemented for coordinateType = " 
	  << params.coordinateType << endl;
        return;      
      }
    }
  }
  
}
//======================================================================
// Here is the inverse for the ellipsoid
//======================================================================
void CrossSectionMapping::
basicInverse(const realArray & x, realArray & r, realArray & rx, MappingParameters & params)
{
  Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

  real startTheta=0.;

  const real aInverse=1./a, bInverse=1./b, cInverse=1./c;
  
  real inverseScale=1./twoPi;
  real rad=outerRadius-innerRadius;
  real inverseRad=1./rad;
  real sScale = 2.*(endS-startS);

  real sScaleInverse = 1./sScale;
  real eps = REAL_MIN*100.;

  const realArray & xn = evaluate( aInverse*(x0-x(I,axis1)) );
  const realArray & yn = evaluate( bInverse*(y0-x(I,axis2)) );
  const realArray & zn = evaluate( cInverse*(z0-x(I,axis3)) );
  
  realArray radius = evaluate( sqrt(pow(xn,2)+pow(yn,2)+pow(zn,2)) );
  where( radius==0. )
    radius=eps;

  
  const realArray & theta = evaluate(atan2(yn,xn)+Pi); 

  if( computeMap )
  {
    r(I,tAxis)=(theta-twoPi*startTheta)*inverseScale;   
    if( getIsPeriodic(tAxis) )
      r(I,tAxis)=fmod(r(I,tAxis)+1.,1.);  // map back to [0,1]
    r(I,sAxis)=(zn/radius+(-1.+2*startS))*(-sScaleInverse);
    if( domainDimension==3 )
      r(I,axis3)=(radius-innerRadius)*inverseRad;
  }


  realArray temp;
  if( computeMapDerivative )
  {
    realArray x2y2=evaluate(SQR(xn)+SQR(yn));
    x2y2=max(x2y2,eps);

    switch (params.coordinateType)
    {
     case cartesian:  // mapping returned in cartesian form
                     // derivatives: ( d/d(r1), d/d(r2), d/d(r3) )
	
      temp=inverseScale/x2y2;
      rx(I,tAxis,axis1)=yn*( aInverse)*temp;
      rx(I,tAxis,axis2)=xn*(-bInverse)*temp;
      rx(I,tAxis,axis3)=0.;

      temp = (-sScaleInverse)/(radius*radius*radius);
      rx(I,sAxis,axis3)=-x2y2*cInverse*temp;
      rx(I,sAxis,axis1)=zn*xn*aInverse*temp;
      rx(I,sAxis,axis2)=zn*yn*bInverse*temp;

      if( domainDimension==3 )
      {
        temp=(-1./rad)/radius;
	rx(I,axis3,axis1)=xn*aInverse*temp;
	rx(I,axis3,axis2)=yn*bInverse*temp;
	rx(I,axis3,axis3)=zn*cInverse*temp;
      }
      break;

    case cylindrical: 
        // Mapping returned in cylindrical form
        // Return: (-1/rho) ds/d(x_i),  rho d(theta)/d(x_i), dr/d(x_i)

      rx(I,tAxis,axis1)=sin(theta)*(-aInverse*inverseScale)/radius;   // *rho
      rx(I,tAxis,axis2)=cos(theta)*( bInverse*inverseScale)/radius;   // *rho
      rx(I,tAxis,axis3)=0.;

      assert( computeMap );
      temp=sScale*r(I,sAxis)-(1.-2.*startS);        // zeta
      temp=SQRT(fabs(1.-SQR(temp)));           // rho

      rx(I,sAxis,axis3)=(cInverse)*(-sScaleInverse)*temp/radius;        // *(-1/rho)
      rx(I,sAxis,axis1)=zn*cos(theta)*aInverse*(-sScaleInverse)/(radius*radius);   // *(-1/rho)
      rx(I,sAxis,axis2)=zn*sin(theta)*bInverse*(-sScaleInverse)/(radius*radius);   // *(-1/rho)

      if( domainDimension==3 )
      {
        temp=(-1./rad)/radius;
	rx(I,axis3,axis1)=xn*aInverse*temp;
	rx(I,axis3,axis2)=yn*bInverse*temp;
	rx(I,axis3,axis3)=zn*cInverse*temp;
      }
/* ----
      rx(I,axis2,axis1)=-sin(theta(I))*inverseScale/(radius);  // *sin(phi)
      rx(I,axis2,axis2)= cos(theta(I))*inverseScale/(radius);  // *sin(phi)
      rx(I,axis2,axis3)=0.;

      rx(I,axis1,axis3)=-(SQR(x(I,axis1)-x0)+SQR(x(I,axis2)-y0))*cos(theta(I))*sScaleInverse
	  /(SQR(radius)*(x(I,axis1)-x0));
      rx(I,axis1,axis1)=(x(I,axis3)-z0)*cos(theta(I))*sScaleInverse/(SQR(radius));
      rx(I,axis1,axis2)=(x(I,axis3)-z0)*sin(theta(I))*sScaleInverse/(SQR(radius));

      if( domainDimension==3 )
      {
	rx(I,axis3,axis1)=(x(I,axis1)-x0)/(rad*radius);
	rx(I,axis3,axis2)=(x(I,axis2)-y0)/(rad*radius);
	rx(I,axis3,axis3)=(x(I,axis3)-z0)/(rad*radius);
      }
--- */
      break;
    default:
      cerr << "CrossSectionMapping::map: ERROR not implemented for coordinateType = " << params.coordinateType << endl;
      exit(1);
    }
  }
}



#undef RADIUS



int CrossSectionMapping::
get( const GenericDataBase & dir, const aString & name)
// ================================================================
// /Description:
//   Get a mapping from the database.
// ================================================================
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( (debug/4) % 2 )
    cout << "Entering CrossSectionMapping::get" << endl;

  subDir.get( CrossSectionMapping::className,"className" ); 
  if( CrossSectionMapping::className != "CrossSectionMapping" )
  {
    cout << "CrossSectionMapping::get ERROR in className!" << endl;
    cout << "className from the database = " << CrossSectionMapping::className << endl;
  }

  for( int i=0; i<numberOfCrossSections; i++ )
  {
    if( crossSection[i]!=NULL && crossSection[i]->decrementReferenceCount() == 0)
    {
      delete crossSection[i];
    }
  }
  delete [] crossSection;
  crossSection=NULL;

  subDir.get( tAxis,"tAxis" );
  subDir.get( sAxis,"sAxis" );
  int temp;
  subDir.get( temp,"crossSectionType" ); crossSectionType=(CrossSectionTypes)temp;
  subDir.get( numberOfCrossSections,"numberOfCrossSections" );
  aString curveClassName;
  char buff[80];

  if( numberOfCrossSections>0 )
    crossSection =  new Mapping* [numberOfCrossSections];
    
  for( int cs=0; cs<numberOfCrossSections; cs++ )
  {
    curveClassName="";
    subDir.get(curveClassName,sPrintF("curveClassName%i",cs));
    // printf("CrossSectionMapping::get: cross-section=%i className=%s\n",cs,(const char*)curveClassName);

    crossSection[cs] = Mapping::makeMapping( curveClassName ); // ***** this does a new -- who will delete? ***
    
    if( crossSection[cs]==NULL )
    {
      cout << "CrossSectionMapping::get:ERROR unable to make the mapping with className = " 
	   << (const char *)curveClassName << endl;
      {throw "error";}
    }
    
    crossSection[cs]->get( subDir,sPrintF(buff,"cross-section %i",cs) );
    crossSection[cs]->incrementReferenceCount();

  }
  
  subDir.get( temp,"axialApproximation" ); axialApproximation=(AxialApproximation)temp;
  subDir.get( temp,"parameterization" ); parameterization=(Parameterization)temp;
  subDir.getDistributed( s,"s" );
  subDir.getDistributed( centroid,"centroid" );
  subDir.getDistributed( csRadius,"csRadius" );

  int mapExists;
  subDir.get( mapExists , "parameterMapExists");
  if( mapExists )
  {
    subDir.get(curveClassName,"curveClassName");
    parameterMap = Mapping::makeMapping( curveClassName ); 
    if( parameterMap==0 )
    {
      cout << "CrossSectionMapping::get:ERROR unable to make the mapping with className = " 
	   << (const char *)curveClassName << endl;
      {throw "error";}
    }
    parameterMap->get( subDir,"parameterMap" );
  }
  subDir.get( mapExists , "radiusSplineExists");
  if( mapExists )
  {
    subDir.get(curveClassName,"curveClassName");
    radiusSpline = (SplineMapping*) Mapping::makeMapping( curveClassName ); 
    if( radiusSpline==0 )
    {
      cout << "CrossSectionMapping::get:ERROR unable to make the mapping with className = " 
	   << (const char *)curveClassName << endl;
      {throw "error";}
    }
    radiusSpline->get( subDir,"radiusSpline" );
  }

  subDir.get( polarSingularityFactor,"polarSingularityFactor" );

  subDir.get( innerRadius,"innerRadius" );
  subDir.get( outerRadius,"outerRadius" );
  subDir.get( x0,"x0" );
  subDir.get( y0,"y0" );
  subDir.get( z0,"z0" );
  subDir.get( length,"length" );
  subDir.get( startAngle,"startAngle" );
  subDir.get( endAngle,"endAngle" );
  subDir.get( a,"a" );
  subDir.get( b,"b" );
  subDir.get( c,"c" );
  subDir.get( startS,"startS" );
  subDir.get( endS,"endS" );
  subDir.get( joukowskyDelta,"joukowskyDelta" );
  subDir.get( joukowskyD,"joukowskyD" );
  subDir.get( joukowskyA,"joukowskyA" );
  subDir.get( joukowskyLength,"joukowskyLength" );
  subDir.get( joukowskyBeta,"joukowskyBeta" );

  Mapping::get( subDir, "Mapping" );
  mappingHasChanged();
  delete &subDir;

  return 0;
}
int CrossSectionMapping::
put( GenericDataBase & dir, const aString & name) const
// ================================================================
// /Description:
//   Put a mapping to a database.
// ================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( CrossSectionMapping::className,"className" );

  subDir.put( tAxis,"tAxis" );
  subDir.put( sAxis,"sAxis" );
  subDir.put( (int)crossSectionType,"crossSectionType" );
  subDir.put( numberOfCrossSections,"numberOfCrossSections" );
  char buff[80];
  for( int cs=0; cs<numberOfCrossSections; cs++ )
  {
//     printf("CrossSectionMapping::put: cross-section=%i className=%s\n",cs,
//             (const char*)crossSection[cs]->getClassName());

    subDir.put(crossSection[cs]->getClassName(),sPrintF("curveClassName%i",cs));
    crossSection[cs]->put( subDir,sPrintF(buff,"cross-section %i",cs) );

  }

  subDir.put( (int)axialApproximation,"axialApproximation" );
  subDir.put( (int)parameterization,"parameterization" );
  subDir.putDistributed( s,"s" );
  subDir.putDistributed( centroid,"centroid" );
  subDir.putDistributed( csRadius,"csRadius" );

  int mapExists = parameterMap!=0;
  subDir.put( mapExists , "parameterMapExists");
  if( mapExists )
  {
    subDir.put(parameterMap->getClassName(),"curveClassName");
    parameterMap->put( subDir,"parameterMap" );
  }
  mapExists = radiusSpline!=0;
  subDir.put( mapExists , "radiusSplineExists");
  if( mapExists )
  {
    subDir.put(radiusSpline->getClassName(),"curveClassName");
    radiusSpline->put( subDir,"radiusSpline" );
  }

  subDir.put( polarSingularityFactor,"polarSingularityFactor" );

  subDir.put( innerRadius,"innerRadius" );
  subDir.put( outerRadius,"outerRadius" );
  subDir.put( x0,"x0" );
  subDir.put( y0,"y0" );
  subDir.put( z0,"z0" );
  subDir.put( length,"length" );
  subDir.put( startAngle,"startAngle" );
  subDir.put( endAngle,"endAngle" );
  subDir.put( a,"a" );
  subDir.put( b,"b" );
  subDir.put( c,"c" );
  subDir.put( startS,"startS" );
  subDir.put( endS,"endS" );
  subDir.put( joukowskyDelta,"joukowskyDelta" );
  subDir.put( joukowskyD,"joukowskyD" );
  subDir.put( joukowskyA,"joukowskyA" );
  subDir.put( joukowskyLength,"joukowskyLength" );
  subDir.put( joukowskyBeta,"joukowskyBeta" );

  Mapping::put( subDir, "Mapping" );
  delete & subDir;
  return 0;
}

Mapping* CrossSectionMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==CrossSectionMapping::className )
    retval = new CrossSectionMapping();
  return retval;
}

aString CrossSectionMapping::
getClassName() const
{
  return CrossSectionMapping::className;
}

//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int CrossSectionMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!CrossSectionMapping",
      ">cross section type",
        "general",
        "ellipse",
        "joukowsky",
      "<>parameterization",
        "arclength",
  	"index",
        "user defined",
        "change arclength spline parameters",
        "change radius spline parameters",
      "<>axial approximation",
        "linear",
        "cubic",
      "<>general parameters",
        "polar singularity at start",
        "polar singularity at end",
        "no singularity at start",
        "no singularity at end",
        "polar singularity factor",
        "plot radius spline",
      "<>build in types",
        ">ellipse parameters",
          "centre for ellipse",
          "a,b,c for ellipse",
        "<>joukowsky parameters",
          "joukowsky delta",
          "joukowsky d",
          "joukowsky a",
          "joukowsky length",
          "joukowsky beta",
        "<start axial",
        "end axial",
        "start angle",
        "end angle",
        "inner radius",
        "outer radius",
      "<surface or volume (toggle)",
      "plot cross sections (toggle)",
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
      "cross section type",
      "  general         : build a mapping from a set of cross sections",
      "  ellipse         : build an ellipsoid",
      "  joukowsky       : build a wing with Joukowsky airfoil cross sections",
      "parameterization  : choose the parameterization for a general cross section",
      "  arclength",
      "  index",
      "  user defined",
      "  change arclength spline parameters",
      "axial approximation",
      "  linear          : use a piecewise linear approximation in the axial direction",
      "  cubic           : use a piecewise cubic  approximation in the axial direction",
      "general parameters",
      "  polar singularity at start",
      "  polar singularity at end",
      "  no singularity at start",
      "  no singularity at end",
      "  polar singularity factor",
      "  plot radius spline",
      "build in types             : parameters for the ellipse or joukowsky",
      "  ellipse parameters",
      "    centre for ellipse     : specify (x0,y0,z0) for the centre",
      "    a,b,c for ellipse      : length of axes for ellipse (also scaled by inner and outer radius)",
      "  joukowsky parameters",
      "    joukowsky delta",
      "    joukowsky d",
      "    joukowsky a",
      "    joukowsky length",
      "    joukowsky beta",
      "  start axial              : initial value for the axial variable s, in [0,1]",
      "  end axial                : final value for the axial variable s, in [0,1]",
      "  start angle              : Set the value where the angular variable starts, in [0,1]",
      "  end angle                : Set the value where the angular variable ends, in [0,1]",
      "  inner radius             : Specify the inner radius",
      "  outer radius             : Specify the outer radius",
      "surface or volume (toggle)",
      "plot cross sections (toggle)",
      " ",
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

  aString answer,answer2,line;
  int i;
  
  bool plotObject=TRUE;
  bool plotCrossSections=TRUE;
  
  GraphicsParameters params;
  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("CrossSection>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 
    if( answer=="general" || answer=="ellipse" || answer=="joukowsky" || answer=="banana" )
    {
      if( answer=="general" )
	crossSectionType=general;
      else if( answer=="ellipse" )
	crossSectionType=ellipse;
      else if( answer=="joukowsky" )
	crossSectionType=joukowsky;
      else if( answer=="banana" )
	crossSectionType=banana;
      else
      {
	cout << "Unknown cross section type! Choosing ellipse\n";
	crossSectionType=ellipse;
      }
      if( crossSectionType==ellipse )
      {
	if( startS==0. )
	  setTypeOfCoordinateSingularity( Start,sAxis,polarSingularity ); // s has a "polar" singularity
        else
	  setTypeOfCoordinateSingularity( Start,sAxis,noCoordinateSingularity ); // s has a "polar" singularity
	if( fabs(endS-1.)<REAL_EPSILON*5. )
	  setTypeOfCoordinateSingularity( End  ,sAxis,polarSingularity ); 
        else
	  setTypeOfCoordinateSingularity( End  ,sAxis,noCoordinateSingularity ); // s has a "polar" singularity
	setCoordinateEvaluationType( cylindrical,TRUE );  // Mapping can be evaluated in cylindrical coordinates

	setBoundaryCondition( Start,sAxis,0 ); 
	setBoundaryCondition( End  ,sAxis,0 );

	setBoundaryCondition( Start,axis3,1 );
	setBoundaryCondition( End  ,axis3,0 );
	if( fabs(endAngle-startAngle)-1. < REAL_EPSILON*10. )
	{
	  setIsPeriodic(tAxis, functionPeriodic );  
	  setBoundaryCondition( Start,tAxis,-1);
	  setBoundaryCondition( End  ,tAxis,-1);
	}
	else
	{
	  setBoundaryCondition( Start,tAxis,0 );
	  setBoundaryCondition( End  ,tAxis,0 );
	}
      }
      else if( crossSectionType==joukowsky )
      {
	domainDimension=2;
        // startS=.5;                      // just make a half wing by default
        // setGridDimensions( sAxis,21 );  // length
        // setGridDimensions( tAxis,41 );  // angle
        setGridDimensions( sAxis,41 );  // length
        setGridDimensions( tAxis,41 );  // angle

	if( startS==0. )
	  setTypeOfCoordinateSingularity( Start,sAxis,polarSingularity ); // s has a "polar" singularity
        else
	  setTypeOfCoordinateSingularity( Start,sAxis,noCoordinateSingularity ); // s has a "polar" singularity
	if(  fabs(endS-1.)<REAL_EPSILON*5. )
	  setTypeOfCoordinateSingularity( End  ,sAxis,polarSingularity );
        else
	  setTypeOfCoordinateSingularity( End  ,sAxis,noCoordinateSingularity ); // s has a "polar" singularity
	setCoordinateEvaluationType( cylindrical,TRUE );  // Mapping can be evaluated in cylindrical coordinates

	setBoundaryCondition( Start,sAxis,0 );  
	setBoundaryCondition( End  ,sAxis,0 );

	// setBoundaryCondition( Start,axis3,1 );
	// setBoundaryCondition( End  ,axis3,0 );
	if( fabs(endAngle-startAngle)-1. < REAL_EPSILON*10. )
	{
	  setIsPeriodic(tAxis, functionPeriodic );  
	  setBoundaryCondition( Start,tAxis,-1);
	  setBoundaryCondition( End  ,tAxis,-1);
	}
	else
	{
	  setBoundaryCondition( Start,tAxis,0 );
	  setBoundaryCondition( End  ,tAxis,0 );
	}
	  
      }
      else
      {
	setTypeOfCoordinateSingularity( Start,sAxis,noCoordinateSingularity ); 
	setTypeOfCoordinateSingularity( End  ,sAxis,noCoordinateSingularity ); 
	setIsPeriodic(tAxis, notPeriodic );  
	for( int axis=axis1; axis<domainDimension; axis++ )
	{
	  setBoundaryCondition( Start,axis,1 );
	  setBoundaryCondition( End  ,axis,1 );
	}
      }
      if( crossSectionType==general )
      {
	if( crossSection!=NULL )
	{
	  for( int i=0; i<numberOfCrossSections; i++ )
	  {
	    if( crossSection[i]!=NULL && crossSection[i]->decrementReferenceCount() == 0)
	    {
	      delete crossSection[i];
	    }
	  }
	  delete [] crossSection;
	  crossSection=NULL;
	}

	numberOfCrossSections=2;
	gi.inputString(line,sPrintF(buff,"Enter the number of cross sections (default=%i): ",numberOfCrossSections));
	if( line!="" ) sScanF(line,"%i",&numberOfCrossSections);
	if( numberOfCrossSections<1 )
	{
	  gi.outputString("ERROR: there must be at least 2 cross sections! \n");
	  continue;
	}
	crossSection= new Mapping* [numberOfCrossSections];

	// Make a menu with the Mapping names (only 3D curves or 3D planar surfaces)
	int num=mapInfo.mappingList.getLength();
	aString *menu2 = new aString[num+2];
	IntegerArray subListNumbering(num);
	int j=0;
	for( i=0; i<num; i++ )
	{
	  MappingRC & map = mapInfo.mappingList[i];
	  if(( map.getDomainDimension()==1 &&  map.getRangeDimension()==3 )||
	     ( map.getDomainDimension()==2 &&  map.getRangeDimension()==3 ))
	  {
	    subListNumbering(j)=i;
	    menu2[j++]=map.getName(mappingName);
	  }
	}
	if( j==0 )
	{
	  gi.outputString("crossSectionMapping::WARNING: There are no 3D curves  or surfaces to choose from");
	  continue;
	}
	menu2[j]="";   // null string terminates the menu
	
	bool sectionsArePeriodic=TRUE;
	for( i=0; i<numberOfCrossSections; i++ )
	{
	  int mapNumber = gi.getMenuItem(menu2,answer2,sPrintF(buff,"Enter cross section %i",i));
          if( mapNumber<0 )
	  {
	    cout << "Unknown response: " << answer2 << endl;
            gi.stopReadingCommandFile();
	    mapNumber = gi.getMenuItem(menu2,answer2,sPrintF(buff,"Enter cross section %i",i));
	  }
	  mapNumber=subListNumbering(mapNumber);  // map number in the original list

	  crossSection[i]=mapInfo.mappingList[mapNumber].mapPointer;
	  crossSection[i]->incrementReferenceCount();

	  sectionsArePeriodic=sectionsArePeriodic && 
                 ((bool)crossSection[i]->getIsPeriodic(axis1) || 
                  (crossSection[i]->getDomainDimension()>1 && (bool)crossSection[i]->getIsPeriodic(axis2)) );

          if( crossSection[i]->getDomainDimension() != crossSection[0]->getDomainDimension() ||
              crossSection[i]->getRangeDimension()  != crossSection[0]->getRangeDimension() )
	  {
            if( crossSection[i]->getDomainDimension() != crossSection[0]->getDomainDimension() )
  	      printf("CrossSectionMapping::ERROR: cross section %i does not have the same domainDimesion\n",i);
	    else
  	      printf("CrossSectionMapping::ERROR: cross section %i does not have the same rangeDimesion\n",i);
            printf("...try again\n");
	    i--;
	  }
	}
      
	delete [] menu2;
	setDomainDimension(crossSection[0]->getDomainDimension()+1);
	setRangeDimension(3);
	if( crossSection[0]->getDomainDimension()==1 )
	{
	  setGridDimensions(sAxis,max(11,numberOfCrossSections*5));
	  setGridDimensions(tAxis,crossSection[0]->getGridDimensions(axis1));
	  if( sectionsArePeriodic )
	    setIsPeriodic(tAxis,functionPeriodic);
	}
	else if( crossSection[0]->getDomainDimension()==2 )
	{
	  setGridDimensions(axis3,max(11,numberOfCrossSections*5));
	  for( int axis=axis1; axis<domainDimension-1; axis++ )
	  {
	    setGridDimensions(axis,crossSection[0]->getGridDimensions(axis));
	    setIsPeriodic(axis,crossSection[0]->getIsPeriodic(axis));
	    for( int side=Start; side<=End; side++ )
	      setTypeOfCoordinateSingularity(side,axis,crossSection[0]->getTypeOfCoordinateSingularity(side,axis));
	  }
	}
      }
      initialize();
      mappingHasChanged();
    }
    else if( answer=="arclength" || answer=="index" || answer=="user defined" )
    {
      if( crossSectionType!=general )
	printf("WARNING: You can only change the parameterization of a `general' cross section\n");
	
      if( answer=="arclength" )
	parameterization =arcLength;
      else if( answer=="index" )
	parameterization=index;
      
      if(parameterization==userDefined )
      {
	s.redim(numberOfCrossSections);  s=0;
	for( i=0; i<numberOfCrossSections; i++ )
	{
	  gi.inputString(line,sPrintF(buff,"Enter parameter value for point %i",i));
	  sScanF(line,"%e",&s(i));
	}
      }
      initialize();
      mappingHasChanged();
    }
    else if( answer=="linear" )
    {
      if( axialApproximation!=linear )
      {
	axialApproximation=linear;
        mappingHasChanged();
      }
    }
    else if( answer=="cubic" )
    {
      if( axialApproximation!=cubic )
      {
	axialApproximation=cubic;
        mappingHasChanged();
      }
    }
    else if( answer=="change arclength spline parameters" )
    {
      if( parameterMap==0 )
      {
	printf("CrossSectionMapping::The arclength spline has not been built yet\n");
      }
      else
      {
	SplineMapping & spline = (SplineMapping&) *parameterMap;
	gi.erase();
	spline.update(mapInfo);
	mappingHasChanged();
      }
    }
    else if( answer=="change radius spline parameters" )
    {
      if( radiusSpline==0 )
      {
	printf("CrossSectionMapping::The radius spline has not been built yet\n");
      }
      else
      {
	gi.erase();
	radiusSpline->update(mapInfo);
	mappingHasChanged();
      }
    }
    else if( answer=="plot radius spline" )
    {
      if( radiusSpline==0 )
      {
	printf("CrossSectionMapping::The radius spline has not been built yet\n");
      }
      else
      {
	gi.erase();
	radiusSpline->update(mapInfo);
	mappingHasChanged();
      }
    }
    else if( answer=="centre for annulus" ) 
    {
      gi.inputString(line,sPrintF(buff,"Enter (x0,y0,z0) for centre (default=(%e,%e,%e)): ",x0,y0,z0));
      if( line!="" ) sScanF(line,"%e %e %e",&x0,&y0,&z0);
      mappingHasChanged();
    }
    else if( answer=="a,b,c for ellipse" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the a,b,c (default=%e,%e,%e): ",a,b,c));
      if( line!="" ) sScanF( line,"%e %e %e",&a,&b,&c);
      mappingHasChanged();
    }
    else if( answer=="start axial" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the starting axial value, s,  in [0,1] (default=%e): ",startS));
      if( line!="" ) sScanF( line,"%e",&startS);
      mappingHasChanged();
    }
    else if( answer=="end axial" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the ending axial value, s, in [0,1] (default=%e): ",endS));
      if( line!="" ) sScanF( line,"%e",&endS);
      mappingHasChanged();
    }
    else if( answer=="start angle" || answer=="end angle" )
    {
      if( answer=="start angle" )
      {
	gi.inputString(line,sPrintF(buff,"Enter the starting `angle' in [0,1] (default=%e): ",startAngle));
	if( line!="" ) sScanF( line,"%e",&startAngle);
	mappingHasChanged();
      }
      else if( answer=="end angle" )
      {
	gi.inputString(line,sPrintF(buff,"Enter the ending angle in [0,1] (default=%e): ",endAngle));
	if( line!="" ) sScanF( line,"%e",&endAngle);
	mappingHasChanged();
      }
      if( fabs(endAngle-startAngle)-1. < REAL_EPSILON*10. ) // make periodic if closed.
      {
	setIsPeriodic(tAxis, functionPeriodic );  
	setBoundaryCondition( Start,tAxis,-1);
	setBoundaryCondition( End  ,tAxis,-1);
      }
      else
      {
	setBoundaryCondition( Start,tAxis,0 );
	setBoundaryCondition( End  ,tAxis,0 );
      }
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
    else if( answer=="joukowsky delta" )
    {
      gi.inputString(line,sPrintF(buff,"Enter delta (degrees) (default=%e): ",joukowskyDelta*180./Pi));
      if( line!="" ) 
      {
        sScanF( line,"%e",&joukowskyDelta);
        joukowskyDelta*=Pi/180;
      }
      mappingHasChanged();
    }
    else if( answer=="joukowsky d" )
    {
      gi.inputString(line,sPrintF(buff,"Enter d (default=%e): ",joukowskyD));
      if( line!="" ) sScanF( line,"%e",&joukowskyD);
      mappingHasChanged();
    }
    else if( answer=="joukowsky a" )
    {
      gi.inputString(line,sPrintF(buff,"Enter a (default=%e): ",joukowskyA));
      if( line!="" ) sScanF( line,"%e",&joukowskyA);
      mappingHasChanged();
    }
    else if( answer=="joukowsky length" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the length (default=%e): ",joukowskyLength));
      if( line!="" ) sScanF( line,"%e",&joukowskyLength);
      mappingHasChanged();
    }
    else if( answer=="joukowsky beta" )
    {
      gi.inputString(line,sPrintF(buff,"Enter beta (default=%e): ",joukowskyBeta));
      if( line!="" ) sScanF( line,"%e",&joukowskyBeta);
      mappingHasChanged();
    }
    else if( answer=="polar singularity factor" )
    {
      gi.inputString(line,sPrintF(buff,"Enter polarSingularityFactor (default=%e): ",polarSingularityFactor));
      if( line!="" )
      {
        sScanF( line,"%e",&polarSingularityFactor);
        mappingHasChanged();
      }
    }
    else if(answer=="surface or volume (toggle)")
    {
      if( domainDimension==3 )
        setDomainDimension(2);
      else
        setDomainDimension(3);
      mappingHasChanged();
    }
    else if( answer=="polar singularity at start" )
    {
      setTypeOfCoordinateSingularity( Start,sAxis,polarSingularity ); 
      initialize();  // adjust the arclength ----
      mappingHasChanged();
    }
    else if( answer=="polar singularity at end" )
    {
      setTypeOfCoordinateSingularity( End  ,sAxis,polarSingularity ); 
      initialize();  // adjust the arclength ----
      
      mappingHasChanged();
    }
    else if( answer=="no singularity at start" )
    {
      setTypeOfCoordinateSingularity( Start,sAxis,noCoordinateSingularity ); 
      initialize();  // adjust the arclength ----
      mappingHasChanged();
    }
    else if( answer=="no singularity at end" )
    {
      setTypeOfCoordinateSingularity( End  ,sAxis,noCoordinateSingularity ); 
      initialize();  // adjust the arclength ----
      mappingHasChanged();
    }
    else if( answer=="plot cross sections (toggle)" )
    {
      plotCrossSections=!plotCrossSections;
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity"  ||
             answer=="check" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
    }
    else if( answer=="show parameters" )
    {
      printf("cross section type = %s \n",
             crossSectionType==ellipse ? "ellipse" :
             crossSectionType==joukowsky ? "joukowsky" : "general");
      if(crossSectionType==general )
      {
        printf("number of cross sections = %i \n",numberOfCrossSections);
	printf("axial approximation is piecewise %s \n",axialApproximation==linear ? "linear" : "cubic");
      }
      else
      {
        printf(" (innerRadius,outerRadius=(%e,%e)\n centre: (x0,y0,z0)=(%e,%e,%e)\n",
           innerRadius,outerRadius,x0,y0,z0);
        printf(" (startAngle,endAngle)=(%e,%e)\n",startAngle,endAngle);
      }
      Mapping::display();
    }
    else if( answer=="plot" )
    {
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      params.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,params); 
      params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
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

    if( answer=="check" )
    {
      Mapping::debug=31;
      realArray r(1,3),x(1,3),ss(1,3);
      ss=0.;
      for(;;)
      {
	gi.inputString(answer,"Evaluate the cross section at which point r? (hit return to continue)");
        if( answer!="" )
	{
	  sScanF(answer,"%e %e %e",&r(0,0),&r(0,1),&r(0,2));
          map(r,x);
	  inverseMap(x,ss);
	  printf(" r=(%6.2e,%6.2e,%6.2e) x=(%6.2e,%6.2e,%6.2e), inverse=(%6.2e,%6.2e,%6.2e)\n",
             r(0,0),r(0,1),r(0,2),x(0,0),x(0,1),x(0,2),ss(0,0),ss(0,1),ss(0,2));
	}
	else
	{
	  break;
	}
      }
    }
    

    if( plotObject )
    {
      // first plot the mapping.
      params.set(GI_TOP_LABEL,getName(mappingName));
      params.set(GI_MAPPING_COLOUR,"red");
      gi.erase();
      PlotIt::plot(gi,*this,params);   
 
      if( plotCrossSections && crossSectionType==general && numberOfCrossSections>1 )
      {
        // Plot the cross sections
	params.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	params.set(GI_MAPPING_COLOUR,"green");
        params.set(GI_USE_PLOT_BOUNDS,TRUE); 
        params.set(GI_SURFACE_OFFSET,(real)-20.);  // *** fix this *** need to offset lines ?
	for( i=0; i<numberOfCrossSections; i++ )
	{
	  PlotIt::plot(gi,*crossSection[i],params);
	}
        params.set(GI_SURFACE_OFFSET,(real)+3.);   // reset to default   *** fix ***
        params.set(GI_USE_PLOT_BOUNDS,FALSE); 
      }
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}

