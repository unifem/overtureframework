//#define BOUNDS_CHECK
//#define OV_DEBUG

#include "rap.h"
// *wdh* 
#include "ModelBuilder.h"

#include "AdvancingFront.h"
#include "BoxMapping.h"
#include <fstream>

#include "CompositeGridFunction.h"
#include "MeshQuality.h"

#include "optMesh.h"
#include "uns_templates.h"

#ifdef SILO
extern "C" {
#include "silo.h"
}
#endif

using namespace std;

void optimize(UnstructuredMapping &umap, RealCompositeGridFunction *cf)
{ 
  if (cf)
    {
      MetricCGFunctionEvaluator me(cf);
      optimize(umap, me);
    }
  else
    {
      IdentityMetricEvaluator me;
      optimize(umap,me); 
    }
}

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

namespace {


  ArraySimpleFixed<int,6,2,3,1> HOEdgeMap;
  ArraySimpleFixed<int,4,3,3,1> HOFaceMap;

  void initHOMaps() 
  {
    // edge 0:
    //  e0
    HOEdgeMap(0,0,0) = 0;
    HOEdgeMap(0,0,1) = 0;
    HOEdgeMap(0,0,2) = 0;
    //  m
    HOEdgeMap(0,1,0) = 0;
    HOEdgeMap(0,1,1) = 0;
    HOEdgeMap(0,1,2) = 1;

    // edge 1:
    //  e0
    HOEdgeMap(1,0,0) = 1;
    HOEdgeMap(1,0,1) = 0;
    HOEdgeMap(1,0,2) = 0;
    //  m
    HOEdgeMap(1,1,0) = -1;
    HOEdgeMap(1,1,1) = 0;
    HOEdgeMap(1,1,2) = 1;

    // edge 2:
    //  e0
    HOEdgeMap(2,0,0) = 0;
    HOEdgeMap(2,0,1) = 1;
    HOEdgeMap(2,0,2) = 0;
    //  m
    HOEdgeMap(2,1,0) = 0;
    HOEdgeMap(2,1,1) = -1;
    HOEdgeMap(2,1,2) = 1;

    // edge 3:
    //  e0
    HOEdgeMap(3,0,0) = 0;
    HOEdgeMap(3,0,1) = 0;
    HOEdgeMap(3,0,2) = 0;
    //  m
    HOEdgeMap(3,1,0) = 0;
    HOEdgeMap(3,1,1) = 1;
    HOEdgeMap(3,1,2) = 0;

    // edge 4:
    //  e0
    HOEdgeMap(4,0,0) = 0;
    HOEdgeMap(4,0,1) = 0;
    HOEdgeMap(4,0,2) = 0;
    //  m
    HOEdgeMap(4,1,0) = 1;
    HOEdgeMap(4,1,1) = 0;
    HOEdgeMap(4,1,2) = 0;

    // edge 5:
    //  e0
    HOEdgeMap(5,0,0) = 1;
    HOEdgeMap(5,0,1) = 0;
    HOEdgeMap(5,0,2) = 0;
    //  m
    HOEdgeMap(5,1,0) = -1;
    HOEdgeMap(5,1,1) = 1;
    HOEdgeMap(5,1,2) = 0;

    // face 0:
    //   f0
    HOFaceMap(0,0,0) = 0;
    HOFaceMap(0,0,1) = 0;
    HOFaceMap(0,0,2) = 0;
    //   M(:,0)
    HOFaceMap(0,1,0) = 1;
    HOFaceMap(0,1,1) = 0;
    HOFaceMap(0,1,2) = 0;
    //   M(:,1)
    HOFaceMap(0,2,0) = 0;
    HOFaceMap(0,2,1) = 0;
    HOFaceMap(0,2,2) = 1;

    // face 1:
    //   f0
    HOFaceMap(1,0,0) = 0;
    HOFaceMap(1,0,1) = 0;
    HOFaceMap(1,0,2) = 0;
    //   M(:,0)
    HOFaceMap(1,1,0) = 0;
    HOFaceMap(1,1,1) = 1;
    HOFaceMap(1,1,2) = 0;
    //   M(:,1)
    HOFaceMap(1,2,0) = 1;
    HOFaceMap(1,2,1) = 0;
    HOFaceMap(1,2,2) = 0;

    // face 2:
    //   f0
    HOFaceMap(2,0,0) = 0;
    HOFaceMap(2,0,1) = 0;
    HOFaceMap(2,0,2) = 0;
    //   M(:,0)
    HOFaceMap(2,1,0) = 0;
    HOFaceMap(2,1,1) = 0;
    HOFaceMap(2,1,2) = 1;
    //   M(:,1)
    HOFaceMap(2,2,0) = 0;
    HOFaceMap(2,2,1) = 1;
    HOFaceMap(2,2,2) = 0;

    // face 3:
    //   f0
    HOFaceMap(3,0,0) = 1;
    HOFaceMap(3,0,1) = 0;
    HOFaceMap(3,0,2) = 0;
    //   M(:,0)
    HOFaceMap(3,1,0) = -1;
    HOFaceMap(3,1,1) = 1;
    HOFaceMap(3,1,2) = 0;
    //   M(:,1)
    HOFaceMap(3,2,0) = -1;
    HOFaceMap(3,2,1) = 0;
    HOFaceMap(3,2,2) = 1;

  }
  
  bool initializeModelAndFront( CompositeSurface &model, AdvancingFront &advFront, BoxMapping &bgmap, MappedGrid &bggrid, CompositeGrid &cg,
				realCompositeGridFunction &strFun, real el, GenericGraphicsInterface &gi, bool recompute=true )
  {

    if ( !model.numberOfSubSurfaces() )
      return true;

    model.getCompositeTopology(true)->setDeltaS(el);
    model.getCompositeTopology(true)->setMaximumArea(.5*el*el);
    
    Range all;

    bool ok = recompute ? model.computeTopology(gi) : ( model.getCompositeTopology(true)->getTriangulation() ? true : model.computeTopology(gi));
    
    if ( !ok )
      {
	return false;
      }
    else
      {
	real x1,x2,y1,y2,z1,z2;
	
	x1 = model.getRangeBound(0,0);
	x2 = model.getRangeBound(1,0);
	y1 = model.getRangeBound(0,1);
	y2 = model.getRangeBound(1,1);
	z1 = model.getRangeBound(0,2);
	z2 = model.getRangeBound(1,2);
	
	real dx = el;
	bgmap.setVertices(x1-10*dx,x2+10*dx,y1-10*dx,y2+10*dx,z1-10*dx,z2+10*dx);
	bggrid.setMapping(bgmap);
	cg[0].update(MappedGrid::THEvertex);
	strFun.updateToMatchGrid(cg,all,all,all,3,3);
	strFun=0;
	
	for ( int a=0; a<3; a++ )
	  strFun[0](all,all,all,a,a) = 1.0/dx;

	cout<<model.getCompositeTopology(true)->getTriangulation()->size(UnstructuredMapping::Face)<<"   "<<model.getCompositeTopology(true)->getTriangulation()->getEntities(UnstructuredMapping::Edge).getLength(0)<<"  "<<model.getCompositeTopology(true)->getTriangulation()->getNodes().getLength(0)<<endl;

	//	verifyUnstructuredConnectivity(*(model.getCompositeTopology(true)->getTriangulation()),true);

	advFront.initialize((intArray &)model.getCompositeTopology(true)->getTriangulation()->getEntities(UnstructuredMapping::Face),
			    (realArray&)model.getCompositeTopology(true)->getTriangulation()->getNodes());
      }

    return true;
  }

  void setupAdvFrontDialog( AdvancingFrontParameters & params, DialogData &dia )
  {
    aString textCommands[] = {"neighbor angle", "number of advances", "quality tolerance", "print face",""};
    aString textLabels[] = {"Neighbor Angle", "Plotting interval (<0 for no plotting)", "Quality Tolerance", "Print Face Verts",""};
    aString textStrings[4];
    sPrintF(textStrings[0], "%f", params.getMaxNeighborAngle());
    sPrintF(textStrings[1], "%i", params.getNumberOfAdvances());
    sPrintF(textStrings[2], "%f", params.getQualityTolerance());
    sPrintF(textStrings[3], "%i", -1);
    dia.setTextBoxes(textCommands, textLabels, textStrings);
    
  }

  real getDefaultEdgeLength( CompositeSurface &cs )
  {
    real el = 0;

    for ( int a=0; a<cs.getRangeDimension(); a++ )
      el = max(el, cs.getRangeBound(1,a)-cs.getRangeBound(0,a));

    el/=25;

    return el;
  }


  bool writeSilo(CompositeSurface &cs, aString filename, UnstructuredMapping &umap, int order, realArray &HOnodes, intArray &HOelems, intArray &bdyFaces, intArray &bdyFaceSurf)
  {

#ifndef SILO
return false;
#else
    // the following optlist code was given to me by Mark Stowell; its what emsolve likes to see
    //       in the silo file
    DBoptlist * optlist = NULL;
    optlist = DBMakeOptlist(4);
    DBAddOption(optlist, DBOPT_XLABEL, (char *)"X Axis");
    DBAddOption(optlist, DBOPT_YLABEL, (char *)"Y Axis");
    DBAddOption(optlist, DBOPT_ZLABEL, (char *)"Z Axis");


    DBfile *sfile = DBCreate((char *)filename.c_str(), DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

    if ( !sfile ) return false;

    const realArray &nodes = umap.getNodes();
    int nv = umap.size(UnstructuredMapping::Vertex);
    double *coords[3];
    for ( int a=0; a<3; a++ )
      {
	coords[a] = new double[nv];
	for ( int n=0; n<nv; n++ )
	  coords[a][n] = nodes(n,a);
      }

    int shapesize[] = { 4 };
    int shapecnt[1];
    shapecnt[0] = umap.size(UnstructuredMapping::Region);
    int nshapes = 1;
    const intArray &faces = umap.getEntities(UnstructuredMapping::Face);
    const intArray &regions = umap.getEntities(UnstructuredMapping::Region);

    int *rpack = new int[ shapecnt[0]*shapesize[0] ];
    for ( int r=0; r<shapecnt[0]; r++ )
      {
	for ( int n=0; n<shapesize[0]; n++ )
	  rpack[r*shapesize[0] + n] = regions(r,n);
// 	int t=rpack[r*shapesize[0]];
// 	rpack[r*shapesize[0]] = rpack[r*shapesize[0]+2];
// 	rpack[r*shapesize[0]+2] = t;
      }

    string zlname = "zonelist";
    cout<<"dbputzl = "<<DBPutZonelist(sfile, (char *)zlname.c_str(), shapecnt[0], umap.getRangeDimension(), 
			rpack, shapecnt[0]*shapesize[0], 0, shapesize, shapecnt, nshapes )<<endl;


#ifdef SILOFACELIST
    string flname = "facelist";
    int fshapesize[] = { 3 };
    int fshapecnt[1];
    fshapecnt[0] = bdyFaces.getLength(0);
    cout<<(int *)bdyFaces.getDataPointer()<<"  "<<bdyFaces.getLength(0)*bdyFaces.getLength(1)<<endl;
    int typelist[] = {1};
    ArraySimple<int> types(bdyFaces.getLength(0));
    for ( int f=0; f<types.size(0); f++ )
      {
	int bc=0;
	aString colour = cs.getColour(bdyFaceSurf(f));
	for ( int c=0; c<GenericGraphicsInterface::numberOfColourNames; c++  )
	  if (Overture::getGraphicsInterface()->getColourName(bc)==colour)
	    {
	      bc = c;
	      break;
	    }

	types(f) = bc;
      }

    DBPutFacelist( sfile, (char *)flname.c_str(),
 		   bdyFaces.getLength(0),umap.getRangeDimension(),(int *)bdyFaces.getDataPointer(),
 		   bdyFaces.getLength(0)*bdyFaces.getLength(1),
 		   0, NULL, fshapesize, fshapecnt,
 		   1, types.ptr(), typelist, 1);

#endif

    ArraySimple<int> bcnum(10);
    int nbc=0;
    for ( int f=0; f<bdyFaces.getLength(0); f++ )
      {
	int bc=0;
	aString colour = cs.getColour(bdyFaceSurf(f));
	for ( int c=0; c<GenericGraphicsInterface::numberOfColourNames; c++  )
	  if (Overture::getGraphicsInterface()->getColourName(c)==colour)
	    {
	      bc = c;
	      break;
	    }
	
	int i=0;
	for ( ; i<nbc; i++ )
	  if ( bc==bcnum(i) ) 
	    break;

	if ( i==nbc )
	  {
	    if ( nbc==bcnum.size(0) )
	      bcnum.resize(bcnum.size(0) + 10);

	    bcnum(i) = bc;
	    nbc++;
	  }
      }

    ArraySimple<int> usedNode(nv);
    usedNode = -1;

    cout<<"NUMBER OF BCs = "<<nbc<<endl;
    for ( int b=0; b<nbc; b++ )
      {// add an ucdmesh for each boundary condition 
	int bc=bcnum(b);
	int nbcf=0;
	for ( int f=0; f<bdyFaces.getLength(0); f++ )
	  {
	    int bcf=-1;
	    aString colour = cs.getColour(bdyFaceSurf(f));
	    for ( int c=0; c<GenericGraphicsInterface::numberOfColourNames; c++  )
	      if (Overture::getGraphicsInterface()->getColourName(c)==colour)
		{
		  bcf = c;
		  break;
		}
	    if ( bcf == bc )
	      nbcf++;
	  }

	ArraySimple<int> tris(3,nbcf);
	
	nbcf=0;
	int nvbc=0;
	for ( int f=0; f<bdyFaces.getLength(0); f++ )
	  {
	    int bcf=-1;
	    aString colour = cs.getColour(bdyFaceSurf(f));
	    for ( int c=0; c<GenericGraphicsInterface::numberOfColourNames; c++  )
	      if (Overture::getGraphicsInterface()->getColourName(c)==colour)
		{
		  bcf = c;
		  break;
		}

	    if ( bcf == bc )
	      {
		for ( int v=0; v<3; v++ )
		  {
		    if ( usedNode( bdyFaces(f,v) )==-1 ) 
		      {
			usedNode( bdyFaces(f,v) ) = nvbc;
			nvbc++;
		      }
		    tris(v,nbcf) = usedNode(bdyFaces(f,v));
		  }
		nbcf++;
	      }
	  }
	
	ArraySimple<int> ngid(nvbc);
	ArraySimple<real> bcnodes(nvbc,3);
	//	nvbc=0;
	for ( int v=0; v<nv; v++ )
	  if(usedNode(v)>-1) 
	    {
	      for ( int a=0; a<3; a++ )
		bcnodes(usedNode(v),a) = nodes(v,a);
	      ngid(usedNode(v)) = v;
	      usedNode(v) = -1;
	    }
	aString zlname ="";
	sPrintF(zlname,"bc_%i_zl",bc);
	int shapesize[] = { 3 };
	int shapecnt[1];
	shapecnt[0] = nbcf;
	int shapetype[] = { DB_ZONETYPE_TRIANGLE };
	int nshapes = 1;
	cout<<"dbputzl("<<zlname<<") = "<<DBPutZonelist2(sfile, 
							(char *)zlname.c_str(), 
							shapecnt[0], umap.getRangeDimension(), 
							 tris.ptr(), tris.size(), 0, 0, 0, 
							shapetype,shapesize, shapecnt, nshapes,0 )<<endl;

	DBoptlist * optlist = NULL;
	optlist = DBMakeOptlist(4);
	DBAddOption(optlist, DBOPT_XLABEL, (char *)"X Axis");
	DBAddOption(optlist, DBOPT_YLABEL, (char *)"Y Axis");
	DBAddOption(optlist, DBOPT_ZLABEL, (char *)"Z Axis");	
	DBAddOption(optlist, DBOPT_NODENUM, (int *)(ngid.ptr()));

	aString ucdname = "";
	sPrintF(ucdname,"bc_%i",bc);
	double *lcoords[3];
	lcoords[0] = &bcnodes(0,0);
	lcoords[1] = &bcnodes(0,1);
	lcoords[2] = &bcnodes(0,2);
	
	DBPutUcdmesh( sfile, (char *)ucdname.c_str(), umap.getRangeDimension(),
		      NULL, (float**)lcoords, nvbc, shapecnt[0], (char *)zlname.c_str(), 
		      NULL, DB_DOUBLE,optlist);
	DBFreeOptlist(optlist);
      }

    DBAddOption(optlist, DBOPT_NODENUM, (int *)(0));
    DBPutUcdmesh( sfile, "mesh", umap.getRangeDimension(),
		  NULL, (float**)coords, nv, shapecnt[0], (char *)zlname.c_str(), 
#ifdef SILOFACELIST
		  (char *)flname.c_str(), 
#else
		  NULL, 
#endif
		  DB_DOUBLE, optlist );

    DBClose(sfile);
    for ( int a=0; a<3; a++ )
      delete [] coords[a];
    delete [] rpack;

    DBFreeOptlist(optlist);

    return true;
#endif
  }


  bool writeMesh(CompositeSurface &cs, aString filename, UnstructuredMapping &umap, int order, realArray &HOnodes, intArray &HOelems, intArray &bdyFaces, intArray &bdyFaceSurf)
  {
    ofstream file(filename.c_str());

    if ( !file )
      return false;

    string spc = "    ";

    int nMat  = 1;
    string tag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Face].c_str();    
    int nBdyF = distance(umap.tag_entity_begin(tag),umap.tag_entity_end(tag));
    
    file<<nMat<<spc<<HOnodes.getLength(0)<<spc<<umap.size(UnstructuredMapping::Region)<<spc<<nBdyF<<spc<<order<<endl;

#if 1
    const realArray & vertices = umap.getNodes();
    UnstructuredMappingIterator vert, vert_end;
    vert_end = umap.end(UnstructuredMapping::Vertex);
    for ( vert=umap.begin(UnstructuredMapping::Vertex); vert!=vert_end; vert++ )
      file<<*vert<<spc<<vertices(*vert,0)<<spc<<vertices(*vert,1)<<spc<<vertices(*vert,2)<<endl;

    int matID = 0;
    const intArray &regions = umap.getEntities(UnstructuredMapping::Region);
    UnstructuredMappingIterator reg, reg_end;
    reg_end = umap.end(UnstructuredMapping::Region);
    for ( reg=umap.begin(UnstructuredMapping::Region); reg!=reg_end; reg++ )
      file<<*reg<<spc<<matID<<spc<<regions(*reg,0)<<spc<<regions(*reg,1)<<spc<<regions(*reg,2)<<spc<<regions(*reg,3)<<endl;

    UnstructuredMapping::tag_entity_iterator bdyFace, bdyF_end;

    bdyF_end = umap.tag_entity_end(tag);
    const intArray &faces = umap.getEntities(UnstructuredMapping::Face);
    int bdyID = 0;
    for ( bdyFace=umap.tag_entity_begin(tag); bdyFace!=bdyF_end; bdyFace++ )
      file<<bdyID<<spc<<faces(bdyFace->e,0)<<spc<<faces(bdyFace->e,1)<<spc<<faces(bdyFace->e,2)<<endl;
#else

    for ( int v=0; v<HOnodes.getLength(0); v++ )
      file<<v<<spc<<HOnodes(v,0)<<spc<<HOnodes(v,1)<<spc<<HOnodes(v,2)<<endl;

    int matID = 0;
    for ( int e=0; e<HOelems.getLength(0); e++ )
      {
	file<<e<<spc<<matID<<spc;
	for ( int ev=0; ev<HOelems.getLength(1); ev++ )
	  file<<HOelems(e,ev)<<spc;

	file<<endl;
      }

    for ( int bf=0; bf<bdyFaces.getLength(0); bf++ )
      {
	file<<"DSI"<<spc<<bdyFaceSurf(bf)<<spc;
	for ( int fv=0; fv<bdyFaces.getLength(1); fv++ )
	  file<<bdyFaces(bf,fv)<<spc;
	file<<endl;
      }
#endif

    // ofstream destructor should close file ... ?! lets try!
    return true;
  }

  inline void countNPerFE( const int p, int &ne, int &nf )
  {
    ne = 0;
    nf = 0;
    for ( int i=1; i<=p; i++ )
      {
	nf+=i;
	for ( int j=1; j<=i; j++ )
	  ne+=j;
      }
  }

  inline int nodeLocation(const int &nnp, const int &xi, const int &eta, const int &zeta)
  {
    int n=0;
    for ( int iz=0; iz<=zeta; iz++ )
      {
	int em = iz==zeta ? eta : nnp-1-iz;
	for ( int ie=0; ie<=em; ie++ )
	  {
	    int xm = (iz==zeta && ie==eta) ? xi : nnp-iz-ie;
	    for ( int ix=0; ix<xm; ix++ )
	      n++;
	  }
      }

    return n;
  }


  void tagFacesWithSurfaceID(AdvancingFront &advFront, CompositeSurface &model, UnstructuredMapping &umap)
  {
    const vector<Face *> & advFFaces = advFront.getFaces();
    const intArray advFElem2Face = advFront.generateElementFaceList();
    const intArray &uFaces = umap.getEntities(UnstructuredMapping::Face);
    const intArray &surfid = model.getCompositeTopology(true)->getTriangulation()->getTags();

    // advFElem2Face.display("elem2face");
    string tag = string("boundary ") + UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Face].c_str();
    string surftag = string("surface");

    ArraySimple<int> fnds(3);

    for ( UnstructuredMapping::tag_entity_iterator tagit=umap.tag_entity_begin(tag);
	  tagit!=umap.tag_entity_end(tag); tagit++ )
      {
	UnstructuredMappingAdjacencyIterator reg = umap.adjacency_begin(*tagit, UnstructuredMapping::Region);

	int ir = *reg;

	UnstructuredMappingAdjacencyIterator regFace, regFace_end;
	regFace_end = umap.adjacency_end(reg, UnstructuredMapping::Face);
	int nf=0;
	for ( regFace = umap.adjacency_begin(reg, UnstructuredMapping::Face);
	      regFace!=regFace_end;
	      regFace++ )
	  {
	    for ( int i=0; i<3; i++ )
	      fnds[i] = advFFaces[advFElem2Face(*reg,nf)]->getVertex(i);
	    
	    //	    cout<<"reg, nf and face "<<*reg<<"  "<<nf<<"  "<<advFElem2Face(*reg,nf)<<"  "<<advFFaces[advFElem2Face(*reg,nf)]->getZ1ID()<<"  "<<advFFaces[advFElem2Face(*reg,nf)]->getZ2ID()<<endl;
	    if ( umap.entitiesAreEquivalent(UnstructuredMapping::Face, tagit->e, fnds) )
	      {
		//		cout<<fnds<<endl;
		//		cout<<uFaces(tagit->e,0)<<"  "<<uFaces(tagit->e,1)<<"  "<<uFaces(tagit->e,2)<<endl;
		//		cout<<"adding surface "<<surfid(advFElem2Face(*reg,nf))<<" to face "<<tagit->e<<endl;
		umap.addTag(UnstructuredMapping::Face, tagit->e, surftag, ((void*) surfid(advFElem2Face(*reg,nf))));
		break;
	      }
	    nf++;
	  }
	
      }

  }

  bool addHONodes(const int p, UnstructuredMapping &umap, CompositeSurface &model,
		  realArray &newNodes, intArray &newElements, intArray &bdyFaces, intArray &bdyFaceSurf, bool project = true)
  {
    initHOMaps();

    string tag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Face].c_str();    
    int nBdyF = distance(umap.tag_entity_begin(tag),umap.tag_entity_end(tag));
    int nVert = umap.size(UnstructuredMapping::Vertex);
    int nEdge = umap.size(UnstructuredMapping::Edge);
    int nFace = umap.size(UnstructuredMapping::Face);
    int nElem = umap.size(UnstructuredMapping::Region);
    const intArray &edges = umap.getEntities(UnstructuredMapping::Edge);
    const intArray &faces = umap.getEntities(UnstructuredMapping::Face);
    const intArray &elems = umap.getEntities(UnstructuredMapping::Region);

    int rDim = model.getRangeDimension();

    int nnPerEdgeToAdd = max(0,p-2);
    int nnPerFaceToAdd = 0;
    int nnPerElemToAdd = 0;

    int dum;
    countNPerFE(p-3, dum,nnPerFaceToAdd);
    countNPerFE(p-4, nnPerElemToAdd, dum);

    int nnPerElem = 0;
    int nnPerFace = 0;
    int nnPerEdge = p;
    countNPerFE(p,nnPerElem,nnPerFace);
    
    newElements.resize(nElem,nnPerElem);
    newElements = -1;
    bdyFaces.resize(nBdyF,nnPerFace);
    bdyFaces = -1;
    bdyFaceSurf.resize(nBdyF);
    bdyFaceSurf = -1;

    int nnToAdd = nnPerEdgeToAdd*nEdge + nnPerFaceToAdd*nFace + nnPerElemToAdd*nElem;
    int nnTotal = umap.size(UnstructuredMapping::Vertex) + nnToAdd;

    newNodes.resize(0);
    const realArray &nodes = umap.getNodes();
    //    cout<<umap.size(UnstructuredMapping::Vertex)<<"  "<<nodes.getLength(0)<<endl;
    newNodes = nodes;

    newNodes.resize(Range(nnTotal), Range(model.getRangeDimension()));

    newNodes(Range(nVert,nnTotal-1),Range(rDim)) = 0.;

    realArray pointsToProject;

    cout<<"======== HON =============="<<endl;
    cout<<"p = "<<p<<endl;
    cout<<"nnPerElem = "<<nnPerElem<<endl;
    cout<<"nnPerFace = "<<nnPerFace<<endl;
    
    cout<<"nnPerElemToAdd = "<<nnPerElemToAdd<<endl;
    cout<<"nnPerFaceToAdd = "<<nnPerFaceToAdd<<endl;
    cout<<"nnPerEdgeToAdd = "<<nnPerEdgeToAdd<<endl;

    cout<<"computed nnPerElem = "<<4 + nnPerEdgeToAdd*6 + 4*nnPerFaceToAdd + nnPerElemToAdd<<endl;
    
    assert(nnPerElem == (4 + nnPerEdgeToAdd*6 + 4*nnPerFaceToAdd + nnPerElemToAdd));

    real edgeDS = 1./real(nnPerEdge-1);

    MappingProjectionParameters mp;
    //    mp.setIsAMarchingAlgorithm(true);
    //            mp.setAdjustForCornersWhenMarching(false);

    if ( nnPerEdgeToAdd )
      pointsToProject.redim(nnPerEdge,3);

    std::string bdyTag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[(UnstructuredMapping::Edge)].c_str();
    model.getCompositeTopology(true)->getNumberOfEdgeCurves(); // do this to make sure the composite topology edges are still around
    //    int dbg=0;
    //    model.getCompositeTopology(true)->buildEdgeCurveSearchTree( dbg );

    for ( int e=0; e<nEdge && nnPerEdgeToAdd; e++ )
      {
	int v1 = edges(e,0);
	int v2 = edges(e,1);
	bool isBdy = umap.hasTag(UnstructuredMapping::Edge, e, bdyTag);

	pointsToProject(0,Range(3)) = nodes(v1,Range(3));
	for ( int n=0; n<nnPerEdgeToAdd; n++ )
	  {
	    real w = real(n+1)*edgeDS;
	    int nnid = nVert + nnPerEdgeToAdd*e+n;
	    for ( int a=0; a<rDim; a++ )
	      {
		newNodes(nnid,a) = (1.-w)*nodes(v1,a) + w*nodes(v2,a);
		if ( isBdy ) pointsToProject(1+n,a) = newNodes(nnid,a);
	      }
	  }

	pointsToProject(nnPerEdge-1,Range(3)) = nodes(v2,Range(3));

	if ( isBdy && project )
	  {

	    int surfs[2];
	    surfs[0] = surfs[1] = -1;
	    UnstructuredMappingAdjacencyIterator edgeSurf, edgeSurf_end;
	    int sf=0;
	    edgeSurf_end = umap.adjacency_end(UnstructuredMapping::Edge,e,UnstructuredMapping::Face);

	    for ( edgeSurf=umap.adjacency_begin(UnstructuredMapping::Edge,e,UnstructuredMapping::Face);
		  edgeSurf!=edgeSurf_end && sf<2;
		  edgeSurf++ )
	      {
		if ( umap.hasTag(UnstructuredMapping::Face,*edgeSurf,"surface") )
		  {
		    // *wdh* 100421 surfs[sf] = (int)umap.getTagData(UnstructuredMapping::Face,*edgeSurf,"surface");
		    surfs[sf] = (intptr_t)umap.getTagData(UnstructuredMapping::Face,*edgeSurf,"surface");
		    sf++;
		  }
	      }

	    assert(surfs[0]!=-1 && surfs[1]!=-1);
	    mp.getIntArray(MappingProjectionParameters::elementIndex).redim(0);

	    if ( surfs[0]==surfs[1] )
	      model.project(pointsToProject,mp);
	    else
	      {
		// we need to find the appropriate edge curve!
		real x[3];
		for ( int aa=0; aa<3; aa++ )
		  x[aa] = (nodes(v1,aa) + nodes(v2,aa))/2.;

		int nearestEdgeCurve = model.getCompositeTopology(true)->getNearestEdge(x);

		//		nearestEdgeCurve = model.getCompositeTopology(true)->edgeFromNumber(nearestEdgeCurve)->masterEdgeNumber();
		//		EdgeInfo & edgeCurveInfo = *(model.getCompositeTopology(true)->edgeFromNumber(nearestEdgeCurve));

		Mapping &curve =  model.getCompositeTopology(true)->getEdgeCurve(nearestEdgeCurve);//edgeCurveInfo.curve->getNURBS();
		//		PlotIt::plot(*Overture::getGraphicsInterface(), *curve);
		//		cout<<"USING EDGE CURVE "<<nearestEdgeCurve<<endl;
		//		pointsToProject.display("old");
		curve.project(pointsToProject,mp);
		//		pointsToProject.display("new");
	      }

	    //	    pointsToProject = mp.getRealArray(MappingProjectionParameters::x);
	    for ( int n=0; n<nnPerEdgeToAdd; n++ )
	      {
		int nnid = nVert + nnPerEdgeToAdd*e+n;
		for ( int a=0; a<rDim; a++ )
		  newNodes(nnid,a) = pointsToProject(1+n,a);
	      }
	    //	    mp.getRealArray(MappingProjectionParameters::r) = -1;
	    mp.reset();
	  }
      }

    if ( nnPerFaceToAdd )
      pointsToProject.redim(nnPerFaceToAdd,3);

    bdyTag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[(UnstructuredMapping::Face)].c_str();
    for ( int f=0; f<nFace &&nnPerFaceToAdd; f++ )
      {
	int v1=faces(f,0), v2=faces(f,1), v3=faces(f,2);
	int edg[3], &e1=edg[0], &e2=edg[1], &e3=edg[2];
	int ort[3], &o1=ort[0], &o2=ort[1], &o3=ort[2];
	UnstructuredMappingAdjacencyIterator edgIt, edgIt_end;
	edgIt_end = umap.adjacency_end(UnstructuredMapping::Face, f, UnstructuredMapping::Edge);
	edgIt = umap.adjacency_begin(UnstructuredMapping::Face, f, UnstructuredMapping::Edge);
	int eidx=0;
	for ( ; edgIt!=edgIt_end; edgIt++ )
	  {
	    edg[eidx] = (*edgIt);
	    ort[eidx] = edgIt.orientation();
	    eidx++;
	  }

	int n=0;
	bool isBdy = umap.hasTag(UnstructuredMapping::Face, f, bdyTag);
	//	for ( int n1=0; n1<nnPerEdge-3; n1++ )
	for ( int n1=0; n1<nnPerEdgeToAdd-1; n1++ )
	  {
	    real a = (n1+1)*edgeDS;
	    int nleft = o3<0 ? nVert + nnPerEdgeToAdd*e3+n1 : nVert + nnPerEdgeToAdd*e3+nnPerEdgeToAdd -1-n1;
	    real xl = newNodes(nleft,0);
	    real yl = newNodes(nleft,1);
	    real zl = newNodes(nleft,2);
	    
	    int nright = o2>0 ? nVert + nnPerEdgeToAdd*e2+n1 : nVert + nnPerEdgeToAdd*e2+nnPerEdgeToAdd -1-n1;
	    real xr = newNodes(nright,0);
	    real yr = newNodes(nright,1);
	    real zr = newNodes(nright,2);

// 	    cout<<"v1 v2 v3 "<<v1<<"  "<<v2<<"  "<<v3<<endl;
// 	    cout<<"e3 "<<edges(e3,0)<<"  "<<edges(e3,1)<<"  "<<o3<<endl;
// 	    cout<<"e2 "<<edges(e2,0)<<"  "<<edges(e2,1)<<"  "<<o2<<endl;
// 	    cout<<"e1 "<<edges(e1,0)<<"  "<<edges(e1,1)<<"  "<<o1<<endl;

// 	    assert( (o3>0 && edges(e3,0)==v3) || (o3<0 && edges(e3,0)==v1) );
// 	    assert( (o2>0 && edges(e2,0)==v2) || (o2<0 && edges(e2,0)==v3) );

	    //	    for ( int n2=0; n2<(nnPerEdge-3-n1); n2++ )
	    for ( int n2=0; n2<(nnPerEdgeToAdd-1-n1); n2++ )
	      {
		real b = real(n2+1)/real(nnPerEdge-n1-2);

		//		int nb2use = int(b*(nnPerEdgeToAdd-1));
		int nb2use = int(b*(nnPerEdge-1))-1;
		int nBot = o1>0 ? nVert + nnPerEdgeToAdd*e1+nb2use : nVert + nnPerEdgeToAdd*e1+nnPerEdgeToAdd -1-nb2use;
		int nTop = v3;

		// + ------- + ------- + ------
		//          nb2    b 
		real bc = 1.-(b-(nb2use+1)*edgeDS)/edgeDS;

		int off = o1>0 ? 1 : -1;

		//		bc = .5;
		//if ( n1%2==1 ) off = 0;

		real xb = bc*newNodes(nBot,0)+(1-bc)*newNodes(nBot+off,0);
		real yb = bc*newNodes(nBot,1)+(1-bc)*newNodes(nBot+off,1);
		real zb = bc*newNodes(nBot,2)+(1-bc)*newNodes(nBot+off,2);

		//		cout<<b<<"  "<<edgeDS<<"  "<<nb2use<<"  "<<bc<<"  "<<xb<<"  "<<yb<<"  "<<zb<<endl;
		real xt = newNodes(v3,0);
		real yt = newNodes(v3,1);
		real zt = newNodes(v3,2);

		//		cout<<"a,b "<<a<<"  "<<b<<endl;
		real xn1 = (1-b)*xl + b*xr;
		real yn1 = (1-b)*yl + b*yr;
		real zn1 = (1-b)*zl + b*zr;


		real xn2 = (1.-a)*xb + a*xt;
		real yn2 = (1.-a)*yb + a*yt;
		real zn2 = (1.-a)*zb + a*zt;

//  		real xtp = (1-a)*( (1-b)*newNodes(v1,0)+b*newNodes(v3,0) ) + a*( (1-b)*newNodes(v2,0) + b*newNodes(v3,0));
//  		real ytp = (1-a)*( (1-b)*newNodes(v1,1)+b*newNodes(v3,1) ) + a*( (1-b)*newNodes(v2,1) + b*newNodes(v3,1));
//  		real ztp = (1-a)*( (1-b)*newNodes(v1,2)+b*newNodes(v3,2) ) + a*( (1-b)*newNodes(v2,2) + b*newNodes(v3,2));

 		real xtp = (1-b)*( (1-a)*newNodes(v1,0)+a*newNodes(v3,0) ) + b*( (1-a)*newNodes(v2,0) + a*newNodes(v3,0) );
 		real ytp = (1-b)*( (1-a)*newNodes(v1,1)+a*newNodes(v3,1) ) + b*( (1-a)*newNodes(v2,1) + a*newNodes(v3,1) );
 		real ztp = (1-b)*( (1-a)*newNodes(v1,2)+a*newNodes(v3,2) ) + b*( (1-a)*newNodes(v2,2) + a*newNodes(v3,2) );

		//		real b = (n2+1)*edgeDS;
		//		real c = 1.-a-b;


		newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,0) = xn1 + xn2 - xtp;
		newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,1) = yn1 + yn2 - ytp;
		newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,2) = zn1 + zn2 - ztp;

		for ( int aa=0; aa<rDim && isBdy; aa++ )
		  pointsToProject(n,aa) = newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,aa);
// 		for ( int aa=0; aa<rDim; aa++ )
// 		  {
// 		    newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,aa) = c*nodes(v1,aa)+b*nodes(v2,aa)+a*nodes(v3,aa);
// 		    if ( isBdy )
// 		      pointsToProject(n,aa) = newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,aa);
// 		  }
		n++;
	      }
	  }

	if ( isBdy && project )
	  {
	    assert( umap.hasTag(UnstructuredMapping::Face, f, "surface"));
	    // *wdh* 100421 int surf =  (int)umap.getTagData(UnstructuredMapping::Face, f, "surface");
	    int surf =  (intptr_t)umap.getTagData(UnstructuredMapping::Face, f, "surface");
	    model[surf].project(pointsToProject, mp);
	    //	    pointsToProject = mp.getRealArray(MappingProjectionParameters::x);
	    int n =0;
	    for ( int n1=0; n1<nnPerEdge-3; n1++ )
	      {
		for ( int n2=0; n2<(nnPerEdge-3-n1); n2++ )
		  {
		    for ( int aa=0; aa<rDim; aa++ )
		      newNodes(nVert+nnPerEdgeToAdd*nEdge + f*nnPerFaceToAdd + n,aa) = pointsToProject(n,aa);

		    n++;
		  }
	      }
	    //	    mp.getRealArray(MappingProjectionParameters::r) = -1;
	    mp.reset();
	  }
	assert(n==nnPerFaceToAdd);
      }

    //     for ( int e=0; e<nElem; e++ )
    //       {
    // 	int xi=0,eta=0,zeta=0;
    // 	int nl = nodeLocation(nnPerEdge,xi,eta,zeta);
    // 	newElements(e,nl) = elems(e,0);
	
// 	xi=nnPerEdge-1;
// 	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
// 	newElements(e,nl) = elems(e,1);
	
// 	xi=0; eta=nnPerEdge-1;
// 	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
// 	newElements(e,nl) = elems(e,2);
	
// 	eta=0; zeta=nnPerEdge-1;
// 	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
// 	newElements(e,nl) = elems(e,3);
	
// 	//	newElements.display();
//       }

    int bface = 0;
    for ( int e=0; e<nElem ; e++ )
      {
	int xi=0,eta=0,zeta=0;
	int nl = nodeLocation(nnPerEdge,xi,eta,zeta);
	newElements(e,nl) = elems(e,0);
	
	xi=nnPerEdge-1;
	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
	newElements(e,nl) = elems(e,1);
	
	xi=0; eta=nnPerEdge-1;
	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
	newElements(e,nl) = elems(e,2);
	
	eta=0; zeta=nnPerEdge-1;
	nl = nodeLocation(nnPerEdge,xi,eta,zeta);
	newElements(e,nl) = elems(e,3);

	int v1=elems(e,0);
	int v2=elems(e,1);
	int v3=elems(e,2);
	int v4=elems(e,3);
	
	int n=0;
	int eOffset = nVert+nnPerEdgeToAdd*nEdge + nnPerFaceToAdd*nFace;
	for ( int n1=0; n1<nnPerEdge-4; n1++ )
	  {
	    real a=(n1+1)*edgeDS;
	    for ( int n2=0; n2<(nnPerEdge-4-n1); n2++ )
	      {
		real b=(n2+1)*edgeDS;
		for ( int n3=0; n3<(nnPerEdge-4-n2-n1); n3++ )
		  {
		    real c = (n3+1)*edgeDS;
		    real d = 1.-a-b-c;
		    int nnid = eOffset+nnPerElemToAdd*e+n;
		    for ( int aa=0; aa<rDim; aa++ )
		      newNodes(nnid,aa) = c*nodes(v1,aa) + b*nodes(v2,aa) + a*nodes(v3,aa) + d*nodes(v4,aa);

		    int nl = nodeLocation(nnPerEdge,n3+1,n2+1,n1+1); 
		    newElements(e,nl) = nnid;
		    n++;
		  }
	      }
	  }

	UnstructuredMappingAdjacencyIterator aiter,aiter_end;

	aiter_end = umap.adjacency_end(UnstructuredMapping::Region, e, UnstructuredMapping::Edge);
	aiter = umap.adjacency_begin(UnstructuredMapping::Region, e, UnstructuredMapping::Edge);
	int en=0;
	for ( ; aiter!=aiter_end; aiter++ )
	  {
	    for ( int n=0; n<nnPerEdgeToAdd; n++ )
	      {
		int nle=-1;
		if ( aiter.orientation()>0 )
		  nle = n;
		else
		  nle = (nnPerEdgeToAdd-1-n);

// 		cout<<"edge verts "<<edges(*aiter,0)<<"  "<<edges(*aiter,1)<<endl;
// 		cout<<"reg edge verts "<<elems(e, topo2EdgeVert[UnstructuredMapping::tetrahedron][en][0])<<"  "<<elems(e, topo2EdgeVert[UnstructuredMapping::tetrahedron][en][1])<<"  "<<aiter.orientation()<<endl;
		int off = aiter.orientation();
		int nnid = nVert + nnPerEdgeToAdd*(*aiter)+nle;
#if 0
		int xi = HOEdgeMap(en,0,0)*(nnPerEdge-1) + (nle+off)*HOEdgeMap(en,1,0);
		int eta = HOEdgeMap(en,0,1)*(nnPerEdge-1) + (nle+off)*HOEdgeMap(en,1,1);
		int zeta = HOEdgeMap(en,0,2)*(nnPerEdge-1) + (nle+off)*HOEdgeMap(en,1,2);
#else
		int xi = HOEdgeMap(en,0,0)*(nnPerEdge-1) + (n+1)*HOEdgeMap(en,1,0);
		int eta = HOEdgeMap(en,0,1)*(nnPerEdge-1) + (n+1)*HOEdgeMap(en,1,1);
		int zeta = HOEdgeMap(en,0,2)*(nnPerEdge-1) + (n+1)*HOEdgeMap(en,1,2);
#endif
		int nl = nodeLocation(nnPerEdge,xi,eta,zeta);
		//		cout<<e<<" nle = "<<nle<<"  nl= "<<nl<<"  nnid = "<<nnid<<"  xi,eta,zeta = "<<xi<<"  "<<eta<<"  "<<zeta<<endl;
		newElements(e,nl) = nnid;

	      }
	    en++;
	  }

	aiter_end = umap.adjacency_end(UnstructuredMapping::Region, e, UnstructuredMapping::Face);
	aiter = umap.adjacency_begin(UnstructuredMapping::Region, e, UnstructuredMapping::Face);
	int fn=0;
	ArraySimple<int> fnodes(nnPerFaceToAdd);
	for ( ; aiter!=aiter_end; aiter++ )
	  {
	    fnodes=-1;
	    int f = *aiter;
	    int v1=faces(f,0), v2=faces(f,1), v3=faces(f,2);
	    int n=0;
	    //	    cout<<"face verts "<<v1<<"  "<<v2<<"  "<<v3<<endl;
	    for ( int n1=0; n1<nnPerEdge-3; n1++ )
	      {
		for ( int n2=0; n2<(nnPerEdge-3-n1); n2++ )
		  {
		    fnodes(n) = nVert+nnPerEdgeToAdd*nEdge + *aiter*nnPerFaceToAdd + n;
		    n++;
		  }
	      }

	    int rShift = v1==elems(e,topo2FaceVert[UnstructuredMapping::tetrahedron][fn][0]) ? 0 :
	      ( v1==elems(e,topo2FaceVert[UnstructuredMapping::tetrahedron][fn][1]) ? 1 : 2);

	    if ( rShift && !(aiter.orientation()>0) )
	      rShift = rShift==2 ? 1 : 2;

	    int ix0,ix1,ia00,ia01,ia10,ia11;
	    if ( rShift==1 && (aiter.orientation()>0) )
	      {
		ix0=0;
		ix1=nnPerEdge-1;
		ia00=0;
		ia01=1;
		ia10=-1;
		ia11=-1;
	      }
	    else if ( rShift==1 )
	      {
		ix0=0;
		ix1=nnPerEdge-1;
		ia00=1;
		ia01=0;
		ia10=-1;
		ia11=-1;
	      }
	    else if ( rShift==2 && (aiter.orientation()>0) )
	      {
		ix0=nnPerEdge-1;
		ix1=0;
		ia00=-1;
		ia01=-1;
		ia10=1;
		ia11=0;
	      }
	    else if ( rShift==2 )
	      {
		ix0=nnPerEdge-1;
		ix1=0;
		ia00=-1;
		ia01=-1;
		ia10=0;
		ia11=1;
	      }
	    else if ( !(aiter.orientation()>0) )
	      {  
		ix0=0;
		ix1=0;
		ia00=0;
		ia01=1;
		ia10=1;
		ia11=0;
	      }
	    else
	      {
		ix0=ix1=ia01=ia10 = 0;
		ia00 = ia11 = 1;
	      }

	    //	    cout<<"ix0, ix1 "<<ix0<<"  "<<ix1<<endl;

	    n=0;
	    for ( int n1=0; n1<nnPerEdge-3 ; n1++ )
	      {
		for ( int n2=0; n2<(nnPerEdge-3-n1); n2++ )
		  {

		    int r1 = ix0 + (n2+1)*ia00 + (n1+1)*ia01;
		    int r2 = ix1 + (n2+1)*ia10 + (n1+1)*ia11;

		    int nidx = fnodes(n);
		    
		    int xi = HOFaceMap(fn,0,0)*(nnPerEdge-1) + HOFaceMap(fn,1,0)*r1 + HOFaceMap(fn,2,0)*r2;
		    int eta = HOFaceMap(fn,0,1)*(nnPerEdge-1) + HOFaceMap(fn,1,1)*r1 + HOFaceMap(fn,2,1)*r2;
		    int zeta = HOFaceMap(fn,0,2)*(nnPerEdge-1) + HOFaceMap(fn,1,2)*r1 + HOFaceMap(fn,2,2)*r2;

		    int nl = nodeLocation(nnPerEdge,xi,eta,zeta); 
		    //		    cout<<"face : "<<f<<"  "<<nl<<"  "<<xi<<"  "<<eta<<"  "<<zeta<<endl;
		    newElements(e,nl) = nidx;


		    n++;
		  }
	      }

	    string bdyTag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[(UnstructuredMapping::Face)].c_str();
	    if ( umap.hasTag(UnstructuredMapping::Face, *aiter, bdyTag) )
	      {
		int n=0;
		for ( int n1=0; n1<nnPerEdge ; n1++ )
		  {
		    for ( int n2=0; n2<(nnPerEdge-n1); n2++ )
		      {
			
			int r1 = ix0 + (n2)*ia00 + (n1)*ia01;
			int r2 = ix1 + (n2)*ia10 + (n1)*ia11;
			
			
			int xi = HOFaceMap(fn,0,0)*(nnPerEdge-1) + HOFaceMap(fn,1,0)*r1 + HOFaceMap(fn,2,0)*r2;
			int eta = HOFaceMap(fn,0,1)*(nnPerEdge-1) + HOFaceMap(fn,1,1)*r1 + HOFaceMap(fn,2,1)*r2;
			int zeta = HOFaceMap(fn,0,2)*(nnPerEdge-1) + HOFaceMap(fn,1,2)*r1 + HOFaceMap(fn,2,2)*r2;
			
			int nl = nodeLocation(nnPerEdge,xi,eta,zeta); 
			//			cout<<"face : "<<f<<"  "<<nl<<"  "<<xi<<"  "<<eta<<"  "<<zeta<<endl;

			int nidx = newElements(e,nl);
			bdyFaces(bface,n) = nidx;
			
			if ( umap.hasTag(UnstructuredMapping::Face,*aiter,"surface") )
			  {
			    // *wdh* 100421 int surf = (int)umap.getTagData(UnstructuredMapping::Face,*aiter,"surface");
			    int surf = (intptr_t)umap.getTagData(UnstructuredMapping::Face,*aiter,"surface");
			    bdyFaceSurf(bface) = surf;
			  }

			n++;
		      }
		  }
		bface++;
	      }
	    
	    fn++;
	  }

	assert(n==nnPerElemToAdd);

	// now interpolate new vertices inside the volume using tfi on levels of zeta
	int ntest=0;
	//	ntest = nnPerElemToAdd;
	for ( zeta=1; zeta<nnPerEdge-2 /*&& false*/; zeta++ )
	  {
	    int v1 = newElements(e,nodeLocation(nnPerEdge,0,0,zeta));
	    int v2 = newElements(e,nodeLocation(nnPerEdge,nnPerEdge-1-zeta,0,zeta));
	    int v3 = newElements(e,nodeLocation(nnPerEdge,0,nnPerEdge-1-zeta,zeta));

	    for ( eta=1; eta<nnPerEdge-1-zeta; eta++ )
	      {
		real edgeDS = 1./real(nnPerEdge-zeta-1);
		real A=eta*edgeDS;

		real xl[3];
		real xr[3];

		int nright = newElements(e,nodeLocation(nnPerEdge,nnPerEdge-1-zeta-eta,eta,zeta));
		int nleft = newElements(e,nodeLocation(nnPerEdge,0,eta,zeta));

		for ( int a=0; a<3; a++ )
		  {
		    xl[a] = newNodes(nleft,a);
		    xr[a] = newNodes(nright,a);
		  }

		for ( xi=1; xi<nnPerEdge-1-zeta-eta; xi++ )
		  {
		    real B = real(xi)/real(nnPerEdge-eta-zeta-1);

		    int nb2use = int(B*(nnPerEdge-zeta-1));

		    real bc = 1-(B-(nb2use)*edgeDS)/edgeDS;

		    real xb[3];
		    real xt[3];
		    int nbot = newElements(e,nodeLocation(nnPerEdge,nb2use, 0,zeta));
		    int nbotp1 = newElements(e,nodeLocation(nnPerEdge,nb2use+1, 0,zeta));
		    int ntop = newElements(e,nodeLocation(nnPerEdge,0, nnPerEdge-1-zeta,zeta));
		    //		    cout<<nodeLocation(nnPerEdge,nb2use, 0,zeta)<<"  "<<nodeLocation(nnPerEdge,nb2use+1, 0,zeta)<<"  "<<nodeLocation(nnPerEdge,0, nnPerEdge-1-zeta,zeta)<<endl;
// 		    cout<<"nleft, nright "<<nleft<<"  "<<nright<<endl;
// 		    cout<<"vol bc, nbot, nbotp1, ntop "<<nb2use<<"   "<<bc<<"  "<<nbot<<"  "<<nbotp1<<"  "<<ntop<<endl;
// 		    cout<<"A, B "<<A<<"  "<<B<<endl;
		    //		    int off=1;
		    for ( int a=0; a<3; a++ )
		      {
			xb[a] = bc*newNodes(nbot,a) + (1-bc)*newNodes(nbotp1,a);
			xt[a] = newNodes(ntop,a);
		      }

		    int nl = newElements(e,nodeLocation(nnPerEdge,xi,eta,zeta));

		    for ( int aa=0; aa<3 ; aa++ )
		      {
			real xn1 = (1-B)*xl[aa] + B*xr[aa];
			real xn2 = (1.-A)*xb[aa] + A*xt[aa];
			real xtp = (1-B)*( (1-A)*newNodes(v1,aa)+A*newNodes(v3,aa) ) + B*( (1-A)*newNodes(v2,aa) + A*newNodes(v3,aa) );
			newNodes(nl,aa) = xn1 + xn2 - xtp;
// 			cout<<xl[aa]<<"  "<<xr[aa]<<"  "<<xb[aa]<<"  "<<xt[aa]<<endl;
// 			cout<<xn1<<"  "<<xn2<<"  "<<xtp<<endl;
		      }
		    ntest++; 
		  }
	      }
	  }
	assert(ntest==nnPerElemToAdd);
      }

    return true;
  }

  void assignBoundaryConditions( CompositeSurface &cs, GenericGraphicsInterface &gi )
  {
    GraphicsParameters gp;
    gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    gp.set(GI_PLOT_LINES_ON_MAPPING_BOUNDARIES, FALSE);

    GUIState gui;

    gui.setWindowTitle("Assign Boundary Conditions");
    gui.setExitCommand("Exit","Exit");

    enum txtEnum {
      defbc=0,
      curbc,
      nTXT
    };

    int defBC = 0;
    int curBC = defBC;
    aString txtCmd[] = { "default bc","current bc", "" };
    aString txtData[] = {"","",""};
    sPrintF(txtData[0], "%i", defBC);
    sPrintF(txtData[1], "%i", curBC);
    gui.setTextBoxes(txtCmd,txtCmd,txtData);

    enum pbEnum {
      pbSetDef,
      nPB
    };
    aString pbCmd[] = { "Reset to Default",""};
    gui.setPushButtons(pbCmd,pbCmd);

    aString answer="";
    SelectionInfo select;
    int len;
    
    gi.pushGUI(gui);
    
    bool plot=true;
    for (    int i=0 ;; i++)
      {
	if ( i>0 )
	  {
	    gi.savePickCommands(false);
	    gi.getAnswer(answer,"",select);
	    gi.savePickCommands(true);
	  }
	len=0;

	if ( answer=="Exit" )
	  break;
	else if ( select.nSelect )
	  {
	    plot = false;
	    for( int i=0; i<select.nSelect && !plot; i++ )
	      {
		for( int s=0; s<cs.numberOfSubSurfaces() && !plot; s++ )
		  {
		    if( select.selection(i,0) == cs[s].getGlobalID() )
		      {
			cs.setColour(s,gi.getColourName(curBC));
			aString cmd;
			sPrintF(cmd, "set bc surf %i %i\n", s, curBC);
			gi.outputToCommandFile(cmd);
			plot = true;
		      }
		  }
	      }
	  }
	else if ( (len=answer.matches("set bc surf") ) )
	  {
	    int s=-1,bc=0;
	    sScanF(answer(len, answer.length()-1),"%i %i",&s,&bc);
	    if ( s<0 || s>cs.numberOfSubSurfaces() )
	      gi.createMessageDialog("ERROR : invalid surface id !", errorDialog);
	    else
	      {
		cs.setColour(s,gi.getColourName(bc));
		plot = true;
	      }
	  }
	else if ( gui.getTextValue(answer,"default bc","%i",defBC) ){}//
	else if ( gui.getTextValue(answer,"current bc","%i",curBC) ){}//
	else if ( answer==pbCmd[pbSetDef] )
	  {
	    for( int s=0; s<cs.numberOfSubSurfaces(); s++ )
	      cs.setColour(s,gi.getColourName(defBC));
	    plot = true;
	  }

	if ( plot )
	  {
	    cs.eraseCompositeSurface(gi);
	    PlotIt::plot(gi,cs,gp);
	    plot = false;
	  }
      }

    gi.popGUI();
  }

}

int main(int argc, char *argv[])
{

  aString commandFileName="";
  bool plotOption=true;
  if( argc > 0 && argc<4 )
  { // look at arguments for "noplot" or some other name
    aString line;
    for( int i=1; i<argc; i++ )
    {
      line=argv[i];
      if( line.matches("noplot") )
        plotOption=false;
      else if( commandFileName=="" )
        commandFileName=line;    
    }
  }
  else
    cout << "Usage: `"<<argv[0]<<" [noplot][file.cmd]' \n"
      "          noplot:   run without graphics \n" 
      "          file.cmd: read this command file \n";

  GenericGraphicsInterface & gi = 
    *Overture::getGraphicsInterface("The higher the better",plotOption);

  GraphicsParameters gp;

  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT, TRUE);
  gp.set(GI_PLOT_UNS_BOUNDARY_EDGES,TRUE);
  gp.set(GI_PLOT_UNS_EDGES,TRUE);
  if ( commandFileName.length() )
    gi.readCommandFile(commandFileName);

  gi.saveEchoFile("ov_hot.log");
  gi.saveCommandFile("ov_hot.cmd");
  
  MappingInformation mapInfo; mapInfo.graphXInterface = &gi; mapInfo.gp_ = &gp;
  CompositeSurface model,deletedSurfaces;
  ListOfMappingRC curveList;
  PointList points;

  AdvancingFront advFront;
  AdvancingFrontParameters &advFrontParameters = advFront.getParameters();
  UnstructuredMapping umap;

  realArray HOnodes;
  intArray HOelems, HObdyFaces, HObdyFaceSurf;

  BoxMapping bgmap;
  bgmap.setGridDimensions(0,3);
  bgmap.setGridDimensions(1,3);
  bgmap.setGridDimensions(2,3);
  MappedGrid bggrid(bgmap);
  CompositeGrid cg;
  cg.add(bggrid);
  Range all;
  RealCompositeGridFunction strFun;
  strFun.updateToMatchGrid(cg,all,all,all,3,3);
  strFun=0;
  Index I1,I2,I3;
  getIndex(cg[0].gridIndexRange(),I1,I2,I3);
  cg[0].update(MappedGrid::THEvertex);

  for ( int a=0; a<3; a++ )
    strFun[0](all,all,all,a,a) = 1.0;

  advFront.setControlFunction(cg,strFun);
    
  GUIState gui;
  
  gui.setWindowTitle("3 cent mesh generator");
  gui.setExitCommand("exit","exit");

  enum pd0Enum {
    importIges,
    saveMesh,
    saveSilo,
    saveCG,
    nPD
  };

  aString pdCmd0[] = { "Import Iges", "Save Mesh","Save Silo", "Save CG","" };
  gui.addPulldownMenu("File",pdCmd0,pdCmd0, GI_PUSHBUTTON);
  PullDownMenu &fileMenu  = gui.getPulldownMenu(0);

  enum pd1Enum {
    topoSettings,
    ugenSettings,
    nST
  };

  aString pdCmd1[] = { "Topology", "Volume Mesher","Plotting","" };
  gui.addPulldownMenu("Settings",pdCmd1,pdCmd1,GI_PUSHBUTTON);
  PullDownMenu &settingsMenu = gui.getPulldownMenu(1);
  
  enum pbEnum {
    simpleGeometry,
    editModel,
    generateMesh,
    optimizeMesh,
    boundaryConditions,
    verifyMesh,
    nPB
  };

  aString pbCmd[] = { "Simple Geometry", "Edit Model", "Generate Mesh", "Optimize Mesh",
		      "Boundary Conditions","Verify Mesh","" };
  gui.setPushButtons(pbCmd,pbCmd,nPB/2);

  DialogData & advFrontDialog = gui.getDialogSibling();
  advFrontDialog.setWindowTitle("AdvancingFront Parameters");
  setupAdvFrontDialog(advFrontParameters, advFrontDialog);
  advFrontDialog.setExitCommand("close advancing front dialog", "Close");

  enum txtEnum {
    gridSpacing,
    elementOrder,
    nTXT
  };

  real el=.1; // edge length
  int order = 1; // element order
  aString txtCmd[] = { "Grid Spacing", "Element Order", "" };
  aString txtData[] = { "", "", "" };
  sPrintF(txtData[gridSpacing],"%g",el);
  sPrintF(txtData[elementOrder],"%i",order);
  gui.setTextBoxes(txtCmd,txtCmd,txtData);

  // *wdh*
  ModelBuilder modelBuilder;

  int len;
  aString answer;
  
  gi.pushGUI(gui);

  bool plotAndExit = true;

  for (;;)
    {
      gi.getAnswer(answer,"");

      gui.setSensitive(umap.size(UnstructuredMapping::Region), 
		       DialogData::pushButtonWidget, (int)verifyMesh);

      if ( answer=="exit" )
	break;
      else if ( answer==pbCmd[verifyMesh] )
	{
	  verifyUnstructuredConnectivity(umap,true);
	}
      else if ( answer==pdCmd0[saveCG] )
	{
	  if ( umap.size(UnstructuredMapping::Region) )
	    {
	      CompositeGrid cg;
	      cg.add(umap);

	      aString fileName;
	      gi.inputFileName(fileName,"",".hdf");

	      if ( fileName!="" )
		{
		  HDF_DataBase dataFile;
		  dataFile.mount(fileName,"I");
		  
		  // The following code is nicked from ogen (thanks Bill!)
		  int streamMode=0;
		  if( answer=="save a grid (compressed)" )
		    streamMode=1;  // save in compressed form.
		  else
		    streamMode=0;  // save in uncompressed form.
		  
		  dataFile.put(streamMode,"streamMode");
		  if( !streamMode )
		    dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
		  else
		    {
		      dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
		    }

		  aString gridName = "mesh";
		  cg.put(dataFile,gridName);
		  
		  dataFile.unmount();
		}
	    }
	}
      else if ( answer==pdCmd0[importIges] )
	{
	  // rapNewModel(gi,mapInfo,model);
          modelBuilder.newModel(gi,mapInfo,model);

	  el = getDefaultEdgeLength(model);
	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);

	  if ( !initializeModelAndFront( model, advFront, bgmap, bggrid, cg,strFun, el, gi ) )
	       gi.createMessageDialog("Cannot compute surface triangulation! Try altering the geometry or the topology settings",errorDialog);

	}
      else if ( answer=="Edit Model" )
	{
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	  gi.erase();
	  PlotIt::plot(gi,model,gp);
	  // *wdh* rapEditModel(mapInfo,model,deletedSurfaces,curveList,points);
          modelBuilder.editModel(mapInfo,model,deletedSurfaces,curveList,points);
	  el = getDefaultEdgeLength(model);
	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);
	  if ( !initializeModelAndFront( model, advFront, bgmap, bggrid, cg,strFun, el,gi ) )
	       gi.createMessageDialog("Cannot compute surface triangulation! Try altering the geometry or the topology settings",errorDialog);
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	}
      else if ( answer=="Simple Geometry" )
	{
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

	  gi.erase();
	  PlotIt::plot(gi,model,gp);
	  // *wdh* rapSimpleGeometry(mapInfo,model,curveList,points);
          modelBuilder.simpleGeometry(mapInfo,model,curveList,points);

	  el = getDefaultEdgeLength(model);
	  sPrintF(txtData[gridSpacing],"%g",el);
	  gui.setTextLabel(gridSpacing,txtData[gridSpacing]);
	  if ( !initializeModelAndFront( model, advFront, bgmap, bggrid, cg,strFun, el,gi ) )
	       gi.createMessageDialog("Cannot compute surface triangulation! Try altering the geometry or the topology settings",errorDialog);
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	}
      else if ( answer=="Boundary Conditions" )
	{
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

	  gi.erase();
	  PlotIt::plot(gi,model,gp);

	  assignBoundaryConditions(model,gi);

	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	}
      else if ( answer=="Optimize Mesh" )
	{
	  cout<<"optimizing..."<<endl;
	  optimize(umap, (RealCompositeGridFunction *)&advFront.getControlFunction());
	  Range R(umap.size(UnstructuredMapping::Vertex));
	  Range A(umap.getRangeDimension());
	  ((realArray &)advFront.getVertices())(R,A) = (realArray &)umap.getNodes();  // reset the adv front vertices to the opt locations
	  cout<<" done!"<<endl;
	  MeshQualityMetrics mq(umap);
	  MetricCGFunctionEvaluator metricEval((RealCompositeGridFunction *)&advFront.getControlFunction());
	  //	mq.setReferenceTransformation((RealCompositeGridFunction *)&advancingFront.getControlFunction());
	  mq.setReferenceTransformation(&metricEval);
	  
	  cout<<"computing quality histogram ... "<<endl;
	  mq.outputHistogram();
	}
      else if ( answer=="Save Mesh" )
	{
	  if ( umap.size(UnstructuredMapping::Region) )
	    {
	      aString fileName;
	      gi.inputFileName(fileName,"",".xyz");

	      if (order)
		addHONodes( order+1, umap, model, HOnodes, HOelems, HObdyFaces,HObdyFaceSurf );

	      if ( !writeMesh(model,fileName,umap, order, HOnodes, HOelems, HObdyFaces, HObdyFaceSurf) )
		gi.createMessageDialog("ERROR : could not write to file "+fileName,errorDialog);
	    }
	  else
	    gi.createMessageDialog("ERROR : a mesh must be generated first!",errorDialog);
	}
      else if ( answer=="Save Silo" )
	{
	  if ( umap.size(UnstructuredMapping::Region) )
	    {
	      aString fileName;
	      gi.inputFileName(fileName,"",".silo");

	      if (order)
		addHONodes( order+1, umap, model, HOnodes, HOelems, HObdyFaces,HObdyFaceSurf );
	      if ( !writeSilo(model,fileName,umap, order, HOnodes, HOelems, HObdyFaces, HObdyFaceSurf) )
		gi.createMessageDialog("ERROR : could not write to file "+fileName,errorDialog);
	    }
	  else
	    gi.createMessageDialog("ERROR : a mesh must be generated first!",errorDialog);
	}
      else if ( answer=="Plotting" )
	{
	  plotAndExit = false;
	}
      else if ( (len=answer.matches("number of advances")) )
	{
	  int nsteps = -1;
	  sScanF(answer(len,answer.length()-1),"%i",&nsteps);
	  advFrontParameters.setNumberOfAdvances(nsteps);
	}
      else if ( (len=answer.matches("quality tolerance")) )
	{
	  real qual = advFrontParameters.getQualityTolerance();;
	  sScanF(answer(len,answer.length()-1),"%g",&qual);
	  advFrontParameters.setQualityTolerance(qual);
	}
      else if ( answer==pbCmd[generateMesh] )
	{
	  int nsteps = advFrontParameters.getNumberOfAdvances();
	  
	  try 
	    {
	      cout<<"nsteps = "<<nsteps<<endl;
	      int nlimit = 0;
	      bool stopIt = false;
	      real initialQT = advFrontParameters.getQualityTolerance();
	      bool firstGo = true;

	      while ( (!advFront.isFrontEmpty() && !stopIt) || firstGo )
		{
		  firstGo = false;
		  nlimit += advFront.advanceFront(nsteps);
		  gi.erase();
		  PlotIt::plot(gi,advFront,gp);
		  gi.redraw(TRUE);
		  cout<<"   "<<advFront.getNumberOfElements()<<"  "<<nlimit<<endl;

		  stopIt = nlimit>10;

		  if ( nlimit>5 && !advFront.isFrontEmpty() && !stopIt)
		    {
		      advFrontParameters.setQualityTolerance(advFrontParameters.getQualityTolerance()*.75);
		    }
		  
		}

	      advFrontParameters.setQualityTolerance( initialQT );
	      if ( advFront.isFrontEmpty() )
		{
		  //		  umap.specifyVertices(advFront.getVertices());
		  //		  umap.specifyEntity(UnstructuredMapping::Region, advFront.generateElementList());
		  const intArray &elems = advFront.generateElementList();
		  const realArray &nodes = advFront.getVertices();
		  Range R(advFront.getNumberOfVertices());
		  Range A(umap.getRangeDimension());
		  umap.setNodesAndConnectivity(nodes(R,A),elems,3);

		  // now tag boundary faces with thier original surface ID
		  tagFacesWithSurfaceID(advFront, model, umap);

		  // XXX need to tag boundary vertices so that we compute boundary tags correctly

 		  MeshQualityMetrics mq(umap);
 		  MetricCGFunctionEvaluator metricEval((RealCompositeGridFunction *)&advFront.getControlFunction());
		  // 		  	mq.setReferenceTransformation((RealCompositeGridFunction *)&advancingFront.getControlFunction());
 		  mq.setReferenceTransformation(&metricEval);
	  
 		  mq.outputHistogram();

		}
	    } 
	  catch (AdvancingFrontError &e)
	    {
	      e.debug_print();
	    }
	}
      else if ( answer==pdCmd1[topoSettings] )
	{
	  model.updateTopology();
	  //	  if ( !initializeModelAndFront( model, advFront, bgmap, bggrid, cg,strFun, el, gi, false ) )
	  //	       gi.createMessageDialog("Cannot compute surface triangulation! Try altering the geometry or the topology settings",errorDialog);

	  advFront.initialize((intArray &)model.getCompositeTopology(true)->getTriangulation()->getEntities(UnstructuredMapping::Face),
	  			      (realArray&)model.getCompositeTopology(true)->getTriangulation()->getNodes());
	}
      else if ( answer==pdCmd1[ugenSettings] )
	{
	  advFrontDialog.showSibling();
	}
      else if ( answer=="close advancing front dialog" )
	{
	  advFrontDialog.hideSibling();
	}
      else if ( ( len=answer.matches(txtCmd[gridSpacing])) )
	{
	  real s;
	  sScanF(answer(len,answer.length()-1), "%g", &s);
	  if ( s>0 )
	    {
	      el = s;
	      sPrintF(txtData[gridSpacing],"%g",el);
	      gui.setTextLabel(gridSpacing,txtData[gridSpacing]);

	      if ( !initializeModelAndFront( model, advFront, bgmap, bggrid, cg,strFun, el, gi ) )
		gi.createMessageDialog("Cannot compute surface triangulation! Try altering the geometry or the topology settings",errorDialog);

// 	      model.getCompositeTopology(true)->setDeltaS(el);
// 	      model.getCompositeTopology(true)->setMaximumArea(.5*el*el);
// 	      if ( !model.computeTopology(gi) )
// 		gi.createMessageDialog("Cannot compute surface triangulation! Try altering fixing the geometry or the topology settings",errorDialog);
// 	      else
// 		{
// 		  verifyUnstructuredConnectivity(*model.getCompositeTopology(true)->getTriangulation(),true);
// 		  for ( int a=0; a<3; a++ )
// 		    strFun[0](all,all,all,a,a) = 1.0/el;
// 		  advFront.initialize((intArray &)model.getCompositeTopology(true)->getTriangulation()->getEntities(UnstructuredMapping::Face),
// 				      (realArray&)model.getCompositeTopology(true)->getTriangulation()->getNodes());
// 		}


	    }
	  else
	    gi.createMessageDialog("ERROR: mesh spacing must be greater than 0! ",errorDialog);
	}
      else if ( ( len=answer.matches(txtCmd[elementOrder])) )
	{
	  int s;
	  sScanF(answer(len,answer.length()-1), "%d", &s);
	  if ( s>0 )
	    {
	      order = s;
	      sPrintF(txtData[elementOrder],"%d",order);
	      gui.setTextLabel(elementOrder,txtData[elementOrder]);
	      realArray newNodes;
	      intArray elems,bdyface;
	      addHONodes( order+1, umap, model, HOnodes, HOelems, HObdyFaces,HObdyFaceSurf );

	      if ( order==1 && false)
		{
		  //		  HOelems.display();
		  UnstructuredMapping um2;
		  um2.setNodesAndConnectivity(HOnodes, HOelems,3);
		  //		  verifyUnstructuredConnectivity(um2,true);
		  //		  PlotIt::plot(gi,um2);
		}
	    }
	  else
	    gi.createMessageDialog("ERROR: element order must be greater than 0! ",errorDialog);
	}
      else if ( answer.matches("testtet") )
	{
	  realArray HOnodes;
	  intArray HOelems, HObdyFaces;

	  realArray xyz(4,3);
	  xyz(0,Range(3)) = xyz(3,Range(3)) = 0.;
	  xyz(3,2) = 1.;
	  xyz(1,0) = 1.;
	  xyz(1,1) = xyz(1,2) = 0.;
	  xyz(2,0) = xyz(2,2) = 0;
	  xyz(2,1) = 1.;

	  intArray elems(1,8);
	  elems = -1;

#if 1
	  elems(0,0) = 1;
	  elems(0,1) = 3;
	  elems(0,2) = 2;
	  elems(0,3) = 0;
#else
 	  elems(0,0) = 0;
 	  elems(0,1) = 1;
 	  elems(0,2) = 2;
 	  elems(0,3) = 3;
#endif  
	  UnstructuredMapping utet;
	  utet.setNodesAndConnectivity(xyz,elems,3);
	  verifyUnstructuredConnectivity(utet,true);
	  addHONodes(order+1, utet, model, HOnodes, HOelems, HObdyFaces,HObdyFaceSurf, false );
	  //	  HOnodes.display("HOnodes");
	  for ( int i=0; i<HOnodes.getLength(0); i++ )
	    cout<<i<<"  :  "<<HOnodes(i,0)<<"  "<<HOnodes(i,1)<<"  "<<HOnodes(i,2)<<endl;
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
	  PlotIt::plot(gi,utet,gp);
	  gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	  gi.plotPoints(HOnodes,gp);
	  HOelems.display("HOelems");
	  HObdyFaces.display("bdyFaces");
	}

      if ( plotAndExit )
	gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
      else
	gp.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);

      gi.erase();
      if ( advFront.getNumberOfFaces() && !advFront.isFrontEmpty() )
	PlotIt::plot(gi,advFront,gp);
      else if ( advFront.getNumberOfFaces() )
	PlotIt::plot(gi,umap,gp);
      else if ( model.isTopologyDetermined() )
	PlotIt::plot(gi,*model.getCompositeTopology(true)->getTriangulation(),gp);
      else
	PlotIt::plot(gi,model,gp);

      if ( HOnodes.getLength(0) )
	gi.plotPoints(HOnodes,gp);

      if ( !plotAndExit )  plotAndExit = true;
    }

  gi.popGUI();
}
