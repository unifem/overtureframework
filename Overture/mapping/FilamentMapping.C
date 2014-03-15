//
// FilamentMapping:
// ================
//
//       Implements a 'thick filament': Given a centerline,
//   builds a thicker filament with rounded ends. This can be used to 
//   compute a Hyperbolic (body-fitted) grid around the filam.
//
//   $Id: FilamentMapping.C,v 1.27 2013/01/01 22:55:16 henshaw1 Exp $
//

// AP: Is this really necessary?#include "Overture.h"
#include "FilamentMapping.h"
//#include "DeformingGridGenerationInformation.h"
#include "GenericGraphicsInterface.h"
//#include "MappingEnums.h"
#include "Mapping.h"
#include "SplineMapping.h"

#include "HyperbolicMapping.h"
#include "MappingInformation.h"

//David's Display class
// #include "Display.h"  // *wdh* removed this to allow separation of Mapping's into a separate library
#include "display.h"

#include "CircleMapping.h"

#include <math.h>
//kkc 081124 #include <iostream.h>
#include <iostream>

//kkc 081124 #include <fstream.h>
#include <fstream>

using namespace std;
//..if DEBUG_PRINT_SUB is true --> print subroutine name for every call
//#define DEBUG_PRINT_SUB false
#define DEBUG_PRINT_SUB true


//
// ----- FILAMENT DATA
//     ... Contains frequently changing, application dependent, data for
//         the FilamentMapping
//

// ==============================================================================================================
/// \brief Class that contains frequently changing, application dependent, data for the FilamentMapping.
// ==============================================================================================================
class FilamentData {
public:
  bool  isEndStorageInitialized;
  int   nLeadingEndPoints;
  int   nTrailingEndPoints;
  real  leadingEndSpacing;
  real  trailingEndSpacing;
  real  leadingEndExtra;
  real  trailingEndExtra;
};

//computeNumberOfEndPoints( leadingEndSpacing,  trailingEndSpacing );
//..Could I have, inside FilamentMapping, 'computeLeadingEndSpacing( nFilamPts, stretching)'? or something?

//..TRAVELING WAVE DATA -- FOR INTERNAL USE

/// \brief Container class for the FilamentMapping.
class TravelingWaveParameters
{
public:
  real xOffset, yOffset;  // leading edge of filament
  real a, b;              // amplitude parameters
  real omega, knum;       // time & space freq. for wave

  void setParameters( real x00,     real y00, 
		      real a00,     real b00, 
		      real omega00, real knum00) 
  {
    xOffset=x00; yOffset=y00; a=a00; b=b00; 
    omega=omega00; knum=knum00;
  };
};

//
// ------------------------------------------ FilamentStretching 
//   .. essentially an internal class 
//   ..  within FilamentMapping
//
//const int maxFilamentStretchingLayers = 4;

FilamentStretching::
FilamentStretching() 
{ 
  useStretchingFlag=FALSE; pStretchMap=NULL; numberOfLayers = 0;
  for(int i=0;i<maxFilamentStretchingLayers; ++i) {
    aparam[i]=1.; bparam[i]=10.; 
    cparam[i]=(i-1.)/(maxFilamentStretchingLayers-1.);
  }
  cparam[0] = 0.;  //first layer at the head
  cparam[1] = 1.;  //second layer at the tail
  initialize();
  setNumberOfLayers(2); 
  useStretchingFlag=FALSE;  //want stretchMap initialized, but turned off by default
}

FilamentStretching::
~FilamentStretching() 
{
  delete pStretchMap; 
}

void FilamentStretching::
initialize()
{
  useStretchingFlag=TRUE;
  if( pStretchMap != NULL ) delete pStretchMap;
  pStretchMap  = new StretchMapping( StretchMapping::inverseHyperbolicTangent);
}

bool FilamentStretching::
isUsed() 
{
  return useStretchingFlag;
}


void FilamentStretching::
setNumberOfLayers( int numLayers_) 
{
  if (pStretchMap==NULL) initialize();
  if( numLayers_ <= maxFilamentStretchingLayers ) {
    numberOfLayers = numLayers_;
    pStretchMap->setNumberOfLayers( numberOfLayers);
    copyStretchLayerParameters();
  } else printf("ERROR::FilamentStretching  too many layers %i, not changed\n",numLayers_);
}

void FilamentStretching::
setAllLayerParameters( real a, real b ) 
{
  for (int i=0; i<numberOfLayers; ++i) {     
    aparam[i] = a; bparam[i] = b; 
  }
  copyStretchLayerParameters();
}

void FilamentStretching::
setLayerParameters( int i,  real a, real b, real c ) 
{
  if (pStretchMap==NULL) {
    initialize();
    setNumberOfLayers( 2 ); //default value for # layers
  }
  if( (i>=0) && (i<=maxFilamentStretchingLayers)) {
    aparam[i] = a; bparam[i] = b; cparam[i] =c;
  }
  pStretchMap->setLayerParameters( i, a,b,c);
}

void FilamentStretching::
getLayerParameters( int i,  real &a, real &b, real &c ) 
{
  a= aparam[i]; b= bparam[i]; c= cparam[i];
}

void FilamentStretching::
copyStretchLayerParameters() 
{
  for (int i=0; i<numberOfLayers; ++i) {     
    pStretchMap->setLayerParameters( i, aparam[i], bparam[i], cparam[i] );
  }
} 

void FilamentStretching::
useStretching( bool stretchFlag_ /* = true */ )
{
  if (pStretchMap==NULL) {
    initialize();
    setNumberOfLayers( 2 ); //default value for # layers
  }
  useStretchingFlag = stretchFlag_;
  //if(useStretchingFlag) setNumberOfLayers( numLayers_ );
}

void FilamentStretching::
setStretchingType(  StretchMapping::StretchingType st /*=StretchMapping::inverseHyperbolicTangent*/)
{
  if(pStretchMap ==NULL) {
    initialize();
    setNumberOfLayers( 2 ); //default value for # layers
  }
  pStretchMap->setStretchingType( st );
}

void FilamentStretching::
map( const realArray &r, realArray &x ) 
{
  if( isUsed() && (pStretchMap!=NULL) ) {
    pStretchMap->map(r,x);
  } 
  else {
    x.redim(r);
    x=r;
  }
}

int FilamentStretching::
put( GenericDataBase & dir, const aString & name) const 
{
  int i;
  char aName[80], bName[80], cName[80];
  //..Create a derived data-base object & create sub-directory
  GenericDataBase & subDir = *dir.virtualConstructor();    
  dir.create(subDir,name,"FilamentStretching"); 
  
  subDir.put( (int)useStretchingFlag, "useStretchingFlag");
  subDir.put( numberOfLayers,         "numberOfLayers");
  for( i=0; i< numberOfLayers; ++i ) {
    sPrintF( aName,"aparam %i",i ); 
    sPrintF( bName,"bparam %i",i ); 
    sPrintF( cName,"cparam %i",i );
    subDir.put( aparam[i], aName );
    subDir.put( bparam[i], bName );
    subDir.put( cparam[i], cName );
  }
  
  int exists= pStretchMap!=NULL;
  subDir.put(exists, "stretchMapExists");
  if (exists ) {
    subDir.put( pStretchMap->getClassName(), "pStretchMap->className");
    pStretchMap->put( subDir, "StretchMapping");
  }
  delete &subDir;
  return 0;
}

int FilamentStretching::
get( GenericDataBase & dir, const aString & name) 
{
  int i, temp;
  char aName[80], bName[80], cName[80];
  //..Create a derived data-base object & create sub-directory
  GenericDataBase & subDir = *dir.virtualConstructor();    
  dir.find(subDir,name,"FilamentStretching"); 
  
  subDir.get( temp,              "useStretchingFlag"); useStretchingFlag = (bool)temp;
  subDir.get( numberOfLayers,    "numberOfLayers");
  for( i=0; i< numberOfLayers; ++i ) {
    sPrintF( aName,"aparam %i",i ); 
    sPrintF( bName,"bparam %i",i ); 
    sPrintF( cName,"cparam %i",i );
    
    subDir.get( aparam[i], aName );
    subDir.get( bparam[i], bName );
    subDir.get( cparam[i], cName );
  }
  
  int exists= 0;
  subDir.get(exists, "stretchMapExists");
  if (exists ) {
    aString mappingClassName;
    subDir.get( mappingClassName, "pStretchMap->className");
    if( mappingClassName != "StretchMapping" )
      {
	cout << "FilamentMapping--Stretching::get ERROR looking for a StretchMapping, "
	     << "can't load a className=" 
	     << (const char *) mappingClassName << endl;
      }
    pStretchMap->get( subDir, "StretchMapping");
  }
  delete &subDir;
  return 0;
}


//
//..Constructor & Destructor  --------------- FilamentMapping
//

FilamentMapping::
FilamentMapping(  int nFilamentPoints00,     /* =17   */ 
		  int nEndPoints00,          /* =3    */
		  real thickness00,          /* =0.04 */
		  real endRadius00           /* =0.02 */
		  )		      
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::FilamentMapping called.\n";
  filamentData = new FilamentData;
  filamentData->isEndStorageInitialized = FALSE;
  filamentData->nLeadingEndPoints  =  -1;
  filamentData->nTrailingEndPoints = -1;
  filamentData->leadingEndSpacing  = -7;
  filamentData->trailingEndSpacing = -7;
  filamentData->leadingEndExtra    = 1.;
  filamentData->trailingEndExtra   = 1.;

  constructor( nFilamentPoints00, 
	       thickness00,
	       endRadius00 );
  //constructor( nFilamentPoints00,   //old--no more nEndPoints 
  //	       nEndPoints00,
  //	       thickness00,
  //	       endRadius00 );
}


FilamentMapping::
~FilamentMapping()
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::~FilamentMapping called.\n";

  //delete( filamentData ); //**now done at the end **pf

  //..Delete filament core & boundary splines 
  if (pFilament!=NULL && pFilament->decrementReferenceCount()==0 ) {
    delete(pFilament); if( debug & 2 ) printf("~FilamentMapping: delete pFilament\n");
  }  else  {
    if( debug & 2 ) printf("~FilamentMapping: do NOT delete pFilament\n");
  }

  if (pThickFilament!=NULL && pThickFilament->decrementReferenceCount()==0 )   {
    delete(pThickFilament);
  }  else  {
    if( debug & 2 ) printf("~FilamentMapping: do NOT delete pThickFilament\n");
  }

  if (pUserDefinedCoreCurve!=NULL && pUserDefinedCoreCurve->decrementReferenceCount()==0)   {
    delete pUserDefinedCoreCurve;
  }  else  {
    if( debug & 2 ) printf("~FilamentMapping: do NOT delete pUserDefinedCoreCurve\n");
  }

  //..Delete Bodyfitted map
  if (pHyper!=NULL && pHyper->decrementReferenceCount()==0)   {
    delete(pHyper);
  }  else  {
    if( debug & 2 ) printf("~FilamentMapping: do NOT delete pHyper\n");
  }

  //..Delete parameters..
  if (filamentData!=NULL) delete(filamentData);
}

// ..old -- Constructor code -- had nEndPoints as an input
//void FilamentMapping::
//constructor(  int nFilamentPoints00, 
//	      int nEndPoints00,
//	      real thickness00,     
//	      real endRadius00)

// ..Constructor code
void FilamentMapping::
constructor(  int nFilamentPoints00, 
	      real thickness00,     
	      real endRadius00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::constructor called.\n";
  //..Set default values
  numberOfDimensions = 2;   // 2D always: =RANGE, and =DOMAIN of bodyfitted
  pUserDefinedCoreCurve=NULL; // User can define the centerline curve

  motionType           = TRAVELING_WAVE_MOTION;
  filamentType         = OPEN_FILAMENT;
  filamentBoundaryType = THICK_FILAMENT_BOUNDARY;
  //motionType           = NO_MOTION;
  //filamentType         = CLOSED_FILAMENT;
  //filamentBoundaryType = ONE_SIDED_FILAMENT_BOUNDARY;

  //debug=7;  // FOR DEBUGGING THIS CREEPY CREATURE **pf
  //debug=3;
  debug=0;
  filamentLength =  1.;
  xOffset=          0.;
  yOffset=          0.;
  thickness= thickness00;
  endRadius= endRadius00;

  //nThickSplinePoints=  2*(2*nFilamentPoints+2*nEndPoints) + 1; //not used
  //nEndPoints=          nEndPoints00;                           //nEndPts is obsolete

  if (debug & 4) 
  {
    cout << "FILAMENTMapping:: Constructor--------\n";
    cout << "  nFilamentPoints     ="<< nFilamentPoints <<endl;
    cout << "  nEndPoints          ="<< nEndPoints      <<endl;
    cout << "  nSplinePoints       ="<< nSplinePoints << endl;
    cout << "  nThickSplinePoints  ="<< nThickSplinePoints <<endl;
    cout << endl;
  }

  timeOffset = 0.;
  xOffsetCurrent = xOffset;
  yOffsetCurrent = yOffset;

  //..Default values for TravelingWave parameters

  filamentLength               = 1.0 ;
  aTravelingWaveAmplitude      = 0.1 ;
  bTravelingWaveAmplitude      = 0.05;
  timeFrequencyTravelingWave   = 1.0;
  spaceFrequencyTravelingWave  = 1.0;
  timeForFilament              = 0.0;

  //..Default values for ClosedFilament parameters [HLS] initial data
  //radius        =  1.;
  //  perturbation1 =  0.1;
  //circleMode1   =  2;
  //circlePhase1  =  1./4.;  // times pi=the phase change. =1/4--> mode 1=SINE
  //perturbation2 =0.1;
  //circleMode2   =  3; 
  //circlePhase2  =  0.;     // =0 ->mode 2 is cosine
  radius        =  1.;
  perturbation1 =  0.2;
  circleMode1   =  2;
  circlePhase1  =  1./4.;  // times pi=the phase change. =1/4--> mode 1=SINE
  perturbation2 =0.1;
  circleMode2   =  5; 
  circlePhase2  =  0.;     // =0 ->mode 2 is cosine

  //..BY THE WAY -- this code is braindead -- I should init the dims later & not do it here, at all
  //.....**pf
  nFilamentPoints=     nFilamentPoints00;
  //real spacing= filamentLength/(nFilamentPoints-1.);
  //int ntemp; //= nEndPoints
  //computeFlatSmoothSpacing( spacing, thickness, ntemp);
  //filamentData->nLeadingEndPoints   = ntemp;
  //filamentData->nTrailingEndPoints  = ntemp;
  //filamentData->leadingEndSpacing   = spacing;
  //filamentData->trailingEndSpacing  = spacing;
  nEndPoints = -1; // not used anymore

  //XXFIX -- initialize stretching
  //      -- compute leading edge spacing & trailing edge spacing
  //      -- store in  filamentData-> xxx

  //
  //..Create splines
  //
  nSplinePoints=       nFilamentPoints;      // ---SHOULD this BE MORE GENERAL? Yes, allow,TODO **pf
  pFilament=      new SplineMapping( 2 ); pFilament->incrementReferenceCount();
  assert( pFilament!=NULL );

  //....set properties of the splines
  SplineMapping::ParameterizationType splineParamType;

  splineParamType = SplineMapping::index;
  pFilament->setShapePreserving(TRUE);
  pFilament->setParameterizationType( splineParamType );
  pFilament->setName( Mapping::mappingName, "filament-centerline" );

  //..More data inits
  //pTravelingWaveData = NULL; //NOT USED
  // pDisplay= new Display();  // *wdh* removed this to allow separation of Mapping's into a separate library
  // assert( pDisplay != NULL );

  //..Initialize storage & update the filament centerline & thick filament
  //isFilamentInitialized         =FALSE;
  xThickFilament.redim(0);
  xLeadingEnd.redim(0);
  xTrailingEnd.redim(0);

  setFilamentStorageNeedsUpdate();
  centerLineNeedsUpdate         =TRUE;
  geometricDataNeedsUpdate      =TRUE;    
  initializeFilamentStorage(); 
  evaluateCenterLineAtTime( timeForFilament );  // needs to be paired with 'compThickFilam'... <**>
  updateNumberOfEndPoints();

  nThickSplinePoints=    getNumberOfThickFilamentPoints();
  pThickFilament = new SplineMapping( 2 ); pThickFilament->incrementReferenceCount();
  assert( pThickFilament!=NULL);
  pThickFilament->setShapePreserving(TRUE);
  pThickFilament->setParameterizationType( splineParamType );
  pThickFilament->setName( Mapping::mappingName, "filament-thickboundary" );
  
  computeThickFilament();                      // ... is paired with 'evalCenterLineAtTime'    <**>
  //evaluateAtTime( timeForFilament );

  //..Mapping initialization
  FilamentMapping::className = "FilamentMapping";
  setName( Mapping::mappingName, "filament" );

  setRangeDimension( numberOfDimensions );
  setDomainDimension( numberOfDimensions );

  //int gridDimension1             = 51;  // *wdh*
  //int gridDimension2             = 5;
  //int gridDimension1             =  81; //**pf
  int gridDimension1             = nThickSplinePoints ;
  int gridDimension2             =  8;
  setGridDimensions(axis1, gridDimension1 );
  setGridDimensions(axis2, gridDimension2 );

  
  //..INITIALIZE HYPERBOLIC MAPPING
  pHyper = new HyperbolicMapping;  assert( pHyper != NULL ); pHyper->incrementReferenceCount();
  pHyper->debug = 0;  // turn off debugging output!!
  // ** setBasicInverseOption(canInvert); // use fast inverse from HyperbolicMapping *wdh*

  //....GRID DIMENSIONS for Hyperbolic mapping (=bodyfitted map)

  pHyper->setGridDimensions(axis1, getGridDimensions(axis1));
  pHyper->setGridDimensions(axis2, getGridDimensions(axis2));
  pHyper->setRangeDimension( getRangeDimension() );
  pHyper->setDomainDimension( getDomainDimension() );

  setDefaultHyperbolicParameters();

  //....Boundary conditions
  int side, ax;

  ax=0; side=0;  
  setBoundaryCondition( side, ax, -1);
  pHyper->setBoundaryCondition( side, ax, -1);

  ax=0; side=1;  
  setBoundaryCondition( side, ax, -1);
  pHyper->setBoundaryCondition( side, ax, -1);

  setIsPeriodic(ax,functionPeriodic);         // *wdh*
  pHyper->setIsPeriodic(ax,functionPeriodic); // *wdh*

  ax=1; side=0;  
  setBoundaryCondition( side, ax, 1);
  pHyper->setBoundaryCondition( side, ax, 1);

  ax=1; side=1;  
  setBoundaryCondition( side, ax, 0);
  pHyper->setBoundaryCondition( side, ax, 1);

 // =should still generate the Hyperb. map with correct boundary curve
  bodyFittedMappingNeedsUpdate=TRUE;
}

//.. Initialize curve & thickFilament
void FilamentMapping::
initializeFilamentStorage()
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::initializeFilament called.\n";
  assert( filamentData != NULL );

  //  xAll.redim(nFilamentPoints,2,maximumNumberToSave);  // for tstepper
  normalVector.redim(nFilamentPoints,numberOfDimensions);
  tangentVector.redim(nFilamentPoints,numberOfDimensions);
  normal_t.redim(nFilamentPoints,numberOfDimensions); 
  normal_tt.redim(nFilamentPoints,numberOfDimensions);  
  tangent_t.redim(nFilamentPoints,numberOfDimensions);  
  tangent_tt.redim(nFilamentPoints,numberOfDimensions);  

  //..no need to set number of thick filament points
  //getNumberOfThickFilamentPoints()= 2*nFilamentPoints + 2*nEndPoints + 1; // ADD one for periodic
  
  xTop.redim(nFilamentPoints,numberOfDimensions);
  xBottom.redim(nFilamentPoints,numberOfDimensions);
  xFilament.redim(nFilamentPoints,numberOfDimensions);
  //..NO NEED to redim these here --> redim'd in computeThickFilament et al
  //xThickFilament.redim(getNumberOfThickFilamentPoints(),numberOfDimensions);
  //..also xLeadingEnd, xTrailing --> redim'd in compThickFilam

  x.redim(nFilamentPoints,numberOfDimensions);
  xr.redim(nFilamentPoints,numberOfDimensions);
  r.redim(nFilamentPoints);
  sr.redim(nFilamentPoints);
  amplitude.redim(nFilamentPoints);
  dAmplitude.redim(nFilamentPoints);

  x_t.redim(nFilamentPoints,numberOfDimensions);       
  x_tt.redim(nFilamentPoints,numberOfDimensions);
  xr_t.redim(nFilamentPoints,numberOfDimensions);      
  xr_tt.redim(nFilamentPoints,numberOfDimensions);  

  centerLineNeedsUpdate=TRUE;
  isFilamentInitialized=TRUE;
  //if ( filamentData->isEndStorageInitialized ) setNumberOfThickSplinePoints();
}

void FilamentMapping::
setDefaultHyperbolicParameters()
{
  //....Set Hyperb grid gen. parameters
  real distanceToMarch            =  0.1;
  real dissipation                =  0.1;
  int linesInTheNormalDirection   =  getGridDimensions(axis2);
  cout << "<<setDefaultHyperbolicParameters::linesInTheNormalDirection = "
       << linesInTheNormalDirection<<endl;

  assert( pHyper != NULL );

  switch(filamentBoundaryType)
  {
    case THICK_FILAMENT_BOUNDARY:
      {
      distanceToMarch           = 0.1;
      dissipation               = 0.1;
      }
      break;
    case ONE_SIDED_FILAMENT_BOUNDARY:
      {
	if (filamentType == CLOSED_FILAMENT) 
	{
	  distanceToMarch           = 0.5;
	  dissipation               = 0.1;
	} 
	else 
	{
	  //.. keep defaults for now
	  cout << "WARNING:::setDefaultHyperbolicParameters "
	       << "-- unknown filamentType"<<filamentType<<endl;
        }
	//if (filamentType == _FILAMENT) 
	//{
	//  distanceToMarch           = 0.1;
	//  dissipation               = 0.1;
	//  linesInTheNormalDirection = gridDimension2;
        //}
      }
      break;
    default:
      cout << "WARNING::setDefaultHyperbolicParameters "
	   << "-- unknown filamentBoundaryType\n";
      break;
   }

  IntegerArray ipar(2); RealArray rpar(1); 
  ipar=0; rpar=0.;
  ipar(0)=0; rpar(0)=distanceToMarch;
  pHyper->setParameters( HyperbolicMapping::distanceToMarch, ipar, rpar);
  ipar(0)=0; rpar(0)=dissipation;
  pHyper->setParameters( HyperbolicMapping::dissipation, ipar, rpar);
  
  ipar(0)=0; ipar(1)=linesInTheNormalDirection;
  rpar=0.;  
  pHyper->setParameters( HyperbolicMapping::linesInTheNormalDirection, ipar); 
}


void FilamentMapping::
setCenterLineMapping( Mapping *pMapping )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setCenterLineMapping called.\n";
  if (pUserDefinedCoreCurve!=NULL && pUserDefinedCoreCurve->decrementReferenceCount()==0) 
  {
    delete pUserDefinedCoreCurve;
  }
  pUserDefinedCoreCurve = pMapping;
  pUserDefinedCoreCurve->uncountedReferencesMayExist(); // pMapping may not be reference counted.
  
  filamentType = USER_DEFINED_CURVE_FILAMENT;
  centerLineNeedsUpdate= TRUE; mappingHasChanged();
  //isFilamentInitialized = FALSE;  // need to reinit. the array sizes **pf, Jan 26
  setFilamentStorageNeedsUpdate();
}


// InitializeBodyFittedMapping
//   - set the surface for the HyperbolicMap
//   - uses the GIVEN SURFACE = *pSurface
// 
//    NOTE: must call "initializeFilament" before calling this
//
void FilamentMapping::
initializeBodyFittedMapping( Mapping *pSurface )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::initializeBodyFittedMapping(pSurface) called.\n";
  cout << "WARNING: INITIALIZEBODYFITTEDMAPPING( MAP ) called, could lead to trouble." 
       << " Use without args instead.\n";

  assert( pHyper != NULL );
  pHyper-> setSurface( *pSurface );
  initializeBodyFittedMapping();
}

// InitializeBodyFittedMapping
//   - set the surface for the HyperbolicMap
//   - uses the DEFAULT SURFACE, already set in pHyper
// 
//    NOTE: must call "initializeFilament" before calling this
//
void FilamentMapping::
initializeBodyFittedMapping( )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::initializeBodyFittedMapping() called.\n";
  assert( pHyper != NULL );
  setNumberOfThickSplinePoints(); // new -- **pf 
  regenerateBodyFittedMapping( pHyper );
}

bool FilamentMapping::
updateFilamentAndGrid()
{
  if ( debug & 2 )  cout << "$$ FilamentMapping::updateFilamentAndGrid\n";
  assert( filamentData != NULL );
  bool updateFlag = FALSE;
  if ( !isFilamentInitialized )
  {
    if (debug & 2) 
      cout << "  ...resize storage (updateFilamentAndGrid)\n";
    initializeFilamentStorage();
    updateFlag = TRUE;
  }
  if ( centerLineNeedsUpdate )
  {
    if (debug & 2)
      cout  << "  ...evaluate surface (updateFilamentAndGrid)\n";
    evaluateAtTime( timeForFilament );
    updateFlag = TRUE;
  }
  if ( geometricDataNeedsUpdate )             
  {
    if ( debug & 2) 
      cout << "  ...computeGeometricData (updateFilamentAndGrid)\n";
    computeGeometricData();
    //bodyFittedMappingNeedsUpdate=TRUE; //set by computeGeometricData
    updateFlag = TRUE;
  }
  if ( !filamentData->isEndStorageInitialized ) {
    if ( debug & 2) 
      cout << "  ...updateNumberOfEndPoints (updateFilamentAndGrid)\n";
    updateNumberOfEndPoints();
    updateFlag = TRUE;
  }
  if ( bodyFittedMappingNeedsUpdate )   
  {
    if ( debug & 2)  
      cout << "  ...regenerateBodyFittedMapping (updateFilamentAndGrid)\n";
    initializeBodyFittedMapping();
    updateFlag = TRUE;
  }
  return updateFlag;
}

//
// ..Get correct boundary surface for generating bodyfitted grids
//   * It is the caller's responsibility to check the map pointer,
//       which could be ==NULL
//
Mapping* FilamentMapping::
getSurfaceForHyperbolicMap()
{
  Mapping *pTempMapping = NULL;
  if ( filamentBoundaryType == THICK_FILAMENT_BOUNDARY ) 
  {
    pTempMapping = pThickFilament;
    if ( pTempMapping==NULL && (debug&1)) 
      cout << "WARNING, getSurfaceForHyperbolicMap(): pThickFilament==NULL, but shouldn't be\n";
    if (debug&4) cout << "\n getSurfaceForHyperbolicMap(): returning pThickFilament\n\n";
  }
  else if (filamentBoundaryType == ONE_SIDED_FILAMENT_BOUNDARY )
  {  
    pTempMapping = pFilament;
    if ( pTempMapping==NULL && (debug&1)) 
      cout << "WARNING, getSurfaceForHyperbolicMap(): pThickFilament==NULL, but shouldn't be\n";
    if (debug&2) cout << "\n getSurfaceForHyperbolicMap(): returning pFilament\n\n";  
  }
  else
  {
    cout <<"ERROR, getSurfaceForHyperbolicMap(): unknown FilamentBoundaryType="<<filamentBoundaryType
	 <<",  returning NULL for boundary map pointer...\n";
  }
  return (pTempMapping);
}

//
// ..Regenerates the Bodyfitted mapping
// .... uses the default = pHyper
//
void FilamentMapping::
regenerateBodyFittedMapping( )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::regenerateBodyFittedMapping() called.\n";
  assert( pHyper != NULL);
  regenerateBodyFittedMapping( pHyper );

}

//..Regenerates the _given_ hyperbolic mapping pHyper00
//....Sets the surface (in pHyper00) to be the filament curve
//....Assumes the params in pHyper00 are ok
//....Called by regenerateBodyfittedMapping()
//....This is for INTERNAL USE only!
//
void FilamentMapping::
regenerateBodyFittedMapping( HyperbolicMapping *pHyper00 )
{
  if (debug &2) cout <<"FilamentMapping::regenBodyFittedMapping(pHyper) called\n";
  assert( pHyper00 != NULL );

  if (debug&4) {
    cout << "\n\n@@ FilamMapping::regenBFM -- before pHyper->setSurf()\n\n";
    printHyperbolicDimensions(); //**pf DEBUG
  }

  //int axisKeep1= pHyper00->getGridDimensions(axis1);  // SO HYPERB won't change our dims
  //int axisKeep2= pHyper00->getGridDimensions(axis2);
  int axisKeep1= getGridDimensions(axis1);  // SO HYPERB won't change our dims
  int axisKeep2= getGridDimensions(axis2);
  if (debug&2) {
    cout << "FILAMENTMAPPING -- axis1,2 = ("<< axisKeep1 <<", "<<axisKeep2
	 << ")" <<endl;
  }
  //pHyper00->setSurface(*pThickFilament);
  Mapping *pTempSurface = getSurfaceForHyperbolicMap(); //either pThickFilament or pFilament
  assert( pTempSurface != NULL );
  cout << "FILAM--REGENBODYF, surf dim="
       << pTempSurface->getGridDimensions(axis1)
       << ",  nThickFilamPoints = "<< getNumberOfThickFilamentPoints()<<endl;
  pHyper00->setSurface( * pTempSurface );
  //  mappingHasChanged();

  if (debug&4) {
    cout << "\n\n@@ FilamMapping::regenBFM -- "
	 << "before pHyper->generateNew()\n\n";
    printHyperbolicDimensions(); //**pf DEBUG
  }
  pHyper00->generateNew(); // we assume params for HyperbMap are OK

  // **pf**  setSurf changes pHyper's axis1 -dimension
  // **pf**  generateNew changes pHyper's axis2 dimension (=linesToMarch)
  pHyper00->setGridDimensions(axis1, axisKeep1);
  pHyper00->setGridDimensions(axis2, axisKeep2);

  if (debug&4) {
    cout << "\n\n@@ FilamMapping::regenBFM -- "
	 << "*AFTER* pHyper->generateNew()\n\n";
    printHyperbolicDimensions(); //**pf DEBUG
  }

  bodyFittedMappingNeedsUpdate = FALSE;
  Mapping::reinitialize();      //??

  if (debug&4) {
    cout << "\n\n@@ FilamMapping::regenBFM -- *AFTER* Mapping::reinint()\n\n";
    printHyperbolicDimensions(); //**pf DEBUG
  }
}

//
// First regenerate, then get a copy with 'copyBodyFittedMApping'
//  -->copy FilamentMapping.pHyper into copyMap & renames copy
void FilamentMapping::
copyBodyFittedMapping( HyperbolicMapping &copyMap, 
		       aString *pMappingRename /* = NULL */)
{
  HyperbolicMapping *pTemp = getHyperbolicMappingPointer();
  assert( pTemp != NULL );
  copyMap = *pTemp;
  if ( pMappingRename != NULL ) {
    if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::copyBodyFittedMapping "
		      << " old name=`"<< copyMap.getName(Mapping::mappingName)
		      << "', new name=`"<< *pMappingRename
		      << "' "<<endl;
    copyMap.setName( Mapping::mappingName, *pMappingRename );
  }

  releaseHyperbolicMappingPointer( pTemp );
}

void
FilamentMapping::setHyperbolicGridInfo()
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setHyperbolicGridInfo called.\n";
  assert(pHyper != NULL );
  int thisAxis1=getGridDimensions(axis1);
  int thisAxis2=getGridDimensions(axis2);
  pHyper->setGridDimensions(axis1, thisAxis1 );
  pHyper->setGridDimensions(axis2, thisAxis2 );

  //..This is a fix to a tricky 'feature' I had: **pf 10/12/00
  //.... If the user changed the dimensions of the grid,
  //.... from, e.g., "lines" in update(), then next time
  //.... we regenerated the HyperbGrid, axis2 to would 'magically'
  //.... change to be of dimension=linesInTheNormalDirection.
  //.... IF that happens to be DIFFERENT from thisAxis2,
  //.... then, e.g., my grid velocity code (in DeformingGrid)
  //.... would not work!! 
  //
  //.... It might be we need to change this behavior in HyperbMapping **pf
  //
  IntegerArray ipar(2); 
  ipar(0)=0; ipar(1)=thisAxis2;  //=linesInTheNormalDirection;
  pHyper->setParameters( HyperbolicMapping::linesInTheNormalDirection, ipar);
  // END fix to tricky bug 

  pHyper->setRangeDimension(  getRangeDimension() );  
  pHyper->setDomainDimension( getDomainDimension() ); // ..doesn't change

  int ax, side;
  for ( ax=0; ax<=1; ax++ ) 
  {
    pHyper->setIsPeriodic(ax,getIsPeriodic(ax));         // *wdh*
    for (side=0; side <=1 ; side++ )
    {
      pHyper->setBoundaryCondition( side, ax, getBoundaryCondition(side,ax));
    }
  }
}


void
FilamentMapping::getHyperbolicGridInfo() 
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::getHyperbolicGridInfo called.\n";
  setGridDimensions( axis1, pHyper->getGridDimensions(axis1));
  setGridDimensions( axis2, pHyper->getGridDimensions(axis2));

  int ax, side;
  for ( ax=0; ax<=1; ax++ ) 
  {
    setIsPeriodic(ax,pHyper->getIsPeriodic(ax));         // *wdh*
    for (side=0; side <=1 ; side++ )
    {
      setBoundaryCondition( side, ax, pHyper->getBoundaryCondition(side,ax));
      //..Should I set share flags etc here, too?  **pf
    }
  }
}

//
// getHyperbolicMappingPointer()
//   -- guarantees that the returned map is up to date
//   -- recall that pHyper is evaluated in a lazy way,
//      i.e., it is only generated once it's needed
//   -- caller can request a regeneration of the hyperb. map
//      in three ways:
//      1) call FilamentMapping::map()
//      2) call regenerateBodyfittedMapping()
//      3) call getHyperbolicMappingPointer()
//
//   -- after you are done with the mapPointer, call 'releaseHyperbolicMappingPointer()'
//
HyperbolicMapping *
FilamentMapping::getHyperbolicMappingPointer()
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::getHyperbolicMappingPointer called.\n";
  assert( pHyper != NULL );
  updateFilamentAndGrid();
  //if ( bodyFittedMappingNeedsUpdate ) 
  //  initializeBodyFittedMapping( );
  return pHyper;
}

void
FilamentMapping::releaseHyperbolicMappingPointer(HyperbolicMapping* &pHyp00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::releaseHyperbolicMappingPointer called.\n";
  // do nothing:  could be used to release unnecessary storage, since user promises not to use pHyper;
  // --> simply zero pHyp00 to make sure it's not used
  pHyp00 = NULL;
}

//
//  --Mapping member functions
//


// Copy constructor is deep by default
FilamentMapping::
FilamentMapping( const FilamentMapping & map, const CopyType copyType )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping -- copy constructor called.\n";
  FilamentMapping::className="FilamentMapping";
  if( copyType==DEEP )
  {
    int nFilamentPoints00 = map.nFilamentPoints;
    //int nEndPoints00      = map.nEndPoints;
    real thickness00      = map.thickness;
    real endRadius00      = map.endRadius;

    constructor( nFilamentPoints00, 
		 //nEndPoints00,   // nEndPoints is obsolete
		 thickness00,
		 endRadius00 );
    
    *this=map;
  }
  else
  {
    cout << "FilamentMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}


FilamentMapping & FilamentMapping::
operator =( const FilamentMapping & X0 )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::operator=() called.\n";
  assert( filamentData != NULL );

  if( FilamentMapping::className != X0.getClassName() )
  {
    cout << "FilamentMapping::operator= ERROR trying to set a FilamentMapping = to a" 
      << " mapping of type " << X0.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X0);            // call = for derivee class
  FilamentMapping & X = (FilamentMapping&) X0;  // cast to a Filament mapping

  nFilamentPoints           = X.nFilamentPoints;
  nSplinePoints             = X.nSplinePoints;
  
  setNumberOfFilamentPoints( nFilamentPoints, nSplinePoints );  
  //nThickSplinePoints        = X.nThickSplinePoints;
  //nEndPoints                = X.nEndPoints;
  nEndPoints                = -1;  // obsolete, will be removed in the near future: see the endpoints stuff below

  filamentLength     = X.filamentLength;
  xOffset            = X.xOffset;
  yOffset            = X.yOffset;
  //sXDirection        = X.sXDirection;
  //sYDirection        = X.sYDirection;
  
  thickness          = X.thickness;
  //endRadius          = X.endRadius;

  debug              = X.debug;
  numberOfDimensions = X.numberOfDimensions;
  
  motionType         = X.motionType;
  filamentType       = X.filamentType;
  filamentBoundaryType = X.filamentBoundaryType;

  if( pFilament==NULL )
  {
    pFilament= new SplineMapping( 2 ); pFilament->incrementReferenceCount();
    assert( pFilament!=NULL );
  }
  *pFilament         = *(X.pFilament);
  if( pThickFilament==NULL )
  {
    pThickFilament= new SplineMapping( 2 ); pThickFilament->incrementReferenceCount();
    assert( pThickFilament!=NULL );
  }
  *pThickFilament    = *(X.pThickFilament);

  aTravelingWaveAmplitude     = X.aTravelingWaveAmplitude;
  bTravelingWaveAmplitude     = X.bTravelingWaveAmplitude;
  spaceFrequencyTravelingWave = X.spaceFrequencyTravelingWave;
  timeFrequencyTravelingWave  = X.timeFrequencyTravelingWave;

  timeForFilament             = X.timeForFilament;
  timeOffset                  = X.timeOffset;

  xMotionDirection            = X.xMotionDirection;
  yMotionDirection            = X.yMotionDirection;
  motionVelocity              = X.motionVelocity;
  xOffsetCurrent              = X.xOffsetCurrent;
  yOffsetCurrent              = X.yOffsetCurrent;
  rotationAngleCurrent        = X.rotationAngleCurrent;
  rotationSpeed               = X.rotationAngleCurrent;

  //..not needed, defined in Mapping:: & HyperbolicMapping
  //distanceToMarch             = X.distanceToMarch;
  //dissipation                 = X.dissipation;
  //linesInTheNormalDirection   = X.linesInTheNormalDirection;
  //gridDimension1              = X.gridDimension1;
  //gridDimension2              = X.gridDimension2;
  //gridGeneratorDimension1     = X.gridGeneratorDimension1;
  //gridGeneratorDimension2     = X.gridGeneratorDimension2;

  if( pHyper==NULL )
  {
    pHyper = new HyperbolicMapping;  assert( pHyper != NULL ); pHyper->incrementReferenceCount();
    assert( pHyper!=NULL );
  }
  *pHyper = *(X.pHyper);

  if (X.pUserDefinedCoreCurve !=NULL ) 
  {
    aString centerClassName = (X.pUserDefinedCoreCurve) -> getClassName();
    if( pUserDefinedCoreCurve !=NULL && pUserDefinedCoreCurve->decrementReferenceCount()==0 )
      delete pUserDefinedCoreCurve;
    
    pUserDefinedCoreCurve = (X.pUserDefinedCoreCurve) -> make( centerClassName );
    pUserDefinedCoreCurve->incrementReferenceCount();
    
    *pUserDefinedCoreCurve = *(X.pUserDefinedCoreCurve);
  }

  radius             = X.radius;
  perturbation1      = X.perturbation1;
  perturbation2      = X.perturbation2;
  circleMode1        = X.circleMode1;
  circleMode2        = X.circleMode2;
  circlePhase1       = X.circlePhase1;
  circlePhase2       = X.circlePhase2;
  
  xMotionDirection   = X.xMotionDirection;
  yMotionDirection   = X.yMotionDirection;
  motionVelocity     = X.motionVelocity;
  xOffsetCurrent     = X.xOffsetCurrent;
  xOffsetCurrent     = X.yOffsetCurrent;

  rotationAngleCurrent = X.rotationAngleCurrent;
  rotationSpeed        = X.rotationSpeed;
  
  //if( false ) // *wdh* 001026   Hmm... do I want this? **pf
  //  initializeFilamentStorage();

  xFilament.redim(0);
  xFilament          = X.xFilament;
  //x0.redim(0);  ** removed Jan 2001, not needed -- **pf
  //x0                 = X.x0;  **pf
  xThickFilament.redim(0);
  xThickFilament     = X.xThickFilament;

  x.redim(0);
  x                  = X.x;
  r.redim(0);
  r                  = X.r;
  xr.redim(0);
  xr                 = X.xr;
  sr.redim(0);
  sr                 = X.sr;
  amplitude.redim(0);
  amplitude          = X.amplitude;
  dAmplitude.redim(0);
  dAmplitude         = X.dAmplitude;

  x_t.redim(0); x_tt.redim(0); xr_t.redim(0); xr_tt.redim(0);
  x_t                = X.x_t;
  x_tt               = X.x_tt;
  xr_t               = X.xr_t;
  xr_tt              = X.xr_tt;

  normalVector       = X.normalVector;
  tangentVector      = X.tangentVector;

  isFilamentInitialized = X.isFilamentInitialized;                  // *wdh*
  filamentData->isEndStorageInitialized = X.filamentData->isEndStorageInitialized;
  filamentData->nLeadingEndPoints       = X.filamentData->nLeadingEndPoints;
  filamentData->nTrailingEndPoints      = X.filamentData->nTrailingEndPoints;
  filamentData->leadingEndSpacing      = X.filamentData->leadingEndSpacing;
  filamentData->trailingEndSpacing     = X.filamentData->trailingEndSpacing;
  filamentData->leadingEndExtra        = X.filamentData->leadingEndExtra;
  filamentData->trailingEndExtra       = X.filamentData->trailingEndExtra;
  
  geometricDataNeedsUpdate = X.geometricDataNeedsUpdate;            // *wdh* 
  bodyFittedMappingNeedsUpdate = X.bodyFittedMappingNeedsUpdate;    // *wdh*
  centerLineNeedsUpdate    = X.centerLineNeedsUpdate;
  
  //if( false )  // *wdh*   ==> i.e. unnecessary **pf
  //  initializeBodyFittedMapping( pThickFilament );

  setGridIsValid();  // *wdh* 001026

  return *this;
}



void  FilamentMapping::
map(  const realArray & r_, realArray & x_,realArray & xr_,
	   MappingParameters & params_)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::map called.\n";
  HyperbolicMapping *pHyperLocal;
  pHyperLocal = getHyperbolicMappingPointer();
  assert(pHyperLocal !=NULL );

  pHyperLocal->map( r_,x_,xr_,params_ );

  releaseHyperbolicMappingPointer(pHyperLocal);
}

void FilamentMapping::
basicInverse( const realArray & x00, realArray & r00, realArray & rx00, MappingParameters & params )
// ============================================================================================
// /Description:
//    Define a fast inverse.
// ===========================================================================================
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::basicInverset called.\n";
  if( pHyper==NULL )
  {
    cout << "FilamentMapping::basicInverse: Error: The mapping pHyper has not been defined yet!\n";
    exit(1);    
  }
  
  pHyper->basicInverse(x00,r00,rx00,params );

}


//===========================================================================
// get a mapping from the database
//===========================================================================
int   FilamentMapping::
get( const GenericDataBase & dir, const aString & name)
{

  if ( debug & 2 ) cout << "FilamentMapping::get called.\n";
  assert( filamentData!= NULL);

  //..Create a derived data-base object & create sub-directory
  GenericDataBase & subDir = *dir.virtualConstructor();    
  dir.find(subDir,name,"Mapping"); 
  
  //..Get basic variables
  subDir.get( FilamentMapping::className,"className" );
  if (FilamentMapping::className != "FilamentMapping" )
  {
    cout << "FilamentMapping::get ERROR in className!"<<endl;
  }

  int nFilamentPoints00, nSplinePoints00;
  subDir.get(  nFilamentPoints00        , "nFilamentPoints");
  subDir.get(  nSplinePoints00          , "nSplinePoints");
  
  setNumberOfFilamentPoints( nFilamentPoints00, nSplinePoints00 );
  // ..these variables are functions of 'nFilamentPoints'
  //  subDir.get(  nThickSplinePoints        , "nThickSplinePoints");
  //  subDir.get(  nEndPoints                , "nEndPoints");

  subDir.get(  filamentLength     , "filamentLength");
  subDir.get(  xOffset            , "xOffset");
  subDir.get(  yOffset            , "yOffset");
  //subDir.get(  sXDirection        , "sXDirection");
  //subDir.get(  sYDirection        , "sYDirection");
  
  subDir.get(  thickness          , "thickness");
  //subDir.get(  endRadius          , "endRadius");

  // *wdh* subDir.get(  debug0             , "debug");    
  subDir.get(  numberOfDimensions , "numberOfDimensions");

  int temp;
  subDir.get(  temp, "motionType");
  motionType = (BoundaryMotionType) temp;

  subDir.get(  temp, "filamentType");
  filamentType = (FilamentType) temp;

  subDir.get(  temp, "filamentBoundaryType");
  filamentBoundaryType = (FilamentBoundaryType) temp;
 
  subDir.get(  aTravelingWaveAmplitude     , "aTravelingWaveAmplitude");
  subDir.get(  bTravelingWaveAmplitude     , "bTravelingWaveAmplitude");
  subDir.get(  spaceFrequencyTravelingWave , "spaceFrequencyTravelingWave");
  subDir.get(  timeFrequencyTravelingWave  , "timeFrequencyTravelingWave");

  subDir.get(  timeOffset                  , "timeOffset"); // tFilament=tcomp-timeOffset
  subDir.get(  timeForFilament             , "timeForFilament");

  subDir.get(  xMotionDirection            , "xMotionDirection");
  subDir.get(  yMotionDirection            , "yMotionDirection");
  subDir.get(  motionVelocity              , "motionVelocity");
  subDir.get(  xOffsetCurrent              , "xOffsetCurrent");
  subDir.get(  yOffsetCurrent              , "yOffsetCurrent");
  subDir.get(  rotationAngleCurrent        , "rotationAngleCurrent");
  subDir.get(  rotationSpeed               , "rotationSpeed");

  subDir.get(  radius             , "radius");
  subDir.get(  perturbation1      , "perturbation1");
  subDir.get(  perturbation2      , "perturbation2");
  subDir.get(  circleMode1        , "circleMode1");
  subDir.get(  circleMode2        , "circleMode2");
  subDir.get(  circlePhase1       , "circlePhase1");
  subDir.get(  circlePhase2       , "circlePhase2");
  
  subDir.getDistributed(  xFilament          , "xFilament");
  //subDir.get(  x0                 , "x0");  removed, not needed **pf
  subDir.getDistributed(  xThickFilament     , "xThickFilament");

  subDir.getDistributed(  x                  , "x");
  subDir.getDistributed(  r                  , "r");
  subDir.getDistributed(  xr                 , "xr");
  subDir.getDistributed(  sr                 , "sr");
  subDir.getDistributed(  amplitude          , "amplitude");
  subDir.getDistributed(  dAmplitude         , "dAmplitude");

  subDir.getDistributed(  x_t                , "x_t");
  subDir.getDistributed(  x_tt               , "x_tt");
  subDir.getDistributed(  xr_t               , "xr_t");
  subDir.getDistributed(  xr_tt              , "xr_tt");

  subDir.getDistributed(  normalVector       , "normalVector");
  subDir.getDistributed(  tangentVector      , "tangentVector");

  subDir.get(  isFilamentInitialized,  "isFilamentInitialized");

  subDir.get(  filamentData->isEndStorageInitialized, "isEndStorageInitialized");
  subDir.get(  filamentData->nLeadingEndPoints,   "nLeadingEndPoints");
  subDir.get(  filamentData->nTrailingEndPoints,  "nTrailingEndPoints");
  subDir.get(  filamentData->leadingEndSpacing,  "leadingEndSpacing");
  subDir.get(  filamentData->trailingEndSpacing, "trailingEndSpacing");
  subDir.get(  filamentData->leadingEndExtra,  "leadingEndExtra");
  subDir.get(  filamentData->trailingEndExtra, "trailingEndExtra");


  subDir.get(  geometricDataNeedsUpdate,     "geometricDataNeedsUpdate");
  subDir.get(  bodyFittedMappingNeedsUpdate, "bodyFittedMappingNeedsUpdate");
  subDir.get(  centerLineNeedsUpdate,        "centerLineNeedsUpdate"); 

  int exists= 0;
  subDir.get(exists, "hyperbolicExists");
  if ( exists )
  {
    aString mappingClassName; 
    subDir.get( mappingClassName, "pHyper->className");
    if ( mappingClassName != "HyperbolicMapping" )
    {
      cout << "FilamentMapping::get ERROR looking for a HyperbolicMapping, "
	   << "can't load a className=" 
	   << (const char *) mappingClassName << endl;
    }
    pHyper->get( subDir, "HyperbolicMapping");
  }

  exists=0;
  subDir.get( exists, "centerLineMappingExists");
  if ( exists )
  {
    aString mappingClassName; 
    subDir.get( mappingClassName, "pUserDefinedCoreCurve->className");
    pUserDefinedCoreCurve = Mapping::makeMapping( mappingClassName );
    pUserDefinedCoreCurve->incrementReferenceCount(); // *wdh*
    
    if( pUserDefinedCoreCurve == NULL )
    {
      cout << "FilamentMapping::get:ERROR unable to make the center line mapping with className="
	   << (const char *)mappingClassName << endl;
      {throw "error";}
    }
    pUserDefinedCoreCurve->get( subDir, "CenterLineMapping");
  }
  
  exists=0;
  subDir.get( exists, "filamentSplineExists");
  if (exists )
  {
    aString mappingClassName; 
    subDir.get( mappingClassName, "pFilament->className");
    // *wdh* pFilament = (SplineMapping*)Mapping::makeMapping( mappingClassName );

    if ( mappingClassName != "SplineMapping" )
    {
      cout << "FilamentMapping::get ERROR looking for a (Filament)SplineMapping, "
	   << "can't load a className=" 
	   << (const char *) mappingClassName << endl;
    }
    pFilament->get( subDir, "FilamentSplineMapping");
  }

  exists=0;
  subDir.get( exists, "thickFilamentSplineExists");
  if (exists )
  {
    aString mappingClassName; 
    subDir.get( mappingClassName, "pThickFilament->className");
    if ( mappingClassName != "SplineMapping" )
    {
      cout << "FilamentMapping::get ERROR looking for a (Thick)SplineMapping, "
	   << "can't load a className=" 
	   << (const char *) mappingClassName << endl;
    }
    pThickFilament->get( subDir, "ThickFilamentSplineMapping");
  }

  stretching.get( subDir, "stretching");

  Mapping::get( subDir, "Mapping" );
  delete &subDir;
  
  getHyperbolicGridInfo();
  setGridIsValid();

  return 0;
}

//===========================================================================
// put a mapping to the database
//===========================================================================
int  FilamentMapping::
put( GenericDataBase & dir, const aString & name) const
{
  if ( debug & 2 ) cout << "FilamentMapping::put called.\n";

  //..Create a derived data-base object & create sub-directory
  GenericDataBase & subDir = *dir.virtualConstructor();    
  dir.create(subDir,name,"Mapping"); 

  //..Save basic variables
  subDir.put( FilamentMapping::className,"className" );

  //   subDir.put(

  subDir.put( nFilamentPoints           , "nFilamentPoints");
  subDir.put(  nSplinePoints             , "nSplinePoints");

  //..these are functions of 'nFilamentPoints'
  //subDir.put(  nThickSplinePoints        , "nThickSplinePoints");
  //subDir.put(  nEndPoints                , "nEndPoints");

  subDir.put(  filamentLength     , "filamentLength");
  subDir.put(  xOffset            , "xOffset");
  subDir.put(  yOffset            , "yOffset");
  //
  //subDir.put(  sXDirection        , "sXDirection");
  //subDir.put(  sYDirection        , "sYDirection");
  
  subDir.put(  thickness          , "thickness");
  //subDir.put(  endRadius          , "endRadius");

  // *wdh* subDir.put(  debug              , "debug");
  subDir.put(  numberOfDimensions , "numberOfDimensions");
  
  subDir.put(  (int)motionType          , "motionType");
  subDir.put(  (int)filamentType        , "filamentType");
  subDir.put(  (int)filamentBoundaryType, "filamentBoundaryType");

  subDir.put(  aTravelingWaveAmplitude     , "aTravelingWaveAmplitude");
  subDir.put(  bTravelingWaveAmplitude     , "bTravelingWaveAmplitude");
  subDir.put(  spaceFrequencyTravelingWave , "spaceFrequencyTravelingWave");
  subDir.put(  timeFrequencyTravelingWave  , "timeFrequencyTravelingWave");

  subDir.put(  timeOffset                  , "timeOffset");
  subDir.put(  timeForFilament             , "timeForFilament");

  subDir.put(  xMotionDirection            , "xMotionDirection");
  subDir.put(  yMotionDirection            , "yMotionDirection");
  subDir.put(  motionVelocity              , "motionVelocity");
  subDir.put(  xOffsetCurrent              , "xOffsetCurrent");
  subDir.put(  yOffsetCurrent              , "yOffsetCurrent");
  subDir.put(  rotationAngleCurrent        , "rotationAngleCurrent");
  subDir.put(  rotationSpeed               , "rotationSpeed");

  subDir.put(  radius             , "radius");
  subDir.put(  perturbation1      , "perturbation1");
  subDir.put(  perturbation2      , "perturbation2");
  subDir.put(  circleMode1        , "circleMode1");
  subDir.put(  circleMode2        , "circleMode2");
  subDir.put(  circlePhase1       , "circlePhase1");
  subDir.put(  circlePhase2       , "circlePhase2");
  
  subDir.putDistributed(  xFilament          , "xFilament");
  //subDir.put(  x0                 , "x0");   not needed **pf
  subDir.putDistributed(  xThickFilament     , "xThickFilament");

  subDir.putDistributed(  x                  , "x");
  subDir.putDistributed(  r                  , "r");
  subDir.putDistributed(  xr                 , "xr");
  subDir.putDistributed(  sr                 , "sr");
  subDir.putDistributed(  amplitude          , "amplitude");
  subDir.putDistributed(  dAmplitude         , "dAmplitude");

  subDir.putDistributed(  x_t                , "x_t");
  subDir.putDistributed(  x_tt               , "x_tt");
  subDir.putDistributed(  xr_t               , "xr_t");
  subDir.putDistributed(  xr_tt              , "xr_tt");

  subDir.putDistributed(  normalVector       , "normalVector");
  subDir.putDistributed(  tangentVector      , "tangentVector");

  subDir.put(  isFilamentInitialized,         "isFilamentInitialized");

  subDir.put(  filamentData->isEndStorageInitialized, "isEndStorageInitialized");
  subDir.put(  filamentData->nLeadingEndPoints,   "nLeadingEndPoints");
  subDir.put(  filamentData->nTrailingEndPoints,  "nTrailingEndPoints");
  subDir.put(  filamentData->leadingEndSpacing,  "leadingEndSpacing");
  subDir.put(  filamentData->trailingEndSpacing, "trailingEndSpacing");
  subDir.put(  filamentData->leadingEndExtra,    "leadingEndExtra");
  subDir.put(  filamentData->trailingEndExtra,   "trailingEndExtra");

  subDir.put(  geometricDataNeedsUpdate,      "geometricDataNeedsUpdate");
  subDir.put(  bodyFittedMappingNeedsUpdate,  "bodyFittedMappingNeedsUpdate");
  subDir.put(  centerLineNeedsUpdate,         "centerLineNeedsUpdate");

  int exists= pHyper!=NULL;
  subDir.put(exists, "hyperbolicExists");
  if (exists )
  {
    subDir.put( pHyper->getClassName(), "pHyper->className");
    pHyper->put( subDir, "HyperbolicMapping");
  }
  
  exists= pUserDefinedCoreCurve != NULL;
  subDir.put( exists, "centerLineMappingExists");
  if ( exists )
  {
    subDir.put( pUserDefinedCoreCurve->getClassName(), "pUserDefinedCoreCurve->className");
    pUserDefinedCoreCurve->put( subDir, "CenterLineMapping");
  }
  
  exists= pFilament!=NULL;
  subDir.put(exists, "filamentSplineExists");
  if (exists )
  {
    subDir.put( pFilament->getClassName(), "pFilament->className");
    pFilament->put( subDir, "FilamentSplineMapping");
  }

  exists= pThickFilament!=NULL;
  subDir.put(exists, "thickFilamentSplineExists");
  if (exists )
  {
    subDir.put( pThickFilament->getClassName(), "pThickFilament->className");
    pThickFilament->put( subDir, "ThickFilamentSplineMapping");
  }

  stretching.put( subDir, "stretching");

  Mapping::put( subDir, "Mapping" );
  delete &subDir;
  return 0;
}


Mapping* FilamentMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the className is the name of this Class
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::make called.\n";

  Mapping *retval=0;
  if( FilamentMapping::className==mappingClassName )
    retval = new FilamentMapping;
  return retval;
}


aString FilamentMapping::
getClassName() const
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::getClassName called.\n";
   return className;
}

void FilamentMapping::
displayParameters()
{
  cout << "Filament::displayParameters\n";

  cout << "    nFilamentPoints         = "<< nFilamentPoints << endl;
  cout << "  nSplinePoints             = "<<  nSplinePoints << endl;
  cout << "  nThickSplinePoints        = "<< nThickSplinePoints << endl;
  cout << "  nTotalThickFilamentPoints = "<< getNumberOfThickFilamentPoints() << endl; 

  cout << "  filamentLength            = "<< filamentLength<<endl;
  cout << "  xOffset                   = "<< xOffset <<endl;
  cout << "  yOffset                   = "<< yOffset <<endl;
  cout << "  thickness                 = "<< thickness <<endl;;
  cout << "  endRadius                 = "<< endRadius <<endl;

  cout << "  debug                     = "<< debug <<endl;
  cout << "  numberOfDimensions        = "<< numberOfDimensions <<endl;
  
  cout << "  motionType                = "<< motionType<<endl;

  cout << "--TRAVELING WAVE:"<<endl;

  cout << "  aTravelingWaveAmplitude     = "<< aTravelingWaveAmplitude<<endl;
  cout << "  bTravelingWaveAmplitude     = "<< bTravelingWaveAmplitude <<endl;
  cout << "  spaceFrequencyTravelingWave = "<< spaceFrequencyTravelingWave<<endl;
  cout << "  timeFrequencyTravelingWave  = "<< timeFrequencyTravelingWave<<endl;

  cout << "  timeForFilament             = "<< timeForFilament <<endl;
  cout << "  timeOffset                  = "<< timeOffset <<endl;

  cout << "--TRANSLATING MOTION:"<<endl;

  cout << "  xOffset                     = "<< xOffset << endl;
  cout << "  yOffset                     = "<< yOffset << endl;
  cout << "  xOffsetCurrent              = "<< xOffsetCurrent << endl;
  cout << "  yOffsetCurrent              = "<< yOffsetCurrent << endl;

  cout << "  xMotionDirection            = "<< xMotionDirection << endl;
  cout << "  yMotionDirection            = "<< yMotionDirection << endl;
  cout << "  motionVelocity              = "<< motionVelocity<< endl;

  cout << "--CIRCULAR INTERFACE:"<<endl;

  cout << "   radius                     = "<<   radius<<endl;
  cout << "   perturbation1              = "<<   perturbation1<<endl;
  cout << "   perturbation2              = "<<   perturbation2<<endl;
  cout << "   circleMode1                = "<<   circleMode1<<endl;
  cout << "   circleMode2                = "<<   circleMode2<<endl;
  cout << "   circlePhase1               = "<<   circlePhase1<<endl;
  cout << "   circlePhase2               = "<<   circlePhase2<<endl;

  //  cout << "--HYPERBOLIC GRID:"<<endl;
  //cout << "  distanceToMarch             = "<<distanceToMarch<<endl;
  //cout << "  dissipation                 = "<<dissipation<<endl;
  //cout << "  linesInTheNormalDirection   = "<<linesInTheNormalDirection<<endl;
  //cout << "  gridDimension1              = "<<gridDimension1<<endl;
  //cout << "  gridDimension2              = "<<gridDimension2<<endl;
  //cout << "  gridGeneratorDimension1     = "<<gridGeneratorDimension1<<endl;
  //cout << "  gridGeneratorDimension2     = "<<gridGeneratorDimension2<<endl;

  cout << endl;

  Mapping::display();
}

//
// ...STRETCHING INTERFACE
//
void  FilamentMapping::
useStretching ( StretchMapping::StretchingType st /*=inverseHyperbolicTangent*/,
		int numberOfLayers /*=2*/, bool stretchFlag /* = true*/ )
{
  stretching.useStretching( stretchFlag );
  stretching.setNumberOfLayers( numberOfLayers);
  stretching.setStretchingType( st);
  setFilamentStorageNeedsUpdate(); // need to reinit. the array sizes **pf
}

void  FilamentMapping::
getStretchingParameters( int i,  real &a, real &b, real &c )
{
  stretching.getLayerParameters( i,a,b,c);
}

void FilamentMapping::
setStretchingParameters( int i,  real a, real b, real c )
{
  stretching.setLayerParameters( i,a,b,c);
  //updateNumberOfEndPoints();
  centerLineNeedsUpdate=TRUE; mappingHasChanged();
  setFilamentStorageNeedsUpdate(); // need to reinit. the array sizes **pf
}

//
// -- END POINT COMPUTER
//


int   FilamentMapping::
getLeadingEndNumberOfPoints()
{
  printf("FilamentMapping::   GETLeadingEndNumberOfPoints()  = [ %i ]\n",
	 filamentData->nLeadingEndPoints);

  return( filamentData->nLeadingEndPoints );
};

void   FilamentMapping::
setLeadingEndNumberOfPoints( int jj )
{
  filamentData->nLeadingEndPoints = jj;
  printf( "FilamentMapping::  SETLeadingEndNumberOfPoints()  = [ %i ]\n",
	 filamentData->nLeadingEndPoints);
}

int  FilamentMapping::
getTrailingEndNumberOfPoints()
{
  printf("FilamentMapping::   GETTrailingEndNumberOfPoints()  = [ %i ]\n",
	 filamentData->nTrailingEndPoints );
  return(  filamentData->nTrailingEndPoints );
};

void   FilamentMapping::
setTrailingEndNumberOfPoints( int jj )
{
  filamentData->nTrailingEndPoints = jj;
  printf("FilamentMapping::  SETTrailingEndNumberOfPoints()= [ %i ]\n",
	 filamentData->nTrailingEndPoints);
}

// void computeFlatSmoothSpacing -- for a flat filament
//   IN:
//      sr0         = spacing at the edge = sqrt( (x(1)-x(0))^2 + (y(1)-y(0))^2))
//      thickness   = thickness of the thick filament (= .5*b )
//   OUT:
//      numberOfEndPoints  = n of points for theta in [0,pi]
// 
void   FilamentMapping::
computeFlatSmoothSpacing( real endSpacing, real thickness00, real scaling, int & numberOfEndPoints  )
{
  const real b        =  thickness00/2.;
  const real dtheta   =  endSpacing/b; // spacing of the rounded edge for a flat filament
  const real pi       =  4. *atan(1.);
  const int  minPts   = 4; // under 4 points at the tip/tail does not make sense

  numberOfEndPoints = int( scaling*( 1+ floor( pi/dtheta ) ) );    
  if (numberOfEndPoints<minPts) numberOfEndPoints=minPts;
  printf("FilamentMapping::compFlatSmoothSpacing: space=%16.8f, thick =%16.9f, nPts=%i\n",
	 endSpacing, thickness00, numberOfEndPoints);
  printf("                             ...      : b    =%16.8f, dtheta=%16.9f, pi=%16.8f\n",
	 b, dtheta, pi);
}

// void computeDrFlatSmoothSpacing -- for a flat filament
//   IN:
//      dr          = spacing in r along the centerline
//      sr0         = sqrt(xr^2 + yr^2) at the edge
//      thickness   = thickness of the thick filament (= .5*b )
//   OUT:
//      numberOfEndPoints  = n of points for theta in [0,pi]
// 
void   FilamentMapping::
computeDrFlatSmoothSpacing( real dr00, real sr00, real thickness00, real scaling, int & numberOfEndPoints  )
{
  const real spacing00 =  dr00*sr00;   // spacing of the core, at the tip = dx0
  computeFlatSmoothSpacing( spacing00, thickness00, scaling, numberOfEndPoints );
}

void  FilamentMapping::
updateNumberOfEndPoints()
{
  computeGeometricData();         //get the centerline end spacings
  // computeFlatSmoothSpacing( pass in dr0, sr0, thickness,   get numberOfEndPoints) // LEADING
  // computeFlatSmoothSpacing( pass in dr0, sr0, thickness,   get numberOfEndPoints) // TRAILING
  int ntemp;
  computeFlatSmoothSpacing( filamentData-> leadingEndSpacing, thickness, 
			    filamentData-> leadingEndExtra,   ntemp);
  filamentData-> nLeadingEndPoints = ntemp;
  computeFlatSmoothSpacing( filamentData-> trailingEndSpacing, thickness, 
			    filamentData-> trailingEndExtra,   ntemp);
  filamentData-> nTrailingEndPoints = ntemp;
  filamentData->isEndStorageInitialized = true;

  printf("FilamentMapping::updateNumberOfEndPoints  nLeading= %i,  nTrailing= %i\n",
	 filamentData->nLeadingEndPoints, filamentData->nTrailingEndPoints );

  // set 'mapping needs update' ---> end is rebuilt in 'computeThickFilament'
  //if (isFilamentInitialized) setNumberOfThickSplinePoints();
  //centerLineNeedsUpdate=TRUE;  // core doesn't change
  mappingHasChanged();
}


//
//  ---SET PARAMETERS
//

void FilamentMapping::setFilamentStorageNeedsUpdate()
{
  assert( filamentData != NULL );
  isFilamentInitialized = false;
  filamentData->isEndStorageInitialized=false;
}

void  FilamentMapping::
setOffset( real xOffset00,  /* =0.*/
		real yOffset00   /* =0.*/ )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setOffset called.\n";
  
  xOffset=xOffset00;  yOffset=yOffset00;

  centerLineNeedsUpdate=TRUE; mappingHasChanged();
}

void  FilamentMapping::
setLength(real length00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setLength called.\n";

   filamentLength = length00;
   
   setFilamentStorageNeedsUpdate();
   centerLineNeedsUpdate=TRUE; mappingHasChanged();
}

void FilamentMapping::
setTravelingWaveParameters( real length00,    /* = 2  length */
			    real aParam00,    /* = 0.03  amplitude param A */
			    real bParam00,    /* = 0.01      - "" -  B */
			    real omega00,     /* = 1.   time freq. */
			    real knum00       /* = 1./2.    space freq */
			    )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setTravelingWaveParameters called.\n";
  filamentLength=length00;
  aTravelingWaveAmplitude = aParam00;
  bTravelingWaveAmplitude = bParam00;
  timeFrequencyTravelingWave = omega00;
  spaceFrequencyTravelingWave = knum00;

  //computeNumberOfEndPoints( leadingEndSpacing,  trailingEndSpacing );
  
  centerLineNeedsUpdate=TRUE; mappingHasChanged();
}

void FilamentMapping::
setTranslatingMotionParameters( real tangX, real tangY, real velo)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setTranslatingMotionParameters called.\n";
  // set translating motion params --> normalize the tangent
  real norm=sqrt( tangX*tangX + tangY*tangY);
  if (norm < 1e-16 ) 
  {
    cout << "FilamentMapping::setTranslatingMotionParameters WARNING; "
	 << " Your tangent vector (" << tangX << ", " << tangY <<") "
	 << " seems to have NORM = 0?? (="<< norm <<")."<<endl;
    cout << "   SETTING THE TANGENT VECTOR TO (1,0) ";
    tangX = 1.; tangY = 0.;
  }
  else
  {
    tangX = tangX/norm;
    tangY = tangY/norm;
  }

  xMotionDirection = tangX;
  yMotionDirection = tangY;
  motionVelocity   = velo;
}


void FilamentMapping::
getTranslatingMotionParameters( real &tangX, real &tangY, real &velo)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::getTranslatingMotionParameters called.\n";
  tangX = xMotionDirection;
  tangY = yMotionDirection;
  velo  = motionVelocity;
}


void  FilamentMapping::
setFilamentTimeOffset( real newTime )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setFilamentTimeOffset called.\n";
  //timeForFilament     = newTime;
  timeOffset          = newTime;
  centerLineNeedsUpdate = TRUE;  mappingHasChanged(); //remakeGrid =TRUE; 
}

void  FilamentMapping::
setFilamentTime( real newTime )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setFilamentTime called.\n";
  //timeForFilament     = newTime;
  timeForFilament     = newTime;
  centerLineNeedsUpdate = TRUE;  mappingHasChanged(); 
}

void  FilamentMapping::
setThicknessAndEndRadius( real thick00, real endRadius00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setThicknessAndEndRadius called.\n";
   thickness = thick00;
   endRadius = endRadius00; //NB:  endRadius is OBSOLETE

   centerLineNeedsUpdate = TRUE; mappingHasChanged();
  setFilamentStorageNeedsUpdate(); // need to reinit. the array sizes **pf
}

int FilamentMapping::
getNumberOfThickFilamentPoints()
{ 
  int nFilam    = getNumberOfFilamentPoints();
  int nTrailing = getTrailingEndNumberOfPoints();
  int nLeading  = getLeadingEndNumberOfPoints();  // note: contains +1 for periodicity
  
  return(   2*nFilam + nTrailing + ( nLeading + 1 ) );
  //return( getDefaultNumberOfThickFilamentPoints( nFilamentPoints, nEndPoints ));
};

int FilamentMapping::
getNumberOfFilamentPoints( ) 
{ 
  //printf("  numberOfFilamentPoints = %i\n",nFilamentPoints);
  return nFilamentPoints;
};


void  FilamentMapping::
setNumberOfFilamentPoints( int nFilamentPoints00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setNumberOfFilamentPoints called.\n";

  int nSplinePoints00 = nFilamentPoints;  // the default case
  setNumberOfFilamentPoints( nFilamentPoints00, nSplinePoints00 );

}

void  FilamentMapping::
setNumberOfFilamentPoints( int nFilamentPoints00, int nSplinePoints00)
{
  //cout << "Warning FILAMENTMAPPING -- setNumFilamPts( nFilam, nSpline ) not supported yet\n";
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setNumberOfFilamentPoints called.\n";

  //NOTE: * need to allow different resolution/stretching of the filam  vs. the spline
  //      * key to implementing the 'userdefinedcore'  TODO XXFIX PF
  nFilamentPoints=nFilamentPoints00;
  nSplinePoints=   nSplinePoints00;
  centerLineNeedsUpdate=TRUE; mappingHasChanged();
  setFilamentStorageNeedsUpdate(); // need to reinit. the array sizes **pf
}

void FilamentMapping::
setEndPointScaling( real leadingXtra, real trailingXtra )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setEndPointScaling called.\n";

  assert( filamentData != NULL );
  filamentData-> leadingEndExtra  = leadingXtra;
  filamentData-> trailingEndExtra = trailingXtra;
  setFilamentStorageNeedsUpdate();
}

void  FilamentMapping::
setNumberOfEndPoints( int nEndPoints00 )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setNumberOfEndPoints called.\n";
  cout <<"FilamentMapping::setNumberOfEndPoints -- WARNING -- not to be used any more.\n"
       << "   ---> call 'setNumberOfFilamentPoints' instead, nEndPoints is set automatically.\n";
  
  //nEndPoints = nEndPoints00;
  //setNumberOfThickSplinePoints();
  
  //centerLineNeedsUpdate=TRUE; mappingHasChanged();
  //isFilamentInitialized = FALSE; // need to reinit. the array sizes **pf, Jan 26
}

void  FilamentMapping::
setNumberOfThickSplinePoints( int nThickSplinePoints00 )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setNumberOfThickSplinePoints called.\n";
  nThickSplinePoints= nThickSplinePoints00;
  
  setGridDimensions(axis1, nThickSplinePoints);
  pHyper->setGridDimensions(axis1, getGridDimensions(axis1));

  mappingHasChanged();
  //centerLineNeedsUpdate=TRUE; 
  //isFilamentInitialized = FALSE; // no need to reinit the storage for the core
}
 
void  FilamentMapping::
setNumberOfThickSplinePoints()
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setNumberOfThickSplinePoints called.\n";
  //..Default
  int nThick 
    = getNumberOfThickFilamentPoints(); //thickFILAMENTpoints
  setNumberOfThickSplinePoints( nThick );//thickSPLINEpoints
}

void  FilamentMapping::
setMotionType(  BoundaryMotionType motionType00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setMotionType called.\n";
  motionType = motionType00;
  
  centerLineNeedsUpdate=TRUE; mappingHasChanged();
}


void  FilamentMapping::
setFilamentType(  FilamentType filamentType00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setFilamentType called.\n";
  filamentType = filamentType00;
  centerLineNeedsUpdate=TRUE; mappingHasChanged();
}


void  FilamentMapping::
setFilamentBoundaryType(  FilamentBoundaryType filamentBoundaryType00)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setFilamentType called.\n";
  assert( pHyper != NULL );
  filamentBoundaryType = filamentBoundaryType00;

  Mapping *pTempSurface = getSurfaceForHyperbolicMap(); //either pThickFilament or pFilament
  assert( pTempSurface != NULL );
  pHyper->setSurface( * pTempSurface );

  centerLineNeedsUpdate=TRUE; mappingHasChanged();
}
 
// ..old and obsolete
int FilamentMapping::
getDefaultNumberOfThickFilamentPoints( int nFilam, int nEnd )
{
  cout << "ERROR -- getDefaultNumberOfThickFilamentPoints is NOT USED, exiting\n";
  exit(0);
  return( 2*nFilam  +  2*nEnd + 1 );
}

//...OBSOLETE!!
//void FilamentMapping::
//setDefaultNumberOfThickFilamentPoints( )
//{
//  nTotalThickFilamentPoints=getDefaultNumberOfThickFilamentPoints(nFilamentPoints,nEndPoints);
//}
  
void  FilamentMapping::
setHyperbolicMappingParameters( real distanceToMarch00,           /* = 0.3,*/
				real dissipation00                /* = 0.2,*/ )
{
  cout << endl
       << "*******WARNING FilamentMapping::setHyperbMappingParams -- obsolete!"
       << endl;
  //distanceToMarch              = distanceToMarch00;    //dissipation                  = dissipation00;
  //linesInTheNormalDirection    = linesInTheNormalDirection00;
  bodyFittedMappingNeedsUpdate = TRUE; mappingHasChanged();
}

void  FilamentMapping::
setHyperbolicGridDimensions( int gridDimension1_,    int gridDimension2_,
			     int gridGenDimension1_, int gridGenDimension2_)
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::setHyperbolicGridDimensions called.\n";
  cout << "WARNING -- $$ FilamentMapping::setHyperbolicGridDimensions called, not used anymore...\n";
  //..copy gridDims from Mapping:: to pHyper
  //gridDimension1 = gridDimension1_;               //gridDimension1 = gridDimension2_;
  //gridGeneratorDimension1 = gridGenDimension1_;   //gridGeneratorDimension2 = gridGenDimension2_;
  
  bodyFittedMappingNeedsUpdate = TRUE; mappingHasChanged();
}


//
// computeUserDefinedCenterLineFilament:
// ..Takes user defined centerline *pUserDefinedCoreCurve
//   - evaluates x,xr at the points in r
void FilamentMapping::
computeUserDefinedCenterLineFilament( real t, realArray & r00, 
				      realArray & x00, realArray & xr00 )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::computeUserDefinedCenterLineFilament called.\n";
  assert (pUserDefinedCoreCurve != NULL );
  pUserDefinedCoreCurve->map(r00,x00,xr00);

  centerLineNeedsUpdate=TRUE;  // should update the splines & geom.data
}

void FilamentMapping::
formNormalizedParametrization( realArray & r00 )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::formNormalizedParametrization called.\n";
  int nIncoming = r00.getLength(0);
  if(debug&2) printf("..FormNormParam: length of rIn(%i), rOut(%i)\n",
		     nIncoming, nFilamentPoints);
  Index I(0,nFilamentPoints);
  r00.redim(I);
  r00.seqAdd(0, 1);
  r00=r00/(nFilamentPoints-1);  // r=0, ..., 1.  =normalized parametrization
}

void FilamentMapping::
formChebyshevParametrization( realArray & r00 )
{
  formNormalizedParametrization( r00 );
  // ..then stretch
  const real pi = 4.*atan(1.);
  r00 = .5*( cos( pi*( r00-1.)) +1.);
}

void FilamentMapping::
getParametrization( realArray & r00 )
{
  formNormalizedParametrization( r00 );
  if( stretching.isUsed() ) {
    //cout << "STRETCHING IS USED -- in getParametrization()\n";
    realArray rStretch(r00);
    stretching.map(r00, rStretch);
    r00 = rStretch;
  }
}

void FilamentMapping::
computeGeometricData()
{
  if ( debug & 2 ) cout << "FilamentMapping :: computeGeometricData CALLED\n";
  // Given xFilament
  // --> form the SPLINE pFilament
  // --> evaluate derivatives & normal/tang vectors
  Range all;
  realArray xx=xFilament(all,0);
  realArray yy=xFilament(all,1);
  //  pFilament->setPoints( xFilament(all,0), xFilament(all,1) ); //works?not
  pFilament->setPoints( xx,yy ); 

  switch( filamentType )
    {
    case CLOSED_FILAMENT:
      pFilament->setIsPeriodic(axis1, Mapping::functionPeriodic);
      break;
    case OPEN_FILAMENT:
      pFilament->setIsPeriodic(axis1, Mapping::notPeriodic);
      break;
    case USER_DEFINED_CURVE_FILAMENT:
      assert( pUserDefinedCoreCurve != NULL);
      pFilament->setIsPeriodic( axis1, pUserDefinedCoreCurve->getIsPeriodic(axis1) );
      break;
    }

  //formNormalizedParametrization( r ); // r=0, ..., 1.  =normalized param.
  // Call pFilament.map( r,x,xr, __MAPPING PARAMS__ );  << XXFIX
  //computeTravelingWaveFilament( tcomp, r, x, xr);      //DEBUG-->gets xr

  //TODO -- this should map the spline & get the derivatives from there  TODO XXFIX pf
  //TODO2 -- should compute all necessary centerline derivatives here    TODO XXFIX pf

  sr= sqrt( pow(xr(all,0),2.) + pow(xr(all,1),2.) ); // arc-length deriv.
  tangentVector(all,0) = xr(all,0)/sr(all);
  tangentVector(all,1) = xr(all,1)/sr(all);
  normalVector(all,0) = -tangentVector(all,1);
  normalVector(all,1) = tangentVector(all,0);

  //..end spacings for smooth boundaries
  //SKETCH: evaluate  filament at end & end-1 for both ends
  //        evaluate  spacing of the filament from (x,y) data
  //        store in filamentData->leadingEndSpacing, trailingEndSpacing
  const int nLast=nFilamentPoints-1;

  //xx.display("THIS IS XX");
  //yy.display("THIS IS YY");

  filamentData->leadingEndSpacing= sqrt( pow(xx(1)-xx(0),2.) + pow(yy(1)-yy(0),2.));
  filamentData->trailingEndSpacing= sqrt( pow(xx(nLast)-xx(nLast-1),2.) + pow(yy(nLast)-yy(nLast-1),2.));
  printf( "FilamentMapping::computeGeomData -- leadspacing= %16.8f,  trailingSpacing= %16.8f, nFilamPts=%i, nLast=%i\n",
	  filamentData->leadingEndSpacing, filamentData->trailingEndSpacing, nFilamentPoints,  nLast);

  // Curvature?? >> Whatever is needed for elastic eqs. of mot.


  geometricDataNeedsUpdate     = FALSE ; // now updated, **pf
  bodyFittedMappingNeedsUpdate = TRUE;  // geometry changed, need to update surface **pf
}

void FilamentMapping::
computeThickFilament()
{
  if ( debug & 2 ) cout << "FilamentMapping::computeThickFilament called\n";
  assert( pThickFilament != NULL);
  if( geometricDataNeedsUpdate ) computeGeometricData();

  const real pi =4.*atan(1.);
  const int nLast=nFilamentPoints-1;

  xThickFilament.redim(getNumberOfThickFilamentPoints(),2);
  SplineMapping::ParameterizationType splineParamType;

  splineParamType = SplineMapping::index;
  pThickFilament->setShapePreserving(TRUE);
  pThickFilament->setParameterizationType( splineParamType );

  Range all;
  xTop.redim(nFilamentPoints,numberOfDimensions);
  xBottom.redim(nFilamentPoints,numberOfDimensions);

  ///..TOP & BOTTOM
  realArray & xc   = xFilament;      // shorthand
  realArray & norm = normalVector;
  real        d    = thickness/2.; 

  xTop =     xc - d*norm;
  xBottom =  xc + d*norm;


  //
  // .......1) Leading edge: has extra point to get periodicity
  // 
  int nLeadingEnd = getLeadingEndNumberOfPoints(); 
  cout << "THICKFILAM - nLeading="<<nLeadingEnd<<endl;
  Index ILeadingEnd(0,nLeadingEnd+1);          

  xLeadingEnd.redim(ILeadingEnd,2);      // CONTAINS EXTRA POINT FOR PERIODICITY
  realArray tnCoords(ILeadingEnd,2);     // (tangent,normal) coords
  realArray sRadius(ILeadingEnd);        // smoothed radius function
  realArray theta(ILeadingEnd);          // variable & smoothly matched
  realArray rr(ILeadingEnd);             // uniform
  real      drr;
  realArray pn(ILeadingEnd),  ps(ILeadingEnd);

  real xbase=xFilament(0,0), ybase=xFilament(0,1);  // leading edge of filam-core
  real nx0=normalVector(0,0),   ny0=normalVector(0,1);  
  real tx0=tangentVector(0,0),  ty0=tangentVector(0,1);

  const real b              = thickness/2.;
  int iEdge=0, iOffset=1;
  real dxBase, dyBase, dx2Base, dy2Base, curvBase, curvBase1, curvBase2, sr0;
  getCurveEdgeData( xFilament, iEdge, iOffset, b, dxBase, dyBase, dx2Base, dy2Base, curvBase, sr0, curvBase1, curvBase2);
  cout << "--THICKFILAMENT, leading edge curvature = "<< curvBase << endl;

  real len0      = getEdgeSpacing( xFilament,iEdge, iOffset );
  real lenTop    = getEdgeSpacing( xTop,iEdge, iOffset );
  real lenBottom = getEdgeSpacing( xBottom,iEdge, iOffset );

  rr.seqAdd(1,1);  rr = rr/(nLeadingEnd+1.);  // rr -- runs 0, ..., 1!!
  drr = rr(2)-rr(1);  //int nTip = floor( b*pi/len0 ); //nope, this is computed elsewhere

  //..CREATE the cubic leading edge
  real fn0  = b,  fn1 = -b, dfn0 = 0., dfn1 = 0.; // no point at the tip -- FIX, add the leading point
  real fs0 =  0., fs1 = 0., dfs0 = lenBottom/drr, dfs1 = -lenTop/drr;
  cubic( 1., fn0, fn1, dfn0, dfn1, rr, pn); // normal part
  cubic( 1., fs0, fs1, dfs0, dfs1, rr, ps); // tang.  part
  {
    FILE* fp;    fp = fopen("nsLeading.dat", "w");
    for (int i=0; i< nLeadingEnd+1; i++) fprintf( fp," %24.16e  %24.16e \n",pn(i), ps(i));
    fclose(fp);    fflush(0);
  }

  int iTangent=1, iNormal=0;
  tnCoords(all,iNormal) =  pn;
  tnCoords(all,iTangent) = ps;

  xLeadingEnd(all,0) = xbase - tnCoords(all,iTangent)*tx0 + tnCoords(all,iNormal)*nx0;
  xLeadingEnd(all,1) = ybase - tnCoords(all,iTangent)*ty0 + tnCoords(all,iNormal)*ny0;
  
  //
  // .......2) Trailing edge
  //
  int nTrailingEnd = getTrailingEndNumberOfPoints();
  cout << "THICKFILAM - nTrailing="<<nTrailingEnd<<endl;
  Index Itrailing(0,nTrailingEnd);
  xTrailingEnd.redim(Itrailing,2);
  tnCoords.redim(Itrailing,2);
  rr.redim(Itrailing);   pn.redim(Itrailing);   ps.redim(Itrailing);
  
  //
  //--BEGING trailing
  xbase=xFilament(nLast,0),    ybase =xFilament(nLast,1);  // trailing edge of filam-core
  nx0=normalVector(nLast,0),   ny0   =normalVector(nLast,1);  
  tx0=tangentVector(nLast,0),  ty0   =tangentVector(nLast,1);

  iEdge=nLast, iOffset=-1;
  getCurveEdgeData( xFilament, iEdge, iOffset, b, dxBase, dyBase, dx2Base, dy2Base, curvBase, sr0, curvBase1, curvBase2);
  cout << "--THICKFILAMENT, trailing edge curvature = "<< curvBase << endl;

  len0      = getEdgeSpacing( xFilament,iEdge, iOffset );
  lenTop    = getEdgeSpacing( xTop,iEdge, iOffset );
  lenBottom = getEdgeSpacing( xBottom,iEdge, iOffset );

  rr.seqAdd(1,1);  rr = rr/(nTrailingEnd+1);  // rr -- runs 0, ..., 1!!
  drr = rr(1)-rr(0);  //int nTip = floor( b*pi/len0 ); //nope, this is computed elsewhere

  //..CREATE the cubic edge
  fn0  = -b, fn1 = b, dfn0 = 0., dfn1 = 0.; // no point at the tip -- FIX, add the TRAILING point
  fs0 =  0., fs1 = 0., dfs0 = lenTop/drr, dfs1 = -lenBottom/drr;
  cubic( 1., fn0, fn1, dfn0, dfn1, rr, pn); // normal part
  cubic( 1., fs0, fs1, dfs0, dfs1, rr, ps); // tang.  part
  {
    FILE* fp;    fp = fopen("nsTrailing.dat", "w");
    for (int i=0; i< nTrailingEnd; i++) fprintf( fp," %24.16e  %24.16e \n",pn(i), ps(i));
    fclose(fp);    fflush(0);
  }

  iTangent=1, iNormal=0;
  tnCoords(all,iNormal) =  pn;
  tnCoords(all,iTangent) = ps;

  //--->transform (t,n)-->(x,y)
  xTrailingEnd(all,0) = xbase + tnCoords(all,iTangent)*tx0 + tnCoords(all,iNormal)*nx0;
  xTrailingEnd(all,1) = ybase + tnCoords(all,iTangent)*ty0 + tnCoords(all,iNormal)*ny0;
  
  //--END trailing
  //

  // ....collect to xThickFilament
  xThickFilament=-1.234;
  Index I(0,nFilamentPoints);
  Index Ireverse(0,nFilamentPoints);

  xThickFilament(I,0) = xTop(I,0); // Top part
  xThickFilament(I,1) = xTop(I,1);  

  xThickFilament(nFilamentPoints+Itrailing,0) = xTrailingEnd(Itrailing,0); // Trailing (rounded) end
  xThickFilament(nFilamentPoints+Itrailing,1) = xTrailingEnd(Itrailing,1);

  for (int i=0; i<nFilamentPoints; i++) {
    xThickFilament(nFilamentPoints+nTrailingEnd+i,0) = xBottom(nFilamentPoints-i-1,0);  // Bottom part
    xThickFilament(nFilamentPoints+nTrailingEnd+i,1) = xBottom(nFilamentPoints-i-1,1);  
  }

  xThickFilament(2*nFilamentPoints+nTrailingEnd+ILeadingEnd,0) = xLeadingEnd(ILeadingEnd,0); // Leading edge
  xThickFilament(2*nFilamentPoints+nTrailingEnd+ILeadingEnd,1) = xLeadingEnd(ILeadingEnd,1); 

  ////DEBUG
  //int n=nFilamentPoints;
  ////xDebug.redim(3*n + 2*nEndPoints + 1,2);
  //xDebug.redim(3*n,2);
  //xDebug(I,0) = xTop(I,0); xDebug(I+n,0) = xFilament(I,0); xDebug(I+2*n,0) = xBottom(I,0);
  //Debug(I,1) = xTop(I,1); xDebug(I+n,1) = xFilament(I,1); xDebug(I+2*n,1) = xBottom(I,1);
  {
    FILE* fp;    fp = fopen("xyTopBottom.dat", "w");
    for (int i=0; i<=nLast; i++) fprintf( fp," %24.16e  %24.16e    %24.16e %24.16e   \n",xTop(i,0),xTop(i,1),xBottom(i,0),xBottom(i,1));
    fclose(fp);    fflush(0);
  }
  {
    FILE* fp;    fp = fopen("xyLeading.dat", "w");
    for (int i=0; i< nLeadingEnd+1; i++) fprintf( fp," %24.16e  %24.16e \n",xLeadingEnd(i,0), xLeadingEnd(i,1));
    fclose(fp);    fflush(0);
  }
  {
    FILE* fp;    fp = fopen("xyTrailing.dat", "w");
    for (int i=0; i< nTrailingEnd; i++) fprintf( fp," %24.16e  %24.16e \n",xTrailingEnd(i,0), xTrailingEnd(i,1));
    fclose(fp);  fflush(0);
  }
  {
    int nTotal = getNumberOfThickFilamentPoints();
    FILE* fp;
    fp = fopen("xyThick.dat", "w");
    for (int i=0; i<nTotal; i++) {
      fprintf( fp," %24.16e  %24.16e \n",xThickFilament(i,0), xThickFilament(i,1));
    }
    fclose(fp);
    fflush(0);
  }

  // ....Form the thick spline
  realArray xx=xThickFilament(all,0);
  realArray yy=xThickFilament(all,1);
  //xx.display("XX  for thickfilament");
  //yy.display("YY  for thickfilament");
  pThickFilament->setPoints( xx,yy ); 
  pThickFilament->setIsPeriodic(axis1, Mapping::functionPeriodic);
  pThickFilament->setGridDimensions(axis1, nThickSplinePoints);

  //if(debug&4)

}

//void FilamentMapping::
//computeOneSidedFilament()
//{/
//
//}

void FilamentMapping::
computeTranslatingMotionOffset( real time00,
				real xOffset00, real yOffset00,
				real &xNew,     real &yNew,
				real &xNew_t,   real &yNew_t,
				real &xNew_tt,  real &yNew_tt)
{
  real t = time00 -timeOffset;
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::computeTranslatingMotionOffset called.\n";
  if (debug&2)   { 
    cout<<"FilamentMapping::compTranslMotionOffset:"
	<< " time="<<time00<<", time-offset="<< t
	<<", tang=("<< xMotionDirection<<", "<<yMotionDirection<<"),"
	<<" vel="<< motionVelocity <<endl;
  }

  xNew_t  =  motionVelocity * xMotionDirection;      yNew_t  =  motionVelocity * yMotionDirection;
  xNew_tt = 0.;                                      yNew_tt = 0.;
  xNew    = xOffset + t*xNew_t;                      yNew    = yOffset + t*yNew_t;
  //xNew = xOffset + t*motionVelocity*xMotionDirection;  //replaced 4/8/01 by xNew_t etc
  //yNew = yOffset + t*motionVelocity*yMotionDirection;
  if(debug&2)  {
    cout<<"      old offset = ("<< xOffsetCurrent <<", "<<yOffsetCurrent<<")."
	<<" NEW OFFSET = ("<< xNew << ", "<< yNew <<").\n";
  }
}


// for traveling wave, no translation
void FilamentMapping::
computeTravelingWaveFilament( real tcomp, realArray & r00, 
			      realArray & x00, realArray &xr00)
{
  computeTravelingWaveFilament( tcomp, r00, x00, xr00,
				xOffset, yOffset );    // offset= the initial offset
}

//
// Traveling wave filament with general position for leading edge = offset
//
void FilamentMapping::
computeTravelingWaveFilament( real tcomp, realArray & r00, 
			      realArray & x00, realArray &xr00,
			      real xOffset00, real yOffset00)
  //  Returns the filament position at time t
  //  for GIVEN dynamics (Ampl. modul. traveling wave)
  //  assume  0<= r <= 1
{

  if ( debug & 2 ) cout << "FilamentMapping::ComputeTravelingWaveFilament called\n";
  if ( !isFilamentInitialized )   initializeFilamentStorage();

  //..local shorthand: 
  real a=       aTravelingWaveAmplitude;
  real b=       bTravelingWaveAmplitude;
  real om=      timeFrequencyTravelingWave;
  real k=       spaceFrequencyTravelingWave;
  real xbase=   xOffset00;  // ALLOWS the offset to be changed dynamically!!
  real ybase=   yOffset00;

  real el=      filamentLength;

  real t=       tcomp - timeOffset;

  if ( debug & 2 ) cout << "--ComputeTravelingWaveFilament  at TIME=" << t<<endl;

  Range all;

  //..Position of the filament
  realArray rL = el*r00;      //  rL=normalized; rr in (0,L)
  const real pi = 4.*atan(1.0);

  amplitude =  a*rL + b*rL*rL;
  //amplitude  = a*rL*rL + b*rL*rL*rL;  // new amplitude, leading edge x_s = 0
  x00(all,0) = xbase - rL;
  x00(all,1) = ybase + amplitude * cos( 2*pi*(om*t - k*rL) );   // Y-coordinate

  //..Derivatives of the pos xr=dx/dr etc
  dAmplitude  = a +  2. * b*rL;
  //dAmplitude  = 2*a*rL + 3*b*rL*rL;
  xr00(all,0) = -1.;
  xr00(all,1) =   dAmplitude* cos( 2*pi*(om*t-k*rL) )
                + 2*pi*k*amplitude* sin( 2*pi*(om*t-k*rL) );

  geometricDataNeedsUpdate=TRUE;  // should update the splines & geom.data
  mappingHasChanged();

  //DEBUG
  //saveCurve( x00, "CURVES/x_check.dat");
}


//
// Traveling wave filament with general position for leading edge = offset
//
void FilamentMapping::
computeTravelingWaveFilamentTimeDerivatives
(  real tcomp, realArray & r00, 
   realArray & x00, realArray &xr00,
   realArray & x00_t,  realArray &x00_tt,
   realArray & xr00_t, realArray &xr00_tt,
   const real xOffset00,  const real yOffset00,
   const real xOffset_t,  const real yOffset_t,
   const real xOffset_tt, const real yOffset_tt)
  //  Returns the filament position at time t
  //  for GIVEN dynamics (Ampl. modul. traveling wave)
  //  assume  0<= r <= 1
{

  if ( debug & 2 ) cout << "FilamentMapping::ComputeTravelingWaveFilament called\n";
  if ( !isFilamentInitialized )   initializeFilamentStorage();

  //..local shorthand: 
  real a=       aTravelingWaveAmplitude;
  real b=       bTravelingWaveAmplitude;
  real om=      timeFrequencyTravelingWave;
  real k=       spaceFrequencyTravelingWave;
  real xbase=   xOffset00;  // ALLOWS the offset to be changed dynamically!!
  real ybase=   yOffset00;

  real el=      filamentLength;

  real t=       tcomp - timeOffset;

  if ( debug & 2 ) cout << "--ComputeTravelingWaveFilament  at TIME=" << t<<endl;

  Range all;

  //..Position of the filament
  const int ix=0, iy=1;
  realArray rL = el*r00;      //  rL=normalized; rr in (0,L)
  const real pi = 4.*atan(1.0);
  realArray  phi =   cos( 2*pi*( om*t - k*rL )); 
  realArray dPhi =  -sin( 2*pi*( om*t - k*rL )); 

  amplitude =  a*rL + b*rL*rL;
  //amplitude  = a*rL*rL + b*rL*rL*rL;  // new amplitude, leading edge x_s = 0
  x00(all,ix) = xbase - rL;
  x00(all,iy) = ybase + amplitude * phi;   // Y-coordinate
  //x00(all,iy) = ybase + amplitude * cos( 2*pi*(om*t - k*rL) );   // Y-coordinate
  //xr00(all,iy) =   dAmplitude* cos( 2*pi*(om*t-k*rL) )
  //              + 2*pi*k*amplitude* sin( 2*pi*(om*t-k*rL) );

  //..Derivatives of the pos xr=dx/dr etc
  dAmplitude  = a +  2. * b*rL;
  //dAmplitude  = 2*a*rL + 3*b*rL*rL;
  xr00(all,ix) = -1.;
  xr00(all,iy) =   dAmplitude* phi- 2*pi*k*amplitude* dPhi;

  //DEBUG CODE
  int lenxt0 = x00_t.getLength(0);     int lenxt1  = x00_t.getLength(1);
  int lenAmp = amplitude.getLength(0); int lenDPhi = dPhi.getLength(0);
  printf( "\n..Filament, travelingwave dt's: x00(%i, %i), amp(%i), dPhi(%i)\n",
	  lenxt0, lenxt1, lenAmp, lenDPhi);
 
  //..Time derivatives
  x00_t(all,ix)    =  xOffset_t; x00_tt(all,ix)   = xOffset_tt;
  xr00_t(all,ix)   =  0.;        xr00_tt(all,ix)  = 0.;
  //  xrr_t(all,ix)  = 0.;         xrr_tt(all,ix) = 0.;

  x00_t(all,iy)    =   yOffset_t;  x00_tt(all,iy) = yOffset_tt;
  x00_t(all,iy)   +=   2.*pi*om*amplitude*dPhi;
  x00_tt(all,iy)  +=  -4. * pow(pi*om,2.) * amplitude * phi;

  xr00_t(all,iy)   =     2.*pi*om    * ( dAmplitude * dPhi + 2.*pi*k*amplitude*phi);
  xr00_tt(all,iy)  = pow(2.*pi*om,2.) * ( -dAmplitude* phi  + 2.*pi*k*amplitude*dPhi);

  geometricDataNeedsUpdate=TRUE;  // should update the splines & geom.data
  mappingHasChanged();
}

void FilamentMapping::
computeCurveFrameTimeDerivatives ( realArray &xr00, realArray &xr00_t, realArray &xr00_tt,
				   realArray &n_t,  realArray &tang_t,
				   realArray &n_tt, realArray &tang_tt)
{
  const int ix=0, iy=1, n= xr00.getLength(ix);
  //n00.redim(n,2);       tang00.redim(n,2);
  n_t.redim( n, 2);     n_tt.redim( n,2 );
  tang_t.redim( n,2 );  tang_tt.redim( n,2 );
  realArray s( n ),     rt_norm2( n ),  temp( n,2 ),  xr_xrt, xr_xrtt;
  Range     all;

  dotProduct( xr00, xr00_t,  xr_xrt );
  dotProduct( xr00, xr00_tt, xr_xrtt );
  dotProduct( xr00, xr00, s); s= sqrt(s);
  dotProduct( xr00_t, xr00_t, rt_norm2);

  for (int ic=0;  ic<=1;  ic++ ) {
    //printf("at i=1: x=%f, xr=%f, xr00_t= %f, s=%f, xr_xrt=%f\n", 
    //	   x(1,ic), xr00(1,ic),xr00_t(1,ic),s(1),xr_xrt(1));
    //printf("at i=2: x=%f, xr=%f, xr00_t= %f, s=%f, xr_xrt=%f\n\n", 
    //	   x(2,ic), xr00(2,ic),xr00_t(2,ic),s(1),xr_xrt(2));
    //tang00(all,ic)     = xr00(all,ic)/s;

    tang_t(all,ic)  = xr00_t(all,ic)*pow(s,-1.)  - xr_xrt*xr00(all,ic)*pow(s,-3.);
    tang_tt(all,ic) = xr00_tt(all,ic)*pow(s,-1.) - 2.*xr_xrt*pow(s,-3.)*xr00_t(all,ic)
                     -pow(s,-3.)*( rt_norm2 + xr_xrtt - 3.*pow(xr_xrt/s,2.))*xr00(all,ic);
    
  }
  //WRONG for DEBUGGING   fix **pf
  //tang_t(all,ix)  = xr_t(all,iy)*pow(s,-1.);
  //tang_t(all,iy)  =  xr_xrt*xr00(all,iy)*pow(s,-3.);
  // xr_t.display("xr_t");

  //  getPerpendicular( tang00, n00);
  getPerpendicular( tang_t, n_t );
  getPerpendicular( tang_tt, n_tt );
}

void FilamentMapping::
saveCurve( const realArray &curve, const aString &filename )
{
  ofstream gridOut;
  gridOut.open(filename, ios::out);
  const int ix=0, iy=1, n=curve.getLength(ix);

  for(int i=0; i<n; ++i) {
    gridOut.width(9); 
    gridOut.setf(ios::left,       ios::adjustfield);
    gridOut.setf(ios::scientific, ios::floatfield);
    gridOut.precision(4);
    gridOut << curve(i, ix) << "   " << curve(i,iy) <<endl;
  }
  gridOut.close();
}

void FilamentMapping::
getPerpendicular( const realArray &xin, realArray &out )
  // 
  // ..compute perpendicular vector in 2D: out1 = -x2, out2 = x1
  //
{
  Range all;
  const int ix=0, iy=1, n= xin.getLength(ix);
  out.redim(n,2); 

  out(all, ix) = -xin(all,iy);
  out(all, iy) = xin(all,ix);
}

void FilamentMapping::
dotProduct( const realArray &xin, const realArray &yin, realArray &out)
  //  out = x.y
{
  Range all;
  const int ix=0, iy=1, n= xin.getLength(ix);
  out.redim( n );
  out(all) =  xin(all,ix)*yin(all,ix) + xin(all,iy)*yin(all,iy);
}

//
// .. boundary for Closed Filament curve
//
void FilamentMapping::
computeCircularFilament( real tcomp, realArray & r00, 
			 realArray & x00, realArray &xr00,
			  real xOffset00 /*=0.*/, real yOffset00 /*=0.*/ )
{
  if ( debug & 2 ) cout << "FilamentMapping::ComputeCircularFilament called\n";
  if ( !isFilamentInitialized )   initializeFilamentStorage();

  Range all;
  real t=       tcomp - timeOffset;
  real xbase=   xOffset00;  // ALLOWS the offset to be changed dynamically!!
  real ybase=   yOffset00;

  cout << "+++ComputeCircularFilament t="<< t
       << " (tcomp = "<<tcomp<< ") "<< endl;

  //..Position of the filament
  const real pi = 4.*atan(1.0);
  //realArray  rL = pi*r00;      //  rL=normalized; rr in (0, 2pi)

  if(debug&4)
  {
    cout<<"DEBUG:  computeCircularFilament -- 2*pi*r"<<endl;
    // pDisplay->display( 2.*pi*r); // *wdh* removed this to allow separation of Mapping's into a separate library
    ::display( evaluate(2.*pi*r ));
  }

  //NOTE: make sure x00 & xr00 (the derivative) agree!!
  //new: make bumps rotate opposite directions
  if ( motionType == RIGID_BODY_MOTION ) { //solid body rotation
    amplitude =  radius*(1. +  perturbation1*cos( 2.*pi*(circleMode1*r - circlePhase1))
			 + perturbation2*cos( 2.*pi*(circleMode2*r - circlePhase2)));
    dAmplitude  = - radius*2.*pi*( perturbation1*circleMode1* sin(2.*pi*(circleMode1*r - circlePhase1) )
				   +perturbation2*circleMode2* sin(2.*pi*(circleMode2*r - circlePhase2)));
    
    //to get a rigid rotating filam-> change r->r+t
    x00(all,0) = xbase + amplitude*cos( 2.*pi*( r+t ) ); // X- coord
    x00(all,1) = xbase + amplitude*sin( 2.*pi*( r+t ) ); // Y- coord
    xr00(all,0) =dAmplitude*cos( 2.*pi*(r+t))- 2.*pi*amplitude*sin( 2.*pi*(r+t) );
    xr00(all,1) =dAmplitude*sin( 2.*pi*(r+t))+ 2.*pi*amplitude*cos( 2.*pi*(r+t) );
  }
  else  // deforming boundaries - not rotating
  {
    amplitude =  radius*(1. +  perturbation1*cos( 2.*pi*(circleMode1*r - circlePhase1 -t))
			 + perturbation2*cos( 2.*pi*(circleMode2*r - circlePhase2 +t)));
    dAmplitude  = - radius*2.*pi*( perturbation1*circleMode1* sin(2.*pi*(circleMode1*r - circlePhase1 -t) )
			  +perturbation2*circleMode2* sin(2.*pi*(circleMode2*r - circlePhase2 +t )));
    //to get a rigid rotating filam-> change r->r+t
    x00(all,0) = xbase + amplitude*cos( 2.*pi*( r ) ); // X- coord
    x00(all,1) = xbase + amplitude*sin( 2.*pi*( r ) ); // Y- coord
    xr00(all,0) =dAmplitude*cos( 2.*pi*r)- 2.*pi*amplitude*sin( 2.*pi*r );
    xr00(all,1) =dAmplitude*sin( 2.*pi*r)+ 2.*pi*amplitude*cos( 2.*pi*r );
  }

  if(debug&4) 
  {
    cout<<"DEBUG:  computeCircularFilament -- amplitude"<<endl;
    // pDisplay->display(amplitude); // *wdh* removed this to allow separation of Mapping's into a separate library
    ::display(amplitude);
    cout<<"DEBUG:  computeCircularFilament -- x00"<<endl;
    // pDisplay->display(x00);
    ::display(x00);
  }

  geometricDataNeedsUpdate=TRUE;  // should update the splines & geom.data
  mappingHasChanged();

}

//..ACCESS routines

void FilamentMapping::
replaceHyperbolicMapping( HyperbolicMapping *pNewHyper )
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "$$ FilamentMapping::replaceHyperbolicMapping called.\n";
  //..DON'T USE in DeformingGrids -- we'll copy the local
  //   hyperb. map instead

  //..Replace current HyperbolicMapping (*pHyper) with *pNewHyper
  //..Should 'delete' old, but that's not done yet 001003 **pf

  //..Deleting old --> omitted for now
  //   pHyper->decrementReferenceCount();
  //   if(pHyper->referenceCount() ==0) delete (pHyper);
  
  if( pNewHyper == NULL )
  {
    cout <<"FilamentMapping::replaceHyperbolicMapping WARNING: pNewHyper is NULL.\n";
  }
  pHyper = pNewHyper;
  getHyperbolicGridInfo(); // get grid dimensions & BCs

  bodyFittedMappingNeedsUpdate = FALSE;  // ..we assume pNewHyper is ok **pf
  //remakeGrid=TRUE;  
  mappingHasChanged();
}
//
// evaluateAtTime
//  -- the old version was split in two: 
//       1)  evaluateCenterLineAtTime
//       2)  updateThickFilamentBoundary
//
void FilamentMapping::
evaluateAtTime( real time00 )
{
  if ( debug & 2 ) 
    cout << "FilamentMapping::evaluateAtTime t="<<time00 <<"  called \n";
  evaluateCenterLineAtTime( time00 );
  if ( geometricDataNeedsUpdate)                computeGeometricData();  
  if ( !filamentData->isEndStorageInitialized ) updateNumberOfEndPoints();
  computeThickFilament();

  bodyFittedMappingNeedsUpdate=TRUE;
  mappingHasChanged();
};


// // only for calling by 'evaluateAtTime' 
// //   -- or in the initialization where that had to be split 
// void FilamentMapping::
// updateThickFilamentBoundary()
// {
//   // once filam. has moved, form comput. bdry
//   switch( filamentBoundaryType) 
//     {
//     case THICK_FILAMENT_BOUNDARY:
//       {
// 	computeThickFilament( );  
//       }
//       break;
//     case ONE_SIDED_FILAMENT_BOUNDARY:
//       {
// 	computeGeometricData();
//       }
//       break;
//     default:
//       cout << "WARNING -- evaluateSurfaceAtTime: unknown filamentBoundaryType="<< filamentBoundaryType<<endl; 
//     };
// }

// evaluateCenterLineAtTime -- 
// only for calling by 'evaluateAtTime' 
//   -- or in the initialization where that had to be split 
void FilamentMapping::
evaluateCenterLineAtTime( real time00 )
{
  assert( isFilamentInitialized );

  if ( debug & 2 ) 
    cout << "FilamentMapping::evaluateAtTime t="<<time00 <<"  called \n";

  switch (filamentType) {
    case OPEN_FILAMENT:
      {
	switch( motionType )
	  {
	  case NO_MOTION:
	    {
	      real t00 = 0.; // IS THIS GOOD??
	      if(debug&2)cout<< "--No Motion: Evaluate traveling wave motion t="<<t00<<endl;
	      //formNormalizedParametrization( r ); // r=0, ..., 1.  =normalized param
	      getParametrization( r ); //get r axis [0,1], could be stretched
	      computeTravelingWaveFilament( time00, r, xFilament, xr, xOffset, yOffset );
	      x_t = 0.; x_tt = 0.; xr_t = 0.; xr_tt = 0.; //no motion?!
	    }
	    break;
	  case RIGID_BODY_MOTION:
	    {
	      cout << "FilamentMapping::evaluateSurfaceAtTime ERROR: "
		   << "OPEN_FILAMENT->RIGID_BODY_MOTION not defined\n";
	    }
	    break;
	  case TRANSLATING_MOTION:  // change current offset, set time00=0 & call TravelingWave
	    {
	      if (debug&2) cout << "---Evaluate TRANSLATING motion t="<< time00<<endl;
	      real xNewOffset, yNewOffset;
	      real xOffset_t,  yOffset_t, xOffset_tt, yOffset_tt;
	      computeTranslatingMotionOffset( time00, xOffset, yOffset, xNewOffset, yNewOffset, 
					      xOffset_t, yOffset_t, xOffset_tt, yOffset_tt);

	      xOffsetCurrent = xNewOffset;
	      yOffsetCurrent = yNewOffset;
	      time00 =0.;
	      //formNormalizedParametrization( r );
	      getParametrization( r ); //get r axis [0,1], could be stretched

	      if (debug&2) cout<< "--Translating: x0,y0="
			       << xOffsetCurrent<<", "<<yOffsetCurrent<<endl;
	      //NEW -- changed 4/8/01 **pf
	      //computeTravelingWaveFilament( time00, r, xFilament, xr, 
	      //				    xOffsetCurrent, yOffsetCurrent );
	      computeTravelingWaveFilamentTimeDerivatives( time00, r, xFilament, xr, 
							   x_t,  x_tt, xr_t, xr_tt,
							   xOffsetCurrent, yOffsetCurrent,
							   xOffset_t,      yOffset_t,
							   xOffset_tt,     yOffset_tt);	      
	    }
	    break;
	  case TRAVELING_WAVE_MOTION:
	    {
	      if (debug&2) cout << "--Evaluate traveling wave motion t="<< time00<<endl;
	      //formNormalizedParametrization( r );
	      getParametrization( r ); //get r axis [0,1], could be stretched
	      //computeTravelingWaveFilament( time00, r, xFilament, xr ); //changed 4/8/01 **pf
	      computeTravelingWaveFilamentTimeDerivatives( time00, r, xFilament, xr, 
							   x_t,  x_tt, xr_t, xr_tt,
							   xOffset, yOffset);
		
	    }
	    break;
	  default:
	    {
	      cout <<"ERROR: FilamentMapping::evaluateSurfaceAtTime " 
		   <<"called with unknown filamentType"
		   << "= "<< filamentType<<endl;	      
	    }
	  }; // end switch(motionType)
      };     // end case OPEN_FILAMENT
      break;
    case CLOSED_FILAMENT:
      {
	switch( motionType )
	  {
	  case NO_MOTION:
	    {
	      //real t00 = 0.; // IS THIS GOOD??
	      //if (debug&2) cout << "---Evaluate Closed filament at INITIAL TIME" << endl;
	      //formNormalizedParametrization( r ); 
	      getParametrization( r ); //get r axis [0,1], could be stretched
	      computeCircularFilament(time00, r, xFilament, xr);
	      x_t = 0.; x_tt = 0.; xr_t = 0.; xr_tt = 0.; //no motion?!
	    }
	    break;
	  case RIGID_BODY_MOTION:
	    {
	      if (debug&2) cout << "---Evaluate Closed filament at time=" << time00<< endl;
	      formNormalizedParametrization( r ); // r=0, ... ,1 =normalized parm of curve
	      computeCircularFilament(time00, r, xFilament, xr);
	      if (debug&2) cout << "WARNING -- Closed filament: no bdry acceleration\n";
	      x_t = 0.; x_tt = 0.; xr_t = 0.; xr_tt = 0.; //no motion?!
	    }
	    break;
	  case TRANSLATING_MOTION:  // change current offset, set time00=0 & call CircularFilament
	    {
	      if (debug&2) cout << "---Evaluate Closed filament/TRANSLATING motion t="<< time00<<endl;
	      real xNewOffset, yNewOffset;
	      real xOffset_t,  yOffset_t, xOffset_tt, yOffset_tt;
	      computeTranslatingMotionOffset( time00, xOffset, yOffset, xNewOffset, yNewOffset, 
					      xOffset_t, yOffset_t, xOffset_tt, yOffset_tt);
	      //real xNewOffset, yNewOffset;
	      //computeTranslatingMotionOffset( time00, xOffset, yOffset, xNewOffset, yNewOffset);

	      xOffsetCurrent = xNewOffset;
	      yOffsetCurrent = yNewOffset;
	      //formNormalizedParametrization( r );
	      getParametrization( r ); //get r axis [0,1], could be stretched
	      if (debug&2) cout<< "--Translating: x0,y0="
			       << xOffsetCurrent<<", "<<yOffsetCurrent<<endl;
	      time00 =0.;
	      computeCircularFilament(time00, r, xFilament, xr);
	      if (debug&2) cout << "WARNING -- Closed filament: no bdry acceleration\n";
	      x_t = 0.; x_tt = 0.; xr_t = 0.; xr_tt = 0.; //no motion?!
	    }
	    break;
	  default:
	    {
	      cout << "ERROR: FilamentMapping::evaluateSurfaceAtTime() -- unknown motionType ="
		   << motionType << endl;
	    }
	  }
      }
      break;
    case USER_DEFINED_CURVE_FILAMENT:
      {
	//..Compute r,xFilament, xr
	if(debug&2) cout << "FilamentMapping::evaluateSurface "
			 <<"called for USER_DEFINED_CURVE_FILAMENT\n";
	//formNormalizedParametrization( r );
	getParametrization( r ); //get r axis [0,1], could be stretched
	computeUserDefinedCenterLineFilament( time00, r, xFilament, xr); //??
      }
      break;
    default:
      {
	cout <<"ERROR: FilamentMapping::evaluateSurfaceAtTime called with unknown filamentType"
	     << "= "<< filamentType<<endl;
      }
    break;
  } //end switch(filamentType)

  centerLineNeedsUpdate=     FALSE;
  geometricDataNeedsUpdate=  TRUE;
}

void FilamentMapping::
getCorePoints( realArray &x00)
{
  x00.redim(nFilamentPoints,2);
  x00 = xFilament;
}

void FilamentMapping::
getCoreDerivative( realArray &xr00)
{
  xr00.redim(nFilamentPoints,2);
  xr00 = xr;
}

//
//  get surface velocity from core velocity
//
void FilamentMapping::
getVelocity(realArray & x00_t,     realArray &xr00_t)
{
  x00_t = x_t;  xr00_t = xr_t;
}

void FilamentMapping::
getAcceleration(realArray &x00_tt, realArray &xr00_tt)
{
  x00_tt = x_tt;  xr00_tt = xr_tt;
}

//
//..INITIALIZE normal & tangent, and their time derivatives
// --> allows mapping coreVelocity/Acceleration to the surface
//
void FilamentMapping::
setupTimeDerivatives(realArray & x00_t, realArray & x00_tt, 
		     realArray &xr00, realArray &xr00_t, realArray & xr00_tt)
{
  x_t = x00_t;  x_tt  = x00_tt;
  xr_t= xr00_t; xr_tt = xr00_tt;
  computeCurveFrameTimeDerivatives( xr,      xr_t, xr_tt, 
				    normal_t,  tangent_t,
				    normal_tt, tangent_tt);
}

//..Returns the current xThickFilament, redim's xSurface
void FilamentMapping::
getSurfacePoints( realArray &xSurface )
{
  Range all;
  xSurface.redim(getNumberOfThickFilamentPoints(),2);
  xSurface(all,all) = xThickFilament(all,all);
}

// DUPLICATES code from computeThickFilament --> isolate common piece **pf
void FilamentMapping::
getSurfaceVelocity(     realArray &coreVelocity,      realArray & surfaceVelocity )
{
  surfaceVelocity.redim(getNumberOfThickFilamentPoints(),numberOfDimensions);
  Range all;
  realArray vLeadingEnd, vTrailingEnd, vTop, vBottom;
  vLeadingEnd.redim(nEndPoints+1,2);  // CONTAINS EXTRA POINT FOR PERIODICITY
  vTrailingEnd.redim(nEndPoints,2);
  vTop.redim(nFilamentPoints, 2);   
  vBottom.redim(nFilamentPoints,2);
  vLeadingEnd=-9.;   vTrailingEnd=-8.;   // dummy values, shouldn't show up in the results

  realArray & vc   = coreVelocity;  //short-hand
  real        d    = thickness/2.; 

  vTop =     vc - d*normal_t;
  vBottom =  vc + d*normal_t;

  // ....Round the ends:
  //      As an ellipse with semi-radius along tang=endRadius
  //                         semi-radius along norm=thickness/2

  realArray tnCoords(nEndPoints+1,2);  // (tangent,normal) coords
  real pi=4.*atan(1.);

  // .......Leading edge: has extra point to get periodicity
  Index Iend(0,nEndPoints+1);
  realArray theta(Iend);
  
  real thetaOffset = pi/2.; 
  real nx0_t=normal_t(0,0),   ny0_t=normal_t(0,1);  
  real tx0_t=tangent_t(0,0),  ty0_t=tangent_t(0,1);
  theta.seqAdd(1, 1);
  theta=theta/(nEndPoints+1);
  theta= thetaOffset + pi*theta; // theta= t0 + dtheta:dtheta:pi;

  tnCoords(all,0) = endRadius      * cos(theta); 
  tnCoords(all,1) = (thickness/2.) * sin(theta);  

  //--->transform (t,n)-->(x,y)
  real xbase_t=coreVelocity(0,0), ybase_t=coreVelocity(0,1);  // leading edge of filam.
  const int iTangent=0, iNormal=1;

  vLeadingEnd(all,0) = xbase_t + tnCoords(all,iTangent)*tx0_t + tnCoords(all,iNormal)*nx0_t;
  vLeadingEnd(all,1) = ybase_t + tnCoords(all,iTangent)*ty0_t + tnCoords(all,iNormal)*ny0_t;
  
  // .......Trailing edge
  Index Itrailing(0,nEndPoints);
  theta.redim(Itrailing);
  tnCoords.redim(Itrailing,2);

  thetaOffset = -pi/2.;
  theta.seqAdd(1, 1);
  theta=theta/(nEndPoints+1.);
  theta= thetaOffset + pi*theta; // theta= t0 + dtheta:dtheta:(pi-dtheta);

  tnCoords(all,0) = endRadius      * cos(theta);
  tnCoords(all,1) = (thickness/2.) * sin(theta);  

  //--->transform (t,n)-->(x,y)
  const int nLast=nFilamentPoints-1;

  xbase_t=coreVelocity(nLast,0);  // Trailing edge of the filament
  ybase_t=coreVelocity(nLast,1);  
  real nx1_t=normal_t(nLast,0),   ny1_t=normal_t(nLast,1);  
  real tx1_t=tangent_t(nLast,0),  ty1_t=tangent_t(nLast,1);

  vTrailingEnd(all,0) = xbase_t + tnCoords(all,iTangent)*tx1_t + tnCoords(all,iNormal)*nx1_t;
  vTrailingEnd(all,1) = ybase_t + tnCoords(all,iTangent)*ty1_t + tnCoords(all,iNormal)*ny1_t;
  
  // ....collect to xThickFilament
  surfaceVelocity=-1.234;
  Index I(0,nFilamentPoints);
  Index Ireverse(0,nFilamentPoints);

  surfaceVelocity(I,0) = vTop(I,0); // Top part
  surfaceVelocity(I,1) = vTop(I,1);  

  surfaceVelocity(nFilamentPoints+Itrailing,0) = vTrailingEnd(Itrailing,0); // Trailing (rounded) end
  surfaceVelocity(nFilamentPoints+Itrailing,1) = vTrailingEnd(Itrailing,1);

  for (int i=0; i<nFilamentPoints; i++) {
    surfaceVelocity(nFilamentPoints+nEndPoints+i,0) = vBottom(nFilamentPoints-i-1,0);  // Bottom part
    surfaceVelocity(nFilamentPoints+nEndPoints+i,1) = vBottom(nFilamentPoints-i-1,1);  
  }

  surfaceVelocity(2*nFilamentPoints+nEndPoints+Iend,0) = vLeadingEnd(Iend,0); // Leading edge
  surfaceVelocity(2*nFilamentPoints+nEndPoints+Iend,1) = vLeadingEnd(Iend,1); 
  //ADD Debug code -- save surface velocity to disk!! ??

}


void FilamentMapping::
// DUPLICATES code from computeThickFilament --> isolate common piece **pf
getSurfaceAcceleration(  realArray &coreAcceleration, realArray & surfaceAcceleration )
{
  surfaceAcceleration.redim(getNumberOfThickFilamentPoints(),numberOfDimensions);
  Range all;
  realArray acLeadingEnd, acTrailingEnd, acTop, acBottom;
  acLeadingEnd.redim(nEndPoints+1,2);  // CONTAINS EXTRA POINT FOR PERIODICITY
  acTrailingEnd.redim(nEndPoints,2);
  acTop.redim(nFilamentPoints, 2);   
  acBottom.redim(nFilamentPoints,2);
  acLeadingEnd=-9.;   acTrailingEnd=-8.;   // dummy values, shouldn't show up in the results

  realArray & ac   = coreAcceleration;  //short-hand
  real        d    = thickness/2.; 

  acTop =     ac - d*normal_tt;
  acBottom =  ac + d*normal_tt;

  // ....Round the ends:
  //      As an ellipse with semi-radius along tang=endRadius
  //                         semi-radius along norm=thickness/2

  realArray tnCoords(nEndPoints+1,2);  // (tangent,normal) coords
  real pi=4.*atan(1.);

  // .......Leading edge: has extra point to get periodicity
  Index Iend(0,nEndPoints+1);
  realArray theta(Iend);
  
  real thetaOffset = pi/2.; 
  real nx0_tt=normal_tt(0,0),   ny0_tt=normal_tt(0,1);  
  real tx0_tt=tangent_tt(0,0),  ty0_tt=tangent_tt(0,1);
  theta.seqAdd(1, 1);
  theta=theta/(nEndPoints+1);
  theta= thetaOffset + pi*theta; // theta= t0 + dtheta:dtheta:pi;

  tnCoords(all,0) = endRadius      * cos(theta); 
  tnCoords(all,1) = (thickness/2.) * sin(theta);  

  //--->transform (t,n)-->(x,y)
  real xbase_tt=coreAcceleration(0,0), ybase_tt=coreAcceleration(0,1);  // leading edge of filam.
  const int iTangent=0, iNormal=1;

  acLeadingEnd(all,0) = xbase_tt + tnCoords(all,iTangent)*tx0_tt + tnCoords(all,iNormal)*nx0_tt;
  acLeadingEnd(all,1) = ybase_tt + tnCoords(all,iTangent)*ty0_tt + tnCoords(all,iNormal)*ny0_tt;
  
  // .......Trailing edge
  Index Itrailing(0,nEndPoints);
  theta.redim(Itrailing);
  tnCoords.redim(Itrailing,2);

  thetaOffset = -pi/2.;
  theta.seqAdd(1, 1);
  theta=theta/(nEndPoints+1.);
  theta= thetaOffset + pi*theta; // theta= t0 + dtheta:dtheta:(pi-dtheta);

  tnCoords(all,0) = endRadius      * cos(theta);
  tnCoords(all,1) = (thickness/2.) * sin(theta);  

  //--->transform (t,n)-->(x,y)
  const int nLast=nFilamentPoints-1;

  xbase_tt=coreAcceleration(nLast,0);  // Trailing edge of the filament
  ybase_tt=coreAcceleration(nLast,1);  
  real nx1_tt=normal_tt(nLast,0),   ny1_tt=normal_tt(nLast,1);  
  real tx1_tt=tangent_tt(nLast,0),  ty1_tt=tangent_tt(nLast,1);

  acTrailingEnd(all,0) = xbase_tt + tnCoords(all,iTangent)*tx1_tt + tnCoords(all,iNormal)*nx1_tt;
  acTrailingEnd(all,1) = ybase_tt + tnCoords(all,iTangent)*ty1_tt + tnCoords(all,iNormal)*ny1_tt;
  
  // ....collect to xThickFilament
  surfaceAcceleration=-1.234;
  Index I(0,nFilamentPoints);
  Index Ireverse(0,nFilamentPoints);

  surfaceAcceleration(I,0) = acTop(I,0); // Top part
  surfaceAcceleration(I,1) = acTop(I,1);  

  surfaceAcceleration(nFilamentPoints+Itrailing,0) = acTrailingEnd(Itrailing,0); // Trailing (rounded) end
  surfaceAcceleration(nFilamentPoints+Itrailing,1) = acTrailingEnd(Itrailing,1);

  for (int i=0; i<nFilamentPoints; i++) {
    surfaceAcceleration(nFilamentPoints+nEndPoints+i,0) = acBottom(nFilamentPoints-i-1,0);  // Bottom part
    surfaceAcceleration(nFilamentPoints+nEndPoints+i,1) = acBottom(nFilamentPoints-i-1,1);  
  }

  surfaceAcceleration(2*nFilamentPoints+nEndPoints+Iend,0) = acLeadingEnd(Iend,0); // Leading edge
  surfaceAcceleration(2*nFilamentPoints+nEndPoints+Iend,1) = acLeadingEnd(Iend,1); 

  //ADD debug code -- save surface acceleration to disk

}



// ---------------------------setGridDimensions == User interface
int FilamentMapping::
update( MappingInformation & mapInfo ) 
// ==========================================================================
/// \details 
///     Define moving grid properties interavtively.
// ==========================================================================
{
  if ((debug&2) && DEBUG_PRINT_SUB) cout << "FilamentMapping::update called\n";

  assert(mapInfo.graphXInterface!=NULL);
  GenericGraphicsInterface & gi = *mapInfo.graphXInterface;
  
  aString menu[] =
  {
    "!FilamentMapping",
    "Offset (x0, y0)",
    ">Filament parameters",
      "Length",
      "Thickness & end radius",
      "Number of filament points",
      "Scaling of the number of end points",
      "Use stretching",
      "Set stretching parameters",
      "Set default HyperbolicMapping params",
      "Set debug value",
      "Display parameters",
    "<>Filament type",
      "open",
       ">closed filament",
          "closed",
          "radius",
          "perturbation1",
          "circleMode1",
          "circlePhase1",
          "perturbation2",
          "circleMode2",
          "circlePhase2",
      "<user defined",
    "<>Motions",
      "traveling wave",
      "rigid body motion",
      "translating filament", 
      "no motion",
    "<>Set time for the filament",
      "Set time (not stored)",
      "Set timeOffset (stored)",
    "<Hyperbolic grid generator",
    "Regenerate bodyfitted grid",
    " ",
    //    "lines",
    "boundary conditions",
    "share",
    "mappingName",
    "check inverse",
    "show parameters",
    "plot",
    "help",
    "exit", 
    ""
  };

  aString help [] = { "-Help 1", "-Help 2", "-Help 3(last one)", "" };

  //aString hyperbolicMenu[] =
  //{
  //  "!BodyFittedMapping",
  //  "generate",
  //  "distance to march",
  //  "dissipation",
  //  "lines in the normal direction",
  //  "lines",
  //  "help",
  //  "exit",
  //  ""
  //};

  aString travelingWaveMenu[] =
  {
    "!TravelingWave Parameters",
    "generate",
    "amplitudes (a,b)",
    "time frequency (omega)",
    "space frequency (k)",
    "time (t)",
    "length (el)",
    "help",
    "exit",
    ""
  };

  //aString travelingWaveHelp [] = { "-Help 1", "-Help 2", "-Help 3(last one)", "" };

  
//   aString help[] = 
//     {
//       "centre for annulus : Specify (x0,y0) for the centre",
//       "inner radius       : Specify the inner radius",

  aString answer,line; 
  char buff[180];  // buffer for sprintf

  bool plotObject=TRUE;
  GraphicsParameters parameters;
  parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);

  gi.appendToTheDefaultPrompt("Filament>"); // set the default prompt

  for( int it=0;; it++ )
  {
     if( it==0 && plotObject )
	answer="plotObject";
     else
	gi.getMenuItem(menu,answer);
     
     if( answer=="Offset (x0, y0)" ) 
     {
	real xOffset00, yOffset00;
	gi.inputString(line,sPrintF(buff,
	   "Enter (x0,y0) for endpoint of the filament (default=(%e,%e)): ",
	    xOffset, yOffset));
	if( line!="" ) sScanF(line,"%e %e", &xOffset00, &yOffset00 );
	setOffset( xOffset00, yOffset00 );
     }
     else if( answer== "Length" )
     {
	real el00;
	gi.inputString(line,sPrintF(buff,
     	  "Enter length of the filament (default=%e):",filamentLength));
        if( line!="" ) sScanF(line,"%e", &el00 );
	setLength( el00 );
     }
     else if( answer== "Thickness & end radius")
     {
	real thick00, endRad00;
	gi.inputString(line,sPrintF(buff,
     	  "Enter Thickness & End Radius of the filament (default=%e, %e):",
	   thickness,endRadius));
		       
        if( line!="" ) sScanF(line,"%e %e", &thick00, &endRad00 );
	setThicknessAndEndRadius( thick00, endRad00);
     }
     else if( answer== "Number of filament points" )
     {
	int numPoints00;
	gi.inputString(line,sPrintF(buff,
     	  "Enter number of filament points (default=%i):",
           nFilamentPoints));
        if( line!="" ) sScanF(line,"%i", &numPoints00 );
	setNumberOfFilamentPoints( numPoints00 );
     }
     else if( answer== "Scaling of the number of end points")
     {
	real leadingX  = filamentData-> leadingEndExtra;
	real trailingX = filamentData-> trailingEndExtra;
	gi.inputString(line,sPrintF(buff,
     	  "Enter scaling # pts at the leading & trailing end (default=%e, %e):",
	   leadingX, trailingX ));
		       
        if( line!="" ) sScanF(line,"%e %e", &leadingX, &trailingX );
	setEndPointScaling( leadingX, trailingX );
     }
     else if( answer== "Number of thick spline points" )
     {
       int numPoints00 = getNumberOfThickFilamentPoints();
       gi.inputString(line,sPrintF(buff,
     	  "Enter number of THICK filament points (default=%i, current=%i):",
				   numPoints00, nThickSplinePoints));
       if( line!="" ) sScanF(line,"%i", &numPoints00 );
       setNumberOfThickSplinePoints( numPoints00 );	
     }
     else if( answer== "Number of end points" )
     {
	int numPoints00;
	gi.inputString(line,sPrintF(buff,
	  "Enter number of points in rounded end (default=%i):",
           nEndPoints));
        if( line!="" ) sScanF(line,"%i", &numPoints00 );
	setNumberOfEndPoints( numPoints00 );	
     }
     else if( answer== "Set default hyperbolic generator params")
     {
       gi.outputString("Setting default Hyperbolic grid generator parameters.");
       setDefaultHyperbolicParameters();
       updateFilamentAndGrid();
       plotObject =TRUE;
     }
     else if( answer== "Use stretching" )
     {
       gi.outputString("Toggling stretching");
       stretching.useStretching( !stretching.isUsed() );
       centerLineNeedsUpdate=TRUE; mappingHasChanged();
       updateFilamentAndGrid();
       plotObject = TRUE;
     }
     else if( answer== "Set stretching parameters" )
     {
	real a,b,c;
	//	stretching.getLayerParameters( 0, a,b,c);//now through access fncs
	getStretchingParameters(0,a,b,c); 
	gi.inputString(line,sPrintF(buff,
     	  "Enter Layer -0- parameters a,b,c  (default=%e, %e, %e):",
	   a,b,c));
		       
        if( line!="" ) sScanF(line,"%e %e %e", &a, &b, &c );
	//stretching.setLayerParameters( 0, a,b,c );
	setStretchingParameters(0,a,b,c);

	//stretching.getLayerParameters( 1, a,b,c); 
	getStretchingParameters(1,a,b,c);
	gi.inputString(line,sPrintF(buff,
     	  "Enter Layer -1- parameters a,b,c  (default=%e, %e, %e):",
	   a,b,c));
		       
        if( line!="" ) sScanF(line,"%e %e %e", &a, &b, &c );
	//stretching.setLayerParameters( 1, a,b,c );
	setStretchingParameters(1,a,b,c);
	//computeNumberOfEndPoints( leadingEndSpacing, trailingEndSpacing ); //TODO XXPF

	centerLineNeedsUpdate=TRUE; mappingHasChanged();
	updateFilamentAndGrid();
	plotObject = TRUE;
     }
     else if( answer== "Set debug value")
     {
       int iTemp=debug;
       gi.inputString(line,sPrintF(buff,
	  "Enter debug flag (currently=%i):",
	  iTemp));
       if (line!="" ) sScanF(line,"%i", &iTemp);
       debug = iTemp;
       gi.outputString(sPrintF(buff,"  changed to debug= %i",iTemp));       
     }
     else if( answer== "Display parameters" )
     {
       gi.outputString("Printing param. values to stdout.");
       displayParameters();
     }
     // ---- FILAMENT TYPE
     else if( answer==  "open" )
     {
       gi.outputString("Filament type = open.");
       gi.outputString("  OPEN Filament defaults FilamentBoundaryType=THICK_FILAMENT");

       setFilamentType( OPEN_FILAMENT);
       setFilamentBoundaryType( THICK_FILAMENT_BOUNDARY);
       setDefaultHyperbolicParameters();
       evaluateAtTime( timeForFilament );
     }
     ///-----FILAMENT TYPE: CLOSED FILAMENT
     else if( answer== "closed")
     {
       gi.outputString("Filament type = closed. EXPERIMENTAL version!");
       gi.outputString("  CLOSED Filament defaults FilamentBoundaryType=ONE_SIDED_FILAMENT");

       setFilamentType( CLOSED_FILAMENT );
       setMotionType( NO_MOTION );
       setFilamentBoundaryType( ONE_SIDED_FILAMENT_BOUNDARY);
       setDefaultHyperbolicParameters();
       evaluateAtTime( timeForFilament );
     }
     else if( answer== "radius" )
     {
	real xx = radius;
	gi.inputString(line,sPrintF(buff,
     	  "Enter radius (default=%e):",radius));
        if( line!="" ) sScanF(line,"%e", &xx );
	radius = xx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer==  "perturbation1" )
     {
	real xx = perturbation1;
	gi.inputString(line,sPrintF(buff,
     	  "Enter perturbation1 (default=%e):",perturbation1));
        if( line!="" ) sScanF(line,"%e", &xx );
	perturbation1 = xx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer==  "perturbation2" )
     {
	real xx = perturbation2;
	gi.inputString(line,sPrintF(buff,
     	  "Enter perturbation2 (default=%e):",perturbation2));
        if( line!="" ) sScanF(line,"%e", &xx );
	perturbation2 = xx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer==  "circlePhase1"  )
     {
	real xx = circlePhase1;
	gi.inputString(line,sPrintF(buff,
     	  "Enter circlePhase1 (1/4=sine) (default=%e):",circlePhase1));
        if( line!="" ) sScanF(line,"%e", &xx );
        circlePhase1 = xx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer==  "circlePhase2"  )
     {
	real xx = circlePhase2;
	gi.inputString(line,sPrintF(buff,
     	  "Enter circlePhase2 (1/4=sine) (default=%e):",circlePhase2));
        if( line!="" ) sScanF(line,"%e", &xx );
        circlePhase2 = xx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer== "circleMode1" )
     {
	int  ixx = circleMode1;
	gi.inputString(line,sPrintF(buff,
     	  "Enter circleMode1 (default=%i):",circleMode1));
        if( line!="" ) sScanF(line,"%i", &ixx );
	circleMode1 = ixx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }
     else if( answer== "circleMode2" )
     {
	int  ixx = circleMode2;
	gi.inputString(line,sPrintF(buff,
     	  "Enter circleMode2 (default=%i):",circleMode2));
        if( line!="" ) sScanF(line,"%i", &ixx );
	circleMode2 = ixx;
	centerLineNeedsUpdate=TRUE; mappingHasChanged();
     }

     //------FILAMENT TYPE: USER DEFINED
     else if( answer== "user defined")
     {
      gi.outputString("Choose curve(mapping) for centerline:");
      //~~~~~~~~~~ EDIT

      // Make a menu with the Mapping names 
      int num=mapInfo.mappingList.getLength();
      aString *menu2 = new aString[num+2];
      IntegerArray subListNumbering(num);
      int j=0;
      for( int i=0; i<num; i++ )
      {
	MappingRC & map = mapInfo.mappingList[i];
        if( (map.getDomainDimension()==1 && map.getRangeDimension()==2)
	    && map.mapPointer!=this )
	{
	  subListNumbering(j)=i;
          menu2[j++]=map.getName(mappingName);
	}
      }
      menu2[j++]="none"; 
      menu2[j]="";   // null string terminates the menu
      aString answer2;
      int mapNumber = gi.getMenuItem(menu2,answer2);
      delete [] menu2;
      if( answer2=="none" )
        continue;
      if( mapNumber<0 )
      {
        gi.outputString("Error: unknown mapping to start from!");
        gi.stopReadingCommandFile();
      }
      else
      {
        mapNumber=subListNumbering(mapNumber);  // map number in the original list
	setCenterLineMapping( mapInfo.mappingList[mapNumber].mapPointer );

	setFilamentType( USER_DEFINED_CURVE_FILAMENT );        
      }

       //~~~~~~~~~~ END EDIT
     }

     // ---- MOTION
     else if( answer==  "no motion" )
     {
	gi.outputString("Filament Motiontype = no motion");
	setMotionType( NO_MOTION );
     }
     else if( answer==  "translating filament" )
     {
       gi.outputString("Filament Motiontype = translating filament");
       setMotionType( TRANSLATING_MOTION);
       real tx0, ty0, vel0;
       getTranslatingMotionParameters( tx0, ty0, vel0);
       gi.inputString(line,sPrintF(buff,
				   "Enter tangent to translation (=< %e, %e >):",
				   tx0, ty0));
       if( line!="" ) sScanF(line,"%e %e", &tx0, &ty0 );
       gi.inputString(line,sPrintF(buff,
				   "Enter velocity (= %e ):",
				   vel0));
       if( line!="" ) sScanF(line,"%e", &vel0);
       
       setTranslatingMotionParameters( tx0, ty0, vel0 );
     }
     else if( answer==  "rigid body motion" )
     {
	gi.outputString("Filament Motiontype = rigid body motion || NOT AVAILABLE.");
	gi.outputString("** for closed filament, solid body rotation, counterclockwise");
	setMotionType( RIGID_BODY_MOTION );
	//ANOTHER MENU -> rigid body params
	gi.appendToTheDefaultPrompt("Rigid Body Motion");
	
	gi.unAppendTheDefaultPrompt();
     }
     //----------------------------------------------TRAVELING WAVE WAVE--------
     else if( answer==  "traveling wave" )
     {
	bool plotHyperbolic=FALSE;
	gi.outputString("Filament Motiontype = traveling wave");
	setMotionType( TRAVELING_WAVE_MOTION );

	//ANOTHER MENU --> traveling wave params
	gi.appendToTheDefaultPrompt("Traveling Wave>");

	for( int it_inner=0;; it_inner++ )
        {
	  bool exitThisMenu=FALSE;
	  gi.getMenuItem( travelingWaveMenu,answer );

	  if ( answer== "generate" )
	  {
	    //regenerateBodyFittedMapping( pHyper );
	    updateFilamentAndGrid();
	    plotHyperbolic=TRUE;
	  }
	  else if ( answer==  "amplitudes (a,b)" )
	  {
	    real aa = aTravelingWaveAmplitude;
	    real bb = bTravelingWaveAmplitude;
	    gi.inputString(line,sPrintF(buff,
	       "Enter wave amplitudes (a=%e, b=%e):",
	        aa,bb));
	    
	    if( line!="" ) sScanF(line,"%e %e", &aa,&bb );
	    setTravelingWaveParameters(  filamentLength,
					 aa, bb,  /* <--modified values */
					 timeFrequencyTravelingWave,
					 spaceFrequencyTravelingWave
					 );	    
	    plotObject= TRUE;
	  }
	  else if ( answer==  "time frequency (omega)" )
	  {
	    real om = timeFrequencyTravelingWave;
	    gi.inputString(line,sPrintF(buff,
	       "Enter time frequency for the traveling wave (currently omega=%e):",
	        om));
	    
	    if( line!="" ) sScanF(line,"%e", &om );
	    setTravelingWaveParameters(  filamentLength,
					 aTravelingWaveAmplitude,
					 bTravelingWaveAmplitude,
					 om,   /* <--modified values */
					 spaceFrequencyTravelingWave
					 );	    
	    plotObject= TRUE;
	  }
	  else if ( answer==  "space frequency (k)" )
	  {
	    real knum00=spaceFrequencyTravelingWave;
	    gi.inputString(line,sPrintF(buff,
	       "Enter space frequency for the traveling wave (currently k=%e):",
	        knum00));
	    
	    if( line!="" ) sScanF(line,"%e", &knum00 );
	    setTravelingWaveParameters(  filamentLength,
					 aTravelingWaveAmplitude,
					 bTravelingWaveAmplitude,
					 timeFrequencyTravelingWave,
					 knum00   /* <--modified values */
					 );	    
	    plotObject=TRUE;
	  }
	  else if ( answer== "time (t)" )
	  {
	    real t00=timeForFilament;
	    gi.inputString(line,sPrintF(buff,
	       "Enter time offset for traveling wave solution (currently t=%e):",
	        t00));
	    
	    if( line!="" ) sScanF(line,"%e", &t00 );
	    setFilamentTimeOffset( t00 );
	    plotObject=TRUE;
	  }
	  else if ( answer== "length (el)" )
	  {
	    real len00 = filamentLength;
	    gi.inputString(line,sPrintF(buff,
	       "Enter filament length (current len=%e):",
	        len00));
	    
	    if( line!="" ) sScanF(line,"%e", &len00 );
	    setTravelingWaveParameters(  len00,     /* <--modified value */
					 aTravelingWaveAmplitude,
					 bTravelingWaveAmplitude,
					 timeFrequencyTravelingWave,
					 spaceFrequencyTravelingWave
					 );	    
	    plotObject=TRUE;
	  }
	  else if ( answer == "exit" )
	  {
	    exitThisMenu = TRUE;
	  }
	  else 
	  {
	      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
	      gi.stopReadingCommandFile();
	  }

	  if ( exitThisMenu && plotObject ) 
	  {
	    updateFilamentAndGrid();
	  }
 
	  if (plotHyperbolic  ||  ( exitThisMenu && plotObject) )
          {
	    gi.outputString("Updating the body fitted grid.");
	    if(debug&2) cout << "---PLOT Hyperbolic--\n";
	    parameters.set(GI_TOP_LABEL,getName(mappingName));
	    gi.erase();
	    PlotIt::plot(gi,*this,parameters);   
	    
	    plotObject=FALSE; plotHyperbolic= FALSE;
	  }	  
	  if (exitThisMenu) break;
	}
	gi.unAppendTheDefaultPrompt();
     }
     //  "<>Set time for the filament",
     //    "Set time (not stored)",
     //    "Set timeOffset (stored)",
     else if( answer== "Set time (not stored)" )
     {
       	real newTime;
	gi.inputString(line,sPrintF(buff,
     	  "Enter new time for evaluating filament at (current t=%e):",
	   timeForFilament));
		       
        if( line!="" ) sScanF(line,"%e", &newTime );
	setFilamentTime( newTime );
        evaluateAtTime( timeForFilament );
     }
     else if( answer== "Set timeOffset (stored)" )
     {
       	real newTime;
	gi.inputString(line,sPrintF(buff,
     	  "Enter new time for evaluating filament at (current t=%e):",
	   timeForFilament));
		       
        if( line!="" ) sScanF(line,"%e", &newTime );
	setFilamentTimeOffset( newTime );
        evaluateAtTime( timeForFilament );
     }
     //--HYPERBOLIC GRID GEN.
     else if( answer==  "Hyperbolic grid generator")
     {
       HyperbolicMapping*  pHyperMap;
       pHyperMap = getHyperbolicMappingPointer();
       assert( pHyperMap != NULL);

       setHyperbolicGridInfo();
       pHyperMap->update(mapInfo);
       getHyperbolicGridInfo();

       releaseHyperbolicMappingPointer( pHyperMap );
     }
     //--REGENERATE the bodyfitted grid: force an update
     else if( answer== "Regenerate bodyfitted grid")
     {
       evaluateAtTime( timeForFilament );
       //geometricDataNeedsUpdate =     TRUE;
       //bodyFittedMappingNeedsUpdate = TRUE;
       updateFilamentAndGrid();
       plotObject=true;
     }
     // else if( answer==  )
     else if( answer=="lines"  ||
	      answer=="boundary conditions"  ||
	      answer=="share"  ||
	      answer=="mappingName"  ||
	      answer=="check inverse" )
     { 
        // call the base class to change these parameters:
        // ..These need to go into HyperbolicMapping
	mapInfo.commandOption=MappingInformation::readOneCommand;
	mapInfo.command=&answer;
	Mapping::update(mapInfo); 
	mapInfo.commandOption=MappingInformation::interactive;
        //gridDimension1=getGridDimensions(0);
        //gridDimension2=getGridDimensions(1);
	setHyperbolicGridInfo();
	
	if(debug&4)printHyperbolicDimensions(); //**pf DEBUG

     }
     else if( answer=="show parameters" )
     {
       displayParameters();
     }
     else if( answer=="plot" )
     {
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,FALSE);
	parameters.set(GI_TOP_LABEL,getName(mappingName));
	gi.erase();
	
	//IF Hyperbolic is generated
	PlotIt::plot(gi,*this,parameters); 
	
	//ELSE plot just the filament, no body fitted grid
	// <<-- here
	
	parameters.set(GI_PLOT_THE_OBJECT_AND_EXIT,TRUE);
     }
     else if( answer=="help" )
     {
	for( int i=0; help[i]!=""; i++ )
	   gi.outputString(help[i]);
     }
     else if(answer=="addDebug") // for debugging: allows 'viewing' pFilam & pThickFilam **pf
     {
       if( pFilament!=NULL)      mapInfo.mappingList.addElement( *pFilament );
       if( pThickFilament!=NULL) mapInfo.mappingList.addElement( *pThickFilament );
       if( pHyper!=NULL)         mapInfo.mappingList.addElement( *pHyper );
     }
     else if( answer=="exit" )
	break;
    else if( answer=="plotObject" )
      plotObject=TRUE;
    else 
    {
      gi.outputString( sPrintF(buff,"Unknown response=%s",(const char*)answer) );
      gi.stopReadingCommandFile();
    }
     
    // plot if 1) asked for it earlier, or 2) updated the grid
    bool updateFlag = updateFilamentAndGrid();
    plotObject = plotObject || updateFlag;
    //plotObject = plotObject || updateFilamentAndGrid();  doesn't work, use stuff above

    if( plotObject )
    {
      if( debug&2 ) cout << "---PLOT OBJECT--\n";
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      gi.erase();
      PlotIt::plot(gi,*this,parameters);   // *** recompute every time ?? ***
    }
  }

  gi.erase();
  gi.unAppendTheDefaultPrompt();  // reset
  return 0;
}

void FilamentMapping::printHyperbolicDimensions()
{
  cout << "FILAMENT -- pHyper->gridDimension(axis1) = " // **pf DEBUG
       << pHyper->getGridDimensions(axis1) << endl;
  cout << "FILAMENT -- pHyper->gridDimension(axis2) = " // **pf DEBUG
       << pHyper->getGridDimensions(axis2) << endl;
}

//
// ..PRIVATE -- tools for building the tip/end in FilamentMapping
//

//quintic-Hermite interpolant
//see Dougherty, Edelman, Hyman; Math. Comp. v. 52(1989), p. 471
void FilamentMapping::
quintic( const real dtheta, const real f0, const real f1, 
	 const real df0, const real df1, 
	 const real d2f0, const real d2f1, 
	 const realArray & theta, realArray &p)
{
  real s=(f1- f0)/dtheta;
  real c0=f0,  c1=df0, c2= d2f0/2.;
  real c3= (d2f1-3*d2f0)/(2.*dtheta) + 2*(5*s-3*df0-2*df1)/(dtheta*dtheta);
  real c4= (3*d2f0-2*d2f1)/(2*dtheta*dtheta)+(8*df0+7*df1-15*s)/pow(dtheta,3.);
  real c5= (d2f1-d2f0)/(2.*pow(dtheta,3.))+3*(2*s-df1-df0)/pow(dtheta,4.);

  p  = c0 + c1*theta + c2*pow(theta,2.) + c3*pow(theta,3.) + c4*pow(theta,4.) + c5*pow(theta,5.);
}

void FilamentMapping::
cubic( const real dtheta, const real f0, const real f1, 
       const real df0, const real df1,
       const realArray & theta, realArray &p, realArray &dp )
{
  real s= (f1- f0)/dtheta;
  real c0= f0,  c1=df0;
  real c2= (3.*s -df1-2.*df0)/dtheta;
  real c3= -( 2.*s -df1 -df0)/pow(dtheta,2.);

  p = c0 + c1*theta + c2*pow(theta,2.) + c3*pow(theta,3.);
  if ( dp.getLength(0)>0 ) dp = c1 + 2.*c2*theta + 3*c3*pow(theta,2.);
}

//cubic-Hermite interpolant
//see Dougherty, Edelman, Hyman; Math. Comp. v. 52(1989), p. 471
void FilamentMapping::
cubic( const real dtheta, const real f0, const real f1, 
       const real df0, const real df1,
       realArray & theta, realArray &p)
{
  realArray dp;  dp.redim(0);
  cubic( dtheta, f0, f1, df0, df1, theta, p, dp );
}

// ..getCurveEdgeData
//  --> call with iEdge=0,      iOffset= 1 to get leading edge
//  --> call with iEdge=nLast,  iOffset=-1 to get trailing edge
// .... one sided first derivative:   .5*( -3 4 -1)
// .... one sided second derivative:  (2 -5 4 -1)/dx^2, from Fornberg, p. 16
//...... N.B.: these lack the correct denominators (with dx etc) because it's scaled out of the curvature
//
void FilamentMapping::
getCurveEdgeData( const realArray & xCurve, int iEdge, int iOffset, const real b,
		  real & dxBase, real & dyBase, real & dx2Base, real & dy2Base,
		  real & curv, real &sr0)
{
  dxBase    = (-1.5*xCurve(iEdge,0) + 2*xCurve(iEdge+iOffset,0) -.5*xCurve(iEdge+2*iOffset,0));
  dx2Base   = (2.*xCurve(iEdge,0) -5.*xCurve(iEdge+iOffset,0)+4.*xCurve(iEdge+2*iOffset,0)-xCurve(iEdge+3*iOffset,0));
  dyBase    = (-1.5*xCurve(iEdge,1) + 2*xCurve(iEdge+iOffset,1) -.5*xCurve(iEdge+2*iOffset,1));
  dy2Base   = (2.*xCurve(iEdge,1) -5.*xCurve(iEdge+iOffset,1)+4.*xCurve(iEdge+2*iOffset,1)-xCurve(iEdge+3*iOffset,1));
  curv      = (-dyBase*dx2Base + dxBase*dy2Base)/pow( dxBase*dxBase + dyBase*dyBase, 3./2. );
  sr0       = sqrt( dxBase*dxBase + dyBase*dyBase ); //WRONG, lacks the proper denom!! --- FIX
}

void FilamentMapping::
getCurveEdgeData( const realArray & xCurve, int iEdge, int iOffset, const real b,
		  real & dxBase, real & dyBase, real & dx2Base, real & dy2Base,
		  real & curv, real &sr0, real & curvBase1, real &curvBase2)
{
  getCurveEdgeData( xCurve, iEdge,iOffset, b,
		    dxBase,dyBase,dx2Base,dy2Base, curv,sr0);
  curvBase1 = curv/(  1 + b*curv); //BOTTOM
  curvBase2 = -curv/( 1 - b*curv); //TOP
}

real FilamentMapping::
getEdgeSpacing( const realArray &x00, int iEdge, int iOffset)
{
  return( sqrt( pow(x00(iEdge+iOffset,0)-x00(iEdge,0),2.) + pow(x00(iEdge+iOffset,1)-x00(iEdge+iOffset,1),2.)));
}

