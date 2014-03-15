#include "Mapping.h"

#include "NurbsMapping.h"
#include "ArraySimple.h"
#include "MappingProjectionParameters.h"

extern bool findRBound(realArray &rc, bool collapsedEdge[2][3], real &rBound, int &axis, bool &atMin);

namespace {

  bool curvatureTolExceeded(Mapping *surf, realArray &midPx, realArray &midPr, realArray &xr, realArray &xrr, 
			    Index &I1, Index &I2, real &midD, real &edgeD, real &distTol)
  {

    // we check the normal curvature (=k) in each of the two parameter space coordinate directions
    // refine if :
    // 1. the radius of curvature (1/k) is less than 1/2 the edge length ( unless edgeD<.25*distTol )
    // 2. if ( .5 edgeD < 1/k ) :
    //      consider a circle of radius 1/k with a chord produced by a line segment of length edgeD
    //      refine the edge if the distance between the midpoint of the line and the circle is greater than distTol

    Range AXES(3);

    midPr = -1;
    surf->inverseMap(midPx,midPr);

    bool trustDerivative = true;
    if ( surf->getTypeOfCoordinateSingularity( 0,1 )==Mapping::polarSingularity && 
	 fabs(midPr(0,1)-(real)surf->getDomainBound(0,1))<5*distTol )
      trustDerivative = false;
    
    if ( surf->getTypeOfCoordinateSingularity( 1,1 )==Mapping::polarSingularity && 
	 fabs(midPr(0,1)-(real)surf->getDomainBound(1,1))<5*distTol )
      trustDerivative = false;

    surf->map(midPr, midPx, xr);
    surf->secondOrderDerivative(I1,midPr,xrr,0,0);
    surf->secondOrderDerivative(I1,midPr,xrr,1,0);
    surf->secondOrderDerivative(I1,midPr,xrr,2,0);
    
    real dsdr = sqrt(sum(pow(xr(0,AXES,0),2)));
    
    real dsdr_c_d2sdr2 = sqrt(  (xr(0,1,0)*xrr(0,2)-xr(0,2,0)*xrr(0,1))*(xr(0,1,0)*xrr(0,2)-xr(0,2,0)*xrr(0,1)) +
				(xr(0,0,0)*xrr(0,2)-xr(0,2,0)*xrr(0,0))*(xr(0,0,0)*xrr(0,2)-xr(0,2,0)*xrr(0,0)) +
				(xr(0,0,0)*xrr(0,1)-xr(0,1,0)*xrr(0,0))*(xr(0,0,0)*xrr(0,1)-xr(0,1,0)*xrr(0,0)) );

    real k = fabs(dsdr)>1000*REAL_EPSILON ? dsdr_c_d2sdr2/(dsdr*dsdr*dsdr) : REAL_MAX;

    //    cout<<"curvature in dir 0 "<<k<< ( trustDerivative ? " and I believe it ": " and I don't believe it")<<endl;

    if ( trustDerivative )
      {
	if ( k>10*REAL_EPSILON )
	  {
	    if ( (1./k < .5*edgeD) && (edgeD>.25*distTol) ) // condition 1
	      return true;
	    
	    real d = 1./k - sqrt( 1./k/k - edgeD*edgeD/4. );
	    if ( d>distTol )
	      return true;
	  }
      }
    else
      if ( edgeD>5*distTol )
	return true;

    trustDerivative = true;
    if ( surf->getTypeOfCoordinateSingularity( 0,0 )==Mapping::polarSingularity && 
	 fabs(midPr(0,0)-(real)surf->getDomainBound(0,0))<5*distTol )
      trustDerivative = false;
    
    if ( surf->getTypeOfCoordinateSingularity( 1,0 )==Mapping::polarSingularity && 
	 fabs(midPr(0,0)-(real)surf->getDomainBound(1,0))<5*distTol )
      trustDerivative = false;

    surf->secondOrderDerivative(I1,midPr,xrr,0,1);
    surf->secondOrderDerivative(I1,midPr,xrr,1,1);
    surf->secondOrderDerivative(I1,midPr,xrr,2,1);
    
    dsdr = sqrt(sum(pow(xr(0,AXES,1),2)));
    dsdr_c_d2sdr2 = sqrt(  (xr(0,1,1)*xrr(0,2)-xr(0,2,1)*xrr(0,1))*(xr(0,1,1)*xrr(0,2)-xr(0,2,1)*xrr(0,1)) +
			   (xr(0,0,1)*xrr(0,2)-xr(0,2,1)*xrr(0,0))*(xr(0,0,1)*xrr(0,2)-xr(0,2,1)*xrr(0,0)) +
			   (xr(0,0,1)*xrr(0,1)-xr(0,1,1)*xrr(0,0))*(xr(0,0,1)*xrr(0,1)-xr(0,1,1)*xrr(0,0)) );
    
    k = fabs(dsdr)>1000*REAL_EPSILON ? max(k,dsdr_c_d2sdr2/(dsdr*dsdr*dsdr)) : REAL_MAX;

    //    cout<<"curvature in dir 1 "<<k<<( trustDerivative ? " and I believe it ": " and I don't believe it")<<endl;

    if ( trustDerivative )
      {
	if ( k>10*REAL_EPSILON )
	  {
	    if ( (1./k < .5*edgeD) && (edgeD>.25*distTol) ) // condition 1
	      return true;
	    
	    real d = 1./k - sqrt( 1./k/k - edgeD*edgeD/4. );
	    if ( d>distTol )
	      return true;
	  }
      }
    else
      if ( edgeD>5*distTol )
	return true;

    return false;
  }

}

bool
refineCurve(NurbsMapping &curve, Mapping *surf1, Mapping *surf2, real distTol, real curveTol, realArray &g)
{
  // kkc 030423
  // refine the grid g of the curve according to tolerances specified by distTol and curveTol

  // a segement in g will be refined if the midpoint distance to the curve is greater than distTol
  //    ... curvature tolerance (what is this exactly!).

  bool isOk = true;

  bool someSplit = true;
  int nv = g.getLength(0);
  Range AXES(3);

  ArraySimpleFixed<real,3,1,1,1> p1,p2,e;
  realArray pc(3);
  intArray split(2*nv);
  realArray midP(1,3), midPP(1,3), midPx(1,3), midPr(1,2), edgeD, midD;
  realArray xr(1,3,2), xrr(2,3);
  Index I1(0),I2(1);
  MappingProjectionParameters mp;
  mp.getRealArray(MappingProjectionParameters::x).redim(0);
  mp.getRealArray(MappingProjectionParameters::r).redim(0);
  mp.getRealArray(MappingProjectionParameters::xr).redim(0);
  mp.getRealArray(MappingProjectionParameters::normal).redim(0);

  bool collapsedEdge1[2][3], collapsedEdge2[2][3];

  int numberOfGridPoints[2];
  real averageArclength[2];
  real elementDensityTolerance=.05;

  //  g.display("grid before");

#if 0
  if ( surf1 )
    surf1->determineResolution(numberOfGridPoints,collapsedEdge1,averageArclength,elementDensityTolerance );

  if ( surf2 )
    surf2->determineResolution(numberOfGridPoints,collapsedEdge2,averageArclength,elementDensityTolerance );
#endif

  while ( someSplit  ) 
    {
      int nvold = nv;
      
      int a,nsplit;
      someSplit = false;
      nsplit = 0;
      split = 1;

#if 0
      midP.redim(nvold-1,1,1,3);
      midP = .5 * ( g(Range(1,nvold-1),0,0,AXES) + g(Range(0,nvold-2),0,0,AXES) );
      midP.reshape(nvold-1,3);
      midPP.redim(midP);
      midPP = midP;

      mp.getRealArray(MappingProjectionParameters::r).resize(nvold-1,1);
      mp.getRealArray(MappingProjectionParameters::r) = -1;
      mp.getRealArray(MappingProjectionParameters::x).redim(0);
      mp.getRealArray(MappingProjectionParameters::x) = midP;
      
      curve.project(midPP,mp);
#endif
      
      for ( int v=0; v<nvold-1; v++ )
	{
	  if ( split(v) ) // this was split on the last cycle through
	    {

	      //	      midP.redim(nvold-1,1,1,3);
	      midP.reshape(1,1,1,3);
	      midP = .5 * ( g(v+1,0,0,AXES) + g(v,0,0,AXES) );
	      midP.reshape(1,3);
	      //	      midPP.redim(midP);
	      midPP = midP;
	      
	      mp.getRealArray(MappingProjectionParameters::r).resize(1,1);
	      mp.getRealArray(MappingProjectionParameters::r) = -1;
	      mp.getRealArray(MappingProjectionParameters::x).resize(1,3);
	      mp.getRealArray(MappingProjectionParameters::x) = midP;
	      
	      curve.project(midPP,mp);

	      real midD = sqrt(sum(pow(midP(0,AXES)-midPP(0,AXES),2)));
	      real edgeD = sqrt(sum(pow(g(v+1,0,0,AXES) - g(v,0,0,AXES),2)));
	      
	      bool splitEdge = midD>distTol;
	      
	      xrr = 0;
	      midPx = midPP(0,AXES);
	      if ( surf1 && !splitEdge )
		splitEdge = curvatureTolExceeded(surf1, midPx,midPr,xr,xrr, I1, I2, midD, edgeD, distTol);
	      
	      if ( surf2 && !splitEdge )
		splitEdge = curvatureTolExceeded(surf2, midPx,midPr,xr,xrr, I1, I2, midD, edgeD, distTol);
	      
	      if ( splitEdge ) 
		{
		  split(v) = 1;
		  someSplit = true;
		  nsplit++;
		}
	      else
		{
		  split(v) = 0;
		}
	    }
	}
      
      if ( someSplit )
	{
	  nv = nvold + nsplit;
	  int vv=0;
	  int v;
	  
	  g.resize(nv,1,1,3);
	  intArray tmpSplit(nv);
	  tmpSplit = 0;
	  realArray oldVerts;
	  oldVerts = g;
	  int sv;
	  vv = sv = 0;
	  for ( v=0; v<nvold-1; v++ )
	    if ( split(v)==1 )
	      {
		midP.reshape(1,1,1,3);
		midP = .5 * ( oldVerts(v+1,0,0,AXES) + oldVerts(v,0,0,AXES) );
		midP.reshape(1,3);
		//	      midPP.redim(midP);
		midPP = midP;

		mp.getRealArray(MappingProjectionParameters::r).resize(1,1);
		mp.getRealArray(MappingProjectionParameters::r) = -1;
		mp.getRealArray(MappingProjectionParameters::x).resize(1,3);
		mp.getRealArray(MappingProjectionParameters::x) = midP;
		
		curve.project(midPP,mp);

		for ( a=0; a<3; a++ )
		  g(vv+1,0,0,a) = midPP(0,a);
		
		tmpSplit(vv)=1;
		vv++; 
		
		for ( a=0; a<3; a++ )
		  g(vv+1,0,0,a) = oldVerts(v+1,0,0,a);
		tmpSplit(vv)=1;
		vv++;
	      }
	    else
	      {
		for ( a=0; a<3; a++ )
		  g(vv+1,0,0,a) = oldVerts(v+1,0,0,a);
		tmpSplit(vv)=0;
		vv++;
	      }
	  
	  //	  if ( nv>split.getLength(0) ) split.resize(nv+10);
	  split.redim(0);
	  split = tmpSplit;

	  //	  cout<<"nv = "<<nv<<endl;
	}

    }
  //  g.display("grid after");

  return isOk;
}
