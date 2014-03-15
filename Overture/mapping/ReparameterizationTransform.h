#ifndef REPARAMETERIZATION_TRANSFORM
#define REPARAMETERIZATION_TRANSFORM

#include "ComposeMapping.h"

class OrthographicTransform;

// ================================================================================
/// \brief Reparameterize another Mapping to create a new Mapping. Options are 
///    orthographic transform (to remove a polar singularity), restriction (to choose a sub-domain),
///    equidistribution (to redistribute grid points evenly), and reorient the parameter axes.
// ================================================================================
class ReparameterizationTransform : public ComposeMapping
{
//----------------------------------------------------------------
//  Reparameterize a Mapping in various ways
//  ----------------------------------------
//
//----------------------------------------------------------------
 public:

  enum ReparameterizationTypes
  {
    defaultReparameterization,
    orthographic,
    restriction,
    equidistribution,
    reorientDomainCoordinates
  };


 public:

  ReparameterizationTransform( );
  //
  // Constructor, supply a Mapping to reparameterize
  // It will replace multiple reparams with just one reparam if map is actually of this class
  ReparameterizationTransform(Mapping & map,     const ReparameterizationTypes type=defaultReparameterization );
  ReparameterizationTransform(MappingRC & mapRC, const ReparameterizationTypes type=defaultReparameterization );

  // Copy constructor is deep by default
  ReparameterizationTransform( const ReparameterizationTransform &, const CopyType copyType=DEEP );

  // Copy like constructor that makes a deep copy of all but the transformed grid which is replaced
  ReparameterizationTransform( const ReparameterizationTransform &, MappingRC & map );

  ~ReparameterizationTransform();

  ReparameterizationTransform & operator =( const ReparameterizationTransform & X );

  // set equidistribution parameterization parameters
  int setEquidistributionParameters(const real & arcLengthWeight=1., 
                                    const real & curvatureWeight=0.,
                                    const int & numberOfSmooths = 3 );
  
  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  void inverseMap( const realArray & x, realArray & r, realArray & rx = Overture::nullRealDistributedArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void basicInverse( const realArray & x, realArray & r, realArray & rx= Overture::nullRealDistributedArray(), 
                     MappingParameters & params=Overture::nullMappingParameters());

  void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
             MappingParameters & params =Overture::nullMappingParameters());

  void inverseMapS( const RealArray & x, RealArray & r, RealArray & rx = Overture::nullRealArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void basicInverseS(const RealArray & x, 
		     RealArray & r,
		     RealArray & rx =Overture::nullRealArray(),
		     MappingParameters & params =Overture::nullMappingParameters());


  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping* make( const aString & mappingClassName );

  aString getClassName() const { return ReparameterizationTransform::className; }

  Mapping *getReparameterizeMapping() const { return reparameterize;} 

  int update( MappingInformation & mapInfo ) ;

  // scale the current bounds for a restriction mapping
  int scaleBounds(const real ra=0., 
		  const real rb=1., 
		  const real sa=0.,
		  const real sb=1.,
		  const real ta=0.,
		  const real tb=1. );
  // set absolute bounds for a restriction mapping
  int setBounds(const real ra=0., 
		const real rb=1., 
		const real sa=0.,
		const real sb=1.,
		const real ta=0.,
		const real tb=1. );

  int getBounds(real & ra, real & rb, real & sa, real & sb, real & ta, real & tb ) const;

  int getBoundsForMultipleReparameterizations(real & ra, real & rb, real & sa, real & sb, real & ta, real & tb ) const;

  int getBoundsForMultipleReparameterizations( real mrBounds[6] ) const;
  int setBoundsForMultipleReparameterizations( real mrBounds[6] );
  
  virtual RealArray getBoundingBox( const int & side=-1, const int & axis=-1 ) const;
  virtual int       getBoundingBox( const IntegerArray & indexRange, const IntegerArray & gridIndexRange,
                                    RealArray & xBounds, bool local=false ) const;
  virtual int       getBoundingBox( const RealArray & rBounds, RealArray & xBounds ) const;

  enum 
  {
    maximumNumberOfRecursionLevels=10
  };



protected:
  void constructor(Mapping & map, const ReparameterizationTypes type);
  void constructorForMultipleReparams(ReparameterizationTransform & map );
  void setMappingProperties(Mapping *mapPointer);

  int initializeEquidistribution(const bool & useOriginalMapping = TRUE);

protected:
  aString className;
  Mapping *reparameterize;                 // points to either orthographic or restriction or Reorient
public:
  // needed so we can invert in a different coordinate system: 
  // (make public so we can delete in initStaticMappingVariables)
  static MappingParameters *localParams[maximumNumberOfRecursionLevels];  
protected:
  static int localParamsAreBeingUsed[maximumNumberOfRecursionLevels];
  int coordinateType; 
  ReparameterizationTypes reparameterizationType;
  real arcLengthWeight, curvatureWeight;
  int numberOfEquidistributionSmooths;
  bool equidistributionInitialized;
  real mr[6];    // for multiple compositions of restriction mappings -- keep scaling to original

private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((ReparameterizationTransform &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((ReparameterizationTransform &)x); }    
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new ReparameterizationTransform(*this, ct); }
  };


#endif
