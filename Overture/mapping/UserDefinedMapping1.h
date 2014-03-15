#ifndef USER_DEFINED_MAPPING1_H
#define USER_DEFINED_MAPPING1_H "UserDefinedMapping1.h"

#include "Mapping.h"
  
//-------------------------------------------------------------
/// \brief Example of a user defined Mapping.
//-------------------------------------------------------------
class UserDefinedMapping1 : public Mapping
{
public:

  // Here are different options
  enum UserDefinedMappingEnum
  {
    unitSquare,
    helicalWire,
    filletForTwoCylinders,
    blade
  };




  UserDefinedMapping1();

  // Copy constructor is deep by default
  UserDefinedMapping1( const UserDefinedMapping1 &, const CopyType copyType=DEEP );

  ~UserDefinedMapping1();

  UserDefinedMapping1 & operator =( const UserDefinedMapping1 & X0 );

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  void basicInverse( const realArray & x, realArray & r, realArray & rx = Overture::nullRealDistributedArray(),
		    MappingParameters & params =Overture::nullMappingParameters() );

  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  virtual void basicInverseS(const RealArray & x, 
			    RealArray & r,
			    RealArray & rx =Overture::nullRealArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  void getParameters(IntegerArray & ipar, RealArray & rpar ) const;
  void setParameters(const IntegerArray & ipar, const RealArray & rpar );

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return UserDefinedMapping1::className; }

  int update( MappingInformation & mapInfo ) ;

protected:

  UserDefinedMappingEnum mappingType;

  aString className;
  RealArray rp;
  IntegerArray ip;
  
  int  bladeSetup(MappingInformation & mapInfo);

  private:
  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
  virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=((UserDefinedMapping1 &)x); }
  virtual void reference( const ReferenceCounting& x) 
    { reference((UserDefinedMapping1 &)x); }     // *** Conversion to this class for the virtual = ****
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new UserDefinedMapping1(*this, ct); }

};


#endif   


