#define BOUNDS_CHECK
#include "UnstructuredMapping.h"
#include "ArraySimple.h"
#include "UnstructuredOperators.h"

inline static real computeElementArea(int e, const UnstructuredMapping & umap, const RealArray &nodes, const intArray &elems)
{
  int nne = umap.getNumberOfNodesThisElement(e);
  int ne;
  
  realArray enodes(4,2);
  for ( ne=0; ne<nne; ne++ )
    {
      enodes(ne,0) = nodes(elems(e,ne),0);
      enodes(ne,1) = nodes(elems(e,ne),1);
    }
  
  real area;
  if ( nne==4 )
    area = 0.5*( (enodes(2,0)-enodes(0,0))*(enodes(3,1)-enodes(1,1)) -
		 (enodes(2,1)-enodes(0,1))*(enodes(3,0)-enodes(1,0)) );
  else if (nne=3)
    area = ( enodes(0,0)*(enodes(1,1)-enodes(2,1)) -
	     enodes(0,1)*(enodes(1,0)-enodes(2,0)) + 
	     enodes(1,0)*enodes(2,1)-enodes(2,0)*enodes(1,1) )/2;

  return area;
}

RealMappedGridFunction
UnsOperatorsEDGEFV::
x(const RealMappedGridFunction &sf)
{
  const RealArray &scalarFunc = (const RealArray &)sf;

  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  //  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction xoutmgf(*mg);
  RealArray &xout = (RealArray &)((const RealArray &)xoutmgf);

  xout = 0;
  real cent[3];

  int ne;
  real f[4];
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int n1=faces(e,0);
      int n2=faces(e,1);
      real f = (scalarFunc(n1)+scalarFunc(n2))*areaNormals(e,0)/2;
      xout(n1,0,0,0) += f/areas(n1);
      xout(n2,0,0,0) -= f/areas(n2);
    }

  return xoutmgf;
}

RealMappedGridFunction
UnsOperatorsEDGEFV::
xx(const RealMappedGridFunction &sf)
{
  const RealArray &scalarFunc = (const RealArray &)sf;

  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");

  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction xxoutmgf(*mg);
  RealArray & xxout = (RealArray &)((const RealArray &)xxoutmgf);
  xxout = 0;
  real cent[3];

  int ne;
  real f[4];
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int e1=faceElements(e,0);
      int e2=faceElements(e,1);
      
      real e1dsdx=0, e2dsdx=0;
      real e1area,e2area;
      e1area = computeElementArea(e1,umap,nodes,elems);

      int nne=umap.getNumberOfNodesThisElement(e1);
      for ( int i=0; i<nne; i++ )
	{
	  e1dsdx += (nodes(elems(e1,(i+1)%nne),1)-nodes(elems(e1,i),1))*
	    (scalarFunc(elems(e1,(i+1)%nne))+scalarFunc(elems(e1,i)))/2/e1area;
	}

      if ( e2>-1 )
	{
	  e2area = computeElementArea(e2,umap,nodes,elems);

	  nne=umap.getNumberOfNodesThisElement(e2);
	  for ( int i=0; i<nne; i++ )
	    {
	      e2dsdx += (nodes(elems(e2,(i+1)%nne),1)-nodes(elems(e2,i),1))*
		(scalarFunc(elems(e2,(i+1)%nne))+scalarFunc(elems(e2,i)))/2/e2area;
	    }
	}

      int n1=faces(e,0);
      int n2=faces(e,1);
      real f = (e1dsdx+e2dsdx)*areaNormals(e,0)/2;
      xxout(n1,0,0,0) += f/areas(n1);
      xxout(n2,0,0,0) -= f/areas(n2);
    }

  return xxoutmgf;

}

RealMappedGridFunction
UnsOperatorsEDGEFV::
y(const RealMappedGridFunction &sf)
{
  
  const RealArray &scalarFunc = (const RealArray &)sf;

  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  //  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction youtmgf(*mg);
  RealArray &yout = (RealArray &)((const RealArray &)youtmgf);
  yout = 0;

  int ne;
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int n1=faces(e,0);
      int n2=faces(e,1);
      real f = (scalarFunc(n1)+scalarFunc(n2))*areaNormals(e,1)/2;
      yout(n1,0,0,0) += f/areas(n1);
      yout(n2,0,0,0) -= f/areas(n2);
    }

  return youtmgf;

}

RealMappedGridFunction
UnsOperatorsEDGEFV::
yy(const RealMappedGridFunction &sf)
{
  
  const RealArray &scalarFunc = (const RealArray &)sf;

  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction yyoutmgf(*mg);
  RealArray &yyout = (RealArray &)((const RealArray &)yyoutmgf);
  yyout = 0;
  real cent[3];

  int ne;
  real f[4];
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int e1=faceElements(e,0);
      int e2=faceElements(e,1);
      
      real e1dsdy=0, e2dsdy=0;
      real e1area,e2area;
      e1area = computeElementArea(e1,umap,nodes,elems);

      int nne=umap.getNumberOfNodesThisElement(e1);
      for ( int i=0; i<nne; i++ )
	{	  
	  e1dsdy -= (nodes(elems(e1,(i+1)%nne),0)-nodes(elems(e1,i),0))*
	    (scalarFunc(elems(e1,(i+1)%nne))+scalarFunc(elems(e1,i)))/2/e1area;
	}

      if ( e2>-1 )
	{
	  e2area = computeElementArea(e2,umap,nodes,elems);

	  nne=umap.getNumberOfNodesThisElement(e2);
	  for ( int i=0; i<nne; i++ )
	    {
	      e2dsdy -= (nodes(elems(e2,(i+1)%nne),0)-nodes(elems(e2,i),0))*
		(scalarFunc(elems(e2,(i+1)%nne))+scalarFunc(elems(e2,i)))/2/e2area;
	    }
	}

      int n1=faces(e,0);
      int n2=faces(e,1);
      real f = (e1dsdy+e2dsdy)*areaNormals(e,1)/2;
      yyout(n1,0,0,0) += f/areas(n1);
      yyout(n2,0,0,0) -= f/areas(n2);
    }

  return yyoutmgf;

}

RealMappedGridFunction 
UnsOperatorsEDGEFV::
grad(const RealMappedGridFunction &sf)
{
  
  const RealArray &scalarFunc = (const RealArray &)sf;

  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  //  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  Range all;
  RealMappedGridFunction gradientmgf(*mg,all,all,all,rangeDimension);
  RealArray &gradient = (RealArray &)((const RealArray &)gradientmgf);

  gradient = 0;

  int ne;
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int n1=faces(e,0);
      int n2=faces(e,1);
      real fx = (scalarFunc(n1)+scalarFunc(n2))*areaNormals(e,0)/2;
      real fy = (scalarFunc(n1)+scalarFunc(n2))*areaNormals(e,1)/2;
      gradient(n1,0,0,0) += fx/areas(n1);
      gradient(n2,0,0,0) -= fx/areas(n2);
      gradient(n1,0,0,1) += fy/areas(n1);
      gradient(n2,0,0,1) -= fy/areas(n2);
    }

  return gradientmgf;
}

RealMappedGridFunction 
UnsOperatorsEDGEFV::
div(const RealMappedGridFunction &vf)
{
  
  const RealArray &vectorFunction = (const RealArray &)vf;


  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  //  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction divergencemgf(*mg);
  RealArray &divergence = (RealArray &) ((const RealArray &)divergencemgf);
  divergence = 0;

  int ne;
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int n1=faces(e,0);
      int n2=faces(e,1);
      real fx = (vectorFunction(n1,0,0,0)+vectorFunction(n2,0,0,0))*areaNormals(e,0)/2;
      real fy = (vectorFunction(n1,0,0,1)+vectorFunction(n2,0,0,1))*areaNormals(e,1)/2;
      divergence(n1,0,0,0) += (fx+fy)/areas(n1);
      divergence(n2,0,0,0) -= (fx+fy)/areas(n2);
    }

  return divergencemgf;
}

RealMappedGridFunction
UnsOperatorsEDGEFV::
laplacian(const RealMappedGridFunction &sf)
{
  
  const RealArray &scalarFunc = (const RealArray &)sf;

  RealMappedGridFunction lap;
  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);
  
  int nnodes = umap.getNumberOfNodes();
  //int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();
  int nedges = umap.getNumberOfEdges();
  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & edges = umap.getEdges();
  const intArray & faceElements = umap.getFaceElements();

  RealMappedGridFunction lapoutmgf(*mg);
  RealArray &lapout = (RealArray &)((const RealArray &)lapoutmgf);
  lapout = 0;

  int ne;
  int i;

  if ( areas.getLength(0)==0 )
    initialize();

  for ( int e=0; e<nfaces; e++ )
    {
      int e1=faceElements(e,0);
      int e2=faceElements(e,1);
      
      real e1dsdx=0, e2dsdx=0;
      real e1dsdy=0, e2dsdy=0;
      real e1area,e2area;
      e1area = computeElementArea(e1,umap,nodes,elems);

      int nne=umap.getNumberOfNodesThisElement(e1);
      for ( int i=0; i<nne; i++ )
	{
	  e1dsdx += (nodes(elems(e1,(i+1)%nne),1)-nodes(elems(e1,i),1))*
	    (scalarFunc(elems(e1,(i+1)%nne))+scalarFunc(elems(e1,i)))/2/e1area;

	  e1dsdy -= (nodes(elems(e1,(i+1)%nne),0)-nodes(elems(e1,i),0))*
	    (scalarFunc(elems(e1,(i+1)%nne))+scalarFunc(elems(e1,i)))/2/e1area;
       	}

      if ( e2>-1 )
	{
	  e2area = computeElementArea(e2,umap,nodes,elems);
	  
	  nne=umap.getNumberOfNodesThisElement(e2);
	  for ( int i=0; i<nne; i++ )
	    {	   
	      e2dsdx += (nodes(elems(e2,(i+1)%nne),1)-nodes(elems(e2,i),1))*
		(scalarFunc(elems(e2,(i+1)%nne))+scalarFunc(elems(e2,i)))/2/e2area;
	      e2dsdy -= (nodes(elems(e2,(i+1)%nne),0)-nodes(elems(e2,i),0))*
		(scalarFunc(elems(e2,(i+1)%nne))+scalarFunc(elems(e2,i)))/2/e2area;
	    }
	}

      int n1=faces(e,0);
      int n2=faces(e,1);
      real f = ((e1dsdx+e2dsdx)*areaNormals(e,0)+(e1dsdy+e2dsdy)*areaNormals(e,1))/2;
      lapout(n1,0,0,0) += f/areas(n1);
      lapout(n2,0,0,0) -= f/areas(n2);
    }

  return lapoutmgf;
}

void 
UnsOperatorsEDGEFV::
initialize()
{
  assert(mg!=NULL);
  assert(mg->mapping().getClassName()=="UnstructuredMapping");
  
  const UnstructuredMapping &umap = (UnstructuredMapping &)mg->mapping().getMapping();
  int rangeDimension = umap.getRangeDimension();
  assert(rangeDimension==2);

  int nnodes = umap.getNumberOfNodes();
  int nelems = umap.getNumberOfElements();
  int nfaces = umap.getNumberOfFaces();

  const intArray  & elems = umap.getElements();
  const RealArray & nodes = umap.getNodes();
  const intArray & faces = umap.getFaces();
  const intArray & faceElems = umap.getFaceElements();

  areas.redim(nnodes);
  areas = 0;
  areaNormals.redim(nfaces,rangeDimension);

  elementCenters.redim(nelems,rangeDimension);
  elementCenters = 0;

  real cent[2];
  real hmid[2];
  real d;

  ArraySimpleFixed<real,4,2,1,1> enodes;

  for ( int e=0; e<nelems; e++ )
    {
      int nne = umap.getNumberOfNodesThisElement(e);

      for ( int i=0; i<nne; i++ )
	for( int a=0; a<2; a++ )
	  elementCenters(e,a) += nodes(elems(e,i),a)/real(nne);

      if (nne==3)
	{ // triangle
	  int ne;
	  for ( ne=0; ne<nne; ne++ )
	    {
	      enodes(ne,0) = nodes(elems(e,ne),0);
	      enodes(ne,1) = nodes(elems(e,ne),1);
	    }

	  real area = ( enodes(0,0)*(enodes(1,1)-enodes(2,1)) -
			enodes(0,1)*(enodes(1,0)-enodes(2,0)) + 
			enodes(1,0)*enodes(2,1)-enodes(2,0)*enodes(1,1) )/2;

	  for ( ne=0; ne<nne; ne++ )
	    areas(elems(e,ne))+=area/3;
	}
      else
	{

	  int ne;
	  
	  for ( ne=0; ne<nne; ne++ )
	    {
	      enodes(ne,0) = nodes(elems(e,ne),0);
	      enodes(ne,1) = nodes(elems(e,ne),1);
	    }

	  real area = 0.5*( (enodes(2,0)-enodes(0,0))*(enodes(3,1)-enodes(1,1)) -
			    (enodes(2,1)-enodes(0,1))*(enodes(3,0)-enodes(1,0)) );
	  
	  for ( ne=0; ne<nne; ne++ )
	    areas(elems(e,ne))+=area/4;
	  
	}
    }

  for ( int f=0; f<nfaces; f++ )
    {
      int e1=faceElems(f,0);
      int e2=faceElems(f,1);
      int n1=faces(f,0);
      int n2=faces(f,1);

      if ( e2>-1 )
	{
	  areaNormals(f,0) = elementCenters(e1,1)-elementCenters(e2,1);
	  areaNormals(f,1) = -(elementCenters(e1,0)-elementCenters(e2,0));
	}
      else
	{ // boundaries
	  areaNormals(f,0) = elementCenters(e1,1) - (nodes(n1,1)+nodes(n2,1))/2;
	  areaNormals(f,1) = -(elementCenters(e1,0) - (nodes(n1,0)+nodes(n2,0)))/2;
	}
    }
}
