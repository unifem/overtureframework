//
// Stretching 
//
#ifndef STRETCH_H
#define STRETCH_H "Stretch.h"

#include "Mapping.h"
#include "mathutil.h"

//-------------------------------------------------------------
/// \brief Define stretching routines to cluster grid points including
///  inverse-hyperbolic-tangent, hyperbolic-tangent, exponent, exponential-blend,
///  and exponential-to-linear.
//------------------------------------------------------------
class StretchMapping : public Mapping{


 public:

  enum StretchingType
  {
    inverseHyperbolicTangent=0,
    hyperbolicTangent,
    exponential,
    exponentialBlend,
    exponentialToLinear, 
    noStretching,
    linearSpacing, // from Philip Blakely -- not really linear
    numberOfStretchingTypes
  };

  StretchMapping( const StretchingType & stretchingType=noStretching );  
  StretchMapping( const int numberOfLayers, const int numberOfIntervals=0 );  

  // Copy constructor is deep by default
  StretchMapping( const StretchMapping &, const CopyType copyType=DEEP );

  ~StretchMapping();

  //  assignment with = is a deep copy
  StretchMapping & operator =( const StretchMapping & X );

  int setStretchingType(  const StretchingType & stretchingType );

  int setNumberOfLayers( const int numberOfLayers );

  int setNumberOfIntervals( const int numberOfIntervals );

  int setLayerParameters( const int index, const real a, const real b, const real c );

  int setIntervalParameters( const int index, const real d, const real e, const real f );

  int setIsNormalized( const bool & trueOrFalse=TRUE );
  
  int setEndPoints( const real rmin, const real rmax );

  int setScaleParameters( const real origin, const real scale );

  int setNumberOfSplinePoints( const int numberOfSplinePoints );

  int setIsPeriodic( const int trueOrFalse );
  virtual void setIsPeriodic( const int axis, const periodicType isPeriodic );

  int setHyperbolicTangentParameters(const real & a0, 
				     const real & ar, 
				     const real & a1, 
				     const real & b1, 
				     const real & c1);

  int setExponentialParameters(const real & a0, 
			       const real & ar,
			       const real & a1, 
			       const real & b1, 
			       const real & c1);

  int setExponentialToLinearParameters(const real & a, 
				       const real & b,
				       const real & c );

  int setLinearSpacingParameters(const real & a0, const real & a1);

  int initialize( );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping* make( const aString & mappingClassName );

  aString getClassName() const { return StretchMapping::className; }

  void display( const aString & label = blankString ) const;

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  void inverseMap( const realArray & x, realArray & r, realArray & rx = Overture::nullRealDistributedArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void basicInverse( const realArray & x, realArray & r, realArray & rx = Overture::nullRealDistributedArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  void inverseMapS( const RealArray & x, RealArray & r, RealArray & rx = Overture::nullRealArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void basicInverseS(const RealArray & x, 
			    RealArray & r,
			    RealArray & rx =Overture::nullRealArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  int update( MappingInformation & mapInfo );  

 protected:

  aString className;
  IntegerArray iw;
  int numberOfLayers;
  int numberOfIntervals;
  int numberOfSplinePoints;
  int unInitialized;
  RealArray rw;
  int iopt,ierr;
  real ra,rb,r0,r1;
  RealArray abc;
  RealArray def;

  StretchingType stretchingType;
  bool normalized;
  real origin,scale;
  real a0,ar,a1,b1,c1;
  real a0Normalized,arNormalized,a1Normalized;  
  
  void setup();

  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=( (StretchMapping & )x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((StretchMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new StretchMapping(*this, ct); }
};



#endif // STRETCH_H
