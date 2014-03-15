//-------------------------------------------------------------------------
//  StretchMapping:
//
//  These routines provide an interface to the STRTCH stretching routines
//
//------------------------------------------------------------------------

#include "StretchMapping.h"
#include "MappingInformation.h"
#include "ParallelUtility.h"
#include "DataPointMapping.h"


#define STINIT EXTERN_C_NAME(stinit)
#define STTR   EXTERN_C_NAME(sttr)
#define STRT   EXTERN_C_NAME(strt)
#define R1MACH EXTERN_C_NAME(r1mach)

extern "C" { 
//  void STINIT( int & ndi, int *iw, int & ndr, real * rw, int & iopt,
//                int & ndwk, real *wk, int & ierr );
  void STINIT( int & ndi, int & iw, int & ndr, real & rw, int & iopt,
                int & ndwk, real & wk, int & ierr );
  void STTR( real & t, real & r, real & rt, int & iw, real & rw, int & ierr );
  void STRT( real & r, real & t, real & tr, int & iw, real & rw, int & ierr );
  float R1MACH( const int & i );
}

StretchMapping::
StretchMapping( const StretchingType & stretchingType_ /* = noStretching */ )
:Mapping(1,1,parameterSpace,parameterSpace)
//===========================================================================
/// \brief  Construct a function with the given stretching type, one of
///   <ul>
///    <li> <B>inverseHyperbolicTangent</B> : the most commonly used stretching function
///       defined in an inverse way as a combination of hyperbolic tangents and
///       logarithms of hyperbolic cosines.
///    <li> <B>hyperbolicTangent</B> : hyperbolic tangent stretching.
///    <li> <B>exponential</B> : exponential stretching.
///    <li> <B>exponentialBlend</B> : a $C^\infty$ blending function that is exactly 0 for $r<{1\over4}$
///         and exactly 1 for $r>{3\over4}$.
///    <li> <B>exponentialToLinear</B> exponential stretching that transitions to linear stretching
///   </ul>
/// \param stretchingType_ (input):
//===========================================================================
{
  stretchingType=stretchingType_;
  if( stretchingType==inverseHyperbolicTangent )
    numberOfLayers=1;
  else
    numberOfLayers=0;
  numberOfIntervals=0;
  
  inverseIsDistributed=false; // *wdh* 110421 -> the entire grid for the inverse will be stored on each processor

  setup();
}



StretchMapping::
StretchMapping( const int numberOfLayers_, 
		const int numberOfIntervals_ /* = 0 */ ) 
  :Mapping(1,1,parameterSpace,parameterSpace)
//===========================================================================
/// \brief  Construct an {\tt inverseHyperbolicTangent} stretching function.
/// \param numberOfLayers_ (input): number of layers.
/// \param numberOfIntervals_ (input): number of intervals.
//===========================================================================
{ 
  stretchingType=inverseHyperbolicTangent;
  numberOfLayers=numberOfLayers_; 
  numberOfIntervals=numberOfIntervals_; 

  inverseIsDistributed=false; // *wdh* 110421 -> the entire grid for the inverse will be stored on each processor

  setup();
}

void StretchMapping::
setup()
{
  StretchMapping::className="StretchMapping";
  setName( Mapping::mappingName,"stretch");
  setGridDimensions( axis1,11 );
  unInitialized=true;
  Mapping::setIsPeriodic( axis1, Mapping::notPeriodic );
  numberOfSplinePoints=0; 

  Mapping::setInvertible( true );
  setBasicInverseOption(canInvert);  // basicInverse is available (except for the exponential stretching)

  iw.redim(1);
  rw.redim(1);
  iopt=0;             // use default values
  ra=0.; rb=0.;
  r0=0.; r1=0.;

  abc.redim(3,max(numberOfLayers,1));        // allocate space
  abc=0.;
  def.redim(3,numberOfIntervals+1);
  def=0.;

  a0=0.; ar=1.; b1=5.; a1=-.9*a0/b1; c1=.5;   // choose a0 > a1*b1 to be invertible
  normalized=TRUE;
  
  mappingHasChanged();
}

// Copy constructor is deep by default
StretchMapping::
StretchMapping( const StretchMapping & map, const CopyType copyType )
{
  StretchMapping::className="StretchMapping";
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "StretchMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}


StretchMapping::
~StretchMapping()
{
  if( debug & 4 )
     cout << " StretchMapping::Destructor called" << endl;
}

StretchMapping & StretchMapping::
operator =( const StretchMapping & X )
{
  if( StretchMapping::className != X.getClassName() )
  {
    cout << "StretchMapping::operator= ERROR trying to set a StretchMapping = to a" 
      << " mapping of type <" << X.getClassName() << ">" << endl;
    cout << "className = <" << StretchMapping::className << ">" << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  StretchMapping::className=X.getClassName();
  iw.redim(0); iw=X.iw; iw.setBase(1);
  numberOfLayers=X.numberOfLayers;
  numberOfIntervals=X.numberOfIntervals;
  numberOfSplinePoints=X.numberOfSplinePoints;
  unInitialized=X.unInitialized;
  rw.redim(0);  rw=X.rw; rw.setBase(1);
  iopt=X.iopt;
  ra=X.ra;
  rb=X.rb;
  r0=X.r0;
  r1=X.r1;
  abc.redim(0); abc=X.abc;
  def.redim(0); def=X.def;

  stretchingType=X.stretchingType;
  a0=X.a0; 
  ar=X.ar; 
  a1=X.a1; 
  b1=X.b1; 
  c1=X.c1;
  a0Normalized=X.a0Normalized;
  arNormalized=X.arNormalized;
  a1Normalized=X.a1Normalized;
  normalized=X.normalized;

  return *this;
}

int StretchMapping:: 
setStretchingType(  const StretchingType & stretchingType_ )
//===========================================================================
/// \details  Set the stretching type, one of
///   <ul>
///    <li> <B>inverseHyperbolicTangent</B> : the most commonly used stretching function
///       defined in an inverse way as a combination of hyperbolic tangents and
///       logarithms of hyperbolic cosines.
///    <li> <B>hyperbolicTangent</B> : hyperbolic tangent stretching.
///    <li> <B>exponential</B> : exponential stretching.
///    <li> <B>exponentialBlend</B> : a $C^\infty$ blending function that is exactly 0 for $r<{1\over4}$
///         and exactly 1 for $r>{3\over4}$.
///    <li> <B>exponentialToLinear</B> exponential stretching that transitions to linear stretching
///   </ul>
/// \param stretchingType_ (input):
//===========================================================================
{
  stretchingType=stretchingType_;
  unInitialized=TRUE;
  return 0;
}



int StretchMapping::
setNumberOfLayers( const int numberOfLayers_ ) 
//===========================================================================
/// \details  
///     Set the number of layer (tanh) functions in the {\tt inverseHyperbolicTangent} stretching
///    function.
/// \param numberOfLayers_ (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{ 
  if( stretchingType==noStretching )
    setStretchingType(inverseHyperbolicTangent);
  
  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setNumberOfLayers:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  // const int oldNumberOfLayers=numberOfLayers;
  numberOfLayers=numberOfLayers_; 
  RealArray temp;
  temp=abc;
  abc.redim(3,max(numberOfLayers,1));
  abc=0.;
  
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setNumberOfIntervals( const int numberOfIntervals_ )
//===========================================================================
/// \details  
///     Set the number of interval (log(cosh)) functions in the {\tt inverseHyperbolicTangent} stretching
///    function.
/// \param numberOfIntervals_ (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{ 
  if( stretchingType==noStretching )
    setStretchingType(inverseHyperbolicTangent);

  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setNumberOfIntervals:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  const int oldNumberOfIntervals=numberOfIntervals;
  numberOfIntervals=numberOfIntervals_;
  def.resize(3,numberOfIntervals+1);
  if( numberOfIntervals > oldNumberOfIntervals )
    def(Range(0,2),Range(oldNumberOfIntervals,numberOfIntervals-1))=0.;
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setNumberOfSplinePoints( const int numberOfSplinePoints0 )
//===========================================================================
/// \details  
///     Set the number of interval (log(cosh)) functions in the {\tt inverseHyperbolicTangent} stretching
///    function.
/// \param numberOfIntervals_ (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{
  if( stretchingType==noStretching )
    setStretchingType(inverseHyperbolicTangent);

  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setNumberOfSplinePoints:ERROR: you should first set the stretching type to "
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  numberOfSplinePoints=numberOfSplinePoints0;
  return 0;
}

int StretchMapping::
setLayerParameters( const int index, const real a, const real b, const real c )
//===========================================================================
/// \details  
///     Set parameters for the interval (log(cosh)) function numbered {\tt index}.
/// \param a,b,c (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{  
  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setLayerParameters:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  if( (index >= 0) && (index < numberOfLayers) )
  {
    abc(0,index)=a;
    abc(1,index)=b;
    abc(2,index)=c;
  }
  else
    cout << "StretchMapping::setLayerParameters:ERROR index out if range" << endl;
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setIntervalParameters( const int index, const real d, const real e,  const real f )
//===========================================================================
/// \details  
///     Set parameters for the interval (log(cosh)) function numbered {\tt index}.
/// \param d,e,f (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{  
  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setIntervalParameters:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  if( (index >= 0) && (index < numberOfIntervals+1) )
  {
    def(0,index)=d;
    def(1,index)=e;
    def(2,index)=f;
  }
  else
    cout << "StretchMapping::setLayerParameters:ERROR index out if range" << endl;
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setEndPoints( const real rmin, const real rmax )
//===========================================================================
/// \details  
///     Set the end points for the {\tt inverseHyperbolicTangent} stretching function.
/// \param rmin,rmax (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{ 
  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setEndPoints:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  iopt=iopt - 2*( iopt % 2);
  ra=rmin; 
  rb=rmax;
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setIsNormalized( const bool & trueOrFalse /* =TRUE */  )
//===========================================================================
/// \details  
///     Indicate whether the stretching function should be normalized to go from 0 to 1.
/// \param trueOrFalse (input): if TRUE the function is normalized.
//===========================================================================
{
  normalized=trueOrFalse;
  return 0;
}




int StretchMapping::
setScaleParameters( const real origin_, const real scale_ )
//===========================================================================
/// \details  
///     Set the origin and scale parameters for the {\tt inverseHyperbolicTangent} stretching
///    function.
/// \param origin_, scale_ (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{ 
  if( stretchingType!=inverseHyperbolicTangent )
  {
    cout << "StretchMapping::setScaleParameters:ERROR: you should first set the stretching type to " 
         << "inverse hyperbolic tangent \n";
    return 1;
  }
  iopt=iopt + 2 - 2*(iopt % 2);
  origin=origin_;
  scale=scale_;
  r0=origin; 
  r1=scale; 
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

int StretchMapping::
setIsPeriodic( const int trueOrFalse )
//===========================================================================
/// \details  
///    Define the periodicity of the function, only applies to the {\tt inverseHyperbolicTangent} stretching
///    function.
/// \param trueOrFalse (input): TRUE or FALSE.
/// \return  0 on success, 1 if the stretching type has not been set to {\tt inverseHyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{
  if( trueOrFalse && stretchingType!=inverseHyperbolicTangent && stretchingType!=noStretching )
  {
    cout << "StretchMapping::setIsPeriodic:ERROR: only a inverse tanh or noStretching can be periodic\n";
    return 1;
  }
  periodicType isPeriodic0 = trueOrFalse ? derivativePeriodic : notPeriodic;
  Mapping::setIsPeriodic( axis1, isPeriodic0 );
  mappingHasChanged(); 
  unInitialized=TRUE;
  return 0;
}

void StretchMapping::
setIsPeriodic( const int axis, const periodicType isPeriodic0 )
{
  if( (bool)isPeriodic0 && stretchingType!=inverseHyperbolicTangent && stretchingType!=noStretching)
  {
    cout << "StretchMapping::setIsPeriodic:ERROR: only inverse tanh or noStretching can be periodic\n";
    return;
  }
  Mapping::setIsPeriodic(axis,isPeriodic0);  
}

int StretchMapping::
setHyperbolicTangentParameters(const real & a0_,
			       const real & ar_, 
			       const real & a1_, 
			       const real & b1_, 
			       const real & c1_)
//===========================================================================
/// \details  
///     Set the parameters for the {\tt hyperbolicTangent} stretching
///    function.
/// \param a0_,ar_,a1_,b1_,c1_, (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt hyperbolicTangent}
///    in which case no changes are made.  
//===========================================================================
{
  if( stretchingType!=hyperbolicTangent )
  {
    cout << "StretchMapping::setHyperbolicTangentParameters:ERROR: you should first set the stretching type to " 
         << "hyperbolic tangent \n";
    return 1;
  }
  a0=a0_;
  ar=ar_;
  a1=a1_;
  b1=b1_;
  c1=c1_;
  return 0;
}

int StretchMapping::
setExponentialParameters(const real & a0_, 
			 const real & ar_, 
			 const real & a1_, 
			 const real & b1_, 
			 const real & c1_)
//===========================================================================
/// \details  
///     Set the parameters for the {\tt exponential} stretching
///    function.
/// \param a0_,a1_,b1_,c1_, (input):
/// \return  0 on success, 1 if the stretching type has not been set to {\tt exponential}
///    in which case no changes are made.  
//===========================================================================
{
  if( stretchingType!=exponential )
  {
    cout << "StretchMapping::setExponentialParameters:ERROR: you should first set the stretching type to " 
         << "exponential \n";
    return 1;
  }
  a0=a0_;
  ar=ar_;
  a1=a1_;
  b1=b1_;
  c1=c1_;
  return 0;
}

int StretchMapping::
setLinearSpacingParameters(const real & a0_, const real & a1_)
//===========================================================================
/// \details  
///     Set the parameters for the {\tt linear spacing} stretching
///    function. 
///     The grid spacing for this stretching will exponentially increase from a0 to a1. (wdh)
/// \param a0, a1 (input): specify the grid spacings at r=0 and r=1
/// \return  0 on success, 1 if the stretching type has not been set to {\tt linearSpacing}
///    in which case no changes are made.  
//===========================================================================
{
  if( stretchingType!=linearSpacing )
  {
    cout << "StretchMapping::setLinearSpacingParameters:ERROR: you should first set the stretching type to " 
         << "linearSpacing \n";
    return 1;
  }
  a0=a0_;
  a1=a1_;
  return 0;
}

int StretchMapping::
setExponentialToLinearParameters(const real & a, 
				 const real & b,
                                 const real & c )
//===========================================================================
/// \details  
///     Set the parameters for the {\tt exponentialToLinear} stretching
///    function.
/// \param a,b,c (input): see documentation for the formula.
/// \return  0 on success, 1 if the stretching type has not been set to {\tt exponential}
///    in which case no changes are made.  
//===========================================================================
{
  if( stretchingType!=exponentialToLinear )
  {
    printF("StretchMapping::setExponentialToLinearParameters:ERROR: you should first set the stretching type to " 
           "exponentialToLinear \n");
    return 1;
  }
  a1=a;
  b1=b;
  c1=c;
  return 0;
}

int StretchMapping::
initialize( )
{ 
  if( stretchingType==inverseHyperbolicTangent )
  {
    Mapping::setInvertible( TRUE );
    int ndi=12;
    if( getIsPeriodic(axis1) )
      ndi=ndi+(numberOfLayers+numberOfIntervals)*2;  
    iw.redim(ndi); iw.setBase(1);

    if( getInvertible() )
    {
      if( numberOfSplinePoints==0 )
      {
	numberOfSplinePoints=50;
	numberOfSplinePoints=500;
      }
    }
    else
    { 
      numberOfSplinePoints=0;
      iopt=iopt+1-(iopt % 2);
    }

    iw(1)=numberOfLayers;     // nu
    iw(2)=numberOfIntervals;  // nv
    iw(3)=getIsPeriodic(axis1)? 1:0 ;
    iw(4)=numberOfSplinePoints;
    
    int ndr=3*(numberOfLayers+numberOfIntervals+1)+4+numberOfIntervals+
      4*numberOfSplinePoints ;
    if( getIsPeriodic(axis1) )
      ndr=ndr+3*(numberOfLayers+numberOfIntervals)+20;

    rw.redim(ndr); rw.setBase(1);
    int i;
    for( i=0; i<numberOfLayers; i++)
    { 
      rw(3*i+1)=abc(0,i);
      rw(3*i+2)=abc(1,i);
      rw(3*i+3)=abc(2,i);
    }
    for( i=0; i<numberOfIntervals+1; i++)
    {
      rw(3*(i+numberOfLayers)+1)=def(0,i);
      rw(3*(i+numberOfLayers)+2)=def(1,i);
      rw(3*(i+numberOfLayers)+3)=def(2,i);
    }
    rw(3*(numberOfLayers+numberOfIntervals)+1)=ra;
    rw(3*(numberOfLayers+numberOfIntervals)+2)=rb;

    if( debug & 4 )
    { 
      for( i=1; i<= 3*(numberOfLayers+numberOfIntervals)+2 ; i++)
        printf(" stretch: i = %d , rw(i) = %7.4f", i,rw(i)); printf("\n");
    }

    int ndwk=max(1,numberOfSplinePoints);  // ***
    RealArray wk(ndwk);

    STINIT( ndi, iw(1), ndr, rw(1), iopt, ndwk, wk(0), ierr );

    if( ierr != 0 )
      cout << "StretchMapping:Error from stinit, ierr = " << ierr << endl;
  }
  else if( stretchingType==hyperbolicTangent )
  {
    // check the slope at the extreme points -- all signs should be the same
    if(  (ar+a1*b1*(1.-SQR(tanh(b1*(0.-c1)))))    // left
	*(ar+a1*b1*(1.-SQR(tanh(b1*(1.-c1)))))   // right
	*(ar+a1*b1*(1.)) > 0.           )        // r=c1 (assumes 0 <= c1 <= 1
    {
      Mapping::setInvertible( TRUE ); 
    }
    else
    {
      cout << "StretchMapping::WARNING: this hyperbolic tangent stretching function is not invertible \n";
      Mapping::setInvertible( FALSE ); 
    }
    setIsPeriodic( FALSE );

    real normalization;
    if( normalized )
    {
      origin=-(a0+a1*(tanh(-b1*c1)));
      normalization=a0+ar+a1*tanh(b1*(1.-c1))+origin;
      if( normalization==0. )
      {
	cout << "StretchMapping::ERROR: normalization for stretching is 0! setting to 1., mapping will be incorrect\n";
	normalization=1.;
      }
    }
    else
    {
      origin=0.;
      normalization=1.;
    }
    a0Normalized=(a0+origin)/normalization;
    arNormalized=ar/normalization;
    a1Normalized=a1/normalization;
  }
  else if( stretchingType==exponential )
  {
    if( ( ar+a1*b1*exp(-b1*c1) )*( ar+a1*b1*exp(b1*(1.-c1)) ) > 0. )
    {
      Mapping::setInvertible( TRUE ); 
    }
    else
    {
      cout << "StretchMapping::WARNING: this exponential stretching function is not invertible \n";
      Mapping::setInvertible( FALSE ); 
    }
    setIsPeriodic( FALSE );

    real normalization;
    if( normalized )
    {
      origin=-( a0 + a1*exp(-b1*c1) );
      normalization=a0+origin+ar+a1*exp(b1*(1.-c1));
      if( normalization==0. )
      {
	cout << "StretchMapping::ERROR: normalization for exponential stretching is 0! setting to 1., "
             "mapping will be incorrect\n";
	normalization=1.;
      }
    }
    else
    {
      origin=0.;
      normalization=1.;
    }
    a0Normalized=(a0+origin)/normalization;
    arNormalized=ar/normalization;
    a1Normalized=a1/normalization;
  }
  else if( stretchingType==exponentialBlend )
  {
    Mapping::setInvertible( FALSE );
  }
  else if( stretchingType==exponentialToLinear )
  {
    // finish me 
  }
  else if( stretchingType==linearSpacing)
  {
    Mapping::setInvertible( TRUE );
  }
  mappingHasChanged(); 
  unInitialized=FALSE;

  return ierr;

}

  // get a mapping from the database
int StretchMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");

  subDir.get( StretchMapping::className,"className" ); 
  if( StretchMapping::className != "StretchMapping" )
  {
    cout << "StretchMapping::get ERROR in className!" << endl;
  }
  iw.redim(0);   // redim to 0 so the get will work
  subDir.get( iw,"iw" );  iw.setBase(1);
  subDir.get( numberOfLayers,"nu" );
  subDir.get( numberOfIntervals,"nv" );
  subDir.get( numberOfSplinePoints,"nsp" );
  subDir.get( unInitialized,"unInitialized" );
  rw.redim(0); 
  subDir.get( rw,"rw" );   rw.setBase(1);
  subDir.get( iopt,"iopt" );
  subDir.get( ra,"ra" );
  subDir.get( rb,"rb" );
  subDir.get( r0,"r0" );
  subDir.get( r1,"r1" );
  abc.redim(3,max(numberOfLayers,1));
  subDir.get( abc,"abc" );
  def.redim(3,numberOfIntervals+1);
  subDir.get( def,"def" );

  int temp;
  subDir.get(temp,"stretchingType"); stretchingType=(StretchingType)temp;
  subDir.get(a0,"a0"); 
  subDir.get(ar,"ar");
  subDir.get(a1,"a1");
  subDir.get(b1,"b1"); 
  subDir.get(c1,"c1");
  subDir.get(normalized,"normalized");
  subDir.get(a0Normalized,"a0Normalized");
  subDir.get(arNormalized,"arNormalized");
  subDir.get(a1Normalized,"a1Normalized");

  Mapping::get( subDir, "Mapping" );

  if( debug & 4 )
    cout << "StretchMapping::get - nu = " << numberOfLayers << " , " 
         << " nv = " << numberOfIntervals << endl;
  delete &subDir;
  mappingHasChanged();

  // put this here for now -- for reading in old grids --- this can be removed eventually 
  inverseIsDistributed=false; // *wdh* 110421 -> the entire grid for the inverse will be stored on each processor

  return 0;
}
int StretchMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 

  subDir.put( StretchMapping::className,"className"  );
  subDir.put( iw,"iw"  );
  subDir.put( numberOfLayers,"nu" );
  subDir.put( numberOfIntervals,"nv" );
  subDir.put( numberOfSplinePoints,"nsp" );
  subDir.put( unInitialized,"unInitialized" );
  subDir.put( rw,"rw" );
  subDir.put( iopt,"iopt" );
  subDir.put( ra,"ra" );
  subDir.put( rb,"rb" );
  subDir.put( r0,"r0" );
  subDir.put( r1,"r1" );
  if( debug & 4 )
  {
    cout << "StretchMapping::Put: abc.base= " << abc.getBase(0) << "," 
                                              << abc.getBase(1) << endl;
    cout << "StretchMapping::Put: abc.bound= " << abc.getBound(0) << "," 
                                               << abc.getBound(1) << endl;
  }
  subDir.put( abc,"abc" );
  subDir.put( def,"def" );
  subDir.put((int)stretchingType,"stretchingType");
  subDir.put(a0,"a0"); 
  subDir.put(ar,"ar");
  subDir.put(a1,"a1");
  subDir.put(b1,"b1"); 
  subDir.put(c1,"c1");
  subDir.put(normalized,"normalized");
  subDir.put(a0Normalized,"a0Normalized");
  subDir.put(arNormalized,"arNormalized");
  subDir.put(a1Normalized,"a1Normalized");

  Mapping::put( subDir, "Mapping" );

  delete &subDir;
  return 0;
}

Mapping *StretchMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=NULL;
  if( mappingClassName==StretchMapping::className )
    retval = new StretchMapping();
  return retval;
}


void StretchMapping::
map( const realArray & r, realArray & x, realArray & xr,
                          MappingParameters & params)
// Apply the stretching
{ 
  #ifdef USE_PPP
    RealArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
    RealArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    RealArray xrLocal; getLocalArrayWithGhostBoundaries(xr,xrLocal);
    mapS(rLocal,xLocal,xrLocal,params);
    return;
  #else
    mapS(r,x,xr,params);
    return;
  #endif
}

void StretchMapping::
inverseMap( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
// Invert the stretching
{
  #ifdef USE_PPP
    RealArray xLocal; getLocalArrayWithGhostBoundaries(x,xLocal);
    RealArray rLocal; getLocalArrayWithGhostBoundaries(r,rLocal);
    RealArray rxLocal; getLocalArrayWithGhostBoundaries(rx,rxLocal);
    inverseMapS(xLocal,rLocal,rxLocal,params);
    return;
  #else
    inverseMapS(x,r,rx,params);
    return;
  #endif
}

// define a basic inverse too
void StretchMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{ 
  inverseMap( x,r,rx,params );
}

// =======================================================================================================
// Apply the stretching - see the documentation for details 
// 
// Version that takes a serial array
// =======================================================================================================
void StretchMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
{ 
  if( unInitialized )
    initialize();

  Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

  real dummy;
  int i;
  int axis=r.getBase(1);  // ****use this for the base****
  if( stretchingType==hyperbolicTangent )
  {
    // hyperbolic tangent stretching 
    RealArray tanhI(I);   // ***** fix ****
    tanhI=tanh(b1*(r(I,axis)-c1));
    if( computeMap )
      x(I,axis)= a0Normalized + arNormalized*r(I,axis) + a1Normalized*tanh(b1*(r(I,axis)-c1));
    if( computeMapDerivative )
      xr(I,axis,axis)= arNormalized + (a1Normalized*b1)*( 1.-tanhI*tanhI );
  }
  else if(  stretchingType==exponential )
  {
    // exponential
    RealArray expI(I);   // ***** fix ****
    expI=a1Normalized*exp(b1*(r(I,axis)-c1));
    if( computeMap )
      x(I,axis)= a0Normalized + arNormalized*r(I,axis) + expI;
    if( computeMapDerivative )
      xr(I,axis,axis)= arNormalized + b1*expI;
  }  
  else if( stretchingType==exponentialToLinear )
  {
    // exponential to linear streching 
    const real * rp = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
    #undef R
    #define R(i0,i1) rp[i0+rDim0*(i1)]
    real * xp = x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
    #undef X
    #define X(i0,i1) xp[i0+xDim0*(i1)]
    real * xrp = xr.Array_Descriptor.Array_View_Pointer2;
    const int xrDim0=xr.getRawDataSize(0);
    const int xrDim1=xr.getRawDataSize(1);
    #undef XR
    #define XR(i0,i1,i2) xrp[i0+xrDim0*(i1+xrDim1*(i2))]


    //  phi(x) = x/(1+x)
    //  phi(x) = x , as  x -> 0
    //  phi(x) = 1, as y-> infinity 

    //    x(r) =   c0*[ log( 1 + (a/f)*exp(b*r) ) - log(1+ a/f) ]
    //   dx/dr = c0*b*phi( (a/f)*exp(b*r) ) 

    // real a = .001,  b=15., f=1.;
    // printF(" exponentialToLinear a1,b1,c1=%e,%e,%e\n",a1,b1,c1);

    const real a = a1,  b=b1;

    // real c0= (f)/(b);

    real x0,scale;

    if( c1==0. || c1==1. )
    {
      // --- boundary stretching ---
      const real s = 1.-2.*c1;   // s = +1 or -1 for c1=0. or 1. 
      if( c1==0. )
      {
	x0 = log(1.+a);        // "x(0)"
	scale = log(1.+a*exp(b)) -x0;    // "x(1)-x(0)" 
      }
      else
      {
	x0 = log(1.+a*exp(b));        // "x(0)"
	scale = log(1.+a) -x0;    // "x(1)-x(0)" 
      }
      
      
      const real c0 = 1./scale;
      const real c2  = s*b/scale;
      for( int i=base; i<=bound; i++ )
      {
	real rr = s*(R(i,axis)-c1);
	real ebr = exp(b*rr);
	real debr = a*ebr;
	if( computeMap )
	{
	  X(i,axis) =  c0*( log(1.+debr ) -x0);

	  // printF(" r=%10.2e exponentialToLinear=%10.2e\n",rr,X(i,0));
	
	}
	if( computeMapDerivative )
	{
	  XR(i,axis,axis) = c2*( debr )/( 1.+debr );
	}
	// if( computeMap && computeMapDerivative )
	//   printF(" exponentialToLinear: r=%10.2e, x=%10.2e xr=%10.2e\n",rr,X(i,0),XR(i,0,0));
      }
    }
    else
    {
      // -- interior stretching --

      real ebc = exp(b*c1);
      real ebcm= exp(b*(c1-1.));
      x0 = log( (1.+a/ebc)/(1.+a*ebc) );        // "x(0)"
      scale = log( (1.+a/ebcm)/(1.+a*ebcm) ) -x0;    // "x(1)-x(0)" 
      
      
      const real c0 = 1./scale;
      const real c2  = b/scale;
      for( int i=base; i<=bound; i++ )
      {
	real rr = R(i,axis)-c1;
	real ebr = exp(b*rr);
	real debr = a*ebr;
	real debri = a/ebr;
	if( computeMap )
	{
	  X(i,axis) =  c0*( log( (1.+debr)/(1.+debri) ) -x0 );

	  // printF(" r=%10.2e exponentialToLinear=%10.2e\n",rr,X(i,0));
	
	}
	if( computeMapDerivative )
	{
	  XR(i,axis,axis) = c2*( debr/( 1.+debr) + debri/( 1.+debri) );
	}
	
      }
    }
    
  }  
  else if(  stretchingType==linearSpacing )
  {
    real c = log( fabs(a0 / (a1-a0)) );
    real k = log( fabs(1 + a0/(a1-a0)) ) - log(fabs(a0/(a1-a0)));;

    real *r_ptr = r.Array_Descriptor.Array_View_Pointer3;
    const int r_Dim0 = r.getRawDataSize(0);
#define r(i,j) r_ptr[ (i) + (j)*r_Dim0 ]

    real *x_ptr = x.Array_Descriptor.Array_View_Pointer3;
    const int x_Dim0 = x.getRawDataSize(0);
#define x(i,j) x_ptr[ (i) + (j)*x_Dim0 ]

    real *xr_ptr = xr.Array_Descriptor.Array_View_Pointer3;
    const int xr_Dim0 = xr.getRawDataSize(0);
    const int xr_Dim1 = xr.getRawDataSize(1)*xr_Dim0;
#define xr(i,j,k) xr_ptr[ (i) + (j)*xr_Dim0 + (k)*xr_Dim1]

    for(int i = I.getBase() ; i<=I.getBound() ; i++){
      // | x+ a0/(a1-a0) | = exp(k*r+c) - and we know that x increases monotonically with r, so use same sign as k for mod sign
      if(computeMap)
	x(i,axis) = fabs(k)/k*exp( k*r(i,axis) + c ) - fabs(a0/(a1-a0));
      if(computeMapDerivative) 
	xr(i,axis,axis) = fabs(k)*exp(k*r(i,axis) + c );
    };

#undef x
#undef r
#undef xr
  }
  else if(  stretchingType==inverseHyperbolicTangent )
  {
    // ***** default stretching is inverseHyperbolicTangent ******
    if( computeMap && computeMapDerivative )
    {
      if( stretchingType==inverseHyperbolicTangent )
      {
	for( i=base; i<= bound; i++ )
	{	
	  STRT( r(i,axis),x(i,axis),xr(i,axis,axis), iw(1),rw(1),ierr );
	  if( debug & 2 && ierr >0 )
	    cout << "StretchMapping:Error from strt, ierr = " << ierr << endl;
	}
      }
    }
    else
    {
      if( computeMap )
      {
	for( i=base; i<= bound; i++ )
	{ 
	  STRT( r(i,axis),x(i,axis),dummy, iw(1),rw(1),ierr );
	  if( debug & 2 && ierr > 0 )
	    cout << "StretchMapping:Error from strt, ierr = " << ierr << endl;
	}
      }
      else
      {
	if( computeMapDerivative )
	{
	  for( i=base; i<= bound; i++ )
	  {	
	    STRT( r(i,axis),dummy,xr(i,axis,axis), iw(1),rw(1),ierr );
	    if( debug & 2 && ierr > 0 )
	      cout << "StretchMapping:Error from strt, ierr = " << ierr << endl;
	  }
	}
      }
    }
  }
  else if( stretchingType==exponentialBlend )
  {
    real beta=-SQRT(3.)/4.;
    RealArray expI(I);
    const RealArray & rr = r(I,axis);
    where( rr >= .25 && rr <=.75 )
    {
      expI= 1./( 1. + exp( beta*( 2.*rr-1. )/( (rr-.25)*(.75-rr) ) ) );
    }
    otherwise( )
    {
      expI= 0.;
    }
    
    if( computeMap )
    {
      where( rr>=.75 )
      {
        x(I,axis)=1.;
      }
      otherwise( )
      {
        x(I,axis)=expI;
      }
    }
    if( computeMapDerivative )
    {
      where( rr <= .25 || rr>=.75 )
      {
        xr(I,axis,axis)=0.;
      }
      otherwise( )
      {
        xr(I,axis,axis)=(-beta*( 2.*rr*(rr-1)+5./8.)*exp( beta*( 2.*rr-1. )/( (rr-.25)*(.75-rr) ) ) )
                     *SQR(expI/((rr-.25)*(.75-rr)));
      }
    }
  }
  else if( stretchingType==noStretching )
  {
    if( computeMap )
      x(I,axis)=r(I,axis);
    if( computeMapDerivative )
      xr(I,axis,axis)= 1.;
  }
  else
  {
    printf("StretchMapping::map:ERROR: invalid stretchingType=%i\n",stretchingType);
    Overture::abort("error");
  }
  
  
}

// ========================================================================================================
//  
// Invert the stretching
// 
// ========================================================================================================
void StretchMapping::
inverseMapS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  if( stretchingType==noStretching )
  {
    Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
    const int axis=x.getBase(1);  // use this for the base
    if( computeMap )
      r(I,axis)=x(I,axis);
    if( computeMapDerivative )
      rx(I,axis,axis)= 1.;
  }
  else if(stretchingType==linearSpacing)
  {
    real r_coeff = 2/(1+a0);
    // real r2_coeff = 1-r_coeff;

    Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
    int i;
    int axis=x.getBase(1);  // use this for the base

    real *r_ptr = r.Array_Descriptor.Array_View_Pointer3;
    const int r_Dim0 = r.getRawDataSize(0);
#define r(i,j) r_ptr[ (i) + (j)*r_Dim0 ]
      
    real *x_ptr = x.Array_Descriptor.Array_View_Pointer3;
    const int x_Dim0 = x.getRawDataSize(0);
#define x(i,j) x_ptr[ (i) + (j)*x_Dim0 ]

    real *rx_ptr = rx.Array_Descriptor.Array_View_Pointer3;
    const int rx_Dim0 = rx.getRawDataSize(0);
    const int rx_Dim1 = rx.getRawDataSize(1)*rx_Dim0;
#define rx(i,j,k) rx_ptr[ (i) + (j)*rx_Dim0 + (k)*rx_Dim1]


    for( i=base ; i<=bound ; i++){
      real c = log( fabs(a0 / (a1-a0)) );
      real k = log( fabs(1+a0/(a1-a0)) ) - log( fabs(a0/(a1-a0)) );

      if(computeMap)
	r(i,axis) = ( log( fabs(x(i,axis) + a0/(a1-a0)) ) - c ) / k;
      if(computeMapDerivative)
	rx(i,axis,axis) = 1/k/( x(i,axis) + a0/(a1-a0) );
    };
#undef x
#undef r
#undef rx

  }
  else if( stretchingType==inverseHyperbolicTangent )  // inverseHyperbolicTangent 
  {
    if( unInitialized )
      initialize();
    if( debug & 64 )
      cout << "StretchMapping::inverseMap - params.isNull =" << params.isNull << endl;

    Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );

    real dummy;
    int i;
    const int axis=x.getBase(1);  // use this for the base
    if( computeMap && computeMapDerivative )
    {
      for( i=base; i<= bound; i++ )
      {	
	STTR( x(i,axis),r(i,axis),rx(i,axis,axis), iw(1),rw(1),ierr );
	if(  debug & 2 && ierr > 0 )
	  cout << "StretchMapping:Error from sttr, ierr = " << ierr << endl;
      }
    }  
    else
    {
      if( computeMap )
      {
	for( i=base; i<= bound; i++ )
	{ 
	  STTR( x(i,axis),r(i,axis),dummy, iw(1),rw(1),ierr );
	  if(  debug & 2 && ierr > 0 )
	    cout << "StretchMapping:Error from sttr, ierr = " << ierr << endl;
	}
      }    
      else
      {
	if( computeMapDerivative )
	{
	  for( i=base; i<= bound; i++ )
	  {	
	    STTR( x(i,axis),dummy,rx(i,axis,axis), iw(1),rw(1),ierr );
	    if(  debug & 2 && ierr > 0 )
	      cout << "StretchMapping:Error from sttr, ierr = " << ierr << endl;
	  }
	}
      }
    }
  }
  else if( stretchingType==exponentialToLinear )
  {
    // ---------------------------------------
    // --- exponential to linear streching --- 
    // ---------------------------------------

    if( unInitialized )
      initialize();

    Index I = getIndex( x,r,rx,base,bound,computeMap,computeMapDerivative );
    const int axis=x.getBase(1);  // use this for the base

    real *rp       = r.Array_Descriptor.Array_View_Pointer1;
    const int rDim0=r.getRawDataSize(0);
    #undef R
    #define R(i0,i1) rp[i0+rDim0*(i1)]
    const real *xp =x.Array_Descriptor.Array_View_Pointer1;
    const int xDim0=x.getRawDataSize(0);
    #undef X
    #define X(i0,i1) xp[i0+xDim0*(i1)]
    real *rxp=rx.Array_Descriptor.Array_View_Pointer2;
    const int rxDim0=rx.getRawDataSize(0);
    const int rxDim1=rx.getRawDataSize(1);
    #undef RX
    #define RX(i0,i1,i2) rxp[i0+rxDim0*(i1+rxDim1*(i2))]


    const real a = a1,  b=b1;
    // Avoid evaluating the log for arguements less than: 
    const real epsForLog=REAL_EPSILON;  // this could be smaller but probably not important

    real x0,scale;

    if( c1==0. || c1==1. )
    {
      // --- Invert boundary stretching ---
      const real s = 1.-2.*c1;   // s = +1 or -1 for c1=0. or 1. 
      if( c1==0. )
      {
	x0 = log(1.+a);                  // "x(0)"
	scale = log(1.+a*exp(b)) -x0;    // "x(1)-x(0)" 
      }
      else
      {
	x0 = log(1.+a*exp(b));        // "x(0)"
	scale = log(1.+a) -x0;        // "x(1)-x(0)" 
      }
      
      // Forward map:
      //   x =  ( log(1.+a*exp(s*b*(r-c1))) -x0)/scale;
      // Inverse: 
      //  r = log[ {exp( x*scale + x0 ) - 1 }/a ]/b 
      //  r.x = exp( .. )/( exp(..) - 1. )*(scale/b)
      
      const real sb=s*b;
      const real ai=1./a;
      const real bi=1./sb; 
      const real sbi=scale*bi; // scale=1./d 
      
      for( int i=base; i<=bound; i++ )
      {
        real xx = X(i,axis)*scale+x0;
	real ex = exp(xx);
	real exm1 = ex-1.;
	if( computeMap )
	{
          if( exm1>epsForLog ) // *wdh* 110904 -- avoid nans
            R(i,axis) = log( exm1*ai )*bi + c1;
          else
            R(i,axis) = Mapping::bogus;

	  // if( R(i,axis)!=R(i,axis) )
	  // {
          //   printF("StretchMapping:inverseMapS(1): ERROR: nan found R(i=%i,axis=%i)=%e\n"
          //          "  x=%e x0=%e xx=%e, ex=exp(xx)=%e, ai=%e, (ex-1.)=%e (ex-1.)*ai=%e \n",
          //          i,axis,R(i,axis),X(i,axis),x0,xx,ex,ai,(ex-1.),(ex-1.)*ai);
	  // }

	}
	if( computeMapDerivative )
	{
	  RX(i,axis,axis) = ( ex/(ex-1.) )*sbi;
	}

      }
    }
    else
    {
      // -- Invert interior stretching 0 < c1 < 1 --

      real ebc = exp(b*c1);
      real ebcm= exp(b*(c1-1.));
      x0 = log( (1.+a/ebc)/(1.+a*ebc) );        // "x(0)"
      scale = log( (1.+a/ebcm)/(1.+a*ebcm) ) -x0;    // "x(1)-x(0)" 
      
      
      // Forward map:
      //   x =  [ log{ (1.+a*exp(b*(r-c1)/(1+a*exp(-b*(r-c1)) } -x0 ]/scale;
      //     =  [ log{ (1.+a*z)/(1+a/z) } -x0 ]/scale,  where z=exp(b*(r-c1))
      // Inverse map:
      //   -> 1+a*z = eta*( 1+a/z) , where  eta=exp(x*scale+x0)
      //   ->  a*z^2 + (1-eta)*z - eta*a = 0 
      //       z = [ (eta-1) + sqrt( (eta-1)^2 + 4*eta*a^2 ) ]/(2a)
      //     r = log( z )/b + c1 
      //
      //   r.x = 1/(b*z)*z.x
      //   a*z.x = eta.x*(1+a/z) -eta*a*z.x/z^2 
      //   z.x = (1/a)*(eta.x*(1+a/z)-1)/( 1+eta/z^2 )
      const real ai=1./a;
      const real bi=1./b;
      for( int i=base; i<=bound; i++ )
      {
	real xx = X(i,axis)*scale+x0;
        real eta = exp(xx);
        real z = ( (eta-1.) + sqrt( SQR(eta-1.) + 4.*eta*a*a) )/(2.*a);
      
	if( computeMap )
	{
          if( z>epsForLog ) // *wdh* 110904 -- avoid nans
  	    R(i,axis) =  log( z )*bi + c1;
          else
            R(i,axis) = Mapping::bogus;

	  //if( R(i,axis)!=R(i,axis) )
	  //{
          //  printF("StretchMapping:inverseMapS(2): ERROR: nan found R(i=%i,axis=%i)=%e\n",i,axis,R(i,axis));
	  //}
	  
	}
	if( computeMapDerivative )
	{
          real zx = ai*( eta*(z+a)*scale )/(z+eta/z);
	  RX(i,axis,axis) = (bi/z)*zx;
	}
	
      }
    }
    
  }  


  else 
  {
    if( !getInvertible() )
    {
      printF("StretchMapping::inverseMap:ERROR: this mapping is NOT invertible! \n");
      OV_ABORT("error"); 
    }
    // printF("StretchMapping::inverseMapS: inverseIsDistributed=%i\n",(int)inverseIsDistributed);

    // -- we cannot invert the exponential mapping
    setBasicInverseOption(canDoNothing);  // do this to avoid a recursion below:  

    Mapping::inverseMapS( x,r,rx,params ); // no inverse for hyperbolic tangent and exponential (could be though!)
  }

}

// define a basic inverse too
void StretchMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{ 
  inverseMapS( x,r,rx,params );
}

void StretchMapping::
display( const aString & label ) const
{ // Display information about the mapping
  cout << "======================================================" << endl;
  cout << "StretchMapping::display: " << label << endl;
  if( stretchingType==inverseHyperbolicTangent )
  {
    cout << "numberOfLayers = " << numberOfLayers << endl;
    cout << "numberOfIntervals = " << numberOfIntervals << endl;
    cout << "numberOfSplinePoints = " << numberOfSplinePoints << endl;
    cout << "iopt = " << iopt << endl;
    cout << "unInitialized = " << unInitialized << endl;
    printf(" ra = %e, rb = %e, r0 = %e, r1 = %e\n",ra,rb,r0,r1);
    abc.display("abc");
    def.display("def");
    iw.display("iw");
    rw.display("rw");
  }
  else if( stretchingType==hyperbolicTangent )
  {
    cout << "stretching type is hyperbolicTangent: x = (a0 + ar*r + a1*tanh(b1*(r-c1)) +origin)*scale \n";
    printf(" a0=%e, ar=%e, a1=%e, b1=%e, c1=%e \n",a0,ar,a1,b1,c1);
  }
  else if( stretchingType==exponential )
  {
    cout << "stretching type is exponential: x = (a0 + ar*r + a1* exp(b1*(r-c1) +origin)*scale\n";
    printf(" a0=%e, ar=%e, a1=%e, b1=%e, c1=%e, origin=%e, scale=%e \n",a0,ar,a1,b1,c1,origin,scale);
  }
  else if( stretchingType==exponentialToLinear )
  {
    printF("stretching type is exponentialToLinear: \n"
           "    x(r) = [ log( 1 + a*exp(s*b*(r-c)) ) - x0 ]*scale \n"
           "  a = dxMin/dxMax (e.g. 1.e-2 or 1.e-3 ),\n"
           "  b = stretching exponent (you probably want a*exp(b*r) > 5 ), \n"
           "  c = 0. or 1. to put the stretching ar r=0 or r=1. (then s=1-2*c is +1 or -1),\n"
           "  x0 and scale are chosen automatically so that x(0)=0 and x(1)=1.\n"
           " a=%e, b=%e, c=%e \n",a1,b1,c1);
  }
  else if( stretchingType==noStretching )
  {
    printF("stretching type is noStretching\n");
  }
  else
  {
    printF("ERROR: unknown stretching type=%i\n",(int)stretchingType);
  }
  
  
  Mapping::display();  
  cout << "======================================================" << endl;

  
}


//=============================================================================
//   Prompt for changes to parameters
//   
//=============================================================================
int StretchMapping::
update( MappingInformation & mapInfo ) 
{

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  char buff[180];  // buffer for sprintf
  aString menu[] = 
    {
      "!StretchMapping",
      "stretching type",
      "layers",
      "intervals",
      "end points",
      "scale parameters",
      "hyperbolic tangent parameters",
      "exponential parameters",
      "exponentialToLinear parameters",
      "normalize",
      "plot first derivative", 
      "plot second derivative",
      " ",
      "lines",
      "boundary conditions",
      "share",
      "mappingName",
      "periodicity",
      "check",
      "check inverse",
      "show parameters",
      "plot",
      "help",
      "exit", 
      "" 
     };
  aString help[] = 
    {
      "stretching type    : choose type of stretching",
      "layers             : Enter a*tanh(b*(r-c)) layer stretching",
      "intervals          : Enter interval stretching",
      "end points         : set rmin and rmax",
      "scale parameters   : set scaling parameters origin and scale",
      "hyperbolic tangent parameters : parameters for hyperbolic tangent stretching",
      "exponential parameters : parameters for exponential stretching",
      "exponentialToLinear parameters : parameters for exponentialToLinear stretching",
      "normalize          : normalize the function to [0,1] or not",
      "lines              : specify number of grid lines",
      "boundary conditions: specify boundary conditions",
      "share              : specify share values for sides",
      "mappingName        : specify the name of this mapping",
      "periodicity        : set the periodicity of the stretching function",
      "check              : check the mapping and derivatives",
      "check inverse      : input points to check the inverse",
      "show parameters    : print current values for parameters",
      "plot               : enter plot menu (for changing ploting options)",
      "help               : Print this list",
      "exit               : Finished with changes",
      "" 
    };

  aString answer,line; 

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
//  parameters.set(GI_TOP_LABEL,"Stretching Function");
  int trial;

  gi.appendToTheDefaultPrompt("Stretch>"); // set the default prompt

  for( int it=0;; it++ )
  {
    if( it==0 && plotObject )
      answer="plotObject";
    else
      gi.getMenuItem(menu,answer);
 

    if( answer=="stretching type" ) 
    {
      aString menu[] = {  
                         "inverse hyperbolic tangent",
                         "hyperbolic tangent",
                         "exponential",
                         "exponentialBlend",
                         "linear spacing",
                         "exponentialToLinear",
                         "no stretching",
                         ""
                       }; 
      int response = gi.getMenuItem(menu,answer,"Enter the stretching type");   

      stretchingType = (answer=="inverse hyperbolic tangent" ? inverseHyperbolicTangent :
			answer== "hyperbolic tangent" ? hyperbolicTangent : 
			answer== "exponential" ? exponential : 
			answer== "exponentialBlend" ? exponentialBlend : 
			answer== "linear spacing" ? linearSpacing : 
			answer== "exponentialToLinear" ? exponentialToLinear : noStretching );
      if( stretchingType==hyperbolicTangent )
      {
	a0=0.; ar=1.; b1=5.; a1=-.9*ar/b1; c1=.5;   // choose a0 > a1*b1 to be invertible
      }
      else if( stretchingType==exponential )
      {
	a0=0.; ar=1.; b1=5.; a1=1.; c1=.5;   // choose a0 > a1*b1 to be invertible
      }
      else if( stretchingType==exponentialToLinear )
      {
	a1=.01; b1=10.; c1=0.;
      }
	
      mappingHasChanged(); 
      unInitialized=true;
    }
    else if( answer=="layers" ) 
    {
      if( stretchingType==noStretching )
        setStretchingType(inverseHyperbolicTangent);

      if( stretchingType!=inverseHyperbolicTangent )
      {
	gi.outputString("ERROR: you should first set the stretching type to inverse hyperbolic tangent");
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter number of layers (default=%i): ",numberOfLayers));
	if( line!="" ) sScanF(line,"%i",&numberOfLayers);
	setNumberOfLayers(numberOfLayers);
	for( int i=0; i<numberOfLayers; i++)
	{
	  for( trial=1; trial<5; trial++ )
	  {
            if( abc(0,i)==0. )
	    { // set some default values
	      abc(0,i)=1.;
	      abc(1,i)=5.;
	      abc(2,i)=.5;
	    }
	    gi.inputString(line,sPrintF(buff,"Enter a,b,c for layer %i (default=(%f,%f,%f)): ",
					i,abc(0,i),abc(1,i),abc(2,i)));
	    if( line!="" ) sScanF(line,"%e %e %e",&abc(0,i),&abc(1,i),&abc(2,i));
	    if( abc(0,i)<0. || abc(1,i)<0. || abc(2,i)<0. || abc(2,i) > 1. )
	      cout << "ERROR: values should satisfy a>0, b>0 and 0<= c <=1   ...try again\n";
	    else
	      break;
	  }
	}
        mappingHasChanged(); 
        unInitialized=TRUE;
      }
    }
    else if( answer=="intervals" )
    {
      //   e(j) > 0 j=1,..,nv   f(1) <= 1 f(nv) >= 0  0 =< f(j) =< 1 j=2,..,nv-1
      //                        f(1) < f(2) < f(3) ... < f(nv)
      if( stretchingType==noStretching )
        setStretchingType(inverseHyperbolicTangent);

      if( stretchingType!=inverseHyperbolicTangent )
      {
	gi.outputString("ERROR: you should first set the stretching type to inverse hyperbolic tangent");
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter number of intervals (default=%i): ",numberOfIntervals));
	if( line!="" ) sScanF(line,"%i",&numberOfIntervals);
	setNumberOfIntervals(numberOfIntervals);
	if( numberOfIntervals>0 )
	{
          int i;
	  for( i=0; i<numberOfIntervals; i++)
	  {
	    for( trial=1; trial<5; trial++ )
	    {
	      gi.inputString(line,sPrintF(buff,"Enter d,e,f for layer %i (default=(%f,%f,%f)): ",
					  i,def(0,i),def(1,i),def(2,i)));
	      if( line!="" ) sScanF(line,"%e %e %e",&def(0,i),&def(1,i),&def(2,i));
	      if( def(1,i)<0. || def(2,i)<0. || def(2,i) > 1. || (i>0 && def(2,i)<=def(2,i-1)) )
		cout << "ERROR: values should satisfy e(i)>0, 0<=f(i)<=1, f(i)>f(i-1)   ...try again\n";
	      else
		break;
	    }
	  }
	  i=numberOfIntervals;
	  for( trial=1; trial<5; trial++ )
	  {
	    gi.inputString(line,sPrintF(buff,"Enter f(i) (default=%f): ",
					i,def(2,i)));
	    if( line!="" ) sScanF(line,"%e ",&def(2,i));
	    if( def(2,i)<=def(2,i-1) )
	      cout << "ERROR: values should satisfy  f(i)>f(i-1)   ...try again\n";
	    else
	      break;
	  }
	}
	mappingHasChanged(); 
        unInitialized=TRUE;
      }
    }
    else if( answer=="periodicity" )
    {
      if( stretchingType!=inverseHyperbolicTangent )
	gi.outputString(" only a inverse hyperbolic tangent stretching can be periodic");
      else
      {
	aString periodicMenu[] = {"derivative of the function is periodic",
				 "function is not periodic",
				 "" };  
	gi.getMenuItem(periodicMenu,answer);
	if(answer=="derivative of the function is periodic")
	  setIsPeriodic(TRUE);
	else if(answer=="function is not periodic")
	  setIsPeriodic(FALSE);
	else
	  cout << "unknown resposnse! answer =" << answer << endl;
      }
    }
    else if( answer=="end points" )
    {
      if( stretchingType!=inverseHyperbolicTangent )
      {
	gi.outputString("ERROR: you should first set the stretching type to inverse hyperbolic tangent");
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter rmin,rmax (default=(%e,%e)): ",ra,rb));
	if( line!="" )
	{
	  sScanF(line,"%e %e",&ra,&rb);
	  setEndPoints(ra,rb);
	}
	mappingHasChanged(); 
        unInitialized=TRUE;
      }
    }
    else if( answer=="scale parameters" )
    {
      if( stretchingType==noStretching )
        setStretchingType(inverseHyperbolicTangent);

      if( stretchingType!=inverseHyperbolicTangent )
      {
	gi.outputString("ERROR: you should first set the stretching type to inverse hyperbolic tangent");
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter origin,scale (default=(%e,%e)): ",r0,r1));
	if( line!="" ) 
	{
	  sScanF(line,"%e %e",&r0,&r1);
	  setScaleParameters(r0,r1);
	}
	mappingHasChanged(); 
        unInitialized=TRUE;
      }
    }
    else if( answer=="hyperbolic tangent parameters" )
    {
      if( stretchingType!=hyperbolicTangent )
      {
	gi.outputString("ERROR: you should first set the stretching type to hyperbolic tangent");
      }
      else
      {
        gi.outputString("Info: Choose ar > a1*b1 to make the function invertible");
	gi.inputString(line,sPrintF(buff,"Enter a0 ar a1 b1 c1 (default=(%e,%e,%e,%e,%e)): ",a0,ar,a1,b1,c1));
	if( line!="" ) 
	{
	  sScanF(line,"%e %e %e %e %e",&a0,&ar,&a1,&b1,&c1);
	}
        unInitialized=TRUE;
	mappingHasChanged(); 
      }
    }
    else if( answer=="exponential parameters" )
    {
      if( stretchingType!=exponential )
      {
	gi.outputString("ERROR: you should first set the stretching type to exponential");
      }
      else
      {
	gi.inputString(line,sPrintF(buff,"Enter a0 ar a1 b1 c1 (default=(%e,%e,%e,%e,%e)): ",a0,ar,a1,b1,c1));
	if( line!="" ) 
	{
	  sScanF(line,"%e %e %e %e %e",&a0,&ar,&a1,&b1,&c1);
	}
        unInitialized=TRUE;
	mappingHasChanged(); 
      }
    }
    else if( answer=="exponentialToLinear parameters" )
    {
      printF("INFO: exponentialToLinear stretching: \n"
	     "    x(r) = [ log( 1 + a*exp(s*b*(r-c)) ) - x0 ]*scale \n"
	     "  a = dxMin/dxMax (e.g. 1.e-2 or 1.e-3 ),\n"
	     "  b = stretching exponent (you probably want a*exp(b*r) > 5 ), \n"
	     "  c = 0. or 1. to put the stretching ar r=0 or r=1. (then s=1-2*c is +1 or -1),\n"
	     "  x0 and scale are chosen automatically so that x(0)=0 and x(1)=1.\n");

      gi.inputString(line,sPrintF(buff,"Enter a b c (default=(%e,%e,%e)): ",a1,b1,c1));
      if( line!="" ) 
      {
	sScanF(line,"%e %e %e",&a1,&b1,&c1);
      }
      if( stretchingType==exponentialToLinear )
      {
	unInitialized=true;
	mappingHasChanged(); 
      }
    }
    else if( answer=="normalize" )
    {
      gi.inputString(line,sPrintF(buff,"normalize the function to [0,1]? (yes/no) (current=%s): ",
				  (normalized ? "yes" : "no" )));
      if( line!="" )
      {
	normalized = line(0,0)=='y' ? TRUE : FALSE;
      }
      unInitialized=TRUE;
      mappingHasChanged(); 
    }
    else if( answer=="show parameters" )
    {
      display();
    }
    else if( answer=="plot" )
    {
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters); 
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
    }
    else if( answer=="plot first derivative" || answer=="plot second derivative" )
    {
      const int order = answer=="plot first derivative" ? 1 : 2;
      if( order==1 )
        parameters.set(GI_TOP_LABEL,"First derivative");
      else
        parameters.set(GI_TOP_LABEL,"Second derivatives");

      gi.erase();
      const int n = getGridDimensions(0);
      real dr = 1./max(1,n-1);
      realArray r(n,1);
      r.seqAdd(0.,dr);
      
      realArray x(n,rangeDimension),xr(n,rangeDimension,1);
      if( order==1 )
      {
        map(r,x,xr);
      }
      else
      {
        Index I = Range(n);
	for(  int axis=0; axis<rangeDimension; axis++ )
	  secondOrderDerivative(I,r,xr,axis,0);
      }
    
      real xrMax=max(fabs(xr));
      xr*=1./max(REAL_MIN*100.,xrMax);
      printf("Derivative scaled by %8.2e\n",xrMax);
      
      realArray xrd(n,1,1,2);
      Range R=n;
      for( int axis=0; axis<rangeDimension; axis++ )
      {
	xrd(R,0,0,0)=r(R,0);
	xrd(R,0,0,1)=xr(R,axis);
      
	DataPointMapping xrMap;
	xrMap.setDataPoints(xrd,3,1);
	xrMap.setIsPeriodic(axis1,getIsPeriodic(axis1));

	parameters.set(GI_MAPPING_COLOUR,"green");
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,false);
	PlotIt::plot(gi,xrMap,parameters);  
      }
      parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,true);
      gi.erase();
    }
    else if( answer=="help" )
    {
      for( int i=0; help[i]!=""; i++ )
        gi.outputString(help[i]);
    }
    else if( answer=="lines"  ||
             answer=="boundary conditions"  ||
             answer=="share"  ||
             answer=="mappingName" ||
             answer=="check"||
             answer=="check inverse" )
    { // call the base class to change these parameters:
      mapInfo.commandOption=MappingInformation::readOneCommand;
      mapInfo.command=&answer;
      Mapping::update(mapInfo); 
      mapInfo.commandOption=MappingInformation::interactive;
      plotObject=true;
    }
    else if( answer=="exit" )
      break;
    else if( answer=="plotObject" )
      plotObject=true;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s\n",(const char*)answer) );
      gi.stopReadingCommandFile();
    }

    if( stretchingType==inverseHyperbolicTangent && (unInitialized || numberOfSplinePoints==0) )
    {
      // estimate the number of spline points needed (for inverse)
      int num=40;
      int i;
      for( i=0; i<numberOfLayers; i++)
	num+=int(abc(1,i));
      for( i=0; i<numberOfIntervals; i++)
	num+=int(def(1,i));
      setNumberOfSplinePoints(num);
      setGridDimensions(0,num);
    }
    if( plotObject )
    {
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);  

    }
  }
  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
  
}

