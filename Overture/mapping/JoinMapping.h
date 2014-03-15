#ifndef JOIN_MAPPING_H
#define JOIN_MAPPING_H 

#include "IntersectionMapping.h"
  
//-------------------------------------------------------------
/// \brief Define a Mapping that joins two intersecting mappings.
/// 
/// \details
///  Define a mapping that tranforms a "source-mapping" so that
/// it intersects another "clip-surface" "exactly". For example,
/// a Mapping for a wing (source)  can be joined to a fuselage (clip-surface).
//
//-------------------------------------------------------------
class JoinMapping : public Mapping
{
public:

  enum JoinType    // There are two types of joins
  {
    parametric=0,
    nonParametric
  };

  JoinMapping();
  JoinMapping(Mapping & sourceMapping, 
	      Mapping & clipSurface);

  // Copy constructor is deep by default
  JoinMapping( const JoinMapping &, const CopyType copyType=DEEP );

  ~JoinMapping();

  JoinMapping & operator =( const JoinMapping & X0 );

  int setCurves(Mapping & sourceMapping, 
		Mapping & clipSurface);

  int setEndOfJoin( const real & endOfJoin );
  
  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return JoinMapping::className; }

  int update( MappingInformation & mapInfo ) ;

 protected:
  aString className;
  Mapping *curve[2];
  bool uninitialized;
  IntersectionMapping inter, inter2;
  int numberOfIntersections;       // number of intersection points/curves
  int intersectionToUse;           // make a join at this intersection
  bool intersectionFound;          // true if an intersection curve has been found.
  real endOfJoin;                  // r value for end of the join
  // for intersection of curves:
  bool newCurves;
  JoinType joinType;
  
  Mapping *line[2], *tfi[3], *join, *join1, *join2, *join3, *surface1, *surface2, *surface3;

  void setup();
  void initialize();
  
 private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((JoinMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((JoinMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new JoinMapping(*this, ct); }

};


#endif  
