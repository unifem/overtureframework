#ifndef MAPPING_RC_H
#define MAPPING_RC_H "MappingRC.h"

#include "Mapping.h"

class MappingInformation;

//=================================================================
//     Reference Counted Class for class Mapping
//
//  Use this class to hold a pointer to a mapping. An object of this
// class behaves in many ways like a Mapping since all the member
// functions of the base class are directly available. This class is
// needed since the Mapping class is not have full support for 
// reference counting since we want to simplify the process of
// writing new Mappings by derivation.
//
//
//  Examples
//    
//    SquareMapping square;               // here is a regular mapping
//    MappingRC maprc(square);            // make a MappingRC that points to square
//    ...
//    maprc.map(r,x,xr );                 // evaluate like a regular mapping
//
//    MappingRC maprc2("SquareMapping");  // make a mapping with the given class name
//    maprc2=square;                      // deep copy
//    maprc2=maprc3;                      // deep copy
//
//    MappingRC maprc3;
//    maprc3.reference(square);           // reference to a Mapping
//    maprc3.reference(maprc2);           // reference to a MappingRC
//    ...
//    maprc3.breakReference();
//
//
//================================================================ 
typedef Mapping::coordinateSystem      coordinateSystem;
typedef Mapping::mappingSpace          mappingSpace;
typedef Mapping::periodicType          periodicType;
typedef Mapping::coordinateSingularity coordinateSingularity;
typedef Mapping::basicInverseOptions   basicInverseOptions;
typedef Mapping::topologyEnum          topologyEnum;

// ===================================================================================================
/// \brief Define a class that can reference count Mappings.
// ===================================================================================================
class MappingRC : public ReferenceCounting
{ 
 public:

  enum DataBaseModeEnum
  {
    doNotLinkMappings=0,
    linkMappings
  };

  
  static int setDataBaseMode(DataBaseModeEnum mode);
  static DataBaseModeEnum getDataBaseMode();

  Mapping *mapPointer;

  // construct a mapping with the given Class name, standard Mapping by default
  MappingRC( const aString & mappingClassName=nullString ); 

  MappingRC( Mapping & map ); // constructor, assign pointer to the given mapping

  ~MappingRC();

  // ----- copy constructor, deep copy by default----
  MappingRC( const MappingRC & maprc, const CopyType copyType=DEEP );
  
  // ---Assignment operator : deep copy ---
  virtual MappingRC & operator=( const MappingRC & maprc );

  // ---Assignment operator : deep copy ---
  virtual MappingRC & operator=( const Mapping & maprc );
  
  // reference to another MappingRC
  virtual void reference( const MappingRC & maprc );
  // reference to a Mapping
  virtual void reference( const Mapping & map );

  //-----------------------------------------------------------------------------------
  // To break a reference we make a new copy!
  //---------------------------------------------------------------------------------
  virtual void breakReference();

  // This function is used to create a new member of the Class provided the
  // mappingClassName is equal to the name of the class
  virtual Mapping *make( const aString & mappingClassName );

  // Map the domain r to the range x
  virtual void map( const realArray & r, realArray & x, realArray &xr = Overture::nullRealDistributedArray(),
		   MappingParameters & params =Overture::nullMappingParameters() );

  // Map the range x back to the domain r
  virtual void inverseMap( const realArray & x, realArray & r, realArray & rx =Overture::nullRealDistributedArray(),
			  MappingParameters & params=Overture::nullMappingParameters() );

  virtual void basicInverse( const realArray & x, realArray & r, realArray & rx =Overture::nullRealDistributedArray(),
			  MappingParameters & params=Overture::nullMappingParameters() );

  virtual void mapGrid(const realArray & r, 
		       realArray & x, 
		       realArray & xr =Overture::nullRealDistributedArray(),
		       MappingParameters & params=Overture::nullMappingParameters() );

  // Here are versions of map and inverseMap needed by some compilers (IBM:xlC) that don't like passing
  // views of arrays to non-const references, as in mapping.mapC(r(I),x(I),xr(I))
  virtual void mapC( const realArray & r, const realArray & x, const realArray &xr = Overture::nullRealDistributedArray(),
                    MappingParameters & params =Overture::nullMappingParameters());
  virtual void inverseMapC( const realArray & x, const realArray & r, const realArray & rx =Overture::nullRealDistributedArray(),
			  MappingParameters & params =Overture::nullMappingParameters());

  // Map the domain r to the range x
  virtual void mapS( const RealArray & r, RealArray & x, RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());

  // Map the range x back to the domain r
  virtual void inverseMapS( const RealArray & x, RealArray & r, RealArray & rx =Overture::nullRealArray(),
			  MappingParameters & params =Overture::nullMappingParameters());

  // If you know the inverse of your mapping supply this next function, 
  //      and set basicInverseOption=canInvert
  // If you don't know the inverse but know how to determine if a point is not in the
  // range (better than a bounding box) then set supply this function,
  //      and  set basicInverseOption=canDetermineOutside
  virtual void basicInverseS(const RealArray & x, 
			    RealArray & r,
			    RealArray & rx =Overture::nullRealArray(),
			    MappingParameters & params =Overture::nullMappingParameters());

  // Here are versions of map and inverseMap needed by some compilers (IBM:xlC) that don't like passing
  // views of arrays to non-const references, as in mapping.mapC(r(I),x(I),xr(I))
  virtual void mapCS( const RealArray & r, const RealArray & x, const RealArray &xr = Overture::nullRealArray(),
                    MappingParameters & params =Overture::nullMappingParameters());
  virtual void inverseMapCS( const RealArray & x, const RealArray & r, const RealArray & rx =Overture::nullRealArray(),
			  MappingParameters & params =Overture::nullMappingParameters());

  // map a grid of points: r(0:n1,1), or r(0:n1,0:n2,2) or r((0:n1,0:n2,0:n3,3) for 1, 2 or 3d
  virtual void mapGridS(const RealArray & r, 
		       RealArray & x, 
		       RealArray & xr =Overture::nullRealArray(),
		       MappingParameters & params=Overture::nullMappingParameters() );

  // inverse map a grid of points
  virtual void inverseMapGridS(const RealArray & x, 
			      RealArray & r, 
			      RealArray & rx =Overture::nullRealArray(),
			      MappingParameters & params=Overture::nullMappingParameters() );


  // return size of this object  
  virtual real sizeOf(FILE *file = NULL ) const;

  virtual void update( MappingInformation & mapInfo );  // update mapping, change parameters interactively

  virtual void display( const aString & label ) const;

  int checkMapping(); // Check the mapping - check derivatives and inverse, return 0 if ok

//--------------Access Functions----------------------------- 

  basicInverseOptions   getBasicInverseOption() const;
  int                   getBoundaryCondition( const int side, const int axis ) const;
  virtual aString        getClassName() const;
  int                   getCoordinateEvaluationType( const Mapping::coordinateSystem type ) const; 
  Bound                 getDomainBound( const int side, const int axis ) const;
  coordinateSystem      getDomainCoordinateSystem() const;
  Bound                 getDomainCoordinateSystemBound( const int side, const int axis ) const;
  int                   getDomainDimension() const;
  mappingSpace          getDomainSpace() const;
  int                   getGridDimensions( const int axis ) const;
  virtual const realArray& getGrid(MappingParameters & params=Overture::nullMappingParameters());
  int                   getID() const;
  int                   getInvertible() const;
  aString                getName( const Mapping::mappingItemName item ) const;
  periodicType          getIsPeriodic( const int axis ) const;
  real                  getPeriodVector( const int axis, const int direction ) const;
  int                   getRangeDimension() const;
  Bound                 getRangeBound( const int side, const int axis ) const;
  coordinateSystem      getRangeCoordinateSystem() const;
  Bound                 getRangeCoordinateSystemBound( const int side, const int axis ) const;
  mappingSpace          getRangeSpace() const;
  int                   getShare( const int side, const int axis ) const;
  real                  getSignForJacobian() const;
  topologyEnum          getTopology( const int side, const int axis ) const;
  coordinateSingularity getTypeOfCoordinateSingularity( const int side, const int axis ) const ;


// --------------set functions-------------------------

  void setBasicInverseOption( const basicInverseOptions option );
  void setBoundaryCondition( const int side, const int axis, const int bc );
  void setCoordinateEvaluationType( const Mapping::coordinateSystem type, const int trueOrFalse );
  void setDomainDimension( const int domainDimension );
  void setDomainBound( const int side, const int axis, const Bound domainBound );
  void setDomainCoordinateSystem( const Mapping::coordinateSystem domainCoordinateSystem );
  void setDomainCoordinateSystemBound( const int side, const int axis,
                                       const Bound domainCoordinateSystemBound );
  void setDomainSpace( const Mapping::mappingSpace domainSpace );
  void setGrid(realArray & grid, IntegerArray & gridIndexRange);
  void setGridDimensions( const int axis, const int dim );
  void setInvertible( const int invertible );
  void setID();
  void setIsPeriodic( const int axis, const Mapping::periodicType isPeriodic );
  void setName( const Mapping::mappingItemName item, const aString & name );
  void setPeriodVector( const int axis, const int direction, const real periodVectorComponent );
  void setRangeDimension( const int rangeDimension );
  void setRangeSpace( const Mapping::mappingSpace rangeSpace ); 
  void setRangeCoordinateSystem( const Mapping::coordinateSystem rangeCoordinateSystem );
  void setRangeBound( const int side, const int axis, const Bound rangeBound );
  void setRangeCoordinateSystemBound( const int side, const int axis,
                                      const Bound rangeCoordinateSystemBound );
  void setShare( const int side, const int axis, const int share );
  void setSignForJacobian( const real signForJac );
  void setTopology( const int side, const int axis, const topologyEnum topo );

  void setTypeOfCoordinateSingularity( const int side, const int axis,
                                       const Mapping::coordinateSingularity type );

  bool usesDistributedInverse() const; 
  void useRobustInverse(const bool trueOrFalse=TRUE );


  // get from a database file: (optionally supply the partition to use in the Mapping)
  virtual int get( const GenericDataBase & dir, const aString & name, Partitioning_Type *partition=NULL); 
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  // return a reference to the Mapping, an error occurs if there is no Mapping
  Mapping & getMapping() const;

  private :
    int uncountedReferencesMayExist;   // =1 if Mapping is not reference counted
    void initialize( const aString & mappingClassName );

  static DataBaseModeEnum dataBaseMode;
  
  private:
  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
    virtual ReferenceCounting& operator=(const ReferenceCounting& x)
      { return operator=(*(MappingRC*)&x); }
    virtual void reference(const ReferenceCounting& x)
      { reference((MappingRC &) x); }
    virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
      { return ::new MappingRC(*this, ct); }

};

#endif
