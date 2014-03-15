#ifndef AIRFOIL_H
#define AIRFOIL_H "Airfoil.h"

#include "Mapping.h"
  
// ==================================================================================
/// \brief Define an curve for an airfoil such as arc, NACA or Joukowsky.
// ==================================================================================
class AirfoilMapping : public Mapping
{

public:

  enum AirfoilTypes
  {
    arc,
    sinusoid,
    diamond,
    naca,
    joukowsky
  };

  AirfoilMapping(const AirfoilTypes & aft=arc, 
		 const real xa=-1.5, 
		 const real xb=1.5, 
		 const real ya=0., 
		 const real yb=2. );

  // Copy constructor is deep by default
  AirfoilMapping( const AirfoilMapping &, const CopyType copyType=DEEP );

  ~AirfoilMapping();

  AirfoilMapping & operator =( const AirfoilMapping & X0 );

  void map(const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
	   MappingParameters & params=Overture::nullMappingParameters());

  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  // set bounds on the rectangle that the airfoil sits in
  int setBoxBounds(const real xa=-1.5, 
		   const real xb=1.5, 
		   const real ya=0., 
		   const real yb=2. );
 
  int setParameters(const AirfoilTypes & airfoilType_,
                    const real & chord=1., 
                    const real & thicknessToChordRatio=.1,
                    const real & maximumCamber=0.,
                    const real & positionOfMaximumCamber=0.,
                    const real & trailingEdgeEpsilon =.02,
                    const real & sinusoidPower = 1. );

  int setJoukowskyParameters( const real & a, const real & d, const real & delta );
  
  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return AirfoilMapping::className; }

  int update( MappingInformation & mapInfo ) ;

 protected:
  aString className;
  RealArray xBound;
  real chord;
  real thicknessToChordRatio;  // ratio of airfoil thickness to chord
  real sinusoidPower;          // sin(...)^sinusoidPower

  real maximumCamber,positionOfMaximumCamber;  // addition parameters for NACA 4 airfoil
  real trailingEdgeEpsilon;

//  realArray yc,yt,theta, ytt, yct, thetat;
  AirfoilTypes airfoilType;
  real joukowskyDelta,joukowskyD,joukowskyA;

  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((AirfoilMapping &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((AirfoilMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new AirfoilMapping(*this, ct); }

};


#endif   // AIRFOIL_H


