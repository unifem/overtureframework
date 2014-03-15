#ifndef REORIENT_H
#define REORIENT_H "Reorient.h"

#include "Mapping.h"
class ReparameterizationTransform;
  
//-------------------------------------------------------------
/// \brief This mapping can be used to re-orient the order of the domain parameter space.
/// For example, it can change a mapping x(r,s,t) -> x(s,r,t) or x(t,s,r) etc.
// 
//-------------------------------------------------------------
class ReorientMapping : public Mapping
{
  friend class ReparameterizationTransform;
  
private:
  aString className;
  int dir1,dir2,dir3;  // permutation of (0,1,2)


public:

  ReorientMapping(const int dir1=0, const int dir2=1, const int dir3=2,
                  const int dimension=2 );    // 2D by default

  // Copy constructor is deep by default
  ReorientMapping( const ReorientMapping &, const CopyType copyType=DEEP );

  ~ReorientMapping();

  ReorientMapping & operator =( const ReorientMapping & X0 );

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

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return ReorientMapping::className; }

  int update( MappingInformation & mapInfo ) ;

  // scale the current bounds
  int setOrientation(const int dir1, const int dir2, const int dir3=-1);
  int getOrientation(int & dir1, int & dir2, int & dir3) const;
  
  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((ReorientMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((ReorientMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new ReorientMapping(*this, ct); }

};


#endif   // REORIENT_H


