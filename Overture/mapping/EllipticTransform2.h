#ifndef ELLIPTIC2_TRANSFORM
#define ELLIPTIC2_TRANSFORM

#include "Mapping.h"
#include "MappedGridOperators.h"

// forward declarations:
class GenericGraphicsInterface; 
class GraphicsParameters;       
class ComposeMapping;           
class DataPointMapping;

extern GraphicsParameters Overture::defaultGraphicsParameters();
// const int Jacobi=1; 
// const int lineImplicit=2;

class EllipticTransform2 : public Mapping
{
//----------------------------------------------------------------
//  Smooth out a mapping with Elliptic Grid Generation
//  -------------------------------------------------
//
//----------------------------------------------------------------

  enum
  {
    Jacobi=1,
    lineImplicit=2
  };
  

 public:

  EllipticTransform2();

  // Copy constructor is deep by default
  EllipticTransform2( const EllipticTransform2 &, const CopyType copyType=DEEP );

  ~EllipticTransform2();

  EllipticTransform2 & EllipticTransform2::operator =( const EllipticTransform2 & X );

  void map( const RealArray & r, RealArray & x, RealArray & xr = Overture::nullRealArray(),
            MappingParameters & params =Overture::nullMappingParameters() );

  void inverseMap( const RealArray & x, RealArray & r, RealArray & rx = Overture::nullRealArray(),
                   MappingParameters & params =Overture::nullMappingParameters() );

  void generateGrid(GenericGraphicsInterface *gi=NULL, 
                    GraphicsParameters & parameters=Overture::defaultGraphicsParameters());

  void periodicUpdate(realArray &x,realArray xo,realArray pSrc,realArray qSrc);
  void bcUpdate(realArray &x,realArray xo,realArray &pSrc, int iteration);
  void initialize();
  void findSourceTerms(realArray &Src, const int rangedimension, Index I, Index J, Index K);
  real Signf(real x);
  real vDot (realArray a,realArray b,int dim);

  virtual int get( const GenericDataBase & dir, const aString & name);    // get from a database file
  virtual int put( GenericDataBase & dir, const aString & name) const;    // put to a database file

  Mapping* make( const aString & mappingClassName );

  aString getClassName() const { return EllipticTransform2::className; }

  int update( MappingInformation & mapInfo ) ;


 protected:

  int setup();
  void resetDataPointMapping( realArray & x,Index I, Index J, Index K);

 private:

  aString className;
  Mapping *userMap;             // original mapping before elliptic grid generation
  bool project;                 // if true we project the elliptic grid back onto the original Mapping
  ComposeMapping *compose;      // holds the composite map if project==TRUE
  DataPointMapping *dpm;        // This holds the elliptic grid if project==FALSE or
                                // the reparameterization map if project==TRUE
  bool ellipticGridDefined;     // TRUE when the grid has been generated

  real omega;			//=1.8;
  real lambda;                  //=1.5; source interpolation coefficient
  int maxIter;			//=5000;
  real epsilon;			//=1.0e-5;
  int numDim;			//=2;
  int iDim;			
  int jDim;			
  int srcDefault;
  real di,dj;
  int jb,ib,kb,numOfPeriods;
  int solutionMethod;
  realArray xe,rTilde;    // xe holds the grid points of the elliptic transform

  int numOfILineSources,numOfJLineSources,numOfPointSources;
  realArray powOfILineSources,difOfILineSources,powOfJLineSources,difOfJLineSources,
    powOfPointSources,difOfPointSources;
  intArray locOfILineSources,locOfJLineSources,locOfPointSources;
  intArray gridBc;
  realArray dB,dS;

 private:

  //
  //  Virtual member functions used only through class ReferenceCounting:
  //
  virtual ReferenceCounting& operator=(const ReferenceCounting& x)
    { return operator=((EllipticTransform2 &)x); }
  virtual void reference( const ReferenceCounting& x) 
    { reference((EllipticTransform2 &)x); }     
  virtual ReferenceCounting* virtualConstructor( const CopyType ct = DEEP ) const
    { return ::new EllipticTransform2(*this, ct); }
};


#endif
