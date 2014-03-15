#ifndef CHANNEL_H
#define CHANNEL_H "Channel.h"

#include "Mapping.h"
  
class ChannelMapping : public Mapping
{
//-------------------------------------------------------------
//  Here is a derived class to define a Channel in 2D
//-------------------------------------------------------------
private:
  aString className;
  real xa,xb,ya,yb;

public:

  ChannelMapping(const real xa=0., 
		 const real xb=1., 
		 const real ya=0.,
                 const real yb=1. );

  // Copy constructor is deep by default
  ChannelMapping( const ChannelMapping &, const CopyType copyType=DEEP );

  ~ChannelMapping();

  ChannelMapping & operator =( const ChannelMapping & X0 );

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return ChannelMapping::className; }

  int update( MappingInformation & mapInfo ) ;

  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((ChannelMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((ChannelMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor(const CopyType ct = DEEP) const
      { return ::new ChannelMapping(*this, ct); }

};


#endif   // CHANNEL_H


