//#define OV_DEBUG
//#define BOUNDS_CHECK
//kkc 081124 #include <iostream.h>
#include <iostream>

#include "AdvancingFront.h"
#include "Geom.h"
#include "interpPoints.h"
#include "MappingProjectionParameters.h"
#include "CompositeSurface.h"
#include "MeshQuality.h"

//static const bool debug_af = true; // set this to false to turn off lots of assertions
#define debug_af false
//#define debug_af ( test_face==966 )

static real faceNormalTiming;
static real matvecTiming;
static int test_face=-1;

extern void exactinit();

using namespace std;

void
idealVertexIteration(ArraySimple<real> &faceVerts, ArraySimple<real> &pIdeal, real delta)
{
  //cout<<"old pIdeal "<<pIdeal<<endl;

  real pmag[4];
  real pmagt[4];
  real ptmp[3];
  real ctmp[3];

  int nfv = faceVerts.size(0);

  pmag[0] = pmag[1] = pmag[2] = pmag[3] = 0;

  int p,a;

  real tolmax = -REAL_MAX;
  for ( p=0; p<nfv; p++ )
    {
      for ( a=0; a<3; a++ )
	pmag[p] += (pIdeal(a)-faceVerts(p,a))*(pIdeal(a)-faceVerts(p,a));

      pmag[p] = sqrt(pmag[p]);
      pmagt[p] = pmag[p];
      tolmax = max(tolmax,fabs(delta-sqrt(pmagt[p])));
    }

  real tol = 0.01;
  int itmax = 10;
  real w = .05;

  int it = 0;
  
  while ( it<itmax && tolmax>tol )
    {
      ctmp[0] = ctmp[1] = ctmp[2] = 0;
      for ( p=0; p<nfv; p++ )
	{
	  for ( int a=0; a<3; a++ )
	    ctmp[a] += (faceVerts(p,a) + ( pIdeal(a)-faceVerts(p,a) )/pmag[p])/real(nfv);
	}

      for ( a=0; a<3; a++ )
	{
	  pIdeal[a] += w*(ctmp[a]-pIdeal[a]);
	}

      tolmax = -REAL_MAX;
      for ( p=0; p<nfv; p++ )
	{
	  pmagt[p] = 0.;
	  for ( a=0; a<3; a++ )
	    pmagt[p] += (pIdeal(a)-faceVerts(p,a))*(pIdeal(a)-faceVerts(p,a));
	  tolmax = max(tolmax,fabs(delta-sqrt(pmagt[p])));
	}
      
      //      cout<<"tolmax "<<tolmax<<endl;
      it++;
    }

  //cout<<"new pIdeal "<<pIdeal<<endl;
}

AdvancingFrontParameters::
AdvancingFrontParameters( real maxang   /*=80.*/, 
			  real egrowth  /*=-1.0*/,
			  bool usefunc  /*=true*/,
			  int defAdvNum /*=-1*/,
			  real qt /*=0.01*/ ) 
{
  setMaxNeighborAngle( maxang  );
  setEdgeGrowthFactor( egrowth );
  useControlFunction = usefunc;
  setNumberOfAdvances( defAdvNum );
  
  setAuxiliaryAngleTolerance(2.);

  setQualityTolerance(qt);

  for ( int i=0; i<int(lastParam); i++ )
    toggleParams[StateEnum(i)] = false;

  toggle(frontFaces);
  toggle(frontEdges);

  highlightedFace = -1;

  searchDistFactor = 1.5;
  discardDistFactor = 1. - 10*FLT_EPSILON;
}

//
// Several static functions used only during the advancement of a front follow:
//  computeFaceSize : compute an edge length or area (3d) of a face given only the vertices
//  computeFaceNormal : compute a face normal for a face given only the vertices
//

//ArraySimple<real> 
static inline void
//AdvancingFront::
matVectMult(const ArraySimple<real> &M, const ArraySimple<real> &V, ArraySimple<real> &res )
{

  real t0 = getCPU();

  AssertException (!debug_af ||
		   (M.size(1) == V.size(0)), DimensionError());

  const int len0 = M.size(0);
  const int len1 = M.size(1);

  //ArraySimple<real> res(len0);

  for ( int r=0; r<len0; r++ )
    {
      res[r] = 0.0;
      for ( int r1=0; r1<len1; r1++ )
	  res[r] += M[r1*len0+r]*V(r1);
    }

  matvecTiming += getCPU()-t0;

  //return res;
}

//==========================================================================================

static inline real 
computeFaceSize(const ArraySimple<real> & xyz)
{
  // compute the "size" of a face described by the vertices in xyz
  int domainDimension = xyz.size(0)==2 ? 2 : 3;

  //int domainDimension = xyz.size(1);
  real size = 0.0;
  if (xyz.size(0)==2) // in 2D this is just the length of the line segment between the two points
  {
    if ( xyz.size(1)==2 )
      size = sqrt( (xyz(1,0)-xyz(0,0))*(xyz(1,0)-xyz(0,0)) +
		   (xyz(1,1)-xyz(0,1))*(xyz(1,1)-xyz(0,1)) );
    else
      size = sqrt( (xyz(1,0)-xyz(0,0))*(xyz(1,0)-xyz(0,0)) +
		   (xyz(1,1)-xyz(0,1))*(xyz(1,1)-xyz(0,1)) +
		   (xyz(1,2)-xyz(0,2))*(xyz(1,2)-xyz(0,2)) );
  }
  else
    {
      // Well, lets assume that in 3D we can have 3, 4, ..., n sided faces.
      // In this case, faces with more than 3 edges could be non planar.

      // The "size" of a face will be defined as the sum of the areas of triangluar sides
      // consisting of the center of the face and the two vertices along an edge.
      // XXX 5/1/01, try the Area/(longest edge)

      int a;
      int nv=xyz.size(0);
#if 0
      VectorSimple<real> center(domainDimension);
      for ( a=0; a<domainDimension; a++ ) center[a] = 0.0;

      for ( a=0; a<domainDimension; a++ )
	for ( int vv=0; vv<nv; vv++ )
	  center[a] += xyz(vv, a)/real(nv);

      real e1[3], e2[3];
      for (int v=0; v<nv; v++)
	{
	  int vp = (v+1)%nv;
	  
	  for ( a=0; a<domainDimension; a++ )
	    {
	      e1[a] = xyz(v, a) - center[a];
	      e2[a] = xyz(vp,a) - center[a];
	    }
	  
	  real ee1 = e1[1]*e2[2] - e1[2]*e2[1];
	  real ee2 = e1[0]*e2[2] - e1[2]*e2[0];
	  real ee3 = e1[0]*e2[1] - e1[1]*e2[0];
	  size += (Real) 0.5 * sqrt( ee1*ee1 + ee2*ee2 + ee3*ee3 );
	}
#else
      int v;

      real cent[3],e1[3],e2[3],norm[3];
      cent[0] = cent[1] = cent[2] = 0.;

      for ( v=0; v<nv; v++ )
	for ( a=0; a<3; a++ )
	  cent[a] += xyz(v,a)/real(nv);
      
      real asum;
      real minmag = REAL_MAX;
      real maxmag = -REAL_MAX;
      real avmag = 0.0;
      real area = 0.0;
      for (v=0; v<nv; v++)
	{
	  int vp = (v+1)%nv;
	  asum = 0.0;
	  for ( a=0; a<domainDimension; a++ )
	    {
	      asum += (xyz(v,a) - xyz(vp,a))*(xyz(v,a) - xyz(vp,a));
	      e1[a] = cent[a] - xyz(v, a);
	      e2[a] = cent[a] - xyz(vp,a);
	    }
	  minmag = min(minmag,asum);
	  maxmag = max(maxmag,asum);
	  avmag += sqrt(asum);

	  norm[0] = e1[1]*e2[2] - e2[1]*e1[2];
	  norm[1] = - (e1[0]*e2[2] - e2[0]*e1[2]);
	  norm[2] = e1[0]*e2[1] - e2[0]*e1[1];
	  area += 0.5*sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
	}
      
      //size = avmag/real(nv);
      size = area/sqrt(maxmag);
      //if (  nv==3 ) size /= 2;

      //size = sqrt(minmag);
#endif
    }

  return size;
}

inline 
static
real 
computeAvgEdgeLength(const ArraySimple<real> & xyz, real &minl, real &maxl)
{
  // compute the "size" of a face described by the vertices in xyz
  int domainDimension = xyz.size(0)==2 ? 2 : 3;

  //int domainDimension = xyz.size(1);
  real size = 0.0;
  if (xyz.size(0)==2) // in 2D this is just the length of the line segment between the two points
  {
    if ( xyz.size(1)==2 )
      size = sqrt( (xyz(1,0)-xyz(0,0))*(xyz(1,0)-xyz(0,0)) +
		   (xyz(1,1)-xyz(0,1))*(xyz(1,1)-xyz(0,1)) );
    else
      size = sqrt( (xyz(1,0)-xyz(0,0))*(xyz(1,0)-xyz(0,0)) +
		   (xyz(1,1)-xyz(0,1))*(xyz(1,1)-xyz(0,1)) +
		   (xyz(1,2)-xyz(0,2))*(xyz(1,2)-xyz(0,2)) );
  }
  else
    {
      // Well, lets assume that in 3D we can have 3, 4, ..., n sided faces.
      // In this case, faces with more than 3 edges could be non planar.

      // The "size" of a face will be defined as the sum of the areas of triangluar sides
      // consisting of the center of the face and the two vertices along an edge.
      // XXX 5/1/01, try the Area/(longest edge)

      int a;
      int nv=xyz.size(0);
#if 0
      VectorSimple<real> center(domainDimension);
      for ( a=0; a<domainDimension; a++ ) center[a] = 0.0;

      for ( a=0; a<domainDimension; a++ )
	for ( int vv=0; vv<nv; vv++ )
	  center[a] += xyz(vv, a)/real(nv);

      real e1[3], e2[3];
      for (int v=0; v<nv; v++)
	{
	  int vp = (v+1)%nv;
	  
	  for ( a=0; a<domainDimension; a++ )
	    {
	      e1[a] = xyz(v, a) - center[a];
	      e2[a] = xyz(vp,a) - center[a];
	    }
	  
	  real ee1 = e1[1]*e2[2] - e1[2]*e2[1];
	  real ee2 = e1[0]*e2[2] - e1[2]*e2[0];
	  real ee3 = e1[0]*e2[1] - e1[1]*e2[0];
	  size += (Real) 0.5 * sqrt( ee1*ee1 + ee2*ee2 + ee3*ee3 );
	}
#else
      int v;

      real cent[3],e1[3],e2[3],norm[3];
      cent[0] = cent[1] = cent[2] = 0.;

      for ( v=0; v<nv; v++ )
	for ( a=0; a<3; a++ )
	  cent[a] += xyz(v,a)/real(nv);
      
      real asum;
      real minmag = REAL_MAX;
      real maxmag = -REAL_MAX;
      real avmag = 0.0;
      real area = 0.0;
      for (v=0; v<nv; v++)
	{
	  int vp = (v+1)%nv;
	  asum = 0.0;
	  for ( a=0; a<domainDimension; a++ )
	    {
	      asum += (xyz(v,a) - xyz(vp,a))*(xyz(v,a) - xyz(vp,a));
	      e1[a] = cent[a] - xyz(v, a);
	      e2[a] = cent[a] - xyz(vp,a);
	    }
	  minmag = min(minmag,asum);
	  maxmag = max(maxmag,asum);
	  avmag += sqrt(asum);

	  norm[0] = e1[1]*e2[2] - e2[1]*e1[2];
	  norm[1] = - (e1[0]*e2[2] - e2[0]*e1[2]);
	  norm[2] = e1[0]*e2[1] - e2[0]*e1[1];
	  area += sqrt(norm[0]*norm[0] + norm[1]*norm[1] + norm[2]*norm[2]);
	}
      
      size = avmag/real(nv);
      maxl = sqrt(maxmag);
      minl = sqrt(minmag);
      //size = area/sqrt(maxmag);
      //size = sqrt(minmag);
#endif
    }

  return size;
}
//==========================================================================================
ArraySimpleFixed<real,3,1,1,1>
AdvancingFront::
computeSurfaceNormal(const ArraySimpleFixed<real,3,1,1,1> &vert, int subsurf)
{
  ArraySimpleFixed<real,3,1,1,1> n;
  realArray verticesToInvert(1,rangeDimension), 
    r(1,domainDimension), xr(1,rangeDimension,domainDimension);

  bool inverted = false;
  real dist=REAL_MAX;
  Range A(0,2);
  for ( int m=0; !inverted && m<backgroundMappings.mappingList.getLength(); m++ )
    {

      // don't really need the first two vertices, only the midpoint...
      verticesToInvert = 0.;
      r = -1.;
      xr = 0.;
	      
      for ( int axis=0; axis<rangeDimension; axis++ )
	verticesToInvert(0,axis) = vert[axis];
	      
      realArray xp;
      xp =verticesToInvert;
      //verticesToInvert(2,AXES) = (verticesToInvert(0,AXES)+verticesToInvert(1,AXES))/2.;

      Mapping & map = *(backgroundMappings.mappingList[m].mapPointer);

      MappingProjectionParameters mp;
      mp.getRealArray(MappingProjectionParameters::r) = r;
      mp.getRealArray(MappingProjectionParameters::x) = verticesToInvert;
      mp.getRealArray(MappingProjectionParameters::xr) = xr;
      mp.getRealArray(MappingProjectionParameters::normal).redim(1,rangeDimension);
      mp.getIntArray(MappingProjectionParameters::subSurfaceIndex).redim(1);
      mp.getIntArray(MappingProjectionParameters::subSurfaceIndex) = subsurf;
	      
      if ( subsurf!=-1 )
	((CompositeSurface &)map)[subsurf].project(verticesToInvert,mp);
      else
	map.project(verticesToInvert, mp);
	      
      real sj;
      if ( map.getClassName()=="CompositeSurface" )
	{
	  int s = mp.getIntArray(MappingProjectionParameters::subSurfaceIndex)(0);
	  //sj = ((CompositeSurface &)map)[s].getSignForJacobian();
	  sj = ((CompositeSurface &)map).getSignForNormal(s);
      //cout<<"sj = "<<sj<<", "<<((CompositeSurface &)map)[s].getSignForJacobian()<<endl;
	  //	  sj *= ((CompositeSurface &)map).getSignForNormal(s)>0 ? 1. : -1;;
	}
      else
	sj = map.getSignForJacobian();

      xr = mp.getRealArray(MappingProjectionParameters::xr);
      // surface normal comes from the cross product of xr 
          real dist_cand = sum(pow(verticesToInvert(0,A)-xp(0,A),2));
          //verticesToInvert.display("vi");
          //xp.display("xp");
          
      //    cout<<"dist, dist_cand "<<m<<" : "<<dist<<"  "<<dist_cand<<endl;
      //sj=;

      if ( dist_cand < dist ) {
          n(0) =    sj*(xr(0,1,0)*xr(0,2,1) - xr(0,1,1)*xr(0,2,0));
          n(1) = -sj*( xr(0,0,0)*xr(0,2,1) - xr(0,0,1)*xr(0,2,0) );
          n(2) =    sj*(xr(0,0,0)*xr(0,1,1) - xr(0,0,1)*xr(0,1,0));
         dist = dist_cand;
      }
      //inverted = true;
    }

  real smag = sqrt(ASmag2(n));
  for ( int a=0; a<3; a++ )
    n[a] /= smag;

  return n;

}

void 
AdvancingFront::
computeFaceNormal(const ArraySimple<real> &vertices, ArraySimple<real> & normal, int subsurf)
{
  real t0 = getCPU();
  // compute the normal of a face defined by vertices, stick the result into normal
    
  //int rangeDimension = normal.size();

  int a,a1;
  for ( a=0; a<normal.size(); a++ ) normal[a] = 0.0;

  real mag;

  if ( vertices.size(0)==2 )
    { 
      if ( rangeDimension==2 )
	{
	  // /Comments :
	  // \begin{verbatim} 
	  //
	  //   X1 +  v1
	  //      .
	  //      .
	  //      .---> N
	  //      . 
	  //      .
	  //   X2 + v2
	  // \end{verbatim}
	  // X1, X2, and N are vectors
	  // The vertices should be ordered such that the front will grow in the correct
	  // direction.  This means, in 2D, that X2-X1 .cross. N sticks up out of the plane
	  // using the right-hand-rule.  This orientation causes N to be 
	  // N = - (X2y - X1y) i + (X2x - X1x) j
	  
	  real mag = sqrt ((vertices(1,0)-vertices(0,0))*(vertices(1,0)-vertices(0,0)) + 
			   (vertices(1,1)-vertices(0,1))*(vertices(1,1)-vertices(0,1)));
	  
	  normal(0) = -(vertices(1,1)-vertices(0,1))/mag;
	  normal(1) = (vertices(1,0)-vertices(0,0))/mag;
	}
      else
	{
	  // here we are generating a surface mesh

	  // /Comments :
	  // \begin{verbatim} 
	  //
	  //   X1 +  v1
	  //      |
	  //      |
	  //     n.---> N
	  //      | 
	  //      |
	  //   X2 + v2
	  // \end{verbatim}
	  // X1, X2, n, and N are vectors. n is a vector normal to the underlying surface pointing 
	  // and is normal to N.
	  // The vertices should be ordered such that the front will grow in the correct
	  // direction.  This means that X2-X1 .cross. N points in the direction of n, or normal
	  // to the surface using the right-hand-rule.  This orientation causes N to be 
	  // N = - ( X2 - X1 ) X n
	  
	  // use the underlying mappings to determine the normal to the surface at the midpoint
	  //   of the face.  

	  Range AXES(0,rangeDimension-1);
	  realArray verticesToInvert(1,rangeDimension), r(1,domainDimension), xr(1,rangeDimension,domainDimension);
	  ArraySimpleFixed<real,3,1,1,1> surfNorm;

	  bool inverted = false;
       real dist = REAL_MAX;
	  for ( int m=0; !inverted && m<backgroundMappings.mappingList.getLength(); m++ )
	    {

	      // don't really need the first two vertices, only the midpoint...
	      verticesToInvert = 0.;
	      r = -1.;
	      xr = 0.;
	      
	      for ( int axis=0; axis<rangeDimension; axis++ )
		verticesToInvert(0,axis) = (vertices(0,axis)+vertices(1,axis))/2.;
	      
	      //verticesToInvert(2,AXES) = (verticesToInvert(0,AXES)+verticesToInvert(1,AXES))/2.;

	      Mapping & map = *(backgroundMappings.mappingList[m].mapPointer);

	      MappingProjectionParameters mp;
	      mp.getRealArray(MappingProjectionParameters::r) = r;
	      mp.getRealArray(MappingProjectionParameters::x) = verticesToInvert;
	      mp.getRealArray(MappingProjectionParameters::xr) = xr;
	      mp.getRealArray(MappingProjectionParameters::normal).redim(1,rangeDimension);
	      mp.getIntArray(MappingProjectionParameters::subSurfaceIndex).redim(1);
	      mp.getIntArray(MappingProjectionParameters::subSurfaceIndex) = subsurf;
	      
	      if ( subsurf!=-1 )
		((CompositeSurface &)map)[subsurf].project(verticesToInvert,mp);
	      else
		map.project(verticesToInvert, mp);
	      
	      real sj;
	      if ( map.getClassName()=="CompositeSurface" )
		{
		  int s = mp.getIntArray(MappingProjectionParameters::subSurfaceIndex)(0);
		  //sj = ((CompositeSurface &)map)[s].getSignForJacobian();
		  sj = ((CompositeSurface &)map).getSignForNormal(s);
      //cout<<"sj ("<<s<<") = "<<sj<<", "<<((CompositeSurface &)map)[s].getSignForJacobian()<<endl;
		  //	  sj *= ((CompositeSurface &)map).getSignForNormal(s)>0 ? 1. : -1;;
		}
	      else
		sj = map.getSignForJacobian();
      //sj=1;
#if 1 
	      xr = mp.getRealArray(MappingProjectionParameters::xr);
	      realArray xp;
          xp = verticesToInvert;
	      // surface normal comes from the cross product of xr at the midpoint
	      surfNorm(0) =    sj*(xr(0,1,0)*xr(0,2,1) - xr(0,1,1)*xr(0,2,0));
	      surfNorm(1) = -sj*( xr(0,0,0)*xr(0,2,1) - xr(0,0,1)*xr(0,2,0) );
	      surfNorm(2) =    sj*(xr(0,0,0)*xr(0,1,1) - xr(0,0,1)*xr(0,1,0));

#else
	      //map.project(verticesToInvert, mp);
	      surfNorm(0) = (mp.getRealArray(MappingProjectionParameters::normal)(0,0));
	      surfNorm(1) = (mp.getRealArray(MappingProjectionParameters::normal)(0,1));
	      surfNorm(2) = (mp.getRealArray(MappingProjectionParameters::normal)(0,2));

#endif

          real dist_cand = sum(pow(verticesToInvert-xp,2));
	      //cout<<"sign for jac "<<sj<<endl;
	      // XXX ! should check to make sure that the points inverted onto mapping m properly!
          if ( dist>dist_cand )
          {
	        normal(0) = -(( vertices(1,1)-vertices(0,1) )*surfNorm[2] - ( vertices(1,2)-vertices(0,2) )*surfNorm[1]);
	        normal(1) =   ( vertices(1,0)-vertices(0,0) )*surfNorm[2] - ( vertices(1,2)-vertices(0,2) )*surfNorm[0];
	        normal(2) = -(( vertices(1,0)-vertices(0,0) )*surfNorm[1] - ( vertices(1,1)-vertices(0,1) )*surfNorm[0]);
            dist = dist_cand;
	        real nmag = sqrt(ASmag2(normal));
	       // inverted = nmag>REAL_EPSILON;//true;
          }
	    }

	  //real nmag = sqrt(ASmag2(surfNorm));
	  //for ( int a=0; a<rangeDimension; a++ ) surfNorm[a] /= nmag;
	  
	  real nmag = sqrt(ASmag2(normal));
      if ( debug_af ) cout<<"NMAG = "<<nmag<<endl;
	  for ( int a=0; a<rangeDimension; a++ ) normal[a] /= nmag;
      if ( debug_af ) cout<<"computed normal = "<<normal<<endl;
      if ( debug_af ) cout<<"surf normal = "<<surfNorm<<endl;
      if ( debug_af ) cout<<"vertices "<<(ArraySimple<real>&)vertices<<endl;
	}
    }
  else
    { 
      // Well, lets assume that in 3D we can have 3, 4, ..., n sided faces.
      // In this case, faces with more than 3 edges could be non planar.
      // The normal of a face will be defined as the average of the normals of triangluar sides
      // consisting of the center of the face and the two vertices along an edge.
      
      ArraySimple<real> center(rangeDimension);
      for ( a=0; a<normal.size(); a++ ) center[a] = 0.0;

      for ( a=0; a<normal.size(); a++ )
	for ( int vv=0; vv<vertices.size(0); vv++ )
	  center(a) += vertices(vv, a)/real(vertices.size(0));

      int nv=vertices.size(0);
      ArraySimple<real> e1(3), e2(3), norm(3);
      for (int v=0; v<nv; v++)
	{
	  int vp = (v+1)%nv;
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      e1[a] = center(a) - vertices(v, a);
	      e2[a] = center(a) - vertices(vp,a);
	    }
	  norm(0) = e1(1)*e2(2) - e2(1)*e1(2);
	  norm(1) = - (e1(0)*e2(2) - e2(0)*e1(2));
	  norm(2) = e1(0)*e2(1) - e2(0)*e1(1);
	  real mag = sqrt(norm(0)*norm(0) + norm(1)*norm(1) + norm(2)*norm(2));
	  for ( a=0; a<normal.size(); a++ )
	    normal[a] += norm(a)/mag/real(nv);
	}
      
      real asum = 0.0;
      for ( a=0; a<rangeDimension; a++ ) asum += normal[a]*normal[a];
      asum = sqrt(asum);
      for ( a=0; a<rangeDimension; a++ ) normal[a] = normal[a]/asum;

    }
  faceNormalTiming += getCPU()-t0;
}

//\begin{>AdvancingFront.tex}{\subsection{Default Constructor}}
AdvancingFront::
AdvancingFront()
//===========================================================================
// /Purpose: Create an uninitialized AdvancingFront.
//\end{AdvancingFront.tex}
//===========================================================================
{
  //  exactinit();

  nFacesFront = 0;
  nFacesTotal = 0;
  nFacesEst = 0;
  nptsTotal = 0;
  nptsEst = 0;
  nElements = 0;
  nElementsEst = 0;
  //backgroundMapping = NULL;

  nexpansions = 0;
}

//\begin{>>AdvancingFront.tex}{\subsection{Constructor}}
AdvancingFront::
AdvancingFront(intArray &initialFaces, 
	       realArray &xyz_in, 
	       MappingInformation *backgroundMappings_ /* = NULL */ )
//===========================================================================
// /Purpose: Create an AdvancingFront initialized with a set of initial faces and a background mapping.
// /initialFaces (input) : the list of initial faces (vertex lists for each face)
// /xyz\_in       (input) : the initial list of vertices, referred to in initialFaces
// /backroundMapping\_ (input, optional) : a pointer the the mapping that represents the underlying surface
//\end{AdvancingFront.tex}
//===========================================================================
{

  initialize(initialFaces, xyz_in, backgroundMappings_);

}

//\begin{>>AdvancingFront.tex}{\subsection{initialize}}
void
AdvancingFront::
initialize(intArray &initialFaces, realArray &xyz_in, MappingInformation *backgroundMappings_ /* = NULL */ , intArray &initialFaceSurfaceMapping_)
//===========================================================================
// /Purpose: Initialize an AdvancingFront with a set of initial faces and a background mapping.
// /initialFaces (input) : the list of initial faces (vertex lists for each face)
// /xyz\_in       (input) : the initial list of vertices, referred to in initialFaces
// /backroundMapping\_ (input, optional) : a pointer the the mapping that represents the underlying surface
//\end{AdvancingFront.tex}
//===========================================================================
{

  destroyFront();

  faceNormalTiming = matvecTiming = 0.;
  for ( int tt=0; tt<numberOfTimings; tt++ )
    timing[tt] = 0.;

  rangeDimension = xyz_in.getLength(1);
  if (backgroundMappings_!=NULL)
    {
      backgroundMappings.mappingList.destroy();

      for ( int mm=0; mm<backgroundMappings_->mappingList.getLength(); mm++ )
	this->backgroundMappings.mappingList.addElement(backgroundMappings_->mappingList[mm]);

      //      this->backgroundMappings = *backgroundMappings_;

      if (backgroundMappings.mappingList.getLength()!=0)
	domainDimension = backgroundMappings.mappingList[0].getDomainDimension();
      else
	throw AdvancingFrontError();

      initialFaceSurfaceMapping.redim(0);
      initialFaceSurfaceMapping = initialFaceSurfaceMapping_;
    }
  else
    domainDimension = rangeDimension;

  // always have these setup exceptions on, they may be usefull and they are cheap

  AssertException ((domainDimension == 2 || domainDimension == 3), DimensionError());
  AssertException ((rangeDimension == 2 || rangeDimension == 3), DimensionError());

  averageFaceSize = 0.0;

  // estimate the needed number of vertices
  nptsTotal = xyz_in.getLength(0);
  nptsEst = 100*nptsTotal;

  nptsTotal = xyz_in.getLength(0);
  Range AXES(0,rangeDimension-1);

  Range Rin(xyz_in.getBase(0),xyz_in.getBound(0));
  xyz.redim(nptsEst, rangeDimension);
  xyz = 0.0;
  // set up the background mapping
//   if (backgroundMappings.mappingList.getLength()==0) 
//       this->backgroundMappings = *backgroundMapping_;
//       if (backgroundMappings.mappingList.getLength()!=0)
// 	if (transformCoordinatesToParametricSpace(xyz_in, xyz)<0) throw AdvancingFrontError();
//       else
// 	throwAdvancingFrontError();
//     } else {
      //this->backgroundMapping = NULL;
  xyz(Rin,AXES) = xyz_in(Rin,AXES);
      //    }

  // set up element topology information
  if (domainDimension == 2) 
    {
      nVerticesPerFaceMax = 2;
      nFacesPerElementMax = 4;
    } else if(domainDimension == 3) {
      nVerticesPerFaceMax = 4;
      nFacesPerElementMax = 6;
    }

  // estimate the number of faces and elements needed
  nFacesEst = initialFaces.getLength(0)*100;//pow(initialFaces.getLength(0), 2); // be generous, hopefully the need to reallocate space will be limited

  nFacesTotal = 0;
  nFacesFront = 0;
  nElements = 0;
  nElementsEst = 2*nFacesEst;

  // allocate some space to start with
  faces.clear();
  //  faces.reserve(nFacesEst);
  //  faceNormals.redim(nFacesEst, rangeDimension);
  resizeFaces(nFacesEst);
  elements.reserve(nElementsEst);
  elementQuality.redim(nElementsEst);

  // get the information needed to initialize the face search tree
  // construct the bounding box domain for the geometric search trees
  ArraySimple<real> boundingBox(2,AXES.getLength());
#if 0
  realArray boundingBox(4,AXES.getLength());
#endif

  int i = initialFaces.getBase(0);
  int axis;

  for (axis=0; axis<=AXES.getBound(); axis++) 
    {
      boundingBox(0,axis) = xyz(initialFaces(i,0),axis);
      boundingBox(1,axis) = xyz(initialFaces(i,0),axis);

#if 0
      boundingBox(0,axis) = xyz(initialFaces(i,0),axis);
      boundingBox(1,axis) = xyz(initialFaces(i,0),axis);
      boundingBox(2,axis) = xyz(initialFaces(i,0),axis);
      boundingBox(3,axis) = xyz(initialFaces(i,0),axis);
#endif
    }

  for (i=initialFaces.getBase(0); i<=initialFaces.getBound(0); i++)
    {
      // loop through each coordinate axis determining the extents of the bounding boxes
      //   for all the faces.
      for (axis=0; axis<=AXES.getBound(); axis++)
	{
	  real xmin = xyz(initialFaces(i,0),axis);
	  real xmax = xmin;
	  // first find the min and max for this coordinate axis on face i
	  for (int vi = 1; vi<nVerticesPerFaceMax; vi++)
	    {
	      int v = initialFaces(i,vi);
	      if (v != -1) 
		{
		  xmin = min(xmin, xyz(v, axis));
		  xmax = max(xmax, xyz(v, axis));
		}
	    }
	  boundingBox(0,axis) = min(boundingBox(0,axis),xmin);
	  boundingBox(1,axis) = max(boundingBox(1,axis),xmax);
#if 0
	  boundingBox(0,axis) = min(boundingBox(0,axis), xmin);  // minXmin
	  boundingBox(1,axis) = max(boundingBox(1,axis), xmin);  // maxXmin
	  boundingBox(2,axis) = min(boundingBox(2,axis), xmax);  // minXmax
	  boundingBox(3,axis) = max(boundingBox(3,axis), xmax);  // maxXmax
#endif
	}
    }

  for ( int mm=0; mm<backgroundMappings.mappingList.getLength(); mm++ )
    {
      //realArray &mbb = backgroundMappings.mappingList[0].getMapping().getBoundingBox();
      Mapping & mbb = *(backgroundMappings.mappingList[mm].mapPointer);
      for (axis=0; axis<=AXES.getBound(); axis++)
	{
	  boundingBox(0,axis) = min(boundingBox(0,axis),mbb.getRangeBound(0,axis));
	  boundingBox(1,axis) = max(boundingBox(1,axis),mbb.getRangeBound(1,axis));
	}
    }

  // pad the bounding box by 10% to force vertices/faces on the boundaries to be inside
  for (axis=0; axis<=AXES.getBound(); axis++)
    {
      real per = 0.1;
      real da = per*(boundingBox(1,axis) - boundingBox(0,axis));
      boundingBox(0,axis) = boundingBox(0,axis) - da;
      boundingBox(1,axis) = boundingBox(1,axis) + da;
#if 0
      da = per*(boundingBox(3,axis) - boundingBox(2,axis));
      boundingBox(2,axis) = boundingBox(2,axis) - da;
      boundingBox(3,axis) = boundingBox(3,axis) + da;
#endif
    }

  // build the face searching tree
  //cout<<boundingBox<<endl;
  ArraySimple<real> bb0(boundingBox.size());
  bb0 = boundingBox;
  faceSearcher.initTree(rangeDimension, bb0);
  vertexSearcher.initTree(rangeDimension, bb0);
#if 0
  faceSearcher.initTree(boundingBox.reshape(4*AXES.getLength()));
  vertexSearcher.initTree(boundingBox);
#endif

  // now insert each face into the front
  //IntegerArray faceVertices(nVerticesPerFaceMax);
  ArraySimple<int> faceVertices(nVerticesPerFaceMax);
  bool inserted;
  for (i=initialFaces.getBase(0); i<=initialFaces.getBound(0); i++)
    {
      // make a compact list of the vertices in each face
      int nVerticesOnFace = 0;
      //cout<<"face "<<i<<" : ";
      for (int vi = 0; vi<nVerticesPerFaceMax; vi++)
	{
	  int v = initialFaces(i, vi);
	  if ( v != -1 ) 
	    {
	      faceVertices(nVerticesOnFace) = v;
	      nVerticesOnFace++;
	    } 
	  //cout<<v<<" ";
	}
      //cout<<endl;
      //faceVertices.resize(nVerticesOnFace);
      ArraySimple<int> faceV(nVerticesOnFace);
      for ( int v=0; v<nVerticesOnFace; v++ )
	    faceV[v] = faceVertices[v];
      // insert the new face
      inserted = insertFace(faceV, -1, -1)>-1;
      assert(inserted);
    }

  // insert the vertices into the vertexSearcher
  ArraySimple<real> vbbox(2*rangeDimension);
#if 0
  realArray vbbox(2*rangeDimension);
#endif
    for ( int v=0; v<nptsTotal; v++ )
      {
	if ( pointFaceMapping[v].size()>0 )
	  {
	    for ( int a=0; a<rangeDimension; a++ )
	      vbbox(2*a) = vbbox(2*a+1) = xyz(v,a);
	    try{
	      vertexSearcher.addElement(vbbox, v);
	    } catch ( GeometricADTError &e ) {
	      e.debug_print();
	      
	      throw FrontInsertionFailedError();
	    }
	  }
      }

    // scan for and remove duplicate vertices
    int nDup=0;
    for ( int v=0; v<nptsTotal; v++ )
    {        
	  for ( int a=0; a<rangeDimension; a++ )
	   {
        vbbox(2*a) = xyz(v,a);  
        vbbox(2*a+1) = xyz(v,a);
       }
     
      GeometricADT<int>::traversor vtrav(vertexSearcher, vbbox);
      vector<PriorityQueue::iterator> &vFaces = pointFaceMapping[v];
      if ( vtrav.isFinished() && debug_af ) cout<<"vertex traversor for "<<v<<" found no matches"<<endl;
      while ( vFaces.size() && !vtrav.isFinished() )
      {
          int vv=(*vtrav).data;
          real dist2 = 0;
          for ( int a=0; a<rangeDimension; a++ )
            dist2+= (xyz(v,a)-xyz(vv,a))*(xyz(v,a)-xyz(vv,a));
          if ( v!=vv && dist2<100*REAL_MIN )
          {   
              nDup++;
              vector<PriorityQueue::iterator> &vvFaces = pointFaceMapping[vv];
              for (vector<PriorityQueue::iterator>::iterator f=vvFaces.begin();f!=vvFaces.end(); f++)
	          { 
                Face &face = ***f;
                for ( int fv=0; fv<face.getNumberOfVertices(); fv++ )
                    if ( face.getVertex(fv)==vv ) 
                    {
                        face.setVertex(fv,v);
                        vFaces.push_back(*f);
                        if (debug_af) cout<<"merged vertex "<<vv<<" into vertex "<<v<<endl;
                    }
              }
              vvFaces.clear();

          }
          else if ( v!=vv && debug_af )
          {
              cout<<"distance^2 between vert "<<v<<" and "<<vv<<" : "<<dist2<<endl;
          }
              
          ++vtrav;
      }
      if ( pointFaceMapping[v].size()==0 )
      {
	    for ( int a=0; a<rangeDimension; a++ )
            vbbox(2*a) = vbbox(2*a+1) = xyz(v,a);
        GeometricADT<int>::iterator vit(vertexSearcher, vbbox);
        while ( !vit.isTerminal() && (*vit).data!=v ) ++vit;
        if ( (*vit).data==v ) vertexSearcher.delElement(vit);
      }

    } 
    if (nDup) 
    {
        if (debug_af) cout<<"found "<<nDup<<" duplicate vertices during front initialization"<<endl;
        // this can mess up vertex mappings in the calling code removeUnusedNodes();
    }
  nexpansions = 0;

}

//\begin{>>AdvancingFront.tex}{\subsection{~AdvancingFront}}
AdvancingFront::
~AdvancingFront()
//===========================================================================
// /Purpose: Destructor, calls destroyFront
//\end{AdvancingFront.tex}
//===========================================================================
{

  destroyFront();

}

//\begin{>>AdvancingFront.tex}{\subsection{destroyFront}}
void
AdvancingFront::
destroyFront()
//===========================================================================
// /Purpose: destroy all the data in the front and associated data structures
//\end{AdvancingFront.tex}
//===========================================================================
{
  for (vector<Face *>::iterator it=faces.begin(); it!=faces.end(); it++)
    delete *it;

  faces.clear();

  front.clear();

  xyz.redim(0);

  faceNormals.redim(0);

  for (vector<vector<int> >::iterator i=elements.begin(); i!=elements.end(); i++)
    i->clear();

  elements.clear();
  elementQuality.redim(0);

  pointFaceMapping.erase(pointFaceMapping.begin(), pointFaceMapping.end());

  nFacesFront=0;  
  nFacesTotal=0;  
  nFacesEst=0;    
  nptsTotal=0;  
  nptsEst=0;    
  nElements=0;  
  nElementsEst=0;
  
  rangeDimension=0;
  domainDimension=0;
  nFacesPerElementMax=0;
  nVerticesPerFaceMax=0;

  nexpansions=0;

  averageFaceSize=0.;
}

//\begin{>>AdvancingFront.tex}{\subsection{isFrontEmpty}}
bool
AdvancingFront::
isFrontEmpty() const
//===========================================================================
// /Purpose: Returns true if the front is empty, false otherwise
//\end{AdvancingFront.tex}
//===========================================================================
{
  return front.empty();
}

//==========================================================================================
//\begin{>>AdvancingFront.tex}{\subsection{insertFace}}
int
AdvancingFront::
//insertFace(const IntegerArray &vertexIDs, int z1, int z2)
insertFace(const ArraySimple<int> &vertexIDs, int z1, int z2)
//===========================================================================
// /Purpose: Insert a face into the front
// /vertexIDs (input) : indices for the vertices in the face
// /z1, z2 (input) : elements on either side of the face
// /
// /Comments :
// \begin{verbatim} 
//
//   X1 +  v1
//      .
//      .
//      .---> N
//      . 
//      .
//   X2 + v2
// \end{verbatim}
// X1, X2, and N are vectors
// The vertices should be ordered such that the front will grow in the correct
// direction.  This means, in 2D, that X2-X1 .cross. N sticks up out of the plane
// using the right-hand-rule.
//\end{AdvancingFront.tex}
//===========================================================================
{

  real t0 = getCPU();

  Range AXES(0,rangeDimension-1);

  // insert into faces
  int id = nFacesTotal;
  //  if ( debug_af ) cout<<"inserting face (id = "<<id<<") \n"<<(ArraySimple<int>&)vertexIDs<<endl;
  
  Face *newFace = new Face(vertexIDs, z1, z2, id);
  assert(newFace!=0);
  faces.push_back(newFace);

  AssertException (!debug_af || (id+1 == faces.size()), BookKeepingError());

  nFacesTotal++;

  ArraySimple<real> faceVertices(vertexIDs.size(0), rangeDimension);
  int a;
  for ( a=0; a<rangeDimension; a++ )
    for ( int vert=0; vert<vertexIDs.size(0); vert++ )
      faceVertices(vert,a) = xyz(vertexIDs[vert],a);

  //real newFaceSize = computeFaceSize(evaluate(xyz(vertexIDs,AXES)));
  real newFaceSize = computeFaceSize(faceVertices);

  if ( newFaceSize<100*FLT_MIN )
    {
      faces.pop_back();
      nFacesTotal--;
      delete newFace;
      return 1;
    }
      

  real minl,maxl;
  averageFaceSize = (averageFaceSize*(nFacesTotal-1) + computeAvgEdgeLength(faceVertices,minl,maxl))/nFacesTotal;

  ArraySimple<real> normal(rangeDimension);
  if ( debug_af ) cout<<"computing normal for face "<<id<<endl;
  if ( id<initialFaceSurfaceMapping.getLength(0) )
    computeFaceNormal(faceVertices,normal, initialFaceSurfaceMapping(id) );
  else
    computeFaceNormal(faceVertices,normal);

  for ( a=0; a<rangeDimension; a++ )
    faceNormals(id,a) = normal(a);

  // assign priority based on face "size":
  // "higher" priorities should correspond to smaller size so set the priority
  // to -"size". if it is a 3d triangular face, make it -1000*"size"
  // so that quadrilateral faces are generally first and ordered by thier size
  //real priority = ( newFace->getNumberOfVertices()>3 ) ? -newFaceSize : newFaceSize;//1.0;//1.0/newFaceSize;
  //PriorityQueue::iterator faceFrontLocation = front.insert(newFace, 1);
  real priority = ( newFace->getNumberOfVertices()>3 ) ? -newFaceSize : -1000*newFaceSize;//1.0;//1.0/newFaceSize;
  //real priority = -newFaceSize; 
  //if ( rangeDimension==2  ) priority=1; //priority = -newFaceSize; //priority = 1.;
  //priority = 1.;
  PriorityQueue::iterator faceFrontLocation = front.insert(newFace, priority);
  nFacesFront++;

  // insert into pointFaceMapping
  for ( int v=0; v<vertexIDs.size(0); v++ )
    pointFaceMapping[vertexIDs(v)].push_back(faceFrontLocation);

  // insert into faceSearcher
  ArraySimple<real> minmax(2*rangeDimension);

  for ( int ax=0; ax<rangeDimension; ax++ )
    {
      minmax(2*ax) = REAL_MAX;
      minmax(1+2*ax) = -REAL_MAX;
    }

  for ( int vv=0; vv<vertexIDs.size(0); vv++ )
    {
      for ( int axis=0; axis<rangeDimension; axis++ )
	{
	  minmax(2*axis) = min(minmax(2*axis),faceVertices(vv, axis));
	  minmax(1+2*axis) = max(minmax(1+2*axis),faceVertices(vv, axis));
	}
    }
  //cout<<minmax<<endl;

  try { // to add the face to the search data structure
    faceSearcher.addElement(minmax, newFace);
  }
  catch(GeometricADTError & e) {
    e.debug_print();
    cout<<"bounding box\n"<<minmax<<endl;
    throw FrontInsertionFailedError();
  }

  // check sizes
  if (nFacesTotal>=nFacesEst)
    {
      nFacesEst = nFacesTotal + 10*nFacesFront;
      resizeFaces(nFacesEst);
    }

  // add face to its elements
  if (z1 >= 0) addFaceToElement(id, z1); 
  if (z2 >= 0) addFaceToElement(id, z2);
  
  timing[int(insertion)]+=getCPU()-t0;
  return id;
}

//\begin{>>AdvancingFront.tex}{\subsection{advanceFront}}
int 
AdvancingFront::
advanceFront(int nSteps /* = 1 */ )
//===========================================================================
// /Purpose: Advance the front (ie grow the mesh)
// /nSteps (input) : attempt to grow the mesh nSteps times
//\end{AdvancingFront.tex}
//===========================================================================
{

  //            * PIdeal
  //           . .
  //          .   .
  //         .     .
  //     p1 * ----- * p2

  // suffices :  Phys refers to coordinates in the physical space
  //             Trans refers to coordinates after transformation due to mesh control parameters

  // for example: pIdealPhys is the vector representing the physical location of pIdeal
  //              pIdealTrans is the vector representing the transformed location of pIdeal

  // here is a general outline of what happens in this method:
  
  // Initialize advancement attempt
  // gather points in the face
  // compute the transformation to a space normalized by the stretching
  // Transform face point coordinates, compute the length of the "ideal" edge
  // determine pIdealPhys and pIdealTrans
  // search/gather for existing candidates in the front
  // compute candidate new points based on the candidates found in the front
  // sort all of the candidates
  // Initialize the face creation search
  // check vertices that already exist in the front
  // if there are no usable front vertices, check candidate new points
  // finalize front advancment, delete/add faces, check for bookeeping errors

  // a useful little Range
  Range AXES(0, domainDimension-1);

  int maxNumberOfSteps = (nSteps==-1) ? nSteps : parameters.getNumberOfAdvances();

  //
  // define a bunch of variables that will be reused
  //
  ArraySimple<real> T(rangeDimension, rangeDimension), Tinv(rangeDimension, rangeDimension);
  ArraySimple<real> pMidPhys(rangeDimension), pMidTrans(rangeDimension);
  ArraySimple<real> pIdealPhys(rangeDimension), pIdealTrans(rangeDimension);
  ArraySimple<real> faceNormal(rangeDimension), faceNormalTrans(rangeDimension);
  ArraySimple<real> tmp(rangeDimension), tmp1(rangeDimension); 
  
  vector<PriorityQueue::iterator> oldFrontFaces;
  vector<int> existing_candidates_adj, existing_candidates_neighb, existing_candidates, local_nodes;
  IntegerArray newFace(nVerticesPerFaceMax);

  // try to advance the front nSteps, stop if the front gets empty
  //   exceptions could be thrown and should be caught be the calling scope
  int step = 0;
  real originalAngleTol = parameters.getMaxNeighborAngle();
  real originalAuxAngle = parameters.getAuxiliaryAngleTolerance();
  real hfac = 1.0;
  bool pulledBack = false;

  bool struggledAtEnd = false;

  bool finished = (maxNumberOfSteps==-1) ? isFrontEmpty() :  ! ( step<maxNumberOfSteps && !isFrontEmpty() );

  real maxQ, minQ;

  maxQ = -REAL_MAX;
  minQ = REAL_MAX;

  while ( !finished )
    {
      // Initialize advancement attempt for this step

      int tries = 0;

      // determine the new element id
      int newElementID = nElements;

      test_face = newElementID;

      // // create an iterator for the next face to advance
      PriorityQueue::iterator next = front.begin();

      // set the advancement termination variable to false
      bool advanced = false;

      // create a list of the faces to delete from the front (and reserve some space)
      oldFrontFaces.clear();
      oldFrontFaces.reserve(4);

      // create a list for the existing candidates in the front, reserve some space
      existing_candidates_adj.clear();
      existing_candidates_adj.reserve(10);
      existing_candidates_neighb.clear();
      existing_candidates_neighb.reserve(10);
      existing_candidates.clear();
      existing_candidates.reserve(10);
      local_nodes.clear();
      local_nodes.reserve(10);

      // newFace will contain the vertex indices for the new face
      newFace = -1;

      // diagnostic counter on the number of advancement attempts
      int adv = 0;

      // now try to advance the front for this step
      while (!advanced && next!=front.end() && adv<=nFacesFront)
	{
	  
	  real t0 = getCPU();

	  //
	  // initialize the temporaries
	  //
	  int a, a1;
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      pMidPhys(a) = pMidTrans(a) = pIdealPhys(a) = pIdealTrans(a) = tmp(a) = 0.0;
	      for ( a1=0; a1<rangeDimension; a1++ )
		T(a,a1) = Tinv(a,a1) = 0.0;
	    }

	  Face & currentFace = **next;

	  ++adv;

	  if ( debug_af ) cout<<"advancement attempt "<<adv<<" using face "<<currentFace.getID()<<endl;
	  
	  //	  cout<<"advancement attempt "<<adv<<" using face "<<currentFace.getID()<<endl;


	  // gather vertices in currentFace
	  int nVertsCurrentFace = currentFace.getNumberOfVertices();

// 	  cout<<"current face vertices : ";
// 	  for ( int nvcf=0; nvcf<nVertsCurrentFace; nvcf++ )
// 	    cout<<currentFace.getVertex(nvcf)<<" ";
// 	  cout<<endl;

	  ArraySimple<real> currentFaceVerticesPhys(nVertsCurrentFace,rangeDimension), 
	    currentFaceVerticesTrans(nVertsCurrentFace,rangeDimension);

	  for ( a=0; a<rangeDimension; a++ ) 
	    {
	      pMidPhys(a) = 0.;
	      faceNormal[a] = faceNormals(currentFace.getID(),a);
	    }

	  //cout<<"face normal\n "<<faceNormal<<endl;
	  for ( int vfi=0; vfi<nVertsCurrentFace; vfi++ )
	    {
	      int pf = currentFace.getVertex(vfi);
	      for ( int a=0; a<rangeDimension; a++ )
		{
		  currentFaceVerticesPhys(vfi,a) = xyz(pf,a);
		  pMidPhys(a) += xyz(pf,a)/real(nVertsCurrentFace);
		}
	    }

	  real t01 = getCPU();
	  computeFaceTransformation(currentFace, T, Tinv);
	  timing[faceTrans] += getCPU()-t01;
	  
	  if ( debug_af )
	    {
	      cout<<" **** T ****\n"<<T<<endl;
	      cout<<" **** Tinv ****\n"<<Tinv<<endl;
	    }

	  // get the face middle vertex in the transformed space
	  matVectMult(T, pMidPhys, pMidTrans);

	  // Transform face point coordinates, compute Delta1, the distance from the face center to the ideal point

	  for ( int v=0; v<nVertsCurrentFace; v++ )
	    {
	      for ( a=0; a<rangeDimension; a++ )
		tmp(a) = currentFaceVerticesPhys(v,a);

	      matVectMult(T, tmp, tmp1);

	      for ( a=0; a<rangeDimension; a++ )
		currentFaceVerticesTrans(v,a) = tmp1(a);
	    }

	  // get the "size" of the face in physical space
	  //real sizeFacePhys = computeFaceSize(currentFaceVerticesPhys);
	  real maxl,minl, maxlt,minlt;
	  real sizeFacePhys = computeAvgEdgeLength(currentFaceVerticesPhys, minl, maxl);
	  // get the "size" of the face in the transformed space 
	  real sizeFaceTrans = computeAvgEdgeLength(currentFaceVerticesTrans,minlt,maxlt);

	  // now determine how far the front should grow normal to the current face (in the normalized space)
	  double growthEdgeLengthTrans = hfac*computeNormalizedGrowthDistance(sizeFacePhys, sizeFaceTrans);

	  if (debug_af) cout<<"GROWTH FACTOR "<<sizeFacePhys<<" "<<sizeFaceTrans<<" "<<computeNormalizedGrowthDistance(sizeFacePhys, sizeFaceTrans)<<endl;
	  if ( nVertsCurrentFace==4 ) growthEdgeLengthTrans = (sqrt(3.)/2.)*max(sizeFaceTrans,growthEdgeLengthTrans);

	  double growthDistanceTrans = sqrt(growthEdgeLengthTrans*growthEdgeLengthTrans - sizeFaceTrans*sizeFaceTrans/4.0);
	  if ( nVertsCurrentFace==4 ) growthDistanceTrans = max(growthEdgeLengthTrans,.5);
 
	  // determine pIdealPhys and pIdealTrans

	  if ( rangeDimension==domainDimension )
	    computeFaceNormal(currentFaceVerticesTrans, faceNormalTrans);
	  else // stretch the normal using the transformation
	    {
	      matVectMult(T,faceNormal, faceNormalTrans);
	      real fntmag = sqrt(ASmag2(faceNormalTrans));
	      for ( a=0; a<rangeDimension; a++ ) faceNormalTrans[a] /= fntmag;
	      
	    }

	  real transcale = 1.;
	  //if ( currentFace.getNumberOfVertices()==4 ) transcale = .5;

	  for ( a=0; a<rangeDimension; a++ )
	    pIdealTrans(a) = pMidTrans(a) + growthDistanceTrans*faceNormalTrans(a)*transcale;
	  
	  //	    pIdealTrans(a) = pMidTrans(a) + growthDistanceTrans*faceNormalTrans(a)/real(rangeDimension-1);

	  //if ( rangeDimension==3 && nVertsCurrentFace!=4) idealVertexIteration(currentFaceVerticesTrans, pIdealTrans,growthEdgeLengthTrans);

	  matVectMult(Tinv, pIdealTrans, pIdealPhys);


	  real t1 = getCPU();
	  timing[int(initializeAdv)] += t1-t0;

	  // search/gather for existing candidates in the front
	  existing_candidates_adj.clear();
	  existing_candidates_neighb.clear();
	  existing_candidates.clear();
	  local_nodes.clear();
	  gatherExistingCandidates(currentFace, growthEdgeLengthTrans, pIdealTrans, pIdealPhys, 
				   T, Tinv, existing_candidates_adj, existing_candidates_neighb,
				   local_nodes);

	  vector<int>::iterator exa, exn;

#if 1
	  for ( exn=existing_candidates_neighb.begin(); 
		exn!=existing_candidates_neighb.end(); exn++ )
	    existing_candidates.push_back(*exn);

	  for ( exa=existing_candidates_adj.begin(); exa!=existing_candidates_adj.end(); exa++ )
	    existing_candidates.push_back(*exa);
#endif

	  //cout<<"number of existing candidates "<<existing_candidates.size()<<endl;

	  real t2 = getCPU();
	  timing[int(findExistingCandidates)] += t2-t1;

	  // compute candidate new points based on the candidates found in the front
	  // then sort all the candidates

	  if ( debug_af )
	    {
	      cout<<"===== existing candidates: \n";
	      for ( int ex=0; ex<existing_candidates.size(); ex++ ) {
		cout<<"candidate : "<<existing_candidates[ex]<<" faces are : ";
		for ( vector<PriorityQueue::iterator>::iterator fi = pointFaceMapping[existing_candidates[ex]].begin(); 
		      fi != pointFaceMapping[existing_candidates[ex]].end(); fi++ )
		  cout<<(**fi)->getID()<<" ";
		cout<<"---"<<endl;
	      }
	      cout<<endl;
	    }


#if 0
	  //cout<<"existing candidates "<<existing_candidates_adj.size()<<" "<<existing_candidates_neighb.size()<<endl;
 	  ArraySimple<real> new_candidatesTrans, new_candidatesPhys;
 	  computeVertexCandidates(currentFaceVerticesTrans, pIdealTrans, 
				  existing_candidates_neighb, T, new_candidatesTrans, growthEdgeLengthTrans);

 	  ArraySimple<real> new_candidatesTrans_adj, new_candidatesPhys_adj;
 	  computeVertexCandidates(currentFaceVerticesTrans, pIdealTrans, 
				  existing_candidates_adj, T, new_candidatesTrans_adj, growthEdgeLengthTrans);

	  //	  vector<int>::iterator exa, exn;
	  for ( exa=existing_candidates_adj.begin(); exa!=existing_candidates_adj.end(); exa++ )
	    existing_candidates.push_back(*exa);

	  for ( exn=existing_candidates_neighb.begin(); 
		exn!=existing_candidates_neighb.end(); exn++ )
	    existing_candidates.push_back(*exn);
#else
	  ArraySimple<real> new_candidatesTrans, new_candidatesPhys;
 	  computeVertexCandidates(currentFaceVerticesTrans, pIdealTrans, 
				  existing_candidates, T, new_candidatesTrans, growthEdgeLengthTrans);
#endif

	  // now get the new candidate locations in the physical space
	  //new_candidatesPhys.redim(new_candidatesTrans.getLength(0), domainDimension);
	  new_candidatesPhys.resize(new_candidatesTrans.size(0), rangeDimension);

	  for ( int vc=0; vc<new_candidatesTrans.size(0); vc++ )
	    {
	      for ( a=0; a<rangeDimension; a++ )
		tmp(a) = new_candidatesTrans(vc,a);

	      matVectMult(Tinv,tmp, tmp1);

	      for ( a=0; a<rangeDimension; a++ )
		new_candidatesPhys(vc,a) = tmp1(a);
	    }

	  // now project the new candidates if we are generating a surface mesh
	  if ( rangeDimension==3 && domainDimension==2 )
	    {
	      //cout<<"new candidates before projection"<<endl;
	      //cout<<new_candidatesPhys<<endl;
	      
	      // now project the new vertices onto the underlying surface
	      realArray xToProject(new_candidatesPhys.size(0),rangeDimension);
	      
	      int cand;
	      for ( cand=0; cand<new_candidatesPhys.size(0); cand++ )
		for ( a=0; a<rangeDimension; a++ )
		  xToProject(cand,a) = new_candidatesPhys(cand,a);
	      
	      MappingProjectionParameters mp;// = defaultMappingProjectionParameters;
	      mp.getRealArray(MappingProjectionParameters::r).redim(new_candidatesPhys.size(0),rangeDimension);
	      mp.getRealArray(MappingProjectionParameters::r) = -1;
	      mp.getRealArray(MappingProjectionParameters::x) = xToProject;

	      int subsurf = -1;
	      if ( currentFace.getID()<initialFaceSurfaceMapping.getLength(0) )
		subsurf = initialFaceSurfaceMapping(currentFace.getID());

	      if ( debug_af) cout<<"subSurf is "<<subsurf<<"  face "<<currentFace.getID()<<"  "<<initialFaceSurfaceMapping.getLength(0)<<endl;
	      CompositeSurface &cs = (*(CompositeSurface *)backgroundMappings.mappingList[0].mapPointer);
	      
	      if ( subsurf!=-1 )
		cs[subsurf].project(xToProject,mp);
	      else
		cs.project(xToProject,mp);

	      //	      if ( mp.isAMarchingAlgorithm() ) cout<<"marching true"<<endl;

	      for ( cand=0; cand<new_candidatesPhys.size(0); cand++ )
		for ( a=0; a<rangeDimension; a++ )
		  new_candidatesPhys(cand,a) = xToProject(cand,a);
	      
	      //cout<<"new_candidates after projection"<<endl;
	      //cout<<new_candidatesPhys<<endl;
	    }


	  real t3 = getCPU();
	  timing[int(computeNew)] += t3-t2;

	  //if ( debug_af ) new_candidatesPhys.display("new vertex candidates");
	  if ( debug_af ) cout<<"new vertex candidates \n"<<new_candidatesPhys<<endl;
	  // cout<<"new vertex candidates \n"<<new_candidatesPhys<<endl;

	  if ( debug_af )
	    {
	      cout<<"===== existing candidates after sort: \n";
	      for ( int ex=0; ex<existing_candidates.size(); ex++ ) {
		cout<<"candidate : "<<existing_candidates[ex]<<" faces are : ";
		for ( vector<PriorityQueue::iterator>::iterator fi = pointFaceMapping[existing_candidates[ex]].begin(); 
		      fi != pointFaceMapping[existing_candidates[ex]].end(); fi++ )
		  cout<<(**fi)->getID()<<" ";
		cout<<"---"<<endl;
	      }
	      cout<<endl;
	    }

	  // Initialize the face creation search
	  bool madeAConsistentElement = false;
	  oldFrontFaces.clear();
	  oldFrontFaces.push_back(next);

#if 0
	  // if the front is growing off of a non-triangular face in 3d, try to make some other element first
	  //	  cout<<"nv current face "<<nVertsCurrentFace<<endl;
	  if ( nVertsCurrentFace > 3 )
	    madeAConsistentElement = makePrismPyramidHex(currentFace, newElementID, pIdealPhys, 
							 T, existing_candidates, oldFrontFaces);
#endif

	  real t32;
	  // check vertices that already exist in the front
	  if ( rangeDimension==domainDimension )
	    {
	      if ( !madeAConsistentElement )
		madeAConsistentElement = 
		  makeTriTetFromExistingVertices(currentFace, newElementID, existing_candidates, local_nodes, oldFrontFaces);
	      
	      real t31 = getCPU();
	      timing[int(creationInsertion_1)] += t31-t3;
	      
	      if ( debug_af && madeAConsistentElement ) cout<<"-->made an element with an adjacent face"<<endl;
	      
	      // if there are no usable front vertices, check candidate new points
	      if ( !madeAConsistentElement )
		{
		  madeAConsistentElement = makeTriTetFromNewVertex(currentFace, newElementID, 
								   new_candidatesPhys, local_nodes);
		  if ( debug_af && madeAConsistentElement ) cout<<"-->made an element with a new vertex"<<endl;
		}
	      
	      // 	  if ( !madeAConsistentElement )
	      // 	    {
	      // 	      madeAConsistentElement = 
	      // 		makeTriTetFromExistingVertices(currentFace, newElementID, existing_candidates_neighb, oldFrontFaces);
	      // 	      if ( debug_af && madeAConsistentElement ) cout<<"-->made an element with a neighboring face"<<endl;
	      // 	    }
	      
	      t32 = getCPU();
	      timing[int(creationInsertion_2)] += t32-t31;
	    }
	  else
	    {
	      madeAConsistentElement = makeTriOnSurface(currentFace,newElementID, 
							existing_candidates,new_candidatesPhys,oldFrontFaces);
	      t32 = getCPU();
	    }


	  // finalize front advancement, delete/add faces, check for bookeeping errors
	  if (madeAConsistentElement)
	    {
	      if (debug_af) cout<<"created a new element"<<endl;
	      advanced = true;
	      // remove old front faces, adding each to the appropriate element
	      for (int df =0; df<oldFrontFaces.size(); df++) 
		{
		  Face & fremove = **(oldFrontFaces[df]);
		  // first add fremove to the correct element
		  if(fremove.getZ2ID()<0)
		    { // add fremove to newElementID
		      fremove.setZ2ID(newElementID);
		      addFaceToElement(fremove.getID(), newElementID);
		    }
		  else
		    { // something probably went wrong when the initial element list was created
		      cout<<"ERROR : the face already has a second element!"<<endl;
		      throw BookKeepingError();
		    }
		  // now remove fremove from the front
		  if ( debug_af ) cout<<"removing face "<<(*oldFrontFaces[df])->getID()<<endl;
		  removeFaceFromFront(oldFrontFaces[df]);
		}
	      
	      // compute the quality of the new element, used later for improvement
	      real tq0 = getCPU();
	      elementQuality(newElementID) = computeElementQuality(newElementID);
	      timing[elemQual]+=getCPU()-tq0;
	      
	      maxQ = max(maxQ, elementQuality(newElementID));
	      minQ = min(minQ, elementQuality(newElementID));

	    }
	  else {
	    // the front could not be advanced with this face,
	    //   increment next and reset the advancement,
	    //   return to the top to try the next face
	    //next++;
	    if ( adv!=nFacesFront )
		{
		  PriorityBatchQueue<Face *>::iterator repiter = next;
		  
		  list<int> slot;
		  for ( int v=0; v<(*repiter)->getNumberOfVertices(); v++ )
		    {
		      vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[(*repiter)->getVertex(v)];
		      bool found = false;
		      int i_slot=0;
		      for (vector<PriorityQueue::iterator>::iterator f=v1Faces.begin();f!=v1Faces.end() && !found;f++,i_slot++)
			{
			  if ((**f)->getID() == (*repiter)->getID()) 
			    {
			      //v1Faces.erase(f);
			      //			      *f = repiter;
			      slot.push_back(i_slot);
			      found = true;
			    }
			}
		      AssertException(found, AdvancingFrontError());
			
		    }
		  
		  next = front.reprioritize(repiter, -10*fabs(front.minPriority()));

		  // adjust the reference to repiter in pointFaceMapping, 
		  list<int>::iterator i_slot = slot.begin();
		  for ( int v=0; v<(*repiter)->getNumberOfVertices(); v++ )
		    {
		      vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[(*repiter)->getVertex(v)];
		      v1Faces[*i_slot] = repiter;
		      i_slot++;
		    }
		  slot.clear();
		}
	    advanced = false;
	    tries++;
	    oldFrontFaces.clear();
	  }
	  real t4 = getCPU();
	  timing[int(creationInsertion_3)] += t4-t32;
	  timing[int(creationInsertion)]+= t4-t3;
	  
	  timing[int(totalTime)] += t4-t0;

	  //cout<<nElements<<"  "<<nFacesFront<<"  "<<t4-t0<<"  "<<timing[int(totalTime)]<<endl;
	} // while(!advanced...)
      
      // if we got here but could not advance the front there was some horrible error somewhere
      if (!advanced && (!isFrontEmpty())) {// || adv>=nFacesFront)) {

	struggledAtEnd = true;
	if ( nexpansions<9 || (!pulledBack) )
	  {
	    
	    //pulledBack = true;
	    int npullb = nexpansions+1;
	    cout<<"WARNING : expanding front, expanding  "<<npullb<<" times"<<endl;
	    nexpansions = 0;
	    
	    pulledBack = true;
	    // kkc switched !pulledBack to pulledBack everywhere in the next for loop
	    for ( int p=0; p<npullb && pulledBack; p++ )
	      {
		pulledBack = (expandFront());
		if ( pulledBack ) nexpansions++;
	      }
	    next = front.begin();
	  }
	else 
	  {
	    nexpansions = 0;
	    //	    if ( parameters.getMaxNeighborAngle()>89.5 )
	      if ( hfac>0.01 )
		{
		  hfac*=.5;
		  cout<<"WARNING : setting hfac to "<<hfac<<endl;
		  next = front.begin();
		}
	      else
		{
		  parameters.setMaxNeighborAngle(originalAngleTol);
		  parameters.setAuxiliaryAngleTolerance(originalAuxAngle);
		  cout<<"nFacesFront "<<nFacesFront<<"   "<<front.size()<<endl;
		  throw AdvanceFailedError();
		}
#if 0
	    else
	      {
		//parameters.setMaxNeighborAngle(parameters.getMaxNeighborAngle()*2.);
		parameters.setMaxNeighborAngle(90.);
		parameters.setAuxiliaryAngleTolerance(0.);
		cout<<"WARNING : reducing angle tolerance to "<<parameters.getMaxNeighborAngle()<<endl;
		adv = 0;
		//npullb = 0;
		pulledBack = false;
		next = front.begin();
	      }
#endif
	  }
      }
      else if ( advanced && (parameters.getMaxNeighborAngle() != originalAngleTol || hfac<1.) )
	{
	  hfac = 1;
	  parameters.setMaxNeighborAngle(originalAngleTol);
	  parameters.setAuxiliaryAngleTolerance(originalAuxAngle);
	  pulledBack = false;
	  //npullb = 0;

	  adv=0;
	  cout<<"resetting angle tolerance to "<<parameters.getMaxNeighborAngle()<<endl;
	  next = front.begin();
	}
      else
	{
	  //else
	  //cout<<"number of tries "<<tries<<endl;
	  adv=0;
	  hfac = 1.;
	  pulledBack = false;
	  //npullb = 0;
	  nElements++;
	  // resize the element list if neccessary
	  if (nElements==nElementsEst) 
	    {
	      nElementsEst = 2*nFacesEst;
	      elements.reserve(nElementsEst);
	      elementQuality.resize(nElementsEst);
	    }
	  
	  step++;
	  finished = (maxNumberOfSteps==-1) ? isFrontEmpty() :  ! ( step<maxNumberOfSteps && !isFrontEmpty() );
	}
    } // for ( nSteps )

  cout<<"*********** timings *************"<<endl;
  cout<<"total time advancing   = "<<timing[totalTime]<<endl;
  cout<<"initializeAdv          = "<<timing[initializeAdv]<<endl;
  cout<<"findExistingCandidates = "<<timing[findExistingCandidates]<<endl;
  cout<<"existingInit           = "<<timing[existingInit]<<endl;
  cout<<"existingInCircle       = "<<timing[existingInCircle]<<endl;
  cout<<"existingInCircleInit   = "<<timing[existingInCircleInit]<<endl;
  cout<<"existingInCircle_1     = "<<timing[existingInCircle_1]<<endl;
  cout<<"existingInCircle_2     = "<<timing[existingInCircle_2]<<endl;
  cout<<"existingInCircle_3     = "<<timing[existingInCircle_3]<<endl;
  cout<<"existingInCircle_4     = "<<timing[existingInCircle_4]<<endl;
  cout<<"existingInCircle_5     = "<<timing[existingInCircle_5]<<endl;
  cout<<"existingInCircle_trav  = "<<timing[existingInCircle_trav]<<endl;
  cout<<"existingAdj            = "<<timing[existingAdj]<<endl;
  cout<<"computeNew             = "<<timing[computeNew]<<endl;
  cout<<"creationInsersion      = "<<timing[creationInsertion]<<endl;
  cout<<"creationInsersion_1    = "<<timing[creationInsertion_1]<<endl;
  cout<<"creationInsersion_2    = "<<timing[creationInsertion_2]<<endl;
  cout<<"creationInsersion_20   = "<<timing[creationInsertion_20]<<endl;
  cout<<"creationInsersion_21   = "<<timing[creationInsertion_21]<<endl;
  cout<<"creationInsersion_3    = "<<timing[creationInsertion_3]<<endl;
  //  cout<<"intersections          = "<<timing[intersections]<<" "<<timing[intersections]/nFacesFront<<" "<<timing[intersections]/(nFacesFront*nFacesFront)<<endl;
  cout<<"intersections          = "<<timing[intersections]<<endl;
  cout<<"insertion              = "<<timing[insertion]<<endl;
  cout<<"matvec                 = "<<matvecTiming<<endl;
  cout<<"getCircleCent          = "<<timing[getCircleCent]<<endl;
  cout<<"faceNormalTiming       = "<<faceNormalTiming<<endl;
  cout<<"faceTrans              = "<<timing[faceTrans]<<endl;
  cout<<"faceTrans_1            = "<<timing[faceTrans_1]<<endl;
  cout<<"faceTrans_2            = "<<timing[faceTrans_2]<<endl;
  cout<<"faceTrans_3            = "<<timing[faceTrans_3]<<endl;
  cout<<"faceTrans_4            = "<<timing[faceTrans_4]<<endl;
  cout<<"elemQual               = "<<timing[elemQual]<<endl;
  cout<<"*********************************"<<endl;
  cout<<"nFacesFront "<<nFacesFront<<endl;
  cout<<"elementQuality min, max "<<minQ<<" "<<maxQ<<endl;

  nexpansions = 0;
  int fin = 0;
  if ( nFacesFront==0 )
    {
      improveQuality();
      if (nFacesFront)
	fin = 1;
    }
  else
    fin = struggledAtEnd ? 1 : 0;

  return fin;
}

//\begin{>>AdvancingFront.tex}{\subsection{gatherExistingCandidates}}
void
AdvancingFront::
gatherExistingCandidates(const Face & face, real distance, ArraySimple<real> & pIdealTrans, 
			 ArraySimple<real> & pIdealPhys,
			 ArraySimple<real> & T, ArraySimple<real> & Tinv, 
			 vector<int> &existing_candidates, vector<int> &existing_candidates_neighb,
			 vector<int> &local_nodes)
//===========================================================================
// /Purpose: find vertices in the front that could make a an element with face
// /face (input) : the face attempting to grow
// /distance (input) : the distance the face wants to grow
// /pIdealTrans (input) : the location of the "ideal" vertex in the normalized space
// /T (input) : the stretching transformation
// /Tinv (input) : the inverse of T
// /existing_candidates (output) : the list of candidates already in the front
//\end{AdvancingFront.tex}
//===========================================================================
{
  // // // using the new point and the boundingBox defined by the transformation:
  // // //     gather existing candidates encompassed by the boundingBox
  // // // transform the coordinates of the existing candidate points, filtering out those 
  // // //     that are not in the circle of consideration in the transformed space

  // // // finally, add any vertices from adjacent front faces having normals within a specified
  // // //     angle of the current face's normal

  real t0 = getCPU();
  ArraySimpleFixed<real,3,1,1,1> pp[4];
  
  int a,v;

  distance *= parameters.getDiscardDistFactor();

  for ( v=0; v<face.getNumberOfVertices(); v++ )
    for ( a=0; a<rangeDimension; a++ )
      pp[v][a] = xyz(face.getVertex(v),a);

  ArraySimple<real> faceNormal(rangeDimension);

  for ( a=0; a<rangeDimension; a++ )
    faceNormal[a] = faceNormals(face.getID(), a);

  real t1 = getCPU();
  timing[existingInit] += t1-t0;

  ArraySimple<real> candPhys(rangeDimension), candTrans(rangeDimension);//, aFaceVertex(rangeDimension);

  vector<int> discarded;
  discarded.reserve(10);

  //
  // now gather vertices on adjacent faces not within the search circle ( consider these also
  // to help prevent slivers... )
  real ang = parameters.getMaxNeighborAngle();
  //real adjacentFaceAngleTolerance = acos(-1.) * ( 180. - ang )/180.;
  real cosang = cos(M_PI*(1.-ang/180.));

#if 1
  //if ( domainDimension==2)
  for ( v=0; v<face.getNumberOfVertices(); v++ )
    {

      vector<PriorityQueue::iterator> & vFaces = pointFaceMapping[face.getVertex(v)];

      // search each adjacent face for vertices not already gathered
      ArraySimple<real> candNormal(rangeDimension);

      for ( vector<PriorityQueue::iterator>::iterator f=vFaces.begin(); f!=vFaces.end(); f++ )
	{
	  Face & fCand = ***f;
	  for ( a=0; a<rangeDimension; a++ )
	    candNormal[a] = faceNormals(fCand.getID(), a);

	  real dotp = ASdot(faceNormal, candNormal)/sqrt(ASmag2(faceNormal)*ASmag2(candNormal));

 	  if ( debug_af ) cout<<"faceNormal\n"<<faceNormal<<"\ncandNormal\n"<<candNormal<<endl;
	  if ( debug_af ) cout<<dotp<<" "<<cosang<<endl;

	  bool angleOK = dotp<cosang;
	  if ( angleOK && domainDimension==3 )
	    {
	      // only consider vertices on this face if it shares 2 vertices with the currentFace
	      int vf;
	      for ( vf=0; vf<fCand.getNumberOfVertices(); vf++ )
		if ( fCand.getVertex(vf)==face.getVertex(v) ) break;

	      angleOK = ( fCand.getVertex((vf+1)%fCand.getNumberOfVertices()) == 
			  face.getVertex((v+face.getNumberOfVertices()-1)%face.getNumberOfVertices()) ) ||
		( fCand.getVertex((vf+fCand.getNumberOfVertices()-1)%fCand.getNumberOfVertices()) == 
		  face.getVertex((v+1)%face.getNumberOfVertices()) );

	    }

	  for ( int vf=0; vf<fCand.getNumberOfVertices(); vf++ )
	    {
	      //    1. it is not in the growth face "face"
	      bool isInFace = false;
	      int vface = 0;
	      while ( !isInFace && vface<face.getNumberOfVertices() )
		isInFace = (face.getVertex(vface++) == fCand.getVertex(vf));

	      if ( !isInFace )
		{
		  //    2. it has not already been identified 
		  bool alreadyFound = false;
		  for ( vector<int>::iterator exvert=existing_candidates.begin(); 
			exvert!=existing_candidates.end() && !alreadyFound; exvert++ )
		    if ( (*exvert)==fCand.getVertex(vf) ) 
		      {
			alreadyFound = true;
			//existing_candidates_neighb.erase(exvert);
			//break;
			//exvert = existing_candidates_neighb.begin();
			//if ( !angleOK ) existing_candidates.erase(exvert);
		      }

#if 0
		  for ( vector<int>::iterator exvertn=existing_candidates_neighb.begin(); 
			exvertn!=existing_candidates_neighb.end() && !alreadyFound; exvertn++ )
		    if ( (*exvertn)==fCand.getVertex(vf) ) 
		      
		      {
			alreadyFound = true;
			//existing_candidates_neighb.erase(exvertn);
			//existing_candidates.push_back(*exvertn);
			//break;
		      }
#endif

		  ArraySimple<real> candPhys(rangeDimension);
		  for ( a=0; a<rangeDimension; a++ )
		    candPhys(a) = xyz(fCand.getVertex(vf), a);

		  matVectMult(T,candPhys, candTrans);

		  real sum=0.0;
		  real asum = 0.0;
		  //bool correctDir = true;
		  for ( a=0; a<rangeDimension; a++ )
		    {
		      //		      candPhys(a) = xyz(candidateVertex,a);
		      //aFaceVertex(a) = xyz(face.getVertex(0),a);
		      sum += faceNormal[a] * ( candPhys[a] - xyz(face.getVertex(0),a) );
		      asum += (candTrans(a)-pIdealTrans(a))*(candTrans(a)-pIdealTrans(a));
		    }

		  bool closeEnough = true;
		  //closeEnough = asum<(1.5*1.5*distance*distance);

		  //if (sum>100*REAL_EPSILON) correctDir = true;
		  bool correctDir = sum>100*FLT_MIN;
		  if ( rangeDimension==2 )
		    correctDir = orient2d(pp[0].ptr(),pp[1].ptr(),candPhys.ptr())>0;//sum>100*FLT_MIN;
		  else if ( domainDimension==3 )
		    {
		      correctDir = orient3d(pp[0].ptr(), pp[1].ptr(), pp[2].ptr(), candPhys.ptr())<0;
		      if ( face.getNumberOfVertices()==4 )
			correctDir = correctDir && orient3d(pp[1].ptr(), pp[2].ptr(), pp[3].ptr(), candPhys.ptr())<0;
		      
		    }
		  else 
		    correctDir = sum>100.*REAL_EPSILON && angleOK; // XXX ! not robust!

		  if ( debug_af ) cout<<"correct dir sum1 for vertex "<<fCand.getVertex(vf)<<" "<<sum<<endl;

		  //cout<<"sum "<<sum<<endl;

		  // add only if it has not already been found and it would cause the front
		  // to grow in the correct direction
		  //bool vertexIsOnFacePlane = isOnFacePlane(face, candPhys);
		  if ( angleOK && (!alreadyFound) && closeEnough &&
		       //(!vertexIsOnFacePlane) &&
		       correctDir )
		    //checkVertexDirection(face, candPhys) ) 
		    {
		      if ( debug_af ) cout<<"inserting adj vertex "<<fCand.getVertex(vf)<<endl;
		      //cout<<"inserting adj vertex "<<fCand.getVertex(vf)<<" with orientation "<<orient3d(pp[0].ptr(), pp[1].ptr(), pp[2].ptr(), candPhys.ptr())<<endl;
		      //cout<<"here's the bloody face \n"<<candPhys<<endl;
		      existing_candidates.push_back(fCand.getVertex(vf));
		      local_nodes.push_back(fCand.getVertex(vf));
		    }
		  else
		    {
		      // keep track of this vertex as discarded
		      discarded.push_back( fCand.getVertex(vf) );
		    }
		    
		} // if !isInFace
	    } // for int vf...
	} // for interator f
    } // for int v
#endif

  // compute the bounding box for the face search traversal of the GeometricADT
  ArraySimple<real> boundingBox(2*rangeDimension);

  ArraySimple<real> bboxMin(3), bboxMax(3), tmp(3);
  //ArraySimpleFixed<real,3> bboxMin,bboxMax;

#if 0

  real fact = sqrt(.5)*real(rangeDimension);
  for ( a=0; a<rangeDimension; a++ )
    {
      bboxMin(a) = pIdealTrans(a)-fact*distance;
      bboxMax(a) = pIdealTrans(a)+fact*distance;
    }

  tmp = bboxMin;
  matVectMult(Tinv, tmp, bboxMin);
  tmp = bboxMax;
  matVectMult(Tinv, tmp, bboxMax);
  
  for ( a=0; a<rangeDimension; a++ )
    {
      boundingBox(0,a) = min(bboxMin(a),bboxMax(a));
      boundingBox(1,a) = max(bboxMin(a),bboxMax(a));
    }
#else
  real radmax = 0;
  real radmin = REAL_MAX;
  real rad = 0;
  int vv;
  for ( v=0; v<face.getNumberOfVertices(); v++ )
    {
      rad = 0;
      vv = face.getVertex(v);
      for ( a=0; a<rangeDimension; a++ )
	rad += (pIdealPhys(a)-xyz(vv,a))*(pIdealPhys(a)-xyz(vv,a));
      radmax = max(radmax, rad);
      radmin = min(radmin,rad);
    }

  for ( v=0; v<existing_candidates.size(); v++ )
    {
      rad = 0;
      vv = existing_candidates[v];
      for ( a=0; a<rangeDimension; a++ )
	rad += (pIdealPhys(a)-xyz(vv,a))*(pIdealPhys(a)-xyz(vv,a));
      radmax = max(radmax, rad);
      radmin = min(radmin,rad);
    }

  radmax = parameters.getSearchDistFactor()*sqrt(radmax);
  radmin = parameters.getSearchDistFactor()*sqrt(radmin);
  //  radmax = 2*sqrt(radmax);
  //  radmin = 2*sqrt(radmin);
  for ( a=0; a<rangeDimension; a++ )
    {
      boundingBox(2*a) = pIdealPhys(a)-radmax;
      boundingBox(1+2*a) = pIdealPhys(a)+radmax;
    }
#endif
  // create a traversor of faceSearcher that will search the boundingBox

  GeometricADT<int>::traversor vtrav(vertexSearcher, boundingBox);

  timing[existingInCircleInit]+= getCPU()-t1;
  bool isInFace,correctDirection,closeEnough;
  int vface;
  while ( !vtrav.isFinished() )
    {
      real t01 = getCPU();
      int candidateVertex = (*vtrav).data;

      isInFace = false;
#if 1
      vface = 0;
      while ( !isInFace && vface<face.getNumberOfVertices() )
	isInFace = (face.getVertex(vface++) == candidateVertex);
#endif

      bool discardedVert = false;
#if 0
      for ( vector<int>::iterator vd = discarded.begin(); !isInFace && !discardedVert && vd!=discarded.end(); vd++ )
	discardedVert = (*vd==candidateVertex);
#endif

      real t02 = getCPU();
      timing[existingInCircle_1]+=t02-t01;

      if ( !isInFace && !discardedVert )
	{
	  real t03 = getCPU();
	  
	  bool alreadyFound = false;
	  for ( vector<int>::iterator exvert=existing_candidates.begin(); 
		exvert!=existing_candidates.end() && !alreadyFound; exvert++ )
	    if ( (*exvert)==candidateVertex ) 
	      {
		alreadyFound = true;
		//existing_candidates_neighb.erase(exvert);
		//break;
		//exvert = existing_candidates_neighb.begin();
		//if ( !angleOK ) existing_candidates.erase(exvert);
	      }

	  real sum=0.0;
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      candPhys(a) = xyz(candidateVertex,a);
	      //aFaceVertex(a) = xyz(face.getVertex(0),a);
	      sum += faceNormal[a] * ( candPhys[a] - xyz(face.getVertex(0),a) );
	    }

	  //    3. it will permit the front to grow in the correct direction
	  //correctDirection = sum>100*FLT_MIN;//::checkVertexDirection(faceNormal, aFaceVertex, candPhys);

	  if ( rangeDimension==2 )
	    correctDirection = orient2d(pp[0].ptr(),pp[1].ptr(),candPhys.ptr())>0;
	  else if ( rangeDimension==3 && domainDimension==3 )
	    {
	      correctDirection = orient3d(pp[0].ptr(), pp[1].ptr(), pp[2].ptr(), candPhys.ptr())<0;
	      if ( face.getNumberOfVertices()==4 )
		correctDirection = correctDirection && orient3d(pp[1].ptr(), pp[2].ptr(), pp[3].ptr(), candPhys.ptr())<0.;
	      //correctDirection = correctDirection && orient3d(pp[1].ptr(), pp[2].ptr(), pp[3].ptr(), candPhys.ptr())<-1e-30;
	    }
	  else
	    correctDirection=sum>100*REAL_EPSILON;
	    //	    correctDirection=true; // XXX should project onto a plane and then check direction ?

	  if ( debug_af ) cout<<"correct dir sum for vertex "<<candidateVertex<<" "<<sum<<endl;
	  if ( debug_af ) cout<<"                           "<<orient2d(pp[0].ptr(),pp[1].ptr(),candPhys.ptr())/ASmag2(faceNormal)<<endl;

	  real t04 = getCPU();
	  timing[existingInCircle_2] += t04-t03;

	  if ( correctDirection && !alreadyFound )
	    {
	      local_nodes.push_back(candidateVertex);


	      matVectMult(T,candPhys, candTrans);
	      
	      real asum = 0.0;
	      //	      if ( rangeDimension!=domainDimension )
	      //	closeEnough = true;
	      //else
		{
		  for ( a=0; a<rangeDimension; a++ )
		    asum += (candTrans(a)-pIdealTrans(a))*(candTrans(a)-pIdealTrans(a));
		  //closeEnough = asum<(distance*distance);
		  //cout<<"asum, distance*distance "<<asum<<" "<<distance*distance<<endl;
		  
		  closeEnough = FLT_EPSILON<(distance-sqrt(asum));
		  //closeEnough = 0.<(distance-sqrt(asum));
		}
	      //closeEnough = asum<1.0;
	      //closeEnough = sqrt(asum)<0.9*distance;
	      if ( closeEnough )
		{
		  if (debug_af) cout<<"close enough true for "<<candidateVertex<<" : "<<distance-sqrt(asum)<<" "<<distance<<" "<<sqrt(asum)<<"  "<<distance<<endl;
		  //cout<<"inserting neighb vertex "<<candidateVertex<<" with orientation "<<orient3d(pp[0].ptr(), pp[1].ptr(), pp[2].ptr(), candPhys.ptr())<<endl;
		  //cout<<"here's the bloody face \n"<<candPhys<<endl;
		  existing_candidates_neighb.push_back( candidateVertex );
		  //nvcand++;
		}
	      // else
// 		cout<<candidateVertex<<" failed closeenough"<<endl;
	      timing[existingInCircle_3] += getCPU()-t04;
	    }
	  //	  else
	  // cout<<candidateVertex<<" failed correct direction"<<endl;
	}
    //   else
// 	cout<<candidateVertex<<" failed isInFace "<<endl;
      real t05 = getCPU();
      ++vtrav;
      timing[existingInCircle_4] += getCPU()-t05;
    }
  
  real t2 = getCPU();
  timing[existingInCircle] += t2-t1;

  real t3 = getCPU();
  timing[existingAdj]+= t3-t2;

}

//\begin{>>AdvancingFront.tex}{\subsection{computeVertexCandidates}}
void
AdvancingFront::
computeVertexCandidates(const ArraySimple<real> &currentFaceVerticesTrans, 
			const ArraySimple<real> &pIdealTrans, 
			vector<int> &existing_candidates, 
			const ArraySimple<real> &T, ArraySimple<real> &new_candidates, real growthRadius)
//===========================================================================
// /Purpose: compute a list of candidate new vertices to add to the mesh
// /currentFaceVerticesTrans (input) : the transformed vertex coordinates for the growth face
// /pIdealTrans (input) : the location of the "ideal" vertex in the normalized space
// /T (input) : the stretching transformation
// /existing_candidates (input) : the list of candidates already in the front
// /new_candidates (output) : the list of new candidates
//\end{AdvancingFront.tex}
//===========================================================================
{

  
  Range AXES(rangeDimension);

  MeshQualityMetrics mq;
  //  mq.setReferenceTransformation(&controlFunction);

  //new_candidates.redim(1+existing_candidates.size(),domainDimension);
  new_candidates.resize(1+existing_candidates.size(),rangeDimension);

  int a;
  for ( a=0; a<rangeDimension; a++ ) new_candidates(0,a) = pIdealTrans(a);

  real rad2 = growthRadius*growthRadius;


  if ( existing_candidates.size()>0 )
    {
      ArraySimple<real> elQuality_ex(existing_candidates.size());
      ArraySimple<real> elQuality_new(1+existing_candidates.size());
      elQuality_new(0) = 1.;

     //realArray newVertexDistance( existing_candidates.size() );
      ArraySimple<real> newVertexDistance( existing_candidates.size() );
      
      ArraySimple<real> newVertex(rangeDimension);
      ArraySimple<real> exCandTrans(rangeDimension), tmp1(rangeDimension);
      ArraySimple<real> p1(rangeDimension), p2(rangeDimension),p3(rangeDimension);
      
      for ( a=0; a<rangeDimension; a++ )
	{
	  p1(a) = currentFaceVerticesTrans(0,a);
	  p2(a) = currentFaceVerticesTrans(1,a);
	  if ( rangeDimension==3 && domainDimension==3 )
	    p3(a) = currentFaceVerticesTrans(2,a);
	}
      
      ArraySimple<real> exVertexDist( existing_candidates.size() );
      
      if ( domainDimension == 2 ) 
	{ // use circle center
	  
	  if ( rangeDimension==2 )
	    {
	      for ( int cand=0; cand<existing_candidates.size(); cand++ )
		{
		  int cidx = existing_candidates[cand];
		  for ( a=0; a<rangeDimension; a++ ) 
		    {
		      tmp1(a) = xyz(cidx, a);
		    }
		  matVectMult(T,tmp1,exCandTrans);
		  // the new candidate vertex is the center of a circle made by the
		  // vertices in the original face plus one of the existing candidates
		  // // note, if get_circle_center returns <0 then the given vertices are on
		  // //       a line which indicates that the gatherExistingFaces part failed
	      
	      
		  real tg0 = getCPU();
		  if ( get_circle_center(exCandTrans, p1,p2, newVertex) < 0 ) 
		    if ( debug_af )
		      abort();
		    else
		      {
			cout<<"AdvancingFront : WARNING : nearly co-planar vertex candidate!"<<endl;
			newVertex[0] = .5*(p1[0]+p2[0]);
			newVertex[1] = .5*(p1[1]+p2[1]);
			//			throw AdvancingFrontError();
		      }

		  timing[getCircleCent] += getCPU()-tg0;
	      
		  newVertexDistance(cand) = 0.0;
		  exVertexDist(cand) = 0.0;
		  ArraySimpleFixed<real,2,1,1,1> pp1,pp2,pp3,ppe;
		  ArraySimpleFixed<real,2,2,1,1> jac;

		  for ( a=0; a<rangeDimension; a++ )
		    {
		      newVertexDistance(cand) += (newVertex(a)-new_candidates(0,a))*(newVertex(a)-new_candidates(0,a));
		      exVertexDist(cand) += 
			(exCandTrans(a)-new_candidates(0,a))*(exCandTrans(a)-new_candidates(0,a));
		      new_candidates(cand+1,a) = newVertex(a);
		      pp1[a] = p1[a];
		      pp2[a] = p2[a];
		      pp3[a] = newVertex[a];
		      ppe[a] = exCandTrans[a];
		    }

		  ArraySimpleFixed<real,2,2,1,1> TT = mq.computeWeight(pp1,UnstructuredMapping::triangle);
		  jac = mq.computeJacobian(pp1,pp2,ppe,TT);
		  real N2=0,K=0,det=0;
		  mq.computeJacobianProperties(N2,det,K,jac);
		  //elQuality_ex(cand) = min(det,1/det)*2./K;
		  elQuality_ex(cand) = 2./K;

		  jac = mq.computeJacobian(pp1,pp2,pp3,TT);
		  mq.computeJacobianProperties(N2,det,K,jac);
		  //elQuality_new(cand+1) = min(det,1/det)*2./K;
		  elQuality_new(cand+1) = 2./K;
		}
	    }
	  else
	    {

	      ArraySimpleFixed<real,3,1,1,1> fnorm,snorm,pp1,pp2,pidealc;
	      
	      ArraySimpleFixed<real,3,1,1,1> e;
	      
	      ArraySimpleFixed<real,2,1,1,1> p1t,p2t,p3t,p4t,p5t,pc;  // 2D points on plane with axes origin at the face midpoint

	      int axis;

	      for ( axis=0; axis<3; axis++ ) 
		{
		  pp1[axis] = p1[axis];
		  pp2[axis] = p2[axis];
		  pidealc[axis] = pIdealTrans(axis);
		  e[axis] = p2[axis]-p1[axis];
		}
	      
	      snorm = areaNormal3D(pp1,pp2,pidealc);

	      real emag = sqrt(ASmag2(e));
	      fnorm[0] = -(e[1]*snorm[2]-e[2]*snorm[1]);
	      fnorm[1] = e[0]*snorm[2]-e[2]*snorm[0];
	      fnorm[2] = -(e[0]*snorm[1]-e[1]*snorm[0]);
	      real fmag = sqrt(ASmag2(fnorm));

	      p1t[0]=p1t[1]=p2t[0]=p2t[1]=p3t[0]=p3t[1] = 0.;
	      
	      for ( axis=0; axis<3; axis++ ) 
		{
		  real t = 0.5*(p2[axis]-p1[axis]);
		  p1t[0] -= t*e[axis]/emag;
		  p1t[1] -= t*fnorm[axis]/fmag;
		  p2t[0] += t*e[axis]/emag;
		  p2t[1] += t*fnorm[axis]/fmag;

		  real m = 0.5*(p2[axis]+p1[axis]);
		  p3t[0] += (pidealc[axis]-m)*e[axis]/emag;
		  p3t[1] += (pidealc[axis]-m)*fnorm[axis]/fmag;
		}


	      int cand;
	      for ( cand=0; cand<existing_candidates.size(); cand++ )
		{
		  int cidx = existing_candidates[cand];
		  if ( debug_af )cout<<"geting circle center for candidate id "<<cidx<<endl;

		  for ( a=0; a<rangeDimension; a++ ) 
		    {
		      tmp1(a) = xyz(cidx, a);
		    }

		  matVectMult(T,tmp1,exCandTrans);
		  real tg0 = getCPU();
		  if ( get_circle_center_on_plane(exCandTrans, p1,p2, newVertex) < 0 ) 
		    {
		      cout<<"BAD SURFACE MESH CANDIDATE"<<endl;
		      throw AdvancingFrontError();
		    }

		  timing[getCircleCent] += getCPU()-tg0;

		  if ( debug_af )
		    {
		      cout<<"P1 "<<p1<<endl;
		      cout<<"P2 "<<p2<<endl;
		      cout<<"cand "<<exCandTrans<<endl;
		      cout<<"center "<<newVertex<<endl;
		    }

		  newVertexDistance(cand) = 0.0;
		  exVertexDist(cand) = 0.0;
		  pc[0] = pc[1] = 0.;
		  p4t[0]=p4t[1]=0.;
		  for ( axis=0; axis<3; axis++ ) 
		    {
		      real m = 0.5*(p1[axis]+p2[axis]);
		      pc[0] += (newVertex(axis)-m)*e[axis]/emag;
		      pc[1] += (newVertex(axis)-m)*fnorm[axis]/fmag;
		      p4t[0] += (exCandTrans(axis)-m)*e[axis]/emag;
		      p4t[1] += (exCandTrans(axis)-m)*fnorm[axis]/fmag;
		    }

		  newVertexDistance(cand) = (pc[0]-p3t[0])*(pc[0]-p3t[0]) + 
		    (pc[1]-p3t[1])*(pc[1]-p3t[1]);

		  for ( a=0; a<rangeDimension; a++ )
		    {

		      exVertexDist(cand) += 
			(exCandTrans(a)-new_candidates(0,a))*(exCandTrans(a)-new_candidates(0,a));
		      new_candidates(cand+1,a) = newVertex(a);
		    }

		  ArraySimpleFixed<real,2,2,1,1> TT = mq.computeWeight(pc,UnstructuredMapping::triangle);		  
		  ArraySimpleFixed<real,2,2,1,1> jac;
		  jac = mq.computeJacobian(p1t,p2t,p4t,TT);
		  real N2,K,det;
		  mq.computeJacobianProperties(N2,det,K,jac);
		  //elQuality_ex(cand) = min(det,1/det)*2./K;
		  elQuality_ex(cand) = 2./K;
		  
		  jac = mq.computeJacobian(p1t,p2t,pc,TT);
		  mq.computeJacobianProperties(N2,det,K,jac);
		  //elQuality_new(cand+1) = min(det,1/det)*2./K;
		  elQuality_new(cand+1) = 2./K;
		}

	    }
	}
      else
	{ 
	  for ( int cand=0; cand<existing_candidates.size(); cand++ )
	    {
	      int cidx = existing_candidates[cand];
	      for ( a=0; a<rangeDimension; a++ ) 
		{
		  tmp1(a) = xyz(cidx, a);
		}
	      matVectMult(T,tmp1,exCandTrans);
	      
	      if ( get_sphere_center(p1,p2,p3,exCandTrans,newVertex)<0 ) {
		cout<<"P1 "<<p1<<endl;
		cout<<"P2 "<<p2<<endl;
		cout<<"P3 "<<p3<<endl;
		cout<<"cand"<<exCandTrans<<endl;
		cout<<"trying to make a sphere with a coplanar point!! "<<endl;
		cout<<"vertex is "<<cidx<<endl;
		//cout<<"perturbing existing candidate and trying again"<<endl;
		ArraySimple<real> n=areaNormal3D(p1,p2,p3);
		real nmag = sqrt(ASmag2(n));
		//exCandTrans[0] += 10*REAL_EPSILON*n[0]/nmag;
		//exCandTrans[1] += 10*REAL_EPSILON*n[1]/nmag;
		//exCandTrans[2] += 10*REAL_EPSILON*n[2]/nmag;
		// Force it to make an invalid element
		for ( int aa=0; aa<3; aa++ )
		  newVertex[aa] = (p1[aa]+p2[aa]+p3[aa])/3.0 - 100*FLT_MIN*n[aa]/nmag; //?
		//if ( get_sphere_center(p1,p2,p3,exCandTrans,newVertex)<0 )
		//  throw AdvancingFrontError();
	      }

	      newVertexDistance(cand) = 0.0;
	      exVertexDist(cand) = 0.0;

	      ArraySimpleFixed<real,3,1,1,1> pp1,pp2,pp3,pp4,ppe;
	      ArraySimpleFixed<real,3,3,1,1> jac;

	      for ( a=0; a<rangeDimension; a++ )
		{
		  newVertexDistance(cand) += ( newVertex(a)-new_candidates(0,a))*( newVertex(a)-new_candidates(0,a));
		  exVertexDist(cand) += 
		    (exCandTrans(a)-new_candidates(0,a))*(exCandTrans(a)-new_candidates(0,a));
		  new_candidates(cand+1,a) = newVertex(a);
		  pp1[a] = p1[a];
		  pp2[a] = p2[a];
		  pp3[a] = p3[a];
		  pp4[a] = newVertex[a];
		  ppe[a] = exCandTrans[a];
		}
	      
	      ArraySimpleFixed<real,3,3,1,1> TT = mq.computeWeight(pp1,UnstructuredMapping::tetrahedron);
	      jac = mq.computeJacobian(pp1,pp2,pp3,ppe,TT);
	      real N2,K,det;
	      mq.computeJacobianProperties(N2,det,K,jac);
	      //elQuality_ex(cand) = min(det,1/det)*3./K;
	      elQuality_ex(cand) = 3./K;
	      
	      jac = mq.computeJacobian(pp1,pp2,pp3,pp4,TT);
	      mq.computeJacobianProperties(N2,det,K,jac);
	      //elQuality_new(cand+1) = min(det,1/det)*3./K;
	      elQuality_new(cand+1) = 3./K;

	    }
	  
	}
      
      // now bubble sort candidates 
      // (hey, its easy and should be fast enough for the small lists of candidates)
      
      bool sorted = true;
      if (existing_candidates.size()>1) sorted = false;
      
      // the vertices are sorted so that the existing candidates are ordered 
      // in descending order of thier corresponding new_candidates distance from pIdealTrans
      int tmp;
      real npTmp;
      ArraySimple<real> aTmp(rangeDimension);

#if 0
      // sort by quality

      while ( !sorted )
	{
	  sorted = true;
	  for (int c=0; c<existing_candidates.size()-1; c++)
	    {
	      if ( elQuality_new(c+1)<elQuality_new(c+2) )
		{
		  for ( a=0; a<rangeDimension; a++ )
		    {
		      aTmp(a) = new_candidates(c+1,a);
		      new_candidates(c+1,a) = new_candidates(c+2,a);
		      new_candidates(c+2,a) = aTmp(a);
		    }
		  sorted = false;
		  npTmp = elQuality_new(c+1);
		  elQuality_new(c+1) = elQuality_new(c+2);
		  elQuality_new(c+2) = npTmp;
		}
	    }
	}

      sorted = false;
      
      while ( !sorted )
	{
	  sorted = true;
	  for (int c=0; c<existing_candidates.size()-1; c++)
	    {
	      if ( elQuality_ex(c)<elQuality_ex(c+1) )
		{
		  tmp = existing_candidates[c];
		  existing_candidates[c] = existing_candidates[c+1];
		  existing_candidates[c+1] = tmp;
		  sorted = false;
		  npTmp = elQuality_ex(c);
		  elQuality_ex(c) = elQuality_ex(c+1);
		  elQuality_ex(c+1) = npTmp;
		}
	    }
	}

//       cout<<"NEW QUALITY"<<endl;
//       cout<<elQuality_new<<endl;
//       cout<<"EXISTING QUALITY"<<endl;
//       cout<<elQuality_ex<<endl;
#else      
      while(!sorted)
	{
	  sorted = true;
	  for (int c=0; c<existing_candidates.size()-1; c++)
	    {
#if 0
	      if ((newVertexDistance(c)<newVertexDistance(c+1) && 
		   exVertexDist(c)<=rad2 &&
		   exVertexDist(c+1)<=rad2 ) || 
		  (exVertexDist(c)>rad2 && exVertexDist(c+1)<rad2) )
#else
         if ( debug_af ) cout<<"existing "<<existing_candidates[c]<<", "<<exVertexDist(c)<<", "<<existing_candidates[c+1]<<", "<<exVertexDist(c+1)<<endl;
		if ( exVertexDist(c)>exVertexDist(c+1) )
#endif
		{
		  // swap the entries into existing_candidates and new_candidates
		  tmp = existing_candidates[c];
		  existing_candidates[c] = existing_candidates[c+1];
		  existing_candidates[c+1] = tmp;
		  //realArray aTmp; 
		  for ( a=0; a<rangeDimension; a++ )
		    {
		      aTmp(a) = new_candidates(c+1,a);
		      new_candidates(c+1,a) = new_candidates(c+2,a);
		      new_candidates(c+2,a) = aTmp(a);
		    }
		  npTmp = newVertexDistance(c);
		  newVertexDistance(c) = newVertexDistance(c+1);
		  newVertexDistance(c+1) = npTmp;
		  
		  npTmp = exVertexDist(c);
		  exVertexDist(c) = exVertexDist(c+1);
		  exVertexDist(c+1) = npTmp;
		  
		  sorted = false;
		} // if swap
	    } // for (int c...
	} // while (!sorted...
      
      // the new_candidates are arranged in the opposite
      int c;
      ArraySimple<real> newCandtmp(new_candidates.size(0)-1,rangeDimension);
#if 1
      
      sorted = false;
      while (!sorted)
	{
	  sorted = true;
	  for ( int c=0; c<existing_candidates.size()-1; c++ )
	    if ( newVertexDistance(c)>newVertexDistance(c+1) )
	      {
		sorted = false;
		for ( a=0; a<rangeDimension; a++ )
		  {
		      aTmp(a) = new_candidates(c+1,a);
		      new_candidates(c+1,a) = new_candidates(c+2,a);
		      new_candidates(c+2,a) = aTmp(a);
		  }
		npTmp = newVertexDistance(c);
		newVertexDistance(c) = newVertexDistance(c+1);
		newVertexDistance(c+1) = npTmp;
	      }
	}
#endif 
#if 0
      for ( c=1; c<new_candidates.size(0); c++ )
	for ( a=0; a<rangeDimension; a++ )
	  newCandtmp(c-1,a) = new_candidates(c,a);
      
      for ( c=0; c<new_candidates.size(0)-1; c++ )
	for ( a=0; a<rangeDimension; a++ )
	  new_candidates(c+1,a) = newCandtmp(newCandtmp.size(0)-c-1,a);
#endif
      if ( debug_af )
	{ 
	  cout<<"exVertexDist "<<exVertexDist<<endl;
	  cout<<"newVertexDistance "<<newVertexDistance<<endl;
	}
#endif
    }

}

bool 
AdvancingFront::
makeTriOnSurface(const Face &currentFace, int newElementID,
		 const vector<int> &existing_candidates, 
		 ArraySimple<real> &new_candidates, vector<PriorityQueue::iterator > &oldFrontFaces)
{
  ArraySimpleFixed<real,3,1,1,1,1> fnorm, snorm, sn1,sn2,cnrm;

  ArraySimpleFixed<real,3,1,1,1> p1,p2,p3,e;

  ArraySimple<real> p1t(2),p2t(2),p3t(2),p4t(2),p5t(2);  // 2D points on plane with axes origin at the face midpoint


  real nrmd;

  int v1 = currentFace.getVertex(0);
  int v2 = currentFace.getVertex(1);
  int axis,a;
  int cfid = currentFace.getID();
  for ( axis=0; axis<3; axis++ ) 
    {
      fnorm[axis] = faceNormals(cfid,axis);
      p1[axis] = xyz(v1,axis);
      p2[axis] = xyz(v2,axis);
      e[axis] = p2[axis]-p1[axis];
    }

  real emag = sqrt(ASmag2(e));
  snorm[0] = e[1]*fnorm[2]-e[2]*fnorm[1];
  snorm[1] = -(e[0]*fnorm[2]-e[2]*fnorm[0]);
  snorm[2] = e[0]*fnorm[1]-e[1]*fnorm[0];

  p1t[0]=p1t[1]=p2t[0]=p2t[1] = 0.;

  real smag = sqrt(ASmag2(snorm));

  for ( axis=0; axis<3; axis++ ) 
    {
      real t = 0.5*(p2[axis]-p1[axis]);
      p1t[0] -= t*e[axis]/emag;
      p1t[1] -= t*fnorm[axis];
      p2t[0] += t*e[axis]/emag;
      p2t[1] += t*fnorm[axis];
      
      snorm[axis] /= smag;
    }

  for (axis=0; axis<2; axis++ )
    {
      if (fabs(p1t[axis])<10*REAL_EPSILON) p1t[axis]=0.;
      if (fabs(p2t[axis])<10*REAL_EPSILON) p2t[axis]=0.;
    }

  //cout<<"p1t,p2t"<<endl;
  //cout<<p1t<<"\n\n"<<p2t<<endl;

    // // // check existing candidates for a usable candidate ( includes the while loop below )  
  vector<int>::const_iterator exPt = existing_candidates.begin();

  //IntegerArray newFaceVerts(domainDimension); // number of vertices on a face is 2 in 2D, 3 in 3D
  ArraySimple<int> newFaceVerts(domainDimension);

  bool createNewFaceWithEdge[2]; // keeps track of new faces
  bool foundConsistentFaces[2]; // keeps track of how many consistent faces found
  bool createdElement = false; // was a new element created ?
  bool oldFace1, oldFace2;
  oldFace1=oldFace2=false;

  vector<int> candidateIntersections;
  candidateIntersections.reserve(20);
  int filterFace = currentFace.getID();

  // filter out all candidates that are on different surfaces
//   while ( exPt!=existing_candidates.end() )
//     {
//       p3t[0] = p3t[1] = 0.;
//       nrmd=0;

//       for ( axis=0; axis<3; axis++ )
// 	{
// 	  p3[axis] = xyz(*exPt, axis);
// 	  p3t[0] += (xyz(*exPt, axis)-0.5*(p1[axis]+p2[axis]))*e[axis]/emag;
// 	  p3t[1] += (xyz(*exPt, axis)-0.5*(p1[axis]+p2[axis]))*fnorm[axis];
// 	  nrmd += (p3[axis]-p1[axis])*snorm[axis];
// 	}

//       bool tooFarOff;

//       const vector<PriorityQueue::iterator> &exPtFaces = pointFaceMapping[*exPt];
      
//       real dotp = -REAL_MAX;
//       for ( vector<PriorityQueue::iterator>::const_iterator f = exPtFaces.begin(); f!=exPtFaces.end(); f++ )
// 	{
// 	  int ss = (**f)->getID()<initialFaceSurfaceMapping.getLength(0) ? initialFaceSurfaceMapping((**f)->getID()) : -1;
// 	  dotp = max(dotp, ASdot(snorm,computeSurfaceNormal(p3,ss)));

// 	  if (debug_af)
// 	    cout<<"comparing normals between face "<<(**f)->getID()<<" and vertex "<<*exPt<<" : "<<ASdot(snorm,computeSurfaceNormal(p3,ss))<<", dotp is  "<<dotp<<endl;

// 	}

//       if ( dotp<0.5 ) {
// 	existing_candidates.erase(exPt);
// 	exPt = existing_candidates.begin();
//       }
//       else
// 	exPt++;
	  

//     }

//   exPt = existing_candidates.begin();

  // loop through all the existing candidates and see if any will
  // create a new element with the current face.
  while(!createdElement && exPt!=existing_candidates.end())
    {
      
      oldFace1 = oldFace2 = false;

      if ( debug_af ) cout<<"processing candidate "<<*exPt<<", there are "<<existing_candidates.size()<<endl;
      for ( int vf=0; vf<currentFace.getNumberOfVertices(); vf++ )
	{
	  createNewFaceWithEdge[vf] = false;
	  foundConsistentFaces[vf] = false;
	}

      createdElement = false;

      p3t[0] = p3t[1] = 0.;
      nrmd=0;

      for ( axis=0; axis<3; axis++ )
	{
	  p3[axis] = xyz(*exPt, axis);
	  p3t[0] += (xyz(*exPt, axis)-0.5*(p1[axis]+p2[axis]))*e[axis]/emag;
	  p3t[1] += (xyz(*exPt, axis)-0.5*(p1[axis]+p2[axis]))*fnorm[axis];
	  nrmd += (p3[axis]-p1[axis])*snorm[axis];
	}

      for ( axis=0; axis<2; axis++ )
	if ( fabs(p3t[axis])<10*REAL_EPSILON ) p3t[axis]=0.;

      bool tooFarOff;

      tooFarOff = ( fabs(nrmd)>(0.25*emag*emag) );

      const vector<PriorityQueue::iterator> &exPtFaces = pointFaceMapping[*exPt];
      
      real dotp = -REAL_MAX;
      for ( vector<PriorityQueue::iterator>::const_iterator f = exPtFaces.begin(); f!=exPtFaces.end(); f++ )
	{
	  int ss = (**f)->getID()<initialFaceSurfaceMapping.getLength(0) ? initialFaceSurfaceMapping((**f)->getID()) : -1;
	  dotp = max(dotp, ASdot(snorm,computeSurfaceNormal(p3,ss)));

	  if (debug_af)
	    {
	      cout<<"comparing normals between face "<<(**f)->getID()<<" and vertex "<<*exPt<<" : "<<ASdot(snorm,computeSurfaceNormal(p3,ss))<<", dotp is  "<<dotp<<endl;
	      cnrm = computeSurfaceNormal(p3,ss);
	      cout<<"   snorm = "<<snorm<<endl<<"   surface norm at exPt = "<<cnrm<<endl;
	      cout<<"   ss = "<<ss<<endl;
	    }

	}

      tooFarOff = dotp<0.5; //angle between surface normals is greater than 60 degrees

      newFaceVerts(0) = *exPt;
      newFaceVerts(1) = currentFace.getVertex(0);
      
      bool isInFront = existsInFront(newFaceVerts);
      if ( isInFront )
	{
	  oldFrontFaces.push_back(getFrontIteratorForFace(newFaceVerts));
	  foundConsistentFaces[0]=true;
	  oldFace1 = true;
	}

      // first check to see if the face already exists
      newFaceVerts(0) = currentFace.getVertex(1);
      newFaceVerts(1) = *exPt;
      isInFront = existsInFront(newFaceVerts);
      if ( isInFront )
	{
	  oldFrontFaces.push_back(getFrontIteratorForFace(newFaceVerts));
	  foundConsistentFaces[1]=true;
	  oldFace2 = true;
	}

      ArraySimple<real> minmax(2*rangeDimension);
      int axis;
      real dm = -REAL_MAX;
      for (axis=0; axis<rangeDimension; axis++)
	{
	  minmax(2*axis) = min(p1(axis), p2(axis), xyz(*exPt,axis));
	  minmax(1+2*axis) = max(p1(axis), p2(axis), xyz(*exPt,axis));
	  dm =  max(dm,minmax(1+2*axis)-minmax(2*axis));
	}

      for (axis=0; axis<rangeDimension; axis++)
  	{
	  
  	  minmax(2*axis) -= 0.001*dm;
  	  minmax(2*axis+1) += 0.001*dm;
  	}

      GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);
      bool intersects = false;
      bool isParallel = false;
      createdElement = foundConsistentFaces[0] && foundConsistentFaces[1];

      bool invalid = tooFarOff;//false;
      if ( debug_af) cout<<"oldFace1,2 "<<oldFace1<<" "<<oldFace2<<endl;
      while( !candSearch.isFinished() && !invalid && (!oldFace1 || !oldFace2) )
	{
	  if ((*candSearch).data->getID() != filterFace)
	    {
	      int p4id = (*candSearch).data->getVertex(0);
	      int p5id = (*candSearch).data->getVertex(1);
	      p4t[0]=p4t[1]=p5t[0]=p5t[1]=0.;
	      for ( a=0; a<rangeDimension; a++ )
		{
		  p4t(0) += (xyz(p4id,a) - 0.5*(p2[a]+p1[a]))*e[a]/emag;
		  p4t(1) += (xyz(p4id,a) - 0.5*(p2[a]+p1[a]))*fnorm[a];
		  p5t(0) += (xyz(p5id,a) - 0.5*(p2[a]+p1[a]))*e[a]/emag;
		  p5t(1) += (xyz(p5id,a) - 0.5*(p2[a]+p1[a]))*fnorm[a];
		}

	      for ( a=0; a<2; a++ )
		{
		  if ( fabs(p4t[a])<10*REAL_EPSILON ) p4t[a]=0.;
		  if ( fabs(p5t[a])<10*REAL_EPSILON ) p5t[a]=0.;
		}

	      if ( p4id==v1 ) p4t=p1t;
	      if ( p5id==v1 ) p5t=p1t;
	      if ( p4id==v2 ) p4t=p2t;
	      if ( p5id==v2 ) p5t=p2t;

	      //cout<<"p4t, p5t\n"<<p4t<<"\n\n"<<p5t<<endl;

	      if ( !oldFace1 )
		{
		  //cout<<"checking 0"<<endl;
		  intersects = intersect2D(p1t,p3t,p4t,p5t, isParallel);
		  
		  if (intersects) 
		    {
		      if (debug_af) cout <<"intersection found with "<<(*candSearch).data->getID()<<endl;
		      foundConsistentFaces[0] = false;
		    }
		  else
		    {
		      foundConsistentFaces[0] = true;
		    }
		  
		  if (isParallel && !intersects)
		    {
		      // check to see if the parallel faces overlap each other
		      //cout <<"parallel at 1 "<<endl;
		      if ( v1!=(*candSearch).data->getVertex(0) && v1!=(*candSearch).data->getVertex(1) )
			if ( isBetweenOpenInterval2D(p1t,p3t,p4t) ) foundConsistentFaces[0] = false;
		      
		      if ( v2!=(*candSearch).data->getVertex(0) && v2!=(*candSearch).data->getVertex(1) )
			if ( isBetweenOpenInterval2D(p1t,p3t,p5t) ) foundConsistentFaces[0] = false;
		      
		      if ( !foundConsistentFaces[0] && debug_af ) cout<<"parallel overlapping at 1 found with "<<(*candSearch).data->getID()<<endl;
		    }
		  invalid = !foundConsistentFaces[0];
		  createNewFaceWithEdge[0] = foundConsistentFaces[0];
		}

	      if ( !invalid && !oldFace2 )
		{
		  //cout<<"checking 1 "<<endl;
		  intersects = intersect2D(p2t,p3t,p4t,p5t, isParallel);
		  if (intersects) 
		    {
		      if (debug_af) cout <<"intersection found with "<<(*candSearch).data->getID()<<endl;
		      //cout <<"intersection found with "<<(*candSearch).data->getID()<<endl;
		      foundConsistentFaces[1] = false;;
		    }
		  else
		    {
		      foundConsistentFaces[1] = true;
		    }

		  if (isParallel)
		    {
		      // check to see if the parallel faces overlap each other
		      //cout <<"parallel at 1 "<<endl;
		      if ( v1!=(*candSearch).data->getVertex(0) && v1!=(*candSearch).data->getVertex(1) )
			if ( isBetweenOpenInterval2D(p2t,p3t,p4t) ) foundConsistentFaces[1] = false;
		      
		      if ( v2!=(*candSearch).data->getVertex(0) && v2!=(*candSearch).data->getVertex(1) )
			if ( isBetweenOpenInterval2D(p2t,p3t,p5t) ) foundConsistentFaces[1] = false;
		      if ( !foundConsistentFaces[1] && debug_af ) cout<<"parallel overlapping at 2 found with "<<(*candSearch).data->getID()<<endl;
		    }

		  invalid = !foundConsistentFaces[1];
		  createNewFaceWithEdge[1] = foundConsistentFaces[1];
		}
	    }

	  //if ( invalid ) cout<<"INVALID FACE!"<<endl;
	  ++candSearch;
	}

      createdElement = (foundConsistentFaces[0] && foundConsistentFaces[1]);
      //cout<<"foundConsistentFaces[0],[1] "<<foundConsistentFaces[0]<<" "<<foundConsistentFaces[1]<<endl;

#if 1
      if ( createdElement )
	{
	  // make sure none of the remaining existing candidates would be inside the triangle

	  ArraySimpleFixed<real,2,1,1,1> pc;
	  ArraySimpleFixed<real,3,1,1,1> pcc;
	  
	  for ( int vv=0; createdElement && vv<currentFace.getNumberOfVertices(); vv++ )
	    {
	      const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[currentFace.getVertex(vv)];
	      for ( vector<PriorityQueue::iterator>::const_iterator f=v1Faces.begin(); createdElement && 
			  f!=v1Faces.end(); f++ )
		if ( (**f)->getID()!=currentFace.getID() )
		  for ( int vvf=0; createdElement && vvf<(**f)->getNumberOfVertices(); vvf++ )
		    if ( (**f)->getVertex(vvf)!=currentFace.getVertex(0) && 
			 (**f)->getVertex(vvf)!=currentFace.getVertex(1) && 
			 (**f)->getVertex(vvf)!=*exPt)
		      {
			int vc = (**f)->getVertex(vvf);
			pc[0]=pc[1]=0.;
			for ( a=0; a<3; a++ )
			  {
			    pc[0] += (xyz(vc, a)-0.5*(p1[a]+p2[a]))*e[a]/emag;
			    pc[1] += (xyz(vc, a)-0.5*(p1[a]+p2[a]))*fnorm[a];

			    pcc[a] = xyz(vc,a);
			  }

			for ( a=0; a<2; a++ )
			  {
			    if ( fabs(pc[a])<10*REAL_EPSILON ) pc[a]=0.;
			    if ( fabs(pc[a])<10*REAL_EPSILON ) pc[a]=0.;
			  }

			const vector<PriorityQueue::iterator> &pcFaces = pointFaceMapping[vc];
			
			real dotp = -REAL_MAX;
			for ( vector<PriorityQueue::iterator>::const_iterator f =pcFaces.begin(); f!=pcFaces.end(); f++ )
			  {
			    int ss = (**f)->getID()<initialFaceSurfaceMapping.getLength(0) ? initialFaceSurfaceMapping((**f)->getID()) : -1;
			    dotp = max(dotp, ASdot(snorm,computeSurfaceNormal(pcc,ss)));
			    
			    if (debug_af)
			      cout<<"comparing normals between face, in intri "<<(**f)->getID()<<" and vertex "<<vc<<" : "<<ASdot(snorm,computeSurfaceNormal(pcc,ss))<<", dotp is  "<<dotp<<endl;
			    
			  }
			
			//cout<<"vertex in triangle check "<<vc<<endl;
			//cout<<p1t<<endl<<p2t<<endl<<p3t<<endl<<pc[0]<<" "<<pc[1]<<endl;
			if ( dotp>0.5 ) {
			  createdElement = !(orient2d(p1t.ptr(),p2t.ptr(),pc.ptr())>=0 && 
					     orient2d(p2t.ptr(),p3t.ptr(),pc.ptr())>=0 &&
					     orient2d(p3t.ptr(),p1t.ptr(),pc.ptr())>=0);
			  if ( !createdElement ) 
			    {
			      cout<<"failed vertex in triangle check with vertex "<<vc<<endl;
			      cout<<orient2d(p1t.ptr(),p2t.ptr(),pc.ptr())<<" "<<orient2d(p2t.ptr(),p3t.ptr(),pc.ptr())<<" "<<orient2d(p3t.ptr(),p1t.ptr(),pc.ptr())<<endl;
			    }
			}

		      }
	    }
	  
	  if ( !createdElement ) createNewFaceWithEdge[0] = createNewFaceWithEdge[1] = false;
	}
#endif

      if (!createdElement || invalid )
	{
	  while ( oldFrontFaces.size()>1 ) oldFrontFaces.pop_back();		
	  foundConsistentFaces[0] = foundConsistentFaces[1] = false;
	  createNewFaceWithEdge[0] = createNewFaceWithEdge[1] = false;
	  createdElement = false;
	}
      
      
      // now create and insert any new faces
      if (createNewFaceWithEdge[0]) {
	newFaceVerts(0) = currentFace.getVertex(0);
	newFaceVerts(1) = *exPt;
	int id = insertFace(newFaceVerts, newElementID, -1);
	if ( currentFace.getID()<initialFaceSurfaceMapping.getLength(0) && id<initialFaceSurfaceMapping.getLength(0))
	  initialFaceSurfaceMapping(id) = initialFaceSurfaceMapping(currentFace.getID());
      }
      if (createNewFaceWithEdge[1]) {
	newFaceVerts(0) = *exPt;
	newFaceVerts(1) = currentFace.getVertex(1);
	int id = insertFace(newFaceVerts, newElementID, -1);
	if ( currentFace.getID()<initialFaceSurfaceMapping.getLength(0) && id<initialFaceSurfaceMapping.getLength(0))
	  initialFaceSurfaceMapping(id) = initialFaceSurfaceMapping(currentFace.getID());
      }

      if ( debug_af )
	{
	  cout<<"old front faces : ";
	  for ( int i=0; i<oldFrontFaces.size(); i++ )
	    cout<<(*oldFrontFaces[i])->getID()<<"  ";
	  cout<<endl;
	}

      exPt++;
    }

  if ( !createdElement ) 
    { // check candidate new vertices 

      //cout<<"checking candidate new vertices"<<endl;
      real t0 = getCPU();
      int c = 0;
      // loop through all the new candidate vertices, look for a valid new element
      while( !createdElement && (c<1))//new_candidates.size(0)) )
	{
	  int a;
	  
	  p3t[0] = p3t[1] = 0.;
	  for ( axis=0; axis<3; axis++ )
	    {
	      p3t[0] += (new_candidates(c, axis)-0.5*(p1[axis]+p2[axis]))*e[axis]/emag;
	      p3t[1] += (new_candidates(c, axis)-0.5*(p1[axis]+p2[axis]))*fnorm[axis];
	    }

	  for ( axis=0; axis<2; axis++ )
	    if ( fabs(p3t[axis])<10*REAL_EPSILON ) p3t[axis]=0.;

	  //cout<<"p3t"<<endl;
	  //cout<<p3t<<endl;

	  ArraySimple<real> minmax(2*rangeDimension);
	  // create the search traversor
	  int axis;
	  real dm = -REAL_MAX;
	  for (axis=0; axis<rangeDimension; axis++)
	    {
	      minmax(2*axis) = min(p1(axis),p2(axis),new_candidates(c,axis));
	      minmax(1+2*axis) = max(p1(axis),p2(axis),new_candidates(c,axis));
	      dm = max(dm,minmax(1+2*axis)-minmax(2*axis));
	    }

	  for (axis=0; axis<rangeDimension; axis++)
	    {
	      minmax(2*axis) -= 0.001*dm;
	      minmax(2*axis+1) += 0.001*dm;
	    }


	  GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);
	  
	  bool intersects = false;
	  bool isParallel = false;

	  while( !candSearch.isFinished() && !intersects )
	    {
	      if ((*candSearch).data->getID() != filterFace)
		{
		  int p4id = (*candSearch).data->getVertex(0);
		  int p5id = (*candSearch).data->getVertex(1);
		  p4t[0]=p4t[1]=p5t[0]=p5t[1]=0.;
		  for ( a=0; a<rangeDimension; a++ )
		    {
		      p4t(0) += (xyz(p4id,a) - 0.5*(p2[a]+p1[a]))*e[a]/emag;
		      p4t(1) += (xyz(p4id,a) - 0.5*(p2[a]+p1[a]))*fnorm[a];
		      p5t(0) += (xyz(p5id,a) - 0.5*(p2[a]+p1[a]))*e[a]/emag;
		      p5t(1) += (xyz(p5id,a) - 0.5*(p2[a]+p1[a]))*fnorm[a];
		    }

		  for ( a=0; a<2; a++ )
		    {
		      if ( fabs(p4t[a])<10*REAL_EPSILON ) p4t[a]=0.;
		      if ( fabs(p5t[a])<10*REAL_EPSILON ) p5t[a]=0.;
		    }

		  if ( p4id==v1 ) p4t=p1t;
		  if ( p5id==v1 ) p5t=p1t;
		  if ( p4id==v2 ) p4t=p2t;
		  if ( p5id==v2 ) p5t=p2t;

		  //cout<<"p4t, p5t\n"<<p4t<<"\n\n"<<p5t<<endl;
		  
		  intersects = intersect2D(p1t,p3t,p4t,p5t,isParallel);
		  
		  if (isParallel)
		    {
		      if (isBetweenOpenInterval2D(p4t,p5t,p1t) || isBetweenOpenInterval2D(p4t,p5t,p3t)) {
			if (debug_af) cout<<"parallel-overlapping face found with "<<(*candSearch).data->getID()<<endl;
			intersects=true;
		      }
		      		      
		    }

		  if ( intersects ) 
		    {
		      cout<<"new face intersection with face "<<(*candSearch).data->getID()<<endl;
		      cout<<p1t<<endl<<p3t<<endl<<p4t<<endl<<p5t<<endl;
		    }

		  if ( !intersects ) intersects = intersect2D(p2t,p3t,p4t,p5t,isParallel);
		  
		  if (isParallel)
		    {
		      if (isBetweenOpenInterval2D(p4t,p5t,p2t) || isBetweenOpenInterval2D(p4t,p5t,p3t)) {
			if (debug_af) cout<<"parallel-overlapping face found with "<<(*candSearch).data->getID()<<endl;
			intersects=true;
		      }
		      		      
		    }
		  
		  if ( intersects ) 
		    {
		      cout<<"new face intersection with face "<<(*candSearch).data->getID()<<endl;
		      cout<<p2t<<endl<<p3t<<endl<<p4t<<endl<<p5t<<endl;
		    }
		}
	      ++candSearch;
	    }
	  createdElement = !intersects;
	  
	  if ( !createdElement ) c++;
	}

      real t1 = getCPU();
      timing[creationInsertion_20]+=t1-t0;
      if (createdElement)
	{ // insert the new faces/edges 
	  ArraySimple<real> newVert(rangeDimension);
	  for ( int a=0; a<rangeDimension; a++ )
	    newVert(a) = new_candidates(c, a);
	  
	  int newVertID = addPoint(newVert);
	  ArraySimple<int> newFaceVerts(2);
	  newFaceVerts(0) = currentFace.getVertex(0);
	  newFaceVerts(1) = newVertID;
	  int id = insertFace(newFaceVerts, newElementID, -1);
	  if ( currentFace.getID()<initialFaceSurfaceMapping.getLength(0) && id<initialFaceSurfaceMapping.getLength(0))
	    initialFaceSurfaceMapping(id) = initialFaceSurfaceMapping(currentFace.getID());

	  newFaceVerts(0) = newVertID;
 	  newFaceVerts(1) = currentFace.getVertex(1);
	  id = insertFace(newFaceVerts, newElementID, -1);
	  if ( currentFace.getID()<initialFaceSurfaceMapping.getLength(0) && id<initialFaceSurfaceMapping.getLength(0))
	    initialFaceSurfaceMapping(id) = initialFaceSurfaceMapping(currentFace.getID());
	}
      real t2 = getCPU();
      timing[creationInsertion_21]+=t2-t1;
      
    }

  return createdElement;
}

//\begin{>>AdvancingFront.tex}{\subsection{makeTriTetFromExistingVertices}}
bool
AdvancingFront::
makeTriTetFromExistingVertices(const Face &currentFace, int newElementID, 
			       const vector<int> &existing_candidates, const vector<int> &local_nodes,
			       vector<PriorityQueue::iterator > &oldFrontFaces)
//===========================================================================
// /Purpose: attempt to make a new element (advance the front) with vertices that already exist.
// this method is called for triangles (2D) or tetrahera and pyramids (3D)
// /currentFace (input) : the in the front to be advanced
// /existing_candidates (input) : vertices in the front to consider 
// /oldFrontFaces (output) : list of vertices to remove from the front
//\end{AdvancingFront.tex}
//===========================================================================
{

  Range AXES(rangeDimension);

  // // // check existing candidates for a usable candidate ( includes the while loop below )  
  vector<int>::const_iterator exPt = existing_candidates.begin();

  //IntegerArray newFaceVerts(domainDimension); // number of vertices on a face is 2 in 2D, 3 in 3D
  ArraySimple<int> newFaceVerts(domainDimension);

  bool createNewFaceWithEdge[4]; // keeps track of new faces
  bool foundConsistentFaces[4]; // keeps track of how many consistent faces found
  bool createdElement = false; // was a new element created ?

  // loop through all the existing candidates and see if any will
  // create a new element with the current face.
  while(!createdElement && exPt!=existing_candidates.end())
    {
      // initialize boolean bookkeepers
      if (debug_af) cout<<"processing candidate "<<*exPt<<", there are "<<existing_candidates.size()<<endl;
      //cout<<"processing candidate "<<*exPt<<", there are "<<existing_candidates.size()<<endl;
      for ( int vf=0; vf<currentFace.getNumberOfVertices(); vf++ )
	{
	  createNewFaceWithEdge[vf] = false;
	  foundConsistentFaces[vf] = false;
	}

      // 2D check
      if ( domainDimension==2 )
	{
	  createdElement = false;
	  // first check to see if the face already exists
	  newFaceVerts(0) = *exPt;
	  newFaceVerts(1) = currentFace.getVertex(0);
	  int e = 0; // first vertex in the 2d face

	  bool isInFront = existsInFront(newFaceVerts);
	  if ( isInFront )
	    {
	      oldFrontFaces.push_back(getFrontIteratorForFace(newFaceVerts));
	      foundConsistentFaces[e]=true;
	    }
	  else if ( !isInFront )
	    { // try to create a new face
	      newFaceVerts(0) = currentFace.getVertex(0);
	      newFaceVerts(1) = *exPt;

	      foundConsistentFaces[e] = isFaceConsistent(newFaceVerts, currentFace);
	      createNewFaceWithEdge[e] = foundConsistentFaces[e];
	    }

	  // second vertex in the 2d face
	  if (foundConsistentFaces[e]) // only check if a valid face was made with the first vertex
	    {
	      newFaceVerts(0) = currentFace.getVertex(1);
	      newFaceVerts(1) = *exPt;

	      bool isInFront = existsInFront(newFaceVerts);
	      if ( isInFront )
		{ // this face exists in the front already
		  oldFrontFaces.push_back(getFrontIteratorForFace(newFaceVerts));
		  foundConsistentFaces[e+1] = true;
		} 
	      else if ( !isInFront ) {// try to make a new face
		newFaceVerts(0) = *exPt;
		newFaceVerts(1) = currentFace.getVertex(1);

		foundConsistentFaces[e+1] = isFaceConsistent(newFaceVerts, currentFace);
		createNewFaceWithEdge[e+1] = foundConsistentFaces[e+1];

	      } // if (existsInFront) .. else.. 
	      
	    } // foundConsistentFaces[e]

	  createdElement = foundConsistentFaces[e+1];

	  if ( createdElement )
	    {
	      // make sure none of the remaining existing candidates would be inside the triangle
	      int a;
	      ArraySimpleFixed<real,2,1,1,1> p1,p2,p3,pc;
	      //	      cout<<"p1,p2,p3"<<endl;
	      for ( a=0; a<2; a++ )
		{
		  p1[a] = xyz(currentFace.getVertex(0),a);
		  p3[a] = xyz(*exPt,a);
		  p2[a] = xyz(currentFace.getVertex(1),a);
		  //		  cout<<p1[a]<<" "<<p2[a]<<" "<<p3[a]<<endl;
		}
	      //	      cout<<"checking vertex in triangle "<<endl;
#if 0
	      for ( vector<int>::const_iterator epc=exPt+1; createdElement && epc!=existing_candidates.end(); epc++ )
		{
		  for ( a=0; a<2; a++ )
		    pc[a] = xyz(*epc,a);		  
		  createdElement = (orient2d(p1.ptr(),p2.ptr(),pc.ptr())>=0 && 
				     orient2d(p2.ptr(),p3.ptr(),pc.ptr())>=0 &&
				     orient2d(p3.ptr(),p1.ptr(),pc.ptr())>=0);
		  cout<<orient2d(p1.ptr(),p2.ptr(),pc.ptr())<<" "<<orient2d(p2.ptr(),p3.ptr(),pc.ptr())<<" "<<orient2d(p3.ptr(),p1.ptr(),pc.ptr())<<endl;
		}
#endif
	      for ( int vv=0; createdElement && vv<currentFace.getNumberOfVertices(); vv++ )
		{
		  const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[currentFace.getVertex(vv)];
		  for ( vector<PriorityQueue::iterator>::const_iterator f=v1Faces.begin(); createdElement && 
			  f!=v1Faces.end(); f++ )
		    if ( (**f)->getID()!=currentFace.getID() )
		      for ( int vvf=0; createdElement && vvf<(**f)->getNumberOfVertices(); vvf++ )
			if ( (**f)->getVertex(vvf)!=currentFace.getVertex(vv) && (**f)->getVertex(vvf)!=*exPt)
			  {
			    for ( a=0; a<2; a++ )
			      pc[a] = xyz((**f)->getVertex(vvf),a);	

			    //			    cout<<"pc : "<<pc[0]<<" "<<pc[1]<<endl;
			    createdElement = !(orient2d(p1.ptr(),p2.ptr(),pc.ptr())>=-REAL_EPSILON && 
					      orient2d(p2.ptr(),p3.ptr(),pc.ptr())>=-REAL_EPSILON &&
					      orient2d(p3.ptr(),p1.ptr(),pc.ptr())>=-REAL_EPSILON);

			    //cout<<orient2d(p1.ptr(),p2.ptr(),pc.ptr())<<" "<<orient2d(p2.ptr(),p3.ptr(),pc.ptr())<<" "<<orient2d(p3.ptr(),p1.ptr(),pc.ptr())<<endl;

			  }
		}

	      if ( !createdElement ) createNewFaceWithEdge[0] = createNewFaceWithEdge[1] = false;
	    }

	  if (!createdElement)
	    {
	      while ( oldFrontFaces.size()>1 ) oldFrontFaces.pop_back();		
	      foundConsistentFaces[e] = foundConsistentFaces[e+1] = false;
	      createNewFaceWithEdge[e] = createNewFaceWithEdge[e+1] = false;
	    }
	  

	  // now create and insert any new faces
	  if (createNewFaceWithEdge[0]) {
	    newFaceVerts(0) = currentFace.getVertex(0);
	    newFaceVerts(1) = *exPt;
	    insertFace(newFaceVerts, newElementID, -1);
	  }
	  if (createNewFaceWithEdge[1]) {
	    newFaceVerts(0) = *exPt;
	    newFaceVerts(1) = currentFace.getVertex(1);
	    insertFace(newFaceVerts, newElementID, -1);
	  }
	  
	} // if ( domainDimension == 2 )

      else 
	{ // 3D element creation (tetrahedron)
	  createdElement = true;
	  int e,ep;
 	//   cout<<"face vertices are ";
//  	  for ( e=0; e<currentFace.getNumberOfVertices(); e++ )
//  	    cout<<currentFace.getVertex(e)<<" ";

//  	  cout<<"attempting with candidate "<<*exPt<<endl;
//  	  cout<<endl;
	  int a;
	  ArraySimpleFixed<real,3,1,1,1> p1,p2,p3,pc;
	  for ( a=0; a<3; a++ )
	    {
	      p1[a] = xyz(currentFace.getVertex(0),a);
	      p2[a] = xyz(currentFace.getVertex(1),a);
	      p3[a] = xyz(currentFace.getVertex(2),a);
	    }
	  //	      cout<<"checking vertex in triangle "<<endl;
	  for ( a=0; a<3; a++ )
	    pc[a] = xyz(*exPt,a);
	  
	  //cout<<"volume of new tet "<<tetVolume(p1,p2,p3,pc)<<endl;
	  //ArraySimple<real> ppc(3); ppc[0]=pc[0];ppc[1]=pc[1]; ppc[2]=pc[2];
	  //cout<<"here's the bloody vertex \n"<<ppc<<endl;
	  createdElement = orient3d(p1.ptr(),p2.ptr(),p3.ptr(),pc.ptr())<-100*REAL_EPSILON; //0.0;
	  //cout<<"orientation of vertex "<<*exPt<<" is "<<orient3d(p1.ptr(),p2.ptr(),p3.ptr(),pc.ptr())<<endl;	 

	  for ( e = 0; e<currentFace.getNumberOfVertices() && createdElement; e++ )
	    { // loop through each edge in the face trying to make a triangular face
	      ep = (e+1)%currentFace.getNumberOfVertices();
	      newFaceVerts(0) = currentFace.getVertex(e);
	      newFaceVerts(1) = currentFace.getVertex(ep);
	      newFaceVerts(2) = *exPt;
	      
	      //cout<<"TRYING FACE WITH THESE VERTICES \n"<<newFaceVerts<<endl;
	      if ( existsInFront(newFaceVerts) // && 
		   ) 
		{ // face already exists
		  // cout<<"exists in front true"<<endl;
		  PriorityQueue::iterator f = getFrontIteratorForFace(newFaceVerts);

		  Face & checkface = **f;
		  int checkv = 0;
		  while ( checkface.getVertex(checkv)!=newFaceVerts(0) ) checkv++;
		  if ( checkface.getVertex((checkv-1+checkface.getNumberOfVertices())%checkface.getNumberOfVertices())
		       == currentFace.getVertex(ep) )
		    {
		      oldFrontFaces.push_back(f);//getFrontIteratorForFace(newFaceVerts));
		      foundConsistentFaces[e] = true;
		    }
		  else
		    createdElement = foundConsistentFaces[e] = false;
		}
	      else 
		{ // try to make a new face
		  foundConsistentFaces[e] = isFaceConsistent(newFaceVerts, currentFace);
		  createNewFaceWithEdge[e] = foundConsistentFaces[e];
		  createdElement = foundConsistentFaces[e];
		}
	    //   if ( createdElement )
// 		cout<<"created a valid face with "<<newFaceVerts(0)<<" "<<newFaceVerts(1)<<" "<<newFaceVerts(2)<<endl;
//  	      else
//  		cout<<"failed face with "<<newFaceVerts(0)<<" "<<newFaceVerts(1)<<" "<<newFaceVerts(2)<<endl;
	    }
	  
	  // insert any new faces only if a new element is created
#if 1
	  if ( createdElement )
	    {
	      int a;
	      ArraySimpleFixed<real,3,1,1,1> p1,p2,p3,pc;

#if 1
	      bool allpos;
	      for ( vector<int>::const_iterator epc=local_nodes.begin();
		    createdElement && epc!=local_nodes.end(); epc++ )
		{
		  
		  if ( *epc!=*exPt )
		    {
		      allpos = true;
		      for ( e = 0; e<currentFace.getNumberOfVertices() && allpos; e++ )
			{ 
			  
			  ep = (e+1)%currentFace.getNumberOfVertices();
			  for ( a=0; a<3; a++ )
			    {
			      p1[a] = xyz(currentFace.getVertex(e),a);
			      p2[a] = xyz(currentFace.getVertex(ep),a);
			      p3[a] = xyz(*exPt,a);
			      pc[a] = xyz(*epc,a);
			    }
			  
			  allpos = allpos && (orient3d(p1.ptr(),p2.ptr(),p3.ptr(),pc.ptr())>=0.);
			  
			}
		      
		      createdElement = !allpos;
		      if (!createdElement) cout<<"invalidated face with vertex "<<*epc<<endl;
		    }

		}
#else
	      for ( int vv=0; createdElement && vv<currentFace.getNumberOfVertices(); vv++ )
		{
		  
		  const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[currentFace.getVertex(vv)];
		  for ( vector<PriorityQueue::iterator>::const_iterator f=v1Faces.begin(); createdElement && 
			  f!=v1Faces.end(); f++ )
		    if ( (**f)->getID()!=currentFace.getID() )
		      for ( int vvf=0; createdElement && vvf<(**f)->getNumberOfVertices(); vvf++ )
			if ( (**f)->getVertex(vvf)!=*exPt )
			  {
			    for ( a=0; a<3; a++ )
			      pc[a] = xyz((**f)->getVertex(vvf),a);	

			    cout<<"pc : "<<pc[0]<<" "<<pc[1]<<endl;

			    createdElement = !(orient2d(p1.ptr(),p2.ptr(),pc.ptr())>=-REAL_EPSILON && 
					      orient2d(p2.ptr(),p3.ptr(),pc.ptr())>=-REAL_EPSILON &&
					      orient2d(p3.ptr(),p1.ptr(),pc.ptr())>=-REAL_EPSILON);

			    cout<<orient2d(p1.ptr(),p2.ptr(),pc.ptr())<<" "<<orient2d(p2.ptr(),p3.ptr(),pc.ptr())<<" "<<orient2d(p3.ptr(),p1.ptr(),pc.ptr())<<endl;

			  }
		}
#endif
	    }
#endif

	  if (createdElement)
	    {
	      //cout<<"volume of new tet "<<tetVolume(p1,p2,p3,pc)<<endl;
	      //ArraySimple<real> ppc(3); ppc[0]=pc[0];ppc[1]=pc[1]; ppc[2]=pc[2];
	      //cout<<"here's the bloody vertex \n"<<ppc<<endl;
	      
	      //cout<<"orientation of vertex "<<*exPt<<" is "<<orient3d(p1.ptr(),p2.ptr(),p3.ptr(),pc.ptr())<<endl;	      
	      for ( e = 0; e<currentFace.getNumberOfVertices(); e++ )
		{
		  if ( createNewFaceWithEdge[e] )
		    {
		      ep = (e+1)%currentFace.getNumberOfVertices();
		      newFaceVerts(0) = currentFace.getVertex(e);
		      newFaceVerts(1) = currentFace.getVertex(ep);
		      newFaceVerts(2) = *exPt;
		      insertFace(newFaceVerts, newElementID, -1);
		      //  cout<<"created an element with "<<*exPt<<endl;
		    }
		}
	    }
	  else // reset oldFrontFaces
	    while ( oldFrontFaces.size()>1 ) oldFrontFaces.pop_back();
	} // if 3d

      exPt++; 

    } // while ( !createdElement... )

  if ( debug_af && createdElement ) cout<<"created new element with vertex"<<*(exPt-1)<<endl;

  return createdElement;
}

//\begin{>>AdvancingFront.tex}{\subsection{makeTriTetFromNewVertex}}
bool
AdvancingFront::
//makeTriTetFromNewVertex(const Face & currentFace, int newElementID, realArray &new_candidates)
makeTriTetFromNewVertex(const Face & currentFace, int newElementID, ArraySimple<real> &new_candidates,
			vector<int> &local_nodes)
//===========================================================================
// /Purpose: attempt to make a new element (advance the front) by creating a new vertex
// this method is called for triangles (2D) or tetrahera and pyramids (3D)
// /currentFace (input) : the in the front to be advanced
// /new_candidates (input) : vertices in the front to consider 
//\end{AdvancingFront.tex}
//===========================================================================
{ 


  bool foundConsistentFaces[3]; // keeps track of how many consistent faces found
  bool createdElement = false; // was a new element created ?
  
  // initialize boolean bookkeepers
  for ( int vf=0; vf<currentFace.getNumberOfVertices(); vf++ )
    foundConsistentFaces[vf] = false;
   
  //realArray newFace(domainDimension, domainDimension);
  ArraySimple<real> newFace(rangeDimension, rangeDimension);

  //if (debug_af) new_candidates.display("new_candidates");
  if (debug_af) cout<<"New Candidates\n"<<new_candidates<<endl;
  
  if ( domainDimension==2 )
    {
      real t0 = getCPU();
      ArraySimple<real> p1(rangeDimension), p2(rangeDimension);
      int c = 0;
      // loop through all the new candidate vertices, look for a valid new element
      while( !createdElement && (c<1))//new_candidates.size(0)) )
	{
	  if (debug_af) cout<<"processing new candidate "<<c<<endl;
	  int a;
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      //newFace(0, a) = xyz(currentFace.getVertex(0),a);
	      //newFace(1, a) = new_candidates(c,a);
	      
	      p1(a) = xyz(currentFace.getVertex(0),a);
	      p2(a) = new_candidates(c,a);
	
	    }

	  createdElement = isFaceConsistent2D(p1, p2, currentFace.getID());//isFaceConsistent(newFace, currentFace);
	  
	  if ( debug_af && !createdElement ) cout<<" new element consistency failed at 1"<<endl;
	  if (createdElement) {
	    for ( a=0; a<rangeDimension; a++ )
	      {
		//newFace(0, a) = new_candidates(c,a);
		//newFace(1, a) = xyz(currentFace.getVertex(1),a);
	      
		p1(a) = new_candidates(c,a);
		p2(a) = xyz(currentFace.getVertex(1),a);
	      }
	    createdElement = isFaceConsistent2D(p1, p2, currentFace.getID());//iisFaceConsistent(newFace, currentFace);
	    if ( debug_af && !createdElement ) cout<<" new element consistency failed at 2"<<endl;

	  }
	  if ( !createdElement ) c++;
	}
      real t1 = getCPU();
      timing[creationInsertion_20]+=t1-t0;
      if (createdElement)
	{ // insert the new faces/edges 
	  ArraySimple<real> newVert(rangeDimension);
	  for ( int a=0; a<rangeDimension; a++ )
	    newVert(a) = new_candidates(c, a);

	  int newVertID = addPoint(newVert);
	  ArraySimple<int> newFaceVerts(2);
	  newFaceVerts(0) = currentFace.getVertex(0);
	  newFaceVerts(1) = newVertID;
	  insertFace(newFaceVerts, newElementID, -1);
	  newFaceVerts(0) = newVertID;
	  newFaceVerts(1) = currentFace.getVertex(1);
	  insertFace(newFaceVerts, newElementID, -1);
	}
      real t2 = getCPU();
      timing[creationInsertion_21]+=t2-t1;

    }
  else // 3D
    {
      int c = 0;
      // loop through all the candidate new vertices looking for a valid new element
      ArraySimpleFixed<real,3,1,1,1> p1,p2,p3,pc;
      while ( !createdElement && c<new_candidates.size(0) )
	{
	  pc[0] = new_candidates(c,0);
	  pc[1] = new_candidates(c,1);
	  pc[2] = new_candidates(c,2);
	  p1[0] = xyz(currentFace.getVertex(0),0);
	  p1[1] = xyz(currentFace.getVertex(0),1);
	  p1[2] = xyz(currentFace.getVertex(0),2);
	  p2[0] = xyz(currentFace.getVertex(1),0);
	  p2[1] = xyz(currentFace.getVertex(1),1);
	  p2[2] = xyz(currentFace.getVertex(1),2);
	  p3[0] = xyz(currentFace.getVertex(2),0);
	  p3[1] = xyz(currentFace.getVertex(2),1);
	  p3[2] = xyz(currentFace.getVertex(2),2);

	  createdElement = orient3d(p1.ptr(),p2.ptr(),p3.ptr(),pc.ptr())<-100*REAL_EPSILON;
	  //createdElement = true;
	  int e;
	  // see if a face can be created with the new vertex and all the edges in currentFace
	  for ( e=0; e<currentFace.getNumberOfVertices() && createdElement; e++ )
	    {
	      int ep = (e+1)%currentFace.getNumberOfVertices();
	      for ( int a=0; a<rangeDimension; a++ )
		{
		  newFace(0,a) = xyz(currentFace.getVertex(e),a);
		  newFace(1,a) = xyz(currentFace.getVertex(ep),a);
		  newFace(2,a) = new_candidates(c, a);
		}
	      createdElement = isFaceConsistent(newFace, currentFace);
	    }

	  c++;
	}

      if ( createdElement )
	createdElement = newElementVertexCheck(currentFace, local_nodes, pc);

      if ( createdElement ) 
	{ 
	  c--;
	  // add the new vertex and insert the new faces
	  ArraySimple<real> newVert(rangeDimension);
	  for ( int a=0; a<rangeDimension; a++ )
	    newVert(a) = new_candidates(c, a);

	  int newVertID = addPoint(newVert);
	  for ( int e=0; e<currentFace.getNumberOfVertices(); e++ )
	    {
	      ArraySimple<int> newFaceVerts(3);
	      int ep = (e+1)%currentFace.getNumberOfVertices();
	      newFaceVerts(0) = currentFace.getVertex(e);
	      newFaceVerts(1) = currentFace.getVertex(ep);
	      newFaceVerts(2) = newVertID;
	
	      insertFace(newFaceVerts, newElementID, -1);
	    } // for e
	}
      else 
	c++;
    } // 3d
  
  return createdElement;

}

bool
AdvancingFront::
isOnFacePlane( const Face & face, ArraySimple<real> &vertex )
{

  // return false if vertex is sitting on a plane defined by the vertices in face
  // (false otherwise)
  bool onFacePlane;

  ArraySimple<real> p1(3), p2(3), dummyCenter(3);

  if ( domainDimension == 2 )
    {
      real t0 = getCPU();

      for ( int a=0; a<domainDimension; a++ )
	{
	  p1(a) = xyz(face.getVertex(0), a);
	  p2(a) = xyz(face.getVertex(1), a);
	}
      onFacePlane = get_circle_center(p1,p2,vertex,dummyCenter);

      timing[getCircleCent] += getCPU() - t0;
    }
  else
    throw DimensionError();

  return onFacePlane;
}

bool
AdvancingFront::
checkVertexDirection( const Face & face, const ArraySimple<real> &vertex ) const
{
  // return true if the vertex is inside the "hole" of the advancing front
  // false otherwise
  bool faceDirectionOK=true;

  Range AXES(rangeDimension);
  if ( domainDimension == 2 )
    {
      int fv1 = face.getVertex(0);
      int fv2 = face.getVertex(1);
      ArraySimple<real> p1(rangeDimension),p2(rangeDimension);
      for ( int a=0; a<rangeDimension; a++ )
	{
	  p1[a] = xyz(fv1, a);
	  p2[a] = xyz(fv2, a);
	}
      faceDirectionOK = triangleArea2D(p1,p2,vertex)>10*REAL_EPSILON;
    }
  else
    throw DimensionError();

  return faceDirectionOK;
}

PriorityQueue::iterator
AdvancingFront::
getFrontIteratorForFace(const ArraySimple<int> &faceVertices)
{
  // return an iterator into the front that points to the face with vertices faceVertices
  // (vertices can be in reverse order)
  // similar to existsInFront

  Face currentFace(faceVertices, -1,-1,-1);

  const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[faceVertices(0)];

  bool matchFound = false;
  
  vector<PriorityQueue::iterator>::const_iterator f = v1Faces.begin();

  while ( f!=v1Faces.end() && !matchFound )
    {
      matchFound = faceVerticesAreSame( currentFace, ***f );
      if ( domainDimension==2 && rangeDimension==3 && matchFound )
	matchFound = faceVertices(1)==(**f)->getVertex(1);

      //      matchFound = faceVerticesAreSame( currentFace, ***f );
      if ( !matchFound ) f++;
    }

  return *f;

}

bool
AdvancingFront::
existsInFront(const ArraySimple<int> &faceVertices)
{
  // return true if a face with faceVertices exists (vertices can be in reverse order)
  // similar to getFrontIteratorForFace
  Face currentFace(faceVertices, -1,-1,-1);

  const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[faceVertices(0)];

  bool matchFound = false;
  
  vector<PriorityQueue::iterator>::const_iterator f = v1Faces.begin();
  // cout<<"checking "<<v1Faces.size()<<" faces around vertex "<<faceVertices(0)<<endl;
  while ( f!=v1Faces.end() && !matchFound )
    {
//       cout<<"comparing face vertices for "<<currentFace.getID()<<" and "<<(**f)->getID()<<endl;
//       for ( int v=0; v<currentFace.getNumberOfVertices(); v++ )
// 	cout<<currentFace.getVertex(v)<<" "<<(**f)->getVertex(v)<<endl;
      matchFound = faceVerticesAreSame( currentFace, ***f );

//kkc 070306 why is thsi here? conflicts with insenstivity to reverse order      if ( domainDimension==2 && rangeDimension==3 && matchFound )
//kkc 070306 why is thsi here? conflicts with insenstivity to reverse order  matchFound = faceVertices(1)==(**f)->getVertex(1);

      if ( !matchFound ) f++;
//       else
// 	cout<<"match found is true"<<endl; 
    }

  return matchFound;

}

bool
AdvancingFront::
existsInFront(const int p1, const int p2)
{
  // return true if a face with faceVertices exists (vertices can be in reverse order)
  // similar to getFrontIteratorForFace
  const vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[p1];

  bool matchFound = false;
  
  vector<PriorityQueue::iterator>::const_iterator f = v1Faces.begin();
  // cout<<"checking "<<v1Faces.size()<<" faces around vertex "<<faceVertices(0)<<endl;
  while ( f!=v1Faces.end() && !matchFound )
    {
//       cout<<"comparing face vertices for "<<currentFace.getID()<<" and "<<(**f)->getID()<<endl;
//       for ( int v=0; v<currentFace.getNumberOfVertices(); v++ )
// 	cout<<currentFace.getVertex(v)<<" "<<(**f)->getVertex(v)<<endl;
      for ( int v=0; v<(**f)->getNumberOfVertices() && !matchFound; v++ )
	matchFound = (**f)->getVertex(v)==p2;

      if ( !matchFound ) f++;
//       else
// 	cout<<"match found is true"<<endl; 
    }

  return matchFound;

}

bool
AdvancingFront::
//isFaceConsistent(const IntegerArray & newFace, const Face & face) //const 
isFaceConsistent(const ArraySimple<int> & newFace, const Face & face) //const 
{
  // check to see if a face with the vertices with indices into xyz
  // (specified by newFace) is consistent (has no intersections)
  
  real t0 = getCPU();

  bool isConsistent = true;

  if ( domainDimension==2 ) 
    {
    //if ( rangeDimension==2 )
#if 0
       isConsistent = isFaceConsistent2D(newFace(0), newFace(1), face.getID());
#else
      ArraySimple<real> p1(rangeDimension), p2(rangeDimension);
      for ( int a=0; a<rangeDimension; a++ )
	{
	  p1(a) = xyz(newFace(0),a);
	  p2(a) = xyz(newFace(1),a);
	}
      isConsistent = isFaceConsistent2D(p1, p2, face.getID());
#endif
  //else
  //isConsistent = isFaceConsistent2D_3R(newFace, face.getID());
    }
  else 
    isConsistent = isFaceConsistent3D(newFace, face);

  real t1 = getCPU();
  timing[int(intersections)] += t1-t0;
  //cout<<"intersectsions "<<t1-t0<<" "<<(t1-t0)/nFacesFront<<" "<<(t1-t0)/(nFacesFront*nFacesFront)<<endl;
  return isConsistent;
}

bool
AdvancingFront::
//isFaceConsistent(const realArray & newFace, const Face & face) //const 
isFaceConsistent(const ArraySimple<real> & newFace, const Face & face) //const 
{
  // check to see if a face with the vertices specified in newFace
  // is consistent (has no intersections)

  real t0 = getCPU();

  bool isConsistent = true;

  Range AXES(rangeDimension);
  if ( domainDimension==2 ) 
    {
      ArraySimple<real> p1(rangeDimension), p2(rangeDimension);
      for ( int a=0; a<rangeDimension; a++ )
	{
	  p1(a) = newFace(0,a);
	  p2(a) = newFace(1,a);
	}
      //if ( rangeDimension == 2 )
	isConsistent = isFaceConsistent2D(p1, p2, face.getID());
	//else
	//isConsistent = isFaceConsistent2D_3R(newFace,face.getID());
    }
  else 
    isConsistent = isFaceConsistent3D(newFace, face);

  real t1 = getCPU();
  timing[int(intersections)] += t1-t0;
  //cout<<"intersectsions "<<t1-t0<<" "<<(t1-t0)/nFacesFront<<" "<<(t1-t0)/(nFacesFront*nFacesFront)<<endl;

  return isConsistent;
}

bool 
AdvancingFront::
isFaceConsistent2D(int v1, int v2, int filterFace) const
{

  Index AXES(0,rangeDimension);
  
  // create the traversor for faceSearch
  ArraySimple<real> minmax(2*rangeDimension);
  for (int axis=0; axis<rangeDimension; axis++)
    {
      minmax(2*axis) = min(xyz(v1,axis), xyz(v2,axis));
      minmax(1+2*axis) = max(xyz(v1,axis), xyz(v2,axis));
    }
  
  GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);

  ArraySimple<real> p1(3),p2(3),p3(3),p4(3);
  int a;
  for ( a=0; a<rangeDimension; a++ )
    {
      p1(a) = xyz(v1,a);
      p2(a) = xyz(v2,a);
    }

  bool intersects = false;
  bool isParallel = false;
  while( ! candSearch.isFinished() )
    {
      if ((*candSearch).data->getID() != filterFace)
	{
	  int p3id = (*candSearch).data->getVertex(0);
	  int p4id = (*candSearch).data->getVertex(1);
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      p3(a) = xyz(p3id,a);
	      p4(a) = xyz(p4id,a);
	    }
	  intersects = intersect2D(p1,p2,p3,p4, isParallel);
	  
	  if (intersects) 
	    {
	      if (debug_af) cout <<"intersection found with "<<(*candSearch).data->getID()<<endl;
	      return false;
	    }
	  
	  if (isParallel)
	    {
	      // check to see if the parallel faces overlap each other
#if 0
	      if ( v1!=(*candSearch).data->getVertex(0) && v1!=(*candSearch).data->getVertex(1) )
		if ( isBetweenOpenInterval2D(p1,p2,p3) ) return false;

	      if ( v2!=(*candSearch).data->getVertex(0) && v2!=(*candSearch).data->getVertex(1) )
		if ( isBetweenOpenInterval2D(p1,p2,p4) ) return false;
	      if (isBetweenOpenInterval2D(p1,p2,p3) || isBetweenOpenInterval2D(p1,p2,p3)) {
		if (debug_af) cout<<"parallel-overlapping face found with "<<(*candSearch).data->getID()<<endl;
		return false;
	      }
	      else 
#endif
		isParallel = true;
	    }
	}
      ++candSearch;
    }

  // one last check, make sure the front would grow in the correct direction...

  if ( v1 != faces[filterFace]->getVertex(0) )
    return checkVertexDirection(*(faces[filterFace]), p1);
  else 
    return checkVertexDirection(*(faces[filterFace]), p2);
      
  return true;

}


bool
AdvancingFront::
isFaceConsistent2D(const ArraySimple<real> &p1, const ArraySimple<real> &p2, const int filterFace) const
{
  AssertException (!debug_af || 
		   (p1.size(0)==rangeDimension && p2.size(0)==rangeDimension), DimensionError());

  Index AXES(0,rangeDimension);

  ArraySimple<real> minmax(2*rangeDimension);
  // create the search traversor
  for (int axis=0; axis<rangeDimension; axis++)
    {
      minmax(2*axis) = min(p1(axis),p2(axis));
      minmax(1+2*axis) = max(p1(axis),p2(axis));
    }
  GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);

  ArraySimple<real> p3(3),p4(3);

  int a;

  bool intersects = false;
  bool isParallel = false;
  while( ! candSearch.isFinished() )
    {
      if ((*candSearch).data->getID() != filterFace)
	{
	  //	  cout << "checking intersection with "<<(*candSearch).data->getID()<<endl;
	  int p3id = (*candSearch).data->getVertex(0);
	  int p4id = (*candSearch).data->getVertex(1);
	  for ( a=0; a<rangeDimension; a++ )
	    {
	      p3(a) = xyz(p3id,a);
	      p4(a) = xyz(p4id,a);
	    }

// 	  if ( faces[filterFace]->getVertex(0)==p3id ) p3 = p1;
// 	  if ( faces[filterFace]->getVertex(0)==p4id ) p4 = p1;
// 	  if ( faces[filterFace]->getVertex(1)==p3id ) p3 = p2;
// 	  if ( faces[filterFace]->getVertex(1)==p4id ) p4 = p2;
	  
	  intersects = intersect2D(p1,p2,p3,p4,isParallel);

	  if (intersects) 
	    {
	      if ( debug_af) cout<<"intersection found with "<<(*candSearch).data->getID()<<endl;
	      return false;
	    }
	  if (isParallel)
	    {
	      if (isBetweenOpenInterval2D(p3,p4,p1) || isBetweenOpenInterval2D(p3,p4,p2)) {
		if (debug_af) cout<<"parallel-overlapping face found with "<<(*candSearch).data->getID()<<endl;
		return false;
	      }
	      else 
		isParallel = true;
	      
	    }
	}
      ++candSearch;
    }

  ArraySimple<real> xyz1(rangeDimension);
  ArraySimple<real> xyz2(rangeDimension);
  for ( a=0; a<rangeDimension; a++ )
    {
      xyz1(a) = xyz(faces[filterFace]->getVertex(0), a);
      xyz2(a) = xyz(faces[filterFace]->getVertex(1), a);
    }

  // one last check, make sure the front would grow in the correct direction...
  double area1 = triangleArea2D(xyz1, xyz2, p1);
  double area2 = triangleArea2D(xyz1, xyz2, p2);

  if ( debug_af ) cout<<" intersects is "<< (intersects ? "true" : "false")<<endl;
  if ( debug_af ) cout<<" isFaceConsistent2D areas "<<area1<<"  "<<area2<<endl;
  if ( debug_af ) cout<<" abs values of areas is "<<fabs(area1)<<"  "<<fabs(area2)<<endl;
  if (fabs(area1)>fabs(area2))
    {    
      if (area1<0.0) return false;
    }
  else
    if (area2<0.0) return false;

  return true;
}

static inline bool neitherVertexInFace(int &v1, int &v2, ArraySimple<int> triVerts)
{
  bool v1in = v1==triVerts[0] || v1==triVerts[1] || v1==triVerts[2];
  bool v2in = v2==triVerts[0] || v2==triVerts[1] || v2==triVerts[2];

  return !(v1in && v2in);
#if 0
  return (v1!=triVerts[0] && v1!=triVerts[1] && v1!=triVerts[2] &&
	  v2!=triVerts[0] && v2!=triVerts[1] && v2!=triVerts[2] );
#endif
}

static inline bool neitherVertexInFace(ArraySimple<real> &v1, ArraySimple<real> &v2, ArraySimple<real> triVerts)
{
#if 0
  int dim = v1.size(0);
  ArraySimple<real> v1diff1(dim), v1diff2(dim), v1diff3(dim);
  ArraySimple<real> v2diff1(dim), v2diff2(dim), v2diff3(dim);

  for ( int axis=0; axis<v1.size(0); axis++ )
    {
      v1diff1[axis] = v1[axis]-triVerts(0,axis);
      v1diff2[axis] = v1[axis]-triVerts(1,axis);
      v1diff3[axis] = v1[axis]-triVerts(2,axis);
      v2diff1[axis] = v2[axis]-triVerts(0,axis);
      v2diff2[axis] = v2[axis]-triVerts(1,axis);
      v2diff3[axis] = v2[axis]-triVerts(2,axis);
    }

  real v1diff = min( ASmag2(v1diff1), ASmag2(v1diff2), ASmag2(v1diff3) );
  real v2diff = min( ASmag2(v2diff1), ASmag2(v2diff2), ASmag2(v2diff3) );

  return !(fabs(v1diff)<100*FLT_MIN || fabs(v2diff)<100*FLT_MIN);
#else
  return true;
#endif
}

bool 
AdvancingFront::
isFaceConsistent3D(const ArraySimple<int> &facev, const Face & filterFace)
{
 
  Face candidateFace(facev, -1,-1,-1);
  int v,a;
  ArraySimple<real> minmax(2*rangeDimension), faceNormal(3), faceVertices(facev.size(0),3);
  real angle;
  // create the search traversor
  int axis;

  for (axis=0; axis<rangeDimension; axis++ )
    {
      minmax(2*axis) = REAL_MAX;
      minmax(2*axis+1) = -REAL_MAX;
    }

  real da = 100*FLT_EPSILON;

  for ( v=0; v<facev.size(0); v++ )
    {
      for (axis=0; axis<rangeDimension; axis++)
	{
	  faceVertices(v,axis) = xyz(facev(v),axis);
	  minmax(2*axis) = min(minmax(2*axis),xyz(facev(v),axis));
	  minmax(1+2*axis) = max(minmax(2*axis+1),xyz(facev(v),axis));

	  da = max(da,(minmax(2*axis+1)-minmax(2*axis)));
	}
    }

  computeFaceNormal(faceVertices, faceNormal);

#if 1
  //real da=0.;
  for (axis=0; axis<rangeDimension; axis++)
    {
      // pad the bounding box by a little bit to make sure we get faces that are 
      //   right on the edge of this bounding box...
      //da = max(100*FLT_EPSILON,0.01*(minmax(2*axis+1)-minmax(2*axis)));
      minmax(2*axis) -= da;
      minmax(2*axis+1) += da;
    }
#endif

  GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);

  real flatTol = 0.;//100*FLT_EPSILON;
  
  bool intersects = false;
  bool isParallel = false;
  //  ArraySimple<real> p1(3), p2(3), ev(3),ep(3), en(3);
//   ArraySimple<real> triVertices(3,3);
//   ArraySimple<int> triFace(3);
//   ArraySimple<real> candNormal(3);

  ArraySimpleFixed<real,3,1,1,1> p1, p2, p3, ev,ep, en;
  ArraySimpleFixed<real,3,3,1,1> triVertices;
  ArraySimpleFixed<int,3,1,1,1> triFace;
  ArraySimpleFixed<real,3,1,1,1> candNormal;

  int ncand =0;
  real can = cos( acos(-1.)*(1./2.-parameters.getAuxiliaryAngleTolerance()/180.));
  flatTol = can;
  
  while( ! candSearch.isFinished() )
    {
      const Face *face = (*candSearch).data;
      ncand++;
      int v1,v2,vv1,vv2;
      //cout<<"a possible candidate is "<<face->getID()<<" filtered by "<<filterFace.getID()<<endl;
      if (face->getID() != filterFace.getID())
	{
	  //cout<<"checking face "<<face->getID()<<endl;
	  // 
	  // first check to make sure that none of the potential new edges intersect other
	  //    faces in the front (split quads into two triangles to check for intersections)
	  //    only check the "new" edge
	  for ( v=0; v<3; v++ )
	    {
	      for ( a=0; a<3; a++ )
		triVertices(v,a) = xyz( face->getVertex(v), a );
	      triFace(v) = face->getVertex(v);
	    }

	  //computeFaceNormal(triVertices,candNormal);

	  //if ( !auxiliaryCheck(candNormal,faceNormal) ) return false;
	  for ( v=1; v<(facev.size(0)) && !intersects; v++ )
	    { // start at 1 to only consider the new edges (facev(facev.size(0)-1) is the candidate)
	      v1 = facev[v];
	      v2 = facev[(v+1)%facev.size(0)];
	      //bool notanedge = true;
	      bool needtocheck = true;

	      needtocheck = !existsInFront(v1,v2); //true;//neitherVertexInFace(v1,v2,triFace);
	      //cout<<v1<<" "<<v2<<"\ntriFace\n"<<triFace<<endl;
	      if ( needtocheck )
		{
		  for ( a=0; a<3; a++ )
		    {
		      p1[a] = xyz( facev(v), a );
		      p2[a] = xyz( facev((v+1)%facev.size(0)), a );
		      p3[a] = xyz( facev((v-1+facev.size(0))%facev.size(0)),a );
		    }

		  intersects = intersect3D( triVertices, p1, p2, isParallel, angle, flatTol );
		  //		  if ( !intersects && angle!=REAL_MAX ) 
		  //		    intersects = 180.*(angle)/M_PI>parameters.getAuxiliaryAngleTolerance();

		  //if ( isParallel ) cout<<"edge on face plane 1"<<endl;
		  //			  intersects =  angle<cos( acos(-1.)*(90-parameters.getMaxNeighborAngle())/180.);
#if 0

		  if ( !intersects && angle>0. && !isParallel )
		    {
		      // check the adjacent face angle (jan and tanner aux test II).

		      vv1 = -1;
		      for ( int vv=0; vv<face->getNumberOfVertices(); vv++ )
			if ( v1==face->getVertex(vv) || v2==face->getVertex(vv) ) vv1=vv;
		      
		      // use orient 3d to see if the edge is above the triangle

		      if ( face->getVertex(vv1)==v1 )
			for ( a=0; a<3; a++ )
			  ev[a] = p2[a];
		      else
			for ( a=0; a<3; a++ )
			  ev[a] = p1[a];
		     
		      real len = max(fabs(p2[0]-p1[0]), fabs(p2[1]-p1[1]), fabs(p2[2]-p1[2]));
		      
		      for ( a=0; a<3; a++ )
			{
			  ep[a] = xyz(face->getVertex((vv1+1)%face->getNumberOfVertices()),a);
			  en[a] = ev[a] + len*faceNormals(face->getID(),a);
			}
			  
		      
		      bool isabove = false;
		      if ( face->getVertex(vv1)==v1 )
			isabove = orient3d(p1.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;
		      else
			isabove = orient3d(p2.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;

		      for ( a=0; a<3; a++ )
			{
			  ep[a] = 
			    xyz(face->getVertex((vv1-1+face->getNumberOfVertices())%face->getNumberOfVertices()),a);
			}

		      if ( face->getVertex(vv1)==v1 )
			isabove = isabove && orient3d(p1.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;
		      else
			isabove = isabove && orient3d(p2.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;

		      //isabove = isabove && orient3d(p1.ptr(),p2.ptr(),p3.ptr(),ev.ptr())<0;

		      intersects = isabove && angle<can;

		      //		      if ( intersects ) cout<<"angle, can "<<angle<<" "<<can<<endl;
     
		    }
#endif
		}
	    } 

	  if ( intersects ) 
	    {
	  //     cout<<"found intersection 1"<<endl;
	      return false;
	    }

	  // second quad triangle
	  if ( face->getNumberOfVertices()==4 ) 
	    {

	      //cout<<"checking quad triangle with vertices "<<triFace[0]<<" "<<triFace[1]<<" "<<triFace[2]<<endl;
	      Face triF(triFace, -1,-1,-1);
	      if ( faceVerticesAreSame(candidateFace, triF) )
		return false;

	      for ( v=1; v<4; v++ )
		{
		  triFace(v-1) = face->getVertex(v);
		}
	      
	      //cout<<"checking quad triangle with vertices "<<triFace[0]<<" "<<triFace[1]<<" "<<triFace[2]<<endl;
	      Face triF2(triFace,-1,-1,-1);
	      if ( faceVerticesAreSame(candidateFace, triF2) )
		return false;

	      // check the remaining quad to triangle splitting possibilities...
	      for ( v=2; v<5; v++ )
		{
		  triFace(v-2) = face->getVertex(v%4);
		  for ( a=0; a<3; a++ )
		    triVertices(v-2,a) = xyz( face->getVertex(v%4), a );
		}
	      
	      //cout<<"checking quad triangle with vertices "<<triFace[0]<<" "<<triFace[1]<<" "<<triFace[2]<<endl;
	      //cout<<"checking quad triangle with vertices "<<triFace[0]<<" "<<triFace[1]<<" "<<triFace[2]<<endl;
	      Face triF3(triFace,-1,-1,-1);
	      if ( faceVerticesAreSame(candidateFace, triF3) )
		return false;

	      for ( v=1; v<(facev.size(0)) && !intersects; v++ )
		{
		  v1 = facev[v];
		  v2 = facev[(v+1)%facev.size(0)];

		  bool needtocheck = !existsInFront(v1,v2);//true;//neitherVertexInFace(v1,v2,triFace);
		  if ( needtocheck ) //notanedge && vv1!=facev(2) && vv2!=facev(2) )
		    {
		      for ( a=0; a<3; a++ )
			{
			  p1[a] = xyz( facev(v), a );
			  p2[a] = xyz( facev((v+1)%facev.size(0)), a );
			}
		      intersects = intersect3D( triVertices, p1, p2, isParallel, angle, flatTol );

		      //			  intersects =  angle<cos( acos(-1.)*(90-parameters.getMaxNeighborAngle())/180.);
// 		      if ( !intersects && angle!=-1 && !isParallel )
// 			{
// 			  intersects = intersect3D( triVertices, p1,p2,isParallel, angle, cos( acos(-1.)*89/180.));
// 			}
#if 0
		      if ( !intersects && angle>0. && !isParallel )
			{
			  // check the adjacent face angle (jan and tanner aux test II).
			  
			  vv1 = -1;
			  for ( int vv=0; vv<face->getNumberOfVertices(); vv++ )
			    if ( v1==face->getVertex(vv) || v2==face->getVertex(vv) ) vv1=vv;
			  
			  // use orient 3d to see if the edge above the triangle
			  
			  if ( face->getVertex(vv1)==v1 )
			    for ( a=0; a<3; a++ )
			      ev[a] = p2[a];
			  else
			    for ( a=0; a<3; a++ )
			      ev[a] = p1[a];
			  
			  real len = max(fabs(p2[0]-p1[0]), fabs(p2[1]-p1[1]), fabs(p2[2]-p1[2]));

			  for ( a=0; a<3; a++ )
			    {
			      ep[a] = xyz(face->getVertex((vv1+1)%face->getNumberOfVertices()),a);
			      en[a] = ev[a] + len*faceNormals(face->getID(),a);
			    }
			  
			  bool isabove = false;
			  if ( face->getVertex(vv1)==v1 )
			    isabove = orient3d(p1.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;
			  else
			    isabove = orient3d(p2.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;
			  
			  for ( a=0; a<3; a++ )
			    {
			      ep[a] = 
				xyz(face->getVertex((vv1-1+face->getNumberOfVertices())%face->getNumberOfVertices()),a);
			    }
			  
			  if ( face->getVertex(vv1)==v1 )
			    isabove = isabove && orient3d(p1.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;
			  else
			    isabove = isabove && orient3d(p2.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;
			  
			  intersects = isabove && angle<can;

			  //			  if ( isabove ) cout<<"angle, can (2) "<<angle<<" "<<can<<endl;
			}
#endif		      
		      //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle();
		      //if ( isParallel ) cout<<"edge on face plane 2"<<endl;
		    }
		}

	      for ( v=3; v<6; v++ )
		{
		  triFace(v-3) = face->getVertex(v%4);
		}
	      
	      //cout<<"checking quad triangle with vertices "<<triFace[0]<<" "<<triFace[1]<<" "<<triFace[2]<<endl;
	      Face triF4(triFace,-1,-1,-1);
	      if ( faceVerticesAreSame(candidateFace, triF4) )
		return false;

	      // check to make sure that the new edge is not diagonal accross the quadrilateral face
	      for ( v=1; v<(facev.size(0)) && !intersects; v++ )
		{
		  v1 = facev[v];
		  v2 = facev[(v+1)%facev.size(0)];
		  vv1 = vv2 = -1;
		  for ( int vv=0; vv<face->getNumberOfVertices(); vv++ )
		    {
		      if ( v1==face->getVertex(vv) ) vv1=vv;
		      if ( v2==face->getVertex(vv) ) vv2=vv;

		      if ( vv1!=-1 && vv2!=-1 )
			if ( abs(vv1-vv2)==2 ) return false;
		    }
		}
		  
	    }

	  if ( intersects )
	    {
	     //  cout<<"found intersection 2"<<endl;
	      return false;
	    }

	  // 
	  // now make sure that none of the candidate's edges intersect the potential face
	  //   (again splitting quad faces into two triangles)
	  for ( v=0; v<3; v++ )
	    for ( a=0; a<3; a++ )
	      triVertices(v,a) = xyz( facev(v),a );
	  
	  for ( v=0; v<face->getNumberOfVertices() && !intersects; v++ )
	    {
	      v1 = face->getVertex(v);
	      v2 = face->getVertex((v+1)%face->getNumberOfVertices());

	      //if ( v1!=facev(2) && v2!=facev(2))
	      if ( true )//neitherVertexInFace(v1,v2,facev))
		{
		  for ( a=0; a<3; a++ )
		    {
		      p1[a] = xyz( face->getVertex(v), a );
		      p2[a] = xyz( face->getVertex((v+1)%face->getNumberOfVertices()), a );
		    }
		  
		  intersects = intersect3D( triVertices, p1, p2, isParallel, angle, flatTol );
		  //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle();
		  //if ( isParallel ) cout<<"edge on face plane 3"<<endl;
		}
	    }

	  if ( intersects ) 
	    {
	      // cout<<"found intersection 3"<<endl;
	      return false;
	    }

	  if ( facev.size(0)==4 )
	    {
	      for ( v=2; v<5; v++ )
		for ( a=0; a<3; a++ )
		  triVertices(v-2,a) = xyz( facev(v%facev.size(0)), a );
	      
	      for ( v=0; v<face->getNumberOfVertices() && !intersects; v++ )
		{
		  v1 = face->getVertex(v);
		  v2 = face->getVertex((v+1)%face->getNumberOfVertices());
		  //bool notanedge = true;
		  //if ( v1!=facev(2) && v2!=facev(2))
		  if ( true )//neitherVertexInFace(v1,v2,facev) )
		    {
		      for ( a=0; a<3; a++ )
			{
			  p1[a] = xyz( face->getVertex(v), a);
			  p2[a] = xyz( face->getVertex((v+1)%face->getNumberOfVertices()),a );
			}
		      intersects = intersect3D( triVertices, p1, p2, isParallel, angle, flatTol );
		    }
		}

	      if (intersects) 
	      {
		// cout<<"found intersection 4"<<endl;
		return false;
	      }

	    }
	}
      ++candSearch;
    }
  
  //cout<<"number checked "<<ncand<<endl;

  return !intersects;
}

bool 
AdvancingFront::
isFaceConsistent3D(const ArraySimple<real> & facev, const Face & filterFace)
{

  ArraySimple<real> minmax(2*rangeDimension), faceNormal(3), faceVertices(facev.size(0),3);
  ArraySimple<real> candNormal(3);

  // create the search traversor
  int axis,v,a;
  
  for (axis=0; axis<rangeDimension; axis++ )
    {
      minmax(2*axis) = REAL_MAX;
      minmax(2*axis+1) = -REAL_MAX;
    }
  
  real da = 100*FLT_EPSILON;
  for ( v=0; v<facev.size(0); v++ )
    for (axis=0; axis<rangeDimension; axis++)
      {
	faceVertices(v,axis) = facev(v,axis);
	minmax(2*axis) = min(minmax(2*axis),facev(v,axis));
	minmax(1+2*axis) = max(minmax(2*axis+1),facev(v,axis));

	da = max(da,(minmax(2*axis+1)-minmax(2*axis)));
      }

  computeFaceNormal(faceVertices, faceNormal);
  for (axis=0; axis<rangeDimension; axis++)
    {
      // pad the bounding box by a little bit to make sure we get faces that are 
      //   right on the edge of this bounding box...
      // da = max(100*FLT_EPSILON,0.01*(minmax(2*axis+1)-minmax(2*axis)));
      minmax(2*axis) -= da;
      minmax(2*axis+1) += da;
    }

  GeometricADT<Face*>::traversor candSearch(faceSearcher, minmax);
  real angle;
  
  bool intersects = false;
  bool isParallel = false;
//   ArraySimple<real> p1(3), p2(3);
//   ArraySimple<real> triVertices(3,3);

  ArraySimpleFixed<real,3,1,1,1> p1, p2;
  ArraySimpleFixed<real,3,3,1,1> triVertices;

  int ncand =0;
  real can = cos( acos(-1.)*(1./2.-parameters.getAuxiliaryAngleTolerance()/180.));
  real flatTol = can;

  while( ! candSearch.isFinished() )
    {
      const Face *face = (*candSearch).data;
      ncand++;
      
      if (face->getID() != filterFace.getID())
	{
	  for ( v=0; v<3; v++ )
	    {
	      for ( a=0; a<3; a++ )
		triVertices(v,a) = xyz( face->getVertex(v), a );
	    }

	  //computeFaceNormal(triVertices,candNormal);

	  //if ( !auxiliaryCheck(candNormal,faceNormal) ) return false;

	  for ( v=1; v<(facev.size(0)) && !intersects; v++ )
	    { // start at 1 to only consider the new edges (facev(facev.size(0)-1) is the candidate)
	      	      
	      for ( a=0; a<3; a++ )
		{
		  p1[a] = facev(v,a);
		  p2[a] = facev((v+1)%facev.size(0),a );
		}
	      //	      bool needtocheck = true;//!existsInFront(v1,v2);//true;//neitherVertexInFace(p1,p2,triVertices);
	      
	      //	      if ( needtocheck )//notanedge && vv1!=facev(2) && vv2!=facev(2))
	      //		{
	      intersects = intersect3D( triVertices, p1, p2, isParallel, angle, flatTol );
	      //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle();
	      //if ( isParallel ) cout<<"edge on face plane 1"<<endl;
	      //		}

#if 0
	      if ( !intersects && angle>0. && angle<can && !isParallel )
		{
		  // check the adjacent face angle (jan and tanner aux test II).

		  vv1 = -1;
		  for ( int vv=0; vv<face->getNumberOfVertices(); vv++ )
		    if ( v1==face->getVertex(vv) || v2==face->getVertex(vv) ) vv1=vv;
		      
		  // use orient 3d to see if the edge is above the triangle

		  if ( face->getVertex(vv1)==v1 )
		    for ( a=0; a<3; a++ )
		      ev[a] = p2[a];
		  else
		    for ( a=0; a<3; a++ )
		      ev[a] = p1[a];
		     
		  real len = max(fabs(p2[0]-p1[0]), fabs(p2[1]-p1[1]), fabs(p2[2]-p1[2]));
		      
		  for ( a=0; a<3; a++ )
		    {
		      ep[a] = xyz(face->getVertex((vv1+1)%face->getNumberOfVertices()),a);
		      en[a] = ev[a] + len*faceNormals(face->getID(),a);
		    }
			  
		      
		  bool isabove = false;
		  if ( face->getVertex(vv1)==v1 )
		    isabove = orient3d(p1.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;
		  else
		    isabove = orient3d(p2.ptr(), ep.ptr(), en.ptr(), ev.ptr())>0;

		  for ( a=0; a<3; a++ )
		    {
		      ep[a] = 
			xyz(face->getVertex((vv1-1+face->getNumberOfVertices())%face->getNumberOfVertices()),a);
		    }

		  if ( face->getVertex(vv1)==v1 )
		    isabove = isabove && orient3d(p1.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;
		  else
		    isabove = isabove && orient3d(p2.ptr(), en.ptr(), ep.ptr(), ev.ptr())>0;

		      //isabove = isabove && orient3d(p1.ptr(),p2.ptr(),p3.ptr(),ev.ptr())<0;

		  intersects = isabove && angle<can;

		  //		      if ( intersects ) cout<<"angle, can "<<angle<<" "<<can<<endl;
     
		}
#endif
	    }

	  if ( intersects ) 
	    {
	   //    cout<<"found intersection 1"<<endl;
	      return false;
	    }

	  // second quad triangle
	  if ( face->getNumberOfVertices()==4 ) 
	    {

	      // check the remaining quad to triangle splitting possibilities...
	      for ( v=2; v<5; v++ )
		{
		  for ( a=0; a<3; a++ )
		    triVertices(v-2,a) = xyz( face->getVertex(v%4), a );
		}
	      
	      for ( v=1; v<(facev.size(0)) && !intersects; v++ )
		{

		  for ( a=0; a<3; a++ )
		    {
		      p1[a] = facev(v,a);
		      p2[a] = facev((v+1)%facev.size(0),a );
		    }
		  bool needtocheck = true;//neitherVertexInFace(p1,p2,triVertices);
	      
		  if ( needtocheck )//notanedge && vv1!=facev(2) && vv2!=facev(2))
		    {
		      intersects = intersect3D( triVertices, p1, p2, isParallel, angle );
		      //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle(); 
		      //if ( isParallel ) cout<<"edge on face plane 1"<<endl;
		    }
		}
	    }

	  if ( intersects )
	    {
	    //   cout<<"found intersection 2"<<endl;
	      return false;
	    }

	  // 
	  // now make sure that none of the candidate's edges intersect the potential face
	  //   (again splitting quad faces into two triangles)
	  for ( v=0; v<3; v++ )
	    for ( a=0; a<3; a++ )
	      triVertices(v,a) = facev(v,a);
	  
	  for ( v=0; v<face->getNumberOfVertices() && !intersects; v++ )
	    {

	      for ( a=0; a<3; a++ )
		{
		  p1[a] = xyz( face->getVertex(v), a );
		  p2[a] = xyz( face->getVertex((v+1)%face->getNumberOfVertices()), a );
		}
	      
	      if ( true )//neitherVertexInFace(p1,p2,triVertices) )
		{
		  intersects = intersect3D( triVertices, p1, p2, isParallel, angle );
		  //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle(); 
		  //if ( isParallel ) cout<<"edge on face plane 3"<<endl;
		}
	    }

	  if ( intersects ) 
	    {
	    //   cout<<"found intersection 3"<<endl;
	      return false;
	    }

	  if ( facev.size(0)==4 )
	    {
	      for ( v=2; v<5; v++ )
		for ( a=0; a<3; a++ )
		  triVertices(v-2,a) = facev(v%4, a);
	      
	      for ( v=0; v<face->getNumberOfVertices() && !intersects; v++ )
		{
		  for ( a=0; a<3; a++ )
		    {
		      p1[a] = xyz( face->getVertex(v), a );
		      p2[a] = xyz( face->getVertex((v+1)%face->getNumberOfVertices()), a );
		    }
		  
		  if ( true )//neitherVertexInFace(p1,p2,triVertices) )
		    {
		      intersects = intersect3D( triVertices, p1, p2, isParallel, angle );
		      //if ( !intersects ) intersects = (90-angle)>parameters.getMaxNeighborAngle(); 
		      //if ( isParallel ) cout<<"edge on face plane 3"<<endl;
		    }
		}

	      if (intersects) 
	      {
		// cout<<"found intersection 4"<<endl;
		return false;
	      }

	    }
	}
      ++candSearch;
    }

  return !intersects;

}

void
AdvancingFront::
computeFaceTransformation(const Face &face, ArraySimple<real> &T, ArraySimple<real> &Tinv)
{

  // compute the stretching based transformation
  
  real t0 = getCPU();
  Range AXES(0,rangeDimension-1);
  int a,a1;
  for ( a=0; a<rangeDimension; a++ )
    for ( a1=0; a1<rangeDimension; a1++ )
      {
	T(a,a1) = 0.0;
	Tinv(a,a1) = 0.0;
      }

  realArray mid(1,rangeDimension);
  realArray faceVertices(face.getNumberOfVertices(), rangeDimension);
  mid = 0.0;

  for ( int fv=0; fv<face.getNumberOfVertices(); fv++ )
    {
      mid(0,AXES) += xyz(face.getVertex(fv),AXES)/real(face.getNumberOfVertices());
      faceVertices(fv, AXES) = xyz(face.getVertex(fv),AXES);
    }

  mid.reshape(AXES);

  double uniformStretchingFactor = parameters.getEdgeGrowthFactor(); // this should be a parameter
  ArraySimple<real> T1(rangeDimension, rangeDimension);

  real t1 = getCPU();
  timing[faceTrans_1] += t1-t0;
  if ( uniformStretchingFactor >= 1.0 )
    computeFaceNormalTransformation(faceVertices, T1, uniformStretchingFactor);

  real t2 = getCPU();
  timing[faceTrans_2] += t2-t1;
  if ( parameters.usingControlFunction() ) computeTransformationAtPoint(mid, T);
  real t3 = getCPU();
  timing[faceTrans_3] += t3-t2;

  if ( parameters.usingControlFunction() && uniformStretchingFactor >= 1.0 )
    {
      for ( a=0; a<rangeDimension; a++ )
	for ( a1=0; a1<rangeDimension; a1++ )
	  {
	    T(a,a1) = ( T(a,a1)+T1(a,a1) )/2;
	  }
    }
  else if ( uniformStretchingFactor >= 1.0 )
    {
      T = T1;
    }

  if ( rangeDimension==2 )
    {
      double det = T(0,0)*T(1,1) - T(0,1)*T(1,0);
      
      AssertException (det!=0.0, AdvancingFrontError());
      
      Tinv(0,0) = T(1,1)/det;
      Tinv(0,1) = -T(1,0)/det;
      Tinv(1,0) = -T(0,1)/det;
      Tinv(1,1) = T(0,0)/det;
    }
  else if ( rangeDimension==3 ) 
    {
      double det = T(0,0)*(T(1,1)*T(2,2)-T(1,2)*T(2,1)) - T(0,1)*(T(1,0)*T(2,2)-T(1,2)*T(2,0)) +
	T(0,2)*(T(1,0)*T(2,1)-T(1,1)*T(2,0));

      AssertException (det!=0.0, AdvancingFrontError());
      Tinv(0,0) = (T(1,1)*T(2,2)-T(1,2)*T(2,1))/det;
      Tinv(1,0) = -(T(1,0)*T(2,2)-T(1,2)*T(2,0))/det;
      Tinv(2,0) = (T(1,0)*T(2,1)-T(1,1)*T(2,0))/det;
      Tinv(0,1) = -(T(0,1)*T(2,2)-T(0,2)*T(2,1))/det;
      Tinv(1,1) = (T(0,0)*T(2,2)-T(0,2)*T(2,0))/det;
      Tinv(2,1) = -(T(0,0)*T(2,1)-T(0,1)*T(2,0))/det;
      Tinv(0,2) = (T(0,1)*T(1,2)-T(0,2)*T(1,1))/det;
      Tinv(1,2) = -(T(0,0)*T(1,2)-T(0,2)*T(1,0))/det;
      Tinv(2,2) = (T(0,0)*T(1,1)-T(0,1)*T(1,0))/det;
      
    }

  real t4 = getCPU();
  timing[faceTrans_4] += t4-t3;
}

void
AdvancingFront::
computeFaceNormalTransformation(const realArray &vertices, ArraySimple<real> &T, double stretch /*=1.0*/)
{

  // compute a stretched transformation from the face and normal stretching specified
  // the resulting transformation will favour growth normal to the face with a normal stretching stretch*IdealLength
  // where IdealLength would be the length if the resultant element were to be uniform.

  Range AXES(rangeDimension);

  double mag = 0.0;
  // use edge length as basic distance in 2d/ average edge length in 3d
  for ( int v=0; v<vertices.getLength(0); v++ )
    mag += sqrt(sum(pow(vertices((v+1)%vertices.getLength(0),AXES) -
			vertices(v,AXES),2)));
  
  mag /= ( rangeDimension==2 ) ? real(2) : real(vertices.getLength(0));
  //if (rangeDimension==3) mag /= real(vertices.getLength(0));
  
  double height = stretch*sqrt(3.0)*mag/2.0;

  realArray tang(1,rangeDimension);
  realArray norm(rangeDimension);
  for ( int a=0; a<rangeDimension; a++ )
    for ( int aa=0; aa<rangeDimension; aa++ )
      T(aa,a) = 0.;

  tang = (vertices(vertices.getLength(0)/2,AXES) - vertices(0,AXES));
  tang.reshape(AXES);
  tang = tang/sqrt(sum(pow(tang,2)));

  // ! XXX 2D
  norm(0) = -tang(1);
  norm(1) = tang(0);

  for ( int a1=0; a1<rangeDimension; a1++ )
    for ( int a2=0; a2<rangeDimension; a2++ )
      T(a1,a2) = tang(a1)*tang(a2)/mag + norm(a1)*norm(a2)/height;

}

int
AdvancingFront::
computeTransformationAtPoint(realArray &midPt, ArraySimple<real> &T)
{
  // interpolate the components of the transformation matrix from the control grid function

  if ( !controlFunction.numberOfGrids() )
    {
      T = 0;
      for ( int i=0; i<T.size(0); i++ )
	T(i,i) = 1;

      return 1;
    }

  int rd;
  rd = controlGrid[0].rangeDimension();

  Range AXES(0,rd-1);
  for ( int a1=0; a1<rd; a1++ )
    for ( int a2=0; a2<rd; a2++ )
      T(a1,a2) = 0.;

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  getIndex(controlGrid[0].gridIndexRange(),I1,I2,I3);

  RealArray controlBounds(2,3); 
  realArray & vertices = controlGrid[0].vertex();

  real dx[3];
  int ii[3];
  ii[2] = I3.getBase();
  real dxa[3];
  dx[2] = dxa[2] = 0.0;
  int axis;

  // compute indices into the control grid
  for ( axis=0; axis<rd; axis++ )
    {
      controlBounds(0,axis) = vertices(I1.getBase(), I2.getBase(), I3.getBase(),axis);
      controlBounds(1,axis) = vertices(I1.getBound(), I2.getBound(), I3.getBound(), axis);
      dx[axis] = (controlBounds(1,axis) - controlBounds(0,axis))/real(Iv[axis].getLength()-1);
      ii[axis] = int( (midPt(axis)-controlBounds(0,axis))/dx[axis] );
    }

  // compute linear interpolation coefficients
  for ( axis=0; axis<rd; axis++ )
    dxa[axis] = (midPt(axis) - vertices(ii[0],ii[1],ii[2],axis))/dx[axis];

  if ( controlFunction.numberOfGrids() )
    {
      // compute the interpolation
      if ( rd==2 )
	{
	  for ( int ti=0; ti<rd; ti++ )
	    for ( int tj=0; tj<rd; tj++ )
	      T(ti,tj) = (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
					      (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
			  (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
					      (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) );
	}
      else
	{
	  for ( int ti=0; ti<rd; ti++ )
	    for ( int tj=0; tj<rd; tj++ )
	      T(ti,tj) = ( (1.0-dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2],ti,tj) + 
							       (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2],ti,tj) ) +
					   (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2],ti,tj) +
							       (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2],ti,tj) ) ) ) +
			   (    dxa[2])*( (( 1.0 -dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1],ii[2]+1,ti,tj) + 
							       (     dxa[0] ) * controlFunction[0](ii[0]+1,ii[1],ii[2]+1,ti,tj) ) +
					   (      dxa[1] ) * ( ( 1.0-dxa[0] ) * controlFunction[0](ii[0],ii[1]+1,ii[2]+1,ti,tj) +
							       (     dxa[0] ) * controlFunction[0](ii[0]+1, ii[1]+1,ii[2]+1,ti,tj) ) ) ));
	  
	}
    }
  else
    for ( int ti=0; ti<rd; ti++ )
      T(ti,ti) = 1.;

  return 0;

}
  


//\begin{>>AdvancingFront.tex}{\subsection{resizeFaces}}
int
AdvancingFront::
resizeFaces(int newSize)
//===========================================================================
// /Purpose: resize the face storage to newSize
// newSize (input) : the new size for the face storage
//\end{AdvancingFront.tex}
//===========================================================================
{
  faces.reserve(newSize);
  faceNormals.resize(newSize,rangeDimension);

  if (  rangeDimension!=domainDimension )
    {
      int oldSize = initialFaceSurfaceMapping.getLength(0);
      initialFaceSurfaceMapping.resize(newSize);
      for ( int i=oldSize; i<newSize; i++ )
	initialFaceSurfaceMapping(i) = -1;

    }

  return 0;
}

//\begin{>>AdvancingFront.tex}{\subsection{addPoint}}
int 
AdvancingFront::
addPoint(const ArraySimple<real> &newPt)
//===========================================================================
// /Purpose: add a vertex to the list of vertices
// Returns : the new vertex id
//\end{AdvancingFront.tex}
//===========================================================================
{
  if (nptsTotal >= nptsEst) 
    {
      nptsEst = nptsEst + 10*nFacesFront;
      xyz.resize(nptsEst, rangeDimension);
    }

  int id = nptsTotal;
  Index I(0, rangeDimension);
  for ( int i=0; i<rangeDimension; i++ )
    xyz(id, i) = newPt(i);

  nptsTotal++;
  
  ArraySimple<real> bbox(2*rangeDimension);
  for ( int a=0; a<rangeDimension; a++ )
    bbox(2*a) = bbox(2*a+1) = xyz(id,a);
  
  try {
    vertexSearcher.addElement(bbox, id);
  } catch (GeometricADTError & e) {
    e.debug_print();
    cout<<"bounding box\n"<<bbox<<endl;
    throw FrontInsertionFailedError();
  }

  return id;
}

//\begin{>>AdvancingFront.tex}{\subsection{generateElementFaceList}}
intArray  
AdvancingFront::
generateElementFaceList()
//===========================================================================
// /Purpose: generate a list of the adv. front faces in each element
//\end{AdvancingFront.tex}
//===========================================================================
{
  int maxNumberOfFacesPerElement = 3;
  if ( domainDimension==3 ) maxNumberOfFacesPerElement = 5;

  intArray elementFaceList(nElements, maxNumberOfFacesPerElement);
  elementFaceList = -1;

  int eid=0;
  for ( int e=0; e<nElements; e++ )
    {
      vector<int> elementFaces = elements[e];
      
      if ( elementFaces.size()>0 )
	{
	  int nf=0;
	  for ( vector<int>::const_iterator f=elementFaces.begin(); f!=elementFaces.end(); f++ )
	    {
	      elementFaceList(eid, nf) = *f;
	      nf++;
	    }
	  eid++;
	}
    }
  
  elementFaceList.resize(eid, maxNumberOfFacesPerElement);

  return elementFaceList;
}

//\begin{>>AdvancingFront.tex}{\subsection{advanceFront}}
intArray 
AdvancingFront::
generateElementList(bool remUnusedNodes)
//===========================================================================
// /Purpose: generate a list of the vertices in each element
//\end{AdvancingFront.tex}
//===========================================================================
{

  int maxNumberOfVerticesPerElement = 3;
  if ( domainDimension==3 ) maxNumberOfVerticesPerElement = 8;
    
  intArray elementList(nElements, maxNumberOfVerticesPerElement);
  intArray nVerticesElement(nElements);

  nVerticesElement = 0;
  int nf=0;

//   for (vector<Face *>::const_iterator fp=faces.begin(); fp!=faces.end(); fp++)
//     {
//       Face & face = *(*fp);
//       cout<<"nf "<<nf++<<" face id "<<face.getID()<<endl;
//     }
	  
  elementList = -1;
  int eid=0;

  if ( false && remUnusedNodes )
    removeUnusedNodes();

  if ( domainDimension==2 )
    { 

      int vertices[3];
      //realArray midPt(3);
      //midPt = 0.;
      //ArraySimple<real> T;
      eid=0;
      for ( int e=0; e<nElements; e++ )
	{
	  vector<int> elementFaces = elements[e];
      
	  if ( elementFaces.size()>0 )
	    {
	      int baseface =-1;
	      int anotherFace=-1;
	      int nVertsOnBase = -1;
	  
	      for ( vector<int>::const_iterator f=elementFaces.begin(); f!=elementFaces.end(); f++ )
		{
		  if ( faces[*f]->getNumberOfVertices() > nVertsOnBase )
		    {
		      baseface=*f;
		      nVertsOnBase = faces[*f]->getNumberOfVertices();
		    }
		  else
		    anotherFace = *f;
		}
	      
	      Face & face = *faces[baseface];
	      int nVertsInElement = face.getNumberOfVertices()+1;
	      
	      if ( face.getZ2ID()==e )
		{
		  // no need to reverse
		  for ( int v=0; v<face.getNumberOfVertices(); v++ )
		    {
		      vertices[v] = face.getVertex(v);
		      //for ( int a=0; a<rangeDimension; a++ ) midPt(a) += xyz(vertices[v],a);
		    }
		}
	      else
		{
		  for ( int v=0; v<face.getNumberOfVertices(); v++ )
		    {
		      vertices[v] = face.getVertex(face.getNumberOfVertices()-v-1);
		      //for ( int a=0; a<rangeDimension; a++ ) midPt(a) += xyz(vertices[v],a);
		    }
		}
	      
	      Face &otherFace = *faces[anotherFace];
	      
	      // find the last vertex
	      vertices[nVertsInElement-1] = -1;
	      for ( int ov=0; ov<otherFace.getNumberOfVertices() && vertices[nVertsInElement-1]==-1; ov++ )
		{
		  bool foundOther;
		  foundOther = true;
		  for ( int fv=0; fv<face.getNumberOfVertices() && foundOther; fv++ )
		    foundOther = foundOther &&  (otherFace.getVertex(ov) != face.getVertex(fv));
		  
		  if ( foundOther ) vertices[nVertsInElement-1] = otherFace.getVertex(ov);
		}
	      
	      for ( int vv=0; vv<nVertsInElement; vv++ )
		elementList(eid,vv) = vertices[vv];
	      
	      eid++;
	    }
	}
    }
  else
    {
      //      for ( vector< vector<int> >::iterator e=elements.begin(); e!=elements.end(); e++ )
      for ( int e=0; e<nElements; e++ )
	{
	  // find tets and pyramids

	  //	  vector<int> & elementFaces = *e;
	  vector<int> & elementFaces = elements[e];
	  if ( elementFaces.size()>0 )
	    {
	      int baseface = 0;
	      int maxfverts = faces[elementFaces[baseface]]->getNumberOfVertices();
	      // first find the face with the most vertices, unless all are the same
	      //  in which case just use the first
	      for ( int f=baseface+1; f<elementFaces.size(); f++ )
		if ( maxfverts<faces[elementFaces[f]]->getNumberOfVertices() )
		  {
		    baseface = f;
		    maxfverts = faces[elementFaces[f]]->getNumberOfVertices();
		  }

	      // copy the face[baseface]'s vertices into the element list first,
	      //   reversing the order if necessary to match the UnstructuredMapping template
	      Face & face = *faces[elementFaces[baseface]];
	      if ( face.getZ2ID()==e )
		{
		  // no need to reverse
		  for ( int v=0; v<face.getNumberOfVertices(); v++ )
		    elementList(eid,v) = face.getVertex(v);
		}
	      else
		{
		  for ( int v=0; v<face.getNumberOfVertices(); v++ )
		    elementList(eid,v) = face.getVertex(face.getNumberOfVertices()-v-1);
		}

	      int nverts = face.getNumberOfVertices();
	      // find the remaining vertex
	      Face &nextFace = *faces[elementFaces[(baseface+1)%elementFaces.size()]];
	      bool foundVertex = false;
	      for ( int v=0; !foundVertex && v<nextFace.getNumberOfVertices(); v++ )
		{ 
		  foundVertex = true;
		  for ( int vv=0; foundVertex && vv<face.getNumberOfVertices(); vv++ )
		    foundVertex = face.getVertex(vv)!=nextFace.getVertex(v);

		  if ( foundVertex )
		    elementList(eid,nverts++) = nextFace.getVertex(v);
		}
	      if ( !foundVertex ) throw BookKeepingError();
	      
	      eid++;
	    }
	}
    }

  elementList.resize(eid,maxNumberOfVerticesPerElement);

  return elementList;
} 

//\begin{>>AdvancingFront.tex}{\subsection{removeFaceFromFront}}
void 
AdvancingFront::
removeFaceFromFront(PriorityQueue::iterator &delIter)
//===========================================================================
// /Purpose: remove a particular face, defined by an iterator through the queue, from
// the front ( the face is also removed from faceSearcher and pointFaceMapping)
// delIter (input) : iterator pointing to the face in the front to delete
//\end{AdvancingFront.tex}
//===========================================================================
{

  // remove from pointFaceMapping, 
  if ( debug_af ) cout<<"trying to remove "<<(*delIter)->getID()<<endl;
  for ( int v=0; v<(*delIter)->getNumberOfVertices(); v++ )
    {
      vector<PriorityQueue::iterator> &v1Faces = pointFaceMapping[(*delIter)->getVertex(v)];
      bool isDel = false;
      for (vector<PriorityQueue::iterator>::iterator f=v1Faces.begin();f!=v1Faces.end() && !isDel;f++)
	{
	  if ((**f)->getID() == (*delIter)->getID()) 
	    {
	      v1Faces.erase(f);
	      if ( v1Faces.size()==0 )
		{
		  ArraySimple<real> bbox(2*rangeDimension);
		  int v1 = (*delIter)->getVertex(v);
		  for ( int a=0; a<rangeDimension; a++ )
		    bbox(2*a) = bbox(2*a+1) = xyz(v1,a);

		  GeometricADT<int>::iterator vdel(vertexSearcher, bbox);
		  // locate the face in the face searcher
		  while(!vdel.isTerminal() && (*vdel).data!=v1) ++vdel;
		  vertexSearcher.delElement(vdel);
		}
	      isDel = true;
	    }
	}
    }

  // remove from faceSearcher
  Index AXES(0, rangeDimension);

  ArraySimple<real> boundingBox(2*rangeDimension);
  real dm = 0;
#if 0
  for (int axis=0; axis<rangeDimension; axis++)
    {
      boundingBox(2*axis) = min(xyz((*delIter)->getVertex(0), axis), xyz((*delIter)->getVertex(1),axis));
      boundingBox(1+2*axis) = max(xyz((*delIter)->getVertex(0), axis), xyz((*delIter)->getVertex(1),axis));
      dm = max(fabs(boundingBox(1+2*axis)-boundingBox(2*axis)), dm);
    }
#else
  for (int axis=0; axis<rangeDimension; axis++)
    {
      boundingBox(2*axis) = REAL_MAX;
      boundingBox(1+2*axis) = -REAL_MAX;
    }

  for (int axis=0; axis<rangeDimension; axis++)
    {
      for ( int v=0; v<(*delIter)->getNumberOfVertices(); v++ )
	{
	  boundingBox(2*axis) = min( boundingBox(2*axis) , xyz((*delIter)->getVertex(v), axis) );
	  boundingBox(1+2*axis) = max( boundingBox(2*axis+1) , xyz((*delIter)->getVertex(v), axis) );
	}
      dm = max(fabs(boundingBox(1+2*axis)-boundingBox(2*axis)), dm);
    }
#endif

   for (int axis=0; axis<rangeDimension; axis++)
     {
       boundingBox(2*axis) -= 0.1*dm;
       boundingBox(1+2*axis) += 0.1*dm;
     }

  //GeometricADT<Face*>::iterator iter(faceSearcher, boundingBox);
  GeometricADT<Face*>::traversor iter(faceSearcher, boundingBox);
  
  // locate the face in the face searcher
  if (debug_af ) cout<<"removal search : "<<(*iter).data->getID()<<" "<<(*delIter)->getID()<<endl;
  //while(!iter.isTerminal() && (*iter).data->getID()!=(*delIter)->getID())
  while(!iter.isFinished() && (*iter).data->getID()!=(*delIter)->getID()) 
    {
      iter++;
      if (debug_af ) cout<<"removal search : "<<(*iter).data->getID()<<" "<<(*delIter)->getID()<<endl;
    }

  //  if ( iter.isFinished() ) 
  if ( iter.isFinished() && (*iter).data->getID()!=(*delIter)->getID() )
    {
      cout<<"did not find face "<<(*delIter)->getID()<<" in face searcher, last was "<<(*iter).data->getID()<<endl;
      abort();
    }

  AssertException (((*iter).data->getID()==(*delIter)->getID()), BookKeepingError());

  faceSearcher.delElement(iter);

  // now remove the face from the front
  front.erase(delIter);

  nFacesFront--;
}

real
AdvancingFront::
computeNormalizedGrowthDistance(real sizePhys, real sizeTrans)
{

  real delta;

  if ( 0.55*sizeTrans<=1.0 && .5<sizeTrans )
    delta = 1.0;
  else if ( 0.55*sizeTrans>1.0 ) // !! < in references !!!
    delta = .55*sizeTrans;
  else if ( sizeTrans<=0.5 )
    delta = 2.0*sizeTrans;
  else
    {
      cout<<"bad size for face "<<sizeTrans<<endl;
      throw AdvancingFrontError();
    }

  //cout<<"sizeTrans, sizePhys, delta "<<sizeTrans<<" "<<sizePhys<<" "<<delta<<endl;
  //if ( delta!=1.0 ) cout<<"adjusted delta : "<<sizeTrans<<" "<<delta<<endl;

  return delta;
}

void 
AdvancingFront::
addFaceToElement(int face, int elem)
{

  if (elem==elements.size()){
    //cout<<"getting a new element in elements "<<elem<<" "<<nElements<<endl;
    
    elements.push_back(vector< int > ()) ;
    elements[elem].reserve(nFacesPerElementMax);

  }
  elements[elem].push_back(face);
  //cout << elements.size()<< " "<<elements.capacity()<<endl;
}

int
AdvancingFront::
transformCoordinatesToParametricSpace(realArray &xyz_in, realArray &xyz_param)
{

  // verbose enough name
  return 0;

}

#if 0
int 
AdvancingFront::
makePrismPyramidHex(const Face & currentFace, int newElementID, const ArraySimple<real> &pIdealPhys, 
		    const ArraySimple<real> &T, vector<int> &existing_candidates, 
		    vector<PriorityQueue::iterator > &oldFrontFaces)
{
  bool madeElement = true;

  int newVertID = addPoint(pIdealPhys);
  for ( int e=0; e<currentFace.getNumberOfVertices(); e++ )
    {
      ArraySimple<int> newFaceVerts(3);
      int ep = (e+1)%currentFace.getNumberOfVertices();
      newFaceVerts(0) = currentFace.getVertex(e);
      newFaceVerts(1) = currentFace.getVertex(ep);
      newFaceVerts(2) = newVertID;
      insertFace(newFaceVerts, newElementID, -1);
    } // for e

  return madeElement;

}
#endif

bool
AdvancingFront::
vertexIsOnFront(int v)
{ 
  if ( v<0 || v>=getNumberOfVertices() )
    {
      cout<<"bad vertex request "<<v<<endl;
      throw AdvancingFrontError();
    }
  else
    return pointFaceMapping[v].size()>0; 
}

bool
AdvancingFront::
auxiliaryCheck(ArraySimple<real> &candNormal, ArraySimple<real> &faceNormal )
{
  // auxillary check for faces nearby
  // in the spirit of Jin and Tanner.

  //ArraySimple<real> candNormal(rangeDimension);
  real ang = parameters.getAuxiliaryAngleTolerance();
  real angletol = cos(M_PI*(1.-ang/180.));
 
  bool passed = true;
#if 0
  for ( int v=0; v<face.getNumberOfVertices() && passed; v++ )
    {
      int vp = (v+1)%face.getNumberOfVertices();
      vector<PriorityQueue::iterator> &vfaces =  pointFaceMapping[face.getVertex(v)];

      for ( vector<PriorityQueue::iterator>::iterator vf=vfaces.begin();
	    passed && vf!=vfaces.end(); vf++ )
	{
	  Face & fcand = ***vf;

	  if ( fcand.getID()!=face.getID() )
	    {
	      for ( int vv=0; vv<fcand.getNumberOfVertices() && passed; vv++ )
		if ( fcand.getVertex(vv)==face.getVertex(v) )
		  if ( fcand.getVertex((vv+fcand.getNumberOfVertices()-1)%fcand.getNumberOfVertices())==face.getVertex(vp) )
		    {
		      for ( int a=0; a<rangeDimension; a++ )
			candNormal[a] = faceNormals(fcand.getID(), a);

		      passed = ASdot(faceNormal, candNormal)>angletol;
		    }
	    }
	}

    }
#endif

  //real dotprod = ASdot(faceNormal, candNormal);
  passed = ASdot(faceNormal, candNormal)>angletol;
  if (!passed) cout<<"failed auxiliary check"<<endl;
  return passed;

}

#if 0
bool
AdvancingFront::
auxiliaryDistanceCheck(ArraySimple<real> candCenter, ArraySimple<real> &newVertex, real dist )
{
  real asum=0.0;
  for ( int a=0; a<rangeDimension; a++ )
    asum += (newVertex[a]-candCenter[a])*(newVertex[a]-candCenter[a]);

  return sqrt(asum)>0.55*dist;
}

#endif

struct faceAddition 
{
  int face;
  int deletedElement;
};

bool
AdvancingFront::
expandFront()
{
  bool frontExpanded = false;

  // first remove all the elements in the front, saving the faces we are going to push
  // onto the new front

  vector<faceAddition> facesToAdd;
  facesToAdd.reserve(nFacesFront*2);

  int v,a;
  PriorityQueue::iterator fi;

  // remove faces from elements
  for ( fi=front.begin(); fi!=front.end(); fi++ )
    {
      Face & face = **fi;
      int z1 = face.getZ1ID();
      if ( z1!=-1 ) // only remove if it is not a boundary face ( bdy faces have z1==-1 )
	{
	  // remove the face from the element
	  for ( vector<int>::iterator el=elements[z1].begin(); el!=elements[z1].end(); el++ )
	    {
	      if ( *el==face.getID() )
		{
		  elements[z1].erase(el);
		  frontExpanded = true;
		  break;
		}
	    }
	}
    }

  // collect faces we are going to add
  for ( fi=front.begin(); fi!=front.end(); fi++ )
    {
      Face & face = **fi;
      int z1 = face.getZ1ID();
      bool duplicate;
      // faces left in the element should be added back to the front
      if ( z1!=-1 )
	{
	  vector<int>::iterator el = elements[z1].begin();
	  while ( el!=elements[z1].end() )
	    {
	      duplicate = false;
	      if ( *el != face.getID() && faces[*el]->getZ2ID()!=-1 )
		{
		  for ( vector<faceAddition>::iterator fa=facesToAdd.begin(); fa!=facesToAdd.end(); fa++ )
		    {
		      // check for duplicates
		      if ( (*fa).face == (*el) )
			{
			  if ( (*fa).deletedElement!=z1 )
			    {
			      for ( vector<int>::iterator el2 = elements[(*fa).deletedElement].begin();
				    el2!=elements[(*fa).deletedElement].end(); el2++ )
				if ( *el2 == *el ) 
				  {
				    elements[(*fa).deletedElement].erase(el2);
				    break;
				  }

			      int fid = faces[*el]->getID();
			      elements[z1].erase(el);
			      delete faces[fid];
			      faces[fid] = NULL;

			      el = elements[z1].begin();
			      facesToAdd.erase(fa);
			    }
			  else
			    el++;
			  duplicate = true;
			  break;
			}
		    }

		  if ( !duplicate )
		    {
		      faceAddition fadd;
		      fadd.face = *el;
		      fadd.deletedElement = z1;
		      
		      facesToAdd.push_back(fadd);
		      el++;
		    }
		}
	      else
		el++;
	    }
	}
    }

  // now remove all the faces we can ( do not remove bdy faces )
  fi=front.begin(); 
  while(fi!=front.end())
    {
      Face & face = **fi;
      int z1 = face.getZ1ID();
      if ( z1!=-1 ) // only remove if it is not a boundary face ( bdy faces have z1==-1 )
	{
	  removeFaceFromFront(fi);
	  fi = front.begin();
	  elements[z1].clear();
	  int fid = face.getID();
	  delete faces[fid];
	  faces[fid] = NULL;
	}
      else
	fi++;
    }
  
  // finally, add all the faces we need back into the front
  for ( vector<faceAddition>::iterator addf=facesToAdd.begin(); addf!=facesToAdd.end(); addf++ )
    {
      if ( faces[(*addf).face]->getZ1ID()==(*addf).deletedElement )
	{
	  // reverse the face
	  faces[(*addf).face]->reverseVertices();
	  int z2 = faces[(*addf).face]->getZ2ID();
	  faces[(*addf).face]->setZ1ID(z2);
	}
      
      faces[(*addf).face]->setZ2ID(-1);
      
      //      cout<<"adding face "<<(*addf).face<<endl;
      addFaceToFront(*faces[(*addf).face]);

    }

  fi=front.begin(); 
  while(fi!=front.end())
    {
      for ( int v=0; v<(*fi)->getNumberOfVertices(); v++ )
	if ( pointFaceMapping[(*fi)->getVertex(v)].size()<rangeDimension )
	  {
	    cout<<"FOUND A HANGING FACE "<<(*fi)->getID()<<" vertex "<<v<<" vertex gid "<<(*fi)->getVertex(v)<<endl;
	  }

      fi++;
    }

  //if ( frontExpanded ) nexpansions++;

  return frontExpanded;
}

void 
AdvancingFront::
addFaceToFront(Face &face)
{
  ArraySimple<real> faceVertices(face.getNumberOfVertices(), rangeDimension);
  int id = face.getID();

  int a;
  for ( a=0; a<rangeDimension; a++ )
    for ( int vert=0; vert<face.getNumberOfVertices(); vert++ )
      faceVertices(vert,a) = xyz(face.getVertex(vert),a);

  ArraySimple<real> normal(rangeDimension);

  if ( id<initialFaceSurfaceMapping.getLength(0) )
    computeFaceNormal(faceVertices,normal, initialFaceSurfaceMapping(id) );
  else
    computeFaceNormal(faceVertices,normal);

  for ( a=0; a<rangeDimension; a++ )
    faceNormals(id,a) = normal(a);

  //real newFaceSize = computeFaceSize(evaluate(xyz(vertexIDs,AXES)));
  real newFaceSize = computeFaceSize(faceVertices);

  real minl,maxl;
  //averageFaceSize = (averageFaceSize*(nFacesTotal-1) + computeAvgEdgeLength(faceVertices,minl,maxl))/nFacesTotal;

  // assign priority based on face "size":
  // "higher" priorities should correspond to smaller size so set the priority
  // to -"size". if it is a 3d triangular face, make it -1000*"size"
  // so that quadrilateral faces are generally first and ordered by thier size
  //real priority = ( newFace->getNumberOfVertices()>3 ) ? -newFaceSize : newFaceSize;//1.0;//1.0/newFaceSize;
  //PriorityQueue::iterator faceFrontLocation = front.insert(newFace, 1);
  real priority = ( face.getNumberOfVertices()>3 ) ? -newFaceSize : -1000*newFaceSize;//1.0;//1.0/newFaceSize;
  //real priority = -newFaceSize; 
  if ( rangeDimension==2  ) priority=1; //priority = -newFaceSize; //priority = 1.;

  //  priority = -newFaceSize;

  Face * facep = &face;
  PriorityQueue::iterator faceFrontLocation = front.insert(facep, priority);
  nFacesFront++;

  // insert into faceSearcher
  ArraySimple<real> minmax(2*rangeDimension);
  for ( int ax=0; ax<rangeDimension; ax++ )
    {
      minmax(2*ax) = REAL_MAX;
      minmax(1+2*ax) = -REAL_MAX;
    }

  int vv;
  for ( vv=0; vv<face.getNumberOfVertices(); vv++ )
    {
      for ( int axis=0; axis<rangeDimension; axis++ )
	{
	  minmax(2*axis) = min(minmax(2*axis),faceVertices(vv, axis));
	  minmax(1+2*axis) = max(minmax(1+2*axis),faceVertices(vv, axis));
	}
      
    }
  //cout<<minmax<<endl;

  try { // to add the face to the search data structure
    faceSearcher.addElement(minmax, facep);
  }
  catch(GeometricADTError & e) {
    e.debug_print();
    cout<<"bounding box\n"<<minmax<<endl;
    throw FrontInsertionFailedError();
  }

  for ( vv=0; vv<face.getNumberOfVertices(); vv++ )
    {
      if ( pointFaceMapping[face.getVertex(vv)].size()==0 )
	{
	  for ( a=0; a<rangeDimension; a++ )
	    minmax(2*a) = minmax(2*a+1) = xyz(face.getVertex(vv),a);

	  int vid = face.getVertex(vv);
	  try {
	    vertexSearcher.addElement(minmax,vid);
	  }
	  catch(GeometricADTError &e ) {
	    e.debug_print();
	    cout<<"bounding box\n"<<minmax<<endl;
	    throw FrontInsertionFailedError();
	  }
	  
	}
    }

  // insert into pointFaceMapping
  for ( int v=0; v<face.getNumberOfVertices(); v++ )
    pointFaceMapping[face.getVertex(v)].push_back(faceFrontLocation);

}

real
AdvancingFront::
computeElementQuality(int element)
{

  int vertices[5];
  //realArray midPt(3);
  //midPt = 0.;
  //ArraySimple<real> T;

  vector<int> elementFaces = elements[element];

  int baseface =-1;
  int anotherFace=-1;
  int nVertsOnBase = -1;

  for ( vector<int>::const_iterator f=elementFaces.begin(); f!=elementFaces.end(); f++ )
    {
      if ( faces[*f]->getNumberOfVertices() > nVertsOnBase )
	{
	  baseface=*f;
	  nVertsOnBase = faces[*f]->getNumberOfVertices();
	}
      else
	anotherFace = *f;
    }

  Face & face = *faces[baseface];
  int nVertsInElement = face.getNumberOfVertices()+1;
  
  if ( face.getZ2ID()==element )
    {
      // no need to reverse
      for ( int v=0; v<face.getNumberOfVertices(); v++ )
	{
	  vertices[v] = face.getVertex(v);
	  //for ( int a=0; a<rangeDimension; a++ ) midPt(a) += xyz(vertices[v],a);
	}
    }
  else
    {
      for ( int v=0; v<face.getNumberOfVertices(); v++ )
	{
	  vertices[v] = face.getVertex(face.getNumberOfVertices()-v-1);
	  //for ( int a=0; a<rangeDimension; a++ ) midPt(a) += xyz(vertices[v],a);
	}
    }

  Face &otherFace = *faces[anotherFace];
  
  // find the last vertex
  vertices[nVertsInElement-1] = -1;
  for ( int ov=0; ov<otherFace.getNumberOfVertices() && vertices[nVertsInElement-1]==-1; ov++ )
    {
      bool foundOther;
      foundOther = true;
      for ( int fv=0; fv<face.getNumberOfVertices() && foundOther; fv++ )
	foundOther = foundOther &&  (otherFace.getVertex(ov) != face.getVertex(fv));

      if ( foundOther ) vertices[nVertsInElement-1] = otherFace.getVertex(ov);
    }
      
  //for ( int a=0; a<rangeDimension; a++ ) midPt(a) += xyz(vertices[nVertsInElement-1],a);

  //computeTransformationAtPoint(midPt, T);

  MeshQualityMetrics mq;
  MetricCGFunctionEvaluator metricEval(&controlFunction);
  if ( domainDimension==rangeDimension )
    mq.setReferenceTransformation(&metricEval);

  real N2,det,K;

  if ( domainDimension==2 )
    {
      ArraySimpleFixed<real,2,1,1,1> pp[3];
      ArraySimpleFixed<real,2,2,1,1> T,J;

      ArraySimpleFixed<real,2,1,1,1> pc;

      if ( rangeDimension==2 )
	{
	  pc[0] = pc[1] = 0.;
	  for ( int v=0; v<3; v++ )
	    for ( int a=0; a<2; a++ )
	      {
		pp[v][a] = xyz(vertices[v],a);
		pc[a] += pp[v][a]/real(nVertsInElement);
	      }
	  
	}
      else
	{
	  ArraySimple<real> pc(3),p1(3),p2(3),p3(3),p1t(3),p2t(3),p3t(3);
	  ArraySimple<real> Tstretch(3,3);

	  realArray mid(3);
	  mid = 0.;

	  Tstretch = 0.;
	  Tstretch(0,0)=Tstretch(1,1)=Tstretch(2,2) = 1;

	  int a;
	  for ( a=0; a<3; a++ )
	    {
	      p1(a) = xyz(vertices[0],a);
	      p2(a) = xyz(vertices[1],a);
	      p3(a) = xyz(vertices[2],a);
	      mid(a) = (p1(a)+p2(a)+p3(a))/real(nVertsInElement);
	    }

	  computeTransformationAtPoint(mid,Tstretch);

	  matVectMult(Tstretch,p1,p1t);
	  matVectMult(Tstretch,p2,p2t);
	  matVectMult(Tstretch,p3,p3t);

	  ArraySimple<real> snorm = areaNormal3D(p1t,p2t,p3t);
	  real mags = sqrt(ASmag2(snorm));
	  for ( a=0; a<3; a++ )
	    snorm[a] /= mags;
	  
	  ArraySimpleFixed<real,3,1,1,1> enorm, edge;
	  for ( a=0; a<3; a++ )
	    edge[a] = p2t[a] - p1t[a];

	  enorm[0] = -edge[1]*snorm[2]+edge[2]*snorm[1];
	  enorm[1] = edge[0]*snorm[2]-edge[2]*snorm[0];
	  enorm[2] = -edge[0]*snorm[1]+edge[1]*snorm[0];
	  real edgemag = sqrt(ASmag2(edge));
	  real enormmag = sqrt(ASmag2(enorm));
	  for ( a=0; a<3; a++ )
	    {
	      enorm[a]/=enormmag;
	      edge[a] /=edgemag;
	    }

	  for ( a=0; a<2; a++ )
	    for ( int v=0; v<3; v++ )
	      pp[v][a] = 0.;

	  for ( a=0; a<3; a++ )
	    {
	      pp[1][0] += (p2t(a)-p1t(a))*edge[a];
	      pp[1][1] += (p2t(a)-p1t(a))*enorm[a];
	      pp[2][0] += (p3t(a)-p1t(a))*edge[a];
	      pp[2][1] += (p3t(a)-p1t(a))*enorm[a];
	    }
	  
	}

      T = mq.computeWeight(pc, UnstructuredMapping::triangle);
      J = mq.computeJacobian(pp[0],pp[1],pp[2],T);
      mq.computeJacobianProperties(N2,det,K,J);
      
    }
  else
    {
      ArraySimpleFixed<real,3,1,1,1> pp[5],pc;
      ArraySimpleFixed<real,3,3,1,1> T,J;

      pc[0] = pc[1] = pc[2] = 0.;
      for ( int v=0; v<nVertsInElement; v++ )
	for ( int a=0; a<3; a++ )
	  {
	    pp[v][a] = xyz(vertices[v],a);
	    pc[a] += pp[v][a]/real(nVertsInElement);
	  }

      if ( nVertsInElement==4 )
	{
	  T = mq.computeWeight(pc,UnstructuredMapping::tetrahedron);
	  J = mq.computeJacobian(pp[0],pp[1],pp[2],pp[3],T);
	}
      else
	{
	  T = mq.computeWeight(pc,UnstructuredMapping::pyramid);
	  J = mq.computeJacobian(pp[0],pp[1],pp[2],pp[3],pp[4],T);
	}

      mq.computeJacobianProperties(N2,det,K,J);

    }

  //  cout<<"det ,K "<<det<<" "<<K<<endl;
  //return min(det,1/det)*real(domainDimension)/K;
  return real(domainDimension)/K;

}

void 
AdvancingFront::
improveQuality() 
{

  // sweep through the faces in the mesh removing "bad" ones and collecting
  // faces that we need to add back into the mesh
  map<int,int,AdvancingFront::cmpFace> facesToAdd;
  vector<int> facesToRemove;
  //facesToAdd.reserve(100);
  facesToRemove.reserve(100);

  real qualityTol = parameters.getQualityTolerance();

  for ( vector<Face *>::iterator f=faces.begin(); f!=faces.end(); f++ )
    {
      if ( (*f)!=NULL )
      if ( (*f)->getZ1ID()!=-1 && (*f)->getZ2ID()!=-1 )
	{
	  if ( elementQuality((*f)->getZ1ID())<qualityTol ||
	       elementQuality((*f)->getZ2ID())<qualityTol)
	    {
	      
	      facesToRemove.push_back((*f)->getID());
	      
	      vector<int> &e1Faces = elements[(*f)->getZ1ID()];
	      for ( vector<int>::iterator f1=e1Faces.begin(); f1!=e1Faces.end(); f1++ )
		if ( (*f1)!=(*f)->getID() && facesToAdd.count(*f1)==0 ) 
		  facesToAdd[(*f1)] = (*f)->getZ1ID();
	      
	      vector<int> &e2Faces = elements[(*f)->getZ2ID()];
	      for ( vector<int>::iterator f2=e2Faces.begin(); f2!=e2Faces.end(); f2++ )
		if ( (*f2)!=(*f)->getID() && facesToAdd.count(*f2)==0 ) 
		  facesToAdd[(*f2)] = (*f)->getZ2ID();

	      // delete the information in the elements
	      elements[(*f)->getZ1ID()].clear();
	      elements[(*f)->getZ2ID()].clear();
	    
	      facesToAdd.erase(facesToAdd.lower_bound((*f)->getID()),
			       facesToAdd.upper_bound((*f)->getID()));

	    }
	}
    }
  
  for ( map<int,int,AdvancingFront::cmpFace>::iterator fa=facesToAdd.begin(); fa!=facesToAdd.end(); fa++ )
    {
      Face &face = *faces[(*fa).first];
      int deletedElement = (*fa).second;

      bool isHanging = false;

      if ( face.getZ1ID()!=-1 && face.getZ2ID()!=-1 )
	isHanging = elements[face.getZ1ID()].size()==0 && elements[face.getZ2ID()].size()==0;

      if (!isHanging)
	{
	  if ( face.getZ1ID()==deletedElement )
	    {
	      // reverse the face
	      face.reverseVertices();
	      int z2 = face.getZ2ID();
	      face.setZ1ID(z2);
	    }
	  
	  face.setZ2ID(-1);
	  
	  //      cout<<"adding face "<<(*addf).face<<endl;
	  addFaceToFront(face);
	}
      else
	facesToRemove.push_back(face.getID());
    }

  for ( vector<int>::iterator fr=facesToRemove.begin(); fr!=facesToRemove.end(); fr++ )
    {
      delete faces[*fr];
      faces[*fr] = NULL;
    }

  expandFront();
  expandFront();
}

bool 
AdvancingFront::
newElementVertexCheck(const Face &currentFace, vector<int> &local_nodes, 
		      ArraySimpleFixed<real,3,1,1,1> &pc, int filterNode)
{

  int e,ep,a;
  ArraySimpleFixed<real,3,1,1,1> p1,p2,p3;

  bool allpos=false;
  for ( vector<int>::const_iterator epc=local_nodes.begin();
	!allpos && epc!=local_nodes.end(); epc++ )
    {
      
      if ( *epc!=filterNode )
	{
	  allpos = true;
	  if ( debug_af )
	    cout<<"newElementVertexCheck for vertex "<<*epc<<" : ";

	  for ( e = 0; e<currentFace.getNumberOfVertices() && allpos; e++ )
	    { 
	      
	      ep = (e+1)%currentFace.getNumberOfVertices();
	      for ( a=0; a<3; a++ )
		{
		  p1[a] = xyz(currentFace.getVertex(e),a);
		  p2[a] = xyz(currentFace.getVertex(ep),a);
		  p3[a] = xyz(*epc,a);
		}
	      
	      allpos =  allpos && (orient3d(p1.ptr(),p2.ptr(),pc.ptr(),p3.ptr())>=0.);
	      if ( debug_af )
		cout << orient3d(p1.ptr(),p2.ptr(),pc.ptr(),p3.ptr()) <<" ";
	    }
	  if ( debug_af )
	    {
	      cout<<endl;
	      if (allpos) cout<<"invalidated face with vertex "<<*epc<<endl;
	    }
	}
      
    }

  return !allpos;
}

void 
AdvancingFront::
removeUnusedNodes()
{

  intArray newNodeIDs(getNumberOfVertices());
  
  newNodeIDs=0;

  // first go through and count the number of times each node is used
  for ( vector<Face*>::iterator f=faces.begin(); f!=faces.end(); f++ )
    {
      if (*f)
	{
	  Face &ff = **f;
	  for ( int fv=0; fv<ff.getNumberOfVertices(); fv++ )
	    newNodeIDs(ff.getVertex(fv))++;
	}
    }

  int nid=0;
  // now determine new node ids for each used node
  for ( int n=0; n<nptsTotal; n++ )
    {
      if ( newNodeIDs(n)>0 )
	{
	  newNodeIDs(n) = nid;
	  nid++;
	}
      else
	{
	  newNodeIDs(n)=-1;
	}
    }

  // now go through and use the node mapping to rename all nodes in the faces
  for ( vector<Face*>::iterator f=faces.begin(); f!=faces.end(); f++ )
    {
      if (*f)
	{
	  Face &ff = **f;
	  for ( int fv=0; fv<ff.getNumberOfVertices(); fv++ )
	    {
	      int on = ff.getVertex(fv);
	      int nn = newNodeIDs(on);
	      assert( nn>-1 ); // something got really hosed if this fails

	      if ( on!=nn )
		ff.setVertex(fv,nn);
	    }
	}
    }

  Range AXES(getRangeDimension());

  // now shift the actual vertices
  for ( int n=0; n<nptsTotal; n++ )
    {
      if ( newNodeIDs(n)>-1 )
	{
	  xyz(newNodeIDs(n),AXES) = xyz(n,AXES);
	}
    }

  nptsTotal = nptsEst = nid;
  xyz.resize(nptsTotal, getRangeDimension());

}
