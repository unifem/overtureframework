//#define BOUNDS_CHECK
#include "Mapping.h"

#include "uns_templates.h"
#include "entityConnectivityBuilder.h"

#include "display.h"

#if 1
#define ENTIDX(ep,e,el,ec,ecl) ep[ (el)*(ec) + e ]
#else
#define ENTIDX(ep,e,el,ec,ecl) ep[ (e)*(ecl) + ec ]
#endif

const int topo2FaceNVerts[][6] = 
  { { 3 },
    { 4 },
    { 3, 3, 3, 3 },
    { 4, 3, 3, 3, 3 },
    { 4, 4, 4, 3, 3 },
    
    { 0, 0, 0, 0, 0, 0 }, // XXX septahedron
    
    { 4, 4, 4, 4, 4, 4 } 
  };

const int topo2FaceVert[][6][4] = 
  { { {0,1,2} }, // triangle
    
    { {0,1,2,3} }, // quadrilateral
    
    { {0,1,3}, {0,2,1}, {0,3,2}, {1,2,3} }, // tetrahedron
    
    { {3,2,1,0}, {0,4,3}, {0,1,4}, {1,2,4}, {2,3,4} }, // pyramid

    { {0,1,4,3}, {0,3,5,2}, {1,2,5,4}, {2,1,0}, {3,4,5} }, // triPrism

    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1} }, // XXX septahedron

    { {0,1,5,4}, {2,3,7,6}, {4,5,6,7}, {3,2,1,0}, {0,4,7,3}, {1,2,6,5} } // hexahedron
  };

const int topo2EdgeVert[][12][2] = 
  { 
    { {0,1}, {1,2}, {2,0} }, // triangle

    { {0,1}, {1,2}, {2,3}, {3,0} }, // quadrilateral

    { {0,3}, {1,3}, {2,3}, {0,2}, {0,1}, {1,2} }, // tetrahedron

    { {0,4}, {1,4}, {2,4}, {3,4}, {0,1}, {1,2}, {2,3}, {0,3} }, // pyramid

    { {0,2}, {3,5}, {4,5}, {1,2}, {0,1}, {0,3}, {3,4}, {1,4}, {2,5} }, // triPrism

    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1} }, // XXX septahedron

    { {0,4}, {1,5}, {2,6}, {3,7}, {0,1}, {1,2}, {2,3}, {0,3}, {4,5}, {5,6}, {6,7}, {4,7} }  // hexahedron
  };

const int topo2FaceEdge[][6][4] =
  {
    { {0,1,2} },

    { {0,1,2,3} },

    { {4,1,0}, {3,5,4}, {0,2,3}, {5,2,1} }, // tetrahedron

    { {6,5,4,7}, {0,3,7}, {4,1,0}, {5,2,1}, {6,3,2} }, // pyramid

    { {4,7,6,5}, {5,1,8,0}, {3,8,2,7}, {3,4,0}, {6,2,1} }, // triPrism

    { {-1}, {-1}, {-1}, {-1}, {-1}, {-1} }, // XXX septahedron

    { {4,1,8,0}, {6,3,10,2}, {8,9,10,11}, {6,5,4,7}, {0,11,3,7}, {5,2,9,1} } // hexahedron
  };

const int topoNVerts[] =
  { 3, // triangle
    4, // quadrilateral
    4, // tetrahedron
    5, // pyramid
    6, // triPrism
    7, // septahedron
    8  // hexahedron
  };

const int topoNEdges[] = 
  { 3, // triangle
    4, // quadrilateral
    6, // tetrahedron
    8, // pyramid
    9, // triPrism
    11, // septahedron
    12  // hexahedron
  };

const int topoNFaces[] = 
  { 1, // triangle
    1, // quadrilateral
    4, // tetrahedron
    5, // pyramid
    5, // triPrism
    6, // septahedron
    6  // hexahedron
  };

namespace {

  inline int entitytopo(int *&rp, const int &e, const int &maxV, const int &nr, const int &dDim)
  {
    if ( ENTIDX(rp,e,maxV-1,nr,maxV) != -1 && dDim==2 ) 
      return 1;
    else if ( ENTIDX(rp,e,maxV-2,nr,maxV) !=-1 && dDim==2 )
      return 0;
    else if ( ENTIDX(rp,e,maxV-1,nr,maxV) !=-1 )
      return 6;
    else if ( ENTIDX(rp,e,maxV-2,nr,maxV)!=-1 )
      return 5;
    else if ( ENTIDX(rp,e,maxV-3,nr,maxV)!=-1 )
      return 4;
    else if ( ENTIDX(rp,e,maxV-4,nr,maxV)!=-1 ) 
      return 3;
    else if ( ENTIDX(rp,e,maxV-5,nr,maxV)!=-1 )
      return 2;
    else
      return -1;
  }

  inline bool facesAreEquivalent(int *&rp, const int &t, const int &r, const int &rf, 
				 const int &t2, const int &r2, int const &rf2, const int &maxV, const int &nr)
  {
    if ( topo2FaceNVerts[t][rf]!=topo2FaceNVerts[t2][rf2] ) return false;

    int vmin, vmin2;
    int vidx, vidx2;

    vmin = vmin2 = INT_MAX;
    vidx = vidx2 = -1;

    int nv=topo2FaceNVerts[t][rf];

    for ( int v=0; v<nv; v++ )
      {
	if ( ENTIDX(rp,r,topo2FaceVert[t][rf][v],nr,maxV)<vmin )
	  {
	    vmin = ENTIDX(rp,r,topo2FaceVert[t][rf][v],nr,maxV);
	    vidx = v;
	  }

	if ( ENTIDX(rp,r2,topo2FaceVert[t2][rf2][v],nr,maxV)<vmin2 )
	  {
	    vmin2 = ENTIDX(rp,r2,topo2FaceVert[t2][rf2][v],nr,maxV);
	    vidx2 = v;
	  }
      }

    if ( vmin!=vmin2 ) return false;

    bool still_same = true;
    
    for ( int v=0; v<nv && still_same; v++ )
      still_same = (ENTIDX(rp,r,topo2FaceVert[t][rf][(vidx+v)%nv],nr,maxV) == ENTIDX(rp,r2,topo2FaceVert[t2][rf2][(vidx2+v)%nv],nr,maxV));
    
    
    if ( !still_same ) // check reverse
      for ( int v=0; v<nv; v++ )
	if ( ENTIDX(rp,r,topo2FaceVert[t][rf][(vidx+v)%nv],nr,maxV) != ENTIDX(rp,r2,topo2FaceVert[t2][rf2][(vidx2+nv-v)%nv],nr,maxV) )
	  return false;

    return true;
  }

}

int constructEdgeEntityFromEntity(intArray &edges, intArray &downward, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient, 
				  intArray &regions, int nReg,
				  int maxVerts, int maxVertIDX, int dDim)
{
  int nedges = 0;

  int nR = regions.getLength(0);
  int *rp = regions.getDataPointer();

  int *offset = new int[maxVertIDX+2];
  
  for ( int v=0; v<=maxVertIDX+1; v++ )
    offset[v] = 0;

  int ne=0;
  for ( int r=0; r<nReg; r++ )
    {
      int type = entitytopo( rp, r, maxVerts, nR, dDim);
      
      ne+=topoNEdges[type];
      for ( int e=0; e<topoNEdges[type]; e++ )
	{
	  int p1 = ENTIDX(rp,r,topo2EdgeVert[type][e][0],nR,maxVerts);
	  int p2 = ENTIDX(rp,r,topo2EdgeVert[type][e][1],nR,maxVerts);
	  assert(p1<=maxVertIDX);
	  assert(p2<=maxVertIDX);

	  if ( p1<p2 )
	    offset[p1+1]++;
	  else
	    offset[p2+1]++;
	}
    }

  for ( int v=1; v<=maxVertIDX+1; v++ )
    offset[v] = offset[v-1]+offset[v];

  struct e_info { int p2; int ecount; bool paired; int r; int el; };

  e_info * einfo = new e_info[ne+1];
  e_info null_e = { -1, 0, false, -1, -1 };
  for ( int e=0; e<ne+1; e++ )
    einfo[e] = null_e;

  if ( dDim==2 )
    downward.redim(nR,4);
  else
    downward.redim(nR,12);
  downward = -1;

  if ( dOrient ) delete [] dOrient;
  dOrient = new char[downward.getLength(0)*downward.getLength(1)];
  for ( int d=0; d<downward.getLength(0)*downward.getLength(1); d++ )
    dOrient[d] = 0x1;

  for ( int r=0; r<nReg; r++ )
    {
      int type = entitytopo( rp, r, maxVerts, nR, dDim);

      for ( int e=0; e<topoNEdges[type]; e++ )
	{
	  int p1 = ENTIDX(rp,r,topo2EdgeVert[type][e][0],nR,maxVerts);
	  int p2 = ENTIDX(rp,r,topo2EdgeVert[type][e][1],nR,maxVerts);

	  if ( p1<p2 )
	    {
	      einfo[offset[p1] + einfo[offset[p1]].ecount].p2 = p2;
	      einfo[offset[p1] + einfo[offset[p1]].ecount].r = r;
	      einfo[offset[p1] + einfo[offset[p1]].ecount].el = e;
	      einfo[offset[p1]].ecount++;
	    }
	  else if ( p2<p1 )
	    {
	      einfo[offset[p2] + einfo[offset[p2]].ecount].p2 = p1;
	      einfo[offset[p2] + einfo[offset[p2]].ecount].r = r;
	      einfo[offset[p2] + einfo[offset[p2]].ecount].el = e;
	      einfo[offset[p2]].ecount++;
	    }
	  else
	  {
	    // abort(); // *wdh* 090622
            printF("\n ********************************************************************************\n"
                   "        entityConnectivityBuilder::constructEdgeEntityFromEntity:ERROR: \n"
                   "  The end points of an edge are the SAME: region=%i, type=%i, edge=%i, ends: p1=%i, p2=%i.\n"
                   "  total number-of-regions=%i, numberOfEdges for this type=%i.\n"
                   "  Something is WRONG with the triangulation. Will continue anyway...\n"
                   " ********************************************************************************\n",
                     r,type,e,p1,p2,nReg,topoNEdges[type]);
	  }
	  
	}
    }
  
  edges.redim(ne,2);
  edges = -1;

  upwardOffset.redim(maxVertIDX+2);
  upwardOffset = 0;
  for ( int v=0; v<maxVertIDX; v++ )
    {
      for ( int e=offset[v]; e<offset[v+1]; e++ )
	{
	  if ( !einfo[e].paired )
	    {
	      int p2 = einfo[e].p2;
	      edges(nedges,0) = v;
	      edges(nedges,1) = p2;

	      assert(v<p2);

	      einfo[e].paired = true;
	      downward(einfo[e].r,einfo[e].el) = nedges;  
	      upwardOffset(v+1)++;
	      upwardOffset(p2+1)++;

	      int type = entitytopo( rp, einfo[e].r, maxVerts, nR, dDim);
	      if ( ENTIDX(rp,einfo[e].r,topo2EdgeVert[type][einfo[e].el][0],nR,maxVerts)!=v)
		dOrient[einfo[e].r + einfo[e].el*downward.getLength(0)] = 0x0;
		   
	      for ( int ef=e+1; ef<offset[v+1]; ef++ )
		if ( !einfo[ef].paired && einfo[ef].p2==p2 ) 
		  {
		    int type = entitytopo( rp, einfo[ef].r, maxVerts, nR, dDim);
		    einfo[ef].paired = true;
		    downward(einfo[ef].r,einfo[ef].el) = nedges; 
		    if ( ENTIDX(rp,einfo[ef].r,topo2EdgeVert[type][einfo[ef].el][0],nR,maxVerts)!=v)
		      dOrient[ einfo[ef].r + einfo[ef].el*downward.getLength(0) ] = 0x0;

		  }
	      nedges++;
	    }					      
	}
    }

  for ( int v=1; v<=maxVertIDX+1; v++ )
    upwardOffset(v) = upwardOffset(v-1)+upwardOffset(v);

  int * u_offset = new int[maxVertIDX+2];
  for ( int v=0; v<=maxVertIDX+1; v++ )
    u_offset[v] = 0;

  upwardIndex.redim(upwardOffset(maxVertIDX+1));
  upwardIndex = -1;
  if ( uOrient ) delete [] uOrient;
  uOrient = new char[upwardOffset(maxVertIDX+1)];
  for ( int u=0; u<upwardOffset(maxVertIDX+1); u++ )
    uOrient[u] = 0x1;

  int nedges2 = 0;
  for ( int v=0; v<maxVertIDX; v++ )
    {
      for ( int e=offset[v]; e<offset[v+1]; e++ )
	{
	  if ( einfo[e].paired )
	    {
	      int p2 = einfo[e].p2;
	      upwardIndex(upwardOffset(v)+u_offset[v+1]) = nedges2;

	      upwardIndex(upwardOffset(p2) + u_offset[p2+1]) = nedges2; 

	      uOrient[upwardOffset(v)+u_offset[v+1]] = 0x1;
	      uOrient[upwardOffset(p2) + u_offset[p2+1]] = 0x0;

	      for ( int ef=e+1; ef<offset[v+1]; ef++ )
		if ( einfo[ef].paired && einfo[ef].p2==p2 ) 
		  einfo[ef].paired = false;
	      
	      u_offset[v+1]++;
	      u_offset[p2+1]++;

	      nedges2++;

	    }
	}
    }

  for ( int v=0; v<maxVertIDX; v++ )
    {
      int c1 = einfo[offset[v]].ecount;
      int c2 = offset[v+1]-offset[v];
      if ( c2 )
	assert(c1==c2);
    }

  edges.resize(nedges,2);

  if ( offset ) delete[] offset;
  if ( einfo ) delete[] einfo;
  if ( u_offset ) delete[] u_offset;

  return nedges;
}

int constructFaceEntityFromRegion(intArray &faces, intArray &downward, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
				  intArray &regions, int nReg, 
				  int maxVertsPerFace, int maxVertIDX)
{
  int nfaces = 0;

  int dDim = 3;

  int nR = regions.getLength(0);
  int *rp = regions.getDataPointer();

  int *offset = new int[maxVertIDX+1];
  for ( int v=0; v<=maxVertIDX; v++ )
    offset[v] = 0;
 
  int maxVertsInReg = 8;
  
  downward.redim(nR,6);
  downward = -1;
  if ( dOrient ) delete [] dOrient;
  dOrient = new char[downward.getLength(0)*downward.getLength(1)];
  for ( int d=0; d<downward.getLength(0)*downward.getLength(1); d++ )
    dOrient[d] = 0x1;

  int nf=0;
  for ( int r=0; r<nReg; r++ )
    {
      int type=entitytopo( rp, r, maxVertsInReg, nR, dDim);

      nf += topoNFaces[type];
      for ( int f=0; f<topoNFaces[type]; f++ )
	{
	  int vmin = INT_MAX;
	  for ( int v=0; v<topo2FaceNVerts[type][f]; v++ )
	    vmin = min(vmin, ENTIDX(rp,r,topo2FaceVert[type][f][v],nR,maxVertsInReg));
	  
	  offset[vmin+1]++;
	}
    }

  for ( int v=1; v<=maxVertIDX; v++ )
    offset[v] = offset[v-1]+offset[v];

  struct f_info { int r; int rf; int fcount; bool paired; };
  f_info *finfo = new f_info[nf+1];
  f_info null_f = { -1, -1, 0, false };

  for ( int f=0; f<(nf+1); f++ )
    finfo[f] = null_f; // the last one (nf) is null in case there are orphan nodes

  for ( int r=0; r<nReg; r++ )
    {
      int type=entitytopo( rp, r, maxVertsInReg, nR, dDim);
      for ( int f=0; f<topoNFaces[type]; f++ )
	{
	  int vmin = INT_MAX;
	  for ( int v=0; v<topo2FaceNVerts[type][f]; v++ )
	    vmin = min(vmin, ENTIDX(rp,r,topo2FaceVert[type][f][v],nR,maxVertsInReg));
	  
	  finfo[offset[vmin] + finfo[offset[vmin]].fcount].r = r;
	  finfo[offset[vmin] + finfo[offset[vmin]].fcount].rf = f;
	  finfo[offset[vmin]].fcount++;
	}
    }

  faces.redim(nf,maxVertsPerFace);
  faces=-1;

  upwardOffset.redim(nf+1);
  upwardOffset = 0;

  for ( int v=0; v<maxVertIDX; v++ )
    {
      for ( int f=offset[v]; f<offset[v+1]; f++ )
	{
	  if ( !finfo[f].paired )
	    {
	      int type=entitytopo( rp, finfo[f].r, maxVertsInReg, nR, dDim);

	      for ( int vf=0; vf<topo2FaceNVerts[type][finfo[f].rf]; vf++ )
		faces(nfaces,vf) = ENTIDX(rp,finfo[f].r,topo2FaceVert[type][finfo[f].rf][vf],nR,maxVertsInReg);

	      finfo[f].paired = true;
	      downward(finfo[f].r,finfo[f].rf) = nfaces; 
	      upwardOffset(nfaces+1)++;
	      for ( int ff=f+1; ff<offset[v+1]; ff++ )
		{
		  int type2=entitytopo(rp,finfo[ff].r,maxVertsInReg,nR,dDim);
		  if ( !finfo[ff].paired && facesAreEquivalent(rp, type, finfo[f].r, finfo[f].rf, 
							      type2,finfo[ff].r, finfo[ff].rf, maxVertsInReg,nR) )
		    {
		      finfo[ff].paired = true;
		      downward(finfo[ff].r,finfo[ff].rf) = nfaces; 
		      dOrient[ finfo[ff].r + finfo[ff].rf*downward.getLength(0) ] = 0x0;
		      upwardOffset(nfaces+1)++;
		      break;
		    }
		}
	      nfaces++;
	    }
	}
    }

  upwardOffset.resize(nfaces+1);
  for ( int v=1; v<=nfaces; v++ )
    upwardOffset(v) = upwardOffset(v-1)+upwardOffset(v);

  int * u_offset = new int[nfaces+1];
  for ( int v=0; v<=nfaces; v++ )
    u_offset[v] = 0;

  upwardIndex.redim(upwardOffset(nfaces));
  upwardIndex = -1;
  if ( uOrient ) delete [] uOrient;
  uOrient = new char[upwardOffset(nfaces)];
  for ( int u=0; u<upwardOffset(nfaces); u++ )
    uOrient[u] = 0x1;

  int nfaces2 = 0;
  for ( int v=0; v<maxVertIDX; v++ )
    {
      for ( int f=offset[v]; f<offset[v+1]; f++ )
	{
	  if ( finfo[f].paired )
	    {
	      int type=entitytopo( rp, finfo[f].r, maxVertsInReg, nR, dDim);

	      finfo[f].paired = false;
	      upwardIndex(upwardOffset(nfaces2) + u_offset[nfaces2]) = finfo[f].r;
	      u_offset[nfaces2]++;
	      for ( int ff=f+1; ff<offset[v+1]; ff++ )
		{
		  int type2=entitytopo(rp,finfo[ff].r,maxVertsInReg,nR,dDim);
		  if ( finfo[ff].paired && facesAreEquivalent(rp, type, finfo[f].r, finfo[f].rf, 
							      type2,finfo[ff].r, finfo[ff].rf, maxVertsInReg,nR) )
		    {
		      finfo[ff].paired = false;
		      upwardIndex(upwardOffset(nfaces2) + u_offset[nfaces2]) = finfo[ff].r; // XXX should specify direction too!
		      uOrient[upwardOffset(nfaces2) + u_offset[nfaces2]] = 0x0;
		      break;
		    }
		}
	      nfaces2++;
	    }
	}
    }

  for ( int v=0; v<maxVertIDX; v++ )
    {
      int c1 = finfo[offset[v]].fcount;
      int c2 = offset[v+1]-offset[v];
      if ( c2 )
	assert(c1==c2);
    }

  faces.resize(nfaces,maxVertsPerFace);

  //  constructUpwardAdjacenciesFromDownward(upwardIndex, upwardOffset, uOrient, downward, nfaces);

  if ( u_offset ) delete [] u_offset;
  if ( offset ) delete [] offset;
  if ( finfo ) delete [] finfo;

  return nfaces;
}

int constructRegion2EdgeFromFaces(intArray &region2Edge, char *&dOrient, 
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient, 
				  intArray &face2Edge, char *faceEdgeOrient, intArray &region2Face, 
				  intArray &faces, intArray &regions, int nReg, int maxVertIDX)
{
  if ( face2Edge.getLength(0)==0 )
    {
      return 1;
    }

  int nR = regions.getLength(0);

  region2Edge.redim(nR,12);
  region2Edge = -1;

  if ( dOrient ) delete [] dOrient;

  dOrient = new char[region2Edge.getLength(0)*region2Edge.getLength(1)];
  for ( int d=0; d<region2Edge.getLength(0)*region2Edge.getLength(1); d++ )
    dOrient[d] = 0x1;
  
  int *rp = regions.getDataPointer();
  int maxEdge =0;
  for ( int r=0; r<nReg; r++ )
    {
      int topo = entitytopo(rp, r, 8, nR, 3);

      for ( int f=0; f<topoNFaces[topo]; f++ )
	{
	  int vmin=INT_MAX,vmin2=INT_MAX;
	  int vidx=0, vidx2=0;
	  int nv = topo2FaceNVerts[topo][f];
	  for ( int e=0; e<nv; e++ )
	    {
	      if ( regions( r, topo2FaceVert[topo][f][e] )<vmin )
		{
		  vmin = regions( r, topo2FaceVert[topo][f][e] );
		  vidx = e;
		}
	      if ( faces( region2Face(r,f), e)<vmin2 )
		{
		  vmin2 = faces( region2Face(r,f), e);
		  vidx2 = e;
		}
	    }

	  assert(vmin==vmin2);
	  bool sense = regions(r, topo2FaceVert[topo][f][(vidx+1)%nv])==faces(region2Face(r,f), (vidx2+1)%nv);

	  int ff = region2Face(r,f);
	  if (sense)
	    {
	      for ( int e=0; e<nv; e++ )
		{
		  int faceE = topo2FaceEdge[topo][f][(vidx2+e)%nv];
		  region2Edge(r, faceE) = face2Edge(ff, (vidx+e)%nv);

		  //	kkc is this an ok change 030122	  if ( regions( r, topo2EdgeVert[topo][faceE][0] )>regions( r, topo2FaceVert[topo][faceE][1] ) )
		  if ( regions( r, topo2EdgeVert[topo][faceE][0] )>regions( r, topo2EdgeVert[topo][faceE][1] ) )
		    dOrient[ r + faceE*region2Edge.getLength(0) ] = 0x0;

		  maxEdge = max(maxEdge,abs(face2Edge(ff, (vidx+e)%nv)));
		}
	    }
	  else
	    {
	      for ( int e=0; e<nv; e++ )
		{
		  int faceE = topo2FaceEdge[topo][f][(vidx2+e)%nv];
		  region2Edge(r, faceE) = face2Edge(ff, (vidx+nv-e-1)%nv);

		  //	kkc is this an ok change 030122		  if ( regions( r, topo2EdgeVert[topo][faceE][0] )>regions( r, topo2FaceVert[topo][faceE][1] ) )
		  if ( regions( r, topo2EdgeVert[topo][faceE][0] )>regions( r, topo2EdgeVert[topo][faceE][1] ) )
		    dOrient[ r + faceE*region2Edge.getLength(0) ] = 0x0;

		  maxEdge = max(maxEdge, abs(face2Edge(ff, (vidx+nv-e-1)%nv)));
		}
	    }
	  
	}
    }

  constructUpwardAdjacenciesFromDownward(upwardIndex, upwardOffset, uOrient, region2Edge, dOrient, maxEdge);

  return 0;
}

int constructFace2EdgeFromRegions(intArray &face2Edge, char *&dOrient,
				  intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
				  intArray &region2Edge, char *regionEdgeOrient, intArray &region2Face, 
				  intArray &faces, intArray &edges, intArray &regions, int nReg, 
				  int maxFaces, int maxVertIDX)
{

  if ( region2Edge.getLength(0)==0 )
    {
      return 1;
    }

  int nR = regions.getLength(0);
  face2Edge.redim(maxFaces,4);
  face2Edge = -1;
  if ( dOrient ) delete [] dOrient;
  dOrient = new char[face2Edge.getLength(0)*face2Edge.getLength(1)];
  for ( int d=0; d<face2Edge.getLength(0)*face2Edge.getLength(1); d++ )
    dOrient[d] = 0x1;

  int *rp = regions.getDataPointer();
  int maxEdge = 0;
  for ( int r=0; r<nReg; r++ )
    {
      int topo = entitytopo(rp, r, 8, nR, 3);

      for ( int f=0; f<topoNFaces[topo]; f++ )
	{
	  int vmin=INT_MAX,vmin2=INT_MAX;
	  int vidx=0, vidx2=0;
	  int nv = topo2FaceNVerts[topo][f];
	  for ( int e=0; e<nv; e++ )
	    {
	      if ( regions( r, topo2FaceVert[topo][f][e] )<vmin )
		{
		  vmin = regions( r, topo2FaceVert[topo][f][e] );
		  vidx = e;
		}
	      if ( faces( region2Face(r,f), e)<vmin2 )
		{
		  vmin2 = faces( region2Face(r,f), e);
		  vidx2 = e;
		}
	    }

	  assert(vmin==vmin2);

	  bool sense = regions(r, topo2FaceVert[topo][f][(vidx+1)%nv])==faces(region2Face(r,f), (vidx2+1)%nv);

	  if (sense)
	    {
	      for ( int e=0; e<nv; e++ )
		{
		  int faceE = topo2FaceEdge[topo][f][(vidx+e)%nv];
		  // WHY DID version 040830 have :
		  //face2Edge(region2Face(r,f), e/*kkc 040830 fix order(vidx2+e)%nv*/) = region2Edge(r, faceE);

		  face2Edge(region2Face(r,f), (vidx2+e)%nv) = region2Edge(r, faceE);

		  if ( edges(region2Edge(r,faceE),1)==faces(region2Face(r,f),(vidx2+e)%nv) )
		    dOrient[region2Face(r,f) + (vidx2+e)%nv*face2Edge.getLength(0)] = 0x0;
		  // WHY DID version 040830 have :
		  //dOrient[region2Face(r,f) + e*face2Edge.getLength(0) /*kkc 040830 fix order (vidx2+e)%nv*face2Edge.getLength(0)*/] = 0x0;
		  else if ( edges(region2Edge(r,faceE),0)!=faces(region2Face(r,f),(vidx2+e)%nv) )
		    return 1;

		  maxEdge = max(maxEdge, abs(region2Edge(r, topo2FaceEdge[topo][f][(vidx+e)%nv])));
		}
	    }
#if 1
	  else
	    {
	      for ( int e=0; e<nv; e++ )
		{
		  int faceE = topo2FaceEdge[topo][f][(vidx+nv-e-1)%nv];
		  face2Edge(region2Face(r,f), (vidx2+e)%nv) = region2Edge(r, faceE);

		  if ( edges(region2Edge(r,faceE),1)==faces(region2Face(r,f),(vidx2+e)%nv) )
		    dOrient[region2Face(r,f) + (vidx2+e)%nv*face2Edge.getLength(0)] = 0x0;
		  else if ( edges(region2Edge(r,faceE),0)!=faces(region2Face(r,f),(vidx2+e)%nv) )
		    return 1;

		  maxEdge = max(maxEdge, abs(region2Edge(r, topo2FaceEdge[topo][f][(vidx+nv-e-1)%nv])));
		}
	    }
#endif
	}
    }

  constructUpwardAdjacenciesFromDownward(upwardIndex, upwardOffset, uOrient, face2Edge, dOrient, maxEdge);

  return 0;
}

int constructUpwardAdjacenciesFromDownward(intArray &upwardIndex, intArray &upwardOffset, char *&uOrient,
					   const intArray &downward, const char *dOrient, int maxIDX)
{

  int nUE = maxIDX+1;
  int nE = downward.getLength(0);
  int nD = downward.getLength(1);

  upwardOffset.redim(nUE+1);
  upwardOffset = 0;

  int *upo = upwardOffset.getDataPointer();
  for ( int e=0; e<nE; e++ )
    for ( int d=0; d<nD && downward(e,d)>-1; d++ )
      upo[downward(e,d)+1]++;


  for ( int e=1; e<=nUE; e++ )
    upo[e] = upo[e-1]+upo[e];

  int *upo2 = new int[nUE+1];
  for ( int e=0; e<=nUE; e++ )
    upo2[e] = 0;

  upwardIndex.redim(upo[nUE]);
  upwardIndex = -1;

  if ( uOrient ) delete [] uOrient;

  uOrient = new char[upo[nUE]];
  for ( int u=0; u<upo[nUE]; u++ )
    uOrient[u] = 0x1;

  int *up = upwardIndex.getDataPointer();

  for ( int e=0; e<nE; e++ )
    for ( int d=0; d<nD && downward(e,d)>-1; d++ )
      {
	up[upo[downward(e,d)]+upo2[downward(e,d)]] = e;
	if ( dOrient ) 
	  {
	    uOrient[upo[downward(e,d)]+upo2[downward(e,d)]] = dOrient[ e + d*downward.getLength(0) ];
	  }
	upo2[downward(e,d)]++;
      }
  
  if (upo2) delete[] upo2;

  return 0;
}

#ifdef KKC_TEMPLATETEST

#include <iostream>

int main(int argc, char *argv[])
{

#if 1
  intArray regions(4,8);
  regions = -1;

  // hex
  regions(0,0) = 0;
  regions(0,1) = 1;
  regions(0,2) = 3;
  regions(0,3) = 7;
  regions(0,4) = 5;
  regions(0,5) = 2;
  regions(0,6) = 4;
  regions(0,7) = 6;

  // prism
  regions(1,0) = 2;
  regions(1,1) = 10;
  regions(1,2) = 4;
  regions(1,3) = 11;
  regions(1,4) = 9;
  regions(1,5) = 8;

  // pyramid
  regions(2,0) = 1;
  regions(2,1) = 3;
  regions(2,2) = 4;
  regions(2,3) = 2;
  regions(2,4) = 10;

  // tet
  regions(3,0) = 1;
  regions(3,1) = 3;
  regions(3,2) = 10;
  regions(3,3) = 12;
#else
  intArray regions(8,4);
  regions = -1;

  // hex
  regions(0,0) = 0;
  regions(1,0) = 1;
  regions(2,0) = 3;
  regions(3,0) = 7;
  regions(4,0) = 5;
  regions(5,0) = 2;
  regions(6,0) = 4;
  regions(7,0) = 6;

  // prism
  regions(0,1) = 2;
  regions(1,1) = 10;
  regions(2,1) = 4;
  regions(3,1) = 11;
  regions(4,1) = 9;
  regions(5,1) = 8;

  // pyramid
  regions(0,2) = 1;
  regions(1,2) = 3;
  regions(2,2) = 4;
  regions(3,2) = 2;
  regions(4,2) = 10;

  // tet
  regions(0,3) = 1;
  regions(1,3) = 3;
  regions(2,3) = 10;
  regions(3,3) = 12;
#endif

  int *rp = regions.getDataPointer();
  for ( int r=0; r<4; r++ )
    {
      int topo=entitytopo( rp, r, 8, 4, 3);

      for ( int f=0; f<topoNFaces[topo]; f++ )
	{
	  for ( int ve=0; ve<topo2FaceNVerts[topo][f]; ve++ )
	    {
	      int face_vertex = topo2FaceVert[topo][f][ve];
	      int face_edge = topo2FaceEdge[topo][f][ve];
	      int reg_vert_from_edge1 = topo2EdgeVert[topo][ face_edge ][0];
	      int reg_vert_from_edge2 = topo2EdgeVert[topo][ face_edge ][1];
	      int vFromFace = regions( r, face_vertex );
	      int vFromFaceEdge1 = regions( r, reg_vert_from_edge1 );
	      int vFromFaceEdge2 = regions( r, reg_vert_from_edge2 );
	      if ( vFromFace!=vFromFaceEdge1 && vFromFace!=vFromFaceEdge2) abort();
	    }
	}
    }

  std::cout<<"templates appear ok!"<<std::endl;

  intArray faces,faces2,edges,edges2,face2edge,face2edge2,region2face,region2edge,region2edge2;
  intArray upwardIndex,upwardOffset;
  char *oUp=0, *oDown=0;
  int ne,ne2,nf;

  std::cout<<"number of edges is "<<(ne=constructEdgeEntityFromEntity(edges, region2edge, oDown, 
								      upwardIndex,upwardOffset, oUp, regions, regions.getLength(0),
								      8, 12, 3))<<std::endl;

  std::cout<<"number of faces is "<<(nf=constructFaceEntityFromRegion(faces, region2face, oDown, 
								      upwardIndex,upwardOffset, oUp, regions, regions.getLength(0),
								      4, 12))<<std::endl;
  faces2=faces;

  std::cout<<"number of edges using faces is "<<(ne2=constructEdgeEntityFromEntity(edges2,face2edge,oDown, 
										   upwardIndex,upwardOffset, oUp, faces2,
										   faces.getLength(0),
										   4,12,2))<<std::endl;
  assert(ne==ne2);

  assert(constructFace2EdgeFromRegions(face2edge2, oDown, upwardIndex,upwardOffset, oUp, region2edge, 0, 
				       region2face,  faces,edges,regions,regions.getLength(0),nf,12)==0);

  for ( int r=0; r<4; r++ )
    {
      int topo=entitytopo( rp, r, 8, 4, 3);
      for ( int f=0; f<topoNFaces[topo]; f++ )
	{
	  int rf = region2face(r,f);
	  int nv = topo2FaceNVerts[topo][f];
	  for ( int ev=0; ev<nv; ev++ )
	    {
	      int v1 = faces(rf,ev);
	      int v2 = faces(rf,(ev+1)%nv);
	      int fv1 = edges(face2edge2(rf,ev),0);
	      int fv2 = edges(face2edge2(rf,ev),1);

	      assert( (v1==fv1&&v2==fv2) || (v1==fv2&&v2==fv1) );
	    }
	}
    }
  

  cout<<"successfully reconstructed face-->edge from regions"<<endl;
  
  assert(constructRegion2EdgeFromFaces(region2edge2, oDown, upwardIndex,upwardOffset, 
				       oUp, face2edge, 0, region2face, faces, regions, regions.getLength(0),
				       12)==0);

  for ( int r=0; r<4; r++ )
    {
      int topo=entitytopo( rp, r, 8, 4, 3);
      for ( int e=0; e<topoNEdges[topo]; e++ )
	{
	  int v1 = regions(r, topo2EdgeVert[topo][e][0]);
	  int v2 = regions(r, topo2EdgeVert[topo][e][1]);
	  int rv1 = edges2(region2edge2(r,e),0);
	  int rv2 = edges2(region2edge2(r,e),1);

	  assert( (v1==rv1&&v2==rv2) || (v1==rv2&&v2==rv1) );
	}
    }

  cout<<"successfully reconstructed region-->edge from faces"<<endl;

  return 0;
}

#endif

#undef ENTIDX
