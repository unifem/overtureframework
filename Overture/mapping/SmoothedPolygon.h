#ifndef SMOOTHEDPOLYGON_H
#define SMOOTHEDPOLYGON_H "SmoothedPolygon.h"

#include "MappingRC.h"

//-------------------------------------------------------------
/// \brief Define a curve or 2D grid using a smoothed polygon.
//-------------------------------------------------------------

class SmoothedPolygon : public Mapping
{

public:

SmoothedPolygon();

// Copy constructor is deep by default
SmoothedPolygon( const SmoothedPolygon &, const CopyType copyType=DEEP );

~SmoothedPolygon();

int update( MappingInformation & mapInfo );  

SmoothedPolygon & operator =( const SmoothedPolygon & X );

void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
	  MappingParameters & params =Overture::nullMappingParameters());

virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
		   MappingParameters & params =Overture::nullMappingParameters());

virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

Mapping *make( const aString & mappingClassName );
aString getClassName() const { return SmoothedPolygon::className; }

// Define the verticies and other properties of the smooth polygon
int setPolygon( const RealArray & xv,
		const RealArray & sharpness = Overture::nullRealArray(),
		const real normalDist = 0. ,
		const RealArray & variableNormalDist = Overture::nullRealArray(),
		const RealArray & tStretch = Overture::nullRealArray(),
		const RealArray & rStretch = Overture::nullRealArray(),
		const bool correctForCorners = false );

protected:

void setDefaultValues();
void initialize();
void assignAdoptions();

int correctPolygonForCorners();

private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=((SmoothedPolygon &)x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((SmoothedPolygon &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new SmoothedPolygon(*this, ct); }

aString className;
bool mappingIsDefined;

// *** we should clean up all the extra arrays in this class ***

int numberOfVertices;
RealArray vertex,arclength;
int NumberOfRStretch;
int userStretchedInT,correctCorners;
RealArray bv,sab,r12b,sr,corner;
int numberOfRStretch;
real normalDistance;                    
real zLevel;
real splineResolutionFactor;  // default 5. : numberOfSplinePoints=splineResolutionFactor*numberOfGridsPoints

int ndiwk,ndrwk;
IntegerArray iwk;
RealArray rwk;

// no need to save the following:
IntegerArray iw;  
RealArray w;  
int ndi;
int ndr;
int nsm1;
RealArray bx, by, ccor;

};


#endif   // SMOOTHEDPOLYGON_H
