#ifndef INVERSE_H
#define INVERSE_H "Inverse.h"

#include "Mapping.h"
#include "BoundingBox.h"  // define BoundingBox and BoundingBoxStack

class Mapping;
class IntersectionMapping;
class GenericDataBase;

// ==================================================================================
/// \brief This Class defines fast search algorithms to approximately invert a Mapping.
// ==================================================================================
class ApproximateGlobalInverse
{
 friend class Mapping;
 friend class IntersectionMapping;
 public:
  static const real bogus;   // Bogus value to indicate no convergence

 protected:

  Mapping *map;
  int domainDimension;
  int rangeDimension;
  int uninitialized;
  bool useRobustApproximateInverse;
  
  int gridDefined;  
  RealArray & grid;           // holds grid points, reference to map->grid
  IntegerArray dimension;       // holds dimensions of grid
  IntegerArray indexRange;      // holds index range for edges of the grid, may depend on singularities

  void constructGrid();
  int base,bound;
  int base0,bound0;

  RealArray boundingBox;   // grid is contained in this box
  BoundingBox boundingBoxTree[2][3];  // root of tree of boxes for each side

  BoundingBox *serialBoundingBox;  // in parallel these are bounding boxes for each local grid array.

  // BoundingBox box,box1,box2;
  //  BoundingBoxStack boxStack;

  real boundingBoxExtensionFactor;  // relative amount to increase the bounding box each direction.
  real stencilWalkBoundingBoxExtensionFactor;  // stencil walk need to converge on a larger region
  bool findBestGuess;   // always find the best guess (ignore bounding boxes);

  RealArray xOrigin;
  RealArray xTangent;

  Index Axes;
  Index xAxes;

  // void setGrid( const RealArray & grid, const IntegerArray & gridIndexRange  );

  void getPeriodicImages( const RealArray & x, RealArray & xI, int & nI, 
                          const int & periodicityOfSpace, const RealArray & periodicityVector );


 public:  // ** for now ***
  
  void intersectLine( const RealArray & x, int & nI, RealArray & xI, 
		  const RealArray & vector, const RealArray & xOrigin, const RealArray & xTangent );
  void intersectPlane( const RealArray & x, int & nI, RealArray & xI, 
		  const RealArray & vector, const RealArray & xOrigin, const RealArray & xTangent );
  void intersectCube( const RealArray & x, int & nI, RealArray & xI, 
		  const RealArray & vector, const RealArray & xOrigin, const RealArray & xTangent );

  void initializeBoundingBoxTrees();
  void binarySearchOverBoundary( real x[3], real & minimumDistance, int iv[3], int side=-1, int axis=-1 );
/* ---
  void robustBinarySearchOverBoundary( real x[3], 
				       real & minimumDistance, 
				       int iv[3],
				       int side,
				       int axis  );
--- */
  int insideGrid( int side, int axis, real x[], int iv[], real & dot );

  int distanceToCell( real x[], int iv[], real & signedDistance, const real minimumDistance );
  
  void findNearestGridPoint( const int base, const int bound, RealArray & x, RealArray & r );
  int findNearestCell(real x[3], int iv[3], real & minimumDistance );
  
  void initializeStencilWalk();
  void countCrossingsWithPolygon(const RealArray & x, 
                                 IntegerArray & crossings,
                                 const int & side=Start, 
                                 const int & axis=axis1,
                                 RealArray & xCross = Overture::nullRealArray(),
				 const IntegerArray & mask = Overture::nullIntArray(),
                                 const  unsigned int & maskBit = UINT_MAX,   // (UINT_MAX : all bits on)
				 const int & maskRatio1 =1 ,
				 const int & maskRatio2 =1 ,
				 const int & maskRatio3 =1 );
  
 public:
  ApproximateGlobalInverse( Mapping & map );
  virtual ~ApproximateGlobalInverse();
  virtual void inverse( const RealArray & x, RealArray & r, RealArray & rx, 
		       MappingWorkSpace & workSpace, MappingParameters & params );

  void initialize();     // initialize if not already done so
  void reinitialize();   // this will force a re-initialize the inverse
  const RealArray & getGrid() const;  // return the grid used for the inverse
  const RealArray & getBoundingBox() const;
  const BoundingBox & getBoundingBoxTree(int side, int axis) const;

  real         getParameter( const MappingParameters::realParameter & param ) const;
  int          getParameter( const MappingParameters::intParameter & param ) const;
  virtual void setParameter( const MappingParameters::realParameter & param, const real & value );
  virtual void setParameter( const MappingParameters::intParameter & param, const int & value );
  virtual void useRobustInverse(const bool trueOrFalse=TRUE );
  virtual bool usingRobustInverse() const;

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

 public:
  // these are used for timings and statistics
  static real timeForApproximateInverse,
    timeForFindNearestGridPoint,
    timeForBinarySearchOverBoundary,
    timeForBinarySearchOnLeaves;
  
  static int numberOfStencilWalks,
             numberOfStencilSearches,
             numberOfBinarySearches,
             numberOfBoxesChecked,
             numberOfBoundingBoxes;

  static void printStatistics();

  // these next arrays are for the stencil walk
  static int numberOfStencilDir2D[9];  // (3,3);
  static int stencilDir2D1[8*3*3];    // (8,3,3)
  static int stencilDir2D2[8*3*3];
  static int stencilDir2D3[8*3*3];
  static int numberOfStencilDir3D[27];  // (3,3,3);
  static int stencilDir3D1[27*3*3*3];    // (27,3,3,3)
  static int stencilDir3D2[27*3*3*3];
  static int stencilDir3D3[27*3*3*3];

};



const int maximumNumberOfRecursionLevels=5;

// ==================================================================================
/// \brief Class to define an exact local inverse using Newton's method.
// ==================================================================================
class ExactLocalInverse
{
 private:
  Mapping *map;
  int domainDimension;
  int rangeDimension;
  RealArray periodVector;
  int base,bound;
  Index Axes;
  Index xAxes;
  int uninitialized;
  bool useRobustExactLocalInverse;

  real nonConvergenceValue;  // value given to inverse when there is no convergence
  real newtonToleranceFactor;  // convergence tolerance is this times the machine epsilon
  real newtonDivergenceValue;  // newton is deemed to have diverged if the r value is this much outside [0,1]
  real newtonL2Factor;         // extra factor used in inverting the closest point to a curve or surface
  
  // Work Arrays for Newton:
  // RealArray y,yr,r2,yr2;   // these arrays cannot be static if the inverse is called recursively
  // bool workArraysTooSmall;

  void initialize();
  inline void periodicShift( RealArray & r, const Index & I );
  void underdeterminedLS(const RealArray & xt, 
                         const RealArray & tx,   // *** should not be const -- do for IBM compiler
                         const RealArray & dy,
          		 const RealArray & dr,   // *** should not be const -- do for IBM compiler
                         real & det );

  inline void invert(RealArray & yr, RealArray & dy, RealArray & det, 
                     RealArray & ry, RealArray & dr, IntegerArray & status);
  inline void invertL2(RealArray & yr, RealArray & dy, RealArray & det, RealArray & yr2, RealArray & yrr, 
                       RealArray & ry, RealArray & dr, IntegerArray & status );
  void minimizeByBisection(RealArray & r, RealArray & x, RealArray & dr, IntegerArray & status, real & eps );

 public:
  ExactLocalInverse( Mapping & map );
  virtual ~ExactLocalInverse();
  virtual void inverse( const RealArray & x1, RealArray & r1, RealArray & rx1, 
                       MappingWorkSpace & workSpace, const int computeGlobalInverse=FALSE );

  void reinitialize();

  real         getParameter( const MappingParameters::realParameter & param ) const;
  virtual void setParameter( const MappingParameters::realParameter & param, const real & value );

  virtual void useRobustInverse(const bool trueOrFalse=true );

  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

 public:
  static real timeForExactInverse;  
  static int numberOfNewtonInversions,
             numberOfNewtonSteps;
  
 protected:
  bool mappingHasACoordinateSingularity;

  int compressConvergedPoints(Index & I,
			      RealArray & x, 
			      RealArray & r, 
			      RealArray & ry, 
			      RealArray & det, 
			      IntegerArray & status,
			      const RealArray & x1, 
			      RealArray & r1, 
			      RealArray & rx1, 
			      MappingWorkSpace & workSpace,
			      const int computeGlobalInverse );
  
};




#endif  // "Inverse.h"
