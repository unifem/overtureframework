// #define BOUNDS_CHECK
#include "TrimmedMapping.h"
#include "QuadTree.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "arrayGetIndex.h"
#include "display.h"
#include <float.h>
#include "NurbsMapping.h"
#include "Inverse.h"
#include "GenericGraphicsInterface.h"
#include "TriangleWrapper.h"
#include "UnstructuredMapping.h"
#include "IntersectionMapping.h"
#include "Geom.h"
#include "DataPointMapping.h"
#include "ParallelUtility.h"

real TrimmedMapping::defaultMinAngleForTriangulation=-1.;  // -1 => use default (20) : e.g. reduce to 5 for fewer triangles
real TrimmedMapping::defaultElementDensityToleranceForTriangulation=.05;
real TrimmedMapping::defaultMaximumAreaForTriangulation=0.;

real TrimmedMapping::defaultFarthestDistanceNearCurve(0.01);
// ... reasonable numbers are .01-.001

//! Build an outer curve for a surface without one.
/*!
 */
// AP: Build the outer curve out of four straight subcurves
void
constructOuterBoundaryCurve(NurbsMapping *newNurb)
{
  int m,n,p;
  p = 1; // linear segments
  n = 1; // 2 control points (0,1)
  m = n+p+1; // 4 knots (0,1,2,3)

  RealArray cPoints(n+1,3);
  RealArray knots(m+1);
  
  // set the control point weights
  cPoints(0,2) = cPoints(1,2) = 1.;
  
  cPoints(0,0) = cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 0.;

  knots(0) = knots(1) = 0;
  knots(2) = knots(3) = 1.;

// first segment
  cPoints(0,0) = cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 0.;

  newNurb->specify(m,n,p,knots,cPoints,2);

// add in subcurves
  NurbsMapping subCurve;
  
// second segment
  cPoints(0,0) = 1.; cPoints(0,1) = 0.;
  cPoints(1,0) = 1.; cPoints(1,1) = 1.;

  subCurve.specify(m,n,p,knots,cPoints,2);

//add in the subcurve
  newNurb->merge(subCurve);

// third segment
  cPoints(0,0) = 1.; cPoints(0,1) = 1.;
  cPoints(1,0) = 0.; cPoints(1,1) = 1.;

  subCurve.specify(m,n,p,knots,cPoints,2);

//add in the subcurve
  newNurb->merge(subCurve);

// fourth segment
  cPoints(0,0) = 0.; cPoints(0,1) = 1.;
  cPoints(1,0) = 0.; cPoints(1,1) = 0.;

  subCurve.specify(m,n,p,knots,cPoints,2);

//add in the subcurve
  newNurb->merge(subCurve);


// *wdh* 010811 : set a reasonable number of points
  newNurb->setGridDimensions(axis1,41);  
};

static int
connectCurveEnds( Mapping &curve )
{
  if ( curve.getIsPeriodic(axis1)!=Mapping::notPeriodic ) // *wdh* 010427 don't force periodic if already periodic
    return 0;

  realArray r(2,1);
  r(0,0) = 0.0;
  r(1,0) = 1.0;
  
  realArray x(2,2);
  curve.map(r,x);

  int status = 0;
  Range AXES(curve.getRangeDimension());
  real tol = 0.0001; // make the tolerance for forced periodicity 
  
  real arclength = curve.getArcLength();

  tol = .001*arclength; // *kkc* 260902 use a tolerance relative to the curve's arclength rather than an absolute tolerance

  real maxdiff = max(fabs(x(1,AXES) - x(0,AXES)));
  
  if( maxdiff<REAL_EPSILON*10. ) 
  {
    curve.setIsPeriodic(axis1,Mapping::functionPeriodic);  // *wdh* i think this is ok
  }
  else if( maxdiff<tol )
  {
    if( curve.getClassName() == "NurbsMapping" )
    {
      if( true || Mapping::debug & 2 )
	cout<<"connectCurveEnds:: forced periodicity required, maxdiff=" << maxdiff << endl;

      status = ((NurbsMapping &)curve).forcePeriodic();

      if( status || Mapping::debug & 2 )
	cout<<"connectCurveEnds:: forcePeriodic failed!"<< endl;

    }
    else 
      status = 1;
  }
  else if ( maxdiff>tol )
    status = 1;
  
  if ( Mapping::debug & 2 )
    (x(1,AXES) - x(0,AXES)).display("curve end diffs");

  return status;
}

static real
getArea( Mapping & curve )
// =============================================================================
// /Description:
//     Compute the area inside a close curve.
// =============================================================================
{
#define X(i,m) xyp[(i)+xyDim0*(m)]

  const realArray & x = curve.getGrid();
  real area=0.;
  const int xyDim0 = x.getRawDataSize(0);
  real *xyp = x.Array_Descriptor.Array_View_Pointer3;

  // area of a polygon = +/- (1/2) sum{ x_i y_{i+1} - x_{i+1} y_i }
  const int base0=x.getBase(0), bound0=x.getBound(0);
  area=0.;
  for( int i=base0; i<bound0; i++ )
  {
    area+= X(i,0)*X(i+1,1) - X(i+1,0)*X(i,1);    
  }
  area*=-.5;
#undef X  

  if ( area!=area )// check for nans (nans are not equal to anything, even themselves)!
    {
      cout<<"ERROR : NAN AREA FOR CURVE "<<endl;
      x.display("strange X");
    }

  return area;
  
}



static bool
curveIsCounterClockwise( Mapping &curve )
// estimate the area, return false if sign is +ve, true otherwise
{
  real area=getArea(curve);
//   int n= 100;
//   realArray r(n,1),x(n,2);
//   real dr=1./(n-1);
//   r.seqAdd(0.,dr);
//   curve.map(r,x);
//   real area = 0.0;
//   for ( int nn=0; nn<n-1; nn++ )
//     area += .5*(x(nn,1)+x(nn+1,1)) * (x(nn+1,0)-x(nn,0));

  if (area>0.0) 
    return false;
  else
    return true;
}

void 
getAreaAndArcLength( Mapping & curve, real & area, real & arcLength, real & scale0, real & scale1 )
// =========================================================================================
// estimate the area, and arclength, and length scales
// =========================================================================================
{
  arcLength=curve.getArcLength();
  

//   int n= 100;
//   realArray r(n,1),x(n,2);
//   real dr=1./(n-1);
//   r.seqAdd(0.,dr);
//   curve.map(r,x);

//   area = 0.0;
//   int nn;
//   for ( nn=0; nn<n-1; nn++ )
//     area += (x(nn,1)+x(nn+1,1)) * (x(nn+1,0)-x(nn,0));

//   area*=.5;

//   arcLength=0.;
//   for ( nn=0; nn<n-1; nn++ )
//     arcLength+=SQRT( SQR(x(nn+1,0)-x(nn,0))+SQR(x(nn+1,1)-x(nn,1)) );

  area=getArea(curve);
  
  const realArray & x = curve.getGrid();
  Range R=x.dimension(0);
  scale0=max(x(R,0,0,0))-min(x(R,0,0,0));
  scale1=max(x(R,0,0,1))-min(x(R,0,0,1));
}

static bool
positiveActiveSurfaceArea( const int & nCurves, Mapping ** &curves )
{
  real area = 0.0;
  for ( int c=0; c<nCurves; c++ )
  {
    Mapping & curve = * ( curves[c] );
    area+=getArea(curve);

//     int n= 100;
//     realArray r(n,1),x(n,2);
//     real dr=1./(n-1);
//     r.seqAdd(0.,dr);
//     curve.map(r,x);
//     for ( int nn=0; nn<n-1; nn++ ) 
//       area += .5*(x(nn,1)+x(nn+1,1)) * (x(nn+1,0)-x(nn,0));
  }

  //kkc 040213 make sure the area is larger than some amount in parameter space
  //           this watches for trim curves that are on top of each other
  //return (area<0) ? true : false; // the area < 0 for a counter clockwise curve.
  // kkc 040409 made this larger  return (area<(-1e-03)) ? true : false; // the area < 0 for a counter clockwise curve.

  // kkc 040409 it may be better to check area/arcLength..., if so, does the tol in curveTooNarrow need to be larger?
  return (area<(-1e-8)) ? true : false; // the area < 0 for a counter clockwise curve.
// All clockwise curves must be surrounded by a counter clockwise curve. This means
// that the variable "area" should be negative when the activeSurfaceArea is positive.

}

bool 
curveTooNarrow(Mapping &curve)
{
  real area, arcLength, scale0,scale1;
  getAreaAndArcLength( curve, area, arcLength, scale0, scale1 );
  
  area=fabs(area);
  real ratio=sqrt(area)/arcLength;
  bool tooNarrow= ratio<0.01;  
  if( tooNarrow )
    printF("TrimmedMapping:curveTooNarrow:INFO: curve is too narrow: area=%8.2e, arcLength=%8.2e\n",area,arcLength);
  return tooNarrow;
}

static bool
curveOutsideDomain(Mapping &curve)
{

  // make sure the trim curves fit inside [0,1]
  real tol = 0.5; // let curves be as much as 0.5 out of the domain

  real maxs = -REAL_MAX;
  real mins =  REAL_MAX;
  const RealArray & bb = curve.getBoundingBox();
  
  for ( int s=0; s<2; s++ )
    {
      for ( int a=0; a<2; a++ )
	{
	  maxs=max(maxs,bb(s,a));
	  mins=min(mins,bb(s,a));
	}
    }
  
  
  return  !( maxs>-tol && maxs<(1.+tol) &&
	     mins>-tol && mins<(1.+tol) );
  
}

// ==================================================================
// Determine if a curve self-intersects by checking intersections
// of the control curve. If two adjacent control segments intersect
// (often happens at a cusp) then the curves will intersect
// ==================================================================
static bool
curveSelfIntersects( const NurbsMapping &map, real angle=110. )
{
  if ( map.getDomainDimension() != 1 ) return false;

  int maprd = map.getRangeDimension();

  const RealArray & cpoints = map.getControlPoints();
  
  real cosang = cos( angle*acos(-1.)/180. );

  Range AXES(map.getRangeDimension());
  Range R(map.getNumberOfControlPoints());

  ArraySimple<real> cpold(R.getLength(),AXES.getLength()),cpderef(R.getLength(),AXES.getLength());

  ArraySimple<int> cpid(R.getLength()), cpidold(R.getLength()), removable(R.getLength());
  for ( int cpp=0; cpp<map.getNumberOfControlPoints(); cpp++ )
    {
      cpid(cpp) = cpidold(cpp) = cpp;
      removable(cpp) = 1;
#if 0
      for ( int a=0; a<AXES.getLength(); a++ )
	cpold(cpp,a) = cpoints(cpp,a);
#endif

    }

  // The first step is to get a coarser version of the control points to
  // make the checking for intersections faster. (*wdh* 100207 is this correct Kyle?)

  int ncpderef = 0;
  bool stillDerefining = true;
  int np = map.getNumberOfControlPoints();
  ArraySimple<real> p1(2),p2(2),p3(2),p4(2),p5(2);
  int p,nRemoved,n,a;
  while ( stillDerefining )
    {
      // assume that the curve is periodic and clamped, so that the first and last 
      // control points are the same

      p=1; // always keep the first control point
      nRemoved = 0;
      n=0;
      //int a;
      //      for ( a=0; a<AXES.getLength(); a++ )
      //	cpderef(n,a) = cpold(0,a);

      n++;

      // loop through the points and determine whether they should be removed.
      //   if a point needs to be kept due to the local angle, keep its immediate neighbors as well...
      removable(0) = removable(np-1) = 0;
      
      for ( p=1; p<(np-1); p++ )
	{
	  // can this point be removed ?
	  for ( a=0; a<2; a++ )
	    {
#if 0
	      p1[a] = cpold((p+np-1)%np,a);
	      p2[a] = cpold(p,a);
	      p3[a] = cpold((p+1)%np,a);
	      p4[a] = cpold((p+np-2)%np,a);
	      p5[a] = cpold((p+2)%np,a);
#endif
	      p1[a] = cpoints(cpidold((p+np-1)%np),a)/cpoints(cpidold((p+np-1)%np), maprd);
	      p2[a] = cpoints(cpidold(p),a)/cpoints(cpidold(p),maprd);
	      p3[a] = cpoints(cpidold((p+1)%np),a)/cpoints(cpidold((p+1)%np),maprd);
	      p4[a] = cpoints(cpidold((p+np-2)%np),a)/cpoints(cpidold((p+np-2)%np),maprd);
	      p5[a] = cpoints(cpidold((p+2)%np),a)/cpoints(cpidold((p+2)%np),maprd);
	    }

	  real dotprod = 0, denom=0,magpm1=0, magpp1=0,magpm2=0,magpp2=0;
	  for ( a=0; a<2; a++ )
	    {
	      magpm1 += (p1[a]-p2[a])*(p1[a]-p2[a]);
	      magpp1 += (p3[a]-p2[a])*(p3[a]-p2[a]);
	      dotprod += (p1[a]-p2[a])*(p3[a]-p2[a]);
	      magpm2 += (p4[a]-p1[a])*(p4[a]-p1[a]);
	      magpp2 += (p5[a]-p3[a])*(p5[a]-p3[a]);
	    }

	  denom = magpm1*magpp1;
	  if ( fabs(denom) <= 10*REAL_EPSILON || (dotprod/sqrt(denom))>cosang )
	    {
	      //nope!
	      removable(p) = removable(p+1) = removable(p-1) = 0;
	    }
	  else if ( (magpm2/magpm1) < 0.0625 ||
		    (magpp2/magpp1) < 0.0625 )
	    {
	      // don't let adjacent segments differ by more than a factor of 4 in length
	      removable(p) = 0;
	    }

	}

      //removable.display("removable");
      p = 1;
      while ( p<(np-1) ) // -1 since using the assumption of periodicity 
        {
	  if ( removable(p)==1 )
	  {
	    // if np-2 is being removed then p+1 is the periodic point which we are keeping
	    //   and the loop is about to end, don't increment n since it is taken care of 
	    //   by the assumption of periodicity
	    if ( p!=(np-2) ) {
	      //for ( a=0; a<2; a++ )
	      //cpderef(n,a) = cpold(p+1,a);
	      cpid(n) = cpidold(p+1);
	      n++;
	    }
	    
	    p+=2;
	    nRemoved++;
	  }
	  else
	  {
	    //for ( a=0; a<2; a++ )
	    // cpderef(n,a) = cpold(p,a);
	    cpid(n) = cpidold(p);
	    n++;
	    p++;
	  }
	}

      //      for ( a=0; a<2; a++ )
      //	cpderef(n,a) = cpold(np-1,a);
      cpid(n) = cpidold(np-1);
      np = n+1;

      for ( int i=0; i<np; i++ )
	{
	  //cpold(i,0) = cpderef(i,0);
	  //cpold(i,1) = cpderef(i,1);
	  removable[i] = 1;
	  cpidold[i] = cpid[i];
	}

#if 0
      cout<<"nRemoved "<<nRemoved<<endl;
      GraphicsParameters params;
      DataPointMapping dpm;
      realArray cpplot(np,2);
      for ( int r=0; r<np; r++ )
	for ( a=0; a<2; a++ )
	  cpplot(r,a) = cpold(r,a);

      dpm.setDataPoints(cpplot, 1);
      params.set(GI_GRID_LINES, true);
      
      PlotIt::plot(*(Overture::getGraphicsInterface()), dpm,params);
#endif
      if (nRemoved==0) stillDerefining = false;
    }

  // now do a brute for check for intersections
  bool intersection = false;
  //  ArraySimple<real> p1(2),p2(2),p3(2),p4(2);
  ArraySimple<real> ccp1(2),ccp2(2),ccp3(2),ccp4(2);
  bool parallel = false;

#if 0
  cout<<"curve self-intersection check, number of segments to check : "<<cpderef.getLength(0)-1;
  cpderef.display("cpderef");

  GraphicsParameters params;
  DataPointMapping dpm;
  dpm.setDataPoints(cpold, 1);
  params.set(GI_GRID_LINES, true);

  PlotIt::plot(*(Overture::getGraphicsInterface()), dpm,params);
#endif

  // for each control segment (c,c+1) : 
  for ( int c=0; (c<np-1) && !intersection; c++ )
    {
      for ( int a=0; a<2; a++ )
	{
#if 0
	  p1(a) = cpderef(c,a);
	  p2(a) = cpderef(c+1,a);
#endif
	  p1[a] = cpoints(cpid(c),a)/cpoints(cpid(c),maprd);
	  p2[a] = cpoints(cpid(c+1),a)/cpoints(cpid(c+1),maprd);
	}

      // check for intersections with control segement [c1,c1+1]
      for ( int c1=c+1; (c1<np) && !intersection; c1++ )
	{
	  for ( int a=0; a<2; a++ )
	    {
#if 0
	      p3(a) = cpderef(c1,a);
	      p4(a) = cpderef((c1+1)%np,a);
#endif
	      p3[a] = cpoints(cpid(c1),a)/cpoints(cpid(c1),maprd);
	      p4[a] = cpoints(cpid((c1+1)%np),a)/cpoints(cpid((c1+1)%np),maprd);
	    }
	  intersection = intersect2D(p1,p2,p3,p4,parallel);
	  if ( intersection ) 
	    { // check original control points in the vicinity of the intersection
	      intersection = false;
	      for ( int cc=cpid(c); cc<cpid(c+1) && !intersection; cc++ )
		{
		  for ( int a=0; a<2; a++ )
		    {
		      ccp1(a) = cpoints(cc,a)/cpoints(cc,maprd);
		      ccp2(a) = cpoints(cc+1,a)/cpoints(cc+1,maprd);
		    }
		  
		  for ( int cc1=cpid(c1); cc1<cpid(c1+1) && !intersection; cc1++ )
		    {
		      for ( int a=0; a<2; a++ )
			{
			  ccp3(a) = cpoints(cc1,a)/cpoints(cc1,maprd);
			  ccp4(a) = cpoints(cc1+1,a)/cpoints(cc1+1,maprd);
			}
		      intersection = intersect2D(ccp1,ccp2,ccp3,ccp4, parallel);
                      if( intersection )
		      {
			if ( map.getIsPeriodic(axis1)==Mapping::functionPeriodic )
			  {
			    if ( cc==0 && (cc1+1)==(cpoints.getLength(0)-1) )
			      intersection=false;
			    else
			    {
			      printf("TrimmedMapping::curveSelfIntersects: control point segment [%i,%i] intersects "
				     " segment [%i,%i]\n",cc,cc+1,cc1,cc1+1);
			      printf("cPoint(%i)=(%e, %e), cPoint(%i)=(%e,%e)\n", cc, ccp1(0), ccp1(1), 
				     cc+1, ccp2(0), ccp2(1));
			      printf("cPoint(%i)=(%e, %e), cPoint(%i)=(%e,%e)\n", cc1, ccp3(0), ccp3(1), 
				     cc1+1, ccp4(0), ccp4(1));
			    }
			  }
			else
			{
			  printf("TrimmedMapping::curveSelfIntersects: control point segment [%i,%i] intersects "
				 " segment [%i,%i]\n",cc,cc+1,cc1,cc1+1);
			  printf("cPoint(%i)=(%e, %e), cPoint(%i)=(%e,%e)\n", cc, ccp1(0), ccp1(1), 
				 cc+1, ccp2(0), ccp2(1));
			  printf("cPoint(%i)=(%e, %e), cPoint(%i)=(%e,%e)\n", cc1, ccp3(0), ccp3(1), 
				 cc1+1, ccp4(0), ccp4(1));
			}
		      }
		    }
		}
	    }
	}
    }
  
  if( intersection )
  {
    cout<<"the curve intersected itself"<<endl;
  }
      

  return intersection;
}

TrimmedMapping::
TrimmedMapping() : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  Default Constructor
//===========================================================================
{ 
  setup();
}

int TrimmedMapping::
setup()
//===========================================================================
// /Access: protected.
// /Purpose: 
//   Initialize a TrimmedMapping.
//
//\end{TrimmedMappingInclude.tex}
//===========================================================================
{
  // printf("TrimmedMapping\n");
  TrimmedMapping::className="TrimmedMapping";
  setName( Mapping::mappingName,"TrimmedMapping");

  // we do provide a specialized inverse.
  // *** wdh *** turn this off now 010807setBasicInverseOption(canInvert);  // basicInverse is available
  setBasicInverseOption(canInvert);  // basicInverse is available

  surface=NULL;
  trimCurves = NULL;
  numberOfTrimCurves = 0;

  trimmingCurveArcLength=NULL;
  
  quadTreeMesh=NULL;
  rCurve=NULL;
  farthestDistanceNearCurve = defaultFarthestDistanceNearCurve;
  rBound[0][0]=rBound[1][0]=rBound[2][0]=0.; // *note* rBound[axis][side]
  rBound[0][1]=rBound[1][1]=rBound[2][1]=1.;
  
  timeForInsideOrOutside=0.;
  timeForFindClosestCurve=0.;
  timeForCreateTrimmedSurface=0.;
  timeForFindDistanceToACurve=0.;
  timeForMapGrid=0.;
  timeForUntrimmedInverse=0.;

  timeForSeg0 = 0;
  timeForSeg1 = 0;

  validTrimming = true;
  allNurbs = false;

  callsOfFindClosestCurve = 0;
  callsOfFindClosestCurve_all = 0;

  triangulation=NULL;
  // use this to reduce allowable angle to we don't get too many triangles
  minAngleForTriangulation=-1.; // use default if negative defaultMinAngleForTriangulation;  // not used if negative
  elementDensityTolerance=-1.;  // use default if negative
  maxArea=-1.;  // use default is negative
  
  oldTrimmingCurves = NULL;
  numberOfOldTrimmingCurves = 0;
  oldTrimOrientations.redim(0);

  mappingHasChanged();
  setUnInitialized();

  return 0;
}


TrimmedMapping::
TrimmedMapping(Mapping & surface_, 
	       Mapping *outerCurve_ /* =NULL */ , 
	       const int & numberOfInnerCurves_ /* =0 */, 
	       Mapping **innerCurve_ /* =NULL */ )
//===========================================================================
/// \brief  Create a trimmed surface
/// \param surface (input) : surface to be trimmed
/// \param outerCurve (input) : curve defining the outer boundary, if NULL then the 
///    outer boundary is the boundary of surface
/// \param numberOfInnerCurves (input) : number of closed curves in the interior that trim
///     the surface
/// \param General Notes:
///     In order to evaluate a trimmed mapping we need to decide whether we are inside or outside.
///   To make this determination faster, we divide the domain space (r) with a quadtree mesh:
///  the domain is broken into 4 squares, each of which is subdivided into 4 again, recursively
///  as needed.  Each square is marked whether it lies inside the domain, outside, or partly in
///  and partly out.  It is quick to traverse the quadtree to find which square a given point is
///  in.  If the square is inside or outside we are done.  If it is mixed, we usually have to
///  check the point against only one curve to determine insideness.
/// 
//===========================================================================
{
//  int debug = Mapping::debug;
//  debug=3;
  
  setup();

  Mapping **trimCurves_ = new Mapping *[numberOfInnerCurves_+1];
  NurbsMapping *newNurb = NULL;
  //
  // check orientations and endpoint periodicity
  //
  if ( outerCurve_!=NULL )
    {
      if ( ! curveIsCounterClockwise(*outerCurve_) )
	{
	  if ( debug & 2 )
	    {
	      cout<<"TrimmedMapping : outer needed reversal"<<endl;
	    }
	  if ( outerCurve_->getClassName() == "NurbsMapping" )
	  {
            // printf("Before reversal periodic=%i \n",outerCurve_->getIsPeriodic(axis1));
	    ((NurbsMapping *)outerCurve_)->reparameterize(1.,0.);
// AP: TrimOrientation is computed in setCurves()
	    //printf("Reversing the curve in the constructor\n");
            // printf("After reversal periodic=%i \n",outerCurve_->getIsPeriodic(axis1));
	  }
	  else
	    invalidateTrimming();
	}
      if ( connectCurveEnds(*outerCurve_)!=0 )
	{
	  if ( debug & 2 )
	    {
	      cout<<"TrimmedMapping : outer is not periodic"<<endl;
	    }
	  invalidateTrimming();
	}
      trimCurves_[0] = outerCurve_;
    }
  else
    { // construct a rectangular outer trimming curve on [0,1] x [0,1]
      newNurb = new NurbsMapping(1,2);
      trimCurves_[0] = (Mapping *)newNurb;
      trimCurves_[0]->incrementReferenceCount();
      
      constructOuterBoundaryCurve(newNurb);

      //newNurb->parametricCurve(surface);
    }

  for ( int i=0; i<numberOfInnerCurves_; i++ )
    {
      if ( curveIsCounterClockwise( *innerCurve_[i] ) )
	{
	  if ( debug & 2 )
	    {
	      cout<<"TrimmedMapping : inner curve "<<i<<" needed reversal"<<endl;
	    }
	  if ( innerCurve_[i]->getClassName()=="NurbsMapping" )
	    ((NurbsMapping *)innerCurve_[i])->reparameterize(1.,0.);
	  else
	    invalidateTrimming();
	}

      if ( connectCurveEnds(*innerCurve_[i]) !=0 )
	{
	  if ( debug &2 )
	    {
	      cout<<"TrimmedMapping: inner curve "<<i<<" is not periodic"<<endl;
	    }
	  invalidateTrimming();
	}
      trimCurves_[i+1] = innerCurve_[i];
    }


  int numberOfTrimCurves_ = numberOfInnerCurves_ + 1;

  setCurves(surface_,numberOfTrimCurves_,trimCurves_);

  if (newNurb && newNurb->decrementReferenceCount() == 0)
    delete newNurb;
  
  delete [] trimCurves_;

  if( surface!=NULL )
    setName( mappingName,"trimmed-"+surface->getName(mappingName) );
  else
    setName( mappingName,"TrimmedMapping");

}

TrimmedMapping::
TrimmedMapping(Mapping & surface_, 
	       const int & numberOfTrimCurves_ /* =0 */, 
	       Mapping **trimCurves_ /* =NULL */ )
//===========================================================================
/// \brief  Create a trimmed surface
/// \param surface (input) : surface to be trimmed
/// \param numberOfTrimCurves_ (input) : number of closed curves that trim the surface
/// \param trimCurves_ (input) : the trimming curves 
/// \param General Notes:
///     In order to evaluate a trimmed mapping we need to decide whether we are inside or outside.
///   To make this determination faster, we divide the domain space (r) with a quadtree mesh:
///  the domain is broken into 4 squares, each of which is subdivided into 4 again, recursively
///  as needed.  Each square is marked whether it lies inside the domain, outside, or partly in
///  and partly out.  It is quick to traverse the quadtree to find which square a given point is
///  in.  If the square is inside or outside we are done.  If it is mixed, we usually have to
///  check the point against only one curve to determine insideness.
/// 
//===========================================================================
{
  setup();

  setCurves(surface_,numberOfTrimCurves_,trimCurves_);

  if( surface!=NULL )
    setName( mappingName,"trimmed-"+surface->getName(mappingName) );
  else
    setName( mappingName,"TrimmedMapping");
}


// Copy constructor is deep by default
TrimmedMapping::
TrimmedMapping( const TrimmedMapping & map, const CopyType copyType )
{
  TrimmedMapping::className="TrimmedMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "TrimmedMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

TrimmedMapping::
~TrimmedMapping()
{
  if( debug & 4 )
    cout << " TrimmedMapping::Destructor called" << endl;

  destroy();

  if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
    delete triangulation;
}

int TrimmedMapping::
destroy()
// ===================================================================
// /Access: protected.
// /Description:
//    Delete stuff.
// ===================================================================
{
  delete [] rCurve; 
  rCurve=NULL;

  if( surface!=NULL && surface->decrementReferenceCount()==0 )
  {
    // printf(" TrimmedMapping::surface being deleted");
    delete surface; 
  }
  surface=NULL;


  if ( trimCurves != NULL )
    {
      for ( int i=0; i<numberOfTrimCurves; i++ )
	{
	  if ( trimCurves[i]->decrementReferenceCount() == 0 )
	    delete trimCurves[i];
	  trimCurves[i] = NULL;
	}
      delete [] trimCurves;
    }
  numberOfTrimCurves = 0;
  trimOrientation.redim(0);

  if ( quadTreeMesh!=NULL ) delete quadTreeMesh;
  quadTreeMesh=NULL;

  if( trimmingCurveArcLength!=NULL )
    delete [] trimmingCurveArcLength;

  trimmingCurveArcLength=NULL;
  
  if ( triangulation!=NULL ) 
    {
      if ( triangulation->decrementReferenceCount()==0) delete triangulation;
      triangulation=NULL;
    }

  return 0;
}


TrimmedMapping & TrimmedMapping::
operator=( const TrimmedMapping & X )
{
  if( TrimmedMapping::className != X.getClassName() )
  {
    cout << "TrimmedMapping::operator= ERROR trying to set a TrimmedMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }

  surface            =X.surface;
  if( surface!=NULL )
    surface->incrementReferenceCount(); // ***wdh*** 990220

  numberOfTrimCurves = X.numberOfTrimCurves;
  trimCurves = X.trimCurves;
  if ( numberOfTrimCurves>0 )
    {
      trimCurves = new Mapping * [numberOfTrimCurves];
      for ( int i=0; i<numberOfTrimCurves; i++ )
	{
	  trimCurves[i] = X.trimCurves[i];
	  if ( trimCurves[i] != NULL ) trimCurves[i]->incrementReferenceCount();
	}
    }
  else
    trimCurves = NULL;

  for( int axis=0; axis<3; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      rBound[axis][side]=X.rBound[axis][side];
    }
  }

  quadTreeMesh = new TMquadRoot;
  *quadTreeMesh = *(X.quadTreeMesh);
  farthestDistanceNearCurve = X.farthestDistanceNearCurve;

  smallestLengthScale = X.smallestLengthScale;
  dRmin = X.dRmin;
  dSmin = X.dSmin;

  this->Mapping::operator=(X);            // call = for derivee class
  if ( X.triangulation!=NULL )
    {
      if ( this->triangulation == NULL )
	this->triangulation = new UnstructuredMapping();

      *triangulation = * X.triangulation;
    }
  else
    if ( this->triangulation != NULL )
      {
	delete triangulation;
	triangulation = NULL;
      }

  return *this;
}

int TrimmedMapping::
getTriangulationParameters( real & area, real & minAngle, real & elementDensity ) const
//===========================================================================
/// \brief  
///     Return the values of parameters that control the triangulation.
//===========================================================================
{
  area =maxArea;
  minAngle=minAngleForTriangulation;
  elementDensity=elementDensityTolerance;
  return 0;
}



int TrimmedMapping::
setMaxAreaForTriangulation( real area /* =.1 */ )
//===========================================================================
/// \brief  
///     Set the maxmimum area (approximately) for triangles.
/// \param area (input) : max area for triangles (approx). 0=use default
//===========================================================================
{
  maxArea=area;
  mappingHasChanged(); 
  return 0;
}

int TrimmedMapping::
setMinAngleForTriangulation( real minAngle /* =20. */ )
//===========================================================================
/// \brief  
///     Set the minium angle for the triangulation of the trimmed mapping.
/// \param minAngle (input) : choosing a smaller value will result in fewer triangles.
///             A negative value means use the default. A value of zero will give the
///            fewest triangles.
//===========================================================================
{
  minAngleForTriangulation=minAngle;
  mappingHasChanged(); 
  return 0;
}

int TrimmedMapping::
setElementDensityToleranceForTriangulation( real elementDensity /* =.05 */ )
//===========================================================================
/// \brief  
///     Specify the element denisty tolerance
/// \param elementDensity (input) : choosing a smaller value will result in more triangles
///     The number of grid points on an edge is based on the ratio of the curvature to the 
///          elementDensity. A negative value means use the default. 
//===========================================================================
{
  elementDensityTolerance=elementDensity;
  mappingHasChanged(); 
  return 0;
}


int TrimmedMapping::
setCurves(Mapping & surface_, 
	  const int & numberOfTrimCurves_ /* =0 */, 
	  Mapping **trimCurves_ /* =NULL */ )
//===========================================================================
/// \brief  
///     Specify the surface and trimming curves.
/// \param surface (input) : surface to be trimmed
/// \param numberOfInnerCurves (input) : number of closed curves that trim the surface
/// \param trimCurves_ (input) : the oriented trim curves that trim the surface
//===========================================================================
{

  destroy();

  farthestDistanceNearCurve = defaultFarthestDistanceNearCurve;
  rBound[0][0]=rBound[1][0]=rBound[2][0]=0.; // *note* rBound[axis][side]
  rBound[0][1]=rBound[1][1]=rBound[2][1]=1.;

  surface=&surface_;
  surface->uncountedReferencesMayExist(); // this call checks if this surface is reference counted
  surface->incrementReferenceCount(); // ***wdh980218

  numberOfTrimCurves = numberOfTrimCurves_;

  trimOrientation.redim(numberOfTrimCurves);
  trimOrientation = 1;
  if ( numberOfTrimCurves>0 )
    trimCurves = new Mapping * [ numberOfTrimCurves ];
  else
    trimCurves = NULL;
  
  allNurbs = (surface->getClassName()=="NurbsMapping");
  for ( int i=0; i<numberOfTrimCurves; i++ )
    {
      if ( trimCurves_[i]->uncountedReferencesMayExist() )
	cout<<"WARNING : TrimmedMapping::trimCurves uncountedReferencesMayExist"<<endl;
      trimCurves[i] = trimCurves_[i];
      trimCurves[i]->incrementReferenceCount();
      if ( trimCurves[i]->getDomainDimension()!=1 || trimCurves[i]->getRangeDimension()!=2 )
	{ 
	  cout << "TrimmedMapping::ERROR : trimCurves are not 2D"<<endl;
	  throw "error";
	}
      if (trimCurves[i]->getIsPeriodic(0)!=functionPeriodic) invalidateTrimming();
      allNurbs = allNurbs && ( trimCurves[i]->getClassName()=="NurbsMapping" );
    }

// Estimate the smallest scale in the trimming curves
// *wdh* move this computation to the createTrimmedSurface ****
  smallestLengthScale = 1.; // Default value
  dRmin = dSmin = 1.;

  initializeTrimCurves();

  if( getName(mappingName)=="TrimmedMapping" )
    setName(mappingName,"trimmed-"+surface->getName(mappingName));
  
  setDomainDimension(surface->getDomainDimension());
  setRangeDimension(surface->getRangeDimension());
  setDomainSpace(surface->getDomainSpace());
  setRangeSpace(surface->getRangeSpace());
  
  for( int axis=0; axis<domainDimension; axis++ )
  {
    setIsPeriodic(axis,surface->getIsPeriodic(axis));
    setGridDimensions(axis,surface->getGridDimensions(axis));
    
    for( int side=0; side<=1; side++ )
    {
      setBoundaryCondition(side,axis,surface->getBoundaryCondition(side,axis));
      setTypeOfCoordinateSingularity(side,axis,surface->getTypeOfCoordinateSingularity(side,axis));
    }
  }

  mappingHasChanged();

  validateTrimming();
  setUnInitialized();
  setBounds();

  return 0;
}

void TrimmedMapping::
setUnInitialized() 
// =====================================================================================================
/// \details 
///     Indicate that this Mapping is not up to date. This will destroy the triangulation used to plot it.
///    
// =====================================================================================================
{ 
  if ( triangulation!=NULL ) 
    {
      if ( (triangulation->decrementReferenceCount()) == 0) delete triangulation;
      triangulation = NULL;
    }
  upToDate = false; 
}

void
TrimmedMapping::
initializeTrimCurves()
// =====================================================================================================
/// \param Access: protected
/// \details 
///    
///      Compute trimming curve arclengths, areas, orientation, dRmin, dSmin.
/// 
// =====================================================================================================
{
  if (numberOfTrimCurves>0)
  {
    real area, arcLen, l1, l2, scale0, scale1;
  
    delete [] trimmingCurveArcLength;
    trimmingCurveArcLength = new real [numberOfTrimCurves];
 
    trimOrientation.redim(numberOfTrimCurves);
    
    for( int ii=0; ii<numberOfTrimCurves; ii++ )
    {
      // compute the circumference and area of each inner curve

      getAreaAndArcLength( *trimCurves[ii], area, arcLen,scale0,scale1 );

      trimmingCurveArcLength[ii]=arcLen;
      
      trimOrientation(ii)= area<0. ? 1 : -1; // 1 is counter clockwise, -1 is clockwise.

      area = fabs(area); // The sign depends on the direction of the parametrization
      l1 = arcLen/Pi;
      l2 = area/l1;

      if( Mapping::debug & 2 )
      {
        printf(" ** area=%e, arcLength=%e scale0=%e, scale1=%e orientation=%i \n",
             area,arcLen,scale0,scale1,trimOrientation(ii));
      }

      smallestLengthScale = min(smallestLengthScale, min(l1, l2));
      dRmin = min( dRmin, scale0 );
      dSmin = min( dSmin, scale1 );

// this is checked below
//        if( numberOfTrimCurves==1 && trimOrientation(ii)==-1 ) 
//        {
//  // AP: SHouln't we reverse the parametrization too???
//          trimOrientation(ii)=1;
//          printf("***WARNING*** there is only 1 Trimming curve but it has the orientation of an inner curve!\n"
//                 "      I will assume this should be an outer curve\n");
//        }
    }
    if( Mapping::debug & 2 )
      {
	cout << "Global smallest length scale: " << smallestLengthScale << endl;
	cout << "Global dRmin: " << dRmin << " global dSmin: " << dSmin << endl;
      }
  }
  
  // if there is only one curve, make sure it is oriented correctly
  if ( numberOfTrimCurves==1 )
    if ( trimCurves[0]->getClassName()=="NurbsMapping" && trimOrientation(0)==-1 )
    {
      ((NurbsMapping *)trimCurves[0])->reparameterize(1.,0.);
      trimOrientation(0) = 1;
      if ( Mapping::debug & 2 )
	printf("Reversing the curve in initializeTrimCurve\n");
    }
  

}

bool TrimmedMapping::
addTrimCurve(Mapping *newCurve)
//===========================================================================
/// \brief  
///     Add a trimming curve to the surface
/// \param newCurve (input) : the new trim curve
/// \param returns : true if there were no problems with the trimming curve, false otherwise
//===========================================================================
{
  bool status=false;

  if ( newCurve==NULL ) return false;

  numberOfTrimCurves+=1;

  Mapping **oldTrim = trimCurves;
  trimCurves = new Mapping*[numberOfTrimCurves];
  for ( int c=0; c<numberOfTrimCurves-1; c++ )
    trimCurves[c] = oldTrim[c];

  trimCurves[numberOfTrimCurves-1] = newCurve;
  newCurve->incrementReferenceCount();

  initializeTrimCurves();
  setUnInitialized();
  invalidateTrimming();

// remove the current triangulation
  if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
    delete triangulation;
  triangulation = NULL;

  status = trimmingIsValid();

  return status;
}

bool TrimmedMapping::
deleteTrimCurve(int curveToDelete)
//===========================================================================
/// \brief  
///     delete a specific trimming curve from the surface. Note : if the curve
///  to be delete is the last counterclockwise curve, then a default trimming
///  curve is built consisting of the untrimmed surface's boundary.
/// \param curveToDelete : index of the trim curve to be removed
/// \param returns : returns false if the last counterclockwise trim curve was removed 
///  resulting in the creation of a default outer curve.
//===========================================================================
{
  return deleteTrimCurve(1,&curveToDelete);
}

bool TrimmedMapping::
deleteTrimCurve( int numberOfCurvesToDelete, int *curvesToDelete)
//===========================================================================
/// \brief  
///     delete multiple trimming curves from the surface. Note : if the curve
///  to be delete is the last counterclockwise curve, then a default trimming
///  curve is built consisting of the untrimmed surface's boundary.
/// \param numberOfCurvesToDelete (input): the length of the array curvesToDelete
/// \param curvesToDelete (input): an array containing a list of the curves to be deleted
/// \param returns : returns false if the last counterclockwise trim curve was removed 
///  resulting in the creation of a default outer curve.
//===========================================================================
{
  bool status = true;

  // fist get rid of any previously stored deleted trim curves, these
  // will become unrecoverable through undo
  if ( oldTrimmingCurves != NULL )
    {
      for ( int oc=0; oc<numberOfOldTrimmingCurves; oc++ )
	{
	  if ( oldTrimmingCurves[oc]!=NULL )
	    if ( (oldTrimmingCurves[oc]->decrementReferenceCount())==0 )
	      delete oldTrimmingCurves[oc];
	}
      delete [] oldTrimmingCurves;
      oldTrimOrientations.redim(0);
    }
  
  // now set the old trimming curves to be the current ones
  oldTrimmingCurves = trimCurves;
  oldTrimOrientations.redim(trimOrientation.getLength(0));
  oldTrimOrientations = trimOrientation;
  numberOfOldTrimmingCurves = numberOfTrimCurves;
  numberOfTrimCurves = numberOfOldTrimmingCurves - numberOfCurvesToDelete;
  trimCurves = new Mapping*[numberOfTrimCurves];
  trimOrientation.redim(numberOfTrimCurves);

  // now delete the curves that have been marked for removal
  int tc=0;
  bool canDelete = true;
  for ( int c=0; c<numberOfOldTrimmingCurves; c++ )
    {
      canDelete = false;
      for ( int cs=0; cs<numberOfCurvesToDelete && !canDelete; cs++)
	{
	  canDelete = (c==curvesToDelete[cs]);
	}
      
      if ( !canDelete ) 
	{
	  trimOrientation(tc) = oldTrimOrientations(c);
	  trimCurves[tc++] = oldTrimmingCurves[c];
	  oldTrimmingCurves[c] = NULL;
	}
      
    }

  // count how many outer boundary curves there are :
  int nCounterClockwise=0;
  for ( int oc=0; oc<numberOfTrimCurves; oc++ )
    if ( trimOrientation(oc)==1 ) nCounterClockwise++;

  if ( numberOfTrimCurves==0 )
    {
      // *wdh* 010811 NurbsMapping *newBdy = new NurbsMapping(2,3);
      printf("deleteTrimCurve: Adding outer boundary, since all trim curves were deleted\n");
      NurbsMapping *newBdy = new NurbsMapping(1,2);  
      newBdy->incrementReferenceCount(); // *wdh* 010926 
      constructOuterBoundaryCurve(newBdy);
      addTrimCurve(newBdy);
      newBdy->decrementReferenceCount(); // *wdh* 010926 
      status = false;
    }

  if (numberOfCurvesToDelete>0)
    {
      initializeTrimCurves(); //sets up trimming curve information
      setUnInitialized(); // indicates that the quad tree needs to be rebuilt
      invalidateTrimming();
// remove the current triangulation
      if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
	delete triangulation;
      triangulation = NULL;
    }

  return status;
}

bool TrimmedMapping::
undoLastDelete()
//===========================================================================
/// \brief  undo the last call to deleteTrimCurve
/// \param returns : true if successfull,  false otherwise
//===========================================================================
{
  bool status = true;

  if ( oldTrimmingCurves != NULL )
    {
      int tc=0;
      for ( int c=0; c<numberOfOldTrimmingCurves; c++ )
	{
	  if ( oldTrimmingCurves[c]==NULL ) 
	    {
	      oldTrimmingCurves[c] = trimCurves[tc++];
	    }
	}

      if (numberOfOldTrimmingCurves==numberOfTrimCurves)
	{
	  // then we added an outer boundary curve, delete it
	  if ((trimCurves[numberOfTrimCurves-1]->decrementReferenceCount())==0)
	    delete trimCurves[numberOfTrimCurves-1];
	}

      delete [] trimCurves;
      trimCurves = oldTrimmingCurves;
      numberOfTrimCurves = numberOfOldTrimmingCurves;
      numberOfOldTrimmingCurves = 0;
      oldTrimmingCurves = NULL;

      trimOrientation.redim(oldTrimOrientations.getLength(0));
      trimOrientation=oldTrimOrientations;
      oldTrimOrientations.redim(0);

      initializeTrimCurves();

      setUnInitialized();
      validateTrimming();

// remove the current triangulation
      if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
	delete triangulation;
      triangulation = NULL;
    
    }
  else
    printf("undoLastDelete: Sorry, no old trimming curves available\n");

  return status;
}

bool TrimmedMapping::
verifyTrimCurve( Mapping *c)
{
  // a valid trim curve is :
  //                        1. periodic 
  //                        2. does not intersect itself
  //                        3. NOT impossibly narrow
  //                        4. Uses all subcurves
  //
  // return values: true means that the trimcurve satisfies the above criteria!

  Mapping &curve = *c;
  if ( curve.getClassName()=="NurbsMapping" )
  {
// check if all subcurves are used!
    NurbsMapping * tc_ = (NurbsMapping *) c;
    bool allSubcurvesUsed = true;
    for ( int sc=0; sc<tc_->numberOfSubCurves(); sc++ )
    {
      bool subCurveUsed=tc_->isSubCurveOriginal(sc);
      allSubcurvesUsed = allSubcurvesUsed && subCurveUsed;
      if( !subCurveUsed )
      {
	printF("TrimmedMapping::verifyTrimCurve:INFO: sub-curve %i is unused.\n",sc);
      }
      
    }
    return (allSubcurvesUsed && !curveSelfIntersects( (NurbsMapping &)curve ) && 
	    (connectCurveEnds(curve)==0) && !curveTooNarrow(curve));
  }
  else
    return connectCurveEnds(curve)==0;

}

int TrimmedMapping::
validateTrimming()
// /Return values: true if the trimming is valid.
{
  //
  // a valid trimming has :
  //                        1. periodic trimming curves
  //                        2. non self-intersecting trim curves
  //                        3. a "positive" surface area estimated from the oriented trimming curves
  //                        4. the surface is not impossibly thin, use sqrt(area)/arcLength < 0.0001
  //                        5. all subcurves in the trim curve are used.
  //                            (i.e. no subcurves with the original marker set to false)
  //                        6. all subcurves lie in [0,1]

  validTrimming = true;

  int c;
  for ( c=0; c<numberOfTrimCurves && validTrimming; c++ )
    {
      Mapping & curve = * ( trimCurves[c] );
      
      validTrimming = validTrimming && ( connectCurveEnds(curve)==0 );
      
    }

  if ( !validTrimming ){
    cout<<"the curve was not periodic"<<endl;
    return validTrimming;
  }
  
  if( true ) // *wdh* false turns off the curve self intersection test
  {
    for ( c=0; c<numberOfTrimCurves && validTrimming; c++ )
    {
      if (trimCurves[c]->getClassName()=="NurbsMapping")
	validTrimming = ! curveSelfIntersects( *((NurbsMapping *)trimCurves[c]) );
    }
  }
  

  if ( !validTrimming ){
    cout<<"the curve intersected itself"<<endl;
    return validTrimming;
  }

  bool posSurface  = positiveActiveSurfaceArea( numberOfTrimCurves, trimCurves );
  if ( !posSurface && numberOfTrimCurves==1 && trimCurves[0]->getClassName()=="NurbsMapping" )
  {
    ((NurbsMapping*)trimCurves[0])->reparameterize(1,0);
    trimOrientation(0) = 1; //-trimOrientation(0);
    printf("Reversing the curve in validateTrimming\n");
    validTrimming = posSurface = positiveActiveSurfaceArea( numberOfTrimCurves, trimCurves );
    
  }
  else if (!posSurface)
    validTrimming = false;
  
  if ( !posSurface ) cout<<"the surface area was not positive"<<endl;

  for ( c=0; c<numberOfTrimCurves && validTrimming; c++ )
    {
      validTrimming = !curveTooNarrow(*trimCurves[c]);
      if ( !validTrimming ) cout<<"curve "<<c<<" appears too narrow!"<<endl;
    }

// check if all visible subcurves are part of the trim curve
  for ( c=0; c<numberOfTrimCurves && validTrimming; c++ )
  {
    if (trimCurves[c]->getClassName()=="NurbsMapping")
    {
      NurbsMapping * tc_ = (NurbsMapping *) trimCurves[c];
      bool allSubcurvesUsed=true;
      for ( int sc=0; sc<tc_->numberOfSubCurves(); sc++ )
      {
	bool subCurveUsed=tc_->isSubCurveOriginal(sc);
	allSubcurvesUsed = allSubcurvesUsed && subCurveUsed;
	if( !subCurveUsed )
	{
	  printF("TrimmedMapping::validateTrimming:INFO: sub-curve %i is unused (trim curve %i).\n",sc,c);
	}

      }
      validTrimming = validTrimming && allSubcurvesUsed;
      if ( !allSubcurvesUsed )
      {
	printf("TrimmedMapping::validateTrimming:INFO:There were unused sub curves in trim curve %i\n", c);
      }
    }
  }
  if( !validTrimming )
  {
    return validTrimming;
  }
  
  
  // make sure the trim curves fit inside [0,1]
  for ( c=0; c<numberOfTrimCurves && validTrimming; c++ )
    {
      validTrimming = !curveOutsideDomain(*trimCurves[c]);

      if (!validTrimming) cout<<"curve "<<c<<" does not lie in [0,1]x[0,1]"<<endl;
    }

  if( (debug & 2) && validTrimming )
  {
    printf("The trim curve appears to be valid\n");
  }
  return validTrimming;
}

// void TrimmedMapping::
// createTrimmedSurface()
// // ==============================================================================
// // /Description:
// //    Create the TrimmedSurface.
// // ==============================================================================
// {

//   real time0 = getCPU();

//   if( true ) // false ) // do not build the quad tree anymore.
//   {
//     if( debug & 2 )
//       printf("**** TrimmedMapping:: build the quadtree ***\n");
//     delete quadTreeMesh;
//     const real centerX=(rBound[0][0]+rBound[0][1])*.5;
//     const real centerY=(rBound[1][0]+rBound[1][1])*.5;
//     const real halfWidth = max( rBound[0][1]-rBound[0][0], rBound[1][1]-rBound[1][0] )*.5;
//     if( Mapping::debug & 2 )
//     {
//       printf("createTrimmedSurface: root center=(%e,%e), half width=%e\n",centerX, centerY, halfWidth);
//     }
  
//     quadTreeMesh = new TMquadRoot( *this,  centerX, centerY, halfWidth );

// //  quadTreeMesh->dxMinNormal = 0.0625;
// //  quadTreeMesh->dxMin2Curve = quadTreeMesh->dxMinNormal / 16;

// //  quadTreeMesh->dxMinNormal = min(1./16., smallestLengthScale/3); // 1/16 was the old default value
//     quadTreeMesh->dxMinNormal = 1./16.; // 1/16 was the old default value
//     quadTreeMesh->dxMin2Curve = quadTreeMesh->dxMinNormal / 16;

//     quadTreeMesh->divide
//       ( *this, quadTreeMesh->sizeOfQuadTreeMesh, quadTreeMesh->minQuadTreeMeshDx );

//     timeForCreateTrimmedSurface += getCPU() - time0;
//     // debug code:
//     // Mapping::debug = 4;
//     if( Mapping::debug & 4 ) {
//       printf(" timeForCreateTrimmedSurface=%e, \n timeForInsideOrOutside=%e, \n timeForFindClosestCurve=%e \n"
// 	     " timeForFindDistanceToACurve=%e \n",
// 	     timeForCreateTrimmedSurface,timeForInsideOrOutside,timeForFindClosestCurve,timeForFindDistanceToACurve);
//     };

//     int xMask=getGridDimensions(axis1), yMask=getGridDimensions(axis2);
//     if( Mapping::debug & 2 )
//     {
//       cout << "Rectangular mask size would be " << xMask << ',' << yMask << "  with square width "
// 	   << ( 1.0 / ( (float) ( xMask - 1 ) ) ) << endl;
//       cout << "Quadtree dxMinNormal " << quadTreeMesh->dxMinNormal <<
// 	"   dxMin2Curve " << quadTreeMesh->dxMin2Curve <<
// 	"   farthestDistanceNearCurve " << farthestDistanceNearCurve << endl;
//       cout << "This quadtree mesh has " << quadTreeMesh->sizeOfQuadTreeMesh << " squares" <<
// 	"  of miniumum width " << quadTreeMesh->minQuadTreeMeshDx << endl;
//       cout << "Total quadtree squares made = " << quadTreeMesh->maxSquares()
// 	   << "  of minimum width " << quadTreeMesh->minSquareWidth() << endl;
//     }
//     assert( quadTreeMesh->minSquareWidth() > 0 );
//     assert( quadTreeMesh->maxSquares() > 0 );
//     assert( quadTreeMesh->sizeOfQuadTreeMesh<=quadTreeMesh->maxSquares() );
//     assert( quadTreeMesh->sizeOfQuadTreeMesh > 0 );
//   }
  
//   setBounds();
// }



const realArray& TrimmedMapping::
getGrid(MappingParameters & params,
        bool includeGhost /* =false */ )
// ==========================================================================
//  /Purpose:  Return the grid that can be used for plotting the mapping
//      or for the inverse.
// ==========================================================================
{

  // *wdh* 010901 if ( !upToDate ) initialize();
  if( Mapping::debug & 4 )
    printf("TrimmedMapping:: getGrid called, gridIsValid=%i \n",gridIsValid());
  
  if( !gridIsValid()  || grid.elementCount()==0 )
    Mapping::getGrid(params,includeGhost);

  params.mask.reference(projectedMask);

  return grid;
}

void TrimmedMapping::
initializeQuadTree(bool buildQuadTree /* =true */ )
//===========================================================================
/// \details 
///     Initialize things needed by the quad-tree search and optionally build the quad-tree.
///   <ul>
///      <li> initialize the bounding boxes for each of the trimming curves
///      <li> Make the array rCurve[c] point to the "grid" for each trimming curve
///      <li> determine the rBound array which holds the bounds on the unit square
///         in which conatins the trimmed surface.
///   </ul>
//===========================================================================
{
  if ( upToDate ) return;

  delete [] rCurve;

  rCurve = new realArray [numberOfTrimCurves];

  Mapping *curve;
  for( int c=0; c<numberOfTrimCurves; c++ )
  {
    curve = trimCurves[c];
    curve->approximateGlobalInverse->initialize(); // inverseMap(x,r);


    int numberOfSegments=curve->getGridDimensions(axis1);   // split curve into  this many segments
    // make sure the bounding boxes and the grid are made (even for mappings with analytic inverses)

    // rCurve[c].reference((realArray &)curve->approximateGlobalInverse->getGrid());
    rCurve[c].reference((realArray &)curve->getGrid());


    int num3=rCurve[c].getLength(3);
    if( num3!=2 )
    {
      printf("TrimmedMapping::initializeQuadTree:WARNING: parameter curve is 3D ?!\n");
    }
    rCurve[c].reshape(numberOfSegments,num3);

    real curveMax[3], curveMin[3];
    
    // real curveMax = max(rCurve[c]);
    // real curveMin = min(rCurve[c]);
    Range all;
    for( int axis=0; axis<domainDimension; axis++ )
    {
      curveMin[axis]=min(rCurve[c](all,axis));
      curveMax[axis]=max(rCurve[c](all,axis));
    }  

    const real eps = FLT_EPSILON;
    
    if( (!getIsPeriodic(0) && (curveMax[0]>1.+eps || curveMin[0]<-eps )) ||
        (!getIsPeriodic(1) && (curveMax[1]>1.+eps || curveMin[1]<-eps ))  )
    {
      cout << "******* TrimmedMapping::initializeQuadTree:WARNING: the trimming curve of mapping `" 
	   << getName(mappingName) 
	   << "'\nlies outside the parameter space"
           << " ********* \n";
      for( int axis=0; axis<=1; axis++ )
      {
        printf(" axis=%i : (min,max)=(%e,%e) (should be in [0,1]) isPeriodic=%i\n",
	       axis,curveMin[axis],curveMax[axis],getIsPeriodic(axis));
      }
      cout << "continuing but results could be wrong \n";

      // ::display(rCurve[c],"grid for trim curve","%3.1f");
      
    }
    if (c==0)
    {
      Range all;
      for( int axis=0; axis<domainDimension; axis++ )
      {
	// rBound[axis][Start]=c==1 ? min(rCurve[c](all,axis))-.05 : 0. ;
	// rBound[axis][End  ]=c==1 ? max(rCurve[c](all,axis))+.05 : 1. ;
	rBound[axis][Start]=curveMin[axis]-.05;
	rBound[axis][End  ]=curveMax[axis]+.05;
      }
    }
  }

// If necessary, update the number of grid lines in the trimmed mapping to at least 
// partially resolve the trimming curves.

#define SC (char *)(const char *)

// only if the trimming is valid though... (kkc)
  if ( validTrimming ) 
    {
      int old_n1 = getGridDimensions(axis1), old_n2 = getGridDimensions(axis2);
      if( old_n1 < 4/dRmin )
      {
	int n1 = min(max(15,int(4/dRmin + 1)),50); // ***ap limited n1 to 50
	if (n1 > old_n1)
	{
	  printf("Refining the grid `%s' in the r1-direction to %i points.\n", SC getName(mappingName), n1);
	  setGridDimensions(axis1, n1);
	}
      }
      if (old_n2 < 4/dSmin)
      {
	int n2 =  min(max(15,int(4/dSmin + 1)),50); // ***ap limited n2 to 50
	if (n2 > old_n2)
	{
	  printf("Refining the grid `%s' in the r2-direction to %i points.\n", SC getName(mappingName), n2);
	  setGridDimensions(axis2, n2);
	}
      }
      
    }
  
  // ****************************************
  // ********* Build Quad Tree **************
  // ****************************************

  real time0 = getCPU();

  if( buildQuadTree ) 
  {
    if( true || debug & 2 )
      printf("**** TrimmedMapping:: build the quadtree ***\n");
    delete quadTreeMesh;
    const real centerX=(rBound[0][0]+rBound[0][1])*.5;
    const real centerY=(rBound[1][0]+rBound[1][1])*.5;
    const real halfWidth = max( rBound[0][1]-rBound[0][0], rBound[1][1]-rBound[1][0] )*.5;
    if( Mapping::debug & 2 )
    {
      printf("initializeQuadTree: root center=(%e,%e), half width=%e\n",centerX, centerY, halfWidth);
    }
  
    quadTreeMesh = new TMquadRoot( *this,  centerX, centerY, halfWidth );

//  quadTreeMesh->dxMinNormal = 0.0625;
//  quadTreeMesh->dxMin2Curve = quadTreeMesh->dxMinNormal / 16;

//  quadTreeMesh->dxMinNormal = min(1./16., smallestLengthScale/3); // 1/16 was the old default value
    quadTreeMesh->dxMinNormal = 1./16.; // 1/16 was the old default value
    quadTreeMesh->dxMin2Curve = quadTreeMesh->dxMinNormal / 16;

    quadTreeMesh->divide
      ( *this, quadTreeMesh->sizeOfQuadTreeMesh, quadTreeMesh->minQuadTreeMeshDx );

    timeForCreateTrimmedSurface += getCPU() - time0;
    // debug code:
    // Mapping::debug = 4;
    if( Mapping::debug & 4 ) {
      printf(" timeForCreateTrimmedSurface=%e, \n timeForInsideOrOutside=%e, \n timeForFindClosestCurve=%e \n"
	     " timeForFindDistanceToACurve=%e \n",
	     timeForCreateTrimmedSurface,timeForInsideOrOutside,timeForFindClosestCurve,timeForFindDistanceToACurve);
    };

    int xMask=getGridDimensions(axis1), yMask=getGridDimensions(axis2);
    if( Mapping::debug & 2 )
    {
      cout << "Rectangular mask size would be " << xMask << ',' << yMask << "  with square width "
	   << ( 1.0 / ( (float) ( xMask - 1 ) ) ) << endl;
      cout << "Quadtree dxMinNormal " << quadTreeMesh->dxMinNormal <<
	"   dxMin2Curve " << quadTreeMesh->dxMin2Curve <<
	"   farthestDistanceNearCurve " << farthestDistanceNearCurve << endl;
      cout << "This quadtree mesh has " << quadTreeMesh->sizeOfQuadTreeMesh << " squares" <<
	"  of miniumum width " << quadTreeMesh->minQuadTreeMeshDx << endl;
      cout << "Total quadtree squares made = " << quadTreeMesh->maxSquares()
	   << "  of minimum width " << quadTreeMesh->minSquareWidth() << endl;
    }
    assert( quadTreeMesh->minSquareWidth() > 0 );
    assert( quadTreeMesh->maxSquares() > 0 );
    assert( quadTreeMesh->sizeOfQuadTreeMesh<=quadTreeMesh->maxSquares() );
    assert( quadTreeMesh->sizeOfQuadTreeMesh > 0 );
  }
  
  setBounds();

  upToDate = true;
}


//! Set the approximate bounds on the mapping, used by plotting routines etc.
/*!
  /param assignBoundsFromTriangulation (input): if true assign the bounds from the triangulation.
 */
void TrimmedMapping::
setBounds(bool assignBoundsFromTriangulation /* = true */ )
{
  if (surface )
  {
    int side, axis;
    if(trimmingIsValid() )
    {
      UnstructuredMapping & um = getTriangulation();
      for (side=0; side<=1; side++)
	for(axis=0; axis<rangeDimension; axis++ )
	  setRangeBound(side, axis, um.getRangeBound(side, axis));
      
    }
    else // get the grid for the untrimmed surface
    {
      for (side=0; side<=1; side++)
	for (axis=0; axis<getRangeDimension(); axis++)
	{
          // printf("setBounds: side=%i, axis=%i bound=%e\n",side,axis,(real) surface->getRangeBound(side, axis));
	  setRangeBound(side, axis, surface->getRangeBound(side, axis));
	}
    }
  }
  
//    if( surface!=NULL && isInitialized() )
//    {
//      Range all;
//      MappingParameters mapParams;
//      const realArray & x = getGrid(mapParams);
//      if( Mapping::debug & 2 )
//      {
//        printf("TrimmedMapping: bounds= ");
//      }
    
//      for( int axis=0; axis<rangeDimension; axis++ )
//      {
//        // for( int side=Start; side<=End; side++ )
//        //   setRangeBound(side,axis,surface->getRangeBound(side,axis));
//        real xMin,xMax;
//        if( mapParams.mask.getLength(0) > 0 )
//        {
//          where( mapParams.mask!=0 ) 
//  	{
//      	  xMin=min(x(all,all,all,axis));
//  	  xMax=max(x(all,all,all,axis));
//  	}
//        }
//        else
//        {
//  	xMin=min(x(all,all,all,axis));
//  	xMax=max(x(all,all,all,axis));
//        }
//        setRangeBound(Start,axis,xMin);
//        setRangeBound(End  ,axis,xMax);
//        if( Mapping::debug & 2 )
//        {
//  	printf(" [%17.10e,%17.10e] ",xMin,xMax);
//        }
//      }
//      if( Mapping::debug & 2 )
//      {
//        printf("\n");
//      }
    
//    } 
//    else if ( surface != NULL )
//    {
//      for ( int axis=0; axis<rangeDimension; axis++ )
//        {
//  	setRangeBound(Start,axis,surface->getRangeBound(Start,axis));
//  	setRangeBound(End,axis,surface->getRangeBound(End,axis));
//        }
//    }
}




int TrimmedMapping::
getNumberOfTrimCurves()
{
  return numberOfTrimCurves;
}

int TrimmedMapping::
getNumberOfBoundarySubCurves()
// =====================================================================================
// /Description:
// If boundary curves are made of sub-curves then return the total of all sub-curves.
//\end{TrimmedMappingInclude.tex}
// ====================================================================================
{
  int num=0;
  for( int c=0; c<numberOfTrimCurves; c++ )
    if( trimCurves[c]->getClassName()=="NurbsMapping")
      num+=((NurbsMapping*)trimCurves[c])->numberOfSubCurvesInList();
    else
      num+=1;

  return num;
}

Mapping* TrimmedMapping::
getOuterCurve()
// =====================================================================================
/// \details 
///    Return a pointer to the outer trimming curve.
// ====================================================================================
{
  return trimCurves[0];
}


Mapping* TrimmedMapping::
getInnerCurve(const int & curveNumber)
// =====================================================================================
/// \details 
///    Return a pointer to the inner trimming curve number curveNumber.
/// \param curveNumber (input) : number of the trimming curve, between 0 and {\tt getNumberOfInnerCurves()}.
///          Return 0 if the curveNumber is invalid.
// ====================================================================================
{
  if( curveNumber>=0 && curveNumber<numberOfTrimCurves )
    return trimCurves[curveNumber];
  else
  {
    printf("TrimmedMapping::getInnerCurve:ERROR: invalid value for curveNumber=%i, numberOfTrimCurves=%i\n",
	   curveNumber,numberOfTrimCurves);
    return 0;
  }
}

Mapping * TrimmedMapping::
getTrimCurve(const int &curveNumber)
{
  if ( curveNumber>=0 && curveNumber<numberOfTrimCurves )
    return trimCurves[curveNumber];
  else
    {
      cout<<"TrimmedMapping::getTrimCurve:ERROR: invalid curve number "<<curveNumber;
      cout<<". There are only "<<numberOfTrimCurves<<" trim curves available"<<endl;
      return NULL;
    }
}

bool TrimmedMapping::
curveGoesThrough(const TMquad& square, const int& c ) const
{
  int segstart = 0;
  int segstop = -1;
  return curveGoesThrough( square, c, segstart, segstop );
};


bool TrimmedMapping::
curveGoesThrough(const TMquad& square, const int& c, int& segstart, int& segstop ) const
//===========================================================================
/// \param Access: public.
/// \details 
/// 
///   Determine whether the polygonal curve c goes through the square quadtree
///  node "square".  If so, return true.
///  One may specify starting and stopping segment numbers of the curve.
///  These will be reset to indicate the curve segments which pass through
///  the square.  0 and -1 mean to use all segments.
/// 
//===========================================================================
{
  realArray & rc = rCurve[c];
  
  real u0,v0, u1,v1, slope, vx0,vx1,uy0;

  bool result = false;

  real squareX0 = square.the_centerX() - square.the_dx();
  real squareX1 = square.the_centerX() + square.the_dx();
  real squareY0 = square.the_centerY() - square.the_dx();
  real squareY1 = square.the_centerY() + square.the_dx();

  if ( segstop==-1 ) {
    segstop = rc.getLength(0)-1;
  };
  int newsegstart = segstop;
  int newsegstop  = segstart;
  for( int m=segstart; m<segstop; m++ ) {

    // segment endpoints; if segment illegally leaves the domain (unit square),
    // treat as stopping at border of domain
    u0=max(0.,min(1.,rc(m  ,0)));
    v0=max(0.,min(1.,rc(m  ,1)));
    u1=max(0.,min(1.,rc(m+1,0)));
    v1=max(0.,min(1.,rc(m+1,1)));

    // Curve goes through square if a segment endpoint is in the square.
    if ( square.insideSquare( u0,v0, squareX0,squareX1,squareY0,squareY1 ) ) {
      result = true;
      newsegstart = (newsegstart < m) ? newsegstart : m;
      newsegstop = m+1;
      continue;
    };
    if ( square.insideSquare( u1,v1, squareX0,squareX1,squareY0,squareY1 ) ) {
      result = true;
      newsegstart = (newsegstart < m) ? newsegstart : m;
      newsegstop = m+1;
      continue;
    };

    // To do:  do a better job of treating near-intersections

    // Curve goes through square if the segment passes through an edge of the square.
    // We only have to test 3 edges, since the endpoints are outside the square.

    if ( fabs(u1-u0) > FLT_EPSILON ) {
      slope = ( v1 - v0 ) / ( u1 - u0 );
      if ( ( u0<=squareX0 && u1>=squareX0 ) ||
	   ( u1<=squareX0 && u0>=squareX0 ) ) {  // curve includes x = left edge of square
	vx0 = v0 + ( squareX0 - u0 ) * slope;    // curve's y where x = left edge of square
	if ( vx0>=squareY0 && vx0<=squareY1 ) {
	  result = true;
	  newsegstart = (newsegstart < m) ? newsegstart : m;
	  newsegstop = m+1;
	  continue;
	}
      };
      if ( ( u0<=squareX1 && u1>=squareX1 ) ||
	   ( u1<=squareX1 && u0>=squareX1 ) ) {  // curve includes x = right edge of square
	vx1 = v0 + ( squareX1 - u0 ) * slope;    // curve's y where x = right edge of square
	if ( vx1>=squareY0 && vx1<=squareY1 ) {
	  result = true;
	  newsegstart = (newsegstart < m) ? newsegstart : m;
	  newsegstop = m+1;
	  continue;
	}
      }
    }
    else if ( u0>=squareX0 && u0<=squareX1 ) {      // vertical curve segment
      if (( v0<=squareY1 && v1>=squareY0 ) || ( v1<=squareY1 && v0>=squareY0 )) {
	result = true;
	newsegstart = (newsegstart < m) ? newsegstart : m;
	newsegstop = m+1;
	continue;
      }
    };
    if ( fabs(v1-v0) > FLT_EPSILON ) {
      if ( ( v0<=squareY0 && v1>=squareY0 ) ||
	   ( v1<=squareY0 && v0>=squareY0 ) ) {  // curve includes y = bottom edge of square
	slope = ( u1 - u0 ) / ( v1 - v0 );
	uy0 = u0 + ( squareY0 - v0 ) * slope;    // curve's x where y = bottom edge of square
	if ( uy0>=squareX0 && uy0<=squareX1 ) {
	  result = true;
	  newsegstart = (newsegstart < m) ? newsegstart : m;
	  newsegstop = m+1;
	  continue;
	}
      }
    }
    else if ( v0>=squareY0 && v0<=squareY1 ) {      // horizontal curve segment
      if (( u0<=squareX1 && u1>=squareX0 ) || ( u1<=squareX1 && u0>=squareX0 )) {
	result = true;
	newsegstart = (newsegstart < m) ? newsegstart : m;
	newsegstop = m+1;
	continue;
      }
    }
  };

  segstart = newsegstart;
  segstop  = newsegstop;

  return result;
};
  


int TrimmedMapping::
insideOrOutside( const realArray & rr_, const int & c )
//===========================================================================
/// \param Access: protected.
/// \brief  find if the point rr lies inside the curve c 
///       (actually inside the polygon defined by rCurve(0:n,0:1)) and return
///    the distance to that curve.
/// \param Method:
///   Use the routine from the mapping inverse to count how many times a vertical ray traced
///   above the point crosses the polygon. 
///    NOTE: points exactly on the boundary are "outside" by this definition
/// \param rr (input): point the the parameter space of the untrimmed surface.
/// \param c (input): curve number.
/// \param distance (output): 
/// \return 
///   <ul>
///     <li>[+1] if the point is inside the outerCurve (c==1) or outside the inner curve (c>1)
///     <li>[-1] otherwise
///   </ul>   
//===========================================================================
{
  
  #ifdef USE_PPP
   const RealArray & rr = rr_.getLocalArray();
  #else
   const RealArray & rr = rr_;
  #endif

  real time0 = getCPU();
  if ( c<0 || c>numberOfTrimCurves )
  {
    cout << "TrimmedMapping::insideOrOutside:ERROR: invalid curve number, c=" << c << endl;
    {throw "error";}
  }

  Mapping *curve = trimCurves[c];

  assert( curve->approximateGlobalInverse !=NULL );

  IntegerArray cross2(1);  // ****** fix this ******
  cross2=0;
  curve->approximateGlobalInverse->countCrossingsWithPolygon( rr,cross2 );

  timeForInsideOrOutside+=getCPU()-time0;
  int returnValue= (cross2(0) % 2 == 0) ? -1 : +1;

  returnValue = trimOrientation(c)*returnValue;

  return  returnValue;
}


int TrimmedMapping::
insideOrOutside( const realArray & rr, const int & c, realArray & distance )
//===========================================================================
/// \param Access: protected.
/// \brief  find if the point rr lies inside the curve c 
///       (actually inside the polygon defined by rCurve(0:n,0:1)) and return
///    the distance to that curve. This routine calls the 
///    {\tt insideOrOutside( const realArray \& rr, const int \& c )}
///    function.
/// \param rr (input): point the the parameter space of the untrimmed surface.
/// \param c (input): curve number.
/// \param distance (output): 
/// \return 
///   <ul>
///     <li>[+1] if the point is inside the outerCurve (c==1) or oustide the inner curve (c>1)
///     <li>[-1] otherwise
///   </ul>   
//===========================================================================
{
  //if ( !upToDate ) initialize(); 

  int inOrOut = insideOrOutside(rr,c);
  IntegerArray cMin(1);
  cMin=c;
  real delta=1.; // fix this ****
  real errorEstimate=1.;
  findDistanceToACurve(rr,cMin,distance,delta);
  
  if( distance(0) < errorEstimate )
  {  // check more closely  if the point is on the same side of the curve as indicated by the polygon check
  }
  

  return inOrOut;
}

int TrimmedMapping::
findClosestCurve(const realArray & x_, 
		 intArray  & cMin_, 
		 realArray & rC_, 
		 realArray & xC_, 
                 realArray & dist_,
                 const int & approximate /* =TRUE */ ) 
// ======================================================================================
///  / N.B. I HAVE CHANGED THIS: Some changed in the specification are to match the actual
///  code, and some changes in the code are to match the pre-existing specification.
///  But the code has changed in that cMin=-2 has a new, special meaning. (jfp 0399)
/// \param Access: protected.
/// \details 
///    Find the closest curve to a point and/or determine if the point is inside the curve.
/// 
/// \param x(R,.) (input) : points in the untrimmed surfaces parameter space
/// \param cMin(R) (input/output) : If cMin(base)>0, then each cMin(i) is the number of
///  the curve to be used for x(i,.).  If cMin(base)<0, then all curves will be checked,
///  and on output cMin(i) will be the number of the curve nearest x(i,.) (This has been
///  implemented only for the case where x is one point.)
///  When cMin(base)>0, cMin(i)==-2 means to skip computing the projection of x(i,).
///  When cMin(base)=-2, nothing is computed.
/// \param xC(R,.) (output) : closest point on closest curve
/// \param dist (output) : dist(R) = minimum distance
/// \param approximate (input) : if TRUE only determine an approximation to the closest closest point
///      on the closest curve (based on the nearest grid point on the polygonal representation of
///      the curve).
///  
// ======================================================================================
{
  #ifdef USE_PPP
    const RealArray & x = x_.getLocalArray();
    IntegerArray cMin; getLocalArrayWithGhostBoundaries(cMin_,cMin);
    RealArray rC; getLocalArrayWithGhostBoundaries(rC_,rC);
    RealArray xC; getLocalArrayWithGhostBoundaries(xC_,xC);
    RealArray dist; getLocalArrayWithGhostBoundaries(dist_,dist);
  #else
    const RealArray & x = x_;
    IntegerArray & cMin = cMin_;
    RealArray & rC = rC_;
    RealArray & xC = xC_;
    RealArray & dist = dist_;
  #endif

  real time0 = getCPU();
  ++callsOfFindClosestCurve;
  int xbase = x.getBase(0);
  if ( cMin(xbase)<=0 ) ++callsOfFindClosestCurve_all;

  Range R(xbase,x.getBound(0));

  RealArray r(R,Range(0,0)), xx(R,Range(0,1)), distance(R);
  r = -1;
  
  real xv[3], xv0[3],xv1[3];
  int iv[3];
  xv[2]=0.;
  iv[1]=iv[2]=0;
  Range xAxes(0,1);
  int c, i;
  
  Mapping *curve;
  dist=FLT_MAX;
  if( cMin(xbase)<= 0 && cMin(xbase)!=-2 )
  {
    
    // find the closest curve:
    cMin=0;
    for ( c=0; c<numberOfTrimCurves; c++ )
    {
      curve = trimCurves[c];
      if( !approximate )   
      {

        curve->inverseMapS(x,r);  // find nearest point on the curve, this is slow
	if( max(r(R,0))==ApproximateGlobalInverse::bogus )
	{
	  cout << "findClosestCurve::ERROR : inverse failed! \n";
	  throw "error";
	}
	curve->mapS(r,xx);
	
	if( Mapping::debug & 4 )
	{
	  printf("++find closest curve with inverseMap : x=(%e,%e) xx=(%e,%e) \n",x(0,0),x(0,1),xx(0,0),xx(0,1));
          printf("++findClosestCurve: xbase=%i, xbound=%i\n",xbase,x.getBound(0));
	}
      }
      else
      {
        real minimumDistance=REAL_MAX;
        xv[0]=x(0,0); xv[1]=x(0,1);
        curve->approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv );
        // r= ?
        const RealArray & gridForCurve = curve->approximateGlobalInverse->getGrid();
     
        if( !approximate )
	{
	  // determine a more accurate distance
	}
	assert( R.getLength()==1 );
	
        // error estimate is = 1/2 the length of the nearby segments
        int n=curve->getGridDimensions(axis1);
        r(0,0)=iv[0]/(n-1.);
	for ( int a=0; a<domainDimension; a++ )
	  {
	    xx(0,a)=gridForCurve(iv[0],iv[1],iv[2],a);
	  }
	if( Mapping::debug & 4 )
	  printf("find closest curve approximate : x=(%e,%e) xx=(%e,%e) ",x(0,0),x(0,1),xx(0,0),xx(0,1));
      }

      distance(R) = SQR(x(R,0)-xx(R,0)) + SQR(x(R,1)-xx(R,1));

      //printf(" c=%i, r=%e, distance=%e, ",c,r(0,0),distance);
      where( distance(R) < dist(R) )
      {
	cMin(R)=c;
	rC(R,0)=r(R,0);
	xC(R,0)=xx(R,0);
	xC(R,1)=xx(R,1);
	
	dist(R)=distance(R); 
      }
    }
    // printf("\n");
  }
/* ----
  else if( FALSE )
  {
    // this is Bill's old way.
    for( int i=R.getBase(); i<=R.getBound(); i++ )
    {
      // assert( cMin(i)>0 && cMin(i)-2 <numberOfInnerCurves );
      if( cMin(i)<0 )
        continue;
      curve = cMin(i)==1 ? outerCurve : innerCurve[cMin(i)-2];

      if( !approximate )
      {
        curve->inverseMapC(x(i,xAxes),rC(i,Range(0,0)));  // find nearest point on the curve
        curve->mapC(rC(i,Range(0,0)),xC(i,xAxes));
	if( Mapping::debug & 4 )
	  printf("find closest curve with inverseMap(2) : i=%i, x=(%e,%e) xx=(%e,%e) \n",i,x(i,0),x(i,1),xC(i,0),xC(i,1));
      }
      else
      {
        real minimumDistance=REAL_MAX;
        xv[0]=x(0,0); xv[1]=x(0,1);
        curve->approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv );
        // r= ?
        const realArray & gridForCurve = curve->approximateGlobalInverse->getGrid();
     
        // error estimate is = 1/2 the length of the nearby segments
        int n=curve->getGridDimensions(axis1);
        rC(0,0)=iv[0]/(n-1.);
        xC(0,0)=gridForCurve(iv[0],iv[1],iv[2],0);
        xC(0,1)=gridForCurve(iv[0],iv[1],iv[2],1);
      }
    }
    dist(R) = SQR(x(R,0)-xC(R,0)) + SQR(x(R,1)-xC(R,1));
  }
---- */
  else
  {
    if ( !upToDate ) initializeQuadTree(); 

    if ( !approximate ) 
    {
      real timeBegin, timeBeginA, timeBeginB;
      real timeAll = getCPU();
      real time1 = 0;
      real time2 = 0;
      real time3 = 0;
      real time2A = 0;
      real time2B = 0;

      // The time of this block is dominated by inverseMapC, which for unknown
      // reasons is slower in vector mode.
      // The mapC call is about 10 times faster in vector mode, but always
      // dominated by inverseMapC

      // Sort the points to be projected by curve...
      Range RSort(0,x.getBound(0)-x.getBase(0)+1);
      RealArray xSort( RSort, xAxes, Range( 0, numberOfTrimCurves ) );
      RealArray rCSort( RSort, Range(0,0), Range( 0, numberOfTrimCurves ) );
      RealArray xCSort( RSort, xAxes, Range( 0, numberOfTrimCurves ) );
      IntegerArray ix( Range( 0, numberOfTrimCurves ) );
      ix = 0;
      // The following line has no effect on the fact that the second vector call is 10
      // times faster (the vector inverseMapC call is 20 times faster the second time;
      // scalar calls are unchanged and don't affect that timing):
      rCSort = -1.; // give initial guess -1 to force routine to choose a smart guess *wdh*
      timeBegin = getCPU();
      for ( i=R.getBase(); i<=R.getBound(); ++i ) 
      {
	if ( cMin(i)==-2 ) continue;
	//	cout<<cMin(i)<<"  "<<numberOfTrimCurves<<endl;
	assert( cMin(i)>=0 && cMin(i)<numberOfTrimCurves );
	xSort( ix(cMin(i)), xAxes, cMin(i) ) = x( i, xAxes );
	++ix( cMin(i) );
      };
      time1 += getCPU() - timeBegin;
      timeBegin = getCPU();
      // For each curve, project all its points:
      for ( c=0; c<numberOfTrimCurves; ++c )
      {
	if ( ix(c) == 0 ) continue;
	curve = trimCurves[c];
	//	curve->inverseMapC( xSort( Range(0,ix(c)-1), xAxes, c ),
	//			    rCSort( Range(0,ix(c)-1), Range(0,0), c ) );
	for ( i=0; i<ix(c); ++i ) 
	{
	  timeBeginA = getCPU();
	  curve->inverseMapCS( xSort(i,xAxes,c), rCSort(i,Range(0,0),c) );
	  time2A += getCPU() - timeBeginA;
	  // 	  timeBeginB = getCPU();
	  // 	  curve->mapC( rCSort(i,Range(0,0),c), xCSort(i,xAxes,c) );
	  // 	  time2B += getCPU() - timeBeginB;
	};
	timeBeginB = getCPU();
	curve->mapCS( rCSort( Range(0,ix(c)-1), 0, c ),
		     xCSort( Range(0,ix(c)-1), xAxes, c ) );
	time2B += getCPU() - timeBeginB;
      };
      time2 += getCPU() - timeBegin;
      timeBegin = getCPU();
      // Unsort rCSort, xCSort to the output arrays rC, xC:
      ix = 0;
      for ( i=R.getBase(); i<=R.getBound(); ++i ) 
      {
	if ( cMin(i)==-2 )
	{
          xC(i,0)=xC(i,1)=0.; // *wdh* (umr)
          continue;
	}
	rC(i,0) = rCSort( ix(cMin(i)), 0, cMin(i) );
	xC(i, xAxes, 0 ) = xCSort( ix(cMin(i)), xAxes, cMin(i) );
	++ix( cMin(i) );
      }
      time3 += getCPU()-timeBegin;

      timeAll = getCPU() - timeAll;
      //      cout << "Time " << timeAll << "  from " << time1 << " + " << time2 << " + " << time3;
      //      cout << " ; Time2=" << time2 << "  from " << time2A << " + " << time2B << endl;
    }
    else 
    {
      // approximate inverse
      for( i=R.getBase(); i<=R.getBound(); i++ )
      {
	if ( cMin(i)==-2 )
	{
          xC(i,0)=xC(i,1)=0.; // *wdh* (umr)
          continue;
	}
	
	assert (cMin(i)>=0 && cMin(i)<numberOfTrimCurves);
	curve = trimCurves[cMin(i)];
	//       if( !approximate )
	//       {
	//         curve->inverseMapC(x(i,xAxes),rC(i,Range(0,0)));  // find nearest point on the curve
	//         curve->mapC(rC(i,Range(0,0)),xC(i,xAxes));
	//       }

	real minimumDistance=REAL_MAX;
	xv[0]=x(i,0); xv[1]=x(i,1);  // bug fix 0->i not tested
	curve->approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv );
	// r= ?
	const RealArray & gridForCurve = curve->approximateGlobalInverse->getGrid();
     
	// error estimate is = 1/2 the length of the nearby segments
	int n=curve->getGridDimensions(axis1);
	rC(i,0)=iv[0]/(n-1.);  // bug fix0->i not tested
	xC(i,0)=gridForCurve(iv[0],iv[1],iv[2],0);  // bug fix 0->i not tested
	xC(i,1)=gridForCurve(iv[0],iv[1],iv[2],1);  // bug fix 0->i not tested

        // project onto the closest pt on the segment (i1-1,i1) or (i1,i1+1)
        real distMin=REAL_MAX;
        for( int side=0; side<=1; side++ )
	{
	  int i1m = side==0 ? iv[0]-1 : iv[0];
	  int i1  = side==0 ? iv[0]   : iv[0]+1;
	  if( i1m>=0 && i1<n )
	  {
	    xv0[0]=gridForCurve(i1m,iv[1],iv[2],0);
	    xv0[1]=gridForCurve(i1m,iv[1],iv[2],1);
	    xv1[0]=gridForCurve(i1 ,iv[1],iv[2],0);
	    xv1[1]=gridForCurve(i1 ,iv[1],iv[2],1);
            real dot = (xv[0]-xv0[0])*(xv1[0]-xv0[0])+(xv[1]-xv0[1])*(xv1[1]-xv0[1]);
	    real normSquared = max(FLT_EPSILON,SQR(xv1[0]-xv0[0])+SQR(xv1[1]-xv0[1]));
	    real s = dot/normSquared;
            if( s>=0. && s<=1. )
	    {
              // printf("TrimmedMapping: project a bndry pt, s=%e\n",s);
              xv0[0]=(1.-s)*xv0[0]+s*xv1[0];
              xv0[1]=(1.-s)*xv0[1]+s*xv1[1];
              real dist= SQR(xv0[0]-xv[0])+SQR(xv0[1]-xv[1]);
	      if( dist<distMin )
	      {
                distMin=dist;
		rC(i,0)=((1.-s)*i1m+s*i1)/(n-1.);
		xC(i,0)=xv0[0];
		xC(i,1)=xv0[1];
	      }
	    }
	  }
	}

      }
    };
    dist(R) = SQR(x(R,0)-xC(R,0)) + SQR(x(R,1)-xC(R,1));
  }
  
  timeForFindClosestCurve+=getCPU()-time0;
  return 0;
  
}

int TrimmedMapping::
findDistanceToACurve(const realArray & x, 
		     IntegerArray  & cMin, 
		     realArray & dist,
                     const real & delta )
// ======================================================================================
/// \param Access: protected.
/// \details 
///    Find the approximate distance to a curve. (approximate if the distance > deltaX )
/// 
/// \param x(R,.) (input) : points
/// \param cMin(R) (input/output) : if >0 on input then use this curve, on output it is the number of closest curve
/// \param dist (output) : dist(R) = approximate distance
///  
// ======================================================================================
{

  if ( !upToDate ) initializeQuadTree();

  real time0 = getCPU();

  int iv[3];
  real xv[3];
  Range xAxes(0,1);

  int & i1 = iv[0];
  int & i2 = iv[1];
  int & i3 = iv[2];
  int i1m,i1p,n;
  xv[2]=0.;
  iv[1]=iv[2]=0;
  
  Mapping *curve;
  dist=FLT_MAX;
  real errorEstimate;

  for( int i=x.getBase(0); i<=x.getBound(0); i++ )
  {
    assert ( cMin(i)>=0 && cMin(i)<numberOfTrimCurves );
    curve = trimCurves[cMin(i)];

    real minimumDistance=0.;
    if( curve->getBasicInverseOption() != canInvert )
    {
      // find the closest grid point -- this is fast
      minimumDistance=REAL_MAX;
      xv[0]=x(i,0); xv[1]=x(i,1);
      curve->approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv );
      // r= ?
      const RealArray & gridForCurve = curve->approximateGlobalInverse->getGrid();
     
      // error estimate is = 1/2 the length of the nearby segments
      n=grid.getLength(0);
      i1p=(i1+1) % n;
      i1m=(i1-1) % n;
      errorEstimate=.125*( SQR(gridForCurve(i1p,i2,i3,0)-gridForCurve(i1m,i2,i3,0))
                          +SQR(gridForCurve(i1p,i2,i3,1)-gridForCurve(i1m,i2,i3,1)) );
	
      dist(i) = SQR(x(i,0)-gridForCurve(i1,i2,i3,0)) + SQR(x(i,1)-gridForCurve(i1,i2,i3,1));
      if( TRUE )
      {
        printf("findDistanceToACurve: c=%i, x=(%e,%e), grid=(%e,%e), distance=%e, errorEstimate=%e \n",
             cMin(i),x(i,0),x(i,1),grid(i1,i2,i3,0),grid(i1,i2,i3,1),dist(i),errorEstimate);
      }
    }
    if( errorEstimate > delta )
    {
      Range R(x.getBase(0),x.getBound(0));
      realArray r(R,Range(0,0)), xx(R,Range(0,1));
      r = -1;
      
      printf(" **** findDistanceToACurve: exact inverse *** ");
      curve->inverseMapC(x(i,xAxes),r(i,Range(0,0)));  // find nearest point on the curve
      curve->mapC(r(i,Range(0,0)),xx(i,xAxes));
      dist(R) = SQR(x(R,0)-xx(R,0)) + SQR(x(R,1)-xx(R,1));
    }
      
  }

  timeForFindDistanceToACurve+=getCPU()-time0;
  return 0;
  
}


void TrimmedMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
//=====================================================================================
/// \brief  Evaluate the Trimmed and/or derivatives. 
/// \param NOTE: In order to evaluate a trimmed surface you MUST provided a MappingParameters argument.
///         Otherwise only the untrimmed mapping will be defined.
///   
/// \param Notes:
///   (1 The array params.mask(I) is returned with the values -1=outside, 0=inside
///    
///   (2) if point i is outside the grid but near the trimmed boundary 
///   the array distanceToBoundary(i) is set to 
///   be the distance (in parameter space) of the point r(i,.) to the nearest
///   trimming curve. The the point is far from the boundary, distance(i) is set to a large value.
/// 
//=====================================================================================
{
  if( surface==NULL )
  {
    printF("TrimmedMapping::map: Error: The surface has not been defined yet!\n");
    Overture::abort("error");
  }

  surface->map(r,x,xr,params );

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if ( !params.isNull && numberOfTrimCurves>0 )
  {

    if ( !upToDate ) initializeQuadTree();


    realArray rr(1,1), xx(1,2), xC(1,2), dist(1);
    // Mapping *curve;
    bool approximate=false;   // for points near the boundary find the exact distance to the nearest curve
    
    intArray & pmask = params.mask;
    pmask.redim(Range(I));
    distanceToBoundary.redim(Range(I));
    
    intArray cMin(1);

    int nx=getGridDimensions(axis1)-1;
    int ny=getGridDimensions(axis2)-1;
    // const real eps = FLT_EPSILON;
    const TMquad* square;

    for( int i=I.getBase(); i<=I.getBound(); i++ ) {
      if ( !quadTreeMesh->inThisSquare( r(i,0), r(i,1) ) ) {   // If not in root square,
	pmask(i) = 0;                                          // then outside
      }
      else {
	square = quadTreeMesh->squareItsIn( r(i,0), r(i,1) );  // leaf square containing r(i,.)
	if ( square->the_inside() == 1 ) {                      // whole square is inside
	  pmask(i) = 1;
	}
	else if ( square->the_inside() == -1 ) {                // whole square is outside
	  pmask(i) = 0;
	  distanceToBoundary(i) = 10.;          // don't bother to estimate the distance.
	}
	else {                                  // square is partly inside, partly outside
	  assert ( square->the_inside() == 0 );
	  for ( int axis=0; axis<domainDimension; axis++ ) {
	    xx(0,axis)=r(i,axis);
	  };
	  int cstart = square->the_curves().curvestart();
	  int cstop  = square->the_curves().curvestop();
	  assert( cstop > cstart );
	  int inOrOut = 1;
	  for ( int c=cstart; c<cstop; ++c ) {
	    inOrOut = min( inOrOut, insideOrOutside( xx, c ) );
	    // ... we know which segments to check in each curve; a special version of
	    // insideOrOutside for 1 or 2 segments would pay off if did this much.
	    if ( inOrOut==-1 ) break;   // outside the domain
	  };
	  pmask(i)= inOrOut==1 ? 1 : 0 ;  // inside==1, outside==0

	  // For outside points in a square intersected by a boundary curve,
	  // find the distance to the nearest such boundary curve:
	  if ( inOrOut == -1 ) {
	    cMin = (cstop>cstart+1) ? -1 : cstart;  // curve to check; -1 means all curves
	    findClosestCurve( xx, cMin, rr, xC, dist, approximate );
	    distanceToBoundary(i) = dist(0);
	  }
	}
      }
    };
    if ( Mapping::debug & 4 && bound-base>2 ) {
      ::display(pmask,"TrimmedMapping:map: here is pmask","%2i");
    }
  }
};

void  TrimmedMapping::
mapGrid(const realArray & r, 
	realArray & x, 
	realArray & xr,
	MappingParameters & params /* =Overture::nullMappingParameters() */ )
//=====================================================================================
/// \brief  Map grid points and project grid points that cross a trimming curve
///     onto the trimming curve.  This routine is called by the plotting routine
///     so that trimmed curves are properly plotted.
//=====================================================================================
{

  timeForInsideOrOutside=0.;
  timeForFindClosestCurve=0.;
  real timeForSquareItsIn=0.;
  real time0=getCPU();
  real time1, time2, timeForSeg2, timeForSeg3;
  real timeForUntrimmedMap = 0;
  int fcc_calls = 0;  // debugging
  int fcc_3calls = 0;  // debugging
  
  if( surface==NULL )
  {
    cout << "TrimmedMapping::map: Error: The surface has not been defined yet!\n";
    {throw "error";    }
  }

  surface->mapGrid(r,x,xr,params );
  // x.display("x after surface eval");

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  if ( !params.isNull && numberOfTrimCurves>0 )
  {
    if ( !upToDate ) initializeQuadTree();
    

    // we reshape to always be in a special form (incoming could be a 3d shaped grid)
    Range R[4], Rx[4];
    int axis;
    for( axis=0; axis<4; axis++ )
    {
      R  [axis]=Range( r.getBase(axis), r.getBound(axis));
      Rx [axis]=Range( x.getBase(axis), x.getBound(axis));
    }

    realArray & r2 = (realArray &)r; // cast away const so we can reshape
  
    r2.reshape(R[0],R[1],domainDimension);                    // ******************* why ?????????????
    x.reshape(Rx[0],Rx[1],Range(0,Rx[2].length()*Rx[3].length()-1));


    realArray rr(1,1), xx(1,2), xC(1,2), xx3(1,3);
    // Mapping *curve;
    
    intArray & pmask = params.mask;
    pmask.redim(Range(r.getBase(0),r.getBound(0)),
                Range(r.getBase(1),r.getBound(1)));

    realArray dist(1);
    intArray cMin(1);

    int i1,i2;
    int nx=getGridDimensions(axis1)-1;
    int ny=getGridDimensions(axis2)-1;
    //    real eps = REAL_EPSILON*max(nx,ny);
    const TMquad* square;
    timeForSeg0 = getCPU() - time0;
    time2 = getCPU();

    for( i2=r.getBase(1); i2<=r.getBound(1); i2++ ) {
      for( i1=r.getBase(0); i1<=r.getBound(0); i1++ ) {
	if ( !quadTreeMesh->inThisSquare( r(i1,i2,0), r(i1,i2,1) ) ) {
	  // If not in root square, then point is way outside the domain.
	  pmask(i1,i2) = 0;                              // pmask=0 for outside
	}
	else {
	  square = quadTreeMesh->squareItsIn( r(i1,i2,0), r(i1,i2,1) );
	  // ... leaf square containing r(i1,i2,.)
	  if ( square->the_inside() == 1 ) {             // whole square is inside
	    pmask(i1,i2) = 1;                            // pmask=1 for inside
	  }
	  else if ( square->the_inside() == -1 ) {       // whole square is outside
	    pmask(i1,i2) = 0;                            // pmask=0 for outside
	  }
	  // Note: a non-imitated part of the previous algorithm is that, if r(i1,i2,)
	  // were within eps of a grid point (mask(in1,in2)) known to be outside, it
	  // would mark r(i1,i2,) as outside.  The quadtree mesh isn't suited to that.
	  else 
          {                                         // square is cut by a curve;
	    //                           the point r(i1,i2,) may be inside or outside
	    assert( square->the_inside() == 0 );
	    for( axis=0; axis<domainDimension; axis++ ) 
            {
	      xx(0,axis)=r(i1,i2,axis);
	    };
	    int cstart = square->the_curves().curvestart();
	    int cstop  = square->the_curves().curvestop();
	    assert( cstop > cstart );
	    int inOrOut = 1;
	    for ( int c=cstart; c<cstop; ++c ) {
	      inOrOut = min( inOrOut, insideOrOutside( xx, c ) );
	      // ... we know which segments to check in each curve; a special version of
	      // insideOrOutside for 1 or 2 segments would pay off
	      if ( inOrOut==-1 ) break;   // outside the domain
	    };
	    assert( inOrOut==-1 || inOrOut==1 );
	    pmask(i1,i2)= inOrOut;       // 1==inside, -1==outside, to be projected in
	    // Projection formerly was done here.  It's been moved below so we
	    // can check for inside neighbors, and project there too.
	  }
	}
      }
    };
    timeForSeg1 = getCPU() - time2;
    time2 = getCPU();

    for( i2=r.getBase(1); i2<=r.getBound(1); i2++ ) {
      for( i1=r.getBase(0); i1<=r.getBound(0); i1++ ) {
	// *wdh* if ( pmask(i1,i2)==0 ) 
	if ( pmask(i1,i2)==0 && quadTreeMesh->inThisSquare( r(i1,i2,0), r(i1,i2,1) ) ) // *wdh* only check if in root
        {  // outside point, not marked for projection
	  // If next to an inside point, mark it as to be projected.
	  //   This was not in the previous algorithm, but should make graphics more
	  // robust w.r.t. differences between graphics grid and insideness "mask" grid.
	  //   Note that we don't want to do this after projection puts outside points
	  // inside.
	  //   Formerly we got a guess for the nearest curve by looking at the parent
	  // squares' cutting curves; that is less reliable than the present method,
	  // but we may want to look at those curves if a future version of
	  // findClosestCurve can use a list of relevant curve segments.
	  int d1min = i1>r.getBase(0) ? -1 : 0;
	  int d1max = i1<r.getBound(0) ? 1 : 0;
	  int d2min = i2>r.getBase(1) ? -1 : 0;
	  int d2max = i2<r.getBound(1) ? 1 : 0;
	  for ( int d1=d1min; d1<=d1max; ++d1 ) {
	    for ( int d2=d2min; d2<=d2max; ++d2 ) {
	      if ( pmask(i1+d1,i2+d2)==1 ) {
		pmask(i1,i2)=-1;  // mark point to be projected inside
	      }
	    }
	  }
	}
      }
    };
    if( Mapping::debug & 4 ) {
      ::display(pmask,"TrimmedMapping: here is enhanced pmask","%3i");
    };
    timeForSeg2 = getCPU() - time2;
    time2 = getCPU();

    for( i2=r.getBase(1); i2<=r.getBound(1); i2++ ) {
      for( i1=r.getBase(0); i1<=r.getBound(0); i1++ ) {
	if ( pmask(i1,i2)<0 ) 
        {  // project outside point to nearest curve
	  assert( pmask(i1,i2)==-1 );
	  square = quadTreeMesh->squareItsIn( r(i1,i2,0), r(i1,i2,1) );
	  int cstart = square->the_curves().curvestart();
	  int cstop  = square->the_curves().curvestop();
	  // cMin is the curve to project to; -1 means nearest of _all_ the curves
	  // square->inside=0: If one curve goes through the enclosing square, we use that
	  // curve without checking any other curves (this will rarely be wrong).
	  // square->inside=-1: The enclosing square is entirely outside; we use the
	  // unique curve which makes it outside.
	  // In both cases, the curve of choice is cstart, and cstop=cstart+1.
	  if ( cstop==cstart+1 ) {
	    cMin = cstart;    // only one candidate curve to project to; most common case by far
	  }
	  else if ( cstop>cstart+1) {
	    cMin = -1;        // several candidate curves to project to; for now just check all curves
	  }
	  else {
	    cMin = -1;        // no knowledge of curves; shouldn't ever get here
	  };
	  for( axis=0; axis<domainDimension; axis++ ) {
	    xx(0,axis)=r(i1,i2,axis);
	  };
	      
 	  const bool approximate=true;  // finding only the approximate distance is much faster
	  // ** const bool approximate=false;
	  findClosestCurve( xx, cMin, rr, xC, dist, approximate );
	  ++fcc_calls;
	  if ( cMin(0)==-1 ) ++fcc_3calls;
	  time1 = getCPU();
	  surface->map(xC,xx3 );
	  timeForUntrimmedMap += getCPU() - time1;
	  //	  printf("projecting point (%i,%i) onto curve %i, x=(%e,%e), xC=(%e,%e) surf=(%e,%e,%e)\n",
	  //		 i1,i2,cMin(0),
	  //		 xx(0,0),xx(0,1),xC(0,0),xC(0,1),xx3(0,0),xx3(0,1),xx3(0,2));
	  for( axis=0; axis<rangeDimension; axis++ )
	    x(i1,i2,axis)=xx3(0,axis);

	  pmask(i1,i2) = 1;     // mark as inside since have projected it inside
	}
      }
    };
    timeForSeg3 = getCPU() - time2;

    if( Mapping::debug & 4 ) {
      ::display(pmask,"TrimmedMapping: here is pmask","%2i");
    };
    
    r2.reshape(R[0],R[1],R[2],R[3]);    // reshape back to original
    x.reshape(Rx[0],Rx[1],Rx[2],Rx[3]);

    projectedMask.redim(0);
    projectedMask=pmask;  // save a copy so we don't have to recompute for plotting 
  }
  else
  {
    if( Mapping::debug & 4 )
      printf("TrimmedMapping::mapGrid: params.isNull=%i \n",params.isNull);
  }
  
  
  timeForMapGrid=getCPU()-time0;
  // x.display("x after mapGrid");
  if( Mapping::debug & 4 )
  {
    printf(" timeForMapGrid=%e \n",timeForMapGrid);
    printf(" timeForSeg0=%e \n",timeForSeg0);
    printf(" timeForSeg1=%e \n",timeForSeg1);
    printf(" timeForSeg2=%e \n",timeForSeg2);
    printf(" timeForSeg3=%e \n",timeForSeg3);
    printf(" timeForInsideOrOutside=%e, \n timeForFindClosestCurve=%e \n",
  	   timeForInsideOrOutside,timeForFindClosestCurve);
    printf(" timeForUntrimmedMap=%e \n",timeForUntrimmedMap );
    printf(" timeForSquareItsIn=%e \n",timeForSquareItsIn);
    printf(" called findClosestCurve %i times, using all curves %i times \n",
	   fcc_calls,fcc_3calls);
  }
  
};


void TrimmedMapping::
basicInverse( const realArray & x, realArray & r0, realArray & rx, MappingParameters & params )
//==================================================================================
// Here is the basic Inverse (this is an inverse that does not know how
//  to deal with space being periodic)
//
// /Notes:
//  (1) The array params.mask(I) is returned with the values -1=outside, 1=inside
//   
//  (2) if point i is outside the grid but near the trimmed boundary 
//  the array distanceToBoundary(i) is set to 
//  be the distance (in parameter space) of the point r(i,.) to the nearest
//  trimming curve. The the point is far from the boundary, distance(i) is set to a large value.
//
// Notes:
//   o does root square cover unit square or just the region bounded by the outer curve?
//=================================================================================
{

  real time0 = getCPU();
  real time1;
  int i;

  // printf(" TrimmedMapping:: basicInverse \n");
  
  if( surface==NULL )
  {
    cout << "TrimmedMapping::basicInverse: Error: The surface has not been defined yet!\n";
    {throw "error";    }
  }

  Index I = getIndex( x,r0,rx,base,bound,computeMap,computeMapDerivative );

  // first invert the un-trimmed surface. This should always work.
  // Note that we always need to compute the r values, not just the rx values.
  realArray rs;
  time1 = getCPU();
  if( !computeMap )
  {
    rs.redim(I,domainDimension);
    rs = -1;
    
    surface->inverseMap(x,rs,rx,params );
  }
  else
    surface->inverseMap(x,r0,rx,params );
  timeForUntrimmedInverse += getCPU() - time1;

  realArray & r = computeMap ? r0 : rs;
  

  if ( !upToDate ) initializeQuadTree();


  // ::display(r,"TrimmedMap: after surface inversion, r");
  
  // If the point is outside the trimmed region, project it onto the trimmed boundary.

  //  realArray rr(1,1), xx(1,2), xC(1,2), dist(1);
  //  IntegerArray cMin(1);
  int numberOutside=0;
  intArray ia(I);
  realArray rOut(I,2);
  realArray xx(1,2);
  intArray cMin(I);
  cMin = -2;
  bool approximate=false;   // for points near the boundary find the exact distance to the nearest curve

  intArray & pmask = params.mask;
  pmask.redim(I); // used to be Range(I)
  distanceToBoundary.redim(I); // used to be Range(I)
  const TMquad* square;

  timeForSeg0 += getCPU() - time0;
  time0 = getCPU();

  int nx=getGridDimensions(axis1)-1;
  int ny=getGridDimensions(axis2)-1;

// make the quad tree if it isn't already there!
  if (!quadTreeMesh)
  {
  }
  
  for( i=I.getBase(); i<=I.getBound(); i++ )
  {
    if( r(i,0)==Mapping::bogus )
    { // unable to invert, assume outside.
      pmask(i)=-1;
      continue;
    };

    int outside = 0;
    if ( !quadTreeMesh->inThisSquare( r(i,0), r(i,1) ) )      // If not in root square,
    {
      outside = -2;                                            // then way outside
      square = quadTreeMesh;
    }
    else 
    {

      square = quadTreeMesh->squareItsIn( r(i,0), r(i,1) );
      //                                          ... leaf square containing r(i,.)
      if ( square->the_inside() == 1 ) 
      {                      // whole square is inside
	pmask(i) = 1;
	if( Mapping::debug & 4 ) {
	  printf("TrimmedMapping::basicInverse: point i=%i r=(%6.2e,%6.2e) is inside\n",i,r(i,0),r(i,1));
	}
	continue;
      }
      else if ( square->the_inside() == -1 )                 // whole square is outside
      {
	outside = -1;
      }
      else {                                                 // square is partly in, partly out
	outside = 0;
      }
    };

    assert( outside<=0 );
    for( int axis=0; axis<domainDimension; axis++ ) 
    {
      xx(0,axis)=r(i,axis);
    };
    int cstart = square->the_curves().curvestart();
    int cstop  = square->the_curves().curvestop();
    int inOrOut;
    if ( outside<0 )   // we are definitely outside
    {
      inOrOut = -1;
    }
    else 
    {
      assert( cstop > cstart );
      inOrOut = 1;
      for ( int c=cstart; c<cstop; ++c ) 
      {
	inOrOut = min( inOrOut, insideOrOutside( xx, c ) );
	// ... we know which segments to check in each curve; a special version of
	// insideOrOutside for 1 or 2 segments would pay off
	if ( inOrOut==-1 ) break;   // outside the domain
      }
    };

    if ( inOrOut==-1 )   // outside the domain
    {
      
      // - For outside points in a square intersected by a boundary curve,
      // find the distance to the nearest such boundary curve.
      // - For points in a square which is entirely outside, find the distance
      // to the unique curve making the square outside.
      // But don't bother if the square is too far from that curve.
      // - For points entirely outside the domain, find the distance to outerCurve.
      // Thus we never should have to search all curves.
      // The biggest speedup we could get in this function would be to call a special
      // version of findClosestCurve using the curve segment data we have saved.

      int curveToCheck = ( cstop==(cstart+1) ) ? cstart : -1 ;  // curve to check; -1 means all curves
      if ( outside==-2 && numberOfTrimCurves==1 ) curveToCheck = 0;

      if ( curveToCheck==-1 ) 
      {
	cout << "WARNING, didn't expect to search all curves!" << endl;
	cout << "cstart,cstop " << cstart << ',' << cstop << "  outside " << outside ;
	cout << "  square X,Y,dx " << square->the_centerX() << ',' << square->the_centerY()
	     << ',' << square->the_dx() << endl;
      };
      // The following block must be skipped for full compatibility with old version...
      // *wdh*      bool oldCompat = false;
      bool oldCompat = TRUE;  // trun off this next block for now
      //      bool oldCompat = true;
      if ( square->the_inside()==-1 && !oldCompat &&
 	   square->the_curves().curveDist() > farthestDistanceNearCurve )
      {
	distanceToBoundary(i) = 10.0;  // mark with arbitrary large value
	curveToCheck = -2; 	  // means not to compute the projection 
      };

      if( Mapping::debug & 4 ) 
      {
	printf("TrimmedMapping::basicInverse: point i=%i r0=(%6.2e,%6.2e) is OUTSIDE, r=(%6.2e,%6.2e) cMin=%i\n",i,
	       r0(i,0),r0(i,1),r(i,0),r(i,1),curveToCheck);
      };

      pmask(i) = -1;
      
      cMin(numberOutside+base)=curveToCheck;
      ia(numberOutside+base)=i;
      for( int axis=0; axis<domainDimension; axis++ ) 
       rOut(numberOutside+base,axis)=r(i,axis);
      numberOutside++;
    }
    else 
    {   // inside the domain
      assert( inOrOut == 1 );
      pmask(i) = 1;

      if( Mapping::debug & 4 ) 
      {
	printf("TrimmedMapping::basicInverse: point i=%i r=(%6.2e,%6.2e) is inside\n",i,r(i,0),r(i,1));
      }

    }
  };

  if( numberOutside>0 )
  {
    // *wdh* 010809: findClosestCurve was changed by Jeff Painter so that it nolonger looks at cMin(i)=-2
    // to avoid finding the closest curve -- therefore I collect up the points outside first.
    
    // If outside, project the points onto the closest curve:
    Range R(base,base+numberOutside-1); // used to be R = numberOutside
//    rOut.resize(numberOutside,2);
    rOut.resize(R,2);

    realArray xC(R,2), rr(R,1);

    //kkc 040217 find closest curve (as used below) does not work for an array of points
    //           if one of them has cMin==-1 (assertion failure).  If one of the cMin is
    //           -1 then make the first -1 to check all the curves for the point.
    //           YES, I should have fixed findClosestCurve.
    for ( int i=cMin.getBase(0)+1; i<cMin.getBound(0); i++ )
      {
	if ( cMin(i)==-1 )
	  {
	    cMin=-1;
	    break;
	  }
      }

    findClosestCurve( rOut, cMin, rr, xC, distanceToBoundary, approximate );


    if( Mapping::debug & 4 ) 
    {
      for( int i=0; i<numberOutside; i++ )
      {
	printf(" +++numberOutside=%i, i=%i ia=%i: before: rOut=(%e,%e), after xC=(%e,%e)\n",
	       numberOutside,i,ia(i),rOut(i,0),rOut(i,1),xC(i,0),xC(i,1));
      }
    }
    
    // ... we don't use rr
    for( int axis=0; axis<domainDimension; axis++ ) 
      r(ia(R),axis)=xC(R,axis);
  }
  
//    *wdh*   where( cMin(I)!=-2 )
//     where( pmask(I)==-1 )
//     {
//       for( int axis=0; axis<domainDimension; axis++ ) 
// 	r(I,axis)=xC(I,axis);
//     }
  
 // ??? what about rx ???

  timeForSeg1 += getCPU() - time0;
}



//=================================================================================
// get a mapping from the database
//=================================================================================
int TrimmedMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering TrimmedMapping::get" << endl;
  subDir.setMode(GenericDataBase::streamInputMode);

  subDir.get( TrimmedMapping::className,"className" ); 
  if( TrimmedMapping::className != "TrimmedMapping" )
  {
    cout << "TrimmedMapping::get ERROR in className!, got=[" << (const char *) TrimmedMapping::className 
         << "]" << endl;
  }

  aString curveClassName;
  int surfaceExists;
  subDir.get(surfaceExists,"surfaceExists");
  assert( surfaceExists==0 || surfaceExists==1 );
  if( surfaceExists )
  {
    subDir.get(curveClassName,"surface class name");
    surface = Mapping::makeMapping( curveClassName );  // ***** this does a new -- who will delete? ***
    if( surface==NULL )
    {
      cout << "TrimmedMapping::get:ERROR unable to make the surface mapping with className = " 
	  << (const char *)curveClassName << endl;
      return 1;
    }
    surface->incrementReferenceCount();    // ***wdh980218
    surface->get(subDir,"surface");
//    cout << "TrimmedMapping::get after getting the surface" << endl;
  }
  else
  {
    cout << "TrimmedMapping::surface not found! \n";
    cout << "TrimmedMapping::get:ERROR unable to make the surface mapping with className = " 
	  << (const char *)curveClassName << endl;
    
    surface=NULL;
  }
  
  subDir.get(numberOfTrimCurves,"numberOfTrimCurves");
  if ( numberOfTrimCurves>0 )
    trimCurves = new Mapping * [numberOfTrimCurves];
  trimOrientation.redim(numberOfTrimCurves);
  char buff[80];
  for ( int i=0; i<numberOfTrimCurves; i++ )
    {
      sprintf(buff,"trimCurveClassName%4.4i",i);
      subDir.get(curveClassName, buff);
      trimCurves[i] = Mapping::makeMapping( curveClassName );
      if ( trimCurves[i] == NULL )
	{
	  cout << "TrimmedMapping::get:ERROR unable to make the trimming curve mapping with className = " 
	       << curveClassName << endl;
	  return 1;
	}
      trimCurves[i]->incrementReferenceCount();
      sprintf(buff, "trimCurve%4.4i",i);
      trimCurves[i]->get(subDir, buff);
//      cout << "TrimmedMapping::get after getting trimcurve " << i << endl;
    }
  
  subDir.getDistributed(trimOrientation, "trimOrientation");


  subDir.getDistributed(projectedMask,"projectedMask");

  subDir.getDistributed(grid,"grid");  
  subDir.get(rBound[0],"rBound",6);

  subDir.get(smallestLengthScale, "smallestLengthScale");
  subDir.get(dRmin, "dRmin");
  subDir.get(dSmin, "dSmin");

  if( numberOfTrimCurves>0 )
  {
    delete [] trimmingCurveArcLength;
    trimmingCurveArcLength = new real[numberOfTrimCurves];
    subDir.get(trimmingCurveArcLength,"trimmingCurveArcLength",numberOfTrimCurves);
  }

// AP: We don't put or get bools
  int upToDateI, validTrimmingI, allNurbsI;
  
  subDir.get(upToDateI,      "upToDate"); // bool
  upToDate = (bool) upToDateI;

  upToDate=false;   // we need to initialize at least rCurve in initialize() *wdh* 010322
  
  subDir.get(validTrimmingI, "validTrimming"); // bool
  subDir.get(allNurbsI,      "allNurbs"); // bool
  validTrimming = (bool) validTrimmingI;
  allNurbs = (bool) allNurbsI;

  int triangulationExists=false;
  subDir.get( triangulationExists,"triangulationExists");  
  if( triangulationExists )
  {
    if( triangulation==NULL ) 
    {
      triangulation= new UnstructuredMapping;
      triangulation->incrementReferenceCount();
    }
    triangulation->get(subDir,"triangulation");
  }


  Mapping::get( subDir, "Mapping" );

  // *wdh* 010901 initialize();

  int quadTreeExists;
  subDir.get(quadTreeExists,"quadTreeExists");
// AP tmp
//  printf("In get(): quadTreeExists: %d\n", quadTreeExists);

  assert( quadTreeExists==0 || quadTreeExists==1 );
  if( quadTreeExists )
  {
    initializeQuadTree(false); // initialize quad tree but do not build.
    
    const real centerX=(rBound[0][0]+rBound[0][1])*.5;
    const real centerY=(rBound[1][0]+rBound[1][1])*.5;
    const real halfWidth = max( rBound[0][1]-rBound[0][0], rBound[1][1]-rBound[0][1] )*.5;

    quadTreeMesh = new TMquadRoot( *this,  centerX, centerY, halfWidth );
    quadTreeMesh->TMget( subDir, "quadTreeMesh", *this );
    quadTreeMesh->getStatics( subDir );
  }

  delete &subDir;
  return 0;
}

int TrimmedMapping::
put( GenericDataBase & dir, const aString & name) const
// =================================================================================
// /Description:
//    Save to a database.
// ================================================================================
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( TrimmedMapping::className,"className" ); 

  aString curveClassName;
  int surfaceExists=surface==NULL ? 0 : 1;
  subDir.put( surfaceExists,"surfaceExists");  
  if( surface!=NULL )
  {
    subDir.put(surface->getClassName(),"surface class name");
    surface->put(subDir,"surface");
  }

  subDir.put(numberOfTrimCurves, "numberOfTrimCurves");
  char buff[80];
  for ( int i=0; i<numberOfTrimCurves; i++ )
    {
      sprintf(buff, "trimCurveClassName%4.4i",i);
      subDir.put(trimCurves[i]->getClassName(), buff);
      sprintf(buff,"trimCurve%4.4i",i);
      trimCurves[i]->put(subDir,buff);
    }
  subDir.putDistributed(trimOrientation, "trimOrientation");

  subDir.putDistributed(projectedMask,"projectedMask");

  subDir.putDistributed(grid,"grid");     // **** save the grid for plotting *****
  subDir.put(rBound[0],"rBound",6);

  subDir.put(smallestLengthScale, "smallestLengthScale");
  subDir.put(dRmin, "dRmin");
  subDir.put(dSmin, "dSmin");

  if( numberOfTrimCurves>0 )
  {
    subDir.put(trimmingCurveArcLength,"trimmingCurveArcLength",numberOfTrimCurves);
  }

  subDir.put((int) upToDate,      "upToDate");     // bool
  subDir.put((int) validTrimming, "validTrimming");// bool
  subDir.put((int) allNurbs,      "allNurbs");     // bool

  int triangulationExists=triangulation==NULL ? 0 : 1;
  subDir.put( triangulationExists,"triangulationExists");  
  if( triangulationExists )
  {
    triangulation->put(subDir,"triangulation");
  }


  Mapping::put( subDir, "Mapping" );


  int quadTreeExists = (quadTreeMesh == NULL)? 0:1;
  subDir.put(quadTreeExists,"quadTreeExists");
// AP tmp
//  printf("In put(): quadTreeExists: %d\n", quadTreeExists);
  if( quadTreeExists )
  {
    quadTreeMesh->put( subDir, "quadTreeMesh" );
    quadTreeMesh->putStatics( subDir );  // better to call from put, so wdb in right dir
  }


  delete &subDir;

  return 0;
}

Mapping *TrimmedMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==TrimmedMapping::className )
    retval = new TrimmedMapping();
  return retval;
}


bool TrimmedMapping::
hasTriangulation() const
// ===============================================================================================
/// \details 
///  return true if there is a triangulation computed
// ===============================================================================================
{
  return triangulation!=NULL;
}


UnstructuredMapping & TrimmedMapping::
getTriangulation()
// ===============================================================================================
/// \details 
///     Return the triangulation for the TrimmedMapping (compute it first if necessary).
// ===============================================================================================
{
  if( triangulation==NULL )
    triangulate();

  assert( triangulation!=NULL );
  return *triangulation;
}

void TrimmedMapping::
triangulate( )
// ===============================================================================================
/// \details 
///     Triangulate a TrimmedMapping.
// ===============================================================================================
{
// for debugging
  const bool debugKnots=false;
  if (debugKnots)
    printf(">>>> TrimmedMapping::Entering triangulate\n");

  real time0=getCPU();

  TriangleWrapper triangle;
  if( triangulation==NULL )
  {
    triangulation= new UnstructuredMapping; triangulation->incrementReferenceCount();
  }

  TriangleWrapperParameters & triangleParameters = triangle.getParameters();

  real maximumArea = maxArea>0. ? maxArea : defaultMaximumAreaForTriangulation;
  // scale maximumArea by the jacobian
  if( maximumArea>0. )
  {
    // Since we triangulate in parameter space we need to 
    // approximately scale the maximum area by the area of the patch
    real xScale=REAL_MIN*10.;
    realArray rg(1,2),xg(1,3),dxdr(1,3,2);
    rg(0,0) = .5;
    rg(0,1) = .5;
    map(rg,xg,dxdr);

    maximumArea /= (.5*sqrt( SQR(dxdr(0,1,0)*dxdr(0,2,1)-dxdr(0,1,1)-dxdr(0,2,0)) + 
			 SQR(dxdr(0,0,0)*dxdr(0,2,1)-dxdr(0,0,1)-dxdr(0,2,0)) +
			 SQR(dxdr(0,0,0)*dxdr(0,1,1)-dxdr(0,0,1)-dxdr(0,1,0)) ));

    const real minArea=SQR(100.*REAL_EPSILON); // This is a guess, may be too small
    if( sqrt(maximumArea) < sqrt(minArea) )
    {
      printf("WARNING: parameter-space-maxArea=%8.2e too small, setting to %8.2e\n",maximumArea,minArea);
      maximumArea=minArea;
    }
    if( Mapping::debug & 8 )
      printf("TrimmedMapping: triangulate: input: maximumArea=%8.2e -> parameter-space-maxArea=%8.2e \n",
	   maximumArea,maximumArea);
  }

  triangleParameters.setMaximumArea(maximumArea);
  triangleParameters.saveNeighbourList();
  triangleParameters.saveVoronoi(false);

  triangleParameters.setQuietMode(true);

  
  // NOTE: We triangulate the trimmed mapping in the parameter space r.

  real xScale=REAL_MIN;
  int axis;
  for( axis=0; axis<rangeDimension; axis++ )
    xScale=max(xScale,(real)getRangeBound(End,axis)-(real)getRangeBound(Start,axis));

  // First make a list of faces and vertices on the boundaries of the trimming curves.

  int numberOfFaces=0, numberOfNodes=0;
  realArray xyz;
  intArray faces;
  Range R2=2, R;
      
  realArray holes;

  int numberOfInnerCurves=sum(trimOrientation==-1);
  int numberOfOuterCurves=sum(trimOrientation==1);

  //  printf(" *** numberOfInnerCurves=%i numberOfOuterCurves=%i \n",numberOfInnerCurves,numberOfOuterCurves);
  
  if( numberOfInnerCurves>0 )
    holes.redim(numberOfInnerCurves,2);

  int numberOfCurves=numberOfOuterCurves==0 ? numberOfTrimCurves+1 : numberOfTrimCurves;

  if ( numberOfOuterCurves==0 ) printf("triangulate: WARNING : adding outer trim curve to triangulation\n");

  intArray  *fc = new intArray  [numberOfCurves];   // holds faces on each trimming curve
  realArray *rc = new realArray [numberOfCurves];   // holds nodes on each trimming curve.
  const real knotEpsilon = REAL_EPSILON*10.;

  real totalArea = 0;

  int c,c0=-1,innerCurve=0;
  for( c=0; c<numberOfCurves; c++ )
  {
    realArray & r = rc[c];
    int numberOfGridPoints;

    if (debugKnots) printf("triangulate: Trim curve # %i\n", c); // debugging

    if( c==0 && numberOfOuterCurves==0 )
    {
      // If there is no outer curve, use the unit square instead
      numberOfGridPoints=40;
      r.redim(numberOfGridPoints,2);
      int numPerSide=10;
      real dr=1./numPerSide;
      
      R=numPerSide;
      r(R,0).seqAdd(0.,dr);
      r(R,1)=0.;

      R+=numPerSide;
      r(R,0)=1.;
      r(R,1).seqAdd(0.,dr);
      
      R+=numPerSide;
      r(R,0).seqAdd(1.,-dr);
      r(R,1)=1.;

      R+=numPerSide;
      r(R,0)=0.;
      r(R,1).seqAdd(1.,-dr);
    }
    else
    {
      c0++;
      Mapping & map = *trimCurves[c0];

      totalArea += getArea(map);

      if( map.getClassName()=="NurbsMapping" )
      {
	NurbsMapping & nurb = (NurbsMapping&)map;
	const RealArray & knot = nurb.getKnots();

	if (debugKnots)
	{
	  realArray endPnts(2,1), rEndPnts(2,2);
	  endPnts(0,0) = 0.;
	  endPnts(1,0) = 1.;
	  for (int qq=0; qq<nurb.numberOfSubCurves(); qq++)
	  {
	    nurb.subCurve(qq).map(endPnts, rEndPnts);
	    printf("Subcurve %i starts at (%e,%e) and ends at (%e,%e)\n", qq, rEndPnts(0,0), rEndPnts(0,1), 
		   rEndPnts(1,0), rEndPnts(1,1));
	  }
	}

	if (debugKnots)
	  ::display(knot,"triangulate: knots");

	int n = knot.getLength(0)-1;  // AP: knot(0:n) are defined
	if (debugKnots)
	  printf("triangulate: There are %i knots in the array \n",n+1); 

        // *wdh* 031123 :
        //    There are periodic Nurbs with knots that lie in the range [-a,1+a] with a>0 (see f16, patch 156)
        const bool isPeriodic = nurb.getIsPeriodic(axis1)==functionPeriodic;
	
        if( (!isPeriodic && (fabs(knot(0))>knotEpsilon || fabs(knot(n)-1.) > knotEpsilon)) ||
	    ( isPeriodic && ( knot(0)>knotEpsilon || knot(n)<(1.-knotEpsilon) ) )  )
	{
	  printf("TrimmedMapping:ERROR: expecting knot(0)<=0 and knot(n)>=1 : knot(0)=%e, knot(n)=%e "
                 "(isPeriodic=%i) knotEpsilon=%8.2e \n",
		 knot(0),knot(n),(int)isPeriodic,knotEpsilon);
	  throw "error";
	}
	
        // skip over final 1's (or knots >1 if periodic)
	while( fabs(knot(n-1)-1.) < knotEpsilon || 
               ( isPeriodic && knot(n-1)>1.-knotEpsilon ) ) // *wdh* 
	{
	  if (debugKnots) printf("Skipping trailing knot = %e\n", knot(n-1));
	  n--;
	}
	
	if (debugKnots)
	  printf("# knots after skipping final 1's: %i\n",n+1); 
	realArray r0(n);
        IntegerArray keepKnot(n); // mark points with multiple knots as points to keep -- corners --
	keepKnot=0;
	
	int j=0;
	r0(j)=0.; keepKnot(j)=1; j++;

        int i=1; 
        // skip repeated 0's at the start (or knots <0 if periodic)
	while( fabs(knot(i)) < knotEpsilon || // AP allowed for roundoff, used to test == 0. 
               ( isPeriodic && knot(n)<knotEpsilon ) ) // *wdh* 
	{
	  if (debugKnots) printf("Skipping leading knot = %e\n", knot(i));
          i++;
	}
	if (debugKnots)
	  printf("# knots after skipping starting 0's: %i\n",n-i+1+1); 
	for( ; i<n; i++ ) 
	{
	  if (debugKnots)
	    printf("knot(%i) = %e\n", i-1, knot(i-1));
	  if( knot(i)!=knot(i-1) )
	  {
	    r0(j)=knot(i); j++;
	  }
          else
	  {
            keepKnot(j-1)=1;
	  }	  
	}
	if (debugKnots)
	  printf("knot(%i) = %e\n", n, knot(n));

        keepKnot(j-1)=1;
	numberOfGridPoints=j;
	if( debugKnots )
	{
	  printf("Initially using %i knots \n",numberOfGridPoints);
	  for (i=0; i<numberOfGridPoints; i++)
	    printf("r0(%i)=%e\n", i, r0(i));
	}
	
	r0.resize(numberOfGridPoints);

	// add in more boundary nodes, so that the total number of boundary nodes = map.getGridDimension(0)-1
	if( numberOfGridPoints < map.getGridDimensions(0)) // map is a reference to the trimming curve
	{
          int minNumberOfGridPoints=map.getGridDimensions(0);
	  int maxNumberOfGridPoints=minNumberOfGridPoints+numberOfGridPoints;
	  realArray r2(maxNumberOfGridPoints);
	  IntegerArray keepKnot2(maxNumberOfGridPoints); 

	  real dr = 1./max(1,minNumberOfGridPoints); // points should not be farther apart than this value.

          r2(0)=r0(0);
          keepKnot2(0)=keepKnot(0);
          
          for( i=1,j=1; i<numberOfGridPoints; i++)
 	  {
            real dist=fabs(r0(i)-r0(i-1));
	    if( dist>dr )
	    {
	      // points are too far apart, add more in between
              int numToAdd = int(dist/dr);
              real delta = dist/(numToAdd+1);
	      for( int k=0; k<numToAdd; k++ )
	      {
		r2(j)=r2(j-1)+delta;
                keepKnot2(j)=0;
                j++;
	      }
	    }
	    r2(j)=r0(i);  keepKnot2(j)=keepKnot(i);
            j++;
	  }
          // last point, periodic wrap.
          real rLast=1.;
	  real dist=fabs(rLast-r0(numberOfGridPoints-1));
	  if( dist>dr )
	  {
	    // points are too far apart, add more in between
	    int numToAdd = int(dist/dr);
	    real delta = dist/(numToAdd+1);
	    for( int k=0; k<numToAdd; k++ )
	    {
	      r2(j)=r2(j-1)+delta;
	      keepKnot2(j)=0;
	      j++;
	    }
	  }

/* ------------
// 	    for (i=0; i<nUniform; i++)
// 	      rUniform(i) = i*dr; // 0, dr, 2*dr, ..., 1-dr
	  rUniform.seqAdd(0.,dr);

	  IntegerArray keepKnot2(nUniform); 
	  keepKnot2=0;

	  // move the closest point in rUniform to match r0(j), j=0,1,...,numberOfGridPoints-1
	  int iFloor;
	  for (j=0; j<numberOfGridPoints; j++)
	  {
	    iFloor = (int) (r0(j)/dr);
	    if (iFloor > 0 && iFloor <= nUniform-2)
	    {
	      //          printf("Moving gridpoints to match knot #%i. r[%i]:%e < %e <= r[%i]:%e\n", j,
	      //                 iFloor, rUniform(iFloor), r0(j), iFloor+1, rUniform(iFloor+1));
	      if ( fabs(iFloor*dr-r0(j)) < fabs((iFloor+1)*dr-r0(j)) )
	      {
		rUniform(iFloor) = r0(j); keepKnot2(iFloor)=keepKnot(j);
	      }
	      else
	      {
		rUniform(iFloor+1) = r0(j); keepKnot2(iFloor+1)=keepKnot(j);
	      }
	    }
	  } // end for j
	  // copy the result to r0
	  numberOfGridPoints = nUniform;
----- */
          numberOfGridPoints=j;
	  r0.redim(numberOfGridPoints);
          Range J=numberOfGridPoints;
	  r0 = r2(J);
          
	  keepKnot.redim(numberOfGridPoints);
	  keepKnot=keepKnot2(J);
	  
	} // end if numberOfGridPoints < map.getGridDimension(0)
	
	r.redim(numberOfGridPoints,2);

	map.map(r0,r);

	if (debugKnots)
	{
	  ::display(r0,"selected knots: r0");
	  ::display(r,"r");
	}

        // ------- remove points that are too close together in x-space ------------
        // this may happen if the nurbs has a poor parameterization (ex. asmo surface 7)
        assert( trimmingCurveArcLength!=NULL );
	const real eps=1.e-2*max(1.e-4,trimmingCurveArcLength[c0]); 

        // printf(" Trim curve %i: arclength=%e eps=%e\n",c0,trimmingCurveArcLength[c0],eps);
	
// AP In double precision it is not always enough to shift a knots by 500*REAL_EPSILON, see kvlcc2.igs, surface 8.
	const real epsDuplicate=FLT_EPSILON*50.;
//	const real epsDuplicate=REAL_EPSILON*50.;
	int numberOfCorners=0;
	IntegerArray cornerIndex(numberOfGridPoints); // hold corners
	
        realArray r2(numberOfGridPoints,2);
        i=0,j=0;
        r2(j,0)=r(i,0), r2(j,1)=r(i,1); j++;
// the first point is always a corner point
	if (debugKnots)
	  printf("Initializing knot %i as corner # %i\n", i, numberOfCorners);
	cornerIndex(numberOfCorners)=i;
	numberOfCorners++;

        for( i=1; i<numberOfGridPoints-1; i++ )
	{
          real dist = SQRT( SQR(r(i,0)-r2(j-1,0))+SQR(r(i,1)-r2(j-1,1)) ); // ****** use fabs ****

	  real d2 = SQR( r0(i+1)-r0(i-1) );
	  real xrr = fabs( r(i+1,0)-2.*r(i,0)+r(i-1,0) )+fabs( r(i+1,1)-2.*r(i,1)+r(i-1,1) );
	  xrr/=d2*trimmingCurveArcLength[c0];
	  if (debugKnots)
	    printf(" i=%i r0=%10.4e r=(%10.4e,%10.4e) dist=%8.2e xrr/d2/arcLength=%8.2e\n",
		   i,r0(i),r(i,0),r(i,1),dist,xrr);

          if( keepKnot(i) ) 
	  {
            // check if we hit a previous corner (cf. plate.igs :  surface 27)
	    if (debugKnots)
	      printf(" keepKnot(i=%i) is true, numberOfCorners=%i:\n", i, numberOfCorners);
            for( int k=0; k<numberOfCorners; k++ )
	    {
	      int kk=cornerIndex(k);
	      real dist2=fabs(r(i,0)-r(kk,0))+fabs(r(i,1)-r(kk,1));
	      if (debugKnots)
		printf(" corner(kk=%i) dist=%e\n",kk,dist2);
	      
              if( dist2<epsDuplicate )
	      {
		printf("***WARNING*** trimming curve intersects itself at a knot. Points\n"
                       "r(%i)=(%8.2e,%8.2e) and r(%i)=(%8.2e,%8.2e), dist=%e,\n"
		       "I will shift the knot to allow triangle to work\n",i,r(i,0),r(i,1),kk,r(kk,0),r(kk,1),dist2);

                minAngleForTriangulation=5.; // *wdh* 031121 reduce allowable angle to we don't get too many triangles
		
                real dist3=fabs(r(i,0)-r(i-1,0))+fabs(r(i,1)-r(i-1,1));
                if( dist3>epsDuplicate*10. )
		{
		  real alpha=min(.5,epsDuplicate*10./dist3);

		  r(i,0)=r(i,0)*(1-alpha)+r(i-1,0)*alpha;
		  r(i,1)=r(i,1)*(1-alpha)+r(i-1,1)*alpha;
		  dist2=fabs(r(i,0)-r(kk,0))+fabs(r(i,1)-r(kk,1));
		  assert( dist2 > epsDuplicate );
		}
		else
		{
		  printf("Unable to shift *fix this Bill* -- will remove it for now.\n");
		  dist=eps*10.;
		  xrr=0.;
		  keepKnot(i) =0;
		}
		
	      }
	    } // end for k=0,...,numberOfCorners-1
	    if (debugKnots)
	      printf("Adding knot %i as corner # %i\n", i, numberOfCorners);
	    
	    cornerIndex(numberOfCorners)=i;
            numberOfCorners++;
	  } // end if keepknot(i)

          if( dist>eps || xrr > 50. || keepKnot(i) ) 
	  {
            if (debugKnots)
	      printf(" keep point\n");
	    r2(j,0)=r(i,0), r2(j,1)=r(i,1); j++;
	  }
	}
        i=numberOfGridPoints-1;
        
        real dist = min( SQRT( SQR(r(i,0)-r2(j-1,0))+SQR(r(i,1)-r2(j-1,1)) ),
                         SQRT( SQR(r(i,0)-r2(  0,0))+SQR(r(i,1)-r2(  0,1)) ) );
        if( dist>eps ) // do not check for keepKnot(i)
	{
          // printf(" keep point %i : dist=%e \n",i,dist);
	  r2(j,0)=r(i,0), r2(j,1)=r(i,1); j++;
	}
	if( j!= numberOfGridPoints )
	{
          numberOfGridPoints=j;
          Range J=numberOfGridPoints;
	  r.redim(numberOfGridPoints,2);
          r=r2(J,Range(0,1));
	}
	
        if( false ) // use true for debugging
	{
          int im1=numberOfGridPoints-1;
	  for( i=0; i<numberOfGridPoints; i++ )
	  {
	    real dist = SQRT( SQR(r(i,0)-r(im1,0))+SQR(r(i,1)-r(im1,1)) );
 	    printf("*final* i=%i r=(%10.4e,%10.4e) im1=%i : r=(%10.4e,%10.4e) dist=%8.2e\n",i,
                 r(i,0),r(i,1), im1,r(im1,0),r(im1,1),dist);
            im1=i;
	  }
	}

      }
      else
      {// the trim-curve is not a nurbsmapping
	const realArray & g = map.getGrid();

	// first determine an appropriate number of grid lines to use.
	// Measure the ratio of the curvature to the arclength.
        real elementDensity =defaultElementDensityToleranceForTriangulation;
	if( elementDensityTolerance>0. )
          elementDensity=elementDensityTolerance;

	int base0=g.getBase(0), bound0=g.getBound(0);
	Range I1=Range(base0+1,bound0-1);

	// Range I(base0+1,bound0);
	real arcLen, curvature, maxCurvature=0.;
	// arcLength=sum(SQRT( SQR( g(I,0,0,0)-g(I-1,0,0,0) ) + 
	//		    SQR( g(I,0,0,1)-g(I-1,0,0,1) ) ));
        arcLen=map.getArcLength();
	arcLen=max(arcLen,1.e-4);  
    
	realArray d2(I1,2);
	for( int axis=0; axis<=1; axis++ )
	  d2(I1,axis)=g(I1+1,0,0,axis)-2.*g(I1,0,0,axis)+g(I1-1,0,0,axis);

	curvature = SQRT(max( SQR(d2(I1,0))+SQR(d2(I1,1)) ))/ arcLen;
	// printf(" i2=%i, arcLen=%e, curvature=%e\n",i2,arcLen,curvature);

	maxCurvature= max( maxCurvature,curvature );
    
	// choose the number of grid points based on curvature AND arcLen.
	numberOfGridPoints = int( SQRT( maxCurvature/elementDensity ) * (bound0-base0+1)
				  + .1*arcLen/elementDensity + .5 );

	numberOfGridPoints = max(3,numberOfGridPoints);

	printf("maxCurvature=%e, elementDensity=%e, old number of grid pts=%i, new num=%i\n",
	       maxCurvature,elementDensity,bound0-base0+1,numberOfGridPoints);

	realArray r0(numberOfGridPoints,1);
	r0.seqAdd(0.,1./numberOfGridPoints);  // note: leave off periodic point
	
	r.redim(numberOfGridPoints,2);
	map.map(r0,r);
      }
    }
    
    int n=numberOfGridPoints;
    R=n;

    numberOfNodes+=n;
    numberOfFaces+=n;
    intArray & f = fc[c];
	
    f.redim(n,2);
    f(R,0).seqAdd(0,1);
    f(R,1)=f(R,0)+1;
//     for( int i=0; i<n; i++ )
//     {
//       f(i,0)=i;
//       f(i,1)=i+1;
//     }
    f(n-1,1)=0;  // periodic wrap

//     for( int i=0; i<n; i++ )
//       printf(" faces: i=%i f=%i,%i \n",i,f(i,0),f(i,1));
      
    if( c0>=0 && trimOrientation(c0)==-1 )
    {
      // find a point inside the trimming curve.

      // kkc average of curve vertices gave incorrect hole seeds in
      //     some cases.  The hole seeds are now computed from the first
      //     segment in the curve given by r.  The seed is placed
      //     FLT_EPSILON from the midpoint of the segment, in a direction
      //     normal to the segement towards the inside of the hole.

      //      for( int axis=0; axis<domainDimension; axis++ )
      //	holes(innerCurve,axis)= sum(r(R,axis))/n;     // --- this could be wrong --- fix this !

      real nr1 = r(1,1)-r(0,1);
      real nr2 = r(0,0)-r(1,0);
      real nm = sqrt(nr1*nr1 + nr2*nr2);
      holes(innerCurve,0) = 0.5*(r(0,0)+r(1,0)) + FLT_EPSILON*nr1/nm;
      holes(innerCurve,1) = 0.5*(r(0,1)+r(1,1)) + FLT_EPSILON*nr2/nm;

      innerCurve++;
    }
  }
  
  if( domainDimension!=2  )
  {
    printf("TrimmedMapping::triangulate:ERROR: domainDimension==%i but expecting 2 \n",domainDimension);
    throw "error";
  }
  
  xyz.redim(numberOfNodes,domainDimension);
  faces.redim(numberOfFaces,2);
      
  int nodeOffset=0, faceOffset=0;
  for( c=0; c<numberOfCurves; c++ )
  {
    realArray & r = rc[c];
    intArray & f = fc[c];

    // printf("**** points on curve c=%i \n",c);
    // ::display(r,"r");

    if( r.getLength(0)>0 )
    {
      R=r.dimension(0);
      xyz(R+nodeOffset,R2)=r(R,R2);

      faces(R+faceOffset,R2)=f(R,R2)+nodeOffset;

      nodeOffset+=R.getLength();
      faceOffset+=R.getLength();
    }
  }
  delete [] rc;
  delete [] fc;
	



  // *** add some points that cover the domain ***
  int numberOfGridPoints[2];
  bool collapsedEdge[2][3];
  real averageArclength[2];
  // number of points is based on curvature/elementDensity
  real elementDensity =defaultElementDensityToleranceForTriangulation;
  if( elementDensityTolerance>0. )
    elementDensity=elementDensityTolerance;

  assert( surface!=NULL );
  real time1=getCPU();
  surface->determineResolution(numberOfGridPoints,collapsedEdge,averageArclength,elementDensity );
  real resolutionTime=getCPU()-time1;

  int nx=numberOfGridPoints[0], ny=numberOfGridPoints[1];
  // printf("*** TrimmedMapping: after determine resolution nx=%i ny=%i\n",nx,ny);

  // If the surface was made with a lot of points then keep more
  real nxScale = 4.*elementDensity/.05;  // =4.
  
  nx=max(numberOfGridPoints[0],int(surface->getGridDimensions(0)/nxScale));
  ny=max(numberOfGridPoints[1],int(surface->getGridDimensions(1)/nxScale));
  
  real aspectRatio=averageArclength[0]/max(REAL_MIN,averageArclength[1]);
  // printf("*** TrimmedMapping: aspectRatio = %e, nx=%i ny=%i\n",aspectRatio,nx,ny);

  aspectRatio=max(.001,aspectRatio); // *wdh* 031121 

  // increase nx or ny to prevent a large aspect ratio
  const real maximumTriangleAspectRatio=2.5;
  if( aspectRatio/nx > maximumTriangleAspectRatio/ny )
  {
    nx=int (aspectRatio/(maximumTriangleAspectRatio/ny) );
  }
  else if( 1./ny > maximumTriangleAspectRatio*aspectRatio/nx )
  {
    ny=int (1. / (maximumTriangleAspectRatio*aspectRatio/nx));
  }
  // printf(" New: nx=%i, ny=%i \n",nx,ny);
  
  

  // ** look for nodes on the trimming curves  that are very close to the background grid
  //  const real epsx=.1, epsy=.1;
  // const real epsx=.25, epsy=.25;
  IntegerArray mask(nx,ny);
  mask=1;
  int numberRemoved=0;
  int j=numberOfNodes-1;  // previous node to i
  for( int i=0; i<numberOfNodes; i++ )
  {
    real r0 = xyz(i,0)*(nx-1), r1=xyz(i,1)*(ny-1);
    real s0 = xyz(j,0)*(nx-1), s1=xyz(j,1)*(ny-1);

    real dr = 1.5+fabs(r0-s0)+fabs(r1-s1);
    int numberOfSubSteps= int(dr);
    for( int k=0; k<numberOfSubSteps; k++ )
    {
      real delta = k/real(numberOfSubSteps);
      real ra=r0+delta*(s0-r0);
      real rb=r1+delta*(s1-r1);
      
      int i0 = int(ra+.5), i1=int(rb+.5);

      if( i0>=0 && i0<nx && i1>=0 && i1<ny && mask(i0,i1) )
      {
// 	printf(" Curve face (%8.2e,%8.2e)- (%8.2e,%8.2e) pt (%8.2e,%8.2e) is close to background grid point (%i,%i)\n",
// 	    r0,r1,s0,s1,ra,rb,i0,i1);
	numberRemoved++;
	mask(i0,i1)=0;
      }
    }
    
    j=i;
  }
  


  Range I1=nx, I2=ny;
  Range I=nx*ny;
	
  if( false )
  {
    numberRemoved=nx*ny;
  }
  else
  {
    realArray r2(nx,ny,2);
    for( int i2=0; i2<ny; i2++ )
      r2(I1,i2,1)=i2/real(ny-1);
    for( int i1=0; i1<nx; i1++ )
      r2(i1,I2,0)=i1/real(nx-1);
	
    r2.reshape(nx*ny,2);
    xyz.resize(numberOfNodes+nx*ny-numberRemoved,2);
    if( numberRemoved==0 )
    {
      xyz(I+numberOfNodes,R2)=r2;
    }
    else
    {
      // printf("*** %i background grid nodes were removed since they were close to trimming curve nodes\n",numberRemoved);
      mask.reshape(nx*ny);
      int j=numberOfNodes;
      for( int i=0; i<nx*ny; i++ )
      {
	if( mask(i) )
	{
	  xyz(j,0)=r2(i,0); xyz(j,1)=r2(i,1);
	  j++;
	}
      }
      assert( j == (numberOfNodes+nx*ny-numberRemoved) );
    }
  }
  
  bool scaleNodes=aspectRatio>1.5 || aspectRatio<.5 ;
  I=numberOfNodes+nx*ny-numberRemoved;
  Range all;
  if( scaleNodes )
  {
    xyz(I,0)*=aspectRatio;  // scale so that triangles will have a better shape on final grid.
    holes(all,0)*=aspectRatio;
  }
  
  if( false )
  {
    cout<<totalArea<<endl;
    ::display(holes,"holes");
    int num=xyz.getBound(0);
    printf(" *** number of nodes to triangle = %i\n",num);
    for( int i=0; i<=num; i++ )
    {
      printf(" node %i: (%14.8e,%14.8e) \n",i,xyz(i,0),xyz(i,1));
    }
  }
  

  // Set angle here since we have have changed minAngleForTriangulation
  // printf(" **** TrimmedMapping::triangulate: minAngleForTriangulation=%e\n",minAngleForTriangulation);
  if( minAngleForTriangulation>0. )
    triangleParameters.setMinimumAngle(minAngleForTriangulation); // default is 20. use smaller value for more robust
  else if( defaultMinAngleForTriangulation>0. )
    triangleParameters.setMinimumAngle(defaultMinAngleForTriangulation); // default is 20. use smaller value for more robust

  //  triangleParameters.setMinimumAngle(0.); // default is 20. use smaller value for more robust


  triangle.initialize( faces,xyz );
  if( numberOfInnerCurves>0 )
    triangle.setHoles( holes );

      // Note that there may be new nodes introduced.
  time1=getCPU();
  triangle.generate();
  real triangleTime=getCPU()-time1;

  const int maxNumberOfTriangles=10000;  
  int numberOfTriangles=triangle.generateElementList().getLength(0); 

  if( Mapping::debug & 4 )
    printf("TrimmedMapping::triangulate: numberOfTriangles=%i\n",numberOfTriangles);
  
  if( numberOfTriangles>maxNumberOfTriangles )
  {
    // too many triangles -- reduce this
    const real minTheta=0.;  // It seems necessary to use 0. here (1.e-3 was not good enough, f16  surface 688)
    triangleParameters.setMinimumAngle(minTheta);
    printf("TrimmedMapping::WARNING The triangulation for the TrimmedMapping has %i triangles\n"
           "   This could be a trimmed mapping with a very narrow region.\n"
           "   I am going to regenerate the triangulation with minTheta=%8.2e so that it has fewer... \n"
           ,numberOfTriangles,minTheta);
    
    triangle.generate();

    printf("   ...new number of triangles =%i\n",triangle.generateElementList().getLength(0));
  }


  const intArray & elements = triangle.generateElementList();
  const realArray & r = triangle.getPoints();
  const intArray & neighbours = triangle.getNeighbours();
  numberOfTriangles=elements.getLength(0);
  if( numberOfTriangles==0 )
  {
    printf("***TrimmedMapping::triangulate:ERROR: numberOfTriangles==0, something is wrong here\n");
    invalidateTrimming();
    return;
  }
  

  if( false )
  {
    for( int i=0; i<numberOfTriangles; i++ )
    {
      printf(" triangle %i: nodes=(%i,%i,%i) neighbours=(%i,%i,%i)\n",
	     i,elements(i,0),elements(i,1),elements(i,2),neighbours(i,0),neighbours(i,1),neighbours(i,2));
    }
  }
  if( scaleNodes )
  {
    r(all,0)*=1./aspectRatio;   // scale back to [0,1]
  }
  

  realArray nodes(r.dimension(0),3);
  assert( surface!=NULL );
  time1=getCPU();
  surface->map( r,nodes );   // compute 3d positions of triangle nodes.
  real nurbTime=getCPU()-time1;

      // *old way* triangulation.setNodesAndConnectivity(nodes,elements,domainDimension );
  numberOfFaces=triangle.getNumberOfEdges();
  int numberOfBoundaryFaces=triangle.getNumberOfBoundaryEdges();
  triangulation->setNodesElementsAndNeighbours(nodes,elements,neighbours,
					      numberOfFaces,numberOfBoundaryFaces);

  triangulation->setName(mappingName,getName(mappingName)+"-unstructured");
    
  real time=getCPU()-time0;
#if 0
  printf("TrimmedMapping::built unstructured grid for %s, cpu=%8.2e (triangle=%8.2e, resolution=%8.2e, nurb=%8.2e)\n",
            (const char*)getName(mappingName),time,triangleTime,resolutionTime,nurbTime);
  
#endif
  // set the range bounds from the triangulation since they will be more accurate 
  for(axis=0; axis<rangeDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      setRangeBound(side,axis,(real) triangulation->getRangeBound(side,axis));
    }
  }
  
}


void TrimmedMapping::
triangulate(MappingInformation & mapInfo)
// ===================================================================================================
// /Description:
//    An interactive interface to the triangulation function.
//
// ===================================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
//  PlotStuff & gi = (PlotStuff&)( *mapInfo.graphXInterface );
  
  char buff[180];  // buffer for sPrintF
  aString menu[] = 
  {
    "!Triangulate a TrimmedMapping",
    "compute",
    "plot",
    "set maximum area",
    "exit", 
    "" 
  };


  aString answer,line;

  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  parameters.set(GI_PLOT_UNS_EDGES,true);
  
  for( int it=0;; it++ )
  {
    gi.getMenuItem(menu,answer);

    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="set maximum area" )
    {
      gi.inputString(line,"Enter the maximum area");
      if( line!="" )
      {
	sScanF(line,"%e",&maxArea );
        printf("maximum area =%e\n",maxArea);

      }
      
    }
    else if( answer=="compute" )
    {

      triangulate();
      assert( triangulation!=NULL );
      
      gi.erase();
      PlotIt::plot( gi,*triangulation,parameters);

    }
    else if( answer=="plot" )
    {
      if( triangulation!=NULL )
      {
	gi.erase();
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot( gi,*triangulation,parameters);
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      }
    }
    
  }
  

}


static void
buildTrimmedMappingInterface( GUIState & interface )
{
  interface.setWindowTitle("Trimmed Mapping");
  interface.setExitCommand("exit","Exit");

  interface.setOptionMenuColumns(1);
  
  aString opViewCommands[] = { "view domain",
			       "view range",
			       "" };

  aString opViewLabels[] = { "Parameter Space",
			     "Physical Space",
			     "" };
  interface.addRadioBox("View",opViewCommands, opViewLabels, 1);

  aString rb2ViewCommands[] = { "view trimmed",
			       "view untrimmed",
			       "" };

  aString rb2ViewLabels[] = { "Trimmed Surface",
			     "Untrimmed Surface",
			     "" };
  interface.addRadioBox("View",rb2ViewCommands, rb2ViewLabels, 0);

  aString opPickCommands[] = { "Mouse Picking 0",
			       "Mouse Picking 1",
			       "Mouse Picking 2",
			       "Mouse Picking 3",
			       "Mouse Picking 4",
			       "" };

  aString opPickLabels[] = { "query curves",
			     "delete curves",
			     "edit curves",
			     "reverse curves",
			     "turn off picking",
			     "" };

  // interface.addRadioBox("Mouse Picking", opPickCommands, opPickLabels, 4, 2); // 2 columns
  interface.addRadioBox("Mouse picking option", opPickLabels, opPickLabels, 4, 2); // 2 columns

  aString tbCommands[] = { "plot curves",
			   "plot triangulation",
			   "plot grid",
			   "plot subcurves",
			   "" };

  aString tbLabels[] = { "Plot Curves",
			 "Plot Triangulation",
			 "Plot Grid",
			 "Plot Subcurves",
			 "" };

  int tbState[] = { 0, 0, 0, 0};

  interface.setToggleButtons(tbCommands, tbLabels, tbState, 2);

  aString pbCommands[] = { "undo last delete",
			   "new trimcurve",
			   "open mapping dialog",
			   "print trimming info",
			   "validate trimming",
			   "force trimming valid",
			   "refine plot",
			   "" };

  aString pbLabels[] = { "Undo Last Delete",
			 "New Trimcurve",
			 "Mapping Parameters",
			 "Trimming Info",
			 "Validate Trimming",
			 "Force Trimming Valid",
			 "Refine Plot",
			 "" };

  interface.setPushButtons(pbCommands, pbLabels, 3);
//    interface.setSensitive(false, DialogData::pushButtonWidget, 0);

  aString popup[] = { "specify mappings",
		      "plot trimming curves",
		      "plot trimming curves and grid",
		      "edit trimming curves",
		      "edit untrimmed surface",
		      "check inverse",
		      "do lotsa inverses",
		      "triangulate",
		      "" };

  interface.buildPopup(popup);

  aString helpCommands[] = { "help spec", 
			     "help curve",
			     "help curve grid",
			     "help edit curve",
			     "help edit surface",
			     "help inverse",
			     "help lotsa inverse",
			     "help triangulate",
			     "help mapping",
			     "" };
  aString helpLabels[] = { "Specify",
			   "Plot Curves",
			   "Plot Curves & Grid",
			   "Edit Curves",
			   "Edit Surface",
			   "Check Inverse",
			   "Lotsa Inverses",
			   "Triangulate",
			   "Mapping Parameters",
			   "" };
  
  interface.addPulldownMenu("Help", helpCommands, helpLabels, GI_PUSHBUTTON);
  interface.setLastPullDownIsHelp(1);

}

int TrimmedMapping::
update( MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively create and/or change the Trimmed mapping.
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{
  enum radioBoxEnum {viewRadioBox=0, trimmedRadioBox, mouseRadioBox};

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
//  PlotStuff & gi = (PlotStuff&)( *mapInfo.graphXInterface );

  GUIState interface;

  buildTrimmedMappingInterface(interface);

  /// initialize the Mapping parameters dialog box
  DialogData * oldInterface = mapInfo.interface; // could be null, right?
  DialogData & mappingInterface = interface.getDialogSibling();
  mapInfo.interface = & mappingInterface;
  Mapping::updateWithCommand(mapInfo,"build mapping dialog");

  char buff[180];  // buffer for sPrintF

  aString answer,line,answer2; 

  bool plotObject=surface!=0;
  // bool mappingChosen=false;
  bool viewRange = true; // view the trimmed mapping in the range space (if false, view in domain)
  bool viewTrimmed = true; // view the trimmed mapping (if false, view the untrimmed mapping)

  GraphicsParameters parameters, domainParam;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  domainParam.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
  domainParam.set(GI_PLOT_END_POINTS_ON_CURVES, true);
  domainParam.set(GI_PLOT_GRID_POINTS_ON_CURVES, false);

// toggle buttons are curves, triangulation, grid and subcurves
  int dumval;
  interface.setToggleState(0, parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES, dumval));
  interface.setToggleState(1, parameters.get(GI_PLOT_UNS_EDGES, dumval));
// copy these values for the untrimmed plotter
  parameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, parameters.get(GI_PLOT_UNS_EDGES,dumval));
  parameters.set(GI_PLOT_MAPPING_EDGES, parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES,dumval));

  enum OpSelectEnum { queryCurve=0, deleteCurve, editCurve, reverseCurve, noOp };
  OpSelectEnum currentSelectOp = noOp;
  bool plotGrid = false;
  bool plotSubcurves = false;
  interface.setSensitive(false, DialogData::toggleButtonWidget, 2);
  interface.setSensitive(false, DialogData::toggleButtonWidget, 3);
  aString validString = "";

  realArray tmpVertex(1,rangeDimension), tmpR(1,domainDimension),xC(1,rangeDimension), rC(1,domainDimension), dist(1);
  tmpR = -1;
  
  intArray cMin(1);
  //  Mapping **oldTrimmingCurves = NULL;
  //int numberOfOldTrimmingCurves = 0;
  int *curvesToDelete = NULL;

// reset the viewing matrix
  if( gi.isGraphicsWindowOpen() )
    gi.initView(gi.getCurrentWindow());

  SelectionInfo select;

  gi.pushGUI(interface);
  int len=0;
  
  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      gi.getAnswer(answer, "", select);

      gi.savePickCommands(true); // restore
    }

    if( answer=="specify mappings" )
    {
      // Make a menu with the Mapping names, surfaces only
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int i,j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==2 && map.getRangeDimension()==3 && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
	
      int mapNumber = gi.getMenuItem(menu2,answer2,"choose a surface");

      if( answer2=="none" )
        continue;

      assert( mapNumber>=0 && mapNumber<j );
      
      mapNumber=subListNumbering(mapNumber);  // map number in the original list
      Mapping & surfaceNew= mapInfo.mappingList[mapNumber].getMapping();

      // make a menu with curve names
      j=0;
      for( i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
	if( map.getDomainDimension()==1 && map.getRangeDimension()==2 && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
    
      menu2[j-1] = "done";
      Mapping **newTrims = new Mapping *[num];
      int numNewTrims = 0;
      for ( i=0; i<num; i++ )
	{
	  mapNumber = gi.getMenuItem(menu2, answer2,"choose a trimming curve");
	  if ( answer2 == "done" )
	    break;
	  for ( int k=0; k<num; k++ )
	    if ( answer2==menu2[k] )
	      {
		mapNumber = k;
		break;
	      }
	  newTrims[numNewTrims] = &(mapInfo.mappingList[subListNumbering(mapNumber)].getMapping());
	  numNewTrims++;
	}
      
      bool outerBoundaryConstructed=false;
      if( numNewTrims==0 )
      {
	// if there are no trimmed curves specified -- make a curve for the outer boundary *wdh* 010926
        outerBoundaryConstructed=true;
	NurbsMapping *newNurb = new NurbsMapping(1,2);
	newTrims[0] = (Mapping *)newNurb;
	newTrims[0]->incrementReferenceCount();
      
	constructOuterBoundaryCurve(newNurb);  // create an order=1 Nurbs (linear)
        numNewTrims++;

	newTrims[0]=newNurb;
      }
      // We need to assign the bounds from the un-trimmed surface here since getBounds will normally
      // get the bounds from the triangulation which gets the bounds from this mapping.
      setDomainDimension(surfaceNew.getDomainDimension());
      setRangeDimension(surfaceNew.getRangeDimension());
      int side,axis;
      for (side=0; side<=1; side++)
	for (axis=0; axis<getRangeDimension(); axis++)
	{
          // printf("setBounds: side=%i, axis=%i bound=%e\n",side,axis,(real) surface->getRangeBound(side, axis));
	  setRangeBound(side, axis, surfaceNew.getRangeBound(side, axis));
	}

      
      setCurves(surfaceNew, numNewTrims, newTrims);
      // *wdh* 010901 initialize();
      if( outerBoundaryConstructed )
        newTrims[0]->decrementReferenceCount();

      delete [] newTrims;

      delete [] menu2;

      // mappingChosen=TRUE;
      plotObject=TRUE;
    }
    else if( answer=="plot trimming curves" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      gi.erase();
      for ( int i=0; i<numberOfTrimCurves; i++ )
	{
	  // parameters.set(GI_USE_PLOT_BOUNDS, TRUE);
	  PlotIt::plot(gi,*trimCurves[i]);
	}
	
      // parameters.set(GI_USE_PLOT_BOUNDS,FALSE);
      // parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      aString menu[] =
      {
	"continue",
	  ""
	  };
      gi.getMenuItem(menu,answer);
    }
    else if ( answer == "plot trimming curves and grid" )
      {
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
// 	RealArray bounds(2,3);
// 	bounds(0,0) = bounds(0,1) = bounds(0,2) = 0.0;
// 	bounds(1,0) = bounds(1,1) = bounds(1,2) = 1.0;
// 	parameters.set(GI_PLOT_BOUNDS,bounds);
// 	parameters.set(GI_USE_PLOT_BOUNDS,TRUE);

	gi.erase();
	initializeQuadTree();
	for ( int i=0; i<numberOfTrimCurves; i++ )
	  {
	    // parameters.set(GI_USE_PLOT_BOUNDS,TRUE);
	    parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,FALSE);
	    PlotIt::plot(gi,*trimCurves[i], parameters);
	  }
	   
	realArray qtpoints(quadTreeMesh->sizeOfQuadTreeMesh,3);
	qtpoints = 0;
	// ... plotPoints will not work in 2D (a bug?)
	realArray qtcolour(quadTreeMesh->sizeOfQuadTreeMesh);
	qtcolour = 0;
	quadTreeMesh->accumulateCenterPoints(qtpoints,qtcolour,0);
	gi.plotPoints(qtpoints,qtcolour,parameters);

	// parameters.set(GI_USE_PLOT_BOUNDS,FALSE);
	parameters.set(GI_LABEL_GRIDS_AND_BOUNDARIES,TRUE);
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	aString menu[] =
	{
	  "continue",
	  ""
	};
	gi.getMenuItem(menu,answer);
      }
    else if( answer=="do lotsa inverses" )
      {
	timeForFindClosestCurve=0.;
	timeForInsideOrOutside=0.;
	timeForSeg0 = 0;
	timeForSeg1 = 0;
	timeForUntrimmedInverse = 0;
	callsOfFindClosestCurve = 0;
	callsOfFindClosestCurve_all = 0;

	int i,j;
	realArray x(1000,3),r(1000,3),xx(1,3);
	r = -1;
	
	Range R(0,999);
	Range Rx(0,2);
	real time0;
	real timeinv = 0;
	int cycle = int( pow( (real)R.length(), 0.5 ) ); // sqrt(length)

	// Set up some representative numbers to invert.
	// The computation of the scaling factors bot,top,wid involves separate
	// integer variables to discourage the compiler from over-optimizing.
	real* wid = new real[3];
	real* bot = new real[3];
	int ibot, itop;
	real top, scale, marg;
	for ( j=0; j<3; ++j ) {
	  wid[j] = getRangeBound(1,j) - getRangeBound(0,j);
	  scale = real( int(101*wid[j]+0.5) ) / ( 100*wid[j] );
	  marg = fabs( scale -1 );
	  bot[j] = (real)getRangeBound(0,j) - marg;
	  top    = (real)getRangeBound(1,j) + marg;
	  ibot = int( bot[j] );
	  itop = int( top + 1.0 );
	  bot[j] = real( ibot );
	  top    = real( itop );
	  wid[j] = top - bot[j];
	};
	for ( i=R.getBase(); i<=R.getBound(); ++i ) {
	  // for length=100, with the nonrandom x(i,2), and before scaling:
	  // x=0.0,0.0,0.00; 0.0,0.1,0.01; 0.0,0.2,0.02;...; 0.0,0.9,0.09;
	  //   0.1,0.0,0.10; 0.1,0.1,0.11; 0.1,0.2,0.12;...
	  //   0.9,0.0,0.90;...                              0.9,0.9,0.99
	  x(i,0) = (float)((int)( (float)i / cycle )) / cycle;
	  x(i,1) = ((float)(i%cycle))/cycle;
	  // x(i,2) = ((float)(i%100))/100.0;
	  x(i,2) = drand48();
	  for ( int j=0; j<3; ++j ) x(i,j) = x(i,j)*wid[j] + bot[j];
	};

	// Compute the inverse, and print the results.
	time0 = getCPU();
	inverseMapC(x(R,Rx),r(R,Rx));
	timeinv += getCPU() - time0;
	  //	  surface->map(r(i,Rx),xx);
	if ( R.length()<=100 ) {
	  for ( i=R.getBase(); i<=R.getBound(); ++i ) {
	    cout << "x=" << x(i,0) << ',' << x(i,1) << ',' << x(i,2) << "  ";
	    cout << "r=" << r(i,0) << ',' << r(i,1) << endl;
	  }
	};
	delete[] wid; delete[] bot;

	cout << "timeForFindClosestCurve = " << timeForFindClosestCurve <<
	  ", from " << callsOfFindClosestCurve << " calls, " << callsOfFindClosestCurve_all
	     << " with all curves" << endl;
	cout << "timeForInsideOrOutside = " << timeForInsideOrOutside << endl;
	cout << "timeForSeg0 = " << timeForSeg0 <<
	  "   timeForSeg1 = " << timeForSeg1 <<
	  "   timeForUntrimmedInverse = " <<  timeForUntrimmedInverse <<
	  endl;
	gi.outputString( sPrintF( buff, "Time = %e \n", timeinv ) );
	aString menu[] =
	{
	  "continue",
	  ""
	};
	gi.getMenuItem(menu,answer);
      }
    else if( answer=="edit trimming curves" )
    {
      gi.erase();
      for ( int i=0; i<numberOfTrimCurves; i++ )
	trimCurves[i]->update(mapInfo);
      mappingHasChanged();
      // *wdh* 010901 initialize();
    }
    else if( answer=="edit untrimmed surface" )
    {
      if( surface!=NULL )
      {
        gi.erase();
	surface->update(mapInfo);
	mappingHasChanged();
	// *wdh* 010901 initialize();
      }
    }
    else if( answer=="show parameters" )
    {
      cout<<"number of trimming curves = "<<numberOfTrimCurves<<endl;
      display();
    }
    else if( answer=="triangulate" )
    {
      triangulate(mapInfo);
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer(0,3)=="help" )
    {
      aString topic = answer(5,answer.length()-1);
      if ( !gi.displayHelp(topic) )
	{
	  aString msg = "Sorry, there is currently no help for "+topic;
	  gi.createMessageDialog(msg, informationDialog);
	}
    }
    else if ( Mapping::updateWithCommand(mapInfo, answer) )
    {
      Mapping::updateWithCommand(mapInfo,"update mapping dialog");

      if( answer(0,4)=="lines" )
	  {
	    mappingHasChanged();
	    setUnInitialized();
	    // *wdh* 010901 initialize();
	  }
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName"  ||
             answer=="periodicity" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      if( answer=="lines" )
      {
	mappingHasChanged();
	setUnInitialized();
	 // *wdh* 010901 initialize();
        // reinitialize();
      }
    }
    else if (answer=="view domain")
    {
      viewRange = false;
      interface.setSensitive(false, DialogData::toggleButtonWidget, 0);
      interface.setSensitive(false, DialogData::toggleButtonWidget, 1);
      interface.setSensitive(true, DialogData::toggleButtonWidget, 2);
      interface.setSensitive(true, DialogData::toggleButtonWidget, 3);
      interface.getRadioBox(trimmedRadioBox).setSensitive(false);
    }
    else if (answer=="view range") 
    {
      viewRange = true;
      interface.setSensitive(true, DialogData::toggleButtonWidget, 0);
      interface.setSensitive(true, DialogData::toggleButtonWidget, 1);
      interface.setSensitive(false, DialogData::toggleButtonWidget, 2);
      interface.setSensitive(false, DialogData::toggleButtonWidget, 3);
      interface.getRadioBox(trimmedRadioBox).setSensitive(true);
    }
    else if (answer=="view trimmed")
    {
      viewTrimmed = true;
    }
    else if (answer=="view untrimmed") 
    {
      viewTrimmed = false;
    }
    else if( (len=answer.matches("edit trim curve")) )
    {
      int currentCurve=-1;
      sScanF(answer(len,answer.length()-1),"%i",&currentCurve);
      if( currentCurve>=0 && currentCurve<numberOfTrimCurves )
      {
	editTrimCurve(* trimCurves[currentCurve], mapInfo );
      }
      else
      {
        gi.outputString("Invalid trim curve to edit");
	gi.stopReadingCommandFile();
      }
      
    }
    else if( answer.matches("query curves") ||
	     answer.matches("delete curves") ||
	     answer.matches("edit curves") ||
	     answer.matches("reverse curves") ||
	     answer.matches("turn off picking") ||
	     answer.matches("Mouse Picking") ) // for backward compatibility
    {

      OpSelectEnum s = noOp;
      if( answer.matches("query curves") )
	s=queryCurve;
      else if( answer.matches("delete curves") )
	s=deleteCurve;
      else if( answer.matches("edit curves") )
	s=editCurve;
      else if( answer.matches("reverse curves") )
	s=reverseCurve;
      else if( answer.matches("turn off picking") )
	s=noOp;
      else if( answer.matches("Mouse Picking") )
      {
	sScanF(answer(13,answer.length()-1), "%i", &s);
	if ( queryCurve<=s && s<= noOp )
	{
	  currentSelectOp = s;
	  interface.getRadioBox(mouseRadioBox).setCurrentChoice(s);
	}
	else
	{
	  aString buff;
	  gi.outputString(sPrintF(buff,"TrimmedMapping Error : Bad Mouse Picking option %d",s));
	  s=noOp;
	}
      }
      currentSelectOp = s;
      interface.getRadioBox(mouseRadioBox).setCurrentChoice(s);
    }
    else if( (len=answer.matches("query trim curve")) )
    {
      int curve;
      sScanF(answer(len,answer.length()-1), "%i", &curve);
      if( curve>=0 && curve<numberOfTrimCurves )
      {
	  sPrintF(answer, "Trimming Curve : %d\nName : %s\nType : %s\n", curve,
			
		  (char *)(const char *)(trimCurves[curve]->getName(Mapping::mappingName)),
		  (char *)(const char *)(trimCurves[curve]->getClassName()));
	  gi.outputString(answer);

      }
      else
      {
        gi.outputString("Error in applying `query trim curve': invalid trim curve number.");
      }
    }
    else if( (len=answer.matches("edit trim curve")) )
    {
      int curve;
      sScanF(answer(len,answer.length()-1), "%i", &curve);
      if( curve>=0 && curve<numberOfTrimCurves )
	editTrimCurve(* trimCurves[curve], mapInfo );
      else
      {
	gi.outputString("Error in applying `edit trim curve': invalid trim curve number.");

      }
    }
    else if( (len=answer.matches("reverse trim curve")) )
    {
      int curve;
      sScanF(answer(len,answer.length()-1), "%i", &curve);
      if( curve>=0 && curve<numberOfTrimCurves && trimCurves[curve]->getClassName()=="NurbsMapping" )
      {
	((NurbsMapping *)trimCurves[curve])->reparameterize(1.,0);
	if ( curveIsCounterClockwise(*((NurbsMapping*)trimCurves[curve])) )
	  trimOrientation(curve) = 1;
	else
	  trimOrientation(curve) = -1;
	if (numberOfTrimCurves == 1 && trimOrientation(curve) == -1)
	{
	  gi.outputString("Adding an outer curve");
	  NurbsMapping *newBdy = new NurbsMapping(2,3);
	  constructOuterBoundaryCurve(newBdy);
	  addTrimCurve(newBdy);
	}
      }
      else
      {
	gi.outputString("Error in applying `reverse trim curve'");
      }
      setUnInitialized();
      validateTrimming();
    }
    else if( (len=answer.matches("delete trim curve")) )
    {
      int curvesToDelete[1];
      sScanF(answer(len,answer.length()-1), "%i", &curvesToDelete[0]);
      if( curvesToDelete[0]>=0 && curvesToDelete[0]<numberOfTrimCurves )
      {
        deleteTrimCurve(1, curvesToDelete);
      }
      else
      {
	gi.outputString("Error in applying `delete trim curve': invalid trim curve number.");

      }
    }
    else if ( select.nSelect>0 && currentSelectOp==noOp )
    {
      plotObject = TRUE;
    }
    else if ( select.nSelect>0 )
    {
      aString buff;
      if ( currentSelectOp == deleteCurve )
      {
	curvesToDelete = new int[select.nSelect];
      }

      for ( int s=0; s<select.nSelect; s++ )
      {
	int currentCurve = -1;
	if ( viewRange==true )
	{
	  for ( int a=0; a<3; a++ ) tmpVertex(0,a) = select.x[a];
	  surface->inverseMap(tmpVertex,tmpR);
	  cMin = -1;
	  findClosestCurve(tmpR,cMin, rC, xC, dist, false);
	  currentCurve = cMin(0);
	}
	else
	  for ( int c=0; c<numberOfTrimCurves; c++ )
	    if ( trimCurves[c]->getGlobalID() == select.selection(s,0) )
	    {
	      currentCurve = c;
	    }
	    
	if ( currentCurve==-1 )
	{
	  aString msg;
	  sPrintF(msg, "Error : could not find a curve near selected point");
	  gi.outputString(msg);
	  gi.createMessageDialog(msg, errorDialog);
	}
	else if ( currentSelectOp==queryCurve )
	{
	  buff = "";
	  sPrintF(buff, "Trimming Curve : %d\nName : %s\nType : %s\n", currentCurve,
		  (char *)(const char *)(trimCurves[currentCurve]->getName(Mapping::mappingName)),
		  (char *)(const char *)(trimCurves[currentCurve]->getClassName()));
	  gi.outputString(buff);

          gi.outputToCommandFile(sPrintF(line,"query trim curve %i\n",currentCurve));
	}
	else if ( currentSelectOp == deleteCurve )
	{
	  curvesToDelete[s] = currentCurve;
	  cout<<"will try to delete curve "<<currentCurve<<" "<<curvesToDelete[s]<<endl;
	  gi.outputToCommandFile(sPrintF(line,"delete trim curve %i\n",currentCurve));
	}
	else if ( currentSelectOp == editCurve )
	{
	  //parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  printF(" Edit curve %i\n",currentCurve);
	  gi.outputToCommandFile(sPrintF(line,"edit trim curve %i\n",currentCurve));
	  editTrimCurve(* trimCurves[currentCurve], mapInfo );
	}
	else if ( currentSelectOp == reverseCurve )
	{
	  if (trimCurves[currentCurve]->getClassName()=="NurbsMapping" )
	  {
	    ((NurbsMapping *)trimCurves[currentCurve])->reparameterize(1.,0);
	    gi.outputToCommandFile(sPrintF(line,"reverse trim curve %i\n",currentCurve));
	    if ( curveIsCounterClockwise(*((NurbsMapping*)trimCurves[currentCurve])) )
	      trimOrientation(currentCurve) = 1;
	    else
	      trimOrientation(currentCurve) = -1;
		 
	    //trimOrientation(currentCurve) = trimOrientation(currentCurve);
	    printf("Reversing the curve in update\n");

	    if (numberOfTrimCurves == 1 && trimOrientation(currentCurve) == -1)
	    {
	      gi.outputString("Adding an outer curve");
	      NurbsMapping *newBdy = new NurbsMapping(2,3);
	      constructOuterBoundaryCurve(newBdy);
	      addTrimCurve(newBdy);
	    }
	  }
	  else
	    gi.createMessageDialog("cannot reverse a "+trimCurves[currentCurve]->getClassName(), errorDialog);
	  setUnInitialized();
	  validateTrimming();
	}
	
	if ( currentSelectOp==deleteCurve )
	{
	  bool curveNotAdded = deleteTrimCurve(select.nSelect, curvesToDelete);
	  printf("Added an outer curve... %s\n", curveNotAdded? "NOT":"");
	    
	  interface.setSensitive(true, DialogData::pushButtonWidget, 0);
	  delete [] curvesToDelete;
	  curvesToDelete = NULL;
	}


// done in editTrimCurve
//  	if ( currentSelectOp==editCurve )
//  	  { // how do I know that the curve has/has not changed?
//  	    setUnInitialized();
//  	    validateTrimming();
//  	  }
      }
    }
    
    else if (answer=="undo last delete")
      {
	undoLastDelete();
	interface.setSensitive(false, DialogData::pushButtonWidget, 0);
      }
    else if (answer.matches("new trimcurve"))
    {
      printf("Making a new trim curve from all sub cuvers in all current trim curves...\n");
      int tc, sc, nc=0;
      NurbsMapping * newTrimCurve_ = new NurbsMapping;
      NurbsMapping & newTrimCurve = *newTrimCurve_;
      Mapping *oneTrimCurve_;
      
      for (tc=0; tc<getNumberOfTrimCurves(); tc++)
      {
	oneTrimCurve_ = getTrimCurve(tc);
	if (oneTrimCurve_->getClassName() == "NurbsMapping")
	{
	  printf("Trimcurve #%d is a NurbsMapping\n", tc);
	  NurbsMapping & oneNurb = (NurbsMapping &) *oneTrimCurve_;
// copy all sub curves from oneNurb to the newTrimCurve
	  for (sc=0; sc<oneNurb.numberOfSubCurvesInList(); sc++)
	  {
// copy the first subcurve, just to make addTrimCurve happy
	    if (nc++ == 0) newTrimCurve = oneNurb.subCurveFromList(sc); 
	    newTrimCurve.addSubCurve( oneNurb.subCurveFromList(sc) );
	  }
	}
      }
// make the new trimcurve
      if (newTrimCurve.numberOfSubCurvesInList() > 0)
      {
	addTrimCurve( &newTrimCurve );
// edit the new trim curve
	int currentCurve = getNumberOfTrimCurves()-1;
	editTrimCurve(* trimCurves[currentCurve], mapInfo );
// Usually need to delete some of the previous curves before this makes sense
	invalidateTrimming();
      }
      else
      {
	printf("Empty list of subcurves! NOT making a new trim curve\n");
      }
      
      
      
    }
    else if ( answer(0,10)=="plot curves" )
      {
	parameters.set(GI_PLOT_UNS_BOUNDARY_EDGES, !parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES,dumval));
	interface.setToggleState(0, parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES,dumval));
// copy this value for the untrimmed plotter
	parameters.set(GI_PLOT_MAPPING_EDGES, parameters.get(GI_PLOT_UNS_BOUNDARY_EDGES,dumval));
      }
    else if ( answer(0,17)=="plot triangulation" )
      {
	parameters.set(GI_PLOT_UNS_EDGES, !parameters.get(GI_PLOT_UNS_EDGES,dumval));
	interface.setToggleState(1, parameters.get(GI_PLOT_UNS_EDGES,dumval));
// copy this value for the untrimmed plotter
	parameters.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, parameters.get(GI_PLOT_UNS_EDGES,dumval));
      }
    else if ( answer(0,8) == "plot grid" )
      {
	plotGrid = !plotGrid;
	interface.setToggleState(2, plotGrid);
      }
    else if ( answer(0,13) == "plot subcurves" )
      {
	plotSubcurves = !plotSubcurves;
	interface.setToggleState(3, plotSubcurves);
      } 
    else if ( answer(0,18) == "print trimming info" )
      {
	gi.outputString(reportTrimmingInfo());
      }
    else if ( answer(0,19) == "force trimming valid" )
      {
	manuallyValidateTrimming();
      }
    else if ( answer.matches("refine plot") )
    {
      // increase grid points for plotting the untrimmed surface
      Mapping & refSurface = *untrimmedSurface();
      for( int axis=0; axis<domainDimension; axis++ )
      {
        refSurface.setGridDimensions(axis, 2*refSurface.getGridDimensions(axis)); 
      }
      if( domainDimension==2 )
	printf(" New grid dimensions = [%i,%i]\n",refSurface.getGridDimensions(0),refSurface.getGridDimensions(1));
      
      if( trimmingIsValid() )
      {
	// add more grid points along all trimming curves
	for( int c=0; c<numberOfTrimCurves; c++ )
	{
	  Mapping & map = *trimCurves[c];
	  map.setGridDimensions(axis1, 2*map.getGridDimensions(axis1));
	}
	// *wdh* also adjust tolerances
	if( elementDensityTolerance<=0. )
	  elementDensityTolerance=.05;
	else
	  elementDensityTolerance/=2.;
	if( maxArea<=0. )
	  maxArea=.1;
	else
	  maxArea/=4.;

	printf("Recompute the triangulation with elementDensityTolerance=%8.2e, maxArea=%8.2e\n",
	       elementDensityTolerance,maxArea);
	

      }
      mappingHasChanged(); 
      plotObject = true;
      
    }
    else if ( answer(0,16) == "validate trimming" )
    {
      if( validateTrimming() )
      {
	gi.outputString("The trim curve appears to be valid.\n");
	
      }
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else if ( !select.active )
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if ( !trimmingIsValid() ) 
      validString = " !!! INVALID TRIMMING !!! ";
    else
      validString = "";

    if( plotObject )
    {
/* ------ *wdh* 010901  replace this block by the one below (is this correct?)
      if ( !isInitialized() ) 
	{
	  initialize();
	  if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
	    delete triangulation;
	  triangulation = NULL;
	}
----------- */
      if( !gridIsValid() )
      {
	if( triangulation!=NULL && triangulation->decrementReferenceCount()==0 )
	  delete triangulation;
	triangulation = NULL;
      }

      parameters.set(GI_TOP_LABEL,getName(mappingName) + validString);

      if ( viewRange )
      {
	gi.erase();
	if (viewTrimmed)
	{
          // printf(" replot the TrimmedMapping...\n");
	  PlotIt::plot(gi,*this, parameters);   // *** recompute every time ?? ***
	}
	else
	  PlotIt::plot(gi,*untrimmedSurface(), parameters);
      }
      else
	{
	  gi.erase();
	  if ( plotGrid && quadTreeMesh != NULL )
	    {
	      realArray qtpoints(quadTreeMesh->sizeOfQuadTreeMesh,3);
	      qtpoints = 0;
	      realArray qtcolour(quadTreeMesh->sizeOfQuadTreeMesh);
	      qtcolour = 0;
	      quadTreeMesh->accumulateCenterPoints(qtpoints,qtcolour,0);
	      gi.plotPoints(qtpoints,qtcolour,domainParam);
	    }
	  for ( int c=0; c<numberOfTrimCurves; c++ )
	  {
	    if ( trimCurves[c]->getClassName()=="NurbsMapping")
	    {
	      NurbsMapping & map = (NurbsMapping &) *trimCurves[c];
	      
	      if (plotSubcurves)
	      {
		aString mapcolour = domainParam.getMappingColour();

		for ( int sc=0; sc<map.numberOfSubCurves(); sc++ )
		{
		  domainParam.set(GI_MAPPING_COLOUR, gi.getColourName(sc));
// temporarily change the globalID to make it clickable
		  int id = map.subCurve(sc).getGlobalID();
		  map.subCurve(sc).setGlobalID(map.getGlobalID());
		  PlotIt::plot(gi,map.subCurve(sc), domainParam);
		  map.subCurve(sc).setGlobalID(id);  // reset
		}
		domainParam.set(GI_MAPPING_COLOUR, mapcolour);
	      }
	      else
		PlotIt::plot(gi,map, domainParam);
	    }
	    else
	      PlotIt::plot(gi, *trimCurves[c], domainParam);
	  }
	  
	}
      
    }
  } // end for it=...

  gi.popGUI();
  gi.erase();
// *AP* noting to reset!  gi.unAppendTheDefaultPrompt();  // reset
  mapInfo.interface = oldInterface;

  return 0;
  
}


aString TrimmedMapping::
reportTrimCurveInfo(Mapping *c, bool & curveok)
//=====================================================================================
/// \brief  return a string describing the state of a trim curve
/// \param c (input) : the curve in question
//=====================================================================================
{
  Mapping &curve = *c;
  curveok = true;
  aString buff = "";
  sPrintF(buff,"global id %d, ", c->getGlobalID());
  if ( curve.getIsPeriodic(axis1)==notPeriodic )
  {
    buff += " is NOT periodic! ";
    curveok = false;
  }
  else
    buff += " is periodic, ";
  
  if ( curve.getClassName()=="NurbsMapping")
  {
    if ( curveSelfIntersects(  (NurbsMapping &)curve ) )
    {
      buff += " Intersects Itself! " ;
      curveok = false;
    }
  }
  
  if ( curveTooNarrow(*c) )
  {
    buff += " appears too narrow! ";
    curveok = false;
  }

// check if all visible subcurves are part of the trim curve
  if (c->getClassName()=="NurbsMapping")
  {
    bool allSubCurvesUsed = true;
    NurbsMapping * tc_ = (NurbsMapping *) c;
    for ( int sc=0; sc<tc_->numberOfSubCurves(); sc++ )
    {
      // allSubCurvesUsed = allSubCurvesUsed && tc_->isSubCurveOriginal(sc);
      bool subCurveUsed=tc_->isSubCurveOriginal(sc);
      allSubCurvesUsed = allSubCurvesUsed && subCurveUsed;
      if( !subCurveUsed )
      {
	printF("TrimmedMapping::verifyTrimCurve:INFO: sub-curve %i is unused.\n",sc);
      }

    }
    if ( !allSubCurvesUsed )
    {
      buff += " there are unused sub curves";
      curveok = false;
    }
  }
  
  if ( curveOutsideDomain(*c) )
    {
      buff += " the curve lies outside the domain";
      curveok = false;
    }
  
  if ( curveok )
    buff+=" appears fine\n";
  else
    buff+=" -- INVALIDATES Trimming !\n";
  
  return buff;
}

aString TrimmedMapping::
reportTrimmingInfo() 
//=====================================================================================
/// \brief  return a string describing the state of the trimming
//=====================================================================================
{
  aString msg;
  msg = "TrimmedMapping :: "+getName(Mapping::mappingName)+"\n";

  bool allCurvesPeriodic = true, curveok, allCurvesOk=true;
  aString buff;
  for ( int c=0; c<numberOfTrimCurves; c++ )
  {
    buff = "";
    sPrintF(buff,"trim curve %d : ",c);
    msg+=buff;
    msg+=reportTrimCurveInfo(trimCurves[c], curveok);
    allCurvesOk = (allCurvesOk && curveok);
  }

  if ( !positiveActiveSurfaceArea(numberOfTrimCurves, trimCurves) )
  {
    msg += "No active surface! Trim curves may not be oriented correctly!\n\n";
    allCurvesOk = false;
  }

// after delete and add operations, trimmingIsValid can be out of sync
  if (allCurvesOk && !trimmingIsValid())
    validateTrimming();

  msg += "Trimming is ";
  if ( trimmingIsValid() ) 
    msg+="valid\n";
  else
// *wdh* 010812: there was a conflict here when I updated. I am not sure if the else clause above
// should be replaced by the single line below?
     msg+="INVALID!\n";

//     {
//       bool allCurvesPeriodic = true;
//       msg+="INVALID!\n";
//       aString buff;
//       for ( int c=0; c<numberOfTrimCurves; c++ )
// 	{
// 	  buff = "";
// 	  sPrintF(buff,"trim curve %d : ",c);
// 	  msg+=buff;
// 	  msg+=reportTrimCurveInfo(trimCurves[c]);
// 	}

//       if ( !positiveActiveSurfaceArea(numberOfTrimCurves, trimCurves) )
// 	msg += "No active surface! Trim curves may not be oriented correctly!\n\n";
//     }


  return msg;
}


int TrimmedMapping::
editTrimCurve( Mapping &trimCurve, MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively edit a trim curve
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{
  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  int status = 0;
  if (trimCurve.getClassName()=="NurbsMapping")
    status = editNurbsTrimCurve( (NurbsMapping &)trimCurve, mapInfo );
  else
    status = trimCurve.update( mapInfo );

  setUnInitialized(); // indicates that the quad tree needs to be rebuilt
// check if the edited trim curve is ok, if not, tries to reverse it and checks again.
  validateTrimming(); 

  return status;
}

int TrimmedMapping::
editNurbsTrimCurve( NurbsMapping &trimCurve, MappingInformation & mapInfo ) 
//=====================================================================================
/// \brief  Interactively edit  a nurbs trim curve
/// \param mapInfo (input): Holds a graphics interface to use.
//=====================================================================================
{
 int status = 0;
 assert(mapInfo.graphXInterface!=NULL);
 GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
 GraphicsParameters parameters;

 char buff[180];  // buffer for sprintf

// turn off the axes inside this function
 bool oldPlotTheAxes = gi.getPlotTheAxes();
 gi.setPlotTheAxes(false);

 GUIState interface;

 interface.setWindowTitle("Edit Trimming Curve");
 interface.setExitCommand("exit","Exit");

// interface.setRadioBoxColumns(1);

// which trim curve did we get?
 int trimCurveNumber = -1;
 int q;
 for (q=0; q<numberOfTrimCurves; q++)
 {
   if (trimCurves[q] == &trimCurve)
     trimCurveNumber = q;
 }
 
// moved to TrimmedMapping.h
//  enum MouseSelectMode { nothing=0,
// 			hideCurve,
// 			lineSegmentJoin,
// 			endpointMove,
// 			intersection,
// 			split,
//                      translate,
// 			updateCurve,
// 			curveAssembly,
// 			numberOfMouseModes
//                        };
			
 MouseSelectMode mouseMode = nothing;

 aString mouseModeCommands[] = { "Mouse Mode NoOp",
				 "Mouse Mode Hide SubCurve",
//				 "Mouse Mode Delete SubCurve", 
				 "Mouse Mode Join W/Line Segment",
				 "Mouse Mode Move Curve Endpoint",
				 "Mouse Mode Snap To Intersection",
				 "Mouse Mode Split",
				 "Mouse Mode Split At Intersection",
				 "Mouse Mode Translate",
				 "Mouse Mode Edit SubCurve",
				 "begin curve",
				 "" };

 aString mouseModeLabels[] = { "No Operation",
			       "Hide SubCurve",
//			       "Delete SubCurve",
			       "Join W/Line Segment",
			       "Move Curve Endpoint",
			       "Snap To Intersection",
			       "Split",
			       "Split At Intersection",
			       "Translate",
			       "Edit SubCurve",
			       "Assemble",
			       "" };

 interface.addRadioBox("Mouse Picking",mouseModeCommands, mouseModeLabels, int(mouseMode), 2); // 2 columns

 // general state variables
 bool plotObject = true;
 bool plotAxes = false; // *ap*
 bool plotCurve = false; // *ap*
 bool plotOriginalCurve = false;
 bool plotAllSubcurves = true;
 bool plotControlPoints = false;
 bool autoExit=true;

 aString tbCommands[] = { "plot curve",
			  "plot original curve",
			  "plot all subcurves",
			  "plot control points",
			  "auto exit",
			  "" };
 aString tbLabels[] = { "Current Curve",
			"Original Curve",
			"SubCurves",
			"Control Points",
			"Exit on Curve Completion",
			"" };
 int tbState[] = {plotCurve,
		  plotOriginalCurve,
		  plotAllSubcurves,
		  plotControlPoints,
		  autoExit};

 interface.setToggleButtons(tbCommands, tbLabels, tbState, 2); 

 enum pushButtons{
   cancelPb=0,
   revertPb,
   autoPb,
   showAllPb,
   showUsedPb,
   hideAllPb,
   hideUnusedPb,
   showOnePb,
   trimInfoPb,
   refinePb,
   numberOfPb
 };
 

 aString pbCommands[] = { "cancel action",
			  "undo curve",
			  "auto assemble",
			  "show all",
			  "show used",
			  "hide all",
			  "hide unused",
			  "show last hidden",
			  "print trimming info",
			  "refine plot",
			  "" };
 aString pbLabels[]   = { "Cancel Action",
			  "Revert Curve",
			  "Auto Assemble",
			  "Show All",
			  "Show Used",
			  "Hide All",
			  "Hide Unused",
			  "Show Last Hidden",
			  "Trim Curve Info",
			  "Refine Plot",
			  "" };

 interface.setPushButtons(pbCommands, pbLabels, 4); // 4 rows

  // ----- Text strings ------
  const int numberOfTextStrings=5;
  aString textCommands[numberOfTextStrings];
  aString textLabels[numberOfTextStrings];
  aString textStrings[numberOfTextStrings];

  int nt=0;

  textCommands[nt] = "debug";  textLabels[nt]=textCommands[nt];
  sPrintF(textStrings[nt], "%i",Mapping::debug);  nt++; 

  
  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textCommands[nt]="";   textLabels[nt]="";   textStrings[nt]="";  
  interface.setTextBoxes(textCommands, textLabels, textStrings);


 SelectionInfo select;
 aString answer="", line;
 aString buf;
 

 // state variables used for endpoint move
 bool movingEndpoint = false;
 int moveCurve = -1;
 int moveEnd = Start;

 // variables for new curve assembly
 int numberOfAssembledCurves = 0;
 NurbsMapping **assemblyCurves = NULL;
 NurbsMapping newCurve, oldCurve;
 oldCurve = trimCurve;
 bool curveRebuilt = false;

 // state variables for intersection calculation
 int curve1=-1, curve2=-1;
 int curve1End=0, curve2End=0;
 real c1click[2];

 // line segment state variables
 int nLineSegmentPoints = 0;
 RealArray linePts1(1,2),linePts2(1,2);
 linePts1 = linePts2 = 0.0;

 // point plotting state
 realArray plotpts;

// show hidden subcurve
 int hiddenSubCurve=-1;

 gi.pushGUI(interface);

// reset the viewing matrix
 gi.initView(gi.getCurrentWindow());

 interface.setSensitive(true,  DialogData::pushButtonWidget, trimInfoPb);
       
 RadioBox & rBox = interface.getRadioBox(0);

 for( int it=0;; it++ )
 {
// set the sensitivity of the GUI
   if (mouseMode == curveAssembly)
   {
     rBox.setSensitive(nothing, false);
     rBox.setSensitive(lineSegmentJoin, false);
     rBox.setSensitive(endpointMove, false);
     rBox.setSensitive(intersection, false);
     rBox.setSensitive(split,false);
     rBox.setSensitive(updateCurve, false);
     //interface.setSensitive(true,  DialogData::pushButtonWidget, cancelPb);  // cancel pb
     interface.setSensitive(false, DialogData::pushButtonWidget, revertPb);// revert pb
   }
   else
   {
     interface.setSensitive(true, DialogData::radioBoxWidget, 0); // radio box active
     interface.setSensitive(true, DialogData::pushButtonWidget, cancelPb);// cancel pb
     interface.setSensitive(true, DialogData::pushButtonWidget,  revertPb);// revert pb
     interface.setSensitive(hiddenSubCurve >= trimCurve.numberOfSubCurves() && 
			    hiddenSubCurve < trimCurve.numberOfSubCurvesInList(), 
			    DialogData::pushButtonWidget,  showOnePb);// show one subcurve
   }
     
     
   if( it==0 && plotObject )
     answer="plotObject";
   else
   {
     gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

     gi.getAnswer(answer, "", select);

     gi.savePickCommands(true); // restore
   }
   
   int len;
   if ( (len = answer.matches("Mouse Mode")) )
   {
     aString mode= (answer.length() > len+2)? answer(len+1,answer.length()-1): (aString)"";
	 
     if (mode.matches("NoOp"))
       mouseMode = nothing;
     else if (mode.matches("Hide SubCurve"))
       mouseMode = hideCurve;
     else if (mode.matches("Join W/Line Segment"))
       mouseMode = lineSegmentJoin;
     else if (mode.matches("Move Curve Endpoint"))
       mouseMode = endpointMove;
     else if (mode.matches("Snap To Intersection"))
       mouseMode = intersection;
     else if (mode.matches("Split At Intersection"))
       mouseMode = splitAtIntersection;
     else if (mode.matches("Split"))
       mouseMode = split;
     else if (mode.matches("Translate"))
       mouseMode = translate;
     else if (mode.matches("Edit SubCurve"))
       mouseMode = updateCurve;
     else
       gi.outputString(sPrintF(buf,"Unknown mouse mode: `%s'", SC mode));
	 
     if ( mouseMode>=0 && mouseMode < numberOfMouseModes )
     {
       if ( !rBox.setCurrentChoice(mouseMode) )
       {
	 aString buf;
	 sPrintF(buf,"ERROR : selection %d is inactive", mouseMode);
	 gi.outputString(buf);
       }
     }
     else
     {
       aString errbuff;
       sPrintF(errbuff, "ERROR : invalid mouse mode %d", int(mouseMode));
       gi.createMessageDialog(errbuff, errorDialog);
       gi.outputString(errbuff);
       mouseMode = nothing;
     }

   }
   else if( interface.getTextValue(answer,"debug","%i",Mapping::debug) ){}//
   else if ( answer(0,9)=="plot curve" )
   {
     plotCurve = !plotCurve;
     interface.setToggleState(0, plotCurve);
     //interface.setSensitive((plotOriginalCurve||plotCurve), DialogData::toggleButtonWidget, 3);
   }
   else if ( answer(0,8)=="auto exit" )
   {
     autoExit = !autoExit;
     interface.setToggleState(4,autoExit);
   }
   else if ( answer(0,18) == "plot original curve" )
   {
     plotOriginalCurve = !plotOriginalCurve;
     interface.setToggleState(1, plotOriginalCurve);
     //interface.setSensitive((plotOriginalCurve||plotCurve), DialogData::toggleButtonWidget, 3);
   }
   else if ( answer(0,17) == "plot all subcurves" )
   {
     plotAllSubcurves = !plotAllSubcurves;
     interface.setToggleState(2,plotAllSubcurves);
   }
   else if ( answer(0,18)=="plot control points")
   {
     plotControlPoints = !plotControlPoints;
     interface.setToggleState(3, plotControlPoints);
   }
//       else if ( answer(0,8) == "plot axes" )
//         {
//  	 plotAxes = !plotAxes;
//  	 interface.setToggleState(4, plotAxes);
//         }
   else if( (len=answer.matches("snap to intersection")) )
   {
     // int curve1,curve2,curve1End,curve2End;
     real c2click[2];
     sScanF(answer(len,answer.length()-1),"%i %i %i %i %e %e %e %e",&curve1,&curve2,&curve1End,&curve2End,
	    &c2click[0],&c2click[1],&c1click[0],&c1click[1]);

     printf(" snap: curve1=%i, curve2=%i, curve1End=%i, curve2End=%i\n",curve1,curve2,curve1End,curve2End);


     if( curve1<0 || curve2<0 )
     {
       gi.outputString("Invalid curves for snap to intersection");
       gi.stopReadingCommandFile();
     }
     else 
     {
       int status =snapCurvesToIntersection(gi,trimCurve,curve1,curve2,curve1End,curve2End,
					    c2click,c1click);
     }
       
   }
   else if( (len=answer.matches("hide curve")) )
   {
     int c=-1;
     sScanF(answer(len,answer.length()-1),"%i",&c);
     if( c>=0 && c<trimCurve.numberOfSubCurves() )
       hiddenSubCurve = trimCurve.toggleSubCurveVisibility(c);
   }
   else if( (len=answer.matches("join with line segment")) )
   {
     int c=-1;
     sScanF(answer(len,answer.length()-1),"%e %e %e %e",&linePts1(0,0),&linePts1(0,1),&linePts2(0,0),&linePts2(0,1));

     NurbsMapping newLine;// = new NurbsMapping(1,2);
     newLine.line(linePts1, linePts2);
     bool added = false;
     trimCurve.addSubCurve(newLine);
   }
   else if( (len=answer.matches("move end point")) )
   {
     RealArray xa(2);
     sScanF(answer(len,answer.length()-1),"%i %i %e %e",&moveCurve,&moveEnd,&xa(0),&xa(1));
     if( moveCurve>=0 && moveCurve<trimCurve.numberOfSubCurves() && moveEnd>=0 && moveEnd<=1 )
     {
       trimCurve.subCurveFromList(moveCurve).moveEndpoint(moveEnd, xa);
       movingEndpoint=false;
     }
     else
     {
       gi.outputString("Invalid arguments to `move end point'");
     }
   }
   else if( (len=answer.matches("split curve")) )
   {
     int curve;
     real rp;    // split curve at this r value
     sScanF(answer(len,answer.length()-1),"%i %e",&curve,&rp);
     if( curve>=0 && curve<trimCurve.numberOfSubCurves() && rp<1.0 && rp>0.0 )
     {
       NurbsMapping *c1 = new NurbsMapping; c1->incrementReferenceCount();
       NurbsMapping *c2 = new NurbsMapping; c2->incrementReferenceCount();
       if ( (trimCurve.subCurveFromList(curve).split(rp, *c1, *c2))==0 )
       {
	 trimCurve.toggleSubCurveVisibility(curve);
	 trimCurve.addSubCurve(*c1);
	 trimCurve.addSubCurve(*c2);
       }
       else
	 gi.createMessageDialog("unknown error : cannot split the curve!", errorDialog);
       
       if( c1->decrementReferenceCount()==0 ) delete c1;
       if( c2->decrementReferenceCount()==0 ) delete c2;
       
     }
     else
     {
       gi.outputString("Invalid arguments to `split curve'");
     }
   }
   else if( (len=answer.matches("assemble trim curve")) )
   {
     int curve;
     sScanF(answer(len,answer.length()-1),"%i",&curve);
     if( curve>=0 && curve<trimCurve.numberOfSubCurves() )
     {
       int status=assembleSubCurves(curve,
				    gi, 
				    trimCurve,
				    newCurve,
				    numberOfAssembledCurves,
				    assemblyCurves,
				    mouseMode,
				    curveRebuilt,
				    plotCurve );

       if( curveRebuilt )
	 interface.setToggleState(0, plotCurve);
       if ( curveRebuilt && autoExit )  // exit to calling function directly!
	 break;
     }
     else
     {
       gi.outputString("Invalid arguments to `assemble trim curve'");
     }
   }
   // ----------------------- mouse selection -------------------------------
   else if ( select.active && select.nSelect>0 )
   {
     real x[] = { 0.0, 0.0 };

     realArray X(1,2), r(2,1), xm(2,2);
     Range AXES(2);
     int end;
     int selectedCurve=-1;
     bool foundcurve=false;
     if ( mouseMode!=nothing )
     {
       // snap the selection to the nearest endpoint of the selected curve
       X(0,0) = select.x[0];
       X(0,1) = select.x[1];
       r(0,0) = 0.0;
       r(1,0) = 1.0;

       foundcurve = false;
       for ( int s=0; s<select.nSelect && !foundcurve; s++ )
       {
	 for ( int sc=0; sc<trimCurve.numberOfSubCurves() && !foundcurve; sc++ )
	 {
	   if ( trimCurve.subCurve(sc).getGlobalID()==select.selection(s,0) )
	   {
	     trimCurve.subCurveFromList(sc).reparameterize(0.,1.); // why? kkc
	     trimCurve.subCurveFromList(sc).map(r,xm);
	     foundcurve = true;
	     selectedCurve = sc;
	     real dist1 = sum(pow(xm(0,AXES)-X(0,AXES),2));
	     real dist2 = sum(pow(xm(1,AXES)-X(0,AXES),2));
	     if ( dist1<dist2 )
	     {
	       x[0] = xm(0,0);
	       x[1] = xm(0,1);
	       end = Start;
	     }
	     else
	     {
	       x[0] = xm(1,0);
	       x[1] = xm(1,1);
	       end = End;
	     }
             printF("Subcurve %i, end=%i selected\n",sc,end);
	     
	   }
	 }
       }
       X(0,0) = x[0];
       X(0,1) = x[1];
     }

     if ( mouseMode==hideCurve && foundcurve )
     {
       if ( (select.r[1]-select.r[0])>FLT_MIN &&
	    (select.r[3]-select.r[2])>FLT_MIN )
       {
	 for ( int s=0; s<select.nSelect; s++ )
	   for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
	     if ( trimCurve.subCurveFromList(sc).getGlobalID() == select.selection(s,0) )
	     {
	       hiddenSubCurve = trimCurve.toggleSubCurveVisibility(sc);
	       gi.outputToCommandFile(sPrintF(line,"hide curve %i\n",sc));
	       break;
	     }
       }
       else
       {
	 if ( !trimCurve.isSubCurveHidden(selectedCurve) )
	 {
	   hiddenSubCurve =trimCurve.toggleSubCurveVisibility(selectedCurve);
	   gi.outputToCommandFile(sPrintF(line,"hide curve %i\n",selectedCurve));
	 }
       }
     }
//  	 else if ( mouseMode==deleteSubCurve && foundcurve )
//  	   {
//  	     if ( (select.r[1]-select.r[0])>FLT_MIN &&
//  		  (select.r[3]-select.r[2])>FLT_MIN )
//  	       {
//  		 for ( int s=0; s<select.nSelect; s++ )
//  		   for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
//  		     if ( trimCurve.subCurveFromList(sc).getGlobalID() == select.selection(s,0) )
//  		       {
//  			 trimCurve.deleteSubCurve(sc);
//  			 break;
//  		       }
//  	       }
//  	     else
//  	       if ( trimCurve.subCurveFromList(selectedCurve).getGlobalID() == select.selection(0,0) )
//  		 {
//  		   trimCurve.deleteSubCurve(selectedCurve);
//  		 }
//  	   }
     else if ( mouseMode==lineSegmentJoin && foundcurve)
     {
       // now begin ( or end ) the line segment
       if ( foundcurve && nLineSegmentPoints == 0 )
       {
         // First curve was chosen for joining with a line segment
	 linePts1(0,0) = x[0];
	 linePts1(0,1) = x[1];
         // *wdh* 051110 plotpts=linePts1;
         plotpts.redim(1,2);
	 plotpts(0,0) = linePts1(0,0);  // *wdh* 051110
	 plotpts(0,1) = linePts1(0,1);
	 nLineSegmentPoints++;
         curve1 = selectedCurve;  // *wdh* 090531 -- draw this curve with a thicker line
       }
       else if ( foundcurve && nLineSegmentPoints == 1 )
       {
         // Second curve was chosen for joining with a line segment
	 linePts2(0,0) = x[0];
	 linePts2(0,1) = x[1];
	 if ( max(fabs(linePts2-linePts1))<FLT_EPSILON )
	 {
	   gi.createMessageDialog("points for line segment are too close together!", errorDialog);
	 }
	 else
	 {
	   NurbsMapping newLine;// = new NurbsMapping(1,2);
	   newLine.line(linePts1, linePts2);
	   bool added = false;
	   trimCurve.addSubCurve(newLine);

           gi.outputToCommandFile(sPrintF(line,"join with line segment %e %e %e %e\n",linePts1(0,0),linePts1(0,1),
                       linePts2(0,0),linePts2(0,1)));
	 }
	 nLineSegmentPoints = 0;
	 linePts1 = linePts2 = 0.0;
	 plotpts.redim(0);
         curve1 = -1;
       }
     }
     else if ( mouseMode == curveAssembly )
     {
      int curve=-1;
      for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
	if ( trimCurve.subCurveFromList(sc).getGlobalID()==select.selection(0,0) )
	{
	  curve=sc;
	  break;
	}

      if( curve>=0 )
      {
	int status=assembleSubCurves(curve,
			  gi, 
			  trimCurve,
			  newCurve,
			  numberOfAssembledCurves,
			  assemblyCurves,
			  mouseMode,
			  curveRebuilt,
			  plotCurve );

        if( status==0 )
          gi.outputToCommandFile(sPrintF(line,"assemble trim curve %i\n",curve));

	printF(" curveAssembly after assembleSubCurves: add curve=%i, status=%i\n",status);

        if( curveRebuilt )
          interface.setToggleState(0, plotCurve);
	if ( curveRebuilt && autoExit )  // exit to calling function directly!
	  break;
      }
      
     }
     else if ( mouseMode==endpointMove )
     {
       if ( !movingEndpoint && foundcurve)
       {
	 plotpts.redim(1,2);
	 plotpts(0,0) = x[0];
	 plotpts(0,1) = x[1];
	 moveCurve = selectedCurve;
	 moveEnd = end;
	 movingEndpoint = true;
       }
       else
       {
	 X(0,0) = x[0];
	 X(0,1) = x[1];
         RealArray xs(2);
         xs(0)=x[0];
	 xs(1)=x[1];
	 trimCurve.subCurveFromList(moveCurve).moveEndpoint(moveEnd,xs);
	 movingEndpoint=false;

         gi.outputToCommandFile(sPrintF(line,"move end point %i %i %e %e \n",moveCurve,moveEnd,x[0],x[1]));

	 plotpts.redim(0);
       }
     }
     else if ( mouseMode==intersection && foundcurve )
     {
       if ( curve1==-1 )
       {
	 c1click[0] = select.x[0];
	 c1click[1] = select.x[1];
	 curve1 = selectedCurve;
	 curve1End = end;
       }
       else if ( curve1!=selectedCurve )
       {
	 curve2 = selectedCurve;
	 curve2End = end;

// save values for the outputToCommandFile, since these values might get changed inside snapCurvesToIntersection
	 int c1=curve1, c2=curve2;
	 int status =snapCurvesToIntersection(gi,trimCurve,curve1,curve2,curve1End,curve2End,
					      select.x,c1click);
	 if( status==0 )
	   gi.outputToCommandFile(sPrintF(line,"snap to intersection %i %i %i %i %e %e %e %e \n",
					  c1, c2, curve1End, curve2End, select.x[0], select.x[1],
					  c1click[0], c1click[1]));
	 plotpts.redim(0);
       }
       else if ( curve1==selectedCurve && curve1!=-1 )
       {
	 gi.createMessageDialog("cannot intersect a curve with itself!", errorDialog);
       }
     }
     else if ( mouseMode==splitAtIntersection && foundcurve )
     {
       // Split a curve where it intersects another curve   *wdh* 100330

       if( curve1==-1 )
       {
	 curve1 = selectedCurve;  // -- draw this curve with a thicker line
         curve1End=end;
	 printF("Split at intersection: curve1=%i will be split. "
                "Choose another curve near the point of intersection.\n",curve1);
	 
       }
       else if ( curve1!=selectedCurve )
       {
	 curve2 = selectedCurve;
         curve2End=end;
	 

         // Find the intersection of curve1 and curve2 near point select.x[0..1]
         
	 IntersectionMapping intersect;
	 int numberOfIntersectionPoints=0;
	 realArray localIntersection;
	 realArray rmap1, rmap2;

	 // Compute: curve1(rmap1(i))=curve2(rmap2(i))=localIntersection(0:1,i) i=0,..,numberOfIntersectionPoints-1
	 bool parallel = intersect.intersectCurves(trimCurve.subCurveFromList(curve1), 
                                                   trimCurve.subCurveFromList(curve2), 
						   numberOfIntersectionPoints, rmap1, rmap2, 
						   localIntersection)==-1;
         // Look for the closest ppoint of intersection (or the closest point on curve1 to the
         // end point of curve2
         real distMin=REAL_MAX;  
	 RealArray xi(1,2);
	 real rSplit=-1.;
         int iSplit=-1;
	 x[0]=select.x[0];
	 x[1]=select.x[1];
	 if( numberOfIntersectionPoints>0 )
	 {

	   printF("Split at intersection: There were %i intersections between curve1=%i and curve2=%i\n",
		  numberOfIntersectionPoints,curve1,curve2);
	   for( int i=0; i<numberOfIntersectionPoints; i++ )
	   {
	     printF("Intersection %i : curve1=%e, curve2=%e, pt=(%e,%e)\n",i,rmap1(i),rmap2(i),
		    localIntersection(0,i),localIntersection(1,i));
	   }
	   printF("Picked point was x=(%e,%e)\n",select.x[0],select.x[1]);
	 
	   // Find the point of intersection closest to the last picked point
	   for( int i=0; i<numberOfIntersectionPoints; i++ )
	   {
	     real dist = SQR( x[0]-localIntersection(0,i) ) + SQR( x[1]-localIntersection(1,i) );
	     printF("Distance of picked pt to pt of intersection =%8.2e\n",dist);
	     if( dist<distMin )
	     {
	       distMin=dist;
	       iSplit=i;
	     }
	   }
	   assert( iSplit>=0 && iSplit<numberOfIntersectionPoints );
	   rSplit=rmap1(iSplit);
           xi(0,0)=localIntersection(0,iSplit);
           xi(0,1)=localIntersection(1,iSplit);
	   

	 }

	 if( numberOfIntersectionPoints!=2 )
	 {
           printF("Split at intersection:INFO: curve1=%i and curve2=%i did not intersect at 2 points.\n"
                  "We also look for for closest point to the end of curve2...\n",curve1,curve2);
           // find the closest point on curve1 from the end point of curve2 : 

           RealArray r(1,1),x2(1,2),x1(1,2);
           r=curve2End;
           trimCurve.subCurveFromList(curve2).mapS(r,x2);         // eval end point of curve2 

           // find a reasonable initial guess : 
	   if( false ) // not really needed once curve2End was set properly -- 
	   {
	     int iv[3];
	     real xv[3];
	     xv[0]=x2(0,0); xv[1]=x2(0,1);  
	     real minimumDistance=REAL_MAX;
	     NurbsMapping & nurbs1 = trimCurve.subCurveFromList(curve1);
	     assert( nurbs1.approximateGlobalInverse!=NULL );
	     nurbs1.approximateGlobalInverse->binarySearchOverBoundary( xv, minimumDistance, iv );
	     // r= ?
	     int n=nurbs1.getGridDimensions(axis1);
	     r(0,0)=iv[0]/(n-1.);  // initial guess
	     printF("Initial guess for r=%8.2e for x2=(%8.2e,%8.2e) curve2End=%i\n",r(0,0),x2(0,0),x2(0,1),curve2End);
	   }
	   else
	   {
	     r=-.1;
	   }
	   
	   trimCurve.subCurveFromList(curve1).inverseMapS(x2,r);  // find r value of closest pt on curve1 
           trimCurve.subCurveFromList(curve1).mapS(r,x1);         // closet point on curve1 (possible split point)

	   real dist = SQR( x[0]-x1(0,0) ) + SQR( x[1]-x1(0,1) );

	   printF("The end of curve2 is near pt r=%8.2e, x=(%8.2e,%8.2e) on curve1\n",r(0,0),x1(0,0),x1(0,1));
	   printF("Distance of picked pt to closest pt on curve1 near the end of curve2 =%8.2e\n",dist);

	   if( dist<distMin )
	   {
	     distMin=dist;
	     rSplit=r(0,0);
	     iSplit=-1;
	     // printF(" Closest point on curve1 was r=%9.3e\n",rSplit);
	     printF("Split at closest point: spitting curve at r=%e, pt=(%e,%e). \n",
		    rSplit, x2(0,0),x2(0,1));
             xi=x1;  // save for printing below
	   }
	   
	 }
	 if( iSplit>=0 )
	   printF("Split at intersection: splitting curve at r=%e, pt=(%e,%e). \n", rSplit, xi(0,0),xi(0,1));

// 	 if( parallel || numberOfIntersectionPoints==0 )
// 	 {
//            printF("Split at intersection:ERROR: curves did not intersect! curve1=%i curve2=%i parallel=%i. Choose another curve.\n",
// 		  curve1,curve2,parallel);
// 	   continue;
// 	 }

	 NurbsMapping *c1 = new NurbsMapping; c1->incrementReferenceCount();
	 NurbsMapping *c2 = new NurbsMapping; c2->incrementReferenceCount();

	 if ( (trimCurve.subCurveFromList(curve1).split( rSplit, *c1, *c2))==0 )
	 {
	   trimCurve.toggleSubCurveVisibility(curve1);
	   trimCurve.addSubCurve(*c1);
	   trimCurve.addSubCurve(*c2);

	   gi.outputToCommandFile(sPrintF(line,"split curve %i %e \n",curve1,rSplit));
	   
	 }
	 else
	   gi.createMessageDialog("Split at intersection: unknown error : cannot split the curve!", errorDialog);

         c1->decrementReferenceCount();
         c2->decrementReferenceCount();

	 curve1 = -1;  // un-highlight curve1 
       }
       else if ( curve1==selectedCurve && curve1!=-1 )
       {
	 gi.createMessageDialog("Split at intersection: cannot intersect a curve with itself!", errorDialog);
       }
     }
     else if ( mouseMode==split && foundcurve )
     {
       realArray x(1,2), r(1,1);
       x(0,0) = select.x[0];
       x(0,1) = select.x[1];

       r=-1.;  // initial guess (none)
       trimCurve.subCurveFromList(selectedCurve).inverseMap(x,r);

       NurbsMapping *c1 = new NurbsMapping; c1->incrementReferenceCount();
       NurbsMapping *c2 = new NurbsMapping; c2->incrementReferenceCount();
       if ( r(0,0)<1.0 && r(0,0)>0.0 )
       {
	 if ( (trimCurve.subCurveFromList(selectedCurve).split(r(0,0), *c1, *c2))==0 )
	 {
	   trimCurve.toggleSubCurveVisibility(selectedCurve);
	   trimCurve.addSubCurve(*c1);
	   trimCurve.addSubCurve(*c2);

	   gi.outputToCommandFile(sPrintF(line,"split curve %i %e \n",selectedCurve,r(0,0)));
	   
	 }
	 else
	   gi.createMessageDialog("unknown error : cannot split the curve!", errorDialog);

       }
       else
	 gi.createMessageDialog("cannot split a curve past its endpoints!", errorDialog);

       if( c1->decrementReferenceCount()==0 ) delete c1;
       if( c2->decrementReferenceCount()==0 ) delete c2;

     }
     else if ( mouseMode == translate && foundcurve )
     {
      real xShift=0., yShift=0., zShift=0.;
      gi.inputString(line,sPrintF(buff,"Enter rShift, sShift (default=(%e,%e)): ",
				  xShift,yShift));
      if( line!="" ) sScanF(line,"%e %e",&xShift,&yShift);

//      NurbsMapping & subCurve = trimCurve.subCurveFromList(selectedCurve);
      trimCurve.subCurveFromList(selectedCurve).shift(xShift,yShift,zShift);
     }
     else if ( mouseMode == updateCurve && foundcurve )
     {
       gi.erase();
       gi.outputToCommandFile(sPrintF(line,"update trim curve %i\n",selectedCurve));
       trimCurve.subCurveFromList(selectedCurve).update(mapInfo);
     }
   }
   else if( (len=answer.matches("update trim curve")) )
   {
     int selectedCurve=-1;
     sScanF(answer(len,answer.length()-1),"%i",&selectedCurve);
     if( selectedCurve>=0 && selectedCurve<trimCurve.numberOfSubCurvesInList() )
     {
       trimCurve.subCurveFromList(selectedCurve).update(mapInfo);
     }
     else
     {
       printF("Invalid trim curve %i to update, there are %i trim curves\n",
              selectedCurve,trimCurve.numberOfSubCurvesInList());
       gi.outputString("Invalid trim curve to update");
       gi.stopReadingCommandFile();
     }
   }
   else if ( answer=="auto assemble" )
   {
     printf(" ************* auto assemble **************\n");

     numberOfAssembledCurves = 0;
     mouseMode = curveAssembly;
     if (!assemblyCurves)
     {
       assemblyCurves = new NurbsMapping*[trimCurve.numberOfSubCurvesInList()];
       for ( int c=0; c<trimCurve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;
     }

     if ( trimCurve.numberOfSubCurvesInList()>0 )
     {
       int sc=0;
       numberOfAssembledCurves=0;
	     
       NurbsMapping **tempList = new NurbsMapping *[trimCurve.numberOfSubCurvesInList()];

       bool completedCurve =false;

       for ( sc=0; sc<trimCurve.numberOfSubCurvesInList() && !completedCurve; sc++ ) 
       {
	 for ( int c=0; c<trimCurve.numberOfSubCurvesInList(); c++ ) tempList[c] = &trimCurve.subCurveFromList(c);
		 
	 if ( !trimCurve.isSubCurveHidden(sc) )
	 {
	   numberOfAssembledCurves = 0;
	   for ( int c=0; c<trimCurve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;

	   //NurbsMapping &tempNewCurve = newCurve;
	   //tempNewCurve= *currentSubCurves[sc];
	   newCurve= trimCurve.subCurveFromList(sc);
	   tempList[sc] = NULL;
	   assemblyCurves[numberOfAssembledCurves++] = &trimCurve.subCurveFromList(sc);
	   bool merged = true;
	   completedCurve = connectCurveEnds(newCurve)==0; //(newCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
	   int ntries = 0;
	   while ( !completedCurve && ntries<(trimCurve.numberOfSubCurvesInList()+1) )
	   {
	     for ( int scj=0; scj<trimCurve.numberOfSubCurvesInList() && !completedCurve; scj++ )
	     {
			     
	       if ( tempList[scj]!=NULL && scj!=sc && !trimCurve.isSubCurveHidden(scj) )
	       {
		 //merged = (tempNewCurve.merge(*tempList[scj], false)==0);
		 merged = (newCurve.merge(*tempList[scj], false)==0);
		 if ( merged ) 
		 {
                   if( debug & 1 ) printF("auto assemble: joined sub-curve %i to master curve.\n",scj);

		   assemblyCurves[numberOfAssembledCurves++] = tempList[scj];
		   tempList[scj] = NULL;
				     
		   //completedCurve = (tempNewCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
		   completedCurve = connectCurveEnds(newCurve)==0; //(newCurve.getIsPeriodic(axis1)!=Mapping::notPeriodic);
		 }
	       }
	     }
	     ntries++;
	   }
	   //if ( completedCurve ) 
	   //{
	   //newCurve = tempNewCurve;
	   //real area, arcLength, scale0, scale1;
	   //getAreaAndArcLength( tempNewCurve, area, arcLength, scale0, scale1 );
	   //if ( completedCurve )
	   //newCurve = tempNewCurve;
	   // }
		     
	 }
       }
       delete [] tempList;
	     
       if ( completedCurve ) 
       {

	 // *wdh* 010901 real area, arcLen, scale0, scale1;
	 // *wdh* 010901 getAreaAndArcLength( newCurve, area, arcLen, scale0, scale1 );
         real area=getArea(newCurve);

	 if ( fabs(area)<FLT_MIN )
	 {
	   completedCurve = false;
	   gi.createMessageDialog("ASSEMBLY FAILED : overlapping curves were found, "
				  "going manual...", errorDialog);
	 }
	 else if ( !verifyTrimCurve((Mapping *)&newCurve) ||
	           numberOfAssembledCurves<trimCurve.numberOfSubCurves() ) 
	 {
	   completedCurve = false;
	   aString buff;
	   bool curveok;
// note that we have not included all subcurves in newCurve at this point
	   if (numberOfAssembledCurves<trimCurve.numberOfSubCurves())
	   {
             printF("INFO: The auto-assembly may fail if there are some very tiny sub-curves."
                    " try assembling manually.\n");
	     buff = "ASSEMBLY FAILED : problems with new curve : there are unused subcurves";
	   }
	   else
	     buff = "ASSEMBLY FAILED : problems with new curve : "+reportTrimCurveInfo( (Mapping*)&newCurve, curveok);
	   buff += "\ngoing manual...";
	   gi.createMessageDialog(buff, errorDialog);
	 }
	 else 
	 {
// add in all the remaining subcurves to newCurve
	   if ( numberOfAssembledCurves<trimCurve.numberOfSubCurvesInList() )
	   {
	     bool used = false;
	     for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
	     {
	       used = false;
	       for ( int as=0; as<numberOfAssembledCurves && !used; as++ )
		 if ( trimCurve.subCurveFromList(sc).getGlobalID()==assemblyCurves[as]->getGlobalID() )
		   used = true;
			     
	       if ( !used ) 
	       {
		 int nc = newCurve.addSubCurve(trimCurve.subCurveFromList(sc));
		 if ( trimCurve.isSubCurveHidden(sc) ) newCurve.toggleSubCurveVisibility(nc);
	       }
	     }
	   }
	   delete [] assemblyCurves;
	   assemblyCurves  = NULL;
	   numberOfAssembledCurves = 0;
	   mouseMode = nothing;
	   trimCurve = newCurve;   // deep copy

	   // ::display(trimCurve.getGrid(),"grid for trimCurve","%3.1f");
           // PlotIt::plot(gi,trimCurve);  // *************

	   curveRebuilt = true;
	   plotCurve = true;
	   interface.setToggleState(0, plotCurve);
	   gi.outputString("Trim curve is complete and appears to be valid!");
	   // AP: exit to calling function directly!
	   if ( autoExit ) break;
	 }
       }
       else
       {
	 gi.createMessageDialog("could not automatically assemble curves, going manual...", warningDialog);
	 gi.outputString( "could not automatically assemble curves, going manual...");
       }
     }
     else
     {
       gi.createMessageDialog("there are no subcurves!", errorDialog);
       gi.outputString("there are no subcurves!");
     }
   }
//                            01234567890123456789
   else if ( answer(0,10)=="begin curve" )
   {
     numberOfAssembledCurves = 0;
     mouseMode = curveAssembly;
     if ( !rBox.setCurrentChoice(mouseMode) )
     {
       aString buf;
       sPrintF(buf,"ERROR : selection %d is inactive", mouseMode);
       gi.outputString(buf);
     }
   }
   else if ( answer=="cancel action" )
   {
     plotpts.redim(0);
     if ( mouseMode==lineSegmentJoin )
     {
       nLineSegmentPoints = 0;
     }
     else if ( mouseMode == endpointMove )
     {
       movingEndpoint = false;
     }
     else if ( mouseMode == intersection )
     {
       curve1=curve2=-1;
     }
     else if ( mouseMode==curveAssembly )
     {
       delete [] assemblyCurves;
       assemblyCurves  = NULL;
       numberOfAssembledCurves = 0;
     }
     interface.setSensitive(true, DialogData::radioBoxWidget, 0); 
     interface.setSensitive(true, DialogData::pushButtonWidget, cancelPb);// cancel pb
     interface.setSensitive(true, DialogData::pushButtonWidget,  revertPb);// revert pb
     mouseMode = nothing;
     if ( !rBox.setCurrentChoice(mouseMode)) 
     {
       gi.outputString("the mouse mode is not active");
     }
   }
   else if ( answer=="undo curve" )
   {
     if ( numberOfAssembledCurves!=0 )
     {
       delete [] assemblyCurves;
       assemblyCurves  = NULL;
       numberOfAssembledCurves = 0;
     }
     trimCurve = oldCurve;
// reset the other modification states
     movingEndpoint = false;
     curve1 = curve2 = -1;
     nLineSegmentPoints = 0;

   }//                      01234567890123456789
   else if ( answer(0,18) == "print trimming info" )
   {
     bool curveok;
     gi.outputString(reportTrimCurveInfo((Mapping *)&trimCurve, curveok));
   }
   else if ( answer.matches("refine plot") )
   {
     NurbsMapping *subCurve_;
// current curve
     for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
     {
       subCurve_ = &trimCurve.subCurveFromList(sc);
       subCurve_->setGridDimensions(axis1, 2*subCurve_->getGridDimensions(axis1)); // double # grid points
     }
// old curve
     for ( int sc=0; sc<oldCurve.numberOfSubCurvesInList(); sc++ )
     {
       subCurve_ = &oldCurve.subCurveFromList(sc);
       subCurve_->setGridDimensions(axis1, 2*subCurve_->getGridDimensions(axis1)); // double # grid points
     }
   }
   else if ( answer.matches("show all") )
   {
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0;c<trimCurve.numberOfSubCurvesInList(); c++ ) 
       {
	 if ( trimCurve.isSubCurveHidden(c) ) 
	 {
	   trimCurve.toggleSubCurveVisibility(c);
	   curveWasHidden = true;
	   break;
	 }
       }
     } while (curveWasHidden);
   }
   else if ( answer.matches("hide all") )
   {
// need to go backwards because elements are moved forwards in list when they are shown
     for ( int c=trimCurve.numberOfSubCurvesInList()-1;c>=0; c--) 
     {
       if ( !trimCurve.isSubCurveHidden(c) ) trimCurve.toggleSubCurveVisibility(c);
     }
   }
   else if ( answer.matches("hide unused") )
   {
// we need to iterate since the list is reorganized every time an element is hidden
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0; c<trimCurve.numberOfSubCurves(); c++)  // only loop over the visible sub curves
       {
	 if ( !trimCurve.isSubCurveOriginal(c) && !trimCurve.isSubCurveHidden(c) ) 
	 {
	   printf("Hiding subcurve %i with globalID=%i\n", c, trimCurve.subCurve(c).getGlobalID());
	   trimCurve.toggleSubCurveVisibility(c); // hide the subcurve
	   curveWasHidden = true;
	   break; // no point continuing since the list is now re-ordered
	 }
       }
     } while(curveWasHidden);
     
   }
   else if ( (len=answer.matches("show last hidden")) )
   {
     if (hiddenSubCurve >= trimCurve.numberOfSubCurves() && 
	 hiddenSubCurve < trimCurve.numberOfSubCurvesInList())
     {
       trimCurve.toggleSubCurveVisibility(hiddenSubCurve);
       hiddenSubCurve = -1;
     }
   }
   else if ( answer.matches("show used") )
   {
// we need to iterate since the list is reorganized every time an element is made visible
     bool curveWasHidden;
     do
     {
       curveWasHidden = false;
       for ( int c=0; c<trimCurve.numberOfSubCurvesInList(); c++)  // loop over all sub curves
       {
	 if ( trimCurve.isSubCurveOriginal(c) && trimCurve.isSubCurveHidden(c) )
	 {
	   trimCurve.toggleSubCurveVisibility(c); // make it visible
	   curveWasHidden = true;
	   break; // no point continuing since the list is now re-ordered
	 }
       }
     } while(curveWasHidden);
   }
   else if ( answer=="exit" )
   {
     break;
   }
   else if ( answer == "plotObject" )
   {
     plotObject=true;
   }
   else if ( !select.active )
   {
     gi.outputString("could not understand command : "+answer);
     gi.stopReadingCommandFile();
   }

   if ( plotObject )
   {
     gi.erase();

     aString mapc = parameters.getMappingColour();

     parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
     parameters.set(GI_USE_PLOT_BOUNDS_OR_LARGER, true);
     parameters.set(GI_PLOT_END_POINTS_ON_CURVES, true);
     parameters.set(GI_PLOT_GRID_POINTS_ON_CURVES, false);
     parameters.set(GI_TOP_LABEL,sPrintF(buf,"Trim curve %i on surface `%s'", 
					 trimCurveNumber,
					 SC this->getName(Mapping::mappingName)));

     if ( plotCurve )
       trimCurve.plot( gi, parameters, plotControlPoints );
	 
     parameters.set(GI_MAPPING_COLOUR, "blue");
     if ( plotOriginalCurve ) 
       oldCurve.plot( gi, parameters, plotControlPoints);
     parameters.set(GI_MAPPING_COLOUR, mapc);

     if ( plotAllSubcurves )
     {
	     
       real linew;
       for ( int sc=0; sc<trimCurve.numberOfSubCurves(); sc++ )
	     
       {
// tmp
//  	 printf("Plotting subcurve %i with globalID=%i, original=%i\n", sc, 
//  		trimCurve.subCurve(sc).getGlobalID(), trimCurve.isSubCurveOriginal(sc));

	 parameters.set(GI_MAPPING_COLOUR, gi.getColourName(sc));
		 
	 if( sc==curve1 )
	 { // draw this curve thicker
	   parameters.get(GraphicsParameters::curveLineWidth, linew);
	   parameters.set(GraphicsParameters::curveLineWidth, linew*3);
	 }

//		 PlotIt::plot(gi,*currentSubCurves[sc], parameters);
	 if ( !trimCurve.isSubCurveHidden(sc) )
	   trimCurve.subCurve(sc).plot( gi, parameters, plotControlPoints );

	 if ( sc==curve1 )
	   parameters.set(GraphicsParameters::curveLineWidth, linew);
		 
       }
       parameters.set(GI_MAPPING_COLOUR, mapc);
     }
	 
     if ( mouseMode == curveAssembly )
     {
       parameters.set(GI_MAPPING_COLOUR, "red");
       real linew;
       parameters.get(GraphicsParameters::curveLineWidth, linew);
       parameters.set(GraphicsParameters::curveLineWidth, 2*linew);
       if ( numberOfAssembledCurves>0 )
	 PlotIt::plot(gi,newCurve, parameters);

       parameters.set(GraphicsParameters::curveLineWidth, linew);
       parameters.set(GI_MAPPING_COLOUR, mapc);
     }

     if ( plotpts.getLength(0)>0 )
       gi.plotPoints(plotpts, parameters);
   }
 }

// erase the trim curves
 gi.erase();
 gi.popGUI();

// reset plotTheAxes
 gi.setPlotTheAxes(oldPlotTheAxes);

 return status;
}


int TrimmedMapping::
snapCurvesToIntersection(GenericGraphicsInterface & gi, 
                         NurbsMapping & trimCurve, 
			 int &curve1, int &curve2, 
                         int curve1End, int curve2End,
                         const real *xSelect,
                         const real *c1click )
// =======================================================================================================
// /Access:
//     This is a protected function.
// /Description:
//     Join to curves where they intersect.
// =======================================================================================================
{
  int returnValue=0;
  
  IntersectionMapping intersect;
  int numberOfIntersectionPoints=0;
  realArray localIntersection;
  realArray rmap1, rmap2;

  bool parallel = intersect.intersectCurves(trimCurve.subCurveFromList(curve1), trimCurve.subCurveFromList(curve2), 
					       numberOfIntersectionPoints, rmap1, rmap2, 
					       localIntersection)==-1;

  if ( parallel )
    gi.createMessageDialog("the curves appear to be parallel!", errorDialog);
  else
  {
    int nIntersections = numberOfIntersectionPoints + 1;
    realArray inter(nIntersections,2);
    int in =0;
    for ( int i=0; i<numberOfIntersectionPoints; i++ )
    {
      inter(in, 0)= localIntersection(0,i);
      inter(in++,1) = localIntersection(1,i);
    }

    // now add the intersection point determined by the selected curve endpoints
    realArray r1(1,1), r2(1,1), x1(1,2), x2(1,2), xr1(1,2), xr2(1,2);
    r1 = real(curve1End);
    r2 = real(curve2End);
    trimCurve.subCurveFromList(curve1).map(r1,x1,xr1);
    trimCurve.subCurveFromList(curve2).map(r2,x2,xr2);
		     
    if ( sum(pow(x2-x1,2))<0.25 && 
	 (1.0-fabs(sum(xr1*xr2)/sqrt(sum(pow(xr1,2))*sum(pow(xr2,2)))))>FLT_EPSILON )
    {
      real dxb1 = xr1(0,0)*x1(0,1) - xr1(0,1)*x1(0,0);
      real dxb2 = xr2(0,0)*x2(0,1) - xr2(0,1)*x2(0,0);
			 
      inter(in,0) = (xr2(0,0)*dxb1-xr1(0,0)*dxb2)/(xr1(0,0)*xr2(0,1)-xr2(0,0)*xr1(0,1));
      if ( fabs(xr1(0,0))>10*REAL_MIN )
	inter(in,1) = (xr1(0,1)*inter(in,0) + dxb1)/xr1(0,0);
      else
	inter(in,1) = (xr2(0,1)*inter(in,0) + dxb2)/xr2(0,0);
			 
    }
    else
    {
      nIntersections--;
    }

    if ( nIntersections!=0 )
    {
      Range AXES(2);

      RealArray interUsed(1,2);
      // choose an appropriate intersection to use
      //   use the intersection closest to the mouse selection
      real mindist = REAL_MAX;
      int useinter = -1;
      real dist;
      for ( int i=0; i<nIntersections; i++ )
      {
	// bound reasonable intersections by something not too much larger than
	// the expected parameter plane
	if ( max(inter(i,AXES))<10 && min(inter(i,AXES)>-10) )
	{
	  // *wdh* dist = (select.x[0]-inter(i,0))*(select.x[0]-inter(i,0)) +
	  dist = (xSelect[0]-inter(i,0))*(xSelect[0]-inter(i,0)) +
	         (xSelect[1]-inter(i,1))*(xSelect[1]-inter(i,1));
				 
	  if ( dist<mindist )
	  {
	    mindist = dist;
	    useinter = i;
	  }
	}
      }

      if ( useinter>-1 )
      {
	interUsed(0,0) = inter(useinter,0);
	interUsed(0,1) = inter(useinter,1);
	realArray r(1,1), rclick(1,1), xclick(1,2), xi(1,2);
        xi(0,0)=interUsed(0,0);
        xi(0,1)=interUsed(0,1);
	
        r=-1.; // initial guess (none)
	trimCurve.subCurveFromList(curve2).inverseMap(xi, r);
			     
	xclick(0,0) = xSelect[0];
	xclick(0,1) = xSelect[1];
        rclick=-1.; // initial guess (none)
	trimCurve.subCurveFromList(curve2).inverseMap(xclick, rclick);
			     
	if ( rclick(0,0)<r(0,0) )
	  curve2End = End;
	else
	  curve2End = Start;
			     
	xclick(0,0) = c1click[0];
	xclick(0,1) = c1click[1];
	trimCurve.subCurveFromList(curve1).inverseMap(xi,r);
	trimCurve.subCurveFromList(curve1).inverseMap(xclick,rclick);
			     
	if ( rclick(0,0)<r(0,0) )
	  curve1End = End;
	else
	  curve1End = Start;
			     
      }
      else
	curve1 = curve2 = -1;
			 
			 		     
      if ( curve1==-1 || curve2 == -1 )
      {
        returnValue=1;
	gi.createMessageDialog("intersection failed, try clicking closer to the intended intersection!", errorDialog);
      }
      else
      {
	// save the original curves, hidden, then create the new curves
	NurbsMapping c1,c2;
	c1 = trimCurve.subCurveFromList(curve1);
	c2 = trimCurve.subCurveFromList(curve2);
	trimCurve.toggleSubCurveVisibility(max(curve1,curve2));
	trimCurve.toggleSubCurveVisibility(min(curve1,curve2)); 
	c1.moveEndpoint(curve1End, interUsed);
	c2.moveEndpoint(curve2End, interUsed);
	trimCurve.addSubCurve(c1);
	trimCurve.addSubCurve(c2);

      }
    }
    else
      gi.createMessageDialog("intersection failed, try clicking closer to the intended intersection!", errorDialog);
		     
  }
  curve1 = curve2 = -1;
// *wdh*  plotpts.redim(0);
  return returnValue;
}


int TrimmedMapping::
assembleSubCurves(int & currentCurve,
		  GenericGraphicsInterface & gi, 
		  NurbsMapping & trimCurve,
                  NurbsMapping & newCurve,
		  int & numberOfAssembledCurves,
                  NurbsMapping ** & assemblyCurves,
		  MouseSelectMode & mouseMode,
		  bool & curveRebuilt,
		  bool & plotCurve )
// =======================================================================================================
// /Access:
//     This is a protected function.
// /Description:
//     This function is called to assemble sub curves.
// =======================================================================================================

{
  int returnValue=0;
  
  // allocate storage for the curves if it isn't there
  if (!assemblyCurves)
  {
    assemblyCurves = new NurbsMapping*[trimCurve.numberOfSubCurvesInList()];
    for ( int c=0; c<trimCurve.numberOfSubCurvesInList(); c++ ) assemblyCurves[c] = NULL;
  }
	     
//   NurbsMapping *curveToAdd = NULL;
//   for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
//     if ( trimCurve.subCurveFromList(sc).getGlobalID()==select.selection(0,0) )
//     {
//       curveToAdd = &trimCurve.subCurveFromList(sc);
//       break;
//     }

  NurbsMapping *curveToAdd=&trimCurve.subCurveFromList(currentCurve);

  // check that we don't use the same curve more than once!
  int q;
  for (q=0; q<numberOfAssembledCurves; q++)
  {
    if (assemblyCurves[q] == curveToAdd)
    {
      curveToAdd = NULL;
      break;
    }
  }
  
  if (curveToAdd == NULL)
  {
    gi.createMessageDialog("You can only use a subcurve once!", errorDialog);
    returnValue=1;
  }
  else
  {
    if ( numberOfAssembledCurves==0 )
    {
      newCurve = *curveToAdd;
      assemblyCurves[numberOfAssembledCurves++] = curveToAdd;
    }
    else if ( newCurve.merge(*curveToAdd)!=0 )
    {
      gi.createMessageDialog("curves are not close enough to join!", errorDialog);
      returnValue=1;
    }
    else
    {
      assemblyCurves[numberOfAssembledCurves++] = curveToAdd;
    }
	 
    if ( connectCurveEnds(newCurve)==0 )//newCurve.getIsPeriodic(axis1) != notPeriodic )
    {
      // --- curve has apparently finished ---

      // *wdh* 010901 real area, arcLen, scale0, scale1;
      // *wdh* 010901 getAreaAndArcLength( newCurve, area, arcLen, scale0, scale1 );
      real area=getArea(newCurve);
      
		 
      if ( fabs(area)<FLT_MIN )
      {
	gi.createMessageDialog("ASSEMBLY FAILED : overlapping curves were found", errorDialog);
        returnValue=1;
      }
      else if ( !verifyTrimCurve((Mapping *)&newCurve) )
      {
	aString buff;
	bool curveok;
	buff = "ASSEMBLY FAILED : problems with new curve : "+reportTrimCurveInfo( (Mapping*)&newCurve, curveok);
	gi.createMessageDialog(buff, errorDialog);
        returnValue=1;
      }
      else if ( trimCurve.numberOfSubCurves()>newCurve.numberOfSubCurves() )
      {
	gi.createMessageDialog("ASSEMBLY FAILED : periodic tolerance satisisfied but unused subcurves were found!",errorDialog);
	returnValue=1;
      }
      else 
      {

	if ( numberOfAssembledCurves<trimCurve.numberOfSubCurvesInList() )
	{
	  // also save the remaining subcurves in newCurve 
	  bool used = false;
	  for ( int sc=0; sc<trimCurve.numberOfSubCurvesInList(); sc++ )
	  {
	    used = false;
	    for ( int as=0; as<numberOfAssembledCurves && !used; as++ )
	    {
	      if ( trimCurve.subCurveFromList(sc).getGlobalID()==assemblyCurves[as]->getGlobalID() )
		used = true;
	    }
	    if ( !used ) 
	    {
	      int nc = newCurve.addSubCurve(trimCurve.subCurveFromList(sc));
	      if ( trimCurve.isSubCurveHidden(sc) ) newCurve.toggleSubCurveVisibility(nc);
	    }

	  }
	}
		     
	delete [] assemblyCurves;
	assemblyCurves  = NULL;
	numberOfAssembledCurves = 0;
	mouseMode = nothing;
	trimCurve = newCurve;
	curveRebuilt = true;
	plotCurve = true;
// *wdh*	interface.setToggleState(0, plotCurve);

	// gi.outputToCommandFile(sPrintF(line,"assemble curves %i\n",selectedCurve));

	gi.outputString("trim curve complete!");

	// AP: exit to calling function directly!
// *wdh*	if ( autoExit )
// *wdh*	  break;
      }
    }
    else
    {
      // *wdh* 081030 returnValue = 1;  // *wdh* -- this should not be an error return 
    }
  } // end if curveToAdd != NULL
	     
  return returnValue;
}
