// define BOUNDS_CHECK
#include "NurbsMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include <float.h>
#include "IgesReader.h"
#include "DataPointMapping.h"
#include "display.h"
#include "ArraySimple.h"
#include "MappingsFromCAD.h"
#include "ParallelUtility.h"

#ifdef GETLENGTH
#define GET_LENGTH dimension
#else
#define GET_LENGTH getLength
#endif

#define DGECO EXTERN_C_NAME(dgeco)
#define SGECO EXTERN_C_NAME(sgeco)
#define DGESL EXTERN_C_NAME(dgesl)
#define SGESL EXTERN_C_NAME(sgesl)

#ifdef OV_USE_DOUBLE
#define GECO DGECO
#define GESL DGESL
#else
#define GECO SGECO
#define GESL SGESL
#endif

real timeToMergeNurbs=0.;
real timeToMergeNurbsAddSubCurve=0.;
real timeToMergeNurbsArcLength=0.;
real timeToMergeNurbsElevateDegree=0.;
real timeToMergeNurbsOther=0.;

extern "C"
{
  void SGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void DGECO( real & b, const int & nbd, const int & nb, int & ipvt,real & rcond, real & work );

  void SGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);

  void DGESL(real & a,const int & lda,const int & n,int & ipvt, real & b, int & job);
  
}

NurbsMapping::
NurbsMapping() : Mapping(1,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor, make a null NURBS. 
/// \param Remarks:
///     The implementation here is based on the reference, {\sl The NURBS Book}
///  by Les Piegl and Wayne Tiller, Springer, 1997. The notation here is:
///  <ul>
///    <li> degree = p  (variables p1,p2 for one and 2D)
///    <li>  number of control points is n+1 (variables n1,n2)
///    <li>  number of knots is m+1 (m=n+p+1) (variables m1,m2)
///    <li> cPoint(0:n,0:r) :  holds the control points and weights. r=rangeDimension.
///    <li> uKnot(0:m) : holds knots along axis1. These are normally scaled to [0,1] (see notes below).
///    <li> vKnot(0:m) : holds knots along axis2 (if domainDimension==2)  
///    <li> note : Knots are scaled to [0,1]
///  </ul>
///  
/// \param NOTES: for those wanting to make changes to this class
/// 
///    {\bf uMin,uMax,vMin,vMax} : A typical NURBS will have knots that span an arbitrary interval.
///      For example the knots may go from $[.5,1.25]$. This mapping however, is parameterized on [0,1].
///      To fix this we first save the actual min and max values for uKnot in [uMin,uMax] and similarly
///      for [vMin,vMax]. We then rescale uKnot and vKnot to lie on the interval [0,1]. Note that
///      the {\tt reparameterize} function may subsequently rescale the knots to a larger interval
///      in which case the NURBS will only represent a part of the initial surface. If we do this then
///      we also rescale uMin,uMax,vMin,vMax. The {\tt parametricCurve} function is used to indicate
///      that this NURBS is actually a parametric curve on another NURBS, nurbs2. By default the values of 
///      uMin,uMax,vMin,vMax from nurbs2 are used to scale this NURBS in order to make it compatible with
///      the rescaled nurbs2.
///      
//===========================================================================
{ 
  NurbsMapping::className="NurbsMapping";
  setName( Mapping::mappingName,"nurbsMapping");

  // *wdh* setBasicInverseOption(canInvert);  // basicInverse is available

  inverseIsDistributed=false;

  initialized=false;
  mappingNeedsToBeReinitialized=false;  // **** no longer needed ***
  numberOfCurves=1;
  subCurves=0;
  subCurveState = 0;
  lastVisible = 0;

  p1=0;
  n1=0;
  m1=1;
  p2=0;
  n2=0;
  m2=0;
  p3=0;
  n3=0;
  m3=0;
  uMin=0.;
  uMax=1.;
  vMin=0.;
  vMax=1.;
  wMin=0.;
  wMax=1.;

  rStart[0]=rStart[1]=rStart[2]=0.;
  rEnd[0]=rEnd[1]=rEnd[2]=1.;
  nurbsIsPeriodic[0]=nurbsIsPeriodic[1]=nurbsIsPeriodic[2]=notPeriodic;

  use_kk_nrb_eval = false;
}


NurbsMapping::
NurbsMapping(const int & domainDimension_ , const int & rangeDimension_ ) 
: Mapping(1,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Constructor, make a default NURBS of the give domain dimension (1,2)
//===========================================================================
{ 
  NurbsMapping::className="NurbsMapping";
  setName( Mapping::mappingName,"nurbsMapping");

  // *wdh*  setBasicInverseOption(canInvert);  // basicInverse is available

  inverseIsDistributed=false;
  initialized=false;
  mappingNeedsToBeReinitialized=false;
  numberOfCurves=1;
  subCurves=0;
  subCurveState = 0;
  lastVisible = 0;

  rStart[0]=rStart[1]=rStart[2]=0.;
  rEnd[0]=rEnd[1]=rEnd[2]=1.;
  nurbsIsPeriodic[0]=nurbsIsPeriodic[1]=nurbsIsPeriodic[2]=notPeriodic;

  if( domainDimension_ < 1 || domainDimension_ > 3  )
  {
    printF("NurbsMapping:constructor:ERROR: domainDimension must be 1, 2 or 3, the value %i is invalid \n",
            domainDimension_);
    OV_ABORT("error");
  }
  if( rangeDimension_<2 || rangeDimension_>3 )
  {
    printf("NurbsMapping:constructor:ERROR: rangeDimension should be 2 or 3, the value %i is invalid. \n",
	   rangeDimension);
    OV_ABORT("error");
  }
  
  setDomainDimension(domainDimension_);
  setRangeDimension(rangeDimension_);

  int nw=rangeDimension;  // position of the weight
  if( domainDimension==1 )
  {
    setGridDimensions( axis1,21 );

    p1=3;
    n1=p1;
    m1=7;

    p2=0;
    n2=0;
    m2=0;

    p3=0;
    n3=0;
    m3=0;

    // knots are clamped
    uKnot.redim(m1+1);
    uKnot(0)=0.; uKnot(1)=0.; uKnot(2)=0.; uKnot(3)=0.; uKnot(4)=1.; uKnot(5)=1.; uKnot(6)=1.; uKnot(7)=1.;
    uMin=0.;
    uMax=1.;

  // control points (holds weight in last position)
    cPoint.redim(n1+1,rangeDimension+1);   
    if( rangeDimension==2 )
    {
      cPoint(0,0)=0.;  cPoint(0,1)=0.; cPoint(0,2)=1.;
      cPoint(1,0)=.25; cPoint(1,1)=1.; cPoint(1,2)=2.;
      cPoint(2,0)=.75; cPoint(2,1)=1.; cPoint(2,2)=2.;
      cPoint(3,0)=1.;  cPoint(3,1)=0.; cPoint(3,2)=1.;
    }
    else
    {
      cPoint(0,0)=0.;  cPoint(0,1)=0.; cPoint(0,2)=0.; cPoint(0,3)=1.;
      cPoint(1,0)=.25; cPoint(1,1)=.7; cPoint(1,2)=.2; cPoint(1,3)=2.;
      cPoint(2,0)=.75; cPoint(2,1)=1.; cPoint(2,2)=.2; cPoint(2,3)=2.;
      cPoint(3,0)=1.;  cPoint(3,1)=0.; cPoint(3,2)=.5; cPoint(3,3)=1.;
    }
    Range R1(0,n1);
    for( int axis=0; axis<rangeDimension; axis++ )
      cPoint(R1,axis)*=cPoint(R1,nw); // multiply by the weight

  }
  else if( domainDimension==2 )
  {
    setGridDimensions( axis1,21 );
    setGridDimensions( axis2,21 );
  
    p1=3;
    n1=p1;
    m1=7;

    p2=3;
    n2=p2;
    m2=7;

    p3=0;
    n3=0;
    m3=0;

    // knots are clamped
    uKnot.redim(m1+1);
    uKnot(0)=0.; uKnot(1)=0.; uKnot(2)=0.; uKnot(3)=0.; uKnot(4)=1.; uKnot(5)=1.; uKnot(6)=1.; uKnot(7)=1.;
    uMin=0.;
    uMax=1.;

    vKnot.redim(m2+1);
    vKnot(0)=0.; vKnot(1)=0.; vKnot(2)=0.; vKnot(3)=0.; vKnot(4)=1.; vKnot(5)=1.; vKnot(6)=1.; vKnot(7)=1.;
    vMin=0.;
    vMax=1.;
    wMin=0.;
    wMax=0.;

    // control points
    cPoint.redim(n1+1,n2+1,4);


    RealArray pPoint(n1+1,3);  // assumes n1=n2
    pPoint(0,0)=0.;  pPoint(0,1)=0.;  pPoint(0,2)=0.;
    pPoint(1,0)=.25; pPoint(1,1)=.25; pPoint(1,2)=.7;
    pPoint(2,0)=.75; pPoint(2,1)=.75; pPoint(2,2)=1.;
    pPoint(3,0)=1.;  pPoint(3,1)=1.;  pPoint(3,2)=0.;

    for( int i2=0; i2<=n2; i2++ )
    {
      for( int i1=0; i1<=n1; i1++ )
      {
	cPoint(i1,i2,0)=pPoint(i1,0);
	cPoint(i1,i2,1)=pPoint(i2,1);
	cPoint(i1,i2,2)=pPoint(i1,2)*pPoint(i2,2);
        cPoint(i1,i2,3)=1.; // +i1*(n1-i1)+i2*(n2-i2);  // weight
      }
    }

    Range R1(0,n1), R2(0,n2);
    for( int axis=0; axis<rangeDimension; axis++ )
      cPoint(R1,R2,axis)*=cPoint(R1,R2,3); // multiply by the weight
    
  }
  else if( domainDimension==3 )
  {
    printF("NurbsMapping::constructor: finish me for domainDimension=3\n");
  }

  use_kk_nrb_eval = false;

  if( domainDimension==1 || domainDimension==2 )
  {
    initialize();
  }
  
}

// Copy constructor is deep by default
NurbsMapping::
NurbsMapping( const NurbsMapping & map, const CopyType copyType )
{
  NurbsMapping::className="NurbsMapping";
  numberOfCurves=0;
  
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "NurbsMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

NurbsMapping::
~NurbsMapping()
{ 
  if( Mapping::debug & 4 )
    cout << "****** NurbsMapping::Destructor called, name = " << (const char*) getName(mappingName) << endl;

  if( numberOfCurves>1 )
  {
    for( int c=0; c<numberOfCurves; c++ )
    {
      if( subCurves[c]->decrementReferenceCount()==0 )
        delete subCurves[c];
    }
    delete [] subCurveState;
    delete [] subCurves;
    lastVisible = 0;
  }
  else
  {
    assert( subCurves==0 );
  }
}

NurbsMapping & NurbsMapping::
operator=( const NurbsMapping & x )
{
  if( NurbsMapping::className != x.getClassName() )
  {
    cout << "NurbsMapping::operator= ERROR trying to set a NurbsMapping = to a" 
      << " mapping of type " << x.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(x);            // call = for derivee class

  n1               =x.n1; 
  m1               =x.m1;
  p1               =x.p1;   
  n2               =x.n2;
  m2               =x.m2;
  p2               =x.p2;
  n3               =x.n3;
  m3               =x.m3;
  p3               =x.p3;
  uKnot.redim(0);
  uKnot            =x.uKnot;
  vKnot.redim(0);
  vKnot            =x.vKnot;
  wKnot.redim(0);
  wKnot            =x.wKnot;
  cPoint.redim(0);
  cPoint           =x.cPoint;
  initialized      =x.initialized;
  nonUniformWeights=x.nonUniformWeights;
  uMin             =x.uMin;
  uMax             =x.uMax;
  vMin             =x.vMin;
  vMax             =x.vMax;
  wMin             =x.wMin;
  wMax             =x.wMax;
  for( int axis=0; axis<3; axis++ )
  {
    rStart[axis]=x.rStart[axis];
    rEnd[axis]=x.rEnd[axis];
    nurbsIsPeriodic[axis]=x.nurbsIsPeriodic[axis];
  }
  mappingNeedsToBeReinitialized=x.mappingNeedsToBeReinitialized;
  if( numberOfCurves>1 )
  {
    for( int c=0; c<numberOfCurves; c++ )
    {
      if( subCurves[c]->decrementReferenceCount()==0 )
        delete subCurves[c];
    }
    delete [] subCurveState;
    delete [] subCurves;
  }
  numberOfCurves=x.numberOfCurves;

  if( numberOfCurves>1 )
  {
    subCurves = new NurbsMapping* [numberOfCurves];
    subCurveState = new char [numberOfCurves];
    assert( subCurves!=NULL );
    for( int c=0; c<numberOfCurves; c++ )
      {
       subCurves[c] = new NurbsMapping;
       subCurves[c]->incrementReferenceCount();
       *subCurves[c] = *(x.subCurves[c]);
       subCurveState[c] = x.subCurveState[c];
      }
  } 
  else
  {
    subCurves=NULL;
    subCurveState=NULL;
  }
  
    
  lastVisible = x.lastVisible;

// only call initialize() if x is initialized, i.e., non-empty.
  use_kk_nrb_eval = x.use_kk_nrb_eval;
  if (x.initialized)
  { // *wdh* Why do we need to initialize at all ??  

    if( false )
    {
      initialize();
    }
    else
    {
      // *wdh* 070727: do not call mappingHasChanged -- no need to rebuild the grid.
      bool setMappingHasChanged=false;
      initialize(setMappingHasChanged); 

      // In parallel, gridSerial should be the entire grid since the inverse is NOT distributed
//       printf("**NurbsMapping::operator= : gridIndexRange=[%i,%i] gridSerial=[%i,%i]\n",
//              gridIndexRange(0,0),gridIndexRange(1,0),gridSerial.getBase(0),gridSerial.getBound(0));

    }
  }
  
  // *wdh* 070727: initialize will call mappinghasChanged. However there is no need to remake the grid
  // or gridSerial if it was valid in x:

//   remakeGrid = x.remakeGrid;
//   remakeGridSerial = x.remakeGridSerial;
  
  return *this;
}


int NurbsMapping::
intersect3DLines( const RealArray & pt0, const RealArray &  t0, 
                  const RealArray & pt1, const RealArray & t1,
                  real & alpha0, real & alpha1,
		  RealArray & pt2) const
// ================================================================================
/// \details 
///     Intersect two lines in 3D:
///  x0(s) = pt0 + s * t0
///  x1(t) = pt1 + t * t1
/// 
/// \param alpha0,alpha1 : values of s and t at the intersection.
/// \param pt2 : point of intrsection, x0(alpha0)=pt2=x1(alpha1)
/// 
/// \return  1 if the line are parallel, 0 otherwise.
// ================================================================================
{

  //  pt0 + s * t0 = pt1 + t * t1
  //    A [ s t]^T = pt1 -pt0  == f
  // A = [ t0 -t1 ]
  // Least squares solution:  
  //     [ t0.t0 -t0.t1 ] [ s ] = [ t0.f ] 
  //     [-t1.t0  t1.t1 ] [ t ] = [-t1.f ] 

  real a00= sum(t0*t0);
  real a01= sum(t0*t1);
  real a11= sum(t1*t1);

  real det=a00*a11-a01*a01;
  if( fabs(det)<=REAL_EPSILON*a00 )
    return 1;
  
  RealArray f(1,3);
  f=pt1-pt0;
  real f0=sum(t0*f);
  real f1=sum(-t1*f);
  alpha0=(f0*a11+f1*a01)/det;
  alpha1=(f0*a01+f1*a00)/det;

  pt2=pt0+alpha0*t0;
  
  return 0;
}

int NurbsMapping::
buildCurveOnSurface(NurbsMapping & curve,
                    real r0, 
                    real r1 /* =-1. */ )
//===========================================================================
/// \details 
///    Build a new Nurbs curve  that matches a coordinate line on the surface.
/// \param curve (output) : on output a curve that matches a coordinate line on the surface.
/// \param r0,r1 (input) : if r1==-1 make a curve ${\mathbf c}(r) = {\mathbf s}(r0,r)$ where ${\mathbf s}(r_0,r_1)$
///    is the NURBS surface defined by this mapping. If r0==-1   
///      make the curve ${\mathbf c}(r) = {\mathbf s}(r,r1)$ 
///   the arc, measured starting from x.
/// 
//===========================================================================
{
  if( domainDimension!=2 || rangeDimension!=3  )
  {
    printf("NurbsMapping::buildCurveOnSurface:ERROR:this NurbsMapping must be a 3D surface!\n");
    return 1;
  }
  

  int axis,m,n,p;
  if( fabs(r1+1.) < REAL_EPSILON*10. )
  {
    if( r0< -.01 || r0>1.01 )
      printf("NurbsMapping::buildCurveOnSurface:ERROR: r0=%e, r1=%e\n",r0,r1);
    
    axis=1;
    m=m2;
    n=n2;
    p=p2;
  }
  else
  {
    if( r1< -.01 || r1>1.01 )
      printf("NurbsMapping::buildCurveOnSurface:ERROR: r0=%e, r1=%e\n",r0,r1);

    axis=0;
    m=m1;
    n=n1;
    p=p1;
  }
  

  RealArray & knot = axis==0 ? uKnot : vKnot;
  Range I=n+1;
  RealArray cp(n+1,4);
  cp=0.;
  
  const int order=0; // only compute function (not deriavative)
  if( axis==1 )
  {
    RealArray uDers(2,p1+1);
    int span = findSpan( n1,p1,r0,uKnot );
    dersBasisFuns(span,r0,p1,order,uKnot,uDers.getDataPointer()); 
    for( int i2=0; i2<=n2; i2++ )
    {
      for( int ii=0; ii<=p1; ii++ )
      {
	cp(i2,0)+=uDers(0,ii)*cPoint(span-p1+ii,i2,0);
	cp(i2,1)+=uDers(0,ii)*cPoint(span-p1+ii,i2,1);
	cp(i2,2)+=uDers(0,ii)*cPoint(span-p1+ii,i2,2);
	cp(i2,3)+=uDers(0,ii)*cPoint(span-p1+ii,i2,3);
      }
    }
  }
  else
  {
    RealArray vDers(2,p2+1);
    int span = findSpan( n2,p2,r1,vKnot );
    dersBasisFuns( span,r1,p2,order,vKnot,vDers.getDataPointer()); 
    for( int ii=0; ii<=p2; ii++ )
    {
      cp(I,0)+=vDers(0,ii)*cPoint(I,span-p2+ii,0);
      cp(I,1)+=vDers(0,ii)*cPoint(I,span-p2+ii,1);
      cp(I,2)+=vDers(0,ii)*cPoint(I,span-p2+ii,2);
      cp(I,3)+=vDers(0,ii)*cPoint(I,span-p2+ii,3);
    }



  }
  if( !nonUniformWeights )
  {
    // weights are uniform, we should not divide by the weight in cp(I,3) since
    // it may not be valid. (it is the original value )
    cp(I,3)=1.;
  }
  
  real wMin=min(cp(I,3));
  real wMax=max(cp(I,3));
  if( fabs(wMax-wMin) < REAL_EPSILON*10.*max(fabs(wMax),fabs(wMin)) )
  {
    // weights are constant 
    // printf("buildCurveOnSurface: weights are constant, wMax=%e\n",wMax);
    
    if( fabs(wMax-1.) > REAL_EPSILON*10. )
    {
      cp(I,0)/=wMax;
      cp(I,1)/=wMax;
      cp(I,2)/=wMax;
      cp(I,3)=1.;
    }
  }
  else
  {
    cp(I,0)/=cp(I,3);  // we divide by the weights since the cp(I,0:2) are stored as cp*weight
    cp(I,1)/=cp(I,3);
    cp(I,2)/=cp(I,3);
  }
  
  // ::display(knot,"buildCurveOnSurface: knot");
  

  bool normalizeTheKnots=false;
  curve.specify(m,n,p,knot,cp,rangeDimension,normalizeTheKnots);

  // set the domain interval: (the knots may not be from [0,1])
//   printf("Set domain interval [%e,%e], uMin=%e, uMax=%e, vMin=%e, vMax=%e\n",rStart[axis],rEnd[axis],
//        uMin,uMax,vMin,vMax);
  curve.rStart[0]=rStart[axis];
  curve.rEnd[0]=rEnd[axis];

  return 0;
}


int NurbsMapping::
buildComponentCurve(NurbsMapping & curve,
                    int component /* = 0 */ )
//===========================================================================
/// \details 
///      Given a curve, (x(s),y(s)[,z(s)]),  in 2D or 3D, build a new curve in 1D that 
///   represents the a given component such as x(s) or y(s) or z(s).
/// \param curve (output) : on output a component curve.
/// \param component (input) : Choose component=0,1, or 2 to form the curve for x,y, or z
/// 
//===========================================================================
{
  if( domainDimension!=1 ) 
  {
    printf("NurbsMapping::buildComponentCurve:ERROR:this NurbsMapping must be a curve! (the domainDimension must be 1)\n");
    return 1;
  }
  if( component<0 || component>=rangeDimension  )
  {
    printf("NurbsMapping::buildComponentCurve:ERROR:invalid component=%i! The component should be\n"
           "  between 0 and %i since the rangeDimension=%i\n",component,rangeDimension-1,rangeDimension);
    return 1;
  }

  // We just need to use the same knots and extract a component from the control points.
  Range I=n1+1;
  RealArray cp(n1+1,2);

  cp(I,0)=cPoint(I,component);       // component we want
  cp(I,1)=cPoint(I,rangeDimension);  // weights
  
  // ::display(knot,"buildCurveOnSurface: knot");

  bool normalizeTheKnots=false;
  const int rangeDim=1;
  curve.specify(m1,n1,p1,uKnot,cp,rangeDim,normalizeTheKnots);

  // set the domain interval: (the knots may not be from [0,1])
  //   printf("Set domain interval [%e,%e], uMin=%e, uMax=%e, vMin=%e, vMax=%e\n",rStart[axis],rEnd[axis],
  //        uMin,uMax,vMin,vMax);
  curve.rStart[0]=rStart[0];
  curve.rEnd[0]=rEnd[0];

  return 0;
}




int NurbsMapping::
circle(RealArray & o,
       RealArray & x, 
       RealArray & y, 
       real r,
       real startAngle /* =0. */,
       real endAngle /* =1. */ )
//===========================================================================
/// \details 
///    Build a circular arc. Reference the NURBS book Algorithm A7.1
/// \param o (input): center of the circle.
/// \param x,y (input): orthogonal unit vectors in the plane of the circle.
/// \param startAngle,endAngle : normalized angles [0,1] for the start and end of
///   the arc, measured starting from x.
/// 
//===========================================================================
{
  domainDimension=1;
  rangeDimension=o.getLength(0);
  n2=p2=m2=0;
  
  assert( rangeDimension==3 );
  
  Range Rx=rangeDimension;
  o.reshape(1,Rx);
  x.reshape(1,Rx);
  y.reshape(1,Rx);
  
  startAngle*=twoPi;
  endAngle*=twoPi;
  
  while( endAngle<=startAngle )
    endAngle += twoPi;

  real theta = endAngle-startAngle ;

  int narcs;
  if( theta<=.5*Pi )
    narcs = 1;
  else
  {
    if( theta<=Pi )
      narcs = 2;
    else
    {
      if( theta<=1.5*Pi )
	narcs = 3;
      else
	narcs = 4;
    }
  }

  real dtheta = theta/narcs;
  if( dtheta<REAL_MIN*1000. )
  {
    printf("NurbsMapping::circle:WARNING: trying to make a circular arc with angle theta=%e -- too small!\n"
           "     radius=%e,  startAngle=%e, endAngle=%e, theta=endAngle-startAngle=%e \n"
           "     *** I am going to make theta small but finite\n",
	   theta,r,theta,startAngle,endAngle,theta);

    dtheta=1.e-3; // REAL_EPSILON*100.;
  }
    

  n1 = 2*narcs;         // n+1 control points
  real w1 = cos(dtheta/2.0); // dtheta/2.0 is base angle

  RealArray pp0(1,rangeDimension),t0(1,rangeDimension),pp2(1,rangeDimension),t2(1,rangeDimension),pp1(1,rangeDimension);

  pp0 = o + r*cos(startAngle)*x + r*sin(startAngle)*y; 
  t0 = -sin(startAngle)*x + cos(startAngle)*y; // initialize start values

  Range Rw=rangeDimension+1;
  cPoint.redim(n1+1,Rw);
  Range N=n1+1;
  cPoint(N,rangeDimension)=1.;

  cPoint(0,Rx) = pp0(0,Rx);

  int i;
  int index = 0;
  real angle = startAngle;
  real alpha0,alpha2;
  
  for( i=1; i<=narcs; i++)
   {
    angle += dtheta;
    pp2 = o+ r*cos(angle)*x + r*sin(angle)*y;  
    cPoint(index+2,Rx) = pp2;
    t2 = -sin(angle)*x + cos(angle)*y;
    intersect3DLines(pp0,t0,pp2,t2,alpha0,alpha2,pp1);
    cPoint(index+1,Rx) = pp1;
    cPoint(index+1,Rw) *= w1;
    index += 2;
    if(i<narcs)
    {
      pp0 = pp2;
      t0 = t2;
    }
  }
  int j = 2*narcs+1; // load the knot vector
  m1 =j+2;  // m1+1 = number of knots
  // m1 = n1+p1+1
  p1=m1-n1-1;
  
  uKnot.redim(m1+1);
  for( i=0; i<3; i++)
  {
    uKnot(i) = 0.0;
    uKnot(i+j) = 1.0;
  }
  switch(narcs)
  {
  case 1: break;
  case 2: 
    uKnot(3) = uKnot(4) = 0.5;
    break;
  case 3:
    uKnot(3) = uKnot(4) = 1.0/3.0;
    uKnot(5) = uKnot(6) = 2.0/3.0;
    break;
  case 4:
    uKnot(3) = uKnot(4) = 0.25;
    uKnot(5) = uKnot(6) = 0.50;  
    uKnot(7) = uKnot(8) = 0.75;
    break;    
  }

  o.reshape(Rx);
  x.reshape(Rx);
  y.reshape(Rx);

  initialize();
  
  if( fabs(theta/twoPi-1.)<100.*REAL_EPSILON )
  {
    setIsPeriodic(axis1,functionPeriodic);
  }
  

  return 0;
}

int NurbsMapping::
splitArc(const RealArray &pt0, const RealArray &pt1, const real & w1, const RealArray &pt2, 
         RealArray &q1, RealArray &s, RealArray &r1, real &wqr) const
// ========================================================================================================
// /Description: Split a conic arc (c.f. NURBS book p. 315)
// /pt0,pt1,w1,pt2 (input): w1=0 => infinite control point
// /q1,s,r1,wqr : output
// ========================================================================================================
{

  q1=(pt0+w1*pt1)/(1.+w1);
  r1=(w1*pt1+pt2)/(1.+w1);

//    printf(" splitArc: w1=%e\n",w1);
  
//    pt0.display("splitArc: pt0");
//    pt1.display("splitArc: pt1");
//    pt2.display("splitArc: pt2");
//    q1.display("splitArc: q1");
//    r1.display("splitArc: r1");
  
  // real wq,wr;
  // wq=wr=.5*(1.+w1);

  s=.5*(q1+r1);

  real ws=.5*(1.+w1);
  
  wqr=sqrt((1+w1)/2.);

  return 0;
}

int NurbsMapping::
makeOneArc( const RealArray &pt0, const RealArray &t0, const RealArray &pt2, const RealArray &t2, const RealArray &p,
            RealArray &pt1, real&w1 ) const
// ========================================================================================================
// /Description: Make a conic arc (c.f. NURBS book p. 314)
// /pt1,w1 : output
// ========================================================================================================
{
  RealArray v02(1,3),v1p(1,3),ptemp(1,3);
  
  v02=pt2-pt0;
  real alpha0,alpha2;
  int i = intersect3DLines(pt0,t0,pt2,t2,alpha0,alpha2,pt1);

//    ::display(pt0,"makeOneArc: pt0");
//    ::display(t0,"makeOneArc: t0");
//    ::display(pt2,"makeOneArc: pt2");
//    ::display(t2,"makeOneArc: t2");
//    ::display(p ,"makeOneArc: p ");
//    ::display(pt1,"makeOneArc: pt1");

  if( i==0 )
  {
    // finite control point
    v1p=p-pt1;
    intersect3DLines(pt1,v1p,pt0,v02,alpha0,alpha2,ptemp);
    real a=sqrt(alpha2/(1.-alpha2));
    real u= a/(1.+a);
    real num = (1.-u)*(1.-u)*sum( (p-pt0)*(pt1-p) ) + u*u*sum( (p-pt2)*(pt1-p) );
    real den = 2.*u*(1.-u)*sum( (pt1-p)*(pt1-p) );
    w1=num/den;
    
//  printf("makeOneArc: alpha0=%e alpha2=%e a=%e u=%e u*u=%e (1-u)^2=%e \n",alpha0,alpha2,a,u,u*u,(1.-u)*(1.-u));
    

  }
  else
  {
    // Infinite control pt, 180 degree arc
    w1=0.;
    intersect3DLines(p,t0,pt0,v02,alpha0,alpha2,ptemp);
    real a=sqrt(alpha2/(1.-alpha2));
    real u= a/(1.+a);
    real b = 2.*u*(1.-u);
    b=-alpha0*(1.-b)/b;
    pt1=b*t0;
  }

  // ::display(pt1,"makeOneArc:done: pt1");
  return 0;
}

real NurbsMapping::
angle( const RealArray &pt0, const RealArray &pt1, const RealArray &pt2 ) const
// ========================================================================================================
// /Description: 
//     Determine the angle (in radians) between two vectors (pt0,pt1) and (pt1,pt2)   
//
// /pt0,pt1,pt2: (input)
//
// /return values: angle in [0,pi]
//
// ========================================================================================================
{
  
  real aNorm1=sum((pt0-pt1)*(pt0-pt1));
  real aNorm2=sum((pt2-pt1)*(pt2-pt1));
  const real eps=REAL_MIN*1000.;
  if( aNorm1<eps ) 
    aNorm1=1.;
  else
    aNorm1=sqrt(aNorm1);
  if( aNorm2<eps ) 
    aNorm2=1.;
  else
    aNorm2=sqrt(aNorm2);

  real cosTheta = sum( (pt0-pt1)*(pt2-pt1) )/ (aNorm1*aNorm2); 

  return acos(cosTheta);
}

int NurbsMapping::
conic( const real a, const real b, const real c, const real d, const real e, const real f, 
       const real z, const real x1, const real y1, const real x2, const real y2 )
// ========================================================================================================
/// \details 
///  Build a NURBS for a conic defined by an implicit formula and two end points
///  \begin{verbatim}
///       a*x^2 + b*x*y + c*y^2 + d*x + e*y + f = 0
///  \end{verbatim}
/// 
/// \param a,b,c,d,e,f : implicit formula for a conic
/// \param z,x1,y1,x2,y2: the end points of the conic are (x1,y1,z) and (x2,y2,z)
/// 
/// \param return values: 0=success, 1=error
/// 
//================================================================================================================
{
  real q1 = a*( c*f-e*e/4.) - (b/2.)*( (b/2.)*f-d*e/4. ) + (d/2.)*( b*e/4.-c*d/2. );
  real q2 = a*c-b*b/4.;
  real q3 = a+c;
  
  enum ConicTypeEnum
  {
    ellipse,
    hyperbola,
    parabola
  } conicType;
  if( q2>0. && q1*q3 <= 0. )
  {
    conicType=ellipse;
  }
  else if( q2<0. && q1!=0. )
    conicType=hyperbola;
  else if( q2==0. && q1!=0. )
    conicType=parabola;
  else
  {
    printf("NurbsMapping::conic: ERROR unknown conic type: a,b,c,d,e,f=%f,%f,%f,%f,%f,%f, q1=%e, q2=%e q3=5e\n",
	   a,b,c,d,e,f,q1,q2,q3);
    Overture::abort("error");
  }

  // determine the tangents and an interior point

  RealArray pt0(1,3), t0(1,3), pt2(1,3), t2(1,3), p(1,3);
  
  pt0(0,0)=x1; pt0(0,1)=y1; pt0(0,2)=z;
  pt2(0,0)=x2; pt2(0,1)=y2; pt2(0,2)=z;

  real x1t,y1t,x2t,y2t,xp,yp;
  
  if( conicType==parabola )
  {
    if( a!=0. && e!=0. && x1!=x2 )
    {
      if( x1<x2 )
      {
	x1t=1.;
	y1t=-(a/e)*2.*x1;

	x2t=1.;
	y2t=-(a/e)*2.*x2;
      }
      else
      {
	x1t=-1.;
	y1t= (a/e)*2.*x1;
	x2t=-1.;
	y2t= (a/e)*2.*x2;

      }
      // here is another point on the parabola
      xp = (x1+x2)/2.;
      yp = -(a/e)*xp*xp;
    }
    else if( c!=0. && d!=0. && y1!=y2 )
    {
      if( y1<y2 )
      {
	y1t=1.;
	x1t=-(c/d)*2.*y1;

	y2t=1.;
	x2t=-(c/d)*2.*y2;
      }
      else
      {
	y1t=-1.;
	x1t= (c/d)*2.*y1;
	y2t=-1.;
	x2t= (c/d)*2.*y2;
      }
      // here is another point on the parabola
      yp = (y1+y2)/2.;
      xp = -(c/d)*yp*yp;
    }
    else
    {
      printf("NurbsMapping::conic:ERROR: parabola but x1,x2,y1,y2,a,e,c,d=%e,%e,%e,%e,%e\n",x1,x2,y1,y2,a,e,c,d);
      Overture::abort("error");
    }
  }
  else if( conicType==ellipse )
  {
    // C(t) = (ae*cos(s),be*sin(s))
    assert( (-f/a)>=0. && (-f/c)>=0. );
    
    real ae=sqrt(-f/a), be=sqrt(-f/c);
    
    // (x1,y1)=(ae*cos(s1),be*sin(s1))
    // (x2,y2)=(ae*cos(s2),be*sin(s2))
    //   0 <= s1 <= 2pi
    //   0 <= s2-s1 <= 2pi

    real s1=atan2((double)y1/be,(double)x1/ae);
    if( s1<=0. ) s1+=2.*Pi;

    real s2=atan2((double)y2/be,(double)x2/ae);
    if( s2<s1 ) s2+=2.*Pi;
    

    x1t=-ae*sin(s1);
    y1t= be*cos(s1);

    x2t=-ae*sin(s2);
    y2t= be*cos(s2);
    
    real sm=.5*(s1+s2);
    xp= ae*cos(sm);
    yp= be*sin(sm);

  }
  else  // hyperbola 
  {
    if( f*a<0. && f*c>0. )
    {
      // C(t) = ( ae*sec(s),be*tan(s) )
      real ae=sqrt(-f/a), be=sqrt(f/c);

      //  tan/sec= sin   sec=1/cos
      real s1=asin((y1/be)/(x1/ae));
      real s2=asin((y2/be)/(x2/ae));
      real sm=.5*(s1+s2);
      
      real cos1=cos(s1), sin1=sin(s1);
      real cos2=cos(s2), sin2=sin(s2);
      
      if( s1<s2 )
      {
	// C(t) = ( ae*sec(s),be*tan(s) )    s1<= s <= s2
        x1t=ae*sin1/(cos1*cos1);
        y1t=     be/(cos1*cos1);

        x2t=ae*sin2/(cos2*cos2);
        y2t=     be/(cos2*cos2);
	
      }
      else
      { 
	// C(t) = ( ae*sec(-s),be*tan(-s) )    -s1 <= s <= -s2
        x1t=-ae*sin1/(cos1*cos1);
        y1t=     -be/(cos1*cos1);

        x2t=-ae*sin2/(cos2*cos2);
        y2t=     -be/(cos2*cos2);
      }

      xp=ae/cos1;
      yp=be*sin1/cos1;
      
    }
    else
    {
      // C(t) = ( ae*tan(s),be*sec(s) )

      real ae=sqrt(f/a), be=sqrt(-f/c);

      //  tan/sec= sin   sec=1/cos
      real s1=asin((x1/ae)/(y1/be));
      real s2=asin((x2/ae)/(y2/be));
      real sm=.5*(s1+s2);
      
      real cos1=cos(s1), sin1=sin(s1);
      real cos2=cos(s2), sin2=sin(s2);
      
      if( s1<s2 )
      {
	// C(t) = ( ae*tan(s),be*sec(s) )    s1<= s <= s2
        y1t=be*sin1/(cos1*cos1);
        x1t=     ae/(cos1*cos1);

        y2t=be*sin2/(cos2*cos2);
        x2t=     ae/(cos2*cos2);
	
      }
      else
      { 
	// C(t) = ( ae*tan(-s),be*sec(-s) )    -s1 <= s <= -s2
        y1t=-be*sin1/(cos1*cos1);
        x1t=     -ae/(cos1*cos1);

        y2t=-be*sin2/(cos2*cos2);
        x2t=     -ae/(cos2*cos2);
      }

      yp=be/cos(sm);
      xp=ae*sin(sm)/cos(sm);

    }

  }

  t0(0,0)=x1t; // tangent to pt0
  t0(0,1)=y1t;  
  t0(0,2)=0.;
  t0/=max(REAL_MIN*100,sqrt(sum(t0*t0)));
    
  t2(0,0)=x2t; // tangent to pt1
  t2(0,1)=y2t;  
  t2(0,2)=0.;   
  t2/=max(REAL_MIN*100,sqrt(sum(t2*t2)));

  p(0,0)=xp;
  p(0,1)=yp;
  p(0,2)=z; 

  return  conic(pt0, t0, pt2, t2, p);

}



int NurbsMapping::
conic( const RealArray &pt0, const RealArray &t0, const RealArray &pt2, const RealArray &t2, const RealArray &p )
// ========================================================================================================
/// \details 
///  Build a NURBS for a conic defined by end points, two tangents, and an additional point
/// \param pt0(0,0:2),pt2(0,0:2) : end points
/// \param t0(0,0:2),t2(0,0:2) : tangent directions at end points
/// \param p(0,0:2) : another point on the conic
/// 
/// \param NOTES: Construct open conic arc in 3D (c.f. makeOpenConic, NURBS book p. 317)
/// 
/// \param NOTE: ****TODO: case of full ellipse, see page 319
/// 
/// \param return values: 0=success, 1=error
/// 
//================================================================================================================
{
  int debugn=1;
  

  domainDimension=1;
  rangeDimension=3;

  if( debugn>0 )
  {
    printf("conic: pt0=(%10.3e,%10.3e,%10.3e) t0=(%10.3e,%10.3e,%10.3e) \n"
           "       pt2=(%10.3e,%10.3e,%10.3e) t2=(%10.3e,%10.3e,%10.3e) \n"
           "         p=(%10.3e,%10.3e,%10.3e) \n",pt0(0,0),pt0(0,1),pt0(0,2),t0(0,0),t0(0,1),t0(0,2),
	   pt2(0,0),pt2(0,1),pt2(0,2),t2(0,0),t2(0,1),t2(0,2), p(0,0),p(0,1),p(0,2));
  }
  

  real w1;
  RealArray pt1(1,3);
  
  makeOneArc(pt0,t0,pt2,t2,p,pt1,w1);
  // ::display(pt1,"pt1 after makeOneArc");

  int nsegs;
  if( w1<= -1. ) // parabola or hyperbola
  {
    printf("NurbsMapping::conic:ERROR: computing weight w1=%e -- could be half an ellipse \n");
    return 1;  // error: outside the convex hull  *** fix this ***
  }
  if( w1>= 1. ) // classify type and number of segments
  {
    nsegs=1;  // hyperbola or parabola
  }
  else
  { // ellipse: determine the number of segemnts
    real dangle=angle(pt0,pt1,pt2)*180./Pi;
    // printf(" *** w1=%e, w1-1.=%e, angle=%e degrees\n",w1,w1-1.,dangle);
    
    if( w1>0. && dangle > 60. )
      nsegs=1;
    else if( w1<0. && dangle > 90. )
      nsegs=4;
    else
      nsegs=2;
  }

  // nsegs=1;

  // printf(" *** w1=%e, nsegs=%i \n",w1,nsegs);
  

  n1=2*nsegs;
  int j = 2*nsegs+1;
  m1 =j+2;  // m1+1 = number of knots
  p1=m1-n1-1;
  
  uKnot.redim(m1+1);
  int i;
  for( i=0; i<3; i++)
  {
    uKnot(i)=0;          // end knots
    uKnot(i+j)=1.;
  }

  Range Rx=rangeDimension;
  Range Rw=rangeDimension+1;
  cPoint.redim(n1+1,Rw);
  cPoint=0.;
  Range N=n1+1;
  cPoint(N,rangeDimension)=1.;

  cPoint(0 ,Rx) = pt0(0,Rx);   // end control points
  cPoint(n1,Rx) = pt2(0,Rx);

  if( nsegs==1 )
  {
    cPoint(1,Rx)=w1*pt1; cPoint(1,rangeDimension)=w1;

    initialize();
    return 0;
  }

  RealArray q1(1,3),s(1,3),r1(1,3);
  real wqr;
  
  splitArc(pt0,pt1,w1,pt2,q1,s,r1,wqr);

  if( nsegs==2 )
  {
    cPoint(2,Rx)=s;
    cPoint(1,Rx)=wqr*q1; cPoint(1,rangeDimension)=wqr;
    cPoint(3,Rx)=wqr*r1; cPoint(3,rangeDimension)=wqr;
    uKnot(3)=.5;
    uKnot(4)=.5;

    initialize();
    return 0;
  }

  cPoint(4,Rx)=s;
  w1=wqr;
  RealArray hq1(1,3),hs(1,3),hr1(1,3);

  splitArc(pt0,q1,w1,s,hq1,hs,hr1,wqr);
  cPoint(2,Rx)=hs;
  cPoint(1,Rx)=wqr*hq1; cPoint(1,rangeDimension)=wqr;
  cPoint(3,Rx)=wqr*hr1; cPoint(3,rangeDimension)=wqr;

  splitArc(s,r1,w1,pt2,hq1,hs,hr1,wqr);
  cPoint(6,Rx)=hs;
  cPoint(5,Rx)=wqr*hq1; cPoint(5,rangeDimension)=wqr;
  cPoint(7,Rx)=wqr*hr1; cPoint(7,rangeDimension)=wqr;
  for( i=0; i<2; i++ )
  {
    uKnot(i+3)=.25;
    uKnot(i+5)=.5;
    uKnot(i+7)=.75;
  }
  
  initialize();
  return 0;
}


int NurbsMapping::
generalCylinder( const Mapping & curve, real d[3] )
//===========================================================================
/// \details 
///     Build a general cylinder (tabulated cylnder) by extruding a curve along a direction vector d.
///     The general cylinder is defined as
///  \begin{verbatim}
///        C(u,v) = curve(u) + v*d
///  \end{verbatim}
/// 
/// \param curve (input) : curve to extrude. If this curve is not a NurbsMapping then it is converted into
///    a NurbsMapping by interpolating the grid points from curve. Increase the number of grid points
///       on curve if you want a more accurate result.
/// \param d[3] input : extrude the curve along this direction vector 
//===========================================================================
{
  if( curve.getDomainDimension()!=1 || curve.getRangeDimension()!=3 )
  {
    printf("NurbsMapping::generalCylinder:ERROR: curve to be extruded is not a 3d curve\n",
           "    curve.getDomainDimension()=%i curve.getRangeDimension()=%i \n",curve.getDomainDimension(),
           curve.getRangeDimension());
    return 1;
  }
  

  NurbsMapping *pncurve=NULL;
  bool ncurveWasMade=false;
  if( curve.getClassName()!="NurbsMapping" )
  {
    ncurveWasMade=true;
    pncurve = new NurbsMapping; pncurve->incrementReferenceCount();

    realArray x; x = ((NurbsMapping&)curve).getGrid();
    x.reshape(x.getLength(0)*x.getLength(1)*x.getLength(2),x.getLength(3));
    pncurve->interpolate(x);
    pncurve->setIsPeriodic(axis1,curve.getIsPeriodic(axis1));
    
    printf("NurbsMapping::generalCylinder:INFO: turning the curve into a NurbsMapping by interpolation\n"
           "   Increase the number of grid point on the curve if you want a better interpolant\n");
  }
  const NurbsMapping & ncurve= pncurve!=NULL ? *pncurve : (const NurbsMapping&)curve;

  domainDimension=2;
  rangeDimension=3;
  

  p1=ncurve.getOrder(0);
  m1 = ncurve.getNumberOfKnots(0)-1; 
  n1 = ncurve.getNumberOfControlPoints(0)-1; 

  uKnot.redim(m1+1);
  Range M1(0,m1);
  uKnot=ncurve.getKnots(0)(M1);
   
  n2=1;
  p2=1;
  m2=3;
  vKnot.redim(m2+1);
  vKnot(0)=0.;
  vKnot(1)=0.;
  vKnot(2)=1.;
  vKnot(3)=1.;

  Range Rw=rangeDimension+1;
  Range R1=n1+1;
  cPoint.redim(n1+1,n2+1,rangeDimension+1);
  
  const RealArray & cp = ncurve.getControlPoints();

  // cp.display(" *** NurbsMapping:generalCylinder: control points for curve**");
  
  for( int axis=0; axis<rangeDimension+1; axis++ )
    cPoint(R1,0,axis)=cp(R1,axis);

  cPoint(R1,1,Rw)=cPoint(R1,0,Rw);

  const int nw=rangeDimension;  // index for weights
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    // *wdh* need to un-scale by weights before adding the shift 040615
    cPoint(R1,1,axis)=(cPoint(R1,1,axis)/cPoint(R1,1,nw)+d[axis])*cPoint(R1,1,nw);
  }
  
  
  initialize();
  
  setIsPeriodic(axis1,curve.getIsPeriodic(axis1) ); 
  
  if( ncurveWasMade && pncurve->decrementReferenceCount()==0 )
  {
    delete pncurve;
  }
  return 0;
}

int NurbsMapping::
generalCylinder( const Mapping & curve1, const Mapping & curve2 )
//===========================================================================
/// \details 
///     Build a general cylinder (tabulated cylnder) by interpolating between two curves
///     The general cylinder is defined as
///  \begin{verbatim}
///        C(u,v) = (1-v)*curve1(u) + v*curve2
///  \end{verbatim}
/// 
/// \param curve1, curve2 (input) : curves to interpolate. If either curve is not a NurbsMapping then it is converted into
///    a NurbsMapping by interpolating the grid points from curve. Increase the number of grid points
///       on curve if you want a more accurate result.
/// 
//===========================================================================
{
  if( curve1.getDomainDimension()!=1 || curve1.getRangeDimension()!=3 )
  {
    printf("NurbsMapping::generalCylinder:ERROR: curve1 to be extruded is not a 3d curve\n",
           "    curve1.getDomainDimension()=%i curve1.getRangeDimension()=%i \n",curve1.getDomainDimension(),
           curve1.getRangeDimension());
    return 1;
  }
  if( curve2.getDomainDimension()!=1 || curve2.getRangeDimension()!=3 )
  {
    printf("NurbsMapping::generalCylinder:ERROR: curve2 to be extruded is not a 3d curve\n",
           "    curve2.getDomainDimension()=%i curve2.getRangeDimension()=%i \n",curve2.getDomainDimension(),
           curve2.getRangeDimension());
    return 1;
  }
  

  NurbsMapping *pncurve1=NULL;
  bool ncurve1WasMade=false;
  if( curve1.getClassName()!="NurbsMapping" )
  {
    ncurve1WasMade=true;
    pncurve1 = new NurbsMapping; pncurve1->incrementReferenceCount();

    realArray x; x = ((NurbsMapping&)curve1).getGrid();
    x.reshape(x.getLength(0)*x.getLength(1)*x.getLength(2),x.getLength(3));
    pncurve1->interpolate(x);
    pncurve1->setIsPeriodic(axis1,curve1.getIsPeriodic(axis1));
    
    printf("NurbsMapping::generalCylinder:INFO: turning the curve1 into a NurbsMapping by interpolation\n"
           "   Increase the number of grid point on the curve1 if you want a better interpolant\n");
  }
  const NurbsMapping & ncurve1= pncurve1!=NULL ? *pncurve1 : (const NurbsMapping&)curve1;

  NurbsMapping *pncurve2=NULL;
  bool ncurve2WasMade=false;
  if( curve2.getClassName()!="NurbsMapping" )
  {
    ncurve2WasMade=true;
    pncurve2 = new NurbsMapping; pncurve2->incrementReferenceCount();

    realArray x; x = ((NurbsMapping&)curve2).getGrid();
    x.reshape(x.getLength(0)*x.getLength(1)*x.getLength(2),x.getLength(3));
    pncurve2->interpolate(x);
    pncurve2->setIsPeriodic(axis1,curve2.getIsPeriodic(axis1));
    
    printf("NurbsMapping::generalCylinder:INFO: turning the curve2 into a NurbsMapping by interpolation\n"
           "   Increase the number of grid point on the curve2 if you want a better interpolant\n");
  }
  const NurbsMapping & ncurve2= pncurve2!=NULL ? *pncurve2 : (const NurbsMapping&)curve2;

  domainDimension=2;
  rangeDimension=3;
  
  if( ncurve1.getNumberOfControlPoints(0)!=ncurve2.getNumberOfControlPoints(0) ||
      ncurve1.getNumberOfKnots(0)!=ncurve2.getNumberOfKnots(0) )
  {
    printf(" NurbsMapping::generalCylinder:ERROR: curve1 and curve2 do not have the same number of control points,\n"
           " or do not have the same number of knots.\n"
           " This option is only implemented if the curves agree in this way\n");
    return 1;
  }

  p1=ncurve1.getOrder(0);
  m1 = ncurve1.getNumberOfKnots(0)-1; 
  n1 = ncurve1.getNumberOfControlPoints(0)-1; 

  uKnot.redim(m1+1);
  Range M1(0,m1);
  uKnot=ncurve1.getKnots(0)(M1);
   
  n2=1;
  p2=1;
  m2=3;
  vKnot.redim(m2+1);
  vKnot(0)=0.;
  vKnot(1)=0.;
  vKnot(2)=1.;
  vKnot(3)=1.;

  Range Rw=rangeDimension+1;
  Range R1=n1+1;
  cPoint.redim(n1+1,n2+1,rangeDimension+1);
  
  const RealArray & cp1 = ncurve1.getControlPoints();
  const RealArray & cp2 = ncurve2.getControlPoints();

  // cp.display(" *** NurbsMapping:generalCylinder: control points for curve**");
  
  for( int axis=0; axis<rangeDimension+1; axis++ )
  {
    cPoint(R1,0,axis)=cp1(R1,axis);
    cPoint(R1,1,axis)=cp2(R1,axis);
  }
  
  initialize();

  if( curve1.getIsPeriodic(axis1)==functionPeriodic && curve2.getIsPeriodic(axis1)==functionPeriodic )
    setIsPeriodic(axis1,curve1.getIsPeriodic(axis1) ); 

  if( ncurve1WasMade && pncurve1->decrementReferenceCount()==0 )
  {
    delete pncurve1;
  }
  if( ncurve2WasMade && pncurve2->decrementReferenceCount()==0 )
  {
    delete pncurve2;
  }
  return 0;
}

int NurbsMapping::
plane( real pt1[3], real pt2[3], real pt3[3] )
//===========================================================================
/// \details 
///     Build a plane that passes through three points
///               
///  \begin{verbatim}
///            pt3
///             * 
///             |
///             |
///             *------* pt2
///            pt1
///  \end{verbatim}
///  
///     The plane is defined as 
///  \begin{verbatim}
///        C(u,v) = pt1 +u*(pt2-pt1) + v*(pt3-pt1)
///  \end{verbatim}
/// 
/// \param pt1[3], pt2[3], pt3[3] (input) : 3 points on the plane
/// 
//===========================================================================
{

  domainDimension=2;
  rangeDimension=3;
  
  n1 = 1;
  p1=1;
  m1 = 3;
  uKnot.redim(m1+1);
  uKnot(0)=0.;
  uKnot(1)=0.;
  uKnot(2)=1.;
  uKnot(3)=1.;
   
  n2=1;
  p2=1;
  m2=3;
  vKnot.redim(m2+1);
  vKnot(0)=0.;
  vKnot(1)=0.;
  vKnot(2)=1.;
  vKnot(3)=1.;

  Range Rw=rangeDimension+1;
  Range R1=n1+1;
  cPoint.redim(n1+1,n2+1,rangeDimension+1);
  
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    cPoint(0,0,axis)=pt1[axis];  // u=0, v=0 
    cPoint(1,0,axis)=pt2[axis];  // u=1, v=0 
    cPoint(0,1,axis)=pt3[axis];
    cPoint(1,1,axis)=pt2[axis]+pt3[axis]-pt1[axis];  // u=1,v=1
  }
  Range all;
  const int nw=rangeDimension;
  cPoint(all,all,nw)=1.;  // set the weight
  
  initialize();
  
  return 0;
}



const RealArray & NurbsMapping::
getKnots( int direction /* =0 */ ) const
//===========================================================================
/// \brief  get uKnot or vKnot, the knots in the first or second direction.
/// \param direction: 0=return uKnot, 1= return vKnot.
//===========================================================================
{
   if( direction==0 )
     return uKnot;
   else
     return vKnot;
}

const RealArray & NurbsMapping::
getControlPoints() const
//===========================================================================
/// \brief  
///     Return the control points, scaled by the weight.
//===========================================================================
{
  return cPoint;
}



int NurbsMapping::
insertKnot(const real & uBar, 
           const int & numberOfTimesToInsert_ /* =1 */ )
//===========================================================================
/// \brief  Insert a knot
/// \param uBar (input): Insert this knot value.
/// \param numberOfTimesToInsert_ (input): insert the knot this many times. The multiplicity
///     of the knot will not be allowed to exceed p1.
//===========================================================================
{
  if( domainDimension!=1 )
  {
    cout << " NurbsMapping::insertKnot:ERROR only implemented for domainDimension==1 \n";
    return 1;
  }
    
  int numberOfTimesToInsert = numberOfTimesToInsert_;
  int index= findSpan( n1,p1,uBar,uKnot );   // uKnot(k) <= uBar < uKnot(k+1)

#if 1

  // rewrote following the Nurbs Book, kkc
  // "The Nurbs Book", Algorithm A5.1

  int k = index;
  int s = 0; // multiplity
  int i = index;
  while ( i>=0 && uKnot(i)==uBar )
  {
    s++;
    i--;
  }

  if( s+numberOfTimesToInsert>p1 )
  { // we only insert at most p1 times.
    numberOfTimesToInsert=p1-s;
    // printf("NurbsMapping::insertKnot:WARNING trying to insert a knot too many times. Will insert only %i times \n",
    //    numberOfTimesToInsert);
    // ::display(uKnot,"Here are the knots");
  }

  // create the new knot vector
  if ( numberOfTimesToInsert>0 )
    {
      RealArray newKnots(m1 + numberOfTimesToInsert + 1);
      for ( i=0; i<=k; i++ ) newKnots(i) = uKnot(i);
      for ( i=1; i<=numberOfTimesToInsert; i++ ) newKnots(k+i) = uBar;
      for ( i=k+1; i<m1+1; i++ ) newKnots(i+numberOfTimesToInsert) = uKnot(i);
      
      // save unaltered control points
      Range AXES(rangeDimension+1);
      RealArray newCP( n1+1+numberOfTimesToInsert, AXES.getLength() ); // the new control points
      RealArray adjCP(p1+1, AXES.getLength()); // points used to compute new control points
      
      for ( i=0; i<=k-p1; i++ ) newCP(i,AXES) = cPoint(i,AXES);
      for ( i=k-s; i<n1+1; i++ ) newCP(i+numberOfTimesToInsert,AXES) = cPoint(i,AXES);
      // save control points used for creation of new points
      for ( i=0; i<=p1-s; i++ ) adjCP(i,AXES) = cPoint(k-p1+i,AXES);
      
      // create new control points
      int L;
      int j;
      for ( j=1; j<=numberOfTimesToInsert; j++ )
	{
	  L = k-p1+j;
	  for ( i=0; i<=p1-j-s; i++ )
	    {
	      real alpha = ( uBar-uKnot(L+i) )/( uKnot(i+k+1)-uKnot(L+i) );
	      adjCP(i,AXES) = alpha*adjCP(i+1,AXES) + (1.0-alpha)*adjCP(i,AXES);
	    }
	  newCP(L,AXES) = adjCP(0,AXES);
	  newCP(k+numberOfTimesToInsert-j-s,AXES) = adjCP(p1-j-s,AXES);
	}
      
      // copy over the remaining points
      for ( i=L+1; i<k-s; i++ )
	newCP(i,AXES) = adjCP(i-L,AXES);
      
      uKnot.redim(newKnots);  uKnot=newKnots;
      cPoint.redim(newCP);   cPoint=newCP;
      m1=m1+numberOfTimesToInsert;
      n1=n1+numberOfTimesToInsert;
    }

#else

  int multiplicity=1;
  int i=index-1;
  while( i>=0 && uKnot(i)==uKnot(index) )
  {
    multiplicity++;
    i--;
  }
  if( multiplicity+numberOfTimesToInsert > p1 )
  {
    numberOfTimesToInsert=p1-multiplicity;
    printf("NurbsMapping::insertKnot:WARNING trying to insert a knot too many times. Will insert only %i times \n",
     numberOfTimesToInsert);
    // ::display(uKnot,"Here are the knots");
    
  }

  int mNew=m1+numberOfTimesToInsert;
  int nNew=n1+numberOfTimesToInsert;

  // new knot array
  RealArray knot(mNew+1);
  knot(Range(0,index))=uKnot(Range(0,index));
  knot(Range(index+1,index+numberOfTimesToInsert))=uBar;
  knot(Range(index+1,m1)+numberOfTimesToInsert)=uKnot(Range(index+1,m1));
  
  // new control points
  RealArray cp(nNew+1,rangeDimension+1);
  Range Rw(0,rangeDimension);
  cp(Range(0,index-p1),Rw)=cPoint(Range(0,index-p1),Rw);
  cp(Range(index-multiplicity,n1)+numberOfTimesToInsert,Rw)=cPoint(Range(index-multiplicity,n1),Rw);
  
  RealArray cRw(Range(0,p1-multiplicity),Rw);
  cRw=cPoint(Range(index-p1,p1-multiplicity+1),Rw);

  int l;
  for( int j=1; j<=numberOfTimesToInsert; j++ )
  {
    l=index-p1+j;
    for( int i=0; i<=p1-j-multiplicity; i++ )
    {
      real alpha=(uBar-uKnot(l+i))/(uKnot(i+index+1)-uKnot(l+i));
      cRw(i,Rw)=alpha*cRw(i+1,Rw)+(1.-alpha)*cRw(i,Rw);
    }
    cp(l,Rw)=cRw(0,Rw);
    cp(index+numberOfTimesToInsert-j-multiplicity,Rw)=cRw(p1-j-multiplicity,Rw);
  }
  cp(Range(l+1,index-multiplicity))=cRw(Range(l+1,index-multiplicity)-l);
  
  uKnot.redim(knot);  uKnot=knot;
  cPoint.redim(cp);   cPoint=cp;
  m1=mNew;
  n1=nNew;
#endif

  if ( Mapping::debug & 2 )
    {
      uKnot.display("insertKnot: Here is the new uKnot");
      cPoint.display("insertKnot: Here is the new cPoint");
    }

    if ( numberOfTimesToInsert>0 )
    {
      
      initialize();
      reinitialize();
    }
    
  
  return 0;
}

int NurbsMapping::
normalizeKnots() 
//===========================================================================
/// \param Access: Protected routine.
/// \brief  
///     Normalize the knots, uKnot (and vKnot if domainDimension==2) to
///  lie from 0 to 1. This routine will NOT change the values of uMin,uMax, vMin,vMax since
///  these values indicate the original bounds on uKnot and vKnot.
//===========================================================================
{
  real ua=uKnot(0); // min(uKnot);
  real ub=uKnot(m1); // max(uKnot);
  real d = ub-ua;
  if( d==0. )
  {
    printf("NurbsMapping::normalizeKnots:FATAL error: The knots have all the same value. min(u)=%e, max(u)=%e\n",
	   ua,ub);
  }
  uKnot=(uKnot-ua)*(1./d);
  if( domainDimension>1 )
  {
    real va=vKnot(0);  // min(vKnot);
    real vb=vKnot(m2); // max(vKnot);
    d = vb-va;
    if( d==0. )
    {
      printf("NurbsMapping::normalizeKnots:FATAL error: The knots have all the same value. min(v)=%e, max(v)=%e\n",
	     va,vb);
    }
    vKnot=(vKnot-va)*(1./d);
  }
  return 0;
}

int NurbsMapping::
readFromIgesFile( IgesReader & iges, const int & item, bool normKnots /*=true*/  )
//===========================================================================
/// \brief  Read a NURBS from an IGES file. An IGES file is a data file
///  containing geometrical objects, usually generated by a CAD program.
/// \param iges (input) : Use this object to read the IGES file.
/// \param item (input) : read this item from the IGES file.
//===========================================================================
{
  uMin=0.; uMax=1.;
  vMin=0.; vMax=1.;
  
  int closedInU=false,closedInV=false,periodicInU=false,periodicInV=false;

  bool reallyNormalizeKnots=normKnots;  // we may decide not too for periodic nurbs
  const real knotEpsilon = REAL_EPSILON*10.;
  
  if( iges.entity(item)==IgesReader::rationalBSplineCurve ) // 126
  {
    // create a curve
    //  data(0) : entity number 
    //  data(1) : upper index of sum 
    //  data(2) : degree of basis function 
    //  data(3) : 0 = nonplanar, 1 = planar 
    //  data(4) : 0 = open curve, 1 = closed curve 
    //  data(5) : 0 = rational, 1 = polynomal 
    //  data(6) : 0 = nonperiodic, 1 = periodic 
    RealArray data(7); // kkc changed size from 6 to 7 reflecting information above
    iges.readData(item,data,7);
    setDomainDimension(1);
    setRangeDimension(3);

    n1 = (int) data(1) ;   // n1= # of control points -1 in u direction
    p1 = (int) data(2) ;
    m1 = n1+p1+1         ;  //  number of knots = m1+1
    int c  = n1+p1+1;

    closedInU   = (int)data(4);
    periodicInU = (int)data(6);
    if( periodicInU )
      setIsPeriodic(axis1,functionPeriodic);

    int maxData = 19+c+4*(n1+1); // AP: why not just read 16+c+4*n1 ?
    data.redim(maxData);
   
    int resid = iges.readData(item,data,maxData); // resid should be zero -- we read all the data

    uKnot.redim(m1+1);
    cPoint.redim(n1+1,rangeDimension+1);  // holds weights too
   
    int i,ii;
    Range R1(0,m1);
    uKnot(R1)=data(R1+7);
    if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: Here is uKnot");
   
    R1=Range(0,n1);
/* ---
    for ( ii=0, i=8+m1; ii<=n1; ii++, i+=4 )
    {
      cPoint(ii,0) = data(i);
      cPoint(ii,1) = data(i+1);
      cPoint(ii,2) = data(i+2);
      cPoint(ii,3) = data(i+3);
    }
--- */

    int j = 8+c ;
    for (i=j,ii=0;i<9+c+n1;i++,ii++)
    {
      cPoint(ii,0)= data(i+n1+1+2*(i-j));
      cPoint(ii,1)= data(i+n1+2+2*(i-j));
      cPoint(ii,2)= data(i+n1+3+2*(i-j));
      cPoint(ii,3)= data(i);
    }

    if( Mapping::debug & 4 )
       cPoint.display("NurbsMapping::readFromIgesFile Here is cPoint: NurbsMapping");

    for( int axis=0; axis<rangeDimension; axis++ )
      cPoint(R1,axis)*=cPoint(R1,rangeDimension); // multiply by the weight


    if (resid <= 1)
    {
      uMin = data(12+c+4*n1); // data[12+a+4*k];
      uMax = data(13+c+4*n1);
      if( uMin >= uMax )
      {
        printf("NurbsMapping::readFromIgesFile:WARNING: uMin=%e >= uMax=%e ! I am setting uMin=0, uMax=1.\n",
                uMin,uMax);
	uMin = 0.;
	uMax=1.;
      }

      // ************************************************ *wdh* 050610 
      bool truncateBounds=true;  // make this an option 
      if( truncateBounds &&
          (uMin!=min(uKnot) || uMax!=max(uKnot)) )
      {
        if( debug & 2 )
	  printf(" ***NurbsMapping::readFromIgesFile:INFO: truncateToDomainBounds: [uMin,uMax]=[%9.3e,%9.3e]\n",
		 uMin,uMax);
        rStart[0]=uMin;
	rEnd[0]=uMax;
	truncateToDomainBounds();
	rStart[0]=0.;
	rEnd[0]=1.;
        // reparameterize(uMin,uMax);
        
      }
      // *************************************************************

    }

    int matrixTransform=iges.matrix(item);
    if( matrixTransform!=0 )
      printf("***NurbsMapping::readFromIgesFile:WARNING: matrixTransform!=0 for a rationalBSplineCurve\n");
  
    if( Mapping::debug & 4 )
      printf("NurbsMapping::readFromIgesFile: n1=%i, p1=%i, uMin=%e, uMax=%e, periodic= %i, mt=%i  \n",
                n1,p1,uMin,uMax,periodicInU,matrixTransform);

    setGridDimensions( axis1,max(11,n1*2) );

    if( getIsPeriodic(axis1)==functionPeriodic )
    {
      if( uKnot(0)<-knotEpsilon || uKnot(m1)>1.+knotEpsilon )
      {
        if( Mapping::debug & 4 )
          printf("NurbsMapping::readFromIgesFile:INFO: periodic NURBS with uMin=%e and uMax=%e \n",uKnot(0),uKnot(m1));
	reallyNormalizeKnots=false;
      }
      else
      { // make sure this is true, see volvo surface 209 for a false case
	Range AXES(rangeDimension);
// AP: This test assumes that the NURBS is clamped!
	if ( sqrt(sum(pow(cPoint(0,AXES)-cPoint(n1,AXES),2)))>FLT_EPSILON ) 
	{
	  // hey! its not really periodic!
	  if (Mapping::debug & 2 )
	    cout<<"NurbsMapping:::readFromIgesFile:curve was not really periodic"<<endl;

	  setIsPeriodic(axis1, notPeriodic);
	}
      }
    }
  }
  else if( iges.entity(item)==IgesReader::parametricSplineCurve ) // 112
  {
    // create a curve
    //  data(0) : entity number 
    //  data(1) : type of spline
    //  data(2) : degree of continuity
    //  data(3) : number of dimensions
    //  data(4) : number of segments

    RealArray data(5); // need a new array
    iges.readData(item, data, 5);

//    printf("readFromIgesFile: parametric spline curve data: %f, %f, %f, %f\n", data(1), data(2), data(3), data(4));

    setDomainDimension(1);
    setRangeDimension(3);

    int nSegment = (int) data(4);
//    printf("Number of segments: %i\n", nSegment);
    int maxData = 17+13*nSegment + 1;
    data.redim(maxData);
   
    int resid = iges.readData(item,data,maxData); // resid should be zero -- we read all the data

// break points of piecewise polynomial
// number of knots = m1+1,

    p1 = 3; // This is a cubic B-spline
    m1 = (nSegment+1)*p1 + 1; // all internal knots have multiplicity p. The first and last have p+1.
    n1 = nSegment*p1; // Each segment gives 3 unique control points, and the last segment gives 4.
    
    uKnot.redim(m1+1);
    cPoint.redim(n1+1,rangeDimension+1);  // holds weights too

    ArraySimple<real> T(nSegment+1); // breakpoints for power representation
    int i;
    for (i=0; i<= nSegment; i++)
    {
      T(i) = data(i+5);
//      printf("T(%i) = %e\n", i, T(i));
    }

    ArraySimple<real> Ax(nSegment), Ay(nSegment), Az(nSegment), 
      Bx(nSegment), By(nSegment), Bz(nSegment), 
      Cx(nSegment), Cy(nSegment), Cz(nSegment), 
      Dx(nSegment), Dy(nSegment), Dz(nSegment);
    for (i=0; i< nSegment; i++) // one set of coefficient for each segment
    {
// Ax(0) = data(6+nSegment)
// Bx(0) = data(7+nSegment)
// Cx(0) = data(8+nSegment)
// Dx(0) = data(9+nSegment)

// Ay(0) = data(10+nSegment)
// By(0) = data(11+nSegment)
// Cy(0) = data(12+nSegment)
// Dy(0) = data(13+nSegment)

// Az(0) = data(14+nSegment)
// Bz(0) = data(15+nSegment)
// Cz(0) = data(16+nSegment)
// Dz(0) = data(17+nSegment)

// Ax(1) = data(18+nSegment)

// general formula:
// Ax(i) = data(6+nSegment+i*12)
// Bx(i) = data(7+nSegment+i*12)
// etc
      Ax(i) = data(6+nSegment+i*12);
      Bx(i) = data(7+nSegment+i*12);
      Cx(i) = data(8+nSegment+i*12);
      Dx(i) = data(9+nSegment+i*12);

      Ay(i) = data(10+nSegment+i*12);
      By(i) = data(11+nSegment+i*12);
      Cy(i) = data(12+nSegment+i*12);
      Dy(i) = data(13+nSegment+i*12);
      
      Az(i) = data(14+nSegment+i*12);
      Bz(i) = data(15+nSegment+i*12);
      Cz(i) = data(16+nSegment+i*12);
      Dz(i) = data(17+nSegment+i*12);

//      printf("Break point %i: (%e, %e, %e)\n", i, Ax(i), Ay(i), Az(i));
    }
   
// local Bezier control points for each segment
//    printf("Computing local Bezier control points...\n");
    
    ArraySimple<real> cPointLocal(nSegment,4,3);
    real dt, dt2, dt3;
    for (i=0; i<nSegment; i++)
    {
// step size
      dt = T(i+1)-T(i);
      dt2 = dt*dt;
      dt3 = dt*dt2;
      
// cPoint 0
      cPointLocal(i,0,0) = Ax(i);
      cPointLocal(i,0,1) = Ay(i);
      cPointLocal(i,0,2) = Az(i);

// cPoint 1
      cPointLocal(i,1,0) = Ax(i) + dt*Bx(i)/3.;
      cPointLocal(i,1,1) = Ay(i) + dt*By(i)/3.;
      cPointLocal(i,1,2) = Az(i) + dt*Bz(i)/3.;

// cPoint 2
      cPointLocal(i,2,0) = Ax(i) + dt*Bx(i)*2./3. + dt2*Cx(i)/3.;
      cPointLocal(i,2,1) = Ay(i) + dt*By(i)*2./3. + dt2*Cy(i)/3.;
      cPointLocal(i,2,2) = Az(i) + dt*Bz(i)*2./3. + dt2*Cz(i)/3.;

// cPoint 3
      cPointLocal(i,3,0) = Ax(i) + dt*Bx(i) + dt2*Cx(i) + dt3*Dx(i);
      cPointLocal(i,3,1) = Ay(i) + dt*By(i) + dt2*Cy(i) + dt3*Dy(i);
      cPointLocal(i,3,2) = Az(i) + dt*Bz(i) + dt2*Cz(i) + dt3*Dz(i);

//        printf("Segment %i: local control points:\n(%e, %e, %e)\n(%e, %e, %e)\n(%e, %e, %e)\n(%e, %e, %e)\n",
//  	     i, 
//  	     cPointLocal(i,0,0), cPointLocal(i,0,1), cPointLocal(i,0,2),
//  	     cPointLocal(i,1,0), cPointLocal(i,1,1), cPointLocal(i,1,2),
//  	     cPointLocal(i,2,0), cPointLocal(i,2,1), cPointLocal(i,2,2),
//  	     cPointLocal(i,3,0), cPointLocal(i,3,1), cPointLocal(i,3,2));
    }

// turn the local Bezier functions into a global B-spline (with many internal knots)    
//    printf("Assembling the local Bezier curves into a global B-spline...\n");

// global knot vector
    uKnot(0) = T(0);
    uKnot(1) = T(0);
    uKnot(2) = T(0);
    uKnot(3) = T(0);
    for (i=1; i<=nSegment-1; i++)
    {
      uKnot(4+(i-1)*3) = T(i);
      uKnot(5+(i-1)*3) = T(i);
      uKnot(6+(i-1)*3) = T(i);
    }
    uKnot(nSegment*3+1) = T(nSegment);
    uKnot(nSegment*3+2) = T(nSegment);
    uKnot(nSegment*3+3) = T(nSegment);
    uKnot(nSegment*3+4) = T(nSegment);
    
//      for (i=0; i<=m1; i++)
//      {
//        printf("uKnot(%i) = %e\n", i, uKnot(i));
//      }

// global control points
    int j;
    for (i=0; i<nSegment; i++)
    {
      for (j=0; j<3; j++)
      {
	cPoint(i*3,j)   = cPointLocal(i,0,j);
	cPoint(i*3+1,j) = cPointLocal(i,1,j);
	cPoint(i*3+2,j) = cPointLocal(i,2,j);
      }
      cPoint(i*3,3) = 1.; // weight
      cPoint(i*3+1,3) = 1.; // weight
      cPoint(i*3+2,3) = 1.; // weight
    }
// last control point
    for (j=0; j<3; j++)
    {
      cPoint(nSegment*3,j)   = cPointLocal(nSegment-1,3,j);
    }
    cPoint(nSegment*3,3) = 1.; // weight

// print the global control points:
//      for (i=0; i<= n1; i++)
//      {
//        printf("cPoint(%i) = (%e, %e, %e, %e)\n", i, cPoint(i,0), cPoint(i,1), cPoint(i,2), cPoint(i,3));
//      }
    

    uMin = T(0);
    uMax = T(nSegment);

// remove unneccessary interiour knots
    int numberRemoved;
    real tolerance=5.e-2;
    for (i=1; i<nSegment; i++) // don't remove the first nor last knots
    {
// find first location of T(i) in uKnot(j)
      j=-1;
      for (j=m1; j>=0; j--)
      {
	if (fabs(T(i)-uKnot(j)) <= 10*FLT_EPSILON)
	  break;
      }
      if (j>=0 && j<=m1)
      {
	removeKnot(j, p1-1, numberRemoved, tolerance);
	if (numberRemoved != p1-1)
	  printf("Knot %i was removed %i times, m1=%i\n", j, numberRemoved, m1);
      }
    }
    

    setGridDimensions( axis1,max(11,n1) );

// periodicity?
      
// done?
//    printf("Conversion done\n");
    
  } // end parametricSplineCurve
  else if( iges.entity(item)==IgesReader::rationalBSplineSurface ) // 128
  {
    RealArray data(10);
    iges.readData(item,data,10);
  
    // pdata[0] : entity number 
    // pdata[1] : upper index of first sum 
    // pdata[2] : upper index of second sum 
    // pdata[3] : degree of first set of basis functions 
    // pdata[4] : degree of second set of basis functions 
    // pdata[5] : 1 = Closed in first parametric variable direction
    //              0 = Not Closed 
    // pdata[6] : 1 = Closed in second parametric variable direction
    //              0 = Not Closed 
    // pdata[7] : 0 = Rational
    //              1 = Polynomial 
    // pdata[8] : 0 = Nonperiodic in first parametric variable direction
    //              1 = Periodic in first parametric variable direction 
    // pdata[9] : 0 = Nonperiodic in second parametric variable direction
    //              1 = Periodic in second parametric variable direction 

    int entity=(int)data(0);
    if( entity!=IgesReader::rationalBSplineSurface )
    {
      cout << "error:NurbsMapping::readFromIgesFile: This item is not a nurbs surface \n";
      return 1;
    }
    setDomainDimension(2);
    setRangeDimension(3);

    n1 = (int) data(1) ;   // n1= # of control points -1 in u direction
    n2 = (int) data(2) ;
    p1 = (int) data(3) ;
    p2 = (int) data(4) ;

    closedInU   = (int)data(5);
    closedInV   = (int)data(6);
    periodicInU = (int)data(8);
    periodicInV = (int)data(9);

    if( closedInU ) //  periodicInU  *wdh* 010831 : should use closed to represent "periodic"
      setIsPeriodic(axis1,functionPeriodic);
    if( closedInV ) // periodicInV )
      setIsPeriodic(axis2,functionPeriodic);

    if( debug & 2 )
    {
      if( closedInU || closedInV || periodicInU || periodicInV )
      {
	printf("NurbsMapping:: closedInU=%i, closedInV=%i, periodicInU=%i, periodicInV=%i form=%i\n",
	       closedInU,closedInV,periodicInU,periodicInV,iges.formData(item));
      }
    }
    

    m1 = n1+p1+1         ;  //  number of knots = m1+1
    m2 = n2+p2+1        ;  // 
    int c  = (1+n1)*(1+n2)  ;


  // allocate a proper memory for the NURBS surface   

    int maxData = 19+m1+m2+4*(n1+1)*(n2+1);
    data.redim(maxData);
   
    int resid = iges.readData(item,data,maxData); // resid should be zero -- we read all the data

    uKnot.redim(m1+1);
    vKnot.redim(m2+1);
    cPoint.redim(n1+1,n2+1,rangeDimension+1);  // holds weights too
   
    int i,j,ii,jj;
    Range R1(0,m1);
    uKnot(R1)=data(R1+10);
   
    Range R2(0,m2);
    vKnot(R2)=data(R2+11+m1);

    R1=Range(0,n1);
    R2=Range(0,n2);
    ii = 0;
    jj = 0;
    j = 12+m1+m2 ;
    for (i=j;i < (j+c);i++)
    {
      cPoint(ii,jj,0) = data(i+c+2*(i-j));
      cPoint(ii,jj,1) = data(i+c+2*(i-j)+1);
      cPoint(ii,jj,2) = data(i+c+2*(i-j)+2);
      cPoint(ii,jj,3) = data(i);

      if(ii == n1)
      {
	ii=0;
	jj++;
      }
      else
	ii++;
    }


    // cPoint.display("NurbsMapping:: Here is cPoint for a surface");

    const int minimumNumberOfPoints=11;
    const int maximumNumberOfPoints=1001;  // *wdh* increased from 101
    
    if( true || // *wdh* always do this check ***
        n1<minimumNumberOfPoints || n2<minimumNumberOfPoints )
    {
      // there are only a few control points, guess how many points are needed for plotting
      // by computing the arclength and curvature in each direction.
      real dist1=0., dist2=0., curvature1=0., curvature2=0.;
      Range Rx(0,rangeDimension-1);
      int ii,jj;
      for( ii=1; ii<=n1; ii++ )
        dist1+=sum(fabs(cPoint(ii,R2,Rx)-cPoint(ii-1,R2,Rx)));

      for( ii=1; ii<n1; ii++ )
        curvature1+=sum(fabs(cPoint(ii+1,R2,Rx)-2.*cPoint(ii,R2,Rx)+cPoint(ii-1,R2,Rx)));

      for( jj=1; jj<=n2; jj++ )
        dist2+=sum(fabs(cPoint(R1,jj,Rx)-cPoint(R1,jj-1,Rx)));

      for( jj=1; jj<n2; jj++ )
        curvature2+=sum(fabs(cPoint(R1,jj+1,Rx)-2.*cPoint(R1,jj,Rx)+cPoint(R1,jj-1,Rx)));

      if( Mapping::debug & 2 )
        printf("NurbsMapping: dist1=%e, curvature1=%e, dist2=%e, curvature2=%e\n",
  	     dist1,curvature1,dist2,curvature2);
      
      if( dist1==0. || dist2==0. )
      {
	if( dist1>0. )
          dist2=dist1;
	else if( dist2>0. )
          dist1=dist2;
	else
          dist1=dist2=1.;
      }
      // The ratio curvature1/dist1 should always be less than 2.
      setGridDimensions( axis1,(int)min(real(maximumNumberOfPoints),
                               (int)max(minimumNumberOfPoints,n1,int(.5*minimumNumberOfPoints*dist1/dist2))+
                         20.*min(2.,curvature1/dist1)) );
      setGridDimensions( axis2,(int)min(real(maximumNumberOfPoints),
                         (int)max(minimumNumberOfPoints,n2,int(.5*minimumNumberOfPoints*dist2/dist1))+
                         20.*min(2.,curvature2/dist2)) );
    }
    else
    {
      // setGridDimensions( axis1,max(15,n1) );
      // setGridDimensions( axis2,max(15,n2) );
      setGridDimensions( axis1,n1 );
      setGridDimensions( axis2,n2 );
    }
    
    for( int axis=0; axis<rangeDimension; axis++ )
      cPoint(R1,R2,axis)*=cPoint(R1,R2,rangeDimension); // multiply by the weight *******


    if (resid <= 1)
    {
      uMin = data(12+m1+m2+4*c);
      uMax = data(13+m1+m2+4*c);
      if( uMin >= uMax )
      {
	uMin = 0.;
	uMax=1.;
      }
      vMin = data(14+m1+m2+4*c);
      vMax = data(15+m1+m2+4*c);
      if( vMin>vMax )
      {
	vMin=0.;
	vMax=1.;
      }
    }
    const real ua=uKnot(0);  // min(uKnot);
    const real ub=uKnot(m1); // max(uKnot);
    const real va=vKnot(0);  // min(vKnot);
    const real vb=vKnot(m2); // max(vKnot);

    if( Mapping::debug & 4 )
      printf(" ***** NurbsMapping: ua=%e, ub=%e, va=%e, vb=%e \n",ua,ub,va,vb);
    

    int matrixTransformation=iges.matrix(item);
  
    if( Mapping::debug & 4 )
      printf(" n1=%i, n2=%i, p1=%i, p2=%i, uMin=%e, uMax=%e, vMin=%e, vMax=%e, periodic= %i,%i, mt=%i  \n",n1,n2,p1,p2,
	   uMin,uMax,vMin,vMax,periodicInU,periodicInV,matrixTransformation);



  }
  else if( iges.entity(item)==IgesReader::circularArc )
  {
    
    // Note: the arc lies in a constant z-plane

    // data(0) : entity type
    // data(1) : z value
    // data(2),data(3) : x,y of centre
    // data(4),data(5) : x,y of point 1
    // data(6),data(7) : x,y of point 2
    
    RealArray data(10);
    iges.readData(item,data,8);
    RealArray o(3),pt1(3),pt2(3);
    
    o(0)=data(2);
    o(1)=data(3);
    o(2)=data(1);
    pt1(0)=data(4);
    pt1(1)=data(5);
    pt1(2)=data(1);

    pt2(0)=data(6);
    pt2(1)=data(7);
    pt2(2)=data(1);
    


    pt1-=o;
    pt2-=o;
    real radius=SQRT(sum(pt1*pt1));
    if( radius==0. )
    {
      printf("ERROR: reading a circular arc from an IGES file: radius==0\n");
      throw "error";
    }
    int matrix=iges.matrix(item);
    RealArray transform(3,3), translation(3);
    if( matrix!=0 )
    {
      if( Mapping::debug & 8 ) printf("***NurbsMapping::readFromIgesFile:INFO: matrixTransform!=0 for a circularArc\n");
      if( matrix!=0 )
      {
	matrix=iges.sequenceToItem(matrix);
	int returnValue=MappingsFromCAD::getTransformationMatrix(matrix,iges,transform,translation);
	if( returnValue==0 && Mapping::debug & 8 )
	{
	  ::display(transform,"circularArc: transform");
	  ::display(translation,"circularArc: translation");
	}
      }
    }
    if( Mapping::debug & 4 )
    {
      
      printf("NurbsMapping:: circularArc: radius=%e, centre=(%e,%e,%e)\n",radius,o(0),o(1),o(2));
      printf("                          : pt1=(%e,%e,%e) pt2=(%e,%e,%e) matrixTransformation=%i\n",
	     pt1(0)+o(0),pt1(1)+o(1),pt1(2)+o(2),
	     pt2(0)+o(0),pt2(1)+o(1),pt2(2)+o(2),matrix );
    }
    
    real cosTheta=sum(pt1*pt2)/SQR(radius);
    real sinTheta = (pt1(0)*pt2(1)-pt1(1)*pt2(0))/SQR(radius);
    real theta = atan2((double)sinTheta,(double)cosTheta); // cast to double for kcc

    if( theta<0. )
      theta+=Pi;

    if( fabs(theta)<1.e-3 )
    {
      // here is the distance between the points
      real pDist = sqrt( SQR(pt1(0)-pt2(0))+SQR(pt1(1)-pt2(1))+SQR(pt1(2)-pt2(2)) );
      if( pDist>radius )
      {
	// theta is close to zero but the two points are almost 2*radius apart -- the angle must be near Pi
	// *wdh* 050618
	theta+=Pi;
      }
      
    }
    

    if( Mapping::debug & 4 ) 
      printf("NurbsMapping:: circularArc: cos=%e, sin=%e, start=0, end:theta=%e \n",cosTheta,sinTheta,theta);
    

    pt1/=radius;
    
    pt2(0)=-pt1(1);
    pt2(1)= pt1(0);
    
     
    real startAngle=0.;
    real endAngle=theta/twoPi;  // endAngle in [0,1]
    
    
    circle( o,pt1,pt2,radius,startAngle,endAngle);
    
    if( matrix!=0 )
    { // apply the matrix and then translate
      matrixTransform( transform );
      shift(translation(0),translation(1),translation(2));
    }
    if( false )
    {
      realArray r(3,1), x(3,3);
      r(0,0)=0.;
      r(1,0)=.5;
      r(2,0)=1.;
    
      map(r,x);
    
      printf("  : x(r=0)=(%e,%e,%e) x(r=.5)=(%e,%e,%e) x(r=1)=(%e,%e,%e)\n",
	     x(0,0),x(0,1),x(0,2),x(1,0),x(1,1),x(1,2),x(2,0),x(2,1),x(2,2));
      
    }
    
  }
  else if( iges.entity(item)==IgesReader::parametricSplineSurface )
  {
    RealArray data(5);
    iges.readData(item,data,5);
    int mu=(int)data(3);   // number of u patches
    int mv=(int)data(4);   // number of v patches

    // printf("NurbsMapping:: parametricSplineSurface: mu=%i, mv=%i\n",mu,mv);

    int num=6+mu+mv+48*(mu*(mv+1)+(mv+1));
    data.redim(num);
    iges.readData(item,data,num);

    RealArray tu(mu+1), tv(mv+1);
    Range R=mu+1;
    tu(R)=data(R+5);
    R=mv+1;
    tv(R)=data(R+6+mu);
    // ::display(tu,"tu");
    // ::display(tv,"tv");

    RealArray poly(4*4*3,mu,mv);
    R=4*4*3;
    
    num=4*4*3;
    for( int n=0; n<mv; n++ )
      for( int m=0; m<mu; m++ )
	poly(R,m,n)=data(R+7+mu+mv + num*(n+(mv+1)*m));   // data is stored as (m,n) transposed 
    
    poly.reshape(4,4,3,mu,mv);
    parametricSplineSurface(mu,mv,tu,tv,poly);

    setGridDimensions( axis1,n1 );
    setGridDimensions( axis2,n2 );
  }
  else if( iges.entity(item)==IgesReader::line )
  { // entity 110

    int num=7;
    RealArray data(num);
    iges.readData(item,data,num);

    // note: form=1 : semi-infinite line
    //       form=2 : infinite line
    RealArray p1(1,3), p2(1,3);
    p1(0,0)=data(1);
    p1(0,1)=data(2);
    p1(0,2)=data(3);
    
    p2(0,0)=data(4);
    p2(0,1)=data(5);
    p2(0,2)=data(6);
    
    if( Mapping::debug & 1 )
      printf(" NurbsMapping::readFromIgesFile:create a line from p1=(%8.2e,%8.2e,%8.2e) to p2=(%8.2e,%8.2e,%8.2e)\n",
	     p1(0,0),p1(0,1),p1(0,2),p2(0,0),p2(0,1),p2(0,2));
    
    line( p1,p2 );
    
  }
  else
  {
    cout << "NurbsMapping::readFromIgesFile:ERROR: do not know entity = " << iges.entity(item) << endl;
    return 1;
  }
  

  //uKnot.display("uKnot before iges normalize");
  if( uMin!=0. || uMax!=1. || // *wdh* 990805
      vMin!=0 || vMax!=1. ) // *wdh* 991130
  {
    if ( reallyNormalizeKnots ) normalizeKnots();
  }
  
  // if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: uKnot after iges normalize");

  initialize();

  // if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: uKnot after initialize");
  // if( Mapping::debug & 4 ) cPoint.display("NurbsMapping::readFromIgesFile: cPoint after initialize");

  // *wdh* 031123
  //  Check to see if the nurb is clamped (Some nurbs are not clamped)
  //  Most other NurbsMapping functions assume the Nurbs is clamped!!
  //  Therefore we cannot simply call insertKnot to clamp this Nurbs
  if( getIsPeriodic(axis1)==functionPeriodic && 
      (uKnot(0)<-knotEpsilon || uKnot(m1)>1.+knotEpsilon) )
  {
    if( Mapping::debug & 4 )
     printf("NurbsMapping::readFromIgesFile:truncating knots a periodic nurbs to the interval [0,1]\n");
    rStart[0]=0.;
    rEnd[0]=1.;
    int k=0,num0=1,num1=1;
    for( k=1; k<=p1; k++ )
    {
      if( fabs(uKnot(k)-uKnot(0))<knotEpsilon )
      {
	num0++;
      }
      if( fabs(uKnot(m1-k)-uKnot(m1))<knotEpsilon )
      {
	num1++;
      }
    }
    if( num0!=(p1+1) || num1!=(p1+1) )
    {
      if( Mapping::debug & 4 )
        printf("NurbsMapping::readFromIgesFile:ERROR: the nurbs is not clamped at the ends! I will clamp it...\n");

      int extraLeft =p1+1-num0;
      int extraRight=p1+1-num1;
      
      int m1New=m1+extraLeft+extraRight;
      RealArray uKnotNew(m1New+1);
      Range R=m1+1;
      uKnotNew(R+extraLeft)=uKnot(R);
            
      //  m1=n1+p1+1;
      int n1New=m1New-p1-1;
      Range Rw=rangeDimension+1;

      RealArray cPointNew(n1New+1,rangeDimension+1);

      if( Mapping::debug & 4 )
        printf(" m1=%i, n1=%i, p1=%i, m1New=%i n1New=%i extraLeft=%i extraRight=%i\n",m1,n1,p1,m1New,n1New,
           extraLeft,extraRight);
      

      R=n1+1;
      cPointNew(R+extraLeft,Rw)=cPoint(R,Rw);
      int k;
      for( k=0; k<extraLeft; k++ )
      {
        uKnotNew(k)=uKnot(0);
        cPointNew(k,Rw)=cPoint(0,Rw);
      }
      for( k=0; k<extraRight; k++ )
      {
        uKnotNew(m1New-k)=uKnot(m1);
        cPointNew(n1New-k,Rw)=cPoint(n1,Rw);
      }
      uKnot.redim(0);
      cPoint.redim(0);
      m1=m1New;
      n1=n1New;
      uKnot=uKnotNew;
      cPoint=cPointNew;

      if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: uKnot after clamping");
      if( Mapping::debug & 4 ) cPoint.display("NurbsMapping::readFromIgesFile: cPoint after clamping");

      reinitialize();
      initialize();

      if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: uKnot after clamping and init");
      if( Mapping::debug & 4 ) cPoint.display("NurbsMapping::readFromIgesFile: cPoint after clamping and init");

    }

    truncateToDomainBounds();
    if( Mapping::debug & 4 ) uKnot.display("NurbsMapping::readFromIgesFile: uKnot after trucateToDomainBounds");
  }
  
  if( Mapping::debug & 8 )
  {
    printf(" NurbsMapping::readFromIgesFile: save data for reading with get\n"
           " ****************************** START %s ***************************\n",(const char*)getName(mappingName));
    put(stdout);
    printf(" ****************************** END %s ***************************\n",(const char*)getName(mappingName));
  }
  

  //uKnot.display("uKnot after iges");
// *wdh* Do not check for periodicity after all, cat2, no. 112 does not fit correctly if the surface
// is periodic.
//   if( false && iges.entity(item)==IgesReader::rationalBSplineSurface )
//   {
//     // Here we check to see if the spline surface is periodic -- there seem to
//     // be surfaces (e.g. cat2.igs, no. 112) that are not marked as periodic but should be.
//     Range R1=n1+1;
//     Range R2=n2+1;
//     Range Rw=rangeDimension+1;
//     if( !getIsPeriodic(axis1) )
//     {
//       // check for perioidicity
//       real dist = max(fabs(cPoint(0,R2,Rw)-cPoint(n1,R2,Rw)));
//       real scale = (real)getRangeBound(End,axis2)-(real)getRangeBound(Start,axis2);
//       if( dist <= scale*REAL_EPSILON*10. )
//       {
// 	if( debug & 2 ) printf("***INFO*** setting this NURBS surface to be periodic along axis=0\n");
// 	setIsPeriodic(axis1,functionPeriodic);
//       }
//     }
//     if( !getIsPeriodic(axis2) )
//     {
//       // check for perioidicity
//       real dist = max(fabs(cPoint(R1,0,Rw)-cPoint(R1,n2,Rw)));
//       real scale = (real)getRangeBound(End,axis1)-(real)getRangeBound(Start,axis1);
//       if( dist <= scale*REAL_EPSILON*10. )
//       {
// 	if( debug & 2 ) printf("***INFO*** setting this NURBS surface to be periodic along axis=1\n");
// 	setIsPeriodic(axis2,functionPeriodic);
//       }
//     }
//  }
  

  return 0;
}


int NurbsMapping::
parametricSplineSurface(int mu, int mv, RealArray & u, RealArray & v, RealArray & poly )
// =================================================================================
// Convert a parametric spline surface of mu X mv patches, each patch contains a
// bi-cubic polynomial, into a NURBS
//
// /u (input) : u(0:mu) : break points in u
// /v (input) : v(0:mv) : break points in v
// /poly(4,4,3,mu,mv) : patch coefficients
// ================================================================================
{
  domainDimension=2;
  rangeDimension=3;

  int i,j,m,n,nn;
  
  p1=3;
  m1=(mu+1)*3+1;
  uKnot.redim(m1+1);
  for( i=1, m=0; m<=mu; m++)
    for( nn=1; nn<=3; nn++,i++)
      uKnot(i) = u(m);
  uKnot(0) = uKnot(1);
  uKnot(i) = uKnot(i-1);
  assert( m1==i);

  p2=3;
  m2=(mv+1)*3+1;
  vKnot.redim(m2+1);
  for( i=1, m=0; m<=mv; m++)
    for( nn=1; nn<=3; nn++,i++)
      vKnot(i) = v(m);
  vKnot(0) = vKnot(1);
  vKnot(i) = vKnot(i-1);

  assert( m2==i);

  uMin=uKnot(0);  // min(uKnot);
  uMax=uKnot(m1); // max(uKnot);
  vMin=vKnot(0);  // min(vKnot);
  vMax=vKnot(m2); // max(vKnot);
  

  // a0(i,j,k,m,n) = poly(i,j,k,m,n)*u^i * v^j;
  RealArray a0(4,4,3,mu,mv);
  Range Rx=rangeDimension;
  
  real du[4], dv[4];
  for( m=0; m<mu; m++)
  {
    du[0]=1.;
    du[1]=u(m+1)-u(m);
    du[2]=du[1]*du[1];
    du[3]=du[2]*du[1];
    for( n=0; n<mv; n++)
    {
      dv[0]=1.;
      dv[1]=v(n+1)-v(n);
      dv[2]=dv[1]*dv[1];
      dv[3]=dv[2]*dv[1];
      
      for( int j=0; j<4; j++ )
	for( int i=0; i<4; i++ )
	  a0(i,j,Rx,m,n)=poly(i,j,Rx,m,n)*du[i]*dv[j];
    }
  }

  n1=3*mu;  // number of control points = n1+1
  n2=3*mv;
  cPoint.redim(n1+1,n2+1,rangeDimension+1);
  
  assert( m1 == n1+p1+1 );
  assert( m2 == n2+p2+1 );
  
   
  Range all, R3=3, R4=4;
  int nw=rangeDimension;
  cPoint(all,all,nw)=1.;  // weights
  
  // for each patc we compute 4x4 control points -- but we share the
  // last control pt with the next patch. We only need to keep it
  // for the last patch in each direction.
  RealArray cp(4,4,rangeDimension);
  
  for( i=m=0; m<mu; m++,i+=3 )
  {
    for( j=n=0; n<mv; n++,j+=3 )
    {
      const RealArray & aa = a0(all,all,Rx,m,n);

      cp(0,0,Rx) = aa(0,0,Rx);

      cp(1,0,Rx) = aa(0,0,Rx)+aa(1,0,Rx)/3.;

      cp(2,0,Rx) = aa(0,0,Rx)+2.*aa(1,0,Rx)/3.+aa(2,0,Rx)/3.;

      cp(3,0,Rx) = aa(0,0,Rx)+aa(1,0,Rx)+aa(2,0,Rx)+aa(3,0,Rx);

      cp(3,1,Rx) = aa(0,0,Rx)+aa(1,0,Rx)+aa(2,0,Rx)+aa(3,0,Rx)
                        + (aa(0,1,Rx)+aa(1,1,Rx)+aa(2,1,Rx)+aa(3,1,Rx))/3.;

      cp(3,2,Rx) = aa(0,0,Rx)+aa(1,0,Rx)+aa(2,0,Rx)+aa(3,0,Rx)+
                       2.*(aa(0,1,Rx)+aa(1,1,Rx)+aa(2,1,Rx)+aa(3,1,Rx))/3.+
                          (aa(0,2,Rx)+aa(1,2,Rx)+aa(2,2,Rx)+aa(3,2,Rx))/3.;

      cp(3,3,Rx) = aa(0,0,Rx)+aa(1,0,Rx)+aa(2,0,Rx)+aa(3,0,Rx)
                 + aa(0,1,Rx)+aa(1,1,Rx)+aa(2,1,Rx)+aa(3,1,Rx)
                 + aa(0,2,Rx)+aa(1,2,Rx)+aa(2,2,Rx)+aa(3,2,Rx)
                 + aa(0,3,Rx)+aa(1,3,Rx)+aa(2,3,Rx)+aa(3,3,Rx);

      cp(2,3,Rx) = aa(0,0,Rx)+aa(0,1,Rx)+aa(0,2,Rx)+aa(0,3,Rx)+
                       2.*(aa(1,0,Rx)+aa(1,1,Rx)+aa(1,2,Rx)+aa(1,3,Rx))/3.+
                          (aa(2,0,Rx)+aa(2,1,Rx)+aa(2,2,Rx)+aa(2,3,Rx))/3.;

      cp(1,3,Rx) = aa(0,0,Rx)+aa(0,1,Rx)+aa(0,2,Rx)+aa(0,3,Rx)
                         +(aa(1,0,Rx)+aa(1,1,Rx)+aa(1,2,Rx)+aa(1,3,Rx))/3.;

      cp(0,3,Rx) = aa(0,0,Rx)+aa(0,1,Rx)+aa(0,2,Rx)+aa(0,3,Rx);

      cp(0,2,Rx) = aa(0,0,Rx)+2.*aa(0,1,Rx)/3.+aa(0,2,Rx)/3.;

      cp(0,1,Rx) = aa(0,0,Rx)+aa(0,1,Rx)/3.;

      cp(1,1,Rx) = aa(0,0,Rx)+aa(1,0,Rx)/3.
                         + aa(0,1,Rx)/3.+aa(1,1,Rx)/9.;

      cp(2,1,Rx) = aa(0,0,Rx)+2.*aa(1,0,Rx)/3.+aa(2,0,Rx)/3.
                         + aa(0,1,Rx)/3.+2.*aa(1,1,Rx)/9.+aa(2,1,Rx)/9.;

      cp(1,2,Rx) = aa(0,0,Rx)+aa(1,0,Rx)/3.+2.*aa(0,1,Rx)/3.
                      + 2.*aa(1,1,Rx)/9.+aa(0,2,Rx)/3.+aa(1,2,Rx)/9.;

      cp(2,2,Rx) = aa(0,0,Rx)+2.*aa(1,0,Rx)/3.+aa(2,0,Rx)/3.
                      + 2.*aa(0,1,Rx)/3.+4.*aa(1,1,Rx)/9.+2.*aa(2,1,Rx)/9.
                         + aa(0,2,Rx)/3.+2.*aa(1,2,Rx)/9.+aa(2,2,Rx)/9.;

      cPoint(i+R4,j+R4,Rx)=cp(R4,R4,Rx);
      
    }
  }
  
  
  if( uMin!=0. || uMax!=1. || 
      vMin!=0. || vMax!=1.  )
    normalizeKnots();

  initialize();

/* ----
  if( false )
  {
    // check ...
    realArray r(1,2),x(1,3),t(2),xp(3);
    r=.5;

    for(;;)
    {
      printf("parametric spline surface: enter r0,r1 \n");
      cin >> r(0,0) >> r(0,1);
      
      if( r(0,0)<0. )
	break;
      
      map(r,x);
  
      t(0)=uMin+r(0,0)*(uMax-uMin);
      t(1)=vMin+r(0,1)*(vMax-vMin);
          
      m=0;  // patch number
      while( m<mu-1 && t(0)>u(m+1) )
	m++;

      n=0;  // patch number
      while( n<mv-1 && t(1)>v(n+1) )
	n++;

      du[0]=1.;
      du[1]=t(0)-u(m);
      du[2]=du[1]*du[1];
      du[3]=du[2]*du[1];
      dv[0]=1.;
      dv[1]=t(1)-v(n);
      dv[2]=dv[1]*dv[1];
      dv[3]=dv[2]*dv[1];
      
      xp=0.;
      for( int j=0; j<4; j++ )
	for( int i=0; i<4; i++ )
	  for( int axis=0; axis<rangeDimension; axis++ )
	    xp(axis)+=poly(i,j,axis,m,n)*du[i]*dv[j];

      printf(" r=(%8.2e,%8.2e) x=(%e,%e,%e) t=(%8.2e,%8.2e) m=%i,n=%i, xp=(%e,%e,%e)\n",
             r(0,0),r(0,1),x(0,0),x(0,1),x(0,2),
	     t(0),t(1),m,n,xp(0),xp(1),xp(2));
    }
    
  }
---- */
  
  return 0;
  

}


int NurbsMapping:: 
parametricCurve(const NurbsMapping & nurbs,
                const bool & scaleParameterSpace /* = true */ )
//===========================================================================
/// \brief 
///    Indicate that this nurb is actually a parametric curve on another nurb surface.
/// \param nurbs (input) : Here is the NURBS surface for which this NURBS is a parametric surface.
/// \param scaleParameterSpace (input) : if true, scale the range space of this nurb
///    to be on the unit interval. This is usually required since the NurbsMapping scales
///    the knots to lie on [0,1] (normally) and so we then need to scale this Mapping to
///    be consistent.
//===========================================================================
{
  assert( domainDimension==1 && rangeDimension<=3 );
  
  Range all;
  if( rangeDimension==3 )
  {
    RealArray cp(Range(cPoint.getBase(0),cPoint.getBound(0)),Range(0,2));
    for( int axis=0; axis<rangeDimension-1; axis++ )
      cp(all,axis)=cPoint(all,axis);

    cp(all,2)=cPoint(all,3);  // weights
    cPoint.reference(cp);
    setRangeDimension(2);
  }

  assert( rangeDimension==2 && domainDimension==1 );
  
  if( scaleParameterSpace )
  {
    assert( nurbs.getDomainDimension()==2 );

    real xMn[2] = { nurbs.uMin,nurbs.vMin };
    real xMx[2] = { nurbs.uMax,nurbs.vMax };
    if( Mapping::debug & 4 )
      printf("----NurbsMapping::parametricCurve:scaleParameterSpace: uMin=%e, uMax=%e, vMin=%e, vMax=%e  \n",
            nurbs.uMin,nurbs.uMax,nurbs.vMin,nurbs.vMax);
    
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      cPoint(all,axis)/=cPoint(all,rangeDimension); // scale by weight first *wdh* 990106
      cPoint(all,axis)=(cPoint(all,axis)-xMn[axis])/(xMx[axis]-xMn[axis]);
      cPoint(all,axis)*=cPoint(all,rangeDimension);
    }
    
    
  }
  setRangeSpace(parameterSpace);
  
  mappingHasChanged();  // *wdh* 011018
  setBounds();
  // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse
  reinitialize(); 
  
  return 0;
}


int NurbsMapping::
shift(const real & shiftx /* =0. */, 
      const real & shifty /* =0. */, 
      const real & shiftz /* =0.*/ )
//===========================================================================
/// \brief  Shift the NURBS in space.
//===========================================================================
{
  const real shift[3]={shiftx,shifty,shiftz};
  Range all;
  RealArray weight;
  if( domainDimension==1 )
    weight=cPoint(all,rangeDimension);
  else if( domainDimension==2 )
    weight=cPoint(all,all,rangeDimension);
  else
    weight=cPoint(all,all,all,rangeDimension);
  where( weight==0. )
    weight=1.;
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    if( domainDimension==1 )
      cPoint(all,axis)=(cPoint(all,axis)/weight+shift[axis])*weight;
    else if( domainDimension==2 )
      cPoint(all,all,axis)=(cPoint(all,all,axis)/weight+shift[axis])*weight;
    else 
      cPoint(all,all,all,axis)=(cPoint(all,all,all,axis)/weight+shift[axis])*weight;
  }
  setBounds();
  reinitialize(); 
  mappingHasChanged();  // *wdh* 011018

  return 0;
}

int NurbsMapping::
scale(const real & scalex /* =0. */, 
      const real & scaley /* =0. */, 
      const real & scalez /* =0.*/ )
//===========================================================================
/// \brief  Scale the NURBS in space.
//===========================================================================
{
  const real scale[3]={scalex,scaley,scalez};
  Range all;
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    if( domainDimension==1 )
      cPoint(all,axis)*=scale[axis];
    else if( domainDimension==2 )
      cPoint(all,all,axis)*=scale[axis];
    else 
      cPoint(all,all,all,axis)*=scale[axis];
  }
  setBounds();
  reinitialize(); 
  mappingHasChanged();  // *wdh* 011018
  return 0;
}

int NurbsMapping::
rotate( const int & axis, const real & theta )
//===========================================================================
/// \brief  Perform a rotation about a given axis. This rotation is applied
///    after any existing transformations. 
/// \param axis (input) : axis to rotate about (0,1,2)
/// \param theta (input) : angle in radians to rotate by.
//===========================================================================
{
  if( rangeDimension==1 )
    return 1;
  if( rangeDimension==2 && axis!=axis3 )
  {
    printf("NurbsMapping::rotate:ERROR: Can only rotate a NURBS with rangeDimension==2 around axis==2\n");
    return 1;
  }
  const real ct = cos(theta); 
  const real st = sin(theta); 

  const int i1 = (axis+1) % 3;
  const int i2 = (axis+2) % 3;
  Range all;
  if( domainDimension==1 )
  {
    const RealArray cPoint1=cPoint(all,i1)*ct-cPoint(all,i2)*st;
    cPoint(all,i2)=         cPoint(all,i1)*st+cPoint(all,i2)*ct;
    cPoint(all,i1)=cPoint1;
  }
  else if( domainDimension==2 )
  {
    const RealArray cPoint1=cPoint(all,all,i1)*ct-cPoint(all,all,i2)*st;
    cPoint(all,all,i2)=     cPoint(all,all,i1)*st+cPoint(all,all,i2)*ct;
    cPoint(all,all,i1)=cPoint1;
  }
  else if( domainDimension==3 )
  {
    const RealArray cPoint1=cPoint(all,all,all,i1)*ct-cPoint(all,all,all,i2)*st;
    cPoint(all,all,all,i2)= cPoint(all,all,all,i1)*st+cPoint(all,all,all,i2)*ct;
    cPoint(all,all,all,i1)=cPoint1;
  }

  setBounds();
  reinitialize(); 
  mappingHasChanged();  // *wdh* 011018
  return 0;
}


int NurbsMapping::
matrixTransform( const RealArray & m )
//===========================================================================
/// \brief  
///     Perform a general matrix transform using a 2x2 or 3x3 matrix.
///   Convert the NURBS to 2D or 3D if the transformation so specifies -- i.e.
///  if you transform a NURBS with rangeDimension==2 with a 3x3 matrix then the
///  result will be a NURBS with rangeDimension==3.
/// \param m (input) : m(0:2,0:2) matrix to transform with
//===========================================================================
{
  int mDim=m.getLength(0);
  if( (mDim!=2 && mDim!=3) || mDim!=m.getLength(1) )
  {
    printf("NurbsMapping::matrixTransform:ERROR: matrix must be 2x2 or 3x3\n");
    return 1;
  }
  
  if( mDim<rangeDimension )
  {
    printf("NurbsMapping::matrixTransform:ERROR: matrix must be at least %ix%i (rangeDimension=%i)\n",
             rangeDimension,rangeDimension,rangeDimension);
    return 1;
  }
  

  int newRangeDimension = max(mDim,rangeDimension);

  Range all;
  Range Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  I1=1; I2=1; I3=1;
  int num=1, axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    Iv[axis]=cPoint.dimension(axis);
    num*=Iv[axis].getLength();
  }
  
  Range Rw=rangeDimension+1;
  cPoint.reshape(num,Rw);

  Range Rw2=newRangeDimension+1;
  Range Rnum=num;
  RealArray cp(Rnum,Rw2);
  cp(all,newRangeDimension)=cPoint(all,rangeDimension);  // weights
  
  for( axis=0; axis<newRangeDimension; axis++ )
  {
    cp(all,axis)=0.;
    for( int dir=0; dir<rangeDimension; dir++ )
      cp(all,axis)+=m(axis,dir)*cPoint(all,dir);
  }
  cPoint.redim(num,Rw2);
  cPoint=cp;
  if( domainDimension==1 )
    cPoint.reshape(I1,Rw2);
  else if( domainDimension==2 )
    cPoint.reshape(I1,I2,Rw2);
  else
    cPoint.reshape(I1,I2,I3,Rw2);

  if( newRangeDimension==rangeDimension )
  {
    setBounds();
    reinitialize(); 
  }
  else
  {
    rangeDimension=newRangeDimension;
    // *wdh* 010328 setBounds();
    initialize(); 
  }
  mappingHasChanged();  // *wdh* 011018
  
  return 0;
}

void NurbsMapping::
lowerRangeDimension()
{
// reduce the range dimension from 3 to 2
  if (domainDimension != 1 || rangeDimension != 3)
  {
    printf("NurbsMapping:: lowerRangeDimension: Error, domainDim = %i, rangeDim = %i\n", 
	   domainDimension, rangeDimension);
    return;
  }
  printf("NurbsMapping:: lowerRangeDimension: reshuffling the weights in the control point array\n");
  Range all;
  cPoint(all,2) = cPoint(all,3);
  setRangeDimension(2);
  initialize();
}


int NurbsMapping::
specify(const int &  m,
	const int & n,
	const int & p,
	const RealArray & knot,
	const RealArray & controlPoint,
	const int & rangeDimension_ /* =3 */,
	bool normalizeTheKnots /* =true*/  )
//===========================================================================
/// \brief  Specify a {\bf curve} in 2D or 3D using knots and control points
/// \param m (input) : The number of knots is m+1
/// \param n (input) : the number of control points is n+1
/// \param p (input) : order of the B-spline
/// \param controlPoint(0:n,0:rangeDimension) (input) : control points and weights
/// \param normalizeTheKnots (input) : by default, normalize the knots to [0,1]. Set to false
///     if you do not want the knots normalized. 
//===========================================================================
{
  //knot.display("knot in specify");
  setDomainDimension(1);
  setRangeDimension(rangeDimension_);
  m1=m; m2=0;
  n1=n; n2=0;
  p1=p; p2=0;
  uKnot.redim(knot); vKnot.redim(0);
  uKnot=knot;
  uMin=uKnot(0); // min(uKnot);
  uMax=uKnot(m1); // max(uKnot);
  cPoint.redim(controlPoint);
  cPoint=controlPoint;

  int nw=rangeDimension;  // position of the weight
  Range R1(0,n1);
  for( int axis=0; axis<rangeDimension; axis++ )
    cPoint(R1,axis)*=cPoint(R1,nw); // multiply by the weight
  
  if( normalizeTheKnots ) normalizeKnots();
  setGridDimensions(axis1, max(11,n1));

  initialize();
  return 0;
}

int NurbsMapping::
specify(const int & n1_, 
	const int & n2_,
	const int & p1_, 
	const int & p2_, 
	const RealArray & uKnot_, 
	const RealArray & vKnot_,
	const RealArray & controlPoint,
	const int & rangeDimension_ /* =3 */,
	bool normalizeTheKnots /* =true*/  )
//===========================================================================
/// \brief  Specify a NURBS with domainDimension==2 using knots and control points
/// \param n1_,n2_ (input) : the number of control points is n1+1 by n2+1
/// \param p1_,p2_ (input) : order of the B-spline in each direction.
/// \param uKnot_,vKnot_ (input) : knots.
/// \param controlPoint(0:n1,0:n2,0:rangeDimenion) (input) : control points and weights
/// \param normalizeTheKnots (input) : by default, normailize the knots to [0,1]. Set to false
///     if you do not want the knots normalized. 
//===========================================================================
{
  setDomainDimension(2);
  setRangeDimension(rangeDimension_);
  // m1=m1_; m2=m2_;
  n1=n1_; n2=n2_;
  p1=p1_; p2=p2_;

  m1=n1+p1+1;
  m2=n2+p2+1;
  
  uKnot.redim(0); 
  uKnot=uKnot_;
  vKnot.redim(0);
  vKnot=vKnot_;
  uMin=uKnot(0);  // min(uKnot);
  uMax=uKnot(m1); // max(uKnot);
  vMin=vKnot(0);  // min(vKnot);
  vMax=vKnot(m2); // max(vKnot);
  cPoint.redim(0);
  cPoint=controlPoint;

  int nw=rangeDimension;  // position of the weight
  Range R1(0,n1),R2(0,n2);
  for( int axis=0; axis<rangeDimension; axis++ )
    cPoint(R1,R2,axis)*=cPoint(R1,R2,nw); // multiply by the weight
  
  if( normalizeTheKnots ) normalizeKnots();
  setGridDimensions(axis1, max(11,n1));
  setGridDimensions(axis1, max(11,n2));

  initialize();
  return 0;
}

int NurbsMapping:: 
setDomainInterval(const real & r1Start /* =0. */, 
                  const real & r1End /* =1. */,
                  const real & r2Start /* =0. */, 
                  const real & r2End /* =1. */,
                  const real & r3Start /* =0. */, 
                  const real & r3End /* =1. */ )
//=====================================================================================
/// \details 
///  Restrict the domain of the nurbs.
///  By default the nurbs is parameterized on the interval [0,1] (1D) or [0,1]x[0,1] in 2D etc.
///  You may choose a sub-section of the nurbs by choosing a new interval [rStart,rEnd].
///  For periodic nurbss the interval may lie in [-1,2] so the sub-section can cross the branch cut.
///  You may even choose rEnd<rStart to reverse the order of the parameterization.
/// \param rStart1,rEnd1,rStart2,rEnd2,rStart3,rEnd3 (input) : define the new interval.
//=====================================================================================
{
  real ra[3]={r1Start,r2Start,r3Start}; 
  real rb[3]={r1End,r2End,r3End}; 
  for( int axis=0; axis<domainDimension; axis++ )
  {

    if( fabs(rb[axis]-ra[axis])==1. )
      nurbsIsPeriodic[axis]=getIsPeriodic(axis);
  
    rStart[axis]=ra[axis];
    rEnd[axis]=rb[axis];
	
    const real rMin = nurbsIsPeriodic[axis] ? -1. : 0.;
    if( rStart[axis]<rMin )
    {
      printf("NurbsMapping::setDomainInterval:ERROR: rStart=%f must be at least %f. Setting to %f\n",rStart[axis],
            rMin,rMin);
      rStart[axis]=rMin;
    }
    if( rEnd[axis]==rStart[axis] )
    {
      printf("NurbsMapping::setDomainInterval:ERROR: rStart=rEnd=%e. Setting rStart=0., rEnd=1.\n",rStart[axis]);
      rStart[axis]=0., rEnd[axis]=1.;
    }
    if( nurbsIsPeriodic[axis] )
      if( fabs(rEnd[axis]-rStart[axis])!=1. )
	setIsPeriodic(axis,notPeriodic);
      else
	setIsPeriodic(axis,(periodicType)nurbsIsPeriodic[axis]);
  }
  
  return 0;
}

void NurbsMapping::
initialize( bool setMappingHasChanged /* =true */  )
//===========================================================================
/// \brief  Initialize the NURBS. This is a protected routine.
///  Determine if the weights are constant so
///  that we can use more efficient routines. Set bounds for the Mapping.
/// 
/// \param setMappingHasChanged (input) : if true (by default) we call mappingHasChanged so that the grid etc. will be marked
///                   out of date. 
/// \param NOTES: Normally we multiply the control points by the weights.
///      BUT,  if the weights are constant we divide everything by this constant value so
///  we can avoid dividing by the weight term when we evaluate. When the weights are
///  constant {\tt nonUniformWeights==false};
//===========================================================================
{
  if( setMappingHasChanged )
    mappingHasChanged();

  //uKnot.display("uKnot starting initialize");

  // Check to see if the weights are constant (in which case we have more efficient algorithms)
  nonUniformWeights=false; 

  Range R1(0,n1), R2(0,n2), R3(0,n3);
  real wgtMin,wgtMax;
  int nw=rangeDimension;
  if( domainDimension==1 )
  {
    if( (p1+1) > maximumOrder )
    {
      printf("NurbsMapping::ERROR: the order p1+1=%i is greater than the maximumOrder=%i\n"
             " You will need to increase the value of maximumOrder in NurbsMapping.h\n"
             ,p1+1,maximumOrder);
      throw "error";
    }

    if( cPoint.getBound(1)<nw )
    { // no weights supplied
      wgtMin=1.;
      wgtMax=1.;
    }
    else
    {
      wgtMin=min(cPoint(R1,nw));
      wgtMax=max(cPoint(R1,nw));
    }
  }
  else if( domainDimension==2 )
  {
    if( (p2+1) > maximumOrder )
    {
      printf("NurbsMapping::ERROR: the order p2+1=%i is greater than the maximumOrder=%i\n"
             " You will need to increase the value of maximumOrder in NurbsMapping.h\n"
             ,p2+1,maximumOrder);
      throw "error";
    }
    if( cPoint.getBound(2)<nw )
    {// no weights supplied
      wgtMin=1.;
      wgtMax=1.;
    }
    else
    {
      wgtMax=max(cPoint(R1,R2,nw));
      wgtMin=min(cPoint(R1,R2,nw));
    }
  }
  else if( domainDimension==3 )
  {
    if( (p3+1) > maximumOrder )
    {
      printf("NurbsMapping::ERROR: the order p3+1=%i is greater than the maximumOrder=%i\n"
             " You will need to increase the value of maximumOrder in NurbsMapping.h\n"
             ,p3+1,maximumOrder);
      throw "error";
    }
    if( cPoint.getBound(3)<nw )
    {// no weights supplied
      wgtMin=1.;
      wgtMax=1.;
    }
    else
    {
      wgtMax=max(cPoint(R1,R2,R3,nw));
      wgtMin=min(cPoint(R1,R2,R3,nw));
    }
  }
  if( wgtMax-wgtMin > wgtMax*REAL_EPSILON )
  {
    nonUniformWeights=true;
    if( Mapping::debug & 4 )
      printf("Weights are not constant: min(w)=%e, max(w) =%e \n",wgtMin,wgtMax);
  }
  else
  {
    if( fabs(wgtMax-1.) > wgtMax*REAL_EPSILON )
    {
      printF("\n\n NurbsMapping: &&&&&&&&&&&&&&& weights constant but not one!! &&&&&&&&&&&&& \n\n");
      if( wgtMax<=0. )
      {
	printF("NurbsMapping::initialize:ERROR: weights are zero or negative!! \n");
	::display(cPoint,"cPoint","%5.2f ");
	OV_ABORT("error");
      }
      if( domainDimension==1 )
      {
	Range R1(0,n1);
	for( int axis=0; axis<rangeDimension; axis++ )
	  cPoint(R1,axis)/=cPoint(R1,3); // divide by constant weight
      }
      else if (domainDimension==2 )
      {
	Range R1(0,n1), R2(0,n2);
	for( int axis=0; axis<rangeDimension; axis++ )
	  cPoint(R1,R2,axis)/=cPoint(R1,R2,3); // divide by constant weight
      }
    }
  }

  //uKnot.display("uKnot after initialize");
  setBounds();
  initialized=true;

// AP: Test periodicity:
  if (domainDimension==2)
  {
    int nPoints=3, i;
    real dr = 1./(nPoints-1), diff;
  
    RealArray rp1(nPoints,domainDimension), xp1(nPoints,rangeDimension), rp2(nPoints,domainDimension), 
      xp2(nPoints,rangeDimension);
// periodic in direction axis1?
    for (i=0; i<nPoints; i++)
    {
      rp1(i,0) = 0;
      rp1(i,1) = i*dr;
      rp2(i,0) = 1;
      rp2(i,1) = i*dr;
    }
    mapS(rp1,xp1);
    mapS(rp2,xp2);
    diff = sum(fabs(xp1-xp2));
    if (diff < 100*REAL_MIN && getIsPeriodic(axis1) != functionPeriodic)
    {
      printf("Nurbsmapping: periodicity test axis1: diff=%e. Making the mapping functionPeriodic "
	     "along axis1!\n", diff);
      setIsPeriodic(axis1, functionPeriodic);
      nurbsIsPeriodic[axis1]=functionPeriodic;
    }
// periodic in direction axis2?
    for (i=0; i<nPoints; i++)
    {
      rp1(i,0) = i*dr;
      rp1(i,1) = 0;
      rp2(i,0) = i*dr;
      rp2(i,1) = 1;
    }
    mapS(rp1,xp1);
    mapS(rp2,xp2);
    diff = sum(fabs(xp1-xp2));
    if (diff < 100*REAL_MIN && getIsPeriodic(axis2) != functionPeriodic)
    {
      printf("Nurbsmapping: periodicity test axis2: diff=%e. Making the mapping functionPeriodic "
	     "along axis2!\n", diff);
      setIsPeriodic(axis2, functionPeriodic);
      nurbsIsPeriodic[axis2]=functionPeriodic;
    }
  } // end if domainDimension == 2
  
  
  
  
}
 

void NurbsMapping::
setBounds()
//===========================================================================
/// \brief  protected routine. Set the approximate bounds on the mapping, used by plotting routines etc.
///  Use the control points as an approximation
///  *** note only apply this to the normalized control-points ***
//===========================================================================
{
  int nw=rangeDimension; // poistion of the weights
  if( domainDimension==1 )
  {
    Range R1(0,n1);
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      if( nonUniformWeights )
      {
        setRangeBound(Start,axis,min(cPoint(R1,axis)/cPoint(R1,nw)));
        setRangeBound(End  ,axis,max(cPoint(R1,axis)/cPoint(R1,nw)));
      }
      else
      {
        setRangeBound(Start,axis,min(cPoint(R1,axis)));
        setRangeBound(End  ,axis,max(cPoint(R1,axis)));
      }
    }
  }
  else
  {
    Range R1(0,n1), R2(0,n2);
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      if( nonUniformWeights )
      {
        setRangeBound(Start,axis,min(cPoint(R1,R2,axis)/cPoint(R1,R2,nw)));
        setRangeBound(End  ,axis,max(cPoint(R1,R2,axis)/cPoint(R1,R2,nw)));
      }
      else
      {
        setRangeBound(Start,axis,min(cPoint(R1,R2,axis)));
        setRangeBound(End  ,axis,max(cPoint(R1,R2,axis)));
      }
    }
  }
}

// =======================================================================
//  Old version - here we measured the 4D distance including the weight
// =======================================================================
// real NurbsMapping::
// distance4D( const RealArray & x, const RealArray & y )
// {
//   Range Rw= nonUniformWeights ? Range(0,rangeDimension) : Range(0,rangeDimension-1);
//   real dist = SQRT( sum(SQR( x(nullRange,Rw)-y(nullRange,Rw))) );
//   return dist;
// }

// ==========================================================================
// *wdh* 100208 Measure the distance between two control points. In this version
//  we measure the actual distance between the coordinates (by dividing by the weights)
// ==========================================================================
real NurbsMapping::
distance4D( const RealArray & x, const RealArray & y )
{
  real dist=0.;
  for( int i=x.getBase(0), j=y.getBase(0); i<=x.getBound(0); i++,j++ )
  {
    real wx=x(i,rangeDimension), wy=y(j,rangeDimension);
    for( int dir=0; dir<rangeDimension; dir++ )
    {
      real d = x(i,dir)/wx - y(j,dir)/wy;
      dist+= d*d;
    }
  }
  dist=sqrt(dist);
  return dist;
}

int NurbsMapping::
removeKnot(const int & index, 
           const int & numberOfTimesToRemove, // why is this int passed by reference?
           int &  numberRemoved, const real & tol  ) // why is tol passed by reference?
//=====================================================================================
/// \brief  Remove a knot (if possible) so that the Nurbs remains unchanged
/// \param index (input) : try to remove the knot at this index. AP: NOTE: uKnot(index) != uKnot(index+1)
/// \param numberOfTimesToRemove (input) : the number of times to try and remove the knot. 
/// \param numberRemoved (output):  the actual number of times the knot was removed
//=====================================================================================
{
  if( domainDimension==1 )
  {
    if( index<0 || index>m1 )
    {
      cout << "NurbsMapping::removeKnot: ERROR: the knot index you are trying to remove is out of range\n";
      numberRemoved=0;
      return 1;
    }
  
    //    real tolerance=10.*REAL_EPSILON;  // needs to be dimensional
    real tolerance=tol; //100.*FLT_EPSILON;  // needs to be dimensional

    int multiplicity=1;
    int i=index-1;
    while( i>=0 && uKnot(i)==uKnot(index) )
    {
      multiplicity++;
      i--;
    }
    i=index+1;
    while( i<=m1 && uKnot(i)==uKnot(index) )
    {
      multiplicity++;
      i++;
    }
  
    int fout = (2*index-multiplicity-p1)/2;  // first control point out
    int last = index-multiplicity;
    int first = index-p1;

    if( first<0 || first>m1 )
    {
      cout << "NurbsMapping::removeKnot: ERROR: the knot index you are trying to remove is out of range\n";
      cout << " index = " << index << endl;
      numberRemoved=0;
      return 1;
    }

    int ord = p1+1;
    real u=uKnot(index);

    // Range Rw= nonUniformWeights ? Range(0,rangeDimension) : Range(0,rangeDimension-1);
    Range Rw(0,rangeDimension);

    RealArray temp(Range(0,2*p1+1-1),Rw);
    real alfi,alfj, dist4;
    int j,ii,jj,t;
//    printf("removeKnot: numberOfTimesToRemove=%i\n", numberOfTimesToRemove);
    for( t=0; t<numberOfTimesToRemove; t++ )
    {
      int off=first-1;  // difference in index between temp and p
      temp(0,Rw)=cPoint(off,Rw);
      temp(last+1-off,Rw)=cPoint(last+1,Rw);
      i=first; j=last; ii=1; jj=last-off;
      int remFlag=0;
      while( j-i > t )
      {
	alfi = (u-uKnot(i))/(uKnot(i+ord+t)-uKnot(i));
	alfj = (u-uKnot(j-t))/(uKnot(j+ord)-uKnot(j-t));
	temp(ii,Rw)=(cPoint(i,Rw)-(1.-alfi)*temp(ii-1,Rw))/alfi;
	temp(jj,Rw)=(cPoint(j,Rw)-alfj*temp(jj+1,Rw))/(1.-alfj);
	i++; ii++; j--; jj--;
      }
      if( j-i < t )
      { // check if the knot is removable
	if( (dist4=distance4D(temp(ii-1,Rw),temp(jj+1,Rw))) <= tolerance )
	{
	  remFlag=1;
	}
      }
      else
      {
	alfi=(u-uKnot(i))/(uKnot(i+ord+t)-uKnot(i));
	if( (dist4=distance4D(cPoint(i,Rw),alfi*temp(ii+t+1,Rw)+(1.-alfi)*temp(ii-1,Rw))) <= tolerance )
	{
	  remFlag=1;
	}
      }

      if( remFlag==0 )
      {
//	printf("removeKnot: Cant' remove any more than knots %i. dist4=%e, tolerance=%e\n", t, dist4, tolerance);
	break;   // cannot remove any more knots
      }
      else
      {
	// knot can be removed, save new control points
	i=first; j=last;
	while( j-i > t )
	{
	  cPoint(i,Rw)=temp(i-off,Rw);
	  cPoint(j,Rw)=temp(j-off,Rw);
	  i++; j--;
	}
      }
      first--;  last++;
    }
    if( t==0 )
    {
      numberRemoved=0;
      if( Mapping::debug & 4 )
        cout << "NurbsMapping::removeKnot:INFO: no knots removed! \n";
      return 0;
    }
    // shift knots
    int k;
    for( k=index+1; k<=m1; k++ )
      uKnot(k-t)=uKnot(k);
    j=fout; i=j;
    for( k=1; k<t; k++ )
    {
      if( (k % 2) == 1 )
	i++;
      else
	j--;
    }
    for( k=i+1; k<=n1; k++ )
    {
      cPoint(j,Rw)=cPoint(k,Rw);
      j++;
    }
    numberRemoved=t;
  
    // uKnot.display("removeKnot: Here is the new uKnot");

    m1-=numberRemoved;
    n1-=numberRemoved;
    
    initialize();
    
    return 0;
  }
  else
  {
    cout << "NurbsMapping::removeKnot: ERROR: only implemented for curves\n";
    numberRemoved=0;
    return 1;
  }
}

int NurbsMapping:: 
getParameterBounds( int axis, real & rStart_, real & rEnd_ ) const
//=====================================================================================
/// \brief  
///    Return current values for the parameter bounds.
/// \param axis (input) : return bounds for this axis.
/// \param rStart_, rEnd_: bounds.
//=====================================================================================
{
  assert( axis>=0 && axis<3 );
  
  rStart_=rStart[axis];
  rEnd_=rEnd[axis];
  return 0;
}



int NurbsMapping:: 
reparameterize(const real & uMin_, 
	       const real & uMax_,
	       const real & vMin_ /* =0. */ , 
	       const real & vMax_ /* =1. */ )
//=====================================================================================
/// \brief  Reparameterize the nurb to only use a sub-rectangle of the parameter space.
///     This function can also be used to reverse the direction of the parameterization by choosing
///    $\mbox{uMin} > \mbox{uMax}$ and/or $\mbox{vMin} > \mbox{vMax}$.
/// \param uMin,uMax (input):  subrange of u values to use, normally $0 \le \mbox{uMin} \ne \mbox{uMax} \le 1$
/// \param vMin,vMax (input): subrange of v values to use, normally  $0 \le \mbox{vMin} \ne \mbox{vMax} \le 1$ 
///      (for domainDimension==2)
/// 
/// \param Notes: this routine just scales the knots to be on a larger interval than [0,1]. Thus when
///     the Mapping is evaluated on [0,1] the result will only be a portion of the original surface.
/// \return  0 : success,  1 : failure
//=====================================================================================
{
  //uKnot.display("uKnot before reparameterize");

  if( Mapping::debug & 4 )
    printf("NurbsMapping::reparameterize: uMin=%e, uMax=%e, vMin=%e, vMax=%e \n",uMin_,uMax_,vMin_,vMax_);

  if( false ) // allow uMin and uMax to be larger than [0,1] since we may be removing a previous reparameterization
  {
    if( uMin_<rStart[0]-FLT_EPSILON || uMax_>rEnd[0]+FLT_EPSILON || uMin_==uMax_ || ( domainDimension>1 && (
      vMin_<rStart[1]-FLT_EPSILON || vMax_>rEnd[1]+FLT_EPSILON || vMin_==vMax_ )) )
    {
      cout << "NurbsMapping::reparameterize: invalid input values \n";
      printf("uMin=%e, uMax=%e, vMin=%e, vMax=%e. rStart[0]=%e, rEnd[0]=%e, rStart[1]=%e, rEnd[1]=%e \n",
	     uMin_,uMax_,vMin_,vMax_,rStart[0],rEnd[0],rStart[1],rEnd[1]);
      return 1;
    }
  }
  
  // reparameterize relative to current [0,1] interval. *wdh* 010309

//   real ua = uMin_*(uKnot(m1)-uKnot(0))+uKnot(0);
//   real ub = uMax_*(uKnot(m1)-uKnot(0))+uKnot(0);
//   real va = domainDimension==1 ? 0. : vMin_*(vKnot(m2)-vKnot(0))+vKnot(0);
//   real vb = domainDimension==1 ? 1. : vMax_*(vKnot(m2)-vKnot(0))+vKnot(0);

  real ua = uMin_*(rEnd[0]-rStart[0])+rStart[0];
  real ub = uMax_*(rEnd[0]-rStart[0])+rStart[0];
  real va = domainDimension==1 ? 0. : vMin_*(rEnd[1]-rStart[1])+rStart[1];
  real vb = domainDimension==1 ? 1. : vMax_*(rEnd[1]-rStart[1])+rStart[1];

  if( ub > ua )
  {
    uKnot= (uKnot-ua)/(ub-ua);
  }
  else
  {
    // reorder the knots
    RealArray temp(uKnot);
    for( int i=0; i<=m1; i++ )
      temp(i)=1.-uKnot(m1-i);    // ******** this only seems to work for ua=1., ub=0. ********************
    uKnot=temp;
    temp.redim(cPoint);
    Range R(0,rangeDimension);
    if( domainDimension==1 )
    {
      for( int i=0; i<=n1; i++ )
        temp(i,R)=cPoint(n1-i,R);
    }
    else if( domainDimension==2 )
    {
      Range R2(0,n2);
      for( int i=0; i<=n1; i++ )
        temp(i,R2,R)=cPoint(n1-i,R2,R);
    }
    cPoint=temp;
  }
  const real eps=REAL_EPSILON*2.;
  if( getIsPeriodic(axis1) )
  {
    // the curve will remain periodic if ua=0, ub=1. OR ua=1. ub=0.
    if( !( (fabs(ua)<eps && fabs(ub-1.)<eps ) || (fabs(ub)<eps && fabs(ua-1.)<eps)) )
    {
      printf("NURBS: repar: isPeriodic(u) rStart=%e, rEnd=%e, uMin_=%e, uMax_=%e, ua=%e, ub=%e\n",rStart[0],rEnd[0],
        uMin_,uMax_,ua,ub);
      setIsPeriodic(axis1,notPeriodic); // ** fix this *** must be able to reset
      nurbsIsPeriodic[axis1]=(int)notPeriodic;
    }
  }

  if( domainDimension>1 )
  {
    if( vb > va )
    {
      vKnot= (vKnot-va)/(vb-va);
    }
    else
    {
      // reorder the knots
      RealArray temp(vKnot);
      int i;
      for( i=0; i<=m2; i++ )
	temp(i)=1.-vKnot(m2-i);
      vKnot=temp;
      temp.redim(cPoint);
      Range R(0,rangeDimension);
      Range R1(0,n1);
      for( i=0; i<=n2; i++ )
	temp(R1,i,R)=cPoint(R1,n2-i,R);

      cPoint=temp;
    }
    if( getIsPeriodic(axis2) )
    {
      if( !((fabs(va)<eps && fabs(vb-1.)<eps ) || (fabs(vb)<eps && fabs(va-1.)<eps)) )
      {
	printf("NurbsMapping::reparameterization: Is no longer periodic in v.");
	setIsPeriodic(axis2,notPeriodic);
        nurbsIsPeriodic[axis2]=(int)notPeriodic;
	
      }
    }


  }
  
  reinitialize();
  initialize();

  return 0;
}

  
int NurbsMapping:: 
transformKnots(const real & uScale, 
	       const real & uShift,
	       const real & vScale /* =1. */ , 
	       const real & vShift /* =0. */ )
//=====================================================================================
/// \brief  
///     Apply a scaling and shift to the to the knots: uScale*uKnots+uShift.
///  The scale factors should be positive.
/// 
/// \param uScale,uShift (input):  scaling and shift for the knots in the u direction.
/// \param vScale,vShift (input): scaling and shift for the knots in the v direction.
///      (for domainDimension==2)
/// 
//=====================================================================================
{

  uKnot= uKnot*uScale+uShift;

  const real eps=REAL_EPSILON*2.;
  if( getIsPeriodic(axis1) )
  {
    // the curve will remain periodic if ua=0, ub=1. OR ua=1. ub=0.
    real ua=uKnot(0), ub=uKnot(m1);
    if( !( (fabs(ua)<eps && fabs(ub-1.)<eps ) || (fabs(ub)<eps && fabs(ua-1.)<eps)) )
    {
      setIsPeriodic(axis1,notPeriodic); // ** fix this *** must be able to reset
      nurbsIsPeriodic[axis1]=(int)notPeriodic;
    }
  }

  if( domainDimension>1 )
  {
    
    vKnot= vKnot*vScale+vShift;
    if( getIsPeriodic(axis2) )
    {
      real va=vKnot(0), vb=vKnot(m2);
      if( !((fabs(va)<eps && fabs(vb-1.)<eps ) || (fabs(vb)<eps && fabs(va-1.)<eps)) )
      {
	printf("NurbsMapping::reparameterization: Is no longer periodic in v.");
	setIsPeriodic(axis2,notPeriodic);
        nurbsIsPeriodic[axis2]=(int)notPeriodic;
	
      }
    }


  }
  
  reinitialize();
  initialize();

  return 0;
}

/* ------
void NurbsMapping:: 
redistribute( real arcLengthWeight, real curvatureWeight )
//=====================================================================================
// /Purpose: Change the parameterization to equidistribute some weighted
//    combination of the arc-length and the curvature.
//=====================================================================================
{

  if( domainDimension==1 )
  {
    // first evaluate the NURBS curve
    map(r,x);

    realArray weight(n1+1);
    weight(R)=arcLengthWeight*arc(R)+curvatureWeight*curvature(R);
    equidistribute( r,weight );
    
    map(r,x);  // re-evaluate at the new points

    interpolate( x );
  }

}
------ */

int NurbsMapping::
binomial(const int m, const int n)
// return the binomial coefficients  m!/( (m-n)! n! )
{
  if( m<=n )
    return 1;
  int i;
  // first compute m!/(m-n)! = m(m-1)...(m-n+1) : note this is 1 if m==n==0
  real bin=1;
  int mm=m, nn=n;
  for( i=0; i<n; i++ )
  {
    bin*=mm/real(nn);  // compute this way to avoid integer overflows.
    mm--;
    nn--;
  }
  return int(bin+.5);
}



int NurbsMapping::
elevateDegree(const int increment)
//=====================================================================================
/// \brief  
///     Elevate the degree of the nurbs.
/// \param increment (input): increase the degree of the nurb by this amount >=0 
/// \return  0 : success, 1 : failure
//=====================================================================================
{
  if( increment==0 )
    return 0;
  if( increment<0 || increment>1000 )
  {
    printf(" NurbsMapping::elevateDegree:ERROR: attempting to elevate degree by %i \n",increment);
    return 1;
  }

  int &p=p1, 
      &m=m1, 
      &n=n1;
  
  Range R(0,n1);


  const int ph=p+increment;
  const int ph2=ph/2;
  RealArray bezalfs(ph+1,p+1);  // Bezier degree elevation coeff's
  bezalfs(0,0)=1.; bezalfs(ph,p)=1.;
  int i,j,k,mpi;
  // precompute binomial coeff's
  IntegerArray bin(ph+1,ph+1); bin=1;
  for( i=0; i<=ph; i++ )
    for( j=0; j<=i; j++ )
      bin(i,j)=binomial(i,j);
      
  // ::display(bin,"binomial coefficients");  
  for( i=1; i<=ph2; i++ )
  {
    real inv=1./bin(ph,i);
    mpi=min(p,i);
    for( j=max(0,i-increment); j<=mpi; j++ )
      bezalfs(i,j)=inv*bin(p,j)*bin(increment,i-j);
  }
  for( i=ph2+1; i<=ph-1; i++ )
  {
    mpi=min(p,i);
    for( j=max(0,i-increment); j<=mpi; j++ )
      bezalfs(i,j)=bezalfs(ph-i,p-j);
  }
  // ::display(bezalfs," bezalfs");
  
  int mh=ph, kind=ph+1, r=-1, aa=p, b=p+1, cind=1;
  real ua=uKnot(0);
  Range Rx(0,rangeDimension);
  RealArray qw(Range((m+1)*(increment+2)),Rx);  // new control points, we don't yet know how many
  RealArray uh(Range((m+1)*(increment+2)));     // new knots
  
  RealArray alfs(p-1); 
  Range P(0,p);
  RealArray bpts(P,Rx), nextBpts(P,Rx), ebpts(Range(p+increment+1),Rx);
  

  qw(0,Rx)=cPoint(0,Rx);
  uh(Range(0,ph))=ua;
  bpts(P,Rx)=cPoint(P,Rx);  // initialize first bezier segement
  
  while( b<m )
  {
    i=b;
    while( b<m && uKnot(b)==uKnot(b+1) )
      b++;
    int mul=b-i+1;
    mh+=mul+increment;
    real ub=uKnot(b);
    int oldr=r;  
    r=p-mul;
    // insert knot u(b) r times 
    int lbz= oldr>0 ? (oldr+2)/2 : 1;
    int rbz = r>0 ? ph-(r+1)/2 : ph;
    if( r>0 )
    {
      // printf(" 222 insert knot r=%i \n");
      // insert the knot to get the Bezier segment
      real numer=ub-ua;
      for( k=p; k>mul; k-- )
        alfs(k-mul-1)=numer/(uKnot(aa+k)-ua);
      for( j=1; j<=r; j++ )
      {
	int save=r-j, s=mul+j;
	for( k=p; k>=s; k-- )
	  bpts(k,Rx)=alfs(k-s)*bpts(k,Rx)+(1.-alfs(k-s))*bpts(k-1,Rx);
        nextBpts(save,Rx)=bpts(p,Rx);
      }
    }
    for( i=lbz; i<=ph; i++ )
    {
      // degree elevate Bezier
      // printf(" 333 : elevate Bezier **i=%i**, b=%i, r=%i, mul=%i, lbz=%i, ph=%i\n",i,b,r,mul,lbz,ph);
      ebpts(i,Rx)=0.;
      mpi=min(p,i);
      for( j=max(0,i-increment); j<=mpi; j++ )
      {
	// printf(" use bezalf(%i,%i)=%e, bpts=(%6.2e,%6.2e) \n",i,j,bezalfs(i,j),bpts(j,0),bpts(j,1));
        ebpts(i,Rx)+=bezalfs(i,j)*bpts(j,Rx);
      }
    }
    if( oldr>1 )
    {
      // printf(" 5555 remove knots \n");
      // must remove knot u=U[aa] oldr times.
      int first=kind-2, last=kind;
      real den=ub-ua;
      real bet=(ub-uh(kind-1))/den;
      for( int tr=1; tr<oldr; tr++ )
      {
	// knot removal loop
        i=first;  j=last;
	int kj=j-kind+1;
	while( j-i > tr )
	{
	  // compute the control points for one removal step.
          if( i<cind )
	  {
	    real alf=(ub-uh(i))/(ua-uh(i));
            // printf(" **** alf=%e \n",alf);
	    qw(i,Rx)=alf*qw(i,Rx)+(1.-alf)*qw(i-1,Rx);
	  }
	  if( j>=lbz )
	  {
	    if( j-tr<=kind-ph+oldr )
	    {
	      real gam=(ub-uh(j-tr))/den;
	      ebpts(kj,Rx)=gam*ebpts(kj,Rx)+(1.-gam)*ebpts(kj+1,Rx);
	    }
	    else
	    {
	      ebpts(kj,Rx)=bet*ebpts(kj,Rx)+(1.-bet)*ebpts(kj+1,Rx);
	    }
	  }
	  i++; j--;  kj--;
	}
	first--; last++;
      }
    }
    if( aa!=p )
    { // load the knot ua
      for( i=0; i<ph-oldr; i++ )
      {
	uh(kind)=ua;
	kind++;
      }
    }
    for( j=lbz; j<=rbz; j++ )
    { // load control pts into Qw
      // printf("load the control pts into Qw, lbz=%i, rbz=%i, ebpts(j)=(%6.2e,%6.2e) \n",lbz,rbz,ebpts(j,0),ebpts(j,1));
      qw(cind,Rx)=ebpts(j,Rx);
      cind++;
    }
    if( b<m )
    {
      if( r>0 )
      {
        bpts(Range(0,r-1),Rx)=nextBpts(Range(0,r-1),Rx);
        bpts(Range(r,p),Rx)=cPoint(Range(r,p)+b-p,Rx);
      }
      else
      {
        // printf(" WARNING r=%i, b=%i, p=%i \n",r,b,p);
        // *wdh* this fix for when r<0 seems to work but I am not sure if it is really correct.
        for( j=r; j<0; j++ )
	{
          qw(cind,Rx)=ebpts(rbz,Rx);
          cind++;
	}
        bpts(Range(0,p),Rx)=cPoint(Range(0,p)+b-p,Rx);  // *wdh* Is this correct??
      }
      aa=b;  b++;
      ua=ub;
    }
    else
    {
      // end knot
      uh(Range(kind,kind+ph))=ub;
    }
  } // end while( b<m )
  int nh=mh-ph-1;
  
  m=mh;
  n=nh;
  p=ph;
  // now replace the existing NURBS
  // ::display(uKnot,"Here are the old knots before degree elevation");
  // ::display(cPoint,"Here are the old control points before degree elevation");

  uKnot.redim(m+1);
  uKnot(Range(0,m))=uh(Range(0,m));
  
  R=Range(0,n);
  cPoint.redim(R,Rx);
  cPoint(R,Rx)=qw(R,Rx);

  // ::display(uKnot,"Here are the new knots AFTER degree elevation");
  // ::display(cPoint,"Here are the new control points AFTER degree elevation");
  
  initialize();
  // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse
  reinitialize(); 

  return 0;
}


// include "PlotStuff.h"

int NurbsMapping::
merge(NurbsMapping & nurbs, bool keepFailed /* = true */, real eps /*=-1*/, bool attemptPeriodic /*=true*/ )
//=====================================================================================
/// \brief  Try to merge "this" nurbs with the input nurbs. This routine will merge
///   the two NURBS's into one if the endpoint of one matches the end point of the second.
/// \param nurbs (input): nurbs to merge with
/// \return  0 : success, 1 : failure
//=====================================================================================
{
  if( domainDimension!=nurbs.getDomainDimension() || rangeDimension!=nurbs.getRangeDimension() )
  {
    cout<<domainDimension<<" "<<nurbs.getDomainDimension()<<" "<<rangeDimension<<" "<<nurbs.getRangeDimension()<<endl;
    cout << "NurbsMapping::merge: nurbs' cannot be merged, dimensions incompatible \n";
    return 1;
  }
  // Mapping::debug=7;

  // We keep track of all the subCurves in the list subCurves[]
  // First build a new list of the correct size then reset the subCurves pointer.
  
  // ***** wdh ****** We make a copy of nurbs, why don't we just change this copy and leave
  //                  nurbs alone??? ********************************************************************
  //   also copy will not have the degree raised ??

  real time0=getCPU();
  int newcurveIndex = addSubCurve(nurbs);
  timeToMergeNurbsAddSubCurve+=getCPU()-time0;
  
  real time2=getCPU();
  if( p1!=nurbs.p1 )
  {
    if( Mapping::debug & 4 )
      printf("NurbsMapping::merge: nurbs orders are not the same, p1=%i, nurbs.p1=%i, elevating... \n",
             p1,nurbs.p1);

    if( false )
    {
      ::display(nurbs.uKnot,"nurbs.uKnot");
      ::display(nurbs.cPoint,"nurbs.cPoint");
      ::display(uKnot, "uKnot");
      ::display(cPoint,"cPoint");
    }
    
    if( nurbs.p1>p1 )
    {
      elevateDegree( nurbs.p1-p1 );  // increase the degree of this nurb to match the other
      // ::display(uKnot,"uKnot AFTER");
      // ::display(cPoint,"cPoint AFTER");
    }
    else
    {
      // ::display(nurbs.uKnot,"nurbs.uKnot BEFORE");
      // ::display(nurbs.cPoint,"nurbs.cPoint BEFORE");
      nurbs.elevateDegree( p1-nurbs.p1 );
      // ::display(nurbs.uKnot,"nurbs.uKnot AFTER");
      // ::display(nurbs.cPoint,"nurbs.cPoint AFTER");
    }
    
  }
  timeToMergeNurbsElevateDegree+=getCPU()-time2;
  time2=getCPU();
  

  enum
  {
    toEnd=0,
    toStart,
    toNeither
  } addNurb=toNeither;
  
//  real eps= FLT_EPSILON*100.; // relative tolerance for matching end points *** use single precision tolerance ***
  // *kkc* now an optional argument real eps = 0.001; // relative tolerance for matching end points
                    // this will become one thousandth of the maximum distance between
                    // the ends of a curve

  bool nurbIsPeriodic=false;
  
  if( domainDimension==1 )
  {
    // check if the end points match -- decide which way to merge
    RealArray r(1,1), 
      x10(1,rangeDimension), x11(1,rangeDimension), 
      x20(1,rangeDimension), x21(1,rangeDimension);

    r=0.;    mapS(r,x10);  nurbs.mapS(r,x20);
    r=1.;    mapS(r,x11);  nurbs.mapS(r,x21);

    //const real tol = eps*max(fabs(x20)+fabs(x11)+fabs(x21)+fabs(x10));
    real tol = 0.001*max(fabs(x21-x20)+fabs(x11-x10));
    if ( eps<0 )
      tol = 0.001*max(fabs(x21-x20)+fabs(x11-x10));
    else
      tol = eps;

    real endDist=max(fabs(x20-x11));
    real startDist=max(fabs(x21-x10));
    real endDistRev   =sum(fabs(x21-x11)); // *kkc added curve reversal check to merge
    real startDistRev =sum(fabs(x20-x10));
    bool reversed = false;

// need to check periodicity before anything is added to startDist or endDist
    if( (endDist <= tol && startDist<=tol) || (endDistRev<=tol && startDistRev<=tol) )
      nurbIsPeriodic=true; // we cannot set isPeriodic here before we compute arclengths.

     // could be trouble if both end match very closely -- should prefer add toEnd

    // *kkc changed to FLT_EPSILON to make the result independent of single/double
    //                 const real small = REAL_EPSILON*100.;
    const real small = FLT_EPSILON*100.;
    if( endDist<small && startDist<small )
    {
      startDist=endDist+small;
    }
    if( endDistRev<small && startDistRev<small )
    {
      startDistRev=endDistRev+small;
    }
    
//     printf("NurbsMapping::merge: startDist=%8.2e, endDist=%8.2e, startDistRev=%8.2e, endDistRev=%8.2e small=%8.2e\n",
// 	   startDist,endDist,startDistRev,endDistRev,small);
    

    if( endDist<=tol && endDist<=min(startDist,startDistRev,endDistRev) )
    {
      addNurb=toEnd;
    }
    else if( startDist<=tol && startDist<=min(endDist,startDistRev,endDistRev)  )
    {
      addNurb=toStart;
    }
    else if( endDistRev<=tol && endDistRev<=min(startDist,endDist,startDistRev) )
    {
      addNurb = toEnd;
      nurbs.reparameterize(1.,0.);
      subCurve(newcurveIndex).reparameterize(1.,0.);
      reversed = true;
    }
    else if( startDistRev<=tol && startDistRev<=min(startDist,endDist,endDistRev) )
    {
      addNurb = toStart;
      nurbs.reparameterize(1.,0.);
      subCurve(newcurveIndex).reparameterize(1.,0.);
      reversed = true;
    }

    if( addNurb==toNeither )
    {
      if ( Mapping::debug & 2 )
	printf("**** NurbsMapping::merge:end points don't match !! startDist=%e, endDist=%e, tol=%e********\n",
	       startDist,endDist,tol);
#if 0
      ::display(x10,"Start of curve i-1","%e ");
      ::display(x11,"End   of curve i-1","%e ");
      ::display(x20,"Start of curve i  ","%e ");
      ::display(x21,"End   of curve i  ","%e ");
#endif

      if ( !keepFailed ) 
	deleteSubCurve(newcurveIndex);
	
      return 1;
    }
    
    
    // *** If the knots extend outside [0,1] we need to move the last knot to 0 or 1
    //     so that we can merge the two nurbs. We do this by splitting the nurbs into two pieces
    //     and using one of the pieces. *wdh* 010419

    if( true )
    {
      const real epsSplit=1000.*REAL_EPSILON;
      if( addNurb==toEnd )
      {
	real uMax0=uKnot(m1);
	real uMin1=nurbs.uKnot(0);
	if( uMax0>1.+epsSplit )
	{
	  // split this at 1
	  // printf("NurbsMapping::merge: split this at 1\n");
//           printf("**BEFORE split\n");
// 	  ::display(uKnot,"uKnot");
// 	  ::display(cPoint,"cPoint");
	  
	  NurbsMapping nurbLeft,nurbRight;
//           PlotStuff & gi = *Overture::getGraphicsInterface();
//           gi.erase();
// 	  PlotIt::plot(gi,*this);
	  
	  this->split(1.,nurbLeft,nurbRight);
	  // Copy relevant data (do not use the '=' operator as this would destroy the sub-curves)
	  m1=nurbLeft.m1;
	  n1=nurbLeft.n1;
	  uKnot.redim(0);
	  uKnot=nurbLeft.uKnot;  
	  cPoint.redim(0);
	  cPoint=nurbLeft.cPoint;

	  mappingHasChanged();
	  
	}
	if( uMin1<-epsSplit )
	{
	  // split nurbs at 0.
	  // printf("NurbsMapping::merge: split nurbs at 0\n");
	  NurbsMapping nurbLeft,nurbRight;
	  nurbs.split(0.,nurbLeft,nurbRight);
	  nurbs=nurbRight;
	}
      }
      else if( addNurb==toStart )
      {
	real uMin0=uKnot(0);
	real uMax1=nurbs.uKnot(nurbs.m1);
	if( uMin0<-epsSplit )
	{
	  // split this at 0
	  // printf("NurbsMapping::merge: split this at 0\n");
	  NurbsMapping nurbLeft,nurbRight;
	  this->split(0.,nurbLeft,nurbRight);
	  // Copy relevant data (do not use the '=' operator as this would destroy the sub-curves)
	  m1=nurbRight.m1;
	  n1=nurbRight.n1;
	  uKnot.redim(0);
	  uKnot=nurbRight.uKnot; 
	  cPoint.redim(0);
	  cPoint=nurbRight.cPoint;
	  mappingHasChanged();

	}
	if( uMax1>1.+epsSplit )
	{
	  // split nurbs at 1.
	  // printf("NurbsMapping::merge: split nurbs at 1\n");
	  NurbsMapping nurbLeft,nurbRight;
	  nurbs.split(1.,nurbLeft,nurbRight);
	  nurbs=nurbLeft;
	}
      }
      
      // ::display(uKnot,"After split, uKnot");
      // ::display(nurbs.uKnot,"After split, nurbs.uKnot");
      
      
    } // end if (true)
    
    
    // compute the arclengths of each curve so we can scale the knots in a
    // reasonable way
    timeToMergeNurbsOther+=getCPU()-time2;
  

    real arc[2];
    if( false )
    {
      Mapping *map;
      for( int c=0; c<=1; c++ )
      {
	map = c==0 ? this : &nurbs;
	int n= map->getGridDimensions(axis1);
	real h=1./(n-1.);
	realArray r(n,1), x(n,rangeDimension);

	r.seqAdd(0.,h);
	map->map(r,x);
	arc[c]=0.;
	Range I(1,n-1);
	if( rangeDimension==1 )
	{
	  // for( i=1; i<n; i++ )
	  //   arc[c]+=fabs(x(i,0)-x(i-1,0));
	  arc[c]=sum(fabs(x(I,0)-x(I-1,0)));
	}
	else if( rangeDimension==2 )
	{
	  // for( i=1; i<n; i++ )
	  //  arc[c]+=SQRT( SQR( x(i,0)-x(i-1,0) ) + SQR( x(i,1)-x(i-1,1) ) );
	  arc[c]=sum( SQRT( SQR( x(I,0)-x(I-1,0) ) + SQR( x(I,1)-x(I-1,1) ) ));
	}
	else
	{
	  // for( i=1; i<n; i++ )
	  //  arc[c]+=SQRT( SQR( x(i,0)-x(i-1,0) ) + SQR( x(i,1)-x(i-1,1) ) +  SQR( x(i,2)-x(i-1,2) ) );
	  arc[c]=sum(SQRT( SQR( x(I,0)-x(I-1,0) ) + SQR( x(I,1)-x(I-1,1) ) +  SQR( x(I,2)-x(I-1,2) ) ));
	}
	// printf(" ******* curve c=%i (0=this, 1=nurbs) arc=%e ******* \n",c,arc[c]);
	// ::display(x,"Here are the curve coordinates");
      }
      if( true || Mapping::debug & 4 )
	printf("merge: arcLength[this] = %e(new=%e), arcLength[nurbs]=%e(new=%e) \n",arc[0],
	       getArcLength(),arc[1],nurbs.getArcLength());
    }
    else
    {
      real time1=getCPU();
      
      arc[0]=getArcLength();
      arc[1]=nurbs.getArcLength();
      
      timeToMergeNurbsArcLength+=getCPU()-time1;
    }
    

    if( arc[0]==0. || arc[1]==0. )
      printf("merge:ERROR? a zero arclength, arcLength[this] = %e, arcLength[nurbs]=%e \n",arc[0],arc[1]);
      
    real arcRatio = arc[1]!=0. ? arc[0]/arc[1] : 1.;
    
    uKnot*=arcRatio; // scale these

    Range Rw(0,rangeDimension);
    // first tack on the second knots and control points to the first
    int n1New=n1+nurbs.n1+1;
    int m1New=n1New+p1+1;

    uKnot.resize(m1New+1);

    cPoint.resize(n1New+1,rangeDimension+1);   
    real ub=1.;  // for normalizing knots
    if( addNurb==toEnd )
    {
      // printf("NurbsMapping::merge: addNurb==toEnd\n");
      
      ub=uKnot(m1)+1.;
      
      Range R(nurbs.p1+1,nurbs.m1);     // throw away first p1+1 knots
      uKnot(R+m1+1-p1-1)=nurbs.uKnot(R)+uKnot(m1);  // shift nurbs.uKnot to match end of uKnot
      //Range R2(m1, m1+nurbs.m1-p1-1);
      //uKnot(R+m1-1-p1-1)=nurbs.uKnot(R)+uKnot(m1);  // shift nurbs.uKnot to match end of uKnot
      //real offset = uKnot(m1);
      //uKnot(R2)=nurbs.uKnot(R)+offset;  // shift nurbs.uKnot to match end of uKnot
      R=Range(0,nurbs.n1);
      cPoint(R+n1+1,Rw)=nurbs.cPoint(R,Rw);
      //cPoint(R+n1,Rw)=nurbs.cPoint(R,Rw);
      
    }
    else
    {
      ub=nurbs.uKnot(nurbs.m1)+1.*arcRatio;

      // printf("NurbsMapping::merge: addNurb==toStart\n");
      // reorder the list of subCurves to the latest one is first
      NurbsMapping *tempNurb=subCurves[newcurveIndex];  // this one we just added.
      char tstate = subCurveState[newcurveIndex];
      for( int n=newcurveIndex; n>0; n-- )
      {
	subCurves[n]=subCurves[n-1];
	subCurveState[n] = subCurveState[n-1];
      }
      
      subCurves[0]=tempNurb;
      subCurveState[0] = tstate;
      newcurveIndex = 0;
      
      Range R1(p1+1,m1), R2(0,nurbs.m1);
      uKnot(R1+nurbs.m1+1-p1-1)=uKnot(R1)+nurbs.uKnot(nurbs.m1);
      uKnot(R2)=nurbs.uKnot(R2);
      R1=Range(0,n1); R2=Range(0,nurbs.n1);
      cPoint(R1+nurbs.n1+1,Rw)=cPoint(R1,Rw);
      cPoint(R2,Rw)=nurbs.cPoint(R2,Rw);
    }
    if( Mapping::debug & 4 )
    {
      uKnot.display("merge: uKnot:");
      cPoint.display("merge: cPoint:");
    }
 
     
    int m1Old=m1, p1Old=p1;

    n1=n1New; // assign new values
    m1=m1New;

    uMin=uKnot(0);  // min(uKnot); *wdh* 010829
    uMax=uKnot(m1); // max(uKnot);

    // now try and remove duplicate knots	
    if( true )
    {
      int numberRemoved=0;
      if( addNurb==toEnd )
      {
	removeKnot( m1Old,p1Old,numberRemoved, tol );            // only 1 should be removeable (?)
        if( Mapping::debug & 4 )
	  cout << "NurbsMapping::merge: number of common knots removed = " << numberRemoved << endl;
      }
      if( addNurb==toStart )
      {
	removeKnot( nurbs.m1,nurbs.p1,numberRemoved,tol );       // only 1 should be removeable
        if( Mapping::debug & 4 )
  	  cout << "NurbsMapping::merge: number of common knots removed = " << numberRemoved << endl;
      }
    }
    
    if( Mapping::debug & 4 )
    {
      printf("n1=%i, m1=%i, p1=%i \n",n1,m1,p1);
      uKnot.display("merge: after removeKnot uKnot:");
      cPoint.display("merge:  after removeKnot cPoint:");
    }
    
    setGridDimensions( axis1,getGridDimensions(axis1)+nurbs.getGridDimensions(axis1) );

    uKnot.resize(m1+1);
    cPoint.resize(n1+1, rangeDimension+1);

    // normalizeKnots();  // *wdh* 010418
    if( ub>REAL_EPSILON*10. )
      uKnot*=1./ub;         // rescale so ub-> 1. 
    else
    {
      printf("merge: ERROR: attempting to scale knots by ub=%e. This should be >=1 !\n",ub);
    }
    
    // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse

    
    if (! isSubCurveOriginal(newcurveIndex) )
    {
       //   printf("merge: Making subcurve %i with globalID=%i original\n", 
      //                   newcurveIndex, subCurve(newcurveIndex).getGlobalID());
      toggleSubCurveOriginal(newcurveIndex);
    }

    // *kkc* make the new curve the same as the altered nurbs (perhaps only alter the subcurve?)
    subCurve(newcurveIndex) = nurbs;

    reinitialize(); 
    mappingHasChanged();
    initialize();
    setArcLength(arc[0]+arc[1]); 

    if( nurbIsPeriodic && attemptPeriodic )
    {
      if( Mapping::debug & 4 )
	cout << "============NurbsMapping: merged nurbs is periodic =============\n";

      
      // forcePeriodic();  // *wdh* 010427 why do we need to force??
      //                      *kkc* because tol may be too large and the real end 
      //                            control points may specify a self-intersecting nurb

      RealArray endpt(1,rangeDimension);
      if ( addNurb==toEnd )
      {
	for( int dir=0; dir<rangeDimension; dir++ )
  	  endpt(0,dir) = x10(0,dir);
      }
      else
      {
	for( int dir=0; dir<rangeDimension; dir++ )
	  endpt(0,dir) = reversed ? x21(0,dir) : x20(0,dir);
      }
      
      subCurve(numberOfSubCurves()-1).moveEndpoint(End,endpt);

      moveEndpoint(End,endpt);

      setIsPeriodic(axis1,functionPeriodic);
      
    } else {
      setIsPeriodic(axis1,notPeriodic);
    }

    reinitialize(); 
    mappingHasChanged();
    initialize();
    
    setArcLength(arc[0]+arc[1]);   // do this after initialize since initialize will invalidate the arcLength

    // turn on the original marker we got this far
    if (! isSubCurveOriginal(newcurveIndex) )
    {
       //   printf("merge: Making subcurve %i with globalID=%i original\n", 
      //                   newcurveIndex, subCurve(newcurveIndex).getGlobalID());
      toggleSubCurveOriginal(newcurveIndex);
    }
    
  } // end if domainDimension == 1
  

/* ----
  int multiplicity = 0;
  real kprev = uKnot(0);
  cout<<"multiplicity begin : m1+1 = "<<m1+1<<endl;
  cout<<"                     n1+1 = "<<n1+1<<endl;
  for ( int k=0; k<m1+1; k++ )
    if ( uKnot(k) == kprev )
      {
	multiplicity++;
      }
    else
      {
	cout <<"multiplicity "<<multiplicity<<endl;
	multiplicity = 1;
	kprev = uKnot(k);
      }
  cout<<"multiplicity "<<multiplicity<<endl;
  cout<<"multiplicity end"<<endl;
  uKnot.display("uKnot");
  cPoint.display("cPoint");
---- */

  timeToMergeNurbs+=getCPU()-time0;
  return 0;
}       

void 
createLineAsNurbs( NurbsMapping *nurb, const RealArray &p1, const RealArray &p2 )
{
  // code taken from createCompositeCurve
  
  real x0=p1(0,0), y0=p1(0,1), z0= p1.getLength(1)==3 ? p1(0,2) : 0.0 ;
  real x1=p2(0,0), y1=p2(0,1), z1= p2.getLength(1)==3 ? p2(0,2) : 0.0 ;
  
  // make a nurbs for a line
  int p=1;
  int n=1;
  int m=n+p+1;
  RealArray knot(m+1), cp(n+1,p1.getLength(1)+1);
#if 0
  knot(0)=0.; knot(1)=0.; knot(2)=0.; knot(3)=0.;
  knot(4)=1.; knot(5)=1.; knot(6)=1.; knot(7)=1.;
  
  cp(0,0)=x0; cp(0,1)=y0;  
  cp(1,0)=x0; cp(1,1)=y0;  
  cp(2,0)=x1; cp(2,1)=y1;  
  cp(3,0)=x1; cp(3,1)=y1;  
#endif
  knot(0) = knot(1) = 0.;
  knot(2) = knot(3) = 1.;

  cp(0,0) = x0; cp(0,1)=y0;  
  cp(1,0)=x1; cp(1,1)=y1;  

  if ( p1.getLength(1) == 2 )
    {
      cp(0,2) = 1.;
      cp(1,2) = 1.;
#if 0
      cp(2,2) = 1.;
      cp(3,2) = 1.;
#endif
    } else {
      
      cp(0,2) = z0; cp(0,3)=1.;
      cp(1,2) = z1; cp(1,3)=1.;
#if 0
      cp(2,2) = z1; cp(2,3)=1.;
      cp(3,2) = z1; cp(3,3)=1.;
#endif
      
    }

  nurb->specify( m,n,p,knot,cp,p1.getLength(1));
}

int 
NurbsMapping::
line( const RealArray &p1_, const RealArray &p2_ )
{
  createLineAsNurbs(this, p1_, p2_);
  return 0;
}

int NurbsMapping::
forcedMerge(NurbsMapping & nurbs  )
//=====================================================================================
/// \brief  Force a merge of "this" nurbs with the input nurbs. This routine will merge
///   the two NURBS's into one if the endpoint of one matches the end point of the second. If
///   the endpoints do not match, a straight line section is added between the closest end
///   points.
/// \param nurbs (input): nurbs to merge with
/// \return  0 : success, 1 : failure
//=====================================================================================
{
  int simpleMerge = merge(nurbs);
  
  if ( simpleMerge == 0 ) return 0;  // a simple merge worked, return success

  int numberOfIntersectionPoints=0;

  // ok, MAKE this work if the curves are compatible

  enum
  {
    toEnd=0,
    toStart,
    toNeither
  } addNurb=toNeither;
  
  //real eps= FLT_EPSILON*100.; // relative tolerance for matching end points *** use single precision tolerance ***
  real eps = 0.001; 

  bool nurbIsPeriodic=false;
  int status;

  if( domainDimension==1 )
  {
    NurbsMapping *newPatch = new NurbsMapping; newPatch->incrementReferenceCount();

    // check if the end points match -- decide which way to merge
    RealArray r(1,1), 
      x10(1,rangeDimension), x11(1,rangeDimension), 
      x20(1,rangeDimension), x21(1,rangeDimension);
    r=0.;    mapS(r,x10);  nurbs.mapS(r,x20);
    r=1.;    mapS(r,x11);  nurbs.mapS(r,x21);
    //const real tol = eps*max(fabs(x20)+fabs(x11)+fabs(x21)+fabs(x10));
    //    const real tol = eps*max(max(fabs(x21-x20)),max(fabs(x11-x10)));
    const real tol = eps*max(fabs(x21-x20)+fabs(x11-x10));
    const real endDist      =sum(fabs(x20-x11)); // *wdh* change max to sum so we find closest.
    const real startDist    =sum(fabs(x21-x10));
    const real endDistRev   =sum(fabs(x21-x11));
    const real startDistRev =sum(fabs(x20-x10));

    if( endDist<=startDist || endDistRev<startDistRev )
      // could be trouble if both end match very closely -- should prefer add toEnd
    {
      addNurb=toEnd;
      // if( endDist<endDistRev ) // *wdh* really force a merge   ( endDist<(10.*tol) )
      if( endDist<(10.*tol) )
	createLineAsNurbs(newPatch, x11, x20);
      // else if( true ) // *wdh* 
      else if( endDistRev<(10.*tol) )
      {
	nurbs.reparameterize(1.,0.);
	return merge(nurbs) ;
      }
      else
      {
	cout<<"Could not force a connection between two nurbs curve, gap is too large "<<endl;
	if ( newPatch->decrementReferenceCount() ==0 ) delete newPatch;
	return 1;
      }
      
    }
    else 
    {
      addNurb=toStart;
      // if( startDist < startDistRev ) // *wdh*  ( startDist<(10.*tol) )
      if( startDist<(10.*tol) )
	createLineAsNurbs(newPatch, x21, x10);
      // else if( true ) // *wdh* if ( startDistRev<(10.*tol) )
      else if( startDistRev<(10.*tol) )
      {
	nurbs.reparameterize(1.,0.);
	return merge(nurbs);
      }
      else
      {
	cout<<"Could not force a connection between two nurbs curve, gap is too large "<<endl;
	if ( newPatch->decrementReferenceCount() ==0 ) delete newPatch;
	return 1;
      }
    }

    if ( merge(*newPatch)==1 )  // this *HAS* to work
    {
      cout<<"NurbsMapping::forcedMerge FAILED when adding patch segment !"<<endl;
      if ( newPatch->decrementReferenceCount() ==0 ) delete newPatch;
      return 1;
    }

    if ( merge(nurbs)==1 )
    {
      cout<<"NurbsMapping::forcedMerge FAILED !"<<endl;
      return 1;
    }

  }

  return 0;

}       

int NurbsMapping::
forcePeriodic()
//=====================================================================================
/// \brief  force this mapping to be periodic by making the last control points the
///  same as the first ( if the knots are "clamped", eg the knots are 0 0 0 0 ... 1 1 1 1 )
/// \return  0 : success, 1 : failure
//=====================================================================================
{
  // first count the multiplicity at the beginning and the end, we will only 
  //   force periodicity if the knot multiplicities are the same (ie the beginning and
  //   end are probably clamped, so make them depend on the same control points)

  if ( domainDimension==1 )
    {
      Range AXES(rangeDimension);
      int multBegin=1, multEnd=1;
      real oldBegin, oldEnd;
      oldBegin = uKnot(0);
      oldEnd   = uKnot(m1);
      real currentUBegin = oldBegin;
      real currentUEnd = oldEnd;
      
      while ( currentUBegin==oldBegin || currentUEnd==oldEnd )
	{
	  if ( currentUBegin == oldBegin ) 
	    {
	      currentUBegin = uKnot(multBegin);
	      multBegin++;
	    }

	  if ( currentUEnd == oldEnd )
	    {
	      currentUEnd = uKnot(m1-(multEnd));
	      multEnd++;
	    }
	}

      if ( multBegin!=multEnd ) return 1;

      RealArray pt(rangeDimension);
      for ( int aa=0; aa<rangeDimension; aa++ )
	pt(aa) = cPoint(0,aa);

      if( moveEndpoint(End, pt)!=0 )  // *wdh* 010427 check for error return
      {
	printf("NurbsMapping::forcePeriodic: **unable to moveEndpoint\n");
        return 1;
      }
      //for ( int ax=0; ax<rangeDimension+1; ax++ )
      //	cPoint(n1, ax) = cPoint(0,ax);
    }
  else
    return 1;

  setIsPeriodic(axis1,functionPeriodic); 

  reinitialize();
  initialize();

  return 0;
}

int NurbsMapping::
split(real uSplit, NurbsMapping &c1, NurbsMapping&c2, bool normalizePieces /* =true */ )
// =============================================================================================
/// \details 
///   Split a nurb curve into two pieces.
/// \param uSplit (input) : parameter value to split the curve at
/// \param c1 (output) : curve on the "left", parameter bounds [0,uSplit]
/// \param c2 (output) : curve on the "right", parameter bounds [uSplit,1]
/// \param normalizePieces (input) : in true then normalize each piece
/// \param Returns : 0 on success, 1 on failure ( uSplit<0 or uSplit>1 )
// =============================================================================================
{
  assert( domainDimension == 1 );
  const real knotEpsilon = REAL_EPSILON*10.; // ( note this matches the knotEpsilon in TrimmedMapping::triangulate! )

  if ( uSplit<=uKnot(0) || uSplit>=uKnot(uKnot.getBound(0)) )
  {
    cout<<" NurbsMapping::split::ERROR: Cannot split a curve beyond its endpoints, uSplit, uBase, uBound "<<
      uSplit<<" "<<uKnot(0)<<" "<<uKnot(uKnot.getBound(0))<<endl;
    return 1;
  }

  if( Mapping::debug & 2 )
    printf("Entering split, uSplit=%e, uKnot(0)=%e, uKnot(N)=%e\n", uSplit, uKnot(0), uKnot(uKnot.getBound(0)));

  int p = p1;

  NurbsMapping nurbTemp;
  nurbTemp = *this;


  int k;
//    // *wdh* 031123 : No need to insert a knot p-times if there is one already there
//    int numFound=0;
//    for( k=0; k<=m1; k++)
//    {
//      if( fabs(uKnot(k)-uSplit)<knotEpsilon )
//      {
//        numFound++;
//      }
//    }
//    if( (p-numFound)>0 )
//    {
//      nurbTemp.insertKnot(uSplit, p-numFound );
//  //   printf("After insert knot\n");
//  //   PlotStuff & gi = *Overture::getGraphicsInterface();
//  //   PlotIt::plot(gi,nurbTemp);
//    }
  
  nurbTemp.insertKnot(uSplit, p );

  int nNewKnotsLeft=nurbTemp.findSpan(nurbTemp.getNumberOfControlPoints()-1, p, uSplit, nurbTemp.uKnot) +2 ;
  if( Mapping::debug & 2 )
    printf("split:nNewKnotsLeft=%i\n",nNewKnotsLeft);

  RealArray newKnots(nNewKnotsLeft);
  const RealArray & tempKnots=nurbTemp.getKnots(0);
  const RealArray & tempControlPoints=nurbTemp.getControlPoints();
  for ( k=0; k<nNewKnotsLeft-1; k++ )
    newKnots(k) = tempKnots(k);
  
  newKnots(nNewKnotsLeft-1) = newKnots(nNewKnotsLeft-2);  // add an extra knot at right

  const int pShift=p+1;

  int nCPLeft = nNewKnotsLeft - pShift;
  RealArray newCP(nCPLeft, nurbTemp.getRangeDimension()+1);
  int a,c;
  for ( c=0; c<nCPLeft; c++ )
    for ( a=0; a<nurbTemp.getRangeDimension()+1; a++ )
      newCP(c,a) = tempControlPoints(c,a);
  
// ---
  Range R1=nCPLeft;
  for( a=0; a<rangeDimension; a++ )
    newCP(R1,a)/=newCP(R1,rangeDimension);   // un-normalize control points for the call to specify

  // we scale the knots here since ua may not be equal to 0.
  // scale to that 0->0 and uSplit->1.
//
// AP: uSplit is never changed and can exceed 1
//
  bool normalizeKnots=false; 
// AP  if( uSplit>0. )
  if( uSplit>uKnot(0) )
  {
    if( normalizePieces  )  // *wdh* 
    {
      real uScale=1./max(REAL_MIN*100.,uSplit-uKnot(0));
      for ( k=0; k<nNewKnotsLeft; k++ )
	newKnots(k)=(newKnots(k)-uKnot(0))*uScale;
    }
    if( Mapping::debug & 2 ) 
      newKnots.display("NurbsMapping::split: newKnots for left piece");
    c1.specify(nNewKnotsLeft-1, nCPLeft-1, p, newKnots, newCP, nurbTemp.getRangeDimension(), normalizeKnots);
  }
  
//   printf("Left portion\n");
//   PlotIt::plot(gi,nurbTemp);

  int nNewKnotsRight = nurbTemp.getNumberOfKnots(0)-(nNewKnotsLeft-1)+pShift;

  if( Mapping::debug & 2 )
    printf("split: p=%i numTempKnots=%i, nNewKnotsLeft=%i nNewKnotsRight=%i\n",
               p,nurbTemp.getNumberOfKnots(0),nNewKnotsLeft,nNewKnotsRight);

  newKnots.redim(nNewKnotsRight);
  int nCPRight = nNewKnotsRight - pShift;
  for ( k=1; k<nNewKnotsRight; k++ )
    newKnots(k) = tempKnots(k+(nNewKnotsLeft-1)-pShift);   // fill knots starting at 1
  
  newKnots(0) = newKnots(1);                            // add an extra knot at 0

  newCP.redim(nCPRight, nurbTemp.getRangeDimension()+1);
  for ( c=nCPLeft-1; c<nurbTemp.getNumberOfControlPoints(); c++ )
    for ( a=0; a<nurbTemp.getRangeDimension()+1; a++ )
      newCP(c-nCPLeft+1,a) = tempControlPoints(c,a);

  R1=nCPRight;
  for( a=0; a<rangeDimension; a++ )
    newCP(R1,a)/=newCP(R1,rangeDimension);  // un-normalize control points for the call to specify

  // c2 makes no sense if uSplit==1
  // AP  if( uSplit<1. )
  if( uSplit<uKnot(uKnot.getBound(0)) )
  {
    // AP    real uScale=1./max(REAL_MIN*100.,1.-uSplit);
    if( normalizePieces  )  // *wdh* 
    {
      real uScale=1./max(REAL_MIN*100.,uKnot(uKnot.getBound(0))-uSplit);
      for ( k=0; k<nNewKnotsRight; k++ )
	newKnots(k)=(newKnots(k)-uSplit)*uScale;   // rescale knots
    }
    
    if( Mapping::debug & 2 ) 
      newKnots.display("NurbsMapping::split: newKnots for right piece");
    
    c2.specify(nNewKnotsRight-1, nCPRight-1, p, newKnots, newCP, nurbTemp.getRangeDimension(), normalizeKnots);
  }

  c1.setGridDimensions(axis1,10*nCPLeft);
  c2.setGridDimensions(axis1,10*nCPRight);

  return 0;
}


// =======================================================================================================
//! Split a sub-curve at a given position
/*!
    \param subCurveNumber (input): Split this sub-curve
    \param uSplit (input): split sub-curve at this parameter value.
    \note Currently this function will destroy the original subcurve. The two new sub-curves are ordered to be
           subCurveNumber and subCurveNumber+1
         
    \return 0==success, 1==failure (rSplit out of bounds).
 */
// =======================================================================================================
int NurbsMapping::
splitSubCurve( int subCurveNumber, real rSplit )
{

  if( subCurveNumber<0 || subCurveNumber>=numberOfCurves )
  {
    printf("NurbsMapping::splitSubCurve:ERROR invalid subCurveNumber=%i, numberOfSubCurves=%i\n",
	   subCurveNumber,numberOfCurves);
  }
  
  NurbsMapping & curve = subCurve(subCurveNumber);  // this is the curve we split
  
  // two new curves :
  NurbsMapping & curve1= *new NurbsMapping; curve1.incrementReferenceCount(); 
  NurbsMapping & curve2= *new NurbsMapping; curve2.incrementReferenceCount();
  
  // split the sub-curve into two.
  bool ok = curve.split(rSplit, curve1, curve2) == 0;
  if (!ok)
  {
    if (curve1.decrementReferenceCount() == 0)
      delete &curve1;
    if (curve2.decrementReferenceCount() == 0)
      delete &curve2;
    return 1;
  }
  
    

  // delete original
  if( curve.decrementReferenceCount()==0 )
    delete &curve;   // delete the curve we split (we could in principle save this as a hidden curve).
  
  // Now add the new sub-curves
  NurbsMapping **temp = new NurbsMapping* [numberOfCurves+1];
  char * scstat = new char[numberOfCurves+1];

  int n;
  for( n=0; n<subCurveNumber; n++ )
  {
    temp[n]=subCurves[n];
    scstat[n] = subCurveState[n];
  }
  if( subCurveState==NULL ) // *wdh* added 030825
  {
    assert( numberOfCurves==1 );
    subCurveState=new char[numberOfCurves];
    subCurveState[0]=01; // is this correct? who "documented" subCurveState ?
  }

  temp[subCurveNumber]=&curve1;    scstat[subCurveNumber  ]=subCurveState[subCurveNumber];
  temp[subCurveNumber+1]=&curve2;  scstat[subCurveNumber+1]=subCurveState[subCurveNumber];
  

  for( n=subCurveNumber+2; n<=numberOfCurves; n++ )
  {
    temp[n]=subCurves[n-1];
    scstat[n] = subCurveState[n-1];
  }
  
  if ( subCurves!=NULL ) delete [] subCurves;
  if ( subCurveState!=NULL ) delete [] subCurveState;

  subCurves=temp;
  subCurveState = scstat;
  numberOfCurves++;
  if( subCurveNumber<=lastVisible )
    lastVisible++;   // we split a visible sub-curve, both new curves are visible.
  
  return 0;
}


// =======================================================================================================
//! Join two consequtive sub-curves and modify the subcurve list
/*!
    \param subCurveNumber (input): Join sub-curve subCurveNumber and subCurveNumber+1. The new sub-curve 
          will be ordered to be subCurveNumber. In the case subCurveNumber == lastVisible,
          it will joined with curve 0 and the new curve will get number 0. Also, if subCurveNumber = 
          numberOfCurves-1, the curve will get joined to curve lastVisible+1, and the new curve will get 
          number lastVisible+1
    \note Visible and invisible curves are separated. If subCurveNumber==lastVisible, the
          curve will be joined to subCurve 0 and the new curve will get number 0.
    \note Currently this function will destroy (if the reference count becomes zero) the two original 
          subcurves and make one new curve
         
    \return : success: new sub curve number, failure: -1
 */
// =======================================================================================================
int NurbsMapping::
joinSubCurves( int subCurveNumber )
{

  if( subCurveNumber<0 || subCurveNumber>=numberOfCurves )
  {
    printf("NurbsMapping::joinSubCurve:ERROR invalid subCurveNumber=%i, numberOfSubCurves=%i\n",
	   subCurveNumber, numberOfCurves);
    return -1;
  }
  int newCurveNumber=-1;
  
  NurbsMapping *curve1, *curve2;
  bool joinLastVisible=false;
  bool joinLastInvisible=false;
  
  if (subCurveNumber == lastVisible)
  {
    printf("joinSubCurves: joining the last visible with the first curve\n");
    curve1 = &subCurve(subCurveNumber);
    curve2 = &subCurve(0);
    joinLastVisible=true;
  }
  else if (subCurveNumber == numberOfCurves-1)
  {
    printf("joinSubCurves: joining the last invisible with curve lastVisible+1\n");
    curve1 = &subCurve(subCurveNumber);
    curve2 = &subCurve(lastVisible+1);
    joinLastInvisible=true;
  }
  else
  {
    printf("joinSubCurves: joining curve %i with the next curve\n", subCurveNumber);
    curve1 = &subCurve(subCurveNumber);
    curve2 = &subCurve(subCurveNumber+1);
  }
  
  NurbsMapping & curve = *new NurbsMapping(*curve1); // start out with a copy of curve1
  curve.incrementReferenceCount();
  
  curve.merge(*curve2);  // add in curve2
  
  // delete originals
  if( curve1->decrementReferenceCount()==0 )
    delete curve1;
  if( curve2->decrementReferenceCount()==0 )
    delete curve2;
  
  // Now add the new sub-curves
  NurbsMapping **temp = new NurbsMapping* [numberOfCurves-1];
  char * scstat = new char[numberOfCurves-1];

  int n;
  if (joinLastVisible)
  {
    temp[0] = &curve;
    scstat[0] = subCurveState[subCurveNumber];
    newCurveNumber=0;

    for( n=1; n<subCurveNumber; n++ ) // subCurveNumber == lastVisible
    {
      temp[n]=subCurves[n];
      scstat[n] = subCurveState[n];
    }
    for( n=subCurveNumber; n<numberOfCurves-1; n++ )
    {
      temp[n]=subCurves[n+1];
      scstat[n] = subCurveState[n+1];
    }
  }
  else if (joinLastInvisible)
  {
    for( n=0; n <= lastVisible; n++ )
    {
      temp[n]   = subCurves[n];
      scstat[n] = subCurveState[n];
    }
    temp[lastVisible+1]   = &curve;
    scstat[lastVisible+1] = subCurveState[subCurveNumber];
    newCurveNumber=lastVisible+1;
    
    for( n=lastVisible+2; n<numberOfCurves-1; n++ )
    {
      temp[n]   = subCurves[n+1];
      scstat[n] = subCurveState[n+1];
    }
  }
  else
  {
    for( n=0; n<subCurveNumber; n++ )
    {
      temp[n]   = subCurves[n];
      scstat[n] = subCurveState[n];
    }
    temp[subCurveNumber]   = &curve;
    scstat[subCurveNumber] = subCurveState[subCurveNumber];
    newCurveNumber=subCurveNumber;

    for( n=subCurveNumber+1; n<numberOfCurves-1; n++ )
    {
      temp[n]   = subCurves[n+1];
      scstat[n] = subCurveState[n+1];
    }
  }
  
// AP: What about reference counting subCurves?
  if ( subCurves!=NULL ) delete [] subCurves;
  if ( subCurveState!=NULL ) delete [] subCurveState;

  subCurves=temp;
  subCurveState = scstat;
  numberOfCurves--;
  if( subCurveNumber<=lastVisible )
    lastVisible--;   // we join two visible sub-curves, the new curve becomes visible.
  
  return newCurveNumber;
}



int NurbsMapping::
moveEndpoint( int end, const RealArray &endPoint, real tol /*=-1*/ )
// =============================================================================================
/// \details 
///   Move either the beginning or the end of the curve to endPoint.
//=============================================================================================
{

  if ( !( end==Start || end==End ) ) 
    return 1;

  if ( domainDimension==1 )
  {
    realArray r(1,1),x(1,rangeDimension);
    r = (real)end; // starting point near the end we are moving
    for( int dir=0; dir<rangeDimension; dir++)
      x(0,dir)=endPoint(dir);

    inverseMap(x, r);

//    printf("moveEndpoint: end=%i, r=%e\n",end,r(0,0));
    
    tol = tol>0 ? tol : 100*FLT_EPSILON;

    if( ((end==0 && r(0,0)>(uKnot(0) +tol)) ||   // *wdh* 010427 restrict allowable movements
        (end==1 && r(0,0)<(uKnot(m1)-tol))) && // *kkc* 020919 allow user specified tolerance  
	( fabs(r(0,0))>tol && fabs(r(0,0)-1)>tol ) ) // *kkc* make sure we are not sitting right on the endpoint    
    {
//      printf("moveEndpoint: attempting to split the curve...\n");
      NurbsMapping leftCurve, rightCurve;
      if ( split(r(0,0), leftCurve, rightCurve)==0 )
      {
	if ( end==Start ) 
	{
	  // *wdh* 010427 *this = rightCurve;  // avoid this copy since it will destroy sub-curves
	  m1=rightCurve.m1;
	  n1=rightCurve.n1;
	  uKnot.redim(0);
	  uKnot=rightCurve.uKnot;  
	  cPoint.redim(0);
	  cPoint=rightCurve.cPoint;
	}
	else
	{
	  // *wdh* 010427 *this = leftCurve; // avoid this copy since it will destroy sub-curves
	  m1=leftCurve.m1;
	  n1=leftCurve.n1;
	  uKnot.redim(0);
	  uKnot=leftCurve.uKnot;  
	  cPoint.redim(0);
	  cPoint=leftCurve.cPoint;

	}
	      
      }
      else
	return 1;
    }
    
//      else
//        printf("moveEndpoint: NOT splitting the curve...\n");
    

    // now actually move the endpoint 
    bool finishedMoving = false;
    int cpmove = 0;
    int dir = end==1 ? -1 : 1;
    RealArray originalEnd(1,rangeDimension);
    Range AXES(0,rangeDimension-1);
    originalEnd = cPoint(end*n1, AXES);
    
//      printf("before: cPoint=( ");
//      int i,j;
//      for (j=0; j<rangeDimension; j++)
//      {
//        for (i=0; i<=n1; i++)
//  	printf("%e ", cPoint(i,j));
//        printf("\n");
//      }
//      printf(")\n");

    while ( !finishedMoving )
    {
      int p = end*n1 + dir*cpmove;
	
      if ( p >= cPoint.getBase(0) && p <= cPoint.getBound(0) && // prevents array bounds error
	   sum(pow(originalEnd-cPoint(p,AXES),2))<100*REAL_MIN ) 
      {
	// there could be multiple control points at the end and each one
	//   needs to be shifted
	//  (ie in a piecwise linear curve represented by a cubic NURBS)
//	    printf("Moving control point %i\n", p);
	for ( int a=0; a<rangeDimension; a++ )
	{
	  // *wdh* 100207  cPoint( p, a ) = endPoint(a);
          // *wdh* 100207 (Chatillon France) -- the cPoint array is scaled by the weight so we
          // need to scale the end point 
	  cPoint( p, a ) = endPoint(a)*cPoint(p,rangeDimension);
	}
      }
      else
	finishedMoving = true;
	
      cpmove++;
    }
//    printf("After moveEndPoint: cpmove=%i, order=%i\n", cpmove, getOrder());
    
//      printf("after: cPoint=( ");
//      for (j=0; j<rangeDimension; j++)
//      {
//        for (i=0; i<=n1; i++)
//  	printf("%e ", cPoint(i,j));
//        printf("\n");
//      }
//      printf(")\n");
    
  } // end if domainDimension == 1
  
  else
    return 1;

  initialize();
  return 0;
}

int NurbsMapping::
numberOfSubCurves() const
// =============================================================================================
/// \details 
///   If the Nurb is formed by merging a sequence of Nurbs then function will return that number.
///   By default the numberOfSubCurves would be 1 if no Nurbs were merged.
//=====================================================================================
{
  return lastVisible+1;
}

int NurbsMapping::
numberOfSubCurvesInList() const
// =============================================================================================
/// \details 
///   Return the number of subcurves used to build the Nurb plus the number of hidden curves
///   By default the numberOfSubCurvesInList would be 1 if no Nurbs were merged.
//=====================================================================================
{
  return numberOfCurves;
}

  // Here is the sub curve.
NurbsMapping& NurbsMapping::
subCurve(int subCurveNumber)
// =============================================================================================
/// \details 
///   If the Nurb is formed by merging a sequence of Nurbs then function will return that Nurbs.
///  If the numberOfSubCurves is 1 then the current (full) Nurbs is returned.
///   
//=====================================================================================
{
  //  if( subCurveNumber < numberOfCurves )
  if( subCurveNumber <= lastVisible )
  {
    if( numberOfCurves==1 )
      return *this;
    else
    {
      assert( subCurves!=0 && subCurves[subCurveNumber]!=0 );
      return *subCurves[subCurveNumber];
    }
  }
  else
  {
    printf("NurbsMapping:subCurve:ERROR:Invalid subCurveNumber = %i . The number of subCurves is %i \n",
	   subCurveNumber,numberOfCurves);
    {throw "error";}
  }
}

NurbsMapping & NurbsMapping::
subCurveFromList(int subCurveNumber)
// =============================================================================================
/// \details 
///   Return a nurb curve directly from the list of subcurves.  This can be a curve used to generate
///  the nurb itself or one of the "hidden" curves.
///  If the numberOfSubCurves is 1 then the current (full) Nurbs is returned.
///   
//=====================================================================================
{
  if( subCurveNumber < numberOfCurves )
  {
    if( numberOfCurves==1 )
      return *this;
    else
    {
      assert( subCurves!=0 && subCurves[subCurveNumber]!=0 );
      return *subCurves[subCurveNumber];
    }
  }
  else
  {
    printf("NurbsMapping:subCurve:ERROR:Invalid subCurveNumber = %i . The number of subCurves is %i \n",
	   subCurveNumber,numberOfCurves);
    {throw "error";}
  }
}


int NurbsMapping::
put( const aString & fileName, const FileFormat & fileFormat /* = xxww */ ) 
// =============================================================================
/// \details 
///    put NURBS data into an ascii readable file.
/// \param fileName (input) : name of the file.
/// \param fileFormat (input) : specify the file format. (see the comments with the get(const aString\&,...) function).
// =============================================================================
{
  FILE *file;
  file = fopen ((const char*)fileName, "w");

  if( file== NULL)
  {
    printf ("NurbsMapping::put:ERROR: File %s could not be opened\n", (const char*)fileName); 
    return 1;
  }
  
  int returnValue=put(file,fileFormat);
  fclose(file);
  return returnValue;
}

int NurbsMapping::
put( FILE *file, const FileFormat & fileFormat /* = xxww */ )
// =============================================================================
/// \details 
///    Save the NURBS data to an ascii readable file.
/// \param fileFormat (input) : specify the file format. (see the comments with the get(const aString\&,...) function).
// =============================================================================
{
  fprintf(file,"%i %i %i %i %i %i\n",domainDimension,rangeDimension,p1,n1,p2,n2);
  int i,axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    const int m= axis==0 ? m1 : m2;
    const RealArray & knot = axis==0 ? uKnot : vKnot;
    for( i=0; i<=m; i++ )
    {
      fprintf(file,"%e ",knot(i));
      if( i % 10 == 9 || i==m )
	fprintf(file,"\n");
    }
  }
  const int nw=rangeDimension;
  cPoint.reshape(n1+1,n2+1,rangeDimension+1);
  if( fileFormat==xxww )
  {
    int j=0;
    for( axis=0; axis<=rangeDimension; axis++ )
    {
      for( int i2=0; i2<=n2; i2++ )
      {
	for( i=0; i<=n1; i++ )
	{
	  // save control points unscaled by the weight factor, (x,y,z,w)
	  fprintf(file,"%e ",axis!=nw && cPoint(i,i2,nw)!=0. ? cPoint(i,i2,axis)/cPoint(i,i2,nw) :cPoint(i,i2,axis) );
	  j++;
	  if( j % 10 == 9 || i==n1 )
	  {
	    j=0;
	    fprintf(file,"\n");
	  }
	}
      }
    }
  }
  else
  {
    for( int i2=0; i2<=n2; i2++ )
    {
      for( int i1=0; i1<=n1; i1++ )
      {
        real w = cPoint(i1,i2,nw)==0. ? 1. : 1./cPoint(i1,i2,nw);
        if( rangeDimension==1 )
	  fprintf(file,"%e %e ",cPoint(i1,i2,0)*w,1./w);//cPoint(i1,i2,nw));
        else if( rangeDimension==2 )
	  fprintf(file,"%e %e %e ",cPoint(i1,i2,0)*w,cPoint(i1,i2,1)*w,1./w);//cPoint(i1,i2,nw));
        else
	  fprintf(file,"%e %e %e %e ",cPoint(i1,i2,0)*w,cPoint(i1,i2,1)*w,cPoint(i1,i2,2)*w,1./w);//cPoint(i1,i2,nw));

      }
    }
    
  }
  
  if( domainDimension==1 )
    cPoint.reshape(n1+1,rangeDimension+1);

  return 0;
}

int 
getLineFromFile( FILE *file, char s[], int lim);

int NurbsMapping::
get( const aString & fileName, const FileFormat & fileFormat /* = xxww */  )
// =============================================================================
/// \details 
///    read NURBS data from an ascii readable file.
/// \param fileName (input) : get from this file.
/// \param fileFormat (input) : specify the file format.
/// 
///  \noindent Here is the file format for {\tt fileFormat=xxww} for a surface in 3D
///  \begin{verbatim}
///   domainDimension rangeDimension p1 n1 p2 n2  
///   uKnot(0) uKnot(1) ... uKnot(m1)  --- on possibly multiple lines, at most 10 values per line
///   vKnot(0) vKnot(1) ... vKnot(m2)
///   x0 x1 x2 ...            --- x coords of control pts. on multiple lines, at most 10 per line
///   y0 y1 y2 ...            --- y coords of control pts.                                  
///   z0 z1 z2 ...            --- z coords of control pts.                                  
///   w0 w1 w2 ...            --- weights of control pts.                                  
///  \end{verbatim}
///  If the domainDimension==1 then leave off p2 and n2. If the rangeDimension is 2 then leave 
///   off the z values. Here m1=n1+p1+1 and m2=n2+p2+1.
/// 
///  \noindent Here is the file format for {\tt fileFormat=xwxw} for a surface in 3D
///  \begin{verbatim}
///   domainDimension rangeDimension p1 n1 p2 n2
///   uKnot(0) uKnot(1) ... uKnot(m1)  --- on possibly multiple lines, at most 10 values per line
///   vKnot(0) vKnot(1) ... vKnot(m2)
///   x0 y0 z0 w0                   --- control point 0
///   x1 y1 z1 w1                   --- control point 1
///   x1 y1 z1 w1                   --- control point 2
///   ... 
///  \end{verbatim}
///  If the domainDimension==1 then leave off p2 and n2. If the rangeDimension is 2 then leave 
///   off the z values.
// =============================================================================
{
  FILE *file;
  file = fopen ((const char*)fileName, "r");

  if( file == NULL)
  {
    printf ("NurbsMapping::get:ERROR: File %s could not be opened\n", (const char*)fileName); 
    return 1;
  }
  
  int returnValue=get(file,fileFormat);
  fclose(file);
  if( returnValue==0 )
  {
    if( getName(mappingName)=="nurbsMapping" )
      setName(mappingName,fileName);
  }

  return returnValue;
}


int NurbsMapping::
get( FILE *file, const FileFormat & fileFormat /* = xxww */ )
// =============================================================================
/// \details 
///    read NURBS data from an ascii readable file.
/// \param file (input) : get from this file.
/// \param fileFormat (input) : specify the file format. (see the comments with the get(const aString\&,...) function).
// =============================================================================
{
  domainDimension=0; rangeDimension=0; p1=0; n1=0; p2=0; n2=0;
  int numberRead;

  if( fileFormat!=cheryl )
    numberRead=fScanF(file,"%i %i %i %i %i %i",&domainDimension,&rangeDimension,&p1,&n1,&p2,&n2);
  else
  {
    domainDimension=2;
    rangeDimension=3;
    numberRead=fScanF(file,"%i %i %i %i %i %i",&p1,&p2,&m1,&m2,&n1,&n2);
  }
  
  if( domainDimension<0 || domainDimension>2 || rangeDimension<1 || rangeDimension>3 )
  {
    printf("NurbsMapping::invalid domainDimension=%i or rangeDimension=%i\n",domainDimension,rangeDimension);
    return 1;
  }
  if( domainDimension==1 && ( p2!=0 || n2!=0 ) )
  {
    printf("NurbsMapping::get:invalid values: domainDimension=%i but p2=%i n2=%i\n",domainDimension,p2,n2);
    return 1;
  }
  m1=n1+p1+1;
  m2=n2+p2+1;



  int i;
  // read in the knots
  int axis;
  for( axis=0; axis<domainDimension; axis++ )
  {
    const int m= axis==0 ? m1 : m2;
    RealArray & knot = axis==0 ? uKnot : vKnot;
    knot.redim(m+1);
    for( i=0; i<=m; i++ )
    {
      numberRead=fScanF(file,"%e",&knot(i));
      if( numberRead==0 )
      {
	printf("NurbsMapping::get:ERROR reading knots\n");
        return 1;
      }
    }
  }

  cPoint.redim(n1+1,n2+1,rangeDimension+1);
  numberRead=0;

  if( fileFormat==xxww )
  {
    for( int n=0; n<=rangeDimension; n++ )
    {
      for( int i2=0; i2<=n2; i2++ )
      {
	for( int i1=0; i1<=n1; i1++ )
	{
	  numberRead=fScanF(file,"%e",&cPoint(i1,i2,n));
	  if( numberRead<=0 )
	  {
	    printf("NurbsMapping::get:ERROR reading file. Error occured while reading control points\n");
	    printf("currently reading point i1=%i, i2=%i. \n",i1,i2);
	    printf("domainDimension=%i, rangeDimension=%i, p1=%i, n1=%i, m1=%i, p2=%i, n2=%i, m2=%i, \n",
		   domainDimension,rangeDimension,p1,n1,m1,p2,n2,m2);
	    throw "error";
	  }
	}
      }
    }
  }
  else if( fileFormat==xwxw )
  {
    for( int i2=0; i2<=n2; i2++ )
    {
      for( int i1=0; i1<=n1; i1++ )
      {
	if( rangeDimension==1 )
	  numberRead=fScanF(file,"%e %e ",&cPoint(i1,i2,0),&cPoint(i1,i2,1));
	else if( rangeDimension==2 )
	  numberRead=fScanF(file,"%e %e %e ",&cPoint(i1,i2,0),&cPoint(i1,i2,1),&cPoint(i1,i2,2));
	else 
	  numberRead=fScanF(file,"%e %e %e %e ",&cPoint(i1,i2,0),&cPoint(i1,i2,1),&cPoint(i1,i2,2),&cPoint(i1,i2,3));

	if( numberRead<=0 )
	{
	  printf("NurbsMapping::get:ERROR reading file. Error occured while reading control points\n");
	  printf("currently reading point i1=%i, i2=%i. \n",i1,i2);
	  printf("domainDimension=%i, rangeDimension=%i, p1=%i, n1=%i, m1=%i, p2=%i, n2=%i, m2=%i, \n",
		 domainDimension,rangeDimension,p1,n1,m1,p2,n2,m2);
	  throw "error";
	}
      }
    }
  }
  else
  {
    aString iFormat=ftor("%i %i"); // (const char*)iFormat);
    const char *iformat = (const char *)iFormat;
    aString e4Format = ftor("%e %e %e %e");
    const char *e4format = (const char *)e4Format;
    int i1,i2;
    for( int j2=0; j2<=n2; j2++ )
    {
      for( int j1=0; j1<=n1; j1++ )
      {
        numberRead=fscanf(file,iformat,&i1,&i2);
        assert( numberRead>0 && i1>=0 && i1<=n1 && i2>=0 && i2<=n2 );
	
	if( rangeDimension==1 )
	  numberRead=fScanF(file,"%e %e ",&cPoint(i1,i2,0),&cPoint(i1,i2,1));
	else if( rangeDimension==2 )
	  numberRead=fScanF(file,"%e %e %e ",&cPoint(i1,i2,0),&cPoint(i1,i2,1),&cPoint(i1,i2,2));
	else 
	  numberRead=fscanf(file,e4format,&cPoint(i1,i2,0),&cPoint(i1,i2,1),&cPoint(i1,i2,2),&cPoint(i1,i2,3));

	if( numberRead<=0 )
	{
	  printf("NurbsMapping::get:ERROR reading file. Error occured while reading control points\n");
	  printf("currently reading point i1=%i, i2=%i. \n",i1,i2);
	  printf("domainDimension=%i, rangeDimension=%i, p1=%i, n1=%i, m1=%i, p2=%i, n2=%i, m2=%i, \n",
		 domainDimension,rangeDimension,p1,n1,m1,p2,n2,m2);
	  throw "error";
	}
      }
    }
  }
  
  Range R1(0,n1),R2(0,n2);
  for( axis=0; axis<rangeDimension; axis++ )
    cPoint(R1,R2,axis)*=cPoint(R1,R2,rangeDimension); // multiply by the weight
  if( domainDimension==1 )
    cPoint.reshape(n1+1,rangeDimension+1);

  mappingHasChanged();
  initialized=false;
  uMin=uKnot(0);  // min(uKnot);
  uMax=uKnot(m1); // max(uKnot);
  if( domainDimension>1 )
  {
    vMin=vKnot(0);  // min(vKnot);
    vMax=vKnot(m2); // max(vKnot);
  }

  setGridDimensions( axis1,max(n1,11) );
  if( domainDimension>1 )
    setGridDimensions( axis2,max(n2,11) );


  initialize();

  // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse
  reinitialize(); 

  return 0;
}


int NurbsMapping::
getOrder( int axis /* =0 */ ) const
//===========================================================================
/// \brief  
///    Return the order, p.
//===========================================================================
{
  return axis==0 ? p1 : ( axis==1 ? p2 : p3 );
}

int NurbsMapping::
getNumberOfKnots( int axis /* =0 */ ) const
//===========================================================================
/// \brief  
///   Return the number of knots, m+1.
//===========================================================================
{
  return axis==0 ? m1+1 : ( axis==1 ? m2+1 : m3+1 );
}

int NurbsMapping::
getNumberOfControlPoints( int axis /* =0 */ ) const
//===========================================================================
/// \brief  
///    Return the number of control points, n+1.
//===========================================================================
{
  return axis==0 ? n1+1 : ( axis==1 ? n2+1 : n3+1 );
}

int NurbsMapping::
buildSubCurves( real angle /* =60. */ )
// ============================================================================
/// \brief 
///    Split a NURBS curve at corners into sub-curves. Currently this only applies if the
///   order of the NURBS is 1 (piece-wise linear).
/// \param angle (input) : divide the curve at points where the tangent changes by more than
///     this angle (degrees)
//=============================================================================
{
  if( domainDimension==1 && rangeDimension==2 && p1==1 )
  {
    // this trim curve may have corners since it is piece-wise linear (?)
    // we may need to split it into pieces
    // ::display(knots,"knots","%8.2e ");
    // ::display(cp,"control points","%8.2e ");
    const real tol = cos( angle*Pi/180. );

    if( numberOfSubCurves()>1 )
    {
      // If there are multiple sub-curves we split each one into sub-curves

      // note that we will ignore and delete all the invisible subcurves (? should we do this ?)
      int newNumberOfCurves=0;
      int c;
      for( c=0; c<numberOfSubCurves(); c++ )
      {
	subCurves[c]->buildSubCurves(angle);
	newNumberOfCurves+=subCurves[c]->numberOfSubCurves();
      }
      // Now make a list of all the new ones.
      NurbsMapping **temp = new NurbsMapping *[newNumberOfCurves];
      char *tmpstate = new char[newNumberOfCurves];
      int cc=0;
      for( c=0; c<numberOfSubCurves(); c++ )
      {
        NurbsMapping & map = *subCurves[c];
	for( int sc=0; sc<map.numberOfSubCurves(); sc++ )
	{
	  temp[cc]=&map.subCurve(sc); temp[cc]->incrementReferenceCount();
	  tmpstate[cc] = subCurveState[cc];
	  cc++;
	}
      }

      // delete old ones, including invisible ones which will now disappear forever.
      //      for( c=0; c<numberOfSubCurvesInList(); c++ )
      //	deleteSubCurve(c);
      for( c=0; c<numberOfSubCurvesInList(); c++ )
	if( subCurves[c]->decrementReferenceCount()==0 )
	  delete subCurves[c];
      delete [] subCurves;
      delete [] subCurveState;

      //      for ( c=0; c<newNumberOfCurves; c++ )
      //	{
      //	  addSubCurve(*temp[c]);
      //	  subCurveState[c] = tmpstate[c];
      //	}
      //deleteSubCurve(0);
      subCurves=temp;
      subCurveState = tmpstate;
      numberOfCurves=newNumberOfCurves;  // *wdh* 010824 why was this commented out?
      lastVisible = numberOfCurves-1;
    }
    else
    {
      real d0[2],d1[2];
      IntegerArray split(n1+1);  // there can be at most n1 splits.
      split(0)=0;
      int newNumberOfCurves = numberOfSubCurves();
      //if ( numberOfSubCurves()==1 ) newNumberOfCurves = 0;
      for( int i=1; i<n1; i++ )
      {
	d0[0]=cPoint(i,0)-cPoint(i-1,0);
	d0[1]=cPoint(i,1)-cPoint(i-1,1);

	d1[0]=cPoint(i+1,0)-cPoint(i,0);
	d1[1]=cPoint(i+1,1)-cPoint(i,1);
              
	real dot = (d0[0]*d1[0]+d0[1]*d1[1])/
	  max(REAL_MIN,SQRT( (d0[0]*d0[0]+d0[1]*d0[1])*(d1[0]*d1[0]+d1[1]*d1[1])));
	if( dot<tol )
	{
	  // printf("NurbsMapping::buildSubCurves i=%i, dot=%e, corner found. (d0=(%8.2e,%8.2e),"
          //      "d1=(%8.2e,%8.2e)\n",i,dot,d0[0],d0[1],d1[0],d1[1]);
	  split(newNumberOfCurves)=i;
	  newNumberOfCurves++;
	}
      }

      split(newNumberOfCurves)=n1;
    
      if( newNumberOfCurves>1 )
      {
	numberOfCurves = newNumberOfCurves;
	subCurves = new NurbsMapping*[numberOfCurves];
	subCurveState = new char[numberOfCurves];
	for( int c=0; c<newNumberOfCurves; c++ )
	{
	  int ia=split(c), ib=split(c+1);
    
	  int n=ib-ia;
	  int m=n+p1+1;
	  RealArray cp(n+1,3), knot(m+1);
	  cp=cPoint(Range(ia,ib),Range(0,2));

	  knot(Range(1,m-1)).seqAdd(0.,1./max(1.,m-2));
	  knot(0)=0.;  // double knot at ends
	  knot(1)=0.;
	  knot(m-1)=1.;
	  knot(m  )=1.;
		  
	  NurbsMapping & nurb = *new NurbsMapping(); nurb.incrementReferenceCount();
	
	  // ::display(knot,"knot");
	  // ::display(cp,"cp");
	
	  nurb.specify(m,n,p1,knot,cp,rangeDimension);
	  
	  // nurb.getGrid();
	  subCurves[c]=&nurb;
	  subCurveState[c] = 01;
	}
	lastVisible = numberOfCurves -1;
      }
    }
  }
  

  return 0;
}

real NurbsMapping::
getOriginalDomainBound(int side, int axis)
{
  if ( axis==0 )
    if ( side==0 )
      return uMin;
    else
      return uMax;
  else if (axis==1)
    if ( side==0 )
      return vMin;
    else
      return vMax;
  else
    return -1;

}

int NurbsMapping::
truncateToDomainBounds()
//=====================================================================================
/// \brief  clip the knots and control polygon to the bounds set in rstart and rend
//=====================================================================================
{
  const real knotEpsilon = REAL_EPSILON*10.; // ( note this matches the knotEpsilon in TrimmedMapping::triangulate! )

  if ( fabs(rStart[0]-uKnot(0))<knotEpsilon && fabs(rEnd[0]-uKnot(uKnot.getLength(0)-1))<knotEpsilon )
    return 0; // knots already match rstart and rend

  NurbsMapping left,right;

  bool normalizePieces=false;  // *wdh* 031123
  if ( (rStart[0]-uKnot(0))>=knotEpsilon )
  {
    split(rStart[0],left,right,normalizePieces);

    m1=right.m1;
    n1=right.n1;
    uKnot.redim(0);
    uKnot=right.uKnot;  
    cPoint.redim(0);
    cPoint=right.cPoint;
    // *this = right;
    // uKnot.display("truncateToDomainBounds: uKnot after truncation at r=0");
  }

  reinitialize();
  initialize();
 
  // uKnot.display("truncateToDomainBounds: uKnot after truncation at r=0 and initialize");

  if ( (uKnot(uKnot.getLength(0)-1)-rEnd[0])>=knotEpsilon )
  {
    split(rEnd[0], left, right,normalizePieces);
    m1=left.m1;
    n1=left.n1;
    uKnot.redim(0);
    uKnot=left.uKnot;  
    cPoint.redim(0);
    cPoint=left.cPoint;
    // *this = left;

    // uKnot.display("truncateToDomainBounds: uKnot after truncation at r=1");
  }

  normalizeKnots();  //  *wdh* 031123
  
  reinitialize();
  initialize();

  //uKnot.display("uKnot after truncation");
  return 0;
}

int NurbsMapping::
toggleSubCurveVisibility( int sc )
//===========================================================================
/// \details 
///    Toggle a subcurve's "visibility", 
///       a visible subcurve is accessible through NurbsMapping::subCurve(..) method
///     an invisible subcurve is only accessible through NurbsMapping::subCurveFromList()
/// \param sc (input) : the subcurve to toggle
///  Returns : the new subcurve number
///  NOTES : this will reorder the subcurves in the subCurves array
//===========================================================================
{
  int newidx;
  if ( sc>lastVisible )
  { // the curve is currently invisible, make it visible by sticking it at the
    //  end of the list of visible curves
    if( Mapping::debug & 1 ) printF("NurbsMapping::toggleSubCurveVisibility: Make sub-curve %i VISIBLE\n",sc);
    
    NurbsMapping *temp = subCurves[lastVisible+1];
    char tc = subCurveState[lastVisible+1];
    subCurves[lastVisible+1] = subCurves[sc];
    subCurveState[lastVisible+1] = subCurveState[sc];
    subCurves[sc] = temp;
    subCurveState[sc] = tc;
    newidx = lastVisible+1;
    lastVisible++;
  }
  else if ( sc<=lastVisible )
  { // the curve is visible, make it invisible by moving it to the end of the list
    if( Mapping::debug & 1 ) printF("NurbsMapping::toggleSubCurveVisibility: Make sub-curve %i INVISIBLE\n",sc);
    if( numberOfCurves>1 )
    {
      int nn=sc, n=sc+1;
      NurbsMapping * temp = subCurves[sc];
      char tc = subCurveState[sc];
      for ( n=sc+1; n<numberOfCurves; n++ )
      {
	subCurves[nn] = subCurves[n];
	subCurveState[nn] = subCurveState[n];
	nn++;
      }
      subCurves[nn] = temp;
      subCurveState[nn] = tc;
      lastVisible--;
      newidx = nn;
    }
    else
    {
      lastVisible--;
      newidx=0;
    }
      
  }

  return newidx;
}


bool NurbsMapping::
isSubCurveHidden( int sc )
//===========================================================================
/// \details 
///    find out if a subcurve is hidden or not, returns true if hidden, false if visible
/// \param sc (input) : the subcurve to querry
//===========================================================================
{
  return sc>lastVisible;
}

bool NurbsMapping::
isSubCurveOriginal( int sc )
//===========================================================================
/// \details 
///    find out if a subcurve is marked as "original"
/// \param sc (input) : the subcurve to querry
//===========================================================================  
{
  if ( numberOfCurves==1 )
    return true;
  else
    return subCurveState[sc]==01;
}

void NurbsMapping::
toggleSubCurveOriginal( int sc )
//===========================================================================
/// \details 
///    toggle the "original" status on a subcurve, "original" is just a marker
///    used to distingish the original subcurves used to build this nurb from 
///    subsequent modifications.
/// \param sc (input) : the subcurve to alter
//=========================================================================== 
{
  if ( sc>=0 && sc<numberOfCurves && numberOfCurves>1 )
    {
      if ( subCurveState[sc] == 01 )
	subCurveState[sc] = 0;
      else
	subCurveState[sc] = 01;
    }
}

int NurbsMapping::
addSubCurve(NurbsMapping &nurbs)
//===========================================================================
/// \details 
///    Add a subcurve to this mapping.  Note that the nurb is copied and is 
///   set to visible.  The "original" marker is set to false;
/// \param Returns : the index of the new curve in the list of visible curves
//===========================================================================  
{
  NurbsMapping **temp = new NurbsMapping* [numberOfCurves+1];
  char * scstat = new char[numberOfCurves+1];

  int nn = 0;
  if( numberOfCurves==1 )
    {
      temp[0] = new NurbsMapping;
      *temp[0]=*this;  // deep copy. Save this original nurb
      temp[0]->setName(mappingName,getName(mappingName)+"-merged");
      temp[0]->incrementReferenceCount();
      scstat[0] = 01;
    }
  else
    for( int n=0; n<numberOfCurves; n++ )
      {
	temp[n]=subCurves[n];
	scstat[n] = subCurveState[n];
      }
  
  temp[numberOfCurves]=new NurbsMapping;
  *temp[numberOfCurves]=nurbs;  // deep copy
  temp[numberOfCurves]->incrementReferenceCount();
  scstat[numberOfCurves] = 0;  // default state is not original, not hidden
  if ( subCurves!=NULL ) delete [] subCurves;
  if ( subCurveState!=NULL ) delete [] subCurveState;
  subCurves=temp;
  subCurveState = scstat;
  numberOfCurves++;
  
  return toggleSubCurveVisibility(numberOfCurves-1); // make the subcurve visible
}

int NurbsMapping::
deleteSubCurve(int sc)
//===========================================================================
/// \details 
///    Delete a subcurve from the list of curves.  Note this shifts the subcurve list
///     making previous indices invalid
/// \param sc (input): the curve to delete
/// \param Returns : 0 on success
//===========================================================================  
{
  if ( sc>=0 && sc<numberOfCurves )
    {
      if ( sc<=lastVisible ) lastVisible--;

      NurbsMapping **temp;
      char * scstat;
      if ( numberOfCurves>2 )
	{
	  temp = new NurbsMapping* [numberOfCurves-1];
	  scstat = new char[numberOfCurves-1];
	  int nn=0;
	  for( int n=0; n<numberOfCurves; n++ )
	    if ( n!=sc )
	      {
		temp[nn]=subCurves[n];
		scstat[nn] = subCurveState[n];
		nn++;
	      }
	  
	}
      else
	{
	  temp = NULL;
	  scstat = NULL;
	}

      if ( (subCurves[sc]->decrementReferenceCount())==0 ) delete subCurves[sc];

      delete [] subCurves;
      delete [] subCurveState;
      subCurves = temp;
      subCurveState = scstat;
      numberOfCurves--;
    }

  return 0;
}


//! Join two sub-curves (merge into one)
/*!
     This function will attempt to join subCurve1 to subCurve2 and if successful then delete subCurve2.

 /param subCurve1, subCurve2 (input) : attempt to join these two subcurves.
 /return : 0 for success.
 */
int NurbsMapping::
joinSubCurves( int subCurve1, int subCurve2 )
{
  bool curvesWereJoined=false;
  
  if( subCurve1<0 || subCurve1>numberOfCurves ||
      subCurve2<0 || subCurve2>numberOfCurves )
  {
    printf("NurbsMapping::joinSubCurves:ERROR: subCurves to join invalid, subCurve1=%i, subCurve2=%i "
           "but numberOfCurves=%i\n",subCurve1,subCurve2,numberOfCurves);
  }
  else
  {
    NurbsMapping & curve1=*subCurves[subCurve1];
    NurbsMapping & curve2=*subCurves[subCurve2];
    
    curvesWereJoined = curve1.merge(curve2)==0;
    if( curvesWereJoined )
    {
      deleteSubCurve( subCurve2 );
    }

  }

  return curvesWereJoined==false;
}


void  NurbsMapping::
display( const aString & label /* =blankString */ ) const
{

  cout << "***************NurbsMapping*****************\n";
  if( label!="" )
    cout << "******** " << label << "*************\n";

  printf("axis1: number of knots = %i, number of control points =%i, order of B-spline =%i \n",
	 m1+1,n1+1,p1);
  if( domainDimension==1 )
    printf("number of sub curves = %i \n",numberOfSubCurves());
  
  // ::display(uKnot,"Here are the knots along axis1");
  int i=0;
  for( i=0; i<=m1; i++ )
    printf(" uKnot i=%i : %e\n",i,uKnot(i));
  
  if( domainDimension>1 )
  {
    printf("axis2: number of knots = %i, number of control points =%i, order of B-spline =%i \n",
	   m2+1,n2+1,p2);
    // ::display(vKnot,"Here are the knots along axis2");
    for( i=0; i<=m2; i++ )
      printf(" vKnot i=%i : %e\n",i,vKnot(i));
  }
  if( domainDimension>2 )
  {
    printf("axis3: number of knots = %i, number of control points =%i, order of B-spline =%i \n",
	   m3+1,n3+1,p3);
    for( i=0; i<=m3; i++ )
      printf(" wKnot i=%i : %e\n",i,wKnot(i));
  }
  // ::display(cPoint,"Here are the control points (x*w,y*w,z*w,w), w=weight");
  if ( domainDimension==1 )
  {
    for( i=0; i<=n1; i++ )
    {
      // *wdh* 100208 real w = cPoint(i,rangeDimension)!=0. ? 1./cPoint(i,rangeDimension) : 1.;
      real w = cPoint(i,rangeDimension)!=0. ? cPoint(i,rangeDimension) : 1.;
      printf(" control-point/weight i=%i : (%e,%e,%e)  weight=%e \n",i,cPoint(i,0)/w,cPoint(i,1)/w,
	     (rangeDimension==2 ? 0. : cPoint(i,2)/w),cPoint(i,rangeDimension));
    }
  }
  else if ( domainDimension==2 )
  {
    for ( int j=0; j<=n2; j++ )
      for( i=0; i<=n1; i++ )
      {
	real w = cPoint(i,j,rangeDimension)!=0. ? cPoint(i,j,rangeDimension) : 1.;
	printf(" control-point/weight i,j=%i,%i : (%e,%e,%e) weight=%e \n",i,j,cPoint(i,j,0)/w,cPoint(i,j,1)/w,
	       (rangeDimension==2 ? 0. : cPoint(i,j,2)/w),cPoint(i,j,rangeDimension));
      }
  }
  else if ( domainDimension==3 )
  {
    for ( int k=0; k<=n3; k++ )
    for ( int j=0; j<=n2; j++ )
      for( i=0; i<=n1; i++ )
      {
	real w = cPoint(i,j,k,rangeDimension)!=0. ? cPoint(i,j,k,rangeDimension) : 1.;
	printf(" control-point/weight i,j,k=%i,%i,%i : (%e,%e,%e) weight=%e \n",i,j,k,
               cPoint(i,j,k,0)/w,cPoint(i,j,k,1)/w,
	       (rangeDimension==2 ? 0. : cPoint(i,j,k,2)/w),cPoint(i,j,k,rangeDimension));
      }
  }

  
  Mapping::display();
    
}


//=================================================================================
// get a mapping from the database
//=================================================================================
int NurbsMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  if( debug & 4 )
    cout << "Entering NurbsMapping::get" << endl;

  subDir.get( NurbsMapping::className,"className" ); 
  if( NurbsMapping::className != "NurbsMapping" )
  {
    cout << "NurbsMapping::get ERROR in className!" << endl;
  }

  subDir.get(n1,"n1");
  subDir.get(n2,"n2");
  subDir.get(n3,"n3");
  subDir.get(m1,"m1");
  subDir.get(m2,"m2");
  subDir.get(m3,"m3");
  subDir.get(p1,"p1");
  subDir.get(p2,"p2");
  subDir.get(p3,"p3");
  
  subDir.get(uKnot,"uKnot");
  subDir.get(vKnot,"vKnot");
  subDir.get(wKnot,"wKnot");

  subDir.get(cPoint,"cPoint");
  subDir.get(initialized,"initialized");
  subDir.get(nonUniformWeights,"nonUniformWeights");

  subDir.get(uMin,"uMin");
  subDir.get(uMax,"uMax");

  subDir.get(vMin,"vMin");
  subDir.get(vMax,"vMax");

  subDir.get(wMin,"wMin");
  subDir.get(wMax,"wMax");

  subDir.get(rStart,"rStart",3);
  subDir.get(rEnd,"rEnd",3);
  subDir.get(nurbsIsPeriodic,"nurbsIsPeriodic",3);

  subDir.get(numberOfCurves,"numberOfCurves");
  aString buff;
  if( numberOfCurves>1 )
  {
    subDir.get(lastVisible,"lastVisible");
    subCurves = new NurbsMapping* [numberOfCurves];
    subCurveState = new char[numberOfCurves];
    for( int cs=0; cs<numberOfCurves; cs++ )
    {
      buff = "";
      subCurves[cs]=new NurbsMapping;
      subCurves[cs]->incrementReferenceCount();
      subCurves[cs]->get( subDir,sPrintF(buff,"subCurve %i",cs) );
      buff="";
      int scs=0;
      subDir.get(scs,sPrintF(buff,"subCurveState %i",cs));
      
      subCurveState[cs] = scs==1 ? 01 : 0;
    }
  }

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  
  initialize(); 
  
  return 0;
}

int NurbsMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( NurbsMapping::className,"className" );

  subDir.put(n1,"n1");
  subDir.put(n2,"n2");
  subDir.put(n3,"n3");
  subDir.put(m1,"m1");
  subDir.put(m2,"m2");
  subDir.put(m3,"m3");
  subDir.put(p1,"p1");
  subDir.put(p2,"p2");
  subDir.put(p3,"p3");
  
  subDir.put(uKnot,"uKnot");
  subDir.put(vKnot,"vKnot");
  subDir.put(wKnot,"wKnot");
  subDir.put(cPoint,"cPoint");
  subDir.put(initialized,"initialized");
  subDir.put(nonUniformWeights,"nonUniformWeights");

  subDir.put(uMin,"uMin");
  subDir.put(uMax,"uMax");

  subDir.put(vMin,"vMin");
  subDir.put(vMax,"vMax");

  subDir.put(vMin,"wMin");
  subDir.put(vMax,"wMax");

  subDir.put(rStart,"rStart",3);
  subDir.put(rEnd,"rEnd",3);
  subDir.put(nurbsIsPeriodic,"nurbsIsPeriodic",3);

  subDir.put(numberOfCurves,"numberOfCurves");
  aString buff;
  if( numberOfCurves>1 )
  {
    subDir.put(lastVisible,"lastVisible");   
    for( int cs=0; cs<numberOfCurves; cs++ )
      {
	buff = "";
	subCurves[cs]->put( subDir,sPrintF(buff,"subCurve %i",cs) );
	buff="";
	int scs = subCurveState[cs] & 01 ? 1 : 0;
	subDir.put(scs,sPrintF(buff,"subCurveState %i",cs));
      }
  }

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}

Mapping *NurbsMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==NurbsMapping::className )
    retval = new NurbsMapping();
  return retval;
}

int NurbsMapping::
plot(GenericGraphicsInterface & gi, GraphicsParameters & parameters, bool plotControlPoints/* = false */ )
// =====================================================================================
// /Description 
//   Internal plotting routine.
// =====================================================================================
{
  if (!initialized)
    return 1;
  
//  parameters.set(GI_TOP_LABEL,getName(mappingName));
  aString curveColour;
  parameters.get(GI_MAPPING_COLOUR, curveColour);
//  gi.erase();
  if( plotControlPoints )
  {
    // make a DataPointMapping to plot the control points.
    int plotSurface; 
    parameters.get(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotSurface); // get current value
    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,false);
    DataPointMapping dpm;
    realArray xd;
    // un-normalize by the weight
    if( domainDimension==1 )
    {
      Range R1(0,n1);
      xd.redim(R1,rangeDimension);
      for( int axis=0; axis<rangeDimension; axis++ )
      {
        #ifndef USE_PPP
    	xd(R1,axis)=cPoint(R1,axis)/cPoint(R1,rangeDimension);  // un-normalize by the weight
        #else
        for( int i1=0; i1<=n1; i1++ )
          xd(i1,axis)=cPoint(i1,axis)/cPoint(i1,rangeDimension); 
        #endif
      }
      
      parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,true); // set to true to get grid lines plotted

    }
    else
    {
      Range R1(0,n1), R2(0,n2);
      xd.redim(R1,R2,rangeDimension);
      for( int axis=0; axis<rangeDimension; axis++ )
      {
        #ifndef USE_PPP
	xd(R1,R2,axis)=cPoint(R1,R2,axis)/cPoint(R1,R2,rangeDimension);  // un-normalize by the weight
        #else
        for( int i2=0; i2<=n2; i2++ )
        for( int i1=0; i1<=n1; i1++ )
  	  xd(i1,i2,axis)=cPoint(i1,i2,axis)/cPoint(i1,i2,rangeDimension);  // un-normalize by the weight
        #endif
      }
    }
    // ::display(xd,"data points for control points");
    dpm.setDataPoints(xd,domainDimension);
    parameters.set(GI_MAPPING_COLOUR,"black");
    PlotIt::plot(gi,dpm,parameters);
    if( domainDimension==1 )
      gi.plotPoints(xd,parameters);
    parameters.set(GI_USE_PLOT_BOUNDS,false); 
    parameters.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES,plotSurface);   // reset
    parameters.set(GI_USE_PLOT_BOUNDS,true); 
  }
  parameters.set(GI_MAPPING_COLOUR, curveColour);
  PlotIt::plot(gi,*this,parameters);  
  if( plotControlPoints )
    parameters.set(GI_USE_PLOT_BOUNDS,false); 

  return 0;
  
}



int NurbsMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the nurbs mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!NurbsMapping",
      "enter control points",
      "enter points",      
      "change control points",
      ">shapes",
        "circle",
        "conic",
        "general cylinder",
      "<set domain dimension",
      "set range dimension",
      "plot control points",
      "plot sub curves",
      "do not plot sub curves",
      "plot points on curves",
      "do not plot points on curves",      
      "plot first derivatives", 
      "plot second derivatives",
      "elevate the degree",
      "restrict the domain",
      "reparameterize",
      "reset reparameterization",
      "rotate",
      "scale",
      "shift",
      "interpolate lofted surface",
      "interpolate from a mapping",
      "interpolate from mapping with options",
      "project to 2d",
      "merge",
      "parameterize by chord length",
      "parameterize by index (uniform)",
      "use Eleven eval",
      "do not use Eleven eval",
//      "do not normalize knots",
      ">file input/output",
        "save NURBS data, format xwxw",
        "read NURBS data, format xwxw",
        "save NURBS data, format xxww",
        "read NURBS data, format xxww",
//        "save NURBS data, format cheryl",
        "read NURBS data, format cheryl",
        "save points in matlab format",
      "<check inverse",
      "check",
      "use robust inverse",
      "do not use robust inverse",
//    "make a coordinate curve",  // for debugging
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "enter control points: enter knots, control points and weights",
      "enter points        : specify points on the curve",
      "change control points : change existing control points",
      "circle            : make a circle.",
      "set domain dimension: specify if nurb is a curve (1) or surface (2)",
      "set range dimension : specify if nurbs is 2D or 3D",
      "plot control points",
      "plot sub curves     : plot subcurves that form the NURBS curve",
      "elevate the degree",
      "restrict the domain : reparameterize using a portion of domain unit cube",
      "reparameterize      : reparameterize using a portion of domain unit cube",
      "rotate",
      "scale",
      "shift",
      "interpolate from a mapping",
      "interpolate from mapping with options",
      "merge",
      "parameterize by chord length : when entering points or interpolating from a mapping",
      "parameterize by index (uniform) : when entering points or interpolating from a mapping",
      "project to 2d       : convert a 3D curve to 2D by projecting onto a plane",
      "save NURBS data, format xwxw : save an ascii file with format x1,y1,w1, x2,y2,w2,...",
      "read NURBS data, format xwxw",
      "save NURBS data, format xxww : save an ascii file with format x1,x2,...,y1,y2,...,w1,w2,...",
      "read NURBS data, format xxww",
      " ",
      "check inverse       : check the inverse of the mapping",
      "check               : chech the mapping",
      "lines               : specify number of grid lines",
      "boundary conditions : specify boundary conditions",
      "share               : specify share values for sides",
      "mappingName         : specify the name of this mapping",
      "periodicity         : specify periodicity in each direction",
      "show parameters     : print current values for parameters",
      "plot                : enter plot menu (for changing ploting options)",
      "help                : Print this list",
      "exit                : Finished with changes",
      "" 
    };

  aString answer,line,answer2; 

  bool plotObject=true;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);

  gi.appendToTheDefaultPrompt("Nurbs>"); // set the default prompt

  bool plotControlPoints=false;
  bool plotSubCurves=false;
  ParameterizationTypeEnum parameterizationType=parameterizeByChordLength;

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="enter control points" )
    { 
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter degree p >0 (default=%i) :",p1));
	if( line!="" ) sScanF(line,"%i ",&p1);
	gi.inputString(line,sPrintF(buff,"Enter the number of control points :"));
	if( line!="" ) 
	{
	  sScanF(line,"%i ",&n1);
	  n1--;
	}
        p2=0;
	n2=0;
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter degrees p1,p2 >0 (default=%i,%i) :",p1,p2));
	if( line!="" ) sScanF(line,"%i %i",&p1,&p2);
	gi.inputString(line,sPrintF(buff,"Enter the number of control points n1,n2"));
	if( line!="" ) 
	{
	  sScanF(line,"%i %i",&n1,&n2);
	  n1--; n2--;
	}
      }
      else
      {
	printf("Sorry: the case domainDimension=%i is not implemented\n",domainDimension);
	continue;
      }
      if( n1<p1 || n2<p2 )
      {
	printf("ERROR: The number of control points must be greater than the degree\n"
	       "       p1=%i, number of control points =n1+1=%i \n"
	       "       p2=%i, number of control points =n2+1=%i \n",
	       p1,n1+1,p2,n2+1);
	gi.stopReadingCommandFile();
      }
      
   
      m1=n1+p1+1;
      uKnot.redim(m1+1);
      int i;
      for( i=0; i<=p1; i++ )
	uKnot(i)=0;
      for( i=p1+1; i<m1-p1; i++ )
      {
	gi.inputString(line,sPrintF(buff,"Enter uKnot %i (Knots 0...%i are clamped to 0, %i,..,%i to 1.)",i,p1,
              m1-p1,m1));
        if( line!="" ) sScanF(line,"%e ",&uKnot(i));
      }
      for( i=m1-p1; i<=m1; i++ )
        uKnot(i)=1.;

      if( domainDimension==2 )
      {
	m2=n2+p2+1;
	vKnot.redim(m2+1);
	for( i=0; i<=p2; i++ )
	  vKnot(i)=0;
	for( i=p2+1; i<m2-p2; i++ )
	{
	  gi.inputString(line,sPrintF(buff,"Enter vKnot %i (Knots 0...%i are clamped to 0, %i,..,%i to 1.)",i,p2,
				      m2-p2,m2));
	  if( line!="" ) sScanF(line,"%e ",&vKnot(i));
	}
	for( i=m2-p2; i<=m2; i++ )
	  vKnot(i)=1.;
      }
      
      if( domainDimension==1 )
      {
	cPoint.redim(n1+1,rangeDimension+1);
	for( i=0; i<n1+1; i++ )
	{
	  if( rangeDimension==2 )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point %i and weight : x1,x2,w",i));
	    if( line!="" ) sScanF(line,"%e %e %e ",&cPoint(i,0),&cPoint(i,1),&cPoint(i,2));
	  }
	  else
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point %i and weight : x1,x2,x3,w",i));
	    if( line!="" ) sScanF(line,"%e %e %e %e",&cPoint(i,0),&cPoint(i,1),&cPoint(i,2),&cPoint(i,3));
	  }
	}
	Range R1(0,n1);
	for( int axis=0; axis<rangeDimension; axis++ )
	  cPoint(R1,axis)*=cPoint(R1,rangeDimension); // multiply by the weight

      }
      else if( domainDimension==2 )
      {
	cPoint.redim(n1+1,n2+1,rangeDimension+1);
	for( int j=0; j<n2+1; j++ )
	for( i=0; i<n1+1; i++ )
	{
	  if( rangeDimension==2 )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i) and weight : x1,x2,w",i,j));
	    if( line!="" ) sScanF(line,"%e %e %e ",&cPoint(i,j,0),&cPoint(i,j,1),&cPoint(i,j,2));
	  }
	  else
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i) and weight : x1,x2,x3,w",i,j));
	    if( line!="" ) sScanF(line,"%e %e %e %e",&cPoint(i,j,0),&cPoint(i,j,1),&cPoint(i,j,2),&cPoint(i,j,3));
	  }
	}
	Range R1(0,n1), R2(0,n2);
	for( int axis=0; axis<rangeDimension; axis++ )
	  cPoint(R1,R2,axis)*=cPoint(R1,R2,rangeDimension); // multiply by the weight
      }
      
	
      mappingHasChanged();
      initialized=false;
      uMin=uKnot(0);  // min(uKnot);
      uMax=uKnot(m1); // max(uKnot);
      if( domainDimension==2 )
      {
	vMin=vKnot(0);  // min(vKnot);
	vMax=vKnot(m2); // max(vKnot);
      }
      
      initialize();
      // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse
      reinitialize(); 
      plotObject=true;
    }
    else if( answer=="change control points" )
    {
      aString menu2[]={"done","show control points",""};  //
      for( ;; )
      {
        int i1,i2;
        if( domainDimension==1 )
          gi.getMenuItem(menu2,line,sPrintF(buff,"Change which control point i? i in [0,%i]\n",n1));
        else
          gi.getMenuItem(menu2,line,sPrintF(buff,"Change which control point i1,i2? ([0,%i],[0,%i])\n",n1,n2));
        if( line=="show control points" )
	{
          NurbsMapping::display();
          continue;
	}
        if( line=="done" || line=="" )
          break;
        int i=-1;
        if( domainDimension==1 )
	{
	  sScanF(line,"%i",&i);
	  if( i<0 || i>n1 )
	  {
	    printf("Invalid number for a control point, i=%i. Should be in the range [0,%i]\n",i,n1);
	    continue;
	  }
	}
	else
	{
	  sScanF(line,"%i %i",&i1,&i2);
	  if( i1<0 || i1>n1 || i2<0 || i2>n2 )
	  {
	    printf("Invalid number for a control point, (i1,i2)=(%i,%i). "
               "Should be in the range ([0,%i],[0,%i])\n",i1,i2,n1,n2);
	    continue;
	  }
	}
	
        if( domainDimension==1 && rangeDimension==2 )
	{
          real w=max(REAL_MIN*10.,cPoint(i,2));
          gi.inputString(line,sPrintF(buff,"current=(%6.2e,%6.2e,%6.2e) Enter new point and weight : x1,x2,w",
                  cPoint(i,0)/w,cPoint(i,1)/w,cPoint(i,2)));
          if( line!="" )
	  {
            sScanF(line,"%e %e %e ",&cPoint(i,0),&cPoint(i,1),&cPoint(i,2));
	    cPoint(i,Range(0,1))*=cPoint(i,2);
	  }
	}
	else if( domainDimension==2 && rangeDimension==3 )
	{
          real w=max(REAL_MIN*10.,cPoint(i1,i2,3));
          gi.inputString(line,sPrintF(buff,"current=(%6.2e,%6.2e,%6.2e,%6.2e) Enter new point and weight : x1,x2,x3,w",
                  cPoint(i1,i2,0)/w,cPoint(i1,i2,1)/w,cPoint(i1,i2,2)/w,cPoint(i1,i2,3)));
          if( line!="" ) 
	  {
	    sScanF(line,"%e %e %e %e",&cPoint(i1,i2,0),&cPoint(i1,i2,1),&cPoint(i1,i2,2),&cPoint(i1,i2,3));
	    cPoint(i1,i2,Range(0,2))*=cPoint(i1,i2,3);
	  }
	  
	}
        else
	{
          printf("Sorry: not implemented for domainDimension=%i rangeDimension=%i\n",domainDimension,rangeDimension);
	}
	
      }
      mappingHasChanged();
      initialized=false;
      uMin=uKnot(0);  // min(uKnot);
      uMax=uKnot(m1); // max(uKnot);
      initialize();
      // mappingNeedsToBeReinitialized=true; // re-init mapping and inverse
      reinitialize(); 
      plotObject=true;
    }
    else if( answer=="enter points" )
    { 
      // Define a NURBS by interpolating points

      int degree=-1;
      n1=0;
      n2=0;
      n3=0;
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter the number of points and the order of the nurb (default=3)"));
	if( line!="" ) sScanF(line,"%i %i",&n1, &degree);
	n1--;
	printf("Setting n1=%i, degree=%i\n",n1,degree);
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter nr1,nr2, order (the number of points in each direction and the order of the nurb (default=3)"));
	if( line!="" ) sScanF(line,"%i %i %i",&n1,&n2, &degree);
	n1--; n2--;
	printf("Setting n1=%i, n2=%i, degree=%i\n",n1,n2,degree);
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter nr1,nr2,nr3, order (the number of points in each direction and the order of the nurb (default=3)"));
	if( line!="" ) sScanF(line,"%i %i %i %i",&n1,&n2,&n3, &degree);
	n1--; n2--; n3--;
	printf("Setting n1=%i, n2=%i, n3=%i, degree=%i\n",n1,n2,n3,degree);
      }
	
      if( degree<=0 )
      {
	degree=3;
      }
	

      RealArray x;  // we save the points here
      if( domainDimension==1 )
      {
	x.redim(n1+1,rangeDimension);  

	for( int i1=0; i1<n1+1; i1++ )
	{
	  if( rangeDimension==2 )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point %i : (x1,x2)",i1));
	    if( line!="" ) sScanF(line,"%e %e ",&x(i1,0),&x(i1,1));
	  }
	  else
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point %i : (x1,x2,x3)",i1));
	    if( line!="" ) sScanF(line,"%e %e %e ",&x(i1,0),&x(i1,1),&x(i1,2));
	  }
	}
      }
      else if( domainDimension==2 )
      {
	x.redim(n1+1,n2+1,rangeDimension);  // we save the points here

	for( int i2=0; i2<n2+1; i2++ )
	for( int i1=0; i1<n1+1; i1++ )
	{
	  if( rangeDimension==2 )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i) : (x1,x2)",i1,i2));
	    if( line!="" ) sScanF(line,"%e %e ",&x(i1,i2,0),&x(i1,i2,1));
	  }
	  else
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i) : (x1,x2,x3)",i1,i2));
	    if( line!="" ) sScanF(line,"%e %e %e ",&x(i1,i2,0),&x(i1,i2,1),&x(i1,i2,2));
	  }
	}
      }
      else
      {
	x.redim(n1+1,n2+1,n3+1,rangeDimension);  // we can save the points here

	for( int i3=0; i3<n3+1; i3++ )
	for( int i2=0; i2<n2+1; i2++ )
	for( int i1=0; i1<n1+1; i1++ )
	{
	  if( rangeDimension==2 )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i,%i) : (x1,x2)",i1,i2,i3));
	    if( line!="" ) sScanF(line,"%e %e ",&x(i1,i2,i3,0),&x(i1,i2,i3,1));
	  }
	  else
	  {
	    gi.inputString(line,sPrintF(buff,"Enter point (%i,%i,%i) : (x1,x2,x3)",i1,i2,i3));
	    if( line!="" ) sScanF(line,"%e %e %e ",&x(i1,i2,i3,0),&x(i1,i2,i3,1),&x(i1,i2,i3,2));
	  }
	}
      }
      
      // *wdh* 090413 interpolate( x,0,Overture::nullRealArray(),degree);
      interpolate( x,0,Overture::nullRealArray(),degree,parameterizationType);

      // interpolate( x,0,Overture::nullRealDistributedArray(),degree);

      plotObject=true;
    }
    else if( answer=="set domain dimension" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the domain dimension (1,2, or 3) (current=%i)",domainDimension));
      if( line!="" ) sScanF(line,"%i",&domainDimension);
      domainDimension=max(1,min(3,domainDimension));

      initialized=false;
      plotObject=false;
    }
    else if( answer=="set range dimension" )
    {
      gi.inputString(line,sPrintF(buff,"Enter the range dimension (2 or 3) (current=%i)",rangeDimension));
      if( line!="" ) sScanF(line,"%i",&rangeDimension);
      rangeDimension=max(2,min(3,rangeDimension));

      initialized=false;
      plotObject=false;
      
    }
    else if( answer=="use robust inverse" )
    {
      approximateGlobalInverse->useRobustInverse(TRUE);
    }
    else if( answer=="do not use robust inverse" )
    {
      approximateGlobalInverse->useRobustInverse(FALSE);
    }
    else if( answer=="project to 2d" )
    {
      if( domainDimension!=1 || rangeDimension!=3 )
      {
        printf("project to 2d: This option only valid for 3D curves.\n");
	continue;
	
      }
      real t1[3]={1.,0.,0.}, t2[3]={0.,1.,0.};  //
      gi.inputString(line,"Enter the tangent vectors of the plane: x1,y1,z1, x2,y2,z2");
      if( line!="" ) sScanF(line,"%e %e %e %e %e %e",&t1[0],&t1[1],&t1[2], &t2[0],&t2[1],&t2[2]);

      printf(" Using: t1=[%9.3e,%9.3e,%9.3e] and t2=[%9.3e,%9.3e,%9.3e]\n",t1[0],t1[1],t1[2],t2[0],t2[1],t2[2]);
      
      Range R1(0,n1);
      RealArray cp(R1,3);
      
      cp(R1,axis1)=t1[0]*cPoint(R1,axis1)+t1[1]*cPoint(R1,axis2)+t1[2]*cPoint(R1,axis3);
      cp(R1,axis2)=t2[0]*cPoint(R1,axis1)+t2[1]*cPoint(R1,axis2)+t2[2]*cPoint(R1,axis3);
      cp(R1,axis3)=cPoint(R1,3); // weight

      cPoint.redim(0);
      cPoint=cp;
      

      rangeDimension=2;
      initialize();
      plotObject=true;
	    

    }
    else if( answer=="parameterize by chord length" )
    {
      parameterizationType=parameterizeByChordLength;
    }
    else if( answer=="parameterize by index (uniform)" )
    {
      parameterizationType=parameterizeByIndex;
    }
    else if( answer=="plot points on curves"  || answer=="do not plot points on curves" )
    {
      parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES,answer=="plot points on curves");
    }
    else if( answer=="plot sub curves" || answer=="do not plot sub curves" )
    {
      plotSubCurves=answer=="plot sub curves";
    }
    else if( answer=="plot control points" )
    {
      plotControlPoints=!plotControlPoints;
      plotObject=true;
    }
    else if( answer=="elevate the degree" )
    {
      if( domainDimension!=1 )
      {
	printf("NURBSMAPPING:Sorry: can only currently elevate the degree for curves\n");
	continue;
      }
      gi.inputString(line,sPrintF(buff,"Enter the increment"));
      int increment=1;
      if( line!="" ) sScanF(line,"%i",&increment);
      elevateDegree(increment);
    }
    else if( answer=="reparameterize" )
    {
      real ua=0., ub=1., va=0., vb=1.;
      ua=(0.-uKnot(0))/(uKnot(m1)-uKnot(0));
      ub=(1.-uKnot(0))/(uKnot(m1)-uKnot(0));
      if( domainDimension==1 )
        gi.inputString(line,sPrintF(buff,"Enter uMin,uMax (range [0,1], current=[%f,%f])",ua,ub));
      else
      {
	va=(0.-vKnot(0))/(vKnot(m2)-vKnot(0));
	vb=(1.-vKnot(0))/(vKnot(m2)-vKnot(0));
        gi.inputString(line,sPrintF(buff,"Enter uMin,uMax, vMin,vMax (range [0,1]x[0,1], current=[%f,%f]x[%f,%f])",
				    ua,ub,va,vb ));
      }
      if( line!="" )
      {
        if( domainDimension==1 )
          sScanF(line,"%e %e",&ua,&ub);
        else
          sScanF(line,"%e %e %e %e ",&ua,&ub,&va,&vb);
        reparameterize(ua,ub, va,vb );
      }
    }
    else if( answer=="reset reparameterization" )
    {
      // un = (u-ua)/(ub-ua)  : this is the formula to reparamterize
      // Thus:    u = un*(ub-ua) + ua 
      //            = (un- ua/(ub-ua))*(ub-ua)
      //            = (un -una)/(unb-una)
      // unb = una + 1/(ub-ua)
      // una = ua/(ub-ua), unb = (ua+1)/(ub-ua) 

      real ua=0., ub=1., va=0., vb=1.;
      real una=0., unb=1., vna=0., vnb=1.;
      ua=(0.-uKnot(0))/(uKnot(m1)-uKnot(0));
      ub=(1.-uKnot(0))/(uKnot(m1)-uKnot(0));
      una = -ua/(ub-ua); unb = (1.-ua)/(ub-ua);
      if( domainDimension>1 )
      {
	va=(0.-vKnot(0))/(vKnot(m2)-vKnot(0));
	vb=(1.-vKnot(0))/(vKnot(m2)-vKnot(0));
        vna = -va/(vb-va), vnb = (1.-va)/(vb-va);
      }
      
      printF(" setting [ua,ub]=[%g,%g], [va,vb]=[%g,%g]\n",una,unb,vna,vnb);
      
      reparameterize(una,unb, vna,vnb );
    }
    else if( answer=="restrict the domain" )
    {
      printf("By default the NURBS is parameterized on the interval [0,1]\n"
             "You may choose a sub-section of the spline by choosing a new interval [rStart,rEnd]\n"
             "For periodic NURBS the interval may lie in [-1,2] so the sub-section can cross the branch cut.\n");
 
      if( domainDimension==1 )
      {
	gi.inputString(line,sPrintF(buff,"Enter the new interval rStart,rEnd  current=[%6.2e,%6.2e]\n",
              rStart[0],rEnd[0]));
	if( line!="" )
	{
	  real rStart_=0., rEnd_=1.;
	  sScanF(line,"%e %e",&rStart_,&rEnd_);
	  setDomainInterval(rStart_,rEnd_);
	  mappingHasChanged();
	}
      }
      else if( domainDimension==2 )
      {
	gi.inputString(line,sPrintF(buff,"Enter the new interval ra,rb, sa,sb, current=[%6.2e,%6.2e]x[%6.2e,%6.2e]n",
              rStart[0],rEnd[0],rStart[1],rEnd[1]));
	if( line!="" )
	{
	  real r1Start_=0., r1End_=1., r2Start_=0., r2End_=1.;
	  sScanF(line,"%e %e %e %e",&r1Start_,&r1End_,&r2Start_,&r2End_);
	  setDomainInterval(r1Start_,r1End_,r2Start_,r2End_);
	  mappingHasChanged();
	}
      }
      else
        {throw "error";}
      
    }
/* ---
    else if( answer=="do not normalize knots" )
    {
      real ua=uKnot(0);  // min(uKnot);
      real ub=uKnot(m1); // max(uKnot);
      real uba=ub-ua;
      if( (ua!=uMin || ub!=uMax) && uba!=0.  )
      {
        uKnot=uMin+(uKnot-ua)*((uMax-uMin)/uba);
      }
      if( domainDimension>1 )
      {
	real va=vKnot(0); // min(vKnot);
	real vb=vKnot(m2); // max(vKnot);
	real vba=vb-va;
	if( (va!=vMin || vb!=vMax) && vba!=0.  )
	{
	  vKnot=vMin+(vKnot-va)*((vMax-vMin)/vba);
	}
      }
      initialize();
      reinitialize(); 
    }
--- */
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

    else if( answer=="circle" )
    {
      
      RealArray o(3),x(3),y(3);
      real r=.5;
      real startAngle=0., endAngle=1.;
      o=0.;
      x=0.; x(0)=1.;
      y=0.; y(1)=1.;
      const real conversion=360.;
      
      printf("A circle is specified by a center point, two basis vectors that define the plane of the circle,\n"
             "a radius, plus a startAngle and endAngle as measured from the first basis vector\n"
              "The current circle has \n"
               " centre=(%7.2e,%7.2e,%7.2e), radius=%e, startAngle=%7.3e (degrees) endAngle=%7.3e (degrees)\n"
               " vector1=(%7.2e,%7.2e,%7.2e), vector2=(%7.2e,%7.2e,%7.2e)\n",
                  o(0),o(1),o(2),r,startAngle*conversion,endAngle*conversion,
                  x(0),x(1),x(2),y(0),y(1),y(2));
      aString menu2[]=
      {
        "!circle",
	"centre",
        "radius",
        "angles",
        "basis vectors",
        "done",
	"exit",
        ""
      };

      for( ;; )
      {
        circle(o,x,y,r,startAngle,endAngle);

        gi.erase();
        plot(gi,parameters,true);

	
	gi.getMenuItem(menu2,answer,"Choose an item");
        if( answer=="done" || answer=="exit" )
	{
	  break;
	}
	else if( answer=="centre" || answer=="center" )
	{
	  gi.inputString(answer,sPrintF(buff,"Enter the center (current=(%7.2e,%7.2e,%7.2e)",o(0),o(1),o(2)));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e %e %e",&o(0),&o(1),&o(2));
	  }
	}
	else if( answer=="radius" )
	{
	  gi.inputString(answer,sPrintF(buff,"Enter the radius (current=(%7.2e)",r));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e ",&r);
	  }
	}
	else if( answer=="angles" )
	{
	  gi.inputString(answer,sPrintF(buff,"Enter the start and end angles (degrees) (current=(%7.2e,%7.2e)",
                 startAngle*conversion,endAngle*conversion));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e %e",&startAngle,&endAngle);
            startAngle/=conversion;
	    endAngle/=conversion;
	  }
	}
	else if( answer=="basis vectors" )
	{
	  gi.inputString(answer,sPrintF(buff,"Enter the basis vectors (6 numbers)"));
	  if( answer!="" )
	  {
	    sScanF(answer,"%e %e %e %e %e %e",&x(0),&x(1),&x(2),&y(0),&y(1),&y(2));
	    printf(" vector1=(%7.3e,%7.3e,%7.3e) vector2=(%7.3e,%7.3e,%7.3e) \n",
               x(0),x(1),x(2),y(0),y(1),y(2));
	  }
	}
	else
	{
	  gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
	  gi.stopReadingCommandFile();
	}
	
      }
    }
    else if( answer=="conic" )
    {
      RealArray pt0(1,3),t0(1,3),pt2(1,3),t2(1,3),p(1,3);
      
      printf("Here is a test for building open conic arcs\n");

      aString menu2[]=
      {
        "parabola",
        "parabola2",
	"ellipse",
	"ellipse2",
        "hyperbola",
        "hyperbola2",
        "done",
	"exit",
        ""
      };

      for( int it=0; ; it++ )
      {
        if( it>0 )
	{

	  gi.erase();
	  plot(gi,parameters,true);

	
	  gi.getMenuItem(menu2,answer,"Choose an item");
	}
	else
	{
	  answer="parabola";
	}
	
        if( answer=="done" || answer=="exit" )
	{
	  break;
	}
	else if( answer=="parabola" )
	{
          // x=t         t=[-1,1]
          // y=t^2  
#define XP(t) (t)
#define YP(t) (t)*(t)
#define ZP(t) 0.

          pt0(0,0)=XP(-1.);
          pt0(0,1)=YP(-1.);
          pt0(0,2)=ZP(-1.);
	  
          pt2(0,0)=XP( 1.);
          pt2(0,1)=YP( 1.);
          pt2(0,2)=ZP( 1.);
	  
          p(0,0)=XP( 0.);
          p(0,1)=YP( 0.);
          p(0,2)=ZP( 0.);
	  
          t0(0,0)=1.;
          t0(0,1)=-2.;
          t0(0,2)=0.;
          t0/=sqrt(sum(t0*t0));

          t2(0,0)=1.;
          t2(0,1)=2.;
          t2(0,2)=0.;
          t2/=sqrt(sum(t2*t2));
	  
	  conic( pt0,t0,pt2,t2,p );

	}
        else if( answer=="parabola2" )
	{
	  //   a*x^2 + b*x*y + c*y^2 + d*x + e*y + f = 0
          real a=0.,b=0.,c=0.,d=0.,e=0.,f=0.,x1=0.,y1=0.,x2=0.,y2=0.,z=0.;
	  
          // x^2 - y = 0
          a=1.;
	  e=-1.;
	  
          x1=XP(-1.);
          y1=YP(-1.);

          x2=XP( 1.);
          y2=YP( 1.);

          conic( a,b,c,d,e,f,z,x1,y1,x2,y2 );
	}
	else if( answer=="ellipse" )
	{
          const real ae=1., be=2.;

	  // x = a*cos(Pi*t)
          // y = b*sin(Pi*t)   0<= t <= 1
#undef XP
#undef YP
#undef ZP
#define XP(t) ae*cos(Pi*(t))
#define YP(t) be*sin(Pi*(t)) 
#define ZP(t) 0.

          real ta=.1;
	  real tb=.9;

          pt0(0,0)=XP(ta);
          pt0(0,1)=YP(ta);
          pt0(0,2)=ZP(ta);
	  
          pt2(0,0)=XP(tb);
          pt2(0,1)=YP(tb);
          pt2(0,2)=ZP(tb);
	  
          p(0,0)=XP(.5);
          p(0,1)=YP(.5);
          p(0,2)=ZP(.5);
	  
          t0(0,0)=-ae*sin(Pi*ta);
          t0(0,1)= be*cos(Pi*ta);
          t0(0,2)=0.;
          t0/=sqrt(sum(t0*t0));

          t2(0,0)=-ae*sin(Pi*tb);
          t2(0,1)= be*cos(Pi*tb);
          t2(0,2)=0.;
          t2/=sqrt(sum(t2*t2));
	  
          // ** p=-p;  // flip sign to get other part of the ellipse
	  

	  conic( pt0,t0,pt2,t2,p );

	}
        else if( answer=="ellipse2" )
	{
          const real ae=1., be=2.;

	  //   a*x^2 + b*x*y + c*y^2 + d*x + e*y + f = 0
          real a=0.,b=0.,c=0.,d=0.,e=0.,f=0.,x1=0.,y1=0.,x2=0.,y2=0.,z=0.;


          // x^2 - y = 0

          real ta=.1;
	  real tb=.9;
	  
          x1=XP(ta);
          y1=YP(ta);

          x2=XP(tb);
          y2=YP(tb);

          a=1./(ae*ae);
	  c=1./(be*be);
          f = -1.;
	  
          conic( a,b,c,d,e,f,z,x1,y1,x2,y2 );
	}
	else if( answer=="hyperbola" )
	{
          // y^2 - x^2 =  1
	  // x = t 
          // y = sqrt(1+x^2)   -1 <= t <= 1
#undef XP
#undef YP
#undef ZP
#define XP(t) (t)
#define YP(t) sqrt(1.+(t)*(t))
#define ZP(t) 0.
#define YPT(t) (t)/sqrt(1.+(t)*(t))
          pt0(0,0)=XP(-1.);
          pt0(0,1)=YP(-1.);
          pt0(0,2)=ZP(-1.);
	  
          pt2(0,0)=XP( 1.);
          pt2(0,1)=YP( 1.);
          pt2(0,2)=ZP( 1.);
	  
          p(0,0)=XP( 0.);
          p(0,1)=YP( 0.);
          p(0,2)=ZP( 0.);
	  
          t0(0,0)=1.;
          t0(0,1)=YPT(-1.);
          t0(0,2)=0.;
          t0/=sqrt(sum(t0*t0));

          t2(0,0)=1.;
          t2(0,1)=YPT(1.);
          t2(0,2)=0.;
          t2/=sqrt(sum(t2*t2));
	  conic( pt0,t0,pt2,t2,p );

	}

        else if( answer=="hyperbola2" )
	{
	  //   - x^2 + y^2  - 1 = 0 
          real a=0.,b=0.,c=0.,d=0.,e=0.,f=0.,x1=0.,y1=0.,x2=0.,y2=0.,z=0.;

          a=-1.;
	  c=1.;
	  f=-1.;

          real ta=-1.;
	  real tb=1.;
	  
          x1=XP(ta);
          y1=YP(ta);

          x2=XP(tb);
          y2=YP(tb);

          conic( a,b,c,d,e,f,z,x1,y1,x2,y2 );
	}

	else
	{
	  gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
	  gi.stopReadingCommandFile();
	}
	
      }
#undef XP
#undef YP
#undef ZP
    }
    else if(answer=="interpolate lofted surface")
    {
      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i,j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	// kkc 051031 removed the domainDimension check since we can build volumes now
	if(map.getDomainDimension()==1 && map.getRangeDimension()==3 && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="done"; 
      menu2[j]="";   // null string terminates the menu
	
      vector<Mapping*> curves;
      while(true)
	{
	  int mapNumber = gi.getMenuItem(menu2,answer2,"choose a mapping");
	  assert( mapNumber>=0 && mapNumber<j );
	  if( answer2=="done" )
	    break;

	  curves.push_back(&mapInfo.mappingList[subListNumbering(mapNumber)].getMapping());
	}

      int degree1=3,degree2=3;
      int numberOfGhostPoints=0;
      parameterizationType=parameterizeByChordLength;
      {
	aString menu2[] = {"parameterize by chord length",
			   "parameterize by index (uniform)",
			   "degree",
			   "done",
			   ""};
	
	int len=0;
	for( ;; )
	  {
	    gi.getMenuItem(menu2,answer2,"choose an option");
	    if( answer2=="done" )
	      {
		break;
	      }
	    else if( answer2=="parameterize by chord length" )
	      {
		parameterizationType=parameterizeByChordLength;
	      }
	    else if( answer2=="parameterize by index (uniform)" )
	      {
		parameterizationType=parameterizeByIndex;
	      }
	    else if ( len=answer2.matches("degree") )
	      {
		sScanF(answer2(len,answer2.length()-1),"%i %i",&degree1, &degree2);
	      }
	  }
      }
      interpolateLoftedSurface(curves, degree1,degree2,parameterizationType, numberOfGhostPoints
);
      curves.clear();
    }
    else if( answer=="interpolate from a mapping" ||
             answer=="interpolate from mapping with options" )
    {
      // Make a menu with the Mapping names
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i,j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	// kkc 051031 removed the domainDimension check since we can build volumes now
	if( /*(map.getDomainDimension()==1 || map.getDomainDimension()==2 ) 
	      &&*/ map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      int mapNumber = gi.getMenuItem(menu2,answer2,"choose a mapping");

      if( answer2=="none" )
        continue;

      assert( mapNumber>=0 && mapNumber<j );

      delete [] menu2;
      
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      Mapping & map = mapInfo.mappingList[mapNumber].getMapping();

      // By default we interpolate the mapping using the same number of grid points as the mapping.
      // 
      int ni[3]={1,1,1};
      for( int dir=0; dir<map.getDomainDimension(); dir++ )
      {
	ni[dir]=map.getGridDimensions(dir);
      }
      

      int degree=3;
      int numberOfGhostPoints=0;
      if( answer=="interpolate from mapping with options" )
      {
        
        const aString menu2[]={"parameterize by chord length",
                               "parameterize by index (uniform)",
                               "choose degree",
                               "number of ghost points to include",
                               "number of points to interpolate",
                               "done",""};  //
        for( ;; )
	{
          gi.getMenuItem(menu2,answer2,"choose an option");
          if( answer2=="done" )
	  {
	    break;
	  }
	  else if( answer2=="parameterize by chord length" )
	  {
            parameterizationType=parameterizeByChordLength;
	  }
          else if( answer2=="parameterize by index (uniform)" )
	  {
            parameterizationType=parameterizeByIndex;
	  }
	  else if( answer2=="choose degree" )
	  {
            gi.inputString(answer2,sPrintF("Enter the degree (default=%i)",degree));
	    sScanF(answer2,"%i",&degree);
            printF("Using degree =%i\n",degree);
	  }
	  else if( answer2=="number of ghost points to include" )
	  {
            printF("INFO:You may specify how many ghost points from the source Mapping to be interpolated\n"
                   " onto the target NurbsMapping. In this way the ghost points of the NurbsMapping will match\n"
                   " the source Mapping.\n");
            gi.inputString(answer2,sPrintF("Enter the number of ghost points to include (default=%i)",
                       numberOfGhostPoints));
	    sScanF(answer2,"%i",&numberOfGhostPoints);
            printF("Using number of ghost points =%i\n",numberOfGhostPoints);
	  }
          else if( answer2=="number of points to interpolate" )
	  {
            printF("INFO: By default the target mapping will be interpolated with the number of grid points\n"
                   " specified in the source mapping. You may change the number of points used to interpolate here.\n"
                   " This can save space and time by defining the Nurbs with fewer control points. The number\n"
                   " of grid points in the target mapping will still equal that of the source even though the\n"
                   " number of control points will be different.\n");
	    if( map.getDomainDimension()==1 )
	    {
	      gi.inputString(answer2,sPrintF("Enter the number of points to interpolate: n1 (default=%i)",
					     ni[0]));
	      sScanF(answer2,"%i",&ni[0]);
	      printF("Will interpolate with %i points\n",ni[0]);
	    }
	    else if( map.getDomainDimension()==2 )
	    {
	      gi.inputString(answer2,sPrintF("Enter the number of points to interpolate: n1,n2 (default=%i,%i)",
					     ni[0],ni[1]));
	      sScanF(answer2,"%i %i",&ni[0],&ni[1]);
	      printF("Will interpolate with %i, %i points\n",ni[0],ni[1]);
	    }
	    else
	    {
	      gi.inputString(answer2,sPrintF("Enter the number of points to interpolate: n1,n2,n3 (default=%i,%i,%i)",
					     ni[0],ni[1],ni[2]));
	      sScanF(answer2,"%i %i %i",&ni[0],&ni[1],&ni[2]);
	      printF("Will interpolate with %i, %i, %i points\n",ni[0],ni[1],ni[2]);
	    }
	    
	  }
	  
	  else
	  {
	    printf("Unknown response=%s",(const char*)answer2);
	    gi.stopReadingCommandFile();	  
	  }
	}
      }
      

      interpolate( map,degree,parameterizationType,numberOfGhostPoints,ni );
      
    }
    else if( answer=="merge" )
    {
            // Make a menu with the Mapping names, NURBS curves only
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i,j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==1 && map.mapPointer!=this &&
            map.getClassName()=="NurbsMapping" )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      int mapNumber = gi.getMenuItem(menu2,answer2,"merge with which mapping?");

      if( answer2=="none" )
        continue;

      assert( mapNumber>=0 && mapNumber<j );

      delete [] menu2;
      
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      NurbsMapping & map = (NurbsMapping&) mapInfo.mappingList[mapNumber].getMapping();

      merge(map);

    }
    else if ( answer=="use Eleven eval" )
      {
	use_kk_nrb_eval = true;
      }
    else if ( answer=="do not use Eleven eval" )
      {
	use_kk_nrb_eval = false;
      }
    else if( answer=="general cylinder" )
    {
            // Make a menu with the Mapping names, NURBS curves only
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i,j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==1 && map.getRangeDimension()==3 && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      int mapNumber = gi.getMenuItem(menu2,answer2,"Extrude which curve?");

      if( answer2=="none" )
        continue;

      assert( mapNumber>=0 && mapNumber<j );

      delete [] menu2;
      
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      Mapping & map = mapInfo.mappingList[mapNumber].getMapping();

      gi.inputString(line,"Enter a direction vector d1,d2,d3");
      real d[3]={1.,0.,0.}; //
      sScanF(line,"%e %e %e",&d[0],&d[1],&d[2]);
      printf(" Using direction vector=(%9.3e,%9.3e,%9.3e)\n",d[0],d[1],d[2]);

      generalCylinder(map,d);

    }
    else if( answer=="show parameters" )
    {
      NurbsMapping::display();
    }
    else if( answer(0,14)=="save NURBS data" )
    {
      gi.inputString(answer2,"Enter the file to save the data in");
      if( answer2!="" )
        put(answer2,answer=="save NURBS data, format xwxw" ? xwxw : 
                    answer=="save NURBS data, format xxww" ? xxww : cheryl);
    }
    else if(  answer(0,14)=="read NURBS data" )
    {
      gi.inputString(answer2,"Enter the file to read the data from");
      if( answer2!="" )
      {
        int returnValue;
        returnValue=get(answer2,answer=="read NURBS data, format xwxw" ? xwxw : 
                        answer=="read NURBS data, format xxww" ? xxww : cheryl);
        if( returnValue!=0 )
	  gi.stopReadingCommandFile();
      }
    }
    else if( answer=="save points in matlab format" )
    {
      aString fileName="nurbs.m";
      gi.inputString(fileName,"Enter the name of the matlab file (e.g. nurbs.m)");
      FILE *file = fopen((const char*)fileName,"w");
      if( file ==NULL )
      {
	printF("ERROR: unable to open the file %s\n",(const char*)fileName);
	continue;
      }
      const realArray & x = getGrid();
      if( domainDimension==1 )
      {
	for( int dir=0; dir<rangeDimension; dir++ )
	{
	  fprintf(file,"x%i=[",dir);
	  for( int i1=x.getBase(0); i1<=x.getBound(0); i1++ )
	  {
	    fprintf(file," %18.12e \n",x(i1,0,0,dir));
	  }
	  fprintf(file,"];\n");
	}
	
      }
      else
      {
	printF("ERROR: save points in matlab format not yet implemented for domainDimension>1 \n");
      }
      fclose(file);
      printF("Wrote file %s\n",(const char*)fileName);
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    }
//     else if( answer=="make a coordinate curve" )
//     {
//       NurbsMapping curve;

//       for( ;; )
//       {
// 	real r0=0.;
// 	real r1=-1.;
// 	cout << "enter r0 and r1 (one must be -1., enter -1. -1. to stop)\n";
//         cin >> r0 >> r1;
//         if( r0<-.5 && r1<-.5 )
//          break;
	
// 	buildCurveOnSurface(curve,r0,r1);

// 	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);

// 	parameters.set(GI_MAPPING_COLOUR,"green");
// 	real oldCurveLineWidth;
// 	parameters.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	parameters.set(GraphicsParameters::curveLineWidth,3.);
// 	PlotIt::plot(gi,curve,parameters);
// 	parameters.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
// 	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
//       }
      
//     }

//    This is now done above
//     else if( answer=="check inverse" )
//     {
//       realArray x(2,3),r(1,3),xx(1,3),xxr(1,3,2),n(1,3),nn(11,3);
//       Range Rx(0,2);
//       aString menu2[]=
//       {
// 	"enter a point",
//         "exit",
//         ""
//       };
//       Mapping::debug=7;
      
//       for( int i=0;;i++ )
//       {
//         gi.getMenuItem(menu2,answer,"choose");
// 	if( answer=="enter a point" )
// 	{
// 	  gi.inputString(answer,"Enter a point (x,y,z) to invert (null string to terminate)");
// 	  if( answer!="" )
// 	  {
// 	    sScanF(answer,"%e %e %e",&x(0,0),&x(0,1),&x(0,2));
//             r=-1;
// 	    inverseMap(x(0,Rx),r);
// 	    map(r,xx,xxr);
// 	    x(1,Rx)=xx(0,Rx);
// 	    printf(" x=(%6.2e,%6.2e,%6.2e), r=(%6.2e,%6.2e), projected x=(%6.2e,%6.2e,%6.2e)\n",
// 		   x(0,0),x(0,1),x(0,2), r(0,0),r(0,1), x(1,0),x(1,1),x(1,2));
// 	    gi.erase();
// 	    PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
// 	    parameters.set(GI_USE_PLOT_BOUNDS,true);
// 	    parameters.set(GI_POINT_SIZE,(real)6.);
// 	    gi.plotPoints(x,parameters);

//             // plot the normal
// 	    n(0,0)=xxr(0,1,0)*xxr(0,2,1)-xxr(0,2,0)*xxr(0,1,1);
// 	    n(0,1)=xxr(0,2,0)*xxr(0,0,1)-xxr(0,0,0)*xxr(0,2,1);
// 	    n(0,2)=xxr(0,0,0)*xxr(0,1,1)-xxr(0,1,0)*xxr(0,0,1);

// 	    real l2Norm=SQRT(SQR(n(0,0))+SQR(n(0,1))+SQR(n(0,2)));
//             n/=l2Norm;
//             real dist=SQRT(sum(SQR(xx(0,Rx)-x(0,Rx))));
// 	    for( int i=0; i<11; i++ )
// 	    {
//               real alpha=i*.1;
// 	      nn(i,Rx)=xx(0,Rx)+alpha*dist*n(0,Rx);
// 	    }
// 	    parameters.set(GI_POINT_SIZE,(real)4.);
// 	    gi.plotPoints(nn,parameters);
	    

// 	    parameters.set(GI_USE_PLOT_BOUNDS,false);
// 	  }
// 	}
// 	else if( answer=="exit" )
//           break;
//       }
//     }
    else if( answer=="plot first derivatives" || answer=="plot second derivatives" )
    {
      if( domainDimension!=1 )
      {
	gi.outputString("Sorry: Can only plot derivatives of curves");
	continue;
      }
      
      const int order = answer=="plot first derivatives" ? 1 : 2;
      if( order==1 )
        parameters.set(GI_TOP_LABEL,"First derivatives: xr=green, yr=red");
      else
        parameters.set(GI_TOP_LABEL,"Second derivatives: xrr=green, yrr=red");

      gi.erase();
      const int n = getGridDimensions(0);
      real dr = 1./max(1,n-1);
      realArray r(n,1);
      r.seqAdd(0.,dr);
      
      realArray x(n,rangeDimension),xr(n,rangeDimension,1);
      if( order==1 )
        map(r,x,xr);
      else
      {
        Index I = Range(n);
	for(  int axis=0; axis<rangeDimension; axis++ )
	  secondOrderDerivative(I,r,xr,axis,0);
      }
    
      real xrMax=max(fabs(xr));
      xr*=1./max(REAL_MIN*100.,xrMax);
      printf("Derivative of spline function scaled by %8.2e\n",xrMax);
      
      realArray xrd(n,1,1,2);
      Range R=n;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xrd(R,0,0,0)=r(R,0);
	xrd(R,0,0,1)=xr(R,axis);
      
	DataPointMapping xrMap;
	xrMap.setDataPoints(xrd,3,1);
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
             answer=="check" ||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      const int nxOld=getGridDimensions(axis1);

      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="periodicity" )
      {
        initialized=false;
	for( int axis=0; axis<domainDimension; axis++ )
	{
	  if( fabs(rEnd[axis]-rStart[axis])==1. )
	    nurbsIsPeriodic[axis]=getIsPeriodic(axis);
	}
      }
      // Since the plotter plots the sub-curves we change the resolution there too if the lines
      // have increased; so that we get an improved plot.
      if( answer=="lines" && domainDimension==1 )
      {
        real ratio=real(getGridDimensions(axis1))/max(1,nxOld);
        printf("Increase resolution on sub-curves, ratio=%8.2e\n",ratio);
	
        if( ratio>1. )
	{
	  for( int sc=0; sc<numberOfSubCurves(); sc++ )
	  {
	    int nx=subCurve(sc).getGridDimensions(0);
	    
	    subCurve(sc).setGridDimensions(axis1,int(nx*ratio+.5));
            // printf(" sc=%i nx=%i new=%i, newGrid=%i\n",sc,nx,int(nx*ratio+.5),subCurve(sc).getGrid().getLength(0));
	    
	  }
	}
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( plotObject )
    {
      gi.erase();
      if( plotSubCurves )
      {
	aString originalColour;
	parameters.get(GI_MAPPING_COLOUR, originalColour);

	for ( int sc=0; sc<numberOfSubCurves(); sc++ )
	{
	  parameters.set(GI_MAPPING_COLOUR, gi.getColourName(sc));
	  if ( !isSubCurveHidden(sc) )
	    subCurve(sc).plot( gi, parameters, plotControlPoints );
	}
	parameters.set(GI_MAPPING_COLOUR, originalColour);
      }
      else
      {
	int plotAsSubCurves;
	parameters.get(GI_PLOT_NURBS_CURVES_AS_SUBCURVES,plotAsSubCurves);
	parameters.set(GI_PLOT_NURBS_CURVES_AS_SUBCURVES,false);
      
	plot(gi,parameters,plotControlPoints);

	parameters.set(GI_PLOT_NURBS_CURVES_AS_SUBCURVES,plotAsSubCurves);  // reset
      }
    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}


int NurbsMapping::
interpolateLoftedSurface(vector<Mapping*> &curves, int degree1 /*=3*/, 
			 int degree2 /*=3*/ ,
			 ParameterizationTypeEnum  parameterizationType /* =parameterizeByChordLength */,
			 int numberOfGhostPoints /* =0 */)
//===========================================================================
/// \details 
///     Interpolate a list of curves to make a lofted surface.  Note this actually
///     interpolates each curve evaluated at a number of points; an actual Nurbs lofted
///     surface could be created using a list of Nurbs curves but we don't do that 
///     right now.
/// 
/// \param curves (input) : a list of mappings to interpolate
//===========================================================================
{
  if ( curves.size()<2 ) 
    {
      printF("NurbsMapping::interpolateLoftedSurface:: ERROR : at least 2 curves must be provided");
      return -1;
    }
  
  int maxN = 0;
  // first check to make sure the curves are actually curves and find out some basic information
  for ( vector<Mapping*>::iterator curve=curves.begin(); curve!=curves.end(); curve++ )
    {
      Mapping &map = **curve;
      if ( map.getDomainDimension()!=1 || map.getRangeDimension()!=3 ) 
	{
	  printF("NurbsMapping::interpolateLoftedSurface:: ERROR : all curves must have domainDimension==1 and rangeDimension==3");
	  return -1;
	}

      maxN = max(maxN, map.getGridDimensions(0));
    }

  // now build the points to interpolate
  RealArray x(maxN, curves.size(),3);

  for ( int c=0; c<curves.size(); c++ )
    {
      Mapping &map = *curves[c];
      map.setGridDimensions(0,maxN);

      const realArray &map_x = map.getGrid();
      for ( int a=0; a<3; a++ )
	for ( int n=0; n<maxN; n++ )
	  x(n,c,a) = map_x(n,a);
    }

  interpolateSurface(x,degree1,parameterizationType, numberOfGhostPoints, degree2);

  return 0;
}

