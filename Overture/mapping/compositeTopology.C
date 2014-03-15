// define BOUNDS_CHECK

#include "UnstructuredMapping.h"
#include "MappingProjectionParameters.h"
#include "display.h"
#include "CompositeSurface.h"
#include "TrimmedMapping.h"
#include "ReductionMapping.h"
#include "NurbsMapping.h"
#include "GenericGraphicsInterface.h"

#include "MappingInformation.h"
#include "TriangleWrapper.h"

#include "CompositeTopology.h"
#include "SplineMapping.h"

#include "GL_GraphicsInterface.h"
#include "FaceInfo.h"

#include "GeometricADT.h"

void
constructOuterBoundaryCurve(NurbsMapping *newNurb);

#define SC (char *)(const char *)
extern int triangleErrorDetected;  // error code set by triangle. *** move this to TriangleWrapper

extern real refineTriangulation(UnstructuredMapping &umap, Mapping &cmap, real absoluteTol);
extern bool refineCurve(NurbsMapping &curve, Mapping *surf1, Mapping *surf2, real distTol, real curveTol, realArray &g);

//! Find an edge with the given end points
int CompositeTopology::
getEdgeFromEndPoints(real *x0, real *x1)
{
  int eMatch=-1;
  real xScale=max(max(fabs(x0[0]),fabs(x0[1]),fabs(x0[2])),max(fabs(x1[0]),fabs(x1[1]),fabs(x1[2])));
  real distMin=xScale*1.e-5;  // we should get 5 digits correct.

  if ( allEdges == NULL )
  {
    setupAllEdges();
  }

  for( int e=0; e<numberOfUniqueEdgeCurves; e++ )
    if (allEdges[e])
    {
      const RealDistributedArray & xg = allEdges[e]->curve->getNURBS()->getGrid();
      const int n=xg.getBound(0);
    
      real dist0 = fabs(xg(0,0,0,0)-x0[0])+fabs(xg(0,0,0,1)-x0[1])+fabs(xg(0,0,0,2)-x0[2]);
      if( dist0<distMin )
      {
	real dist1 = fabs(xg(n,0,0,0)-x1[0])+fabs(xg(n,0,0,1)-x1[1])+fabs(xg(n,0,0,2)-x1[2]);
	if( dist1<distMin )
	{
	  eMatch=e;
	  break;
	}
      }
    }
    
  return eMatch;
}

// =======================================================================================================
//! This function is called to check for the presence of a duplicate node. 
/*! 
    We are trying to look for node 'node' in the triangle element(e,.). This function is called
    if 'node' is not found. We look for 'node' in the list of duplicateNodes to see if a duplicate
    node is in the element.
  
   \param elementNodeNumber (output) : if successful (return value==true) this will be a value 0,1,2
      of the node value {\tt node} found in the triangle element(e,.)
   \param node (input/output) : a duplicate node number found (if return value==true)
   \param e (input): find a node in this element.
*/
// =======================================================================================================
bool
duplicateNodeFound(int & elementNodeNumber, int & node, int e, 
                   const intArray & elements, 
                   const intArray & duplicateNodes,
                   int numberOfDuplicateNodes)
{
  elementNodeNumber=-1;
  for( int dn=0; dn<numberOfDuplicateNodes; dn++ )
  {
    elementNodeNumber=-1;
    if( node==duplicateNodes(0,dn) )
    {
      // this is a duplicate node, replace with na:
      int na= duplicateNodes(1,dn);
      elementNodeNumber = elements(e,0)==na ? 0 : elements(e,1)==na ? 1 : elements(e,2)==na ? 2 : -1;
      if( elementNodeNumber>=0 )
      {
        node=na;   // found as a duplicate node.
	break;
      }
    }
    else if( node==duplicateNodes(1,dn) )  // also check if elements array is not correct for duplicates 010512
    {
      int na= duplicateNodes(0,dn);
      elementNodeNumber = elements(e,0)==na ? 0 : elements(e,1)==na ? 1 : elements(e,2)==na ? 2 : -1;
      if( elementNodeNumber>=0 )
      {
	elements(e,elementNodeNumber)=node;
	break;
      }
    }
  }

  return elementNodeNumber!=-1;

}

// ===============================================================================
//! Determine area of a  triangular element and the lengths of the 3 sides.
// ===============================================================================
static void
getTriangleGeometry(real x0[3], real x1[3] , real x2[3], 
                    real & area, 
                    real & length0, 
                    real & length1, 
                    real & length2  )
{
  real a0,a1,a2, b0,b1,b2, c0,c1,c2;
  
  a0=x1[0]-x0[0]; a1=x1[1]-x0[1]; a2=x1[2]-x0[2];
  b0=x2[0]-x1[0]; b1=x2[1]-x1[1]; b2=x2[2]-x1[2];
  c0=x0[0]-x2[0]; c1=x0[1]-x2[1]; c2=x0[2]-x2[2];

  real d0=a1*c2-a2*c1;
  real d1=a2*c0-a0*c2;
  real d2=a0*c1-a1*c0;
  
  length0= sqrt( a0*a0+a1*a1+a2*a2 );
  length1= sqrt( b0*b0+b1*b1+b2*b2 );
  length2= sqrt( c0*c0+c1*c1+c2*c2 );
  
  area= .5*sqrt( d0*d0+d1*d1+d2*d2 );
}

static real
elementNormal(int na, int nb, int nc, realArray & nodes, real *N)
{
  Real Xab[3], Xac[3];
  int i;
  
  for (i=0; i<3; i++)
  {
    Xab[i] = nodes(nb,i) - nodes(na,i);
    Xac[i] = nodes(nc,i) - nodes(na,i);
  }
// N = Xab x Xac
  N[0] = Xab[1] * Xac[2] - Xab[2] * Xac[1];
  N[1] = Xab[2] * Xac[0] - Xab[0] * Xac[2];
  N[2] = Xab[0] * Xac[1] - Xab[1] * Xac[0];
// normalize
  real norm = sqrt(SQR(N[0]) + SQR(N[1]) + SQR(N[2]));
  if (norm > REAL_MIN)
  {
    for (i=0; i<3; i++) N[i] /= norm;
  }
  else
    norm = -1;

  return norm;
}

// swap an edge (face) if the normals of the two new elements are more similar than the two
// existing ones. Only swap if the new diagonal lies inside the elements and both elements belong
// to the same surface patch. Obviously, a boundary face can not be swapped.
bool
swapFace(int f, FILE* infoFile,
	 CompositeSurface & cs,
	 int & numberOfNodes, realArray & nodes,
	 int & numberOfElements, intArray & elements,
	 int & numberOfFaces, intArray & faces,
	 intArray & faceElements,
	 intArray & elementFaces,
	 intArray & elementSurface,
	 intArray & elementCoordinates, 
	 int & numberOfCoordinateNodes,
	 realArray & rNodes )
{
// return code: true: successful swap
//              false: swap illegal or not performed
//  printf("Entering swapFace with face # %i\n", f);
  bool retCode=true;
  int e1=faceElements(f,0), e2=faceElements(f,1);
// boundary face?
  if (e1 < 0)
  {
    printf("swapFace: detected negative faceElements(%i,0)!\n", f);
    return false;
  }
  if (e2 < 0)
  {
//    printf("No swapping of face %i since it is on the boundary\n", f);
    return false;
  }
  
// e1 and e2 on different patches?
  if (elementSurface(e1) != elementSurface(e2))
  {
//    printf("No swapping of face %i since the elemnents lie on different patches\n", f);
    return false;
  }
  

// get face number in elements e1 and e2
  int side1, side2;
  if (elementFaces(e1,0) == f)
    side1 = 0;
  else if (elementFaces(e1,1) == f)
    side1 = 1;
  else if (elementFaces(e1,2) == f)
    side1 = 2;
  else
  {
    printf("Can not find face %i in element e1=%i\n", f, e1);
    throw "Error";
  }
// e2
  if (elementFaces(e2,0) == f)
    side2 = 0;
  else if (elementFaces(e2,1) == f)
    side2 = 1;
  else if (elementFaces(e2,2) == f)
    side2 = 2;
  else
  {
    printf("Can not find face %i in element e2=%i\n", f, e2);
    throw "Error";
  }
  
  int na, nb, nc, nd; 
  nb = elements(e1,side1);
  nc = elements(e1,(side1+1)%3);
  na = elements(e1,(side1+2)%3);
// nd is the node in element e2 NOT on the face `side2'
  nd = elements(e2,(side2+2)%3);

//  printf("na=%i, nb=%i, nc=%i, nd=%i, faces(f,0)=%i, faces(f,1)=%i\n", na, nb, nc, nd, 
//	 faces(f,0), faces(f,1));
  
  
// node numbers in rNodes can be different due to duplication at inter-patch boundaries
  int ma, mb, mc, md;
  mb = elementCoordinates(e1,side1);
  mc = elementCoordinates(e1,(side1+1)%3);
// ma is the node in element e1 NOT on the face `side1'
  ma = elementCoordinates(e1,(side1+2)%3);
// md is the node in element e2 NOT on the face `side2'
  md = elementCoordinates(e2,(side2+2)%3);

// does the new diagonal lie outside elements e1 & e2 (in parameter space?)
  real Rbc[2], Rad[2], Rab[2], Nbc[2], Nad[2];
  int i;
  for (i=0; i<2; i++)
  {
    Rbc[i] = rNodes(mc, i) - rNodes(mb, i);
    Rad[i] = rNodes(md, i) - rNodes(ma, i);
    Rab[i] = rNodes(mb, i) - rNodes(ma, i);
  }
  Nbc[0] = Rbc[1]; Nbc[1] = -Rbc[0];
  Nad[0] = Rad[1]; Nad[1] = -Rad[0];
// intersection coordinates
  real tau, sigma, det;
// Note: Nbc*Rad = Rbc[1]*Rad[0] - Rbc[0]*Rad[1] = Rbc[1]*(-Nad[1]) - Rbc[0]*Nad[0] = -Nad*Rbc
  det = Nbc[0]*Rad[0] + Nbc[1]*Rad[1]; 
  if (fabs(det) > 100*REAL_MIN)
  {
    tau = (Nad[0]*Rab[0] + Nad[1]*Rab[1])/det;
    sigma = (Nbc[0]*Rab[0] + Nbc[1]*Rab[1])/det;
  }
  else
  {
    tau = -99;
    sigma = -99;
  }
// check if the intersection is outside elements e1 & e2?
  if (sigma <= 0. || tau <= 0. || sigma >= 1. || tau >= 1.)
  {
//    printf("No swapping of face %i since the new diagonal would lie outside the elemnents\n", f);
//    printf("sigma=%e, tau=%e\n", sigma, tau);
    return false;
  }
  
  
// ok, the new diagonal lies inside the elements which both lie on the same surface
// we can now compare normals
  real N1[3], N2[3];
  if (elementNormal(na, nb, nc, nodes, N1) < 0) return false; // this indicates an undefined normal
  if (elementNormal(nd, nc, nb, nodes, N2) < 0) return false; // this indicates an undefined normal
  
  real cosN1N2=0.;
  for (i=0; i<3; i++)
    cosN1N2 += N1[i]*N2[i];
  
// need to evaluate the new normals and compare...
  real N1new[3], N2new[3];
  if (elementNormal(na, nb, nd, nodes, N1new) < 0) return false; // this indicates an undefined normal
  if (elementNormal(nd, nc, na, nodes, N2new) < 0) return false; // this indicates an undefined normal
  
  real cosN1N2new=0.;
  for (i=0; i<3; i++)
    cosN1N2new += N1new[i]*N2new[i];

  if (cosN1N2new <= cosN1N2)
  {
//    printf("No swapping of face %i since the new normals are less parallel than the old ones\n", f);
    return false;
  }
  
//  printf("Swapping face %i cosN1N2 = %e, cosN1N2new = %e\n", f, cosN1N2, cosN1N2new);
  int Fab, Fac, Fbd, Fdc;
  Fac = elementFaces(e1, (side1+1)%3);
  Fab = elementFaces(e1, (side1+2)%3);
  Fbd = elementFaces(e2, (side2+1)%3);
  Fdc = elementFaces(e2, (side2+2)%3);

// new face
  faces(f, 0) = na;
  faces(f, 1) = nd;
  
// new element e1
  elementFaces(e1, 0) = Fab;
  elementFaces(e1, 1) = Fbd;
  elementFaces(e1, 2) = f;

  elements(e1,0) = na;
  elements(e1,1) = nb;
  elements(e1,2) = nd;

  elementCoordinates(e1,0) = ma;
  elementCoordinates(e1,1) = mb;
  elementCoordinates(e1,2) = md;

  if (faceElements(Fbd, 0) == e2)
    faceElements(Fbd, 0) = e1;
  else if (faceElements(Fbd, 1) == e2)
    faceElements(Fbd, 1) = e1;
  else
    printf("Warning (error?) can not find element e2=%i in faceElements, Fbd=%i\n", e2, Fbd);

// new element e2
  elementFaces(e2, 0) = Fdc;
  elementFaces(e2, 1) = Fac;
  elementFaces(e2, 2) = f;
  
  elements(e2,0) = nd;
  elements(e2,1) = nc;
  elements(e2,2) = na;

  elementCoordinates(e2,0) = md;
  elementCoordinates(e2,1) = mc;
  elementCoordinates(e2,2) = ma;

  if (faceElements(Fac, 0) == e1)
    faceElements(Fac, 0) = e2;
  else if (faceElements(Fac, 1) == e1)
    faceElements(Fac, 1) = e2;
  else
    printf("Warning (error?) can not find element e1=%i in faceElements, Fac=%i\n", e1, Fac);
  
// check normals
//    real N1chk[3], N2chk[3];
//    if ((elementNormal(elements(e1,0), elements(e1,1), elements(e1,2), nodes, N1chk) > 0) &&
//        (elementNormal(elements(e2,0), elements(e2,1), elements(e2,2), nodes, N2chk) > 0))
//    {
//      real cosN1N2chk=0.;
//      for (i=0; i<3; i++)
//        cosN1N2chk += N1chk[i]*N2chk[i];
//      printf("Check: cosN1N2new=%e, cosN1N2chk=%e\n", cosN1N2new, cosN1N2chk);
//  }
  
  
  return retCode;
}


static real
maxAngle(real *Ra, real *Rb, real *Rc)
{
  real cosA=0., cosB=0., cosC=0., RabNorm=0., RacNorm=0., RbcNorm=0.;
  int i;
  for (i=0; i<2; i++)
  {
    RabNorm += SQR(Rb[i]-Ra[i]);
    RacNorm += SQR(Rc[i]-Ra[i]);
    RbcNorm += SQR(Rc[i]-Rb[i]);
    
    cosA += (Rb[i]-Ra[i])*(Rc[i]-Ra[i]);
    cosB += (Ra[i]-Rb[i])*(Rc[i]-Rb[i]);
    cosC += (Ra[i]-Rc[i])*(Rb[i]-Rc[i]);
  }
  RabNorm = sqrt(RabNorm);
  RacNorm = sqrt(RacNorm);
  RbcNorm = sqrt(RbcNorm);

  if (RabNorm < 10*REAL_MIN || RacNorm < 10*REAL_MIN || RbcNorm < 10*REAL_MIN)
  {
    printf("Zero face in maxAngle!\n");
    return -1.;
  }
  
  cosA /= (RabNorm*RacNorm);
  cosB /= (RabNorm*RbcNorm);
  cosC /= (RacNorm*RbcNorm);
  
  real minCos=cosA;
  minCos = min(minCos, cosB);
  minCos = min(minCos, cosC);

  return minCos;
}



// refine by splitting a face. The adjacent elements will be splitted
int 
splitFace2(int f, FILE* infoFile,
	   realArray & r, realArray & x,
	   CompositeSurface & cs,
	   int & numberOfNodes, realArray & nodes,
	   int & numberOfElements, intArray & elements,
	   int & numberOfFaces, intArray & faces,
	   intArray & faceElements,
	   intArray & elementFaces,
	   intArray & elementSurface,
	   intArray & elementCoordinates, 
	   int & numberOfCoordinateNodes,
	   realArray & rNodes )
{
  int splitSide, e = faceElements(f,0), e2 = faceElements(f,1);

// tmp
  if (infoFile)
  {
    fprintf(infoFile, "Entering splitFace2 to split face %i connecting nodes (%i, %i) and elements %i and %i\n", 
	    f, faces(f,0), faces(f,1), e, e2);
  }
  
  Range R2=2, R3=3;

  int mv[3];
  mv[0]=elementCoordinates(e,0), mv[1]=elementCoordinates(e,1), mv[2]=elementCoordinates(e,2);
  
// which face of element e does f correspond to?
  if (elementFaces(e,0) == f)
    splitSide = 0;
  else if (elementFaces(e,1) == f)
    splitSide = 1;
  else if (elementFaces(e,2) == f)
    splitSide = 2;
  else
  {
    printf("Unable to find face %i in elementFaces(%i): (%i, %i, %i)\n", f, e,
	   elementFaces(e,0), elementFaces(e,1), elementFaces(e,1));
    exit(-1);
  }
  // split face splitSide

  //                /|mb
  //          n   /  |
  //            X    |
  //          /      |
  //        /        |
  //       -----------ma
  //       mc

  // order nodes starting at the point opposite the longest face.
  // (ma,mb,mc) permutation of (0,1,2)
  // (na,nb,nc) : actual node numbers corresponding to (ma,mb,mc)
     
  int ma=(splitSide+2)%3, mb=splitSide, mc=(splitSide+1)%3;
  int na=elements(e,ma), nb=elements(e,mb), nc=elements(e,mc);
      
// these are the faces to keep
  int fa = elementFaces(e,ma);
  int fc = elementFaces(e,mc);

  // determine ordering of nodes on e2
  int splitSide2=0, ma2=0,mb2=1,mc2=2, na2=0,nb2=1,nc2=2, fa2=0,fc2=1;
  if( e2>=0 )
  {
// which face of element e2 does f correspond to?
    if( elementFaces(e2,0)==f )
      splitSide2=0;
    else if( elementFaces(e2,1)==f )
      splitSide2=1;
    else if( elementFaces(e2,2)==f )
      splitSide2=2;
    else
    {
      throw "error";
    }
    ma2=(splitSide2+2)%3, mb2=splitSide2, mc2=(splitSide2+1)%3;
    na2=elements(e2,ma2), nb2=elements(e2,mb2), nc2=elements(e2,mc2);
// nb2 = nc, nc2 = nb

    fa2 = elementFaces(e2,ma2);
    fc2 = elementFaces(e2,mc2);
  }
      
// r and x could be passed into this routine since they are already computed
// parameter coordinate for the new node. Disregard coordinate signularities
    int dir;
//      for (dir=0; dir<2; dir++)
//      {
//        if (rNodes(mv[mb],dir) < -.5)
//  	r(0,dir) = rNodes(mv[mc],dir);
//        else if (rNodes(mv[mc],dir) < -.5)
//  	r(0,dir) = rNodes(mv[mb],dir);
//        else
//  	r(0,dir) = 0.5*(rNodes(mv[mb],dir) + rNodes(mv[mc],dir));
//      }
    if (infoFile)
    {
      fprintf(infoFile, "splitSide=%i, ma=%i, mb=%i, mc=%i\n", splitSide, ma, mb, mc);
      fprintf(infoFile, "faces(f,0)=%i, faces(f,1)=%i, elements(ma)=%i, elements(mb)=%i, elements(mc)=%i\n", 
	      faces(f,0), faces(f,1), elements(e,ma), elements(e,mb), elements(e,mc));
      fprintf(infoFile, "nodes(ma)=(%e,%e,%e)\n", nodes(elements(e,ma),0), nodes(elements(e,ma),1), 
	      nodes(elements(e,ma),2));
      fprintf(infoFile, "nodes(mb)=(%e,%e,%e)\n", nodes(elements(e,mb),0), nodes(elements(e,mb),1), 
	      nodes(elements(e,mb),2));
      fprintf(infoFile, "nodes(mc)=(%e,%e,%e)\n", nodes(elements(e,mc),0), nodes(elements(e,mc),1), 
	      nodes(elements(e,mc),2));

      fprintf(infoFile, "elementCoordinates(ma)=%i, elementCoordinates(mb)=%i, elementCoordinates(mc)=%i\n"
	      "mv[ma]=%i, mv[mb]=%i, mv[mc]=%i\n",
	      elementCoordinates(e,ma), elementCoordinates(e,mb), elementCoordinates(e,mc), mv[ma], mv[mb], mv[mc]);
      
      fprintf(infoFile, "rNodes(mv[ma])=(%e,%e), rNodes(mv[mb])=(%e,%e) and rNodes(mv[mc])=(%e,%e)\n",
	      rNodes(mv[ma],0), rNodes(mv[ma],1), 
	      rNodes(mv[mb],0), rNodes(mv[mb],1), 
	      rNodes(mv[mc],0), rNodes(mv[mc],1));
      
      realArray rTest(3,2), xTest(3,3);
      int surf=elementSurface(e);
      fprintf(infoFile, "surface number %i\n", surf);
      assert( surf>=0 && surf<cs.numberOfSubSurfaces() );
      Mapping & surface = cs[surf];
    
      rTest(0,R2) = rNodes(mv[ma],R2);
      rTest(1,R2) = rNodes(mv[mb],R2);
      rTest(2,R2) = rNodes(mv[mc],R2);
      
      surface.map(rTest,xTest);
      fprintf(infoFile, "x(mv[ma])=(%e,%e,%e)\n", xTest(0,0), xTest(0,1), xTest(0,2));
      fprintf(infoFile, "x(mv[mb])=(%e,%e,%e)\n", xTest(1,0), xTest(1,1), xTest(1,2));
      fprintf(infoFile, "x(mv[mc])=(%e,%e,%e)\n", xTest(2,0), xTest(2,1), xTest(2,2));
    }

// check the biggest angle in the 4 new elements. Cancel the face split operation if it is too large.
  if( e2>=0 && elementSurface(e2)==elementSurface(e) )
  {
// the new triangles are (both live on the same patch)
// {ma, mb, new}, {ma,new,mc}, {mb, ma2, new}, {ma2, mc, new}
    real Ra[2], Rb[2], Rc[2], Rd[2], Rn[2];
    int qq;
    for (qq=0; qq<2; qq++)
    {
      Ra[qq] = rNodes(elementCoordinates(e,ma), qq);
      Rb[qq] = rNodes(elementCoordinates(e,mb), qq);
      Rc[qq] = rNodes(elementCoordinates(e,mc), qq);
      Rd[qq] = rNodes(elementCoordinates(e2,ma2), qq);
      Rn[qq] = r(f, qq);
    }
    real cosOrig[2];
    cosOrig[0] = maxAngle(Ra, Rb, Rc);
    if (cosOrig[0] < -0.999)
    {
      printf("Very wide angle detected in triangle ma=%i, mb=%i, mc=%i\n", elementCoordinates(e,ma), 
	     elementCoordinates(e,mb), elementCoordinates(e,mc));
      printf("Ra=(%e,%e), Rb=(%e,%e), Rc=(%e,%e)\n", Ra[0], Ra[1], Rb[0], Rb[1], Rc[0], Rc[1]);
    }
    
    cosOrig[1] = maxAngle(Rb, Rd, Rc);
    if (cosOrig[1] < -0.999)
    {
      printf("Very wide angle detected in triangle mb=%i, md=%i, mc=%i, \n", elementCoordinates(e,mb), 
	     elementCoordinates(e2,ma2), elementCoordinates(e,mc));
      printf("Rb=(%e,%e), Rd=(%e,%e), Rc=(%e,%e)\n", Rb[0], Rb[1], Rd[0], Rd[1], Rc[0], Rc[1]);
    }
    real cosOrigMin = min(cosOrig[0], cosOrig[1]); // obtuse angles have cos < 0
    
    real cosNew[4];
    cosNew[0] = maxAngle(Ra, Rb, Rn);
    cosNew[1] = maxAngle(Ra, Rn, Rc);
    cosNew[2] = maxAngle(Rb, Rd, Rn);
    cosNew[3] = maxAngle(Rd, Rc, Rn);
    real cosNewMin=cosNew[0];
    for (qq=1; qq<4; qq++)
      cosNewMin = min(cosNewMin, cosNew[qq]);
    
    if (cosNewMin <= -0.9)
    {
      printf("Not splitting face %i since it would create at least one triangles with a very obtuse (r,s) angle\n", f);
      printf("cosOrigMin=%e, cosNewMin=%e\n", cosOrigMin, cosNewMin);
      return 1;
    }
    
//    printf("Splitting face %i, cosOrigMin=%e, cosNewMin=%e\n", f, cosOrigMin, cosNewMin);
    
  }
      
      
  int newNode=numberOfNodes;
  nodes(newNode,R3)=x(f,R3);
  numberOfNodes++;

  int f2=numberOfFaces;
  int f3=f2+1, f4=f3+1;   // new face numbers

  int e3=numberOfElements;  // new elements
  int e4=e3+1;
  if( e2<0 )
    e4=-1;

  faces(f,0)=nb;   // change longest face
  faces(f,1)=newNode;

  faces(f2,0)=newNode;
  faces(f2,1)=nc;

  faces(f3,0)=na;
  faces(f3,1)=newNode;

  elements(e,mc)=newNode;

  elements(e3,0)=na;
  elements(e3,1)=newNode;
  elements(e3,2)=nc;

  elementSurface(e3)=elementSurface(e);
      
  elementFaces(e3,0)=f3;
  elementFaces(e3,1)=f2;
  elementFaces(e3,2)=fc;
      
  elementFaces(e,mc)=f3;

  // change faceElements
  faceElements(f2,0)=e3;
  faceElements(f2,1)=e4;

  faceElements(f3,0)=e; 
  faceElements(f3,1)=e3;

  if( faceElements(fc,0)==e )
    faceElements(fc,0)=e3;
  else
    faceElements(fc,1)=e3;

// add entries into rNodes. We may have to add two new r-coordinates if e2 lies on a different sub-surface
  int n=  numberOfCoordinateNodes;
  rNodes(n,R2)=r(f,R2);
  int n2=n;
  numberOfCoordinateNodes++;
  if( e2>=0 && elementSurface(e2)!=elementSurface(e) ) // used to test elementSurface(e4) which isn't assigned
  {
    n2=  numberOfCoordinateNodes;
    int mm0=elementCoordinates(e2,mb2), mm1=elementCoordinates(e2,mc2); 
// get initial guess for parameter coordinate by averaging
// parameter coordinate for the new node. Disregard the parameter coordinate for points on surface singularities
    for (dir=0; dir<2; dir++)
    {
      if (rNodes(mm0,dir) < -.5)
 	rNodes(n2,dir) = rNodes(mm1,dir);
      else if (rNodes(mm1,dir) < -.5)
 	rNodes(n2,dir) = rNodes(mm0,dir);
      else
 	rNodes(n2,dir) = 0.5*(rNodes(mm0,dir) + rNodes(mm1,dir));
    }
    int s2=elementSurface(e2);
    const bool isTrimmedMapping = cs[s2].getClassName()=="TrimmedMapping";
    Mapping &surface2 = (!isTrimmedMapping) ? cs[s2] : *((TrimmedMapping&)cs[s2]).untrimmedSurface();
    realArray x2(1,3), r2(1,2);
    x2(0,R3) = x(f,R3);
    r2(0,R2) = rNodes(n2, R2); // initial guess
    surface2.inverseMap(x2,r2);
    rNodes(n2, R2) = r2(0,R2); // store the projected point
    
// tmp
    if (infoFile)
    {
      fprintf(infoFile, "Adding rNodes=(%e,%e) between 2 patches by interpolating (%e,%e) and (%e,%e)\n",
	     rNodes(n2,0), rNodes(n2,1), rNodes(mm0,0), rNodes(mm0,1), rNodes(mm1,0), rNodes(mm1,1));
    }
    
    
    numberOfCoordinateNodes++;
  }
      
  elementCoordinates(e3,0)=elementCoordinates(e,ma);
  elementCoordinates(e3,1)=n;
  elementCoordinates(e3,2)=elementCoordinates(e,mc);


  if( e2>=0 )
  {
    elements(e2,mb2)=newNode;

    faces(f4,0)=newNode;
    faces(f4,1)=na2;

    elements(e4,0)=na2;
    elements(e4,1)=nb2;
    elements(e4,2)=newNode;

    elementSurface(e4)=elementSurface(e2);

    elementFaces(e4,0)=fa2;
    elementFaces(e4,1)=f2;
    elementFaces(e4,2)=f4;

    elementFaces(e2,ma2)=f4;

    faceElements(f4,0)=e2;
    faceElements(f4,1)=e4;

    if( faceElements(fa2,0)==e2 )
      faceElements(fa2,0)=e4;
    else
      faceElements(fa2,1)=e4;

    elementCoordinates(e4,0)=elementCoordinates(e2,ma2);
    elementCoordinates(e4,1)=elementCoordinates(e2,mb2);
    elementCoordinates(e4,2)=n2;

  }
      
  // do these last
  elementCoordinates(e,mc)=n;
  elementCoordinates(e2,mb2)=n2;
      

  if( e2>=0 )
  {
    numberOfFaces+=3;
    numberOfElements+=2;
  }
  else
  {
    numberOfFaces+=2; 
    numberOfElements+=1;
  }

// tmp
  if (infoFile)
  {
    fprintf(infoFile,"Exiting splitFace2, New node %i, new elements %i and %i, splitSide=%i, splitSide2=%i.\n", 
	    newNode, e3, e4, splitSide, splitSide2);
    fprintf(infoFile,"face %i now connects nodes (%i, %i) and elements %i and %i\n", 
	    f, faces(f,0), faces(f,1), faceElements(f,0), faceElements(f,1));
    fprintf(infoFile,"New face %i now connects nodes (%i, %i) and elements %i and %i\n", 
	    f2, faces(f2,0), faces(f2,1), faceElements(f2,0), faceElements(f2,1));
    fprintf(infoFile,"New face %i now connects nodes (%i, %i) and elements %i and %i\n", 
	    f3, faces(f3,0), faces(f3,1), faceElements(f3,0), faceElements(f3,1));
    if (e2 >= 0)
    {
      fprintf(infoFile,"New face %i now connects nodes (%i, %i) and elements %i and %i\n", 
	      f4, faces(f4,0), faces(f4,1), faceElements(f4,0), faceElements(f4,1));
      fprintf(infoFile,"This was an interior face\n");
    }
    else
      fprintf(infoFile,"This was a boundary face\n");
    fprintf(infoFile,"Element %i connects nodes (%i, %i, %i)\n", e, elements(e,0), elements(e,1), elements(e,2));
    fprintf(infoFile,"Element %i connects nodes (%i, %i, %i)\n", e3, elements(e3,0), elements(e3,1), elements(e3,2));
    if (e2 >= 0)
    {
      fprintf(infoFile,"Element %i connects nodes (%i, %i, %i)\n", e2, elements(e2,0), elements(e2,1), elements(e2,2));
      fprintf(infoFile,"Element %i connects nodes (%i, %i, %i)\n", e4, elements(e4,0), elements(e4,1), elements(e4,2));
    }
    fprintf(infoFile,"\n");
  }
  
  return 0;
  
}

// ==================================================================================================
//! Refine an element by splitting it at a face. The adjacent element is also split.  
// ==================================================================================================
int 
splitFace(int e, int side,
	  CompositeSurface & cs,
	  int & numberOfNodes, realArray & nodes,
	  int & numberOfElements, intArray & elements,
	  int & numberOfFaces, intArray & faces,
	  intArray & faceElements,
	  intArray & elementFaces,
	  intArray & elementSurface,
	  intArray & elementCoordinates, 
	  int & numberOfCoordinateNodes,
	  realArray & rNodes )
{

  int longSide=side;

  realArray r(1,2),x(1,3);
  Range R2=2, R3=3;

  int mv[3], &m0=mv[0], &m1=mv[1], &m2=mv[2];
  m0=elementCoordinates(e,0), m1=elementCoordinates(e,1), m2=elementCoordinates(e,2);
  
  // split face longSide

  //                /|mb
  //          n   /  |
  //            X    |
  //          /      |
  //        /        |
  //       -----------ma
  //       mc

  const int f = elementFaces(e,longSide);  // longest face
  assert( f>=0 && f<numberOfFaces );
  // order nodes starting at the point opposite the longest face.
  // (ma,mb,mc) permutation of (0,1,2)
  // (na,nb,nc) : actual node numbers corresponding to (ma,mb,mc)
     
  int ma=(longSide+2)%3, mb=longSide, mc=(longSide+1)%3;
  int na=elements(e,ma), nb=elements(e,mb), nc=elements(e,mc);
      
// these are the faces to keep
  int fa = elementFaces(e,ma);
  int fc = elementFaces(e,mc);

  int e2;   // adjacent element (opposite longest face)
  if( faceElements(f,0)==e )
    e2=faceElements(f,1);
  else if( faceElements(f,1)==e )
    e2=faceElements(f,0);
  else
  {
    throw "error";
  }

  // determine ordering of nodes on e2
  int longSide2=0, ma2=0,mb2=1,mc2=2, na2=0,nb2=1,nc2=2, fa2=0,fc2=1;
  if( e2>=0 )
  {
    if( elementFaces(e2,0)==f )
      longSide2=0;
    else if( elementFaces(e2,1)==f )
      longSide2=1;
    else if( elementFaces(e2,2)==f )
      longSide2=2;
    else
    {
      throw "error";
    }
    ma2=(longSide2+2)%3, mb2=longSide2, mc2=(longSide2+1)%3;
    na2=elements(e2,ma2), nb2=elements(e2,mb2), nc2=elements(e2,mc2);
// nb2 = nc, nc2 = nb

    fa2 = elementFaces(e2,ma2);
    fc2 = elementFaces(e2,mc2);
  }
      
// r and x could be passed into this routine since they are already computed
  r(0,R2)= .5*(rNodes(mv[mb],R2)+rNodes(mv[mc],R2));   // r values at midpoint of long-face
  int surf=elementSurface(e);
  assert( surf>=0 && surf<cs.numberOfSubSurfaces() );
  Mapping & surface = cs[surf];
    
  surface.map(r,x);
      
      
  int newNode=numberOfNodes;
  nodes(newNode,R3)=x(0,R3);
  numberOfNodes++;

  int f2=numberOfFaces;
  int f3=f2+1, f4=f3+1;   // new face numbers

  int e3=numberOfElements;  // new elements
  int e4=e3+1;
  if( e2<0 )
    e4=-1;

  faces(f,0)=nb;   // change longest face
  faces(f,1)=newNode;

  faces(f2,0)=newNode;
  faces(f2,1)=nc;

  faces(f3,0)=na;
  faces(f3,1)=newNode;

  elements(e,mc)=newNode;

  elements(e3,0)=na;
  elements(e3,1)=newNode;
  elements(e3,2)=nc;

  elementSurface(e3)=elementSurface(e);
      
  elementFaces(e3,0)=f3;
  elementFaces(e3,1)=f2;
  elementFaces(e3,2)=fc;
      
  elementFaces(e,mc)=f3;

  // change faceElements
  faceElements(f2,0)=e3;
  faceElements(f2,1)=e4;

  faceElements(f3,0)=e; 
  faceElements(f3,1)=e3;

  if( faceElements(fc,0)==e )
    faceElements(fc,0)=e3;
  else
    faceElements(fc,1)=e3;

      // add entries into rNodes. We may have to add two new r-coordinates if e2 lies on a different sub-surface
  int n=  numberOfCoordinateNodes;
  rNodes(n,R2)=r(0,R2);
  int n2=n;
  numberOfCoordinateNodes++;
  if( e2>=0 && elementSurface(e4)!=elementSurface(e) )
  {
    n2=  numberOfCoordinateNodes;
    int mm0=elementCoordinates(e2,mb2), mm1=elementCoordinates(e2,mc2); // *** should invert mapping instead??
    rNodes(n2,R2)=.5*(rNodes(mm0,R2)+rNodes(mm1,R2));  
    numberOfCoordinateNodes++;
  }
      
  elementCoordinates(e3,0)=elementCoordinates(e,ma);
  elementCoordinates(e3,1)=n;
  elementCoordinates(e3,2)=elementCoordinates(e,mc);


  if( e2>=0 )
  {
    elements(e2,mb2)=newNode;

    faces(f4,0)=newNode;
    faces(f4,1)=na2;

    elements(e4,0)=na2;
    elements(e4,1)=nb2;
    elements(e4,2)=newNode;

    elementSurface(e4)=elementSurface(e2);

    elementFaces(e4,0)=fa2;
    elementFaces(e4,1)=f2;
    elementFaces(e4,2)=f4;

    elementFaces(e2,ma2)=f4;

    faceElements(f4,0)=e2;
    faceElements(f4,1)=e4;

    if( faceElements(fa2,0)==e2 )
      faceElements(fa2,0)=e4;
    else
      faceElements(fa2,1)=e4;

    elementCoordinates(e4,0)=elementCoordinates(e2,ma2);
    elementCoordinates(e4,1)=elementCoordinates(e2,mb2);
    elementCoordinates(e4,2)=n2;

  }
      
  // do these last
  elementCoordinates(e,mc)=n;
  elementCoordinates(e2,mb2)=n2;
      

  if( e2>=0 )
  {
    numberOfFaces+=3;
    numberOfElements+=2;
  }
  else
  {
    numberOfFaces+=2; 
    numberOfElements+=1;
  }

  return 0;
  
}

// ==================================================================================================
//! Refine an element by splitting it at the centroid.
// ==================================================================================================
int 
splitElement(int e, 
	     CompositeSurface & cs,
	     int & numberOfNodes, realArray & nodes,
	     int & numberOfElements, intArray & elements,
	     int & numberOfFaces, intArray & faces,
	     intArray & faceElements,
	     intArray & elementFaces,
	     intArray & elementSurface,
	     intArray & elementCoordinates, 
	     int & numberOfCoordinateNodes,
	     realArray & rNodes )
{

  int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);

  const int f0 = elementFaces(e,0), f1 = elementFaces(e,1), f2 = elementFaces(e,2);
  assert( f0>=0 && f0<numberOfFaces && f1>=0 && f1<numberOfFaces && f2>=0 && f2<numberOfFaces );

  int mv[3], &m0=mv[0], &m1=mv[1], &m2=mv[2];
  m0=elementCoordinates(e,0), m1=elementCoordinates(e,1), m2=elementCoordinates(e,2);

  realArray r(1,2),x(1,3);
  Range R2=2, R3=3;

  r(0,R2)= (rNodes(m0,R2)+rNodes(m1,R2)+rNodes(m2,R2))/3.;   // r values at centroid of parameter space
  int surf=elementSurface(e);
  assert( surf>=0 && surf<cs.numberOfSubSurfaces() );
  Mapping & surface = cs[surf];
    
  surface.map(r,x);
      
  int n3=numberOfNodes;
  nodes(n3,R3)=x(0,R3);
  numberOfNodes++;

  int m3 =  numberOfCoordinateNodes;
  rNodes(m3,R2)=r(0,R2);
  numberOfCoordinateNodes++;

  // neighbouring elements
  int en0 = faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
  int en1 = faceElements(f1,0)==e ? faceElements(f1,1) : faceElements(f1,0);
  int en2 = faceElements(f2,0)==e ? faceElements(f2,1) : faceElements(f2,0);
  

  int e1=numberOfElements, e2=e1+1;
  numberOfElements+=2;
  
  int f3=numberOfFaces, f4=f3+1, f5=f4+1;
  numberOfFaces+=3;
  
  elements(e,2)=n3;
  elementCoordinates(e,2)=m3;
  elementFaces(e,0)=f0;
  elementFaces(e,1)=f4;
  elementFaces(e,2)=f3;
  
  elements(e1,0)=n1;
  elements(e1,1)=n2;
  elements(e1,2)=n3;
  elementCoordinates(e1,0)=m1;
  elementCoordinates(e1,1)=m2;
  elementCoordinates(e1,2)=m3;
  elementFaces(e1,0)=f1;
  elementFaces(e1,1)=f5;
  elementFaces(e1,2)=f4;
  elementSurface(e1)=elementSurface(e);
  
  elements(e2,0)=n2;
  elements(e2,1)=n0;
  elements(e2,2)=n3;
  elementCoordinates(e2,0)=m2;
  elementCoordinates(e2,1)=m0;
  elementCoordinates(e2,2)=m3;
  elementFaces(e2,0)=f2;
  elementFaces(e2,1)=f3;
  elementFaces(e2,2)=f5;
  elementSurface(e2)=elementSurface(e);

  faces(f3,0)=n0;
  faces(f3,1)=n3;
  faceElements(f3,0)=e;
  faceElements(f3,1)=e2;
  
  faces(f4,0)=n1;
  faces(f4,1)=n3;
  faceElements(f4,0)=e;
  faceElements(f4,1)=e1;
  
  faces(f5,0)=n2;
  faces(f5,1)=n3;
  faceElements(f5,0)=e1;
  faceElements(f5,1)=e2;
  

  faceElements(f0,0)=e;
  faceElements(f0,1)=en0;

  faceElements(f1,0)=e1;
  faceElements(f1,1)=en1;

  faceElements(f2,0)=e2;
  faceElements(f2,1)=en2;


  return 0;
  
}



// ===================================================================================================
//! Improve the quality of a triangulation by inserting new nodes.
/* 
   \param cs (input) : triangulation lies on this composite surface
   
   \param elementCoordinates (input/ouput) :  pointers into rNodes, parameter space coordinates of the nodes.
   \param rNodes (input/output) : parameter space coordinates of the nodes (include duplicate nodes)
 */
// ===================================================================================================
int
improveTriangulation( CompositeSurface & cs, real maxDist, real maxArea,
		      int & numberOfNodes, realArray & nodes,
		      int & numberOfElements, intArray & elements,
		      int & numberOfFaces, intArray & faces,
		      intArray & faceElements,
		      intArray & elementFaces,
		      intArray & elementSurface,
		      intArray & elementCoordinates, 
                      int & numberOfCoordinateNodes,
		      realArray & rNodes)
{
  int debug=0;
  // real maxArea = maximumArea < REAL_MAX ? maximumArea : .05; // , dist; // hardwired for now

  int e, f;
  real x0[3],x1[3],x2[3];
  
  Range R2=2, R3=3;
  
  int numberOfElementsAdded=0, longSide;
  int initialNumberOfElements=numberOfElements;
  bool splitted;
  
  FILE * infoFile = NULL;
  if (debug)
    infoFile = fopen("split.txt", "w");
  
  int numberOfFacesSplitted=0;
  real time1=getCPU(), startTime, mapTime=0;
  int endFace;
  realArray x, rp, xp, rtmp, xtmp;
// keep track of faces that get split
  int evaluateDimension=2*numberOfFaces;
  intArray evaluate(evaluateDimension); //reserve some extra space for later...
// initially all faces need to be evaluated
  evaluate = 1;
  
// iterate until no new faces get added
  int iter=0, maxIter=10;
  do
  {
    endFace=numberOfFaces;
    printf("Number of faces: %i\n", numberOfFaces);
    x.redim(numberOfFaces,3);
    rp.redim(numberOfFaces,2);
    xp.redim(numberOfFaces,3);
    
// first collect all points on each surface
    int s;
    for (s=0; s<cs.numberOfSubSurfaces(); s++)
    {
      rtmp.redim(numberOfFaces,2); // make sure there is space for everyone
// tmp for inverseMap
      xtmp.redim(numberOfFaces,3); // make sure there is space for everyone
    
      const bool isTrimmedMapping = cs[s].getClassName()=="TrimmedMapping";
      Mapping &surface = (!isTrimmedMapping) ? cs[s] : *((TrimmedMapping&)cs[s]).untrimmedSurface();

      int nFaces=0;
      for( f=0; f<endFace; f++ )
	if (evaluate(f))
	{
// we need to go through one of the adjacent elements to look up elementCoordinates
// always work with the first element (the second element is absent for boundary faces)
	  e = faceElements(f,0);
// only do surface #s
	  if (elementSurface(e) == s)
	  {
// which face of element e does f correspond to?
	    if (elementFaces(e,0) == f)
	      longSide = 0;
	    else if (elementFaces(e,1) == f)
	      longSide = 1;
	    else if (elementFaces(e,2) == f)
	      longSide = 2;
	    else
	    {
	      printf("Unable to find face %i in elementFaces(%i): (%i, %i, %i)\n", f, e,
		     elementFaces(e,0), elementFaces(e,1), elementFaces(e,1));
	      exit(-1);
	    }
    
    
// split face longSide

//                /|m1
//          n   /  |
//            X    |
//          /      |
//        /        |
//       -----------m0
//       m2	
    int m0=(longSide+2)%3, m1=longSide, m2=(longSide+1)%3;
	    int na=elements(e,m0), nb=elements(e,m1), nc=elements(e,m2);
	    int ma=elementCoordinates(e,m0), mb=elementCoordinates(e,m1), mc=elementCoordinates(e,m2);
      
// face longSide: connecting nodes #nb and #nc
	    x(f,0) = 0.5*(nodes(nb,0) + nodes(nc,0));
	    x(f,1) = 0.5*(nodes(nb,1) + nodes(nc,1));
	    x(f,2) = 0.5*(nodes(nb,2) + nodes(nc,2));
    
// parameter coordinate for the new node. Disregard coordinate singularities
	    int dir;
	    for (dir=0; dir<2; dir++)
	    {
	      if (rNodes(mb,dir) < -.5)
	      {
		printf("detected singular node rNodes(%i,%i)=%e\n", mb, dir, rNodes(mb,dir));
		rtmp(nFaces,dir) = rNodes(mc,dir);
	      }
	      else if (rNodes(mc,dir) < -.5)
	      {
		printf("detected singular node rNodes(%i,%i)=%e\n", mc, dir, rNodes(mc,dir));
		rtmp(nFaces,dir) = rNodes(mb,dir);
	      }
	      else
		rtmp(nFaces,dir) = 0.5*(rNodes(mb,dir) + rNodes(mc,dir));
	    }

	    if (infoFile)
	    {
	      if (rtmp(nFaces,0) < -.5 || rtmp(nFaces,1) < -.5)
	      {
		printf("Created new negative rNode=(%e,%e) by interpolating (%e,%e) and (%e,%e)\n",
		       rtmp(nFaces,0), rtmp(nFaces,1), rNodes(mb,0), rNodes(mb,1), rNodes(mc,0), rNodes(mc,1));
	      }
	    }
// testing inversemap
	    xtmp(nFaces,R3)=x(f,R3);
	    
	    nFaces++;
	  }
	} // end if evaluate(f)...
      
    
// rtmp now contains all parameter coordinates for surface #s
      rtmp.resize(nFaces,2);
      xtmp.resize(nFaces,3);
    
// approximate projection to save CPU time
// map back to get physical coordinate on surface
      startTime = getCPU();
// map all points in the array at once...
// test
      surface.inverseMap(xtmp, rtmp); // use rtmp as initial guess
      surface.map(rtmp, xtmp);
      mapTime += getCPU()-startTime;

// copy xtmp to the right spot in xp
      nFaces=0;
      for( f=0; f<endFace; f++ )
	if (evaluate(f))
	{
// we need to go through one of the adjacent elements to look up elementCoordinates
// always work with the first element (the second element is absent for boundary faces)
	  e = faceElements(f,0);
// only do surface #s
	  if (elementSurface(e) == s)
	  {
	    rp(f,R2) = rtmp(nFaces,R2);
	    xp(f,R3) = xtmp(nFaces,R3);
	    nFaces++;
	  }
	}
    } // end for s=0...cs.numberOfSubSurfaces()
  
    printf("Done evaluating all faces...\n");
  
// now check the midpoint distance and split if that distance exceeds maxDist. Each split will
// generate 1 or 3 new faces.
    real dist;
    for (f=0; f<endFace; f++)
      if (evaluate(f))
      {
	dist = sqrt(SQR(xp(f,0) - x(f,0)) + SQR(xp(f,1) - x(f,1)) + SQR(xp(f,2) - x(f,2)));
// decide if we need to refine face longSide
	if (dist > maxDist)
	{
// refine this element 
//      printf("refine face %i (out of %i faces), distance %e\n", f, numberOfFaces, dist);

// increase array sizes if necessary
	  if( numberOfNodes+1 >=nodes.getLength(0) )
	  {
	    int newNumberOfNodes=numberOfNodes+100;  // how many should we add??
	    nodes.resize(newNumberOfNodes,3);  
	  }
	  if( numberOfCoordinateNodes+2 >= rNodes.getLength(0) )
	  {
	    int newNumberOfCoordinateNodes=numberOfCoordinateNodes+100;  // how many should we add??
	    rNodes.resize(newNumberOfCoordinateNodes,2);
	  }
      
	  if( numberOfElements+2 >=elements.getLength(0) )
	  {
	    int newNumberOfElements=numberOfElements+100;
	    elements.resize(newNumberOfElements,3);
	    elementFaces.resize(newNumberOfElements,3);
	    elementCoordinates.resize(newNumberOfElements,3);
	    elementSurface.resize(newNumberOfElements);
	  }
	  if( numberOfFaces+3 >=faces.getLength(0) )
	  {
	    int newNumberOfFaces=numberOfFaces+100;
	    faces.resize(newNumberOfFaces,2);
	    faceElements.resize(newNumberOfFaces,2);
	  }

// before splitting, figure out candidates for face swapping
	  int swapF[4], nSwap=0, el[2];
	  el[0] = faceElements(f,0); 
	  el[1] = faceElements(f,1); // might be a boundary element with only one face
// should try to swap all faces != f
	  int qq;
	  for (qq=0; qq<2; qq++)
	    if (el[qq] >= 0 )
	    {
	      if (elementFaces(el[qq],0) == f)
	      {
		swapF[nSwap++] = elementFaces(el[qq],1);
		swapF[nSwap++] = elementFaces(el[qq],2);
	      }
	      else if (elementFaces(el[qq],1) == f)
	      {
		swapF[nSwap++] = elementFaces(el[qq],0);
		swapF[nSwap++] = elementFaces(el[qq],2);
	      }
	      else if (elementFaces(el[qq],2) == f)
	      {
		swapF[nSwap++] = elementFaces(el[qq],0);
		swapF[nSwap++] = elementFaces(el[qq],1);
	      }
	      else
	      {
		printf("Warning: could not find face %i in elementFaces for element %i\n", f, el[qq]);
	      }
	    }

	  splitFace2(f, infoFile,
		     rp, xp,
		     cs,
		     numberOfNodes, nodes,
		     numberOfElements,elements,
		     numberOfFaces,faces,
		     faceElements,
		     elementFaces,
		     elementSurface,
		     elementCoordinates, 
		     numberOfCoordinateNodes,
		     rNodes );

// attempt to swap neighboring faces to minimize jump in normal
	  for (qq=0; qq< nSwap; qq++)
	  {
	    swapFace(swapF[qq], infoFile, cs,
		     numberOfNodes, nodes,
		     numberOfElements, elements,
		     numberOfFaces, faces,
		     faceElements,
		     elementFaces,
		     elementSurface,
		     elementCoordinates, 
		     numberOfCoordinateNodes,
		     rNodes );
//	    evaluate(swapF[qq]) = 1; // check this face again
	  }

	  numberOfElementsAdded=numberOfElements-initialNumberOfElements;
	  numberOfFacesSplitted++;
	  evaluate(f) = 1; // check this face again
	}
	else
	{
	  evaluate(f) = 0; // this face doesn't need to be checked again
	}
      } // end splitting (if (evaluate(f)), for f=0...endFace)

// loop through all faces to swap any remaining irregularities
// this triggers a bug somewhere???
//      printf("Looping through all faces again to swap any remaining irregular normals\n");;
//      for (f=0; f<endFace; f++)
//        swapFace(f, infoFile, cs,
//  	       numberOfNodes, nodes,
//  	       numberOfElements, elements,
//  	       numberOfFaces, faces,
//  	       faceElements,
//  	       elementFaces,
//  	       elementSurface,
//  	       elementCoordinates, 
//  	       numberOfCoordinateNodes,
//  	       rNodes );
    
//
// now we need to resize evaluate to include all new faces and initialize all new elements to 1    
//
    if (numberOfFaces > evaluateDimension)
    {
      evaluateDimension = numberOfFaces+1000; // get a few extra elements so we don't have to resize too often
      evaluate.resize(evaluateDimension);
    }
    Range Re(endFace,numberOfFaces-1);
    evaluate(Re)=1;
      
  } while (numberOfFaces > endFace && ++iter < maxIter);
  if (numberOfFaces > endFace)
    printf("WARNING: There are still faces that need refinement. Increase maxIter and re-compile!\n");
  
  
  printf("Final number of faces: %i\n", numberOfFaces);

  if (debug)
    fclose(infoFile);
  
  printf("improveTriangulation: splitted %i faces, totalNumberOfFaces=%i, total CPU time = %e, mapTime = %e\n", 
	 numberOfFacesSplitted, numberOfFaces, getCPU()-time1, mapTime);
//  printf("improveTriangulation: %i elements added\n", numberOfElementsAdded);

  return 0;
}


// ===========================================================================================
//! Add interior nodes to the points given to the triangulation.
// ===========================================================================================
int
addInteriorNodes(int & numberOfNodes, Mapping & surface, real & aspectRatio, 
                 int numberOfGridPoints[2], realArray & rc, const int & debug )
{
  
  // *** add some points that cover the domain ***
//   int numberOfGridPoints[2];
//   bool collapsedEdge[2][3];
//   real averageArclength[2];
//   real elementDensityTolerance=.05;

//   assert( surface!=NULL );
//   real time1=getCPU();
//   surface->determineResolution(numberOfGridPoints,collapsedEdge,averageArclength,elementDensityTolerance );
//   real resolutionTime=getCPU()-time1;

//   int nx=numberOfGridPoints[0], ny=numberOfGridPoints[1];
//   // printf("*** TrimmedMapping: after determine resolution nx=%i ny=%i\n",nx,ny);

  if( false )
  {
    rc.resize(numberOfNodes,2);
    return 0;
  }
  

  int nx=numberOfGridPoints[0];
  int ny=numberOfGridPoints[1];

  if( debug & 2 ) printf("*** addInteriorNodes: aspectRatio = %e, nx=%i ny=%i\n",aspectRatio,nx,ny);

  
  if( false ) // this is already done in determine resolution
  {
    // If the surface was made with a lot of points then keep more
    int nx=max(numberOfGridPoints[0],surface.getGridDimensions(0)/4);
    int ny=max(numberOfGridPoints[1],surface.getGridDimensions(1)/4);

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
    if( debug & 2 ) printf(" addInteriorNodes: New: nx=%i, ny=%i \n",nx,ny);
  }
  
  

  // ** look for nodes on the trimming curves  that are very close to the background grid
  //  const real epsx=.1, epsy=.1;
  // const real epsx=.25, epsy=.25;
  IntegerArray mask(nx,ny);
  mask=1;
  int numberRemoved=0;
  int j=numberOfNodes-1;  // previous node to i
  for( int i=0; i<numberOfNodes; i++ )
  {
    // Check to see where the line segment from point i-1 to i hits the bacground grid
    //
    //     (r0,r1) X-------------------X (s0,s1)
    //  
    real r0 = rc(i,0)*(nx-1), r1=rc(i,1)*(ny-1);
    real s0 = rc(j,0)*(nx-1), s1=rc(j,1)*(ny-1);

    real dr = 1.5+fabs(r0-s0)+fabs(r1-s1);
    int numberOfSubSteps= int(dr);
    for( int k=0; k<numberOfSubSteps; k++ )
    {
      real delta = k/real(numberOfSubSteps);
      real ra=r0+delta*(s0-r0);
      real rb=r1+delta*(s1-r1);
      
      int i0 = int(ra+.5), i1=int(rb+.5);

      // remove the closest point on the background grid
      if( i0>=0 && i0<nx && i1>=0 && i1<ny && mask(i0,i1) )
      {
 	// printf(" Curve face (%8.2e,%8.2e)- (%8.2e,%8.2e) pt (%8.2e,%8.2e) is close to background grid point (%i,%i)\n",
 	//  r0,r1,s0,s1,ra,rb,i0,i1);
	numberRemoved++;
	mask(i0,i1)=0;
      }
    }
    
    j=i;
  }
  

  Range I1=nx, I2=ny, R2=2;
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
    rc.resize(numberOfNodes+nx*ny-numberRemoved,2);
    if( numberRemoved==0 )
    {
      rc(I+numberOfNodes,R2)=r2;
    }
    else
    {
      if( debug & 2 ) 
        printf("*** %i background grid nodes were removed since they were close to trimming curve nodes\n",
             numberRemoved);
      mask.reshape(nx*ny);
      int j=numberOfNodes;
      for( int i=0; i<nx*ny; i++ )
      {
	if( mask(i) )
	{
	  rc(j,0)=r2(i,0); rc(j,1)=r2(i,1);
	  j++;
	}
      }
      assert( j == (numberOfNodes+nx*ny-numberRemoved) );
      numberOfNodes=j;
    }
  }

  return 0;
}



// ==================================================================================================
//!  Build a triangulation on a sub-surface using the merged edge curves.
//
// ==================================================================================================
int CompositeTopology::
buildSubSurfaceTriangulation(int s, 
			     IntegerArray & numberOfBoundaryNodes, 
			     realArray *rCoordinates,
			     IntegerArray *edgeNodeInfop,
			     IntegerArray *boundaryNodeInfop,
			     int & totalNumberOfNodes,
			     int & totalNumberOfFaces,
			     int & totalNumberOfElements,
			     real & totalTimeToBuildSeparateTriangulations,
			     real & totalTriangleTime,
			     real & totalNurbTime,
			     real & totalResolutionTime,
                             int & debug,
			     GenericGraphicsInterface & gi,
			     GraphicsParameters& params)
{
  real times=getCPU();
  int debugs=0;

  FaceInfo & currentFace = faceInfoArray[s];
// remove any unused edges
  int l;
  EdgeInfo *e;
  for (l=0; l<currentFace.numberOfLoops; l++)
  {
    Loop & currentLoop = currentFace.loop[l];
    int sc;
    for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	 sc++, e=e->next )
    {
      if (e->status == EdgeInfo::edgeCurveIsNotUsed)
      {
	printf("Deleting unused edge %i\n", e->edgeNumber);
	currentLoop.removeEdge(e);
	delete e;
      }
    }
  }

// initially, set the number of nodes to zero (in case this triangulation fails)
  numberOfBoundaryNodes(s)=0;

  const int domainDimension=2, rangeDimension=3;
  Mapping & surface = cs[s];
  
  const bool isTrimmedMapping = surface.getClassName()=="TrimmedMapping";
	
// if surface is a TrimmedMapping, we build the triangulation on the underlying reference surface
  Mapping & referenceSurface = !isTrimmedMapping ? surface : *((TrimmedMapping&)surface).untrimmedSurface();

// count numberOfEdgePoints for this face
  int numberOfEdgePoints=0;
  for (l=0; l<currentFace.numberOfLoops; l++)
  {
    Loop & currentLoop = currentFace.loop[l];
    int sc, ne=currentLoop.numberOfEdges();
    for (sc = 0, e = currentLoop.firstEdge; sc<ne; 
	 sc++, e=e->next )
    {
      int numEdge = e->curve->getNURBS()->getGridDimensions(axis1);
      numberOfEdgePoints += numEdge - 1;
// size this array to be used later in the global triangulation
      IntegerArray & edgeNodeInfo = edgeNodeInfop[e->edgeNumber];
      if( edgeNodeInfo.getLength(0)==0 )
      {
//  	printf("buildSubSurfaceTri: dimensioning edgeNodeInfop[%i], numEdge=%i\n", e->edgeNumber, 
//  	       numEdge);
	// the first time we see an edge curve, allocate space in the edgeNodeInfo array
	edgeNodeInfo.redim(numEdge);
	edgeNodeInfo=-1;
      }
    }
  }

  const int maxNumberOfEdgePoints=numberOfEdgePoints; 
//  printf("maxNumberOfEdgePoints=%i\n", maxNumberOfEdgePoints);
  
// boundaryNodeInfo(i,0) : grid point index on the edge curve
// boundaryNodeInfo(i,1) : MASTER (?) edge curve number
  IntegerArray & boundaryNodeInfo = boundaryNodeInfop[s];
  boundaryNodeInfo.redim(maxNumberOfEdgePoints,2);
  boundaryNodeInfo=-1;

  realArray rc(maxNumberOfEdgePoints,2), r;
  intArray faces(maxNumberOfEdgePoints,2);

  realArray holes; // point inside each hole (interior trim-) curve

// we store the trimorientation info in each loop. That way untrimmed and trimmed mappings
// can be treated the same way

  int numberOfInnerCurves=0;
  for (l=0; l<currentFace.numberOfLoops; l++)
  {
    Loop & currentLoop = currentFace.loop[l];
    if ( currentLoop.trimOrientation == -1)
      numberOfInnerCurves++;
  }
//  printf("numberOfInnerCurves: %i\n", numberOfInnerCurves);
  
  if( numberOfInnerCurves>0 )
    holes.redim(numberOfInnerCurves,2);

// compute bounding box and xScale
  ArraySimple<real> bb(2,3);
  real xScale=REAL_MIN*10.;
  for( int dir=0; dir<rangeDimension; dir++ )
  {
    bb(0,dir)=referenceSurface.getRangeBound(Start,dir);
    bb(1,dir)=referenceSurface.getRangeBound(End  ,dir);
    xScale=max(xScale,bb(1,dir)-bb(0,dir));
  }

// fill in rc and holes arrays
  int innerCurve=0;  // counts inner curves
  Range R2=2;
  bool ok=true;
  int trimNumber;
  numberOfEdgePoints=0;
  
// *******************************************
// *** build the nodes for this trim curve ***
// *******************************************

  for (trimNumber=0; trimNumber<currentFace.numberOfLoops && ok; trimNumber++)
  {
    Loop & currentLoop = currentFace.loop[trimNumber];
    
    int edgePointStart=numberOfEdgePoints;
    int nEPL = 0;
    
//    printf("currentLoop.trimOrientation: %i\n", currentLoop.trimOrientation);

// fill in all the coordinates for one loop, then invert the mapping
    realArray xLoop(maxNumberOfEdgePoints,3), xLocal, rLocal, rLocal_test;
    r.redim(maxNumberOfEdgePoints,2);
    Range I, R3=3;
    
    int sc, ne=currentLoop.numberOfEdges();
    for (sc = 0, e = currentLoop.firstEdge; sc<ne && ok; sc++, e=e->next )
    {
      NurbsMapping & edgeMap = *e->curve->getNURBS();

// *** compute the parameter space coordinates by inverting the 3D edge coordinates ****
      realArray g; 
      g = edgeMap.getGrid();

      //            cout<<"curve "<<sc<<" has "<<g.getLength(0)<<" points "<<endl;
      // kkc refine g so that it satisfies the distance tolerances
      if ( improveTri )
	{
	  //	  debug |= 8;
	  //	  debug |= 3;
	  Mapping *m1=NULL,*m2=NULL;

	  m1 = e->master ? &cs[e->master->faceNumber] : &surface;
	  m2 = e->slave ? &cs[e->slave->faceNumber] : NULL;
	  refineCurve( edgeMap, m1, m2, maxDist, maxDist, g);
	  cout<<"refined curve has "<<g.getLength(0)<<" points"<<endl;
	}

      I = g.getLength(0); 
      assert( I.getBase()==0 );
      g.reshape(I,3);
//      printf("Edge %i, orientation = %i\n", e->edgeNumber, e->orientation);
      Range Im=Range(I.getBase(),I.getBound()-1);

// invert mapping to get parameter coordinates
      rLocal.redim(I,2);
      rLocal=-1.;
      rLocal_test.redim(I,2);
      rLocal_test=-1;
      referenceSurface.inverseMap(g, rLocal);

// check the inversion before the periodic correction -- some points can be wrong if the parameterization is bad
      xLocal.redim(I,3);
      
      //kkc 040217      referenceSurface.map(rLocal, xLocal);
      //kkc        project back onto the actual (trimmed) surface to perform the test
      surface.map(rLocal, xLocal);

      real err = max(fabs(g-xLocal));

      real rerr = 0;
      if ( false &&  // *wdh* 050513 -- do not do this test since it builds the quad tree
           isTrimmedMapping )
	{ // check the inversion on the trimmed mapping, if the underlying surface
	  // is bad or periodic a correction may be needed
	  surface.inverseMap(xLocal, rLocal_test);
	  rerr = max(fabs(rLocal-rLocal_test));
	  err = max(err,rerr);
	}

      //      printf("Max mapping error before periodic correction: %e\n", err);
    //    Overture::getGraphicsInterface()->plotPoints(rLocal);

      //      rLocal.display("rLocal");
      if( err > xScale*1.e-2 ) // AP increased to 1.e-2 from 1.e-3 (feeling brave today)
      {
	printf("**WARNING** errors inverting loop %i, sub-curve %i onto surface s=%i, "
	       " err=%8.2e, rel.err=%8.2e .. will try to fix...\n",
	       currentLoop.firstEdge->loopNumber, sc, s, err, err/xScale);
	int k;
	if( debug & 2 )
	{
	  for( k=0; k<=I.getBound(); k++ )
	  {
	    printf("BEFORE: k=%i g=(%8.3e,%8.3e,%8.3e) r=(%8.3e,%8.3e) \n",k,g(k,0),g(k,1),g(k,2),
		   rLocal(k,0),rLocal(k,1));
	  }
	}
// fix the problem by projecting onto trimming curves in the parameter plane
	if ((e->curve->surfaceNumber == s && e->curve->subCurve != NULL) || 
	    (e->initialCurve->surfaceNumber == s && e->initialCurve->subCurve != NULL))
	{
	  printf("Trying to fix inversion problem by projecting onto trim curves, \n"
		 "s=%i, curve->surfaceNumber=%i, initialCurve->surfaceNumber=%i\n", s, 
		 e->curve->surfaceNumber, e->initialCurve->surfaceNumber);
// either use e->curve or e->initialCurve
	  CurveSegment *curve;
	  if (e->curve->surfaceNumber == s && e->curve->subCurve != NULL)
	  {
	    curve = e->curve;
	  }
	  else
	  {
	    curve = e->initialCurve;
	  }
// both the trimmed and un-trimmed cases should have subCurves in parameter space!
	  if (curve->subCurve != NULL)
	  {
//  	    assert(isTrimmedMapping);
//  	    TrimmedMapping & trim = (TrimmedMapping&)surface;
// this is the (2D) curve in the parameter plane
//	    NurbsMapping & trimCurve = * curve->surfaceLoop;
//	    NurbsMapping & subCurve = * curve->subCurve;
	    
// project g onto curve -> r1, use the trimCurve to map r1 -> rLocal
	    realArray r1(I,1);
	    curve->getNURBS()->inverseMap(g, r1);
	    curve->subCurve->map(r1, rLocal);

	    referenceSurface.map(rLocal, xLocal);
	    real err = max(fabs(g-xLocal));
	    printf("**INFO** after fixing: max err inverting loop %i onto surface s=%i, "
		   " err=%8.2e, rel.err=%8.2e\n",
		   currentLoop.firstEdge->loopNumber, s, err, err/xScale);
	  }
// untrimmed case: this is a boundary curve
	  else
	  {
	    printf("***ERROR*** Can't fix the inversion since the curve doesn't have a counterpart in parameter space,\n"
		   "ANDERS, GET YOUR ACT TOGETHER!\n");
//  	    int sc = curve->boundaryCurve;
	    
//  	    const int axis = (sc+1) % 2;
//  	    const int side = ((sc+1)/2) %2;
//  	    printf("The untrimmed case: sc = %i, axis = %i, side = %i\n", sc, axis, side);
//  	    realArray r1(I,1);
//  	    curve->getNURBS()->inverseMap(g, r1);
//  	    int axisp1 = (axis+1) %2;
//  	    rLocal(I,axis)=(real)side;
//  	    if( sc<2 )
//  	    {
//  	      rLocal(I,axisp1) = r1(I,0);
//  	    }
//  	    else
//  	    {
//  	      rLocal(I,axisp1) = 1. - r1(I,0);
//  	    }
	  }
	  
	}
	else
	{
	  printf("***ERROR*** Can't fix the inversion since the curve doesn't have a counterpart in parameter space,\n"
		 "i.e., it is the result of a split or join operation!\n ANDERS, GET YOUR ACT TOGETHER!\n");
	}
      } // end if err > 1.e-2*xScale

      ok=true; //  let go by default false;
// copy the information to the global arrays

      if ( improveTri )
      {
	// kkc the size of xLoop may need to be increased
	if ( xLoop.getLength(0) < (I.length()+nEPL) )
	{
	  xLoop.resize(nEPL + 2*I.length(),3); // multiply by 2 to buffer for the next 
	  r.resize(nEPL + 2*I.length(),2);
	  int oldSize = boundaryNodeInfo.getLength(0);
	  boundaryNodeInfo.resize(nEPL + 2*I.length(),2);
	  boundaryNodeInfo(Range(oldSize,boundaryNodeInfo.getLength(0)-1),Range(2)) = -1;
	      
	  rc.resize(nEPL + 2*I.length(),2);
	  faces.resize(nEPL + 2*I.length(),2);
	}

	if ( edgeNodeInfop[e->edgeNumber].getLength(0)<I.length() )
	{
	  // *wdh* 031124 edgeNodeInfop[e->edgeNumber].resize(I,2);
	  edgeNodeInfop[e->edgeNumber].resize(I);
	  edgeNodeInfop[e->edgeNumber] = -1;
	}

      }

      if (e->orientation == 1)
      {
// copy all but the last point
	xLoop(Im+nEPL,R3) = g(Im,R3);
	r(Im+nEPL,R2) = rLocal(Im,R2);
	boundaryNodeInfo(Im+numberOfEdgePoints+nEPL,0).seqAdd(I.getBase(),1); // index into e->curve->getNURBS()
	boundaryNodeInfo(Im+numberOfEdgePoints+nEPL,1) = e->masterEdgeNumber();
// the edge number of the first point is determined by the startingPoint
	boundaryNodeInfo(numberOfEdgePoints+nEPL,0) = 0;
	boundaryNodeInfo(numberOfEdgePoints+nEPL,1) = masterEdge.array[e->curve->startingPoint]->edgeNumber;
// debug
//  	printf("Assigning boundarynodeInfo edge %i between %i and %i\n", e->edgeNumber,
//  	       Im.getBase()+numberOfEdgePoints+nEPL, Im.getBound()+numberOfEdgePoints+nEPL);
      }
      else
      {
//leave off the first point
	const int base=I.getBase()+1;   
	const int offset=nEPL+I.getBound();
	for( int m=I.getBound(); m>=base; m-- )
	{ // we have to reverse the orientation.
	  xLoop(offset-m,R3)=g(m,R3);   
	  r(offset-m,R2)=rLocal(m,R2);   
	}
	boundaryNodeInfo(Im+numberOfEdgePoints+nEPL,0).seqAdd(I.getBound(),-1); // index into e->curve->getNURBS()
	boundaryNodeInfo(Im+numberOfEdgePoints+nEPL,1) = e->masterEdgeNumber();  // these nodes sit on edge curve e
// the edge number of the first point is determined by the startingPoint
	boundaryNodeInfo(numberOfEdgePoints+nEPL,0) = 0;
	boundaryNodeInfo(numberOfEdgePoints+nEPL,1) = masterEdge.array[e->curve->endingPoint]->edgeNumber;
// debug
//  	printf("Assigning boundarynodeInfo edge %i between %i and %i\n", e->edgeNumber,
//  	       Im.getBase()+numberOfEdgePoints+nEPL, Im.getBound()+numberOfEdgePoints+nEPL);
      }
// debug
//        printf("Assigning boundaryNodeInfo for edge %i to be connected to master edge %i, \n", 
//  	     e->edgeNumber, e->masterEdgeNumber());
	
// AP: start/end points should be taken from the endPoints array to get consistent coordinates at corners
      
      nEPL +=I.getLength()-1;
    } // end for sc
    
    I = nEPL;
    xLoop.resize(I,3);
    
    r.resize(I,2);

// copy r to rc, reverse if inside a hole  AP: reversing only causes problems
    Range Im=Range(I.getBase(),I.getBound());
    rc(Im+numberOfEdgePoints,R2)=r(Im,R2);
	    
// add in the edge points from this loop
    numberOfEdgePoints += nEPL;

    //    printf("numberOfEdgePoints: %i\n", numberOfEdgePoints);
    //    cout<<"size of rc "<<rc.getLength(0)<<endl;

    Range R(edgePointStart,numberOfEdgePoints-1);
    faces(R,0).seqAdd(edgePointStart,1);
    faces(R,1)=faces(R,0)+1;
    faces(numberOfEdgePoints-1,1)=edgePointStart; // each Loop is periodic

    // if the reference surface is periodic we may have to shift the r-values so that
    // we have a closed curve -- e.g. we may have to shift values near the left edge to
    // lie to the right of the right edge.  // *kkc 051801 added != to fix compiler error on dec
    if( debug & 4 &&
        (referenceSurface.getIsPeriodic(0)!=Mapping::notPeriodic || 
	 referenceSurface.getIsPeriodic(1)!=Mapping::notPeriodic) )
    {
      ::display(rc(R,Range(0,1)),"BEFORE: r");
    }

// plot both curves for debug
    GraphicsParameters par;
    par.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    if (false)
    {
      gi.erase();
      params.set(GI_POINT_COLOUR,"blue");
      gi.plotPoints(rc, par);
    }

    for( int dir=0; dir<=1; dir++ )
    {
      if( referenceSurface.getIsPeriodic(dir)==Mapping::functionPeriodic )
      {

	int base=R.getBase();
	int bound=R.getBound();
	real rbase = referenceSurface.getDomainBound(0,dir);
	real rbound = referenceSurface.getDomainBound(1,dir);

        if( debug & 2 ) 
	{
	  printf("Adjust points for periodicity on surface %i (periodic=%i,%i) "
                 "(rBase,rBound)=(%5.2f,%5.2f)\n",s,
		 referenceSurface.getIsPeriodic(0),referenceSurface.getIsPeriodic(1),rbase,rbound );
	  
          printf(" First pt: r=%6.3f last-pt: r=%6.3f\n",rc(base,dir),rc(bound,dir));
	  // if( fabs(rc(base,dir))<1.e-6 )
          // 	    rc(base,dir)+=1.;
	  
	}
	
        // *wdh* 050513
        const real ptol=1.e-3;  // tolerance for how close a point is to a branch cut (in parameter space)
        if( fabs(rc(base,dir))<ptol || fabs(rc(base,dir)-1.)<ptol )
	{
	  // if the first point is near the branch cut we need to decide which side it should really be on
          real rCut =  fabs(rc(base,dir))<ptol ? 0. : 1.;
          bool found=false;
	  // look at the next points and find one that moves off the branch cut -- this will
          // tell us which side we are on
	  for( int i=base+1; i<=bound; i++ )
	  {
	    if( fabs(rc(i,dir)-rCut)>ptol )
	    {
	      // this point is not too near the branch cut -- use this side for the first point

              if( fabs(rc(i,dir))<.5 )  // inside point is nearer to zero
	      {
		if( rCut>.5 )
		{
		  rc(base,dir)-=1.;
		}
	      }
	      else  // inside point is nearer to 1
	      {
		if( rCut<.5 )
		{
		  rc(base,dir)+=1.;
		}
	      }
              found=true;
              break;
	    }
	  }
	  if( !found )
	  {
	    printf("buildSubSurfaceTriangulation:ERROR:unable to determine which side of a branch cut"
                   " the first point on a\n"
                   "trimming curve should lie!\n");
	  }
	  
	}
	

	for( int i=base+1; i<=bound; i++ )
	{
	  real rdist=rc(i,dir)-rc(i-1,dir);
	  if( debug & 2 ) cout<<"i, rc "<<i<<"  "<<rc(i,dir)<<endl;
	  if( fabs(rdist)>.5                // consecutive pts are far apart
	      /*kkc 040217 added this check to make trimmed revolutions work
	                   eg surf 219 of AirCollectorAssby.igs */
              // *wdh* 050513 -- turned this off -- I don't understand why it is needed
	      // *wdh* 050513 && ((rc(i,dir)<rbase)||(rc(i,dir)>rbound)) ) 
	       ) 
	  {
	    // cout<<"rbase, rbound "<<rbase<<"  "<<rbound<<endl;
	    // cout<<" adjust: rc(i,dir) "<<rc(i,dir)<<endl;
	    if( rdist<0. )
	      rc(i,dir)+=1.;   // shift to make them closer.
	    else
	      rc(i,dir)-=1.; 
	  }
	}
      }
    }
      
    // *kkc 051801 added != to fix compile error on dec
    if( debug & 4 && 
        (referenceSurface.getIsPeriodic(0)!=Mapping::notPeriodic ||
	 referenceSurface.getIsPeriodic(1)!=Mapping::notPeriodic) ) 
    {
      ::display(rc(R,Range(0,1)),"AFTER: rc");
    }

// assign a point inside each hole
    if( currentLoop.trimOrientation == -1 )
    {
      // find a point inside the trimming curve.
      int n=numberOfEdgePoints-edgePointStart;
      for( int axis=0; axis<domainDimension; axis++ )
      {
	holes(innerCurve,axis)= sum(rc(R,axis))/n;     // --- this could be wrong --- fix this !
      }
//      printf("holes(%i): (%e, %e)\n", innerCurve, holes(innerCurve,0), holes(innerCurve,1));
      innerCurve++;
    }

  } // for trimNumber

  if( debug & 2 ) printf("surface %i, numberOfEdgePoints: %i\n", s, numberOfEdgePoints);

// check rc and holes...
//  rc.display("Here is rc:");

  // build a triangulation
  if( !ok )
    return 1;

  int numberOfGridPoints[2];
  bool collapsedEdge[2][3];
  real averageArclength[2];
  real elementDensityTolerance=curvatureTolerance;

  real time1=getCPU();
// estimate the number of grid points that are needed in order to represent the mapping based
// on the curvature. Also locate collapsed edges (singularities) in the mapping.
  referenceSurface.determineResolution(numberOfGridPoints, collapsedEdge, averageArclength, 
				       elementDensityTolerance );
  real resolutionTime=getCPU()-time1;
  real aspectRatio=averageArclength[0]/max(REAL_MIN,averageArclength[1]);

  int numberOfNodes=numberOfEdgePoints;
  if( false ) // AP: this call sometimes generates nodes outside the boundary!
  {
    addInteriorNodes(numberOfNodes, surface, aspectRatio, numberOfGridPoints, rc, debug);
  }
  else
  {
    rc.resize(numberOfNodes,2);
  }
  
  faces.resize(numberOfEdgePoints,2);
  numberOfBoundaryNodes(s)=numberOfEdgePoints;


  Range R=numberOfNodes;

  if( debug & 2 )
  {
    for(int i=0; i<numberOfEdgePoints; i++ )
    {
      printf(" s=%i node %i : (%10.4e,%10.4e)\n",s, i, rc(i,0),rc(i,1));
    }
  }
	
// build the triangulation
  TriangleWrapper triangle;
  UnstructuredMapping & triangulation= *new UnstructuredMapping; 
  triangulation.incrementReferenceCount();

  triangulationSurface[s]=&triangulation;
//  printf("INFO: triangulationSurface[%i]->referenceCount=%i\n", s, triangulationSurface[s]->getReferenceCount());
	
  TriangleWrapperParameters & triangleParameters = triangle.getParameters();

  real maxArea=maximumArea < REAL_MAX ? maximumArea : 0.;
  if( maxArea>0. )
  {
    // Since we triangulate in parameter space we need to 
    // approximately scale the maximum area by the area of the patch
    real xScale=REAL_MIN*10.;

    if ( false )
      {
	// kkc this gives high aspect ratio surfaces too small an estimate
	for( int dir=0; dir<rangeDimension; dir++ )
	  xScale=max(xScale,referenceSurface.getRangeBound(End  ,dir)-referenceSurface.getRangeBound(Start,dir));
	
	maxArea/=xScale*xScale;
      }
    else
      {
	// use a very coarse grid to estimate the surface area
	int n=11;
	realArray rg(5,2),xg(5,3),dxdrg(5,3,2),dxdr(1,3,2);

	rg(0,0) = rg(0,1) = 0.;
	rg(1,0) = rg(1,1) = 1.;
	rg(2,0) = 0.; rg(2,1) = 1.;
	rg(3,0) = 1.; rg(3,1) = 0.;
	rg(4,0) = rg(4,1) = .5;
	//	rg(0,0) = .5;
	//	rg(0,1) = .5;
	referenceSurface.map(rg,xg,dxdrg);

	Range XR(3), RR(2);
	real scale=0;
	for ( int i=0; i<rg.getLength(0); i++ )
	  {

 	    scale += sqrt( SQR(dxdrg(i,1,0)*dxdrg(i,2,1)-dxdrg(i,1,1)*dxdrg(i,2,0)) + 
 			   SQR(dxdrg(i,0,0)*dxdrg(i,2,1)-dxdrg(i,0,1)*dxdrg(i,2,0)) +
 			   SQR(dxdrg(i,0,0)*dxdrg(i,1,1)-dxdrg(i,0,1)*dxdrg(i,1,0)) );

	  }

	scale = .5*scale/real(rg.getLength(0));

	xScale = sqrt(2.*scale);
	maxArea /= scale;

// 	maxArea /= (.5*sqrt( SQR(dxdr(0,1,0)*dxdr(0,2,1)-dxdr(0,1,1)*dxdr(0,2,0)) + 
// 			 SQR(dxdr(0,0,0)*dxdr(0,2,1)-dxdr(0,0,1)*dxdr(0,2,0)) +
// 			 SQR(dxdr(0,0,0)*dxdr(0,1,1)-dxdr(0,0,1)*dxdr(0,1,0)) ));
      }


    const real minArea=SQR(100.*REAL_EPSILON); // This is a guess, may be too small
    if( sqrt(maxArea) < sqrt(minArea) )
    {
      printf("WARNING: parameter-space-maxArea=%8.2e too small, setting to %8.2e\n",maxArea,minArea);
      maxArea=minArea;
    }
    printf("surface %i: triangle input: maximumArea=%8.2e -> parameter-space-maxArea=%8.2e (xScale=%8.2e) \n",
              s,maximumArea,maxArea,xScale);
  }

/* ----
>>>>>>> 1.52

<<<<<<< compositeTopology.C
    if ( false )
      {
	// kkc this gives high aspect ratio surfaces too small an estimate
	for( int dir=0; dir<rangeDimension; dir++ )
	  xScale=max(xScale,referenceSurface.getRangeBound(End  ,dir)-referenceSurface.getRangeBound(Start,dir));
	
	maxArea/=xScale*xScale;
      }
    else
      {
	int n=11;
	RealArray rg(1,2),xg(1,3),dxdr(1,3,2);
	rg(0,0) = .5;
	rg(0,1) = .5;
	referenceSurface.map(rg,xg,dxdr);

	maxArea /= (.5*sqrt( SQR(dxdr(0,1,0)*dxdr(0,2,1)-dxdr(0,1,1)-dxdr(0,2,0)) + 
			 SQR(dxdr(0,0,0)*dxdr(0,2,1)-dxdr(0,0,1)-dxdr(0,2,0)) +
			 SQR(dxdr(0,0,0)*dxdr(0,1,1)-dxdr(0,0,1)-dxdr(0,1,0)) ));
      }


    const real minArea=SQR(100.*REAL_EPSILON); // This is a guess, may be too small
    if( sqrt(maxArea) < sqrt(minArea) )
    {
      printf("WARNING: parameter-space-maxArea=%8.2e too small, setting to %8.2e\n",maxArea,minArea);
      maxArea=minArea;
    }
    printf("surface %i: triangle input: maximumArea=%8.2e -> parameter-space-maxArea=%8.2e (xScale=%8.2e) \n",
              s,maximumArea,maxArea,xScale);
  }
  
=======
  real maxArea=maximumArea < REAL_MAX ? maximumArea : 0.;
  if( maxArea>0. )
  {
    // Since we triangulate in parameter space we need to 
    // approximately scale the maximum area by the area of the patch
    real xScale=REAL_MIN*10.;
    for( int dir=0; dir<rangeDimension; dir++ )
      xScale=max(xScale,referenceSurface.getRangeBound(End  ,dir)-referenceSurface.getRangeBound(Start,dir));

    maxArea/=xScale*xScale;
    const real minArea=SQR(100.*REAL_EPSILON); // This is a guess, may be too small
    if( sqrt(maxArea) < sqrt(minArea) )
    {
      printf("WARNING: parameter-space-maxArea=%8.2e too small, setting to %8.2e\n",maxArea,minArea);
      maxArea=minArea;
    }
    printf("surface %i: triangle input: maximumArea=%8.2e -> parameter-space-maxArea=%8.2e (xScale=%8.2e) \n",
              s,maximumArea,maxArea,xScale);
  }
  ---- */
  
  triangleParameters.setMaximumArea(maxArea);
  triangleParameters.saveNeighbourList();
  triangleParameters.saveVoronoi(false);
  if ( !triangleParameters.getFreezeSegments() )
    triangleParameters.toggleFreezeSegments(); // do not split boundary segements
	
  triangleParameters.setQuietMode(true);
  // triangleParameters.setMinimumAngle(0.);
  
// scale nodes on collapsed edge -- only scale down to ra rather than 0.
  

  //  rc.display("rc");

  Range all;
  // kkc use the actual domainBound for the mapping since it may not be in [0,1]x[0,1] // const real ra=.05, rba=1.-ra;
  const real ra = improveTri ? min(0.05,maxDist) : 0.05;

  const real ra_r=(real)referenceSurface.getDomainBound(0,0)+ra, 
             ra_s=(real)referenceSurface.getDomainBound(0,1)+ra,
             rba_r=(real)referenceSurface.getDomainBound(1,0)-ra_r, 
             rba_s=(real)referenceSurface.getDomainBound(1,1)-ra_s;

  // real rShift = s==0 ? .25 : .75;
  real rShift=.5;
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
    if( isTrimmedMapping )
    {
      // On a TrimmedMapping we guess the r location of the singular point
      //  *** this could be done better ***
      if( collapsedEdge[0][1] || collapsedEdge[1][1] )
      {
        // rShift=average value or r or s .5*( min + max ) ? 
	Range E=numberOfEdgePoints;  // don't count extra interior nodes.
        rShift=.5*( min(rc(E,0))+max(rc(E,0)) );
        printf(" ** guess singular location to be r0=%e\n",rShift);
      }
      else
      {
	Range E=numberOfEdgePoints;  // don't count extra interior nodes.
        rShift=.5*( min(rc(E,1))+max(rc(E,1)) );
        printf(" ** guess singular location to be r1=%e\n",rShift);
      }
    }

    // Here is a function g(r) satisfying g(0)=g(1)=ra, g(.5)=1., g(r)>0 for 0<= r <=1 :
    #define SCALE_TWO_SIDES(r,ra) ( 1. - SQR((r)-.5) *4.*(1.-(ra)) )


    if( collapsedEdge[0][1] && collapsedEdge[1][1] )
    {
       printf(" ** bottom AND top collapsed, scale nodes to get better triangles shapes\n");

      rc(R,0) = (rc(R,0)-rShift)*SCALE_TWO_SIDES(rc(R,1),ra_s);
      if( numberOfInnerCurves>0 )
	holes(all,0) = (holes(all,0)-rShift)*SCALE_TWO_SIDES(holes(all,1),ra_s); 
    }
    else if( collapsedEdge[0][0] && collapsedEdge[1][0] )
    {
       printf(" ** left and right collapsed, scale nodes to get better triangles shapes\n");

      rc(R,1) = (rc(R,1)-rShift)*SCALE_TWO_SIDES(rc(R,0),ra_r); // *(ra_r + rba_r*rc(R,0));
      if( numberOfInnerCurves>0 )
	holes(all,1) = (holes(all,1)-rShift)*SCALE_TWO_SIDES(holes(all,0),ra_r); //  *(ra_r + rba_r*holes(all,0));

    }
    else if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      printf(" ** bottom collapsed, scale nodes to get better triangles shapes\n");

      //      rc(R,0) = (rc(R,0)-rShift)*(ra + rba*rc(R,1));
      rc(R,0) = (rc(R,0)-rShift)*(ra_s + rba_s*rc(R,1));
      // kkc rc(R,0) *= ra + rba*rc(R,1);
      if( numberOfInnerCurves>0 )
	holes(all,0) = (holes(all,0)-rShift)*(ra_s + rba_s*holes(all,1));
      //kkc	holes(all,0) = (holes(all,0)-rShift)*(ra + rba*holes(all,1));
      // holes(all,0)*=ra + rba*holes(all,1);
    }
    else if( collapsedEdge[1][1] )
    {
      // top collapsed -- apply a scaling function to the nodes:
      printf(" ** top collapsed, scale nodes to get better triangles shapes\n");
      rc(R,0) = (rc(R,0)-rShift)*(1.-rba_s*rc(R,1));
      //kkc       rc(R,0) = (rc(R,0)-rShift)*(1.-rba*rc(R,1));
      // rc(R,0) *= 1.-rba*rc(R,1);
      if( numberOfInnerCurves>0 )
	holes(all,0) = (holes(all,0)-rShift)*(1.-rba_s*holes(all,1));
      //kkc	holes(all,0) = (holes(all,0)-rShift)*(1.-rba*holes(all,1));
      // holes(all,0)*=1.-rba*holes(all,1);
    }
    else if( collapsedEdge[0][0] )
    {
      printf(" ** left collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(ra_r + rba_r*rc(R,0));
      // kkc      rc(R,1) = (rc(R,1)-rShift)*(ra + rba*rc(R,0));
      // rc(R,1) *= ra + rba*rc(R,0);
      if( numberOfInnerCurves>0 )
	holes(all,1) = (holes(all,1)-rShift)*(ra_r + rba_r*holes(all,0));
      // kkc	holes(all,1) = (holes(all,1)-rShift)*(ra + rba*holes(all,0));
      // holes(all,1)*=ra + rba*holes(all,0);
    }
    else if( collapsedEdge[1][0] )
    {
      printf(" ** right collapsed, scale nodes to get better triangles shapes\n");
      rc(R,1) = (rc(R,1)-rShift)*(1.-rba_r*rc(R,0));
      // kkc      rc(R,1) = (rc(R,1)-rShift)*(1.-rba*rc(R,0));
      // rc(R,1) *= 1.-rba*rc(R,0);
      if( numberOfInnerCurves>0 )
	holes(all,1) = (holes(all,1)-rShift)*(1.-rba_r*holes(all,0));
      //kkc	holes(all,1) = (holes(all,1)-rShift)*(1.-rba*holes(all,0));
      // holes(all,1)*=1.-rba*holes(all,0);
    }
  }

  bool scaleNodes=aspectRatio>1.5 || aspectRatio<.5 ;
  if( scaleNodes )
  {
    if( debug & 2 ) printf("Scale nodes: aspectRatio=%8.2e\n",aspectRatio);
	  
    rc(R,0)*=aspectRatio;  // scale so that triangles will have a better shape on final grid.
    maxArea *=aspectRatio;
    triangleParameters.setMaximumArea(maxArea);

    if( numberOfInnerCurves>0 )
      holes(all,0)*=aspectRatio;
  }

  time1=getCPU();

  bool plotBoundary=false;
  bool plotPoints=false;
  bool printPoints=false;
  
  if (plotBoundary || plotPoints || printPoints)
  {
// print all points
    if (printPoints)
    {
      for( int i=0; i<numberOfEdgePoints; i++ )
	printf("point(%i)=(%e,%e)\n", i, rc(i,0), rc(i,1));
    }
    // plot faces for debugging
    gi.erase();
    GraphicsParameters par;
    par.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
    if (plotPoints)
      gi.plotPoints(rc, par);
    if (plotBoundary)
    {
      realArray line(numberOfEdgePoints-1,2,2);
      for( int i=0; i<numberOfEdgePoints-1; i++ )
      {
	line(i,0,0)=rc(i,0);
	line(i,1,0)=rc(i,1);
	line(i,0,1)=rc(i+1,0);
	line(i,1,1)=rc(i+1,1);
      }
      gi.plotLines(line, par);
    }
    gi.stopReadingCommandFile();  // ***************
    
    aString answer, menu[]={"continue",""};  //
    gi.getMenuItem(menu,answer);
  }

  //  cout<<"rc size "<<rc.getLength(0)<<endl;
  triangle.initialize( faces, rc );
  // display(holes,"holes");
  if(numberOfInnerCurves>0 )
    triangle.setHoles( holes );

  // Note that there may be new nodes introduced.

  if( false && s==5 )
  {
    triangle.getParameters().setQuietMode(false);
    triangle.getParameters().setVerboseMode(2);
  }
  
  // **********************************************************
  // ****************** call triangulate **********************
  // **********************************************************

  triangle.generate();

  real triangleTime=getCPU()-time1;
	
  if( triangleErrorDetected )
  {
    printf("\n ****************************************************************************\n");
    printf("buildSubSurfaceTriangulation:ERROR detected in call to triangle! surface s=%i\n",s);
    printf(" *****************************************************************************\n\n");
  }

  const intArray & elements = triangle.generateElementList();
  // const realArray & rt = triangle.getPoints();

  realArray & rt = rCoordinates[s];
  rt=triangle.getPoints(); // keep a copy

  //  cout<<"after gen : "<<rt.getLength(0)<<"  "<<elements.getLength(0)<<endl;
  const intArray & neighbours = triangle.getNeighbours();
// tmp
//    int nf=triangle.getNumberOfEdges();
//    int nbf=triangle.getNumberOfBoundaryEdges();
//    UnstructuredMapping u;
//    u.setNodesElementsAndNeighbours(rt,elements,neighbours,nf, nbf);
//    gi.erase();
//    PlotIt::plot(gi, u);
  
  
  int numberOfTriangles=elements.getLength(0);
  if( numberOfTriangles==0 )
  {
    printf("***buildSubSurfaceTriangulation::triangulate:ERROR: numberOfTriangles==0, something is wrong here\n");
    return 1;
  }
  if( scaleNodes )
  {
    rt(all,0)*=1./aspectRatio;   // scale back to [0,1]
  }
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
    if( collapsedEdge[0][1] && collapsedEdge[1][1] )
    {
       printf(" ** bottom AND top collapsed, un-scale nodes to get better triangles shapes\n");
 
      rt(all,0) = rt(all,0)/SCALE_TWO_SIDES(rt(all,1),ra_s) +rShift; // /(ra_s + rba_s*rt(all,1))+rShift;
    }
    else if( collapsedEdge[0][0] && collapsedEdge[1][0] )
    {
       printf(" ** left and right collapsed, un-scale nodes to get better triangles shapes\n");
       rt(all,1) = rt(all,1)/SCALE_TWO_SIDES(rt(all,0),ra_r) +rShift;    //   /(ra_r + rba_r*rt(all,0))+rShift;
    }
    else if( collapsedEdge[0][1] )
    {
      // bottom collapsed -- apply a scaling function to the nodes:
      printf(" ** bottom collapsed, un-scale nodes to get better triangles shapes\n");
      rt(all,0) = rt(all,0)/(ra_s + rba_s*rt(all,1))+rShift;
      //kkc      rt(all,0) = rt(all,0)/(ra + rba*rt(all,1))+rShift;
      // rt(all,0) /= ra + rba*rt(all,1);
    }
    else if( collapsedEdge[1][1] )
    {
      rt(all,0) = rt(all,0)/(1.-rba_s*rt(all,1))+rShift;
      //kkc      rt(all,0) = rt(all,0)/(1.-rba*rt(all,1))+rShift;
      // rt(all,0) /= 1.-rba*rt(all,1);
    }
    else if( collapsedEdge[0][0] )
    {
      rt(all,1) = rt(all,1)/(ra_r + rba_r*rt(all,0))+rShift;
      //kkc      rt(all,1) = rt(all,1)/(ra + rba*rt(all,0))+rShift;
      // rt(all,1) /= ra + rba*rt(all,0);
    }
    else if( collapsedEdge[1][0] )
    {
      rt(all,1) = rt(all,1)/(1.-rba_r*rt(all,0))+rShift;
      //kkc      rt(all,1) = rt(all,1)/(1.-rba*rt(all,0))+rShift;
      // rt(all,1) /= 1.-rba*rt(all,0);
    }
  }

  //  Overture::getGraphicsInterface()->plotPoints(rt);
  //  Overture::getGraphicsInterface()->erase();

  realArray nodes(rt.dimension(0),3);
  time1=getCPU();
  referenceSurface.map( rt,nodes );   // compute 3d positions of triangle nodes.
  real nurbTime=getCPU()-time1;

// AP: Change rt for nodes on collapsed edges (we assume that only one edge is collapsed...)
  if( collapsedEdge[0][0] || collapsedEdge[1][0] || collapsedEdge[0][1] || collapsedEdge[1][1] )
  {
    int iCollapsed=0, i, dir;
    if( collapsedEdge[0][1] ) // collapsed[startEnd][rs]
    {
// look for i: min(rt(i,1)
      real sMin = rt(0,1);
      dir = 0; // change the r-coordinate
      for (i=1; i<rt.getLength(0); i++)
	if (rt(i,1) < sMin)
	{
	  sMin = rt(i,1);
	  iCollapsed = i;
	}
      printf("sub-surface %i, s=0 collapsed, sMin = %e, index=%i\n", s, sMin, iCollapsed);
    }
    else if( collapsedEdge[1][1] )
    {
// look for i: max(rt(i,1)
      real sMax = rt(0,1);
      dir = 0; // change the r-coordinate
      for (i=1; i<rt.getLength(0); i++)
	if (rt(i,1) > sMax)
	{
	  sMax = rt(i,1);
	  iCollapsed = i;
	}
      printf("sub-surface %i, s=1 collapsed, sMax = %e, index=%i\n", s, sMax, iCollapsed);
    }
    else if( collapsedEdge[0][0] )
    {
// look for i: min(rt(i,0)
      real rMin = rt(0,0);
      dir = 1; // change the s-coordinate
      for (i=1; i<rt.getLength(0); i++)
	if (rt(i,0) < rMin)
	{
	  rMin = rt(i,0);
	  iCollapsed = i;
	}
      printf("sub-surface %i, r=0 collapsed, rMin = %e, index=%i\n", s, rMin, iCollapsed);
    }
    else if( collapsedEdge[1][0] )
    {
// look for i: max(rt(i,0)
      real rMax = rt(0,0);
      dir = 1; // change the s-coordinate
      for (i=1; i<rt.getLength(0); i++)
	if (rt(i,0) > rMax)
	{
	  rMax = rt(i,0);
	  iCollapsed = i;
	}
      printf("sub-surface %i, r=1 collapsed, rMax = %e, index=%i\n", s, rMax, iCollapsed);
    }
// change rt(iCollapsed,0)
    rt(iCollapsed, dir) = -1;
  }


  int numberOfFaces=triangle.getNumberOfEdges();
  int numberOfBoundaryFaces=triangle.getNumberOfBoundaryEdges();
  triangulation.setNodesElementsAndNeighbours(nodes,elements,neighbours,
					      numberOfFaces,numberOfBoundaryFaces);

  triangulation.setName(Mapping::mappingName,cs.getName(Mapping::mappingName)+"-unstructured");

  //  	 	  gi.erase();
  //  	 	  PlotIt::plot(gi,triangulation);
  // *** kkc triangle refinement!
  if ( improveTri ) {
    
    int step = 0;
    real dev;
    real dev_old = REAL_MAX;
    real absoluteTol = maxDist;
    while ( (dev=refineTriangulation(triangulation,cs[s],absoluteTol))>absoluteTol && (dev_old-dev)>absoluteTol/100 && step<10 )
      {
	cout<<"deviation after step "<<step<<" is "<<dev<<endl;
	step++;
	dev_old = dev;
      }
    cout<<"deviation after "<<step<<" steps is "<<dev<<endl;

    rt.redim(0);
    rt = triangulation.getNodes();
  }
  // ******

  real timeForTriangulation=getCPU()-times;

  if( debug & 2 )
    printf("built unstructured grid for %s, cpu=%8.2e (triangle=%8.2e, nurb=%8.2e, resolution=%8.2e)\n",
  	   (const char*)referenceSurface.getName(Mapping::mappingName),timeForTriangulation,triangleTime,nurbTime,
	   resolutionTime);

  totalTimeToBuildSeparateTriangulations+=timeForTriangulation;
  totalTriangleTime+=triangleTime;
  totalNurbTime+=nurbTime;
  totalResolutionTime+=resolutionTime;
    
  totalNumberOfNodes+=triangulation.getNumberOfNodes();
  totalNumberOfFaces+=triangulation.getNumberOfFaces();
  totalNumberOfElements+=triangulation.getNumberOfElements();


  if( debug & 4 )
  {
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
    gi.erase();
    PlotIt::plot(gi,triangulation,params);

    if( debug & 8 )
    {
      UnstructuredMapping uns;
      uns.setNodesElementsAndNeighbours(rt,elements,neighbours,
					numberOfFaces,numberOfBoundaryFaces);
      gi.erase();
      params.set(GI_TOP_LABEL,sPrintF("triangulation for surface %i",s));
      
      PlotIt::plot(gi,uns,params);
    }
    params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  }

  return 0;
} // end buildSubSurfaceTriangulation

    
//! Check the consistency of an element with it's faces and nodes. Attempt to fix
//! inconsistencies by replacing nodes by their duplicates.
/*!
   /param e (input): check this element
 */
static int
checkElement( int e, 
	      int numberOfNodes, 
	      int numberOfFaces, 
              const intArray & elements, 
	      const intArray & faces,
	      const intArray & elementFaces,
	      const intArray & faceElements, 
              int numberOfDuplicateNodes,
              const intArray & duplicateNodes,
              int debug )
{
  const int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
  if( n0<0 || n0>=numberOfNodes || n1<0 || n1>=numberOfNodes || n2<0 || n2>=numberOfNodes )
  {
    printf("check:ERROR for element e=%i, nodes=(%i,%i,%i). "
	   "Node numbers out of range, numberOfNodes=%i\n",e,n0,n1,n2,numberOfNodes);
  }
  for( int m=0; m<3; m++ )
  {
    const int f0=elementFaces(e,m);
    if( f0<0 || f0>=numberOfFaces )
    {
      printf("check:ERROR for element e=%i, face=%i. "
	     "Face numbers out of range, numberOfFaces=%i\n",e,f0,numberOfNodes);
    }
    else
    {
      for( int side=0; side<=1; side++ )
      {
	int faceNode=faces(f0,side);
	if( faceNode!=n0 && faceNode!=n1 && faceNode!=n2 )
	{
	  // check for a duplicate node
	  for( int n=0; n<numberOfDuplicateNodes; n++ )
	  {
	    if( duplicateNodes(0,n)==faceNode )
	    {
	      faces(f0,side)=duplicateNodes(1,n);
	      if( debug & 2 )
		printf("INFO: replace node %i with node %i in face %i (e=%i).\n",faceNode,faces(f0,side),f0,e);
                    
	      // If we change a face we need to go back and check elements connected to this face
	      for( int nn=0; nn<=1; nn++ )
	      {
		int ee = faceElements(f0,nn); // check the elements next to this face
                bool elementChanged=false;
		for( int mm=0; mm<=2; mm++ )
		{
		  if( elements(ee,mm)==faceNode )
		  {
		    elements(ee,mm)=duplicateNodes(1,n);
                    elementChanged=true;
		    if( debug & 2 )
		      printf("INFO: replace node %i with node %i in element %i.\n",faceNode,faces(f0,side),ee);
		  }
		}
                if( ee<e && elementChanged )
		{
                  // if we change a previous element we need to go back and check it's faces.
		  checkElement( ee, numberOfNodes,numberOfFaces,elements,faces,elementFaces,faceElements,
				numberOfDuplicateNodes,duplicateNodes,debug );

		}
		
	      }
              break;
	    } //end if duplicateNode
	    
	  }
	}
      }
    }
  }
  return 0;
}


// ====================================================================================================
//! Build a global triangulation for a CompositeSurface.
/*!  Given the set of Triangulations for the sub-surfaces and the edge curve information,
     build a global triangulation. This requires removing duplicate nodes and building the
    connectivity information between the sub-surfaces.
 */ 
// ====================================================================================================
int CompositeTopology::
triangulateCompositeSurface(int & debug, GenericGraphicsInterface & gi, GraphicsParameters & params)
{
  real time0=getCPU();
  
// delete old triangulations for each surface
  int s;
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    if( triangulationSurface[s]!=NULL && triangulationSurface[s]->decrementReferenceCount()==0 )
    {
      delete triangulationSurface[s];
    }
    triangulationSurface[s]=NULL;
  }

  IntegerArray numberOfBoundaryNodes(cs.numberOfSubSurfaces());
  realArray *rCoordinates = new realArray [cs.numberOfSubSurfaces()];

  IntegerArray *boundaryNodeInfop = new IntegerArray [cs.numberOfSubSurfaces()];

// edgNodeInfo : holds global node numbering for nodes on the edge curve.
  IntegerArray *edgeNodeInfop = new IntegerArray [numberOfEdgeCurves];

  int debugs=0; // 7; // *****
  
  const int domainDimension=2;
  const int rangeDimension=3;

  int totalNumberOfNodes=0;
  int totalNumberOfFaces=0;
  int totalNumberOfElements=0;

  real totalTimeToBuildSeparateTriangulations=0.;
  real totalTriangleTime=0.;
  real totalNurbTime=0.;
  real totalResolutionTime=0.;

  // **************************************************
  // *** Build triangulations for each sub-surface ****
  // **************************************************

  int nFail=0;
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    real times=getCPU();
    Mapping & surface = cs[s];

    buildSubSurfaceTriangulation(s, 
				 numberOfBoundaryNodes, 
				 rCoordinates,
				 edgeNodeInfop,
				 boundaryNodeInfop,
				 totalNumberOfNodes,
				 totalNumberOfFaces,
				 totalNumberOfElements,
				 totalTimeToBuildSeparateTriangulations,
				 totalTriangleTime,
				 totalNurbTime,
				 totalResolutionTime,
				 debug,
				 gi,
				 params);
    if (triangulationSurface[s] == NULL)
      nFail++;
  }  // for s
      
  aString buf;
  printf("\n ***built %i unstructured grids, cpu=%8.2e (triangle=%8.2e, nurb=%8.2e, resolution=%8.2e)\n",
	 cs.numberOfSubSurfaces()-nFail,totalTimeToBuildSeparateTriangulations,
	 totalTriangleTime,totalNurbTime,totalResolutionTime);
  if (nFail > 0)
    gi.outputString(sPrintF(buf,"FAILED to triangulate %i surfaces", nFail));
  else
    gi.outputString("All surfaces were triangulated successfully");
  
  // ***************************************
  // *** Build a global Triangulation ******
  // ***************************************

  int maxNumberOfFaces=totalNumberOfElements*3;

  realArray nodes(totalNumberOfNodes,3);
  intArray elements(totalNumberOfElements,3);
  intArray faces(maxNumberOfFaces,2);

  intArray elementSurface(totalNumberOfElements);  // sub-surface number for each element
  elementSurface=-1;
  
  int numberOfDuplicateNodes=0;
  intArray duplicateNodes(2,100);  // holds a list of duplicate nodes

  // signForNormal(s): indicates whether the normal to surface "s" points in one direction or the other.
  const int numberOfSurfaces = cs.numberOfSubSurfaces();
  signForNormal.redim(numberOfSurfaces);
  for( s=0; s<numberOfSurfaces; s++ )
    signForNormal(s)=numberOfSurfaces*10 + s;   // give each surface a unique id
    
  signForNormal(0)=+1;  // sub surface 0 gets sign=+1

  intArray consistent(numberOfSurfaces,numberOfSurfaces),inconsistent(numberOfSurfaces,numberOfSurfaces);
  consistent=0;
  inconsistent=0;


  intArray elementFaces(totalNumberOfElements,3);  // 3 faces on an element
  intArray faceElements(maxNumberOfFaces,2);     // 2 elements adjacent to each face
  faceElements=-1;
  
  intArray nodeInfo(totalNumberOfNodes,2);
  nodeInfo=-1;
  
  realArray rNodes(totalNumberOfNodes,2);   // parameter space coordinates for nodes (including duplicates)
  intArray elementCoordinates(totalNumberOfElements,3);  // pointers from elements to rNodes

  // Here is the global triangulation:
  // UnstructuredMapping globalTriangulation;
  
  int numberOfNodes=0;
  int numberOfElements=0;
  int numberOfFaces=0;
  int globalNumber=0;
  int globalNumberOfFaces=0;
  int numberOfCoordinateNodes=0;
  
  Range R, R2=2, R3=3;
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    // add nodes, elements, faces, and neighbours from this surface triangulation to global triangulation

    IntegerArray & boundaryNodeInfo = boundaryNodeInfop[s];
    if( triangulationSurface[s]==NULL )
      continue;

    UnstructuredMapping & uns = *triangulationSurface[s];
    const realArray & sNodes = uns.getNodes();  // surface nodes
    
    int sNumberOfNodes=uns.getNumberOfNodes();
    Range N=uns.getNumberOfNodes();

    // *** assign new nodes ***
    // only add nodes that are not already in the list

    // ***** first assign boundary nodes ****
    int num = numberOfBoundaryNodes(s);
    
//    assert( num <= sNumberOfNodes );
    if (num > sNumberOfNodes)
    {
      gi.outputString(sPrintF(buf,"ERROR: surface %i: number of boundary nodes: %i, total number of nodes: %i", 
			      s, num, sNumberOfNodes));
      return 1;
    }
    
    
    intArray nodeTranslation(N);
    intArray nodeIsNew(N);
    nodeIsNew=true;
    
    int i;
    for( i=0; i<num; i++ )
    { // add boundary nodes if they are not already added from another surface.
      int j= boundaryNodeInfo(i,0);
      int e = boundaryNodeInfo(i,1);
      
      assert( e>=0 && e<numberOfEdgeCurves );
      if( edgeNodeInfop[e](j)==-1 )
      { 
        // first time we see an edge node we give it a global number
	edgeNodeInfop[e](j)=globalNumber;
/* ----
        // if this edge point is on the start or end of the edge we should also
        // mark the point on the adjoining edge
        int j2=boundaryNodeInfo(i,2);
        if( j2>=0 )
	{
	  int e2=boundaryNodeInfo(i,3);
          edgeNodeInfop[e2](j2)=globalNumber;
	}
---- */	
        nodes(globalNumber,R3)=sNodes(i,R3);
	globalNumber++;
      }
      else
      {
	nodeIsNew(i)=false; // this node is already there.
      }
      
      nodeTranslation(i)=edgeNodeInfop[e](j);  // use the global number for this edge node.
    } // end for i...
    
    // *** add new nodes that are in the interior ***
    for( i=num; i<sNumberOfNodes; i++ )
    {
      nodeTranslation(i)=globalNumber;
      nodes(globalNumber,R3)=sNodes(i,R3);
      globalNumber++;
    }

    // display(sNodes,"sNodes");
    // display(nodes,"nodes");
    
    // nodes(N+numberOfNodes,R3)=sNodes;
    
    // **** assign new elements ****
    const intArray & sElements = uns.getElements();
    int sNumberOfElements=uns.getNumberOfElements();
    R=uns.getNumberOfElements();

    int j=numberOfElements;
    for( i=0; i<sNumberOfElements; i++,j++ )
    {
      elements(j,0)=nodeTranslation(sElements(i,0));
      elements(j,1)=nodeTranslation(sElements(i,1));
      elements(j,2)=nodeTranslation(sElements(i,2));

      elementSurface(j)=s;
    }
    
    // **assign new faces, faceElements ***
    int sNumberOfFaces=uns.getNumberOfFaces();
    const intArray & sFaces = uns.getFaces();
    const intArray & sFaceElements = uns.getFaceElements();
    const intArray & sElementFaces = uns.getElementFaces();


    IntegerArray faceTranslation(sNumberOfFaces);
    int f;
    for( f=0; f<sNumberOfFaces; f++ )
    {
      int n0=sFaces(f,0), n1=sFaces(f,1);
      // ** we could determine from the node number, what edge curve and point we are on **
      // ** since the initial nodes are numbered ***
      // if( n0 < numberOfBoundaryNodes
      // boundaryNodeInfo(n0,0) = index into edge curve
      // boundaryNodeInfo(n0,1) = edge curve number
      bool boundaryFace = sFaceElements(f,1)<0;

      // add this face if it is not a boundary face or at least one node is new
      if( !boundaryFace || nodeIsNew(n0) || nodeIsNew(n1) ) 
      {
	// add this new face
        int n0Global=nodeTranslation(n0);
	int n1Global=nodeTranslation(n1);
        faces(globalNumberOfFaces,0)=n0Global;
        faces(globalNumberOfFaces,1)=n1Global;
        faceTranslation(f)=globalNumberOfFaces;

//         printf(" s=%i add face %i n0=%i n0Global=%i n1=%i n1Global=%i boundaryFace=%i\n",
// 	       s,globalNumberOfFaces,n0,n0Global,n1,n1Global,boundaryFace);
	
        // assign element numbers for this new face
	assert( sFaceElements(f,0)>=0 );
        faceElements(globalNumberOfFaces,0)=sFaceElements(f,0)+numberOfElements;
	if( sFaceElements(f,1)>=0 )
	  faceElements(globalNumberOfFaces,1)=sFaceElements(f,1)+numberOfElements;

        if( sFaceElements(f,1)<0 )
	{
          // must be a boundary face -- assign the nodeInfo array: boundary nodes point back to
          // the two boundary faces that belong to the node.
	  if( nodeIsNew(n0) )
	  {
	    if( nodeInfo(n0Global,0)==-1 )
	      nodeInfo(n0Global,0)=globalNumberOfFaces;
	    else if( nodeInfo(n0Global,1)==-1 )
	      nodeInfo(n0Global,1)=globalNumberOfFaces;
	    else
	    {
              printf("****WARNING***: s=%i, n0=%i, n0Global=%i, boundary face but nodeInfo is already set\n"
                     "                nodeInfo=(%i,%i), new face %i has nodes (%i,%i)\n",
                        s,n0,n0Global,nodeInfo(n0Global,0),nodeInfo(n0Global,1),globalNumberOfFaces,n0Global,n1Global);
	      // throw "error";
	    }
	  }
	  if( nodeIsNew(n1) )
	  {
	    if( nodeInfo(n1Global,0)==-1 )
	      nodeInfo(n1Global,0)=globalNumberOfFaces;
	    else if( nodeInfo(n1Global,1)==-1 )
	      nodeInfo(n1Global,1)=globalNumberOfFaces;
	    else
	    {
              printf("****WARNING***: s=%i, n0=%i, n1Global=%i, boundary face but nodeInfo is already set\n"
                     "                nodeInfo=(%i,%i), new face %i has nodes (%i,%i)\n",
                        s,n0,n1Global,nodeInfo(n1Global,0),nodeInfo(n1Global,1),globalNumberOfFaces,n0Global,n1Global);
	      
	      // throw "error";
	    }
	  }
	}
	
	globalNumberOfFaces++;
      }
      else
      {
        // *** both nodes are already in the global triangulation, the face must already be there ***

        int n0Global=nodeTranslation(n0);
	int n1Global=nodeTranslation(n1);
        int fGlobal=-1;
	if( nodeInfo(n0Global,0)==nodeInfo(n1Global,0) || nodeInfo(n0Global,0)==nodeInfo(n1Global,1) )
	{
	  fGlobal=nodeInfo(n0Global,0);  // this face is attached to both nodes
	  if( fGlobal<0 )
	  {
	    printf("ERROR: Both nodes are already in the triangulation but nodeInfo incorrect\n"
		   " s=%i, n0=%i, n0Global=%i, n1=%i, n1Global=%i nodeInfo(n0Global,0:1)=%i,%i "
		   "nodeInfo(n0Global,0:1)=%i,%i \n",s,n0,n0Global,n1,n1Global,
		   nodeInfo(n0Global,0),nodeInfo(n0Global,1), nodeInfo(n1Global,0),nodeInfo(n1Global,1));
	    continue;
	  }
	}
	else
	{
          if( nodeInfo(n0Global,1)==nodeInfo(n1Global,0) || nodeInfo(n0Global,1)==nodeInfo(n1Global,1) )
	  {
  	    fGlobal=nodeInfo(n0Global,1);  //  this face is attached to both nodes
            if( fGlobal<0 )
	    {
	      printf("ERROR: Both nodes are already in the triangulation but nodeInfo incorrect\n"
                     " s=%i, n0=%i, n0Global=%i, n1=%i, n1Global=%i nodeInfo(n0Global,0:1)=%i,%i "
                     "nodeInfo(n0Global,0:1)=%i,%i \n",s,n0,n0Global,n1,n1Global,
		     nodeInfo(n0Global,0),nodeInfo(n0Global,1), nodeInfo(n1Global,0),nodeInfo(n1Global,1));
	      continue;
	    }
	  }
	  else
	  {
            // must be a corner -- we have to search a little harder
            int ff[4]={nodeInfo(n0Global,0),nodeInfo(n0Global,1),nodeInfo(n1Global,0),nodeInfo(n1Global,1)};  //

            for( int m=0; m<4; m++ )
	    {
	      if( (n0Global==faces(ff[m],0) && n1Global==faces(ff[m],1)) ||
                  (n0Global==faces(ff[m],1) && n1Global==faces(ff[m],0)) )
	      {
		fGlobal=ff[m];
		break;
	      }
	    }
	    if( fGlobal==-1 )
	    {
              // no match found -- it could be there is a "duplicate point" that was not
              // detected since previous surfaces only touched at a point, not on a face.

	      // fGlobal=nodeInfo(n0Global,1);

              real distMin=REAL_MAX, distMax=0.;
              int na=-1, nb=-1, fg=-1;
              int m;
	      for( m=0; m<4; m++ ) // there are four possible faces that we could match to.
	      {
                int f0=ff[m];
                if( f0==-1 ) continue;
		
                int n0=faces(f0,0), n1=faces(f0,1);

                for( int m0=0; m0<=1; m0++ )
		{
		  int ma=m0==0 ? n0Global : n1Global;
		  for( int m1=0; m1<=1; m1++ )
		  {
		    int mb=m1==0 ? n0 : n1;
                    if( ma!=mb )
		    {
		      real dist=(fabs(nodes(ma,0)-nodes(mb,0))+
				 fabs(nodes(ma,1)-nodes(mb,1))+
				 fabs(nodes(ma,2)-nodes(mb,2)));
		
		      if( dist<distMin ){ distMin=dist; na=ma; nb=mb; fg=f0; } //

		      distMax=max(distMax,dist);
		    }
		  }
		}

                if( debug & 2 )
         	  printf("match face:check for duplicate: f0=%i=(%i,%i) ([%9.3e,%9.3e,%9.3e],[%9.3e,%9.3e,%9.3e])\n"
                         "  distMin=%8.2e, distMax=%8.2e \n",
		       f0,n0,n1,nodes(n0,0),nodes(n0,1),nodes(n0,2),nodes(n1,0),nodes(n1,1),nodes(n1,2),distMin,
                         distMax);
                

	      }
              // *wdh* 010917 if( distMin<.001*distMax ) // we expect the duplicate nodes to be close relative the the face length
              if( distMin<mergeTolerance ) // 010917 : use merge tolerance
	      {
                printf(" ***INFO*** nodes %i and %i appear to be the same node, dist=%e, distMax=%e\n",
                     na,nb,distMin,distMax);
                assert( fg!=-1 );
                fGlobal=fg;

                // we should replace all occurences of nb with na (or vice versa)
                // fGlobal -> element -> (nodes,faces) 
                int node0 = na<nb ? na : nb;
                int node1 = na<nb ? nb : na;
		// replace node1 with node0
                // int ee = faceElement(fGlobal,0);
                if( numberOfDuplicateNodes>= duplicateNodes.getLength(1) )
		{
		  duplicateNodes.resize(2,numberOfDuplicateNodes+100);
		}
		
		duplicateNodes(0,numberOfDuplicateNodes)=node1;
		duplicateNodes(1,numberOfDuplicateNodes)=node0;
		numberOfDuplicateNodes++;

		// duplicateNodes(0,numberOfDuplicateNodes)=node0;  // add this too if we don't replace duplicates
		// duplicateNodes(1,numberOfDuplicateNodes)=node1;
		// numberOfDuplicateNodes++;
		
                int e0 = sFaceElements(f,0)+numberOfElements; // element on new surface
                int e1=faceElements(fGlobal,0);               // element on global surface
                
                if( true ) 
		{
		  printf(" replace node1=%i with node0=%i in elements e0=%i and e1=%i\n",node1,node0,e0,e1);
		  // Here is a start -- more to do
		  for( int m=0; m<3; m++ )
		  {
		    if( elements(e0,m)==node1 )
		      elements(e0,m)=node0;
		    if( elements(e1,m)==node1 )
		      elements(e1,m)=node0;
		  }
		  if( n0Global==node1 )
		    n0Global=node0;
		  if( n1Global==node1 )
		    n1Global=node0;
		
		}
		
	      }
	      else
	      {
                printf(" ***ERROR*** unable to match a face! n0Global=%i, n1Global=%i, distMin=%e, distMax=%e\n",
                        n0Global,n1Global,distMin,distMax);
	      }
	      
              if( fGlobal==-1 )
	      {
		printf("****ERROR*** no common face n0Global=%i ->faces (%i,%i) n1Global=%i ->faces (%i,%i)\n",
		       n0Global,nodeInfo(n0Global,0),nodeInfo(n0Global,1),n1Global,
		       nodeInfo(n1Global,0),nodeInfo(n1Global,1));
                
                fGlobal=nodeInfo(n0Global,1);  // do this for now
	      }

	    }
	  }
	  
	}
        if( fGlobal<0 )
	{
	  printf("****MAJOR ERROR: unable to determine fGlobal for subsurface s=%i, face f=%i\n",s,f);
	  continue;
	}
	
	
        faceTranslation(f)=fGlobal;
	
        // assign the element that is next to the face on the global grid which is now matched to face f
        assert( sFaceElements(f,0)>=0 );
        int e0 = sFaceElements(f,0)+numberOfElements;
        // assert( faceElements(fGlobal,1)==-1 );
        if( faceElements(fGlobal,1)!=-1 )
	{
	  printf("***ERROR*** faceElements(fGlobal=%i,1)!=-1, s=%i, \n"
                 " The face we are inserting has nodes n0Global=%i, n1Global=%i which are already in the\n"
                 " global triangulation. We are looking for the boundary face in the global triangulation\n"
                 " which has these nodes; We found fGlobal=%i but this face already has two elements as\n"
                 " neighbours, faceElements(fGlobal,0:1)=(%i,%i) surfaces=(%i,%i)\n"
                 " -- this error may be caused by the merge tolerance being too small, an edge may be then be\n"
                 " split when it should not be split, leaving a small triangle\n",
                 fGlobal,s,n0Global,n1Global,fGlobal,faceElements(fGlobal,0),faceElements(fGlobal,1),
                    elementSurface(faceElements(fGlobal,0)),elementSurface(faceElements(fGlobal,1)));
	  
	  int ff[4]={nodeInfo(n0Global,0),nodeInfo(n0Global,1),nodeInfo(n1Global,0),nodeInfo(n1Global,1)};  //
	  int f0=ff[0], f1=ff[1], f2=ff[2], f3=ff[3];
          printf("Nodes n0Global=%i, n1Global=%i are connected to these faces:\n"
		 " f0=%i=(%i,%i) f1=%i=(%i,%i) f2=%i=(%i,%i) f3=%i=(%i,%i) \n",n0Global,n1Global,
		 f0,faces(f0,0),faces(f0,1),
		 f1,faces(f1,0),faces(f1,1),
		 f2,faces(f2,0),faces(f2,1),
		 f3,faces(f3,0),faces(f3,1));

	}
        faceElements(fGlobal,1)=e0;

	int e1=faceElements(fGlobal,0);
	
        // check orientation of elements e0 and e1
        int ea=e0, eb=e1;
	int na=n0Global, nb=n1Global;
	int a0 = elements(ea,0)==na ? 0 : elements(ea,1)==na ? 1 : 2;
	int a1 = elements(ea,0)==nb ? 0 : elements(ea,1)==nb ? 1 : 2;

        if( elements(ea,a0)!=na )
	{
	  if( !duplicateNodeFound(a0,na,ea,elements,duplicateNodes,numberOfDuplicateNodes) )
	  {
  	    printf("+++ ERROR: unable to find node na=%i in element ea=%i\n",na,ea);
            printf(" element ea=%i: nodes=(%i,%i,%i) \n",ea,elements(ea,0),elements(ea,1),elements(ea,2));
	  }
	}
        if( elements(ea,a1)!=nb )
	{
	  if( !duplicateNodeFound(a1,nb,ea,elements,duplicateNodes,numberOfDuplicateNodes) )
	  {
    	    printf("+++ ERROR: unable to find node nb=%i in element ea=%i\n",nb,ea);
            printf(" element ea=%i: nodes=(%i,%i,%i) \n",ea,elements(ea,0),elements(ea,1),elements(ea,2));
	  }
	}
	


	int b0 = elements(eb,0)==na ? 0 : elements(eb,1)==na ? 1 : 2;
	int b1 = elements(eb,0)==nb ? 0 : elements(eb,1)==nb ? 1 : 2;
        if( elements(eb,b0)!=na )
	{
	  if( !duplicateNodeFound(b0,na,eb,elements,duplicateNodes,numberOfDuplicateNodes) )
	  {
    	    printf("+++ ERROR: unable to find node na=%i in element eb=%i\n",na,eb);
            printf(" element eb=%i: nodes=(%i,%i,%i) \n",eb,elements(eb,0),elements(eb,1),elements(eb,2));

	    if( numberOfDuplicateNodes>0 )
	      display(duplicateNodes(Range(0,1),Range(numberOfDuplicateNodes)),"duplicateNodes");
            else
              printf(" **There are no duplicate nodes currently found\n");
	    

//           printf("n0=%i, n0Global=%i, n1=%i,n1Global=%i, fGlobal=%i -> (%i,%i)\n",n0,n0Global,n1,n1Global,
//                     fGlobal,faces(fGlobal,0),faces(fGlobal,1));
	  }
	}
        if( elements(eb,b1)!=nb )
	{
	  if( !duplicateNodeFound(b1,nb,eb,elements,duplicateNodes,numberOfDuplicateNodes) )
	  {
	    printf("+++ ERROR: unable to find node nb=%i in element eb=%i\n",nb,eb);
            printf(" element eb=%i: nodes=(%i,%i,%i) \n",eb,elements(eb,0),elements(eb,1),elements(eb,2));
            if( numberOfDuplicateNodes>0 )
	      display(duplicateNodes(Range(0,1),Range(numberOfDuplicateNodes)),"duplicateNodes");
            else
              printf(" **There are no duplicate nodes currently found\n");
	    
	  }
	  
	}

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
	    printf("**ERROR** Consistency error for normals on surface %i and surface %i \n",surfa,surfb);
            printf(" signForNormal(s=%i)=%i, signForNormal(s=%i)=%i\n",surfa,signa,surfb,signb);
	    printf(" orientation of element ea=%i is c1=%i, orientation of element eb=%i is c2=%i\n",ea,c1,eb,c2);
	    
	    printf(" element a=%i, nodes=(%i,%i,%i), element b=%i nodes=(%i,%i,%i)\n"
		   "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
		   "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
		   "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
		   "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
		   "     node %i : (%11.5e,%11.5e,%11.5e)  \n"
		   "     nodes %i : (%11.5e,%11.5e,%11.5e)  \n",
		   ea,elements(ea,0),elements(ea,1),elements(ea,2),
		   eb,elements(eb,0),elements(eb,1),elements(eb,2),
		   elements(ea,0),nodes(elements(ea,0),0),nodes(elements(ea,0),1),nodes(elements(ea,0),2),
		   elements(ea,1),nodes(elements(ea,1),0),nodes(elements(ea,1),1),nodes(elements(ea,1),2),
		   elements(ea,2),nodes(elements(ea,2),0),nodes(elements(ea,2),1),nodes(elements(ea,2),2),
		   elements(eb,0),nodes(elements(eb,0),0),nodes(elements(eb,0),1),nodes(elements(eb,0),2),
		   elements(eb,1),nodes(elements(eb,1),0),nodes(elements(eb,1),1),nodes(elements(eb,1),2),
		   elements(eb,2),nodes(elements(eb,2),0),nodes(elements(eb,2),1),nodes(elements(eb,2),2)
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
	
      }
      
    }

    // *** assign elementFaces ***
    int e,eg=numberOfElements;
    for( e=0; e<sNumberOfElements; e++,eg++ )
    {
      elementFaces(eg,0)=faceTranslation(sElementFaces(e,0));
      elementFaces(eg,1)=faceTranslation(sElementFaces(e,1));
      elementFaces(eg,2)=faceTranslation(sElementFaces(e,2));
    }

    // *** assign rNodes and elementCoordinates
    realArray & rt = rCoordinates[s];
    j=numberOfNodes;
    numberOfCoordinateNodes+=sNumberOfNodes;
    for( i=0; i<sNumberOfNodes; i++,j++ )
    {
      rNodes(j,0)=rt(i,0);
      rNodes(j,1)=rt(i,1);
    }
    
    const int rNodeOffset=numberOfNodes;
    j=numberOfElements;
    for( i=0; i<sNumberOfElements; i++,j++ )
    {
      elementCoordinates(j,0)=sElements(i,0)+rNodeOffset;
      elementCoordinates(j,1)=sElements(i,1)+rNodeOffset;
      elementCoordinates(j,2)=sElements(i,2)+rNodeOffset;
    }


    numberOfNodes+=uns.getNumberOfNodes();
    numberOfElements+=uns.getNumberOfElements();

    
  }  // end for s
  
    
  int numberOfInconsistencies=sum(inconsistent);
  int numberOfConsistencies=sum(consistent);
  gi.outputString(sPrintF(buf,"-----> number of consistencies=%i, number of inconsistencies=%i <------",
           numberOfConsistencies,numberOfInconsistencies));
  if( numberOfInconsistencies>0  )
  {
    printf("CompositeSurface::WARNING: There were inconsistencies found, number=%i\n",numberOfInconsistencies);
    for( s=0; s<numberOfSurfaces; s++ )
    {
      for( int s2=s+1; s2<numberOfSurfaces; s2++ )
      {
	if( inconsistent(s,s2)>0 )
	{
	  gi.outputString(sPrintF(buf, "WARNING: surface s=%i, s2=%i : number of consistencies=%i,"
				  " number of inconsistencies=%i", s,s2,consistent(s,s2),inconsistent(s,s2)));
	}
      }
    }
    printf("*** It could be that reducing the stitching tolerance will fix the inconsistencies\n");
    
  }
  if( debug & 2 )
    ::display(signForNormal,"signForNormal");

  // flip orientation of elements on surfaces with a normal in the wrong direction
  if( sum(signForNormal<0) > 0 )
  {
    printf("flip some elements to obtain a consistent normal\n");
    for( int e=0; e<numberOfElements; e++ )
    {
      if( signForNormal(elementSurface(e))<0 )
      {
	int temp=elements(e,1);
	elements(e,1)=elements(e,2);
	elements(e,2)=temp;
	temp=elementFaces(e,0);
	elementFaces(e,0)=elementFaces(e,2);
	elementFaces(e,2)=temp;
// AP: Also need to flip elementCoordinates!!!
	temp=elementCoordinates(e,1);
	elementCoordinates(e,1)=elementCoordinates(e,2);
	elementCoordinates(e,2)=temp;
      }
    }
  }

  signForNormal=min(1,max(-1,signForNormal));

  assert( numberOfNodes==totalNumberOfNodes && numberOfElements==totalNumberOfElements );
  printf(" number of nodes on all surfaces=%i, number of nodes on global =%i\n",numberOfNodes,globalNumber);

  if( false )
  {
    intArray & ef = elementFaces;
    for( int e=0; e<numberOfElements; e++ )
    {
      const int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);    // faces on this element
      // adjacent elements
      int ae0 = faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
      int ae1 = faceElements(f1,0)==e ? faceElements(f1,1) : faceElements(f1,0);
      int ae2 = faceElements(f2,0)==e ? faceElements(f2,1) : faceElements(f2,0);

      printf("element %i: nodes=(%i,%i,%i), faces=(%i,%i,%i), adjacent elements=(%i,%i,%i)\n",
	      e,elements(e,0),elements(e,1),elements(e,2),ef(e,0),ef(e,1),ef(e,2),ae0,ae1,ae2);
    }
  
    for( int f=0; f<numberOfFaces; f++ )
    {
      printf("face %i: nodes (%i,%i) next to elements %i and %i\n",f,faces(f,0),faces(f,1),
	      faceElements(f,0),faceElements(f,1));
    }
    
  }
  

// OLD triangulation improvement...
  // *** Improve the triangulation here **** 
  //    add nodes to resolve curvature etc.
  numberOfNodes=globalNumber;
  numberOfFaces=globalNumberOfFaces;
  if( false )
  {
    real maxArea = maximumArea < REAL_MAX ? maximumArea : .05; // , dist; // hardwired for now
    // real maxDist=.01*deltaS;
    gi.outputString("Improving triangulation...");
    improveTriangulation( cs, maxDist, maxArea,
			  numberOfNodes, nodes,
			  numberOfElements,elements,
			  numberOfFaces,faces,
			  faceElements,
			  elementFaces,
			  elementSurface,
			  elementCoordinates, 
                          numberOfCoordinateNodes,
			  rNodes);

    elements.resize(numberOfElements,3);
    elementSurface.resize(numberOfElements);
    faces.resize(numberOfFaces,2);
    
  }

  nodes.resize(numberOfNodes,3);

// remove any existing triangulation
  if( globalTriangulation && globalTriangulation->decrementReferenceCount()==0 )
    delete globalTriangulation;
  
  globalTriangulation = new UnstructuredMapping;
  globalTriangulation->incrementReferenceCount();

  if( false )
  {
    globalTriangulation->setNodesAndConnectivity(nodes,elements);
  
    printf("\n\n *************************** from setNodesAndConnectivity \n");
  
    globalTriangulation->printConnectivity();
  }
  else
  {


    if( numberOfDuplicateNodes>0 )
    {
      // check for faces that we have missed in replacing duplicate nodes

      for( int e=0; e<numberOfElements; e++ )
      {
        checkElement( e, numberOfNodes,numberOfFaces,elements,faces,elementFaces,faceElements,
                      numberOfDuplicateNodes,duplicateNodes,debug );

//  /* -----
//  	const int n0=elements(e,0), n1=elements(e,1), n2=elements(e,2);
//  	if( n0<0 || n0>=numberOfNodes || n1<0 || n1>=numberOfNodes || n2<0 || n2>=numberOfNodes )
//  	{
//  	  printf("check:ERROR for element e=%i, nodes=(%i,%i,%i). "
//  		 "Node numbers out of range, numberOfNodes=%i\n",e,n0,n1,n2,numberOfNodes);
//  	}
//          for( int m=0; m<3; m++ )
//  	{
//  	  const int f0=elementFaces(e,m);
//  	  if( f0<0 || f0>=numberOfFaces )
//  	  {
//  	    printf("check:ERROR for element e=%i, face=%i. "
//  		   "Face numbers out of range, numberOfFaces=%i\n",e,f0,numberOfNodes);
//  	  }
//  	  else
//  	  {
//              for( int side=0; side<=1; side++ )
//  	    {
//  	      int faceNode=faces(f0,side);
//  	      if( faceNode!=n0 && faceNode!=n1 && faceNode!=n2 )
//  	      {
//  		// check for a duplicate node
//  		for( int n=0; n<numberOfDuplicateNodes; n++ )
//  		{
//  		  if( duplicateNodes(0,n)==faceNode )
//  		  {
//                      faces(f0,side)=duplicateNodes(1,n);
//                      if( debug & 2 )
//     		      printf("INFO: replace node %i with node %i in face %i (e=%i).\n",faceNode,faces(f0,side),f0,e);
                    
//                      // If we change a face we need to go back and check elements connected to this face
//  		    for( int nn=0; nn<=1; nn++ )
//  		    {
//  		      int ee = faceElements(f0,nn); // check the elements next to this face
//                        for( int mm=0; mm<=2; mm++ )
//  		      {
//  			if( elements(ee,mm)==faceNode )
//  			{
//  			  elements(ee,mm)=duplicateNodes(1,n);
//                            if( debug & 2 )
//                               printf("INFO: replace node %i with node %i in element %i.\n",faceNode,faces(f0,side),ee);
//  			}
//  		      }
//  		    }
//  		  }
//  		}
//  	      }
//  	    }
//  	  }
//  	}
//  	--------- */
      }
    }
    


    int numberOfBoundaryfaces=-1;
    globalTriangulation->setNodesAndConnectivity(nodes,elements,faces,faceElements,elementFaces,
                      numberOfFaces, numberOfBoundaryfaces, domainDimension, true);

    globalTriangulation->setTags(elementSurface);

    if( debugs & 4 )
    {
      printf("\n\n *************************** after setNodesAndConnectivity \n");
      globalTriangulation->printConnectivity();
    }

    if( false )
      globalTriangulation->checkConnectivity();
    
  } // end checking elements and setting nodes and connectivity
  
  
  printf(" Final triangulation: numberOfBoundaryFaces =%i ",globalTriangulation->getNumberOfBoundaryFaces());
  printf("\n Total time to build global triangulation =%8.2e\n",getCPU()-time0);  
//   int e;
//   for( e=0; e<numberOfElements; e++ )
//   {
//     printf("");
//   }
  
// cleanup temporaries
  delete [] rCoordinates;
  delete [] boundaryNodeInfop;
  delete [] edgeNodeInfop;

  return 0;
  
} // end triangulateCompositeSurface


// ==============================================================================================
//! Determine the number of grid points to put on a curve.
/*!
   \param arcLength (input) : arcLength of the curve defined by the grid points x.
   \param x (input) :
   \param maxCurvature (output):
   \return numberOfGridPoints : number of nodes that will appear on the curve when it is
   triangulated -- this will be different from the number of points we use to
   approximate the curve with
 */ 
// ===============================================================================================
int CompositeTopology::
computeNumberOfGridPoints( real arcLength, realArray & x, real & maxCurvature )
{
  
  int debug=3;

  int numberOfGridPoints;
  
  // this seems to work ok
  Range I(x.getBase(0)+1,x.getBound(0)-1);

  maxCurvature=max( (fabs(x(I+1,0)-2.*x(I,0)+x(I-1,0))+
		     fabs(x(I+1,1)-2.*x(I,1)+x(I-1,1))+
		     fabs(x(I+1,2)-2.*x(I,2)+x(I-1,2))) );
	      

  int numForArcLength=int( arcLength/deltaS );
  // curvatureResolution/sqrt(curvatureTolerance) is approx number of points on a circle due to curvature only (?)
  const int curvatureResolution=10; 
  //    int numForCurvature=curvatureResolution*sqrt(maxCurvature/(curvatureTolerance*arcLength));
  int numForCurvature=int( curvatureResolution*(maxCurvature/(curvatureTolerance*arcLength)) );
  
//  numberOfGridPoints = max(minNumberOfPointsOnAnEdge,numForArcLength,numForCurvature);
  numberOfGridPoints = max(minNumberOfPointsOnAnEdge,numForArcLength);
  
  
  return numberOfGridPoints;
}


// parameterizeTrimCurvesByArclength : set to true if max(ds)/min(ds) > 10. in which case
// we parameterize by arclength as opposed to uniform spacing.
static bool parameterizeTrimCurvesByArclength=false;

// If true merge semi-short edges with a neighbouring edge.
static bool mergeShortEdges=true;


// ====================================================================================================
//!  Build an edge curve by interpolation of points on the sub-surface.
/*! 
    Here we build both the edgeCurve (saved in edge) and the set of points used in the triangulation
   of the edge.
  
   \param trimCurve (input) : this edge curve is from a trimming curve (or a boundary curve for untrimmed surfaces)
   \param surface (input) : Mapping that defines the sub-surface.
   \param edge (output) : If not NULL then an edge curve was built AND the referenceCount was incremented. 
                          If NULL no edge curve was built.
      since the arcLength of the curve was too small. 
   \param numberOfGridPoints (output) : number of points on the curve to use for the triangulation, based on
     the current value of deltaS
   \param arcLength (output): the arc-length of the edge curve
 */
// ====================================================================================================
int CompositeTopology::
buildEdgeSegment(Mapping & trimCurve,
                 Mapping & surface,
                 NurbsMapping *&edge,
                 int & numberOfGridPoints,
                 real & arcLength,
                 int & debug,
                 GenericGraphicsInterface & gi )
{
  realArray r, r2;
  Range I, R3=3;

  int num;
  r = trimCurve.getGrid(); 
  num=r.getLength(0);
  I=r.dimension(0);
  int rDim=r.getLength(3); // should be 2 but readMappings currently may build a line as 3d
  r.reshape(I,rDim);

  if( debug & 32 )
  {
    GraphicsParameters params;
    params.set(GI_TOP_LABEL,"buildEdgeSegment: initial trim curve");
    gi.erase();
    PlotIt::plot(gi,trimCurve,params);
  }

  realArray x(I,3);
  surface.map(r,x);

  // compute the arc-length
  Range Im=Range(I.getBase(),I.getBound()-1);
  realArray ds;

  // if we are improving the triangulation then use the L2 arc length estimate
  if ( improveTri && true)
    ds= sqrt(pow(fabs(x(Im+1,0)-x(Im,0)),2)+pow(fabs(x(Im+1,1)-x(Im,1)),2)+pow(fabs(x(Im+1,2)-x(Im,2)),2));
  else
    ds= fabs(x(Im+1,0)-x(Im,0))+fabs(x(Im+1,1)-x(Im,1))+fabs(x(Im+1,2)-x(Im,2));
  
  arcLength=sum( ds );
  // if mergeShortEdges==true only discard edges < mergeTolerance*.01, otherwise discard edges < mergeTolerance
  if( (!mergeShortEdges && (arcLength < mergeTolerance) ) ||
      ( mergeShortEdges && (arcLength < mergeTolerance*.01) ) )
  {
    edge=NULL;
    return 1;
  }

  real dsMax=max(ds);
  real dsMin=min(ds);
  real dsRatio = dsMax/max(dsMax*1.e-4,dsMin);
  
  if( dsRatio>10. )
  {
    if( debug & 2 )
      printf("***buildEdgeSegment: dsMin=%8.2e, dsMax=%8.2e : dsMax/dsMin=%8.2e : use arclength parameterization\n",
          dsMin,dsMax,dsRatio);
    parameterizeTrimCurvesByArclength=true;
  }
  else
  {
    parameterizeTrimCurvesByArclength=false;
  }
  

  // numberOfGridPoints : number of nodes that will appear on the curve when it is
  // triangulated -- this will be different from the number of points we use to
  // approximate the curve with
  real maxCurvature;
  numberOfGridPoints=computeNumberOfGridPoints(arcLength,x,maxCurvature);
  
  // *** compute an estimate of the number of points needed based on curvature/arclength *****

  const int maxNumberOfPoints=601; // **** 301;  // ************************************

  real tol=1.e-3;                         // these should be parameters in the class *******************************

  // const real tolerance=arcLength*tol;
  const real tolerance=arcLength*curveResolutionTolerance;
  num = int( min(sqrt(maxCurvature/tolerance)*num, real(maxNumberOfPoints)) );

  num=max(num,minNumberOfPointsOnAnEdge);

  if( false && parameterizeTrimCurvesByArclength && dsRatio>10. ) // *******************************
  {
    num=int( min(num*sqrt(dsRatio), real(maxNumberOfPoints*2)) );
    printf("Increase number of points on curve to %i\n",num);
  }
  
  // *wdh* num=max(numberOfGridPoints,num);   // shouldn't do this since it makes curve resolution depend on deltaS

  if( false )
    printf("buildEdgeSegment: edge %i, arcLength=%8.2e, max(x'')=%8.2e, deltaS=%8.2e, "
	   "recompute:num=%i (grid points=%i)\n",
	   numberOfEdgeCurves,arcLength,maxCurvature,deltaS,num,numberOfGridPoints);

  // **** re-evaluate the curve with the new number of  grid points  ****
  I=num;
  r.redim(I,2); x.redim(I,3);
  r2.redim(I,1);
  real dr=1./(num-1);
  r2(I,0).seqAdd(0.,dr);
  r2(I.getBound(),0) = 1.;

  trimCurve.map(r2,r);
  if( debug & 32 )
  {
    GraphicsParameters params;
    params.set(GI_TOP_LABEL,"buildEdgeSegment: new trim curve r points");
    gi.plotPoints(r,params);
  }
  surface.map(r,x);

  if( debug & 16 )
  {
      
    Mapping & referenceSurface = surface.getClassName()=="TrimmedMapping" ? 
      *((TrimmedMapping&)surface).untrimmedSurface() : surface;
    r2.redim(I,2);
    r2=-1.;
    referenceSurface.inverseMap(x,r2);
    for( int i=0; i<num; i++ )
    {
      real diff=fabs(r(i,0)-r2(i,0))+fabs(r(i,1)-r2(i,1));
      int ip = i<num-1 ? i+1 : i-1;
      real arc=fabs(x(ip,0)-x(i,0))+fabs(x(ip,1)-x(i,1))+fabs(x(ip,2)-x(i,2));
	
//          printf("i=%i, r=(%9.3e,%9.3e) -> x=(%9.3e,%9.3e,%9.3e) -> r2=(%9.3e,%9.3e) diff(r)=%8.2e arc=%8.2e\n",
//  	       i,r(i,0),r(i,1),x(i,0),x(i,1),x(i,2),r2(i,0),r2(i,1),diff,arc);
    }
  }

  edge = new NurbsMapping();  edge->incrementReferenceCount();

  // **** build the edge curve by interpolation ****
  // specify the parameterization as r2 (i.e. do not reparameterize by arclength)

  int order=2;  // lower order interpolation is faster.
  if( trimCurve.getClassName()=="NurbsMapping" )
  {
    // use the same order as the trim curve -- NB for order==1 -- there may be corners (asmo)
    // ** order =((NurbsMapping&)(*trimCurve)).getOrder();

    order =min(((NurbsMapping&)trimCurve).getOrder(),2);   // use at most 2nd order *wdh* 018031
  }

  // option : 0=use parameterization, 1=return parameterization
  int parameterizationOption= parameterizeTrimCurvesByArclength ? 1 : 0;  
  
  ((NurbsMapping*)edge)->interpolate(x,parameterizationOption,r2,order);

  edge->setGridDimensions(axis1,numberOfGridPoints);  // set number of points for plotting/triangulation.

  // printf("$$$$buildEdgeSegment: trimCurve.getIsPeriodic=%i\n",(int)trimCurve.getIsPeriodic(axis1));
  edge->setIsPeriodic(axis1,trimCurve.getIsPeriodic(axis1)); // *wdh* 030825
  

// tmp
//    const realArray & xg = edge->getGrid();
//    int nxg = xg.getBound(0);

//    printf("From getGrid(): first point (%e, %e, %e), last point (%e, %e, %e)\n", 
//  	   xg(0,0), xg(0,1), xg(0,2), xg(nxg,0), xg(nxg,1), xg(nxg,2));
  

  if( debug & 16 )
  {
    GraphicsParameters params;
    params.set(GI_TOP_LABEL,"buildEdgeSegment: new trim curve x-space");
    gi.erase();
    PlotIt::plot(gi,*edge,params);

    
    if( false && debug & 16 )
    {
      
      Mapping & referenceSurface = surface.getClassName()=="TrimmedMapping" ? 
                        *((TrimmedMapping&)surface).untrimmedSurface() : surface;
      realArray xx; xx= edge->getGrid();
      Range I=xx.dimension(0);
      xx.reshape(I,3);
      int num=I.getLength();
      realArray rr(I,2), x2(I,3);
      rr=-1.;
      Mapping::debug=63;
      referenceSurface.inverseMap(xx,rr);
      referenceSurface.map(rr,x2);
      Mapping::debug=0;
      for( int i=0; i<num; i++ )
      {
        real diff=fabs(xx(i,0)-x2(i,0))+fabs(xx(i,1)-x2(i,1))+fabs(xx(i,2)-x2(i,2));
	
        printf("i=%i, xx=(%9.3e,%9.3e,%9.3e)->rr=(%9.3e,%9.3e)->x2=(%9.3e,%9.3e,%9.3e) diff(x)=%8.2e\n",
	       i,xx(i,0),xx(i,1),xx(i,2),rr(i,0),rr(i,1),x2(i,0),x2(i,1),x2(i,2),diff);
      }

      gi.erase();
      params.set(GI_TOP_LABEL,"r coord's of edge curve grid");
      gi.plotPoints(rr,params);
    }

  }


//   if( false && parameterizationOption==1 )
//   {
//     // return the parameterization 
//     // *** fit a spline to the arclength parameterization used by the NURBS
//     //     We then need to find parameter values on the orginal curve corresponding
//     //     to equi-spaced point in arclength.
//     SplineMapping spline;
//     spline.setPoints(r2);
//     spline.setShapePreserving();
//     spline.setGridDimensions(axis1,num);
//     spline.useRobustInverse();
    
//     // PlotStuff & gi = *Overture::getGraphicsInterface();
//     // spline.interactiveUpdate(gi);

//     realArray r3(numberOfGridPoints);
//     real dr=1./(numberOfGridPoints-1);
//     r3.seqAdd(0.,dr);

//     r2.redim(numberOfGridPoints);
//     r2=-1.; // r3;  // initial guess
//     spline.inverseMap(r3,r2);

//     r2=r3;
    
//     // ::display(r2,"r2 after spline.inverseMap(r3,r2)");
    
//   }
   

  return 0;
}




// ============================================================================================
//!  Build edge curves for all sub-surfaces.
/*!
    An edge curve is a 3D (physical space) representation of a trimming sub-curve.
 */
// ===========================================================================================
int CompositeTopology::
buildEdgeCurves( GenericGraphicsInterface & gi )
{
  
  real time0=getCPU();
  
  int debug=0;

// get rid of the previous virtual topology datastructure
  cleanup();
  
// safer to use numberOfFaces if the CompositeSurface object cs changes
  numberOfFaces = cs.numberOfSubSurfaces(); 
// allocate a new data structure for the virtual topology
  faceInfoArray = new FaceInfo[numberOfFaces];

// outside this function

  triangulationSurface = new UnstructuredMapping *[numberOfFaces];
  for( int s=0; s<numberOfFaces; s++ )
  {
    triangulationSurface[s]=NULL;
  }


  NurbsMapping *newNurbs;

  numberOfEdgeCurves=0;
  int surfaceStartEdge;

// dimension the endPoint and masterEdge arrays
  numberOfEndPoints = 0;
// this is a conservative estimate since all untrimmed mappings have 4 edges
  masterEdge.resize(cs.numberOfSubSurfaces()*4); 
  endPoint.redim(cs.numberOfSubSurfaces()*4, 3);
  
  int numberOfGridPoints=0, q, pt, next;
  real arcLength;
  Range R3=3;

  int s;
  bool skippedFirst, skippedPrevious;
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    Mapping & map = cs[s];
    
    if( map.getIsPeriodic(axis1)==Mapping::functionPeriodic ||
        map.getIsPeriodic(axis2)==Mapping::functionPeriodic )
    {
      printf(" ******* CompositeTopology::buildEdgeCurves:WARNING: surface number %i is periodic. ***********\n"
             "    You may need to split this surface into two pieces since the topology does not\n"
             "    entirely support periodic Surfaces. This may still work if the surface is trimmed.\n",s);
      
    }
    
    const bool trimmedMapping = map.getClassName()=="TrimmedMapping";

    surfaceStartEdge=numberOfEdgeCurves;   // first edge on this surface
    if( !trimmedMapping )
    {
// only one loop for untrimmed mappings
      faceInfoArray[s].allocateLoops(1);
      Loop & currentLoop = faceInfoArray[s].loop[0];
      currentLoop.trimOrientation = 1;
      
      skippedFirst=false;
      skippedPrevious=false;
      
// make a square in parameter space
      NurbsMapping & nurb = * new NurbsMapping(1,2);
      nurb.incrementReferenceCount();
      constructOuterBoundaryCurve(& nurb);

      for( int b=0; b<4; b++ )
      {
	NurbsMapping & trimCurve = nurb.subCurve(b);
//	printf("buildEdgeCurve: untrimmed, reference counts tc: %i, sc[%i]: %i\n", nurb.getReferenceCount(), b, trimCurve.getReferenceCount());
	// treat faces in a counter-clockwise order
	//  (side,axis) = (0,1), (1,0), (1,1) (0,0)
	int axis = (b+1) % 2;
	int side=  ((b+1)/2) %2;
	
// If the Mapping is a NurbsMapping then we can directly build the exact curves on the
// boundaries.
// AP: But the end points of the resulting curves can not always be moved with 
// NurbsMapping::moveEndpoint(), so we don't use this method

//	printf("Building edge curves on an untrimmed surface\n");
	NurbsMapping *edge=NULL;
	buildEdgeSegment(trimCurve,map,edge,numberOfGridPoints,arcLength,debug,gi);

	if( edge!=NULL )
	{
	  newNurbs = edge;
//	    printf("The order of curve on the non-NURBS surface is %i\n", newNurbs->getOrder());
//  	  printf("buildEdgeCurve: untrimmed, after buildEdgeSegment: reference count map: %i\n", 
//  		 edge->getReferenceCount());
	}
	else
	{
	  printf(" s=%i, skipping short edge curve %i on an untrimmed %s, arcLength=%8.2e\n",s,
		 numberOfEdgeCurves, SC map.getClassName(), arcLength);
	  if (b==0) 
	    skippedFirst = true;
	  else
	    skippedPrevious = true;
	  continue;
	}
	
// save starting point info
	const realArray & x = newNurbs->getGrid();
	
	if (endPoint.getLength(0) <= numberOfEndPoints)
	{
	  masterEdge.resize(numberOfEndPoints+1000);
	  endPoint.resize(numberOfEndPoints+1000,3);
	}
	  
	for (q=0; q<3; q++)
	  endPoint(numberOfEndPoints, q) = x(0, 0, 0, q); // starting point

// check if the last curve segment was too short: Then we need to adjust the ending point of the previous 
// curve to match with the starting point of next curve. Special case if the first segment is skipped:
// then we need to adjust the ending point of the last segment (see below)
	if (skippedPrevious)
	{
//	  printf("Adjusting the ending point of the previous segment since we skipped a segment\n");
	  CurveSegment *lastCurve=currentLoop.lastEdge->curve;
	  const realArray & x=lastCurve->getNURBS()->getGrid();
	  int n = x.getLength(0)-1;
	  RealArray newLocation(3);
	  for (q=0; q<3; q++)
	    newLocation(q) = endPoint(numberOfEndPoints,q);
//	  printf("Present endpoint: (%e,%e,%e)\nnewLocation: (%e,%e,%e)\n", x(n,0,0,0), x(n,0,0,1), x(n,0,0,2),
//		 newLocation(0),  newLocation(1),  newLocation(2));
	
	  lastCurve->getNURBS()->moveEndpoint(1, newLocation);
// check the result
	  const realArray & y=lastCurve->getNURBS()->getGrid();
	  n=y.getLength(0)-1;
//	  printf("Resulting curve endpoint: (%e,%e,%e)\n", y(n,0,0,0), y(n,0,0,1), y(n,0,0,2));
	
	}

// make a new curve segment
// the starting point number is "numberOfEndPoints"
// surface s, no trim curve, just a boundary
	CurveSegment *newCurve = new CurveSegment(*newNurbs, numberOfEndPoints, s, &nurb, &trimCurve); 
// we are done with newNurbs!
	if (newNurbs->decrementReferenceCount() == 0)
	  delete newNurbs;
	newNurbs = NULL;

	newCurve->numberOfGridPoints=numberOfGridPoints;
	newCurve->arcLength=arcLength;

// make an EdgeInfo object
	EdgeInfo *newEdge = new EdgeInfo(newCurve, 0, s, 1, numberOfEdgeCurves); // loop is 0, direction is 1
// add it to the loop
	currentLoop.insertEdge( newEdge );
      
	masterEdge.array[numberOfEndPoints] = newEdge; // the newEdge initially rules the starting point
      
	numberOfEndPoints++;
	numberOfEdgeCurves++;
      } // end for b=0,...,3
// assign end point numbers for the currentLoop
      currentLoop.assignEndPointNumbers();
// modify the ending points so they exactly end at the starting point of the next segment!
      RealArray newLocation(3);
      int nE=currentLoop.numberOfEdges(), i, q;
      EdgeInfo *edge;
      for (i=0, edge = currentLoop.firstEdge; i<nE; i++, edge = edge->next)
      {
	for (q=0; q<3; q++)
	  newLocation(q) = endPoint(edge->next->curve->startingPoint,q);
	edge->curve->getNURBS()->moveEndpoint(1, newLocation);
      }

// if we skipped the first segment, we must adjust the ending point of the last segment to coincide with the
// starting point of the first segment  
      if (skippedFirst)
      {
//	printf("Adjusting the ending point of the last segment since the first segment was skipped\n");
	CurveSegment *lastCurve=currentLoop.lastEdge->curve;
	CurveSegment *firstCurve=currentLoop.firstEdge->curve;
	RealArray newLocation(3);
	for (q=0; q<3; q++)
	  newLocation(q) = endPoint(firstCurve->startingPoint,q);
	
	lastCurve->getNURBS()->moveEndpoint(1, newLocation);
      }
// don't need nurb anymore
      if (nurb.decrementReferenceCount()==0)
	delete &nurb;

    } // end if !trimmedMapping
    else
    {
      // *** this is a TrimmedMapping ****

      // The trimming curves are in the parameter space of the surface of the TrimmedMapping.
      // We need to build a curve in cartesian space.
      TrimmedMapping & trim = (TrimmedMapping&)map;

      // printf("edge curves: TrimmedMapping: outerCurve=%i, getNumberOfInnerCurves()=%i, "
      //    "getNumberOfBoundarySubCurves()=%i \n",
      //            trim.getOuterCurve(),trim.getNumberOfInnerCurves(),trim.getNumberOfBoundarySubCurves());

// make one loop for each trimming curve
      faceInfoArray[s].allocateLoops(trim.getNumberOfTrimCurves());

      for ( int i=0; i<trim.getNumberOfTrimCurves(); i++ )
      {
	Loop & currentLoop = faceInfoArray[s].loop[i];
	currentLoop.trimOrientation = trim.trimOrientation(i); // copy the orientation from the trimmedMapping

	Mapping *c = trim.getTrimCurve(i);
	if( c!=0 )
	{
          if( c->getClassName()=="NurbsMapping" )
	  {
            NurbsMapping & nurb = (NurbsMapping&)(*c);

            // printf("surface s=%i, Trim curve %i has %i sub-curves, (order=%i)\n",s,i,nurb.numberOfSubCurves(),
            //  nurb.getOrder());

            // if( nurb.numberOfSubCurves()==1 && nurb.getOrder()==1 )
            if( nurb.getOrder()==1 )
	    {
              // this trim curve may have corners since it is piece-wise linear (?)
              // we may need to split it into pieces
              // angle: split an order 1 nurb where the tangent changes by more than this angle.
              const real angle=40.;  // default is 60 degrees
	      nurb.buildSubCurves(angle);
	      
              if( debug & 2 ) 
	      {
                printf("   ***** order=1 : s=%i, trim curve=%i split into %i sub-curves\n",
                        s,i,nurb.numberOfSubCurves());
	      }
	      
	    }
	    
            int edgeStart=numberOfEdgeCurves;
	    for( int sc=0; sc<nurb.numberOfSubCurves(); sc++ )
	    {
              NurbsMapping & trimCurve = nurb.subCurve(sc);

//  	      printf("buildEdgeCurve: trimmed, reference counts tc: %i, sc[%i]: %i\n", 
//  		     nurb.getReferenceCount(), sc, trimCurve.getReferenceCount());

	      NurbsMapping *edge=NULL;
              assert( trim.surface!=0 );

              // printf("buildEdgeSegment: sc=%i, nurb.numberOfSubCurves()=%i numberOfEdgeCurves=%i\n",
              //    sc,nurb.numberOfSubCurves(),numberOfEdgeCurves);

              real arcLength;
	      buildEdgeSegment(trimCurve,*trim.surface,edge,numberOfGridPoints,arcLength,debug,gi);

	      if( edge!=NULL )
	      {
		newNurbs = edge;
	      }
	      else
	      {
		printf("trim: s=%i, skipping short edge curve %i on a trimmed Mapping. arcLength=%8.2e\n",s,
                        numberOfEdgeCurves,arcLength);
		continue;
	      }     

// save starting point info
	      const realArray & x = newNurbs->getGrid();

	      if (endPoint.getLength(0) <= numberOfEndPoints)
	      {
		masterEdge.resize(numberOfEndPoints+1000);
		endPoint.resize(numberOfEndPoints+1000,3);
	      }
	  
	      for (q=0; q<3; q++)
		endPoint(numberOfEndPoints, q) = x(0, 0, 0, q); // starting point

// make a new sub curve
// surface s, trim curve nurb, sub curve sc
	      CurveSegment *newCurve = new CurveSegment( *newNurbs, numberOfEndPoints, s, &nurb, &trimCurve);
// we are done with newNurbs!
	      if (newNurbs->decrementReferenceCount() == 0)
		delete newNurbs;
	      newNurbs = NULL;
	      
	      
	      newCurve->numberOfGridPoints=numberOfGridPoints;
	      newCurve->arcLength=arcLength;

// make an EdgeInfo object
	      EdgeInfo *newEdge = new EdgeInfo(newCurve, i, s, 1, numberOfEdgeCurves); // loop is "i", direction is 1

// add the new curve to the loop
	      currentLoop.insertEdge( newEdge );

// the newEdge initially rules the starting point
	      masterEdge.array[numberOfEndPoints] = newEdge; 

	      numberOfEndPoints++;
	      numberOfEdgeCurves++;
	    }  // end for sc


	    bool shortEdgeFound=false;
	    
// look for any short edges and attempt to merge with neighbours if they join smoothly.
            if( currentLoop.numberOfEdges() > 1 )
	    {
	      EdgeInfo *currentEdge;
	      int cnt;
	      for( cnt=0, currentEdge=currentLoop.firstEdge; cnt<currentLoop.numberOfEdges(); 
		   currentEdge=currentEdge->next, cnt++ )
	      {
             
// AP: Note that buildEdgeCurves only skips a subcurve if the arclength < 0.01*mergeTolerance
// (when mergeShortEdges=true (global variable))
// On the other hand, here we only skip the edge if it can not be joined with either the
// previous or the next edge.
		if( currentEdge->curve->arcLength < 0.1*mergeTolerance ) // AP: changed factor from 0.5
		{
		  // short edge found 
		  shortEdgeFound=true;
//    		  printf("Before short edge has been joined/deleted: numberOfEdges=%i, next edge=%i,"
//  			 " 2nd next edge=%i\n", currentLoop.numberOfEdges(), currentEdge->next->edgeNumber, 
//  			 currentEdge->next->next->edgeNumber);

		  EdgeInfo *prevEdge, *nextEdge;
		  prevEdge = currentEdge->prev;
		  nextEdge = currentEdge->next;
		  
		  const realArray & xp= prevEdge->curve->getNURBS()->getGrid();      // previous edge
		  const realArray & xc= currentEdge->curve->getNURBS()->getGrid();   // current edge
		  const realArray & xn= nextEdge->curve->getNURBS()->getGrid();      // next edge
		
                  int nc=xc.getBound(0), np=xp.getBound(0);
		  
                  real vp[3], vs[3], ve[3], vn[3];

		  vp[0]=xp(np,0,0,0)-xp(np-1,0,0,0); 
                  vp[1]=xp(np,0,0,1)-xp(np-1,0,0,1); 
                  vp[2]=xp(np,0,0,2)-xp(np-1,0,0,2);  
                  real vpNorm=sqrt(vp[0]*vp[0]+vp[1]*vp[1]+vp[2]*vp[2]);
		  
		  vs[0]=xc(1,0,0,0)-xc(0,0,0,0); 
                  vs[1]=xc(1,0,0,1)-xc(0,0,0,1); 
                  vs[2]=xc(1,0,0,2)-xc(0,0,0,2);
                  real vsNorm=sqrt(vs[0]*vs[0]+vs[1]*vs[1]+vs[2]*vs[2]);
		  ve[0]=xc(nc,0,0,0)-xc(nc-1,0,0,0); 
		  ve[1]=xc(nc,0,0,1)-xc(nc-1,0,0,1); 
		  ve[2]=xc(nc,0,0,2)-xc(nc-1,0,0,2);
                  real veNorm=sqrt(ve[0]*ve[0]+ve[1]*ve[1]+ve[2]*ve[2]);

		  vn[0]=xn(1,0,0,0)-xn(0,0,0,0); 
		  vn[1]=xn(1,0,0,1)-xn(0,0,0,1); 
		  vn[2]=xn(1,0,0,2)-xn(0,0,0,2);
                  real vnNorm=sqrt(vn[0]*vn[0]+vn[1]*vn[1]+vn[2]*vn[2]);
		  
                  real vDotp=(vp[0]*vs[0]+vp[1]*vs[1]+vp[2]*vs[2])/max(REAL_MIN*100.,vpNorm*vsNorm);
		  real vDotn=(ve[0]*vn[0]+ve[1]*vn[1]+ve[2]*vn[2])/max(REAL_MIN*100.,veNorm*vnNorm);

                  printf("Found short edge (e=%i, surface=%i, loop=%i), arcLength=%8.2e < mergeTolerance=%8.2e\n"
                         "  The cosine of angle with previous edge is %8.2e, next edge is=%8.2e\n",
			 currentEdge->edgeNumber, currentEdge->faceNumber, currentEdge->loopNumber, 
			 currentEdge->curve->arcLength, mergeTolerance, vDotp, vDotn);
		  
                  bool ok=true;
		  int dum=0;
                  if( vDotp > vDotn && vDotp > 0.8 )
		  {
// The edge is reasonably in the same direction as the previous edge, join to the previous
		    printf("Joining with the previos edge\n");
// joinEdgeCurves removes currentEdge from the loop!
		    EdgeInfo *next=currentEdge->next;
  		    ok= joinEdgeCurves(*currentEdge, false)==0; // AP: CHECK THIS!
		    currentEdge = next->prev;
		  }
		  else if( vDotn > vDotp && vDotn > 0.8 )
		  {
// The edge is reasonably in the same direction as the next edge, join to the next
		    printf("Joining with the next edge\n");
// joinEdgeCurves removes currentEdge from the loop!
		    EdgeInfo *prev=currentEdge->prev;
  		    ok = joinEdgeCurves(*currentEdge, true)==0; // AP: CHECK THIS!
		    currentEdge=prev->next;
		  }
		  else
		  {
		    printf("Keeping the short edge since it isn't sufficiently aligned with either\n"
			   "the previous nor the next edge\n");
		  }
		  
		  if( !ok )
		  {
		    printf("Deleting the short edge, since joinEdgeCurves failed.\n");

// the next statement will delete currentEdge, which would prevent us from evaluating next on it below
		    currentEdge = currentEdge->prev; 
		    currentLoop.deleteEdge(currentEdge->next); 
		  }
//    		  printf("After short edge has been joined/deleted: numberOfEdges=%i, next edge=%i\n", 
//    			 currentLoop.numberOfEdges(), currentEdge->next->edgeNumber);
		  
		} // end if short edge
		currentEdge=currentEdge->next;
	      } // end for all new edgecurves

	    } // end if more than 1 new edge

// tmp: check the loop if at least one short edge was found
//  	    if (shortEdgeFound)
//  	    {
//  	      printf("INFO: loop with short edge, numberOfEdges=%i:\n", currentLoop.numberOfEdges());
//  	      EdgeInfo *currentEdge;
//  	      int cnt;
//  	      for( cnt=0, currentEdge=currentLoop.firstEdge; cnt<currentLoop.numberOfEdges(); 
//  		   currentEdge=currentEdge->next, cnt++ )
//  	      {
//  		printf("Edge %i with number %i, startpoint=%i, endpoint=%i\n",cnt, currentEdge->edgeNumber,
//  		       currentEdge->getStartPoint(), currentEdge->getEndPoint());
//  	      }
//  	    }
// end tmp	    
	    
	  } // end if NURBS trimming curve
	  else
	  {
// ** trim curve is not a NURBS
            Overture::abort("error: trim curve is not a NURBS");
	  }// end if not a nurbs trimming curve
	  
	} // end if c!=0

// assign end point numbers for the currentLoop
	currentLoop.assignEndPointNumbers();
// modify the ending points so they exactly end at the starting point of the next segment!
	RealArray newLocation(3);
	int nE=currentLoop.numberOfEdges(), ii, q;
	EdgeInfo *edge;
	for (ii=0, edge = currentLoop.firstEdge; ii<nE; ii++, edge = edge->next)
	{
	  for (q=0; q<3; q++)
	    newLocation(q) = endPoint(edge->next->curve->startingPoint,q);
	  edge->curve->getNURBS()->moveEndpoint(1, newLocation);
	}
	
      } // end for all trimming curves
      
    } // end if trimmed surface
    
  } // end for all surfaces
  
// save endpoint coordinates
// all edge curves are loops or collection of loops, i.e., they end up at the same place as they begin
//  printf("Number of end points: %i, number of edges: %i\n", numberOfEndPoints, numberOfEdgeCurves);
  masterEdge.resize(numberOfEndPoints); 
  endPoint.resize(numberOfEndPoints, 3);
  
  printf("Time to build %i edge curves =%8.2e\n",numberOfEdgeCurves,getCPU()-time0);

// check reference counts

//    for( s=0; s<numberOfFaces; s++ )
//    {
//      for ( int i=0; i<faceInfoArray[s].numberOfLoops; i++ )
//      {
//        Loop & currentLoop = faceInfoArray[s].loop[i];
//        EdgeInfo *eOrig;
//        int cnt;
//        for( cnt=0, eOrig=currentLoop.firstEdge; cnt<currentLoop.numberOfEdges(); 
//  	   eOrig=eOrig->next, cnt++ )
//        {
//  	NurbsMapping * map = eOrig->curve->getNURBS();
//  	NurbsMapping * sc = eOrig->curve->subCurve;
//  	NurbsMapping * tc = eOrig->curve->surfaceLoop;
//  	printf("Reference count for edge #%i: map: %i, tc: %i, sc: %i\n", 
//  	       eOrig->edgeNumber, map->getReferenceCount(), tc->getReferenceCount(), 
//  	       sc->getReferenceCount());
//        }
//      }
//    }
  
  gi.outputString("Exiting buildEdgeCurves");

  if (!checkConsistency())
    return 1;
  else
    return 0;

  
} // end buildEdgeCurves()

// ============================================================================================
//! Build the boundary nodes on the edge curves (based on deltaS) that will be used in the triangulation.
// ===========================================================================================
int CompositeTopology::
buildEdgeCurveBoundaryNodes()
{
  int s, l, sc, ne, oldNumber, newNumber;
  real maxCurvature;
  
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    FaceInfo & currentFace = faceInfoArray[s];
    EdgeInfo *e;

    for (l=0; l<currentFace.numberOfLoops; l++)
    {
      Loop & currentLoop = currentFace.loop[l];
      ne = currentLoop.numberOfEdges();
      for (sc = 0, e = currentLoop.firstEdge; sc<ne; 
	   sc++, e=e->next )
      {
	oldNumber = e->curve->getNURBS()->getGridDimensions(axis1);
	realArray x = e->curve->getNURBS()->getGrid();
	Range I=x.dimension(0);
	x.reshape(I,3);
	newNumber = computeNumberOfGridPoints(e->curve->arcLength, x, maxCurvature);
	e->curve->getNURBS()->setGridDimensions(axis1,newNumber);
	e->curve->numberOfGridPoints = newNumber;
//	printf("buildEdgeCurveBN: s=%i, l=%i, sc=%i, oldNumber=%i, newNumber=%i\n", s, l, sc, 
//	       oldNumber, newNumber);
      }
    }
  }
  
  return 0;
}

// ==========================================================================================================
//!  Attempt to merge the edgeCurve e with other edge curves.
/*!
   \param e (input) : try to merge this edge curve, edgeCurve[e]
   \param eStart (input): check other edge curves starting at this one.
 */
// ==========================================================================================================
int CompositeTopology::
merge( EdgeInfo *e, int debug )
// return code:
// 1: successful merging
// 0: could not be merged
// -1: error while merging (erro returned from mergeTwoEdges
{
  NurbsMapping & edge = *e->curve->getNURBS();
  const bool isPeriodic = edge.getIsPeriodic(axis1)==Mapping::functionPeriodic;
  
  real xa[3], xb[3];
  int q;
  
// important to use the starting and ending points from the endPoint array, since they are exact
  int startP = e->getStartPoint();
  int endP = e->getEndPoint();
  for (q=0; q<3; q++)
  {
    xa[q]=endPoint(startP,q);
    xb[q]=endPoint(endP,q);
  }

  realArray x(1,3),r(1,1),x2(1,3);
	
  EdgeInfoADT::traversor traversor(*searchTree);

  // build a bounding box around one endpoint for the adt tree search, twice as big as the merge tolerance
  ArraySimple<real> bb(2,3);
  real delta=mergeTolerance*2.;
  bb(0,0)=xa[0]-delta, bb(1,0)=xa[0]+delta;
  bb(0,1)=xa[1]-delta, bb(1,1)=xa[1]+delta;
  bb(0,2)=xa[2]-delta, bb(1,2)=xa[2]+delta;

  traversor.setTarget(bb);

  if( debug & 4 )
    printf("merge: check edge %i with bbox =[%e,%e]x[%e,%e]x[%e,%e]\n",
	   e->edgeNumber, bb(0,0), bb(1,0), bb(0,1), bb(1,1), bb(0,2), bb(1,2));

  // evaluate the mid-point of the curve
  r(0,0)=.5;
  edge.map(r,x);

  EdgeInfo * eMin=NULL;             // best edge that can be merged
  real minDistance=mergeTolerance;  // holds current best merging distance
  real midPointDist=0.;
  int orientation=1;
  real distaa, distab, distba, distbb;

  while( !traversor.isFinished() )
  {
    // printf(" (%8.2e,%8.2e,%8.2e) is inside box %i \n",x,y,z,leaf.data);
    EdgeInfo * e2 = (*traversor).data;
//    if( ( e2->status == EdgeInfo::edgeCurveIsBoundary ||  e2->status == EdgeInfo::edgeCurveIsSplit)
    if( e2->status != EdgeInfo::edgeCurveIsSlave && e2->status != EdgeInfo::edgeCurveIsNotUsed &&
        e!=e2 && e->faceNumber != e2->faceNumber && e2->master == NULL ) 
    {

      NurbsMapping & edge2 = *e2->curve->getNURBS();
      real xa2[3], xb2[3];
// important to use the starting and ending points from the endPoint array, since they are exact
      int startP2 = e2->getStartPoint();
      int endP2 = e2->getEndPoint();
      for (q=0; q<3; q++)
      {
	xa2[q]=endPoint(startP2,q);
	xb2[q]=endPoint(endP2,q);
      }
	      
      distaa=fabs(xa[0]-xa2[0])+fabs(xa[1]-xa2[1])+fabs(xa[2]-xa2[2]);
      distab=fabs(xa[0]-xb2[0])+fabs(xa[1]-xb2[1])+fabs(xa[2]-xb2[2]);
      distba=fabs(xb[0]-xa2[0])+fabs(xb[1]-xa2[1])+fabs(xb[2]-xa2[2]);
      distbb=fabs(xb[0]-xb2[0])+fabs(xb[1]-xb2[1])+fabs(xb[2]-xb2[2]);
	      
      if( debug & 4 )
	printf("Merge: check edges %i and %i with dista=(%8.2e,%8.2e) distb=(%8.2e,%8.2e) isPeriodic=%i isPeriodic2=%i\n",
	       e->edgeNumber, e2->edgeNumber, distaa, distab, distba, distbb,isPeriodic,
               (int)edge2.getIsPeriodic(axis1) );

      if( (distaa<minDistance && distbb<minDistance ) || (distab<minDistance && distba<minDistance ) )
      {
        // end points match -- check another point on the curve 
        // ** for now only check the midpoint of the curve **

        r=-1.;
	edge2.inverseMap(x,r);
	edge2.map(r,x2);
        real dist=fabs(x(0,0)-x2(0,0))+fabs(x(0,1)-x2(0,1))+fabs(x(0,2)-x2(0,2));
	if( dist < minDistance )
	{
// a match or better match was found
          if( eMin )
	  {
	    printf("merge:WARNING:There are multiple matching edge curves for edge %i on surface s=%i, loop=%i.\n"
                   "Will use best match.\n", e->edgeNumber, e->faceNumber, e->loopNumber);
	  }
	    
          eMin=e2;
// for some short edges, both combinations can be smaller than minDistance <= mergeTolerance, 
// so we must take the best match
          if( max(distaa, distbb) < max(distab, distba) ) 
	  {
	    orientation=1;
  	    minDistance=max(distaa,distbb,dist);
	  }
	  else
	  {
	    orientation=-1;
  	    minDistance=max(distab,distba,dist);
	  }
          if( isPeriodic || max(distaa, distbb, distab, distba)< REAL_MIN*100. ) // NOTE: periodic surfaces dont' work yet
	  { // *wdh* 030825
	    // if the curve is periodic we need to be more careful in checking the orientation since
            // the endpoints may all coincide 
            // check the dot product of the tangent vectors to the curves
            realArray xr(1,3),xr2(1,3);
	    r(0,0)=0.;
	    edge.map(r,x,xr);
	    edge2.map(r,x,xr2);
            real norm=sqrt(xr(0,0)*xr(0,0)+xr(0,1)*xr(0,1)+xr(0,2)*xr(0,2));
            real norm2=sqrt(xr2(0,0)*xr2(0,0)+xr2(0,1)*xr2(0,1)+xr2(0,2)*xr2(0,2));
            const real normEps=REAL_MIN*100.;
	    if( norm<normEps || norm2<normEps )
	    {
	      printf("merge:ERROR:periodic curve has a 0 derivative, norm=%e, norm2=%e\n",norm,norm2);
	      norm=max(norm,normEps);
	      norm2=max(norm2,normEps);
	    }
            real dot = (xr(0,0)*xr2(0,0)+xr(0,1)*xr2(0,1)+xr(0,2)*xr2(0,2))/(norm*norm2);
	    if( dot>0. )
	    {
              orientation=1;
	      minDistance=max(distaa,distbb,dist);
	    }
            else
	    {
	      orientation=-1;
	      minDistance=max(distab,distba,dist);
	    }
  	    printf(" merge: periodic curve: dot=%8.2e, choosing orientation=%i\n",dot,orientation);
          }

	  midPointDist=dist;
//  	  printf("Edge #%i, merge candidate edge #%i, minDist=%e\n", e->edgeNumber, 
//  		 eMin->edgeNumber, minDistance);
	  break; // To detect non-manifold geometry, any curve within the mergeTolerance must get merged
	}
	else
	{
	  if (debug & 4)
	    printf("INFO: edge curves %i and %i have matching end points but not the midpoint, dist=%8.2e "
		   "(best dist so far=%8.2e)\n", e->edgeNumber, e2->edgeNumber, dist, minDistance);
	  minimumUnmergedDistance=min(minimumUnmergedDistance,dist);
	}
      }
      
    }

    traversor++;
  }  // while traversor
  

  if( eMin )
  {
    maximumMergeDistance = max(maximumMergeDistance, minDistance);
    averageMergeDistance += minDistance;

    if (mergeTwoEdges(e, eMin, orientation, mergeTolerance, debug))
      return 1;
    else
      return -1; // something went wrong...
  }
  else
    return 0;
} // end merge

bool CompositeTopology::
mergeTwoEdges(EdgeInfo *e, EdgeInfo *eMin, int orientation, real tolerance, int debug)
{
  if( debug & 2 )
  {
    printf("**Edge curves #%i (s=%i, l=%i) and #%i (s=%i, l=%i) are being merged. orientation=%i\n",
	   e->edgeNumber, e->faceNumber, e->loopNumber, eMin->edgeNumber, eMin->faceNumber, 
	   eMin->loopNumber, orientation);
  }
	  
  e->status = EdgeInfo::edgeCurveIsMaster;
  e->eraseEdge();
  
// important to assign the orientation first, (setStartPoint and setEndPoint depend on it!)
  eMin->status = EdgeInfo::edgeCurveIsSlave;
  eMin->eraseEdge();
  EdgeInfo *s;
  EdgeInfo *removeEdge;
  Loop *loopy;
    
// the recursive calls in setEndPoint and setStartPoint can get back to the original curve (s), in which
// case it is necessary to change the orientation and curve pointer before calling setStartPoint or setEndPoint 

  eMin->orientation = orientation; 
// copy curve pointers
  e->curve->usage++;
  eMin->curve->usage--;
// we don't delete the curve, since we might need it later on, if the edge is un-merged interactively. 
// A pointer to the initial curve is kept in the EdgeInfo class.
  eMin->curve = e->curve;

// also needs to erase all edges where an endpoint gets moved!
  for (s=eMin->slave; s!= NULL; s = s->slave)
  {
    s->orientation *= orientation;
    s->curve->usage--;
    e->curve->usage++;
    s->curve = e->curve;
  }
  
// now update the starting and ending point numbers
  for (s=eMin; s!= NULL; s = s->slave)
  {
    EdgeInfo *p = s->prev, *n = s->next;

    if (debug & 2)
    {
      printf("Adjusting endpoints: e=%i (o=%i), s=%i (o=%i), p=%i (o=%i), n=%i (o=%i)\n", 
	     e->edgeNumber, e->orientation, s->edgeNumber, s->orientation, 
	     p->edgeNumber, p->orientation, n->edgeNumber, n->orientation);
    }
    
// we don't change the starting and ending point numbers on the slave, since that curve will 
// not be used anymore and keeping the old numbers enables us to un-merge interactively
// (unless the merged edge gets un-merged in which case the old point numbers are relevant again)
    if (e->orientation * s->orientation > 0) // oriented the same way
    {
      if (!p->setEndPoint( e->getStartPoint(), endPoint, tolerance, s->edgeNumber, masterEdge, unusedEdges))
      {
	printf("Setting end point of edge %i to point %i failed\n", p->edgeNumber, e->getStartPoint());
      }
      
      if (!n->setStartPoint( e->getEndPoint(), endPoint, tolerance, s->edgeNumber, masterEdge, unusedEdges))
      {
	printf("Setting start point of edge %i to point %i failed\n", n->edgeNumber, e->getEndPoint());
      }
    }
    else // oriented in opposite ways
    {
      if (!p->setEndPoint( e->getEndPoint(), endPoint, tolerance, s->edgeNumber, masterEdge, unusedEdges))
      {
	printf("Setting end point of edge %i to point %i failed\n", p->edgeNumber, e->getEndPoint());
      }
      if (!n->setStartPoint( e->getStartPoint(), endPoint, tolerance, s->edgeNumber, masterEdge, unusedEdges))
      {
	printf("Setting start point of edge %i to point %i failed\n", n->edgeNumber, e->getStartPoint());
      }
    }
    
    
    if (debug & 2)
    {
      printf("After merging:\n"
	     "edge prev=%i, orientation=%i, startingPoint=%i, newStartPoint=%i, endingPoint=%i, "
	     "newEndPoint=%i\n",  p->edgeNumber, p->orientation, p->curve->startingPoint, 
	     p->curve->newStartPoint, p->curve->endingPoint, p->curve->newEndPoint);
      printf("edge next=%i, orientation=%i, startingPoint=%i, newStartPoint=%i, endingPoint=%i, "
	     "newEndPoint=%i\n",  n->edgeNumber, n->orientation, n->curve->startingPoint, 
	     n->curve->newStartPoint, n->curve->endingPoint, n->curve->newEndPoint);
    }
    
  }

  e->slave = eMin;
  eMin->master = e;
    
  return true;
} // end mergeTwoEdges()


// =================================================================================================
//! Attempt to force a merge between two edge curves. Double check that the merge make sense.
/*! 
   \param e,e2 (input): try to merge these two edge curves.
   \return 0==success.
 */
// =================================================================================================
int CompositeTopology::
mergeEdgeCurves(EdgeInfo & e, EdgeInfo & e2, int debug /* =0 */)
{
// do some basic checks
  if( e.edgeNumber == e2.edgeNumber )
  {
    printf("Cannot merge edge curves e1=%i and e2=%i since they are the same\n", 
	   e.edgeNumber, e2.edgeNumber);
    return 1;
  }
  else if( e.status != EdgeInfo::edgeCurveIsBoundary )
  {
    printf("Cannot merge edge curves since edge curve e1=%i is not a boundary edge.\n",e.edgeNumber);
    return 1;
  }
  else if( e2.status != EdgeInfo::edgeCurveIsBoundary )
  {
    printf("Cannot merge edge curves since edge curve e2=%i is not a boundary edge.\n",e2.edgeNumber);
    return 1;
  }
  else if( e.faceNumber==e2.faceNumber )
  {
    printf("Cannot merge edge curves %i and %i since they belong to the same subsurface, s=%i\n",
	   e.edgeNumber,e2.edgeNumber,e.faceNumber);
    return 1;
  }

  bool curvesWereMerged=false;
  
  Mapping & edge = *e.curve->getNURBS();
  real xa[3], xb[3];
  int q;
// important to use the starting and ending points from the endPoint array, since they are exact
  int startP = e.getStartPoint();
  int endP = e.getEndPoint();
  for (q=0; q<3; q++)
  {
    xa[q]=endPoint(startP,q);
    xb[q]=endPoint(endP,q);
  }

  realArray x(1,3),r(1,1),x2(1,3);
	
  // evaluate the mid-point of the curve
  r(0,0)=.5;
  edge.map(r,x);

  real weakMergeTolerance=mergeTolerance*100.;

  Mapping & edge2 = *e2.curve->getNURBS();
  real xa2[3], xb2[3];
// important to use the starting and ending points from the endPoint array, since they are exact
  int startP2 = e2.getStartPoint();
  int endP2 = e2.getEndPoint();
  for (q=0; q<3; q++)
  {
    xa2[q]=endPoint(startP2,q);
    xb2[q]=endPoint(endP2,q);
  }
	      
  real distaa=fabs(xa[0]-xa2[0])+fabs(xa[1]-xa2[1])+fabs(xa[2]-xa2[2]);
  real distab=fabs(xa[0]-xb2[0])+fabs(xa[1]-xb2[1])+fabs(xa[2]-xb2[2]);
  real distba=fabs(xb[0]-xa2[0])+fabs(xb[1]-xa2[1])+fabs(xb[2]-xa2[2]);
  real distbb=fabs(xb[0]-xb2[0])+fabs(xb[1]-xb2[1])+fabs(xb[2]-xb2[2]);
	      
  if( debug & 4 )
    printf("Merge: check e=%i with e2=%i dista=(%8.2e,%8.2e) distb=(%8.2e,%8.2e) \n",
	   e.edgeNumber, e2.edgeNumber, distaa, distab, distba, distbb );
  
  if( (distaa<weakMergeTolerance && distbb<weakMergeTolerance ) || 
      (distab<weakMergeTolerance && distba<weakMergeTolerance ) )
  {
      // end points match -- check another point on the curve 
      // ** for now only check the midpoint of the curve **

    r=-1.;
    edge2.inverseMap(x,r);
    edge2.map(r,x2);
    real dist=fabs(x(0,0)-x2(0,0))+fabs(x(0,1)-x2(0,1))+fabs(x(0,2)-x2(0,2));
    if( dist<weakMergeTolerance )
    {
      maximumMergeDistance=max(maximumMergeDistance,dist);
      averageMergeDistance+=dist;

      if( debug & 2 )
      {
	printf("**Edge curves %i(s=%i) and %i(s=%i) are merged. dist at midpoint=%e, ",e.edgeNumber,
	       e.faceNumber, e2.edgeNumber, e2.faceNumber, dist);
	if( distaa<weakMergeTolerance && distbb<weakMergeTolerance )
	  printf(" dist at ends=(%8.2e,%8.2e)\n", distaa, distbb);
	else
	  printf(" dist at ends=(%8.2e,%8.2e)\n", distab, distba);
      }

      int orientation;
// for some short edges, both combinations can be smaller than weakMergeTolerance, 
// so we must take the best match
      if( max(distaa, distbb) < max(distab, distba) )
      {
	orientation=1;
      }
      else
      {
	orientation=-1;
      }

      curvesWereMerged = mergeTwoEdges(&e, &e2, orientation, weakMergeTolerance, debug);
      
    }
    else
    {
      printf("INFO: edge curves %i and %i, end points match but not the midpoint, dist=%e\n", 
	     e.edgeNumber, e2.edgeNumber, dist);
    }
  }
  else
  {
    printf("INFO: edge curves %i and %i, do not match at the end points. dist=%8.2e\n",
	   e.edgeNumber, e2.edgeNumber, min(max(distaa,distbb),max(distab,distba)));
  }

  return curvesWereMerged==false;
} // end mergeEdgeCurves();

static bool first=true;

// =================================================================================================
//! Attempt to join two adjacent edges curves (which normally will be on the same surface).
/*! 
   \param e,e2 (input): try to join these two edge curves.
   \return 0==success.
 */
// =================================================================================================
int CompositeTopology::
joinEdgeCurves(EdgeInfo &eOrig, bool toNext, int debug /* =0 */)
{
  bool curvesWereJoined=false, parameterCurvesJoined=false;
  Loop &currentLoop = *eOrig.loopy;
  
  printf("Entering joinEdgeCurves to join edge %i with its %s neighbor\n", eOrig.edgeNumber, 
	 toNext? "next": "previous");
  EdgeInfo *e1, *e2;
  e1 = &eOrig;
  e2 = (toNext)? eOrig.next : eOrig.prev;
// e1 & e2 should be boundaryEdges and have orientation == 1
  if (e1->status != EdgeInfo::edgeCurveIsBoundary || e2->status != EdgeInfo::edgeCurveIsBoundary || 
      e1->orientation != 1 || e2->orientation != 1)
  {
    printf("joinEdgeCurves: Only two positively oriented boundary edges (status = 0) may be joined!\n"
	   "Edge e1: %i, e1->status = %i, e1->orientation = %i, edge e2: %i, e2->status = %i, "
	   "e2->orientation = %i.\n", 
	   e1->edgeNumber, e1->status, e1->orientation, e2->edgeNumber, e2->status, e2->orientation);
    GenericGraphicsInterface & gi = *Overture::getGraphicsInterface(); 
    gi.outputString("Can't join those edges!\n"
		    "You might be trying to join edges from different surfaces; "
		    "toggle on the merged (green) edges to check this!");
    return 1;
  }
  
//    printf("joinEdgeCurve: reference count for e1=%i before join: map: %i, tc: %i, sc: %i\n", 
//  	 e1->edgeNumber, e1->curve->getNURBS()->getReferenceCount(), 
//  	 e1->curve->surfaceLoop->getReferenceCount(), e1->curve->subCurve->getReferenceCount());
//    printf("joinEdgeCurve: reference count for e2=%i before join: map: %i, tc: %i, sc: %i\n", 
//  	 e2->edgeNumber, e2->curve->getNURBS()->getReferenceCount(), 
//  	 e2->curve->surfaceLoop->getReferenceCount(), e2->curve->subCurve->getReferenceCount());

// make the new piece to hold the merged NURBS
  NurbsMapping &nurbs1 = *new NurbsMapping(*e1->curve->getNURBS()); // make a copy of the NURBS curve
  nurbs1.incrementReferenceCount();
// merge the copy of the original edge with e2->curve
  int numLines=max(minNumberOfPointsOnAnEdge, e1->curve->numberOfGridPoints + e2->curve->numberOfGridPoints );
  nurbs1.setGridDimensions(axis1,numLines);

  curvesWereJoined = (nurbs1.merge(*e2->curve->getNURBS(), false) == 0);

  if (!curvesWereJoined)
  {
    printf("joinEdgeCurves: ERROR: NURBS concatenation unsuccessful\n");
    if (nurbs1.decrementReferenceCount() == 0)
      delete &nurbs1;
    return 1;
  }
    
// call the new function joinSubCurves to make a new subcurve in parameter space
  NurbsMapping * tc = e1->curve->surfaceLoop;
  
  int q1;
  for (q1=0; q1 < tc->numberOfSubCurves(); q1++)
  {
    if (&(tc->subCurve(q1)) == e1->curve->subCurve) break;
  }
//    if (q1>=0 && q1<tc->numberOfSubCurves())
//    {
//      printf("INFO: joinEdge: old sub curve 1 has number = %i\n", q1);
//    }
  int q2;
  for (q2=0; q2 < tc->numberOfSubCurves(); q2++)
  {
    if (&(tc->subCurve(q2)) == e2->curve->subCurve) break;
  }
//    if (q2>=0 && q2<tc->numberOfSubCurves())
//    {
//      printf("INFO: joinEdge: old sub curve 2 has number = %i\n", q2);
//    }
// join subCurves q1 and q2
  int lastVisible=tc->numberOfSubCurves()-1;
//  printf("Joining subcurves %i and %i, lastVisible=%i\n", q1, q2, lastVisible);
  int q;
  if (max(q1,q2) == lastVisible && min(q1,q2) == 0) // join curves lastVisible and 0
    q = tc->joinSubCurves(lastVisible);
  else
    q = tc->joinSubCurves(min(q1,q2));
    
// our new sub curve after joinSubCurves
  NurbsMapping *sc1 = &tc->subCurve(q);

  int startP, endP;
  if (toNext)
  {
    startP = e1->getStartPoint();
    endP = e2->getEndPoint();
  }
  else
  {
    startP = e2->getStartPoint();
    endP = e1->getEndPoint();
  }
  
// joined curves do now have orgination info (surface, trim curve, sub curve)!
  CurveSegment *curve1 = new CurveSegment(nurbs1, startP, e1->curve->surfaceNumber, tc, sc1); 
  
// orientation of boundary curves is always 1
  EdgeInfo *eNew = new EdgeInfo(curve1, e1->loopNumber, e1->faceNumber, 1, numberOfEdgeCurves);
  numberOfEdgeCurves++;
   
  eNew->curve->numberOfGridPoints=numLines;
  eNew->curve->arcLength = e1->curve->arcLength + e2->curve->arcLength;

  eNew->status = EdgeInfo::edgeCurveIsBoundary;
// need to fill in starting/ending point info and more...
  eNew->curve->startingPoint = startP;
  eNew->curve->newStartPoint = -1;
  
  eNew->curve->endingPoint = endP;
  eNew->curve->newEndPoint = -1;
  eNew->curve->numberOfGridPoints = numLines; // assigned above
  
// replace e1 by eNew in the current loop
  if (!currentLoop.replaceEdge(eNew, e1))
    printf("Warning: replacing e1 by eNew failed...\n");

// remove e2
  currentLoop.removeEdge(e2);
  
// modify masterEdge info (the orientation is == 1 for all these edges, since these are boundary edges)
  if (toNext)
  {
    if (masterEdge.array[startP] == e1)
      masterEdge.array[startP] = eNew;
    if (masterEdge.array[endP] == e2)
      masterEdge.array[endP] = eNew;
  }
  else
  {
    if (masterEdge.array[startP] == e2)
      masterEdge.array[startP] = eNew;
    if (masterEdge.array[endP] == e1)
      masterEdge.array[endP] = eNew;
  }
  
// e1 is only pointed to by the ADT tree, so we push it onto the unusedEdges stack
  e1->setUnused(unusedEdges);
  
//  printf("Pushing edge #%i onto the unusedEdges stack\n", e1->edgeNumber);
  if (currentLoop.edgeInLoop(e1))
    printf("ERROR: edge %i was replaced but is still in the Loop!\n", e1->edgeNumber);

// e2 is only pointed to by the ADT tree, so we push it onto the unusedEdges stack
  e2->setUnused(unusedEdges);

//    printf("Pushing edge #%i onto the unusedEdges stack\n", e2->edgeNumber);
  if (currentLoop.edgeInLoop(e2))
    printf("ERROR: edge %i was replaced but is still in the Loop!\n", e2->edgeNumber);

// insert eNew into the search tree
  ArraySimple<real> bb(2,3);
// first piece
  bb(0,0)=(real)nurbs1.getRangeBound(Start,0), bb(1,0)=(real)nurbs1.getRangeBound(End,0);
  bb(0,1)=(real)nurbs1.getRangeBound(Start,1), bb(1,1)=(real)nurbs1.getRangeBound(End,1);
  bb(0,2)=(real)nurbs1.getRangeBound(Start,2), bb(1,2)=(real)nurbs1.getRangeBound(End,2);

  if (searchTree) // the search tree is not around when the edge curves first are being built
    searchTree->addElement(bb,eNew);		  

  if (nurbs1.decrementReferenceCount()==0)
    delete &nurbs1;
  
//    printf("joinEdge: reference count for eNew after join: map: %i, tc: %i, sc: %i\n", 
//  	 eNew->curve->getNURBS()->getReferenceCount(), eNew->curve->surfaceLoop->getReferenceCount(), 
//  	 eNew->curve->subCurve->getReferenceCount());

  return curvesWereJoined==false;
}


// =========================================================================================================
//! This Class can be used to determine the connectivity (topology) of a CompositeSurface by determining
//!  where adjacent sub-surfaces meet.
/*!
    The CompositeTopology class can be used to build a global triangulation for the CompositeSurface.
    There are two steps in this process. First determine the connectivity of the CompositeSurface
    by matching edge curves on the sub-surface boundaries. Second build triangulations for each sub-surface
    using common nodes at the shared edges.

   \param cs (input): determine the topology for this surface.

 */
// =========================================================================================================
CompositeTopology::
CompositeTopology(CompositeSurface & cs_)
  : cs(cs_), boundingBox(2,3)
{
  globalTriangulation=NULL;   // includes all surfaces
  globalTriangulationForVisibleSurfaces=NULL;  // does not include hidden surfaces.
  
  numberOfEdgeCurves=0;
  numberOfUniqueEdgeCurves = 0;
  allEdges = NULL;
  searchTree = NULL;
  
  triangulationSurface=NULL;
  
  int i,j;
  for (i=0; i<2; i++)
    for (j=0; j<3; j++)
      boundingBox(i,j) = 0.;

  edgeCurvesAreBuilt=false;  
  mergedCurvesAreValid=false;   // set to true when merged curves are consistent with current parameters
  recomputeEdgeCurveBoundaryNodes=false; // set to true if user changes deltaS
  
  deltaS=1.;
  maximumArea=REAL_MAX;
  splitToleranceFactor=10.;  // splitTolerance = mergeTolerance * splitToleranceFactor

  curveResolutionTolerance=1.e-3;

  curvatureTolerance=.05;
  
  boundingBoxExtension=.1; // .05; // increase the size of the bounding boxes by this amount
  
  maxDist=0.02; // default max distance from midpoint of edge to surface
  improveTri=false;
  
  numberOfEndPoints=0;

  faceInfoArray = NULL;
  numberOfFaces = 0;

// initialize tolerances and the bounding box using the underlying CompositeSurface
  initializeTopology();
}

// =========================================================================================================
//! Destroy a CompositeTopology.
// =========================================================================================================
CompositeTopology::
~CompositeTopology()
{
//  printf("CompositeTopology destructor called\n");
  
  cleanup();
  
  delete searchTree;  // *wdh* 030825 should this be in cleanup ?

  if (globalTriangulation && globalTriangulation->decrementReferenceCount() == 0)
    delete globalTriangulation;

  if (globalTriangulationForVisibleSurfaces && globalTriangulationForVisibleSurfaces->decrementReferenceCount() == 0)
    delete globalTriangulationForVisibleSurfaces;

//  printf("Exiting CompositeTopology desctructor\n");
  
}


// =============================================================================================
//! Cleanup temporary storage used while determining the connectivity and global triangulation.
/*!
   Call this next routine to cleanup objects that are only needed while the 
   topology in the process of being computed or changed.
   For example, call this routine to release memory once you have computed a valid global
   triangulation.
   */
// =============================================================================================
int CompositeTopology::
cleanup()
{
//  printf("Entering cleanup()...\n");
  
// get rid of sub surface triangulations
  if( triangulationSurface )
  {
//    printf("Deleting triangulationSurface...\n");
// delete sub surface triangulations
    for(int s=0; s<numberOfFaces; s++ )
    {
      if( triangulationSurface[s] && triangulationSurface[s]->decrementReferenceCount()==0 )
      {
	delete triangulationSurface[s];
      }
//        else if( triangulationSurface[s])
//        {
//  	printf("cleanup: triangulationSurface[%i]->referenceCount=%i\n", s, 
//  	       triangulationSurface[s]->getReferenceCount());
//        }
      triangulationSurface[s]=NULL;
    }
    delete [] triangulationSurface;
  }

// remove any existing edgeInfos first
  if (faceInfoArray)
  {
//    printf("Deleting faceInfoArray...\n");
    delete [] faceInfoArray;
    faceInfoArray = NULL;
  }
  numberOfFaces=0;
// reset the CurveSegment count to zero
  CurveSegment::resetGlobalCount();
  

// the allEdges datastructure is setup on the first call to getEdgeCurve()
  if (allEdges)
  {
    delete [] allEdges;
    allEdges = NULL;
  }
  numberOfEdgeCurves=0;

// free any unused edges
//  printf("Deleting unused edges...\n");
  EdgeInfo *e;
  while (e=unusedEdges.pop())
  {
    delete e;
  }

  return 0;
}


CompositeTopology & CompositeTopology::
operator =( const CompositeTopology & X )
{
  return *this;
}


// ========================================================================================
//!  Get from a data base file.
// Note that this function usually is called from CompositeSurface::get(), since it takes a
// CompositeSurface object to create a CompositeTopology object.
// ========================================================================================
int CompositeTopology::
get( const GenericDataBase & dir, const aString & name)    // get from a database file
{

  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"CompositeTopology");

// new virtual topology stuff
  int topologyPresent;
  subDir.get(topologyPresent,"topologyPresent");
  if (topologyPresent)
  {
    aString buf;
    int q, f;
    NurbsMapping **allSurfaceLoops = new NurbsMapping*[cs.numberOfSubSurfaces()];
    q=0;
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      if (cs[f].getClassName()=="TrimmedMapping")
      {
	TrimmedMapping &trim = (TrimmedMapping &)cs[f];
	allSurfaceLoops[f] = (NurbsMapping *)trim.getTrimCurve(0); // this loop has already been read!
      }
      else // untrimmed surface
      {
	allSurfaceLoops[f] = new NurbsMapping;
	allSurfaceLoops[f]->get(subDir,sPrintF(buf,"surfaceLoopMapping-%i",f));
	q++;
      }
      allSurfaceLoops[f]->incrementReferenceCount();
    }
//    printf("CompositeTopology::get() read %i untrimmed surface loops\n", q);
  
// make an array of pointers to all curveSegment's
    CurveSegment::resetGlobalCount(); //This is a static member function 
    int numberOfCurveSegments;
    subDir.get(numberOfCurveSegments, "numberOfCurveSegments");
    CurveSegment ** allCurveSegments = new CurveSegment* [numberOfCurveSegments];
// initialize
    for (q=0; q<numberOfCurveSegments; q++)
      allCurveSegments[q] = NULL;

// read all CurveSegments  
    for (q=0; q<numberOfCurveSegments; q++)
    {
      int curveSegmentPresent = allCurveSegments[q] != NULL;
      subDir.get(curveSegmentPresent, sPrintF(buf, "curveSegmentPresent-%i", q));
      if (curveSegmentPresent)
      {
	allCurveSegments[q] = new CurveSegment;
	allCurveSegments[q]->get(subDir, sPrintF(buf, "CurveSegmentData-%i", q), cs, allSurfaceLoops);
      }
    
    }
//    printf("Read %i CurveSegments\n", numberOfCurveSegments);

// make a pointer array to all EdgeInfo's
    subDir.get(numberOfEdgeCurves, "numberOfEdgeCurves");
    EdgeInfo ** allEdgeInfos = new EdgeInfo* [numberOfEdgeCurves];
// initialize
    for (q=0; q<numberOfEdgeCurves; q++)
      allEdgeInfos[q] = NULL;

// read all EdgeInfo and save pointers in allEdgeInfos  
    for (q=0; q<numberOfEdgeCurves; q++)
    {
      int edgeInfoPresent;
      subDir.get(edgeInfoPresent, sPrintF(buf, "edgeInfoPresent-%i", q));
      if (edgeInfoPresent)
      {
	allEdgeInfos[q] = new EdgeInfo;
	allEdgeInfos[q]->get(subDir, sPrintF(buf, "EdgeInfo-%i", q), allCurveSegments);
      }
    }
//    printf("Read %i EdgeInfo's\n", numberOfCurveSegments);

// fill in the prev/next/slave/master pointers in these EdgeInfo objects
    for (q=0; q<numberOfEdgeCurves; q++)
    {
      int edgeInfoPresent = allEdgeInfos[q] != NULL;
      if (edgeInfoPresent)
	allEdgeInfos[q]->assignPointers(allEdgeInfos);
    }

// read all FaceInfo's (which allocates and reads the Loops)
    numberOfFaces = cs.numberOfSubSurfaces(); 
// allocate a new data structure for the virtual topology
    faceInfoArray = new FaceInfo[numberOfFaces];

    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      faceInfoArray[f].get(subDir,sPrintF(buf, "FaceInfo-%i", f), allEdgeInfos);
    }
//    printf("Read %i FaceInfo's\n", cs.numberOfSubSurfaces());
  
// save end point info
    subDir.get(numberOfEndPoints, "numberOfEndPoints");
    subDir.getDistributed(endPoint, "endPoint");
    masterEdge.get(subDir, "masterEdge", allEdgeInfos);
//    printf("Read end point info\n");

// cleanup
    for (f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      if (allSurfaceLoops[f]->decrementReferenceCount() == 0)
      {
	printf("compositeTopology: get: WARNING: zero reference count for surface loop %i\n", f);
	delete allSurfaceLoops[f];
      }
    }
    delete [] allSurfaceLoops;
    delete [] allCurveSegments;
    delete [] allEdgeInfos;
  } // end if topologyPresent
  
  subDir.get(triangulationIsValid,"triangulationIsValid");
  subDir.get(edgeCurvesAreBuilt,"edgeCurvesAreBuilt");
  subDir.get(mergedCurvesAreValid,"mergedCurvesAreValid");

// get the bounding box
  subDir.get(boundingBox(0,0),"boundingBox(0,0)");
  subDir.get(boundingBox(1,0),"boundingBox(1,0)");
  subDir.get(boundingBox(0,1),"boundingBox(0,1)");
  subDir.get(boundingBox(1,1),"boundingBox(1,1)");
  subDir.get(boundingBox(0,2),"boundingBox(0,2)");
  subDir.get(boundingBox(1,2),"boundingBox(1,2)");
  
// tmp
//    printf("get: boundingBox of the entire surface:\n"
//  	 "[%e,%e]x[%e,%e]x[%e,%e]\n", boundingBox(0,0), boundingBox(1,0),
//  	 boundingBox(0,1), boundingBox(1,1), boundingBox(0,2), boundingBox(1,2));

// more old stuff that we will keep
  int triangulationSaved;
  subDir.get(triangulationSaved,"triangulationSaved");
  if( triangulationSaved )
  {
// remove any existing triangulation
    if( globalTriangulation && globalTriangulation->decrementReferenceCount()==0 )
      delete globalTriangulation;

    globalTriangulation= new UnstructuredMapping;
    globalTriangulation->incrementReferenceCount();

    globalTriangulation->get(subDir,"globalTriangulation");
  }

  subDir.get(mergeTolerance,"mergeTolerance");
  subDir.get(splitToleranceFactor,"splitToleranceFactor");
  subDir.get(deltaS,"deltaS");
  subDir.get(curvatureTolerance,"curvatureTolerance");
  subDir.get(curveResolutionTolerance,"curveResolutionTolerance");
  subDir.get(maximumArea,"maximumArea");
  
  subDir.get(minNumberOfPointsOnAnEdge,"minNumberOfPointsOnAnEdge");
  subDir.get(signForNormal,"signForNormal");

  if (topologyPresent)
  {
// does the topology make sense?
    checkConsistency();
// setup the search tree
    int debug = 0;
    buildEdgeCurveSearchTree(debug);
  }

// initialize
  triangulationSurface = new UnstructuredMapping *[numberOfFaces];
  for( int s=0; s<numberOfFaces; s++ )
  {
    triangulationSurface[s]=NULL;
  }

  delete &subDir;
  return 0;
}


// ===============================================================================================
//! Put to a data-base.
// Note that this function is usually called from CompositeSurface::put(), since it takes a
// CompositeSurface object to create a CompositeTopology object.
// ===============================================================================================
int CompositeTopology::
put( GenericDataBase & dir, const aString & name) const    // put to a database file
{
  
  GenericDataBase & subDir = *dir.virtualConstructor();    // create a derived data-base object
  dir.create(subDir,name,"CompositeTopology");             // create a sub-directory 

// save the virtual topology datastructure
// first check if it is there!
  subDir.put((faceInfoArray? 1:0),"topologyPresent");
  if (faceInfoArray)
  {
    aString buf;
    int q, f, l;
    EdgeInfo *e;
  
    q=0;
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
// only save the loops for the untrimmed surfaces
      int isTrimmed = (cs[f].getClassName()=="TrimmedMapping");
      if (!isTrimmed)
      {
	Loop & currentLoop = faceInfoArray[f].loop[0]; // only one loop in untrimmed surfaces
	e = currentLoop.firstEdge;
// the surfaceLoop is the same for all e->initialCurves
	CurveSegment *c = e->initialCurve;
	c->surfaceLoop->put(subDir,sPrintF(buf,"surfaceLoopMapping-%i",f));
	q++;
      }
    }
//    printf("CompositeTopology::put() saved %i untrimmed surface loops\n", q);

// make an array of pointers to all curveSegment's
    int numberOfCurveSegments = CurveSegment::getGlobalCount(); //This is a static member function
    subDir.put(numberOfCurveSegments, "numberOfCurveSegments");
    CurveSegment ** allCurveSegments = new CurveSegment* [numberOfCurveSegments];
// initialize
    for (q=0; q<numberOfCurveSegments; q++)
      allCurveSegments[q] = NULL;
// all CurveSegments have a unique curveNumber, 0 <= curveNumber < numberOfCurveSegments!  

// traverse all CurveSegment objects in the topology and save pointers to them
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
      {
	Loop & currentLoop = faceInfoArray[f].loop[l];
	int sc, ne = currentLoop.numberOfEdges();
	for (sc=0, e = currentLoop.firstEdge; sc<ne; sc++, e = e->next)
	{
	  CurveSegment *ic = e->initialCurve;
	  CurveSegment *c = e->curve;
	  allCurveSegments[ic->getCurveNumber()] = ic;
// not all edges have assigned the curve pointer
	  if (c)
	    allCurveSegments[c->getCurveNumber()] = c;
	}
      }
    } // end for f

// save all CurveSegments  
    for (q=0; q<numberOfCurveSegments; q++)
    {
      int curveSegmentPresent = allCurveSegments[q] != NULL;
      subDir.put(curveSegmentPresent, sPrintF(buf, "curveSegmentPresent-%i", q));
      if (curveSegmentPresent)
	allCurveSegments[q]->put(subDir, sPrintF(buf, "CurveSegmentData-%i", q), cs);
    }
//    printf("Saved %i CurveSegments\n", numberOfCurveSegments);

// make a pointer array to all EdgeInfo's
    subDir.put(numberOfEdgeCurves, "numberOfEdgeCurves");
    EdgeInfo ** allEdgeInfos = new EdgeInfo* [numberOfEdgeCurves];
// initialize
    for (q=0; q<numberOfEdgeCurves; q++)
      allEdgeInfos[q] = NULL;

// all EdgeInfo's have a unique edgeNumber, 0 <= edgeNumber < numberOfEdgeCurves  

// traverse all EdgeInfo objects in the topology and save pointers to them
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
      {
	Loop & currentLoop = faceInfoArray[f].loop[l];
	int sc, ne = currentLoop.numberOfEdges();
	for (sc=0, e = currentLoop.firstEdge; sc<ne; sc++, e = e->next)
	{
	  allEdgeInfos[e->edgeNumber] = e;
// not all edges have assigned the slave and master pointers assigned
	  if (e->slave)
	    allEdgeInfos[e->slave->edgeNumber] = e->slave;
	  if (e->master)
	    allEdgeInfos[e->master->edgeNumber] = e->master;
	}
      }
    } // end for f
// save the EdgeInfo's pointed to by allEdgeInfos  
    for (q=0; q<numberOfEdgeCurves; q++)
    {
      int edgeInfoPresent = allEdgeInfos[q] != NULL;
      subDir.put(edgeInfoPresent, sPrintF(buf, "edgeInfoPresent-%i", q));
      if (edgeInfoPresent)
	allEdgeInfos[q]->put(subDir, sPrintF(buf, "EdgeInfo-%i", q));
    }
//    printf("Saved %i EdgeInfo's\n", numberOfCurveSegments);
  
// save all FaceInfo's (which saves the Loops)
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      faceInfoArray[f].put(subDir,sPrintF(buf, "FaceInfo-%i", f));
    }
//    printf("Saved %i FaceInfo's\n", cs.numberOfSubSurfaces());
  
// save end point info
    subDir.put(numberOfEndPoints, "numberOfEndPoints");
    subDir.putDistributed(endPoint, "endPoint");
    masterEdge.put(subDir, "masterEdge");
//    printf("Saved end point info\n");

// cleanup
    delete [] allEdgeInfos;
    delete [] allCurveSegments;
  } // end if faceInfoArray

  subDir.put(triangulationIsValid,"triangulationIsValid");
  subDir.put(edgeCurvesAreBuilt,"edgeCurvesAreBuilt");
  subDir.put(mergedCurvesAreValid,"mergedCurvesAreValid");

// put the bounding box
  subDir.put(boundingBox(0,0),"boundingBox(0,0)");
  subDir.put(boundingBox(1,0),"boundingBox(1,0)");
  subDir.put(boundingBox(0,1),"boundingBox(0,1)");
  subDir.put(boundingBox(1,1),"boundingBox(1,1)");
  subDir.put(boundingBox(0,2),"boundingBox(0,2)");
  subDir.put(boundingBox(1,2),"boundingBox(1,2)");
  
//    printf("put: boundingBox of the entire surface:\n"
//  	 "[%e,%e]x[%e,%e]x[%e,%e]\n", boundingBox(0,0), boundingBox(1,0),
//  	 boundingBox(0,1), boundingBox(1,1), boundingBox(0,2), boundingBox(1,2));

// more old stuff
  int triangulationSaved=globalTriangulation!=NULL;
  subDir.put(triangulationSaved,"triangulationSaved");
  if( triangulationSaved )
  {
    globalTriangulation->put(subDir,"globalTriangulation");
  }

  subDir.put(mergeTolerance,"mergeTolerance");
  subDir.put(splitToleranceFactor,"splitToleranceFactor");
  subDir.put(deltaS,"deltaS");
  subDir.put(curvatureTolerance,"curvatureTolerance");
  subDir.put(curveResolutionTolerance,"curveResolutionTolerance");
  subDir.put(maximumArea,"maximumArea");
  
  subDir.put(minNumberOfPointsOnAnEdge,"minNumberOfPointsOnAnEdge");
  subDir.put(signForNormal,"signForNormal");
  
  delete &subDir;
  
  return 0;
}



// =====================================================================================================
//!    Given a global triangulation for all surfaces, build a new triangulation for only visible surfaces.
/*! 
    This routine will build a new triangulation for those sub-surfaces that are still visible.
 */
// =====================================================================================================
int CompositeTopology::
buildTriangulationForVisibleSurfaces()
{
  if( globalTriangulation!=NULL )
  {
    UnstructuredMapping & global = *globalTriangulation;
    if( globalTriangulationForVisibleSurfaces && globalTriangulationForVisibleSurfaces->decrementReferenceCount() )
      delete globalTriangulationForVisibleSurfaces;

    globalTriangulationForVisibleSurfaces = new UnstructuredMapping;
    globalTriangulationForVisibleSurfaces->incrementReferenceCount();
    
    const realArray & gNode = global.getNodes();
    const intArray & gElement = global.getElements();
    const intArray & gFace = global.getFaces();
    const intArray & gFaceElements = global.getFaceElements();
    const intArray & gef = global.getElementFaces();
    const intArray & gElementSurface = global.getTags();

    const int gNumberOfElements=global.getNumberOfElements();
    const int gNumberOfNodes=global.getNumberOfNodes();
    const int gNumberOfFaces=global.getNumberOfFaces();

    realArray node(gNumberOfNodes,3);
    intArray element(gNumberOfElements,3);
    intArray face(gNumberOfFaces,2);
    intArray ef(gNumberOfElements,3);
    intArray faceElement(gNumberOfFaces,2);   faceElement=-1;
    intArray elementSurface(gNumberOfElements);

    intArray nodeTranslation(gNumberOfNodes); nodeTranslation=-1;
    intArray faceTranslation(gNumberOfFaces); faceTranslation=-1;

    int numberOfNodes=0;
    int numberOfFaces=0;
    int e=0;
    for( int i=0; i<gNumberOfElements; i++ )
    {
      if( cs.isVisible(gElementSurface(i)) )
      {
        // first add any new nodes we find to the list of new nodes.
        int m;
        for( m=0; m<3; m++ )
	{
	  int n0=gElement(i,m);
	  if( nodeTranslation(n0)==-1 )
	  {
	    // this is a new node
            // nodeTranslation(old node number) = new node number
	    nodeTranslation(n0)=numberOfNodes;

	    node(numberOfNodes,0)=gNode(n0,0);
	    node(numberOfNodes,1)=gNode(n0,1);
	    node(numberOfNodes,2)=gNode(n0,2);
	  
	    numberOfNodes++;
	  }
	}
	
        // now add any new faces we find to the list of new faces.
 	for( m=0; m<3; m++ )
 	{
 	  int f=gef(i,m);
 	  if( faceTranslation(f)==-1 )
 	  {
 	    // this is a new face
            // faceTranslation(old face number) = new face number.
            faceTranslation(f)=numberOfFaces;

            face(numberOfFaces,0)=nodeTranslation(gFace(f,0));
            face(numberOfFaces,1)=nodeTranslation(gFace(f,1));

            faceElement(numberOfFaces,0)=e;
	    
	    numberOfFaces++;
 	  }
          else 
	  {
            faceElement(faceTranslation(f),1)=e;
	  }
	  
          ef(e,m)=faceTranslation(f);
 	}


	element(e,0)=nodeTranslation(gElement(i,0));
	element(e,1)=nodeTranslation(gElement(i,1));
	element(e,2)=nodeTranslation(gElement(i,2));

        elementSurface(e)=gElementSurface(i);

	e++;
	

      }
    }
    int  numberOfElements=e;
    
    node.resize(numberOfNodes,3);
    element.resize(numberOfElements,3);
    face.resize( numberOfFaces,2);
    globalTriangulationForVisibleSurfaces->
      setNodesAndConnectivity(node, element, face, faceElement, ef, numberOfFaces,-1,2,true);

    elementSurface.resize(numberOfElements);
    globalTriangulationForVisibleSurfaces->setTags(elementSurface);

    globalTriangulationForVisibleSurfaces->checkConnectivity();
    
  } // end if globalTriangulation != NULL

  return 0;
}



// =================================================================================================
//! Return the status of an edge curve.
/*!
   \param number (input): edge curve number.
   \return The status of the edge curve.
 */
// =================================================================================================
CompositeTopology::EdgeCurveStatusEnum CompositeTopology::
getEdgeCurveStatus(int number)
{
  if ( allEdges == NULL )
  {
    setupAllEdges();
  }

  if (number < 0 || number >= numberOfUniqueEdgeCurves || allEdges[number] == NULL)
  {
    return edgeCurveIsNotDefined;
  }
  else
  {
    EdgeCurveStatusEnum retCode;
    
    switch (allEdges[number]->status)
    {
    case EdgeInfo::edgeCurveIsBoundary:
      retCode = edgeCurveIsNotMerged;
      break;
    case EdgeInfo::edgeCurveIsMaster:
      retCode = edgeCurveIsMerged;
      break;
    case EdgeInfo::edgeCurveIsSlave:
    case EdgeInfo::edgeCurveIsNotUsed:
      retCode = edgeCurveIsRemoved;
      break;
    default:
      retCode = edgeCurveIsNotDefined;
      break;
    }
    
    return retCode;
  }
  
}

int CompositeTopology::
getNumberOfEdgeCurves()
{
  if ( allEdges == NULL )
    setupAllEdges();

  return numberOfUniqueEdgeCurves;
}


// =================================================================================================
//! Return a reference to an edge curve.
/*!
   \param number (input): edge curve number.
   \return A reference to the edge curve.
 */
// =================================================================================================
Mapping& CompositeTopology::
getEdgeCurve(int number)
{
  if ( allEdges == NULL )
  {
    setupAllEdges();
  }
  if (number < 0 || number >= numberOfUniqueEdgeCurves || allEdges[number] == NULL)
  {
    printf("CompositeTopology::getEdgeCurve:ERROR: number = %i out of bounds [0,%i) or allEdges[%i] "
	   "undefined\n", number, numberOfUniqueEdgeCurves, number);
    throw "error";
  }
// Since there are only boundary and master edges in allEdges, all these curves should be there
  return *allEdges[number]->curve->getNURBS(); 
}

// =================================================================================================
//! Return the number of the edge curve nearest to the point x.
/*!
   \param x[3] (input): A point in physical space.
   \return: Failure: -1. Success: Number of the edge curve. This number can be used 
            as an argument to getEdgeCurve or getEdgeCurveStatus.
 */
// =================================================================================================
int CompositeTopology::
getNearestEdge(real x[3])
{
//  printf("Entering getNearestEdge with x=(%8.2e, %8.2e, %8.2e)\n", x[0], x[1], x[2]);
  
  realArray x1(1,3), r(1,1), x2(1,3);
	
  EdgeInfoADT::traversor traversor(*searchTree);

  // build a bounding box around one endpoint for the adt tree search, twice as big as the merge tolerance
  ArraySimple<real> bb(2,3);
  EdgeInfo * eMin=NULL;             // best edge that can be merged
  real minDistance=1.e7;  // holds current best merging distance

  real delta=0.5*mergeTolerance;
  int outerIter=0;
  
  for (outerIter=0; outerIter<5; outerIter++)
  {
    delta *= 2;
    
    bb(0,0)=x[0]-delta, bb(1,0)=x[0]+delta;
    bb(0,1)=x[1]-delta, bb(1,1)=x[1]+delta;
    bb(0,2)=x[2]-delta, bb(1,2)=x[2]+delta;

    traversor.setTarget(bb);

    while( !traversor.isFinished() )
    {
      EdgeInfo * e = (*traversor).data;

      if( e->status == EdgeInfo::edgeCurveIsBoundary ||
	  e->status == EdgeInfo::edgeCurveIsMaster )
      {

	NurbsMapping & edge = *e->curve->getNURBS();
// check mid point on the curve by projection 

	r=-1.;
	x1(0,0) = x[0];
	x1(0,1) = x[1];
	x1(0,2) = x[2];
	edge.inverseMap(x1,r);

        r(0,0)=max(0.,min(1.,r(0,0)));  // *wdh* 030303 : restrict r to [0,1] 
	
	edge.map(r,x2);
	real dist=fabs(x[0]-x2(0,0))+fabs(x[1]-x2(0,1))+fabs(x[2]-x2(0,2));
	if( dist < minDistance )
	{
// a match or better match was found
	  if( eMin )
	  {
	    printf("getNearestEdge:WARNING:There are multiple edge curves near (%8.2e, %8.2e, %8.2e), dist and mindist are %8.2e, %8.2e.\n",
		   x[0], x[1], x[2], dist, minDistance);
	  }
	    
	  eMin=e;
	  minDistance=dist;
	}
      }
      traversor++;
    }  // while traversor
    if (eMin) break;
  } // end for outerIter
  
  int number=-1;
  if (eMin)
  {
// get the number corresponding to the nearest edge
    int i;
    for (i=0; i<numberOfUniqueEdgeCurves; i++)
    {
      if (allEdges[i] == eMin){
	number = i;
	break;
      }
      
    }

    if (number == -1)
    {
      printf("ERROR: Could not find the number corresponding to the nearest edge\n");
    }
  }
  else
  {
    printf("ERROR: Could not find an edge near (%8.2e, %8.2e, %8.2e).\n", x[0], x[1], x[2]);
  }
  
  
// Since there are only boundary and master edges in allEdges, all these curves should be there
  return number; 
}

void CompositeTopology::
setupAllEdges()
{
  EdgeInfo *e;
  int sc, f, l, n;
    
// first count the number of boundary and master edges
  numberOfUniqueEdgeCurves = 0;
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if (e->status == EdgeInfo::edgeCurveIsBoundary ||
	    e->status == EdgeInfo::edgeCurveIsMaster)
	{
	  numberOfUniqueEdgeCurves++;
	}
      }
    }
  } // end f...

// allocate the array by traversing the datastructure
  allEdges = new EdgeInfo* [numberOfUniqueEdgeCurves];
    
// initialize
  for (n=0; n<numberOfUniqueEdgeCurves; n++)
    allEdges[n] = NULL;

  n=0;
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if (e->status == EdgeInfo::edgeCurveIsBoundary ||
	    e->status == EdgeInfo::edgeCurveIsMaster)
	{
	  allEdges[n++] = e;
	}
      }
    }
  } // end f...
} // end setupAllEdges


// ===================================================================================================
//!  Locate boundary curves on a CompositeSurface. Merge boundary edge curves that form a 
//!   smooth portion of the boundary.
/*! \param numberOfBoundaryCurves (output) : number of boundary curves found.
    \param boundaryCurves (output) : Boundary curves. 
    \note This routine will increment the reference count for you.
*/
// ===================================================================================================
int CompositeTopology::
findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves )
{
  int debug=0;
  
  numberOfBoundaryCurves=0;

  
  EdgeInfo *e;
  int sc, f, l;

  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if (e->status == EdgeInfo::edgeCurveIsBoundary)
	{
	  numberOfBoundaryCurves++;
	}
      }
    }
  } // end f...

  if( numberOfBoundaryCurves>0 )
  {
    boundaryCurves= new Mapping *[numberOfBoundaryCurves];
    int b=0;
    for( f=0; f<cs.numberOfSubSurfaces(); f++ )
    {
      for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
      {
	Loop & currentLoop = faceInfoArray[f].loop[l];
	for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	     sc++, e=e->next )
	{
	  if (e->status == EdgeInfo::edgeCurveIsBoundary)
	  {
	    boundaryCurves[b++]=e->curve->getNURBS();
	  }
	}
      }
    } // end for f...

// try to join curves that seem to match well both in location and tangent
    // what about reference counting?

    // *** build an ADT search tree ***
    // first determine a global bounding box.
    ArraySimple<real> edgeCurveBox(2,3);
    real scale=0.;
    int axis;
    for( axis=0; axis<cs.getRangeDimension(); axis++ )
    {
      if( !cs.getRangeBound(Start,axis).isFinite() || !cs.getRangeBound(End,axis).isFinite() )
      {
	printf("*** WARNING: rangeBound not finite! axis=%i [%e,%e]\n",axis,
	       (real)cs.getRangeBound(Start,axis),(real)cs.getRangeBound(End,axis) );
	cs.getGrid();
      }
      real xa=cs.getRangeBound(Start,axis);
      real xb=cs.getRangeBound(End,axis);
    
      scale=max(scale,xb-xa);

      edgeCurveBox(Start,axis)=xa-.01*scale;
      edgeCurveBox(End  ,axis)=xb+.01*scale;
    
      // printf(" *** xa=%e xb=%e scale=%e\n",xa,xb,scale);
    }

    const int rangeDimension=3;
    IntADT search(rangeDimension,edgeCurveBox);

    // Fill in the search tree with the bounding box for each edgeCurve
    ArraySimple<real> bb(2,3);
    for( b=0; b<numberOfBoundaryCurves; b++ )
    {
      Mapping & map = *boundaryCurves[b];
      for( int dir=0; dir<rangeDimension; dir++ )
      {
	bb(0,dir)=map.getRangeBound(Start,dir);
	bb(1,dir)=map.getRangeBound(End  ,dir);

      }
      if( debug & 4 )
	printf(" ADT: insert edge curve: %i boundingBox=[%e,%e]x[%e,%e]x[%e,%e]\n",
	       e,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
      if( bb(0,0)>=boundingBox(0,0) && bb(1,0)<=boundingBox(1,0) &&
	  bb(0,1)>=boundingBox(0,1) && bb(1,1)<=boundingBox(1,1) &&
	  bb(0,2)>=boundingBox(0,2) && bb(1,2)<=boundingBox(1,2)  )
      {
	search.addElement(bb, b);
      }
      else
      {
	printf("ADT:ERROR: boundingBox of edge %i is out of bounds for the box for the entire surface! \n"
	       "edge boundingBox=[%e,%e]x[%e,%e]x[%e,%e]\n",
	       e,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
      }
    }


    IntADT::traversor traversor(search); 

    // boundaryCurveStatus(b) = 1 : boundary edge is not merged with another one
    //                        = 0 : boundary edge has been merged.
    IntegerArray boundaryCurveStatus(numberOfBoundaryCurves);
    boundaryCurveStatus=1;
    realArray r(1,1),x(1,3),xr1(1,3,1),xr2(1,3,1);
    real xa[3], xb[3];
    
    int newNumberOfBoundaryCurves=numberOfBoundaryCurves;
    for( b=0; b<numberOfBoundaryCurves; b++ )
    {
       if( boundaryCurveStatus(b)==0 )
         continue;  // this curve has already been merged.
       
       NurbsMapping & edge = (NurbsMapping&)(*boundaryCurves[b]);
       
       // attempt to merge this edge with other boundary curves
       // Use a tolerance based on the scale of the curve:
       real scale=0.;
       for( int dir=0; dir<rangeDimension; dir++ )
	 scale=max(scale,(real)edge.getRangeBound(End,dir)-(real)edge.getRangeBound(Start,dir));

       real tol=.005*scale; // .001*scale;

       bool merged=false;

       // check each end point to see if there are nearby edge curves.
       const realArray & g = edge.getGrid();
       for( int side=0; side<=1 && !merged; side++ )
       {
	 int n= side==0 ? 0 : edge.getGridDimensions(0)-1;

	 xa[0]=g(n,0,0,0); xa[1]=g(n,0,0,1); xa[2]=g(n,0,0,2);

	 // build a bounding box around one endpoint for the adt tree search

	 ArraySimple<real> bb(2,3);
	 real delta=tol;
	 bb(0,0)=xa[0]-delta, bb(1,0)=xa[0]+delta;
	 bb(0,1)=xa[1]-delta, bb(1,1)=xa[1]+delta;
	 bb(0,2)=xa[2]-delta, bb(1,2)=xa[2]+delta;
	 traversor.setTarget(bb);

	 
	 while( !traversor.isFinished() )
	 {
	   // printf(" (%8.2e,%8.2e,%8.2e) is inside box %i \n",x,y,z,leaf.data);
	   int b2 = (*traversor).data;
	   
           if( b2!=b && boundaryCurveStatus(b2)==1 )
	   {
	     // check to see if we can merge boundary curves b and b2
             NurbsMapping & edge2 = (NurbsMapping&)(*boundaryCurves[b2]);
             const realArray & g2 = edge2.getGrid();
	     bool tryMerging=false;

	     for( int side2=0; side2<=1; side2++ )
	     {
	       int n2= side2==0 ? 0 : edge2.getGridDimensions(0)-1;
	       xb[0]=g2(n2,0,0,0); xb[1]=g2(n2,0,0,1); xb[2]=g2(n2,0,0,2);

               real dist = fabs(xa[0]-xb[0])+fabs(xa[1]-xb[1])+fabs(xa[2]-xb[2]);
	       if( debug & 1 )
                  printf("findBoundaryCurve: check b=%i with b2=%i, dist=%8.2e, tol=%8.2e \n",b,b2,dist,tol);
               if( dist < tol )
	       {
		 // end points are close, check the derivatives to see if they are nearly parallel
                 r = side==0 ? 0. : 1.;
                 edge.map(r,x,xr1);
		 r= side2==0 ? 0. : 1.;
		 edge2.map(r,x,xr2);
		 
                 real norm1=sqrt(xr1(0,0,0)*xr1(0,0,0)+xr1(0,1,0)*xr1(0,1,0)+xr1(0,2,0)*xr1(0,2,0));
                 real norm2=sqrt(xr2(0,0,0)*xr2(0,0,0)+xr2(0,1,0)*xr2(0,1,0)+xr2(0,2,0)*xr2(0,2,0));
		 if( norm1>0. && norm2>0. )
		 {
		   real dot = (xr1(0,0,0)*xr2(0,0,0)+xr1(0,1,0)*xr2(0,1,0)+xr1(0,2,0)*xr2(0,2,0))/(norm1*norm2);
                   if( debug & 1 ) printf(" dot=%8.2e \n",dot);
		   
		   if( fabs(dot) > .7 ) // do not merge if edges meet at a corner.
		   {
		     tryMerging=true;
		     break;
		   }
		 }
		 else
		 {
		   printf("findBoundaryCurves:WARNING: tangent of edge curve has norm==0!\n");
		 }
		     
	       }
	     } // end for side2
	     if( tryMerging )
	     {
               // printf("Try to merge edge curves %i and %i\n",b,b2);
               if( boundaryCurveStatus(b)==1 )
	       {
                 // Make a new Mapping to hold the merged curve. 
                 NurbsMapping & newEdge = *new NurbsMapping();  
		 boundaryCurves[b] =&newEdge;
		 newEdge=(NurbsMapping&)edge; // deep copy.
		 boundaryCurveStatus(b)=2;
	       }


               // ***** merge curves ******
               merged = ((NurbsMapping *)boundaryCurves[b])->merge(edge2)==0;

	       if( merged )
	       {
                 if( debug &1 ) printf("++Edge curve b2=%i was merged with edge curve b=%i\n",b2,b);
		 boundaryCurveStatus(b2)=0;
		 boundaryCurveStatus(b)=2;
                 newNumberOfBoundaryCurves--;
	       }
	       else
	       {
                 if( debug &1 ) printf("Merge failed. Unable to merge edge curve b2=%i with edge curve b=%i.\n",b2,b);

	       }
	       
	     }
	   }// end if b2!=b 
	   if( merged && newNumberOfBoundaryCurves>0 )
	   {
             // if the curve was merged, go back and try again with the new end points
	     b--;
	     break;
	   }

	   traversor++;
	 }
	 
       }
    } // end for b

    if( newNumberOfBoundaryCurves!=numberOfBoundaryCurves )
    {
      // make a new list of boundary curves, removing ones that were merged.
      Mapping **temp= new Mapping *[newNumberOfBoundaryCurves];   
       int bb=0;
       for( b=0; b<numberOfBoundaryCurves; b++ )
       {
	 if( boundaryCurveStatus(b)>0 )
	 {
	   temp[bb]=boundaryCurves[b];
//            NurbsMapping & nrb = (NurbsMapping&)(*temp[bb]);

//            printf("*** boundary curve %i (b=%i)\n",bb,b);
// 	   nrb.display("");
	   
// 	   PlotStuff & gi = *Overture::getGraphicsInterface();
//            gi.erase();
//    	    PlotIt::plot(gi,nrb);


	   bb++;
	 }
       }
       delete [] boundaryCurves;
       boundaryCurves=temp;
       numberOfBoundaryCurves=newNumberOfBoundaryCurves;
       
    }
    for( b=0; b<numberOfBoundaryCurves; b++ )
      boundaryCurves[b]->incrementReferenceCount();
    
  } // end if numberOfBoundaryCurves > 0
  
  return 0;
}// end findBoundaryCurves()

// =============================================================================================
//! Initialize parameters such as the suggested deltaS and mergeTolerance.
// ============================================================================================
int CompositeTopology::
initializeTopology()
{
  real scale=0.;
  int axis;
  for( axis=0; axis<cs.getRangeDimension(); axis++ )
  {
    if( !cs.getRangeBound(Start,axis).isFinite() || !cs.getRangeBound(End,axis).isFinite() )
    {
      printf("*** WARNING: rangeBound not finite! axis=%i [%e,%e]\n",axis,
             (real)cs.getRangeBound(Start,axis),(real)cs.getRangeBound(End,axis) );
      cs.getGrid();
    }
    real xa=cs.getRangeBound(Start,axis);
    real xb=cs.getRangeBound(End,axis);
    
    scale=max(scale,xb-xa);
    boundingBox(Start,axis)=xa;
    boundingBox(End  ,axis)=xb;
    
    // printf(" *** xa=%e xb=%e scale=%e\n",xa,xb,scale);
  }
  // increase the size of the bounding box
  const real epsx=boundingBoxExtension*scale;
  for( axis=0; axis<cs.getRangeDimension(); axis++ )
  {
    boundingBox(Start,axis)-=epsx;
    boundingBox(End  ,axis)+=epsx;
  }
  
  // get average size of a bounding box
  real averageScale=0.;
  int s;
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    real xScale=0.;
    for( axis=0; axis<cs.getRangeDimension(); axis++ )
    {
      real xa=(real)cs[s].getRangeBound(Start,axis);
      real xb=(real)cs[s].getRangeBound(End,axis);
      xScale=max(xScale,xb-xa);

      // printf(" *** surface %i: axis=%i, xa=%e xb=%e \n",s,axis,xa,xb);

    }
    averageScale+=xScale;
  }
  averageScale/=cs.numberOfSubSurfaces();
   
  
  if( false &&    // *wdh* 081105 -- the cad tolerance can be funny in some cases
      cs.getTolerance()>0. )
  {
    mergeTolerance=cs.getTolerance()*5.;
  }
  else
  {
    // *wdh* 010922 mergeTolerance = scale*.001;  // scale*.0001;
    mergeTolerance = averageScale*.01;
  }
    
  deltaS = averageScale*.02;
  minNumberOfPointsOnAnEdge=4;
  printf("Initializing topology with scale=%8.2e, averageScale=%8.2e, \n"
	 "mergeTolerance=%8.2e, deltaS=%8.2e cadTolerance=%8.2e \n",scale,
	 averageScale,mergeTolerance,deltaS,cs.getTolerance());
  
  numberOfEdgeCurves=0;

  triangulationIsValid=false;

  edgeCurvesAreBuilt=false;  
  mergedCurvesAreValid=false;   // set to true when merged curves are consistent with current parameters
  recomputeEdgeCurveBoundaryNodes=false; // set to true if user changes deltaS
  
  maximumMergeDistance=0.;
  averageMergeDistance=0.;
  minimumUnmergedDistance=REAL_MAX;
    
  return 0;
}

//! Split an edge curve into two.
/*!
   Utility routine.
   \return values: 0==success, 1==failure
 */
int CompositeTopology::
splitEdge( EdgeInfo **ePP, real rSplit, int & debug, bool mergeNewEdges /* = true */ )
{
  EdgeInfo *eOrig = *ePP;
  Loop &currentLoop = *eOrig->loopy;
  
// e should be a boundaryEdge and have orientation == 1
  if (eOrig->status != EdgeInfo::edgeCurveIsBoundary || eOrig->orientation != 1)
    printf("WARNING: splitEdge: eOrig->status = %i and eOrig->orientation = %i. This will cause problems "
	   "later!\n", eOrig->status, eOrig->orientation);

  if (debug & 2)
    printf("splitEdge: splitting (and removing) edge %i\n", eOrig->edgeNumber);

// split the existing NURBS
  NurbsMapping &nurbs1 = *new NurbsMapping();
  NurbsMapping &nurbs2 = *new NurbsMapping();
  nurbs1.incrementReferenceCount();
  nurbs2.incrementReferenceCount();

// also split the corresponding subCurve in parameterspace
  NurbsMapping * sc = eOrig->curve->subCurve, *sc1=NULL, *sc2=NULL;
  NurbsMapping * tc = eOrig->curve->surfaceLoop;
//    printf("splitEdge: reference count for eOrig before split: map: %i, tc: %i, sc: %i\n", 
//  	 eOrig->curve->getNURBS()->getReferenceCount(), tc->getReferenceCount(), sc->getReferenceCount());
  
// find the sub curve number in tc
  int q;
  for (q=0; q < tc->numberOfSubCurves(); q++)
  {
    if (&(tc->subCurve(q)) == sc) break;
  }
  if (q>=0 && q<tc->numberOfSubCurves())
  {
//    printf("INFO: splitEdge: subCurve has number = %i\n", q);
// split the sub curve. The new sub curves will get numbers q and q+1
// old way
// splitSubCurve decrements the reference count of subcurve q and deletes it if the count is zero
    if (tc->splitSubCurve(q, rSplit) != 0)
    {
// something went wrong
      if (nurbs1.decrementReferenceCount() == 0)
	delete &nurbs1;
      if (nurbs2.decrementReferenceCount() == 0)
	delete &nurbs2;
      return 1;
    }
    
// NOTE: splitSubCurve increments the reference count of the new subcurves
    sc1 = &(tc->subCurve(q));
    sc2 = &(tc->subCurve(q+1));
  }
  else
  {
    printf("WARNING: splitEdge: subCurve could not be found in trimcurve\n");
  }
  
// split the 3D curve
  if (eOrig->curve->getNURBS()->split(rSplit, nurbs1, nurbs2)!=0)
  {
// something went wrong
    if (nurbs1.decrementReferenceCount() == 0)
      delete &nurbs1;
    if (nurbs2.decrementReferenceCount() == 0)
      delete &nurbs2;
    return 1;
  }

  int numLines=max(minNumberOfPointsOnAnEdge, int( fabs(rSplit) * eOrig->curve->numberOfGridPoints+.5) );
  nurbs1.setGridDimensions(axis1,numLines);
  
// the starting point info is overwritten below
  CurveSegment *curve1 = new CurveSegment(nurbs1, numberOfEndPoints, eOrig->curve->surfaceNumber, tc, sc1); 
  
// orientation of boundary curves is always 1
  EdgeInfo *e1 = new EdgeInfo(curve1, eOrig->loopNumber, eOrig->faceNumber, 1, numberOfEdgeCurves);
  numberOfEdgeCurves++;
  if (debug & 2)
    printf("splitEdge: making new edge1: %i\n", e1->edgeNumber);
   
  e1->curve->numberOfGridPoints=numLines;
  e1->curve->arcLength*=max(.01,rSplit);  // scale the arcLength

  e1->status = EdgeInfo::edgeCurveIsBoundary;
// need to fill in starting/ending point info and more...
  e1->curve->startingPoint = eOrig->curve->startingPoint;
  e1->curve->newStartPoint = eOrig->curve->newStartPoint;
  e1->curve->endingPoint = -1; // to be changed below
  e1->curve->newEndPoint = -1;
  e1->curve->numberOfGridPoints = numLines; // assigned above
  e1->curve->arcLength = eOrig->curve->arcLength*rSplit; // scale the arcLength
//  printf("Estimated arclength edge %i: %e\n", e1->edgeNumber, e1->curve->arcLength);
  
// replace eOrig by e1 in the current loop
  if (!currentLoop.replaceEdge(e1, eOrig))
    printf("Warning: replacing eOrig by e1 failed...\n");

// set the grid size for the second piece
  
  numLines=max(minNumberOfPointsOnAnEdge, int( fabs(1.-rSplit)*eOrig->curve->numberOfGridPoints + 0.5) );
  nurbs2.setGridDimensions(axis1,numLines);

// evaluate the starting point of piece 2
  const realArray & x = nurbs2.getGrid();

  if (endPoint.getLength(0) <= numberOfEndPoints)
  {
    masterEdge.resize(numberOfEndPoints+1000);
    endPoint.resize(numberOfEndPoints+1000,3);
  }
  RealArray newLocation(3);
  for (q=0; q<3; q++)
  {
    endPoint(numberOfEndPoints, q) = x(0, 0, 0, q); // starting point
    newLocation(q) = x(0, 0, 0, q); // used below in moveEndpoint
  }
  
// modify ending point info for first piece
  e1->curve->endingPoint = numberOfEndPoints; // endpoint for the first piece

// modify the first piece so it exactly ends at the starting point of the second piece!
  e1->curve->getNURBS()->moveEndpoint(1, newLocation);

  CurveSegment *curve2 = new CurveSegment(nurbs2, numberOfEndPoints, eOrig->curve->surfaceNumber, tc, sc2);
  
// orientation of boundary curves is always 1
  EdgeInfo * e2 = new EdgeInfo(curve2, eOrig->loopNumber, eOrig->faceNumber, 1, numberOfEdgeCurves); 
  numberOfEdgeCurves++;
  if (debug & 2)
    printf("splitEdge: making new edge2: %i\n", e2->edgeNumber);
   
// the newEdge initially rules the starting point
  masterEdge.array[numberOfEndPoints] = e2;

  e2->status = EdgeInfo::edgeCurveIsBoundary;

// need to fill in starting/ending point info and more...
// starting point is assigned above
  e2->curve->endingPoint = eOrig->curve->endingPoint;
  e2->curve->newEndPoint = eOrig->curve->newEndPoint;
  e2->curve->numberOfGridPoints=numLines;
  e2->curve->arcLength = eOrig->curve->arcLength*(1.-rSplit); // scale the arcLength
//  printf("Estimated arclength edge %i: %e\n", e2->edgeNumber, e2->curve->arcLength);
		
  if( debug & 2 )
    printf("split edge eOrig = %i into e1=%i and e2=%i, split at r0=%8.2e\n", 
	   eOrig->edgeNumber ,e1->edgeNumber ,e2->edgeNumber , rSplit);
  		
// insert e2 after e1 in the linked list, assign starting and ending point info
  currentLoop.addEdge(e2, e1);
  
// modify masterEdge info (the orientation is == 1 for all these edges, since these are boundary edges)
  if (masterEdge.array[e1->getStartPoint()] == eOrig)
    masterEdge.array[e1->getStartPoint()] = e1;
  if (masterEdge.array[e2->getEndPoint()] == eOrig)
    masterEdge.array[e2->getEndPoint()] = e2;

  if( mergeNewEdges )
  {
    // try to merge the new pieces
    if (merge( e1, debug )==1)
    {
      if (debug & 2) printf("Merging successful for the split edge %i\n", e1->edgeNumber);
    }
    else
    {
      if (debug & 2) printf("Merging could not be done for the split edge %i\n", e1->edgeNumber);
    }
    
    if (merge( e2,debug )==1)
    {
      if (debug & 2) printf("Merging successful for the split edge %i\n", e2->edgeNumber);
    }
    else
    {
      if (debug & 2) printf("Merging could not be done for the split edge %i\n", e2->edgeNumber);
    }
    
  }
  
// add the new edge curve2 to the ADT tree
  ArraySimple<real> bb(2,3);
// first piece
  bb(0,0)=(real)nurbs1.getRangeBound(Start,0), bb(1,0)=(real)nurbs1.getRangeBound(End,0);
  bb(0,1)=(real)nurbs1.getRangeBound(Start,1), bb(1,1)=(real)nurbs1.getRangeBound(End,1);
  bb(0,2)=(real)nurbs1.getRangeBound(Start,2), bb(1,2)=(real)nurbs1.getRangeBound(End,2);

  searchTree->addElement(bb,e1);		  
  if( debug & 4 )
    printf("ADT: add edge e1=%i, bbox =[%e,%e]x[%e,%e]x[%e,%e]\n",
	   e1->edgeNumber,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));

// second piece
  bb(0,0)=(real)nurbs2.getRangeBound(Start,0), bb(1,0)=(real)nurbs2.getRangeBound(End,0);
  bb(0,1)=(real)nurbs2.getRangeBound(Start,1), bb(1,1)=(real)nurbs2.getRangeBound(End,1);
  bb(0,2)=(real)nurbs2.getRangeBound(Start,2), bb(1,2)=(real)nurbs2.getRangeBound(End,2);

  searchTree->addElement(bb,e2);		  
  if( debug & 4 )
    printf("ADT: add edge e2=%i, bbox =[%e,%e]x[%e,%e]x[%e,%e]\n",
	   e2->edgeNumber,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));

  numberOfEndPoints++;
  
// cleanup

// since eOrig was replaced in the Loop there is nothing that points to it anymore, except the ADT
// search tree. We store pointers to all unused edge curves in the array unusedEdges
  eOrig->setUnused(unusedEdges);

//  printf("splitEdge: Pushing edge #%i onto the unusedEdges stack\n", eOrig->edgeNumber);
  if (currentLoop.edgeInLoop(eOrig))
    printf("splitEdge: ERROR: edge %i was replaced but is still in the Loop!\n", eOrig->edgeNumber);
  
  *ePP = e1;
  
  if (nurbs1.decrementReferenceCount()==0)
    delete &nurbs1;
  if (nurbs2.decrementReferenceCount()==0)
    delete &nurbs2;
  
//    printf("splitEdge: reference count for e1 after split: map: %i, tc: %i, sc: %i\n", 
//  	 e1->curve->getNURBS()->getReferenceCount(), tc->getReferenceCount(), sc1->getReferenceCount());
//    printf("splitEdge: reference count for e2 after split: map: %i, tc: %i, sc: %i\n", 
//  	 e2->curve->getNURBS()->getReferenceCount(), tc->getReferenceCount(), sc2->getReferenceCount());

  if (!checkConsistency(true))
    return 1;
  else
    return 0;
}


// ======================================================================================================
//! Split and merge edge curves.
// ======================================================================================================
int CompositeTopology::
splitAndMergeEdgeCurves( GenericGraphicsInterface & gi, int & debug )
{
//  printf("Entering splitAndMergeEdgeCurves\n");
  
  triangulationIsValid=false;

  int s;
  real timeForInverseMap=0.;
  real time2=getCPU();

  // *** first try to merge as many edge curves as possible (before splitting) ****
  maximumMergeDistance=0.;
  averageMergeDistance=0.;
  minimumUnmergedDistance=REAL_MAX;

  gi.outputString("Merge...");
  int f, l, sc, retCode;
  EdgeInfo *e;
  bool mergeFailed = false;

  real time1=getCPU();
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
	if( e->status == EdgeInfo::edgeCurveIsBoundary )  
	{
	  if (merge( e, debug ) == -1)
	    mergeFailed=true;
	}
    } // end for all loops
  } // end for all faces (subsurfaces)

  if (mergeFailed)
    printf("WARNING: Merge returned an error at least once\n");  

// check consistency of all loops
  if (!checkConsistency())
    return 1;
  
// move curve endpoints to make all corners consistent (can make the search tree inaccurate)
  adjustEndPoints();

// check consistency of all loops
  if (!checkConsistency())
    return 1;

// tmp output all edges we have
//    printf("after straight merge\n");
//    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
//    {
//      for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
//      {
//        Loop & currentLoop = faceInfoArray[s].loop[l];
//        int sc;
//        EdgeInfo *e;
//        for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
//  	   sc++, e=e->next )
//        {
//  	printf("EdgeInfo object: curve->mapID:%i, initialCurve->mapID:%i\n",
//  	       e->curve->getNURBS()->getGlobalID(), e->initialCurve->getNURBS()->getGlobalID());
//        }
//      }
//    }
// end tmp      

  // **** now split and merge edge curves *********
  real timeToSplit=getCPU();
  gi.outputString("split and merge...");
  EdgeInfoADT::traversor traversor(*searchTree);
      
  realArray x(1,3), x2(1,3),r(1,3);
  int q;
  bool someSplitsFailed=false;
  real newSplitToleranceFactor = splitToleranceFactor;
  
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
	if( e->status == EdgeInfo::edgeCurveIsBoundary )  
	{
      // attempt to split this edge curve
// important to use the new starting and ending points, since e might have been modified before
	  int startP = e->getStartPoint();
	  int endP = e->getEndPoint();
	  NurbsMapping & edge = *e->curve->getNURBS();

	  real xa[3], xb[3];
	  for (q=0; q<3; q++)
	  {
	    xa[q]=endPoint(startP,q);
	    xb[q]=endPoint(endP,q);
	  }
	  
	  ArraySimple<real> bb(2,3);
	  real bb2[6];
	  real delta=mergeTolerance*2.;
	  bb(0,0)=(real)edge.getRangeBound(Start,0)-delta, bb(1,0)=(real)edge.getRangeBound(End,0)+delta;
          bb(0,1)=(real)edge.getRangeBound(Start,1)-delta, bb(1,1)=(real)edge.getRangeBound(End,1)+delta;
	  bb(0,2)=(real)edge.getRangeBound(Start,2)-delta, bb(1,2)=(real)edge.getRangeBound(End,2)+delta;

// we need to increase the bounding box since it may be slightly inaccurate
	  const real scale=max(bb(1,0)-bb(0,0), bb(1,1)-bb(0,1), bb(1,2)-bb(0,2));
	  const real epsx=scale*boundingBoxExtension*.9; // decrease by a bit less than the global to stay inside
	  bb(0,0)-=epsx;  bb(1,0)+=epsx;
	  bb(0,1)-=epsx;  bb(1,1)+=epsx;
	  bb(0,2)-=epsx;  bb(1,2)+=epsx;
	  
	  traversor.setTarget(bb);

	  if( debug & 2 )
	    printf("\n*** attempt to split edge e=%i (s=%i) box=[%10.4e,%10.4e]x[%10.4e,%10.4e]x[%10.4e,%10.4e]\n",
	       e->edgeNumber, e->faceNumber, bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
	  

// iterate until e gets merged
	  while( !traversor.isFinished() && e->status==EdgeInfo::edgeCurveIsBoundary ) 
	  {
	    EdgeInfo * e2 = (*traversor).data;
	    
	// only check edges that belong to a different surface
	    if( e->faceNumber != e2->faceNumber )
	    {
	    
	      if( debug & 4 )
	      {
		if( e2->status==EdgeInfo::edgeCurveIsSlave )
		  printf("Split e=%i(s=%i) with e2=%i(s=%i) ? No since e2 is removed (merged with another)\n",
			 e->edgeNumber,e->faceNumber,e2->edgeNumber,e2->faceNumber);
	      }
	    
	      if ( e2->status != EdgeInfo::edgeCurveIsSlave && e2->status != EdgeInfo::edgeCurveIsNotUsed && 
		   e2 != e )
	      {
// important to use the new starting and ending points, since e2 might been modified before
		int startP2 = e2->getStartPoint();
		int endP2 = e2->getEndPoint();
		real xa2[3], xb2[3];
		for (q=0; q<3; q++)
		{
		  xa2[q]=endPoint(startP2, q);
		  xb2[q]=endPoint(endP2, q);
		}
		
		for( int side=0; side<=1; side++ )
		{
		  if( side==0 )
		  {
		    x2(0,0)=xa2[0], x2(0,1)=xa2[1], x2(0,2)=xa2[2];
		  }
		  else
		  {
		    x2(0,0)=xb2[0], x2(0,1)=xb2[1], x2(0,2)=xb2[2];
		  }
// check to see if the end point is in the bounding box.
		  if( x2(0,0)<bb(0,0) || x2(0,0)>bb(1,0) ||
		      x2(0,1)<bb(0,1) || x2(0,1)>bb(1,1) ||
		      x2(0,2)<bb(0,2) || x2(0,2)>bb(1,2) )
		  {
		    if( debug & 4 )
		    {
		      printf("Check: e=%i(s=%i) to be split by e2=%i(s=%i)? no, end point outside bbox\n",
			     e->edgeNumber, e2->faceNumber, e2->edgeNumber, e2->faceNumber);
		      printf(" side=%i x=(%8.2e,%8.2e,%8.2e) "
			     " bb=[%8.2e,%8.2e][%8.2e,%8.2e][%8.2e,%8.2e]\n", side,
			     x2(0,0),x2(0,1),x2(0,2),bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
		    }
		    continue;
		  }
		  
		  real timen=getCPU();
		  r=-1.;
		  edge.inverseMap(x2,r);
		  if( debug & 4 )
		  {
		    printf("Split:check e=%i to be split by e2=%i on side=%i, r_e=%8.2e "
			   "x_e2=(%12.6e,%12.6e,%12.6e)\n",
			   e->edgeNumber,e2->edgeNumber,side,r(0,0),x2(0,0),x2(0,1),x2(0,2));
		  }

		  if( r(0,0)<0. || r(0,0)>1. )
		  {
		    timeForInverseMap+=getCPU()-timen;
		    continue;
		  }
		  edge.map(r,x);
		  timeForInverseMap+=getCPU()-timen;
	      
		  if( debug & 4 )
		  {
		    printf("Split:check x_e=(%12.6e,%12.6e,%12.6e)\n", x(0,0), x(0,1), x(0,2));
		  }

		  real dist=fabs(x(0,0)-x2(0,0))+fabs(x(0,1)-x2(0,1))+fabs(x(0,2)-x2(0,2));
// dista,distb  : distance from the endpoints of the edge curve to the split point
// do NOT split the edge curve if dista or distb is too small

		  real splitTolerance= mergeTolerance*splitToleranceFactor;
		
		  real dista=fabs(x(0,0)-xa[0])+fabs(x(0,1)-xa[1])+fabs(x(0,2)-xa[2]);
		  real distb=fabs(x(0,0)-xb[0])+fabs(x(0,1)-xb[1])+fabs(x(0,2)-xb[2]);

		  if( debug & 4 )
		    printf("INFO: split edge e=%i with end point of e2=%i(s=%i) ? dist=%8.2e, "
			   "dista=%8.2e, distb=%8.2e (tol=%8.2e,%8.2e)\n",
			   e->edgeNumber, e2->edgeNumber, e2->faceNumber, dist, dista, distb, 
			   mergeTolerance,splitTolerance);

		  if( debug & 4 && (dist<mergeTolerance &&( dista<splitTolerance || distb<splitTolerance) ))
		  {
		    printf("INFO: don't split edge e=%i with end pnt of e2=%i (side=%i) since it is too close to one of e's endpoints.\n"
			   "Midpoint distance: %8.2e, mergeTolerance: %8.2e\n"
			   "Endpoint distances: (dista=%8.2e, distb=%8.2e), splitTolerance=%8.2e\n",
			   e->edgeNumber, e2->edgeNumber, side, 
			   dist, mergeTolerance,
			   dista, distb, splitTolerance);
		  }
// output this info to indicate when it would help to reduce the split tolerance factor
		  if( dist<mergeTolerance && min(dista, distb) < splitTolerance && min(dista, distb) >= mergeTolerance )
		  {
		    someSplitsFailed = true;
		    newSplitToleranceFactor = min(newSplitToleranceFactor, min(dista, distb)/mergeTolerance);
		  }

		  if( dist<mergeTolerance && dista>splitTolerance && distb>splitTolerance )
		  {
                    
		    if( debug & 2 )
		    {
		      printf(" **e=%i(s=%i), e2=%i(s=%i) dist=%e : split edge e at r=%e,\n"
			     "Endpoint distances: %e, %e\n",
			     e->edgeNumber, e->faceNumber, e2->edgeNumber, e2->faceNumber, dist, 
			     r(0,0), dista, distb);
		      printf("   e=%i has end points [%8.2e,%8.2e,%8.2e]->[%8.2e,%8.2e,%8.2e]\n",
			     e->edgeNumber, xa[0],xa[1],xa[2],xb[0],xb[1],xb[2]);
		      printf("  e2=%i has end points [%8.2e,%8.2e,%8.2e]->[%8.2e,%8.2e,%8.2e]\n",
			     e2->edgeNumber, xa2[0],xa2[1],xa2[2],xb2[0],xb2[1],xb2[2]);
		      printf(" e2 intersects e at x=[%8.2e,%8.2e,%8.2e]\n",x(0,0),x(0,1),x(0,2));
		      
		    }
		    
		    // split edge curve e and attempt to merge the resulting segments.
// Now do the actual splitting and try to merge both pieces with other edges in the model
		    EdgeInfo *eOld = e;
// note that e will be modified by splitEdge
// exit if it fails
		    if (splitEdge( &e, r(0,0), debug ) != 0) return 1;
		    
		    if( eOld != e ) 
		    {
// re-evaluate the end points
		      edge = *e->curve->getNURBS();
		      startP = e->getStartPoint();
		      endP = e->getEndPoint();
		      for (q=0; q<3; q++)
		      {
			xa[q]=endPoint(startP,q);
			xb[q]=endPoint(endP,q);
		      }
		    }

		    break;
		  } // end if dist < mergeTolerance...
		  
		  
		} // end for side...
		
		
	      }  // if( e2->status
	      
	    } // if ( e->faceNumber != e2->faceNumber...
	    
	    traversor++;
	  } // end while( !traversor...
	} // end if e->status == EdgeInfo::edgeIsBoundary
    } // end for all loops
  } // end for all faces

  timeToSplit=getCPU()-timeToSplit;

  if (someSplitsFailed)
  {
    aString buf;
    gi.outputString("Some splits didn't happen because the split point was too close to an endpoint");
    gi.outputString(sPrintF(buf, "You would allow all of those splits to occur by reducing the split tolerance factor to %8.2e", 
			    newSplitToleranceFactor));
  }
  

// move curve endpoints to make all corners consistent (this makes the seach tree slightly inaccurate)
  adjustEndPoints();
  
// check consistency of all loops
  if (!checkConsistency())
    return 1;
  
// make a final adjustment of all start/end points used by master and boundary edges 
// (mostly to make it plot perfectly)
  RealArray newLocation(3);
  real startDist, endDist;
  
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
	if (e->status == EdgeInfo::edgeCurveIsBoundary ||
	    e->status == EdgeInfo::edgeCurveIsMaster)
	{
// adjust starting end
	  for (q=0; q<3; q++)
	    newLocation(q) = endPoint(e->curve->startingPoint,q);
	  e->curve->getNURBS()->moveEndpoint(0, newLocation);
// adjust ending end
	  for (q=0; q<3; q++)
	    newLocation(q) = endPoint(e->curve->endingPoint,q);
	  e->curve->getNURBS()->moveEndpoint(1, newLocation);
// check the distance
//  	  const realArray &x= e->curve->getNURBS()->getGrid(); // consistent with mapped coordinates at start/end
//  	  const realArray &c= e->curve->getNURBS()->getControlPoints(); // always at the true start/end
	  
//  	  int xN = x.getBound(0), cN = c.getBound(0);
	  
//  	  startDist = 0.;
//  	  for (q=0; q<3; q++)
//  	    startDist += SQR(endPoint(e->curve->startingPoint,q) - x(0,0,0,q));
//  	  startDist = sqrt(startDist);
//  	  endDist = 0.;
//  	  for (q=0; q<3; q++)
//  	    endDist += SQR(endPoint(e->curve->endingPoint,q) - x(xN,0,0,q));
//  	  endDist = sqrt(endDist);
//  	  printf("edge %i, startDist=%e, endDist=%e\n", e->edgeNumber, startDist, endDist);
//  	  startDist = 0.;
//  	  for (q=0; q<3; q++)
//  	    startDist += SQR(endPoint(e->curve->startingPoint,q) - c(0,q));
//  	  startDist = sqrt(startDist);
//  	  endDist = 0.;
//  	  for (q=0; q<3; q++)
//  	    endDist += SQR(endPoint(e->curve->endingPoint,q) - c(cN,q));
//  	  endDist = sqrt(endDist);
//  	  printf("edge %i, control point startDist=%e, endDist=%e\n", e->edgeNumber, startDist, endDist);
	} // end for e / if boundary or master edge
    } // end for l
  } // end for f
  

//  printf("Exiting splitAndMergeEdgeCurves\n");

//    printf("MasterEdge:\n");
//    for (int i=0; i<numberOfEndPoints; i++)
//      printf("masterEdgeNumber[%i] = %i\n", i, masterEdge.array[i]->edgeNumber);
  
  time2=getCPU()-time2;
  printf("Time to merge edge curves=%8.2e, (time for split and merge=%8.2e)\n",time2,timeToSplit);

  return 0;
} // end splitAndMergeEdges()


void CompositeTopology::
adjustEndPoints()
{
  printf("Adjusting end points of curve segments...\n");
  
  int f, l, sc;
  EdgeInfo *e;
  
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if (!e->adjustOneSegmentEndPoints(endPoint, mergeTolerance))
	{
	  printf("Warning: adjustOneSegmentEndPoints failed for edge %i\n", e->edgeNumber);
	}
	
	
      } // end for sc...
      
    }
  }
} // end adjustEndPoints

bool CompositeTopology::
checkConsistency(bool quiet /* = false */)
{
  if (!quiet)
    printf("Checking the consistency of all loops\n");
  real dist;
  
  int f, l, q, sc, ne, nErr=0;
  EdgeInfo *e;
  
  GenericGraphicsInterface &gi = *Overture::getGraphicsInterface();
  aString buf;
  
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      ne = currentLoop.numberOfEdges();
      for (sc = 0, e = currentLoop.firstEdge; sc<ne; 
	   sc++, e=e->next )
      {
	if (e->getStartPoint() != e->prev->getEndPoint())
	{
	  nErr++;
	  gi.outputString(sPrintF(buf, "Inconsistent loop %i, face %i, edge %i starting point = %i, "
				  "prev->ending point = %i", l, f, e->edgeNumber, e->getStartPoint(), 
				  e->prev->getEndPoint()));
	}
	if (e->getEndPoint() != e->next->getStartPoint())
	{
	  nErr++;
	  gi.outputString(sPrintF(buf, "Inconsistent loop %i, face %i, edge %i ending point = %i, "
				  "next->starting point = %i", l, f, e->edgeNumber, e->getEndPoint(), 
				  e->next->getStartPoint()));
	}
	if (e->getStartPoint() == e->getEndPoint())
	{
          if( ne==1 )
	  {
	    // could be a periodic loop *wdh* 030825
            NurbsMapping & edge = *e->curve->getNURBS();
            if( edge.getIsPeriodic(0)==Mapping::functionPeriodic )
	    {
              gi.outputString(sPrintF(buf,"checkConsistency:loop %i, face %i, edge %i,"
                   " end pts match but the edge curve is periodic (getIsPeriodic=%i)",l, f, e->edgeNumber,
                   (int)edge.getIsPeriodic(0) ));
	      continue;
	    }
	  }
	  nErr++;
	  gi.outputString(sPrintF(buf, "Inconsistent loop %i, face %i, edge %i starting point = %i "
				  "same as ending point = %i", l, f, e->edgeNumber, e->getStartPoint(), 
				  e->getEndPoint()));
	}
	
      } // end for sc...
      
    }
  }
  
  if (nErr == 0)
  {
    if (!quiet)
      gi.outputString("Consistency check: All loops appear to be closed");
    return true;
  }
  else
  {
    return false;
  }
} // end checkConsistency


//! Split an edge curve at a given point.
/*!
   This function is called when the user wants to explicitly split an edge curve.
  /param e (input): split this edge curve
  /param xSplit (input): split curve at this point.
  /return: 0==success.
 */



// ===============================================================================================
//! Fill in the search tree with the bounding box for each edgeCurve
/*!
   \debug(input): debug level
 */
// ===============================================================================================
int CompositeTopology::
buildEdgeCurveSearchTree( int & debug )
{
  const int rangeDimension=cs.getRangeDimension();
  delete searchTree;
  searchTree = new EdgeInfoADT(rangeDimension, boundingBox);

  ArraySimple<real> bb(2,3);
  int f, l, sc;
  EdgeInfo *e;

// tmp
//    printf("buildEdgeCurveSearchTree: boundingBox of the entire surface:\n"
//  	 "[%e,%e]x[%e,%e]x[%e,%e]\n", boundingBox(0,0), boundingBox(1,0),
//  	 boundingBox(0,1), boundingBox(1,1), boundingBox(0,2), boundingBox(1,2));

  real time1=getCPU();
  for( f=0; f<cs.numberOfSubSurfaces(); f++ )
  {
    for (l=0; l<faceInfoArray[f].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[f].loop[l];
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
// some edges might have been joined during the initial build of the search tree
	if (e->status == EdgeInfo::edgeCurveIsNotUsed) continue;
	
	NurbsMapping &nurb = *(e->curve->getNURBS());
	real scale=0.;
	int dir;
	for( dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0,dir)=nurb.getRangeBound(Start,dir);
	  bb(1,dir)=nurb.getRangeBound(End  ,dir);
	  scale=max(scale,bb(1,dir)-bb(0,dir));
	}
	// increase bounding box since bounds are based on a set of grid points representing map.
	const real epsx = scale*boundingBoxExtension*.9; // decrease by a bit less than the global to stay inside
	for( dir=0; dir<rangeDimension; dir++ )
	{
	  bb(0,dir)-=epsx;
	  bb(1,dir)+=epsx;
	}
	
	if( bb(0,0)>=boundingBox(0,0) && bb(1,0)<=boundingBox(1,0) &&
	    bb(0,1)>=boundingBox(0,1) && bb(1,1)<=boundingBox(1,1) &&
	    bb(0,2)>=boundingBox(0,2) && bb(1,2)<=boundingBox(1,2)  )
	{
	  searchTree->addElement(bb, e);
	}
	else
	{
	  printf("ADT:ERROR: boundingBox of edge %i is out of bounds for the box for the entire surface! \n"
		 "edge boundingBox=[%e,%e]x[%e,%e]x[%e,%e]\n",
		 e->edgeNumber,bb(0,0),bb(1,0),bb(0,1),bb(1,1),bb(0,2),bb(1,2));
	}
      } // end for all edgeInfo's
    } // end for all loops
  } // end for all faces (subsurfaces)
  
  printf("Time to build ADT tree from %i EdgeInfo's = %8.2e\n", numberOfEdgeCurves, getCPU()-time1);

  return 0;
}

//! Print a summary of the number of edge curves of the various types.
int CompositeTopology::
printEdgeCurveInfo(GenericGraphicsInterface & gi)
{
  int numberMerged=0;
  int numberNotMerged=0;
  int s, l, sc;
  EdgeInfo *e;
  
  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    for ( l=0; l<faceInfoArray[s].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[s].loop[l];
      int sc;
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if (e->status != EdgeInfo::edgeCurveIsSlave)
	{
	  if( e->status == EdgeInfo::edgeCurveIsBoundary )
	    numberNotMerged++;
	  else if( e->status == EdgeInfo::edgeCurveIsMaster )
	    numberMerged++;
	}
	  
      }
    } // end for all loops
  } // end for all subsurfaces
  aString buff;
  gi.outputString(sPrintF(buff,
    "INFO: numberOfEdgeCurves=%i, number merged=%i (green), number not merged=%i(blue), number removed=%i,"
    " red=three or more edges meet (non-manifold geometry)",
	 numberOfEdgeCurves,numberMerged,numberNotMerged,numberOfEdgeCurves-numberMerged-numberNotMerged));

  return 0;
}

// ===============================================================================================
//! Prompt for another edge
// /param edgeChosen (output): this edge was chosen
// /question (input): ask this question
// /cancel (input): prompt to cancel 
// /return 0 equals success.
// ===============================================================================================
int CompositeTopology:: 
getAnotherEdge(EdgeInfo* &edgeChosen, 
               GenericGraphicsInterface & gi,
	       const aString & prompt, 
               const aString & cancel )
{
  int returnValue=0;
  GUIState doneDialog;
	  
  doneDialog.setExitCommand(cancel,cancel);

  doneDialog.addInfoLabel(prompt);

  gi.pushGUI(doneDialog);           

  aString answer;
  SelectionInfo select; select.nSelect=0;
  EdgeInfo *e;
  int numberChosen=0;
  for( int it=0; numberChosen==0; it++ )
  {
    gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

    gi.getAnswer(answer,"", select);
         
    gi.savePickCommands(true); // turn back on

    if( answer==cancel )
    {
      gi.outputString("No edges chosen.\n");
      returnValue=1;
      break;
    }
    else if( select.nSelect  )
    {
      for (int i=0; i<select.nSelect && numberChosen==0; i++)
      {
	for( int s=0; s<cs.numberOfSubSurfaces() && numberChosen==0; s++ )
	{
	  for (int l=0; l<faceInfoArray[s].numberOfLoops && numberChosen==0; l++)
	  {
	    Loop & currentLoop = faceInfoArray[s].loop[l];
	    int sc;
	    for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		 sc++, e=e->next )
	    {
	      if(e->status == EdgeInfo::edgeCurveIsBoundary &&
		 e->curve->getNURBS()->getGlobalID() == select.selection(i,0) )
	      {
		edgeChosen=e;
		numberChosen++;
		break;
	      }
	    }
	  }
	}
      }
    } // end if select.nSelect...
    
  } // end for it...
  
  gi.popGUI();

  return returnValue;
}


//! Print info about a particular edge curve
/*!
   /param e (input): edge curve number
 */
void CompositeTopology::
printInfoForAnEdgeCurve( EdgeInfo * e )
{
  printf("Sorry: new version of printInfoForAnEdgeCurve is not implemented\n");
}


// ================================================================================================
//! Interactively build the connectivity information for a CompositeSurface
//
// ============================================================================================
int CompositeTopology::
update()
{
  real time0=getCPU();
  assert( Overture::getGraphicsInterface()!=NULL );
  GenericGraphicsInterface & gi = (GenericGraphicsInterface &) *Overture::getGraphicsInterface();
  GraphicsParameters params, refSurPar;

  refSurPar.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  refSurPar.set(GI_PLOT_UNS_FACES,true);
  refSurPar.set(GI_PLOT_SHADED_MAPPING_BOUNDARIES, true);
  refSurPar.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES,false);
  refSurPar.set(GI_PLOT_UNS_EDGES,false);
  refSurPar.set(GI_PLOT_UNS_BOUNDARY_EDGES,false); // these edge curves can conflict with to subsurface edges
  refSurPar.set(GI_PLOT_MAPPING_EDGES, false);
  refSurPar.set(GI_PLOT_BLOCK_BOUNDARIES,false);
  refSurPar.set(GI_PLOT_MAPPING_NORMALS,false);

  int debug=0;
  
  const int domainDimension=cs.getDomainDimension();
  const int rangeDimension=cs.getRangeDimension();
  
// faceInfoArray holds the virtual topology datastructure
  if (faceInfoArray == NULL) 
  {
    edgeCurvesAreBuilt=false;
  }
  else
  {
    printf("**** topology has already been initialized\n");
  }

  aString buf;
  bool plotNormals=false;
  bool flatShading=false;  // *wdh* 050514 

  params.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
  params.set(GI_PLOT_UNS_EDGES,true);
  params.set(GI_UNS_FLAT_SHADING,flatShading);
  params.set(GI_PLOT_UNS_BOUNDARY_EDGES,false); // these edge curves can conflict with to subsurface edges
  params.set(GI_PLOT_BLOCK_BOUNDARIES,true);
  params.set(GI_PLOT_MAPPING_NORMALS,plotNormals);

  GUIState gui;
  gui.setWindowTitle("CompositeSurface Topology");
  gui.setExitCommand("exit", "continue");
  DialogData & dialog = (DialogData &)gui;

// *wdh* These are no longer needed: 031116
//    enum
//    {
//      buildEdgeCurvesPushButton=0,
//      mergeEdgeCurvesPushButton,
//      triangulatePushButton,
//      flipNormalsPushButton,
//      examineReferenceSurfacePushButton,
//      examineTriangulationPushButton
//    };
  
  aString pbLabels[] = {"Compute topology",
                        "Build edge curves",
			"Merge edge curves",
                        "Triangulate",
			"Flip normals",
                        "Reference surface",
                        "Examine triangulation",
			""};
  aString pbCmds[] = {"compute topology",
                      "build edge curves",
		      "merge edge curves",
		      "triangulate",
		      "flip normals",
		      "examine reference surface",
		      "examine triangulation",
		      ""};
  // addPrefix(pbLabels,prefix,cmd,maxCommands);
  int numRows=3;
  dialog.setPushButtons(pbCmds,  pbLabels, numRows ); 

// define pulldown menus
  aString pdCommand0[] = {"compute topology", "save triangulation", "exit", ""};
  aString pdLabel0[] = {"Compute topology", "Save triangulation...", "Exit", ""};
  dialog.addPulldownMenu("Topology", pdCommand0, pdLabel0, GI_PUSHBUTTON);
  
  
  aString pdCommand2[] = {"help topology", ""};
  aString pdLabel2[] = {"Topology", ""};
  dialog.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);

  dialog.setLastPullDownIsHelp(1);
// done defining pulldown menus  

  enum PickingOptionsEnum
  {
    pickToMergeEdgeCurves,
    pickToUnMergeEdgeCurves,
    pickToJoinEdgeCurves,
    pickToSplitEdgeCurves,
    pickToQueryEdgeCurves,
    pickToEditEdgeCurves,
    pickToQueryElement,
    pickingOff
  };
  PickingOptionsEnum pickingOption=pickToQueryEdgeCurves;
  
  aString opLabel1[] = {"merge edges","un-merge edges","join edges",
                        "split edges",
                        "query edges","edit edges",
                        "query element","off",""};
  // GUIState::addPrefix(opLabel1,"picking:",cmd,maxCommands);
  // dialog.addOptionMenu("Picking:", opLabel1,opLabel1,(int)pickingOption);
  int numberOfColumns=3;
  dialog.addRadioBox("Picking:", opLabel1,opLabel1,(int)pickingOption,numberOfColumns);

  bool plotReferenceSurface=false;
  bool plotTriangulatedSurface=true;
  bool plotEdgeCurves=true;
  bool plotMergedCurves=true;
  bool plotUnMergedCurves=true;
  bool plotBadElements=true;
  
// *wdh* These are no longer needed: 031116
//    enum toggleButtonEnum{plotReferenceSurfaceTB=0, plotTriangulatedSurfaceTB, plotEdgeCurvesTB, 
//  			plotMergedCurvesTB, plotUnMergedCurvesTB, plotBadElementsTB , plotNormalsTB, 
//  			flatShadingTB, improveTriTB, 
//  			numberOfToggleButtons  };
  

  aString tbCommands[] = {"plot reference surface",
                          "plot triangulated surface",
                          "plot edge curves",
                          "plot green merged curves",
                          "plot blue unmerged curves",
                          "plot bad elements",
			  "plot normals",
			  "flat shading",
			  "improve triangulation",
			  ""};
  int tbState[9];
  tbState[0] = plotReferenceSurface;
  tbState[1] = plotTriangulatedSurface;
  tbState[2] = plotEdgeCurves;
  tbState[3] = plotMergedCurves;
  tbState[4] = plotUnMergedCurves;
  tbState[5] = plotBadElements;
  tbState[6] = plotNormals;
  tbState[7] = flatShading;
  tbState[8] = improveTri;

  int numColumns=2;
  dialog.setToggleButtons(tbCommands, tbCommands, tbState, numColumns);

  const int numberOfTextStrings=8;
  aString textLabels[numberOfTextStrings], textStrings[numberOfTextStrings];

  int nt=0;
  textLabels[nt] = "debug"; 
  sPrintF(textStrings[nt], "%i", debug); nt++; 

  textLabels[nt] = "merge tolerance"; 
  sPrintF(textStrings[nt], "%g", mergeTolerance); nt++; 

  textLabels[nt] = "split tolerance factor"; 
  sPrintF(textStrings[nt], "%g", splitToleranceFactor); nt++; 

  textLabels[nt] = "deltaS"; 
  sPrintF(textStrings[nt], "%g", deltaS); nt++; 

//   textLabels[nt] = "curvatureTolerance"; 
//   sPrintF(textStrings[nt], "%g", curvatureTolerance); nt++; 

  textLabels[nt] = "minNumberOfPointsOnAnEdge";
  sPrintF(textStrings[nt], "%i", minNumberOfPointsOnAnEdge); nt++; 

//   textLabels[nt] = "curveResolutionTolerance"; 
//   sPrintF(textStrings[nt], "%g", curveResolutionTolerance); nt++; 

  textLabels[nt] = "maximum area"; 
  sPrintF(textStrings[nt], "%g", maximumArea); nt++; 

  textLabels[nt] = "max edge distance"; 
  sPrintF(textStrings[nt], "%g", maxDist); nt++; 

  // null strings terminal list
  assert( nt<numberOfTextStrings );
  textLabels[nt]="";   textStrings[nt]="";  

  // addPrefix(textLabels,prefix,cmd,maxCommands);
  dialog.setTextBoxes(textLabels, textLabels, textStrings);


  gi.pushGUI(gui);
//  gi.appendToTheDefaultPrompt("topology>"); // set the default prompt

  SelectionInfo select; select.nSelect=0;

  bool badElementsComputed=false;

  IntegerArray badElements;  // holds a list of bad elements on the triangulation.

  IntegerArray edgeCurveDisplayList;

  aString answer,line;
  int len=0;
  int e,s;
  bool rePlot;
  
  for( int it=0; ; it++ )
  {
    // pushbuttons
    dialog.setSensitive(true,DialogData::pushButtonWidget,"Build edge curves"); // always on?
    dialog.setSensitive(numberOfEdgeCurves>0,DialogData::pushButtonWidget,"Merge edge curves");
    dialog.setSensitive(mergedCurvesAreValid,DialogData::pushButtonWidget,"Triangulate");
    dialog.setSensitive(triangulationIsValid,DialogData::pushButtonWidget,"Flip normals");
    dialog.setSensitive(triangulationIsValid,DialogData::pushButtonWidget,"Examine triangulation");

    // toggle buttons
    dialog.setSensitive(triangulationIsValid,DialogData::toggleButtonWidget,"plot triangulated surface");
    dialog.setSensitive(triangulationIsValid,DialogData::toggleButtonWidget,"plot bad elements");
    dialog.setSensitive(triangulationIsValid,DialogData::toggleButtonWidget,"plot normals");
    dialog.setSensitive(triangulationIsValid,DialogData::toggleButtonWidget,"flat shading");
    dialog.setSensitive(mergedCurvesAreValid,DialogData::toggleButtonWidget,"improve triangulation");

    rePlot=true;
    
    if( it==0 )
    {
      answer="plot";
    }
    else
    {
      gi.savePickCommands(false); // temporarily turn off saving of pick commands.     

      gi.getAnswer(answer,"", select);
         
      gi.savePickCommands(true); // turn back on
    }
    
    
    if( answer=="exit" )
    {
      break;
    }
    else if( answer=="compute topology" )
    {
      if( triangulationIsValid )
      {
	gi.outputString("The global triangulation is valid. No need to recompute the topology");
	continue;
      }
      
      // build all the edge curves
      if( !edgeCurvesAreBuilt )
      {
        gi.outputString("Build edge curves...");
	buildEdgeCurves(  gi  );
        edgeCurvesAreBuilt=true;
	recomputeEdgeCurveBoundaryNodes=false;
	
        // Fill in the search tree with the bounding box for each edgeCurve
        buildEdgeCurveSearchTree(debug);
      }
      
      if( true || !mergedCurvesAreValid ) // always merge
      {
	// merge edge curves
        gi.outputString("Merge edge curves...");
	int rt = splitAndMergeEdgeCurves( gi,debug );

        if( rt!=0 ) // *wdh* 030825
	{
	  printf("ERROR return from splitAndMergeEdgeCurves\n");
	  gi.stopReadingCommandFile();
	  //*wdh* 030825  continue;
	}
        mergedCurvesAreValid=true;  
	
      }

// AP: Shouldn't this be done after merging? (probably doesn't matter)
      if( recomputeEdgeCurveBoundaryNodes )
      {
        gi.outputString("Determine new boundary nodes on edge curves...");
        buildEdgeCurveBoundaryNodes();
	recomputeEdgeCurveBoundaryNodes=false;
      }

      printEdgeCurveInfo(gi);
      
      // build triangulation
      gi.outputString("Build global triangulation...");
      triangulateCompositeSurface(debug, gi, params );

      if (!globalTriangulation)
      {
	gi.outputString("The global triangulation could not be formed due to previous errors");
	triangulationIsValid=false;
      }
      else
      {
	globalTriangulation->checkConnectivity(true,&badElements);
	badElementsComputed=true;

	if( badElements.getLength(0)==0 )
	{
	  gi.outputString("There were no bad elements");
	}
	else
	{
	  aString buf;
	  gi.outputString( sPrintF(buf,"There were %i bad elements in the triangulation\n",
				   badElements.getLength(0)) );
	}
	triangulationIsValid=true;
      }

      plotEdgeCurves=true;
      dialog.setToggleState("plot edge curves", plotEdgeCurves);

    }
    else if( answer=="save triangulation" )
    {
      if (triangulationIsValid)
      {
// build a little GUI to set the file format and name
	GUIState saveGUI;
	saveGUI.setWindowTitle("Saving a triangulation");
	saveGUI.setExitCommand("exit", "Close");
	aString rbCommand[] = {"file format ingrid", "file format cart3d", ""};
	aString rbLabel[] = {"InGrid", "Cart3D", "" };
	enum fileFormatEnum{
	  inGridFormat=0, cart3dFormat, numberOfFormats
	};
// initial choice: cart3d
	fileFormatEnum format=cart3dFormat;
	saveGUI.addRadioBox( "File format", rbCommand, rbLabel, format, 2); // 2 columns
	RadioBox &rBox = saveGUI.getRadioBox(0);
	aString pbLabels[] = {"Browse...", "Save", ""};
	aString pbCmds[] = {"browse", "save", ""};
	saveGUI.setPushButtons(pbCmds,  pbLabels, 1 ); // 1 row

	aString fname="triangulation.tri";
	aString textCommands[2], textLabels[2], textStrings[2];
	int nt=0;
	const int fnIndex=0;
	textCommands[nt] = "file name"; 
	textLabels[nt]   = "File name"; 
	sPrintF(textStrings[nt], "%s", SC fname); nt++; 
	// null strings terminal list
	textCommands[nt]=""; textLabels[nt]="";  textStrings[nt]="";
	saveGUI.setTextBoxes(textCommands, textLabels, textStrings);
// define pulldown menus
	aString pdCommand2[] = {"help format", ""};
	aString pdLabel2[] = {"Format", ""};
	saveGUI.addPulldownMenu("Help", pdCommand2, pdLabel2, GI_PUSHBUTTON);
	saveGUI.setLastPullDownIsHelp(true);
// done defining pulldown menus  

// open the gui
	gi.pushGUI(saveGUI);
	
	for (;;)
	{
	  gi.getAnswer(answer, "");
	  
	  if ( answer.matches("exit") )
	  {
	    break;
	  }
	  else if ( answer.matches("save") )
	  {
	    UnstructuredMapping *tri=getTriangulation();
	    if (format==inGridFormat)
	    {
	      tri->put(fname);
	    }
	    else if (format==cart3dFormat)
	    {
	      int nNode = tri->getNumberOfNodes();
	      int nElem = tri->getNumberOfElements();
	      const realArray & nodes = tri->getNodes();
	      const intArray & elements = tri->getElements();
	      FILE *outfile=fopen(SC fname,"w");
	      fprintf(outfile,"%i %i\n", nNode, nElem);
	      int i;
	      for (i=0; i<nNode; i++)
		fprintf(outfile,"%.12e %.12e %.12e\n", nodes(i,0), nodes(i,1), nodes(i,2));
	      printf("Using new element numbering...\n");
	      for (i=0; i<nElem; i++)
//		fprintf(outfile,"%i %i %i\n", elements(i,0), elements(i,1), elements(i,2));
		fprintf(outfile,"%i %i %i\n", elements(i,0)+1, elements(i,1)+1, elements(i,2)+1);
// all elements belong to the same configuration here
	      for (i=0; i<nElem; i++)
		fprintf(outfile,"1\n");
	      fclose(outfile);
	    }
	    break;
	  }
	  else if ( len=answer.matches("file format") )
	  {
	    aString fstr=answer(len+1,answer.length()-1);
	    if (fstr.matches("ingrid"))
	      format=inGridFormat;
	    else if (fstr.matches("cart3d"))
	      format=cart3dFormat;
	    else
	      gi.outputString(sPrintF(buf,"Unknown format `%s'",SC fstr));
	    if (!rBox.setCurrentChoice(format))
	    {
	      gi.outputString(sPrintF(buf, "ERROR: format %s (#%d) is inactive", 
				      SC rbLabel[format], format));
	    }
	  }
	  else if ( answer.matches("browse") )
	  {
	    gi.inputFileName(fname,"Enter the name of the file to save","tri");
            // set the text in the widget
	    saveGUI.setTextLabel(fnIndex, fname); // (re)set the textlabel
	  }
	  else if ( len=answer.matches("file name") )
	  {
// read the new file name off the end of the string...
	    aString newName = "";
	    if (answer.length() > len)
	      newName = answer(len+1,answer.length()-1);

	    if (newName != "" && newName != " ")
	    {
	      fname = newName;
	    }
	    else
	      gi.outputString("Invalid name");

	    saveGUI.setTextLabel(fnIndex, fname); // (re)set the textlabel
	  }
	  else if ( answer.matches("help format") )
	  {
	    if (format==inGridFormat)
	    {
	      gi.createMessageDialog("The InGrid format is\n"
                                     "DescriptiveString\n"
				     "1 numberOfNodes numberOfElements maxNumberOfNodesPerElement "
				     "domainDimension rangeDimension\n"
				     "1 x1 y1 z1  (or just x1 y1 in 2D \n"
				     "2 x2 y2 z2 \n"
				     " ...     \n"
				     "n xn yn zn \n"
				     "1 m1 n1 l1  (node numbers for element 1)\n"
				     "2 m2 n2 l2 \n"
				     "  ...    ", informationDialog);
	    }
	    else if (format==cart3dFormat)
	    {
	      gi.createMessageDialog("The Cart3D format is\n"
				     "numberOfNodes numberOfElements\n"
				     "x1 y1 z1\n"
				     "x2 y2 z2\n"
				     "...\n"
				     "xN yN zN (N=numberOfNodes)\n"
				     "node11 node12 node13\n"
				     "node21 node22 node23\n"
				     "...\n"
				     "nodeM1 nodeM2 nodeM3 (M=numberOfElements)\n", informationDialog);
	    }
	  }
	  
	  
	} // end inifite loop
	
	gi.popGUI();
      } // end "save triangulation"
      else
      {
	gi.createMessageDialog("You must compute a valid triangulation before you can save it!", errorDialog);
      }
      
      continue;
    }
    else if( answer=="build edge curves" )
    {

      // build all the edge curves
      buildEdgeCurves(  gi  );
      edgeCurvesAreBuilt=true;

      // Fill in the search tree with the bounding box for each edgeCurve
      buildEdgeCurveSearchTree(debug);
      
      triangulationIsValid=false;
      mergedCurvesAreValid=false;
      recomputeEdgeCurveBoundaryNodes=false;
      badElementsComputed=false;
      
      plotEdgeCurves=true;
      dialog.setToggleState("plot edge curves", plotEdgeCurves);

    }
    else if( answer=="merge edge curves" )
    {
      if( true || !mergedCurvesAreValid )  // don't check if valid for now.
      {
	if( searchTree==NULL )
	{
	  gi.outputString("You must build the edge curves first");
	  continue;
	}

	int rt = splitAndMergeEdgeCurves( gi,debug );
        if( rt!=0 ) // *wdh* 030825
	{
	  printf("ERROR return from splitAndMergeEdgeCurves\n");
	  gi.stopReadingCommandFile();
	  //*wdh* 030825 continue;
	}

	mergedCurvesAreValid=true;
        badElementsComputed=false;

        printEdgeCurveInfo(gi);
	triangulationIsValid=false;
      }
      else
      {
	gi.outputString("The edge curves have already been merged.");
      }
    }
    else if( answer=="triangulate" )
    {
//       if( triangulationIsValid )
//       {
// 	gi.outputString("Triangulation is computed and valid!");
// 	continue;
//       }
      if( searchTree==NULL )
      {
	gi.outputString("You must build the edge curves first");
	continue;
      }

// previous split/join operations might affect the node distribution along edges
      recomputeEdgeCurveBoundaryNodes = true;
      
      if( recomputeEdgeCurveBoundaryNodes )
      {
        printf("Determine new boundary nodes on edge curves...\n");
        buildEdgeCurveBoundaryNodes();
	recomputeEdgeCurveBoundaryNodes=false;
        printf("...done\n");

	triangulationIsValid=false;
      }

      triangulateCompositeSurface(debug, gi, params );

      if (!globalTriangulation)
      {
	gi.outputString("The global triangulation could not be formed due to previous errors");
	triangulationIsValid=false;
      }
      else
      {
	globalTriangulation->checkConnectivity(true,&badElements);
	badElementsComputed=true;
	if( badElements.getLength(0)==0 )
	{
	  gi.outputString("There were no bad elements");
	}
	else
	{
	  aString buf;
	  gi.outputString( sPrintF(buf,"There were %i bad elements in the triangulation\n",
				   badElements.getLength(0)) );
	}
	triangulationIsValid=true;
      }
    }
    else if( len=answer.matches("merge two edge curves") )
    {
      if( !faceInfoArray )
      {
	printF("CompositeTopology::update:ERROR: Cannot merge edges. There are no edges defined yet. "
                "You should build the errors first.\n");
	gi.stopReadingCommandFile();
        continue;
      }

      int eNumber1=-1,eNumber2=-1;
      sScanF(answer(len,answer.length()-1),"%i %i",&eNumber1, &eNumber2);
      EdgeInfo *e1 = edgeFromNumber(eNumber1);
      EdgeInfo *e2 = edgeFromNumber(eNumber2);
      if (e1 && e2)
      {
	if (mergeEdgeCurves(*e1, *e2) == 0)
	  gi.outputString(sPrintF(buf, "Edge curves %i and %i were merged", e1->edgeNumber, e2->edgeNumber));
	else
	  gi.outputString(sPrintF(buf, "Edge curves %i and %i could NOT be merged", e1->edgeNumber, e2->edgeNumber));
      }
      else
	gi.outputString(sPrintF(buf, "Could not find edges %i AND %i", eNumber1, eNumber2));
      mergedCurvesAreValid=true;
    }
    else if( len=answer.matches("un-merge edge curves") )
    {
      int eNumber=-1;
      sScanF(answer(len,answer.length()-1), "%i", &eNumber);       
      EdgeInfo *e = edgeFromNumber(eNumber);
      if( e )
      {
	unMergeEdge(*e);
	gi.outputString(sPrintF(buf,"Un-merging edge %i", e->edgeNumber));
      }
      else
      {
	gi.outputString(sPrintF(buf, "Invalid edge number: %i", eNumber));
      }

      triangulationIsValid=false;
      mergedCurvesAreValid=false;
    }
    else if( len=answer.matches("join edge curve") )
    {
      int eNumber=-1, toNext=0;
      sScanF(answer(len, answer.length()-1),"%i %i",&eNumber, &toNext);
      EdgeInfo *e = edgeFromNumber(eNumber);
      if (e)
      {
	if (e->status != EdgeInfo::edgeCurveIsBoundary)
	{
	  gi.outputString("Only boundary curves can be joined!");
	}
	else
	{
	  joinEdgeCurves(*e, toNext, debug);
	}
      }
      else
      {
	gi.outputString(sPrintF(buf, "Cannot find edge %i!", eNumber));
      }
    }
    else if( len=answer.matches("split an edge") )
    {
      int eNumber=-1;
      real rSplit=0.;
      sScanF(answer(len, answer.length()-1), "%i %e", &eNumber, &rSplit); 
      EdgeInfo *e=edgeFromNumber(eNumber);
      if (e)
      {
	if (e->status != EdgeInfo::edgeCurveIsBoundary || e->orientation != 1)
	  gi.outputString(sPrintF(buf, "cannot split an edge with status = %i orientation = %i",
				  e->status, e->orientation));
	else
	  splitEdge(&e, rSplit, debug, false); // don't merge automatically
      }
      else
      {
	gi.outputString(sPrintF(buf, "Cannot find edge %i!", eNumber));
      }
    }
    else if( len=answer.matches("edit edge curve") )
    {
// Anders' new version not implemented yet
    }
    else if( answer=="cancel merge" || answer=="cancel join" )
    {
      // these commands could be generated by getAnotherEdge, we can just ignore it
      continue;
    }
    else if( select.nSelect )  
    {
//      printf("**Selection \n");
      if( pickingOption==pickToQueryEdgeCurves )
      {
	EdgeInfo *e;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
		if(e->status != EdgeInfo::edgeCurveIsNotUsed && 
		   e->curve->getNURBS()->getGlobalID()==select.selection(i,0) )
		{
		  aString buf;
		  if (e->status == EdgeInfo::edgeCurveIsSlave)
		    buf = "SLAVE";
		  else if (e->status == EdgeInfo::edgeCurveIsMaster)
		    buf = "MASTER";
		  else if (e->status == EdgeInfo::edgeCurveIsBoundary)
		    buf = "BOUNDARY";
		  else
		    buf = "OTHER";
		  
		  aString obuf;
		  gi.outputString(sPrintF(obuf,
					  "Edge %i, %s, start point #%i, end point #%i, slave %i, master %i,\n"
					  "prev->edge %i, prev->startpoint #%i, prev->endpoint #%i,\n"
					  "next->edge %i, next->startPoint #%i, next->endPoint #%i,\n"
					  "curve usage=%i, loop %i (with %i edges) on face %i", 
					  e->edgeNumber, SC buf, e->getStartPoint(), e->getEndPoint(), 
					  (e->slave)? e->slave->edgeNumber: -1, (e->master)? e->master->edgeNumber: -1,
					  e->prev->edgeNumber, e->prev->getStartPoint(), e->prev->getEndPoint(),
					  e->next->edgeNumber, e->next->getStartPoint(), e->next->getEndPoint(),
					  e->curve->usage, e->loopNumber, currentLoop.numberOfEdges(), e->faceNumber));
		}
	      }
	    } // end for all loops
	  } // end for all subsurfaces
	} // end for all selections
	printf("\n");
	rePlot = false;
      }
      else if( pickingOption==pickToEditEdgeCurves )
      {
	EdgeInfo *e;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
		if(e->status != EdgeInfo::edgeCurveIsNotUsed && 
		   e->curve->getNURBS()->getGlobalID()==select.selection(i,0) )
		{
		  gi.outputToCommandFile(sPrintF(line,"edit edge curve %i\n",e->edgeNumber));
		  e->curve->getNURBS()->interactiveUpdate(gi);
		}
	      }
	    } // end for all loops
	  } // end for all subsurfaces
	} // end for all selections
      }
      else if( pickingOption==pickToMergeEdgeCurves )
      {
        int curveFound=-1;
        const int maxNumberChosen=10;
        EdgeInfo * edgeChosen[maxNumberChosen], *e;
        int numberOfEdgeCurvesChosen=0;
	for (int i=0; i<select.nSelect && numberOfEdgeCurvesChosen<maxNumberChosen; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
		if(e->status == EdgeInfo::edgeCurveIsBoundary &&
		   e->curve->getNURBS()->getGlobalID() == select.selection(i,0) )
		{
		  edgeChosen[numberOfEdgeCurvesChosen]=e;
		  numberOfEdgeCurvesChosen++;
		}
	      }
	    }
	  }
	}
	

        if( numberOfEdgeCurvesChosen==1 )
	{
	  printf("When merging two edge curves the first edge curve is chosen as the master.\n");

          EdgeInfo * eTwo=NULL;
          getAnotherEdge(eTwo, gi, "Choose another curve to merge to.", "cancel merge");
          if( eTwo )
	  {
	    edgeChosen[1]=eTwo;
	    numberOfEdgeCurvesChosen++;
	  }
	}

        if( numberOfEdgeCurvesChosen==2 )
	{
          EdgeInfo * e1=edgeChosen[0];
	  EdgeInfo * e2=edgeChosen[1];
	  
          printf("Attempt to merge edge curves %i and %i \n", e1->edgeNumber, e2->edgeNumber);
	  
          if( mergeEdgeCurves(*e1, *e2)==0 )
	  {
	    gi.outputString("Edge curves were merged");
	    mergedCurvesAreValid=true;
	    gi.outputToCommandFile(sPrintF(line,"merge two edge curves %i %i\n",e1->edgeNumber, e2->edgeNumber));
	  }
	  else
	  {
	    gi.outputString("Edge curves were not merged");
	  }
	  checkConsistency();
	}
        else if( numberOfEdgeCurvesChosen>2 )
	{
	  printf("More than 2 edge curves were chosen. Try again.\n");
	}
	else
	{
	  printf("You should choose exactly two edge curves\n");
	}
      }
      else if( pickingOption==pickToUnMergeEdgeCurves )
      {
// Anders' version
	EdgeInfo *e;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
// this should get all edges using this curve, whether they are master or slaves
		if((e->status == EdgeInfo::edgeCurveIsSlave || e->status == EdgeInfo::edgeCurveIsMaster) &&
		   e->curve->getNURBS()->getGlobalID() == select.selection(i,0) )
		{
		  unMergeEdge(*e);
		  gi.outputToCommandFile(sPrintF(line,"un-merge edge curves %i\n",e->edgeNumber));
		} // end if slave or master		
	      }
	    } // end for all loops
	  } // end for all subsurfaces
	} // end for all selections
	checkConsistency();
      }
      else if( pickingOption==pickToJoinEdgeCurves )
      {
	EdgeInfo *e;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
		if(e->status != EdgeInfo::edgeCurveIsNotUsed &&
		   e->curve->getNURBS()->getGlobalID() == select.selection(i,0) )
		{
		  if (e->status != EdgeInfo::edgeCurveIsBoundary)
		  {
		    gi.outputString("Only boundary curves can be joined!");
		  }
		  else
		  {
		    bool toEnd;
// project the picking coordinate onto the NURB to see which end it is closest to
		    realArray xp(1,3), rp(1,1);
		    int q;
		    for (q=0; q<3; q++)
		      xp(0,q) = select.x[q];
		    rp(0,0) = -1; // no guess
		    e->curve->getNURBS()->inverseMap(xp,rp);
		    printf("Parameter coordinate: %e\n", rp(0,0));
		    if (rp(0,0) > 0.5)
		      toEnd = true;
		    else
		      toEnd = false;
		    joinEdgeCurves(*e, toEnd, debug);
		    gi.outputToCommandFile(sPrintF(line,"join edge curve %i %i\n", e->edgeNumber, toEnd));
		  }
		}
	      } // end for sc...
	    }
	  }
	}
	checkConsistency();
      }
      else if( pickingOption==pickToSplitEdgeCurves )
      {
	EdgeInfo *e;
	for (int i=0; i<select.nSelect; i++)
	{
	  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
	    {
	      Loop & currentLoop = faceInfoArray[s].loop[l];
	      int sc;
	      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		   sc++, e=e->next )
	      {
		if(e->status != EdgeInfo::edgeCurveIsNotUsed &&
		   e->curve->getNURBS()->getGlobalID() == select.selection(i,0) )
		{
		  if (e->status != EdgeInfo::edgeCurveIsBoundary)
		  {
		    gi.outputString("Only boundary curves can be split!");
		  }
		  else
		  {
// project the picking coordinate onto the NURB to get the split point
		    realArray xp(1,3), rp(1,1);
		    int q;
		    for (q=0; q<3; q++)
		      xp(0,q) = select.x[q];
		    rp(0,0) = -1; // no guess
		    e->curve->getNURBS()->inverseMap(xp,rp);
		    printf("Split parameter coordinate: %e\n", rp(0,0));
		    int originalEdgeNumber = e->edgeNumber;
		    splitEdge(&e, rp(0,0), debug, false); // don't merge automatically
		    gi.outputToCommandFile(sPrintF( line, "split an edge %i %e\n", originalEdgeNumber, rp(0,0) ));
		    break; // don't split anything else in this loop
		  }
		  
		}
	      } // end for sc...
	    }
	  }
	}
	checkConsistency();
      }
      else if( pickingOption==pickToQueryElement )
      {
        // Print some info about the triangle that was picked
	if( globalTriangulation==NULL )
	{
	  printf("Unable to pick an element if there is no global triangulation\n");
          continue;
	}

        printf("Picked point coordinates: %e, %e, %e\n", select.x[0], select.x[1], select.x[2]);

	
	UnstructuredMapping & global = *globalTriangulation;
        const int numberOfElements= global.getNumberOfElements();
	
	const intArray & ef = global.getElementFaces();
	const intArray & element = global.getElements();
	const intArray & face = global.getFaces();
	const intArray & faceElements = global.getFaceElements();
	const intArray & tags = global.getTags();

	MappingProjectionParameters mpParams;
	intArray & subSurfaceIndex = mpParams.getIntArray(MappingProjectionParameters::subSurfaceIndex);
	intArray & elementIndex = mpParams.getIntArray(MappingProjectionParameters::elementIndex);

	realArray x(1,3), x0(1,3);
	Range Rx=rangeDimension;

	x(0,0)=select.x[0], x(0,1)=select.x[1], x(0,2)=select.x[2];
	    
	x0(0,Rx)=x(0,Rx);
	subSurfaceIndex=-1;
	elementIndex=-1;
	  
	global.project(x,mpParams);

	printf("Pt (%e,%e,%e) was projected to (%e,%e,%e) \n",x0(0,0),x0(0,1),x0(0,2), x(0,0),x(0,1),x(0,2));
//	int e=subSurfaceIndex(0);
	int e=elementIndex(0);
	if( e>=0 && e<numberOfElements )
	{
	  int f0=ef(e,0), f1=ef(e,1), f2=ef(e,2);
	  int ae0 = faceElements(f0,0)==e ? faceElements(f0,1) : faceElements(f0,0);
	  int ae1 = faceElements(f1,0)==e ? faceElements(f1,1) : faceElements(f1,0);
	  int ae2 = faceElements(f2,0)==e ? faceElements(f2,1) : faceElements(f2,0);
	  printf("Element e=%i, nodes=(%i,%i,%i), faces=(%i,%i,%i) adj elements=(%i,%i,%i) sub-surface=%i \n",
		 e,element(e,0),element(e,1),element(e,2),
		 f0,f1,f2,ae0,ae1,ae2,tags(e));
	  printf(" face f0=%i nodes=(%i,%i) e=(%i,%i), f1=%i nodes=(%i,%i) e=(%i,%i), f2=%i nodes=(%i,%i) e=(%i,%i)\n",
		 f0,face(f0,0),face(f0,1),faceElements(f0,0),faceElements(f0,1),
		 f1,face(f1,0),face(f1,1),faceElements(f1,0),faceElements(f1,1),
		 f2,face(f2,0),face(f2,1),faceElements(f2,0),faceElements(f2,1));
	}
	else
	{
	  printf("Invalid element=%i\n",e);
	}
      }
      
      
    }
    else if( answer.matches("merge edges") )
    {
      pickingOption=pickToMergeEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("un-merge edges") )
    {
      pickingOption=pickToUnMergeEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("join edges") )
    {
      pickingOption=pickToJoinEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("split edges") )
    {
      pickingOption=pickToSplitEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("query edges") )
    {
      pickingOption=pickToQueryEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("edit edges") )
    {
      pickingOption=pickToEditEdgeCurves;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("query element") )
    {
      pickingOption=pickToQueryElement;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( answer.matches("off") )
    {
      pickingOption=pickingOff;
      dialog.getRadioBox(0).setCurrentChoice(pickingOption);
      continue;
    }
    else if( len=answer.matches("plot reference surface") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotReferenceSurface=toggle;
      dialog.setToggleState("plot reference surface", toggle);
      printf(" plotReferenceSurface=%i\n",plotReferenceSurface);
      
    }
    else if( len=answer.matches("plot triangulated surface") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotTriangulatedSurface=toggle;
      dialog.setToggleState("plot triangulated surface", toggle);
      printf(" plotTriangulatedSurface=%i\n",plotTriangulatedSurface);
      
    }
    else if( len=answer.matches("plot edge curves") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotEdgeCurves=toggle;
      dialog.setToggleState("plot edge curves", toggle);
      printf(" plotEdgeCurves=%i\n",plotEdgeCurves);

      params.set(GI_PLOT_BLOCK_BOUNDARIES,plotEdgeCurves);   // this could be a separate option.
//      params.set(GI_PLOT_UNS_BOUNDARY_EDGES,plotEdgeCurves);
      
    }
    else if( len=answer.matches("plot green merged curves") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotMergedCurves=toggle;
      dialog.setToggleState("plot green merged curves", toggle);
    }
    else if( len=answer.matches("plot blue unmerged curves") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotUnMergedCurves=toggle;
      dialog.setToggleState("plot blue unmerged curves", toggle);
    }
    else if( len=answer.matches("plot blue/red unmerged curves") ) // for backward compatibility
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotUnMergedCurves=toggle;
      dialog.setToggleState("plot blue unmerged curves", toggle);
    }
    else if( len=answer.matches("plot bad elements") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotBadElements=toggle;
      dialog.setToggleState("plot bad elements", toggle);
    }
    else if( len=answer.matches("plot normals") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      plotNormals=toggle;
      params.set(GI_PLOT_MAPPING_NORMALS, plotNormals);
      dialog.setToggleState("plot normals", toggle);
    }
    else if( len=answer.matches("flat shading") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      flatShading=toggle;
      params.set(GI_UNS_FLAT_SHADING, flatShading);
      dialog.setToggleState("flat shading", toggle);
// need to erase the faces in the triangulation      
      if (globalTriangulation) globalTriangulation->eraseUnstructuredMapping(gi);
    }
    else if( len=answer.matches("improve triangulation") )
    {
      int toggle;
      sScanF(answer(len,answer.length()-1),"%i",&toggle);       
      improveTri=toggle;
      dialog.setToggleState("improve triangulation", toggle);
      rePlot = false;
    }
    else if( len=answer.matches("debug") )
    {
      sScanF(answer(len,answer.length()),"%i",&debug);
      dialog.setTextLabel("debug",sPrintF(answer,"%i",debug));
      printf("debug=%i\n",debug);
      rePlot = false;
    }
    else if( len=answer.matches("merge tolerance") )
    {
      real oldMergeTolerance=mergeTolerance;
      sScanF(answer(len,answer.length()),"%e",&mergeTolerance);
      dialog.setTextLabel("merge tolerance",sPrintF(answer,"%g",mergeTolerance));
      printf("mergeTolerance=%g\n",mergeTolerance);

      if( mergeTolerance<oldMergeTolerance )
      {
	gi.outputString("You must build the edge curves again if you wish to use a smaller merge tolerance");
      }
      else
      {
	gi.outputString("Choose `merge edge curves' to merge any un-merged edges");
      }

      mergedCurvesAreValid=false;
      rePlot = false;
    }
    else if( len=answer.matches("split tolerance factor") )
    {
      real newSplitToleranceFactor=0;
      sScanF(answer(len,answer.length()),"%e",&newSplitToleranceFactor);
      if (newSplitToleranceFactor >= 1.)
      {
	splitToleranceFactor = newSplitToleranceFactor;
	printf("splitToleranceFactor=%g\n", splitToleranceFactor);
      }
      else
      {
	gi.createMessageDialog("splitToleranceFactor >= 1", errorDialog);
      }
      dialog.setTextLabel("split tolerance factor",sPrintF(answer,"%g", splitToleranceFactor));

      rePlot = false;
    }
    else if( len=answer.matches("deltaS") )
    {
      real oldDeltaS=deltaS;
      sScanF(answer(len,answer.length()),"%e",&deltaS);
      dialog.setTextLabel("deltaS",sPrintF(answer,"%g",deltaS));
      printf("deltaS=%g\n",deltaS);

      if(faceInfoArray && oldDeltaS!=deltaS )
      {
        triangulationIsValid=false;
        recomputeEdgeCurveBoundaryNodes=true;

        // recompute nodes on boundary curves so they will be plotted.
        gi.outputString("Determine new boundary nodes on edge curves...");
        buildEdgeCurveBoundaryNodes();
	recomputeEdgeCurveBoundaryNodes=false;
      }
      rePlot = false;
    }
//      else if( len=answer.matches("curvatureTolerance") )
//      {
//        sScanF(answer(len,answer.length()),"%e",&curvatureTolerance);
//        dialog.setTextLabel("curvatureTolerance",sPrintF(answer,"%g",curvatureTolerance));
//        printf("curvatureTolerance=%g\n",curvatureTolerance);
//  //      dialog.setSensitive(true,DialogData::pushButtonWidget,buildEdgeCurvesPushButton);

//      }
    else if( len=answer.matches("minNumberOfPointsOnAnEdge") )
    {
      int newNumber=0;
      if (sScanF(answer(len,answer.length()),"%i",&newNumber) == 1)
      {
	int howBraveAreYouGrasshopper = 3; // kkc 030122
	if (newNumber >= howBraveAreYouGrasshopper && newNumber != minNumberOfPointsOnAnEdge)
	{
	  minNumberOfPointsOnAnEdge=newNumber;

	  triangulationIsValid=false;
	  recomputeEdgeCurveBoundaryNodes=true;

	    // recompute nodes on boundary curves so they will be plotted.
	  gi.outputString("Determine new boundary nodes on edge curves...");
	  buildEdgeCurveBoundaryNodes();
	  recomputeEdgeCurveBoundaryNodes=false;

	  printf("minNumberOfPointsOnAnEdge=%i\n",minNumberOfPointsOnAnEdge);
	}
	else if (newNumber < howBraveAreYouGrasshopper)
	{
	  aString msg;
	  sPrintF(msg,"minNumberOfPointsOnAnEdge >= %i",howBraveAreYouGrasshopper);
	  gi.createMessageDialog(msg, errorDialog);
	}
	dialog.setTextLabel("minNumberOfPointsOnAnEdge",sPrintF(answer,"%i",minNumberOfPointsOnAnEdge));
      }
      rePlot = false;
    }
//      else if( len=answer.matches("curveResolutionTolerance") )
//      {
//        sScanF(answer(len,answer.length()),"%e",&curveResolutionTolerance);
//        dialog.setTextLabel("curveResolutionTolerance",sPrintF(answer,"%g",curveResolutionTolerance));
//        printf("curveResolutionTolerance=%g\n",curveResolutionTolerance);

//  //      dialog.setSensitive(true,DialogData::pushButtonWidget,buildEdgeCurvesPushButton);
//      }
    else if( len=answer.matches("maximum area") )
    {
      sScanF(answer(len,answer.length()),"%e",&maximumArea);
      dialog.setTextLabel("maximum area",sPrintF(answer,"%g",maximumArea));
      printf("maximumArea=%g\n",maximumArea);

      triangulationIsValid=false;
      
    //      dialog.setSensitive(false,DialogData::pushButtonWidget,mergeEdgeCurvesPushButton);
    }
    else if( len=answer.matches("max edge distance") )
    {
      real oldMaxDist=maxDist;
      sScanF(answer(len,answer.length()),"%e",&maxDist);
      if (maxDist <= 0.)
      {
	maxDist = oldMaxDist;
	gi.createMessageDialog("The max distance between the midpoint of the edges and\n"
			       "the surface must be positive\n", errorDialog);
      }
      dialog.setTextLabel("max edge distance",sPrintF(answer,"%g",maxDist));
	
      printf("max edge distance=%g\n",maxDist);

      if( maxDist<oldMaxDist )
      {
	gi.outputString("You must re-build the triangulation if you wish to use a smaller edge distance");
        // dialog.setSensitive(true,DialogData::pushButtonWidget,buildEdgeCurvesPushButton);
      }
      rePlot = false;
    }
    else if( answer=="flip normals" )
    {
      gi.outputString("Flipping normals");
      int numberOfElements = globalTriangulation->getNumberOfElements();
      intArray & elements = (intArray &) globalTriangulation->getElements();
      intArray & elementFaces  = (intArray &) globalTriangulation->getElementFaces();
      for( int e=0; e<numberOfElements; e++ )
      {
	int temp=elements(e,1);
	elements(e,1)=elements(e,2);
	elements(e,2)=temp;
	temp=elementFaces(e,0);
	elementFaces(e,0)=elementFaces(e,2);
	elementFaces(e,2)=temp;
      }
      globalTriangulation->eraseUnstructuredMapping(gi);
      globalTriangulation->checkConnectivity();
      globalTriangulation->specifyEntity(UnstructuredMapping::Face, elements); // to get the new connectivity working
      rePlot = true;
    }
    else if( answer=="examine reference surface" )
    {
      gi.erase();
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&gi;
      mapInfo.gp_ = &refSurPar;
      cs.update(mapInfo);

    }
    else if( answer=="examine triangulation" )
    {
      MappingInformation mapInfo;
      mapInfo.graphXInterface=&gi;
      if( globalTriangulation!=NULL )
      {
	gi.erase();
	globalTriangulation->update(mapInfo);
      }
      else
      {
	for( s=0; s<cs.numberOfSubSurfaces(); s++ )
	{
	  if( triangulationSurface[s]!=NULL )
	  {
	    gi.erase();
	    triangulationSurface[s]->update(mapInfo);
	  }
	}
      }
      
    }
    else if( answer=="plot sub-surface triangulations" )
    {
      gi.erase();
      for( s=0; s<cs.numberOfSubSurfaces(); s++ )
      {
	if( triangulationSurface[s]!=NULL )
	{
	  PlotIt::plot(gi, *triangulationSurface[s],params);
	}
    
      }
      gi.pause();
    }
    else if( answer=="plot" )
    {
      rePlot = true;
    }
    else if( answer == "help topology" )
    {
      gi.createMessageDialog(
	"   Build the topology (connectivity) of a CompositeSurface\n"
	"   -------------------------------------------------------\n"
	"The object is to merge edge curves and build a global triangulation.\n"
	"\t build edge curve: \tBuild a 3D representation of all trimming curve or part of a trimming curve\n"
	"\t merge edge curves: \tMatch edge curves with matching edges on adjacent surfaces\n"
	"User parameters:\n"
	"\t merge tolerance: expected tolerance in matching adjacent surfaces\n"
	"\t deltaS \t\t\t: spacing between points on edges of the global triangulation\n", informationDialog);
      rePlot = false;
    }
    else if (select.active && select.nSelect==0)
    {
      gi.outputString("Sorry, no object was selected");
      rePlot = false;
    }
    else
    {
      printf("Unknown response: [%s] \n",(const char*)answer);
      gi.stopReadingCommandFile();
    }

    if( rePlot && gi.isGraphicsWindowOpen() )
    {
      
      const RealArray & xBound = gi.getGlobalBound();

      gi.erase();
      if( plotReferenceSurface )
	PlotIt::plot(gi,cs, refSurPar); // **** plot the surface ****

      // use at least the current bounds for plotting.
      gi.setGlobalBound(xBound);   


      //    params.set(GI_SURFACE_OFFSET,(real)20.);  // offset the surfaces so we see the edges better


      if( plotEdgeCurves && faceInfoArray) // can't plot them unless they are there!
      {
	real oldCurveLineWidth;
	params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	params.set(GraphicsParameters::curveLineWidth,(real)2.);

	params.set(GI_POINT_COLOUR,"black");
	params.set(GI_PLOT_GRID_POINTS_ON_CURVES,false);
	params.set(GI_PLOT_END_POINTS_ON_CURVES,true); // mark end points of edge curves.

	for(s=0; s<cs.numberOfSubSurfaces(); s++ )
	{
	  EdgeInfo *currentEdge;
	  
	  int i, sc;
	  for (i=0; i<faceInfoArray[s].numberOfLoops; i++)
	  {
	    Loop & currentLoop = faceInfoArray[s].loop[i];
	    
	    for (sc = 0, currentEdge = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
		 sc++, currentEdge=currentEdge->next )
	    {
// is there already a list?
	      if (currentEdge->dList > 0 && glIsList(currentEdge->dList))
	      {
// plot boundary edges if plotUnMergedCurves==true and plot merged edges (manifold & non-manifold) if
// plotMergedCurves == true
		if( (currentEdge->curve->isBoundary() && plotUnMergedCurves) ||
		    ((currentEdge->curve->isManifold() || currentEdge->curve->isNonManifold()) && plotMergedCurves))
		  gi.setPlotDL(currentEdge->dList, true);
		else
// all other curves are turned off
		  gi.setPlotDL(currentEdge->dList, false);
	      }
	      else if( (currentEdge->curve->isBoundary() && plotUnMergedCurves) ||
		       ((currentEdge->curve->isManifold() || 
			 currentEdge->curve->isNonManifold()) && plotMergedCurves))
	      { // get a new list which is unlit, plotted, hideable and interactive
		currentEdge->dList = gi.generateNewDisplayList(false, true, true, true);  
		glNewList(currentEdge->dList, GL_COMPILE);
// colour coding of curve segments
		if (currentEdge->curve->isBoundary())
		  params.set(GI_MAPPING_COLOUR,"blue"); // one surface
		else if (currentEdge->curve->isManifold())
		  params.set(GI_MAPPING_COLOUR,"green"); // two surfaces meet
		else if (currentEdge->curve->isNonManifold())
		  params.set(GI_MAPPING_COLOUR,"red"); // > two surfaces meet
		else // should never be used
		  params.set(GI_MAPPING_COLOUR,"brown"); // removed or undefined 

		PlotIt::plot(gi,*(currentEdge->curve->getNURBS()), params, currentEdge->dList, false);
		glEndList();
	      }
	    } // end for sc...
	    
	  } // end for all surfaces
	} // end for s...
	params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
      } // end if plotEdgeCurves
	
    
      if( plotTriangulatedSurface && triangulationIsValid )
      {
	params.set(GI_MAPPING_COLOUR,"LIGHTSTEELBLUE"); // "green"); 

	if( globalTriangulation!=NULL && globalTriangulation->getNumberOfNodes()>0 )
	{
	  globalTriangulation->setColour("LIGHTSTEELBLUE"); 
	  PlotIt::plot( gi, *globalTriangulation, params );
	}
	else
	{
	  for( s=0; s<cs.numberOfSubSurfaces(); s++ )
	  {
	    if( triangulationSurface[s]!=NULL )
	    {
	      PlotIt::plot(gi, *triangulationSurface[s], params);
	    }
    
	  }
	}
      }
      if( plotBadElements && badElementsComputed && globalTriangulation!=NULL )
      {
	UnstructuredMapping & global = *globalTriangulation;

	int numberOfBadElements=badElements.getLength(0);
	printf("plot %i bad elements\n",numberOfBadElements);
      
	if( numberOfBadElements>0 )
	{
	  int numberOfFaces=numberOfBadElements*3;
      
	  const int numberOfElements = global.getNumberOfElements();
	  const int numberOfNodes = global.getNumberOfNodes();
      
	  const realArray & node = global.getNodes();
	  const intArray & element = global.getElements();

	  realArray line(numberOfFaces,3,2);
	  int j=0;
	  for( int i=0; i<numberOfBadElements; i++ )
	  {
	    int e=badElements(i);
	    assert( e>=0 && e<numberOfElements );
	
	    int n0=element(e,0), n1=element(e,1), n2=element(e,2);
	
	    for( int n=0; n<3; n++ )
	    {
	      int n0=element(e,n);
	      int n1=element(e,(n+1)%3);
	  
	      line(j,0,0)=node(n0,0);
	      line(j,1,0)=node(n0,1);
	      line(j,2,0)=node(n0,2);
	      line(j,0,1)=node(n1,0);
	      line(j,1,1)=node(n1,1);
	      line(j,2,1)=node(n1,2);
	      j++;
	  
	    }
	  }

	  // add line colour, line width
	  params.set(GI_LINE_COLOUR,"red");
	  real oldCurveLineWidth;
	  params.get(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	  params.set(GraphicsParameters::curveLineWidth,4.);

	  gi.plotLines(line,params);

	  params.set(GraphicsParameters::curveLineWidth,oldCurveLineWidth);
	}
      
      }
    } // end if rePlot
  
  }
  
  


  // delete edge curve display lists
  int oldNumber=edgeCurveDisplayList.getLength(0);
  for( e=0; e<oldNumber; e++ )
  {
    if( edgeCurveDisplayList(e)>0 )
      gi.deleteList(edgeCurveDisplayList(e));
  }

  gi.popGUI();
//  gi.unAppendTheDefaultPrompt();


  return 0;
}

EdgeInfo * CompositeTopology::
edgeFromNumber(int n)
{
  if( !faceInfoArray )
  {
    printF("CompositeTopology::edgeFromNumber:ERROR: There are no edges defined yet. "
	   "You should build the errors first\n");
    OV_ABORT("error");
  }

  EdgeInfo *e=NULL;
  for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
  {
    for (int l=0; l<faceInfoArray[s].numberOfLoops; l++)
    {
      Loop & currentLoop = faceInfoArray[s].loop[l];
      int sc;
      for (sc = 0, e = currentLoop.firstEdge; sc<currentLoop.numberOfEdges(); 
	   sc++, e=e->next )
      {
	if(e->edgeNumber == n)
	  return e;
      }
    }
  }
  return NULL;
}

void CompositeTopology::
unMergeEdge(EdgeInfo & e)
{
//  printf("Un-merging edge %i\n", e->edgeNumber);
  e.status = EdgeInfo::edgeCurveIsBoundary;
  e.eraseEdge();
		  
  if (e.slave)
  {
    e.slave->master = NULL;
    e.slave = NULL;
  }
  if (e.master)
  {
    e.master->slave = NULL;
    e.master = NULL;
  }
		  
  e.curve->usage--;
  e.initialCurve->usage++;
  e.curve = e.initialCurve;
  e.orientation = 1; // all initial curves have positive orientation
// the node numbering needs to be fixed too, on the previous and next edge, to ensure a closed loop
// Only change the prev and next segments if they are boundaryEdges, otherwise modify this segment
  if (e.prev->status == EdgeInfo::edgeCurveIsBoundary)
  {
    e.prev->setEndPoint( e.getStartPoint(), endPoint, mergeTolerance, e.edgeNumber, masterEdge, unusedEdges );
  }
  else
  {
    e.setStartPoint( e.prev->getEndPoint(), endPoint, mergeTolerance, e.edgeNumber, masterEdge, unusedEdges );
  }
  if (e.next->status == EdgeInfo::edgeCurveIsBoundary)
  {
    e.next->setStartPoint( e.getEndPoint(), endPoint, mergeTolerance, e.edgeNumber, masterEdge, unusedEdges );
  }
  else
  {
    e.setEndPoint( e.next->getStartPoint(), endPoint, mergeTolerance, e.edgeNumber, masterEdge, unusedEdges );
  }
} // end unMergeEdge

// kkc
void 
CompositeTopology::
setMaximumArea(real maxa)
{
  if ( maxa>0 ) 
    maximumArea = maxa;
  else
    cout<<"ERROR : CompositeToplogy : maximumArea must be greater than 0 !"<<endl;
}

//kkc
real 
CompositeTopology::
getMaximumArea() const
{
  return maximumArea;
}

void 
CompositeTopology::
setDeltaS(real ds)
{
  if ( ds>0 ) 
    {
      deltaS = ds;
      recomputeEdgeCurveBoundaryNodes=true;
    }
  else
    cout<<"ERROR : CompositeToplogy : deltaS must be greater than 0 !"<<endl;

}

real 
CompositeTopology::
getDeltaS() const
{
  return deltaS;
}

void
CompositeTopology::
invalidateTopology()
{
  edgeCurvesAreBuilt = mergedCurvesAreValid = false;
  recomputeEdgeCurveBoundaryNodes = true;
}

bool 
CompositeTopology::
computeTopology(GenericGraphicsInterface & gi, int debug)
{
  bool isOK = true;

  // reset the triangulation
  triangulationIsValid = false;

  // build all the edge curves
  if( !edgeCurvesAreBuilt )
    {
      gi.outputString("Build edge curves...");
      buildEdgeCurves(  gi  );
      edgeCurvesAreBuilt=true;
      recomputeEdgeCurveBoundaryNodes=false;
      
      // Fill in the search tree with the bounding box for each edgeCurve
      buildEdgeCurveSearchTree(debug);
    }
  
  if( true || !mergedCurvesAreValid ) // always merge
    {
      // merge edge curves
      gi.outputString("Merge edge curves...");
      int rt = splitAndMergeEdgeCurves( gi,debug );
      if( rt!=0 ) // *wdh* 030825
	{
	  printf("ERROR return from splitAndMergeEdgeCurves\n");
	  gi.stopReadingCommandFile();
	  return false;
	}
      
      mergedCurvesAreValid=true;
    }
  
  // AP: Shouldn't this be done after merging? (probably doesn't matter)
  if( recomputeEdgeCurveBoundaryNodes )
    {
      gi.outputString("Determine new boundary nodes on edge curves...");
      buildEdgeCurveBoundaryNodes();
      recomputeEdgeCurveBoundaryNodes=false;
    }
  
  printEdgeCurveInfo(gi);
  
  // build triangulation
  gi.outputString("Build global triangulation...");
  GraphicsParameters params;
  triangulateCompositeSurface(debug, gi, params );
  
  IntegerArray badElements;
  if (!globalTriangulation)
    {
      gi.outputString("The global triangulation could not be formed due to previous errors");
      triangulationIsValid=false;
    }
  else
    {
      globalTriangulation->checkConnectivity(true,&badElements);
      
      if( badElements.getLength(0)==0 )
	{
	  gi.outputString("There were no bad elements");
	}
	else
	  {
	    aString buf;
	    gi.outputString( sPrintF(buf,"There were %i bad elements in the triangulation\n",
				     badElements.getLength(0)) );
	  }
      triangulationIsValid=true;
    }

  return isOK && globalTriangulation && (badElements.getLength(0)==0);
  
}
