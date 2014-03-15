#ifndef __KKC_AdvancingFront__
#define __KKC_AdvancingFront__

#define USE_SARRAY
//#define USE_GADT2

#include "OvertureDefine.h"
#include "OvertureTypes.h"

#ifndef OV_USE_OLD_STL_HEADERS
#include <vector>
#include <map>
#include <list>
OV_USINGNAMESPACE(std);
#else
#include <vector.h>
#include <map.h>
#include <list.h>
#endif

#include "PriorityQueue.h"

#include "Overture.h"
#include "PlotStuff.h"
#include "Mapping.h"
#include "MappingInformation.h"
#include "Face.h"

#ifdef USE_GADT2
#include "GeometricADT2.h"
#else
#include "GeometricADT.h"
#endif

#include "OvertureDefine.h"
#include "AbstractException.h"
#include "Mapping.h"
#include "UnstructuredMapping.h"
#include "CompositeGridFunction.h"
#include "AdvancingFrontParameters.h"
#include "ArraySimple.h"

//typedef PriorityQueueTemplate<Face *> PriorityQueue;
typedef PriorityBatchQueue<Face *> PriorityQueue;

class AdvancingFront
{

  //  friend class PlotStuff;

public:
  

  AdvancingFront();
  AdvancingFront(intArray &initialFaces, realArray &xyz_in, MappingInformation *backgroundMappings_=NULL);
  
  virtual ~AdvancingFront();

  void initialize(intArray &initialFaces, realArray &xyz_in, MappingInformation *backgroundMappings_= NULL, intArray &initialFaceSurfaceMapping_ = Overture::nullIntegerDistributedArray());

  bool isFrontEmpty() const;

  //int insertFace(const IntegerArray &vertexIDs, int z1, int z2);
  int insertFace( const ArraySimple<int> &vertexIDs, int z1, int z2 );
  int advanceFront(int nSteps = 1);
  
  AdvancingFrontParameters & getParameters() { return parameters; }

  const AdvancingFrontParameters & getParameters() const { return parameters; }
  
  //  PriorityQueue::iterator getFrontIteratorForFace(const IntegerArray &faceVertices);
  //  bool existsInFront(const IntegerArray &);
  PriorityQueue::iterator getFrontIteratorForFace(const ArraySimple<int> &faceVertices);
  bool existsInFront(const ArraySimple<int> &);
  bool existsInFront(const int p1, const int p2);

  void plot(PlotStuff & ps, PlotStuffParameters & psp);

  bool expandFront();

  // getFaces and getFront are used in the plotAdvancingFront method
  const vector<Face *> & getFaces() const { return faces; }
  const PriorityQueue & getFront() const { return front; }

  const realArray & getVertices() { return xyz; }
  const vector< vector<int> > & getElements() const { return elements; }

  intArray  generateElementList( bool removeUnusedNodes = true ); // generates the list of points in each element, used to build an unstructuredMapping
  intArray  generateElementFaceList(); // generate the list of Adv. Front faces in each element

  // get various dimensional information
  int getRangeDimension() const { return rangeDimension; }
  int getDomainDimension() const { return domainDimension; }
  int getNumberOfVertices() const { return nptsTotal; }
  int getNumberOfFaces() const { return nFacesTotal; }
  int getNumberOfElements() const { return nElements; }

  real getAverageFaceSize() const { return averageFaceSize; }

  realArray & getFaceNormals() { return faceNormals; }
  bool vertexIsOnFront(int v);

  // get and set the spacing control grid
  const realCompositeGridFunction & getControlFunction() const { return controlFunction; }
  void setControlFunction(const CompositeGrid & controlGrid_, const realCompositeGridFunction & controlFunction_) 
    { 
      controlGrid.reference(controlGrid_);
      controlGrid.updateReferences();
      controlFunction.reference(controlFunction_);
    }

  // destroy the information in this class, reset to some basic state
  void destroyFront();

  int computeTransformationAtPoint(realArray &midPt, ArraySimple<real> &T); // computes a mesh control transformation based on the data in the controlFunction 

protected:

  AdvancingFrontParameters parameters;

  int nFacesFront;  // number of faces in the front
  int nFacesTotal;  // total number of faces generated
  int nFacesEst;    // estimated number of faces to be generated
  int nptsTotal;  // number of points in the mesh so far (size of used portion of xyz)
  int nptsEst;    // estimated number of points to be created
  int nElements;     // total number of elements generated
  int nElementsEst;  // current estimate on the total number of elements to be generated
  
  int rangeDimension;
  int domainDimension;
  int nFacesPerElementMax;
  int nVerticesPerFaceMax;

  int nexpansions;

  real averageFaceSize;

  vector<Face *> faces;  // a vector of all the faces, indexed by face id
  vector<int> foo;
  vector< vector<int> > elements; // a list containing the elements identified by thier faces
  realArray elementQuality; // the quality of each valid element in elements
  PriorityQueue front;       // a priority queue for the front
  realArray xyz;         // the positions of the points
  realArray faceNormals; // precomputed face normals

  intArray initialFaceSurfaceMapping;

  enum TimingsEnum {
    totalTime=0,
    initializeAdv,
    findExistingCandidates,
    existingInit,
    existingInCircle,
    existingInCircleInit,
    existingInCircle_1,
    existingInCircle_2,
    existingInCircle_3,
    existingInCircle_4,
    existingInCircle_5,
    existingInCircle_trav,
    existingAdj,
    computeNew,
    creationInsertion,
    creationInsertion_1,
    creationInsertion_2,
    creationInsertion_20,
    creationInsertion_21,
    creationInsertion_3,
    insertion,
    intersections,
    getCircleCent,
    faceTrans,
    faceTrans_1,
    faceTrans_2,
    faceTrans_3,
    faceTrans_4,
    elemQual,
    numberOfTimings
  };

  real timing[numberOfTimings];

  struct cmpFace { bool operator() (const int i1, const int i2) const { return (i1<i2); } }; // used in the map below
  map<int, vector< PriorityQueue::iterator >, cmpFace> pointFaceMapping; // maps a point id to the faces that have that point as point "1" (basically a simple hash table)
    
#ifdef USE_GADT2
  GeometricADT<Face *,4> faceSearcher;  // Geometric search ADT used to detect intersections
  GeometricADT<int,4> vertexSearcher; // Geometric search ADT used to find vertices in bounding boxes
#else
  GeometricADT<Face *> faceSearcher;  // Geometric search ADT used to detect intersections
  GeometricADT<int> vertexSearcher; // Geometric search ADT used to find vertices in bounding boxes
#endif

  CompositeGrid controlGrid; // meshes used to define control functions
  realCompositeGridFunction controlFunction;  // function used to define stretching/distribution functions
  //Mapping *backgroundMapping; // background parametric mapping
  MappingInformation backgroundMappings;  // mappings used to project points while makeing surface meshes

  real computeNormalizedGrowthDistance(real, real);

  //void gatherExistingCandidates(const Face & face, real distance, ArraySimple<real> & pIdealTrans,
  //			ArraySimple<real> & T, ArraySimple<real> & Tinv, vector<int> &existing_candidates);
  void gatherExistingCandidates(const Face & face, real distance, ArraySimple<real> & pIdealTrans,
				ArraySimple<real> &pIdealPhys, ArraySimple<real> & T, ArraySimple<real> & Tinv, vector<int> &existing_candidates, vector<int> &existing_candidates_neighb, vector<int> &local_nodes);

  void computeVertexCandidates(const ArraySimple<real> &currentFaceVerticesTrans, 
			       const ArraySimple<real> &pIdealTrans, vector<int> &existing_candidates, 
			       const ArraySimple<real> &T, ArraySimple<real> &new_candidates, real rad);


  bool makeTriTetFromExistingVertices(const Face &currentFace, int newElementID,
				      const vector<int> &existing_candidates, const vector<int> &local_nodes,
				      vector<PriorityQueue::iterator > &oldFrontFaces);
  
  //bool makeTriTetFromNewVertex(const Face & currentFace, int newElementID, 
  //			       realArray &new_candidates);
  bool makeTriTetFromNewVertex(const Face & currentFace, int newElementID, 
			       ArraySimple<real> &new_candidates, vector<int> &local_nodes);

  bool makeTriOnSurface(const Face &currentFace, int newElementID, 
			const vector<int> &existing_candidates, 
			ArraySimple<real> &new_candidates, vector<PriorityQueue::iterator > &oldFrontFaces);

#if 0
  bool makePrismPyramidHex(const Face & currentFace, int newElementID, const ArraySimple<real> &pIdealPhys, 
			   const ArraySimple<real> &T, vector<int> &existing_candidates, 
			   vector<PriorityQueue::iterator > &oldFrontFaces);
#endif

  //  bool isFaceConsistent(const IntegerArray &, const Face & filterFace); //const;  // essentially checks for intersections
  //bool isFaceConsistent(const realArray & , const Face & filterFace); //const; // again.
  bool isFaceConsistent(const ArraySimple<int> &, const Face & filterFace); //const;  // essentially checks for intersections
  bool isFaceConsistent(const ArraySimple<real> & , const Face & filterFace); //const; // again.
  bool isFaceConsistent2D(const ArraySimple<real> &p1, const ArraySimple<real> &p2, const int filterFace) const;

  bool isFaceConsistent2D(int v1, int v2, int filterFace) const;

  //  bool isFaceConsistent2D_3R(const ArraySimple<int> &, const Face & filterFace); //const;  // essentially checks for intersections
  //  bool isFaceConsistent2D_3R(const ArraySimple<real> & , const Face & filterFace); //const; // again.

  bool isFaceConsistent3D(const ArraySimple<int> &, const Face & filterFace); //const;  // essentially checks for intersections
  bool isFaceConsistent3D(const ArraySimple<real> & , const Face & filterFace); //const; // again.
  
  bool isOnFacePlane( const Face & face, ArraySimple<real> &vertex );
  bool checkVertexDirection( const Face & face, const ArraySimple<real> &vertex ) const;

  ArraySimpleFixed<real,3,1,1,1> computeSurfaceNormal(const ArraySimpleFixed<real,3,1,1,1> &vert, int subsurf=-1);
  void computeFaceNormal(const ArraySimple<real> &vertices, ArraySimple<real> & normal, int subsurf = -1);

  void addFaceToFront(Face &);

  //  bool auxiliaryCheck(Face &face, ArraySimple<real> &faceNormal);
  bool auxiliaryCheck(ArraySimple<real> &candNormal, ArraySimple<real> &faceNormal);
  int resizeFaces(int newSize); // resizes face relevant arrays
  //int addPoint(const realArray &newPt); // adds a new point to the list of points, resizes if needed
  int addPoint(const ArraySimple<real> &newPt); // adds a new point to the list of points, resizes if needed

  void addFaceToElement(int face, int elem); // adds a face to an element
  void removeFaceFromFront(PriorityQueue::iterator&); // deletes a face from the front as well as associated data structures
  int transformCoordinatesToParametricSpace(realArray &xyz_in, realArray &xyz_param); // for parameterized surfacees
	
  void computeFaceTransformation(const Face &face, ArraySimple<real> &T, ArraySimple<real> &Tinv);
  void computeFaceNormalTransformation(const realArray &vertices, ArraySimple<real> &T, double stretch=1.0); // computes a mesh control transformation  based on the current Face`s size and position
  real computeElementQuality(int element);
  void improveQuality();

  bool newElementVertexCheck(const Face &currentFace, vector<int> &local_nodes, 
			     ArraySimpleFixed<real,3,1,1,1> &pc, int filterNode = -1);

  void removeUnusedNodes();

private:

};

// AdvancingFront exceptions ( it would be nice to use namespaces...)
class AdvancingFrontError : public AbstractException
{
public:
  virtual void debug_print() const { cerr<<"\nAdvancingFront Error"; }
};

class BookKeepingError : public AdvancingFrontError
{
public:
  void debug_print() const 
  {
    //AdvancingFront::AdvancingFrontError::debug_print();
    cerr<<": BookKeepingError : Errors found with mesh bookeeping ";
  }
};

class DimensionError : public AdvancingFrontError
{
public:
  void debug_print() const 
  {
    //AdvancingFront::AdvancingFrontError::debug_print();
    cerr<<": DimensionError : Internal error with dimensions used in internal vectors";
  }
};

class AdvanceFailedError : public AdvancingFrontError
{
public:
  void debug_print() const 
  {
    //AdvancingFront::AdvancingFrontError::debug_print();
    cerr<<": AdvanceFailedError : The front is not empty but could not be advanced, probably a bug in the algorithm";
  }
};

class FrontInsertionFailedError : public AdvancingFrontError
{
public:
  void debug_print() const 
  {
    //AdvancingFront::AdvancingFrontError::debug_print();
    cerr<<": FrontInsertionFailedError : A problem occurred inserting a new face into the front, probably a bug in the algorithm";
  }
};


#endif
