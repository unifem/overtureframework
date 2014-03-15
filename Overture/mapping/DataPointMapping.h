#ifndef DATAPOINTMAPPING_H
#define DATAPOINTMAPPING_H "DataPointMapping.h"

#include "MappingRC.h"

class GenericGraphicsInterface;
class GraphicsParameters;

//-------------------------------------------------------------------------
/// \brief  Define a Mapping by interpolation of data points.
//-------------------------------------------------------------------------
class DataPointMapping : public Mapping
{
  
public:

  DataPointMapping();
  
  // Copy constructor is deep by default
  DataPointMapping( const DataPointMapping &, const CopyType copyType=DEEP );

  ~DataPointMapping();

  // supply data points as xd(0:d,I,J[,K]) or xd(I,J,K,0:d-1) or xd(I,J,0:d-1) etc, d=number of dimensions
  int setDataPoints(const realArray & xd, 
		    const int positionOfCoordinates = 3, 
		    const int domainDimension  = -1,
		    const int numberOfGhostLinesInData = 0,
                    const IntegerArray & xGridIndexRange = Overture::nullIntArray() );
  int setDataPoints(const realArray & xd, 
		    const int positionOfCoordinates, 
		    const int domainDimension,
		    const int numberOfGhostLinesInData[2][3],
                    const IntegerArray & xGridIndexRange = Overture::nullIntArray() );
  int setDataPoints(const aString & fileName );  // set data points from a file of data
  int setMapping( Mapping & map );               // acquire data points from this mapping.

  const realArray& getDataPoints();           // return the array of data points
  const IntegerArray & getGridIndexRange();   // return the gridIndexRange(0;1,0:2) for the data points
  const IntegerArray & getDimension();        // return the dimension(0;1,0:2) array for the data points

  void setOrderOfInterpolation( const int order ); // 2 or 4
  int getOrderOfInterpolation(); 
  void useScalarArrayIndexing(const bool & trueOrFalse = FALSE);

  // these next two should be in the base class.
  int specifyTopology(GenericGraphicsInterface & gi, GraphicsParameters & params );
  virtual int setTopologyMask(real cGridTolerance=-1.);

  int update( MappingInformation & mapInfo );  

  DataPointMapping & operator =( const DataPointMapping & X );

  void map( const realArray & r, realArray & x, realArray & xr = Overture::nullRealDistributedArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  // we define a fast inverse for linear interpolation
  virtual void basicInverse(const realArray & x, 
			    realArray & r,
			    realArray & rx =Overture::nullRealDistributedArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  virtual void basicInverseS(const RealArray & x, 
			    RealArray & r,
			    RealArray & rx =Overture::nullRealArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping *make( const aString & mappingClassName );
  aString getClassName() const { return DataPointMapping::className; }

  virtual const realArray& getGrid(MappingParameters & params=Overture::nullMappingParameters(),
                                   bool includeGhost=false);    // grid for plotting

  virtual int setNumberOfGhostLines( IndexRangeType & numberOfGhostLinesNew ); // set the number of ghost lines.

  int projectGhostPoints(MappingInformation & mappingInfo);

  virtual real sizeOf(FILE *file = NULL ) const;

 protected:
  void mapScalar(const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params,
                 int base, int bound, bool computeMap, bool computeMapDerivative ); 

//   void mapVector(const realArray & r, realArray & x, realArray & xr, MappingParameters & params,
//                  const Index & I);

  int computeGhostPoints( IndexRangeType & numberOfGhostLinesOld, IndexRangeType & numberOfGhostLinesNew );

private:
  aString className;
  int orderOfInterpolation;
  friend class HyperbolicMapping;
  realArray xy;              // data points defining the mapping 
  real delta[3],deltaByTwo[3];
  IntegerArray gridIndexRange,dimension;

  // local variables (these do not have to be saved with get/put)
  bool mappingInitialized;
  bool useScalarIndexing;

  private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=( (DataPointMapping &) x); }
    virtual void reference( const ReferenceCounting& x) 
      { reference((DataPointMapping &)x); }     // *** Conversion to this class for the virtual = ****
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new DataPointMapping(*this, ct); }
};


#endif   // DATAPOINTMAPPING_H
