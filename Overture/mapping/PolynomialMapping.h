#ifndef POLYNOMIAL_MAPPING_H
#define POLYNOMIAL_MAPPING_H "Polynomial.h"

#include "Mapping.h"
  
//-------------------------------------------------------------
/// \brief Mapping to define a polynomial curve.
//-------------------------------------------------------------
class PolynomialMapping : public Mapping
{
public:

  PolynomialMapping( int numberOfDimensions =2 );

  // Copy constructor is deep by default
  PolynomialMapping( const PolynomialMapping &, const CopyType copyType=DEEP );

  ~PolynomialMapping();

  PolynomialMapping & operator =( const PolynomialMapping & X0 );

  void setCoefficients(const RealArray & coeff);

  void getCoefficients(RealArray & coeff) const; 

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file


  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return PolynomialMapping::className; }

  int update( MappingInformation & mapInfo ) ;

private:

  aString className;
  RealArray pc;
  
  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((PolynomialMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((PolynomialMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new PolynomialMapping(*this, ct); }

};


#endif   // POLYNOMIAL_H


