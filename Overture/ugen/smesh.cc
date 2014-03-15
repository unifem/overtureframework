//#define BOUNDS_CHECK
//#define OV_DEBUG
//
//#include <cmath>

#include "smesh.hh"
#include "cutcell.hh"

#include "DataPointMapping.h"
#include "MappingProjectionParameters.h"
#include "AdvancingFront.h"
#include "CompositeGridFunction.h"
#include "Geom.h"
#include "mathutil.h"

extern bool verifyUnstructuredConnectivity( UnstructuredMapping &umap, bool verbose );

using namespace std;

int Curve::idCounter = 0;
int Region::idCounter = 0;

namespace {

  bool isCurveClockwise( ArraySimple<real> &x )
  {
    real area=0.;
    int nN = x.size(0);

    for ( int n=0; n<nN-1; n++ )
      area += .5*(x(n,0)*x(n+1,1)-x(n+1,0)*x(n,1));
    
    return area<0.;
  }

  // see the end of the file for these functions, they are used
  //  in UnstructuredRegion::getMapping 
  void buildUmapFromCutout(intArray &initialFaces, realArray &xyz_in,
			   SquareMapping &square, real dx[], 
			   ArraySimple<int> &gridIDList,
			   UnstructuredMapping &umap);

  void buildUmap(intArray &initialFaces, realArray &xyz_in,
		 SquareMapping &square, real dx[], UnstructuredMapping &umap);

  // vertex id management data, used by
  ///  getVertexID();
  stack<int> availableIDs;
  int idCounter = 0;

}

int getVertexID() 
{
  if ( !availableIDs.empty() )
    {
      int val = availableIDs.top();
      availableIDs.pop();
      return val;
    }
  else
    return idCounter++;
}

void freeVertexID(int v)
{
  availableIDs.push(v);
}

void resetVertexIDs() { idCounter=0; while (!availableIDs.empty()) availableIDs.pop(); }

int currentIDCounter() { return idCounter;}

void 
Region::
specifyReferenceGridBounds(real x1, real y1, real x2, real y2)
{
  /// reset the reference grid to new bounds
  int nx = max(2,abs(rounder((x2-x1)/dxRef))+1);
  int ny = max(2,abs(rounder((y2-y1)/dyRef))+1);

  // there seem to be roundoff issues in NurbsMapping::circle
  //  clip at least zero to really be zero!
  if ( fabs(x1)<10*REAL_EPSILON ) x1=0.;
  if ( fabs(x2)<10*REAL_EPSILON ) x2=0.;
  if ( fabs(y1)<10*REAL_EPSILON ) y1=0.;
  if ( fabs(y2)<10*REAL_EPSILON ) y2=0.;

  //  cout<<"new grid bounds "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<endl;

  referenceGrid.setVertices(x1,x2,y1,y2);
  referenceGrid.setGridDimensions(0,nx);
  referenceGrid.setGridDimensions(1,ny);
  //  referenceGrid.reinitialize();
}

void 
Region::
resetReferenceGrid()
{
  real x1,x2,y1,y2;
  referenceGrid.getVertices(x1,x2,y1,y2);

  /// reset the reference grid to new bounds
  int nx = max(2,abs(int((x2-x1)/dxRef))+1);
  int ny = max(2,abs(int((y2-y1)/dyRef))+1);

  referenceGrid.setGridDimensions(0,nx);
  referenceGrid.setGridDimensions(1,ny);
  //  referenceGrid.reinitialize();
}

Mapping &
TFIRegion::
getMapping() 
{
  // generate the unstructured representation
  if ( tfiUpToDate() ) return tfi;

  if ( numberOfCurves()!=4 ) 
    throw "not enough curves";

  //collect up the 4 curves, put them into DataPointMappings
  DataPointMapping *boundingCurves[4];
  int c=0;
  for ( curve_iterator i=curve_begin(); i!=curve_end(); i++ )
    {
      ArraySimple<real> xyz;
      // get the curve discretization
      (*i)->discretize(dxRef, dyRef, xyz);

      // check the opposite curve, if grid sizes are mismatched 
      //   try to make them the same
      if ( (c==1||c==3) && (*i)->numberOfPoints()!=(*(i-1))->numberOfPoints() )
	{
	  Curve *ccp = *i;
	  SimpleCurve *sc = dynamic_cast<SimpleCurve*>(ccp);
	  if ( !(*i)->region_2() && sc && sc->autoGridSize() )
	    {
	      cout<<"WARNING: adjusting number of points on curve "<<ccp->ID()
		  <<" to "<<(*(i-1))->numberOfPoints()<<endl;
	      sc->setNumberOfPoints((*(i-1))->numberOfPoints());
	      sc->discretize(dxRef,dyRef,xyz);
	    }
	}
      else if ( (c==0||c==2) && 
		(*i)->numberOfPoints()!=(*(i+1))->numberOfPoints() )
	{
	  Curve *ccp = *i;
	  SimpleCurve *sc = dynamic_cast<SimpleCurve*>(ccp);
	  if ( !(*i)->region_2() && sc && sc->autoGridSize() )
	    {
	      cout<<"WARNING: adjusting number of points on curve "<<ccp->ID()<<endl;
	      sc->setNumberOfPoints((*(i+1))->numberOfPoints());
	      sc->discretize(dxRef,dyRef,xyz);
	    }
	}

      //            cout<<"xyz for curve "<<*i<<xyz<<endl;

      realArray xyz_a(xyz.size(0),2);
      for ( int p=0; p<xyz.size(0); p++ )
	{
	  xyz_a(p,0) = xyz(p,0);
	  xyz_a(p,1) = xyz(p,1);
	}

      boundingCurves[c] = new DataPointMapping();
      boundingCurves[c++]->setDataPoints(xyz_a, 1);
      
      // XXX ? perhaps the regions should be checked, the curve cannot be used
      //  more than twice
//        if ( (*i)->region_1() ) 
//  	(*i)->region_2(this);
//        else
//  	(*i)->region_1(this);
    }

  // now actually generate the TFIMapping
  tfi.setDomainDimension(2);
  tfi.setRangeDimension(2);
  tfi.setSides(boundingCurves[0],
	       boundingCurves[1],
	       boundingCurves[2],
	       boundingCurves[3]);

  tfi.setGridDimensions(1,boundingCurves[0]->getGridDimensions(0));
  tfi.setGridDimensions(0,boundingCurves[2]->getGridDimensions(0));

  //  EllipticTransform emap;
  //  emap.setUserGrid(tfi);
  //  umap.buildFromAMapping(tfi);
  //  optimize(umap,0);

  tfi_built = true;

//    for ( c=0; c<4; c++ )
//      if ( (boundingCurves[c]->decrementReferenceCount())==0 ) 
//        delete boundingCurves[c];

  return tfi;
}

static int determineSide(TFIMapping &tfi, ArraySimple<real> &curve)
{
  ArraySimpleFixed<real,2,1,1,1> x1,x2,x3,x4,xcs,xce;
  int nx=tfi.getGridDimensions(0);
  int ny=tfi.getGridDimensions(1);
  const realArray & xyz = tfi.getGrid();
  real ds[4],de[4];

  for ( int a=0; a<2; a++ )
    {
      x1[a] = xyz(0,0,0,a);
      x2[a] = xyz(nx-1,0,0,a);
      x3[a] = xyz(nx-1,ny-1,0,a);
      x4[a] = xyz(0,ny-1,0,a);
      xcs[a] = curve(0,a);
      xce[a] = curve(curve.size(0)-1,a);
      ds[a] = ds[a+2] = de[a] = de[a+2] = 0.;
    }

  for ( int a=0; a<2; a++ )
    {
      ds[0] += (x1[a]-xcs[a])*(x1[a]-xcs[a]);
      ds[1] += (x2[a]-xcs[a])*(x2[a]-xcs[a]);
      ds[2] += (x3[a]-xcs[a])*(x3[a]-xcs[a]);
      ds[3] += (x4[a]-xcs[a])*(x4[a]-xcs[a]);
      de[0] += (x1[a]-xce[a])*(x1[a]-xce[a]);
      de[1] += (x2[a]-xce[a])*(x2[a]-xce[a]);
      de[2] += (x3[a]-xce[a])*(x3[a]-xce[a]);
      de[3] += (x4[a]-xce[a])*(x4[a]-xce[a]);
    }

  enum {
    left=1,
    right,
    bottom,
    top
  };

  real tol = 10*REAL_EPSILON;
  if ( ds[0]<tol && de[1]<tol ) 
    return bottom;
  else if ( ds[1]<tol && de[0]<tol )
    return -bottom;
  else if ( ds[1]<tol && de[2]<tol )
    return right;
  else if ( ds[2]<tol && de[1]<tol )
    return -right;
  else if ( ds[3]<tol && de[2]<tol ) 
    return top;
  else if ( ds[2]<tol && de[3]<tol ) 
    return -top;
  else if ( ds[3]<tol && de[0]<tol )
    return -left;
  else if ( ds[0]<tol && de[3]<tol )
    return left;
  else
    abort();
}

ArraySimple<int> &
TFIRegion::
getGridIDList()
{
  if ( gridIDList.size() ) return gridIDList;

  if ( !tfi_built && numberOfCurves()==4 )
    getMapping();

  int nx=tfi.getGridDimensions(0);
  int ny=tfi.getGridDimensions(1);
  const realArray & xyz = tfi.getGrid();

  gridIDList = ArraySimple<int>(nx*ny);
  gridIDList = -1;

  // first fill in the four sides
  enum {
    left=1,
    right,
    bottom,
    top
  };

  for ( curve_iterator ci=curve_begin(); ci!=curve_end(); ci++ )
    { // check each curve for the side and orientation, TFIMapping
      //   may have munged these.
      ArraySimple<int> &ids = (*ci)->getGridIDList();
      ArraySimple<real> & grid = (*ci)->getVertices();
      int side = determineSide(tfi,grid);
      
      if ( abs(side)==left )
	{
	  if ( side>0 )
	    for ( int p=0; p<grid.size(0); p++ )
	      gridIDList(p) = ids(p);
	  else
	    {
	      for ( int p=grid.size(0)-1; p>=0; p-- )
		gridIDList(grid.size(0)-1-p) = ids(p);
	      //	      cout<<"left flipped "<<sum<<endl;
	    }
	}
      else if ( abs(side)==right )
	{
	  if ( side>0 ) 
	    for ( int p=0; p<grid.size(0); p++ )
	      gridIDList((nx-1)*ny+p) = ids(p);
	  else
	    {
	      for ( int p=0; p<grid.size(0); p++ )
		gridIDList((nx-1)*ny+p) = ids(grid.size(0)-1-p);
	      //	      cout<<"right flipped "<<sum<<endl;
	    }	
	}
      else if ( abs(side)==bottom )
	{
	  if ( side>0 ) 
	    for ( int p=1; p<grid.size(0)-1; p++ )
	      gridIDList(p*ny) = ids(p);
	  else
	    {
	      for ( int p=1; p<grid.size(0)-1; p++ )
		gridIDList(p*ny) = ids(grid.size(0)-1-p);
	      //	      cout<<"bottom flipped "<<sum<<endl;
	    }
	}
      else if ( abs(side)==top )
	{
	  if ( side>0 ) 
	    for ( int p=1; p<grid.size(0)-1; p++ )
	      gridIDList(p*ny+ny-1) = ids(p);
	  else
	    {
	      for ( int p=1; p<grid.size(0)-1; p++ )
		gridIDList(p*ny+ny-1) = ids(grid.size(0)-1-p);
	      //	      cout<<"top flipped "<<sum<<endl;
	    }
	}
    }

  for ( int i=1; i<nx-1; i++ )
    for ( int j=1; j<ny-1; j++ )
      gridIDList(i*ny+j) = getVertexID();

  return gridIDList;
}

ArraySimple<real> & 
TFIRegion::
getVertices()
{
  if ( !tfi_built && numberOfCurves()==4 )
    getMapping();

  const realArray &verts = tfi.getGrid();

  int nx=tfi.getGridDimensions(0);
  int ny=tfi.getGridDimensions(1);
  grid = ArraySimple<real>(nx*ny,2);
  for ( int i=0; i<nx; i++ )
    for ( int j=0; j<ny; j++ )
      for ( int a=0; a<2; a++ )
	grid(i*ny+j,a) = verts(i,j,0,a);

  return grid;
}

ArraySimpleFixed<int,4,1,1,1> 
TFIRegion::
getElement(int e)
{
  if ( !gridIDList.size() )
    getGridIDList();

  int nx=tfi.getGridDimensions(0);
  int ny=tfi.getGridDimensions(1);

  ArraySimpleFixed<int,4,1,1,1> out;
  out = -1;
  int i = e/(ny-1);
  int j = e-i*(ny-1);

  //  cout<<i<<" "<<j<<endl;
  //    cout<<"GRID ID LIST IN GET ELEMENT"<<gridIDList<<endl;
  out(0) = gridIDList(i*ny+j);
  out(1) = gridIDList((i+1)*ny+j);
  out(2) = gridIDList((i+1)*ny+j+1);
  out(3) = gridIDList(i*ny+j+1);
  //  cout<<out<<endl;

  if ( tfi.getSignForJacobian()<0 )
    { // reverse the element
      ArraySimpleFixed<int,4,1,1,1> tmp;
      tmp = out;
      for ( int i=0; i<4; i++ )
	out(3-i) = tmp(i);
    }

  return out;

}

int
TFIRegion::
numberOfVertices()
{
  if ( !tfi_built && numberOfCurves()==4 )
    getMapping();

  return tfi.getGridDimensions(0)*tfi.getGridDimensions(1);
}

int TFIRegion::
numberOfElements()
{
  if ( !tfi_built && numberOfCurves()==4 )
    getMapping();

  return (tfi.getGridDimensions(0)-1)*(tfi.getGridDimensions(1)-1);
}

ArraySimple<int> &
UnstructuredRegion::
getGridIDList()
{
  if ( gridIDList.size() ) return gridIDList;

  if ( !umap_built && numberOfCurves()>0 )
    getMapping();

  gridIDList = ArraySimple<int>(umap.getNumberOfNodes());
  gridIDList = -1;
  int idloc = 0;
  for ( curve_iterator ci=curve_begin(); ci!=curve_end(); ci++ )
    {
      ArraySimple<int> & curveIDs = (*ci)->getGridIDList();
      for ( int i=0; i<curveIDs.size(0)-1; i++ )
	gridIDList(idloc++) = curveIDs(i);
    }
  
  for ( ; idloc<umap.getNumberOfNodes(); idloc++ )
    gridIDList(idloc) = getVertexID();

  //    cout<<"GRID ID LIST FOR UREG "<<gridIDList<<endl;
  return gridIDList;
}

ArraySimple<real> & 
UnstructuredRegion::
getVertices()
{
  if ( !umap_built && numberOfCurves()>0 )
    getMapping();

  realArray nodes = umap.getNodes();
  grid = ArraySimple<real>(umap.getNumberOfNodes(),2);
  for ( int n=0; n<grid.size(0); n++ )
    for ( int a=0; a<2; a++ )
      grid(n,a) = nodes(n,a);

  return grid;
}

ArraySimpleFixed<int,4,1,1,1> 
UnstructuredRegion::
getElement(int e)
{
  if ( !gridIDList.size() )
    getGridIDList();

  ArraySimpleFixed<int,4,1,1,1> out;
  out = -1;

  for ( int v=0; v<umap.getNumberOfNodesThisElement(e); v++ )
    out(v) = gridIDList(umap.elementGlobalVertex(e,v));

  //  cout<<"ELEMENT "<<e<<" "<<out<<endl;
  return out;

}

int
UnstructuredRegion::
numberOfVertices()
{
  if ( !umap_built && numberOfCurves()>0 )
    getMapping();
  
  return umap.getNumberOfNodes();
}

int 
UnstructuredRegion::
numberOfElements()
{
  if ( !umap_built && numberOfCurves()>0 )
    getMapping();
  
  return umap.getNumberOfElements();
}

Mapping &
UnstructuredRegion::
getMapping()
{
  if ( umapUpToDate() ) return umap;

  real dx[] = {dxRef, dyRef};

  // the first curve is the outer bounding curve
  Curve *outer = *curve_begin();
  ArraySimple<real> xyz;
  outer->discretize(dxRef,dyRef,xyz);

  //  cout<<"vertices for outer curve "<<xyz<<endl;
  bool clockwise = isCurveClockwise(xyz);

  intArray initialFaces(xyz.size(0)-1,2);
  realArray xyz_in(xyz.size(0)-1,2);
  int f;
  
  for ( f=0; f<xyz.size(0)-2; f++ )
    {
      initialFaces(f,0) = f;
      initialFaces(f,1) = f+1;
      for ( int a=0; a<2; a++ )
	xyz_in(f,a) = xyz(f,a);
    }
  initialFaces(f,0) = f;
  initialFaces(f,1) = 0;
  for ( int a=0; a<2; a++ )
    xyz_in(f,a) = xyz(f,a);

  if ( clockwise ) //reverse faces
    {
      intArray tmp;
      tmp = initialFaces;
      Range FACE(xyz.size(0)-1);
      //        initialFaces(FACE,0) = tmp(FACE,1);
      //         initialFaces(FACE,1) = tmp(FACE,0);
//        intArray tmp;
        tmp = initialFaces;
      //	  Range FACE(oldNF,xyz.size(0)-1);
	for ( int f=0; f<xyz.size(0)-1; f++ )
	  {
	    initialFaces(xyz.size(0)-2-f,0) = tmp(f,1);
	    initialFaces(xyz.size(0)-2-f,1) = tmp(f,0);
	  }
    }
  //  initialFaces.display("init");

  // now add any inner curves
  for ( curve_iterator c=curve_begin()+1; c!=curve_end(); c++ )
    {
      //XXX ! should check intersections with other curves!
      (*c)->discretize(dxRef,dyRef,xyz);
      
      //  cout<<"vertices for outer curve "<<xyz<<endl;
      bool clockwise = isCurveClockwise(xyz);
      
      //      intArray initialFaces(xyz.size(0)-1,2);
      //      realArray xyz_in(xyz.size(0)-1,2);
      int oldNF = initialFaces.getLength(0);
      initialFaces.resize(initialFaces.getLength(0)+xyz.size(0)-1,2);
      xyz_in.resize(xyz_in.getLength(0)+xyz.size(0)-1,2);

      int f=oldNF;
      int fi;
      for ( fi=0; fi<xyz.size(0)-2; fi++, f++ )
	{
	  initialFaces(f,0) = f;
	  initialFaces(f,1) = f+1;
	  for ( int a=0; a<2; a++ )
	    xyz_in(f,a) = xyz(fi,a);
	}
      initialFaces(f,0) = f;
      initialFaces(f,1) = oldNF;
      for ( int a=0; a<2; a++ )
	xyz_in(f,a) = xyz(fi,a);
      
      if ( !clockwise ) //reverse faces
	{
	  intArray tmp;
	  tmp = initialFaces;
	  Range FACE(oldNF,xyz_in.getLength(0)-1);
	  //initialFaces(FACE,0) = tmp(FACE,1);
	  //initialFaces(FACE,1) = tmp(FACE,0);

	  for ( int f=oldNF; f<xyz_in.getLength(0); f++ )
      	    {
      	      initialFaces(xyz_in.getLength(0)-1-f+oldNF,0) = tmp(f,1);
      	      initialFaces(xyz_in.getLength(0)-1-f+oldNF,1) = tmp(f,0);
      	    }
	}
    }

  //  xyz_in.display("xyz_in");
  //  initialFaces.display("initial faces");

  SquareMapping & square = (SquareMapping &)getReferenceGrid();

  if ( use_cutout )
    ::buildUmapFromCutout(initialFaces,xyz_in,square,dx,cutoutIDMap,umap);
  else
    ::buildUmap(initialFaces, xyz_in, square,dx,umap);
  
  gridIDList = ArraySimple<int>();

  umap_built = true;
  return umap;

}

void 
SimpleCurve::
setNumberOfPoints(int n)
{
  discretized=false;
  nPoints=n;
}

ArraySimple<int> &
SimpleCurve::
getGridIDList() 
{
  if ( !gridIDList.size() )
    {
      gridIDList = ArraySimple<int>(numberOfPoints());
      gridIDList = -1;
      gridIDList(0) = getStartPointID();
      for ( int i=1; i<numberOfPoints()-1; i++ )
	gridIDList(i) = getVertexID();
      gridIDList(numberOfPoints()-1) = getEndPointID();
    }

  return gridIDList;
}

ArraySimple<real> &
SimpleCurve::
getVertices() 
{
  return localGrid;
}

void
SimpleCurve::
discretize(real dxRef, real dyRef, ArraySimple<real> &grid)
{
  if ( nPoints<1 && !Curve::region_1() )
    return; // no spacing info yet

  grid = ArraySimple<real>();
  if ( discretized ) 
    {
      grid = localGrid;
      return;
    }

  if ( nPoints>1 )
    {
      // ignore region spacing suggestions, use a specified # of points
      discretized = true;
      realArray verts;      
      nurbRep.setGridDimensions(0,nPoints);
      if ( stretching )
	{
	  stretching->setGridDimensions(0,nPoints);
	  verts = stretching->getGrid();
	}
      else
	verts = nurbRep.getGrid();

      int nv = nPoints;
      localGrid = ArraySimple<real>(nv, 2);
      for ( int v=0; v<nv; v++ )
	for ( int a=0; a<2; a++ )
	  localGrid(v,a) = verts(v,0,0,a);
      
      grid = localGrid;
      return;
    }

  // generate the discretization by iterative bisection using spacing
  //  defined by dxRef and dyRef
  bool someSplit = true;
  realArray verts;
  int dim = nurbRep.getGridDimensions(0);
  real xdBound = nurbRep.getRangeBound(1,0)-nurbRep.getRangeBound(0,0);
  real ydBound = nurbRep.getRangeBound(1,1)-nurbRep.getRangeBound(0,1);

  dim = max(2,abs(rounder(xdBound/dxRef))+1);
  if ( getStartPointID()==getEndPointID() )
    dim += max(2,abs(rounder(ydBound/dyRef))+1);
  else
    dim = max(dim,abs(rounder(ydBound/dyRef))+1);

  //  nurbRep.setGridDimensions(0,5);
  nurbRep.setGridDimensions(0,dim);
  verts = nurbRep.getGrid();

  int nv = verts.getLength(0);

  MappingProjectionParameters mp;
  mp.getRealArray(MappingProjectionParameters::x).redim(0);
  mp.getRealArray(MappingProjectionParameters::r).redim(0);
  mp.getRealArray(MappingProjectionParameters::xr).redim(0);
  mp.getRealArray(MappingProjectionParameters::normal).redim(0);

  //  ArraySimple<int> split(20);
  intArray split(nv);
  ArraySimpleFixed<real,2,1,1,1> edge;

  real dx[] = {dxRef, dyRef};

  while (someSplit)
    {
      int nvold = nv;
      
      int a,nsplit;
      someSplit=false;
      nsplit = 0;
      split = 0;

      //      verts.display("verts");
      for ( int v=0; v<nvold-1; v++ )
	{
	  for ( a=0; a<2; a++ )
	    edge[a] = (verts(v+1,0,0,a)-verts(v,0,0,a))/dx[a];
	  
	  if ( (ASmag2(edge)-2.25)>100*REAL_MIN )
	    {
	      split(v) = 1;
	      someSplit = true;
	      nsplit++;
	    }
	}

      if ( someSplit )
	{
	  nv = nvold + nsplit;
	  realArray splitVerts(nsplit,2);

	  int a,v,vv=0;
	  for ( v=0; v<nvold-1; v++ )
	    if ( split(v)==1 )
	      {
		for ( a=0; a<2; a++ )
		  splitVerts(vv, a) = 0.5*(verts(v,0,0,a)+verts(v+1,0,0,a));
		vv++;
	      }

	  mp.getRealArray(MappingProjectionParameters::r).resize(nsplit,1);
	  mp.getRealArray(MappingProjectionParameters::r) = -1;
	  mp.getRealArray(MappingProjectionParameters::x).redim(0);
	  mp.getRealArray(MappingProjectionParameters::x) = splitVerts;
			  
	  nurbRep.project(splitVerts,mp);
	  
	  realArray oldVerts;
	  oldVerts = verts;
	  
	  verts.resize(nv,1,1,2);

	  int sv;
	  vv = sv = 0;
	  for ( v=0; v<nvold-1; v++ )
	    if ( split(v)==1 )
	      {
		for ( a=0; a<2; a++ )
		  verts(vv+1,0,0,a) = splitVerts(sv,a);

		vv++; sv++;

		for ( a=0; a<2; a++ )
		  verts(vv+1,0,0,a) = oldVerts(v+1,0,0,a);

		vv++;
	      }
	  else
	    {
	      for ( a=0; a<2; a++ )
		verts(vv+1,0,0,a) = oldVerts(v+1,0,0,a);

	      vv++;
	    }

	  if ( nv>=split.getLength(0) ) split.resize(nv+20);
	}
    }

  localGrid = ArraySimple<real>(nv, 2);

  if ( stretching )
    {
      // yeah, this could be done at a better time...
      stretching->setGridDimensions(0,nv);
      verts = stretching->getGrid();
      //      cout<<"using stretched verts "<<endl;
      //      verts.display();
    }

  for ( int v=0; v<nv; v++ )
    for ( int a=0; a<2; a++ )
      localGrid(v,a) = verts(v,0,0,a);

  // set the nurb grid to twice the discretized resolution
  //  to make the curve plot more smoothly than the mesh
  // XXX debug!
  //  nurbRep.setGridDimensions(0,nv);
  nurbRep.setGridDimensions(0,2*nv);
  discretized = true;
  grid = localGrid;
}

void 
SimpleCurve::
deleteStretching()
{
  if ( stretching ) 
    delete stretching;

  stretching = 0;

  discretized = false;
}

void 
SimpleCurve::
stretchPoints()
{
  if ( !stretching ) 
    stretching = new StretchTransform;
  
  if ( nPoints>0 )
    nurbRep.setGridDimensions(0,nPoints);

  nurbRep.getGrid();

  stretching->setMapping(nurbRep);

  if ( nPoints>0 )
    {
      stretching->setGridDimensions(0,nPoints);
    }

  MappingInformation mapInfo;
  mapInfo.graphXInterface = Overture::getGraphicsInterface();
  stretching->update(mapInfo);

  nurbRep.setGridDimensions(0,100);

  discretized = false;
}

bool 
CompositeCurve::
push(Curve *c)
{
  // return true if this works, false otherwise
  // the curve must match at either the beginning
  //  or end of the current curve

  // XXX should check to see if curve c is ok!

  if ( curves.size()==0 )
    {
      // this is the first curve
      curves.push_back(c);
      reverse.push_back(false);
      setStartPointID(c->getStartPointID());
      setEndPointID(c->getEndPointID());
      nurbRep = c->getNurbs();
      return true;
    }

  // make sure this curve has not already been used
  if ( count(this->curves.begin(),this->curves.end(),c)!=0 )
    return false;

  int ps = getStartPointID();
  int pe = getEndPointID();
  int cps = c->getStartPointID();
  int cpe = c->getEndPointID();

  if ( ps==cps )
    {
      curves.insert(curves.begin(),c);
      reverse.insert(reverse.begin(),true);
      setStartPointID(cpe);
    }
  else if ( ps==cpe )
    {
      curves.insert(curves.begin(),c);
      reverse.insert(reverse.begin(),false);
      setStartPointID(cps);
    }
  else if ( pe==cps )
    {
      curves.push_back(c);
      reverse.push_back(false);
      setEndPointID(cpe);
    }
  else if ( pe==cpe )
    {
      curves.push_back(c);
      reverse.push_back(true);
      setEndPointID(cps);
    }
  else
    return false; // does not make a continuous curve

  // make a copy of the nurbs because the merge may flip
  //   the parameterization
  NurbsMapping localNurb;
  localNurb = c->getNurbs();
  nurbRep.merge(localNurb);
  return true;
}

Curve *
CompositeCurve::
pop()
{
  if ( !curves.size() ) return 0;

  reverse.pop_back();
  Curve *c = curves.back();
  curves.pop_back();
  return c;
}

int 
CompositeCurve::
numberOfPoints()
{
#if 0
  int nPts = curve_size() ? curve_size()+1 : 0;
  for ( curve_iterator c=curves.begin(); c!=curves.end(); c++ )
    {
      nPts+=max(1,(*c)->numberOfPoints()-2);
    }
#else
  int nPts=1;
  for ( curve_iterator c=curves.begin(); c!=curves.end(); c++ )
    {
      nPts+=(*c)->numberOfPoints()-1;
    }
#endif

  return nPts;
}

ArraySimple<int> &
CompositeCurve::
getGridIDList() 
{
  if ( !gridIDList.size() )
    {
      gridIDList = ArraySimple<int>(numberOfPoints());
      gridIDList = -1;

      int pcnt = 0; // grid point counter
      
      curve_iterator ci=curves.begin();
      std::vector<bool>::iterator cReverse=reverse.begin();
      int lastPnt;
      
      for ( ; ci!=curves.end() ; ci++, cReverse++ )
	{
	  
	  Curve *c = *ci;
	  ArraySimple<int> &curveIDs = c->getGridIDList();
	  // assign vertices to the grid, skip last vertex since 
	  //   it will be taken care of in the next curve
	  
	  if ( !(*cReverse) ) // do not reverse the curve
	    {	
	      for ( int p=0; p<c->numberOfPoints()-1; p++,pcnt++ )
		gridIDList(pcnt) = curveIDs(p);
	      
	      lastPnt = curveIDs(c->numberOfPoints()-1);
	    }
	  else
	    {
	      for ( int p=c->numberOfPoints()-1; p>0; p--, pcnt++ )
		gridIDList(pcnt) = curveIDs(p);
	      
	      lastPnt = curveIDs(0);
	    }
	  
	}
      
      // now add the last point
      gridIDList(pcnt) = lastPnt;
    }

  return gridIDList;
}

ArraySimple<real> &
CompositeCurve::
getVertices() 
{
  return localGrid;
}

void 
CompositeCurve::
discretize(real dxRef, real dyRef, ArraySimple<real> &grid)
{
  // discretization is determined by the component curves
  //  and cannot be altered by this curve

  // first make sure all the curves are discretized
  for ( curve_iterator c=curves.begin(); c!=curves.end(); c++ )
    (*c)->discretize(dxRef,dyRef,grid);

  int nPtsTotal = numberOfPoints();
  grid = ArraySimple<real>(nPtsTotal, 2);
  grid = 0.;

  int pcnt = 0; // grid point counter
  
  curve_iterator ci=curves.begin();
  std::vector<bool>::iterator cReverse=reverse.begin();
  ArraySimpleFixed<real,2,1,1,1> lastPnt;

  for ( ; ci!=curves.end() ; ci++, cReverse++ )
    {
      
      Curve *c = *ci;
      ArraySimple<real> xyz;
      c->discretize(dxRef,dyRef,xyz);
      // assign vertices to the grid, skip last vertex since 
      //   it will be taken care of in the next curve

      if ( !(*cReverse) ) // do not reverse the curve
	{	
	  //	  cout<<c->getStartPointID()<<" "<<c->getEndPointID()<<" ";
	  for ( int p=0; p<c->numberOfPoints()-1; p++,pcnt++ )
	    for ( int a=0; a<2; a++ )
	      grid(pcnt, a) = xyz(p,a);

	  for ( int a=0; a<2; a++ )
	    lastPnt[a] = xyz(c->numberOfPoints()-1,a);
	}
      else
	{
	  //	  cout<<c->getEndPointID()<<" "<<c->getStartPointID()<<" ";
	  for ( int p=c->numberOfPoints()-1; p>0; p--, pcnt++ )
	    for ( int a=0; a<2; a++ )
	      grid(pcnt, a) = xyz(p,a);

	  for ( int a=0; a<2; a++ )
	    lastPnt[a] = xyz(0,a);
	}
      
    }
  //  cout<<endl;

  // now add the last point
  for ( int a=0; a<2; a++ )
    grid(pcnt, a) = lastPnt[a];
  assert(++pcnt==numberOfPoints());

  localGrid = grid;
  // set the nurb grid to twice the discretized resolution
  //  to make the curve plot more smoothly than the mesh
  nurbRep.setGridDimensions(0,2*(pcnt+1));

  return;
}

void 
CompositeCurve::
region_1(Region *r) 
{ 
  Curve::region_1(r);

  for ( curve_iterator c=curve_begin(); c!=curve_end(); c++ )
    {
      if ( !(*c)->region_1() )
	(*c)->region_1(r);
      else
	(*c)->region_2(r);
    }
}

void 
CompositeCurve::
region_2(Region *r) 
{ 
  //Curve::region_1(r);

  for ( curve_iterator c=curve_begin(); c!=curve_end(); c++ )
    {
      if ( !(*c)->region_1() )
	(*c)->region_1(r);
      else
	(*c)->region_2(r);
    }
}

void
CompositeCurve::
unbindRegion(Region *r)
{
  Curve::unbindRegion(r);

  for ( curve_iterator c=curve_begin(); c!=curve_end(); c++ )
    (*c)->unbindRegion(r);
}

namespace {

  void buildUmapFromCutout(intArray &initialFaces, realArray &xyz_in,
			   SquareMapping &square, real dx[], 
			   ArraySimple<int> &gridIDList,
			   UnstructuredMapping &umap)
  {
    // we will use the advancing front mesh generator
    //   to build the unstructured mesh.  Create a 
    //   simple stretching function based on dxRef,dyRef.
    AdvancingFront af;
    
    real x1,x2,y1,y2;
    square.getVertices(x1,x2,y1,y2);
    SquareMapping bgSquare(x1-dx[0],x2+dx[0],y1-dx[1],y2+dx[1]);
    bgSquare.setGridDimensions(0,2);
    bgSquare.setGridDimensions(1,2);
    MappedGrid bgGrid(bgSquare);
    CompositeGrid cg;
    cg.add(bgGrid);
    Range all;
    RealCompositeGridFunction strFun(cg,all,all,all,2,2);
    strFun = 0.;
    Index I1,I2,I3;
    getIndex(cg[0].gridIndexRange(),I1,I2,I3);
    cg[0].update(MappedGrid::THEvertex);
    for ( int a=0; a<2; a++ )
    strFun[0](I1,I2,I3,a,a) = 1.0/dx[a];
    
    af.setControlFunction(cg,strFun);

    intArray mask;
  
    cutcell( square, dx[0], dx[1], initialFaces, xyz_in, mask);
    int nx=square.getGridDimensions(0);
    int ny=square.getGridDimensions(1);
    real dxs = 
      real(square.getRangeBound(1,0)-square.getRangeBound(0,0))/real(nx-1);
    real dys = 
      real(square.getRangeBound(1,1)-square.getRangeBound(0,1))/real(ny-1);

    real x0 = square.getRangeBound(0,0);
    real y0 = square.getRangeBound(0,1);

    gridIDList = ArraySimple<int>(nx,ny);
    gridIDList = -1;
    int vertexID=xyz_in.getLength(0);
    ArraySimple<bool> curveVertexUsed(xyz_in.getLength(0));
    curveVertexUsed = false;
    int numberOfCurveVertices = xyz_in.getLength(0);
    xyz_in.resize(xyz_in.getLength(0)+nx*ny,2);

  //  mask.display();
#if 1
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny; j++ )
	if ( mask(i,j)==activeNode )
	  {
	    int ip1 = min(nx-1,i+1);
	    int im1 = max(0,i-1);
	    int jp1 = min(ny-1,j+1);
	    int jm1 = max(0,j-1);
	  
	    if ( mask(ip1,j)==blankedNode || mask(im1,j)==blankedNode ||
		 mask(i,jp1)==blankedNode || mask(i,jm1)==blankedNode ||
		 mask(ip1,jp1)==blankedNode || mask(im1,jm1)==blankedNode ||
		 mask(im1,jp1)==blankedNode || mask(ip1,jm1)==blankedNode )
	      mask(i,j) = blankedNode-1;

	  }
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny; j++ )
	if ( mask(i,j)==(blankedNode-1) )
	  mask(i,j) = blankedNode;
#endif
    // scan for and remove dangling edges
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny; j++ )
	{
	  if ( mask(i,j)==activeNode )
	    {
	      int ip1 = min(nx-1,i+1);
	      int im1 = max(0,i-1);
	      int jp1 = min(ny-1,j+1);
	      int jm1 = max(0,j-1);
	    
	      int c=0;
	      if ( mask(ip1,j)==blankedNode ) c++;
	      if ( mask(im1,j)==blankedNode ) c++;
	      if ( mask(i,jp1)==blankedNode ) c++;
	      if ( mask(i,jm1)==blankedNode ) c++;

	      if (c>2) 
		mask(i,j) = blankedNode;
	      else if ( (i==0 || j==0 || i==(nx-1) || j==(ny-1)) && c>1 )
		mask(i,j) = blankedNode;
	      
	      
	    }
	}
	  
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny; j++ )
	if ( mask(i,j)>activeNode ) 
	  {
	    gridIDList(i,j) = mask(i,j);
	    mask(i,j) = 1;
	    curveVertexUsed(gridIDList(i,j)) = true;
	  }
	else if ( mask(i,j)==activeNode )
	  {
	    mask(i,j) = 1;
	    xyz_in(vertexID,0) = x0 + dxs*i;
	    xyz_in(vertexID,1) = y0 + dys*j;

	    gridIDList(i,j) = vertexID++;
	  }
	else
	  mask(i,j) = 0;

    xyz_in.resize(vertexID,2);

    //      mask.display("after dangle");

  // count the number of nodes on the stair-stepped boundary
    int nBdy=0;
    for ( int i=0; i<nx; i++ )
      for ( int j=0; j<ny; j++ )
	{
	  int ip1 = min(nx-1,i+1);
	  int im1 = max(0,i-1);
	  int jp1 = min(ny-1,j+1);
	  int jm1 = max(0,j-1);

	  if ( mask(i,j)>0 )
	    if ( mask(ip1,j)==0 || mask(ip1,jp1)==0 || mask(i,jp1)==0 ||
		 mask(im1,jp1)==0 || mask(im1,j)==0 || mask(im1,jm1)==0 ||
		 mask(i,jm1)==0 || mask(ip1,jm1)==0 )
	      nBdy++;
	}

    intArray newFaces(initialFaces.getLength(0)+2*nBdy, 2);
    newFaces = -1;
    // first add curve edges that are free of the cartesian grid boundary
    int face=0;
    for ( int f=0; f<initialFaces.getLength(0); f++ )
      if ( !( curveVertexUsed(initialFaces(f,0)) && 
	      curveVertexUsed(initialFaces(f,1)) ) )
	{
	  newFaces(face,0) = initialFaces(f,0);
	  newFaces(face,1) = initialFaces(f,1);
	  face++;
	}

    //  cout<<curveVertexUsed<<endl;

  //  newFaces.display("new faces");
  // now sweep through the cartesian mesh adding the additional faces
    for ( int i=0; i<nx-1; i++ )
      for ( int j=0; j<ny-1; j++ )
	{
	
	  if ( mask(i,j)>0 && mask(i+1,j)>0 && 
	       ! (gridIDList(i,j)<numberOfCurveVertices &&
		  gridIDList(i+1,j)<numberOfCurveVertices) &&
	       ( mask(i,j+1)==0 || mask(i+1,j+1)==0 ) )
	    { // bottom edge, hole in +j direction
	      newFaces(face,0) = gridIDList(i,j);
	      newFaces(face,1) = gridIDList(i+1,j);
	      face++;
	    }
	  else if ( mask(i,j+1)>0 && mask(i+1,j+1)>0 &&
		    !( gridIDList(i,j+1)<numberOfCurveVertices &&
		       gridIDList(i+1,j+1)<numberOfCurveVertices ) &&
		    ( mask(i,j)==0 || mask(i+1,j)==0 ) ) 
	    { // top edge, hole in -j direction
	      newFaces(face,0) = gridIDList(i+1,j+1);
	      newFaces(face,1) = gridIDList(i,j+1);
	      face++;
	    }
	  if ( mask(i,j)>0 && mask(i,j+1)>0 &&
	       !( gridIDList(i,j)<numberOfCurveVertices &&
		  gridIDList(i,j+1)<numberOfCurveVertices) &&
	       ( mask(i+1,j)==0 || mask(i+1,j+1)==0 ) )
	    { // left edge, hole in +i direction
	      newFaces(face,0) = gridIDList(i,j+1);
	      newFaces(face,1) = gridIDList(i,j);
	      face++;
	    }
	  else if ( mask(i+1,j)>0 && mask(i+1,j+1)>0 &&
		    !( gridIDList(i+1,j)<numberOfCurveVertices &&
		       gridIDList(i+1,j+1)<numberOfCurveVertices)  &&
		    ( mask(i,j)==0 || mask(i,j+1)==0 ) )
	    { // right edge, hole in -i direction
	      newFaces(face,0) = gridIDList(i+1,j);
	      newFaces(face,1) = gridIDList(i+1,j+1);
	      face++;
	    }

	  if ( i==0 || j==0 || i==(nx-2) || j==(ny-2) )
	    {
	      // check for boundary gaps taken care of by the curves
	      if ( j==0 && mask(i,j)>0 && mask(i+1,j)>0 && 
		   (gridIDList(i,j)<numberOfCurveVertices &&
		    gridIDList(i+1,j)<numberOfCurveVertices) &&
		   ( mask(i,j+1)==0 || mask(i+1,j+1)==0 ) )
		{ // bottom edge, hole in +j direction
		  newFaces(face,0) = gridIDList(i,j);
		  newFaces(face,1) = gridIDList(i+1,j);
		  face++;
		}
	      else if ( j==(ny-2) && mask(i,j+1)>0 && mask(i+1,j+1)>0 &&
			( gridIDList(i,j+1)<numberOfCurveVertices &&
			  gridIDList(i+1,j+1)<numberOfCurveVertices ) &&
			( mask(i,j)==0 || mask(i+1,j)==0 ) ) 
		{ // top edge, hole in -j direction
		  newFaces(face,0) = gridIDList(i+1,j+1);
		  newFaces(face,1) = gridIDList(i,j+1);
		  face++;
		}
	      if ( i==0 && mask(i,j)>0 && mask(i,j+1)>0 &&
		   ( gridIDList(i,j)<numberOfCurveVertices &&
		     gridIDList(i,j+1)<numberOfCurveVertices) &&
		   ( mask(i+1,j)==0 || mask(i+1,j+1)==0 ) )
		{ // left edge, hole in +i direction
		  newFaces(face,0) = gridIDList(i,j+1);
		  newFaces(face,1) = gridIDList(i,j);
		  face++;
		}
	      else if ( i==(nx-2) && mask(i+1,j)>0 && mask(i+1,j+1)>0 &&
			( gridIDList(i+1,j)<numberOfCurveVertices &&
			  gridIDList(i+1,j+1)<numberOfCurveVertices)  &&
			( mask(i,j)==0 || mask(i,j+1)==0 ) )
		{ // right edge, hole in -i direction
		  newFaces(face,0) = gridIDList(i+1,j);
		  newFaces(face,1) = gridIDList(i+1,j+1);
		  face++;
		}
	    }
	}

    newFaces.resize(face,2);
    real MINS = REAL_MAX;
    if ( face )
      {
	for ( int f=0; f<newFaces.getLength(0); f++ )
	  {
	    int p1 = newFaces(f,0);
	    int p2 = newFaces(f,1);

	    real s = (xyz_in(p2,0)-xyz_in(p1,0))*(xyz_in(p2,0)-xyz_in(p1,0)) +
	      (xyz_in(p2,1)-xyz_in(p1,1))*(xyz_in(p2,1)-xyz_in(p1,1));

	    if ( s<REAL_EPSILON )
	      {
		//		cout<<"ZERO SIZED FACE AT "<<f<<endl;
		Range AXES(2);
		xyz_in(p1,AXES).display();
		xyz_in(p2,AXES).display();
		MINS = min(MINS,s);
	      }

	    
	  }
      
	//		cout<<"MINS is "<<MINS<<" face is "<<face<<endl;
	af.initialize(newFaces,xyz_in);

#if 0
	while(!af.isFrontEmpty())
	  {
	    GraphicsParameters gp;
	    Overture::getGraphicsInterface()->stopReadingCommandFile();
	    PlotIt::plot(*Overture::getGraphicsInterface(),af,gp);
	    af.getParameters().setNumberOfAdvances(1);
	    af.advanceFront(1);
	  }
#endif

	int s=0, sMax=10;
	af.getParameters().setQualityTolerance(.5);
	try {
	  while (!af.isFrontEmpty() && s++<sMax )
	    af.advanceFront(-1); // advance till finished with mesh
	  if ( !af.isFrontEmpty() )
	    {
	      af.getParameters().setQualityTolerance(REAL_MIN);
	      af.advanceFront(-1);
	    }
	} 
	catch ( AdvancingFrontError & e )
	  {
	    cout<<"ERROR : advancing front error "<<endl;
	    Overture::getGraphicsInterface()->stopReadingCommandFile();
	    GraphicsParameters gp;
	    PlotIt::plot(*Overture::getGraphicsInterface(),af,gp);
	  }
	catch ( GeometricADTError & e )
	  {
	    cout<<"ERROR : geometric adt error"<<endl;
	    Overture::getGraphicsInterface()->stopReadingCommandFile();
	    GraphicsParameters gp;
	    PlotIt::plot(*Overture::getGraphicsInterface(),af,gp);
	  }
	catch ( ... )
	  {
	    cout<<"ERROR : unknown error"<<endl;
	    Overture::getGraphicsInterface()->stopReadingCommandFile();
	    GraphicsParameters gp;
	    PlotIt::plot(*Overture::getGraphicsInterface(),af,gp);
	  }


	const intArray & afelems = af.generateElementList(false);
	const realArray & afNodes = af.getVertices();
// 	{
// 	  UnstructuredMapping umapt;
// 	  umapt.setNodesAndConnectivity(afNodes, afelems,2);
// 	  verifyUnstructuredConnectivity( umapt, true );
// 	}

	xyz_in.resize(afNodes.getLength(0),2);
	xyz_in = afNodes;
	xyz_in.resize(af.getNumberOfVertices(),2);

	intArray elementList(afelems.getLength(0)+(nx-1)*(ny-1),4);
	elementList = -1;
	Range ELEMS(afelems.getLength(0));
	Range NODES(3);
	elementList(ELEMS,NODES) = afelems(ELEMS,NODES); 
	int el = afelems.getLength(0);
	for ( int i=0; i<(nx-1); i++ )
	  for ( int j=0; j<(ny-1); j++ )
	    if ( mask(i,j)==1 && mask(i+1,j)==1 &&
		 mask(i+1,j+1)==1 && mask(i,j+1)==1 )
	      {
		elementList(el,0) = gridIDList(i,j);
		elementList(el,1) = gridIDList(i+1,j);
		elementList(el,2) = gridIDList(i+1,j+1);
		elementList(el,3) = gridIDList(i,j+1);
		el++;
	      }

	elementList.resize(el,4);
	af.destroyFront();
	umap.setNodesAndConnectivity(xyz_in, elementList,2);
	verifyUnstructuredConnectivity( umap, true );
 
      }
    else
      {
	intArray elementList((nx-1)*(ny-1),4);
	elementList = -1;
	int el = 0;//afelems.getLength(0);
	for ( int i=0; i<(nx-1); i++ )
	  for ( int j=0; j<(ny-1); j++ )
	    if ( mask(i,j)==1 && mask(i+1,j)==1 &&
		 mask(i+1,j+1)==1 && mask(i,j+1)==1 )
	      {
		elementList(el,0) = gridIDList(i,j);
		elementList(el,1) = gridIDList(i+1,j);
		elementList(el,2) = gridIDList(i+1,j+1);
		elementList(el,3) = gridIDList(i,j+1);
		el++;
	      }
	
	elementList.resize(el,4);
	umap.setNodesAndConnectivity(xyz_in, elementList,2,false);
      }

  }

  void buildUmap(intArray &initialFaces, realArray &xyz_in,
		 SquareMapping &square, real dx[], UnstructuredMapping &umap)
  {
    // we will use the advancing front mesh generator
    //   to build the unstructured mesh.  Create a 
    //   simple stretching function based on dxRef,dyRef.
    AdvancingFront af;
    
    real x1,x2,y1,y2;
    square.getVertices(x1,x2,y1,y2);
    SquareMapping bgSquare(x1-dx[0],x2+dx[0],y1-dx[1],y2+dx[1]);
    bgSquare.setGridDimensions(0,max(5,int((x2-x1)/dx[0]+.5)));
    bgSquare.setGridDimensions(1,max(5,int((y2-y1)/dx[1]+.5)));

    int idx = bgSquare.getGridDimensions(0);
    int jdx = bgSquare.getGridDimensions(1);
    real sdx = (x2-x1+2*dx[0])/real(idx-1);
    real sdy = (y2-y1+2*dx[1])/real(jdx-1);

    MappedGrid bgGrid(bgSquare);
    CompositeGrid cg;
    cg.add(bgGrid);
    Range all;
    RealCompositeGridFunction strFun(cg,all,all,all,2,2);
    RealCompositeGridFunction strCnt(cg,all,all,all);
    strFun=0;
    strCnt = 1.;
    Index I1,I2,I3;
    getIndex(cg[0].gridIndexRange(),I1,I2,I3);
    cg[0].update(MappedGrid::THEvertex);

    for ( int a=0; a<2; a++ )
      strFun[0](all,all,all,a,a) = 1.0/dx[a];
    
#if 1
    ArraySimpleFixed<real,2,1,1,1> mid,e;
    for ( int ff=0; ff<initialFaces.getLength(0); ff++ )
      {
	mid[0] = (xyz_in(initialFaces(ff,0),0)+xyz_in(initialFaces(ff,1),0))/2.;
	mid[1] = (xyz_in(initialFaces(ff,0),1)+xyz_in(initialFaces(ff,1),1))/2.;

	e[0] = (xyz_in(initialFaces(ff,1),0)-xyz_in(initialFaces(ff,0),0));
	e[1] = (xyz_in(initialFaces(ff,1),1)-xyz_in(initialFaces(ff,0),1));

	int i = max(0,min(idx,int((mid[0]-x1+dx[0])/sdx)));
	int j = max(0,min(jdx,int((mid[1]-y1+dx[1])/sdy)));

// 	cout<<"mid is "<<mid<<endl;
// 	cout<<"i,j "<<i<<" "<<j<<endl;
// 	cout<<"sdx,sdy "<<sdx<<"  "<<sdy<<endl;
// 	cout<<"idx, jdx "<<idx<<"  "<<jdx<<endl;
// 	cout<<(mid[0]-x1+dx[0])/sdx<<"  "<<(mid[1]-y1+dx[1])/sdy<<endl;

	real len = .95*sqrt(ASmag2(e));
	real d = 1./len;

	for ( int a=0; a<2; a++ )
	  {
	    strFun[0](i,j,I3,a,a) += d;
	    strFun[0](i+1,j,I3,a,a) += d;
	    strFun[0](i+1,j+1,I3,a,a) += d;
	    strFun[0](i,j+1,I3,a,a) += d;
	  }
	strCnt[0](i,j,I3) += 1; 
	strCnt[0](i+1,j,I3)+=1;
	strCnt[0](i+1,j+1,I3)+=1;
	strCnt[0](i,j+1,I3)+=1;
      }

    for ( int a=0; a<2; a++ )
      strFun[0](I1,I2,I3,a,a) /= strCnt[0](I1,I2,I3);

 //    PlotIt::contour(*Overture::getGraphicsInterface(),strCnt);
    //     PlotIt::contour(*Overture::getGraphicsInterface(),strFun);


    getIndex(cg[0].gridIndexRange(),I1,I2,I3,-1);

    for ( int i=0; i<5; i++ )
      {
	for ( int a=0; a<2; a++ )
	  strFun[0](I1,I2,I3,a,a) = (strFun[0](I1-1,I2,I3,a,a) + strFun[0](I1+1,I2,I3,a,a) + 
				    strFun[0](I1,I2-1,I3,a,a) + strFun[0](I1,I2+1,I3,a,a))/4.;
      }
#endif
    
    af.setControlFunction(cg,strFun);

    //    PlotIt::contour(*Overture::getGraphicsInterface(),strFun);
    af.initialize(initialFaces,xyz_in);
    
#if 0
    while(!af.isFrontEmpty())
      {
	GraphicsParameters gp;
	PlotIt::plot(*Overture::getGraphicsInterface(),af,gp);
	af.getParameters().setNumberOfAdvances(1);
	af.advanceFront(1);
      }
#else
    int s=0, sMax=10;
    af.getParameters().setQualityTolerance(.5);
    while (!af.isFrontEmpty() && s++<sMax )
      af.advanceFront(-1); // advance till finished with mesh
    if ( !af.isFrontEmpty() )
      {
	af.getParameters().setQualityTolerance(REAL_MIN);
	af.advanceFront(-1);
      }
#endif

    const intArray & afelems = af.generateElementList();
    const realArray & afNodes = af.getVertices();
    
    xyz_in.resize(afNodes.getLength(0),2);
    xyz_in = afNodes;
    xyz_in.resize(af.getNumberOfVertices(),2);
    
    umap.setNodesAndConnectivity(xyz_in,af.generateElementList(),2,false);
    
  }

}
