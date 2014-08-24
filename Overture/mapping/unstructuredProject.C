#include "UnstructuredMapping.h"
#include "MappingRC.h"
#include "MappingProjectionParameters.h"
#include "display.h"
#include "GeometricADT3dInt.h"
#include "TriangleClass.h"

bool UnstructuredMapping::
projectOnTriangle( real *x0, real *x1, real *x2,  
                   real *xa, real *xb,  
                   real *xt, 
                   int & intersectionFace, int & intersectionFace2, 
                   real & r0, real & s0,
                   real *normal /* =NULL */ )
// ===============================================================================
/// \details 
///    Given a 3d triangle (x0,x1,x2), determine the closest point, xt, on the triangle (x0,x1,x2).
///  to a given point xb.
/// \param xa (input) : a point in the plane of the triangle  *** NOT USED ANYMORE ***
/// \param xb (input) : the target point.
/// \param xt (output): closest point on triangle.
/// \param intersectionFace (output) : 0,1,2 to indicate which face of the element we are closest to.
/// \param intersectionFace2 (output) : 0,1,2 -- at a corner this is the other possible closest face
/// \param r0,s0 (output) : triangle coordinates of xt. 
/// \return  true if the projected point is inside the triangle.
///     
// ===============================================================================
{
  real a[3],b[3],xm[3],aa,ab,bb;
  a[0]=x1[0]-x0[0];
  a[1]=x1[1]-x0[1];
  a[2]=x1[2]-x0[2];

  b[0]=x2[0]-x0[0];
  b[1]=x2[1]-x0[1];
  b[2]=x2[2]-x0[2];
  
  aa=a[0]*a[0]+a[1]*a[1]+a[2]*a[2];
  ab=a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
  bb=b[0]*b[0]+b[1]*b[1]+b[2]*b[2];
  
  xm[0]=xb[0]-x0[0]; xm[1]=xb[1]-x0[1]; xm[2]=xb[2]-x0[2]; 
  real f1=a[0]*xm[0]+a[1]*xm[1]+a[2]*xm[2];
  real f2=b[0]*xm[0]+b[1]*xm[1]+b[2]*xm[2];
  
  real deti = aa*bb-ab*ab;
  if(deti!=0. ) deti=1./deti;

  real r=(f1*bb-f2*ab)*deti;
  real s=(f2*aa-f1*ab)*deti;
  
  // printf("   Before restricting to triangle, (r,s)=(%e,%e) deti*(|a||b|)=%8.2e \n",r,s,deti*SQRT(aa*bb));

  bool inside=false;
  if( r<0. )
  { // above face b, project onto face b
    // orthogonal projection:
    //     x = r*a + s*b
    //     xt = sp*b,  rp=0.
    //   (x-xt).b = 0  ->   sp= s + r*(b.a)/b.b
    intersectionFace=2;
    // printf("project on a triangle r<0, r=%e, s=%e, rp=%e, sp=%e\n",r,s,r,s+r*ab/bb);

    if( bb>0. )
      s += r*ab/bb;

    if( s<=0. )
      intersectionFace2=0;
    else if( 1.-(r+s) <=0. )
      intersectionFace2=1;
    else
      intersectionFace2 = fabs(s) < fabs(1.-(r+s)) ? 0 : 1;

    r=0.;
    s=max(0.,min(1.,s));
  }
  else if( s<0. )
  { // below face a, project onto face a
    // orthogonal projection:
    //     x = r*a + s*b
    //     xt = rp*a,  sp=0.
    //   (x-xt).a = 0  ->   rp= r + s*(b.a)/a.a
    intersectionFace=0;
    // printf("project on a triangle s<0, r=%e, s=%e, rp=%e, sp=%e\n",r,s,r+s*ab/aa,s);

    if( aa>0. )
      r += s*ab/aa;

    if( r<=0. )
      intersectionFace2=2;
    else if( 1.-(r+s) <=0. )
      intersectionFace2=1;
    else
      intersectionFace2 = fabs(r) < fabs(1.-(r+s)) ? 2 : 1;

    s=0.;
    r=max(0.,min(1.,r));
  }
  else if( (r+s)>1. )
  {
    // outside face c=a-b, project onto face a-b (where r+s==1)
    // We do an orthogonal projection onto  c = b-a
    //  projection:  xt = rp*a + sp*b
    //               rp+sp=1
    //               (x-xt).c =0
    // ->
    //      rp = r (-a.c)/c.c + (1-s) b.c/c.c
    //      sp = (1-r) (-a.c)/c.c + s b.c/c.c

    xm[0]=b[0]-a[0]; xm[1]=b[1]-a[1];  xm[2]=b[2]-a[2];  
    real cNormInverse= xm[0]*xm[0]+xm[1]*xm[1]+xm[2]*xm[2];
    if( cNormInverse>0. )
      cNormInverse=1./cNormInverse;
    else
      cNormInverse=1.;
    
    real aDotC = a[0]*xm[0]+a[1]*xm[1]+a[2]*xm[2];
    real bDotC = b[0]*xm[0]+b[1]*xm[1]+b[2]*xm[2];

    real rp = (-r    *aDotC + (1.-s)*bDotC)*cNormInverse;
    real sp = ((r-1.)*aDotC +     s *bDotC)*cNormInverse;
    // printf("project on a triangle r+s>1, r=%e, s=%e, rp=%e, sp=%e\n",r,s,rp,sp);
    
    r=rp;
    s=sp;
    intersectionFace=1;

    // r+s==1 but we may still be outside the triangle if r<0 or s<0
    if( s<=0. )
    {
      r=1.;
      s=0.;
      intersectionFace2=0; // another possible intersection face

    }
    else if( r<=0. )
    {
      s=1.;
      r=0.;
      intersectionFace2=2;  // another possible intersection face

    }
    else
    {
      intersectionFace2 = s<r ? 0 : 2; // this case shouldn't happen.
    }
    
    
//     real rps=r+s;
//     r/=rps;
//     s/=rps;
    
  }
  else
  {
    intersectionFace = 0; // shouldn't matter in this case.
    intersectionFace2=0;
    inside=true;
  }
  

  xt[0]=x0[0]+a[0]*r+b[0]*s;
  xt[1]=x0[1]+a[1]*r+b[1]*s;
  xt[2]=x0[2]+a[2]*r+b[2]*s;
  
  r0=r;
  s0=s;
  
  return inside;
}

void UnstructuredMapping::
getNormal( int e, real *normalVector )
// ===============================================================================
// /Description:
//    determine the normal to a triangular element.
// ===============================================================================
{
  int n0=element(e,0), n1=element(e,1), n2=element(e,2); // assumes triangles
    
  real x0[3], a[3], b[3];
  
  x0[0]=node(n0,0);     x0[1]=node(n0,1);      x0[2]=node(n0,2);
  a[0]=node(n1,0)-x0[0]; a[1]=node(n1,1)-x0[1]; a[2]=node(n1,2)-x0[2];
  b[0]=node(n2,0)-x0[0]; b[1]=node(n2,1)-x0[1]; b[2]=node(n2,2)-x0[2];

  normalVector[0]=a[1]*b[2]-a[2]*b[1];
  normalVector[1]=a[2]*b[0]-a[0]*b[2];
  normalVector[2]=a[0]*b[1]-a[1]*b[0];
  
  real norm = SQRT(SQR(normalVector[0])+SQR(normalVector[1])+SQR(normalVector[2]));
  if( norm!=0. )
  {
    norm=1./norm;
    normalVector[0]*=norm;
    normalVector[1]*=norm;
    normalVector[2]*=norm;
  }
  else
  {
    printf("UnstructuredMapping::getNormal:ERROR: normal to element %i is zero!\n",e);
    normalVector[0]=1.;
  }
  
 

}


static void
getSignedArea( real x0[3], real x1[3] , real x2[3], real area[3] )
// ===============================================================================
// /Description:
//    Determine twice the signed area of a  triangular element.
// ===============================================================================
{
  real a[3], b[3];
  
  a[0]=x1[0]-x0[0]; a[1]=x1[1]-x0[1]; a[2]=x1[2]-x0[2];
  b[0]=x2[0]-x0[0]; b[1]=x2[1]-x0[1]; b[2]=x2[2]-x0[2];

  area[0]=a[1]*b[2]-a[2]*b[1];
  area[1]=a[2]*b[0]-a[0]*b[2];
  area[2]=a[0]*b[1]-a[1]*b[0];
  
}


int UnstructuredMapping::
buildSearchTree()
{
  assert( search==NULL );
  //kkc 040309   assert( rangeDimension==3 );
  RealArray boundingBox(2,3);
  // const real boundingBoxExtensionFactor=.02;
  const real boundingBoxExtensionFactor=.05;  // *wdh* 090531 -- make a bit biger for coarse triangulations
   
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    real xa=(real)getRangeBound(Start,axis);
    real xb=(real)getRangeBound(End  ,axis);
    real diff = max( REAL_EPSILON*100.*max(fabs(xa),fabs(xb)), xb-xa);
    diff=max(diff,REAL_EPSILON*100.);
    boundingBox(Start,axis)=(real)getRangeBound(Start,axis)-boundingBoxExtensionFactor*diff;
    boundingBox(End  ,axis)=(real)getRangeBound(End  ,axis)+boundingBoxExtensionFactor*diff;
  }

  if ( rangeDimension==2 )
  {
    boundingBox(Start,2)= -1;
    boundingBox(End  ,2)=  1;
  }

//   if( false )
//   {
//     printF("UnstructuredMapping::buildSearchTree: mapping bounding box =[%e,%e][%e,%e][%e,%e]\n",
// 	   boundingBox(0,0),boundingBox(1,0),  
//            boundingBox(0,1),boundingBox(1,1), 
//            boundingBox(0,2),boundingBox(1,2));
//   }
  
  search = new GeometricADT3dInt(rangeDimension,boundingBox.getDataPointer());

  // Fill in the search tree with the bounding box for each triangle.
  RealArray bb(2,3);
  real time0=getCPU();
#if 0
  for( int e=0; e<numberOfElements; e++ )
  {
    int n0=element(e,0), n1=element(e,1), n2=element(e,2);
    for( int dir=0; dir<rangeDimension; dir++ )
    {
      bb(0,dir)=min(node(n0,dir),node(n1,dir),node(n2,dir));
      bb(1,dir)=max(node(n0,dir),node(n1,dir),node(n2,dir));

    }
    // bb.reshape(2*rangeDimension);
    
    search->addElement(bb.getDataPointer(),e);
    
    // bb.reshape(2,rangeDimension);
  }
#else
  //kkc 040309 changed to use the new iterator interface so we can search in general grids
  UnstructuredMappingIterator iter, iter_end;
  UnstructuredMappingAdjacencyIterator vert, vert_end;
  iter_end = end( UnstructuredMapping::EntityTypeEnum(getDomainDimension()));
  bb=0.;
  for ( iter=begin( UnstructuredMapping::EntityTypeEnum(getDomainDimension()));
	iter!=iter_end;
	iter++ )
    {
      int e = *iter;
      for( int dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0,dir) = REAL_MAX;
	  bb(1,dir) = -REAL_MAX;
	}
      
      vert_end = adjacency_end(iter, UnstructuredMapping::Vertex);
      for ( vert=adjacency_begin(iter, UnstructuredMapping::Vertex);
	    vert!=vert_end;
	    vert++ )
	{
	  int v=*vert;
	  for( int dir=0; dir<rangeDimension; dir++ )
	    {
	      bb(0,dir)=min(bb(0,dir),node(v,dir));
	      bb(1,dir)=max(bb(1,dir),node(v,dir));
	    }
	  // bb.reshape(2*rangeDimension);
	}

//       if( false )
//       {
// 	printF("UnstructuredMapping::buildSearchTree: element bounding box =[%e,%e][%e,%e][%e,%e]\n",
// 	       bb(0,0),bb(1,0),  
// 	       bb(0,1),bb(1,1), 
// 	       bb(0,2),bb(1,2));
//       }

      search->addElement(bb.getDataPointer(),e);
    }

#endif

  real time=getCPU()-time0;
  printf("ADTree: Time to insert %i nodes = %8.2e, time per node = %8.2e \n",
	 numberOfElements,time,time/numberOfElements);


  return 0;
}

int UnstructuredMapping::
findClosestEntity( UnstructuredMapping::EntityTypeEnum etype, real x, real y, real z/*=0.*/)
//===========================================================================
/// \details 
///     This function will find the entity of type etype whose "center" is closest to point (x,y,z).
///     It will return -1 if no entity is found (i.e. the point is outside the mesh)
//===========================================================================
{
  if ( !size(UnstructuredMapping::EntityTypeEnum(getDomainDimension())) )
    return -1;

  buildEntity(etype);

  if ( !search ) buildSearchTree();

  GeometricADTTraversor3dInt traversor(*search);

  real bb[6], xcent[3];
  bb[0] = bb[1] = x;
  bb[2] = bb[3] = y;
  bb[4] = bb[5] = z;

  traversor.setTarget(bb);

  int closest = -1;
  real minD = REAL_MAX;
  while (!traversor.isFinished())
    {
      int e = (*traversor).data;
      xcent[0] = xcent[1] = xcent[2] = 0.;
      UnstructuredMappingAdjacencyIterator vert, vert_end;
      vert_end = adjacency_end(etype, e, Vertex);
      int nv=0;
      for ( vert=adjacency_begin(etype, e, Vertex);
	    vert!=vert_end;
	    vert++, nv++ )
	{
	  int v=*vert;
	  for ( int a=0; a<rangeDimension; a++ )
	    xcent[a] += node(v,a);
	}

      for ( int a=0; a<rangeDimension; a++ )
	xcent[a] /= real(nv);

      real d2 = SQR(x-xcent[0]) + SQR(y-xcent[1]) + SQR(z-xcent[0]);
      if ( d2<minD )
	{
	  closest = e;
	  minD = d2;
	}
      traversor++;
    }

  return closest;
  
}

int UnstructuredMapping::
project( realArray & x, MappingProjectionParameters & mpParams )
//===========================================================================
/// \details 
///    Project the points x(i,0:2) onto the surface. Also return index of the element containing
///  the projected point.
/// 
/// \param x (input) : project these points.
/// \param mpParameters : holds auxillary data to aid in the projection:
/// 
/// \param elementIndex (input/output) : The element number of the closest point. On input this is
///     is a guess (if >=0 ) to the closest element (perhaps the value from a previous call) 
/// \param x (input) : project these points onto the surface.
/// \param rProject (input/output) : sub-surface coordinates. On input these are an initial
///     guess. On output they are the actual unit square coordinates.
/// \param xProject (input/output) : on input these are the projected points from the previous
///      step (if subSurfaceIndex>=0 on input). On output these are the projected points.
/// \param xrProject (output) : the derivative of the mapping at xProject
/// \param normal (input/output) : on input this is the normal to the surface at the old point. On output
///    this array then it will hold the normal to the
///     surface, normal(i,0:2). The normal vector will be chosen so that it is consistent
///     across all sub-surfaces
///  
//===========================================================================
{
  real time0=getCPU();
  
  // debugs=4;

  int debugp=debugs>=4;
  
  if( numberOfNodes==0 )
  {
    printf("UnstructuredMapping::project:WARNING: there are no nodes on this Mapping\n");
    return 1;
  }

  if( search==NULL )
  {
    buildSearchTree();
  }
  

  typedef MappingProjectionParameters MPP;

  intArray & subSurfaceIndex = mpParams.getIntArray(MPP::subSurfaceIndex);
  intArray & elementIndex    = mpParams.getIntArray(MPP::elementIndex);
  intArray & ignoreThisSubSurface = mpParams.getIntArray(MPP::ignoreThisSubSurface);

  realArray & rProject  = mpParams.getRealArray(MPP::r);
  realArray & xOld      = mpParams.getRealArray(MPP::x);
  realArray & xrProject = mpParams.getRealArray(MPP::xr);
  realArray & normal    = mpParams.getRealArray(MPP::normal);

  const int xBase = x.getBase(0);
  const int xBound = x.getBound(0);
  if( subSurfaceIndex.getBase(0)>xBase || subSurfaceIndex.getBound(0)<xBound )
  {
    Range R(xBase,xBound);
    subSurfaceIndex.redim(R);
    subSurfaceIndex=-1;  // this means we have no guess for the subsurface
    elementIndex.redim(R);
    elementIndex=-1;     // this means we have no guess at the previous element.
    rProject.redim(R,domainDimension); rProject=.5;
    xOld.redim(R,rangeDimension);   xOld=0.;
    xrProject.redim(R,rangeDimension,domainDimension);
    normal.redim(R,rangeDimension);
  }

  if( domainDimension!=2 || rangeDimension!=3 )
  {
    printf("UnstructuredMapping::project:ERROR: Sorry, the project function only works for 3d surfaces.\n");
    return 0;
  }
  
  const intArray & ef = getElementFaces();

  int nv[3], &n0=nv[0], &n1=nv[1], &n2=nv[2];
  real x0[3], x1[3], x2[3];
  real xx[3], xt[3], xp[3], xa[3], bb[6], xm[3], b[3];
  int intersectionFace,intersectionFace2;
  real rp[2], r,s, rOld,sOld,normalVector[3], normalOld[3];
  const real epsilon=REAL_EPSILON*100.;
  
  // subSurfaceIndex=-1;   // ************** force a global search for debugging ****
  // elementIndex=-1;

  // This next option is used when first projecting onto the triangulation before
  // projecting onto the CompositeSurface -- since we just want the subsurface info
  // but do not want to change the positions of points, unless they were adjusted at corners. 
  const bool adjustAllPoints = ! mpParams.onlyChangePointsAdjustedForCornersWhenMarching();

  // printf("unstructuredProject: adjustAllPoints=%i\n",adjustAllPoints);
  
  const real epsCorner=1.e-3;
  bool globalSearchNeeded=false;
  bool firstCorner=true;
  Range Rx=3;
  int i;
  for( i=xBase; i<=xBound; i++ )
  {
    if( elementIndex(i)>=0 )
    {
      // ----------------------------------------
      // ----- Incremental Search ---------------
      // ----------------------------------------
      // project knowing the old position

      xx[0]=x(i,0); xx[1]=x(i,1); xx[2]=x(i,2); // project this point

      int e0=elementIndex(i);  // we should be on this element
      e0=max(0,min(e0,numberOfElements-1));

      xa[0]=xOld(i,0); xa[1]=xOld(i,1); xa[2]=xOld(i,2);

      r=rProject(i,0); s=rProject(i,1);

      bool cornerFound=false;
      int maxNumberOfSteps=50;  // ***************************************** fix ***************************
      int eOlder=-2, eOld=-1;
      int step;
      for( step=0; step<maxNumberOfSteps; step++ )
      {
        eOlder=eOld; rOld=r, sOld=s;
	eOld=e0;

	n0=element(e0,0), n1=element(e0,1), n2=element(e0,2); // assumes triangles
    
	x0[0]=node(n0,0); x0[1]=node(n0,1); x0[2]=node(n0,2);
	x1[0]=node(n1,0); x1[1]=node(n1,1); x1[2]=node(n1,2);
	x2[0]=node(n2,0); x2[1]=node(n2,1); x2[2]=node(n2,2);

        // check if the projected point is inside this triangle, if not find where
        // we leave the triangle
	bool inside = projectOnTriangle(x0,x1,x2, xa,xx, xt,intersectionFace,intersectionFace2,r,s );

	if( inside )
	{
	  if( debugp ) printf("**** Incremental projection: point (%6.2e,%6.2e,%6.2e) "
               "is inside element %i, (r,s)=(%e,%e)\n",xx[0],xx[1],xx[2],e0,r,s);
          break;
	}
	else
	{
	  // move to the element on the other side of the intersection face.
	  int f0 = ef(e0,intersectionFace);

	  int e1;  // this will be the element we move to
	  if( faceElements(f0,0)==e0 )
	    e1=faceElements(f0,1);
	  else
	    e1=faceElements(f0,0);
	
          if( e1==eOlder )
	  {
	    // we may have landed on a node in which case we need to avoid moving back to the same
            // element that we were on before
            //             \  x  /
            //              \ e0/
            //               \ /  e1
            //         -------o-------
            //               / \
            //              /   \
            //                xx
	    bool corner = (int(fabs(r) < epsCorner) + int(fabs(r+s-1.) < epsCorner) + int(fabs(s) < epsCorner))==2;
	    if( corner )
	    {
              f0 = ef(e0,intersectionFace2);
	      if( faceElements(f0,0)==e0 )
		e1=faceElements(f0,1);
	      else
		e1=faceElements(f0,0);

	    }
	  }
	  

          if( debugp )
	  {
            printf("Incremental: step=%i, e0=%i to e1=%i. interFace=%i, e0: xt=(%e,%e,%e) (r,s)=(%e,%e)\n",
                   step,e0,e1,intersectionFace,xt[0],xt[1],xt[2],r,s);

	    int ae0 = faceElements(ef(e0,0),0)==e0 ? faceElements(ef(e0,0),1) : faceElements(ef(e0,0),0);
	    int ae1 = faceElements(ef(e0,1),0)==e0 ? faceElements(ef(e0,1),1) : faceElements(ef(e0,1),0);
	    int ae2 = faceElements(ef(e0,2),0)==e0 ? faceElements(ef(e0,2),1) : faceElements(ef(e0,2),0);
	  
	    printf(" element %i: nodes: (%i,%i,%i) faces: (%i,%i,%i) adj elements: (%i,%i,%i)\n",
		   e0,element(e0,0),element(e0,1),element(e0,2),ef(e0,0),ef(e0,1),ef(e0,2),ae0,ae1,ae2);

            if( e1>=0 )
	    {
	      ae0 = faceElements(ef(e1,0),0)==e1 ? faceElements(ef(e1,0),1) : faceElements(ef(e1,0),0);
	      ae1 = faceElements(ef(e1,1),0)==e1 ? faceElements(ef(e1,1),1) : faceElements(ef(e1,1),0);
	      ae2 = faceElements(ef(e1,2),0)==e1 ? faceElements(ef(e1,2),1) : faceElements(ef(e1,2),0);
	      printf(" element %i: nodes: (%i,%i,%i) faces: (%i,%i,%i) adj elements: (%i,%i,%i)\n",
		     e1,element(e1,0),element(e1,1),element(e1,2),ef(e1,0),ef(e1,1),ef(e1,2),ae0,ae1,ae2);
	    }
	  }
	  
                 
	  // now project the segment (xt,xx) on the element e1.

	  xa[0]=xt[0];  xa[1]=xt[1]; xa[2]=xt[2];
          if( e1>=0 )
	  {
            e0=e1;
	  }
	  else
	  {
            // if e1<0 we must have hit a boundary face

            //  check if we at a corner -> also check the other face
            bool corner = (int(fabs(r) < epsCorner) + int(fabs(r+s-1.) < epsCorner) + int(fabs(s) < epsCorner))==2;
	    if( corner )
	    {
	      f0 = ef(e0,intersectionFace2);
	      if( f0>=0 )
	      {
		if( faceElements(f0,0)==e0 )
		  e1=faceElements(f0,1);
		else
		  e1=faceElements(f0,0);


		// force a global search in this case since we are on a element with two boundary faces
                // this is a touchy case since it may be a local minimum in distance but not global.
		if( debugp ) printf(" corner! ** force a global search\n");
		e0=-1;
		elementIndex(i)=-1;  // this will force a global search below
                globalSearchNeeded=true;
		break;

	      }
	      if( debugp ) printf(" corner! intersectionFace2=%i, new face = %i, e1=%i\n",intersectionFace2,f0,e1);

	      
	    }

	    if( e1>=0 )
              e0=e1;
	    else
	      break;
	  }
	  
          if( e0==eOlder || e0==eOld || e1<0 )  // make sure we have not revisited the previous element
	  {
	    if( e0==eOlder )
	    {
	      r=rOld, s=sOld;   // reset (r,s) to values from eOlder
	    }
            break;
	  }
	  
          
          // The new element is e0, the old element is eOld
          // *** check for a corner in the surface ****
          if( mpParams.adjustForCornersWhenMarching() &&
              elementIndex(i)>=0 ) // only check if we are marching from a previous value.
	  {
	    // normal0[3] normalOld[3]
	    getNormal( e0,normalVector );
	    getNormal( eOld,normalOld );
	    real cosTheta=normalVector[0]*normalOld[0]+normalVector[1]*normalOld[1]+normalVector[2]*normalOld[2];

	    if( cosTheta < .75 ) // no need to adjust if cos(theta) is near +1.
	    {
	      if( firstCorner ){ firstCorner=false; printf("UnstructuredMapping::project: \n");  }
	      cornerFound=true;
	      
              printf(" -> Corner detected i=%i, e0=%i, eOld=%i, "
                     "cosTheta(nOld.nNew)=%8.2e (<.75) (stepping from prev el=%i)\n",
                         i,e0,eOld,cosTheta,elementIndex(i));

	      // rotate the remaining portion of the marching vector around the corner by an angle theta.
	      // theta is the angle between the old normal and the new normal.
              //     xm[] =  xx[]-xt[]    :  
              xm[0]=xx[0]-xt[0];
              xm[1]=xx[1]-xt[1];
              xm[2]=xx[2]-xt[2];
	      
           
	      // b = the vector orthogonal to oldNormal and in the plane of oldNormal and normal
	      // (b is not the tangent since the tangent may cross the corner at an angle).
	      b[0]=normalVector[0]-cosTheta*normalOld[0];
	      b[1]=normalVector[1]-cosTheta*normalOld[1];
	      b[2]=normalVector[2]-cosTheta*normalOld[2];
	      real normB =b[0]*b[0]+b[1]*b[1]+b[2]*b[2];

	      if( normB>epsilon )
	      {
                normB=1./normB;
		b[0]*=normB;
		b[1]*=normB;
		b[2]*=normB;
		
		const real sinTheta = b[0]*normalVector[0]+b[1]*normalVector[1]+b[2]*normalVector[2];

		// tangent = alpha*a() + beta*b() + gamma*c()
		//       a() = old normal
		//       c() = new Normal X old Normal (normalized)
		//       b() = a() X c() =  normal() - cosTheta*oldNormal()  (normalized)
		// new tangent = (alpha*cos-gamma*sin) a() + (+alpha*sin+beta*cos) b() + gamma c()
		//             = oldTangent + (alpha*(cos-1)+gamma*sin) a() + (-alpha*sin+beta*(cos-1)) b()
		real alpha=xm[0]*normalOld[0]+xm[1]*normalOld[1]+xm[2]*normalOld[2];
		real beta=xm[0]*b[0]+xm[1]*b[1]+xm[2]*b[2];
		xm[0]+=(alpha*(cosTheta-1.)-beta*sinTheta)*normalOld[0]+(alpha*sinTheta+beta*(cosTheta-1.))*b[0];
		xm[1]+=(alpha*(cosTheta-1.)-beta*sinTheta)*normalOld[1]+(alpha*sinTheta+beta*(cosTheta-1.))*b[1];
		xm[2]+=(alpha*(cosTheta-1.)-beta*sinTheta)*normalOld[2]+(alpha*sinTheta+beta*(cosTheta-1.))*b[2];
      
      
                // *** adjust xx and then we will go back and keep searching *****
                xx[0]=xt[0]+xm[0];
                xx[1]=xt[1]+xm[1];
                xx[2]=xt[2]+xm[2];
		
                if( true || debugp ) 
		{
		  printf("    :pt %i x=(%8.2e,%8.2e,%8.2e) xOld=(%8.2e,%8.2e,%8.2e) xt=(%8.2e,%8.2e,%8.2e)\n "
                         " xm=(%8.2e,%8.2e,%8.2e), bent around corner to pt (%8.2e,%8.2e,%8.2e)\n",
			 i,x(i,0),x(i,1),x(i,2), xOld(i,0),xOld(i,1),xOld(i,2),xt[0],xt[1],xt[2],
                            xm[0],xm[1],xm[2],xx[0],xx[1],xx[2]);
		}
// 		printf("UNS: Moving pt %4i around the corner n=(%4.1f,%4.1f,%4.1f), t=(%4.1f,%4.1f,%4.1f), cos=%6.2e"
// 		       " sin=%6.2e b=(%4.1f,%4.1f,%4.1f), n_old=(%4.1f,%4.1f,%4.1f)\n",i,
// 		       normal(i,0),normal(i,1),normal(i,2), tangent(0,0),tangent(0,1),tangent(0,2),cosTheta,sinTheta,
// 		       b(0,0),b(0,1),b(0,2), oldNormal(i,0),oldNormal(i,1),oldNormal(i,2));
	      }
	      else if( cosTheta<0. )
	      {
		// this must be a 180 degree turn! really need to get the tangent to the edge in this case
		printf("UNS:project:WARNING: the corner has apparently rotated by 180 degrees, cos(theta)=%e\n"
		       "Setting t -> -t , this case should be handled in a better way",cosTheta);
	  
                xx[0]-=xm[0];
                xx[1]-=xm[1];
                xx[2]-=xm[2];
	      }

	    }
	    
	  }
	  
	  if( false && step>= (maxNumberOfSteps-5) )
	  {
            real dist = SQRT( SQR(xx[0]-xt[0])+SQR(xx[1]-xt[1])+SQR(xx[2]-xt[2]) );
	    printf("*WARNING* step=%i, increm. search: e0=%i, eOld=%i, init guess=%i,"
		   "dist=%8.2e search pt x=(%9.3e,%9.3e,%9.3e), xt=(%9.3e,%9.3e,%9.3e)\n",
		   step,e0,eOld,elementIndex(i),dist,xx[0],xx[1],xx[2],xt[0],xt[1],xt[2]);
	  }

	}
      }  // end for step
      if( step>=maxNumberOfSteps )
      {
	printf("UnstructuredMapping::project:WARNING: i=%i, incremental search, step>=maxNumberOfSteps=%i. "
               "Will perform a global search\n", i,maxNumberOfSteps);

	elementIndex(i)=-1;  // this will force a global search below
        globalSearchNeeded=true;
	
      }
      else if( e0>=0 )
      {
        // ***** The point was found *****
	rProject(i,0)=r; rProject(i,1)=s;
	elementIndex(i)=e0;
        if( adjustAllPoints || cornerFound )
	{
	  x(i,0)=xt[0]; x(i,1)=xt[1]; x(i,2)=xt[2];
          xOld(i,Rx)=x(i,Rx);       // save current value for future reference.
	
	  // getNormal( e0,normalVector );
	  // normal(i,0)=normalVector[0]; normal(i,1)=normalVector[1]; normal(i,2)=normalVector[2];
	}
        // *wdh* 070427 -- compute normals even if we don't adjust the points
	getNormal( e0,normalVector );
	normal(i,0)=normalVector[0]; normal(i,1)=normalVector[1]; normal(i,2)=normalVector[2];

	if( debugp ) printf("UnstructuredMapping::project: local search: i=%i, closest element=%i, "
			    "normal=(%8.2e,%8.2e,%8.2e) \n",
			    i,e0, normal(i,0),normal(i,1),normal(i,2));
      }
      
    } // end local search
    else
    {
      globalSearchNeeded=true;
    }
    
  } // end for i
  real time=getCPU();
  timing[timeForProjectLocalSearch]+=time-time0;
  
  if( globalSearchNeeded )
  {
    
    // compute a box size for the global search -- the box centred at the point
    // must intersect a bounding box around one of the elements.
    real delta=mpParams.searchBoundingBoxSize*.5;
    real searchBoundingBoxMaximumSize = mpParams.searchBoundingBoxMaximumSize;
    if( searchBoundingBoxMaximumSize==0. )
      searchBoundingBoxMaximumSize=REAL_MAX;
  
    if( delta==0. )
    {
      for( int dir=0; dir<rangeDimension; dir++ )
	delta=max(delta,.01*((real)getRangeBound(End,dir)-(real)getRangeBound(Start,dir)));
      if( delta==0. )
	delta=1.;
    }
  
    GeometricADTTraversor3dInt traversor(*search);  // fix this -- just reset target

    for( i=xBase; i<=xBound; i++ )
    {

      if( elementIndex(i)==-1 )
      {
	// ====== perform a global search  =============

	xx[0]=x(i,0); xx[1]=x(i,1); xx[2]=x(i,2); 
	real minDist=REAL_MAX;
	int eMin=-1;  // closest element
	int numberChecked=0;  // keep track of how many ADT leaves we check
	
	for( int it=0; it<5; it++ )  // we may have to increase the box
	{

	  bb[0]=xx[0]-delta, bb[1]=xx[0]+delta;
	  bb[2]=xx[1]-delta, bb[3]=xx[1]+delta;
	  bb[4]=xx[2]-delta, bb[5]=xx[2]+delta;
	  traversor.setTarget(bb);
	  
          numberChecked=0;
	  while( !traversor.isFinished() )
	  {
	    // printf(" (%8.2e,%8.2e,%8.2e) is inside box %i \n",x,y,z,leaf.data);
	    int e = (*traversor).data;
	    numberChecked++;
	    
	    //we are inside the bounding box of element e
	    n0=element(e,0), n1=element(e,1), n2=element(e,2); // assumes triangles
    
	    x0[0]=node(n0,0); x0[1]=node(n0,1); x0[2]=node(n0,2);
	    x1[0]=node(n1,0); x1[1]=node(n1,1); x1[2]=node(n1,2);
	    x2[0]=node(n2,0); x2[1]=node(n2,1); x2[2]=node(n2,2);

	    projectOnTriangle(x0,x1,x2, x0,xx, xt,intersectionFace,intersectionFace2,r,s );
	    real dist= SQR(xt[0]-xx[0])+SQR(xt[1]-xx[1])+SQR(xt[2]-xx[2]);

	    // printf(" Check element e=%i dist=%8.2e\n",e,dist);
	    if( dist<minDist )
	    {
	      eMin=e;
	      minDist=dist;
	      xp[0]=xt[0], xp[1]=xt[1], xp[2]=xt[2];
	      rp[0]=r; rp[1]=s;
	      if( dist==0. )
		break;
	    }
	  

	    traversor++;
	  }

	  // we must check that xp is inside the bounding box or else we could have made a mistake --
	  // there may be a very large element with a big bounding box that we have found, but it may not
	  // be the closest element.
	  if( eMin==-1 || xp[0]<bb[0] || xp[0]>bb[1] || xp[1]<bb[2] || xp[1]>bb[3] || xp[2]<bb[4] || xp[2]>bb[5] )
	  {
	    // no elements were found, the bounding box must be too small
	    if( delta*2.01 > searchBoundingBoxMaximumSize )
	    {
	      // look no further, the box has gotten as big as allowed.
	      if( eMin==-1 )
	      {
		// put bogus but valid values in.
		if( true || debugp ) 
                   printf("project: No bounding box found. Setting bogus values. it=%i delta=%8.2e, maxBoxSize=%e\n",
                             it,delta,searchBoundingBoxMaximumSize);

		eMin=-2;  // this means no point was found.
		minDist=1.e5;
		xp[0]=xx[0]+minDist, xp[1]=xx[1]+minDist, xp[2]=xx[2]+minDist;
		rp[0]=-1., rp[1]=-1.;
	      }
	      break;
	    }
	    else
	    {
              if( debugp ) 
	      {
		if( eMin==-1 )
		{
		  printf("i=%i no elements were found. bounding box must be too small. delta=%8.2e "
			 "trying again with a bigger box.\n",i,delta);

                  bool inside=true;
		  for( int dir=0; dir<rangeDimension; dir++ )
		  {
		    inside=inside && xx[dir]>=(real)getRangeBound(Start,dir) &&
		      xx[dir]<=(real)getRangeBound(End,dir);
		  }
		  if( !inside )
		  {
		    printf(" WARNING: Point is outside the bounding box!! "
                       "xx=(%9.2e,%9.2e,%9.2e) minDist=%8.2e \n",xx[0],xx[1],xx[2],minDist );
		    
		    for( int dir=0; dir<rangeDimension; dir++ )
		      printf(" getRangeBound(0:1,%i)=[%8.2e,%8.2e] ",dir,(real)getRangeBound(Start,dir),
			     (real)getRangeBound(End,dir));
		    printf("\n");
		  }
		  
		}
		else
		  printf("i=%i Projected pt is not in the bounding box! The bounding box is too small, trying again.\n",i);
	      }
	      
	      if( it==0 || eMin>=0 )
	      {
		delta*=2.;
	      }
	      else
	      {
		// we could compute the distance between xx and the bounding box.
		delta*=4.;
	      }
	    }
	    
	  }
	  else
	  {
	    break;  // we have found the closest element
	  }
	  
	}  // end for it
        if( numberChecked>20 )
	{
	  if(  debugp ) 
             printf("UnstructuredMapping:project:INFO: %i ADT leaves were checked. I will decrease the search box.\n",
                numberChecked);
	  delta*=.5;
	}

	if( eMin==-1 )
	{
	  printf("UnstructuredMapping:project:i=%i project:WARNING: no elements were found! \n",i);
	}
	else if( eMin==-2 )
	{
	  eMin=-1;
	}
	else
	{
          assert( eMin>=0 );
	  elementIndex(i)=eMin;
	  rProject(i,0)=rp[0]; rProject(i,1)=rp[1];
	  if( adjustAllPoints )
	  {
	    x(i,0)=xp[0]; x(i,1)=xp[1]; x(i,2)=xp[2];
            xOld(i,Rx)=x(i,Rx);       // save current value for future reference.

	    // getNormal( eMin, normalVector );
	    // normal(i,0)=normalVector[0]; normal(i,1)=normalVector[1]; normal(i,2)=normalVector[2];

	  }
          // *wdh* 070427 -- compute normals even if we don't adjust the points
	  
	  getNormal( eMin, normalVector );
	  normal(i,0)=normalVector[0]; normal(i,1)=normalVector[1]; normal(i,2)=normalVector[2];

	  if( debugp ) printf("uns-project: global search: found pt i=%i eMin=%i minDist=%8.2e "
			      " x=(%8.2e,%8.2e,%8.2e) xp=(%8.2e,%8.2e,%8.2e) n=(%8.2e,%8.2e,%8.2e)\n",i,eMin,
			      minDist,xx[0],xx[1],xx[2],xp[0],xp[1],xp[2],normalVector[0],normalVector[1],
			      normalVector[2]);

	}
	
      } 

    }  // for i
  } // end global search
  
  if( debugp )
  {
    Range R(xBase,xBound);
    if( min(elementIndex(R))<0 )
    {
      printf("UnstructuredMapping::ERROR: some values in elementIndex were not assigned\n");
      ::display(elementIndex,"elementIndex");
    }
  }
  

  timing[timeForProjectGlobalSearch]+=getCPU()-time;
  timing[totalTime]+=getCPU()-time0;
  
  return 0;
}




int UnstructuredMapping::
insideOrOutside( realArray & x, IntegerArray & inside )
//===========================================================================
/// \details 
///    Determine whether the points x(i,0:2) are inside the surface triangulation using ray tracing.
///  Count the number of times a ray crosses the triangulation.
///  The surface triangulation is assumed to be water-tight. 
/// 
/// \param x (input) : check these points these points.
/// \param inside (output) : inside(i) = true if point x(i,0:2) is inside 
/// \return  If successful, the return value is inside(0). Return -1 if there was an error.
///  
//===========================================================================
{
  real time0=getCPU();  

  int debugp=0;    
  // debugp=7; // turn this on for debugging
  
  if( numberOfNodes==0 )
  {
    printf("UnstructuredMapping::project:WARNING: there are no nodes on this Mapping\n");
    return -1;
  }

  if( search==NULL )
  {
    buildSearchTree();
  }

  real boundingBoxp[6], length[3], xMid[3];
  #define boundingBox(side,axis) boundingBoxp[(side)+2*(axis)]
 
  for( int axis=0; axis<rangeDimension; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      boundingBox(side,axis)=(real)getRangeBound(side,axis);
    }
  
    length[axis]=(boundingBox(1,axis)-boundingBox(0,axis))*1.2;
    xMid[axis]=.5*(boundingBox(0,axis)+boundingBox(1,axis));
  }
  
  const int xBase=x.getBase(0), xBound=x.getBound(0);

  if( inside.getBase(0)>xBase || inside.getBound(0)<xBound )
  {
    inside.redim(Range(xBase,xBound));
  }
  
  
  const real epsEdge = REAL_EPSILON*100.;  // 1.e-4;   // double check points that are this close to the boundary of a triangle,
  // the distance is in normalized triangle coordinates

  const real epsY=(boundingBox(End,axis2)-boundingBox(Start,axis2))*REAL_EPSILON*10.;

  real epsX=(boundingBox(End,axis1)-boundingBox(Start,axis1))*epsEdge;
  real epsZ=(boundingBox(End,axis3)-boundingBox(Start,axis3))*epsEdge;
  epsZ*=1.23456789; // add a random perturbation so the virtual perurbation is unlikely to
  // shift along the direction of a grid line
  
  real rayDepth[3]={1.,0.5,1.}; // direction of the virtual perturbation

  // Cast a ray in the b1-direction, [b0,b1,b2] should form an orthonomal basis
  // Note: there is a special efficient implementation for b1=[0,1,0]
//    real b0[3]={1.,0.,0.}; // 
//    real b1[3]={0.,1.,0.}; // cast along y+
//    real b2[3]={0.,0.,1.}; //

  real b0[3]={0.,0.,1.}; // 
  real b1[3]={1.,0.,0.}; // cast along x+
  real b2[3]={0.,1.,0.}; //


  GeometricADTTraversor3dInt traversor(*search); 
  real bb[6], xv[3], xi[3], x0[3],x1[3],x2[3];
  Triangle tri;

  // The xx array holds the points where the ray intersects the triangulation
  int maxNumberOfCrossings=5;  // initial estimate -- this will increase if needed.
  //  realArray xx(Range(xBase,xBound),3,maxNumberOfCrossings);
  realArray xx(1,3,maxNumberOfCrossings);

  intArray crossings(Range(xBase,xBound));
  crossings=0;

  for( int i=xBase; i<=xBound; i++ )
  {
    int cross=crossings(i);
    int initialCross=cross;  // the list may already contain crossing info from previous calls, keep the
    
    xv[0]=x(i,0); xv[1]=x(i,1); xv[2]=x(i,2); 

    if( xv[0]<boundingBox(0,0) || xv[0]>boundingBox(1,0) ||
        xv[1]<boundingBox(0,1) || xv[1]>boundingBox(1,1) ||
        xv[2]<boundingBox(0,2) || xv[2]>boundingBox(1,2) )
    {
      inside(i)=0;
      continue;
    }
    
    // *** Todo: Base direction of the ray casting on where we are in the bounding box
    //     Choose a direction that most likely will result in the fewest intersections

    //          -----------------
    //          | \  .  ^  .  / |
    //          |    .  |  .    |
    //          |...............|   
    //          |    .     .    |
    //          | <- .  ?  . -> |
    //          |    .     .    |
    //          |...............|   
    //          |    .  |  .    |
    //          | /  .  V  .  \ |
    //          -----------------


    if( xv[0]<xMid[0] )
    {
//        b0[0]= 1.;  b0[1]= 0.;  b0[2]=0.; 
//        b1[0]= 0.;  b1[1]= 1.;  b1[2]=0.;
//        b2[0]= 0.;  b2[1]= 0.;  b2[2]=1.;

      b0[0]= 0.;  b0[1]= 0.;  b0[2]=-1.; 
      b1[0]=-1.;  b1[1]= 0.;  b1[2]=0.;
      b2[0]= 0.;  b2[1]=+1.;  b2[2]=0.;
       
    }
    else
    {
      b0[0]= 0.;  b0[1]= 0.;  b0[2]= 1.; 
      b1[0]= 1.;  b1[1]= 0.;  b1[2]= 0.;
      b2[0]= 0.;  b2[1]= 1.;  b2[2]= 0.;
    }

    // Make a long thin box that extends from xv in the direction b1 to the point at infinity
    if( b1[0]>=0. )
      bb[0]=xv[0], bb[1]=xv[0]+b1[0]*length[0];
    else
      bb[1]=xv[0], bb[0]=xv[0]+b1[0]*length[0];
    if( b1[1]>=0. )
      bb[2]=xv[1], bb[3]=xv[1]+b1[1]*length[1];
    else
      bb[3]=xv[1], bb[2]=xv[1]+b1[1]*length[1];
    if( b1[2]>=0. )
      bb[4]=xv[2], bb[5]=xv[2]+b1[2]*length[2];
    else
      bb[5]=xv[2], bb[4]=xv[2]+b1[2]*length[2];

    bool virtualPerturbation=false;

    traversor.setTarget(bb);  // Look for intersections of the box bb with the boxes in the ADT

    int numberChecked=0;
    while( !traversor.isFinished() )
    {
      int e = (*traversor).data;
      numberChecked++;
	    
      if( debugp & 4 )
        printf(" The ray from (%8.2e,%8.2e,%8.2e) is inside the ADT box for element %i \n",xv[0],xv[1],xv[2],e);

      //we are inside the bounding box of element e
      int n0=element(e,0), n1=element(e,1), n2=element(e,2); // assumes triangles
    
      x0[0]=node(n0,0); x0[1]=node(n0,1); x0[2]=node(n0,2);
      x1[0]=node(n1,0); x1[1]=node(n1,1); x1[2]=node(n1,2);
      x2[0]=node(n2,0); x2[1]=node(n2,1); x2[2]=node(n2,2);

        // ****************** return here to redo ***********************
      redo:


      bool intersectionFound=false;

      tri.setVertices(x0,x1,x2);
        
      if( virtualPerturbation )
      {
	tri.x1[0]+=epsX*rayDepth[0]; tri.x1[1]+=epsY*rayDepth[1];  tri.x1[2]+=epsZ*rayDepth[2];
	tri.x2[0]+=epsX*rayDepth[0]; tri.x2[1]+=epsY*rayDepth[1];  tri.x2[2]+=epsZ*rayDepth[2];
	tri.x3[0]+=epsX*rayDepth[0]; tri.x3[1]+=epsY*rayDepth[1];  tri.x3[2]+=epsZ*rayDepth[2];
      }
		  
      if( debugp & 4 )
	tri.display("check this triangle");
      // find any crossings with the triangles
      // xi[0]=-999.; xi[1]=-999.; xi[2]=-999.;
		  
       // does triangle intersect a ray from xv in the direction b1 ? 
      int intersection=tri.intersects(xv,xi,b0,b1,b2); 

      if( intersection )   // xi[3] holds the point of intersection
      {
	if( debugp & 4 )
	  printf(" The ray intersects the triangle e=%i! intersection=%i cross=%i, xi=(%e,%e,%e)\n",e,intersection,
               cross,xi[0],xi[1],xi[2]);
		    
	if( intersection<=1 )
	{ // non-degenerate intersection
	  intersectionFound=true;  // remove multiples later
	}
	else
	{ // degenerate intersection, we need to redo with a perturbed geometry : 
                      
	  if( !virtualPerturbation )
	  {
	    if( debugp & 4 )
	      printf("REDO point %i with a virtual perturbation, epsX=%e, epsY=%e, epsZ=%e\n",i,epsX,epsY,epsZ);
	    // go back and redo this point with a perturbed geometry
	    virtualPerturbation=true;
	    goto redo;
	  }
	  else if( virtualPerturbation )
	  {
	    printF("UnstructuredMapping:unstructuredProject:ERROR: degenerate intersection with a triangle"
                   " found after the virtual perturbation. This should not happen.\n");
	    OV_ABORT("error");
	  }
	  else 
	  {
	    real alpha1,alpha2;
	    int ok = tri.getRelativeCoordinates(xi,alpha1,alpha2);   // get "triangle" parameter coordinates
	    if( ok!=0 )
	      printf(" ERROR from getRelativeCoordinates, x0=(%e,%e,%e), xi=(%e,%e,%e) \n",
		     x0[0],x0[1],x0[2],xi[0],xi[1],xi[2]);
			
	    if( fabs(alpha2)<epsEdge || fabs(alpha1)<epsEdge || fabs(alpha1+alpha2-1.)<epsEdge )
	    {
	      // we are on the boundary of a triangle, perturb the triangle and recheck.
	      // Note that all triangles are perturbed in the same way so that if the ray
	      // hits the intersection of two triangles both triangles will be perturbed. **check this **

	      // ** potential problem: what if the other possible intersection is not detected? ***

	      if( debugp & 4 )
	      {
		printf("INFO:countCrossings: intersection of the edge of a triangle"
		       " - perturbing to recheck\n");
		printf(" epsX=%e, epsZ=%e, alpha1=%e, alpha2=%e, alpha1+alpha2=%e \n",epsX,epsZ,
		       alpha1,alpha2,alpha1+alpha2);
	      }
	      // tri.x1[0]+=epsX;  tri.x1[2]+=epsZ;
	      // tri.x2[0]+=epsX;  tri.x2[2]+=epsZ;
	      // tri.x3[0]+=epsX;  tri.x3[2]+=epsZ;

	      tri.x1[0]+=epsX*rayDepth[0]; tri.x1[1]+=epsY*rayDepth[1];  tri.x1[2]+=epsZ*rayDepth[2];
	      tri.x2[0]+=epsX*rayDepth[0]; tri.x2[1]+=epsY*rayDepth[1];  tri.x2[2]+=epsZ*rayDepth[2];
	      tri.x3[0]+=epsX*rayDepth[0]; tri.x3[1]+=epsY*rayDepth[1];  tri.x3[2]+=epsZ*rayDepth[2];

	      if( tri.intersects(xv,xi,b0,b1,b2) )
	      {
		tri.getRelativeCoordinates(xi,alpha1,alpha2);
		if( fabs(alpha1)<epsEdge || fabs(alpha2)<epsEdge || fabs(alpha1+alpha2-1.)<epsEdge )
		{
		  printf("ERROR:countCrossings: still on an edge after perturbing!. ****** \n");
		  printf(" epsX=%e, epsZ=%e, alpha1=%e, alpha2=%e, alpha1+alpha2=%e \n",epsX,epsZ,
			 alpha1,alpha2,alpha1+alpha2);
			    
		  printf(" ****** For now count the intersection, fix this case Bill ! ****** \n");
		  intersectionFound=true;
		}
		else
		{
		  if( debugp & 4 )
		    printf("INFO:countCrossings: intersection found after perturbing\n");
		  intersectionFound=TRUE;
		}
	      }
	      else
	      {
		if( debugp & 4 )
		  printf("INFO:countCrossings: no intersection found after perturbation.\n");
	      }
	    }
	    else
	      intersectionFound=true;
	  }
	}
      }
      if( intersectionFound )
      {
	// save the point of intersection
	if( xx.getLength(2) <= cross )
	{
	  // allocate more space for crossings
	  printf("countCrossingsWithPolygon:INFO:Too many crossings: allocating more space\n");
	  xx.resize(xx.dimension(0),xx.dimension(1),xx.getLength(2)+5);
	}
	

	//	xx(i,0,cross)=xi[0];   
	//	xx(i,1,cross)=xi[1];
	//	xx(i,2,cross)=xi[2];

	xx(0,0,cross)=xi[0];   
	xx(0,1,cross)=xi[1];
	xx(0,2,cross)=xi[2];
		    
	for( int k=initialCross; k<cross; k++ )
	{
	  // double check for multiple counts of the same point
	  //  A double crossing may occur at a corner of a grid
	  //          ---------------+ <- ray crosses this point twice
	  //                         |
	  //                         |
	  //                         X <- point to check
	  //	  if( xx(i,0,k)==xi[0] && xx(i,1,k)==xi[1] && xx(i,2,k)==xi[2] )
	  if( xx(0,0,k)==xi[0] && xx(0,1,k)==xi[1] && xx(0,2,k)==xi[2] )
	  {
	    if( debug & 4 )
	      printf("INFO:countCrossings: EXACTLY the same point appears more"
		     " than once. I am NOT going to remove it, x0=(%e,%e,%e)\n",x0[0],x0[1],x0[2]);
	    // cross--;
	  }
	  //	  else if( fabs(xx(i,0,k)-xi[0])< epsX &&
	  //                   fabs(xx(i,1,k)-xi[1])< epsY &&
	  //                   fabs(xx(i,2,k)-xi[2])< epsZ )
	  else if( fabs(xx(0,0,k)-xi[0])< epsX &&
                   fabs(xx(0,1,k)-xi[1])< epsY &&
                   fabs(xx(0,2,k)-xi[2])< epsZ )
	  {
	    printf("INFO:countCrossings: NEARLY the same point appears more"
		   " than once. I am NOT going to remove it, x=(%e,%e,%e)\n",x0[0],x0[1],x0[2]);
	    // cross--;
	    break;
	  }
	}
	cross++;

      } // if intersection found

      virtualPerturbation=false;
      traversor++;
    } // end while traversor

    crossings(i)=cross;

    if( debugp & 4 )
      printf("countCrossing: checked point i=%i, x=(%e,%e,%e), crossings=%i \n\n",
	     i,xv[0],xv[1],xv[2],crossings(i));

    inside(i)= (crossings(i) % 2)==1;
    
  } // end for i

  timing[timeForInsideOrOutside]+=getCPU()-time0;
  timing[totalTime]+=getCPU()-time0;
  
  if( (debugp & 2) )
    printf(" --> Time for determining inside or outside by ray tracing = %8.2e (s) (%i points)\n",
                 timing[timeForInsideOrOutside],xBound-xBase+1);
  

  return 0;
}
