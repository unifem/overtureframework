#include "MappingProjectionParameters.h"


MappingProjectionParameters::
MappingProjectionParameters()
{
  marching=false;
  adjustForCorners=true;
  matchNormals=false;
  onlyChangePointsAdjustedForCorners=false;
  projectOntoTheReferenceSurface=true;
  
  searchBoundingBoxSize=0.;
  searchBoundingBoxMaximumSize=0.;

  int i;
  for( i=0; i<numberOfIntegerArrayParameters; i++ )
    integerArrayParameter[i]=NULL;
  for( i=0; i<numberOfRealArrayParameters; i++ )
    realArrayParameter[i]=NULL;
}

MappingProjectionParameters::
~MappingProjectionParameters()
{
  int i;
  for( i=0; i<numberOfIntegerArrayParameters; i++ )
    delete integerArrayParameter[i];
  for( i=0; i<numberOfRealArrayParameters; i++ )
    delete realArrayParameter[i];
}

MappingProjectionParameters & MappingProjectionParameters::
operator =( const MappingProjectionParameters & rhs )
{
  marching=rhs.marching;
  matchNormals=rhs.matchNormals;
  adjustForCorners=rhs.adjustForCorners;
  searchBoundingBoxSize=rhs.searchBoundingBoxSize;
  searchBoundingBoxMaximumSize=rhs.searchBoundingBoxMaximumSize;

  int i;
  for( i=0; i<numberOfIntegerArrayParameters; i++ )
  {
    if( rhs.integerArrayParameter[i]==NULL )
    {
      delete integerArrayParameter[i];
      integerArrayParameter[i]=NULL;
    }
    else
    {
      if( integerArrayParameter[i]==NULL )
        integerArrayParameter[i]=new intArray;
      else
	integerArrayParameter[i]->redim(0);
      *integerArrayParameter[i]=*rhs.integerArrayParameter[i]; // deep copy
    }
  }
  for( i=0; i<numberOfRealArrayParameters; i++ )
  {
    if( rhs.realArrayParameter[i]==NULL )
    {
      delete realArrayParameter[i];
      realArrayParameter[i]=NULL;
    }
    else
    {
      if( realArrayParameter[i]==NULL )
        realArrayParameter[i]=new realArray;
      else
	realArrayParameter[i]->redim(0);
      *realArrayParameter[i]=*rhs.realArrayParameter[i]; // deep copy
    }
  }

  return *this;
}

//! Return an array that represents some integer parameter.
/*!
    \param subSurfaceIndex(i) : number of the sub-surface of a CompositeSurface that a point lies on.
    \param ignoreThisSubSurface(i) : indicate that point i should ignore this sub-surface
    \param elementIndex(i) : the element index if projecting onto a triangulation (or a CompositeSurface with
                       a triangulation).
  */
IntegerDistributedArray & MappingProjectionParameters::
getIntArray(const IntegerArrayName & name)
{
  if( integerArrayParameter[name]==NULL )
    integerArrayParameter[name]=new intArray;
  return *integerArrayParameter[name];
}

//! Return an array that represents some real parameter.
/*!
    \param r : 
    \param x : 
    \param xr : 
    \param normal :
 */
RealDistributedArray & MappingProjectionParameters:: 
getRealArray(const RealArrayName & name)
{
  if( realArrayParameter[name]==NULL )
    realArrayParameter[name]=new realArray;
  return *realArrayParameter[name];
}

int MappingProjectionParameters:: 
setIsAMarchingAlgorithm(const bool & trueOrFalse /* =TRUE */ )
// ===============================================================================
// /Description:
//   Indicate whether the projection is being used with a marching algorithm.
//   If true then the previously projected points are assumed to be from the
//  previous marching step and the projection algorithm may do special things
//  so that it can move around corners in the geometry. In a marching algorithm
//  the CompositeSurface::project will prefer to move to a new subsurface.
//  If this is not a marching algorithm then the CompositeSurface::project
// will prefer to stay on the same sub-surface and to keep a continuous normal.
// ===============================================================================
{
  marching=trueOrFalse;
  return 0;
}

bool MappingProjectionParameters:: 
isAMarchingAlgorithm() const
{
  return marching;
}


int MappingProjectionParameters::
setSearchBoundingBoxSize( real estimated, real maximumAllowed /* = 0. */ )
// ==============================================================================
// /Description:
//  For tree search on unstructured grids we need to know a box in which to look
// around the given point. 
// /estimated (input) : try this size first (it may be increased if necessary)
// /maximumAllowed (input) : do not increase box size past this amount (in which case
//   no nearest point will be found). maximumAllowed==0. means there is no maximum.
// ==============================================================================
{
  searchBoundingBoxSize=estimated;
  searchBoundingBoxMaximumSize=maximumAllowed;
  return 0;
}


int MappingProjectionParameters:: 
setAdjustForCornersWhenMarching(const bool & trueOrFalse /* =TRUE */ )
// ===============================================================================
// /Description:
//   If true (by default) the marching alogrithm will attempt to adjust for
// corners (creases) in the surface.
// ===============================================================================
{
  adjustForCorners=trueOrFalse;
  return 0;
}

int MappingProjectionParameters::
setOnlyChangePointsAdjustedForCornersWhenMarching(const bool & trueOrFalse /*  =true */ )
// ===========================================================================================
// This option is used when first projecting onto the triangulation before
// projecting onto the CompositeSurface -- since we just want the subsurface info
// but do not want to change the positions of points, unless they were adjusted at corners.
// ===========================================================================================
{
  onlyChangePointsAdjustedForCorners=trueOrFalse;
  return 0;
}



//! If on a corner, choose the normal which best matches the input normal.
int MappingProjectionParameters::
setMatchNormals(const bool & trueOrFalse /* =true */ )
{
  matchNormals=trueOrFalse;
  return 0;
}

// ============================================================================================
// Project onto the reference surface (if false, use the surface triangulation if it exists)
// This option is normally only used if the Mapping is a CompositeSurface.
// ============================================================================================
int MappingProjectionParameters::
setProjectOntoReferenceSurface(const bool & trueOrFalse /* =true */ )
{
  projectOntoTheReferenceSurface=trueOrFalse;
  return 0;
}



int MappingProjectionParameters::
reset()
// ==============================================================================
// /Description:
//    Reset the parameters.
// ==============================================================================
{
  int i;
  for( i=0; i<numberOfIntegerArrayParameters; i++ )
    if( integerArrayParameter[i]!=NULL )
      integerArrayParameter[i]->redim(0);
  for( i=0; i<numberOfRealArrayParameters; i++ )
    if( realArrayParameter[i]!=NULL )
      realArrayParameter[i]->redim(0);

  matchNormals=false;
  return 0;
}
