#include "SplineMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "display.h"
#include "DataPointMapping.h"


#include <float.h>

#define CSGEN  EXTERN_C_NAME(csgen)
#define CSEVAL EXTERN_C_NAME(cseval)

// These are the old spline routines from FMM
extern "C"
{
  void CSGEN (int & n, real & x, real & y, real & bcd, int & iopt  );
  void CSEVAL(int & n, real & x, real & y, real & bcd, real & u, real & s, real & sp );
}

// Here are the spline under tension routines.

#define TSPSI EXTERN_C_NAME(tspsi)
#define TSPSP EXTERN_C_NAME(tspsp)
#define TSPSPT EXTERN_C_NAME(tspspt)
#define TSVAL1 EXTERN_C_NAME(tsval1)
#define TSVAL2 EXTERN_C_NAME(tsval2)
#define TSVAL3 EXTERN_C_NAME(tsval3)

#define ARCL2D EXTERN_C_NAME(arcl2d)
#define ARCL3D EXTERN_C_NAME(arcl3d)

extern "C"
{
  
  void TSPSI (const int & N, const real & X, const real & Y, const int & NCD, const int & IENDC, const int & PER, 
              const int & UNIFRM, const int & LWK, real & WK, real & YP, real & SIGMA, const int & IER);
  
  void TSPSP (const int & N, const int & ND, const real & X, const real & Y, const real & Z, const int & NCD, 
              const int & IENDC, const int & PER, const int & UNIFRM, const int & LWK,  const real & WK, 
              const real & T, const real & XP, const real & YP, const real & ZP, const real & SIGMA, int & IER);

  void TSVAL1 (const int & N, const real & X, const real & Y, const real & YP, const real & SIGMA,
               int & IFLAG, const int & NE, real & TE, real & V, int & IER);

  void TSVAL2 (const int & N, const real & T, const real & X, const real & Y, const real & XP, const real & YP, 
               const real & SIGMA, const int & IFLAG, const int & NE, real & TE, real &  VX, real & VY, int & IER);

  void TSVAL3 (const int & N, const real & T, const real & X, const real & Y, const real & Z, const real & XP, 
               const real & YP, const real & ZP, const real & SIGMA, const int & IFLAG, const int & NE,
	       const real & TE,  real & VX, real & VY, real & VZ, int & IER);

  void ARCL2D (const int & N, const real & X, const real & Y, real &  T, int & IER);
  void ARCL3D (const int & N, const real & X, const real & Y, const real & Z,  real & T, int & IER);

  // Bill's altered version that takes the parameterization T as input
  void TSPSPT (const int & N, const int & ND, const real & X, const real & Y, const real & Z, const int & NCD, 
              const int & IENDC, const int & PER, const int & UNIFRM, const int & LWK,  const real & WK, 
              const real & T, const real & XP, const real & YP, const real & ZP, const real & SIGMA, int & IER);

}

int
equidistribute( const RealArray & w, RealArray & r );


SplineMapping::
SplineMapping(const int & rangeDimension_ /* =2 */ ) 
   : Mapping(1,rangeDimension_,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief 
///     Default Constructor: create a spline curve with the given range dimension.
///  Use this Mapping to create a cubic spline curve in two dimensions.
///  This spline is defined by a set of points (knots), $x(i),y(i)$.
///  The spline is normally parameterized by arclength. The pline can also be parameterized
///  by a weighting of arclength and curvature so that more points are placed in regions
///  with high curvature.
///  For a spline which is periodic in space, the Mapping will automatically
///  add an extra point if the first point is not equal to the last point.
/// 
/// \param rangeDimension_ : 1,2, 3
/// 
///  The SplineMapping uses `{\bf TSPACK}: Tension Spline Curve Fitting Package'
///  by Robert J. Renka; available from Netlib. See the TSPACK documentation
///  and the reference 
///  <ul>
///    <li>[RENKA, R.J.] Interpolatory tension splines with automatic selection
///    of tension factors. SIAM J. Sci. Stat. Comput. {\bf 8}, (1987), pp. 393-415.
///  </ul>
/// 
//===========================================================================
{ 
  SplineMapping::className="SplineMapping";
  setName( Mapping::mappingName,"splineMapping");
  setGridDimensions( axis1,31 );
  numberOfSplinePoints=0;
  initialized=false;
  pointAddedForPeriodicity=false;

  useTensionSplines=true;
  shapePreserving=false;
  // make default BC second derivative=0. The monotoneParabolicFit can go wild outside the domain.
  endCondition=secondDerivative; 
  tension=0.;
  bcValue.redim(2,3);
  bcValue=0.;
  arcLengthWeight=1.;
  curvatureWeight=0.;
  parameterizationType=arcLength;
  rStart=0., rEnd=1.; // By default parameterize the whole length of the spline.
  splineIsPeriodic=notPeriodic;  // periodicity of underlying spline, independent of the reparameterization.

  inverseIsDistributed=false;

  mappingHasChanged();
}

// Copy constructor is deep by default
SplineMapping::
SplineMapping( const SplineMapping & map, const CopyType copyType )
{
  SplineMapping::className="SplineMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "SplineMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

SplineMapping::
~SplineMapping()
{ if( debug & 4 )
  cout << " SplineMapping::Desctructor called" << endl;
}

SplineMapping & SplineMapping::
operator=( const SplineMapping & X )
{
  if( SplineMapping::className != X.getClassName() )
  {
    cout << "SplineMapping::operator= ERROR trying to set a SplineMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  numberOfSplinePoints=X.numberOfSplinePoints;
  s.redim(0); s=X.s;
  knots.redim(0); knots=X.knots;
  bcd.redim(0); bcd=X.bcd;
  initialized=X.initialized;
  pointAddedForPeriodicity=X.pointAddedForPeriodicity;

  useTensionSplines=X.useTensionSplines;
  shapePreserving=X.shapePreserving;
  endCondition=X.endCondition;
  tension=X.tension;
  bcValue.redim(0); bcValue=X.bcValue;
  xp.redim(0); xp=X.xp;
  sigma.redim(0); sigma=X.sigma;
  arcLengthWeight=X.arcLengthWeight;
  curvatureWeight=X.curvatureWeight;
  parameterizationType=X.parameterizationType;
  rStart=X.rStart;
  rEnd=X.rEnd;
  splineIsPeriodic=X.splineIsPeriodic;
  
  return *this;
}


int SplineMapping::
shift(const real & shiftx /* =0. */, 
      const real & shifty /* =0. */, 
      const real & shiftz /* =0.*/ )
//===========================================================================
/// \brief  Shift the SPLINE in space.
//===========================================================================
{
  const real shift[3]={shiftx,shifty,shiftz};
  Range all;
  for( int axis=0; axis<rangeDimension; axis++ )
    knots(all,axis)=knots(all,axis)+shift[axis];

  initialized=false;
  return 0;
}

int SplineMapping::
scale(const real & scalex /* =0. */, 
      const real & scaley /* =0. */, 
      const real & scalez /* =0.*/ )
//===========================================================================
/// \brief  Scale the SPLINE in space.
//===========================================================================
{
  const real scale[3]={scalex,scaley,scalez};
  Range all;
  for( int axis=0; axis<rangeDimension; axis++ )
     knots(all,axis)*=scale[axis];

  initialized=false;
  return 0;
}

int SplineMapping::
rotate( const int & axis, const real & theta )
//===========================================================================
/// \brief  Perform a rotation about a given axis. This rotation is applied
///    after any existing transformations. Use the reset function first if you
///    want to remove any existing transformations.
/// \param axis (input) : axis to rotate about (0,1,2)
/// \param theta (input) : angle in radians to rotate by.
//===========================================================================
{
  if( rangeDimension==1 )
    return 1;
  if( rangeDimension==2 && axis!=axis3 )
  {
    printf("SplineMapping::rotate:ERROR: Can only rotate a spline with rangeDimension==2 around axis==2\n");
    return 1;
  }
  const real ct = cos(theta); 
  const real st = sin(theta); 

  const int i1 = (axis+1) % 3;
  const int i2 = (axis+2) % 3;
  Range all;
  const RealArray knots1=knots(all,i1)*ct-knots(all,i2)*st;
  knots(all,i2)=         knots(all,i1)*st+knots(all,i2)*ct;
  knots(all,i1)=knots1;

  initialized=false;
  return 0;
}




int SplineMapping::
setParameterizationType(const ParameterizationType & type)
//=====================================================================================
/// \details 
///    Specify the parameterization for the Spline. With {\tt index} parameterization
///  the knots on the spline are parameterized as being equally spaced. With {\tt arclength}
///   parameterization the knots are parameterized by arclength or a weighted combination
///  of arclength and curvature. With {\tt userDefined} parameterization the user must supply
///  the parameterization through the {\tt setParameterization} function.
/// \param type (input) : One of {\tt index} or {\tt arcLength} or {\tt userDefined}.
//=====================================================================================
{
  parameterizationType=type;
  return 0;
}

const RealArray & SplineMapping::
getParameterizationS() const
//=====================================================================================
/// \details 
///    Return the current parameterization.
//=====================================================================================
{
  return s;
}

int SplineMapping::
getNumberOfKnots() const
//=====================================================================================
/// \brief  
///     Return the number of knots on the spline. 
//=====================================================================================
{
  return knots.getLength(0);
}

//! Return the spline knots
const RealArray& SplineMapping::
getKnotsS() const
{
  return knots;
}




int SplineMapping:: 
setParameterization(const RealArray & s_ )
//=====================================================================================
/// \details 
///    Supply a user defined parameterization. This routine will set the parameterization type
///  to be {\tt userDefined}.
/// \param s_ (input) : An increasing sequence of values that are to be
///      used to parameterize the spline points. These values must cover the interval [0,1] which
///    will be the interval defining the mapping. You could add values outside [0,1] to define the
///  behaviour of the spline at "ghost points".  The number of points in the array must
///      be equal to the number of points supplied when the {\tt setPoints} function is called.
//=====================================================================================
{
  parameterizationType=userDefined;
  s.redim(s_.getLength(0));
  s=s_;
  return 0;
}


int SplineMapping::
parameterize(const real & arcLengthWeight_ /* =1.*/, 
             const real & curvatureWeight_ /* =0.*/ )
//=====================================================================================
/// \details 
///    Set the `arclength' parameterization parameters. The parameterization is chosen to
///  redistribute the points to resolve the arclength and/or the curvature of the curve.
///  By default the spline is parameterized by arclength only. To resolve regions of high
///  curvature choose the recommended values of {\tt arcLengthWeight\_=1.} and
///   {\tt curvatureWeight\_=.5}.
/// 
///   To determine the parameterization we equidistribute the weight function 
///   \[
///      w(r) = 1. + {\rm arcLengthWeight} {s(r)\over |s|_\infty}  
///                + {\rm curvatureWeight} {c(r)\over |c|_\infty}
///   \]
///   where $s(r)$ is the local arclength and $c(r)$ is the curvature. Note that we normalize
///  $s$ and $c$ by their maximum values.
///   
/// \param arcLengthWeight_ (input): A weight for arclength. A negative value may give undefined results.
/// \param curvatureWeight_ (input): A weight for curvature. A negative value may give undefined results.
//=====================================================================================
{
  arcLengthWeight=arcLengthWeight_;
  curvatureWeight=curvatureWeight_;
  return 0;
}


int SplineMapping:: 
setEndConditions(const EndCondition & condition, 
                 const RealArray & endValues /* =Overture::nullRealDistributedArray() */ )
//=====================================================================================
/// \details 
///    Specify end conditions for the spline
/// \param condition (input) : Specify an end condition.
///     <ul>
///       <li>[monontone parabolic fit] : default BC for the shape preserving spline.
///       <li>[first derivative] : user specified first derivatives.
///       <li>[second derivative] : user specified second derivatives.
///     </ul>
/// \param endValues (input) : if {\tt condition==firstDerivative} (or {\tt condition==secondDerivative})
///   then endValues(0:1,0:r-1) should
///   hold the values for the first (or second) derivatives of the spline at the start and end. Here
///    r=rangeDimension.
///      
//=====================================================================================
{
  endCondition=condition;
  if( endCondition==firstDerivative || endCondition==secondDerivative )
    bcValue(Range(0,1),Range(0,rangeDimension-1))=endValues(Range(0,1),Range(0,rangeDimension-1));
    
  initialized=false;
  mappingHasChanged();
  return 0;
}


int SplineMapping::
setPoints( const RealArray & x )
//=====================================================================================
/// \brief  Supply spline points for a 1D curve.
/// \param x (input) : array of spline knots.
///   The spline is parameterized by a NORMALIZED index, i/(number of points -1), i=0,1,...
//=====================================================================================
{
  domainDimension=1;
  rangeDimension=1;

  numberOfSplinePoints=x.getLength(0);
  Range R(0,numberOfSplinePoints-1);
  knots.redim(R,rangeDimension);
  knots(R,0)=x;
  pointAddedForPeriodicity=false;
  mappingHasChanged(); 
  initialized=false;
  initialize();  // we have to do this here ** maybe not anymore ?
  return 0;
}  

int SplineMapping::
setPoints( const RealArray & x, const RealArray & y )
//=====================================================================================
/// \brief  Supply spline points for a 2D curve. Use the points (x(i),y(i)) i=x.getBase(0),..,x.getBound(0)
/// \param x,y (input) : array of spline knots.
//=====================================================================================
{
  domainDimension=1;
  rangeDimension=2;

  numberOfSplinePoints=x.getLength(0);
  Range R(0,numberOfSplinePoints-1);
  knots.redim(R,rangeDimension);
  knots(R,0)=x;
  knots(R,1)=y;
  pointAddedForPeriodicity=false;
  mappingHasChanged(); 
  initialized=false;
  initialize();  // we have to do this here 
  return 0;
}  


int SplineMapping::
setPoints( const RealArray & x, const RealArray & y, const RealArray & z )
//=====================================================================================
/// \brief  Supply spline points for a 3D curve. Use the points (x(i),y(i),z(i)) i=x.getBase(0),..,x.getBound(0)
/// \param x,y,z (input) : array of spline knots.
//=====================================================================================
{
  domainDimension=1;
  rangeDimension=3;

  numberOfSplinePoints=x.getLength(0);
  Range R(0,numberOfSplinePoints-1);
  knots.redim(R,rangeDimension);
  knots(R,0)=x;
  knots(R,1)=y;
  knots(R,2)=z;
  pointAddedForPeriodicity=false;
  mappingHasChanged(); 
  initialized=false;
  initialize();  // we have to do this here 
  
  return 0;
}  

int SplineMapping::
setShapePreserving( const bool trueOrFalse /* = true */ )
//=====================================================================================
/// \details 
///    Create a shape preserving (monotone) spline or not
/// \param trueOrFalse (input) : if true, create a spline that preserves the shape. For a one dimensional
///    curve the shape preserving spline will attempt to remain montone where the knots ar montone.
///    See the comments with TSPACK for further details.
/// 
//=====================================================================================
{
  shapePreserving=trueOrFalse;
  initialized=false;
  mappingHasChanged();
  return 0;
}

int SplineMapping:: 
setTension( const real & tensionFactor )
//=====================================================================================
/// \details 
///    Specify a constant tension factor. Specifying this value will turn off the shape preseeving feature.
/// \param tensionFactor (input): A value from 0. to 85. A value of 0. corresponds to no tension.
/// 
//=====================================================================================
{
  tension=tensionFactor;
  shapePreserving=false;
  initialized=false;
  mappingHasChanged();
  return 0;
}

int SplineMapping:: 
setDomainInterval(const real & rStart_ /* =0. */, 
                  const real & rEnd_ /* =1. */)
//=====================================================================================
/// \details 
///  Restrict the domain of the spline.
///  By default the spline is parameterized on the interval [0,1].
///  You may choose a sub-section of the spline by choosing a new interval [rStart,rEnd].
///  For periodic splines the interval may lie in [-1,2] so the sub-section can cross the branch cut.
///  You may even choose rEnd<rStart to reverse the order of the parameterization.
/// \param rStart_,rEnd_ (input) : define the new interval.
//=====================================================================================
{
  if( fabs(rEnd-rStart)==1. )
    splineIsPeriodic=getIsPeriodic(axis1);
  
  rStart=rStart_;
  rEnd=rEnd_;
	
  const real rMin = splineIsPeriodic ? -1. : 0.;
  if( rStart<rMin )
  {
    printf("SplineMapping::setDomainInterval:ERROR: rStart=%f must be at least %f. Setting to %f\n",rStart,rMin,rMin);
    rStart=rMin;
  }
  if( rEnd==rStart )
  {
    printf("SplineMapping::setDomainInterval:ERROR: rStart=rEnd=%e. Setting rStart=0., rEnd=1.\n",rStart);
    rStart=0., rEnd=1.;
  }
  if( splineIsPeriodic )
    if( fabs(rEnd-rStart)!=1. )
      setIsPeriodic(axis1,notPeriodic);
    else
      setIsPeriodic(axis1,splineIsPeriodic);

  mappingHasChanged();  // *wdh* 021004
  return 0;
}

int SplineMapping:: 
getDomainInterval(real & rStart_, real & rEnd_) const
//=====================================================================================
/// \details 
///   Get the current domain interval.
/// \param rStart_,rEnd_ (output) : the current domain interval.
//=====================================================================================
{
  rStart_=rStart;
  rEnd_=rEnd;
  return 0;
}

void SplineMapping::
setIsPeriodic( const int axis, const periodicType isPeriodic0 )
// =====================================================================================
/// \details 
/// \param axis (input): axis = (0,1,2) (or axis = (axis1,axis2,axis3)) with $axis<domainDimension$.
/// \param Notes:
///     This routine has some side effects. It will change the boundaryConditions to be consistent
///   with the periodicity (if necessary).
// =====================================================================================
{
  Mapping::setIsPeriodic(axis,isPeriodic0);
  splineIsPeriodic=getIsPeriodic(axis1);
  initialized=false;
}


int SplineMapping:: 
useOldSpline( const bool & trueOrFalse /* =true */ )
//=====================================================================================
/// \details 
///   Use the old spline routines from FMM, Forsythe Malcolm and Moler. This is for backward
///  compatability.
/// \param trueOrFalse (input) : If true Use the old spline from FMM, otherwise use the tension splines.
/// 
//=====================================================================================
{
  useTensionSplines=!trueOrFalse;
  initialized=false;
  mappingHasChanged();
  return 0;
}




void SplineMapping::
initialize()
//=====================================================================================
// /Description:
//   Initialize the spline.
//=====================================================================================
{

  if( splineIsPeriodic==functionPeriodic )
  { // Check that the last point is equal to the first on a periodic spline
    Range Rx(0,rangeDimension-1);
    real dist= sum(fabs(knots(0,Rx)-knots(numberOfSplinePoints-1,Rx)));
    
    if( dist > 10.*REAL_EPSILON*max(fabs(knots)) )
    {
      pointAddedForPeriodicity=true;
      numberOfSplinePoints++;
      knots.resize(numberOfSplinePoints,rangeDimension);
      knots(numberOfSplinePoints-1,Rx)=knots(0,Rx);
      printF("SplineMapping: I have added an extra point to make the spline really periodic.\n");
    }
  }
  else if( pointAddedForPeriodicity )
  { // curve has been made nonperiodic, remove the extra point
    pointAddedForPeriodicity=false;
    numberOfSplinePoints--;
  }

  if( parameterizationType!=userDefined )
    s.redim(numberOfSplinePoints);
  else
  {
    if( s.getLength(0) != numberOfSplinePoints )
    {
      printF("SplineMapping::initialize:ERROR: parameterization type is userDefined but the parameter array \n"
	     " is not of the correct size. Maybe you forgot to call setParameterization\n");
      OV_ABORT("error");
    }
  }
  
  if( !useTensionSplines )
  {
    // use spline routines from FMM book:
    if( rangeDimension==1 || parameterizationType==index )
      s.seqAdd(0.,1./(numberOfSplinePoints-1)); 
    else if( parameterizationType==arcLength )
    {
      // compute the arclength and normalize
      s(0)=0.;
      if( rangeDimension==1 )
	for( int i=1; i<numberOfSplinePoints; i++ )
	  s(i)=s(i-1)+fabs(knots(i,0)-knots(i-1,0));
      else if( rangeDimension==2 )
	for( int i=1; i<numberOfSplinePoints; i++ )
	  s(i)=s(i-1)+SQRT( SQR(knots(i,0)-knots(i-1,0))+SQR(knots(i,1)-knots(i-1,1)) );
      else
	for( int i=1; i<numberOfSplinePoints; i++ )
	  s(i)=s(i-1)+SQRT( SQR(knots(i,0)-knots(i-1,0))+SQR(knots(i,1)-knots(i-1,1))+SQR(knots(i,2)-knots(i-1,2)) );
      real totalLength=s(numberOfSplinePoints-1);
      s/=totalLength;
    }

    int option= splineIsPeriodic ? 1 : 0;
    bcd.redim(3,numberOfSplinePoints,rangeDimension);
    CSGEN (numberOfSplinePoints, s(0), knots(0,axis1), bcd(0,0,axis1),option );
    if( rangeDimension>1 )
      CSGEN (numberOfSplinePoints, s(0), knots(0,axis2), bcd(0,0,axis2),option );
    if( rangeDimension>2 )
      CSGEN (numberOfSplinePoints, s(0), knots(0,axis3), bcd(0,0,axis3),option );


    ::display(bcd,"bcd","%12.5e ");
    
  }
  else
  {
    // use splines under tension

    int ncd=2;  // number of continuous derivatives at knots.
    int iendc=endCondition==firstDerivative ? 1 : 
              endCondition==secondDerivative ? 2 : 0; 
    int per = splineIsPeriodic!=notPeriodic;  // periodic?
    int unifrm = !shapePreserving; // true = uniform tension in sigma(0)
  
    int lwk=1;
    sigma.redim(numberOfSplinePoints);  sigma=0.;
    if( unifrm )
      sigma(0)=tension;
    int ier;
  
    xp.redim(numberOfSplinePoints,rangeDimension);
    Range R(0,numberOfSplinePoints-1), Rx(0,rangeDimension-1);
    if( endCondition==firstDerivative || endCondition==secondDerivative )
    {
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xp(0,axis)=bcValue(0,axis);
	xp(numberOfSplinePoints-1,axis)=bcValue(1,axis);
      }
    }
    
    // if the derivative is periodic but not the function then we must
    //  (1) subtract off a linear function from the knots to make them
    //      look periodic
    //  (2) compute the spline
    //  (3) add a linear function back and adjust the spline derivatives.
    if( rangeDimension==1 )
    {
      if( parameterizationType!=userDefined )
        s.seqAdd(0.,1./max(1.,(numberOfSplinePoints-1))); 
      
      if( ncd==1 )
	lwk=1;
      else 
      {
	if( !per && unifrm )
	  lwk=numberOfSplinePoints-1;
	else if( (per && unifrm) || (!per && !unifrm) )
	  lwk=2*numberOfSplinePoints-2;
	else
	  lwk=3*numberOfSplinePoints-3;
      }
      RealArray wk(lwk);

      const real slope=knots(numberOfSplinePoints-1,0)-knots(0,0);
      if( splineIsPeriodic==derivativePeriodic )
        knots(R,0)-=s(R)*slope;
      TSPSI (numberOfSplinePoints,s(0),knots(0,0),ncd,iendc,per,unifrm,lwk, 
             wk(0), xp(0,0),sigma(0), ier);
      if( ier<0 )
      {
	printf("SplineMapping::ERROR return from tspsi: ier=%i \n",ier);
	throw "error";
      }
      if( splineIsPeriodic==derivativePeriodic )
      {
        knots(R,0)+=s(R)*slope;
        xp+=slope;
      }
    }
    else
    {
      if( ncd==1 )
	lwk=1;
      else 
      {
	if( !per && unifrm )
	  lwk=numberOfSplinePoints-1;
	else if( per && unifrm )
	  lwk=2*numberOfSplinePoints-2;
	else if( !per && !unifrm )
	  lwk=(rangeDimension+1)*(numberOfSplinePoints-1);
	else
	  lwk=(rangeDimension+2)*(numberOfSplinePoints-1);
      }

        // compute the arclength
      if( parameterizationType==index )
        s.seqAdd(0.,1./(numberOfSplinePoints-1)); 
      else if( parameterizationType==arcLength )
      {
	if( rangeDimension==2 )
	  ARCL2D(numberOfSplinePoints,knots(0,0),knots(0,1), s(0), ier);
	else
	  ARCL3D(numberOfSplinePoints,knots(0,0),knots(0,1),knots(0,2), s(0), ier);
      }
      
      // normalize
      s/=s(numberOfSplinePoints-1);

      if( parameterizationType==arcLength && curvatureWeight>0. && numberOfSplinePoints>3 )
      {
        // compute the parameterization based on arclength and curvature

        const int n=numberOfSplinePoints-1;
        RealArray weight(numberOfSplinePoints), curvature(numberOfSplinePoints);
        Range R(1,n-1);

        weight(R)=s(R+1)-s(R-1);  // 2 times the delta arclength

	if( splineIsPeriodic==functionPeriodic )
	{
	  weight(0)=s(1)-s(0)+s(n)-s(n-1);
	  weight(n)=weight(0);
	}
	else
	{ // use one sided approx.
	  weight(0)=s(2)-s(0);
	  weight(n)=s(n)-s(n-2);
	}

	if( rangeDimension==2 )
	{
	  curvature(R)=(fabs(knots(R+1,0)-2.*knots(R,0)+knots(R-1,0))+
			fabs(knots(R+1,1)-2.*knots(R,1)+knots(R-1,1)))/SQR(weight(R));
          if( splineIsPeriodic==functionPeriodic )
	  {
	    curvature(0)=(fabs(knots(1,0)-2.*knots(0,0)+knots(n-1,0))+
			  fabs(knots(1,1)-2.*knots(0,1)+knots(n-1,1)))/SQR(weight(0));
	    curvature(n)=curvature(0);
	  }
	  else
	  { // use one sided approx.
	    curvature(0)=curvature(1);
	    curvature(n)=curvature(n-1);
	  }
	}
	else
	{
	  curvature(R)=(fabs(knots(R+1,0)-2.*knots(R,0)+knots(R-1,0))+
			fabs(knots(R+1,1)-2.*knots(R,1)+knots(R-1,1))+
			fabs(knots(R+1,2)-2.*knots(R,2)+knots(R-1,2)))/SQR(weight(R));
          if( splineIsPeriodic==functionPeriodic )
	  {
	    curvature(0)=(fabs(knots(1,0)-2.*knots(0,0)+knots(n-1,0))+
			  fabs(knots(1,1)-2.*knots(0,1)+knots(n-1,1))+
			  fabs(knots(1,2)-2.*knots(0,2)+knots(n-1,2)))/SQR(weight(0));
	    curvature(n)=curvature(0);
	  }
	  else
	  { // use one sided approx.
	    curvature(0)=curvature(1);
	    curvature(n)=curvature(n-1);
	  }
	}
        real sMax=max(weight);
	real cMax=max(curvature);
        cMax=max(sMax*REAL_EPSILON,cMax);
	weight=1.+weight*(arcLengthWeight/sMax)+curvature*(curvatureWeight/cMax);
        // smooth the weight function
        const real omega=.5;
        for( int it=0; it<4; it++ )
	{
          weight(R)=(1.-omega)*weight(R) + omega*.5*(weight(R+1)+weight(R-1));
	  if( splineIsPeriodic )
	  {
	    weight(0)=(1.-omega)*weight(0) + omega*.5*(weight(1)+weight(n-1));
	    weight(n)=weight(0);
	  }
	}
	
	equidistribute(weight,s);
	if( debug & 4 )::display(weight,"Here is the weight function");
	if( debug & 4 )::display(s,"Here is the equidistributed parameter");
        SplineMapping equiSpline;
	equiSpline.setPoints(s);
        RealArray r(numberOfSplinePoints);
	r.seqAdd(0.,1./(numberOfSplinePoints-1));
	equiSpline.inverseMapS(r,s);
	if( debug & 4 )::display(s,"Here is s after inverting the equidistributed parameter");
      }

      RealArray wk(lwk);
      RealArray slope(1,rangeDimension); 
      if( splineIsPeriodic==derivativePeriodic )
      {
	
        slope(0,Rx)=knots(numberOfSplinePoints-1,Rx)-knots(0,Rx);
        for( int axis=0; axis<rangeDimension; axis++ )
          knots(R,axis)-=s(R)*slope(0,axis);
      }
      // call Bill's altered version that takes s as input      
      TSPSPT(numberOfSplinePoints,rangeDimension,knots(0,0),knots(0,1),knots(0,rangeDimension-1),ncd,iendc,per,unifrm,
	    lwk, wk(0),s(0),xp(0,0),xp(0,1),xp(0,rangeDimension-1),sigma(0),ier);

/* ----
      // scale the arclength to the interval [0,1]
      const real sn = s(numberOfSplinePoints-1);
      s/=sn;
      xp*=sn;
---- */
      if( splineIsPeriodic==derivativePeriodic )
      {
        for( int axis=0; axis<rangeDimension; axis++ )
	{
          knots(R,axis)+=s(R)*slope(0,axis);
	  xp(R,axis)+=slope(0,axis);
	}
      }
      // ::display(s,"Here is s");
      // ::display(xp,"Here is xp");
      
      if( ier<0 )
      {
	printf("SplineMapping::ERROR return from tspsp: ier=%i \n",ier);
	throw "error";
      }

    }
    
  }
  reinitialize();  // this will re-initialize the inverse.
  mappingHasChanged(); 
  initialized=true;
}
 
const realArray& SplineMapping::getKnots() const
{
  #ifndef USE_PPP
    return getKnotsS();
  #else
   Overture::abort("SplineMapping::This function is obsolete.");
  #endif
}
const realArray & SplineMapping::getParameterization() const
{
  #ifndef USE_PPP
    return getParameterizationS();
  #else
   Overture::abort("SplineMapping::This function is obsolete.");
  #endif
}

#ifdef USE_PPP
int SplineMapping::setParameterization(const realArray & s )
{
  Overture::abort("SplineMapping::This function is obsolete.");
}
int SplineMapping::setPoints( const realArray & x )
{
  Overture::abort("SplineMapping::This function is obsolete.");
}
int SplineMapping::setPoints( const realArray & x, const realArray & y )
{
  Overture::abort("SplineMapping::This function is obsolete.");
}
int SplineMapping::setPoints( const realArray & x, const realArray & y, const realArray & z )
{
  Overture::abort("SplineMapping::This function is obsolete.");
}
#endif


void SplineMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  #ifndef USE_PPP
    mapS(r,x,xr);
  #else
    Overture::abort("SplineMapping::map: finish me");
  #endif
}


void SplineMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the spline and/or derivatives. 
//=====================================================================================
{
  if( params.coordinateType != cartesian )
    cerr << "SplineMapping::map - coordinateType != cartesian " << endl;

  if( !initialized )
    initialize();    

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );
  if( I.getLength()==0 ) return;
  
  const real rScale=rEnd-rStart;
  RealArray rr(I);
  const bool reScale = rScale!=1. || rStart!=0.;
  if( splineIsPeriodic==functionPeriodic || reScale )  // ****** we need to add on a shift for derivative periodic case
  {
//     if( reScale )
//       printf("SplineMapping: rScale=%e splineIsPeriodic=%i\n",rScale,splineIsPeriodic);
    
    if( !splineIsPeriodic )
      rr=rScale*r(I,axis1)+rStart;  // rescale non-periodic
    else
    {
      if( reScale )
        rr=fmod(rScale*r(I,axis1)+rStart+2.,1.);  // we enforce rStart>=-1.
      else
        rr=fmod(r(I,axis1)+1.,1.);   // map to [0,1] ** assumes r>=-1.
    }
  }
  else
    rr=r(I,axis1);

  Range Rx(0,rangeDimension-1);
  if( !useTensionSplines )
  {
    if( computeMap || computeMapDerivative )
    {
      RealArray sx(I,Rx),sxp(I,Rx);
      int i;
      for( i=base; i<=bound; i++ )
	CSEVAL(numberOfSplinePoints,s(0),knots(0,axis1),bcd(0,0,axis1),rr(i),sx(i,axis1),sxp(i,axis1));
      if( rangeDimension>1 )
	for( i=base; i<=bound; i++ )
	  CSEVAL(numberOfSplinePoints,s(0),knots(0,axis2),bcd(0,0,axis2),rr(i),sx(i,axis2),sxp(i,axis2));
      if( rangeDimension>2 )
	for( i=base; i<=bound; i++ )
	  CSEVAL(numberOfSplinePoints,s(0),knots(0,axis3),bcd(0,0,axis3),rr(i),sx(i,axis3),sxp(i,axis3));
      if( computeMap )
	x(I,Rx)=sx;
      if( computeMapDerivative )
	xr(I,Rx,0)=sxp;
    }
  }
  else
  {
    int numToEvaluate=bound-base+1;
    int ier,iflag; 
    if( computeMap )
    {
      iflag=0;  // compute function values
      if( rangeDimension==1 )
        TSVAL1(numberOfSplinePoints,s(0),knots(0,0),xp(0),sigma(0),iflag,numToEvaluate,rr(base), x(base,0), ier);
      else if( rangeDimension==2 )
	TSVAL2(numberOfSplinePoints,s(0),knots(0,0),knots(0,1),xp(0,0),xp(0,1),sigma(0),iflag,numToEvaluate,
	       rr(base), x(base,0),x(base,1),ier);
      else 
        TSVAL3(numberOfSplinePoints,s(0),knots(0,0),knots(0,1),knots(0,2),xp(0,0),xp(0,1),xp(0,2),
               sigma(0),iflag,numToEvaluate, rr(base), x(base,0),x(base,1),x(base,2),ier);
      if( ier<0 )
      {
	printf("SplineMapping:ERROR return from tsval1: ier=%i \n",ier);
	throw "error";
      }
    }
    if( computeMapDerivative )
    {
      iflag=1;  // compute first derivatives.
      if( rangeDimension==1 )
        TSVAL1(numberOfSplinePoints,s(0),knots(0,0),xp(0),sigma(0),iflag,numToEvaluate,rr(base),xr(base,0,0),ier);
      else if( rangeDimension==2 )
        TSVAL2(numberOfSplinePoints,s(0),knots(0,0),knots(0,1),xp(0,0),xp(0,1),sigma(0),iflag,numToEvaluate,
	       rr(base), xr(base,0,0),xr(base,1,0),ier);
      else 
        TSVAL3(numberOfSplinePoints,s(0),knots(0,0),knots(0,1),knots(0,2),xp(0,0),xp(0,1),xp(0,2),sigma(0),
               iflag,numToEvaluate, rr(base), xr(base,0,0),xr(base,1,0),xr(base,2,0),ier);

      if( ier<0 )
      {
	printf("SplineMapping:ERROR return from tsval%i: ier=%i \n",rangeDimension,ier);
	throw "error";
      }



    }
  }
  if( reScale && computeMapDerivative )
    xr(I,Rx,0)*=rScale;
    
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int SplineMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  if( debug & 4 )
    cout << "Entering SplineMapping::get" << endl;

  subDir.get( SplineMapping::className,"className" ); 
  if( SplineMapping::className != "SplineMapping" )
  {
    cout << "SplineMapping::get ERROR in className!" << endl;
  }
  subDir.get( numberOfSplinePoints,"numberOfSplinePoints" ); 
  subDir.get( s,"s" ); 
  subDir.get( knots,"knots" ); 
  subDir.get( bcd,"bcd" ); 
  subDir.get( initialized,"initialized" ); 
  subDir.get( pointAddedForPeriodicity,"pointAddedForPeriodicity" ); 

  int temp;
  subDir.get( temp,"useTensionSplines" ); useTensionSplines=temp;
  subDir.get( shapePreserving,"shapePreserving");
  subDir.get( temp,"endCondition" ); endCondition=(EndCondition)temp;
  subDir.get( tension,"tension" );
  subDir.get( bcValue,"bcValue" );
  subDir.get( xp,"xp" ); 
  subDir.get( sigma,"sigma" ); 
  subDir.get( arcLengthWeight,"arcLengthWeight");
  subDir.get( curvatureWeight,"curvatureWeight");
  subDir.get( temp,"parameterizationType" ); parameterizationType=(ParameterizationType)temp;
  subDir.get( rStart,"rStart");
  subDir.get( rEnd  ,"rEnd");
  subDir.get( temp,"splineIsPeriodic" ); splineIsPeriodic=(periodicType)temp;

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  
  mappingHasChanged();
  return 0;
}

int SplineMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( SplineMapping::className,"className" );
  subDir.put( numberOfSplinePoints,"numberOfSplinePoints" ); 
  subDir.put( s,"s" ); 
  subDir.put( knots,"knots" ); 
  subDir.put( bcd,"bcd" ); 
  subDir.put( initialized,"initialized" ); 
  subDir.put( pointAddedForPeriodicity,"pointAddedForPeriodicity" ); 

  subDir.put( (int)useTensionSplines,"useTensionSplines" );
  subDir.put( shapePreserving,"shapePreserving");
  subDir.put( (int)endCondition,"endCondition" );
  subDir.put( tension,"tension" );
  subDir.put( bcValue,"bcValue" );
  subDir.put( xp,"xp" ); 
  subDir.put( sigma,"sigma" ); 
  subDir.put( arcLengthWeight,"arcLengthWeight");
  subDir.put( curvatureWeight,"curvatureWeight");
  subDir.put( (int)parameterizationType,"parameterizationType" );
  subDir.put( rStart,"rStart");
  subDir.put( rEnd  ,"rEnd");
  subDir.put( (int)splineIsPeriodic,"splineIsPeriodic" ); 

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *SplineMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==SplineMapping::className )
    retval = new SplineMapping();
  return retval;
}

    

int SplineMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the spline mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!SplineMapping",
      "enter spline points",
      "pick spline points",
      "set range dimension",
      "shape preserving (toggle)",
      "specify tension",
      ">boundary conditions",
        "default boundary condition",
        "specify first derivatives at boundaries",
        "specify second derivatives at boundaries",
        "monotone parabolic fit",
      "<use tension splines (toggle)",
      ">parameterization",
        "index",
        "arc-length",
        "arc-length weight",
        "curvature weight",
        "restrict the domain",
      "<>change",
        "rotate",
        "scale",
        "shift",
        "project onto a plane",
      "<plot knots",
      "do not plot knots",
      "plot grid points",
      "do not plot grid points",
      "plot first derivatives",
      "plot second derivatives",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check points",
      "check",
      "show spline outside boundaries",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "enter spline points: ",
      "pick spline points : pick points with the mouse",
      "set range dimension: 1,2, or 3 for a 1D, 2D or 3D spline curve",
      "shape preserving (toggle): preserve shape (monotonicity for 1D splines)",
      "specify tension : specify a constant tension",
      "specify first derivatives at boundaries : define boundary conditions",
      "specify second derivatives at boundaries: define boundary conditions",
      "use tension splines (toggle) : if false use the old splines from FMM",
      "index : parameterize knots by index (i.e. equally spaced)"
      "arc-length : parameterize knots by arclength and curvature (2D or 3D only)"
      "arc-length weight : weighting factor for arclength in computing the parameterization",
      "curvature weight : weighting factor for curvature in computing the parameterization",
      "restrict the domain: change the spline to cover a sub interval of [0,1]",
      " ",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : specify periodicity in each direction",
      "check points       : evaluate the spline and inverse",
      "check              : check properties of this mapping",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=numberOfSplinePoints>0;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  bool showSplineOutsideBoundaries=false;

  bool plotGridPoints=true;
  bool plotKnots=false;

  gi.appendToTheDefaultPrompt("Spline>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="enter spline points" )
    { 
      gi.inputString(line,sPrintF(buff,"Enter the number of points :"));
      if( line!="" ) sScanF(line,"%i ",&numberOfSplinePoints);
      knots.redim(numberOfSplinePoints,rangeDimension);
      for( int i=0; i<numberOfSplinePoints; i++ )
      {
        if( rangeDimension==1 )
          gi.inputString(line,sPrintF(buff,"Enter x for point %i",i));
        else if(rangeDimension==2 ) 
          gi.inputString(line,sPrintF(buff,"Enter x,y for point %i",i));
        else
          gi.inputString(line,sPrintF(buff,"Enter x,y,z for point %i",i));
        if( rangeDimension==1 )
          sScanF(line,"%e ",&knots(i,axis1));
        else if( rangeDimension==2 ) 
          sScanF(line,"%e %e ",&knots(i,axis1),&knots(i,axis2));
        else 
          sScanF(line,"%e %e %e ",&knots(i,axis1),&knots(i,axis2),&knots(i,axis3));
      }
      initialized=false;
      plotObject=true;
      mappingHasChanged(); 
    }
    else if( answer=="pick spline points" )
    {
      if( rangeDimension!=2 )
      {
	printf("Sorry, one can only pick points with the mouse in 2D\n");
        continue;
      }
      RealArray xBound(2,3);
      xBound=0.;
      xBound(1,nullRange)=1.;

      gi.inputString(line,sPrintF(buff,"Enter bounds xa,xb, ya,yb (default= 0,1, 0,1)"));
      if( line!="" )
        sScanF(line,"%e %e %e %e",&xBound(0,0),&xBound(1,0),&xBound(0,1),&xBound(1,1));
      printf("using plot bounds [%e,%e]x[%e,%e]\n",xBound(0,0),xBound(1,0),xBound(0,1),xBound(1,1));
      
      knots.redim(1000,rangeDimension); // at most 1000 points -- fix this --
      gi.erase();
      gi.setGlobalBound(xBound);
// turn on the back ground grid
      gi.setPlotTheBackgroundGrid(true);
      
#ifndef USE_PPP
      numberOfSplinePoints=gi.pickPoints(knots);
#else
      throw "error";
#endif
      if( numberOfSplinePoints>0 )
      {
	knots.resize(numberOfSplinePoints,rangeDimension);
	initialized=false;
	plotObject=true;
	mappingHasChanged(); 
      }
      
    }
    else if( answer=="set range dimension" )
    {
      gi.inputString(line,sPrintF(buff,"Enter range dimension: 1,2, or 3 (current=%i)",rangeDimension));
      if( line!="" )
      {
        sScanF(line,"%i ",&rangeDimension);
        rangeDimension=max(1,min(3,rangeDimension));
        initialized=false;
        plotObject=false;
        mappingHasChanged();
      }
    }
    else if( answer=="use tension splines (toggle)" )
    {
      // use old spline routines from FMM.
      useTensionSplines=!useTensionSplines;
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="shape preserving (toggle)" )
    {
      shapePreserving=!shapePreserving;
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="specify tension" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the tension (0=no tension, 85=largest tension, default=%6.2e) :",
                     tension));
      if( line!="" )
      {
        sScanF(line,"%e",&tension);
        if( tension<0. || tension>85. )
	{
	  printf("ERROR: input tension=%e is invalid \n",tension);
	  tension=max(0.,min(85.,tension));
	}
        printf("SplineMapping:INFO Setting the tension to be %6.2e \n",tension);
        if( shapePreserving )
	{
	  shapePreserving=false;
	  printf("SplineMapping:INFO: shape preserving turned off since the tension is specified\n");
	}
        initialized=false;
        mappingHasChanged();
      }
    }
    else if( answer=="index" )
    {
      parameterizationType=index;
      printf("The Spline is now being parameterized by index");
    }
    else if( answer=="arc-length" )
    {
      parameterizationType=arcLength;
      printf("The Spline is now being parameterized by arclength and curvature");
    }
    else if( answer=="arc-length weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the arc-length weight (>=0., default=%6.2e) :",arcLengthWeight));
      if( line!="" )
      {
        sScanF(line,"%e",&arcLengthWeight);
	printf("New arcLengthWeight=%e  \n",arcLengthWeight);
	initialized=false;
	mappingHasChanged();
      }
    }
    else if( answer=="curvature weight" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the curvature weight (>=0., default=%6.2e) :",curvatureWeight));
      if( line!="" )
      {
        sScanF(line,"%e",&curvatureWeight);
	printf("New curvatureWeight=%e  \n",curvatureWeight);
	initialized=false;
	mappingHasChanged();
      }
    }
    else if( answer=="default boundary condition" )
    {
       // make default BC second derivative=0. The monotoneParabolicFit can go wild outside the domain.
      printf("The default end condition is second derivatives equal to zero\n");
      endCondition=secondDerivative; 
      bcValue=0.;
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="monotone parabolic fit" )
    {
      printf("WARNING: The monotone parabolic fit can go wild outside the domain [0,1]\n");
      endCondition=monotoneParabolicFit;
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="specify first derivatives at boundaries" )
    {
      // bcValue(0:1,axis)  0=start, 1=end
      endCondition=firstDerivative;

      RealArray r(2,1), x(2,3), xr(2,3,1);
      r(0,0)=0.;
      r(1,0)=1.;
      mapS(r,x,xr);

      if( rangeDimension==1 )
      {
	printf(" Current derivatives at ends: x'(0)=(%9.3e,%9.3e) x'(1)=%9.3e \n",
	       xr(0,0,0),xr(1,0,0));

        gi.inputString(line,sPrintF(buff,"Enter x'(0), x'(1)"));
        if( line!="" )
	  sScanF(line,"%e %e",&bcValue(0,0),&bcValue(1,0));
      }
      else if( rangeDimension==2 )
      {
	printf(" Current derivatives at ends: (x'(0),y'(0))=(%9.3e,%9.3e) (x'(1),y'(1))=(%9.3e,%9.3e) \n",
	       xr(0,0,0),xr(0,1,0),xr(1,0,0),xr(1,1,0));

        gi.inputString(line,sPrintF(buff,"Enter x'(0),y'(0),  x'(1),y'(1)"));
        if( line!="" )
	  sScanF(line,"%e %e %e %e",&bcValue(0,0),&bcValue(0,1),&bcValue(1,0),&bcValue(1,1));
      }
      else 
      {
	printf(" Current derivatives at ends: (x'(0),y'(0),z'(0))=(%9.3e,%9.3e,%9.3e) (x'(1),y'(1),z'(1))=(%9.3e,%9.3e,%9.3e) \n",
	       xr(0,0,0),xr(0,1,0),xr(0,2,0),xr(1,0,0),xr(1,1,0),xr(1,2,0));
	
        gi.inputString(line,sPrintF(buff,"Enter x'(0),y'(0),z'(0),  x'(1),y'(1),z'(1)"));
        if( line!="" )
	  sScanF(line,"%e %e %e %e %e %e",&bcValue(0,0),&bcValue(0,1),&bcValue(0,2),&bcValue(1,0),&bcValue(1,1),&bcValue(1,2));
      }
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="specify second derivatives at boundaries" )
    {
      endCondition=secondDerivative;
      if( rangeDimension==1 )
      {
        gi.inputString(line,sPrintF(buff,"Enter x''(0), x''(1)"));
        if( line!="" )
	  sScanF(line,"%e %e",&bcValue(0,0),&bcValue(1,0));
      }
      else if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter x''(0),y''(0),  x''(1),y''(1)"));
        if( line!="" )
	  sScanF(line,"%e %e",&bcValue(0,0),&bcValue(1,0),&bcValue(0,1),&bcValue(1,1));
      }
      else 
      {
        gi.inputString(line,sPrintF(buff,"Enter x''(0),y''(0),z''(0),  x''(1),y''(1),z''(1)"));
        if( line!="" )
	  sScanF(line,"%e %e",&bcValue(0,0),&bcValue(1,0),&bcValue(0,1),&bcValue(1,1),&bcValue(0,2),&bcValue(1,2));
      }
      initialized=false;
      mappingHasChanged();
    }
    else if( answer=="restrict the domain" )
    {
      printf("By default the spline is parameterized on the interval [0,1]\n"
             "You may choose a sub-section of the spline by choosing a new interval [rStart,rEnd]\n"
             "For periodic splines the interval may lie in [-1,2] so the sub-section can cross the branch cut.\n");
 
      gi.inputString(line,sPrintF(buff,"Enter the new interval rStart,rEnd  current=[%6.2e,%6.2e]\n",rStart,rEnd));
      if( line!="" )
      {
        real rStart_=0., rEnd_=1.;
        sScanF(line,"%e %e",&rStart_,&rEnd_);
        setDomainInterval(rStart_,rEnd_);
        mappingHasChanged();
      }
    }
    else if( answer=="scale" ) 
    {
      real xScale=1.; real yScale=1.; real zScale=1.;
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale (default=(%e,%e)): ",
            xScale,yScale));
        if( line!="" ) sScanF(line,"%e %e",&xScale,&yScale);
      }
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter xScale, yScale, zScale (default=(%e,%e,%e)): ",
            xScale,yScale,zScale));
        if( line!="" ) sScanF(line,"%e %e %e",&xScale,&yScale,&zScale);
      }
      scale(xScale,yScale,zScale);
      mappingHasChanged();
    }
    else if( answer=="shift" ) 
    {
      real xShift=0., yShift=0., zShift=0.;
      if( rangeDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift (default=(%e,%e)): ",
				    xShift,yShift));
	if( line!="" ) sScanF(line,"%e %e",&xShift,&yShift);
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter xShift, yShift, zShift (default=(%e,%e,%e)): ",
				    xShift,yShift,zShift));
	if( line!="" ) sScanF(line,"%e %e %e",&xShift,&yShift,&zShift);
      }
      shift(xShift,yShift,zShift);
      mappingHasChanged();
    }
    else if( answer=="rotate" ) 
    {
      int rotationAxis=2;
      real rotationAngle=45., centerOfRotation[3]={0.,0.,0.};
      if( rangeDimension==2 )
      {
        gi.inputString(line,sPrintF(buff,"Enter the rotation angle(degrees) (default=%e): ",
          rotationAngle));
        if( line!="" ) sScanF(line,"%e",&rotationAngle);
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e): ",
          centerOfRotation[0],centerOfRotation[1]));
        if( line!="" ) sScanF(line,"%e %e",&centerOfRotation[0],&centerOfRotation[1]);
      }        
      else
      {
        gi.inputString(line,sPrintF(buff,"Enter rotation angle(degrees) and axis to rotate about(0,1, or 2)"
				    "(default=(%e,%i)): ",rotationAngle,rotationAxis));
        if( line!="" ) sScanF(line,"%e %i",&rotationAngle,&rotationAxis);
	if( rotationAxis<0 || rotationAxis>2 )
	{
	  cout << "Invalid rotation axis = " << rotationAxis << endl;
	  continue;
	}
        gi.inputString(line,sPrintF(buff,"Enter the point to rotate around (default=%e,%e,%e): ",
				    centerOfRotation[0],centerOfRotation[1],centerOfRotation[2]));
        if( line!="" ) sScanF(line,"%e %e %e",&centerOfRotation[0],&centerOfRotation[1],
                              &centerOfRotation[2]);
      }
      shift(-centerOfRotation[0],-centerOfRotation[1],-centerOfRotation[2]);
      rotate(rotationAxis,rotationAngle*Pi/180.);
      shift(+centerOfRotation[0],+centerOfRotation[1],+centerOfRotation[2]);
      mappingHasChanged();
    }
    else if( answer=="project onto a plane" )
    {
      if( rangeDimension==3 && numberOfSplinePoints>0 )
      {
        real x0=0.,y0=0.,z0=0.;
        real nv[3]={0.,0.,1.};

        gi.inputString(line,sPrintF(buff,"Enter a point on the plane, x,y,z"));
	if( line!="" ) sScanF(line,"%e %e %e",&x0,&y0,&z0);
        gi.inputString(line,sPrintF(buff,"Enter the normal to the plane, nx,ny,nz"));
	if( line!="" ) sScanF(line,"%e %e %e",&nv[0],&nv[1],&nv[2]);
        real norm = SQRT( SQR(nv[0])+SQR(nv[1])+SQR(nv[2]) );
        if( norm==0. )
          norm=1.;
        Range I(0,numberOfSplinePoints-1);
        const RealArray & dot = evaluate( (knots(I,0)-x0)*nv[0]+(knots(I,1)-y0)*nv[1]+(knots(I,2)-z0)*nv[2] );
       	for( int axis=0; axis<rangeDimension; axis++ )
          knots(I,axis)-=dot(I)*(nv[axis]/(norm*norm));
	
	initialized=false;
        mappingHasChanged();
      }
    }
    else if( answer=="show parameters" )
    {
      printf(" shapePreserving = %i \n"
             " tension = %6.2e \n"
             " endCondition = %s \n"
             " parameterization = %s \n"
             " rStart=%8.2e rEnd=%8.2e\n"
             " arcLengthWeight = %6.2e \n"
             " curvatureWeight = %6.2e \n",
             shapePreserving,tension, 
             endCondition==free ? "free" : 
             endCondition==firstDerivative ? "specify first derivative" : 
             endCondition==secondDerivative ? " specify second derivative" : "montone parabolic fit",
             parameterizationType==index ? "index     " : "arc-length",
             rStart,rEnd,
             arcLengthWeight,curvatureWeight );

      ::display(knots,"knots","%7.4f ");
      ::display(s,"parameterization: s","%7.4f ");
      
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="plot knots" )
    {
      plotKnots=true; plotObject=true;
    }
    else if( answer=="plot first derivatives" || answer=="plot second derivatives" )
    {
      const int order = answer=="plot first derivatives" ? 1 : 2;
      if( order==1 )
        parameters.set(GI_TOP_LABEL,"First derivatives: xr=green, yr=red");
      else
        parameters.set(GI_TOP_LABEL,"Second derivatives: xrr=green, yrr=red");

      gi.erase();
      const int n = getGridDimensions(0);
      real dr = 1./max(1,n-1);
      RealArray r(n,1);
      r.seqAdd(0.,dr);
      
      RealArray x(n,rangeDimension),xr(n,rangeDimension,1);
      if( order==1 )
        mapS(r,x,xr);
      else
      {
        Index I = Range(n);
	for(  int axis=0; axis<rangeDimension; axis++ )
	{
          #ifndef USE_PPP
	  secondOrderDerivative(I,r,xr,axis,0);
          #else
	  printF("Finish secondOrderDerivative for parallel\n");
          #endif
	}
	
      }
    
      real xrMax=max(fabs(xr));
      xr*=1./max(REAL_MIN*100.,xrMax);
      printf("Derivative of spline function scaled by %8.2e\n",xrMax);
      
      RealArray xrd(n,1,1,2);
      Range R=n;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xrd(R,0,0,0)=r(R,0);
	xrd(R,0,0,1)=xr(R,axis);
      
	DataPointMapping xrMap;
        #ifndef USE_PPP
	xrMap.setDataPoints(xrd,3,1);
        #else
         printF("Finish xrMap.setDataPoints for parallel\n");
        #endif

	xrMap.setIsPeriodic(axis1,getIsPeriodic(axis1));

	if( axis==0 )
	  parameters.set(GI_MAPPING_COLOUR,"green");
	else if( axis==1 )
	  parameters.set(GI_MAPPING_COLOUR,"red");
	else 
	  parameters.set(GI_MAPPING_COLOUR,"yellow");

        if( axis==rangeDimension-1 )
          parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot(gi,xrMap,parameters);  
      }
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
    else if( answer=="do not plot knots" )
    {
      plotKnots=false; plotObject=true;
    }
    else if( answer=="plot grid points" )
    {
      plotGridPoints=true; plotObject=true;
    }
    else if( answer=="do not plot grid points" )
    {
      plotGridPoints=false; plotObject=true;
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="check points" )
    {
      Mapping::debug=31;
      RealArray r(1,1),x(1,3),xr(1,3,1),ss(1,1);
      ss=0.;
      for(;;)
      {
	gi.inputString(answer,"Evaluate the spline at which point r? (hit return to continue)");
        if( answer!="" )
	{
	  sScanF(answer,"%e",&r(0,0));
          mapS(r,x,xr);
	  inverseMapS(x,ss);
	  printf(" r=%e, x=(%6.2e,%6.2e) xr=(%6.2e,%6.2e) inverse=%e\n",r(0,0),x(0,0),x(0,1),
                 xr(0,0,0),xr(0,1,0),ss(0,0));
	}
	else
	{
	  break;
	}
      }
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
      if( answer=="periodicity" )
      {
        splineIsPeriodic=getIsPeriodic(axis1);
	initialized=false;
      }
    }
    else if( answer=="show spline outside boundaries" )
    {
      showSplineOutsideBoundaries=true;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
    }


    if( plotObject )
    {
      gi.setAxesDimension(2);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,plotGridPoints);
      parameters.set(GI_POINT_COLOUR,gi.getColour(GenericGraphicsInterface::textColour));
      
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  

      
      parameters.set(GI_USE_PLOT_BOUNDS,true); 
      if( plotKnots )
      {
        parameters.set(GI_POINT_COLOUR,"blue");

	if( initialized && rangeDimension==1 )
	{
	  Range R(0,numberOfSplinePoints-1);
	  RealArray points(numberOfSplinePoints,2);
	  points(R,0)=s(R);
	  points(R,1)=knots(R,0);
          #ifndef USE_PPP
	  gi.plotPoints(points,parameters);
          #endif
	}
	else
	{
          #ifndef USE_PPP
	  gi.plotPoints(knots,parameters);
          #endif
	}
	
      }
      

      if( showSplineOutsideBoundaries )
      {
	int n = max(101,getGridDimensions(0));
	real dr = 1./max(1,n-1);
	const real a=-.05, b=1.05;
	n=int( (b-a)/dr+.5);
	Range R(0,n);
	RealArray r(R,1),x(R,3); // rangeDimension);
      
	r.seqAdd(a,dr);
	mapS(r,x);
        
        #ifndef USE_PPP
	gi.plotPoints(x,parameters);
        #endif
      }


      parameters.set(GI_USE_PLOT_BOUNDS,false); 
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}
