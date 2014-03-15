#ifndef TRIMMED_MAPPING_H
#define TRIMMED_MAPPING_H 

#include "Mapping.h"
#include "NurbsMapping.h"
//#include "PlotStuff.h"
#include "GenericGraphicsInterface.h"

#define OLDSTUFF
#undef OLDSTUFF

class TMquad;
class TMquadRoot;
class UnstructuredMapping;

//---------------------------------------------------------------
/// \brief Define a Trimmed Mapping:
///    A trimmed surface has curves defined on it (in parameter space)
///  that "remove" parts the surface.
//---------------------------------------------------------------
class TrimmedMapping : public Mapping
{
 public:  // private // ************************************************
  aString className;
  Mapping *surface;      // here is the surface to be trimmed

#ifdef OLDSTUFF
  int numberOfInnerCurves;    // number of curves that trim the surface
  Mapping *outerCurve;
  Mapping **innerCurve;  // Here are the curves that trim the surface  
#else
  Mapping **trimCurves;   // all the curves that trim the surface (including the boundary)
  intArray trimOrientation; // orientations of the curves : 1 clockwise, -1 counterclockwise
  int numberOfTrimCurves; 
#endif

  bool isInitialized() { return upToDate; }
  void setUnInitialized();

  bool trimmingIsValid() { return validTrimming; }
  int  validateTrimming();
  void manuallyValidateTrimming() { validTrimming = true; }
  void invalidateTrimming() { validTrimming = false; } // can only be turned true by call to validateTrimming

  bool verifyTrimCurve( Mapping *curve );

  void initializeTrimCurves();

  aString reportTrimCurveInfo(Mapping *curve, bool &curveok);
  aString reportTrimmingInfo() ;

  bool addTrimCurve(Mapping *trimCurve);
  bool deleteTrimCurve(int curveToDelete);
  bool deleteTrimCurve( int numberOfCurvesToDelete, int *curvesToDelete);
  bool undoLastDelete();

  intArray projectedMask; // mask from mapGrid
  TMquadRoot * quadTreeMesh;
  static real defaultFarthestDistanceNearCurve;
  real farthestDistanceNearCurve;
  
  realArray distanceToBoundary;  // set by the map function
  real rBound[3][2];  // *note* [axis][side]
  
 protected:
  real smallestLengthScale; // Estimate of the smallest length scale in the trimming curves.
  real dRmin, dSmin; // Smallest bounding boxes of any interior trimming curves. Used to set # lines

  bool upToDate;
  bool validTrimming;
  bool allNurbs;
  real *trimmingCurveArcLength;

 public:

  TrimmedMapping();
#ifdef OLDSTUFF
  TrimmedMapping(Mapping & surface, 
                 Mapping *outerCurve=NULL, 
                 const int & numberOfInnerCurves=0, 
                 Mapping **innerCurve=NULL);
#else
  TrimmedMapping(Mapping & surface, 
                 Mapping *outerCurve=NULL, 
                 const int & numberOfInnerCurves=0, 
                 Mapping **innerCurve=NULL);
  TrimmedMapping(Mapping & surface, 
		 const int & numberOfTrimCurves_=0,
		 Mapping **trimCurves_=NULL);
#endif

  // Copy constructor is deep by default
  TrimmedMapping( const TrimmedMapping &, const CopyType copyType=DEEP );

  ~TrimmedMapping();

  TrimmedMapping & operator =( const TrimmedMapping & X0 );

#ifdef OLDSTUFF
  int setCurves(Mapping & surface_, 
		Mapping *outerCurve_  =NULL, 
		const int & numberOfInnerCurves_ =0, 
		Mapping **innerCurve_ =NULL);
#else
  int setCurves(Mapping & surface_, 
		const int & numberOfTrimCurves_=0,
		Mapping **trimCurves=NULL);
#endif
  
  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  // map a grid of points: r(0:n1,1), or r(0:n1,0:n2,2) or r((0:n1,0:n2,0:n3,3) for 1, 2 or 3d
  virtual void mapGrid(const realArray & r, 
		       realArray & x, 
		       realArray & xr,
		       MappingParameters & params=Overture::nullMappingParameters() );

  virtual void basicInverse(const realArray & x, 
			    realArray & r,
			    realArray & rx =Overture::nullRealDistributedArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  virtual const realArray& getGrid(MappingParameters & params=Overture::nullMappingParameters(),
                                   bool includeGhost=false);

  // If boundary curves are made of sub-curves then return the total of all sub-curves
  int getNumberOfBoundarySubCurves();

#ifdef OLDSTUFF
  int getNumberOfInnerCurves();
  int getNumberOfBoundaryCurves();
#else
  int getNumberOfTrimCurves();
#endif 

#ifdef OLDSTUFF
  // access functions for the outer curve and inner curves
  Mapping* getOuterCurve();
  Mapping* getInnerCurve(const int & curveNumber);

#else
  Mapping* getOuterCurve();
  Mapping* getInnerCurve(const int & curveNumber);
  Mapping * getTrimCurve(const int & curveNumber);
#endif

  bool hasTriangulation() const;  // return true if there is a triangulation computed

  UnstructuredMapping & getTriangulation();

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return TrimmedMapping::className; }

  int update( MappingInformation & mapInfo ) ;
  int editTrimCurve(Mapping &trimCurve, MappingInformation & mapInfo ) ;
  int editNurbsTrimCurve(NurbsMapping &trimCurve, MappingInformation & mapInfo ) ;

  // return a pointer to the untrimmed surface
  Mapping* untrimmedSurface() const {return surface;} 

  void triangulate(MappingInformation & mapInfo);  // interactive interface to triangulate.
  void triangulate();

  bool isAllNurbs() const { return allNurbs; }

 public: // protected:// ************************************************

  int setup();

  // initialize and optionally build the quadtree
  void initializeQuadTree(bool buildQuadTree=true );
  // void initialize();
  // void createTrimmedSurface();

  int destroy();
  
  int findClosestCurve(const realArray & x, 
		       intArray & cMin, 
		       realArray & rC,
		       realArray & xC,
                       realArray & dist,                 
		       const int & approximate =true );
  
  int findDistanceToACurve(const realArray & x, 
			   IntegerArray  & cMin, 
			   realArray & dist,
                           const real & delta );
  int insideOrOutside( const realArray & rr, const int & c );
  int insideOrOutside( const realArray & rr, const int & c, realArray & distance );

  bool curveGoesThrough(const TMquad& square, const int& c, int& segstart, int& segstop )
    const;
  bool curveGoesThrough( const TMquad& square, const int& c ) const;
  void setBounds(bool assignBoundsFromTriangulation = true);

  // These next three functions control the triangulation 
  int setMaxAreaForTriangulation( real area=.1 );
  int setMinAngleForTriangulation( real minAngle=20. );
  int setElementDensityToleranceForTriangulation( real elementDensity=.05 );

  int getTriangulationParameters( real &area, real &minAngle, real &elementDensity ) const;

  int snapCurvesToIntersection( GenericGraphicsInterface & gi, NurbsMapping & trimCurve,
                                int &curve1, int &curve2, int curve1End, int curve2End,
				const real *xSelect, const real *c1click );
  
  enum MouseSelectMode { nothing=0,
			 hideCurve,
			 lineSegmentJoin,
			 endpointMove,
			 intersection,     // snap to intersection
			 split,
                         splitAtIntersection,
			 translate,
			 updateCurve,
			 curveAssembly,
			 numberOfMouseModes };

  int assembleSubCurves(int & currentCurve,
			GenericGraphicsInterface & gi, 
			NurbsMapping & trimCurve,
                        NurbsMapping & newCurve,
			int & numberOfAssembledCurves,
			NurbsMapping ** & assemblyCurves,
			MouseSelectMode & mouseMode,
			bool & curveRebuilt,
			bool & plotCurve );

  realArray *rCurve;

  real timeForInsideOrOutside;
  real timeForFindClosestCurve;
  real timeForCreateTrimmedSurface;
  real timeForFindDistanceToACurve;
  real timeForMapGrid;
  real timeForSeg0 ;  // jfp debug
  real timeForSeg1 ;  // jfp debug
  real timeForUntrimmedInverse ;  // jfp debug
  int  callsOfFindClosestCurve;  // jfp debug
  int  callsOfFindClosestCurve_all;  // jfp debug

  UnstructuredMapping *triangulation;
  real minAngleForTriangulation; // use this to reduce allowable angle to we don't get too many triangles: -1=default
  real elementDensityTolerance;  // -1 = use default
  real maxArea;    // approximate maximum area for triangles, 0=use default

  static real defaultMinAngleForTriangulation;
  static real defaultElementDensityToleranceForTriangulation; // num grid pts based on curvature/(this value)
  static real defaultMaximumAreaForTriangulation; // 0=no max area

  private:
     Mapping **oldTrimmingCurves;
     int numberOfOldTrimmingCurves;
     intArray oldTrimOrientations;
  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((TrimmedMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((TrimmedMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new TrimmedMapping(*this, ct); }

};

#endif  
