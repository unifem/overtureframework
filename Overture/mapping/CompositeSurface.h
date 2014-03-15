#ifndef COMPOSITE_SURFACE_H
#define COMPOSITE_SURFACE_H

#include "Mapping.h"
#include "ListOfMappingRC.h"
  
class GenericGraphicsInterface;
class GraphicsParameters;
class MappingProjectionParameters;
class CompositeTopology;

//---------------------------------------------------------------
/// \brief  Define a Composite Surface consisting of a set of Mappings.
//---------------------------------------------------------------
class CompositeSurface  : public Mapping
{
 public:

  CompositeSurface();

  // Copy constructor is deep by default
  CompositeSurface( const CompositeSurface &, const CopyType copyType=DEEP );

  ~CompositeSurface();

  CompositeSurface & operator =( const CompositeSurface & X0 );

   
  // return the Mapping that represents a subSurface
  Mapping & operator []( const int & subSurfaceIndex ); // why is subSurfaceIndex passed by reference???

  // add a surface to the composite surface
  int add( Mapping & surface,
	   const int & surfaceID = -1 );

  int findBoundaryCurves(int & numberOfBoundaryCurves, Mapping **& boundaryCurves );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file

  aString getClassName() const { return CompositeSurface::className; }

  aString getColour( const int & surfaceNumber ) const;

  CompositeTopology* getCompositeTopology(bool alloc=false) ;

// this function is needed in ogshow/plotCompositeSurface.C
  int getNormals(const intArray & subSurfaceIndex, const realArray & xr, realArray & normal) const;

  int &getSignForNormal(int s) const;

  // get the tolerance
  real getTolerance() const;

  // determine if points are inside or outside a (closed) triangulation using ray tracing
  int insideOrOutside( realArray & x, IntegerArray & inside );

  // Is this surface visible?
  int isVisible(const int & surfaceNumber) const;

  Mapping* make( const aString & mappingClassName );

  virtual void map( const realArray & r, 
		    realArray & x, 
		    realArray & xr = Overture::nullRealDistributedArray(),
		    MappingParameters & params =Overture::nullMappingParameters() );

  int numberOfSubSurfaces() const;

  int printStatistics(FILE *file=stdout);

  // project points onto the surface
  void oldProject( intArray & subSurfaceIndex,
		   realArray & x, 
		   realArray & rProject, 
		   realArray & xProject,
		   realArray & xrProject,
		   realArray & normal = Overture::nullRealDistributedArray(),
		   const intArray & ignoreThisSubSurface  = Overture::nullIntegerDistributedArray() );

  // project points onto the surface
  int project( realArray & x, MappingProjectionParameters & mpParameters );

  // project points onto the surface, old interface:
  void project( intArray & subSurfaceIndex,
		realArray & x, 
		realArray & rProject, 
		realArray & xProject,
		realArray & xrProject,
                realArray & normal = Overture::nullRealDistributedArray(),
                const intArray & ignoreThisSubSurface  = Overture::nullIntegerDistributedArray(),
                bool invertUntrimmedSurface = false );

  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  
  // refine the grid spacing or trianglation of a sub-surface 
  int refineSubSurface( const int s );

  // remove a surface from the composite surface
  int remove( const int & surfaceNumber );

  // recompute the bounding box
  void recomputeBoundingBox();

  // set the colour of a subSurface.
  int setColour( const int & surfaceNumber, const aString & colour );

  // set the visibility
  int setIsVisible(const int & surfaceNumber, const bool & trueOrFalse=TRUE);

  // set the tolerance for how well the surfaces match (may come from the CAD file)
  int setTolerance(real tol);

  int update( MappingInformation & mapInfo ) ;

  void updateTopology();


enum DisplayListProperty 
{
  boundary=0, gridLines, shadedSurface, numberOfDLProperties
};

IntegerArray dList; // int i; DisplayListContents j; dList(j,i) == display list number for surface i, feature j

void
eraseCompositeSurface(GenericGraphicsInterface &gi, int surface = -1);

bool
isTopologyDetermined() const { return topologyDetermined; }

//kkc try to automagically compute the topology 
 bool computeTopology(GenericGraphicsInterface &gi);

int
getSurfaceID(int s) const { return surfaceIdentifier(s); }

bool plotGhostLines;  // normally we do NOT plot ghost lines (takes too long for trimmed surafces)

protected:
  void initialize();
  void findNearbySurfaces( const int & s, 
                           realArray & r,
			   const bool & doubleCheck,
			   IntegerArray & consistent,
			   IntegerArray & inconsistent );
  int  findOutwardTangent( Mapping & map, const realArray & r,  const realArray & x, realArray & outwardTangent );
  
  IntegerArray visible;  // is a sub surface visible
  IntegerArray surfaceIdentifier;  // user defined identifier for the surface
  aString *surfaceColour;   // colour for the subSurface.
  real tolerance;           // holds tolerance for surfaces (from IGES file for example)
  
  enum TimingEnum
  {
    totalTime,
    timeToProject, 
    timeToProjectInvertMapping, 
    timeToProjectEvaluateMapping,
    numberOfTimings
  };
  real timing[numberOfTimings];


 private:

  aString className;
  int numberOfSurfaces;

  ListOfMappingRC surfaces;   // list of surfaces

  CompositeTopology *compositeTopology;
  IntegerArray signForNormal; // multiply normal by this (+1,-1) so that all normals are consistent
  bool topologyDetermined;
  void 
  determineTopology();

 private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((CompositeSurface &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((CompositeSurface &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new CompositeSurface(*this, ct); }

};


#endif  
