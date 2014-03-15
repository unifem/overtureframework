#include "HyperbolicMapping.h"
#include "MappingInformation.h"
#include "MappingRC.h"
#include "DataPointMapping.h"
#include "arrayGetIndex.h"
#include <float.h>
#include "EquiDistribute.h"
#include "display.h"
#include "TridiagonalSolver.h"
#include "StretchMapping.h"
//  #include "GL_GraphicsInterface.h"
//  #include <GL/gl.h>
#include "GenericGraphicsInterface.h"

#include "CompositeSurface.h"

#include "TrimmedMapping.h"
#include "ReductionMapping.h"
#include "NurbsMapping.h"
#include "ComposeMapping.h"
#include "SplineMapping.h"
#include "ReparameterizationTransform.h"
#include "LineMapping.h"
#include "PlaneMapping.h"

#include "EllipticGridGenerator.h"
#include "MappingProjectionParameters.h"
#include "MatchingCurve.h"

// interface to hypgen:
int 
hyper(
      int & IFORM, int & IZSTRT, int & NZREG,
      int & NPZREG, real & ZREG,  real & DZ0, real &  DZ1,
      int & IBCJA,int & IBCJB,int & IBCKA,int & IBCKB,
      int & IVSPEC, real & EPSSS, int & ITSVOL,
      int & IMETH, real & SMU2,
      real & TIMJ, real & TIMK,
      int & IAXIS, real & EXAXIS, real & VOLRES,
      int & JMAX, int & KMAX,
      int & JDIM,int & KDIM,int & LMAX,
      real & X, real & Y, real & Z,
      realArray & XW, realArray & YW, realArray & ZW );

FILE* HyperbolicMapping::debugFile=NULL;


HyperbolicMapping::
HyperbolicMapping() : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  
///     Create a mapping that can be used to generate a hyperbolic volume grid.
///  
//===========================================================================
{ 

  if( (Mapping::debug !=0) && (debugFile==NULL) )
    debugFile = fopen("hype.debug","w" );      // Here is the debug file, who closes this?

  initialize();
  
  initializeHyperbolicGridParameters();
  mappingHasChanged();
}

//! Constructor
/*!
  \param {surface_ (input)} Generate the grid starting from this curve (2D) or surface (3D)
  \param surface_ (input) Generate the grid starting from this curve (2D) or surface (3D)
*/
HyperbolicMapping::
HyperbolicMapping(Mapping & surface_) : Mapping(2,2,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  
///     Create a mapping that can be used to generate a hyperbolic volume grid.
/// \param surface_ (input): Generate the grid starting from this curve (2D) or
///  surface (3D)
//===========================================================================
{ 
  initialize();
  
  surface=&surface_;
  surface->uncountedReferencesMayExist();
  surface->incrementReferenceCount();

  setup();
  initializeHyperbolicGridParameters();
  mappingHasChanged();
}

HyperbolicMapping::
HyperbolicMapping(Mapping & surface_, Mapping & startingCurve) : Mapping(2,3,parameterSpace,cartesianSpace)   
//===========================================================================
/// \brief  
///     Create a  hyperbolic surface grid.
/// \param surface_ (input): Generate the grid on this surface in 3D.
/// \param startingCurve : 
//===========================================================================
{ 
  initialize();
  
  surface=&surface_;
  surface->uncountedReferencesMayExist();
  surface->incrementReferenceCount();

  startCurve=&startingCurve;
  startCurve->uncountedReferencesMayExist();
  startCurve->incrementReferenceCount();
  
  surfaceGrid=true;

  setup();
  initializeHyperbolicGridParameters();
  mappingHasChanged();
}

int HyperbolicMapping::
initialize()
// ===================================================================================
// /Purpose:
//   Initialize stuff required by all constructors.
// ===================================================================================
{
  HyperbolicMapping::className="HyperbolicMapping";
  setName( Mapping::mappingName,"hyperbolicMapping");

  // *wdh* 2011/10/01  
  // We should always set this to be true: (in case the grid is created in serial but read back in parallel)
  mapIsDistributed=true;  
  inverseIsDistributed=true;

  info=0;

  setGridDimensions( axis1,31 );
  setGridDimensions( axis2,7  );
  surface=NULL;
  dpm=NULL;
  normalDistribution=NULL;
  surfaceGrid=false; // true if we are generating a surface grid
  startCurve=NULL;  // defines the surface for a surface grid

  pStartCurveStretchParams=NULL; //  defines stretching of the starting curve or surface
  startCurveStretchMapping=NULL;
  useStartCurveStretchingWhileMarching=true;
  
  applyBoundaryConditionsToStartCurve=false;
  
  projectInitialCurve=true;
  projectNormalsOnMatchingBoundaries=true; // if true, project both points and normals to matching boundary curves/surfaces
  correctProjectionOfInitialCurves=true; // if true correct the projection of the initial curve to match edges on the triangluation
  stopOnNegativeCells=true;
  
  useTriangulation=true;      // if true, use the triangulation of a CompositeSurface for marching.
  projectOntoReferenceSurface=true;  // if true, project points found using the triangulation onto the actual surface
  plotGhostPoints=true;
  numberOfGhostLinesToPlot=1;
  
  saveReferenceSurface=1; // 0=do not save, 1=save for 2D grids, 2=save for all grids
  edgeCurveMatchingTolerance=1.e-4;  // relative tol for matching adjacent edge curves
  distanceToBoundaryCurveTolerance=1.e-3; // for deciding if boundary curves match to the start curve
  
  smoothAndProject=false; // if true smooth and project onto original surface definition.

  // choosePlotBoundsFromGlobalBounds: if true use global bounds for plotting, allows calling program to set the view
  choosePlotBoundsFromGlobalBounds=false;  

  boundaryOffsetWasApplied=false;
  boundaryOffsetOption=1;  // 0=old way, 1=new way. 
  
  // *wdh* new default ghostLineOption=extrapolateAnExtraGhostLine; 
  //  ghostLineOption=useLastLineAsGhostLine;
  
  ghostBoundaryCondition.redim(2,3);
  ghostBoundaryCondition=defaultGhostBoundaryCondition;

  for( int axis=0; axis<=1; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      boundaryConditionMapping[side][axis]=NULL;
      boundaryConditionMappingWasNewed[side][axis]=false;
    }
  }

  numberOfBoundaryCurves=0;
  boundaryCurves=NULL;
  initialCurveIsABoundaryCurve=false;
  
// We may match an interior grid line to a curve
//    numberOfMatchingCurves=0;    // number of interior lines matched
//    matchingCurves=NULL;
  
//    matchingCurvePosition=NULL;    // r coordinate on the start curve where matchin curve starts
//    matchingCurveDirection=NULL;   // match when growing in this direction (forward or backward)
//    matchingCurve=NULL;       // project grid line onto this mapping.
//    matchingProjectionParameters=NULL;
  
  spacingOption=spacingFromDistanceAndLines;
  numberOfLinesWithConstantSpacing=0;
  
  geometricFactor=1.1; 
  targetGridSpacing=-1.;  // negative means the user has not set a target
  initialSpacing=-1.;  // only use if positive

  for( int i=0; i<numberOfTimings; i++ )
    timing[i]=0.;

  totalNumberOfSteps=0;
  return 0;
}

bool HyperbolicMapping::
isDefined() const
// =====================================================================================
/// \details 
///     return true if the Mapping has been defined.
// =====================================================================================
{
  return dpm!=NULL;
}

int HyperbolicMapping::
printStatistics(FILE *file /* =stdout */ )
// ==========================================================================================================
/// \details 
///     Print timing statistics.
// =========================================================================================================
{
  int numberOfGridPoints=domainDimension==2 ? getGridDimensions(0) : getGridDimensions(0)*getGridDimensions(1);
  
  int numberOfStepsTaken=max(totalNumberOfSteps,1);
  numberOfGridPoints*=numberOfStepsTaken;

  fprintf(file,"  ========== Statistics for HyperbolicMapping, grid points=%i, steps=%i =================\n"
         "   Timings:                      seconds    sec/step   sec/step/pt     %%   \n",
           numberOfGridPoints,numberOfStepsTaken );

  timing[totalTime]=max(timing[0],REAL_MIN*10.);
  
  aString timingName[numberOfTimings]=
  {
    "total time",
    "timeForProject",
    "timeForSetupRHS",
    "timeForImplicitSolve",
    "timeForTridiagonalSolve",
    "timeForFormBlockTridiagonalSystem",
    "timeForFormCMatrix",
    "timeForNormalAndSurfaceArea",
    "timeForSmoothing",
    "timeForUpdate",
    "timeForBoundaryConditions"
  };

  int nSpace=35;
  aString dots="......................................................................";
  int i;
  if( timing[totalTime]==0. )
    timing[totalTime]=REAL_MIN;
  for( i=0; i<numberOfTimings; i++ )
    if( timingName[i]!="" )    
      fprintf(file,"%s%s%10.2e  %10.2e  %10.2e   %7.3f\n",(const char*)timingName[i],
         (const char*)dots(0,max(0,nSpace-timingName[i].length())),
	  timing[i],timing[i]/numberOfStepsTaken,timing[i]/numberOfStepsTaken/numberOfGridPoints,
          100.*timing[i]/timing[totalTime]);



  if( surfaceGrid && surface!=NULL && surface->getClassName()=="CompositeSurface" )
    ((CompositeSurface*) surface)->printStatistics(file);
    

  return 0;
}

//============================================================================================
//!  Supply a mapping to match a boundary condition to.
/*! 
  \param side,axis (input) : match to this boundary of the hyperbolic grid.
  \param map (input): Match the boundary values of the grid to lie on this surface or
    match the boundary values to lie on the face of this Mapping defined by
     (mapSide,mapAxis).
  \param mapSide,mapAxis (input) : use this face of the Mapping `map'. Supply these values if
   the hyperbolic grid is to be matched to a face of 'map', rather than map itself.
   */
//============================================================================================
int HyperbolicMapping::
setBoundaryConditionMapping(const int & side, 
			    const int & axis,
			    Mapping & map,
                            const int & mapSide /* =-1 */, 
			    const int & mapAxis /* =-1 */)
//===========================================================================
/// \brief  
///    Supply a mapping to match a boundary condition to.
/// \param side,axis (input) : match to this boundary of the hyperbolic grid.
/// \param map (input): Match the boundary values of the grid to lie on this surface or
///     match the boundary values to lie on the face of this Mapping defined by
///      (mapSide,mapAxis).
/// \param mapSide,mapAxis (input) : use this face of the Mapping `map'. Supply these values if
///    the hyperbolic grid is to be matched to a face of 'map', rather than map itself.
///  
//===========================================================================
{ 
  assert( side>=0 && side<=1 && axis>=0 && axis<=1 );
  
  if( map.getDomainDimension()==map.getRangeDimension()-1 )
  {
    boundaryConditionMapping[side][axis]=&map;
    boundaryConditionMappingWasNewed[side][axis]=false;
  }
  else
  {
    boundaryConditionMapping[side][axis]= new ReductionMapping(map,mapAxis,mapSide);
    boundaryConditionMappingWasNewed[side][axis]=true;
  }
  
  mappingHasChanged();
  return 0;
}


int HyperbolicMapping::
setSurface(Mapping & surface_, bool isSurfaceGrid /* =true */, bool init /* = true */ ) 
//===========================================================================
/// \brief  
///     Supply the curve/surface from which the grid will be generated.
/// \param surface_ (input): Generate the grid starting from this curve (2D) or
///  surface (3D)
/// \param isSurfaceGrid (input) : set to true if a surface grid should be built, set to false if a volume grid
///    should be created.
/// \param init (input) : if true, initialize hyperbolic parameters such as the distance to march etc.
///       If false, keep parameters as they are. 
//===========================================================================
{ 
  if( surface!=0 && surface->decrementReferenceCount()==0 )
    delete surface;

  surface=&surface_;
  surface->uncountedReferencesMayExist();
  surface->incrementReferenceCount();

  if( isSurfaceGrid && surface->getDomainDimension()==2 && surface->getRangeDimension()==3 )
    surfaceGrid=true;

  if( init )
  {
    setup();
    initializeHyperbolicGridParameters(); // *wdh* 080129
  }
  else
  {
    evaluateTheSurface=true;
  }
  
  
  mappingHasChanged();
  return 0;
}


void HyperbolicMapping::
setIsSurfaceGrid( bool trueOrFalse )
//===========================================================================
/// \brief  
///     Indicate whether a surface grid or volume grid should be built.
/// \param trueOrFalse (input) : set to true if a surface grid should be built, set to false if a volume grid
///    should be created.
//===========================================================================
{
  surfaceGrid=trueOrFalse;
}




int HyperbolicMapping::
setStartingCurve(Mapping & startingCurve, bool init /* = true */ )
//===========================================================================
/// \brief  
///     Supply a starting curve for a surface grid.
/// \param startingCurve (input): 
/// \param init (input) : if true, initialize hyperbolic parameters such as the distance to march etc.
///       If false, keep parameters as they are. 
//===========================================================================
{ 
  if( startCurve!=NULL && startCurve->decrementReferenceCount()==0 )
    delete startCurve;   // *wdh* 001206
  
  startCurve=&startingCurve;
  startCurve->uncountedReferencesMayExist();
  startCurve->incrementReferenceCount();

  if( init )
  {
    setup();                              // *wdh* 080129
    initializeHyperbolicGridParameters(); // *wdh* 080129
  }
  else
  {
    evaluateTheSurface=true;
  }

  mappingHasChanged();
  return 0;
}



int HyperbolicMapping::
saveReferenceSurfaceWhenPut(bool trueOrFalse /* = true */ )
//===========================================================================
/// \brief  
///    Save the reference surface and starting curve when 'put' is called.
/// \param init (input) : if true, initialize hyperbolic parameters such as the distance to march etc.
//===========================================================================
{
  if( trueOrFalse )
    saveReferenceSurface=2;  // 2 means save in both 2d and 3d 
  else
    saveReferenceSurface=0;

  return 0;
}

//! Set the boundary condition and offset.
/*!
 \param offset (input) : if offset==-1 (default) then set the default offset
     for the given value of bc.    
 */
int HyperbolicMapping::
setBoundaryConditionAndOffset(int side, int axis, int bcValue, int offset /* = -1 */ )
{
  setBoundaryCondition(side,axis,bcValue);
  if( offset>=0 )
    boundaryOffset[side][axis]=offset;
  else
  {
    // set default values -- 
    //   1) interpolation boundaries on surface grids have default offset=1
    //   2) If the end BC in the marching direction is interpolation then use offset=1
    boundaryOffset[side][axis]=0;
    if( bcValue==0 && domainDimension==2 && rangeDimension==3 )
      boundaryOffset[side][axis]=1;
    else if( bcValue==0 && axis==domainDimension )
      boundaryOffset[side][axis]=1;
    else
      boundaryOffset[side][axis]=0;
  }
  return 0;
}



int HyperbolicMapping::
setup()
//===========================================================================
/// \param Access: protected.
/// \brief  
///     Define properties of this mapping
//===========================================================================
{ 
  assert( surface!=NULL );
  evaluateTheSurface=true;

  
  // Define properties of this mapping
  if( getName(mappingName)=="hyperbolicMapping" )
    setName(mappingName,aString("hyperbolic-")+surface->getName(mappingName));

  if( boundaryCondition.getLength(0)<2 )
  {
    boundaryCondition.redim(2,3);
    boundaryCondition=domainDimension==3 ? outwardSplay : freeFloating;
  }
  
  if( projectGhostPoints.getLength(0)<2 )
  {
    projectGhostPoints.redim(2,3);
    projectGhostPoints=true;
  }
  
  if( surfaceGrid && startCurve==NULL )
  {
    setDomainDimension(2);
    setRangeDimension(3);
    return 0;
  }
  
  assert( surface!=NULL );
  if( surfaceGrid )
    assert( startCurve!=NULL );
  Mapping & map = surfaceGrid ? *startCurve : *surface;
  
  setDomainDimension(map.getDomainDimension()+1);
  setRangeDimension(map.getRangeDimension());

  boundaryOffset[0][0]=0; boundaryOffset[1][0]=0;
  boundaryOffset[0][1]=0; boundaryOffset[1][1]=0;
  boundaryOffset[0][2]=0; boundaryOffset[1][2]=0;
  boundaryOffsetWasApplied=false;
  

  int axis;
  for( axis=0; axis<domainDimension-1; axis++ )
  {
    setGridDimensions(axis,map.getGridDimensions(axis));
    setIsPeriodic(axis,map.getIsPeriodic(axis));

    // printf("**** setup: getIsPeriodic = %i \n",getIsPeriodic(axis1));
    
    for( int side=Start; side<=End; side++ )
    {
      if( (bool)getIsPeriodic(axis) )
        setBoundaryCondition(side,axis,-1);
      else
      {
        setBoundaryConditionAndOffset(side,axis,0);
        // boundaryOffset[side][axis]=1;
      }
      setTypeOfCoordinateSingularity(side,axis,map.getTypeOfCoordinateSingularity(side,axis));
    }
  }
  
  // default number of grid points in normal direction
  axis=domainDimension-1;
  setGridDimensions(axis,11);
  setBoundaryConditionAndOffset(Start,axis,(domainDimension==3 ? (int)outwardSplay : (int)freeFloating));
  setBoundaryConditionAndOffset(End  ,axis,0); 
  // boundaryOffset[End][axis]=1;

  setIsPeriodic(axis,notPeriodic);
  for( int side=Start; side<=End; side++ )
    setTypeOfCoordinateSingularity(side,axis,noCoordinateSingularity);

      

  for( axis=0; axis<domainDimension-1; axis++ )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( (bool)getIsPeriodic(axis) )
      {
	boundaryCondition(side,axis)=periodic;
      }
      else if( getTypeOfCoordinateSingularity(side,axis)==polarSingularity )
      {
	boundaryCondition(side,axis)=singularAxis;
      }
      else if( boundaryCondition(side,axis)==periodic )
      {
	boundaryCondition(side,axis)=domainDimension==3 ? outwardSplay : freeFloating;
      }
    }
  }

  if( surfaceGrid )
  {
    // set some parameters for surface grids
    numberOfVolumeSmoothingIterations=0;
  }
  

  mappingHasChanged();
  return 0;
}


int HyperbolicMapping::
initializeHyperbolicGridParameters()
//===========================================================================
// /Access: protected.
// /Purpose: 
//    Protected routine: Initialize hyperbolic grid parameters.
//\end{HyperbolicMappingInclude.tex}
//===========================================================================
{
  growthOption=1;  // +1, -1 or -2 or 2=grow in both directions

  linesToMarch[0]=linesToMarch[1]=11;
  distance[0]=distance[1]=1.;
  upwindDissipationCoefficient=0.;
  spacingType=constantSpacing;

  
  numberOfVolumeSmoothingIterations=20;
  curvatureSpeedCoefficient=.0;
  uniformDissipationCoefficient=.1;
  boundaryUniformDissipationCoefficient=.01;
  dissipationTransition=0;   // ramp dissipation from line 0 to this line number

  numberOfSmoothingIterations=1;  // for smoothGrid
  
  
  implicitCoefficient=1.;
  minimumGridSpacing=0.;
  arcLengthWeight=1.;
  curvatureWeight=0.;
  normalCurvatureWeight=0.;
  removeNormalSmoothing=false;

  startCurveStart=0.; 
  startCurveEnd=1.;


  equidistributionWeight=.0;
  splayFactor[0][0]=splayFactor[1][0]=splayFactor[0][1]=splayFactor[1][1]=.1;
  
  if( rangeDimension<3 )
    matchToMappingOrthogonalFactor=.5;
  else
  {
    // *wdh* matchToMappingOrthogonalFactor=.0;  // trouble on BC's with a CompositeSurface
    matchToMappingOrthogonalFactor=.5;  // try this now *wdh* 01119
  }
  
  if( domainDimension<rangeDimension )
  {
    // default parameters for a surface grid
    // *wdh* 010428 : implicitCoefficient=0.;  // a value of 1 now works after bug fixed
    uniformDissipationCoefficient=.1;
    // ** numberOfVolumeSmoothingIterations=0;   // leave default.
  }
  
  for( int side=Start; side<=End; side++ )
  {
    for( int axis=0; axis<2; axis++ ) 
      numberOfLinesForNormalBlend[side][axis]=3;
  }
  
  return 0;
}


// Copy constructor is deep by default
HyperbolicMapping::
HyperbolicMapping( const HyperbolicMapping & map, const CopyType copyType )
{
//   HyperbolicMapping::className="HyperbolicMapping";
//   dpm=NULL;
//   normalDistribution=NULL;
//   startCurve=NULL;

  initialize();
  
  if( copyType==DEEP )
  {
    *this=map;
  }
  else
  {
    cout << "HyperbolicMapping:: sorry no shallow copy constructor, doing a deep! \n";
    *this=map;
  }
}

HyperbolicMapping::
~HyperbolicMapping()
{ 
  if( debug & 4 )
    cout << " HyperbolicMapping::Desctructor called" << endl;
  if( dpm!=NULL && dpm->decrementReferenceCount()==0 )
    delete dpm;
  if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
    delete normalDistribution;
  for( int axis=0; axis<=1; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( boundaryConditionMappingWasNewed[side][axis] )
        delete boundaryConditionMapping[side][axis];
    }
  }
  if( surface!=0 && surface->decrementReferenceCount()==0 )
  {
    // printf("**** HyperbolicMapping::deleting surface\n");
    delete surface;
  }
  if( startCurve!=0 && startCurve->decrementReferenceCount()==0 )
  {
    // printf("**** HyperbolicMapping::deleting startCurve\n");
    delete startCurve;
  }

  delete pStartCurveStretchParams;
  if( startCurveStretchMapping!=NULL && startCurveStretchMapping->decrementReferenceCount()==0 )
    delete startCurveStretchMapping;
  
}



HyperbolicMapping & HyperbolicMapping::
operator=( const HyperbolicMapping & X )
{
  if( HyperbolicMapping::className != X.getClassName() )
  {
    cout << "HyperbolicMapping::operator= ERROR trying to set a HyperbolicMapping = to a" 
      << " mapping of type " << X.getClassName() << endl;
    return *this;
  }
  this->Mapping::operator=(X);            // call = for derivee class
  if( surface!=0 && surface->decrementReferenceCount()==0 ) 
    delete surface;
  surface      =X.surface;        // shallow copy. fix this ???? ************************8
  if( surface!=NULL ) surface->incrementReferenceCount();
  
  
  //..Copy parameters    **pf, added 2000-10-03, follows the layout in get()
  int axis, side;
  growthOption                       = X.growthOption;
  distance[0]                        = X.distance[0];
  distance[1]                        = X.distance[1];
  linesToMarch[0]                    = X.linesToMarch[0];
  linesToMarch[1]                    = X.linesToMarch[1];
  upwindDissipationCoefficient       = X.upwindDissipationCoefficient;
  uniformDissipationCoefficient      = X.uniformDissipationCoefficient;
  boundaryUniformDissipationCoefficient=X.boundaryUniformDissipationCoefficient;
  dissipationTransition              = X.dissipationTransition;

  numberOfVolumeSmoothingIterations  = X.numberOfVolumeSmoothingIterations;
  curvatureSpeedCoefficient          = X.curvatureSpeedCoefficient;
  implicitCoefficient                = X.implicitCoefficient;
  removeNormalSmoothing              = X.removeNormalSmoothing;
  equidistributionWeight             = X.equidistributionWeight;
  targetGridSpacing                  = X.targetGridSpacing;

  boundaryCondition.redim(0);
  projectGhostPoints.redim(0);
  boundaryCondition                  = X.boundaryCondition;
  ghostBoundaryCondition             = X.ghostBoundaryCondition;
  projectGhostPoints                 = X.projectGhostPoints;
  //  for( axis=0; axis<domainDimension-1; axis++ ) {
  //    for( side=Start; side<=End; side++ )  {
  //	boundaryCondition(side,axis)   = X.boundaryCondition(side,axis);
  //	projectGhostPoints(side,axis)  = X.projectGhostPoints(side,axis);
  //}}

  //....These are IntegerArrays --> should redim before using?
  indexRange                         = X.indexRange;
  gridIndexRange                     = X.gridIndexRange;
  dimension                          = X.dimension;

  for( axis=0; axis< 2; axis++ ) 
  {
    for( side=0; side< 2; side++ ) 
    {
      numberOfLinesForNormalBlend[side][axis] = X.numberOfLinesForNormalBlend[side][axis]; // *bug* found by Alex Main 2013/08/26
      splayFactor[side][axis]                 = X.splayFactor[side][axis];
    }
  }
  for( axis=0; axis<3; axis++ ) 
  {
    for( side=0; side< 2; side++ ) 
    {
      boundaryOffset[side][axis]=X.boundaryOffset[side][axis];
    }
  }
  boundaryOffsetWasApplied=X.boundaryOffsetWasApplied;

  spacingType                        = X.spacingType;
  spacingOption                      = X.spacingOption;
  geometricFactor                    = X.geometricFactor;
  geometricNormalization[0]          = X.geometricNormalization[0];
  geometricNormalization[1]          = X.geometricNormalization[1];
  initialSpacing                     = X.initialSpacing;
  minimumGridSpacing                 = X.minimumGridSpacing;

  arcLengthWeight                    = X.arcLengthWeight;
  curvatureWeight                    = X.curvatureWeight;
  normalCurvatureWeight              = X.normalCurvatureWeight;
  matchToMappingOrthogonalFactor     = X.matchToMappingOrthogonalFactor;
  surfaceGrid                        = X.surfaceGrid;
  
  projectInitialCurve                = X.projectInitialCurve;
  applyBoundaryConditionsToStartCurve= X.applyBoundaryConditionsToStartCurve;
  stopOnNegativeCells                = X.stopOnNegativeCells;
  plotGhostPoints                    = X.plotGhostPoints;
  saveReferenceSurface               = X.saveReferenceSurface;
  startCurveStart                    = X.startCurveStart; 
  startCurveEnd                      = X.startCurveEnd;
  //  trailingEdgeDirection.redim(1,1,1,rangeDimension);
  trailingEdgeDirection.redim(0);
  trailingEdgeDirection              = X.trailingEdgeDirection;
  smoothAndProject                   = X.smoothAndProject;
  //..END Copy parameters **pf

  if( true )
  {
    // *wdh* 101104 
    // -- deep copy for the DPM 
    if( X.dpm!=NULL )
    {
      printF("*** Hype: operator= DEEP COPY DPM *****\n");
      
      if( dpm==NULL )
      {
	dpm = new DataPointMapping; dpm->incrementReferenceCount();
      }
      *dpm = *X.dpm;
    }
    else
    {
      if( dpm!=NULL && dpm->decrementReferenceCount()==0 )
	delete dpm;
      dpm = NULL;
    }
    
  }
  else
  {
    // old:
    if( dpm!=NULL && dpm->decrementReferenceCount()==0 )
      delete dpm;
    dpm = X.dpm;
    if( dpm!=NULL )
      dpm->incrementReferenceCount();
  }
  
  if( normalDistribution!=NULL && normalDistribution->decrementReferenceCount()==0 )
    delete normalDistribution;
  normalDistribution = X.normalDistribution;
  if( normalDistribution!=NULL )
    normalDistribution->incrementReferenceCount();
    
  if( startCurve!=NULL && startCurve->decrementReferenceCount()==0 )
    delete startCurve;
  startCurve = X.startCurve;
  if( startCurve!=NULL )
    startCurve->incrementReferenceCount();
    
  for( axis=0; axis<=1; axis++ )  // int axis,side is declared above **pf
  {
    for( side=0; side<=1; side++ )
    {
      boundaryConditionMapping[side][axis]=X.boundaryConditionMapping[side][axis];
      boundaryConditionMappingWasNewed[side][axis]=false;  // ****** need to reference count *******
    }
  }
  
  matchingCurves=X.matchingCurves;
  
//    destroyInteriorMatchingCurves();
//    numberOfMatchingCurves=X.numberOfMatchingCurves;
//    matchingCurvePosition = new real [numberOfMatchingCurves+1];
//    matchingCurveDirection= new  int [numberOfMatchingCurves+1];
//    matchingCurve =     new Mapping *[numberOfMatchingCurves+1];
//    matchingProjectionParameters = new MappingProjectionParameters *[numberOfMatchingCurves+1];

//    for( int i=0; i<numberOfMatchingCurves; i++ )
//    {
//      matchingCurvePosition[i]=X.matchingCurvePosition[i];
//      matchingCurveDirection[i]=X.matchingCurveDirection[i];
//      matchingCurve[i]= X.matchingCurve[i];
//      matchingCurve[i]->incrementReferenceCount();
    
//      // make a deep copy here since MappingProjectionParameters are not reference counted.
//      // matchingProjectionParameters[i]=X.matchingProjectionParameters[i];
//      matchingProjectionParameters[i] = new MappingProjectionParameters;
//      *matchingProjectionParameters[i] = *X.matchingProjectionParameters[i];
//    }

  //remakeGrid = true;  // evaluate grid for plotting
  mappingHasChanged();
  return *this;
}


int HyperbolicMapping::
setParameters(const HyperbolicParameter & par, 
		    real value,
	      const Direction & direction /* = bothDirections */ )
{
  IntegerArray ipar;
  RealArray rpar(1);
  rpar(0)=value;
  return setParameters(par,ipar,rpar,direction );
}


int HyperbolicMapping::
setParameters(const HyperbolicParameter & par, 
	      int  value,
	      const Direction & direction /* = bothDirections */ )
{
  IntegerArray ipar(1);
  RealArray rpar;
  ipar(0)=value;
  return setParameters(par,ipar,rpar,direction );
}


int HyperbolicMapping::
setParameters(const HyperbolicParameter & par, 
	      const IntegerArray & ipar /* = Overture::nullIntArray() */, 
	      const RealArray & rpar /* = Overture::nullRealDistributedArray() */ ,
              const Direction & direction /* = bothDirections */ )
//===========================================================================
/// \brief  
///     Define a parameter for the hyperbolic grid generator.
/// \param par (input): The possible value come from the enum {\tt  HyperbolicParameter}:
///   <ul>
///    <li> <B>growInBothDirections</B> : grow the grid in both directions.
///    <li> <B>growInTheReverseDirection</B> : grow the grid in the reverse direction (this will
///       result in a left handed coordinate system.
///    <li> <B>numberOfRegionsInTheNormalDirection</B> 
///    <li> <B>stretchingInTheNormalDirection</B> 
///    <li> <B>linesInTheNormalDirection</B> : specify the number of lines to use in the normal direction.
///    <li> <B>distanceToMarch</B> : ipar(0) = region number, rpar(0) = distance
///    <li> <B>spacing</B> : ipar(0) = region number, rpar(0) = dz0, rpar(1)=dz1
///    <li> <B>boundaryConditions</B> 
///    <li> <B>dissipation</B> 
///    <li> <B>volumeParameters</B> 
///    <li> <B>barthImplicitness</B> 
///    <li> <B>axisParameters</B> 
///    <li> <B>THEtargetGridSpacing</B>: rpar(0) gives the target grid spacing when choosing the number of grid points
///        in the tangential direction (i.e. for the start curve and for marching on surfaces).
///             A negative value means use a best guess.
///    <li> <B>THEinitialGridSpacing</B>: rpar(0) gives the target grid spacing when choosing the number of grid points
///        for marching volume grids (e.g. the spacing of the first grid line for volume grids).
///             A negative value means use a best guess.
///    <li> <B>THEspacingType</B> : a value from SpacingType enum
///    <li> <B>THEspacingOption</B> : a value from SpacinOptionEnum
///    <li> <B>THEgeometricFactor</B> : the geometric spacing factor
///   </ul>
/// \param value (input):
/// \param direction (input) : The hyperbolic surface can be grown in two possible directions
///   (or both directions). {\tt direction} indicates which direction the new parameter 
///    values should apply to: (enum Direction)
///   <ul>
///     <li>[direction=bothDirections]  : parameters apply to both the forward and reverse directions.
///     <li>[direction=forwardDirection] : parameters apply to the forward direction.
///     <li>[direction=reverseDirection] : parameters apply to the reverse direction.
///   </ul>
//===========================================================================
{ 
  Range D(0,1);  // change both directions by default.
  if( direction==forwardDirection )
    D=Range(0,0);     // change only forward direction
  else if( direction==reverseDirection )
    D=Range(1,1);     // change only reverse direction
  switch( par )
  {
  case growInTheReverseDirection:
    growthOption=-1;
    break;
  case growInBothDirections:
    growthOption=2;
    break;
  case numberOfRegionsInTheNormalDirection:
    break;
  case  stretchingInTheNormalDirection:
    break;
  case linesInTheNormalDirection:
    if( direction==forwardDirection || direction==bothDirections )
      linesToMarch[0]=ipar(0);
    if( direction==reverseDirection || direction==bothDirections )
      linesToMarch[1]=ipar(0);
    break;
  case THEboundaryConditions:
    for( int n=0; n<=min(5,ipar.getBound(0)); n++ )
    {
      int side=n%2;
      int axis=(n/2);
      boundaryCondition(side,axis)=ipar(n);
    }
    // ::display(boundaryCondition,"****HYPE:setPar boundaryCondition");
    break;
  case THEghostBoundaryConditions:
    for( int n=0; n<=min(5,ipar.getBound(0)); n++ )
    {
      int side=n%2;
      int axis=(n/2);
      ghostBoundaryCondition(side,axis)=ipar(n);
    }
    // ::display(boundaryCondition,"****HYPE:setPar boundaryCondition");
    break;
  case THEapplyBoundaryConditionsToStartCurve:
    applyBoundaryConditionsToStartCurve=ipar(0);
    break;
  case dissipation:
//    imeth=ipar(0);
//    smu2=rpar(0);
    break;
  case distanceToMarch:
//     if( ipar.getLength(0)<1 || ipar(0)<0 || ipar(0)>nzreg )
//     {
//       cout << "HyperbolicMapping::setParameters:ERROR in setting distanceToMarch, ipar(0) is invalid\n";
//       {throw "error";}
//     }
//    zreg(ipar(0),D)=rpar(0);
    if( direction==forwardDirection || direction==bothDirections )
      distance[0]=rpar(0);
    if( direction==reverseDirection || direction==bothDirections )
      distance[1]=rpar(0);
    break;
  case spacing:
//     if( ipar.getLength(0)<1 || ipar(0)<0 || ipar(0)>nzreg )
//     {
//       cout << "HyperbolicMapping::setParameters:ERROR in setting spacing, ipar(0) is invalid\n";
//       {throw "error";}
//     }
//    dz0(ipar(0),D)=rpar(0);
//    dz1(ipar(0),D)=rpar(1);
    break;
  case volumeParameters:
//     ivspec=ipar(0);
//     epsss=rpar(0);
//     itsvol=ipar(1);

    numberOfVolumeSmoothingIterations=ipar(1);
    
    break;
  case barthImplicitness:
//     timj=rpar(0);
//     timk=rpar(1);
    break;
  case axisParameters:
//     iaxis=ipar(0);
//     exaxis=rpar(0);
//     volres=rpar(1);
    break;
  case projectGhostBoundaries:
    projectGhostPoints(0,0)=ipar(0);
    projectGhostPoints(1,0)=ipar(1);
    projectGhostPoints(0,1)=ipar(2);
    projectGhostPoints(1,1)=ipar(3);

    break;
  case THEtargetGridSpacing:
    targetGridSpacing=rpar(0);
    break;
  case THEinitialGridSpacing:
    initialSpacing=rpar(0);
    break;
  case THEspacingType:
    spacingType=(SpacingType)ipar(0);
    break;
  case THEspacingOption:
    spacingOption=(SpacingOptionEnum)ipar(0);
    break;
  case THEgeometricFactor:
    geometricFactor=rpar(0);
    break;
  default:
    cout << "HyperbolicMapping::setParameter:ERROR: unknown parameter: \n" << par << endl;
    return 1;
  }
  return 0;
}

int HyperbolicMapping::
setPlotOption( PlotOptionEnum option, int value )
//===========================================================================
/// \details  set a plot option.
///  
/// \param choosePlotBoundsFromGlobalBounds: if true use global bounds for plotting, allows calling program to set the view
///  
//===========================================================================
{
  switch (option)
  {
  case setPlotBoundsFromGlobalBounds: 
    choosePlotBoundsFromGlobalBounds=(bool)value;
    break;
  default:
    cout << "HyperbolicMapping::setPlotOption: Unknown plot option!\n";
    
  }
  return 0;
}



int HyperbolicMapping::
hypgen(GenericGraphicsInterface & gi, GraphicsParameters & parameters)
{

  aString answer,line;
  aString menu[] = { "generate",
                    "new generate",
		    "grow grid in opposite direction",
		    "grow grid in both directions (toggle)",
                    "upwind dissipation coefficient",
                    "number of regions in normal direction (NZREG)",
                    "stretching in normal direction (IZSTRT)",
                    "lines in normal direction (NPZREG)",
                    "boundary condition (IBCJA,...)",
                    "dissipation (IMETH,SMU2)",
                    "distance to march (ZREG)",
                    "spacing (DZ0,DZ1)",
                    "volume parameters (IVSPEC,EPSSS,ITSVOL)",
                    "Barth implicitness factors (TIMJ,TIMK)",
                    "axis bc parameters (IAXIS,EXAXIS,VOLRES)",
		    "exit",
                    "" };                       // empty string denotes the end of the menu

  int numberOfDirectionsToGrow = abs(growthOption)>1 ? 2 : 1;
  for(;;)
  {
    gi.getMenuItem(menu,answer);               
    if( answer=="generate" )
    {
      int returnValue = generate();
      if( returnValue!=0 )
        break;
      gi.erase();
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      PlotIt::plot(gi,*this,parameters); 
    }
    else if( answer=="new generate" )
    {
      int returnValue = generateNew();
      if( returnValue!=0 )
        break;
      gi.erase();
      parameters.set(GI_TOP_LABEL,getName(mappingName));
      PlotIt::plot(gi,*this,parameters); 
    }
    else if( answer=="grow grid in opposite direction" )
    {
      growthOption=-growthOption;
    }
    else if( answer=="grow grid in both directions (toggle)" )
    {
      if( abs(growthOption)==2 )
        growthOption= growthOption > 0 ? 1 : -1;
      else
        growthOption= growthOption > 0 ? 2 : -2;
      numberOfDirectionsToGrow = abs(growthOption)>1 ? 2 : 1;
    }
//     else if( answer=="stretching in normal direction (IZSTRT)" )
//     {
//       gi.outputString("IZSTRT = 1  exponential stretching in L");
//       gi.outputString("       = 2  hyperbolic tangent stretching in L");
//       gi.outputString("       = -1 stretching function specified in file zetastr.i (@)");
//       gi.inputString(line,"Enter option (1=exp,2=hyperbolic tangent,3=specify explicitly)");
//       if( line!="" ) sScanF( line,"%i",&izstrt);
//     }
//     else if( answer=="dissipation (IMETH,SMU2)" )
//     {
//       gi.outputString("  IMETH  = 0  constant coef. dissipation");
//       gi.outputString("         = 1  spatially-varying coef. dissipation");
//       gi.outputString("         = 2  severe convex corners treated by solving averaging eqns.");
//       gi.outputString("         = 3  severe convex corners treated by angle-bisecting predictor");
//       gi.outputString("  SMU2   = second order dissipation coef.");
//       gi.inputString(line,sPrintF(buff,"Enter imeth and smu2 (currrent values = %i, %e)",imeth,smu2));
//       if( line!="" ) sScanF( line,"%i %e",&imeth,&smu2);
//     }
    else if( answer=="upwind dissipation coefficient" )
    {
      gi.inputString(line,sPrintF("Enter upwindDissipationCoefficient (default= %e)",
               upwindDissipationCoefficient));
      if( line!="" ) sScanF( line,"%e",&upwindDissipationCoefficient);
    }
//     else if( answer=="lines in normal direction (NPZREG)" )
//     {
//       gi.outputString("NPZREG = number of points (including ends) in each L-region");
//       for( int dir=0; dir<numberOfDirectionsToGrow; dir++ )
//       {
// 	int direction= growthOption==-1 ? 1 : dir;
// 	for( int l=0; l<nzreg; l++ )
// 	{
// 	  gi.inputString(line,
//              sPrintF(buff,"Enter npzreg(%i) for direction=%i (number of lines in the normal direction, current=%i)",
// 				      l,1-2*direction,npzreg(l,direction)));
// 	  if( line!="" ) sScanF( line,"%i",&npzreg(l,direction));
// 	}
//       }
//     }
//     else if( answer=="number of regions in normal direction (NZREG)" )
//     {
//       gi.outputString("   NZREG  = number of L-regions");
//       gi.inputString(line,sPrintF(buff,"Enter nzreg (current=%i)",nzreg));
//       if( line!="" )
//       {
//         sScanF( line,"%i",&nzreg);
//         npzreg.resize(nzreg,2);
// 	dz0.resize(nzreg,2);
// 	dz1.resize(nzreg,2);
//         zreg.resize(nzreg,2);
//       }
//     }
//     else if( answer=="distance to march (ZREG)" )
//     {
//       gi.outputString("   ZREG   > 0  distance to march out for this L-region");
//       gi.outputString("          <=0  variable far field distance specified in file zetavar.i (#)");
//       for( int dir=0; dir<numberOfDirectionsToGrow; dir++ )
//       {
// 	for( int l=0; l<nzreg; l++ )
// 	{
// 	  gi.inputString(line,sPrintF(buff,"Enter zreg(%i) for direction=%i (current=%e)",1-2*dir,l,zreg(l,dir)));
// 	  if( line!="" ) sScanF( line,"%e",&zreg(l,dir));
// 	}
//       }
//     }
//     else if( answer=="spacing (DZ0,DZ1)" )
//     {
//       gi.outputString("   DZ0    = 0  initial spacing is not fixed for this L-region");
//       gi.outputString("          > 0  initial spacing for this L-region");
//       gi.outputString("          < 0  variable initial spacing specified in file zetavar.i (#)");
//       gi.outputString("   DZ1    = 0  end spacing is not fixed for this L-region");
//       gi.outputString("          > 0  end spacing for this L-region");
//       gi.outputString("          < 0  variable end spacing specified in file zetavar.i (#)");
//       gi.outputString("   (#) Applied to first L-region only");
//       for( int dir=0; dir<numberOfDirectionsToGrow; dir++ )
//       {
// 	for( int l=0; l<nzreg; l++ )
// 	{
// 	  gi.inputString(line,sPrintF(buff,"Enter dz0(%i),dz1(%i) for direction=%i (current=%e,%e)",
//                 l,l,1-2*dir,dz0(l,dir),dz1(l,dir)));
// 	  if( line!="" ) sScanF( line,"%e %e",&dz0(l,dir),&dz1(l,dir));
// 	}
//       }
//     }
//     else if( answer=="volume parameters (IVSPEC,EPSSS,ITSVOL)" )
//     {
//       gi.outputString("   IVSPEC = 1  volume spec. by cell area times arc length");
//       gi.outputString("          = 2  volume spec. by mixed spherical volumes scaling");
//       gi.outputString("   EPSSS  = parameter that controls how fast spherical volumes are mixed in");
//       gi.outputString("            (used with IVSPEC=2 only)");
//       gi.outputString("   ITSVOL = number of times volumes are averaged");
//       gi.inputString(line,sPrintF(buff,"Enter ivspec,epsss,itsvol (current=%i,%e,%i)",ivspec,epsss,itsvol));
//       if( line!="" ) sScanF( line,"%i %e %i",&ivspec,&epsss,&itsvol);
//     }
//     else if( answer=="Barth implicitness factors (TIMJ,TIMK)" )
//     {
//       gi.outputString("   TIMJ   = Barth implicitness factor in J");
//       gi.outputString("   TIMK   = Barth implicitness factor in K");
//       gi.inputString(line,sPrintF(buff,"Enter timj,timk (current=%e,%e)",timj,timk));
//       if( line!="" ) sScanF( line,"%e %e",&timj,&timk);
//     }
//     else if( answer=="boundary condition (IBCJA,...)" )
//     {
//       gi.outputString("   The boundary conditions types at J=1, J=JMAX, K=1, K=KMAX are indicated ");
//       gi.outputString("   by IBCJA, IBCJB, IBCKA, IBCKB, respectively");
//       gi.outputString(" ");
//       gi.outputString("   IBCJA  = -1  float X, Y and Z - zero order extrapolation (free floating)");
//       gi.outputString("          < -1  outward-splaying free floating boundary condition which bends");
//       gi.outputString("                the edge away from the interior. Use small $|$IBCJA$|$ for");
//       gi.outputString("                small bending - mixed zeroth and first order extrapolation");
//       gi.outputString("                with EXTJA = -IBCJA/1000.0 where EXTJA must satisfy 0 <EXTJA< 1");
//       gi.outputString("          =  1  fix X, float Y and Z (constant X plane)");
//       gi.outputString("          =  2  fix Y, float X and Z (constant Y plane)");
//       gi.outputString("          =  3  fix Z, float X and Y (constant Z plane)");
//       gi.outputString("          =  4  float X, fix Y and Z");
//       gi.outputString("          =  5  float Y, fix X and Z");
//       gi.outputString("          =  6  float Z, fix X and Y");
//       gi.outputString("          =  7  floating collapsed edge with matching upper and lower sides");
//       gi.outputString("                (points along K=1,(KMAX+1)/2 are matched with those on K=KMAX,(KMAX+1)/2)");
//       gi.outputString("          = 10  periodic condition (*)");
//       gi.outputString("          = 11  reflected symmetry condition with X=constant plane");
//       gi.outputString("          = 12  reflected symmetry condition with Y=constant plane");
//       gi.outputString("          = 13  reflected symmetry condition with Z=constant plane");
//       gi.outputString("          = 20  singular axis point");
//       gi.outputString("          = 21  constant X planes for interior and boundaries slices (*)");
//       gi.outputString("          = 22  constant Y planes for interior and boundaries slices (*)");
//       gi.outputString("          = 23  constant Z planes for interior and boundaries slices (*)");
//       gi.outputString("   (*) Must also apply at the other end condition in J");
//       gi.outputString("");
//       gi.outputString("   IBCJB, IBCKA, IBCKB likewise");
//       gi.inputString(line,sPrintF(buff,"Enter ibcja, ibcjb, ibcka, ibckb, (current =%i,%i,%i,%i)",
//                      ibcja,ibcjb,ibcka,ibckb));
//       if( line!="" ) sScanF( line,"%i %i %i %i",&ibcja, &ibcjb, &ibcka, &ibckb);
//     }
//     else if( answer=="axis bc parameters (IAXIS,EXAXIS,VOLRES)" )
//     {
      
//       gi.outputString("   The following 3 parameters are read in only if axis bc is activated");
//       gi.outputString(" ");
//       gi.outputString("   IAXIS = 1  extrapolation and volume scaling logic");
//       gi.outputString("         = 2  same as 1 but with dimple smoothing");
//       gi.outputString("   EXAXIS = 0  zeroth order extrapolation at axis");
//       gi.outputString("          > 0 and < 1  control local pointedness at axis (~0.3)");
//       gi.outputString("          = 1  first order extrapolation at axis");
//       gi.outputString("   VOLRES      restrict volume at one point from axis. This parameter is");
//       gi.outputString("               only switched on if exaxis is non-zero. Good values are");
//       gi.outputString("               ~0.1 to ~0.5");
//       gi.inputString(line,sPrintF(buff,"Enter iaxis,exaxis,volres (current=%i,%e,%e)",iaxis,exaxis,volres));
//       if( line!="" ) sScanF( line,"%i %e %e",&iaxis,&exaxis,&volres);
//     }
    else if( answer=="exit" )
    {
      break;
    }
    else
    {
      gi.outputString( sPrintF(line,"Unknown response=%s",(const char*)answer) );
    }
  }

  return 0;
}



int HyperbolicMapping::
smooth(GenericGraphicsInterface & gi, GraphicsParameters & parameters)
// ================================================================================================
/// \param Access: protected
/// \details 
///     Smooth the hyperbolic grid using the elliptic grid generator.
/// 
// ================================================================================================
{

  if( surface==NULL || dpm==NULL )
  {
    gi.outputString("Generate the hyperbolic grid first, before smoothing\n");
  }

  if( false )
  {
    real arcLengthWeight_,curvatureWeight_,areaWeight_;
    IntegerArray boundaryCondition0(2,3);
    boundaryCondition0=1;
    equiGridSmoother(*surface,*dpm,gi,parameters,boundaryCondition0,arcLengthWeight_,curvatureWeight_,areaWeight_);
  }
  else
  {
    EllipticGridGenerator gridGenerator;
    if( smoothAndProject )
      gridGenerator.setup(*dpm,surface);  // project onto original surface definition
    else
      gridGenerator.setup(*dpm);          // project onto the data point mapping.
    gridGenerator.startingGrid(dpm->getGrid());
    DataPointMapping dpm2;
    dpm2=*dpm;
    gridGenerator.update(dpm2,&gi,parameters);
    // *dpm=dpm2;
    Index I1,I2,I3;
    Range xAxes(0,rangeDimension-1);
    ::getIndex(gridIndexRange,I1,I2,I3);
    realArray & x = xHyper;
    if( domainDimension==2 )
    {
      x.reshape(x.dimension(0),x.dimension(2),1,xAxes);
      x(I1,I3,0,xAxes)=dpm2.getGrid()(I1,I3,0,xAxes);
      dpm->setDataPoints(x(I1,I3,0,xAxes),3,domainDimension);
      x.reshape(x.dimension(0),1,x.dimension(1),xAxes);
    }
    else
    {
      x(I1,I2,I3,xAxes)=dpm2.getGrid()(I1,I2,I3,xAxes);
      dpm->setDataPoints(x(I1,I2,I3,xAxes),3,domainDimension);
    }
    mappingHasChanged();
  }
  return 0;
}




/* -------------
int HyperbolicMapping::
inspectInitialSurface( realArray & xSurface, realArray & normal )
//===========================================================================
/// \brief  
///       Inspect the initial surface for corners etc.
//===========================================================================
{

  // find the min and max angles between normals to adjacent cells.

  realArray cosAngle;
  cosAngle = normal(I1,I2,0,axis1)*normal(I1+1,I2,0,axis1);
  minCosAngle=min(cosAngle);
  //
  // cosAngle == 0 : 90 degree corner
  //           < 0  : 
  //
  //         ^
   //        |                          |
  //   -------------                    |
  //               |             ^   <--|
  //               |             |      |
  //               | -->   -------------|   
  //               |

  return 0;
}
------------ */



int HyperbolicMapping::
generateOld()
//===========================================================================
/// \brief  
///     Generate the hyperbolic grid.  *** OLD VERSION ***
/// \return  0 on success, 1=hypgen not available
//===========================================================================
{
/* -----
  assert( surface!=NULL );
  if( dpm==NULL )
  {
    dpm=new DataPointMapping;
    dpm->incrementReferenceCount();
  }
  dpm->setName(mappingName,aString("hyperbolic-")+surface->getName(mappingName));
  dpm->setDomainDimension(domainDimension);
  dpm->setRangeDimension(rangeDimension);
      
  for( int axis=0; axis<domainDimension-1; axis++ )
  {
    dpm->setIsPeriodic(axis,getIsPeriodic(axis));
    for( int side=Start; side<=End; side++ )
    {
      dpm->setBoundaryCondition(side,axis,getBoundaryCondition(side,axis));
      dpm->setShare(side,axis,getShare(side,axis));
    }
  }


  int iform=1, jdim,kdim,lmax;
  
  realArray xw,yw,zw,xyz;

  int jmax,kmax;
  jmax=getGridDimensions(axis1);
  kmax=domainDimension==2 ? 1 : getGridDimensions(axis2);
  jdim=jmax+2;
  kdim=kmax+2;


  if( evaluateTheSurface || xSurface.getLength(0)!=jdim || xSurface.getLength(1)!=kdim )
  {
    printf("HyperbolicMapping::generate: evaluate the surface\n");
    
    evaluateTheSurface=false;
    xSurface.redim(jdim,kdim,3);
    realArray r(jdim,kdim,domainDimension-1);
    // -- evaluate the surface ---
    // first compute r:
    r=0.;
    real h1=1./(jmax-1.);
    if( domainDimension==2 )
      r(Range(0,jmax-1),0,0).seqAdd(0.,h1);
    else
    {
      for( int j=0; j<kmax; j++ )
	r(Range(0,jmax-1),j,0).seqAdd(0.,h1);
      real h2=1./(kmax-1.);
      for( int i=0; i<jmax; i++ )
	r(i,Range(0,kmax-1),1).seqAdd(0.,h2);  
    }
    xSurface=0.;  
    surface->mapGrid(r,xSurface);
  }
  
// **  Index I(0,jdim), J(0,kdim);

// -----

  int jmax0=jmax;
  int kmax0=kmax;

      
  bool growBothDirections = fabs(growthOption) > 1;
  int numberOfDirectionsToGrow = growBothDirections ? 2 : 1;
  int lmaxForward=sum(npzreg(Range(0,nzreg-1),0));
  int lmaxReverse=sum(npzreg(Range(0,nzreg-1),1));
  
  int gridLines = growBothDirections ? lmaxForward+lmaxReverse-1 : ( growthOption==1 ? lmaxForward : lmaxReverse);
  setGridDimensions(domainDimension-1,gridLines);
  if( domainDimension==2 )
    xyz.resize(jdim-2,1,gridLines,rangeDimension);   // this array holds the final grid
  else
    xyz.resize(jdim-2,kdim-2,gridLines,rangeDimension);
    
  realArray x0;
  for( int dir=0; dir<numberOfDirectionsToGrow; dir++ )
  {
    lmax = sum(npzreg(Range(0,nzreg-1),dir));
    if( growthOption==-1 || dir==1 )
    {
      // we need to flip the parameterization to grow in the opposite direction
      x0.redim(jdim,kdim,3);
      Range J(0,jdim-1);
      Range K(0,kdim-1);     // *note* negative stride not supported
      Range R3(0,2);
      if( domainDimension==2 )
      {
	for( int j=0; j<jmax; j++ )
	  x0(j,K,R3)=xSurface(jmax-j-1,K,R3);  // x0 holds a copy of the surface ?why? does hypgen change these values?
      }
      else
      { // flip k as this may be faster
	for( int k=0; k<kmax; k++ )
	  x0(J,k,R3)=xSurface(J,kmax-k-1,R3);  // x0 holds a copy of the surface ?why? does hypgen change these values?
      }
    }
    else
      x0=xSurface;    // x0 holds a copy of the surface ?why? does hypgen change these values?

    // x0.display("Here is x0");
    
    int returnValue;
    int lmax0=lmax;
#ifndef USE_PPP    
    returnValue=hyper(iform,izstrt,nzreg,
		      npzreg(0,dir),zreg(0,dir),dz0(0,dir),dz1(0,dir), 
		      ibcja, ibcjb, ibcka, ibckb,
		      ivspec, epsss, itsvol,
		      imeth, smu2,
		      timj, timk,
		      iaxis,exaxis,volres,
		      jmax,kmax,
		      jdim,kdim,lmax,
		      x0(0,0,0), x0(0,0,1), x0(0,0,2),
		      xw,yw,zw );
#else
    returnValue=1;
    printf("Cannot call hyper with P++ yet\n");
    Overture::abort("error");
#endif
    if( returnValue!=0 )
      return returnValue;  // hypgen not available
    // reset to the original values since hypgen changes these
    jmax=jmax0;
    kmax=kmax0;

    // xw.display("here is xw");
    // yw.display("here is yw");
    // zw.display("here is zw");

    printf("\n\n ++++ jmax=%i, kmax=%i, jdim=%i, kdim=%i, lmax=%i \n",jmax,kmax,jdim,kdim,lmax);
    if( domainDimension==2 )
    {
      Range J(0,jmax-1), L(0,lmax-1), L0;
      Range J1; 
      if( ibcja!=10 )
	J1=J+1;  // skip ghost line on a non-periodic boundary
      else
	J1=J; 
      xw.reshape(jdim,kdim,lmax0+2);
      yw.reshape(jdim,kdim,lmax0+2);
      if( dir==0 )
      {
        if( growBothDirections )
	  L0=L+lmaxReverse-1;
	else
          L0=L;
        xyz(J,0,L0,axis1)=xw(J1,1,L);   // 1=skip ghost point
        xyz(J,0,L0,axis2)=yw(J1,1,L); 
      }
      else
      {
        int jmax1=jmax-1+J1.getBase();
        for( int l=0; l<lmax-1; l++ )    // skip l=lmax-1 as this will be the initial surface again
        for( int j=0; j<jmax; j++ )
	{
          xyz(j,0,l,axis1)=xw(jmax1-j,1,lmax-l-1);   // flip back j and reverse l
          xyz(j,0,l,axis2)=yw(jmax1-j,1,lmax-l-1); 
	}
      }
      
      xw.reshape(jdim*kdim*(lmax0+2));
      yw.reshape(jdim*kdim*(lmax0+2));
    }
    else if( domainDimension==3 )
    {

      Range J(0,jmax-1), K(0,kmax-1), L(0,lmax-1), L0;
      Range J1,K1; 
      if( ibcja!=10 )
	J1=J+1;  // skip ghost line on a non-periodic boundary
      else
	J1=J; 
      if( ibcka!=10 )
	K1=K+1;  // skip ghost line on a non-periodic boundary
      else
	K1=K; 
      xw.reshape(jdim,kdim,lmax0+2);
      yw.reshape(jdim,kdim,lmax0+2);
      zw.reshape(jdim,kdim,lmax0+2);
      if( dir==0 )
      {
        if( growBothDirections )
	  L0=L+lmaxReverse-1;
	else
          L0=L;
	xyz(J,K,L0,axis1)=xw(J1,K1,L); 
	xyz(J,K,L0,axis2)=yw(J1,K1,L); 
	xyz(J,K,L0,axis3)=zw(J1,K1,L); 

      }
      else
      {
        int kmax1=kmax-1+K1.getBase();
        for( int l=0; l<lmax-1; l++ ) // skip l=lmax-1 as this will be the initial surface again
        for( int k=0; k<kmax; k++ )
	{
	  xyz(J,k,l,axis1)=xw(J1,kmax1-k,lmax-l-1);  // flip back k and reverse l
	  xyz(J,k,l,axis2)=yw(J1,kmax1-k,lmax-l-1); 
	  xyz(J,k,l,axis3)=zw(J1,kmax1-k,lmax-l-1); 
	}
      }

      xw.reshape(jdim*kdim*(lmax0+2));
      yw.reshape(jdim*kdim*(lmax0+2));
      zw.reshape(jdim*kdim*(lmax0+2));
    }
  }
  if( domainDimension==2 )
    xyz.reshape(jdim-2,gridLines,1,rangeDimension);
  
  // xyz.display("here is xyz");
  printf("domainDimension=%i, rangeDimension=%i \n",domainDimension,rangeDimension);
      
  dpm->setDataPoints(xyz,3,domainDimension);
  mappingHasChanged();
---- */

  return 0;
}


void HyperbolicMapping::
useRobustInverse(const bool trueOrFalse /* =true */ )
// =======================================================================================
// /Description:
//    Use the robust form of the inverse.
// =======================================================================================
{
  if( dpm!=NULL )
  {
    dpm->useRobustInverse(trueOrFalse);
    Mapping::useRobustInverse(trueOrFalse);
  }
  else
  {
    printf("HyperbolicMapping::useRobustInverse:WARNING: no dpm exists for this Mapping\n");
  }
}


void HyperbolicMapping::
map( const realArray & r, realArray & x, realArray & xr, MappingParameters & params )
{
  if( dpm==NULL )
  {

    cout << "HyperbolicMapping::map: WARNING: The hyperbolic mapping has not been defined yet!\n";
    Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

    assert( domainDimension>=2 && rangeDimension>=2 );
    Range R=rangeDimension, D=domainDimension;
    if( computeMap )
    {
      x(I,D)=r(I,D);
      if( domainDimension==2  && rangeDimension==3 )
        x(I,axis3)=r(I,0); // for a 3d surface z=r0
    }
    if( computeMapDerivative )
    {
      xr(I,R,D)=0.;
      for( int axis=0; axis<domainDimension; axis++ )
        xr(I,axis,axis)=1.;
      if( domainDimension==2  && rangeDimension==3 )
	xr(I,2,0)=1.;
    }
  }
  else 
  {
    // call DataPointMapping map function
    dpm->map(r,x,xr );
  }
  
}


void HyperbolicMapping::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
// ============================================================================================
// /Description:
//    Define a fast inverse.
// ===========================================================================================
{
  if( dpm==NULL )
  {
    cout << "HyperbolicMapping::basicInverse: Error: The mapping has not been defined yet!\n";
    exit(1);    
  }
  
  dpm->basicInverse(x,r,rx );

}

// ====================================================================================
/// \brief Here is the map function defined for serial arrays.
// ====================================================================================
void HyperbolicMapping::
mapS( const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
{
  if( dpm==NULL )
  {

    cout << "HyperbolicMapping::map: WARNING: The hyperbolic mapping has not been defined yet!\n";
    Index I = getIndex( r,x,xr,base,bound,computeMap,computeMapDerivative );

    assert( domainDimension>=2 && rangeDimension>=2 );
    Range R=rangeDimension, D=domainDimension;
    if( computeMap )
    {
      x(I,D)=r(I,D);
      if( domainDimension==2  && rangeDimension==3 )
        x(I,axis3)=r(I,0); // for a 3d surface z=r0
    }
    if( computeMapDerivative )
    {
      xr(I,R,D)=0.;
      for( int axis=0; axis<domainDimension; axis++ )
        xr(I,axis,axis)=1.;
      if( domainDimension==2  && rangeDimension==3 )
	xr(I,2,0)=1.;
    }
  }
  else 
  {
    // call DataPointMapping map function
    dpm->mapS(r,x,xr );
  }
  
}


// ============================================================================================
/// \brief Here is the inverse mapping function defined for serial ararys.
// ===========================================================================================
void HyperbolicMapping::
basicInverseS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  if( dpm==NULL )
  {
    printF("HyperbolicMapping::basicInverse: Error: The mapping has not been defined yet!\n");
    OV_ABORT("error");
  }
  
  dpm->basicInverseS(x,r,rx );

}




//=================================================================================
// get a mapping from the database
//=================================================================================
int HyperbolicMapping::
get( const GenericDataBase & dir, const aString & name)
{
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");
  if( debug & 4 )
    cout << "Entering HyperbolicMapping::get" << endl;

  subDir.get( HyperbolicMapping::className,"className" ); 
  if( HyperbolicMapping::className != "HyperbolicMapping" )
  {
    cout << "HyperbolicMapping::get ERROR in className!" << endl;
  }

  aString mappingClassName;

  subDir.get(growthOption,"growthOption");
  subDir.get(distance,"distance",2);
  subDir.get(linesToMarch,"linesToMarch",2);
  subDir.get(upwindDissipationCoefficient,"upwindDissipationCoefficient");
  subDir.get(uniformDissipationCoefficient,"uniformDissipationCoefficient");
  subDir.get(boundaryUniformDissipationCoefficient,"boundaryUniformDissipationCoefficient");
  subDir.get(dissipationTransition,"dissipationTransition");

  subDir.get(numberOfVolumeSmoothingIterations,"numberOfVolumeSmoothingIterations");
  subDir.get(curvatureSpeedCoefficient,"curvatureSpeedCoefficient");
  subDir.get(implicitCoefficient,"implicitCoefficient");
  subDir.get(removeNormalSmoothing,"removeNormalSmoothing");
  subDir.get(equidistributionWeight,"equidistributionWeight");
  subDir.get(targetGridSpacing,"targetGridSpacing");
  
  subDir.get(boundaryCondition,"boundaryCondition");
  subDir.get(ghostBoundaryCondition,"ghostBoundaryCondition");
  subDir.get(projectGhostPoints,"projectGhostPoints");
  subDir.get(indexRange,"indexRange");
  subDir.get(gridIndexRange,"gridIndexRange");
  subDir.get(dimension,"dimension");
//  subDir.get((int*)boundaryConditionMappingProjectionParameters[0],
//             "boundaryConditionMappingProjectionParameters",4);
//  subDir.get((int*)surfaceMappingProjectionParameters,"surfaceMappingProjectionParameters",3);
  subDir.get(numberOfLinesForNormalBlend[0],"numberOfLinesForNormalBlend",4);
//   printF("\n ***** HyperMap: get: numberOfLinesForNormalBlend=%i,%i,%i,%i\n\n",
// 	 numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][0],
// 	 numberOfLinesForNormalBlend[1][1]);

  subDir.get(splayFactor[0],"splayFactor",4);
  subDir.get(boundaryOffset[0],"boundaryOffset",6);
  subDir.get(boundaryOffsetWasApplied,"boundaryOffsetWasApplied");

  int temp;
  subDir.get(temp,"spacingType"); spacingType=(SpacingType)temp;
  subDir.get(temp,"spacingOption"); spacingOption=(SpacingOptionEnum)temp;
  subDir.get(geometricFactor,"geometricFactor");
  subDir.get(geometricNormalization,"geometricNormalization",2);
  subDir.get(initialSpacing,"initialSpacing");
  subDir.get(minimumGridSpacing,"minimumGridSpacing");
  subDir.get(arcLengthWeight,"arcLengthWeight");
  subDir.get(curvatureWeight,"curvatureWeight");
  subDir.get(normalCurvatureWeight,"normalCurvatureWeight");
  subDir.get(matchToMappingOrthogonalFactor,"matchToMappingOrthogonalFactor");
  subDir.get(surfaceGrid,"surfaceGrid");

  subDir.get(projectInitialCurve,"projectInitialCurve");
  subDir.get(applyBoundaryConditionsToStartCurve,"applyBoundaryConditionsToStartCurve");
  subDir.get(stopOnNegativeCells,"stopOnNegativeCells");
  subDir.get(saveReferenceSurface,"saveReferenceSurface");
  subDir.get(startCurveStart,"startCurveStart");
  subDir.get(startCurveEnd,"startCurveEnd");
  subDir.getDistributed(trailingEdgeDirection,"trailingEdgeDirection");
  subDir.get(smoothAndProject,"smoothAndProject");

  int exists=0;
  dpm=NULL;
  subDir.get(exists,"dpmExists");
  if( exists )
  {
    subDir.get(mappingClassName,"dpm.className");
    dpm = (DataPointMapping*)Mapping::makeMapping( mappingClassName );
    if( dpm == NULL )
    {
      printF("HyperbolicMapping::get:ERROR unable to make the mapping with className=%s\n",
	     (const char *)mappingClassName);
      OV_ABORT("error");
    }
    dpm->setPartition(partition);  // The dpm should get the same partition *wdh* 110820
    dpm->get(subDir,"dpm");
    dpm->incrementReferenceCount();
  }

  normalDistribution=NULL;
  subDir.get(exists,"normalDistributionExists");
  if( exists )
  {
    subDir.get(mappingClassName,"normalDistribution.className");
    normalDistribution = Mapping::makeMapping( mappingClassName );
    if( normalDistribution == NULL )
    {
      printF("HyperbolicMapping::get:ERROR unable to make the mapping with className=%s\n",
	     (const char *)mappingClassName);
      OV_ABORT("error");
    }
    normalDistribution->get(subDir,"normalDistribution");
    normalDistribution->incrementReferenceCount();
  }

  // get the Mappings used for boundary conditions.
  for( int axis=0; axis<=1; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( boundaryConditionMappingWasNewed[side][axis] )
        delete boundaryConditionMapping[side][axis];
      boundaryConditionMapping[side][axis]=NULL;
      boundaryConditionMappingWasNewed[side][axis]=false;
      
      int boundaryConditionMappingExists;
      subDir.get(boundaryConditionMappingExists,sPrintF("boundaryConditionMapping%i%iExists",side,axis));
      assert( boundaryConditionMappingExists==0 || boundaryConditionMappingExists==1 );
      if( boundaryConditionMappingExists )
      {
	//kkc 060707	subDir.get(mappingClassName,"curveClassName");
	subDir.get(mappingClassName, sPrintF("curveClassName%i%i",side,axis));
	boundaryConditionMapping[side][axis] = Mapping::makeMapping( mappingClassName );
        boundaryConditionMappingWasNewed[side][axis]=true;
	boundaryConditionMapping[side][axis]->get( subDir,sPrintF("boundaryConditionMapping%i%i",side,axis) );
      }
    }
  }

  subDir.get(exists,"surfaceExists");
  if( exists )
  {
    subDir.get(mappingClassName,"surface.className");
    surface = Mapping::makeMapping( mappingClassName );
    if( surface == NULL )
    {
      cout << "HyperbolicMapping::get:ERROR unable to make the mapping with className="
	   << (const char *)mappingClassName << endl;
      OV_ABORT("error");
    }
    surface->get(subDir,"surface");
    surface->incrementReferenceCount();
  }

  subDir.get(exists,"startCurveExists");
  if( exists )
  {
    subDir.get(mappingClassName,"startCurve.className");
    startCurve = Mapping::makeMapping( mappingClassName );
    if( startCurve == NULL )
    {
      cout << "HyperbolicMapping::get:ERROR unable to make the mapping with className="
	   << (const char *)mappingClassName << endl;
      OV_ABORT("error");
    }
    startCurve->get(subDir,"startCurve");
    startCurve->incrementReferenceCount();
  }

  Mapping::get( subDir, "Mapping" );

  // *wdh* 2011/10/01 -- temp fix -- put this here 
  mapIsDistributed=true;  
  inverseIsDistributed=true;


  mappingHasChanged();
  delete &subDir;

  if( debug & 8 )
    printF("HyperbolicMapping:get: name=%s, usesDistributedInverse=%i usesDistributedMap=%i\n",
	   (const char*)getName(mappingName),
           (int)usesDistributedInverse(),(int)usesDistributedMap());

  return 0;
}

int HyperbolicMapping::
put( GenericDataBase & dir, const aString & name) const
{  
  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                         // create a sub-directory 

  subDir.put( HyperbolicMapping::className,"className" );

  subDir.put(growthOption,"growthOption");
  subDir.put(distance,"distance",2);
  subDir.put(linesToMarch,"linesToMarch",2);
  subDir.put(upwindDissipationCoefficient,"upwindDissipationCoefficient");
  subDir.put(uniformDissipationCoefficient,"uniformDissipationCoefficient");
  subDir.put(boundaryUniformDissipationCoefficient,"boundaryUniformDissipationCoefficient");
  subDir.put(dissipationTransition,"dissipationTransition");

  subDir.put(numberOfVolumeSmoothingIterations,"numberOfVolumeSmoothingIterations");
  subDir.put(curvatureSpeedCoefficient,"curvatureSpeedCoefficient");
  subDir.put(implicitCoefficient,"implicitCoefficient");
  subDir.put(removeNormalSmoothing,"removeNormalSmoothing");
  subDir.put(equidistributionWeight,"equidistributionWeight");
  subDir.put(targetGridSpacing,"targetGridSpacing");

  subDir.put(boundaryCondition,"boundaryCondition");
  subDir.put(ghostBoundaryCondition,"ghostBoundaryCondition");
  subDir.put(projectGhostPoints,"projectGhostPoints");
  subDir.put(indexRange,"indexRange");
  subDir.put(gridIndexRange,"gridIndexRange");
  subDir.put(dimension,"dimension");
//  subDir.put((int*)boundaryConditionMappingProjectionParameters[0],
//             "boundaryConditionMappingProjectionParameters",4);
//  subDir.put((int*)surfaceMappingProjectionParameters,"surfaceMappingProjectionParameters",3);
  subDir.put(numberOfLinesForNormalBlend[0],"numberOfLinesForNormalBlend",4);
//   printF("\n ***** HyperMap: put: numberOfLinesForNormalBlend=%i,%i,%i,%i\n\n",
// 	 numberOfLinesForNormalBlend[0][0],numberOfLinesForNormalBlend[0][1],numberOfLinesForNormalBlend[1][0],
// 	 numberOfLinesForNormalBlend[1][1]);

  subDir.put(splayFactor[0],"splayFactor",4);
  subDir.put(boundaryOffset[0],"boundaryOffset",6);
  subDir.put(boundaryOffsetWasApplied,"boundaryOffsetWasApplied");

  subDir.put((int)spacingType,"spacingType");
  subDir.put((int)spacingOption,"spacingOption");
  subDir.put(geometricFactor,"geometricFactor");
  subDir.put(geometricNormalization,"geometricNormalization",2);
  subDir.put(initialSpacing,"initialSpacing");
  subDir.put(minimumGridSpacing,"minimumGridSpacing");
  subDir.put(arcLengthWeight,"arcLengthWeight");
  subDir.put(curvatureWeight,"curvatureWeight");
  subDir.put(normalCurvatureWeight,"normalCurvatureWeight");
  subDir.put(matchToMappingOrthogonalFactor,"matchToMappingOrthogonalFactor");
  subDir.put(surfaceGrid,"surfaceGrid");
  subDir.put(projectInitialCurve,"projectInitialCurve");
  subDir.put(applyBoundaryConditionsToStartCurve,"applyBoundaryConditionsToStartCurve");
  subDir.put(stopOnNegativeCells,"stopOnNegativeCells");
  subDir.put(saveReferenceSurface,"saveReferenceSurface");
  subDir.put(startCurveStart,"startCurveStart");
  subDir.put(startCurveEnd,"startCurveEnd");

  subDir.putDistributed(trailingEdgeDirection,"trailingEdgeDirection");
  subDir.put(smoothAndProject,"smoothAndProject");


  int exists=dpm!=NULL;
  subDir.put(exists,"dpmExists");
  if( exists )
  {
    subDir.put( dpm->getClassName(), "dpm.className");
    dpm->put(subDir,"dpm");
  }

  // save the normal distribution if it is defined.
  exists=normalDistribution!=NULL;
  subDir.put(exists,"normalDistributionExists");
  if( exists )
  {
    subDir.put( normalDistribution->getClassName(), "normalDistribution.className");
    normalDistribution->put(subDir,"normalDistribution");
  }

  // save the Mappings used for boundary conditions.
  // saveReferenceSurface:  // 0=do not save, 1=save for 2D grids, 2=save for all grids
  for( int axis=0; axis<=1; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      int boundaryConditionMappingExists= boundaryConditionMapping[side][axis]!=NULL  &&
          (saveReferenceSurface==2 || (saveReferenceSurface==1 && rangeDimension==2));
      subDir.put(boundaryConditionMappingExists,sPrintF("boundaryConditionMapping%i%iExists",side,axis));
      if( boundaryConditionMappingExists )
      {
	
	//kkc 060707	subDir.put(boundaryConditionMapping[side][axis]->getClassName(), "curveClassName");
	subDir.put(boundaryConditionMapping[side][axis]->getClassName(), sPrintF("curveClassName%i%i",side,axis));
	boundaryConditionMapping[side][axis]->put( subDir,sPrintF("boundaryConditionMapping%i%i",side,axis) );
      }
    }
  }
  
  exists=surface!=NULL && (saveReferenceSurface==2 || (saveReferenceSurface==1 && rangeDimension==2));
  subDir.put(exists,"surfaceExists");
  if( exists )
  {
    subDir.put( surface->getClassName(), "surface.className");
    surface->put(subDir,"surface");
  }
  exists=startCurve!=NULL && (saveReferenceSurface==2 || (saveReferenceSurface==1 && rangeDimension==2));
  subDir.put(exists,"startCurveExists");
  if( exists )
  {
    subDir.put( startCurve->getClassName(), "startCurve.className");
    startCurve->put(subDir,"startCurve");
  }
  

  Mapping::put( subDir, "Mapping" );
  delete &subDir;

  if( true || debug & 8 )
    printF("HyperbolicMapping:put: name=%s, usesDistributedInverse=%i usesDistributedMap=%i\n",
	   (const char*)getName(mappingName),
           (int)usesDistributedInverse(),(int)usesDistributedMap());


  return 0;
}

Mapping *HyperbolicMapping::
make( const aString & mappingClassName )
{ // Make a new mapping if the mappingClassName is the name of this Class
  Mapping *retval=0;
  if( mappingClassName==HyperbolicMapping::className )
    retval = new HyperbolicMapping();
  return retval;
}

