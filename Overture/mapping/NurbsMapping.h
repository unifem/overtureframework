#ifndef NURBS_MAPPING_H
#define NURBS_MAPPING_H 

#include "Mapping.h"
#include <vector>
  
class IgesReader;  // forward declaration
class GenericGraphicsInterface;
class GraphicsParameters;

// ==================================================================================
/// \brief Define a NURBS for a curve, surface or volume.
// ==================================================================================
class NurbsMapping : public Mapping
{

// AP: this doesn't seem necessary anymore:  friend class PlotStuff;
  
public:

  enum ParameterizationTypeEnum  // these are used by the interpolate functions
  {
    parameterizeByChordLength,
    parameterizeByIndex
  };

  enum NurbsConstantsEnum
  {
    maximumOrder=15 // maximum order for nurbs (p1+1) <= maximumOrder
  };
  
  enum FileFormat
  {
    xxww,  // ascii file format is x1,x2,... y1,y2,... z1,z2,..., w1,w2,...
    xwxw,  // ascii file format is x1,y1,z1,w1, x2,y2,z2,w2,...
    cheryl // cheryl's format
  };

  NurbsMapping();
  NurbsMapping(const int & domainDimension , const int & rangeDimension_);  // make a default NURBS

  // Copy constructor is deep by default
  NurbsMapping( const NurbsMapping &, const CopyType copyType=DEEP );

  ~NurbsMapping();

  NurbsMapping & operator =( const NurbsMapping & X0 );

  int addSubCurve(NurbsMapping &nurbs);

  void basicInverse( const realArray & x, realArray & r, realArray & rx = Overture::nullRealDistributedArray(),
		    MappingParameters & params =Overture::nullMappingParameters() );

  virtual void basicInverseS(const RealArray & x, 
			    RealArray & r,
			    RealArray & rx =Overture::nullRealArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  int binomial(const int m, const int n);  // compute m! /( (m-n)! n! )

  // split a curve at corners into sub-curves.
  int buildSubCurves( real angle = 60. );
  
  // build a new Nurbs curve  that matches a coordinate line on the surface
  int buildCurveOnSurface( NurbsMapping & curve, real r0, real r1=-1. );

  // Build a new curve (e.g. x(s)) that represents one component of an existing curve (x(s),y(s),z(s))
  int buildComponentCurve(NurbsMapping & curve, int component =0 );

  // build a circle with centre o, radius r, in the plane defined by the orthogonal unit vectors x,y
  int circle(RealArray & o,
	     RealArray & x, 
	     RealArray & y, 
	     real r,
	     real startAngle=0.,
	     real endAngle=1. );

  // Build a NURBS for a conic defined by end points, two tangents, and an additional point
  int conic( const RealArray &p0, const RealArray &t0, const RealArray &p2, const RealArray &t2, 
             const RealArray &p );

  // Build a NURBS for a conic defined by an implicit formula and two points
  int conic( const real a, const real b, const real c, const real d, const real e, const real f, 
             const real z, const real x1, const real y1, const real x2, const real y2 );

  int deleteSubCurve(int sc);

  virtual void display( const aString & label=blankString) const;

  int elevateDegree(const int increment);

  // merge two nurbs, add a straight line segment if they do not match up closely
  int forcedMerge(NurbsMapping & nurbs  );

  // force a nurbs mapping to be periodic, moving the last control point if neccessary...
  int forcePeriodic();

  // Build a general cylinder (tabulated cylnder) by extruding a curve along a direction vector
  int generalCylinder( const Mapping & curve, real d[3] );

  // Build a general cylinder (tabulated cylnder) by interpolating two curves
  int generalCylinder( const Mapping & curve1, const Mapping & curve2 );

  const RealArray & getControlPoints() const;

  // get uKnot or vKnot, the knots in the first or second direction.
  const RealArray & getKnots( int direction=0 ) const;

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  int get( FILE *file, const FileFormat & fileFormat=xxww );  // read from an ascii file.
  int get(const aString & fileName, const FileFormat & fileFormat=xxww);
  aString getClassName() const { return NurbsMapping::className; }

  virtual const realArray& getGrid(MappingParameters & params=Overture::nullMappingParameters(),
                                   bool includeGhost=false);


  int getNumberOfControlPoints( int axis=0 ) const;  // n+1

  int getNumberOfKnots( int axis=0 ) const;  // m+1

  int getOrder( int axis=0 ) const;  // get order p

  real getOriginalDomainBound(int side, int axis);

  int getParameterBounds( int axis, real & rStart, real & rEnd ) const;

  // insert a knot
  int insertKnot(const real & uBar, const int & numberOfTimesToInsert=1 );

  // construct a NURBS by interpolating another mapping.
  void interpolate(Mapping & map, 
                   int degree=3, 
                   ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
                   int numberOfGhostPoints=0,
                   int *numPointsToInterpolate=NULL );

  // make a nurb that passes through given points, optionally pass parameterization, optionally get parameterization
  void interpolate(const RealArray & x, 
		   const int & option     = 0 ,
		   RealArray & parameterization  =Overture::nullRealArray(),
                   int degree = 3,
                   ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
                   int numberOfGhostPoints=0 );

  // Interpolate an array of points -- specify which points to utilize using the xDimension 
  // and xGridIndexRange arrays
  void interpolate(const RealArray & x, 
                   int domainDimension, int rangeDimension,
		   const IntegerArray & xDimension, const IntegerArray & xGridIndexRange, 
                   ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
		   int *degree = NULL );



  #ifdef USE_PPP
  // obsolete method -- keep for now for backward compatibility
  void interpolate(const realArray & x, 
		   const int & option     = 0 ,
		   realArray & parameterization  =Overture::nullRealDistributedArray(),
                   int degree = 3,
                   ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
                   int numberOfGhostPoints=0 );
  #endif 

  // build a lofted surface by interpolation from a list of mappings
  int interpolateLoftedSurface(std::vector<Mapping *>&, int degree1=3, 
			       int degree2=3,
			       ParameterizationTypeEnum  parameterizationType=parameterizeByChordLength,
			       int numberOfGhostPoints=0 );

  // make a nurb surface that passes through given points. Accessed through the above routine
  void interpolateSurface(const RealArray & x, 
                          int degree = 3,
                          ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
                          int numberOfGhostPoints=0,
			  int degree2 = 3);

  void interpolateVolume(const RealArray & x, 
                         int degree = 3,
			 ParameterizationTypeEnum parameterizationType=parameterizeByChordLength,
                         int numberOfGhostPoints=0  );

  bool isInitialized(){return initialized;};

  bool isSubCurveHidden(int sc);   // is subCurve sc  hidden?

  bool isSubCurveOriginal(int sc); // is the original marker of subCurve sc set?

  // join two consequtive sub-curves and modify the subCurves and subCurveState arrays
  int joinSubCurves( int subCurveNumber );
  
  // join two sub-curves (merge into one)
  int joinSubCurves( int subCurve1, int subCurve2 );

  int line( const RealArray &p1, const RealArray &p2 );

  void lowerRangeDimension();

  Mapping *make( const aString & mappingClassName );

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  // transform using a 3x3 matrix
  int matrixTransform( const RealArray & r );
  
  // merge (join) two nurbs's
  int merge(NurbsMapping & nurbs, bool keepFailed = true, real eps=-1, bool attemptPeriodic=true  );

  int moveEndpoint( int end, const RealArray &endPoint, real tol=-1. );

  // if the Nurb is formed by merging a sequence of Nurbs then function will return that number.
  int numberOfSubCurves() const; // all visible subcurves
  int numberOfSubCurvesInList() const; // all subcurves

  // Indicate that this nurbs is a parametric curve on another nurbs.
  int parametricCurve(const NurbsMapping & nurbs,
                      const bool & scaleParameterSpace=TRUE );

  int parametricSplineSurface(int mu, int mv, RealArray & u, RealArray & v, RealArray & poly );

  // build a plane that passes through three points 
  int plane( real pt1[3], real pt2[3], real pt3[3] );

  int plot(GenericGraphicsInterface & gi, GraphicsParameters & parameters, bool plotControlPoints = FALSE );

  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file
  int put( FILE *file, const FileFormat & fileFormat=xxww );  // save basic NURBS data to a readable ascii file.
  int put(const aString & fileName, const FileFormat & fileFormat=xxww);

  int readFromIgesFile( IgesReader & iges, const int & item, bool normKnots=true );

  int removeKnot(const int & index, 
                 const int & numberOfTimesToRemove, 
                 int & numberRemoved,
		 const real &tol = 100.*FLT_EPSILON);

  // rescale u and v
  int reparameterize(const real & uMin, 
                     const real & uMax,
                     const real & vMin=0.,
                     const real & vMax=1.);

  // rotate about a given axis
  int rotate( const int & axis, const real & theta );

  // scale the NURBS
  int scale(const real & scalex=1., 
	    const real & scaley=1., 
	    const real & scalez=1. );


  int setDomainInterval(const real & r1Start  =0., 
			const real & r1End    =1.,
			const real & r2Start  =0., 
			const real & r2End    =1.,
			const real & r3Start  =0., 
			const real & r3End    =1. );

  // shift in space
  int shift(const real & shiftx=0., 
	    const real & shifty=0., 
	    const real & shiftz=0. );

  // specify a curve in 2D or 3D
  int specify(const int &  m,
	      const int & n,
	      const int & p,
	      const RealArray & uKnot,
	      const RealArray & cPoint,
              const int & rangeDimension=3,
              bool normalizeTheKnots=true );
  // specify a NURBS with domain dimension = 2
  int specify(const int & n1, 
	      const int & n2,
	      const int & p1, 
	      const int & p2, 
	      const RealArray & uKnot, 
	      const RealArray & vKnot,
	      const RealArray & controlPoint,
	      const int & rangeDimension =3,
              bool normalizeTheKnots=true );

  // split the nurb into two, return the pieces but do not alter the original nurb
  int split(real uSplit, NurbsMapping &c1,  NurbsMapping&c2, bool normalizePieces =true);

  // split a sub-curve at a given position
  int splitSubCurve( int subCurveNumber, real rSplit );

  // Here is the sub curve.
  NurbsMapping& subCurve(int subCurveNumber); // only visible subcurves (Make arg non-ref)

  NurbsMapping& subCurveFromList(int subCurveNumber); // access all subcurves (Make arg non-ref)

  int toggleSubCurveVisibility(int sc);
  void toggleSubCurveOriginal(int sc);

  // apply a scaling and shift to the knots
  int transformKnots(const real & uScale, 
		     const real & uShift,
		     const real & vScale  =1., 
		     const real & vShift  =0. );
  
  // apply a scaling and shift to the control points (scales and shifts the NURBS)
  int transformControlPoints(const RealArray & scale,
			     const RealArray & shift);

  // clip the nurb to the domain bounds ( ie, get rid of any extra control points and knots )
  int truncateToDomainBounds();

  virtual int update( MappingInformation & mapInfo );


 protected:

  real angle( const RealArray &p0, const RealArray &p1, const RealArray &p2 ) const;

  void initialize( bool setMappingHasChanged=true );

  int normalizeKnots();

  int makeOneArc( const RealArray &p0, const RealArray &t0, const RealArray &p2, const RealArray &t2, 
                  const RealArray &p, RealArray &p1, real&w1 ) const;

  int splitArc(const RealArray &p0, const RealArray &p1, const real & w1, const RealArray &p2, 
               RealArray &q1, RealArray &s, RealArray &r1, real &wqr) const;

  int intersect3DLines( const RealArray & p0, const RealArray &  t0, 
			const RealArray & p1, const RealArray & t1,
			real & alpha0, real & alpha1,
			RealArray & p2) const;
  
  real distance4D( const RealArray & x, const RealArray & y );

  // vectorized version of map -- not normally used  --
  virtual void mapVector(const RealArray & r, RealArray & x, RealArray & xr = Overture::nullRealArray(),
                         MappingParameters & params =Overture::nullMappingParameters() );

  void setBounds(); // set bounds for plotting etc.
  
  void privateInterpolate( const RealArray & x, const RealArray *uBar );

 private:

  aString className;
  int n1,m1,p1;   
  int n2,m2,p2;
  int n3,m3,p3; // kkc 051031
  RealArray uKnot,vKnot,wKnot,cPoint; // kkc added wKnot 051031
  bool initialized;
  bool nonUniformWeights; // true if the weights are not all constant
  real uMin,uMax,vMin,vMax,wMin,wMax;  // original knot extent (they are scaled to [0,1])
  int nurbsIsPeriodic[3];  // for reparameterization, remember original periodicity.
  real rStart[3], rEnd[3];  // for reparameterization

  bool mappingNeedsToBeReinitialized;

  // if the Nurb is formed by merging a sequence of Nurbs then we may keep the
  // Nurbs that were merged for later use.

  int numberOfCurves;
  NurbsMapping **subCurves;
  char *subCurveState;
  int lastVisible;

 public:
  bool use_kk_nrb_eval;

  static bool useScalarEvaluation;

  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((NurbsMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((NurbsMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new NurbsMapping(*this, ct); }




  int findSpan(const int & n ,
	       const int & p,
	       const real & u,
	       const RealArray & uKnot );
  void basisFuns(const int & i,
		 const real & u,
		 const int & p,
		 const RealArray & uKnot,
		 RealArray & basis );
  void dersBasisFuns(const int & i,
		     const real & u,
		     const int & p,
		     const int & order,
		     const RealArray & uKnot,
		     real *ders );
  
  // vectorized versions
  void findSpan(const int & n ,
		const int & p,
		const Index & I,
		const RealArray & u,
		const RealArray & knot,
		IntegerArray & span );

  void dersBasisFuns(const Index & I,
		     const IntegerArray & ia,
		     const RealArray & u,
		     const int & p,
		     const int & order,
		     const RealArray & knot,
		     RealArray & ders );
};


#endif  
