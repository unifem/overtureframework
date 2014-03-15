#ifndef __SMESH_HH__
#define __SMESH_HH__

#include <vector>
#include <algorithm>

#include "OvertureTypes.h"
#include "TFIMapping.h"
#include "UnstructuredMapping.h"
#include "SquareMapping.h"
#include "StretchTransform.h"
#include "ArraySimple.h"
#include "NurbsMapping.h"

// getVertexID is used to set global ids for all the vertices
extern int getVertexID();
extern int freeVertexID();
extern void resetVertexIDs();
extern int currentIDCounter();

class Region;

/// Curve base class specifying the basic behavior of all Curves
class Curve {
public:

  //  Curve() : startPointID(-1), endPointID(-1) { regions[0]=regions[1]=0; }
  Curve(int ps=-1, int pe=-1) : startPointID(ps), endPointID(pe), id(idCounter++)
  { regions[0]=regions[1]=0; }

  virtual ~Curve() { }

  // get a NURBS representation of the curve suitable for plotting and mapping
  virtual NurbsMapping & getNurbs()=0;

  // get the range bound for the curve
  virtual real getRangeBound(int side, int axis)=0;

  // get the primary and secondary region pointers
  Region *region_1() { return regions[0]; }
  Region *region_2() { return regions[1]; }

  // set the primary and secondary region pointers
  virtual void region_1(Region *r) { regions[0]=r; }
  virtual void region_2(Region *r) { regions[1]=r;}

  // remove a specific region 
  virtual void unbindRegion(Region *r) 
  {
    if ( regions[0]==r )
      {
	regions[0] = regions[1];
	regions[1] = 0;
      }
    else if ( regions[1]==r )
      regions[1] = 0;
  }

  // get/set endpoints
  void setStartPointID(int pid) { startPointID=pid; };
  void setEndPointID(int pid) { endPointID=pid; }
  int getStartPointID() { return startPointID; };
  int getEndPointID() { return endPointID; }
  
  /// discretize the curve using dxRef and dyRef as a guide, return the
  //   result in the array grid
  virtual void discretize(real dxRef, real dyRef, ArraySimple<real> &grid)=0;

  virtual ArraySimple<int> & getGridIDList() = 0;
  virtual ArraySimple<real> & getVertices() = 0;
  virtual void resetIDList() { gridIDList = ArraySimple<int>(); }

  // return the number of points in the discretized curve
  virtual int numberOfPoints()=0;

  int ID() const { return id; }

protected:
  ArraySimple<int> gridIDList;

private:
  int startPointID, endPointID;
  Region * regions[2];
  int id;
  static int idCounter;
};

/// Base class for regions
class Region {
  
public:
  Region(real dx=.1, real dy=.1, std::string nm="") : 
    dxRef(dx), dyRef(dy), name(nm), id(idCounter++) { }

  virtual ~Region() 
  {
    // use this while loop because delCurve invalidates curve_iterators
    while ( curve_begin()!=curve_end() )
      delCurve(*curve_begin());

  };

  typedef std::vector<Curve *>::iterator curve_iterator;

  /// add a curve to the region, resetting the reference grid accordingly
  void addCurve(Curve *c) 
  { 
    if ( c )
      { 
	real minmax[2][2];
	referenceGrid.getVertices(minmax[0][0],minmax[1][0],
				  minmax[0][1],minmax[1][1]);

	//	NurbsMapping &n = c->getNurbs();
	//	for ( int a=0; a<2; a++ )
	//	  cout<<"nr nurb rangebound"<<a<<" "<<n.getRangeBound(0,a)<<" "<<n.getRangeBound(1,a)<<endl;

	if ( curves.size()!=0 )
	  for ( int a=0; a<2; a++ )
	    {
	      minmax[0][a] = min(minmax[0][a],real(c->getRangeBound(0,a)),
				 real(c->getRangeBound(1,a)));
	      minmax[1][a] = max(minmax[1][a],real(c->getRangeBound(0,a)),
				 real(c->getRangeBound(1,a)));
	    }
	else // this is the first curve
	  for ( int a=0; a<2; a++ )
	    {
	      minmax[0][a] = min(real(c->getRangeBound(0,a)),
				 real(c->getRangeBound(1,a)));
	      minmax[1][a] = max(real(c->getRangeBound(0,a)),
				 real(c->getRangeBound(1,a)));
	    }

	specifyReferenceGridBounds(minmax[0][0],minmax[0][1],minmax[1][0],
				   minmax[1][1]);
	curves.push_back(c); 

	if ( c->region_1() ) 
	  c->region_2(this);
	else
	  c->region_1(this);
      }
  }

  /// delete a curve from the region
  void delCurve(Curve *c) 
  { 
    if ( c )
      c->unbindRegion(this);

    curves.erase(std::find(curves.begin(),curves.end(),c)); 
    // fail silently if curve c is not in the region
  }

  curve_iterator curve_begin() { return curves.begin(); }
  curve_iterator curve_end() { return curves.end(); }

  int numberOfCurves() const { return curves.size(); }
  virtual int numberOfVertices() = 0;
  virtual int numberOfElements() = 0;
  virtual ArraySimpleFixed<int,4,1,1,1> getElement(int e) = 0;

  // return the Region as an unstructured mesh
  virtual Mapping &getMapping()=0;

  virtual ArraySimple<int> & getGridIDList() = 0;
  virtual ArraySimple<real> & getVertices() = 0;
  virtual void resetIDList() { gridIDList = ArraySimple<int>(); }

  /// get the reference grid used to guide the mesh spacing
  const SquareMapping &getReferenceGrid() const { return referenceGrid; }
  
  real getDx() const { return dxRef; }
  real getDy() const { return dyRef; }
  void setDx(real d) { dxRef=d; resetReferenceGrid(); }
  void setDy(real d) { dyRef=d; resetReferenceGrid(); }
  
  std::string getName() const { return name; }
  void setName(std::string nm) { name=nm; }
  
  int ID() const { return id; }
  
protected:
  // bounds are usually obtained from the curve bounding boxes
  void specifyReferenceGridBounds(real x1, real y1, real x2, real y2);
  void resetReferenceGrid();

  real dxRef, dyRef;
  ArraySimple<int> gridIDList;

private:

  std::vector<Curve*> curves;
  //  ArraySimple<int> gridIDList;
  //  ArraySimple<int> mask;
  SquareMapping referenceGrid;
  std::string name;
  int id;

  static int idCounter;

};

/// A region generated using transfinite interpolation
class TFIRegion : public Region {
public:
  TFIRegion(real dx=.1, real dy=.1) : Region(dx,dy), tfi_built(false) { }
  ~TFIRegion() { }

  virtual Mapping & getMapping();
  virtual ArraySimple<int> & getGridIDList();
  virtual ArraySimple<real> & getVertices();

  virtual int numberOfVertices();
  virtual int numberOfElements();

  virtual ArraySimpleFixed<int,4,1,1,1> getElement(int e);

protected:
  bool tfiUpToDate() const { return tfi_built; }
  ArraySimple<real> grid;

private:
  TFIMapping tfi;
  UnstructuredMapping umap;
  bool tfi_built;
};

/// A region generated using a hybrid/unstructured mesh
class UnstructuredRegion : public Region {
public:
  UnstructuredRegion(real dx=.1, real dy=.1) : 
    Region(dx,dy), umap_built(false), use_cutout(true) { }
  ~UnstructuredRegion() { }

  virtual Mapping &getMapping();
  virtual ArraySimple<int> & getGridIDList();
  virtual ArraySimple<real> & getVertices();

  virtual int numberOfVertices();
  virtual int numberOfElements();

  void useCutout() { use_cutout = true; }
  void dontUseCutout() { use_cutout = false; }

  virtual ArraySimpleFixed<int,4,1,1,1> getElement(int e);

protected:
  bool umapUpToDate() const { return umap_built; }
  ArraySimple<real> grid;
  ArraySimple<int> cutoutIDMap;

private:
  UnstructuredMapping umap;
  bool umap_built;
  bool use_cutout;
};

/// simple curve class, represented as a single nurb, assigns vertex ids
class SimpleCurve : public Curve {
public : 
  SimpleCurve() : Curve(), localGrid(),discretized(false), nPoints(-1) { }
  SimpleCurve(const NurbsMapping &nurb, int ps, int pe) : 
    Curve(ps,pe), nurbRep(nurb), localGrid(), 
    discretized(false), nPoints(-1), stretching(0)
  {   }
  virtual ~SimpleCurve() { if ( stretching ) delete stretching; }

  virtual NurbsMapping &getNurbs() { return nurbRep; }

  // get the range bound for the curve
  virtual real getRangeBound(int side, int axis)
  { return nurbRep.getRangeBound(side,axis); }

  virtual void discretize(real dxRef, real dyRef, ArraySimple<real> &grid);

  virtual ArraySimple<int> & getGridIDList();
  virtual ArraySimple<real> & getVertices();

  virtual int numberOfPoints() 
  { 
    if ( !discretized && Curve::region_1() ) 
      discretize(Curve::region_1()->getDx(), Curve::region_1()->getDy(), localGrid); 

    return localGrid.size() ? localGrid.size(0) : nPoints>1 ? nPoints : 0 ;
  }

  virtual void region_1(Region *r) { discretized=false; Curve::region_1(r); }
  virtual void region_2(Region *r) { Curve::region_2(r); }

  //  virtual void computeGridIDs(ArraySimple<int> &gridID);

  virtual void setNumberOfPoints(int n);
  virtual bool autoGridSize() const { return nPoints<2; }

  virtual void deleteStretching();
  virtual void stretchPoints();

private:
  NurbsMapping nurbRep;
  ArraySimple<real> localGrid;
  bool discretized;
  int nPoints;

  StretchTransform *stretching;
};

/// a curve consisting of one or more other curves, 
// vertex ids are specified by the component curves
class CompositeCurve : public Curve {
public:
  CompositeCurve() : Curve(), localGrid() { }
  virtual ~CompositeCurve() { }

  virtual NurbsMapping &getNurbs() { return nurbRep; }

  // get the range bound for the curve
  virtual real getRangeBound(int side, int axis)
  { 
    real b=0;
    if ( side==0 )
      {
	b=REAL_MAX;
	for ( std::vector<Curve *>::iterator c=curves.begin(); c!=curves.end(); c++ )
	  b=min(b,(*c)->getRangeBound(side,axis));
      }
    else
      {
	b=-REAL_MAX;
	for ( std::vector<Curve *>::iterator c=curves.begin(); c!=curves.end(); c++ )
	  b=max(b,(*c)->getRangeBound(side,axis));
      }
    
    return b;
  }

  virtual void discretize(real dxRef, real dyRef, ArraySimple<real> &grid);

  virtual ArraySimple<int> & getGridIDList();
  virtual ArraySimple<real> & getVertices();

  virtual int numberOfPoints();

  virtual void region_1(Region *r);
  virtual void region_2(Region *r);

  virtual void unbindRegion(Region *r);

  typedef std::vector<Curve*>::iterator curve_iterator;
  curve_iterator curve_begin() { return this->curves.begin(); }
  curve_iterator curve_end() { return this->curves.end(); }
  int curve_size() const { return this->curves.size(); }

  bool push(Curve *c);
  Curve *pop();

private:
  NurbsMapping nurbRep;

  std::vector<Curve *> curves;
  std::vector<bool> reverse;
  ArraySimple<real> localGrid;
};

//inline double pow(double d, int p) { return pow( d, (double)p); }
#endif
