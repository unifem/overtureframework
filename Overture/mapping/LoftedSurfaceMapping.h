#ifndef LOFTED_SURFACE_MAPPING_H
#define LOFTED_SURFACE_MAPPING_H "LoftedSurfaceMapping.h"

#include "Mapping.h"
  
//-------------------------------------------------------------
// 
/// \brief Define a Mapping as a lofted surface. 
//  
//-------------------------------------------------------------

class LoftedSurfaceMapping : public Mapping
{
public:

  LoftedSurfaceMapping();

  // Copy constructor is deep by default
  LoftedSurfaceMapping( const LoftedSurfaceMapping &, const CopyType copyType=DEEP );

  ~LoftedSurfaceMapping();

  LoftedSurfaceMapping & operator =( const LoftedSurfaceMapping & X0 );

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  void getParameters(IntegerArray & ipar, RealArray & rpar ) const;
  void setParameters(const IntegerArray & ipar, const RealArray & rpar );

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return LoftedSurfaceMapping::className; }

  int update( MappingInformation & mapInfo ) ;

protected:

  aString className;

  // we store parameters for the lofted mapping here: 
  RealArray rp;
  IntegerArray ip;
  
  int profileSetup(MappingInformation & mapInfo);
  int sectionSetup(MappingInformation & mapInfo);

  // This routine defines the cross-section for ship hulls
  int getShipHullCrossSection( const real s, const real t, const real z, const real & zs, const real & zt,
			       real xc[2], real xcs[2], real xct[2] );

  private:
  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
  virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=((LoftedSurfaceMapping &)x); }
  virtual void reference( const ReferenceCounting& x) 
    { reference((LoftedSurfaceMapping &)x); }     // *** Conversion to this class for the virtual = ****
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new LoftedSurfaceMapping(*this, ct); }

};


#endif   


