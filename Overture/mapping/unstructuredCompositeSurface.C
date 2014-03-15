#include "UnstructuredMapping.h"
#include "MappingProjectionParameters.h"
#include "display.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"

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
splitElement( int e, int relativeEdge, real *x )
//===========================================================================================
/// \details 
///     Add a new node to the edge of an element and update the connectivity information.
/// \param e (input) : element to split.
/// \param relativeEdge (input) : edge to split, relative edges are numbered 0,1,2..
/// \param x (input) : point on the edge (or near the edge). This point will be a new node.
/// 
//==========================================================================================
{
  if( domainDimension!=2 )
  {
    printf("UnstructuredMapping::splitElement:Sorry, only implemented for domainDimension==2 \n");
    return 1;
  }
  bool connectivityComputed = faceElements.getLength(0)>0;  // fix this *************
  
  if( !connectivityComputed )
  {
    printf("UnstructuredMapping::splitElement:Sorry, connectivity has not yet been computed\n");
    return 1;
  }
  

  //               2
  //             / |  
  //       2   /   |  
  //          /    x  1
  //         /     |  
  //       /       |  
  //      0--------1
  //          0


  const int extraNodesToAdd=10;
  const int extraElementsToAdd=10;
  const int extraFacesToAdd=2*extraNodesToAdd;
  

  // add a new node
  if( numberOfNodes > node.getBound(0) )
  {
    node.resize(numberOfNodes+extraNodesToAdd,node.dimension(1));    // add some extra space for future additions ****
  }
  const int n=numberOfNodes;
  for( int axis=0; axis<rangeDimension; axis++ )
    node(n,axis)=x[axis];
  numberOfNodes++;
    

  // add a new element
  if( numberOfElements>element.getBound(0) )
  {
    element.resize(numberOfElements+extraElementsToAdd,element.dimension(1)); // add some extra space.
  }
  
  assert( relativeEdge>=0 && relativeEdge<=2 );

  element(numberOfElements,0)=element(e,0);
  element(numberOfElements,1)=element(e,1);
  element(numberOfElements,2)=element(e,2);

  element(numberOfElements,relativeEdge)=n;
   
  const int eNew=numberOfElements;
  
  numberOfElements++;
  
  element(e,(relativeEdge+1)%3)=n;  //  change the existing element


  // update the connectivity
  intArray & ef = (intArray&)getElementFaces();
    
  // add a new face
  if( numberOfFaces>faceElements.getBound(0) )
    faceElements.resize(numberOfFaces+extraFacesToAdd,faceElements.dimension(1));

  if( numberOfElements>ef.getBound(0) )
    ef.resize(numberOfElements+extraElementsToAdd,ef.dimension(1));


  const int fNew=numberOfFaces;
  faceElements(fNew,0)=e;    // here is the new face we add
  faceElements(fNew,1)=eNew;
  numberOfFaces++;

  const int fNew2=numberOfFaces;
  faceElements(fNew2,0)=eNew;    // here is the second new face we add
  faceElements(fNew2,1)=-1;      // *** assumes we add to the boundary.
  numberOfFaces++;
  

  // change relative face fc of element e:
  const int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);    // faces on this element
  const int fc = ef(e,(relativeEdge+1)%3);  // this face needs to be updated
  assert( fc>=0 );
  
  if( faceElements(fc,0)==e )
    faceElements(fc,0)=eNew;
  else if( faceElements(fc,1)==e )
    faceElements(fc,1)=eNew;
  else
  {
    printf("UnstructuredMapping::splitElement: **ERROR** connectivity wrong, element e=%i, relativeEdge=%i\n",
               e,relativeEdge);
    printf(" faces for element=%i are (%i,%i,%i) \n",e,f0,f1,f2);
    printf(" elements for face %i are (%i,%i) \n",fc,faceElements(fc,0),faceElements(fc,1));
    
  }
  

  // change the face that belong to element e:
  ef(e,(relativeEdge+1)%3)=fNew;
  
  // assign the faces that belong to the new element
  ef(eNew,0)=f0;
  ef(eNew,1)=f1;
  ef(eNew,2)=f2;

  ef(eNew, relativeEdge     )=fNew2;
  ef(eNew,(relativeEdge+2)%3)=fNew;
  
  // adjacent elements


  return 0;
}



int UnstructuredMapping::
computeConnection(int s, int s2, 
                  intArray *bNodep,
                  IntegerArray & numberOfBoundaryNodes,
                  UnstructuredMapping *boundaryp,
                  real epsx,
                  IntegerArray & connectionInfo )
// ===============================================================================================
// /Description:
//    Determine the points on surface 's' that connect to surface 's2'
//
// /connectionInfo (output):
// =============================================================================================
{
  
  UnstructuredMapping & map2 = boundaryp[s2];
  
  intArray & bNode = bNodep[s];
  intArray & bface = map2.bdyFace;
	
  MappingProjectionParameters mpParams;
  intArray & subSurfaceIndex = mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);
  realArray & rProject  = mpParams.getRealArray(MappingProjectionParameters::r);
  

  subSurfaceIndex=-1;  // reset the starting guess.

  realArray x(1,3),x0(1,3);
  Range Rx=rangeDimension;

  int numberConnected=0;
  connectionInfo.redim(numberOfBoundaryNodes(s),6);

  int i;
  for( i=0; i<numberOfBoundaryNodes(s); i++ )
  {
    const int n=bNode(i);   // global node numbering

    assert( n>=0 && n<numberOfNodes );
    x0(0,0)=node(n,0); x0(0,1)=node(n,1); x0(0,2)=node(n,2);

    // check if this boundary node hits a boundary of surface s2.
	
    // check if we are inside the bounding box ****

    x=x0(0,Rx);
    map2.project( x , mpParams );
	
    real dist = SQR(x(0,0)-x0(0,0))+SQR(x(0,1)-x0(0,1))+SQR(x(0,2)-x0(0,2));
	
    printf(" surface s=%i, i=%i, s2=%i, x=(%8.2e,%8.2e,%8.2e) xp=(%8.2e,%8.2e,%8.2e) distance=%e\n",
	   s,i,s2,x0(0,0),x0(0,1),x0(0,2),x(0,0),x(0,1),x(0,2),dist);

    if( dist < epsx ) // && numberOfStitchedElements<6 )
    {
      int localElementNumber = subSurfaceIndex(0);  // we are inside this element of map2.
	
      const int e = map2.tags(localElementNumber);  // elementOffset(s2)+ localElementNumber;
      assert( e>=0 && e<numberOfElements );

      // (r,s) = local triangle coordinates
      real r0=rProject(0,0), r1=rProject(0,1);
	  
// 	    printf("***Create a stitched element by splitting element %i, r=(%g,%g)\n",e,r0,r1);

      real dist0= fabs(r1), dist1=fabs(r0+r1-1.), dist2=fabs(r0);
      real minDist=min(dist0,dist1,dist2);
      int localEdge =  dist0==minDist ? 0 : dist1==minDist ? 1 : 2;

      int f = map2.getElementFaces()(localElementNumber,localEdge);  // face on the boundary
      int bf= -map2.faceElements(f,1)-2;        // map2.bdyFace(bf)
      if( bf>=0 && bf<map2.getNumberOfBoundaryFaces() )
	printf(" f=%i, bf=%i,  map2.bdyFace(bf)=%i (=? f?)\n",f,bf,map2.bdyFace(bf));

      connectionInfo(numberConnected,0)=i;
      connectionInfo(numberConnected,1)=localElementNumber;
      connectionInfo(numberConnected,2)=localEdge;
      connectionInfo(numberConnected,3)=e;
      connectionInfo(numberConnected,4)=f;
      connectionInfo(numberConnected,5)=bf;
      numberConnected++;
      
    }
  }

  return numberConnected;

}


int UnstructuredMapping::
addNodeToInterface( int s1, int & i1, int & j1, int & e1m, int & e1p, IntegerArray & connectionInfo1,
		    int s2, int & i2, int & j2, int & e2m, int & e2p, IntegerArray & connectionInfo2,
		    const intArray & elementFace1, 
		    intArray * bNodep, IntegerArray & faceOffset, IntegerArray & elementOffset,
		    int maxNumberOfElements, int maxNumberOfFaces )
// =====================================================================================================
// /Description:
//     Add a node to the interface when stitching two unstructured grids together.
// =====================================================================================================
{

  // add node i2 of s2 to element e1m of s1
  int n = bNodep[s2](i2);

  printf("add j2=%i, node i2=%i (n=%i) of s2=%i to element e2n=e1m=%i of s1=%i\n",j2,i2,n,s2,e1m,s1);
              
              // split element
  int e = connectionInfo2(j2,3);  // global element number
  int localEdge=connectionInfo2(j2,2); 
              
  int eNew=numberOfElements;
  element(eNew,0)=element(e,0);
  element(eNew,1)=element(e,1);
  element(eNew,2)=element(e,2);

  int is=0;
  if( s1<s2 )
  {
    // ** new element goes on top:
    //  2-------------1
    //    \       new |
    //       \        |
    //         \      N  le=0
    //           \ old|
    //             \  |
    //                0
    //             
    element(eNew,localEdge)=n;     // change this node.
    element(e,(localEdge+1)%3)=n;  //  change the existing element
  }
  else
  {
    // new element goes on top:
    //       2----------1
    //       | new     /
    //  le=2 N       /
    //       |old  /
    //       |   /
    //       | /
    //       0
    is=1;
    element(eNew,(localEdge+1)%3)=n;     // change this node.
    element(e,localEdge)=n;        //  change the existing element
  }
  e1m=eNew;  // ******
  
  numberOfElements++;
  assert( numberOfElements<maxNumberOfElements );

  int newFace1=numberOfFaces;
  numberOfFaces++;
  assert( numberOfFaces<maxNumberOfFaces );
  face(newFace1,0)=element(e,(localEdge+2)%3);  // diagonal face
  face(newFace1,1)=n;

  faceElements(newFace1,0)=e;
  faceElements(newFace1,1)=eNew;

  printf("add face %i: nodes=(%i,%i) elements (%i,%i) \n",newFace1,face(newFace1,0),face(newFace1,1),
	 faceElements(newFace1,0),faceElements(newFace1,1));


  // change the existing face still on old element
  int fc=connectionInfo2(j2,4)+faceOffset(s1);
  if( face(fc,1)==element(eNew,(localEdge+1-is)%3) )
    face(fc,1)=n;
  else
    face(fc,0)=n;
  faceElements(fc,0)=e;
  faceElements(fc,1)=e2m < 0 ? -1 : e2m+elementOffset(s2);

  printf("Change face %i nodes=(%i,%i) elements (%i,%i) \n",fc,face(fc,0),face(fc,1),
	 faceElements(fc,0),faceElements(fc,1));

  // another face to change (was on top of old, now on top of new):
  int e1 = connectionInfo2(j2,1);  // local element number
  fc = elementFace1(e1,(localEdge+1-2*is)%3) + faceOffset(s1);
  if( faceElements(fc,0)==e )
    faceElements(fc,0)=eNew;
  else if( faceElements(fc,1)==e )
    faceElements(fc,1)=eNew;
  else
  {
    printf("addNodeToInterface::ERROR: face fc=%i, faceElements=(%i,%i) but e=%i\n",fc,faceElements(fc,0),
	   faceElements(fc,1),e);
  }
  printf("Change face %i nodes=(%i,%i) elements (%i,%i) \n",fc,face(fc,0),face(fc,1),
	 faceElements(fc,0),faceElements(fc,1));

  int newFace2=numberOfFaces;   // face adjacent to s2
  numberOfFaces++;
  assert( numberOfFaces<maxNumberOfFaces );
  face(newFace2,0)=n;
  face(newFace2,1)=element(eNew,(localEdge+1-is)%3);

  faceElements(newFace2,0)=eNew;
  faceElements(newFace2,1)=e2p<0 ? -1 : e2p+elementOffset(s2);

  printf("add face %i: nodes=(%i,%i) elements (%i,%i) \n",newFace2,face(newFace2,0),face(newFace2,1),
	 faceElements(newFace2,0),faceElements(newFace2,1));
  
  return 0;
}


/* ----
int
findClosestSurface( int i, 
                    int s,
                    int initialGuessForSurface,
                    intArray & bNode, 
                    CompositeSurface & cs,
                    IntegerArray & subSurfacesToCheck,
                    UnstructuredMapping  **boundaryp,
                    realArray & boundingBox, 
                    MappingProjectionParameters & mpParams )
{

}
----- */

bool UnstructuredMapping::
isDuplicateNode(int i, int n, int e, int s, int s2, real & r0, real & r1,
                realArray & x,
                real epsDup,
                intArray & bNode,
                intArray & nodeInfo,
                int & localEdge,
                real & dist0, real & dist1, real & dist2, int debugFlag )
// ==================================================================================================
// ==================================================================================================
{
  bool duplicateNodeFound=false;
  
  // >> this next stuff is not used in this function <<<
  dist0= fabs(r1), dist1=fabs(r0+r1-1.), dist2=fabs(r0);
  real minDist=min(dist0,dist1,dist2);
  localEdge =  dist0==minDist ? 0 : dist1==minDist ? 1 : 2;
	  
  const int n0=element(e,0), n1=element(e,1), n2=element(e,2);
  real xDist0 = SQR(x(0,0)-node(n0,0))+SQR(x(0,1)-node(n0,1))+SQR(x(0,2)-node(n0,2));
  real xDist1 = SQR(x(0,0)-node(n1,0))+SQR(x(0,1)-node(n1,1))+SQR(x(0,2)-node(n1,2));
  real xDist2 = SQR(x(0,0)-node(n2,0))+SQR(x(0,1)-node(n2,1))+SQR(x(0,2)-node(n2,2));
	  
  if( debugFlag & 2 )
  {
       printf(" isDuplicateNode? : distances to nodes  = %i:%8.2e, %i:%8.2e, %i:%8.2e,  epsDup=%8.2e\n",
	      n0,xDist0,n1,xDist1,n2,xDist2,epsDup);
//        printf("node n0=%i : (%e,%e,%e)\n",n0,node(n0,0),node(n0,1),node(n0,2));
//        printf("node n1=%i : (%e,%e,%e)\n",n1,node(n1,0),node(n1,1),node(n1,2));
//        printf("node n2=%i : (%e,%e,%e)\n",n2,node(n2,0),node(n2,1),node(n2,2));
  }
  
  if( min(xDist0,xDist1,xDist2)< epsDup ) // epsDuplicate )
  {
	    
    const int duplicateNode= xDist0<=min(xDist1,xDist2) ? n0 : xDist1<=min(xDist0,xDist2) ? n1 : n2;
	    
    // next line could be wrong! *********************  why?
    real xDist=(SQR(node(n,0)-node(duplicateNode,0))+
		SQR(node(n,1)-node(duplicateNode,1))+
		SQR(node(n,2)-node(duplicateNode,2)));

    if( debugFlag & 2 )
    {
      printf("  s=%i, s2=%i, i=%i, n=%i, New boundary node matches an existing node, xDist=%e\n"
	     "replace node %i by node %i of element e=%i\n",s,s2,i,n,xDist,n,duplicateNode,e);
//       if( xDist > epsx )
// 	printf("****WARNING xDist=%e > epsx = %e \n",xDist,epsx);
    }
	      

    // merge nodes n and duplicateNode
    // we could average the node positions ?
    // *** Here we make a list of duplicate nodes:
    //    nodeInfo(n0,4) -> n1, nodeInfo(n1,4) -> n2, nodeInfo(n2,4)->n3 ...
    int n0=nodeInfo(n,4);
    int n1=nodeInfo(duplicateNode,4);
    if( n0<0 && n1<0 )
    {
      if( n<duplicateNode )
      { 
        nodeInfo(n,4)=duplicateNode; 
        nodeInfo(n,5)=n;               // keep track of the start of the chain
        nodeInfo(duplicateNode,5)=n;   // keep track of the start of the chain
      }
      else
      {
	nodeInfo(duplicateNode,4)=n;
        nodeInfo(n,5)=duplicateNode;    // keep track of the start of the chain
        nodeInfo(duplicateNode,5)=duplicateNode;   // keep track of the start of the chain
      }
      
    }
    else if( n0==duplicateNode || n1==n )
    {
      bNode(i)=n;   // keep looking -- these nodes are already marked as duplicates.
    }
    else
    {
      // check if duplicateNode is in the chain of duplicates of node n, or vice versa
      int na; // = n0>=0 ? n : duplicateNode;
      int nb; // = n0>=0 ? duplicateNode : n;
      if( n0>=0 && n1>=0 )
      {
	na=min(n,duplicateNode);
	nb=max(n,duplicateNode);
      }
      else if( n0>=0 )
      {
	na=n, nb=duplicateNode;
      }
      else
      {
	na=duplicateNode, nb=n;
      }
	      
      int nc= nodeInfo(na,4);
      int endChain=nc;
	      
      bool nodeFound=false;
      for( int c=0; nc>=0 && c<10; c++ )  // assume at most 10 coincident points.
      {
	if( nodeInfo(nc,4)==nb )
	{
	  nodeFound=true;
	  break;
	}
	endChain=nc;
	nc=nodeInfo(nc,4);
      }
      if( nodeFound )
      {
	bNode(i)=n;   // keep looking -- these nodes are already marked as duplicates.
	nodeInfo(n,5)=n;
      }
      else
      {
	assert( endChain>=0 );
	nodeInfo(endChain,4)=nb; // nodeInfo(nb,4)=na;
        nodeInfo(n,5)=nodeInfo(endChain,5);   // start of chain.

      }
	      
    }
    duplicateNodeFound=true;
  }
  return duplicateNodeFound;
	  
}

bool
closerSurfaceFound(const real & dist,
		   int s, int s2,
		   realArray & x0,
		   realArray & x2,
		   int numberOfSurfacesToCheck,
		   IntegerArray & surfacesToCheck,
		   RealArray & boundingBox,
		   UnstructuredMapping **boundaryp,
		   MappingProjectionParameters & mpParams2,
		   int & sNew,
                   real & distNew,
                   int & localElementNumber, 
                   real & r0, real & r1 )
// =========================================================================================================
// /Description:
//    Look for a subsurface that is closer than the current guess, (s2,dist).
// /x0 (input) : point to project
// /x2 (output) : new closest point if found, otherwise x2 is left unchanged.
// =========================================================================================================
{
  bool closerFound=false;
  

  intArray & subSurfaceIndex = mpParams2.getIntArray(MappingProjectionParameters::subSurfaceIndex);
  realArray & rProject  = mpParams2.getRealArray(MappingProjectionParameters::r);


  sNew=s2;
  distNew=dist;
  realArray x(1,3);

  for( int s0=0; s0<numberOfSurfacesToCheck; s0++ )
  {
    int sc = surfacesToCheck(s0);
    // printf("closerSurfaceFound: s=%i s2=%i check sc=%i\n",s,s2,sc );
    
    if( sc>=0 && sc!=s2 )
    {
      if( x0(0,0)>=boundingBox(0,0,sc) && x0(0,0)<=boundingBox(1,0,sc) &&
          x0(0,1)>=boundingBox(0,1,sc) && x0(0,1)<=boundingBox(1,1,sc) &&
          x0(0,2)>=boundingBox(0,2,sc) && x0(0,2)<=boundingBox(1,2,sc) )
      {
        // printf("closerSurfaceFound: s=%i s2=%i check sc=%i\n",s,s2,sc );

        UnstructuredMapping & map = *boundaryp[sc];
        subSurfaceIndex=-1;   // force a global search

        x=x0;
        map.project( x , mpParams2 );
        if( subSurfaceIndex(0)>=0 )
	{
	  real distc=SQR(x(0,0)-x0(0,0))+SQR(x(0,1)-x0(0,1))+SQR(x(0,2)-x0(0,2));
	  // printf("closerSurfaceFound: surface : sc=%i distc=%e \n",sc,distc);

	  if( distc<distNew )
	  {
	    // printf("closerSurfaceFound: surface was closer: sc=%i distc=%e \n",sc,distc);
	    x2=x;
	    distNew=distc;
	    sNew=sc;
	    closerFound=true;
	    localElementNumber=subSurfaceIndex(0);
	    r0=rProject(0,0);
	    r1=rProject(0,1);
	  }
	}
	
      }
    }
  }

  return closerFound;
  
}

//  ---- old version ----
bool UnstructuredMapping::
validStitch( int n, realArray & x0, realArray & x, intArray & nodeInfo, real tol, int debug ) 
{

  // project the point x onto the plane of the boundary triangle on surface s, x -> xp 
  //                         xp
  //                       /  \
  //                     /     \
  //                 ---x1-------x0------x3-------
  //                     \       |
  //                       \     |
  //                         \   |  surface s
  //                           \ |
  //                            x2 


  // the signed area of the triangle [x0,xp,x1] should be in the same direction as triangle [x0,x1,x2]

  bool stitchOk=false;
  for( int faceToCheck=0; faceToCheck<=1; faceToCheck++ )
  {
    //check the two faces connected to this node.
    int f0=nodeInfo(n,faceToCheck);

    assert( f0>=0 );
    int e0=faceElements(f0,0);
    assert( e0>=0 );

    int m0=face(f0,0), m1=face(f0,1);
    int n0=element(e0,0), n1=element(e0,1), n2=element(e0,2);
	    
    real nv1[3], nv2[3], xp[3];
    getNormal(e0,nv1 );
	    

    real xa[3],xb[3];
    int na,nb;
    if( m0==n0 )
    {
      if( m1==n1 )
      {
	na=n0, nb=n1;
      }
      else
      {
	na=n2, nb=n0;
      }
    }
    else if( m0==n1 )
    {
      if( m1==n2 )
      {
	na=n1, nb=n2;
      }
      else
      {
	na=n0, nb=n1;
      }
    }
    else if( m0==n2 )
    {
      if( m1==n0 )
      {
	na=n2, nb=n0;
      }
      else
      {
	na=n1, nb=n2;
      }
    }
    else
    {
      throw "error";
    }
	    
    xa[0]=node(na,0), xa[1]=node(na,1), xa[2]=node(na,2);
    xb[0]=node(nb,0), xb[1]=node(nb,1), xb[2]=node(nb,2);
	    
    // note xc == x0
    real *xc = n==na ? xa : xb;

    xp[0] = x(0,0)-xc[0], xp[1]=x(0,1)-xc[1], xp[2]=x(0,2)-xc[2];
    
    real xdotn = xp[0]*nv1[0]+xp[1]*nv1[1]+xp[2]*nv1[2];
    xp[0] = xp[0]-xdotn*nv1[0];
    xp[1] = xp[1]-xdotn*nv1[1];
    xp[2] = xp[2]-xdotn*nv1[2];

    
    xp[0]+=xc[0];
    xp[1]+=xc[1];
    xp[2]+=xc[2];
    

    getSignedArea( xa,xp,xb, nv2 );
	    
    real dot = nv1[0]*nv2[0]+nv1[1]*nv2[1]+nv1[2]*nv2[2];

    real dot2= dot/max(REAL_MIN*10.,SQRT( nv2[0]*nv2[0]+nv2[1]*nv2[1]+nv2[2]*nv2[2]));
    
    real dist1=SQRT( SQR(xp[0]-x0(0,0))+SQR(xp[1]-x0(0,1))+SQR(xp[2]-x0(0,2)) );
    real dist2=SQRT( SQR(xb[0]-xa[0])+SQR(xb[1]-xa[1])+SQR(xb[2]-xa[2]) );

    real expectedArea=dist1*dist2;  // note: twice the area of the triangle with base (xa,xp) and height dist1

    real xpNorm= SQR(xp[0]-xc[0])+SQR(xp[1]-xc[1])+SQR(xp[2]-xc[2]);

    if( debug & 2 )
    {
      // **** The test here can fail if the projected point x is a long way off in the normal direction --
      // then the vector (xp-xc) may not be nearly normal to (xa,xb) -- could check this

      printf(" n=%i e0=%i:(%i,%i,%i) f0=%i:(%i,%i) na=%i, nb=%i xpNorm=%e <? tol=%8.2e\n",
            n,e0,n0,n1,n2,f0,m0,m1,na,nb,xpNorm,tol);
      printf(" dist1=%e, dist2=%e, x0=(%e,%e,%e) xc=(%e,%e,%e) \n",dist1,dist2,
                     x0(0,0),x0(0,1),x0(0,2),xc[0],xc[1],xc[2]);
      printf(" ***** nv1=(%8.2e,%8.2e,%8.2e) nv2=(%8.2e,%8.2e,%8.2e) dot=%8.2e expectedArea=%8.2e, dot2=%8.2e\n",
	     nv1[0],nv1[1],nv1[2],nv2[0],nv2[1],nv2[2],dot,expectedArea,dot2);
    }
    // if( dot2 > .25 ||  dot > .2*expectedArea || xpNorm<tol  )
    if( dot > .2*expectedArea || xpNorm<tol  )
    {
      stitchOk=true;
      break;
    }
	      
  }  // for( faceToCheck
  if( !stitchOk )
  {
    if( debug & 2 )
      printf("  **** Will not stitch point since the projected point is in the wrong direction\n");
    
  }
  return stitchOk;
}

static int *elementArray; // used by boundaryFaceCompare for sorting boundary faces.

static int elementCompare(const void *i0, const void *j0)
{
  int i = *(int*)i0;
  int j = *(int*)j0;
  
  if( elementArray[i] > elementArray[j] )
    return (1);
  if( elementArray[i] < elementArray[j] )
    return (-1);
  return (0);
  
}



int UnstructuredMapping::
buildFromACompositeSurface( CompositeSurface & cs )
// ================================================================================================
/// \details 
///    Build an Unstructured Mapping from a CompositeSurface
/// 
///    Triangulate each sub-surface and then stitch the sub-surfaces together.
/// 
// ============================================================================================
{
  real time0=getCPU();
  
  if( cs.getClassName()!="CompositeSurface" )
  {
    printf(" UnstructuredMapping::buildFromACompositeSurface:ERROR: this is not a CompositeSurface\n");
    return 1;
  }
  
  rangeDimension=cs.getRangeDimension();
  domainDimension=cs.getDomainDimension();

  assert( domainDimension==2 && rangeDimension==3 );

  intArray *bNodep = new intArray [cs.numberOfSubSurfaces()];   // hold boundary nodes.
  intArray numberOfBoundaryNodes(cs.numberOfSubSurfaces());
  intArray elementOffset(cs.numberOfSubSurfaces());
  elementOffset=0;
  intArray nodeOffset(cs.numberOfSubSurfaces());
  nodeOffset=0;
  intArray faceOffset(cs.numberOfSubSurfaces());
  faceOffset=0;
  // ** count the number of nodes and elements **

  numberOfNodes=0;
  numberOfElements=0;
  numberOfFaces=0;
  
  int s;

  // build an unstructured mapping for each subsurface

  UnstructuredMapping **boundaryp = new UnstructuredMapping* [cs.numberOfSubSurfaces()];

  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    printf("Build an unstructured mapping for sub-surface %i \n",s);

    Mapping & map = cs[s];
    if( map.getClassName()=="TrimmedMapping" )
    {
      printf("Subsurface %i is a TrimmedMapping\n",s);
    }

    if( cs[s].getClassName()=="TrimmedMapping" )
    {
      TrimmedMapping & trim = (TrimmedMapping&)cs[s];
      boundaryp[s] = &trim.getTriangulation();
      boundaryp[s]->incrementReferenceCount();
    }
    else
    {
      boundaryp[s] = new UnstructuredMapping; boundaryp[s]->incrementReferenceCount();
      boundaryp[s]->setPreferTriangles();
      boundaryp[s]->setElementDensityTolerance(elementDensityTolerance);
      boundaryp[s]->buildFromARegularMapping(cs[s]);
      // boundaryp[s]->printConnectivity();
    }
    
    printf(" *** surface s=%i : nodes %i to %i, elements %i to %i\n",s,
           numberOfNodes,numberOfNodes+boundaryp[s]->getNumberOfNodes(),
           numberOfElements,numberOfElements+boundaryp[s]->getNumberOfElements());

    elementOffset(s)=numberOfElements;
    nodeOffset(s)=numberOfNodes;
    faceOffset(s)=numberOfFaces;

    numberOfNodes+=boundaryp[s]->getNumberOfNodes();
    numberOfElements+=boundaryp[s]->getNumberOfElements();
    numberOfFaces+=boundaryp[s]->getNumberOfFaces();

    numberOfBoundaryNodes(s)=boundaryp[s]->getNumberOfBoundaryFaces();  // no. of bndry nodes == no. of bndry faces 

    // tags will hold the global element index
    boundaryp[s]->tags.seqAdd(0,1);
    boundaryp[s]->tags+=elementOffset(s);

    if( debugs & 4 ){ ::display(boundaryp[s]->tags,"boundaryp[s]->tags (global element number)"); }
    

  }
  timing[timeForBuildingSubSurfaces]+=getCPU()-time0;

  Range Rx=rangeDimension, R3=3, R2=2;

  maxNumberOfNodesPerElement=3; // only triangles.

  int maxNumberOfElements=numberOfElements+sum(numberOfBoundaryNodes);  // allow extra for stitching

  Range R=numberOfNodes;
  node.redim(R,Rx);
  element.redim( maxNumberOfElements,maxNumberOfNodesPerElement);


  // final number of faces will probably be less than:
  const int maxNumberOfFaces=numberOfFaces+sum(2*numberOfBoundaryNodes);
  face.redim(maxNumberOfFaces,2);    // 2 nodes per face
  faceElements.redim(maxNumberOfFaces,2);  // 2 elements per face

  if( elementFaces==NULL )
    elementFaces = new intArray;

  intArray & ef = *elementFaces;
  ef.redim(maxNumberOfElements,3);
  ef=-1;

//  intArray elementSurface(maxNumberOfElements);  // an element sits on this subsurface
//  elementSurface=-1;
  // tags will hold the sub-surface that an element sits on
  tags.redim(maxNumberOfElements);
  tags = -1;

  intArray & elementSurface = tags;

  int n0=0, e0=0, f0=0;
  numberOfBoundaryFaces=0;
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    const int nodeOffset = n0;
    const int elementOffset=e0;
    
    // copy nodes
    R=boundaryp[s]->getNumberOfNodes();
    node(R+n0,Rx)= boundaryp[s]->getNodes()(R,Rx);
    n0+=boundaryp[s]->getNumberOfNodes();
    
    // copy elements and elementFaces
    R=boundaryp[s]->getNumberOfElements();
    element(R+e0,R3)=boundaryp[s]->getElements()(R,R3)+nodeOffset;
    ef(R+e0,R3)=boundaryp[s]->getElementFaces()(R,R3)+faceOffset(s);
    elementSurface(R+e0)=s;
    e0+=boundaryp[s]->getNumberOfElements();

    // copy faces and faceElements
    R=boundaryp[s]->getNumberOfFaces();
    face(R+f0,R2)=boundaryp[s]->face(R,R2)+nodeOffset;
//  faceElements(R+f0,R2)=boundaryp[s]->faceElements(R,R2)+elementOffset;
// *** this should work ***
    faceElements(R+f0,R2)=boundaryp[s]->faceElements(R,R2);
    where( faceElements(R+f0,R2)>=0 )
      faceElements(R+f0,R2)+=elementOffset;

    f0+=boundaryp[s]->getNumberOfFaces();

    // get boundary nodes from boundary faces.
    // Here we assume we can take node 0 from each boundary face and get all boundary nodes.
    intArray & bNode = bNodep[s];
    bNode.redim(numberOfBoundaryNodes(s));
    bNode=-1;

    int nbf=boundaryp[s]->getNumberOfBoundaryFaces();
    const intArray & bf = boundaryp[s]->getBoundaryFace();
    const intArray & fc = boundaryp[s]->getFaces();
    Range F=nbf;
    bNode(F)=fc(bf(F),0)+nodeOffset;
        
    numberOfBoundaryFaces+=nbf;
    
//     for( int f=0; f<nbf; f++ )
//     {
//       bNode(f)=fc(bf(f),0)+nodeOffset;
//     }

    // ::display(node,"node");
    // ::display(bNode,"bNode");
    
  }
  
  if( false ) printConnectivity();
  if( false )
  {
    for( int n=0; n<numberOfNodes; n++ )
    {
      printf(" node %i (%12.5e,%12.5e,%12.5e)\n",n,node(n,0),node(n,1),node(n,2));
    }
  }

  int side,axis;
  
  realArray xBound(2,3);
  real xScale=0;
  
  R=numberOfNodes;
  for( axis=0; axis<rangeDimension; axis++ )
  {
    xBound(Start,axis)=min(node(R,axis));
    xBound(End,  axis)=max(node(R,axis));
    setRangeBound(Start,axis,xBound(Start,axis));
    setRangeBound(End  ,axis,xBound(End,  axis));
    
    xScale=max(xScale,xBound(End,axis)-xBound(Start,axis));
  }


  // For each boundary node compute a local distance between nodes that can be used as a 
  // measure of how well the boundary is resolved -- use this to determine when one node
  // lies on another surface and when two nodes are duplicates

  // boundaryNodeSeparation(n) = ...
  realArray * pBoundaryNodeSeparation = new realArray [cs.numberOfSubSurfaces()];
  realArray subSurfaceScale(cs.numberOfSubSurfaces());
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    Mapping & map = cs[s];
    real scale=0.;
    for( axis=0; axis<rangeDimension; axis++ )
    {
      if( !map.getRangeBound(Start,axis).isFinite() || !map.getRangeBound(End,axis).isFinite() )
      {
	printf("*** WARNING: rangeBound for map s=%i is not finite! axis=%i [%e,%e]\n",s,axis,
             (real)map.getRangeBound(Start,axis),(real)map.getRangeBound(End,axis) );
        map.getGrid();
      }
      real xa=map.getRangeBound(Start,axis);
      real xb=map.getRangeBound(End,axis);
      scale=max(scale,xb-xa);
      // printf(" *** xa=%e xb=%e scale=%e\n",xa,xb,scale);
      
    }
    subSurfaceScale(s)=scale;

  }
  

  // ********************************************
  // *** now stitch the sub-surfaces together ***
  // ********************************************

  real timeStitch=getCPU();

  MappingProjectionParameters mpParams;
  intArray & subSurfaceIndex = mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);
  realArray & rProject  = mpParams.getRealArray(MappingProjectionParameters::r);
  
  MappingProjectionParameters mpParams2;

  int numberOfStitchedElements=0;

  // *** nodeInfo allocates space for info about ALL nodes but only boundary nodes are really used ****
  intArray nodeInfo(numberOfNodes,6);
  nodeInfo=-1;

  // for each boundary node, remember 4 boundary faces, global numbering; 
  // nodeInfo(n,0) = first face attached to this node
  // nodeInfo(n,1) = second face attached to this node
  // nodeInfo(n,2:3) : other faces attached to this node (assigned when a node is stitched to another surface)
  // nodeInfo(n,4) = replacement node for duplicate nodes.
  // nodeInfo(n,5) : points to the starting node in a chain of duplicates.
  realArray x(1,3), x0(1,3), x1(1,3), x2(1,3), xSave(1,3), x20(1,3);

  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    UnstructuredMapping & map = *boundaryp[s];
    const int num=map.getNumberOfBoundaryFaces();

    int i;
    for( i=0; i<num; i++ )
    {
      int f = map.bdyFace(i);
      int n0=map.face(f,0)+nodeOffset(s);
      int n1=map.face(f,1)+nodeOffset(s);
      f+=faceOffset(s);
      if( nodeInfo(n0,0)==-1 )
	nodeInfo(n0,0)=f;
      else 
	nodeInfo(n0,1)=f;
      if( nodeInfo(n1,0)==-1 )
	nodeInfo(n1,0)=f;
      else 
	nodeInfo(n1,1)=f;

    }
    // compute the boundary node separation
    intArray & bNode = bNodep[s];
    int numBoundaryNodes=bNode.getBound(0)-bNode.getBase(0)+1;
    realArray & boundaryNodeSeparation = pBoundaryNodeSeparation[s];
    boundaryNodeSeparation.redim(numBoundaryNodes);
    for( i=0; i<numBoundaryNodes; i++ )
    {
      // find the two faces attached to this boundary node and sum the (square) of the lengths of these faces.
      int n=bNode(i);
      int f0=nodeInfo(n,0), f1=nodeInfo(n,1);
      assert( f0>=0 && f1>=0 );
      int n0=face(f0,0), n1=face(f0,1), n2=face(f1,0), n3=face(f1,1);
      
      real dist = (SQR(node(n1,0)-node(n0,0))+SQR(node(n1,1)-node(n0,1))+SQR(node(n1,2)-node(n0,2))+
		   SQR(node(n3,0)-node(n2,0))+SQR(node(n3,1)-node(n2,1))+SQR(node(n3,2)-node(n2,2)));
      boundaryNodeSeparation(i)=dist;
      
    }
    // :: display(boundaryNodeSeparation,"boundaryNodeSeparation");

  }

  RealArray boundingBox(2,3,cs.numberOfSubSurfaces());
  const real extensionTolerance=.05;
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    const real boundingBoxExtensionFactor=extensionTolerance*subSurfaceScale(s);
    // printf("boundingBoxExtensionFactor=%e\n",boundingBoxExtensionFactor);
    
    for( int axis=0; axis<rangeDimension; axis++ )
    {
      boundingBox(Start,axis,s)=(real)boundaryp[s]->getRangeBound(Start,axis)-boundingBoxExtensionFactor;
      boundingBox(End  ,axis,s)=(real)boundaryp[s]->getRangeBound(End  ,axis)+boundingBoxExtensionFactor;
    }
  }
  // ::display(boundingBox,"boundingBox");
  
  

  IntegerArray surfacesToCheck(cs.numberOfSubSurfaces());
  real xPrevious[3];
  

  realArray *pDistMin = new realArray [cs.numberOfSubSurfaces()];   // **** delete these *************
  realArray *pCoordinates = new realArray [cs.numberOfSubSurfaces()];
  intArray *pbNodeInfo = new intArray [cs.numberOfSubSurfaces()];  
  int *numberConnected = new int [cs.numberOfSubSurfaces()];

  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    pDistMin[s].redim(numberOfBoundaryNodes(s));    pDistMin[s]=REAL_MAX;
    pCoordinates[s].redim(numberOfBoundaryNodes(s),2);
    pbNodeInfo[s].redim(numberOfBoundaryNodes(s),4);
    pbNodeInfo[s]=-1;
    numberConnected[s]=0;
  }
  
  const real absoluteStitchingToleranceSquared = absoluteStitchingTolerance < SQRT(REAL_MAX) ? 
    SQR(absoluteStitchingTolerance) : REAL_MAX;

  real xa[3],xb[3],d[3],xp[3];
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    // **** Find the closest sub-surface to each boundary node on surface s *****

    UnstructuredMapping & map = *boundaryp[s];

    intArray & bNode = bNodep[s];
    const realArray & boundaryNodeSeparation = pBoundaryNodeSeparation[s];

    realArray & distMin= pDistMin[s];
    realArray & coordinates = pCoordinates[s];
    intArray & bNodeInfo = pbNodeInfo[s];


    // -- make a list of which other surfaces intersect surface s
    int s2;
    int numberOfSurfacesToCheck=0;
    for( s2=0; s2<cs.numberOfSubSurfaces(); s2++ )
    {
      if( s!=s2 )
      {
	real localScale=max(subSurfaceScale(s),subSurfaceScale(s2));
	const real surfaceExtensionTolerance=localScale*extensionTolerance;
	if( boundaryp[s]->intersects(*boundaryp[s2],-1,-1,-1,-1,surfaceExtensionTolerance) )
	{
	  surfacesToCheck(numberOfSurfacesToCheck)=s2;
	  numberOfSurfacesToCheck++;
	}
      }
    }
	    

    for( int sc=0; sc<numberOfSurfacesToCheck; sc++ )
    {
      s2=surfacesToCheck(sc);
	
      UnstructuredMapping & map2 = *boundaryp[s2];

      real localScale=max(subSurfaceScale(s),subSurfaceScale(s2));
      const real adjacencyTolerance=SQR(subSurfaceScale(s)*.2);  

      // const real tol = stitchingTolerance>0. ? stitchingTolerance : max(1.e-3,elementDensityTolerance*.1);
      const real tol = stitchingTolerance>0. ? stitchingTolerance : 1.e-2;

      // ** const real epsDuplicate = SQR(1.e-4*localScale);
      const real epsDuplicate = SQR(1.e-3*localScale);
      const real epsx = SQR(tol*localScale);  // compare to square of distance

      // get the bounding box for map2 
      real xMin2[3], xMax2[3];
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xMin2[axis]=boundingBox(Start,axis,s2);
	xMax2[axis]=boundingBox(End  ,axis,s2);
      }
  
      real estimatedBoundingBoxSize=min(localScale*.02,absoluteStitchingTolerance);
      mpParams.setSearchBoundingBoxSize( estimatedBoundingBoxSize, absoluteStitchingTolerance );
      mpParams2.setSearchBoundingBoxSize( estimatedBoundingBoxSize, absoluteStitchingTolerance );

      int startingGuess=-1;  // reset the starting guess.
      
      // ******************  loop over boundary nodes of surface s *********************
      int i;
      for( i=0; i<numberOfBoundaryNodes(s); i++ )
      {
	// check if this boundary node hits a boundary of surface s2.
	const int n=bNode(i);   // global node numbering
	
	// check if we are inside the bounding box ****
	if( node(n,0)<xMin2[0] || node(n,0)>xMax2[0] ||
	    node(n,1)<xMin2[1] || node(n,1)>xMax2[1] ||
	    node(n,2)<xMin2[2] || node(n,2)>xMax2[2] )
	{
          startingGuess=-1;   // reset starting guess
	  continue;  // point is outside the bounding box of map2.
	}

	assert( n>=0 && n<numberOfNodes );
	x0(0,0)=node(n,0); x0(0,1)=node(n,1); x0(0,2)=node(n,2);

        // **** check if this node is close to the previous node, if not, reset subSurfaceIndex to use
        // a global search
        if( i>0 && startingGuess!=-1 )
        {
	  real distToPrevious = SQR(x0(0,0)-xPrevious[0])+SQR(x0(0,1)-xPrevious[1])+SQR(x0(0,2)-xPrevious[2]);
	  if(distToPrevious> adjacencyTolerance )
	  {
	    startingGuess=-1;   // reset starting guess
            // printf("*** s=%i, s2=%i i=%i: conseq. bndry nodes not close, use a global search, dist=%e, tol=%e\n",
            //       s,s2,i,distToPrevious,adjacencyTolerance);
	  }
	}
	xPrevious[0]=x0(0,0), xPrevious[1]=x0(0,1), xPrevious[2]=x0(0,2);

        
	x=x0;
	subSurfaceIndex=startingGuess;  // *****
	// subSurfaceIndex=-1;   // force a global search for debugging.
	map2.project( x , mpParams );
        startingGuess=subSurfaceIndex(0);
	
        if( startingGuess==-1 )
	{
	  // printf(" ***** skip point s=%i i=%i n=%i s2=%i. No nearest point.\n",s,i,n,s2);
          continue;   // no nearest point found within maximum size bounding box.
	}
	real dist = SQR(x(0,0)-x0(0,0))+SQR(x(0,1)-x0(0,1))+SQR(x(0,2)-x0(0,2));
	

        const real epsSep=max(epsx*.01,boundaryNodeSeparation(i));
        const real epsStitch= min(epsx,epsSep*SQR(.1)); // =epsx
//        const real epsStitch=min(epsx,epsSep*SQR(.25)); // =epsx
        real epsDup =min(epsDuplicate,epsSep*SQR(.01));
        const real validStitchTolerance=max(epsDup*10.,epsx*SQR(.01));

//  	 const real epsx = SQR(tol*localScale);
// 	 const real epsSep=max(epsx*.01,boundaryNodeSeparation(i));
// 	 const real epsStitch= min(epsx,epsSep*SQR(.1));

        assert( epsStitch>0. && epsDup>0. );

        // if we are inside the element then this could cause troubles.
        bool inside = rProject(0,0)>1.e-4 && rProject(0,1)>1.e-4 && (1.-rProject(0,0)-rProject(0,1)) > 1.e-4;

	
        if( debugs & 2 )
	{
          int ep = subSurfaceIndex(0)>=0 ? map2.tags(subSurfaceIndex(0)) : -1;
	  printf("--> %s s=%i i=%i n=%i s2=%i x=(%12.6e,%12.6e,%12.6e) xp=(%8.2e,%8.2e,%8.2e) el=%i ep=%i "
		 "dist=%8.2e epsS=%7.1e lScale=%7.1e epsx=%7.1e epsDup=%8.2e bNodeSep=%8.2e r=(%g,%g)\n",
                 (inside ? "*inside*" : ""),
 		 s,i,n,s2,x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2),subSurfaceIndex(0),ep,
                  dist,epsStitch,localScale,epsx,
		 epsDup,boundaryNodeSeparation(i),rProject(0,0),rProject(0,1) );
	}
	
        if( (dist < distMin(i) || dist< epsDup) && dist < absoluteStitchingToleranceSquared )
	{
	  // This new surface is closer, or it is a posible duplicate point. 
          // 
	  
	  int localElementNumber = subSurfaceIndex(0);  // we are inside this element of map2.
	
	  int e = map2.tags(localElementNumber);  // elementOffset(s2)+ localElementNumber;
	  assert( e>=0 && e<numberOfElements );

	  // (r,s) = local triangle coordinates
	  real r0=rProject(0,0), r1=rProject(0,1);

          // get the localEdge -- face of element e that is on the boundary

	  real dist0= fabs(r1), dist1=fabs(r0+r1-1.), dist2=fabs(r0);
	  real minDist=min(dist0,dist1,dist2);
	  int localEdge =  dist0==minDist ? 0 : dist1==minDist ? 1 : 2;

	  int fc=ef(e,localEdge);   // face on boundary that will remain with the old element e
          if( faceElements(fc,1) >=0  )
	  {
	    if( debugs & 2 ) 
	      printf("***** WARNING ***** i=%i : e=%i localEdge=%i is probably wrong. fc=%i fe=(%i,%i). \n",i,
		     e,localEdge,fc,faceElements(fc,0),faceElements(fc,1));
	    int newLocalEdge;
	    if( localEdge==0  )
	      newLocalEdge= dist1<dist2 ? 1 : 2;
	    else if( localEdge==1 )
	      newLocalEdge= dist0<dist2 ? 0 : 2;
	    else
	      newLocalEdge= dist0<dist1 ? 0 : 1;

	    fc=ef(e,newLocalEdge);
            if( faceElements(fc,1) <0 )
	    {
              localEdge=newLocalEdge;
              if( debugs & 2 ) printf(" ... ok localEdge=%i seems to work\n",localEdge);
	    }
	    else if( faceElements(fc,1) >=0  )
	    {

	      // **** -> should find the closest boundary face <- *****
	      // one of the boundary nodes should have a boundary face
	      // ** project onto the nearest boundary face and use that element ****
	      real boundaryDistMin=REAL_MAX;
	      int eNew=-1, newLocalEdge=-1;
	      for( int m=0; m<3; m++ )
	      {
		int n0=element(e,m);
		if( nodeInfo(n0,0)>=0 )
		{
		  // this is a boundary node
		  for( int j=0; j<=1; j++ )
		  {
		  
		    int f0=nodeInfo(n0,j);
		    int e0=faceElements(f0,0)>=0 ? faceElements(f0,0) : faceElements(f0,1);
		    assert( e0>=0 );
                    if( e0==e )
                      continue;  // skip this one, we have already checked it.
		    
		    // int na=faces(f0,0), nb=faces(f0,1);
		    // order nodes in a counter-clockwise order so we compute r correctly.
		    int newEdge=ef(e0,0)==f0 ? 0 : ef(e0,1)==f0 ? 1 : 2;
		    int na = element(e0,newEdge), nb=element(e0,(newEdge+1)%3);

		    xa[0]=node(na,0), xa[1]=node(na,1), xa[2]=node(na,2);
		    xb[0]=node(nb,0), xb[1]=node(nb,1), xb[2]=node(nb,2);
		    d[0]=xb[0]-xa[0], d[1]=xb[1]-xa[1], d[2]=xb[2]-xa[2];
		    real dNorm=max(REAL_MIN*10.,d[0]*d[0]+d[1]*d[1]+d[2]*d[2]);
             
		    // project point x0 onto line segment [xa,xb]
		    real r =( (x0(0,0)-xa[0])*d[0]+(x0(0,1)-xa[1])*d[1]+(x0(0,2)-xa[2])*d[2] )/dNorm;
		    r=max(0.,min(1.,r));
		  
		    xp[0]=xa[0]+r*d[0];
		    xp[1]=xa[1]+r*d[1];
		    xp[2]=xa[2]+r*d[2];
		  
		    // compute the distance between x0 and xp
		    real distp = SQR(x0(0,0)-xp[0])+SQR(x0(0,1)-xp[1])+SQR(x0(0,2)-xp[2]);

                    if( debugs & 2 ) 
                      printf(" -- n0=%i, check element e0=%i f0=%i newEdge=%i (na,nb)=(%i,%i) r=%e, distp=%e\n",
			   n0,e0,f0,newEdge,na,nb,r,distp);
		    
		    if( distp<boundaryDistMin )
		    {
		      boundaryDistMin=distp;
		      eNew = e0;
		      newLocalEdge = newEdge;
		      if( newLocalEdge==0 )
		      {
			r0=r, r1=0.;
		      }
		      else if( newLocalEdge==1 )
		      {
			r0=1.-r, r1=r;
		      }
		      else
		      {
			r0=0, r1=1.-r;
		      }
		      x(0,0)=xp[0];
		      x(0,1)=xp[1];
		      x(0,2)=xp[2];
		      
		    }
		  }
		}
	      }
	      if( eNew>=0 )
	      {
		if( debugs & 2) 
                   printf("***** WARNING ***** i=%i : closest element e=%i was not on the boundary,"
		       "   but a nearby boundary face was found\n"
		       " Project onto element eNew=%i, newLocalEdge=%i (r0,r1)=(%8.2e,%8.2e)\n",
		       i,e,eNew,newLocalEdge,r0,r1);

		e=eNew;
		localEdge=newLocalEdge;
		dist=boundaryDistMin;
                
		// **** epsDup=dist*2.; // force a duplicate node.
		
	      
	      }
	      else
	      {
                // *** old way to fix ****
                
		// **** the problem could be that the closest element does not have a boundary face
		//      but does have a node that is on the boundary --- this could be forced to be
		//      a duplicate node if we are reasonably close.
		//
		//                   \  e  /
		//                     \ / 
		//          ------------X---------
		const int n0=element(e,0), n1=element(e,1), n2=element(e,2);
		real xDist0 = SQR(x(0,0)-node(n0,0))+SQR(x(0,1)-node(n0,1))+SQR(x(0,2)-node(n0,2));
		real xDist1 = SQR(x(0,0)-node(n1,0))+SQR(x(0,1)-node(n1,1))+SQR(x(0,2)-node(n1,2));
		real xDist2 = SQR(x(0,0)-node(n2,0))+SQR(x(0,1)-node(n2,1))+SQR(x(0,2)-node(n2,2));
	  
		if( true || debugs & 2 )
		{
		  printf(" element is not on boundary ? --> distances to nodes  = %8.2e, %8.2e, %8.2e,  epsDup=%8.2e\n",
			 xDist0,xDist1,xDist2,epsDup);
		
		  const int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);    // faces on this element
		  // adjacent elements
		  int ae0 = faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
		  int ae1 = faceElements(f1,0)==e ? faceElements(f1,1) : faceElements(f1,0);
		  int ae2 = faceElements(f2,0)==e ? faceElements(f2,1) : faceElements(f2,0);

		  int n0=element(e,0), n1=element(e,1), n2=element(e,2);
		  printf("element %i: nodes=(%i,%i,%i), faces=(%i,%i,%i), adjacent elements=(%i,%i,%i)\n",
			 e,element(e,0),element(e,1),element(e,2),ef(e,0),ef(e,1),ef(e,2),ae0,ae1,ae2);
		  printf("node n=%i nodeInfo=(%i,%i,%i,%i), node n=%i nodeInfo=(%i,%i,%i,%i), "
			 "node n=%i nodeInfo=(%i,%i,%i,%i)\n",
			 n0,nodeInfo(n0,0),nodeInfo(n0,1),nodeInfo(n0,2),nodeInfo(n0,3),
			 n1,nodeInfo(n1,0),nodeInfo(n1,1),nodeInfo(n1,2),nodeInfo(n1,3),
			 n2,nodeInfo(n2,0),nodeInfo(n2,1),nodeInfo(n2,2),nodeInfo(n2,3));
		
		}
	      }
	    }
	  }
	  

          if( false && inside && dist < epsStitch )  // *** do this fixup later ***
	  {
            // Point is inside another surface
            //
            //                  |     \    |
            //                  |  X    \  |
            //                  |         \|
            //            ---------P------------------
            //   

            // project the point onto the boundary
            // *** we could be at a corner in which case there are two possible localEdge's

            real rr;
            if( localEdge==0 )
	    {
              r1=0.;   rr=r0;
	    }
	    else if( localEdge==1 )
	    {
              real rp=max(REAL_MIN*10.,r0+r1);
	      r0/=rp;
	      r1/=rp;
	      rr=r1;
	    }
	    else
	    {
              r0=0;  rr=1.-r1;
	    }
            int na=element(e,localEdge), nb=element(e,(localEdge+1)%3);
            x(0,0)=(1.-rr)*node(na,0)+rr*node(nb,0);
            x(0,1)=(1.-rr)*node(na,1)+rr*node(nb,1);
            x(0,2)=(1.-rr)*node(na,2)+rr*node(nb,2);
	    
            // move the node to the boundary
            node(n,0)=x(0,0), node(n,1)=x(0,1), node(n,2)=x(0,2);
            x0=x;
	    dist=0.;
            printf("Point is inside: New (r0,r1)=(%8.2e,%8.2e) localEdge=%i na=%i, nb=%i, rr=%e"
                   "pt projected to boundary is (%9.3e,%9.3e,%9.3e). dist=%e\n",
		   r0,r1,localEdge,na,nb,rr,x(0,0),x(0,1),x(0,2),dist);
            printf(" **** Node n=%i has been moved **** \n",n);
	    

	  }
	  

          
          // *** first check if this point is a valid stitch -- we need to do this here
	  // since the closest point may be invalid so we must throw it away now ****

          // if( !validStitch )
          //   continue;

          // if we are not looking for the closest surface (such as for a duplicate point when
          // we keep looking) then we must double check the next closest surface that we have
          // not skipped completely over a narrow surface
          
          // if( duplicate point && !validStitchDup ) ...
          //   continue

          if( true )
	  {
	    
// 	    s2=bNodeInfo(i,0);
//          real localScale=max(subSurfaceScale(s),subSurfaceScale(s2));

//  	 const real epsx = SQR(tol*localScale);
// 	 const real epsSep=max(epsx*.01,boundaryNodeSeparation(i));
// 	 const real epsStitch= min(epsx,epsSep*SQR(.1));

//         int n=bNode(i);
	    bool ok=true;

	    real doubleCheckEps=epsStitch*100. ;

	    if( dist > doubleCheckEps )  // *** we need a better measure than epsStitch
	    {
	      if( debugs & 2) printf(" ---- s=%i, i=%i, n=%i s2=%i dist=%8.2e > %8.2e. Stitch not valid.\n",s,i,n,
		     s2,dist,doubleCheckEps);
	      ok=false;
	    }
	    else if( dist > epsStitch*.01 ) // .01
	    {
	      // project back onto surface s (element e0.?)
	      //
	      int f0 = nodeInfo(n,0);
	      assert( f0>=0 );
	      int e0 = faceElements(f0,0); // the node n is near this element
	      assert( e0>=0 );
	      int surfaceGuess=subSurfaceIndex(0);
	      
	      subSurfaceIndex=e0;
	      // subSurfaceIndex=-1;  // ************ problem with triShip n=242
	  
	      if( debugs & 2 )
                 printf("--- s=%i i=%i n=%i s2=%i e=%i e0=%i distMin=%8.2e is suspicious. epsStitch=%8.3e  Double checking.\n",
		     s,i,n,s2,e,e0,dist,epsStitch);

	      x1=x;
	      map.project( x1, mpParams );

              int newLocalElement=subSurfaceIndex(0);
	      subSurfaceIndex=surfaceGuess; // reset
	     
	      

              //          ---------|
	      //  x = node n projected on s2
	      //  x1  = x projected back onto s
	      //  distMin = dist between node n and s2
	      //  dist1 = distance between surface s and s2 near x0
	      //  dist2 = distance between node n and x
	      //
	      //  
	      real dist1 = SQR(x1(0,0)-x(0,0))+ SQR(x1(0,1)-x(0,1))+ SQR(x1(0,2)-x(0,2));
	      real dist2 = SQR(x1(0,0)-node(n,0))+ SQR(x1(0,1)-node(n,1))+ SQR(x1(0,2)-node(n,2));

	      if( debugs & 2 ) printf("  x(s2)=(%8.2e,%8.2e,%8.2e) x1=(%8.2e,%8.2e,%8.2e)\n",x(0,0),x(0,1),x(0,2),
		     x1(0,0),x1(0,1),x1(0,2));
	  

	      if( dist1>dist )
	      {
		printf("** something is wrong dist1=%e > dist=%e -- re-try with a global search *** \n",dist1,dist);
                // **** maybe the local surface failed **** try again with a global surface
		subSurfaceIndex=-1;
		x1=x;
		map.project( x1, mpParams );
		dist1 = SQR(x1(0,0)-x(0,0))+ SQR(x1(0,1)-x(0,1))+ SQR(x1(0,2)-x(0,2));
		dist2 = SQR(x1(0,0)-node(n,0))+ SQR(x1(0,1)-node(n,1))+ SQR(x1(0,2)-node(n,2));

		if( dist1>dist )
		{
                  if( debugs & 2 ) printf(" ***** Something is still wrong! *******\n");
		}
		else
		{
                  if( debugs & 2 ) printf(" --> ok now \n");
		}
		
		
	      }
	  

	      if( dist2>min(dist,dist1) )
	      {
		ok=false;


		if( dist2 < epsStitch  )
		{
                  // if we are close, double check a point that has projected to the inside of an element on s
                  // sometimes there are errors in computing the projected point so that a point
                  // that is actually inside an element of s2 is not properly computed.
		  real r0New=rProject(0,0), r1New=rProject(0,1);
		  const real eps = REAL_EPSILON*10.;
		  bool inside =r0New>eps && r1New>eps && (1.-r0New-r1New) > eps;

		  if( inside )
		  {
         	    if( debug & 2)
                      printf("*** point projected back onto s=%i is inside (r0New,r1New)=(%e,%e)\n",s,r0New,r1New);


		    // printf("*** point projected back onto s=%i is (r0New,r1New)=(%e,%e)\n",s,r0New,r1New);

		    // here is the element we are in
		    int e2 = map.tags(newLocalElement);
		    if( e2==e || element(e2,0)==n || element(e2,1)==n || element(e2,2)==n )
		    {
		      if( debugs & 2 )
                        printf("*** pt projected onto the same element e2=%i or an element with the same node n=%i\n",
			     e2,n);
		  
		      ok=true;
		    }
		    else
		    {
		      if( debugs & 2 ) printf(" --> e=%i e2=%i nodes(e2)=(%i,%i,%i) "
			     "distMin=%8.2e, dist1=|s-s2|=%8.2e dist2=%8.2e\n",
			     e,e2,element(e2,0),element(e2,1),element(e2,2),dist,dist1,dist2);
		    }
		  }
		  
		}
		if( !ok )
		{
		  if( debugs & 2) printf(" stitch is not valid---> distMin=%8.2e, dist1=|s-s2|=%8.2e dist2=%8.2e\n",
                   dist,dist1,dist2);
		}
		
	    
	      }
	      else
	      {
		if( debugs & 2) printf(" stitch valid so far---> distMin=%8.2e, dist1=|s-s2|=%8.2e dist2=%8.2e "
                     "nodeInfo(n,4;5)=(%i,%i)\n", dist,dist1,dist2,nodeInfo(n,4),nodeInfo(n,5));
	      }
	  
	      if( ok )
	      {
		// if the node is still ok -- check that the midpoint between the node and x0 is not
		// inside another surface -- this could happen if we skip across a thin surface from
		// a duplicate point. -- only need to check duplicate points this way ----
                // *** we need to know whether this point will be considered for a duplicate
                // ---> just checkif we are near a corner (r0,r1)
                const real eps=1.e-3;
                
                // bool corner = (int(fabs(r0) < eps) + int(fabs(r0+r1-1.) < eps) + int(fabs(r1) < eps))==2;
		// if( true || dist > epsStitch*.1 || corner  || nodeInfo(n,4)>=0 || nodeInfo(n,5)>=0 )
                if( nodeInfo(n,5)>=0 )
		{
		  if( debugs & 2 )
                    printf(" *** Double check a stitch to a pt that is already a duplicate point **** \n");

		  x2=.5*(x+x1);
		  x20=x2;
		  real r0New,r1New,distNew;
		  int sNew=-1,localElementNumber ;
		  if( closerSurfaceFound(dist,s,s2,x20,x2,numberOfSurfacesToCheck,surfacesToCheck,boundingBox,
					 boundaryp,mpParams2,sNew,distNew,localElementNumber,r0New,r1New ) )
		  {
                    int eNew = boundaryp[sNew]->tags(localElementNumber);
		    assert( eNew>=0 );
                    ok=false;
		    
                    // ***** this code segment is duplicated below -- fix this *****
                    if( nodeInfo(n,5)>=0 )
		    {
		      // ** make sure we have not just matched to a duplicate node ***
		      ok=true;
		      const real eps=.1; 
		      bool corner = (int(fabs(r0New) < eps) + int(fabs(r0New+r1New-1.) < eps) +
                                      int(fabs(r1New) < eps))==2;
                      if( debugs & 2 )
                         printf(" closer surface found : distNew=%e, (r0,r1)=(%e,%e)\n",distNew,r0New,r1New);
		      
		      if( corner )
		      {
			int nc=-1;
			if( fabs(r0New) < eps && fabs(r1New) < eps )
			  nc=0;
			else if( fabs(r0New) < eps && fabs(r0New+r1New-1.) < eps )
			  nc=2;
			else
			  nc=1;

			int eNew = boundaryp[sNew]->tags(localElementNumber);  assert( eNew>=0 );
			int cNode=element(eNew,nc);

			ok=false;
			int dupNode=nodeInfo(n,5);      // starting node
			while( dupNode>=0 ) 
			{
			  if( dupNode==cNode )
			  {
			    ok=true;   // we have just matched a duplicate node.
			    break;
			  }
			  dupNode=nodeInfo(dupNode,4);
			}
		    
		      }
		      else
		      {
			ok=false;  // mid-point is closer to another surface -- invalidate this stitch.
		      }
		    }

		    if( !ok )
		    {
		      // we check if we are inside since otherwise we could have just matched to a duplicate point
		      if( debugs & 2) 
		      {
			printf(" stitch is not valid--->\n");
			printf(" Midpt is closer to another surf: i=%i n=%i s=%i s2=%i : sNew=%i dist=%e distNew=%e\n",
			       i,n,s,s2,sNew,dist,distNew);
		      }
		      // mid-point is closer to another surface -- invalidate this stitch.
		      ok=false;
		    }
		  }
	      
		}
	    
	      }
	    }

	    if( !ok )
	    {
	      continue;
	    }
	  }  // end if( true )
	  


          // --- check for duplicates ----
          // We need to check for duplicates here so we can find all the connections :
          //                          ||
          //                          ||
          //                          ||
          //              ------------AB-------
          //              ------------------------
          //                          ^
          //                          | -- A or B should connect to this point, not to each other
          //
          bNode(i)=-1;  // ** fix this *** this is how we say keep looking
          // fix this: local edge is passed in, no need for dist0,dist1,dist2

          // *** there is a problem here if we mark a point as a duplicate to s2 but then later we
          //     find a closer surface s3 ****
          // ---> there there may be a duplicate	  

          // **** use x0 to test for duplicates *** wdh 010124
          // **** no --> we need to check x, then discard invalid ones ****
          bool isDuplicate=bNodeInfo(i,3)>0;
	  if( dist<epsx && 
              isDuplicateNode(i,n,e,s,s2,r0,r1,x,epsDup,bNode,nodeInfo,localEdge,dist0,dist1,dist2,debugs) )
	  {
            // if a point was already matched to a surface and then became a duplicate we need to 
            // double check it with the more careful check.
            
            if( bNodeInfo(i,3)<1 )
              bNodeInfo(i,3)=1;  // this means this node is a duplicate.
            if( !isDuplicate && bNodeInfo(i,0)>=0 )
	    {
	      if( debugs & 2 )
                printf("***** INFO duplicate point needs to be double checked! bNodeInfo(i,0)=%i ***** \n",
                   bNodeInfo(i,0));
              bNodeInfo(i,3)=2;  // this means double check again
	      
	    }
            
	    // a duplicate node was found. --- this is marked in nodeInfo(n,4)
            // *** the info about a dup node stitched to another surface (at a non-dup node)
            //     is saved with the 'last' dup node ***
            if( bNode(i)==n )
   	      continue;   // do not use this closest surface -- keep looking for another one
	  }
	  if( dist<distMin(i) )
	  {

	    distMin(i)=dist;
	    bNodeInfo(i,0)=s2;
	    bNodeInfo(i,1)=e;
	    bNodeInfo(i,2)=localEdge;
	    coordinates(i,0)=r0;  //  (r,s) = local triangle coordinates
	    coordinates(i,1)=r1;

	    numberConnected[s2]++;   // upper bound for the number of points connect to surface s2.
	  }

	  bNode(i)=n; // reset
	  
	}
	
      }
    }
    

    // Now double check suspicious nodes.
    if( true )
    {
      const real tol = stitchingTolerance>0. ? stitchingTolerance : 1.e-3;
      for( int i=0; i<numberOfBoundaryNodes(s); i++ )
      {
	if( bNodeInfo(i,3)==2 )
	{
	  
	  s2=bNodeInfo(i,0);
	  real localScale=max(subSurfaceScale(s),subSurfaceScale(s2));

	  const real epsx = SQR(tol*localScale);
	  const real epsSep=max(epsx*.01,boundaryNodeSeparation(i));
	  const real epsStitch= min(epsx,epsSep*SQR(.1));

	  int n=bNode(i);
          if( debugs & 2 ) printf(" **** triple check s=%i i=%i, n=%i s2=%i\n",s,i,n,s2);


	  bool ok=true;

	  real doubleCheckEps=epsStitch*100. ;

	  if( distMin(i) > doubleCheckEps )  // *** we need a better measure than epsStitch
	  {
	    if( debugs & 2 ) 
              printf(" ---- s=%i, i=%i, n=%i s2=%i distMin=%8.2e > %8.2e. Stitch not valid.\n",s,i,n,
		   s2,distMin(i),doubleCheckEps);
	    ok=false;
	  }
	  else if( distMin(i) > epsStitch*.01 )
	  {

	  
	    int e = bNodeInfo(i,1);  // The projected pt is in this element
	    assert( e>=0 );
	    int na = element(e,0), nb=element(e,1), nc=element(e,2);

	    xa[0]=node(nb,0)-node(na,0), xa[1]=node(nb,1)-node(na,1), xa[2]=node(nb,2)-node(na,2);
	    xb[0]=node(nc,0)-node(na,0), xb[1]=node(nc,1)-node(na,1), xb[2]=node(nc,2)-node(na,2);
	    // get the coordinates of the projected point
	    real r0= pCoordinates[s](i,0);  //  (r,s) = local triangle coordinates
	    real r1= pCoordinates[s](i,1);  //  (r,s) = local triangle coordinates

	    x0(0,0)=node(na,0)+xa[0]*r0+xb[0]*r1;
	    x0(0,1)=node(na,1)+xa[1]*r0+xb[1]*r1;
	    x0(0,2)=node(na,2)+xa[2]*r0+xb[2]*r1;
	  
	  
	    // project back onto surface s (element e0.?)
	    //
	    int f0 = nodeInfo(n,0);
	    assert( f0>=0 );
	    int e0 = faceElements(f0,0); // the node n is near this element
	    assert( e0>=0 );

            int surfaceGuess=subSurfaceIndex(0);
	    subSurfaceIndex=e0;
	    // subSurfaceIndex=-1;  // ************
	  
	    x=x0;
	    map.project( x, mpParams );

	    


	    //          ---------|
	    //  x0 = node n projected on s2
	    //  x  = x0 projected onto s
	    //  distMin = dist between node n and s2
	    //  dist1 = distance between surface s and s2 near x0
	    //  dist2 = distance between node n and x
	    //
	    //  
	    real dist1 = SQR(x(0,0)-x0(0,0))+ SQR(x(0,1)-x0(0,1))+ SQR(x(0,2)-x0(0,2));
	    real dist2 = SQR(x(0,0)-node(n,0))+ SQR(x(0,1)-node(n,1))+ SQR(x(0,2)-node(n,2));
	    if( debugs & 2)
	    {
	      printf(" ---- s=%i, i=%i, n=%i s2=%i e=%i distMin=%8.2e is suspicious. epsStitch=%8.3e  Double checking.\n",
		     s,i,n,s2,e,distMin(i),epsStitch);
	      printf("  x0(s2)=(%8.2e,%8.2e,%8.2e) x=(%8.2e,%8.2e,%8.2e)\n",x0(0,0),x0(0,1),x0(0,2),
		     x(0,0),x(0,1),x(0,2));
	    }
	    

	    if( dist1>distMin(i) )
	    {
	      if( debugs & 2) printf("** something is wrong*** \n");
	    }
	  

	    if( dist2>min(distMin(i),dist1) )
	    {
	      if( debugs & 2)
                printf(" stitch is not valid---> distMin=%8.2e, dist1=|s-s2|=%8.2e dist2=%8.2e\n",distMin(i),dist1,dist2);
	      ok=false;

	    
	    }
	    else
	    {
	      if( debugs & 2)
                printf(" stitch valid so far---> distMin=%8.2e, dist1=|s-s2|=%8.2e dist2=%8.2e nodeInfo(n,4;5)=(%i,%i)\n",
		     distMin(i),dist1,dist2,nodeInfo(n,4),nodeInfo(n,5));
	    }
	  
	    if( ok )
	    {
	      // if the node is still ok -- check that the midpoint between the node and x0 is not
	      // inside another surface -- this could happen if we skip across a thin surface from
	      // a duplicate point. -- only need to check duplicate points this way ----
	      if( nodeInfo(n,4)>=0 || nodeInfo(n,5)>=0 )
	      {
		if( debugs & 2 )
                  printf(" *** Should double check this duplicate point nodeInfo(n,4)=%i nodeInfo(n,5)=%i **** \n",
                    nodeInfo(n,4),nodeInfo(n,5));
		x2=.5*(x0+x);
		x20=x2;
		real dist=distMin(i), distNew;
		int sNew=-1,localElementNumber ;

                // **** we need to avoid checking any surface that we already match as a duplicate point ***
		

		if( closerSurfaceFound(dist,s,s2,x20,x2,numberOfSurfacesToCheck,surfacesToCheck,boundingBox,
				       boundaryp,mpParams2,sNew,distNew,localElementNumber,r0,r1 ) )
		{
                  // ** make sure we have not just matched to a duplicate node ***
                  if( debugs & 2 ) printf(" closer surface found. sNew=%i, (r0,r1)=(%e,%e)\n",sNew,r0,r1);
		  
                  ok=true;
		  const real eps=.1;  // ****** what to use here ****
		  bool corner = (int(fabs(r0) < eps) + int(fabs(r0+r1-1.) < eps) + int(fabs(r1) < eps))==2;
                  if( corner )
		  {
                    int nc=-1;
		    if( fabs(r0) < eps && fabs(r1) < eps )
                      nc=0;
		    else if( fabs(r0) < eps && fabs(r0+r1-1.) < eps )
		      nc=2;
		    else
                      nc=1;

		    int eNew = boundaryp[sNew]->tags(localElementNumber);  assert( eNew>=0 );
                    int cNode=element(eNew,nc);

                    if( debugs & 2) printf("Corner found, eNew=%i, cNode=%i\n",eNew,cNode);
		    

                    ok=false;
		    int dupNode=nodeInfo(n,5);      // starting node
		    while( dupNode>=0 ) 
		    {
		      if( dupNode==cNode )
		      {
                        ok=true;   // we have just matched a duplicate node.
			break;
		      }
		      dupNode=nodeInfo(dupNode,4);
		    }
		    
		  }
		  else
		  {
		    ok=false;  // mid-point is closer to another surface -- invalidate this stitch.
		  }
		  if( !ok )
		  {
		    if( debugs & 2) 
		    {
		      printf(" stitch is not valid--->\n");
		      printf(" Midpt is closer to another surface: i=%i, n=%i s=%i, s2=%i : sNew=%i, dist=%e distNew=%e\n",
			     i,n,s,s2,sNew,dist,distNew);
		    }
		  }
		  
		}
	      
	      }
	    
	    }
	  }

	  if( !ok )
	  {
	    if( debugs & 2) printf(" *** stitch is not valid ****\n");
	    
	    bNodeInfo(i,0)=-1;  // undo the stitch for this point -- it could still be a duplicate
	  }
	  else
	  {
	    if( debugs & 2) printf(" *** stitch is ok **** bNodeInfo(i,0)=%i\n",bNodeInfo(i,0));
	  }
	  

	}
      }
    }
    

    if( debugs & 4 )
    {
      printf("\n =============== Connection Info for Nodes on surface s=%i ===============\n",s);
      for( int i=0; i<numberOfBoundaryNodes(s); i++ )
      {
	if( bNodeInfo(i,0)>=0 )
	  printf(" s=%i, i=%i, n=%i closest surface s2=%i dist=%e element e=%i edge=%i (r0,r1)=(%8.2e,%8.2e)\n",
		 s,i,bNode(i),bNodeInfo(i,0),distMin(i),bNodeInfo(i,1),bNodeInfo(i,2),
		 coordinates(i,0),coordinates(i,1));
      }
    }
    
  } // end for s
  

  intArray *connectionInfo = new intArray [cs.numberOfSubSurfaces()];
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    if( numberConnected[s]>0 )
    {
      connectionInfo[s].redim(numberConnected[s],2);
      numberConnected[s]=0;
    }
  }
  

  // **** Find all nodes that connect to surface s *****
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    intArray & bNodeInfo = pbNodeInfo[s];
    for( int i=0; i<numberOfBoundaryNodes(s); i++ )
    {
      int s2=bNodeInfo(i,0);
      if( s2>=0 )
      {
	connectionInfo[s2](numberConnected[s2],0)=s;
	connectionInfo[s2](numberConnected[s2],1)=i;
	numberConnected[s2]++;
      }
    }
  }
  
  if( debugs & 4 )
  {
    for( s=0; s<cs.numberOfSubSurfaces(); s++ )
    {
      for( int i=0; i<numberConnected[s]; i++ )
      {
	int s2=connectionInfo[s](i,0), i2=connectionInfo[s](i,1);
	printf("s=%i connection info: point %i on surface s2=%i is connected to e=%i\n",
	       s,i2,s2,pbNodeInfo[s2](i2,1));
      }
    }
  }
  

  // *** stitch surface together ****
  
  int i;
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    intArray & bNodeInfo = pbNodeInfo[s];
    intArray & connection = connectionInfo[s];
    
    intArray el(numberConnected[s]+1);
    el(numberConnected[s])=-1;  // marks the end
    
    for( i=0; i<numberConnected[s]; i++ )
    {
      int s2=connection(i,0);
      int i2=connection(i,1);
      
      el(i)=pbNodeInfo[s2](i2,1);   // element number
    }
    
    // sort connection array by element number

    // *** sort ****

    if( numberConnected[s]==0 )
    {
      if( debugs & 2) printf("WARNING: no points are connected to surface s=%i\n");
      continue;
    }
    
    intArray permutation(numberConnected[s]);
    permutation.seqAdd(0,1);
    
    elementArray = &el(0);  // used by the elementCompare function, above.
    qsort( &permutation(0),numberConnected[s],sizeof(int), elementCompare );

    Range I=numberConnected[s];
    connection(I,0)=connection(permutation,0);
    connection(I,1)=connection(permutation,1);
    el(I)=el(permutation);
    
    if( debugs & 4 )
    {
      for( i=0; i<numberConnected[s]; i++ )
      {
	int s2=connectionInfo[s](i,0), i2=connectionInfo[s](i,1);
	printf("s=%i SORTED connection info: point %i on surface s2=%i is connected to e=%i\n",
	       s,i2,s2,pbNodeInfo[s2](i2,1));

      }
    }
    
    for( i=0; i<numberConnected[s]; )
    {
      int iStart=i;
      int eBase=el(iStart);
      i++;
      while( el(i)==eBase )
      {
	i++;
      }
      int iEnd=i-1;
      
      // connect nodes [iStart,iEnd] to element e

      if( debugs & 2 ) printf(" s=%i, connect nodes [iStart,iEnd]=[%i,%i] to element eBase=%i\n",s,iStart,iEnd,eBase);
      const int maxNumberOfSubElements = 150; // fix this num = iEnd-iStart+1;
      assert( iEnd-iStart+1 < maxNumberOfSubElements );
      
      real rCoord[2][3][maxNumberOfSubElements];  // rCoord[Start,End][edge][element]
      rCoord[0][0][0] = 0.; rCoord[1][0][0] = 1.; // edge 0: r in [0,1]
      rCoord[0][1][0] = 0.; rCoord[1][1][0] = 1.; // edge 1: s in [0,1]
      rCoord[0][2][0] = 0.; rCoord[1][2][0] = 1.; // edge 2: s in [0,1]
      

      const int eStart=numberOfElements;  // this will be the first new element added.
      int j;
      int numberOfSubElements=0;
      for( j=iStart; j<=iEnd; j++ )
      {
        // we just need to keep track of which element we should add to, may not be e after the first time.
        // if we keep the (r0,r1) coordinates of the corners of each element created here then we determine
        // which element we were in.

	int s2=connection(j,0);
	int i2=connection(j,1);

        int n=bNodep[s2](i2);

 	if( nodeInfo(n,4)>=0 )
 	{
 	  // skip this node for now -- it is a duplicate
           continue;
         }

        int localEdge=pbNodeInfo[s2](i2,2);
	
        real r0= pCoordinates[s2](i2,0);  //  (r,s) = local triangle coordinates
        real r1= pCoordinates[s2](i2,1);  //  (r,s) = local triangle coordinates
        real rr = localEdge==0 ? r0 : localEdge==1 ? r1 : r1;

        if( debugs & 2 ) 
          printf(" j=%i: s2=%i, i2=%i, n=%i localEdge=%i (r0,r1)=(%8.2e,%8.2e)\n",j,s2,i2,n,localEdge,r0,r1);
	
        // find the element we are in
        int e=-1;
        if( j==iStart )
	  e=eBase;
	else
	{
          // if localEdge==0 we need rCoord[0][1][e]==0.
          // if localEdge==1 we need rCoord[1][0][e]==1.
          // if localEdge==2 we need rCoord[1][1][e]==1.
	  for( int ee=0; ee<=numberOfSubElements; ee++ )
	  {
	    if( rr>=rCoord[0][localEdge][ee] && rr<=rCoord[1][localEdge][ee] )
	    {
// 	      if( (localEdge==0 && rCoord[0][1][ee]==0. ) ||
// 		  (localEdge==1 && fabs(rCoord[1][0][ee]-1.)<REAL_EPSILON ) ||
// 		  (localEdge==2 && fabs(rCoord[1][1][ee]-1.)<REAL_EPSILON ) )
	      {
		if( ee==0 )
		{
		  e=eBase;
		}
		else
		{
		  e=eStart+ee-1;
		}
		break;
	      }
	    }
	  }
	}
        assert( e>=0 && e<numberOfElements );
	if( debugs & 2 ) 
	{
	  int ee= e==eBase ? 0 : e-eStart+1;
	  printf("Add sub node %i: split element e=%i (eBase=%i) localEdge=%i rr=%e  ee=%i : [%8.2e,%8.2e]\n",
		 j-iStart, e,eBase,localEdge,rr,ee,rCoord[0][localEdge][ee],rCoord[1][localEdge][ee]);
	}
	
	numberOfSubElements++;
	int fc=ef(e,localEdge);   // face on boundary that will remain with the old element e

	// split this element into two
        // 
        //
        //             C
        //           / | \
        //         /   |   \
        //       /     |     \
        //     /   e   | eNew  \
        //   A -----------------B
        //           B' A'

        assert(n>=0 && n<numberOfNodes );

	int eNew=numberOfElements;
	element(eNew,0)=element(e,0);
	element(eNew,1)=element(e,1);
	element(eNew,2)=element(e,2);
	element(eNew,localEdge)=n;     // over-write one of the above

	element(e,(localEdge+1)%3)=n;  //  change the existing element

	if( debugs & 2) 
	{
	  printf(" new element e=%i nodes (%i,%i,%i) \n",e,element(e,0),element(e,1),element(e,2));
	  printf(" new element eNew=%i nodes (%i,%i,%i) \n",eNew,element(eNew,0),element(eNew,1),element(eNew,2));
	}
	

	numberOfElements++;
	assert( numberOfElements<maxNumberOfElements );
	  
	elementSurface(eNew)=s;  // the new element sits on this subsurface

        // assign new rCoord bounds for the new triangles

        int e0 = e==eBase ? 0 : e-eStart+1;
        int en=numberOfSubElements;
        rCoord[0][0][en]=rCoord[0][0][e0]; rCoord[1][0][en]=rCoord[1][0][e0];
        rCoord[0][1][en]=rCoord[0][1][e0]; rCoord[1][1][en]=rCoord[1][1][e0];
        rCoord[0][2][en]=rCoord[0][2][e0]; rCoord[1][2][en]=rCoord[1][2][e0];

	
        if( localEdge==2 )
	{
   	  rCoord[1][localEdge][en]=rr;
          rCoord[0][localEdge][e0]=rr;
	}
	else
	{
   	  rCoord[0][localEdge][en]=rr;
          rCoord[1][localEdge][e0]=rr;
	}
	
        // invalidate interior diagonals
        rCoord[0][(localEdge+1)%3][e0]=-1.;
        rCoord[1][(localEdge+1)%3][e0]=-1.;
        rCoord[0][(localEdge+2)%3][en]=-1.;
        rCoord[1][(localEdge+2)%3][en]=-1.;


	if( debugs & 2 )
	{
	  printf("Split element e  =%i  localEdge=%i new bounds [%e,%e]\n",e,localEdge,rCoord[0][localEdge][e0],
		 rCoord[1][localEdge][e0]);
	  printf("Split element eNew=%i localEdge=%i new bounds [%e,%e]\n",eNew,localEdge,rCoord[0][localEdge][en],
		 rCoord[1][localEdge][en]);
	}
	
        // new faces are generated when the element is split
	if( face(fc,0)==element(e,localEdge) )
	  face(fc,1)=n;
	else
	  face(fc,0)=n;

	if( debugs & 4 ) printf(" Change face on boundary: fc=%i nodes=(%i,%i) \n",fc,face(fc,0),face(fc,1));
	// faceElement(fc,0)= same;
	     

	int newFace=numberOfFaces;
	numberOfFaces++;
	assert( numberOfFaces<maxNumberOfFaces );
	    
	face(newFace,0)=element(eNew,(localEdge+1)%3);
	face(newFace,1)=n;
	    
	faceElements(newFace,0)=eNew;
	faceElements(newFace,1)=-1;
	    
	int newFace2=numberOfFaces;   // diagonal
	numberOfFaces++;
	assert( numberOfFaces<maxNumberOfFaces );
	face(newFace2,0)=element(e,(localEdge+2)%3);
	face(newFace2,1)=n;
	    
	faceElements(newFace2,0)=eNew;
	faceElements(newFace2,1)=e;

	// Another face needs to be changed: this face was removed from e
	int fc2=ef(e,(localEdge+1)%3);
	    
	if( faceElements(fc2,0)==e )
	  faceElements(fc2,0)=eNew;
	else if( faceElements(fc2,1)==e )
	  faceElements(fc2,1)=eNew;
	else
	{
	  printf("***ERROR*** in assigning faceElements for face fc2=%i, fe=(%i,%i) =? e=%i\n",fc2,
		 faceElements(fc2,0),faceElements(fc2,1),e  );
	}
	    
	// assign elementFaces: faces on each element
	ef(eNew,localEdge)=newFace;
	ef(eNew,(localEdge+1)%3)=ef(e,(localEdge+1)%3);
	ef(eNew,(localEdge+2)%3)=newFace2;

	ef(e,(localEdge+1)%3)=newFace2;
            
	  // each boundary node can point to possibly 4 boundary faces.
	nodeInfo(n,2)=fc;
	nodeInfo(n,3)=newFace;

	int node1=element(eNew,(localEdge+1)%3);
	if( nodeInfo(node1,0)==fc )
	  nodeInfo(node1,0)=newFace;
	else if( nodeInfo(node1,1)==fc )
	  nodeInfo(node1,1)=newFace;
	else if( nodeInfo(node1,2)==fc )
	  nodeInfo(node1,2)=newFace;
	else if( nodeInfo(node1,3)==fc )
	  nodeInfo(node1,3)=newFace;
	else
	{
	  int nodeA=element(eNew,0);
	  int nodeB=element(eNew,1);
	  int nodeC=element(eNew,2);
	  printf("****ERROR: unable to match bndry nodes to a face. node1=%i, nodeInfo=(%i,%i,%i,%i)\n"
		 "...I was expecting to see face fc=%i in the nodeInfo list. Adding node n=%i. localEdge=%i\n"
		 "...s=%i, s2=%i, changing element e=%i, nodes=(%i,%i,%i), eNew=%i nodes=(%i,%i,%i)\n"
		 "...nodeInfo(%i,.)=(%i,%i,%i,%i),nodeInfo(%i,.)=(%i,%i,%i,%i),"
		 "nodeInfo(eNew=%i,.)=(%i,%i,%i,%i)\n",
		 node1,nodeInfo(node1,0),nodeInfo(node1,1),nodeInfo(node1,2),nodeInfo(node1,3),fc,n,
		 localEdge,s,s2,e,element(e,0),element(e,1),element(e,2),
		 eNew,element(eNew,0),element(eNew,1),element(eNew,2),
		 nodeA,nodeInfo(nodeA,0),nodeInfo(nodeA,1),nodeInfo(nodeA,2),nodeInfo(nodeA,3),
		 nodeB,nodeInfo(nodeB,0),nodeInfo(nodeB,1),nodeInfo(nodeB,2),nodeInfo(nodeB,3),
		 nodeC,nodeInfo(nodeC,0),nodeInfo(nodeC,1),nodeInfo(nodeC,2),nodeInfo(nodeC,3) );
	}
	if( debugs & 4 ) printf("set nodeInfo for node1=%i: nodeInfo=(%i,%i,%i,%i)\n",node1,
			       nodeInfo(node1,0),nodeInfo(node1,1),
			       nodeInfo(node1,2),nodeInfo(node1,3));

	if( debugs & 4 )
	{
	  printf("add new element to global grid: e=%i, nodes=(%i,%i,%i)\n",eNew,
		 element(eNew,0), element(eNew,1),element(eNew,2));
	  // printf("**** connectivity before split element\n");
	  // mapNew.printConnectivity();
	}


      }

    }
    
  }
  

  // *******************************************
  // **** Now determine the connectivity *******
  // *******************************************

  const int numberOfSurfaces = cs.numberOfSubSurfaces();
  intArray signForNormal(numberOfSurfaces);
  for( s=0; s<numberOfSurfaces; s++ )
    signForNormal(s)=numberOfSurfaces*10 + s;   // give each surface a unique id
    
  signForNormal(0)=+1;  // sub surface 0 gets sign=+1

  intArray consistent(numberOfSurfaces,numberOfSurfaces),inconsistent(numberOfSurfaces,numberOfSurfaces);
  consistent=0;
  inconsistent=0;
  

  maxNumberOfNodesPerElement=3;
  maxNumberOfNodesPerFace=2;
  maxNumberOfFacesPerElement=3;

  numberOfEdges=numberOfFaces;
  edge.reference(face);

  int nf1=0, nf2=0, face1[8], face2[8];

  for( int n=0; n<numberOfNodes; n++ )
  {
    if( nodeInfo(n,0)>=0 )
    {
      if( debugs & 4 )
	printf(" bdy node: n=%i, nodeInfo = (%i,%i,%i,%i) (faces)\n",n,
	       nodeInfo(n,0),nodeInfo(n,1),nodeInfo(n,2),nodeInfo(n,3));
      if( nodeInfo(n,2)>=0 || nodeInfo(n,4)>0 )
      {
	// ** int fa=nodeInfo(n,0), fb=nodeInfo(n,1), fc=nodeInfo(n,2), fd=nodeInfo(n,3);

	if( nodeInfo(n,4)>=0 )
	{ // this must be a duplicate node
	  int n0=nodeInfo(n,4);
	  
	  // save faces to be checked if they are the same.
	  nf1=2, face1[0]=nodeInfo(n,0), face1[1]=nodeInfo(n,1);
	  nf2=2, face2[0]=nodeInfo(n0,0), face2[1]=nodeInfo(n0,1);

	  // **** if nodeInfo(n,2:3) >=0 add these faces to both lists
	  // **** if nodeInfo(n0,2:3) >=0 add these faces to both lists
	  if( nodeInfo(n,2)>=0 )
	  {
	    face1[nf1]=nodeInfo(n,2); nf1++; face1[nf1]=nodeInfo(n,3); nf1++;
	    face2[nf2]=nodeInfo(n,2); nf2++; face2[nf2]=nodeInfo(n,3); nf2++;
	  }
	  if( nodeInfo(n0,2)>=0 )
	  {
	    face1[nf1]=nodeInfo(n0,2); nf1++; face1[nf1]=nodeInfo(n0,3); nf1++;
	    face2[nf2]=nodeInfo(n0,2); nf2++; face2[nf2]=nodeInfo(n0,3); nf2++;
	  }

	  // replace node n0 with node n

	  int nr=n; // replace with the smaller node
	  if( n<n0 )
	    replaceNode( n,n0, nodeInfo,ef );
	  else
	  {
	    nr=n0;
	    replaceNode( n0,n, nodeInfo,ef );  // *****       
	  }
	  
	  if( false )
	  {
	    int count = sum( (element-n0)==0 + sum(face-n0)==0 );
	    if( count >0 )
	    {
	      printf("dup: ERROR: node n0=%i not replaced in element or face \n",n0);
	      where( face==n0 )
		face=n;
	    }
	  }
	    

	  if( nodeInfo(n0,4)>0 && nodeInfo(n0,4)!=n )
	  {
	    int n1=nodeInfo(n0,4);
	    face1[nf1]=nodeInfo(n1,0), face1[nf1+1]=nodeInfo(n1,1); nf1+=2;
	    face2[nf2]=nodeInfo(n1,0), face2[nf2+1]=nodeInfo(n1,1); nf2+=2;

	    replaceNode( nr,n1, nodeInfo,ef );
	      
	    if( nodeInfo(n1,4)>0 && nodeInfo(n1,4)!=n && nodeInfo(n1,4)!=n0 )
	    {
	      int n2=nodeInfo(n1,4);
	      face1[nf1]=nodeInfo(n2,0), face1[nf1+1]=nodeInfo(n2,1); nf1+=2;
	      face2[nf2]=nodeInfo(n2,0), face2[nf2+1]=nodeInfo(n2,1); nf2+=2;

	      replaceNode( nr,n2, nodeInfo,ef );
		
	      if( nodeInfo(n2,4)>0 && nodeInfo(n2,4)!=n && nodeInfo(n2,4)!=n0 && nodeInfo(n2,4)!=n1 )
	      {
		int n3=nodeInfo(n2,4);
		face1[nf1]=nodeInfo(n3,0), face1[nf1+1]=nodeInfo(n3,1); nf1+=2;
		face2[nf2]=nodeInfo(n3,0), face2[nf2+1]=nodeInfo(n3,1); nf2+=2;

		replaceNode( nr,n3, nodeInfo,ef );

		if(debugs & 2) printf("*** quint node: n=%i, will replace n0=%i, n1=%i, n2=%i, n3=%i\n",
				     n,n0,n1,n2,n3);

		if( nodeInfo(n3,4)!=-1 )
		{
                  printf("***** ERROR:There appears to be more than a quint node! ******\n");
                  printf("*** quint node: n=%i, will replace n0=%i, n1=%i, n2=%i, n3=%i\n",
				     n,n0,n1,n2,n3);
		}
		

	      }
	      else
	      {
		if(debugs & 2) printf("*** quadruplicate node: n=%i, will replace n0=%i, n1=%i, n2=%i\n",n,n0,n1,n2);
	      }
	    }
	    else
	    {
	      if( debugs & 2 ) printf("*** triplicate node: n=%i, will replace n0=%i, n1=%i\n",n,n0,n1);
	    }
	  }
	  else
	  {
	    if( debugs & 2 ) printf("*** duplicate node: node n=%i will replace node n0=%i \n",n,n0);
	  }
	}
	else  // this is not a duplicate node.
	{
	  nf1=2, face1[0]=nodeInfo(n,0), face1[1]=nodeInfo(n,1);
	  nf2=2, face2[0]=nodeInfo(n,2), face2[1]=nodeInfo(n,3);

	}
	  
	  
	for( int m1=0; m1<nf1; m1++ )
	{
	  for( int m2=0; m2<nf2; m2++ )
	  {
	    int fa=face1[m1], fb=face2[m2];
	    if( fa!=fb &&
		((face(fa,0)==face(fb,0) && face(fa,1)==face(fb,1)) ||
		 (face(fa,1)==face(fb,0) && face(fa,0)==face(fb,1))) )
	    {
	      int ea=faceElements(fa,0);
	      int eb=faceElements(fb,0);
              if( ea==eb )
                continue;
	      
	      if( debugs & 2 ) printf(" faces %i and %i are the same. elements (%i,%i)\n",fa,fb,ea,eb);
	      faceElements(fa,1)=eb;
	      faceElements(fb,1)=ea;

	      int na=face(fa,0), nb=face(fa,1);
	      int a0 = element(ea,0)==na ? 0 : element(ea,1)==na ? 1 : 2;
	      int a1 = element(ea,0)==nb ? 0 : element(ea,1)==nb ? 1 : 2;

	      int b0 = element(eb,0)==na ? 0 : element(eb,1)==na ? 1 : 2;
	      int b1 = element(eb,0)==nb ? 0 : element(eb,1)==nb ? 1 : 2;

	      int c1 = (a1==( (a0+1)%3 )) ? 1 : -1;  // 1=counter-clockwise order a0-->a1, -1=clockwise
	      int c2 = (b1==( (b0+1)%3 )) ? 1 : -1;

	      int surfa=elementSurface(ea), surfb=elementSurface(eb);
//	      int orienta=orientation(surfa), orientb=orientation(surfb);

              // surfaces surfa and surfb are connected

	      int signa=signForNormal(surfa);
	      int signb=signForNormal(surfb);

	      if( abs(signa) != abs(signb) )
	      {
                if( debugs ) printf("Surface %i is connected to surface %i",surfa,surfb);
		if( c1*c2 < 0  )
		{
		  // same sign
                  signForNormal(surfb)=signa;
                  if( debugs ) printf(", with the same orientation\n");
		}
                else
		{
                  signForNormal(surfb)=-signa;
                  if( debugs ) printf(", with the opposite orientation\n");
		}
		for( int ss=0; ss<numberOfSurfaces; ss++ )
		{
		  if( abs(signForNormal(ss))==abs(signb) )
		    signForNormal(ss)= signForNormal(ss)==signb ? signForNormal(surfb) : -signForNormal(surfb);
		}
		
	      }
              else
	      {
		if( (c1*signa)*(c2*signb) > 0 )
		{
                  if( element(ea,0)==element(ea,1) ||
                      element(ea,1)==element(ea,2) ||
                      element(ea,2)==element(ea,0) )
		  {
                    printf("***WARNING*** element ea=%i has a duplicate node, nodes=(%i,%i,%i)\n",
			   ea,element(ea,0),element(ea,1),element(ea,2));

		    const int f0=ef(ea,0), f1=ef(ea,1), f2=ef(ea,2);    // faces on this element
		    // adjacent elements
		    int ae0 = faceElements(f0,0)==ea ? faceElements(f0,1) : faceElements(f0,0);
		    int ae1 = faceElements(f1,0)==ea ? faceElements(f1,1) : faceElements(f1,0);
		    int ae2 = faceElements(f2,0)==ea ? faceElements(f2,1) : faceElements(f2,0);

		    printf("element %i: nodes=(%i,%i,%i), faces=(%i,%i,%i), adjacent elements=(%i,%i,%i)\n",
			    ea,element(ea,0),element(ea,1),element(ea,2),ef(ea,0),ef(ea,1),ef(ea,2),ae0,ae1,ae2);


		    int edgec = element(ea,0)==element(ea,1) ? 0 : element(ea,1)==element(ea,2) ? 1 : 2;
		    int fc = ef(ea,edgec);
		    if( fc>=0 )
		    {
		      int e2=faceElements(fc,0)==ea ? faceElements(fc,1) : faceElements(fc,0);
                      if( e2>=0 )
                        printf("Element adjacent to collapsed face fc=%i is e2=%i, nodes=(%i,%i,%i)\n",
			     fc,e2,element(e2,0),element(e2,1),element(e2,2));
                      else
		      {
                        printf("There is no element adjacent to collapsed face fc=%i\n",fc);
		      }
		      
		    }
		  }

		  printf("**ERROR** Consistency error for normals on surface %i and surface %i \n",surfa,surfb);
                  printf(" element a=%i, nodes=(%i,%i,%i), element b=%i nodes=(%i,%i,%i)\n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
                         "     node %i : (%11.5e,%11.5e,%11.5e)  \n",
                         ea,element(ea,0),element(ea,1),element(ea,2),
                         eb,element(eb,0),element(eb,1),element(eb,2),
                         element(ea,0),node(element(ea,0),0),node(element(ea,0),1),node(element(ea,0),2),
                         element(ea,1),node(element(ea,1),0),node(element(ea,1),1),node(element(ea,1),2),
                         element(ea,2),node(element(ea,2),0),node(element(ea,2),1),node(element(ea,2),2),
                         element(eb,0),node(element(eb,0),0),node(element(eb,0),1),node(element(eb,0),2),
                         element(eb,1),node(element(eb,1),0),node(element(eb,1),1),node(element(eb,1),2),
                         element(eb,2),node(element(eb,2),0),node(element(eb,2),1),node(element(eb,2),2)
                         );
 
		  inconsistent(surfa,surfb)++;
		  inconsistent(surfb,surfa)++;
		}
		else
		{
		  if( debugs & 2 ) printf("**INFO** Consistency check for normals on surface %i and surface %i \n",
                             surfa,surfb);
		  consistent(surfa,surfb)++;
		  consistent(surfb,surfa)++;
		}
	      }
	      
/* -----
	      if( c1*c2 < 0  )
	      {
		if( debugs & 4 ) printf(" orientation of elements %i and %i looks correct\n",ea,eb);
                  
		orientationCount(surfa,0)+=1;
		orientationCount(surfb,0)+=1;

		if( orienta==0  && orientb==0 )
		{
		  orientation(surfa)=(surfa+1);
		  orientation(surfb)=(surfa+1);
		}
		else if( orientb==0 )
		{
		  orientation(surfb)=orienta;  // same orientation as surfa
		}
		else
		{
		  orientation(surfa)=(surfa+1);
		  where( orientation==orientb )
		    orientation=surfa+1;
		}
	      }
	      else
	      {
		printf("***WARNING*** orientation of elements looks incorrect, ea=%i, eb=%i, "
		       "a0=%i, a1=%i, b0=%i, b1=%i (subsurfaces %i and %i) \n",
		       ea,eb,a0,a1,b0,b1,surfa,surfb);

		orientationCount(surfa,1)+=1;
		orientationCount(surfb,1)+=1;
		  
	      }
---------------- */

	    }
	  }
	}
      }
    }
  }

  if( debugs ) ::display(signForNormal,"signForNormal");

  int numberOfInconsistencies=sum(inconsistent);
  int numberOfConsistencies=sum(consistent);
  printf("\n -----> number of consistencies=%i, number of inconsistencies=%i <------ \n\n",
           numberOfConsistencies,numberOfInconsistencies);
  if( numberOfInconsistencies>0  )
  {
    printf("CompositeSurface::WARNING: There were inconsistencies found, number=%i\n",numberOfInconsistencies);
    for( s=0; s<numberOfSurfaces; s++ )
    {
      for( int s2=s+1; s2<numberOfSurfaces; s2++ )
      {
	if( inconsistent(s,s2)>0 )
	{
	  printf("WARNING: surface s=%i, s2=%i : number of consistencies=%i, number of inconsistencies=%i\n",
                 s,s2,consistent(s,s2),inconsistent(s,s2));
	}
      }
    }
    printf("*** It could be that reducing the stitching tolerance will fix the inconsistencies\n");
    
  }


  // flip orientation of elements on surfaces with a normal in the wrong direction
  if( sum(signForNormal==-1) > 0 )
  {
    for( int e=0; e<numberOfElements; e++ )
    {
      if( signForNormal(elementSurface(e))<0 )
      {
	int temp=element(e,1);
	element(e,1)=element(e,2);
	element(e,2)=temp;
	temp=ef(e,0);
	ef(e,0)=ef(e,2);
	ef(e,2)=temp;
      }
    }
  }
  

/* ----
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    if( signForNormal(s)<0 )
    {
      printf("flipping elements on surface %i \n",s);
      UnstructuredMapping & maps = *boundaryp[s];

      for( int i=0; i<maps.getNumberOfElements(); i++ )
      {
	int eg = maps.tags(i);  // global element number
	// printf("flip global element: %i (element %i on surface %i) \n",eg,i,s);
	int temp=element(eg,1);
	element(eg,1)=element(eg,2);
	element(eg,2)=temp;
	temp=ef(eg,0);
	ef(eg,0)=ef(eg,2);
	ef(eg,2)=temp;
      }

    }
  }
---- */

  timing[timeForStitch]=getCPU()-timeStitch;

  if( debugs & 4 ) printConnectivity();
    
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    timing[timeForProjectLocalSearch]+=boundaryp[s]->timing[timeForProjectLocalSearch];
    timing[timeForProjectGlobalSearch]+=boundaryp[s]->timing[timeForProjectGlobalSearch];
    
    if( boundaryp[s]->decrementReferenceCount()==0 )
      delete boundaryp[s];
  }
  delete [] boundaryp;
  
  element.resize(numberOfElements,maxNumberOfNodesPerElement);


  if( false )
  {
    buildConnectivityLists();
    // printConnectivity();
  }
  else
  {
    const int maxNumberOfBoundaryNodes=numberOfBoundaryFaces+sum(numberOfBoundaryNodes);
    bdyFace.redim(maxNumberOfBoundaryNodes);  // allow for extra
    int bdyFcnt = 0;
    for(int f=0; f<numberOfFaces; f++) 
    {
      if( faceElements(f,1)<0 ) 
      {
	bdyFace(bdyFcnt) = f;
	bdyFcnt++;
      }
    }
    assert( maxNumberOfBoundaryNodes>=bdyFcnt );
    if( numberOfBoundaryFaces!=bdyFcnt )
    {
      if( bdyFcnt>0 )
        bdyFace.resize(bdyFcnt);
      else
	bdyFace.redim(0);
    }
    
    numberOfBoundaryFaces=bdyFcnt;
  }
  
  
  // **** consistency checks *****
  int e;
  for( e=0; e<numberOfElements; e++ )
  {
    for( int m=0; m<=2; m++ )
    {
      if( element(e,m)<0 || element(e,m)>numberOfNodes )
      {
	printf("ERROR: element %i has node %i but numberOfNodes =%i\n",e,element(e,m),numberOfNodes);
	element(e,m)=0;
      }
    }
  }
  for( int f=0; f<numberOfFaces; f++ )
  {
    for( int m=0; m<2; m++ )
    {
      if( face(f,m)<0 || face(f,m)>numberOfNodes )
      {
	printf("ERROR: face %i has node %i but numberOfNodes =%i\n",f,face(f,m),numberOfNodes);
	face(f,m)=0;
      }
    }
  }
  


  tags.resize(numberOfElements);
  // tags = 0;

  delete [] bNodep;
  delete [] pBoundaryNodeSeparation;
  delete [] pDistMin;
  delete [] pCoordinates;
  delete [] pbNodeInfo;
  delete [] numberConnected;
  
  timing[totalTime]+=getCPU()-time0;

  printf("build from a CompositeSurface: numberOfNodes = %i \n", numberOfNodes);
  printf("build from a CompositeSurface: numberOfElements = %i \n", numberOfElements);
  printf("build from a CompositeSurface: numberOfFaces = %i \n", numberOfFaces);
  printf("build from a CompositeSurface: numberOfBoundaryFaces = %i \n", numberOfBoundaryFaces);
  printf("build from a CompositeSurface: numberOfEdges = %i \n", numberOfEdges);
  printf("Time to build from a CompositeSurface = %8.2e\n",getCPU()-time0);

  printStatistics();
  

  return 0;
}
#undef LOCAL_NODE_NUMBER

void
replaceNode( int ea, int n, int n0, intArray & element, intArray & face, intArray & faceElements, intArray & ef )
// ======================================================================================
// /Descritpion:
//   Recursively replace a node n0 with n on element ea and elements that are next to ea
// ======================================================================================
{
  
  bool nodeReplaced=false;
  
  for( int n1=0; n1<3; n1++ )
  {
    if( element(ea,n1)==n0 ) 
    {
      nodeReplaced=true;
      element(ea,n1)=n;
    }
    int f0=ef(ea,n1); // check all faces on this element (to get diagonals too)
    if( f0>=0 )
    {
      for( int m2=0; m2<=1; m2++ )
      {
	if( face(f0,m2)==n0 ) face(f0,m2)=n;
      }
    }
  }
  
  if( nodeReplaced )
  {
    // printf(" Node n0=%i was replaced with node n=%i on element ea=%i\n",n0,n,ea);
    for( int n1=0; n1<3; n1++ )
    {
      int f0=ef(ea,n1); // check all faces on this element (to get diagonals too)
      if( f0>=0 )
      {
	for( int m2=0; m2<=1; m2++ )
	{
	  int eb = faceElements(f0,m2);  // replace nodes on elements that hit face f0
	  if( eb>=0 && eb!=ea )
	  {
	    replaceNode( eb, n, n0, element, face, faceElements, ef );
	  }
	}
      }
    }
  }
}

void UnstructuredMapping::
replaceNode( int n, int n0, intArray & nodeInfo, intArray & ef )
// ======================================================================================
// /Description:
//     Utility routine used to replace one node with another. Called by
// buildFromACompositeSurface.
// ======================================================================================
{
  for( int m=0; m<=3; m++ )
  {
    int f=nodeInfo(n0,m);   // change a face attached to this node
    if( f>=0 )
    {
      for( int side=0; side<=1; side++ )
      {
	int ea=faceElements(f,side);  // change this element which is attached to face f

	if( ea>=0 )
          ::replaceNode( ea, n, n0, element, face, faceElements, ef );

      }
    }
  }
}


