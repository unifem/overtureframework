//
// Who to blame:  Geoff Chesshire and Bill Henshaw
//

#include "MappedGrid.h"
#include "LineMapping.h"
#include "SquareMapping.h"
#include "BoxMapping.h"
#include "ReparameterizationTransform.h"
#include "UnstructuredMapping.h"
#include <algorithm>
#include "ParallelUtility.h"
#include "App.h"

#ifdef USE_STL
#include "RCVector.h"
RCVector_STATIC_MEMBER_DATA(MappedGrid)
#endif // USE_STL


// int MappedGrid::minimumNumberOfDistributedGhostLines=0;


//
// class MappedGrid:
//
// Public member functions:
//
// Default constructor.
//
// If numberOfDimensions_==0 (e.g., by default) then create a null
// MappedGrid.  Otherwise, create a MappedGrid with the given
// number of dimensions.
//
MappedGrid::
MappedGrid(const Integer numberOfDimensions_) :   GenericGrid() 
{
  className = "MappedGrid";
  if( false )
  {
    rcData=NULL;
    isCounted=false;
  }
  else
  {
    rcData = new MappedGridData(numberOfDimensions_);
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    updateReferences();
  }
  parentChildSiblingInfo = NULL;
}

// *wdh* This function could be used to delay the creation of the rcData.
void MappedGrid::
init(int numberOfDimensions_)
{
  assert( rcData==NULL );
  rcData = new MappedGridData(numberOfDimensions_);
  isCounted = LogicalTrue;
  rcData->incrementReferenceCount();
  updateReferences();
}

//
// Copy constructor.  (Does a deep copy by default.)
//
MappedGrid::MappedGrid(
  const MappedGrid& x,
  const CopyType    ct):
  GenericGrid(x, ct) {
  className = "MappedGrid";
  switch (ct) {
  case DEEP:
  case NOCOPY:
    rcData = (MappedGridData*)
      ((ReferenceCounting*)x.rcData)->virtualConstructor(ct);
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    break;
  case SHALLOW:
    rcData = x.rcData;
    isCounted = x.isCounted;
    if (isCounted) rcData->incrementReferenceCount();
    break;
  } // end switch
  updateReferences();
  parentChildSiblingInfo = x.parentChildSiblingInfo;
}
//
// Constructor from a mapping.
//
MappedGrid::MappedGrid(Mapping& x):
  GenericGrid() {
  className = "MappedGrid";
  rcData = new MappedGridData;
  isCounted = LogicalTrue;
  rcData->incrementReferenceCount();

  rcData->gridType=x.getClassName()=="UnstructuredMapping" ? unstructuredGrid : structuredGrid;

  updateReferences();
  parentChildSiblingInfo = NULL;
  reference(x);
}
MappedGrid::
MappedGrid(MappingRC& x) 
{
  className = "MappedGrid";
  rcData = new MappedGridData;
  isCounted = LogicalTrue;
  rcData->incrementReferenceCount();
  rcData->gridType=x.getClassName()=="UnstructuredMapping" ? unstructuredGrid : structuredGrid;
  updateReferences();
  parentChildSiblingInfo = NULL;
  reference(x);
}
//
// Destructor.
//
MappedGrid::
~MappedGrid()
{ 
  if (isCounted && rcData->decrementReferenceCount() == 0) delete rcData;
}

//
// Assignment operator.  (Does a deep copy.)
//
MappedGrid& MappedGrid::
operator=(const MappedGrid& x) 
{
  //  GenericGrid::operator=(x);
  if( rcData==NULL && x.rcData!=NULL ) 
    init(x.numberOfDimensions());
  
  if (rcData != x.rcData) 
  {
    if (rcData->getClassName() == x.rcData->getClassName()) 
    {
      // (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
      rcData->equals(*x.rcData);
      
      updateReferences();
    }
    else 
    {
      MappedGrid& y = *(MappedGrid*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
  } // end if
  parentChildSiblingInfo = x.parentChildSiblingInfo;
  return *this;
}


// ====================================================================================
// /Description:
//    Equals operator plus options. This version is used when copying a GridCollection
// that has AMR grids -- in which case we do not want to make a deep copy of the Mapping.
//
//   /options (input): (options & 2)==1 : do NOT copy the mapping 
// ====================================================================================
MappedGrid& MappedGrid::
equals(const MappedGrid& x, int option /* =0 */ ) 
{
//  GenericGrid::operator=(x);
  if( rcData==NULL && x.rcData!=NULL ) 
    init(x.numberOfDimensions());
  
  if (rcData != x.rcData) 
  {
    if (rcData->getClassName() == x.rcData->getClassName()) 
    {
      // (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
      rcData->equals(*x.rcData,option);
      
      updateReferences();
    }
    else 
    {
      MappedGrid& y = *(MappedGrid*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
  } // end if
  parentChildSiblingInfo = x.parentChildSiblingInfo;
  return *this;
}

//
// Make a reference.  (Does a shallow copy.)
//
void MappedGrid::
reference(const MappedGrid& x)
{
  GenericGrid::reference(x);
  if (rcData != x.rcData) 
  {
    if(isCounted && rcData->decrementReferenceCount() == 0)
      delete rcData;
    rcData = x.rcData;
    isCounted = x.isCounted;
    if (isCounted) rcData->incrementReferenceCount();
    updateReferences();
  } // end if
  parentChildSiblingInfo = x.parentChildSiblingInfo;
}

void MappedGrid::
reference(MappedGridData& x) 
{
  GenericGrid::reference(x);
  if( rcData != &x) 
  {
    if (isCounted && rcData->decrementReferenceCount() == 0)
      delete rcData;
    rcData = &x;
    isCounted = !x.uncountedReferencesMayExist();
    if (isCounted) rcData->incrementReferenceCount();
    updateReferences();
  } // end if
}

MappedGrid::BoundaryFlagEnum MappedGrid::
boundaryFlag(int side, int axis ) const
{
  return (BoundaryFlagEnum)rcData->boundaryFlag[side][axis]; 
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{setMapping}}
void MappedGrid::
setMapping(Mapping& x)
// ==========================================================================
// /Description:
//     Use a given mapping.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  reference(x);
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{setMapping}}
void MappedGrid::
setMapping(MappingRC& x)
// ==========================================================================
// /Description:
//     Use a given mapping.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  reference(x);
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{setMinimumNumberOfDistributedGhostLines}}
void MappedGrid::
setMinimumNumberOfDistributedGhostLines( int numGhost )
// ==========================================================================
// /Description:
// On Parallel machines always add at least this many ghost lines on the arrays
// that are local to each processor.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  minimumNumberOfDistributedGhostLines=numGhost;
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{getMinimumNumberOfDistributedGhostLines}}
int MappedGrid::
getMinimumNumberOfDistributedGhostLines()
// ==========================================================================
// /Description:
// On Parallel machines we always add at least this many ghost lines on the arrays
// that are local to each processor.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  return minimumNumberOfDistributedGhostLines;
}


void MappedGrid::
setNumberOfDimensions(const Integer& numberOfDimensions_) 
{
  assert( rcData!=NULL );
  if (numberOfDimensions() != numberOfDimensions_) destroy(~NOTHING);
  rcData->numberOfDimensions = numberOfDimensions_;
}

void MappedGrid::
setBoundaryCondition( const Integer& ks, const Integer& kd, const Integer& boundaryCondition_)
{
  rcData->boundaryCondition(ks,kd) = boundaryCondition_; 

  // alter the boundaryFlag if appropriate
  if( rcData->boundaryCondition(ks,kd)<=0 || 
      rcData->boundaryFlag[ks][kd]!=MappedGridData::mixedPhysicalInterpolationBoundary )
  {
    rcData->boundaryFlag[ks][kd]=rcData->boundaryCondition(ks,kd) > 0 ?  MappedGridData::physicalBoundary :
      rcData->boundaryCondition(ks,kd)==0 ? MappedGridData::interpolationBoundary :
      mapping().getIsPeriodic(kd)==Mapping::functionPeriodic ? MappedGridData::branchCutPeriodicBoundary : 
      MappedGridData::periodicBoundary;
  }
}
    
void  MappedGrid::
setBoundaryFlag( int side, int axis, MappedGridData::BoundaryFlagEnum bc )
// *** internal use only for now ***
{
  rcData->boundaryFlag[side][axis]=bc;
}



void MappedGrid:: 
setBoundaryDiscretizationWidth(
  const Integer& ks,
  const Integer& kd,
  const Integer& boundaryDiscretizationWidth_)
{ rcData->boundaryDiscretizationWidth(ks,kd) = boundaryDiscretizationWidth_; }

void MappedGrid:: 
setIsCellCentered(
  const Integer& kd,
  const Logical& isCellCentered_) {
  if (isCellCentered(kd) != isCellCentered_) destroy(~NOTHING);
  rcData->isCellCentered(kd) = isCellCentered_;
}

void MappedGrid:: 
setDiscretizationWidth(
  const Integer& kd,
  const Integer& discretizationWidth_)
{ rcData->discretizationWidth(kd) = discretizationWidth_; }

void MappedGrid:: 
setGridIndexRange(
  const Integer& ks,
  const Integer& kd,
  const Integer& gridIndexRange_)
{
  if (gridIndexRange(ks,kd) != gridIndexRange_) destroy(~NOTHING);
  rcData->gridIndexRange(ks,kd) = gridIndexRange_;
}

void MappedGrid:: 
setNumberOfGhostPoints(
  const Integer& ks,
  const Integer& kd,
  const Integer& numberOfGhostPoints_)
{
  if (numberOfGhostPoints(ks,kd) != numberOfGhostPoints_)
    destroy(~NOTHING);
  rcData->numberOfGhostPoints(ks,kd) = numberOfGhostPoints_;
}

void MappedGrid::
setUseGhostPoints(const Logical& useGhostPoints_) 
{
  if (useGhostPoints() != useGhostPoints_) destroy(~NOTHING);
  rcData->useGhostPoints = useGhostPoints_;
}

void MappedGrid::
setIsPeriodic( const Integer& axis, const Mapping::periodicType& isPeriodic_) 
{
  if (isPeriodic(axis) != (int)isPeriodic_) destroy(~NOTHING);
  rcData->isPeriodic(axis) = isPeriodic_;

  if( isPeriodic_==Mapping::functionPeriodic )
    rcData->boundaryFlag[0][axis]=rcData->boundaryFlag[1][axis]=MappedGridData::branchCutPeriodicBoundary;
  else if( isPeriodic_==Mapping::derivativePeriodic )
    rcData->boundaryFlag[0][axis]=rcData->boundaryFlag[1][axis]=MappedGridData::periodicBoundary;
  else if(  isPeriodic_==Mapping::notPeriodic )
  {
    for( int side=Start; side<=End; side++ )
    {
      if( rcData->boundaryFlag[side][axis]==MappedGridData::branchCutPeriodicBoundary ||
	  rcData->boundaryFlag[side][axis]==MappedGridData::periodicBoundary )
	rcData->boundaryFlag[side][axis]=MappedGridData::physicalBoundary;
    }
  }
  
}

void MappedGrid:: 
setSharedBoundaryFlag(
  const Integer& ks,
  const Integer& kd,
  const Integer& sharedBoundaryFlag_)
{ rcData->sharedBoundaryFlag(ks,kd) = sharedBoundaryFlag_; }

void 
MappedGrid::setSharedBoundaryTolerance(
  const Integer& ks,
  const Integer& kd,
  const Real&    sharedBoundaryTolerance_)
{ rcData->sharedBoundaryTolerance(ks,kd) = sharedBoundaryTolerance_; }




//
// Use a given mapping.
//
void MappedGrid::
reference(Mapping& x, bool forceIncompatible/*=false*/ ) 
{ //kkc 050120 added forceIncompatible for updating the grid when the mapping has changed significantly

  rcData->gridType=x.getClassName()=="UnstructuredMapping" ? unstructuredGrid : structuredGrid;

  Logical incompatible = x.getRangeDimension()!=mapping().getRangeDimension() || forceIncompatible;
  incompatible = x.getClassName()!=mapping().getClassName(); // *wdh* 
  Integer kd, ks;
  for (kd=0; kd<mapping().getRangeDimension(); kd++) 
  {
    if( ( incompatible =   (incompatible                                    ||
			    x.getGridDimensions(kd)       != mapping().getGridDimensions(kd)    ||
			    x.getIsPeriodic(kd)           != mapping().getIsPeriodic(kd) )) ) break;
    for (ks=0; ks<2; ks++) 
    {
      if ( (incompatible = (incompatible               ||
			    x.getBoundaryCondition(ks,kd)!= mapping().getBoundaryCondition(ks,kd)||
			    mapping().getShare(ks,kd)      != x.getShare(ks,kd) ) ) ) break;
    }
    
  } // end for, end if
  mapping().reference(x);
  if (incompatible) 
  {
//      Fill out new dimensions with default data.
    for (kd=numberOfDimensions(); kd<mapping().getRangeDimension(); kd++) 
    {
      rcData->isCellCentered(kd) = LogicalFalse;
      rcData->discretizationWidth(kd) = 3;
      for (ks=0; ks<2; ks++) 
      {
	rcData->boundaryDiscretizationWidth(ks, kd) = 3;
	rcData->sharedBoundaryTolerance(ks, kd) = (Real).1;
	rcData->numberOfGhostPoints(ks,kd)=rcData->gridType==structuredGrid ? (discretizationWidth(kd)-1)/2 : 0;
      } // end for
    } // end for

    rcData->useGhostPoints = LogicalTrue;
    rcData->numberOfDimensions = mapping().getRangeDimension();

    // *wdh* 070301 -- for surface grids we allow share and bc for the extra axis
    for (kd=0; kd<numberOfDimensions(); kd++) 
    {
      for (ks=0; ks<2; ks++) 
      {
	rcData->boundaryCondition(ks,kd) =   mapping().getBoundaryCondition(ks,kd);
	rcData->sharedBoundaryFlag(ks, kd) = mapping().getShare(ks,kd);
      } // end for
    } // end for

    // *kkc --changed to accomodate surface grids    for (kd=0; kd<numberOfDimensions(); kd++) 
    for (kd=0; kd<domainDimension(); kd++) 
    {
      rcData->isPeriodic(kd) = mapping().getIsPeriodic(kd);
      for (ks=0; ks<2; ks++) 
      {
        rcData->boundaryFlag[ks][kd]=(rcData->boundaryCondition(ks,kd) > 0 ? MappedGridData::physicalBoundary :
				      rcData->boundaryCondition(ks,kd)==0 ? MappedGridData::interpolationBoundary :
				      mapping().getIsPeriodic(kd)==Mapping::functionPeriodic ? 
				      MappedGridData::branchCutPeriodicBoundary : MappedGridData::periodicBoundary); 
	rcData->gridIndexRange(ks, kd) = ks * (mapping().getGridDimensions(kd) - 1);
      } // end for
    } // end for


    // *kkc --changed to accomodate surface grids   for (kd=numberOfDimensions(); kd<3; kd++) 
    for (kd=domainDimension(); kd<3; kd++) 
    {
      rcData->isCellCentered(kd) = LogicalFalse;
      rcData->discretizationWidth(kd) = 1;
      rcData->isPeriodic(kd) = Mapping::derivativePeriodic;
      for (ks=0; ks<2; ks++) 
      {
        // rcData->boundaryCondition(ks, kd) = -1;
        rcData->boundaryFlag[ks][kd]=MappedGridData::periodicBoundary; 

	rcData->boundaryDiscretizationWidth(ks, kd) = 1;
        // rcData->sharedBoundaryFlag(ks, kd) = 0;
	rcData->sharedBoundaryTolerance(ks, kd) = (Real)0.;
	rcData->gridIndexRange(ks, kd) = 0;
	rcData->numberOfGhostPoints(ks, kd) = 0;
      } // end for
    } // end for
    // 104024 rcData->minimumEdgeLength = rcData->maximumEdgeLength = (Real)0.;
    destroy(~NOTHING); update(NOTHING);
  } 
  else 
  {
    geometryHasChanged(~THEmask);
  } // end if
}

void MappedGrid::
reference(MappingRC& x) 
{ 
  reference(*x.mapPointer); 
}
//
// Break a reference.  (Replaces with a deep copy.)
//
void MappedGrid::breakReference()
{
//  GenericGrid::breakReference();
    if (!isCounted || rcData->getReferenceCount() != 1) {
        MappedGrid x = *this; // Uses the (deep) copy constructor.
        reference(x);
    } // end if
}
//
// Change the grid to be all vertex-centered.
//
void MappedGrid::changeToAllVertexCentered() {
    if (!isAllVertexCentered()) {
        const Integer newGeometry = isAllVertexCentered() ?
          NOTHING : computedGeometry() & (
          THEinverseCenterDerivative |
          THEcenter                  |
          THEcorner                  |
          THEcenterDerivative        |
          THEcenterJacobian          |
          THEcellVolume              |
          THEcenterNormal            |
          THEcenterArea              |
          THEfaceNormal              |
          THEfaceArea                |
          THEcenterBoundaryNormal    |
          THEcenterBoundaryTangent   );
        for (Integer kd=0; kd<numberOfDimensions(); kd++)
          rcData->isCellCentered(kd) = LogicalFalse;
//      Force links between vertex and center data.
        if (newGeometry) destroy(newGeometry);
        update(newGeometry);
    } // end if
}
//
// Change the grid to be all cell-centered.
//
void MappedGrid::
changeToAllCellCentered() 
{
  if (!isAllCellCentered())
  {
    const Integer newGeometry = isAllCellCentered() ?
      NOTHING : computedGeometry() & (
	THEinverseCenterDerivative |
	THEcenter                  |
	THEcorner                  |
	THEcenterDerivative        |
	THEcenterJacobian          |
	THEcellVolume              |
	THEcenterNormal            |
	THEcenterArea              |
	THEfaceNormal              |
	THEfaceArea                |
	THEcenterBoundaryNormal    |
	THEcenterBoundaryTangent   );
    for (Integer kd=0; kd<numberOfDimensions(); kd++)
      rcData->isCellCentered(kd) = LogicalTrue;
//      Force links between vertex and corner data.
    if (newGeometry) destroy(newGeometry);
    update(newGeometry);
  } // end if
}
//
// Check that the data structure is self-consistent.
//
void MappedGrid::
consistencyCheck() const {
    GenericGrid::consistencyCheck();
    if (rcData != GenericGrid::rcData) {
        cerr << className << "::consistencyCheck():  "
             << "rcData != GenericGrid::rcData for "
             << getClassName() << " " << getGlobalID() << "." << endl;
        assert(rcData == GenericGrid::rcData);
    } // end if
}


//\begin{>>MappedGridInclude.tex}{\subsubsection{deltaX}}
int MappedGrid::
getDeltaX( Real dx[3] ) const
// ==========================================================================
// /Description:
//   Return dx for rectangular grids, otherwise return dr
// /dx (output) : the grid spacing for a rectangular grid.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  real xab[2][3];
  return getRectangularGridParameters( dx,xab );
}


//!  Get delta x and corners of a rectangular grid.
/*! 
  \param dx : 
  \param xab : xab[side][axis] are the corners of the rectangular grid.
 */
int MappedGrid:: 
getRectangularGridParameters( Real dx[3], Real xab[2][3] ) const
{
  real xa=0.,xb=1., ya=0.,yb=1., za=0., zb=1.;
  real ra=0.,rb=1., sa=0.,sb=1., ta=0., tb=1.;  // if reparameterized
  bool reparameterized=false;
  if( isRectangular() )
  {
    Mapping *mapPointer=mapping().mapPointer;

    aString mapClassName = mapPointer->getClassName();
    if( mapClassName=="ReparameterizationTransform" )
    {
      reparameterized=true;
      ReparameterizationTransform & rt = (ReparameterizationTransform&)(*mapPointer);
      // rt.getBounds(ra,rb,sa,sb,ta,tb ); // *wdh* 070415
      rt.getBoundsForMultipleReparameterizations(ra,rb,sa,sb,ta,tb );

      // get the Mapping that was reparameterized
      mapPointer= rt.map2.mapPointer;
      mapClassName = mapPointer->getClassName();
    }
    
    Mapping & map = *mapPointer;

    if( numberOfDimensions()==1 && mapClassName=="LineMapping" )
    {
      LineMapping & sq = (LineMapping&) map;
      sq.getPoints( xa,xb );
    }
    else if( numberOfDimensions()==2 && mapClassName=="SquareMapping" )
    {
      SquareMapping & sq = (SquareMapping&) map;
      sq.getVertices( xa,xb,ya,yb );
    }
    else if(  numberOfDimensions()==3 && mapClassName=="BoxMapping" )
    {
      BoxMapping & box = (BoxMapping&) map;
      box.getVertices( xa,xb,ya,yb,za,zb);
    }
    else
    {
      printf("MappedGrid::deltaX:ERROR: mapping is rectangular but of unknown className=%s\n",
	     (const char*)map.getClassName());
      Overture::abort("error");
    }
  }
  if( !reparameterized )
  {
    xab[0][0]=xa;
    xab[1][0]=xb;
    xab[0][1]=ya;
    xab[1][1]=yb;
    xab[0][2]=za;
    xab[1][2]=zb;
    

    dx[0]=(xb-xa)*gridSpacing(0);
    dx[1]=(yb-ya)*gridSpacing(1);
    dx[2]=(zb-za)*gridSpacing(2);
  }
  else
  {
    xab[0][0]=xa+ra*(xb-xa);
    xab[1][0]=xa+rb*(xb-xa);
    xab[0][1]=ya+sa*(yb-ya);
    xab[1][1]=ya+sb*(yb-ya);
    xab[0][2]=za+ta*(zb-za);
    xab[1][2]=za+tb*(zb-za);
    
    dx[0]=(xb-xa)*(rb-ra)*gridSpacing(0);
    dx[1]=(yb-ya)*(sb-sa)*gridSpacing(1);
    dx[2]=(zb-za)*(tb-ta)*gridSpacing(2);
  }
  
  return 0;
}



//\begin{>>MappedGridInclude.tex}{\subsubsection{extendedRange}}
IntegerArray  MappedGrid::
extendedRange() const
// ==========================================================================
// /Description:
//   Return the extendedRange : index range plus extra lines for interpolation,
//  including interpolation outside of mixed boundaries.
//\end{MappedGridInclude.tex}
//==========================================================================
{
#undef extendedRange
#define extendedRange(side,axis) pExtendedRange[(side)+2*(axis)]

  IntegerArray range(2,3);
  for( int axis=0; axis<3; axis++ ) 
    for( int side=0; side<=1; side++) 
      range(side,axis)=rcData->extendedRange(side,axis);
  return range;
}

  


//\begin{>>MappedGridInclude.tex}{\subsubsection{getName}}
aString MappedGrid::
getName() const
// ==========================================================================
// /Description:
//   Get the name of the grid.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  return mapping().getName(Mapping::mappingName);
}



//\begin{>>MappedGridInclude.tex}{\subsubsection{isRectangular}}
bool MappedGrid::
isRectangular() const
// ==========================================================================
// /Description:
//    Return true if the the grid rectangular.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  if( mapping().mapPointer!=NULL )
  { 
    // MappedGrid has a mapping, check the type
    return mapping().mapPointer->getMappingCoordinateSystem()==Mapping::rectangular;
  }
  return FALSE;
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{displayComputedGeometry}}
int  MappedGrid::
displayComputedGeometry(FILE *file /* =stdout */ ) const
// ==========================================================================
// /Description:
//  Show which geometry arrays are built
//\end{MappedGridInclude.tex}
//==========================================================================
{
  fPrintF(file,"---Computed geometry for %s:\n"
	  "  mask=%s,                 inverseVertexDerivative=%s, inverseCenterDerivative=%s,\n"
	  "  vertex=%s,               center=%s,                  corner=%s, \n"
          "  vertexDerivative=%s,     centerDerivative=%s,        vertexJacobian=%s,  \n"
          "  centerJacobian=%s,       cellVolume=%s,              centerNormal=%s \n"
	  "  centerArea=%s,           faceNormal=%s,              faceArea=%s, \n"
          "  vertexBoundaryNormal=%s, centerBoundaryNormal=%s,    centerBoundaryTangent=%s \n",
          (const char *)getName(),
	  (computedGeometry() & THEmask ? "yes" : "no "),
	  (computedGeometry() & THEinverseVertexDerivative ? "yes" : "no "),
	  (computedGeometry() & THEinverseCenterDerivative ? "yes" : "no "),
	  (computedGeometry() & THEvertex ? "yes" : "no "),
	  (computedGeometry() & THEcenter ? "yes" : "no "),
	  (computedGeometry() & THEcorner ? "yes" : "no "),
	  (computedGeometry() & THEvertexDerivative ? "yes" : "no "),
	  (computedGeometry() & THEcenterDerivative ? "yes" : "no "),
	  (computedGeometry() & THEvertexJacobian ? "yes" : "no "),
	  (computedGeometry() & THEcenterJacobian ? "yes" : "no "),
	  (computedGeometry() & THEcellVolume ? "yes" : "no "),
	  (computedGeometry() & THEcenterNormal ? "yes" : "no "),
	  (computedGeometry() & THEcenterArea ? "yes" : "no "),
	  (computedGeometry() & THEfaceNormal ? "yes" : "no "),
	  (computedGeometry() & THEfaceArea ? "yes" : "no "),
	  (computedGeometry() & THEvertexBoundaryNormal ? "yes" : "no "),
	  (computedGeometry() & THEcenterBoundaryNormal ? "yes" : "no "),
	  (computedGeometry() & THEcenterBoundaryTangent ? "yes" : "no "));

  return 0;
}

//\begin{>>MappedGridInclude.tex}{\subsubsection{sizeOf}}
real MappedGrid::
sizeOf(FILE *file /* = NULL */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by Oges; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /Return value: the number of bytes.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  assert( rcData!=NULL );
  
  real size=sizeof(*this) + sizeof( *rcData );
  if( rcData->mask !=NULL ) size+=rcData->mask->sizeOf();

  if( rcData->inverseCenterDerivative!=NULL ) 
    size+=rcData->inverseCenterDerivative->sizeOf();

  if( rcData->inverseVertexDerivative!=NULL && 
      (rcData->inverseCenterDerivative==NULL || isAllCellCentered()) )  // could be ref. to inverseCenterDerivative
    size+=rcData->inverseVertexDerivative->sizeOf();

  if( rcData->center!=NULL ) size+=rcData->center->sizeOf();
  if( rcData->vertex!=NULL  && (rcData->center==NULL || isAllCellCentered()) )
     size+=rcData->vertex->sizeOf();
  if( rcData->corner!=NULL  && 
        ( (rcData->vertex==NULL && isAllCellCentered()) || isAllVertexCentered()) )
     size+=rcData->corner->sizeOf();

  if( rcData->centerDerivative!=NULL ) size+=rcData->centerDerivative->sizeOf();
  if( rcData->vertexDerivative!=NULL  && (rcData->centerDerivative==NULL || isAllCellCentered()) )
     size+=rcData->vertexDerivative->sizeOf();

  if( rcData->centerJacobian!=NULL ) size+=rcData->centerJacobian->sizeOf();
  if( rcData->vertexJacobian!=NULL  && (rcData->centerJacobian==NULL || isAllCellCentered()) )
     size+=rcData->vertexJacobian->sizeOf();

  if( rcData->cellVolume!=NULL ) size+=rcData->cellVolume->sizeOf();
  if( rcData->centerNormal!=NULL ) size+=rcData->centerNormal->sizeOf();
  if( rcData->centerArea!=NULL ) size+=rcData->centerArea->sizeOf();
  if( rcData->faceNormal!=NULL ) size+=rcData->faceNormal->sizeOf();
  if( rcData->faceArea!=NULL ) size+=rcData->faceArea->sizeOf();

  for( int axis=0; axis<3; axis++ )
  {
    for( int side=0; side<=1; side++ )
    {
      if( rcData->centerBoundaryNormal[axis][side]!=NULL ) size+=rcData->centerBoundaryNormal[axis][side]->sizeOf();
      if( rcData->vertexBoundaryNormal[axis][side]!=NULL &&
          (rcData->centerBoundaryNormal[axis][side]==NULL || isAllCellCentered() ) )
        size+=rcData->vertexBoundaryNormal[axis][side]->sizeOf();
      if( rcData->centerBoundaryTangent[axis][side]!=NULL ) size+=rcData->centerBoundaryTangent[axis][side]->sizeOf();

      if( rcData->pCenterBoundaryNormal[axis][side]!=NULL ) 
        size+=rcData->pCenterBoundaryNormal[axis][side]->elementCount()*sizeof(real);
      if( rcData->pVertexBoundaryNormal[axis][side]!=NULL && 
         (rcData->pCenterBoundaryNormal[axis][side]==NULL || isAllCellCentered() ) )
        size+=rcData->pVertexBoundaryNormal[axis][side]->elementCount()*sizeof(real);
      if( rcData->pCenterBoundaryTangent[axis][side]!=NULL ) 
        size+=rcData->pCenterBoundaryTangent[axis][side]->elementCount()*sizeof(real);
    }
  }
    
  size+=(rcData->I1array.elementCount()+rcData->I2array.elementCount()+rcData->I3array.elementCount())*sizeof(int);
  size+=mapping().sizeOf();

  size+=rcData->boundaryCondition.elementCount()*sizeof(int); 
  size+=rcData->boundaryDiscretizationWidth.elementCount()*sizeof(int);
  size+=rcData->boundingBox.elementCount()*sizeof(real);
  size+=rcData->localBoundingBox.elementCount()*sizeof(real);
  size+=rcData->gridSpacing.elementCount()*sizeof(real);
  size+=rcData->isCellCentered.elementCount()*sizeof(int);
  size+=rcData->discretizationWidth.elementCount()*sizeof(int);
  size+=rcData->indexRange.elementCount()*sizeof(int);
  size+=rcData->extendedIndexRange.elementCount()*sizeof(int);
  size+=rcData->gridIndexRange.elementCount()*sizeof(int);
  size+=rcData->dimension.elementCount()*sizeof(int);
  size+=rcData->numberOfGhostPoints.elementCount()*sizeof(int);
  size+=rcData->isPeriodic.elementCount()*sizeof(int);
  size+=rcData->sharedBoundaryFlag.elementCount()*sizeof(int);
  size+=rcData->sharedBoundaryTolerance.elementCount()*sizeof(real);
  // 104024 size+=rcData->minimumEdgeLength.elementCount()*sizeof(real);
  // 104024 size+=rcData->maximumEdgeLength.elementCount()*sizeof(real);

  return size;
}



//
// "Get" and "put" database operations.
//
Integer MappedGrid::
get(const GenericDataBase& db,
    const aString&         name,
    bool getMapping /* =true */ )   // for AMR grids we may not get the mapping.
{
  Integer returnValue = rcData->get(db, name, getMapping);
  updateReferences();
  return returnValue;
}
Integer MappedGrid::
put(
  GenericDataBase& db,
  const aString&   name,
  bool putMapping /* = true */,
  int geometryToPut /* = -1  */
 ) const
// geometryToPut : by default put computedGeometry
{ 
  return rcData->put(db, name,putMapping,geometryToPut); 
}
//
// Set references to reference-counted data.
//
void MappedGrid::updateReferences(const Integer what)
{
  GenericGrid::reference(*rcData);
  GenericGrid::updateReferences(what);
}

//
// Update the grid, sharing the data of another grid.
//
Integer MappedGrid::
update(
  GenericGrid&  x,
  const Integer what,
  const Integer how) 
{
  Integer upd = rcData->update(*((MappedGrid&)x).rcData, what, how);
    
  updateReferences(what);
  updateMappedGridPointers(what);
  return upd;
}

Integer MappedGrid::
updateMappedGridPointers(const Integer what_)
//
// This routine assigns the "mappedGrid" pointer in the geometry grid functions
// that are being updated. This is not done by the update since it only knows
// the rcData pointer and not the MappedGrid pointer.
{
  assert( rcData!=NULL );
  
  int what=rcData->getWhatForGrid(what_);

  if( what & THEmask )
    rcData->mask->mappedGrid=this;

  if( what & THEinverseVertexDerivative )
    rcData->inverseVertexDerivative->mappedGrid=this;

  if( what & THEinverseCenterDerivative )
    rcData->inverseCenterDerivative->mappedGrid=this;

  if( what & THEvertex )
    rcData->vertex->mappedGrid=this;

  if( what & THEcenter )
    rcData->center->mappedGrid=this;

  if( what & THEcorner )
    rcData->corner->mappedGrid=this;

  if( what & THEvertexDerivative )
    rcData->vertexDerivative->mappedGrid=this;

  if( what & THEcenterDerivative )
    rcData->centerDerivative->mappedGrid=this;

  if( what & THEvertexJacobian )
    rcData->vertexJacobian->mappedGrid=this;

  if( what & THEcenterJacobian )
    rcData->centerJacobian->mappedGrid=this;

  if( what & THEcellVolume )
    rcData->cellVolume->mappedGrid=this;

  if( what & THEcenterArea )
    rcData->centerArea->mappedGrid=this;

  if( what & THEfaceNormal )
    rcData->faceNormal->mappedGrid=this;

  if( what & THEvertexBoundaryNormal )
    for( int axis=0; axis<numberOfDimensions(); axis++ )
      for( int side=0; side<=1; side++ )
	if( rcData->vertexBoundaryNormal[axis][side]!=NULL )
          rcData->vertexBoundaryNormal[axis][side]->mappedGrid=this;
  
  if( what & THEcenterBoundaryNormal )
    for( int axis=0; axis<numberOfDimensions(); axis++ )
      for( int side=0; side<=1; side++ )
        if( rcData->centerBoundaryNormal[axis][side]!=NULL )
          rcData->centerBoundaryNormal[axis][side]->mappedGrid=this;

  if( what & THEcenterBoundaryTangent )
    for( int axis=0; axis<numberOfDimensions(); axis++ )
      for( int side=0; side<=1; side++ )
        if( rcData->centerBoundaryTangent[axis][side]!=NULL )
          rcData->centerBoundaryTangent[axis][side]->mappedGrid=this;


  return 0;
}




//
// Destroy optional grid data.
//
void MappedGrid::
destroy(const Integer what) {
    rcData->destroy(what);
    updateReferences();
}
#ifdef ADJUST_FOR_PERIODICITY
//
// Adjust the inverted coordinates for periodicity.
//
void MappedGrid::adjustForPeriodicity(
  const RealArray&    r,
  const LogicalArray& whereMask) {
    const Range p(r.getBase(0), r.getBound(0));
    for (Integer kd=0; kd<numberOfDimensions(); kd++) if (isPeriodic(kd)) {
        where (whereMask && r(p,kd) < (Real)0.) r(p,kd) = r(p,kd) + (Real)1.;
        where (whereMask && r(p,kd) > (Real)1. && r(p,kd) != (Real)10.)
          r(p,kd) = r(p,kd) - (Real)1.;
    } // end if, end for
}
#endif // ADJUST_FOR_PERIODICITY
//
// Adjust the inverted coordinates of boundary points
// in cases where the points lie on a shared boundary.
//
void MappedGrid::adjustBoundary(
  MappedGrid&      g2,
  const Integer&   ks1,
  const Integer&   kd1,
  const RealArray& r2,
  const LogicalArray& whereMask) {
//
//  Check whether points on the (ks1,kd1) side of this grid at
//  coordinates r2 of grid g2 are near a side of grid g2.  If so
//  and that side is a boundary shared with the side (ks1,kd1)
//  of this grid then change r2 so that it it is exactly on that
//  side of grid g2.  Repeat for each side of grid g2.
//
#ifdef ADJUST_FOR_PERIODICITY
    g2.adjustForPeriodicity(r2, whereMask);
#endif // ADJUST_FOR_PERIODICITY
    const Range p(r2.getBase(0), r2.getBound(0));
    for (Integer kd2=0; kd2<numberOfDimensions(); kd2++) {
        if (boundaryCondition(ks1,kd1) > 0 && sharedBoundaryFlag(ks1,kd1)) {
            if (g2.boundaryCondition(0,kd2) > 0 &&
              sharedBoundaryFlag(ks1,kd1) == g2.sharedBoundaryFlag(0,kd2))
                where (whereMask(p) && r2(p,kd2) < (Real).5)
                  r2(p,kd2) = (Real)0.;
            if (g2.boundaryCondition(1,kd2) > 0 &&
              sharedBoundaryFlag(ks1,kd1) == g2.sharedBoundaryFlag(1,kd2))
                where (whereMask(p) && r2(p,kd2) > (Real).5)
                  r2(p,kd2) = (Real)1.;
        } // end if
    } // end for
}
//
// Compute the condition number of the mapping inverse.
//
// The condition number is the max norm (max absolute row sum) of the matrix
//
//   [ 1/dr2   0   ] [ rx2 ry2 ] [ xr1 xs1 ] [ dr1  0  ]
//   [   0   1/ds2 ] [ sx2 sy2 ] [ yr1 ys1 ] [  0  ds1 ]
//
void MappedGrid::getInverseCondition(
  MappedGrid&         g2,
  const RealArray&    xr1,
  const RealArray&    rx2,
  const RealArray&    condition,
  const LogicalArray& whereMask) {
//
//  Xr1 and rx2 are three-dimensional arrays containing the derivative of
//  the mapping of this grid and the inverse derivative of the mapping of
//  grid g2.  Condition is a one-dimensional array to hold the results.
//  Its length determines the number of points.  The third dimension of
//  xr1 and rx2 should be the same as the dimension of condition.
//
    Range p(xr1.getBase(0), xr1.getBound(0));
    if (numberOfDimensions() == 0) {
        condition(p) = (Real)0.;
    } else {
        RealArray rowsum(p), dot(p);
        where (whereMask)
          for (Integer kd1=0; kd1<numberOfDimensions(); kd1++) {
            rowsum = (Real)0.;
            for (Integer kd2=0; kd2<numberOfDimensions(); kd2++) {
                dot = (Real)0.;
                for (Integer kd3=0; kd3<numberOfDimensions(); kd3++)
                  dot += rx2(p,kd1,kd3) * xr1(p,kd3,kd2);
                rowsum += (gridSpacing(kd2) / g2.gridSpacing(kd1)) * abs(dot);
            } // end for
            condition(p) = max(condition, rowsum);
        } // end for, end where
    } // end if
}
//
//  Specify the set of processes over which MappedGridFunctions are distributed.
//  We now support only the specification of a contiguous range of process IDs.
//
void MappedGrid::specifyProcesses(const Range& range)
{ 
  // we must destroy the geometry arrays before we can change the parallel distribution
  destroy(MappedGrid::EVERYTHING);


  rcData->specifyProcesses(range); 
}
//
// Initialize the MappedGrid with the given number of dimensions.
//
void MappedGrid::initialize(const Integer& numberOfDimensions_) {
    GenericGrid::initialize();
    rcData->initialize(numberOfDimensions_);
}
//
// Stream output operator.
//
ostream& operator<<(ostream& s, const MappedGrid& g) {
    return s
      <<   (GenericGrid&)g << endl
      << "  numberOfDimensions()              =  "
      <<  g.numberOfDimensions() << endl
      << "  boundaryCondition()               = ["
      <<  g.boundaryCondition(0,0) << ":"
      <<  g.boundaryCondition(1,0) << ","
      <<  g.boundaryCondition(0,1) << ":"
      <<  g.boundaryCondition(1,1) << ","
      <<  g.boundaryCondition(0,2) << ":"
      <<  g.boundaryCondition(1,2) << "]" << endl
      << "  boundaryDiscretizationWidth()     = ["
      <<  g.boundaryDiscretizationWidth(0,0) << ":"
      <<  g.boundaryDiscretizationWidth(1,0) << ","
      <<  g.boundaryDiscretizationWidth(0,1) << ":"
      <<  g.boundaryDiscretizationWidth(1,1) << ","
      <<  g.boundaryDiscretizationWidth(0,2) << ":"
      <<  g.boundaryDiscretizationWidth(1,2) << "]" << endl
      << "  boundingBox()                     = ["
      <<  g.boundingBox(0,0) << ":"
      <<  g.boundingBox(1,0) << ","
      <<  g.boundingBox(0,1) << ":"
      <<  g.boundingBox(1,1) << ","
      <<  g.boundingBox(0,2) << ":"
      <<  g.boundingBox(1,2) << "]" << endl
      << "  gridSpacing()                     = ["
      <<  g.gridSpacing(0) << ","
      <<  g.gridSpacing(1) << ","
      <<  g.gridSpacing(2) << "]" << endl
      << "  isAllCellCentered()               =  "
      << (g.isAllCellCentered() ? 'T' : 'F') << endl
      << "  isAllVertexCentered()             =  "
      << (g.isAllVertexCentered() ? 'T' : 'F') << endl
      << "  isCellCentered()                  = ["
      << (g.isCellCentered(0) ? 'T' : 'F') << ","
      << (g.isCellCentered(1) ? 'T' : 'F') << ","
      << (g.isCellCentered(2) ? 'T' : 'F') << "]" << endl
      << "  discretizationWidth()             = ["
      <<  g.discretizationWidth(0) << ":"
      <<  g.discretizationWidth(1) << ":"
      <<  g.discretizationWidth(2) << "]" << endl
      << "  indexRange()                      = ["
      <<  g.indexRange(0,0) << ":"
      <<  g.indexRange(1,0) << ","
      <<  g.indexRange(0,1) << ":"
      <<  g.indexRange(1,1) << ","
      <<  g.indexRange(0,2) << ":"
      <<  g.indexRange(1,2) << "]" << endl
      << "  extendedIndexRange()              = ["
      <<  g.extendedIndexRange(0,0) << ":"
      <<  g.extendedIndexRange(1,0) << ","
      <<  g.extendedIndexRange(0,1) << ":"
      <<  g.extendedIndexRange(1,1) << ","
      <<  g.extendedIndexRange(0,2) << ":"
      <<  g.extendedIndexRange(1,2) << "]" << endl
      << "  gridIndexRange()                  = ["
      <<  g.gridIndexRange(0,0) << ":"
      <<  g.gridIndexRange(1,0) << ","
      <<  g.gridIndexRange(0,1) << ":"
      <<  g.gridIndexRange(1,1) << ","
      <<  g.gridIndexRange(0,2) << ":"
      <<  g.gridIndexRange(1,2) << "]" << endl
      << "  dimension()                       = ["
      <<  g.dimension(0,0) << ":"
      <<  g.dimension(1,0) << ","
      <<  g.dimension(0,1) << ":"
      <<  g.dimension(1,1) << ","
      <<  g.dimension(0,2) << ":"
      <<  g.dimension(1,2) << "]" << endl
      << "  numberOfGhostPoints()             = ["
      <<  g.numberOfGhostPoints(0,0) << ":"
      <<  g.numberOfGhostPoints(1,0) << ","
      <<  g.numberOfGhostPoints(0,1) << ":"
      <<  g.numberOfGhostPoints(1,1) << ","
      <<  g.numberOfGhostPoints(0,2) << ":"
      <<  g.numberOfGhostPoints(1,2) << "]" << endl
      << "  useGhostPoints()                  =  "
      <<  g.useGhostPoints() << endl
      << "  isPeriodic()                      = ["
      << (g.isPeriodic(0) ? 'T' : 'F') << ","
      << (g.isPeriodic(1) ? 'T' : 'F') << ","
      << (g.isPeriodic(2) ? 'T' : 'F') << "]" << endl
      << "  sharedBoundaryFlag()              = ["
      <<  g.sharedBoundaryFlag(0,0) << ":"
      <<  g.sharedBoundaryFlag(1,0) << ","
      <<  g.sharedBoundaryFlag(0,1) << ":"
      <<  g.sharedBoundaryFlag(1,1) << ","
      <<  g.sharedBoundaryFlag(0,2) << ":"
      <<  g.sharedBoundaryFlag(1,2) << "]" << endl
      << "  sharedBoundaryTolerance()         = ["
      <<  g.sharedBoundaryTolerance(0,0) << ":"
      <<  g.sharedBoundaryTolerance(1,0) << ","
      <<  g.sharedBoundaryTolerance(0,1) << ":"
      <<  g.sharedBoundaryTolerance(1,1) << ","
      <<  g.sharedBoundaryTolerance(0,2) << ":"
      <<  g.sharedBoundaryTolerance(1,2) << "]" << endl;
    
//       << "  minimumEdgeLength()               = ["
//       <<  g.minimumEdgeLength(0) << ","
//       <<  g.minimumEdgeLength(1) << ","
//       <<  g.minimumEdgeLength(2) << "]" << endl
//       << "  maximumEdgeLength()               = ["
//       <<  g.maximumEdgeLength(0) << ","
//       <<  g.maximumEdgeLength(1) << ","
//       <<  g.maximumEdgeLength(2) << "]";
}

//
// class MappedGridData:
//
MappedGridData::MappedGridData(const Integer numberOfDimensions_):
  GenericGridData() {
    className = "MappedGridData";
    refinementGrid=false;
    partitionInitialized=false;
    matrixPartitionInitialized=false;
    shareGridWithMapping=true;

    mask                              = NULL;
    inverseVertexDerivative           = NULL;
//     inverseVertexDerivative2D         = NULL;
//     inverseVertexDerivative1D         = NULL;
    inverseCenterDerivative           = NULL;
//     inverseCenterDerivative2D         = NULL;
//     inverseCenterDerivative1D         = NULL;
    vertex                            = NULL;
//     vertex2D                          = NULL;
//     vertex1D                          = NULL;
    center                            = NULL;
//     center2D                          = NULL;
//     center1D                          = NULL;
    corner                            = NULL;
//     corner2D                          = NULL;
//     corner1D                          = NULL;
    vertexDerivative                  = NULL;
//     vertexDerivative2D                = NULL;
//     vertexDerivative1D                = NULL;
    centerDerivative                  = NULL;
//     centerDerivative2D                = NULL;
//     centerDerivative1D                = NULL;
    vertexJacobian                    = NULL;
    centerJacobian                    = NULL;
    cellVolume                        = NULL;
    centerNormal                      = NULL;
//     centerNormal2D                    = NULL;
//     centerNormal1D                    = NULL;
    centerArea                        = NULL;
//     centerArea2D                      = NULL;
//     centerArea1D                      = NULL;
    faceNormal                        = NULL;
//     faceNormal2D                      = NULL;
//     faceNormal1D                      = NULL;
    faceArea                          = NULL;
//     faceArea2D                        = NULL;
//     faceArea1D                        = NULL;
    for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++) 
    {
      vertexBoundaryNormal[kd][ks]  = NULL;
      centerBoundaryNormal[kd][ks]  = NULL;
      centerBoundaryTangent[kd][ks] = NULL;

      pVertexBoundaryNormal[kd][ks] = NULL;   // serial array version
      pCenterBoundaryNormal[kd][ks] = NULL;   // serial array version
      pCenterBoundaryTangent[kd][ks] = NULL;  // serial array version
    } // end for, end for
    for ( int i=0; i<4; i++ )
      unstructuredBoundaryConditionInfo[i] = unstructuredPeriodicBoundaryInfo[i] = 0;
    initialize(numberOfDimensions_);
}
MappedGridData::MappedGridData(
  const MappedGridData& x,
  const CopyType        ct):
  GenericGridData() {
    className = "MappedGridData";
    refinementGrid=false;
    partitionInitialized=false;
    matrixPartitionInitialized=false;
    shareGridWithMapping=true;
    mask                              = NULL;
    inverseVertexDerivative           = NULL;
//     inverseVertexDerivative2D         = NULL;
//     inverseVertexDerivative1D         = NULL;
    inverseCenterDerivative           = NULL;
//     inverseCenterDerivative2D         = NULL;
//     inverseCenterDerivative1D         = NULL;
    vertex                            = NULL;
//     vertex2D                          = NULL;
//     vertex1D                          = NULL;
    center                            = NULL;
//     center2D                          = NULL;
//     center1D                          = NULL;
    corner                            = NULL;
//     corner2D                          = NULL;
//     corner1D                          = NULL;
    vertexDerivative                  = NULL;
//     vertexDerivative2D                = NULL;
//     vertexDerivative1D                = NULL;
    centerDerivative                  = NULL;
//     centerDerivative2D                = NULL;
//     centerDerivative1D                = NULL;
    vertexJacobian                    = NULL;
    centerJacobian                    = NULL;
    cellVolume                        = NULL;
    centerNormal                      = NULL;
//     centerNormal2D                    = NULL;
//     centerNormal1D                    = NULL;
    centerArea                        = NULL;
//     centerArea2D                      = NULL;
//     centerArea1D                      = NULL;
    faceNormal                        = NULL;
//     faceNormal2D                      = NULL;
//     faceNormal1D                      = NULL;
    faceArea                          = NULL;
//     faceArea2D                        = NULL;
//     faceArea1D                        = NULL;
    for (Integer kd=0; kd<3; kd++) 
    for (Integer ks=0; ks<2; ks++) 
    {
      vertexBoundaryNormal[kd][ks]  = NULL;
      centerBoundaryNormal[kd][ks]  = NULL;
      centerBoundaryTangent[kd][ks] = NULL;

      pVertexBoundaryNormal[kd][ks] = NULL;   // serial array version
      pCenterBoundaryNormal[kd][ks] = NULL;   // serial array version
      pCenterBoundaryTangent[kd][ks] = NULL;  // serial array version
    } // end for, end for

    for ( int i=0; i<4; i++ )
      unstructuredBoundaryConditionInfo[i] = unstructuredPeriodicBoundaryInfo[i] = 0;

    initialize(x.numberOfDimensions);
    if (ct != NOCOPY) *this = x;
}
MappedGridData::~MappedGridData()
{
  destroy(EVERYTHING);   // *wdh* 981121 - fix a major leak

  // do this for now
#ifdef USE_PPP
//   for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++) 
//   {
//     delete pVertexBoundaryNormal[kd][ks]; 
//     delete pVertexBoundaryTangent[kd][ks]; 
//   }
#endif

}

MappedGridData& MappedGridData::
operator=(const MappedGridData& x) 
{
  equals(x);
  return *this;
}

// ====================================================================================
// /Description:
//    Equals operator plus options. This version is used when copying a GridCollection
// that has AMR grids -- in which case we do not want to make a deep copy of the Mapping.
//
//   /options (input): (options % 2)==1 : do NOT copy the mapping 
// ====================================================================================
GenericGridData& MappedGridData::
equals(const GenericGridData& x_, int option /* =0 */ ) 
{
  // printf("==========MappedGridData::equals: option=%i\n",option);
  
  assert( x_.getClassName()=="MappedGridData" );
  
  MappedGridData & x = (MappedGridData&)x_;

  Integer upd = NOTHING, des = NOTHING, kd, ks;

  GenericGridData::operator=(x);
  numberOfDimensions          = x.numberOfDimensions;
  boundaryCondition           = x.boundaryCondition;
  for (kd=0; kd<3; kd++) for (ks=0; ks<2; ks++)  boundaryFlag[ks][kd]=x.boundaryFlag[ks][kd];
  boundaryDiscretizationWidth = x.boundaryDiscretizationWidth;
  boundingBox                 = x.boundingBox;
  localBoundingBox            = x.localBoundingBox;
  gridSpacing                 = x.gridSpacing;
  isAllCellCentered           = x.isAllCellCentered;
  isAllVertexCentered         = x.isAllVertexCentered;
  isCellCentered              = x.isCellCentered;
  discretizationWidth         = x.discretizationWidth;
  indexRange                  = x.indexRange;
  extendedIndexRange          = x.extendedIndexRange;
  for (kd=0; kd<3; kd++) for (ks=0; ks<2; ks++)  extendedRange(ks,kd)=x.extendedRange(ks,kd);
  gridIndexRange              = x.gridIndexRange;
  dimension                   = x.dimension;
  numberOfGhostPoints         = x.numberOfGhostPoints;
  useGhostPoints              = x.useGhostPoints;
  isPeriodic                  = x.isPeriodic;
  sharedBoundaryFlag          = x.sharedBoundaryFlag;
  sharedBoundaryTolerance     = x.sharedBoundaryTolerance;
//   minimumEdgeLength           = x.minimumEdgeLength;
//   maximumEdgeLength           = x.maximumEdgeLength;


  if( (option % 2 )==0 )
  {
    // printf("  MappedGridData::equals: option=%i copy mapping x.getName()=%s\n",
    //                         option,(const char*)x.mapping.getName(Mapping::mappingName));
    mapping                   = x.mapping;
  }
  else
  {
    // printf("  MappedGridData::equals: option=%i do not copy mapping x.getName()=%s\n",
    //                 option,(const char*)x.mapping.getName(Mapping::mappingName));
  }
  
  refinementGrid              = x.refinementGrid;
  shareGridWithMapping        = x.shareGridWithMapping;
  gridType                    = x.gridType; // kkc 110403

  // *wdh* 060723 -- Sometimes we do not want to to copy the partition, e.g. when we are copying from
  // one parallel distribution to another. 
  // We need an option to indicate whether we should copy the partition.

  // ?? partition is not copied??   partitionInitialized        = x.partitionInitialized;
  
  if (x.mask                    &&
      x.mask                   ->grid == &x)
    upd |= THEmask;
  else des |= THEmask;
  if (x.inverseVertexDerivative &&
      x.inverseVertexDerivative->grid == &x)
    upd |= THEinverseVertexDerivative;
  else des |= THEinverseVertexDerivative;
  if (x.inverseCenterDerivative &&
      x.inverseCenterDerivative->grid == &x)
    upd |= THEinverseCenterDerivative;
  else des |= THEinverseCenterDerivative;
  if (x.vertex                  &&
      x.vertex                 ->grid == &x)
    upd |= THEvertex;
  else des |= THEvertex;
  if (x.center                  &&
      x.center                 ->grid == &x)
    upd |= THEcenter;
  else des |= THEcenter;
  if (x.corner                  &&
      x.corner                 ->grid == &x)
    upd |= THEcorner;
  else des |= THEcorner;
  if (x.vertexDerivative        &&
      x.vertexDerivative       ->grid == &x)
    upd |= THEvertexDerivative;
  else des |= THEvertexDerivative;
  if (x.centerDerivative        &&
      x.centerDerivative       ->grid == &x)
    upd |= THEcenterDerivative;
  else des |= THEcenterDerivative;
  if (x.vertexJacobian          &&
      x.vertexJacobian         ->grid == &x)
    upd |= THEvertexJacobian;
  else des |= THEvertexJacobian;
  if (x.centerJacobian          &&
      x.centerJacobian         ->grid == &x)
    upd |= THEcenterJacobian;
  else des |= THEcenterJacobian;
  if (x.cellVolume              &&
      x.cellVolume             ->grid == &x)
    upd |= THEcellVolume;
  else des |= THEcellVolume;
  if (x.centerNormal            &&
      x.centerNormal           ->grid == &x)
    upd |= THEcenterNormal;
  else des |= THEcenterNormal;
  if (x.centerArea              &&
      x.centerArea             ->grid == &x)
    upd |= THEcenterArea;
  else des |= THEcenterArea;
  if (x.faceNormal              &&
      x.faceNormal             ->grid == &x)
    upd |= THEfaceNormal;
  else des |= THEfaceNormal;
  if (x.faceArea                &&
      x.faceArea               ->grid == &x)
    upd |= THEfaceArea;
  else des |= THEfaceArea;
  for (kd=0; kd<numberOfDimensions; kd++) for (ks=0; ks<2; ks++) 
  {
    
    if( (x.vertexBoundaryNormal[kd][ks] && x.vertexBoundaryNormal[kd][ks]->grid == &x) || 
        x.pVertexBoundaryNormal[kd][ks] )
      upd |= THEvertexBoundaryNormal;
    else 
      des |= THEvertexBoundaryNormal;
    if( (x.centerBoundaryNormal[kd][ks] && x.centerBoundaryNormal[kd][ks]->grid == &x) || 
        x.pCenterBoundaryNormal[kd][ks] )
      upd |= THEcenterBoundaryNormal;
    else 
      des |= THEcenterBoundaryNormal;
    if( (x.centerBoundaryTangent[kd][ks] && x.centerBoundaryTangent[kd][ks]->grid == &x) || 
        x.pCenterBoundaryTangent[kd][ks] )
      upd |= THEcenterBoundaryTangent;
    else 
      des |= THEcenterBoundaryTangent;
  } // end for
  // upd |= x.computedGeometry & (THEminMaxEdgeLength | THEboundingBox);
  upd |= x.computedGeometry & (THEboundingBox);
  if (upd &= ~des) update(upd, COMPUTEnothing); if (des) destroy(des);

//   #ifdef USE_PPP
//     Partitioning_Type & xPartition = x.partition;
//     const intSerialArray & ps  = partition.getProcessorSet();
//     const intSerialArray & xps = xPartition.getProcessorSet();
//     // display(ps,"partition.getProcessorSet()");
//     // display(xps,"xPartition.getProcessorSet()");
    
//     const bool sameParallelDistribution = ps.getLength(0)==xps.getLength(0) && max(abs(ps-xps))==0;
//   #else
//     const bool sameParallelDistribution = true;
//   #endif
  const bool sameParallelDistribution = hasSameDistribution(partition,x.partition);

  const int nd=4;
  Index Iv[4];  // null Index means copy all values. 

  if( mask && mask->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*mask,*x.mask);
    else
    {
      // mask->IntegerDistributedArray::operator = (*x.mask);
      CopyArray::copyArray(*mask,Iv,*x.mask,Iv,nd);
    }
    mask ->updateToMatchGrid(*this);
  } // end if
  if( inverseVertexDerivative && inverseVertexDerivative->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*inverseVertexDerivative,*x.inverseVertexDerivative);
    else
    {
      // inverseVertexDerivative ->RealDistributedArray::operator = (*x.inverseVertexDerivative);
      CopyArray::copyArray(*inverseVertexDerivative,Iv,*x.inverseVertexDerivative,Iv,nd);
    }
    inverseVertexDerivative ->updateToMatchGrid(*this);
  } // end if
  if( inverseCenterDerivative && inverseCenterDerivative->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*inverseCenterDerivative,*x.inverseCenterDerivative);
    else
    {
      // inverseCenterDerivative ->RealDistributedArray::operator = (*x.inverseCenterDerivative);
      CopyArray::copyArray(*inverseCenterDerivative,Iv,*x.inverseCenterDerivative,Iv,nd);

    }
    inverseCenterDerivative ->updateToMatchGrid(*this);
  } // end if
  if( vertex && vertex->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*vertex,*x.vertex);
    else
    {
      // vertex->RealDistributedArray::operator = (*x.vertex);
      CopyArray::copyArray(*vertex,Iv,*x.vertex,Iv,nd);
    }
    vertex->updateToMatchGrid(*this);
  } // end if
  if( center && center->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*center,*x.center);
    else
    {
      // center->RealDistributedArray::operator= (*x.center); 
      CopyArray::copyArray(*center,Iv,*x.center,Iv,nd);
   }
    center->updateToMatchGrid(*this);
  } // end if
  if( corner && corner->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*corner,*x.corner);
    else
    {
      // corner->RealDistributedArray::operator = (*x.corner);
      CopyArray::copyArray(*corner,Iv,*x.corner,Iv,nd);
    }
    corner->updateToMatchGrid(*this);
  } // end if
  if( vertexDerivative && vertexDerivative->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*vertexDerivative,*x.vertexDerivative);
    else
    {
      // vertexDerivative->RealDistributedArray::operator = (*x.vertexDerivative);
      CopyArray::copyArray(*vertexDerivative,Iv,*x.vertexDerivative,Iv,nd);
    }
    vertexDerivative ->updateToMatchGrid(*this);
  } // end if
  if( centerDerivative && centerDerivative->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*centerDerivative,*x.centerDerivative);
    else
    {
      // centerDerivative->RealDistributedArray::operator = (*x.centerDerivative);
      CopyArray::copyArray(*centerDerivative,Iv,*x.centerDerivative,Iv,nd);
    }
    centerDerivative->updateToMatchGrid(*this);
  } // end if
  if( vertexJacobian && vertexJacobian->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*vertexJacobian,*x.vertexJacobian);
    else
    {
      // vertexJacobian->RealDistributedArray::operator = (*x.vertexJacobian);
      CopyArray::copyArray(*vertexJacobian,Iv,*x.vertexJacobian,Iv,nd);
    }
    vertexJacobian->updateToMatchGrid(*this);
  } // end if
  if( centerJacobian && centerJacobian->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*centerJacobian,*x.centerJacobian);
    else
    {
      // centerJacobian->RealDistributedArray::operator = (*x.centerJacobian);
      CopyArray::copyArray(*centerJacobian,Iv,*x.centerJacobian,Iv,nd);
    }
    centerJacobian->updateToMatchGrid(*this);
  } // end if
  if( cellVolume && cellVolume->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*cellVolume,*x.cellVolume);
    else
    {
      // cellVolume->RealDistributedArray::operator = (*x.cellVolume);
      CopyArray::copyArray(*cellVolume,Iv,*x.cellVolume,Iv,nd);
    }
    cellVolume->updateToMatchGrid(*this);
  } // end if
  if( centerNormal && centerNormal->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*centerNormal,*x.centerNormal);
    else
    {
      // centerNormal->RealDistributedArray::operator = (*x.centerNormal);
      CopyArray::copyArray(*centerNormal,Iv,*x.centerNormal,Iv,nd);
    }
    centerNormal->updateToMatchGrid(*this);
  } // end if
  if( centerArea && centerArea->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*centerArea,*x.centerArea);
    else
    {
      // centerArea->RealDistributedArray::operator = (*x.centerArea);
      CopyArray::copyArray(*centerArea,Iv,*x.centerArea,Iv,nd);
    }
    centerArea->updateToMatchGrid(*this);
  } // end if
  if( faceNormal && faceNormal->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*faceNormal,*x.faceNormal);
    else
    {
      // faceNormal->RealDistributedArray::operator = (*x.faceNormal);
      CopyArray::copyArray(*faceNormal,Iv,*x.faceNormal,Iv,nd);
    }
    faceNormal->updateToMatchGrid(*this);
  } // end if
  if( faceArea && faceArea->grid == this) 
  {
    if( sameParallelDistribution )
      assign(*faceArea,*x.faceArea);
    else
    {
      // faceArea->RealDistributedArray::operator = (*x.faceArea);
      CopyArray::copyArray(*faceArea,Iv,*x.faceArea,Iv,nd);
    }
    faceArea->updateToMatchGrid(*this);
  } // end if
  for (kd=0; kd<3; kd++) for (ks=0; ks<2; ks++) 
  {
    if( vertexBoundaryNormal[kd][ks] && vertexBoundaryNormal[kd][ks]->elementCount() )  
    {
      if( sameParallelDistribution )
	assign(*vertexBoundaryNormal[kd][ks],*x.vertexBoundaryNormal[kd][ks]);
      else
      {
	// vertexBoundaryNormal[kd][ks]->RealDistributedArray::operator= (*x.vertexBoundaryNormal[kd][ks]);
        CopyArray::copyArray(*vertexBoundaryNormal[kd][ks],Iv,*x.vertexBoundaryNormal[kd][ks],Iv,nd);
      }
      vertexBoundaryNormal[kd][ks]->updateToMatchGrid(*this);
    } // end if
    if( centerBoundaryNormal[kd][ks] && centerBoundaryNormal[kd][ks]->elementCount() ) 
    {
      if( sameParallelDistribution )
	assign(*centerBoundaryNormal[kd][ks],*x.centerBoundaryNormal[kd][ks]);
      else
      {
	// centerBoundaryNormal[kd][ks]->RealDistributedArray::operator= (*x.centerBoundaryNormal[kd][ks]);
        CopyArray::copyArray(*centerBoundaryNormal[kd][ks],Iv,*x.centerBoundaryNormal[kd][ks],Iv,nd);
      }
      centerBoundaryNormal[kd][ks]->updateToMatchGrid(*this);
    } // end if
    if( centerBoundaryTangent[kd][ks] && centerBoundaryTangent[kd][ks]->elementCount() ) 
    {
      if( sameParallelDistribution )
	assign(*centerBoundaryTangent[kd][ks],*x.centerBoundaryTangent[kd][ks]);
      else
      {
	// centerBoundaryTangent[kd][ks]->RealDistributedArray::operator= (*x.centerBoundaryTangent[kd][ks]);
        CopyArray::copyArray(*centerBoundaryTangent[kd][ks],Iv,*x.centerBoundaryTangent[kd][ks],Iv,nd);
      }
      centerBoundaryTangent[kd][ks]->updateToMatchGrid(*this);
    } // end if

    // **** this next section is wrong -- in parallel the arrays may not be the same
    //   size since the parallel distributions could be different. 

    if( pVertexBoundaryNormal[kd][ks] && pVertexBoundaryNormal[kd][ks]->elementCount() ) 
    {
      // pVertexBoundaryNormal[kd][ks]->RealArray::operator= (*x.pVertexBoundaryNormal[kd][ks]);
      RealArray & u = *pVertexBoundaryNormal[kd][ks];
      const RealArray & v = *x.pVertexBoundaryNormal[kd][ks]; 
      if( u.dimension(0)==v.dimension(0) && 
          u.dimension(1)==v.dimension(1) &&
          u.dimension(2)==v.dimension(2) )
      {
        u=v;
      }
      else
      {
        // ** fix this -- recompute ---
        u.redim(0);
      }
    } // end if
    if( pCenterBoundaryNormal[kd][ks] && pCenterBoundaryNormal[kd][ks]->elementCount() ) 
    {
      // pCenterBoundaryNormal[kd][ks]->RealArray::operator= (*x.pCenterBoundaryNormal[kd][ks]);
      RealArray & u = *pCenterBoundaryNormal[kd][ks];
      const RealArray & v = *x.pCenterBoundaryNormal[kd][ks]; 
      if( u.dimension(0)==v.dimension(0) && 
          u.dimension(1)==v.dimension(1) &&
          u.dimension(2)==v.dimension(2) )
      {
        u=v;
      }
      else
      {
        // ** fix this -- recompute ---
        u.redim(0);
      }

    } // end if
    if( pCenterBoundaryTangent[kd][ks] && pCenterBoundaryTangent[kd][ks]->elementCount() ) 
    {
      // pCenterBoundaryTangent[kd][ks]->RealArray::operator= (*x.pCenterBoundaryTangent[kd][ks]);
      RealArray & u = *pCenterBoundaryTangent[kd][ks];
      const RealArray & v = *x.pCenterBoundaryTangent[kd][ks]; 
      if( u.dimension(0)==v.dimension(0) && 
          u.dimension(1)==v.dimension(1) &&
          u.dimension(2)==v.dimension(2) )
      {
        u=v;
      }
      else
      {
        // ** fix this -- recompute ---
        u.redim(0);
      }
    } // end if
  } // end for
  computedGeometry            |= upd & x.computedGeometry;
  return *this;
}
void MappedGridData::
reference(const MappedGridData& x) 
{
  cerr << "MappedGridData::reference(const MappedGridData&) was called!"
       << endl;
  GenericGridData::reference(x);
}
void MappedGridData::breakReference() {
    cerr << "MappedGridData::breakReference() was called!" << endl;
    GenericGridData::breakReference();
}
void MappedGridData::consistencyCheck() const {
    GenericGridData::consistencyCheck();
    boundaryCondition          .Test_Consistency();
    boundaryDiscretizationWidth.Test_Consistency();
    boundingBox                .Test_Consistency();
    localBoundingBox           .Test_Consistency();
    gridSpacing                .Test_Consistency();
    isCellCentered             .Test_Consistency();
    discretizationWidth        .Test_Consistency();
    indexRange                 .Test_Consistency();
    extendedIndexRange         .Test_Consistency();
    gridIndexRange             .Test_Consistency();
    dimension                  .Test_Consistency();
    numberOfGhostPoints        .Test_Consistency();
    isPeriodic                 .Test_Consistency();
    sharedBoundaryFlag         .Test_Consistency();
    sharedBoundaryTolerance    .Test_Consistency();
//     minimumEdgeLength          .Test_Consistency();
//     maximumEdgeLength          .Test_Consistency();
    if (mask)
        mask                      ->consistencyCheck();
    if (inverseVertexDerivative)
        inverseVertexDerivative   ->consistencyCheck();
//     if (inverseVertexDerivative2D)
//         inverseVertexDerivative2D ->consistencyCheck();
//     if (inverseVertexDerivative1D)
//         inverseVertexDerivative1D ->consistencyCheck();
    if (inverseCenterDerivative)
        inverseCenterDerivative   ->consistencyCheck();
//     if (inverseCenterDerivative2D)
//         inverseCenterDerivative2D ->consistencyCheck();
//     if (inverseCenterDerivative1D)
//         inverseCenterDerivative1D ->consistencyCheck();
    if (vertex)
        vertex                    ->consistencyCheck();
//     if (vertex2D)
//         vertex2D                  ->consistencyCheck();
//     if (vertex1D)
//         vertex1D                  ->consistencyCheck();
    if (center)
        center                    ->consistencyCheck();
//     if (center2D)
//         center2D                  ->consistencyCheck();
//     if (center1D)
//         center1D                  ->consistencyCheck();
    if (corner)
        corner                    ->consistencyCheck();
//     if (corner2D)
//         corner2D                  ->consistencyCheck();
//     if (corner1D)
//         corner1D                  ->consistencyCheck();
    if (vertexDerivative)
        vertexDerivative          ->consistencyCheck();
//     if (vertexDerivative2D)
//         vertexDerivative2D        ->consistencyCheck();
//     if (vertexDerivative1D)
//         vertexDerivative1D        ->consistencyCheck();
    if (centerDerivative)
        centerDerivative          ->consistencyCheck();
//     if (centerDerivative2D)
//         centerDerivative2D        ->consistencyCheck();
//     if (centerDerivative1D)
//         centerDerivative1D        ->consistencyCheck();
    if (vertexJacobian)
        vertexJacobian            ->consistencyCheck();
    if (centerJacobian)
        centerJacobian            ->consistencyCheck();
    if (cellVolume)
        cellVolume                ->consistencyCheck();
    if (centerNormal)
        centerNormal              ->consistencyCheck();
//     if (centerNormal2D)
//         centerNormal2D            ->consistencyCheck();
//     if (centerNormal1D)
//         centerNormal1D            ->consistencyCheck();
    if (centerArea)
        centerArea                ->consistencyCheck();
//     if (centerArea2D)
//         centerArea2D              ->consistencyCheck();
//     if (centerArea1D)
//         centerArea1D              ->consistencyCheck();
    if (faceNormal)
        faceNormal                ->consistencyCheck();
//     if (faceNormal2D)
//         faceNormal2D              ->consistencyCheck();
//     if (faceNormal1D)
//         faceNormal1D              ->consistencyCheck();
    if (faceArea)
        faceArea                  ->consistencyCheck();
//     if (faceArea2D)
//         faceArea2D                ->consistencyCheck();
//     if (faceArea1D)
//         faceArea1D                ->consistencyCheck();
    for (Integer i=0; i<3; i++) for (Integer j=0; j<2; j++) {
        if (vertexBoundaryNormal [i][j])
            vertexBoundaryNormal [i][j]->consistencyCheck();
        if (centerBoundaryNormal [i][j])
            centerBoundaryNormal [i][j]->consistencyCheck();
        if (centerBoundaryTangent[i][j])
            centerBoundaryTangent[i][j]->consistencyCheck();
    } // end for, end for
    mapping                    .consistencyCheck();
    I1array                    .Test_Consistency();
    I2array                    .Test_Consistency();
    I3array                    .Test_Consistency();
}
Integer MappedGridData::
get(const GenericDataBase& db,
    const aString&         name,
    bool getMapping /* =true */ )   
// ===========================================================================================
// /getMapping(input) for AMR grids we may not get the mapping.
// ===========================================================================================
{
    Integer returnValue = 0, kd, ks;
    GenericDataBase& dir = *db.virtualConstructor();
    db.find(dir, name, getClassName());

    returnValue |= GenericGridData::get(dir, "GenericGridData");

    returnValue |= dir.get(numberOfDimensions,      "numberOfDimensions");

    const Integer computedGeometry0 = computedGeometry;
    initialize(numberOfDimensions);

    returnValue |= dir.get(boundaryCondition,       "boundaryCondition");
    int boundaryFlagReturnValue = dir.get((int*)boundaryFlag,"boundaryFlag",6);
    returnValue |= boundaryFlagReturnValue;
    
    returnValue |= dir.get(boundaryDiscretizationWidth,
                          "boundaryDiscretizationWidth");
    returnValue |= dir.get(boundingBox,             "boundingBox");
    returnValue |= dir.get(gridSpacing,             "gridSpacing");
#if defined GNU || defined __PHOTON || defined __DECCXX
    {
    Integer foo;
    returnValue |= dir.get(foo,                     "isAllCellCentered");
    isAllCellCentered = foo;
    returnValue |= dir.get(foo,                     "isAllVertexCentered");
    isAllVertexCentered = foo;
    }
#else
    returnValue |= dir.get(isAllCellCentered,       "isAllCellCentered");
    returnValue |= dir.get(isAllVertexCentered,     "isAllVertexCentered");
#endif // defined GNU || defined __PHOTON || defined __DECCXX
    returnValue |= dir.get(isCellCentered,          "isCellCentered");
    returnValue |= dir.get(discretizationWidth,     "discretizationWidth");
    returnValue |= dir.get(indexRange,              "indexRange");
    returnValue |= dir.get(extendedIndexRange,      "extendedIndexRange");
    int extendedRangeReturnValue = dir.get(pExtendedRange,     "extendedRange",6);
    returnValue |= extendedRangeReturnValue;
    returnValue |= dir.get(gridIndexRange,          "gridIndexRange");
    returnValue |= dir.get(dimension,               "dimension");
    returnValue |= dir.get(numberOfGhostPoints,     "numberOfGhostPoints");
#if defined GNU || defined __PHOTON || defined __DECCXX
    {
    Integer foo;
    returnValue |= dir.get(foo,                     "useGhostPoints");
    useGhostPoints = foo;
    }
#else
    returnValue |= dir.get(useGhostPoints,          "useGhostPoints");
#endif // defined GNU || defined __PHOTON || defined __DECCXX
    returnValue |= dir.get(isPeriodic,              "isPeriodic");
    returnValue |= dir.get(sharedBoundaryFlag,      "sharedBoundaryFlag");
    returnValue |= dir.get(sharedBoundaryTolerance, "sharedBoundaryTolerance");
    // *wdh* 100424 returnValue |= dir.get(minimumEdgeLength,       "minimumEdgeLength");
    // *wdh* 100424 returnValue |= dir.get(maximumEdgeLength,       "maximumEdgeLength");
    returnValue |= dir.get(I1array,                 "I1");
    returnValue |= dir.get(I2array,                 "I2");
    returnValue |= dir.get(I3array,                 "I3");
    I1 = I1array.getDataPointer() - I1array.getBase(0);
    I2 = I2array.getDataPointer() - I2array.getBase(0);
    I3 = I3array.getDataPointer() - I3array.getBase(0);

    int temp;
//    returnValue |= dir.get(temp,"gridType"); gridType=(GenericGrid::GridTypeEnum)temp;
    if (!dir.get(temp,"gridType") )
      gridType=(GenericGrid::GridTypeEnum)temp;
    else // old grids usually are structured
    {
      printF("MappedGrid::get:Warning: gridType missing! Assuming it is a structuredGrid\n");
      gridType=GenericGrid::structuredGrid;
    }
    
    if( !dir.get(shareGridWithMapping,"shareGridWithMapping") )
    {
      shareGridWithMapping=true;
    }

    if( boundaryFlagReturnValue!=0 )
    {
      // printf("filling in boundaryFlag\n");
      for( kd=numberOfDimensions; kd<3; kd++ )
      {
        boundaryFlag[0][kd]=boundaryFlag[1][kd]=periodicBoundary; 
      }
      for (kd=0; kd<numberOfDimensions; kd++) 
      {
	for (ks=0; ks<2; ks++) 
	{
	  boundaryFlag[ks][kd]=boundaryCondition(ks,kd) > 0 ? physicalBoundary :
	    boundaryCondition(ks,kd)==0 ? interpolationBoundary :
	    isPeriodic(kd)==Mapping::functionPeriodic ? branchCutPeriodicBoundary : periodicBoundary;
	} 
      } 
    }
    if( extendedRangeReturnValue!=0 )
    {
      // printf("filling in extendedRange \n");
      for( kd=numberOfDimensions; kd<3; kd++ )
      {
        extendedRange(0,kd)=extendedRange(1,kd)=0;
      }
      for (kd=0; kd<numberOfDimensions; kd++) 
      {
	for (ks=0; ks<2; ks++)
	{
	  extendedRange(ks,kd)=boundaryFlag[ks][kd]!=mixedPhysicalInterpolationBoundary ? extendedIndexRange(ks,kd):
	    indexRange(ks,kd) + (2*ks-1) * (discretizationWidth(kd) - 1) / 2 ;
	}
      } // end for
    }

    initializePartition();

    if( getMapping )
    {
      if( false )
      {
	const intSerialArray & processorSet = partition.getProcessorSet();
	printF("MappedGrid::get partition -> processors=[%i,%i]\n",
	       processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
      }

      // The mapping get the same partition as the MappedGrid. *wdh* 110820
      // This partition is used for the grid in the Mapping and the grid points in the DataPointMapping
      returnValue |= mapping.get(dir, "mapping", &partition);
    }
    
    MappedGridData::update((GenericGridData&)*this,
      computedGeometry0 & EVERYTHING, COMPUTEnothing);
    computedGeometry = computedGeometry0;

    if (computedGeometry &                        THEmask)
    returnValue |= mask                   ->get(dir, "mask");

    // *******************
    // initializePartition();  // *wdh* turned OFF 2011/08/22
    //     printF("MappedGridData:get mask.getGhostBoundaryWidth =[%i,%i]\n",
    // 	   mask->getGhostBoundaryWidth(0),mask->getGhostBoundaryWidth(1));
    // *******************


    if( computedGeometry & THEboundingBox )
    {
      // Now compute the local bounding box for the part of the grid on this processor
      // -- base this on the extendedGridIndexRange (include periodic pts and ghost-pts on interp boundaries)
      #ifdef USE_PPP
       IntegerArray extendedGridIndexRange(2,3);
       extendedGridIndexRange=gridIndexRange;
       for( int side=Start; side<=End; side++ )
       {
         for( int axis=0; axis<numberOfDimensions; axis++ )
         {
	   if( boundaryCondition(side,axis)==0 )
	   {
	     extendedGridIndexRange(side,axis)=extendedIndexRange(side,axis);
	   }
         }
       }

       bool local=true;
       mapping.getMapping().getBoundingBox(extendedGridIndexRange,gridIndexRange,localBoundingBox,local);
      #else
       localBoundingBox=boundingBox;
      #endif

    }
    

    if (computedGeometry &                        THEinverseVertexDerivative)
    returnValue |= inverseVertexDerivative->get(dir, "inverseVertexDerivative");
    if (computedGeometry &                        THEinverseCenterDerivative)
    returnValue |= inverseCenterDerivative->get(dir, "inverseCenterDerivative");
    if (computedGeometry &                        THEvertex)
    returnValue |= vertex                 ->get(dir, "vertex");
    if (computedGeometry &                        THEcenter)
    returnValue |= center                 ->get(dir, "center");
    if (computedGeometry &                        THEcorner)
    returnValue |= corner                 ->get(dir, "corner");
    if (computedGeometry &                        THEvertexDerivative)
    returnValue |= vertexDerivative       ->get(dir, "vertexDerivative");
    if (computedGeometry &                        THEcenterDerivative)
    returnValue |= centerDerivative       ->get(dir, "centerDerivative");
    if (computedGeometry &                        THEvertexJacobian)
    returnValue |= vertexJacobian         ->get(dir, "vertexJacobian");
    if (computedGeometry &                        THEcenterJacobian)
    returnValue |= centerJacobian         ->get(dir, "centerJacobian");
    if (computedGeometry &                        THEcellVolume)
    returnValue |= cellVolume             ->get(dir, "cellVolume");
    if (computedGeometry &                        THEcenterNormal)
    returnValue |= centerNormal           ->get(dir, "centerNormal");
    if (computedGeometry &                        THEcenterArea)
    returnValue |= centerArea             ->get(dir, "centerArea");
    if (computedGeometry &                        THEfaceNormal)
    returnValue |= faceNormal             ->get(dir, "faceNormal");
    if (computedGeometry &                        THEfaceArea)
    returnValue |= faceArea               ->get(dir, "faceArea");

    // *wdh* for (kd=0; kd<3; kd++) 
    for (kd=0; kd<numberOfDimensions; kd++) 
    {
      for (ks=0; ks<2; ks++)
      {
        char normal_kd_ks[32];
        if (computedGeometry &  THEvertexBoundaryNormal) 
	{
	  sprintf(normal_kd_ks, "vertexBoundaryNormal[%d][%d]", kd, ks);
          if( vertexBoundaryNormal[kd][ks]!=NULL )
    	    returnValue |= vertexBoundaryNormal[kd][ks]->get(dir, normal_kd_ks);
          else
    	    returnValue |= dir.get(*pVertexBoundaryNormal[kd][ks],normal_kd_ks);
        } // end if
        if (computedGeometry &  THEcenterBoundaryNormal) 
	{
	  sprintf(normal_kd_ks, "centerBoundaryNormal[%d][%d]", kd, ks);
          if( centerBoundaryNormal[kd][ks]!=NULL )
	    returnValue |= centerBoundaryNormal[kd][ks]->get(dir, normal_kd_ks);
          else
	    returnValue |= dir.get(*pCenterBoundaryNormal[kd][ks],normal_kd_ks);
        } // end if
        if (computedGeometry &  THEcenterBoundaryTangent)
	{
	  sprintf(normal_kd_ks, "centerBoundaryTangent[%d][%d]", kd, ks);
          if( centerBoundaryTangent[kd][ks]!=NULL )
	    returnValue |= centerBoundaryTangent[kd][ks]->get(dir,normal_kd_ks);
          else
	    returnValue |= dir.get(*pCenterBoundaryTangent[kd][ks],normal_kd_ks);
        } // end if
      } // end for
    } // end for

    delete &dir;
    return returnValue;
}

Integer MappedGridData::
put(GenericDataBase& db,
    const aString& name,
    bool putMapping /* = true */,
    int geometryToPut /* = -1  */
 ) const 
// geometryToPut : by default put computedGeometry
{
  // printf("\n ####### MG:put: geometryToPut on input = %i\n", geometryToPut);
  
  Integer returnValue = 0;
  if( geometryToPut==-1 ) 
  {
    geometryToPut=computedGeometry;
  }
  else
  {
    // The user has specified what to save but we can only save what has been computed:
    geometryToPut= geometryToPut & computedGeometry;
    //     printf("\n ####### MG:put: computedGeometry = %i\n", computedGeometry);
    //     printf("\n ####### MG:put: computedGeometry & THEmask = %i\n",int(computedGeometry & THEmask));
    //     printf("\n ####### MG:put: geometryToPut & computedGeometry = %i\n", geometryToPut);
  }
  //   printf("\n ####### MG:put: geometryToPut & THEmask = %i\n\n",int(geometryToPut & THEmask));
  //   printf("\n ####### MG:put: geometryToPut & THEivd = %i\n\n",int(geometryToPut & THEinverseVertexDerivative));

  GenericDataBase& dir = *db.virtualConstructor();
  db.create(dir, name, getClassName());

  returnValue |= GenericGridData::put(dir, "GenericGridData", putMapping, geometryToPut );

  returnValue |= dir.put(numberOfDimensions,      "numberOfDimensions");
  returnValue |= dir.put(boundaryCondition,       "boundaryCondition");
  returnValue |= dir.put((int*)boundaryFlag,"boundaryFlag",6);
  returnValue |= dir.put(boundaryDiscretizationWidth,
			 "boundaryDiscretizationWidth");
  returnValue |= dir.put(boundingBox,             "boundingBox");
  returnValue |= dir.put(gridSpacing,             "gridSpacing");
#if defined GNU || defined __PHOTON || defined __DECCXX
  {
  Integer foo = isAllCellCentered;
  returnValue |= dir.put(foo,                     "isAllCellCentered");
  foo = isAllVertexCentered;
  returnValue |= dir.put(foo,                     "isAllVertexCentered");
  }
#else
  returnValue |= dir.put(isAllCellCentered,       "isAllCellCentered");
  returnValue |= dir.put(isAllVertexCentered,     "isAllVertexCentered");
#endif // defined GNU || defined __PHOTON || defined __DECCXX
  returnValue |= dir.put(isCellCentered,          "isCellCentered");
  returnValue |= dir.put(discretizationWidth,     "discretizationWidth");
  returnValue |= dir.put(indexRange,              "indexRange");
  returnValue |= dir.put(extendedIndexRange,      "extendedIndexRange");
  returnValue |= dir.put(pExtendedRange,     "extendedRange",6);
  returnValue |= dir.put(gridIndexRange,          "gridIndexRange");
  returnValue |= dir.put(dimension,               "dimension");
    returnValue |= dir.put(numberOfGhostPoints,     "numberOfGhostPoints");
#if defined GNU || defined __PHOTON || defined __DECCXX
  {
  Integer foo = useGhostPoints;
  returnValue |= dir.put(foo,                     "useGhostPoints");
  }
#else
  returnValue |= dir.put(useGhostPoints,          "useGhostPoints");
#endif // defined GNU || defined __PHOTON || defined __DECCXX
  returnValue |= dir.put(isPeriodic,              "isPeriodic");
  returnValue |= dir.put(sharedBoundaryFlag,      "sharedBoundaryFlag");
  returnValue |= dir.put(sharedBoundaryTolerance, "sharedBoundaryTolerance");
  // *wdh* 100424 returnValue |= dir.put(minimumEdgeLength,       "minimumEdgeLength");
  // *wdh* 100424 returnValue |= dir.put(maximumEdgeLength,       "maximumEdgeLength");
  returnValue |= dir.put(I1array,                 "I1");
  returnValue |= dir.put(I2array,                 "I2");
  returnValue |= dir.put(I3array,                 "I3");

  returnValue |= dir.put((int)gridType,"gridType"); 
  returnValue |= dir.put(shareGridWithMapping,"shareGridWithMapping"); 
  
  if( putMapping )
    returnValue |= mapping.put(dir, "mapping");

  if (geometryToPut &                        THEmask)
    returnValue |= mask                   ->put(dir, "mask");
  if (geometryToPut &                        THEinverseVertexDerivative)
    returnValue |= inverseVertexDerivative->put(dir, "inverseVertexDerivative");
  if (geometryToPut &                        THEinverseCenterDerivative)
    returnValue |= inverseCenterDerivative->put(dir, "inverseCenterDerivative");
  if (geometryToPut &                        THEvertex)
    returnValue |= vertex                 ->put(dir, "vertex");
  if (geometryToPut &                        THEcenter)
    returnValue |= center                 ->put(dir, "center");
  if (geometryToPut &                        THEcorner)
    returnValue |= corner                 ->put(dir, "corner");
  if (geometryToPut &                        THEvertexDerivative)
    returnValue |= vertexDerivative       ->put(dir, "vertexDerivative");
  if (geometryToPut &                        THEcenterDerivative)
    returnValue |= centerDerivative       ->put(dir, "centerDerivative");
  if (geometryToPut &                        THEvertexJacobian)
    returnValue |= vertexJacobian         ->put(dir, "vertexJacobian");
  if (geometryToPut &                        THEcenterJacobian)
    returnValue |= centerJacobian         ->put(dir, "centerJacobian");
  if (geometryToPut &                        THEcellVolume)
    returnValue |= cellVolume             ->put(dir, "cellVolume");
  if (geometryToPut &                        THEcenterNormal)
    returnValue |= centerNormal           ->put(dir, "centerNormal");
  if (geometryToPut &                        THEcenterArea)
    returnValue |= centerArea             ->put(dir, "centerArea");
  if (geometryToPut &                        THEfaceNormal)
    returnValue |= faceNormal             ->put(dir, "faceNormal");
  if (geometryToPut &                        THEfaceArea)
    returnValue |= faceArea               ->put(dir, "faceArea");

  // *wdh* for (Integer kd=0; kd<3; kd++)
  for (Integer kd=0; kd<numberOfDimensions; kd++) 
  {
    for (Integer ks=0; ks<2; ks++) 
    {
      char normal_kd_ks[32];
      if (geometryToPut &  THEvertexBoundaryNormal) 
      {
	sprintf(normal_kd_ks, "vertexBoundaryNormal[%d][%d]", kd, ks);
        if( vertexBoundaryNormal[kd][ks]!=NULL )
	  returnValue |= vertexBoundaryNormal[kd][ks]->put(dir, normal_kd_ks);
        else
    	  returnValue |= dir.put(*pVertexBoundaryNormal[kd][ks],normal_kd_ks);
      } // end if
      if (geometryToPut &  THEcenterBoundaryNormal) 
      {
	sprintf(normal_kd_ks, "centerBoundaryNormal[%d][%d]", kd, ks);
	if( centerBoundaryNormal[kd][ks]!=NULL )
	  returnValue |= centerBoundaryNormal[kd][ks]->put(dir, normal_kd_ks);
	else
	  returnValue |= dir.put(*pCenterBoundaryNormal[kd][ks],normal_kd_ks);
      } // end if
      if (geometryToPut &  THEcenterBoundaryTangent) 
      {
	sprintf(normal_kd_ks, "centerBoundaryTangent[%d][%d]", kd, ks);
	if( centerBoundaryTangent[kd][ks]!=NULL )
	  returnValue |= centerBoundaryTangent[kd][ks]->put(dir,normal_kd_ks);
	else
	  returnValue |= dir.put(*pCenterBoundaryTangent[kd][ks],normal_kd_ks);
      } // end if
    } // end for
  } // end for

  delete &dir;
  return returnValue;
} 

void MappedGridData::
specifyProcesses(const Range& range)
{ 
  if( false ) 
  {
    partition.SpecifyProcessorRange(range); 
    matrixPartition.SpecifyProcessorRange(range); 
    // for some reason we need to re-initialize the partition after changing the number of processors
    // or otherwise the number of parallel ghost lines is missing
    partitionInitialized=false;
    matrixPartitionInitialized=false;
    #ifdef USE_PPP
      assert( partition.Internal_Partitioning_Object->Starting_Processor == range.getBase() );
      assert( partition.Internal_Partitioning_Object->Ending_Processor   == range.getBound() );
    #endif
  }
  else
  {
    #ifdef USE_PPP
      partition.Internal_Partitioning_Object->Starting_Processor=range.getBase();
      partition.Internal_Partitioning_Object->Ending_Processor=range.getBound();

      matrixPartition.Internal_Partitioning_Object->Starting_Processor=range.getBase();
      matrixPartition.Internal_Partitioning_Object->Ending_Processor=range.getBound();
    #endif
  }

  if( numberOfDimensions>0 )  
    initializePartition();
  
  if( false )
  {
    #ifdef USE_PPP
      const intSerialArray & processorSet = partition.getProcessorSet();
      const int pStart=processorSet(processorSet.getBase(0)), pEnd=processorSet(processorSet.getBound(0));
      printF("MappedGridData:specifyProcesses: range=[%i,%i], partition -> processors=[%i,%i], Starting_Processor=%i Ending_Processor=%i\n",
	   range.getBase(), range.getBound(),pStart,pEnd,
           partition.Internal_Partitioning_Object->Starting_Processor,
           partition.Internal_Partitioning_Object->Ending_Processor
           );
    #endif
  }

}

void MappedGridData::
initialize(const Integer& numberOfDimensions_) 
{
  boundaryCondition          .redim(2, 3);
  boundaryDiscretizationWidth.redim(2, 3);
  boundingBox                .redim(2, 3);
  localBoundingBox           .redim(2, 3);
  gridSpacing                .redim(3);
  isCellCentered             .redim(3);
  discretizationWidth        .redim(3);
  indexRange                 .redim(2, 3);
  extendedIndexRange         .redim(2, 3);
  gridIndexRange             .redim(2, 3);
  dimension                  .redim(2, 3);
  numberOfGhostPoints        .redim(2, 3);
  isPeriodic                 .redim(3);
  sharedBoundaryFlag         .redim(2, 3);
  sharedBoundaryTolerance    .redim(2, 3);
//   minimumEdgeLength          .redim(3);
//   maximumEdgeLength          .redim(3);

  destroy(~NOTHING & ~GenericGridData::EVERYTHING);

  numberOfDimensions  = numberOfDimensions_;
  isAllCellCentered   = LogicalFalse;
  isAllVertexCentered = LogicalTrue;
  useGhostPoints      = LogicalFalse;
  gridType=GenericGrid::structuredGrid;
    
  Integer kd, ks;
  for (kd=0; kd<numberOfDimensions; kd++) 
  {
    gridSpacing(kd)                        = (Real).1;
    isCellCentered(kd)                     = LogicalFalse;
    discretizationWidth(kd)                = 3;
    isPeriodic(kd)                         = Mapping::notPeriodic;
//     minimumEdgeLength(kd)                  = (Real)0.;
//     maximumEdgeLength(kd)                  = (Real)0.;
    for (ks=0; ks<2; ks++)
    {
      boundaryCondition(ks,kd)           = 1;
      boundaryFlag[ks][kd]               = physicalBoundary;
      boundaryDiscretizationWidth(ks,kd) = 3;
      boundingBox(ks,kd)                 = (Real)0.;
      localBoundingBox(ks,kd)            = (Real)0.;
      sharedBoundaryFlag(ks,kd)          = 0;
      sharedBoundaryTolerance(ks,kd)     = (Real).1;
      gridIndexRange(ks,kd)              =
	indexRange(ks,kd)                  = ks * 10;
      numberOfGhostPoints(ks,kd)         =
	(discretizationWidth(kd) - 1) / 2;
      dimension(ks,kd)                   =
	gridIndexRange(ks,kd) + (2 * ks - 1) * numberOfGhostPoints(ks,kd);
    } // end for
    if ((isCellCentered(kd) || isPeriodic(kd)) &&
	indexRange(1,kd) > indexRange(0,kd)) indexRange(1,kd)--;
    // extended index range moves the interpolation points to ghost points on interpolation
    // sides (except for interpolation on sides with polar singularities.)
    // *wdh* extendedRange: include ghost points on c-grid boundaries since we interpolate there.
    for (ks=0; ks<2; ks++)
    {
      extendedIndexRange(ks,kd) =
	max0(indexRange(0,kd) - numberOfGhostPoints(0,kd),
	     min0(indexRange(1,kd) + numberOfGhostPoints(1,kd),
		  boundaryCondition(ks,kd) == 0 && useGhostPoints
		  && mapping.getTypeOfCoordinateSingularity(ks,kd)!=Mapping::polarSingularity ?
		  indexRange(ks,kd) + (2*ks-1) * (discretizationWidth(kd) - 1) / 2 :
		  indexRange(ks,kd)));
      extendedRange(ks,kd)=boundaryFlag[ks][kd]!=mixedPhysicalInterpolationBoundary ? extendedIndexRange(ks,kd):
	indexRange(ks,kd) + (2*ks-1) * (discretizationWidth(kd) - 1) / 2 ;
    }
  } // end for

  for (kd=numberOfDimensions; kd<3; kd++)
  {
    gridSpacing(kd)                        = (Real)1.;
    isCellCentered(kd)                     = LogicalFalse;
    discretizationWidth(kd)                = 1;
    isPeriodic(kd)                         = Mapping::derivativePeriodic;
//     minimumEdgeLength(kd)                  = (Real)0.;
//     maximumEdgeLength(kd)                  = (Real)0.;
    for (ks=0; ks<2; ks++) {
      boundaryCondition(ks,kd)           = -1;
      boundaryFlag[ks][kd]               = periodicBoundary;
      boundaryDiscretizationWidth(ks,kd) = 1;
      boundingBox(ks,kd)                 = (Real)0.;
      localBoundingBox(ks,kd)            = (Real)0.;
      sharedBoundaryFlag(ks,kd)          = 0;
      sharedBoundaryTolerance(ks,kd)     = (Real)0.;
      indexRange(ks,kd)                  = 0;
      extendedIndexRange(ks,kd)          = 0;
      extendedRange(ks,kd)              = 0;
      gridIndexRange(ks,kd)              = 0;
      dimension(ks,kd)                   = 0;
      numberOfGhostPoints(ks,kd)         = 0;
    } // end for
  } // end for
}

int MappedGridData::
getWhatForGrid(const int what_) const
// ============================================================================
// /Description:
// remove items from what that are not appropriate for the gridType:
// /Return type: a new value for 'what' 
// ============================================================================
{
  int what=what_;
  if( gridType==MappedGrid::unstructuredGrid ) // here are things we can update on an unstructured grid
      what&= THEmask | THEvertex | THEcenter | THEcorner | THEcenter | THEcorner | 
	THEcellVolume | THEfaceNormal | THEfaceArea | THEboundingBox | 
	THEcenterNormal | THEcenterArea;

  return what;
}


void MappedGridData::
initializePartition()
// =======================================================================================
// /Description:
//   Initialize the partition object.
// =======================================================================================
{
  int debug=0;

  if( !partitionInitialized )
  {
    assert( numberOfDimensions>0 );

    const int myid=max(0,Communication_Manager::My_Process_Number);
    // if( debug & 1 )
    //   printf("***** myid=%i MappedGrid:: initialize the partition with (internal) address %d ***** \n",
    //           myid,partition.getInternalPartitioningObject());
    partitionInitialized=true;


    partition.SpecifyDecompositionAxes(numberOfDimensions);
    int kd;
    for (kd=0; kd<numberOfDimensions; kd++)
    {
      int numGhost=max(MappedGrid::minimumNumberOfDistributedGhostLines,(discretizationWidth(kd)-1)/2);
      // set partition axes and number of ghost line boundaries
      if( debug & 1 )
        printf("****MappedGridData::initializePartition(): myid=%i, numGhost=%i ***\n",myid,numGhost);
      
      partition.partitionAlongAxis(kd, true, numGhost ); 
    }
    for (kd=numberOfDimensions; kd<MAX_ARRAY_DIMENSION; kd++)
      partition.partitionAlongAxis(kd, false, 0);
  }
  if( !matrixPartitionInitialized )
  {
    // The matrix partition is for coefficient matrices
    matrixPartitionInitialized=true;
    matrixPartition.SpecifyDecompositionAxes(numberOfDimensions+1); // is this needed? 
    int kd;
    kd=0;
    matrixPartition.partitionAlongAxis(kd, false, 0);
    for (kd=1; kd<=numberOfDimensions; kd++)
    {
      int numGhost=max(MappedGrid::minimumNumberOfDistributedGhostLines,(discretizationWidth(kd-1)-1)/2);
      // set partition axes and number of ghost line boundaries
      matrixPartition.partitionAlongAxis(kd, true, numGhost ); 
    }
    for (kd=numberOfDimensions+1; kd<MAX_ARRAY_DIMENSION; kd++)
      matrixPartition.partitionAlongAxis(kd, false, 0);
  }
  
}



Integer MappedGridData::
update(GenericGridData& x,
       const Integer    what_,
       const Integer    how) 
{
  int what=getWhatForGrid(what_);

  Integer upd = GenericGridData::update(x, what, how);
  MappedGridData& y = (MappedGridData&)x;
  const Range all, d0 = numberOfDimensions;
  Integer computeNeeded =
    how & COMPUTEgeometry         ? what :
    how & COMPUTEgeometryAsNeeded ? what & ~computedGeometry :
    NOTHING;
//
//  Compute isAllVertexCentered, isAllCellCentered, indexRange,
//  extendedIndexRange, dimension and gridSpacing from the current values
//  of boundaryCondition, discretizationWidth, isPeriodic, isCellCentered,
//  gridIndexRange numberOfGhostPoints and useGhostPoints.
//
  isAllVertexCentered = isAllCellCentered = LogicalTrue;
  Integer kd, ks;
  for (kd=0; kd<numberOfDimensions; kd++) {
    if ( isCellCentered(kd)) isAllVertexCentered = LogicalFalse;
    if (!isCellCentered(kd)) isAllCellCentered   = LogicalFalse;
    for (ks=0; ks<2; ks++) {
      indexRange(ks,kd) = gridIndexRange(ks,kd);
      dimension(ks,kd)  = gridIndexRange(ks,kd) +
	(2 * ks - 1) * numberOfGhostPoints(ks,kd);
    } // end for
    if ((isCellCentered(kd) || isPeriodic(kd)) &&
	indexRange(1,kd) > indexRange(0,kd)) indexRange(1,kd)--;
    for (ks=0; ks<2; ks++)
    {
      extendedIndexRange(ks,kd) =
	max0(indexRange(0,kd) - numberOfGhostPoints(0,kd),
	     min0(indexRange(1,kd) + numberOfGhostPoints(1,kd),
		  boundaryCondition(ks,kd) == 0 && useGhostPoints
		  && mapping.getTypeOfCoordinateSingularity(ks,kd)!=Mapping::polarSingularity ?
		  indexRange(ks,kd) + (2*ks-1) * (discretizationWidth(kd) - 1) / 2 :
		  indexRange(ks,kd)));
	extendedRange(ks,kd)=boundaryFlag[ks][kd]!=mixedPhysicalInterpolationBoundary ? extendedIndexRange(ks,kd):
	  indexRange(ks,kd) + (2*ks-1) * (discretizationWidth(kd) - 1) / 2 ;
    }
    
    gridSpacing(kd) = (Real)1. /
      max0(1, gridIndexRange(1,kd) - gridIndexRange(0,kd));
  } // end for
  for (kd=numberOfDimensions; kd<3; kd++) {
    for (ks=0; ks<2; ks++)
      dimension(ks,kd)           = indexRange(ks,kd)     =
	extendedIndexRange(ks,kd)  = extendedRange(ks,kd) = gridIndexRange(ks,kd) =
	numberOfGhostPoints(ks,kd) = 0;
    gridSpacing(kd) = (Real)1.;
  } // end for
//
//  Compute the indirect addressing vectors. **** USED BY canInterpolate and the grid generator.
//
    Integer i;

#define MAPPED_GRID_UPDATE_INDIRECT_ADDRESSING_VECTOR(I,Iarray,kd,pad)         \
    Iarray.redim(Range(indexRange(0,kd)-pad,indexRange(1,kd)+pad));            \
    I = Iarray.getDataPointer() - Iarray.getBase(0);                           \
    if (isPeriodic(kd)) {                                                      \
        const Integer thePeriod = indexRange(1,kd) - indexRange(0,kd) + 1;     \
        for (i=indexRange(0,kd); i<=indexRange(1,kd); i++)             \
          I[i] = i;                                                            \
        for (i=indexRange(1,kd)+1; i<=indexRange(1,kd)+pad; i++)               \
          I[i] = I[i-thePeriod];                                               \
        for (i=indexRange(0,kd)-1; i>=indexRange(0,kd)-pad; i--)               \
          I[i] = I[i+thePeriod];                                               \
    } else {                                                                   \
        for (Integer i=indexRange(0,kd)-pad; i<=indexRange(1,kd)+pad; i++)     \
          I[i] = i;                                                            \
    }
    MAPPED_GRID_UPDATE_INDIRECT_ADDRESSING_VECTOR(I1, I1array, 0, 16);
    MAPPED_GRID_UPDATE_INDIRECT_ADDRESSING_VECTOR(I2, I2array, 1, 16);
    MAPPED_GRID_UPDATE_INDIRECT_ADDRESSING_VECTOR(I3, I3array, 2, 16);
#undef MAPPED_GRID_UPDATE_INDIRECT_ADDRESSING_VECTOR

    if (what &                   ( THEmask                    |
      THEinverseVertexDerivative | THEinverseCenterDerivative |
      THEvertex                  | THEcenter                  |
      THEcorner                  | THEvertexDerivative        |
      THEcenterDerivative        | THEvertexJacobian          |
      THEcenterJacobian          | THEcellVolume              |
      THEcenterNormal            | THEcenterArea              |
      THEfaceNormal              | THEfaceArea                |
      THEvertexBoundaryNormal    | THEcenterBoundaryNormal    |
      THEcenterBoundaryTangent   )) {
//
//      Compute the partitioning of distributed arrays.
//
      initializePartition();
      
      
    } // end if

    if( gridType==MappedGrid::structuredGrid )
    {
      //  note: The update function was split into two parts to give the compiler an easier time.
      upd |= update1(y, what, how, computeNeeded) | update2(y, what, how, computeNeeded);
      
    }
    else
    {
      upd |= updateUnstructuredGrid(y, what, how, computeNeeded);
    }
    for (kd=0; kd<3; kd++)
    {
      // *wdh* box.convert(kd, isCellCentered(kd) ? IndexType::CELL : IndexType::NODE);
      // *wdh* 010906: always make the box node centered since it corresponds to the grid vertices.
      box.convert(kd, IndexType::NODE);
      box.setSmall(kd,indexRange(0,kd)); box.setBig(kd,indexRange(1,kd));
    } // end for

    return upd | computeGeometry(computeNeeded, how);
}

#define MG_NEW ::new

Integer MappedGridData::
update1(MappedGridData& y,
	const Integer&  what,
	const Integer&  how,
	Integer&        computeNeeded) 
// ==============================================================================================
//
//  note: The update function was split into two parts to give the compiler an easier time.
// ==============================================================================================
{
  Integer upd = 0;
  const Range all, d0 = numberOfDimensions;

  if (what & THEmask) 
  {
    if (&y != this &&
	y.mask &&
	y.mask->elementCount()) 
    {
      if (mask == NULL)
      {
        // printf("new the mask (2)\n");
        mask = MG_NEW IntegerMappedGridFunction;
      }
      mask->reference(*y.mask);
      if (y.computedGeometry &      THEmask) 
      {
	computedGeometry   |=     THEmask;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEmask;
	computeNeeded      |=     THEmask;
      } // end if
    } 
    else if (mask == NULL) 
    {
      // printf("new the mask (1)\n");
      mask = MG_NEW IntegerMappedGridFunction;
    } // end if
    if (mask                         ->updateToMatchGrid(*this) &
	IntegerMappedGridFunction::updateResized) 
    {
      *mask = ISghostPoint;
      (*mask)(Range(extendedRange(0,0),extendedRange(1,0)),
	      Range(extendedRange(0,1),extendedRange(1,1)),
	      Range(extendedRange(0,2),extendedRange(1,2)))
	= ISdiscretizationPoint;
      mask->periodicUpdate();
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEmask;
      computedGeometry       &=    ~THEmask;
      upd                    |=     THEmask;
    } // end if
  } // end if
  if (what & THEinverseVertexDerivative) 
  {
    if (&y != this &&
	y.inverseVertexDerivative &&
	y.inverseVertexDerivative->elementCount()) 
    {
      if (inverseVertexDerivative == NULL)
	inverseVertexDerivative = MG_NEW RealMappedGridFunction;
      inverseVertexDerivative->reference(*y.inverseVertexDerivative);
      if (y.computedGeometry &      THEinverseVertexDerivative) 
      {
	computedGeometry   |=     THEinverseVertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseVertexDerivative;
	computeNeeded      |=     THEinverseVertexDerivative;
      } // end if
    } 
    else if(isAllVertexCentered && &y != this &&
	    y.inverseCenterDerivative &&
	    y.inverseCenterDerivative->elementCount()) 
    {
      if (inverseVertexDerivative == NULL)
	inverseVertexDerivative = MG_NEW RealMappedGridFunction;
      inverseVertexDerivative->reference(*y.inverseCenterDerivative);
      if (y.computedGeometry &      THEinverseCenterDerivative) 
      {
	computedGeometry   |=     THEinverseVertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseVertexDerivative;
	computeNeeded      |=     THEinverseVertexDerivative;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     inverseCenterDerivative &&
	     inverseCenterDerivative->elementCount()) 
    {
      if (inverseVertexDerivative == NULL)
	inverseVertexDerivative = MG_NEW RealMappedGridFunction;
      inverseVertexDerivative->reference(*inverseCenterDerivative);
      if (computedGeometry   &      THEinverseCenterDerivative) 
      {
	computedGeometry   |=     THEinverseVertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseVertexDerivative;
	computeNeeded      |=     THEinverseVertexDerivative;
      } // end if
    } 
    else if (inverseVertexDerivative == NULL) 
    {
      inverseVertexDerivative = MG_NEW RealMappedGridFunction;
    } // end if
    if (inverseVertexDerivative      ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (inverseVertexDerivative2D == NULL)
// 	  inverseVertexDerivative2D = MG_NEW RealMappedGridFunction;
// 	inverseVertexDerivative2D->reference
// 	  (*inverseVertexDerivative);
// 	inverseVertexDerivative2D->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
// 	inverseVertexDerivative2D->setIsCellCentered(LogicalFalse);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (inverseVertexDerivative1D == NULL)
// 	  inverseVertexDerivative1D = MG_NEW RealMappedGridFunction;
// 	inverseVertexDerivative1D->reference
// 	  (*inverseVertexDerivative);
// 	inverseVertexDerivative1D->updateToMatchGrid
// 	  (*this, all, d0, d0);
// 	inverseVertexDerivative1D->setIsCellCentered(LogicalFalse);
//       } // end if
      *inverseVertexDerivative = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEinverseVertexDerivative;
      computedGeometry       &=    ~THEinverseVertexDerivative;
      upd                    |=     THEinverseVertexDerivative;
    } // end if
    inverseVertexDerivative          ->setIsCellCentered(LogicalFalse);
  } // end if
  if (what & THEinverseCenterDerivative) 
  {
    if (&y != this &&
	y.inverseCenterDerivative &&
	y.inverseCenterDerivative->elementCount()) 
    {
      if (inverseCenterDerivative == NULL)
	inverseCenterDerivative = MG_NEW RealMappedGridFunction;
      inverseCenterDerivative->reference(*y.inverseCenterDerivative);
      if (y.computedGeometry &      THEinverseCenterDerivative) 
      {
	computedGeometry   |=     THEinverseCenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseCenterDerivative;
	computeNeeded      |=     THEinverseCenterDerivative;
      } // end if
    } 
    else if(isAllVertexCentered && &y != this &&
	    y.inverseVertexDerivative &&
	    y.inverseVertexDerivative->elementCount()) 
    {
      if (inverseCenterDerivative == NULL)
	inverseCenterDerivative = MG_NEW RealMappedGridFunction;
      inverseCenterDerivative->reference(*y.inverseVertexDerivative);
      if (y.computedGeometry &      THEinverseVertexDerivative) 
      {
	computedGeometry   |=     THEinverseCenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseCenterDerivative;
	computeNeeded      |=     THEinverseCenterDerivative;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     inverseVertexDerivative &&
	     inverseVertexDerivative->elementCount()) 
    {
      if (inverseCenterDerivative == NULL)
	inverseCenterDerivative = MG_NEW RealMappedGridFunction;
      inverseCenterDerivative->reference(*inverseVertexDerivative);
      if (computedGeometry   &      THEinverseVertexDerivative) 
      {
	computedGeometry   |=     THEinverseCenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEinverseCenterDerivative;
	computeNeeded      |=     THEinverseCenterDerivative;
      } // end if
    } 
    else if (inverseCenterDerivative == NULL) 
    {
      inverseCenterDerivative = MG_NEW RealMappedGridFunction;
    } // end if
    if (inverseCenterDerivative      ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (inverseCenterDerivative2D == NULL)
// 	  inverseCenterDerivative2D = MG_NEW RealMappedGridFunction;
// 	inverseCenterDerivative2D->reference
// 	  (*inverseCenterDerivative);
// 	inverseCenterDerivative2D->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (inverseCenterDerivative1D == NULL)
// 	  inverseCenterDerivative1D = MG_NEW RealMappedGridFunction;
// 	inverseCenterDerivative1D->reference
// 	  (*inverseCenterDerivative);
// 	inverseCenterDerivative1D->updateToMatchGrid
// 	  (*this, all, d0, d0);
//       } // end if
      *inverseCenterDerivative = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEinverseCenterDerivative;
      computedGeometry       &=    ~THEinverseCenterDerivative;
      upd                    |=     THEinverseCenterDerivative;
    } // end if
  } // end if
  if (what & THEvertex) 
  {
    if (&y != this &&
	y.vertex &&
	y.vertex->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.center &&
	     y.center->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.center);
      if (y.computedGeometry &      THEcenter) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllCellCentered && &y != this &&
	     y.corner &&
	     y.corner->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.corner);
      if (y.computedGeometry &      THEcorner) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     center && center->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*center);
      if (computedGeometry   &      THEcenter) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllCellCentered &&
	     corner && corner->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*corner);
      if (computedGeometry   &      THEcorner) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (vertex == NULL) 
    {
      vertex = MG_NEW RealMappedGridFunction;
    } // end if
    if( vertex->updateToMatchGrid(*this, all, all, all, d0) & RealMappedGridFunction::updateResized ) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (vertex2D == NULL) vertex2D = MG_NEW RealMappedGridFunction;
// 	vertex2D                 ->reference(*vertex);
// 	vertex2D                 ->updateToMatchGrid
// 	  (*this, all, all, d0);
// 	vertex2D                 ->setIsCellCentered(LogicalFalse);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (vertex1D == NULL) vertex1D = MG_NEW RealMappedGridFunction;
// 	vertex1D                 ->reference(*vertex);
// 	vertex1D                 ->updateToMatchGrid(*this, all, d0);
// 	vertex1D                 ->setIsCellCentered(LogicalFalse);
//       } // end if
      *vertex = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEvertex;
      computedGeometry       &=    ~THEvertex;
      upd                    |=     THEvertex;
    } // end if
    vertex                           ->setIsCellCentered(LogicalFalse);
  } // end if
  if (what & THEcenter) 
  {
    if (&y != this &&
	y.center && y.center->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*y.center);
      if (y.computedGeometry &      THEcenter) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.vertex && y.vertex->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     vertex && vertex->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*vertex);
      if (computedGeometry   &      THEvertex) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (center == NULL) 
    {
      center = MG_NEW RealMappedGridFunction;
    } // end if
    if (center                       ->updateToMatchGrid
	(*this, all, all, all, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (center2D == NULL) center2D = MG_NEW RealMappedGridFunction;
// 	center2D                 ->reference(*center);
// 	center2D                 ->updateToMatchGrid
// 	  (*this, all, all, d0);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (center1D == NULL) center1D = MG_NEW RealMappedGridFunction;
// 	center1D                 ->reference(*center);
// 	center1D                 ->updateToMatchGrid(*this, all, d0);
//       } // end if
      *center = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenter;
      computedGeometry       &=    ~THEcenter;
      upd                    |=     THEcenter;
    } // end if
  } // end if
  if (what & THEcorner) 
  {
    if (&y != this &&
	y.corner && y.corner->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*y.corner);
      if (y.computedGeometry &      THEcorner) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (isAllCellCentered && &y != this &&
	     y.vertex && y.vertex->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (isAllCellCentered &&
	     vertex && vertex->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*vertex);
      if (computedGeometry   &      THEvertex) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (corner == NULL) 
    {
      corner = MG_NEW RealMappedGridFunction;
    } // end if
    if (corner                       ->updateToMatchGrid
	(*this, all, all, all, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (corner2D == NULL) corner2D = MG_NEW RealMappedGridFunction;
// 	corner2D                 ->reference(*corner);
// 	corner2D                 ->updateToMatchGrid
// 	  (*this, all, all, d0);
// 	for (Integer kd=0; kd<numberOfDimensions; kd++)
// 	  corner2D               ->setIsCellCentered
// 	    (!isCellCentered(kd), kd);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (corner1D == NULL) corner1D = MG_NEW RealMappedGridFunction;
// 	corner1D                 ->reference(*corner);
// 	corner1D                 ->updateToMatchGrid(*this, all, d0);
// 	for (Integer kd=0; kd<numberOfDimensions; kd++)
// 	  corner1D               ->setIsCellCentered
// 	    (!isCellCentered(kd), kd);
//       } // end if
      *corner = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcorner;
      computedGeometry       &=    ~THEcorner;
      upd                    |=     THEcorner;
    } // end if
    for (Integer kd=0; kd<numberOfDimensions; kd++)
      corner                         ->setIsCellCentered
	(!isCellCentered(kd), kd);
  } // end if
  if (what & THEvertexDerivative) 
  {
    if (&y != this &&
	y.vertexDerivative && y.vertexDerivative->elementCount()) 
    {
      if (vertexDerivative == NULL)
	vertexDerivative = MG_NEW RealMappedGridFunction;
      vertexDerivative->reference(*y.vertexDerivative);
      if (y.computedGeometry &      THEvertexDerivative) 
      {
	computedGeometry   |=     THEvertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexDerivative;
	computeNeeded      |=     THEvertexDerivative;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.centerDerivative && y.centerDerivative->elementCount()) 
    {
      if (vertexDerivative == NULL)
	vertexDerivative = MG_NEW RealMappedGridFunction;
      vertexDerivative->reference(*y.centerDerivative);
      if (y.computedGeometry &      THEcenterDerivative) 
      {
	computedGeometry   |=     THEvertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexDerivative;
	computeNeeded      |=     THEvertexDerivative;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     centerDerivative && centerDerivative->elementCount()) 
    {
      if (vertexDerivative == NULL)
	vertexDerivative = MG_NEW RealMappedGridFunction;
      vertexDerivative->reference(*centerDerivative);
      if (computedGeometry   &      THEcenterDerivative) 
      {
	computedGeometry   |=     THEvertexDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexDerivative;
	computeNeeded      |=     THEvertexDerivative;
      } // end if
    } 
    else if (vertexDerivative == NULL) 
    {
      vertexDerivative = MG_NEW RealMappedGridFunction;
    } // end if
    if (vertexDerivative             ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (vertexDerivative2D == NULL)
// 	  vertexDerivative2D = MG_NEW RealMappedGridFunction;
// 	vertexDerivative2D       ->reference(*vertexDerivative);
// 	vertexDerivative2D       ->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
// 	vertexDerivative2D       ->setIsCellCentered(LogicalFalse);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (vertexDerivative1D == NULL)
// 	  vertexDerivative1D = MG_NEW RealMappedGridFunction;
// 	vertexDerivative1D       ->reference(*vertexDerivative);
// 	vertexDerivative1D       ->updateToMatchGrid
// 	  (*this, all, d0, d0);
// 	vertexDerivative1D       ->setIsCellCentered(LogicalFalse);
//       } // end if
      *vertexDerivative = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEvertexDerivative;
      computedGeometry       &=    ~THEvertexDerivative;
      upd                    |=     THEvertexDerivative;
    } // end if
    vertexDerivative                 ->setIsCellCentered(LogicalFalse);
  } // end if
  if (what & THEcenterDerivative) 
  {
    if (&y != this &&
	y.centerDerivative && y.centerDerivative->elementCount()) 
    {
      if (centerDerivative == NULL)
	centerDerivative = MG_NEW RealMappedGridFunction;
      centerDerivative->reference(*y.centerDerivative);
      if (y.computedGeometry &      THEcenterDerivative) 
      {
	computedGeometry   |=     THEcenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterDerivative;
	computeNeeded      |=     THEcenterDerivative;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.vertexDerivative && y.vertexDerivative->elementCount()) 
    {
      if (centerDerivative == NULL)
	centerDerivative = MG_NEW RealMappedGridFunction;
      centerDerivative->reference(*y.vertexDerivative);
      if (y.computedGeometry &      THEvertexDerivative) 
      {
	computedGeometry   |=     THEcenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterDerivative;
	computeNeeded      |=     THEcenterDerivative;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     vertexDerivative && vertexDerivative->elementCount()) 
    {
      if (centerDerivative == NULL)
	centerDerivative = MG_NEW RealMappedGridFunction;
      centerDerivative->reference(*vertexDerivative);
      if (computedGeometry   &      THEvertexDerivative) 
      {
	computedGeometry   |=     THEcenterDerivative;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterDerivative;
	computeNeeded      |=     THEcenterDerivative;
      } // end if
    } 
    else if (centerDerivative == NULL) 
    {
      centerDerivative = MG_NEW RealMappedGridFunction;
    } // end if
    if (centerDerivative             ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (centerDerivative2D == NULL)
// 	  centerDerivative2D = MG_NEW RealMappedGridFunction;
// 	centerDerivative2D       ->reference(*centerDerivative);
// 	centerDerivative2D       ->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (centerDerivative1D == NULL)
// 	  centerDerivative1D = MG_NEW RealMappedGridFunction;
// 	centerDerivative1D       ->reference(*centerDerivative);
// 	centerDerivative1D       ->updateToMatchGrid
// 	  (*this, all, d0, d0);
//       } // end if
      *centerDerivative = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenterDerivative;
      computedGeometry       &=    ~THEcenterDerivative;
      upd                    |=     THEcenterDerivative;
    } // end if
  } // end if

  return upd;
}

Integer MappedGridData::
update2( MappedGridData& y,
	 const Integer&  what,
	 const Integer&  how,
	 Integer&        computeNeeded) 
{
  Integer upd = 0;
  const Range all, d0 = numberOfDimensions;

  if (what & THEvertexJacobian) 
  {
    if (&y != this &&
	y.vertexJacobian && y.vertexJacobian->elementCount()) 
    {
      if (vertexJacobian == NULL)
	vertexJacobian = MG_NEW RealMappedGridFunction;
      vertexJacobian->reference(*y.vertexJacobian);
      if (y.computedGeometry &      THEvertexJacobian) 
      {
	computedGeometry   |=     THEvertexJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexJacobian;
	computeNeeded      |=     THEvertexJacobian;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.centerJacobian && y.centerJacobian->elementCount()) 
    {
      if (vertexJacobian == NULL)
	vertexJacobian = MG_NEW RealMappedGridFunction;
      vertexJacobian->reference(*y.centerJacobian);
      if (y.computedGeometry &      THEcenterJacobian) 
      {
	computedGeometry   |=     THEvertexJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexJacobian;
	computeNeeded      |=     THEvertexJacobian;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     centerJacobian && centerJacobian->elementCount()) 
    {
      if (vertexJacobian == NULL)
	vertexJacobian = MG_NEW RealMappedGridFunction;
      vertexJacobian->reference(*centerJacobian);
      if (computedGeometry   &      THEcenterJacobian) 
      {
	computedGeometry   |=     THEvertexJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertexJacobian;
	computeNeeded      |=     THEvertexJacobian;
      } // end if
    } 
    else if (vertexJacobian == NULL) 
    {
      vertexJacobian = MG_NEW RealMappedGridFunction;
    } // end if
    if (vertexJacobian               ->updateToMatchGrid(*this) &
	RealMappedGridFunction::updateResized) 
    {
      *vertexJacobian = (Real)1.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEvertexJacobian;
      computedGeometry       &=    ~THEvertexJacobian;
      upd                    |=     THEvertexJacobian;
    } // end if
    vertexJacobian                   ->setIsCellCentered(LogicalFalse);
  } // end if
  if (what & THEcenterJacobian) 
  {
    if (&y != this &&
	y.centerJacobian && y.centerJacobian->elementCount()) 
    {
      if (centerJacobian == NULL)
	centerJacobian = MG_NEW RealMappedGridFunction;
      centerJacobian->reference(*y.centerJacobian);
      if (y.computedGeometry &      THEcenterJacobian) 
      {
	computedGeometry   |=     THEcenterJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterJacobian;
	computeNeeded      |=     THEcenterJacobian;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     y.vertexJacobian && y.vertexJacobian->elementCount()) 
    {
      if (centerJacobian == NULL)
	centerJacobian = MG_NEW RealMappedGridFunction;
      centerJacobian->reference(*y.vertexJacobian);
      if (y.computedGeometry &      THEvertexJacobian) 
      {
	computedGeometry   |=     THEcenterJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterJacobian;
	computeNeeded      |=     THEcenterJacobian;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     vertexJacobian && vertexJacobian->elementCount()) 
    {
      if (centerJacobian == NULL)
	centerJacobian = MG_NEW RealMappedGridFunction;
      centerJacobian->reference(*vertexJacobian);
      if (computedGeometry   &      THEvertexJacobian) 
      {
	computedGeometry   |=     THEcenterJacobian;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterJacobian;
	computeNeeded      |=     THEcenterJacobian;
      } // end if
    } 
    else if (centerJacobian == NULL) 
    {
      centerJacobian = MG_NEW RealMappedGridFunction;
    } // end if
    if (centerJacobian               ->updateToMatchGrid(*this) &
	RealMappedGridFunction::updateResized) 
    {
      // *centerJacobian = (Real)1.;
      assign(*centerJacobian,1., nullIndex,nullIndex,nullIndex,nullIndex);
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenterJacobian;
      computedGeometry       &=    ~THEcenterJacobian;
      upd                    |=     THEcenterJacobian;
    } // end if
  } // end if
  if (what & THEcellVolume) 
  {
    if (&y != this &&
	y.cellVolume && y.cellVolume->elementCount()) 
    {
      if (cellVolume == NULL) cellVolume = MG_NEW RealMappedGridFunction;
      cellVolume->reference(*y.cellVolume);
      if (y.computedGeometry &      THEcellVolume) 
      {
	computedGeometry   |=     THEcellVolume;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcellVolume;
	computeNeeded      |=     THEcellVolume;
      } // end if
    } 
    else if (cellVolume == NULL) 
    {
      cellVolume = MG_NEW RealMappedGridFunction;
    } // end if
    if (cellVolume                   ->updateToMatchGrid(*this) &
	RealMappedGridFunction::updateResized) 
    {
      *cellVolume = (Real)1.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcellVolume;
      computedGeometry       &=    ~THEcellVolume;
      upd                    |=     THEcellVolume;
    } // end if
  } // end if
  if (what & THEcenterNormal) 
  {
    if (&y != this &&
	y.centerNormal && y.centerNormal->elementCount()) 
    {
      if (centerNormal == NULL) centerNormal = MG_NEW RealMappedGridFunction;
      centerNormal->reference(*y.centerNormal);
      if (y.computedGeometry &      THEcenterNormal) 
      {
	computedGeometry   |=     THEcenterNormal;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterNormal;
	computeNeeded      |=     THEcenterNormal;
      } // end if
    } 
    else if (centerNormal == NULL) 
    {
      centerNormal = MG_NEW RealMappedGridFunction;
    } // end if
    if (centerNormal                 ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (centerNormal2D == NULL)
// 	  centerNormal2D = MG_NEW RealMappedGridFunction;
// 	centerNormal2D           ->reference(*centerNormal);
// 	centerNormal2D           ->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (centerNormal1D == NULL)
// 	  centerNormal1D = MG_NEW RealMappedGridFunction;
// 	centerNormal1D           ->reference(*centerNormal);
// 	centerNormal1D           ->updateToMatchGrid
// 	  (*this, all, d0, d0);
//       } // end if
      *centerNormal = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenterNormal;
      computedGeometry       &=    ~THEcenterNormal;
      upd                    |=     THEcenterNormal;
    } // end if
  } // end if
  if (what & THEcenterArea) 
  {
    if (&y != this &&
	y.centerArea && y.centerArea->elementCount()) 
    {
      if (centerArea == NULL) centerArea = MG_NEW RealMappedGridFunction;
      centerArea->reference(*y.centerArea);
      if (y.computedGeometry &      THEcenterArea) 
      {
	computedGeometry   |=     THEcenterArea;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenterArea;
	computeNeeded      |=     THEcenterArea;
      } // end if
    } 
    else if (centerArea == NULL) 
    {
      centerArea = MG_NEW RealMappedGridFunction;
    } // end if
    if (centerArea                   ->updateToMatchGrid
	(*this, all, all, all, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (centerArea2D == NULL)
// 	  centerArea2D = MG_NEW RealMappedGridFunction;
// 	centerArea2D             ->reference(*centerArea);
// 	centerArea2D             ->updateToMatchGrid
// 	  (*this, all, all, d0);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (centerArea1D == NULL)
// 	  centerArea1D = MG_NEW RealMappedGridFunction;
// 	centerArea1D             ->reference(*centerArea);
// 	centerArea1D             ->updateToMatchGrid(*this, all, d0);
//       } // end if
      *centerArea = (Real)1.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenterArea;
      computedGeometry       &=    ~THEcenterArea;
      upd                    |=     THEcenterArea;
    } // end if
  } // end if
  if (what & THEfaceNormal) 
  {
    if (&y != this &&
	y.faceNormal && y.faceNormal->elementCount()) 
    {
      if (faceNormal == NULL) faceNormal = MG_NEW RealMappedGridFunction;
      faceNormal->reference(*y.faceNormal);
      if (y.computedGeometry &      THEfaceNormal) 
      {
	computedGeometry   |=     THEfaceNormal;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEfaceNormal;
	computeNeeded      |=     THEfaceNormal;
      } // end if
    } 
    else if (faceNormal == NULL) 
    {
      faceNormal = MG_NEW RealMappedGridFunction;
    } // end if
    if (faceNormal                   ->updateToMatchGrid
	(*this, all, all, all, d0, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (faceNormal2D == NULL)
// 	  faceNormal2D = MG_NEW RealMappedGridFunction;
// 	faceNormal2D             ->reference(*faceNormal);
// 	faceNormal2D             ->updateToMatchGrid
// 	  (*this, all, all, d0, d0);
// 	for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
// 	  for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
// 	    faceNormal2D         ->setIsCellCentered
// 	      (!isCellCentered(kd2), kd1, kd2);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (faceNormal1D == NULL)
// 	  faceNormal1D = MG_NEW RealMappedGridFunction;
// 	faceNormal1D             ->reference(*faceNormal);
// 	faceNormal1D             ->updateToMatchGrid
// 	  (*this, all, d0, d0);
// 	for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
// 	  for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
// 	    faceNormal1D         ->setIsCellCentered
// 	      (!isCellCentered(kd2), kd1, kd2);
//       } // end if
      *faceNormal = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEfaceNormal;
      computedGeometry       &=    ~THEfaceNormal;
      upd                    |=     THEfaceNormal;
    } // end if
    for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
      for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
	faceNormal                   ->setIsCellCentered
	  (!isCellCentered(kd2), kd1, kd2);
  } // end if
  if (what & THEfaceArea) 
  {
    if (&y != this &&
	y.faceArea && y.faceArea->elementCount()) 
    {
      if (faceArea == NULL) faceArea = MG_NEW RealMappedGridFunction;
      faceArea->reference(*y.faceArea);
      if (y.computedGeometry &      THEfaceArea) 
      {
	computedGeometry   |=     THEfaceArea;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEfaceArea;
	computeNeeded      |=     THEfaceArea;
      } // end if
    } 
    else if (faceArea == NULL) 
    {
      faceArea = MG_NEW RealMappedGridFunction;
    } // end if
    if (faceArea                     ->updateToMatchGrid
	(*this, all, all, all, d0) &
	RealMappedGridFunction::updateResized) 
    {
//       if (numberOfDimensions <= 2) 
//       {
// 	if (faceArea2D == NULL)
// 	  faceArea2D = MG_NEW RealMappedGridFunction;
// 	faceArea2D               ->reference(*faceArea);
// 	faceArea2D               ->updateToMatchGrid
// 	  (*this, all, all, d0);
// 	for (Integer kd=0; kd<numberOfDimensions; kd++)
// 	  faceArea2D             ->setIsCellCentered
// 	    (!isCellCentered(kd), kd);
//       } // end if
//       if (numberOfDimensions <= 1) 
//       {
// 	if (faceArea1D == NULL)
// 	  faceArea1D = MG_NEW RealMappedGridFunction;
// 	faceArea1D               ->reference(*faceArea);
// 	faceArea1D               ->updateToMatchGrid(*this, all, d0);
// 	for (Integer kd=0; kd<numberOfDimensions; kd++)
// 	  faceArea1D             ->setIsCellCentered
// 	    (!isCellCentered(kd), kd);
//       } // end if
      *faceArea = (Real)1.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEfaceArea;
      computedGeometry       &=    ~THEfaceArea;
      upd                    |=     THEfaceArea;
    } // end if
    for (Integer kd=0; kd<numberOfDimensions; kd++)
      faceArea                       ->setIsCellCentered
	(!isCellCentered(kd), kd);
  } // end if

#ifndef USE_PPP
  // **** serial version ****
  if (what & THEvertexBoundaryNormal) 
  {
    for (Integer kd=0; kd<3; kd++)
    for (Integer ks=0; ks<2; ks++) 
    if (kd < numberOfDimensions) 
    {
      if (&y != this &&
          y.vertexBoundaryNormal[kd][ks] &&
          y.vertexBoundaryNormal[kd][ks]->elementCount()) 
      {
	if (vertexBoundaryNormal[kd][ks] == NULL)
	  vertexBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	vertexBoundaryNormal[kd][ks]->reference(*y.vertexBoundaryNormal[kd][ks]);
	if (y.computedGeometry &      THEvertexBoundaryNormal) 
	{
	  computedGeometry   |=     THEvertexBoundaryNormal;
	} 
	else if (how         &      COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &=    ~THEvertexBoundaryNormal;
	  computeNeeded      |=     THEvertexBoundaryNormal;
	} // end if
      } 
      else if (isAllVertexCentered && &y != this &&
	       (vertexBoundaryNormal[kd][ks] == NULL ||
		vertexBoundaryNormal[kd][ks]->elementCount() == 0) &&
	       (y.centerBoundaryNormal[kd][ks] &&
		y.centerBoundaryNormal[kd][ks]->elementCount() != 0)) 
      {
	if (vertexBoundaryNormal[kd][ks] == NULL)
	  vertexBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	vertexBoundaryNormal[kd][ks]->reference(*y.centerBoundaryNormal[kd][ks]);
	if (y.computedGeometry &   THEcenterBoundaryNormal) 
	{
	  computedGeometry   |=  THEvertexBoundaryNormal;
	} 
	else if (how         &   COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &= ~THEvertexBoundaryNormal;
	  computeNeeded      |=  THEvertexBoundaryNormal;
	} // end if
      } 
      else if (isAllVertexCentered &&
	       (vertexBoundaryNormal[kd][ks] == NULL ||
		vertexBoundaryNormal[kd][ks]->elementCount() == 0) &&
	       (centerBoundaryNormal[kd][ks] &&
		centerBoundaryNormal[kd][ks]->elementCount() != 0)) 
      {
	if (vertexBoundaryNormal[kd][ks] == NULL)
	  vertexBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	vertexBoundaryNormal[kd][ks]->reference(*centerBoundaryNormal[kd][ks]);
	if (computedGeometry   &   THEcenterBoundaryNormal) 
	{
	  computedGeometry   |=  THEvertexBoundaryNormal;
	} 
	else if (how         &   COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &= ~THEvertexBoundaryNormal;
	  computeNeeded      |=  THEvertexBoundaryNormal;
	} // end if
      } 
      else if (vertexBoundaryNormal[kd][ks] == NULL) 
      {
	vertexBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
      } // end if
      Range side[2];
      side[0] = Range(RealMappedGridFunction::startingGridIndex,
		      RealMappedGridFunction::startingGridIndex);
      side[1] = Range(RealMappedGridFunction::endingGridIndex,
		      RealMappedGridFunction::endingGridIndex);
      if (vertexBoundaryNormal[kd][ks]->updateToMatchGrid(*this,
							  kd == 0 ? side[ks] : all, kd == 1 ? side[ks] : all,
							  kd == 2 ? side[ks] : all, d0) &
          RealMappedGridFunction::updateResized) 
      {
	*vertexBoundaryNormal[kd][ks] = (Real)0.;
	if (how                &      COMPUTEgeometryAsNeeded)
	  computeNeeded        |=     THEvertexBoundaryNormal;
	computedGeometry       &=    ~THEvertexBoundaryNormal;
	upd                    |=     THEvertexBoundaryNormal;
      } // end if
      vertexBoundaryNormal[kd][ks]->setIsCellCentered(LogicalFalse);
    } 
    else if (centerBoundaryNormal[kd][ks]) 
    {
      delete centerBoundaryNormal[kd][ks];
      centerBoundaryNormal[kd][ks] = NULL;
    } // end if
  } // end vertexBoundaryNormal
  
  if (what & THEcenterBoundaryNormal) 
  {
    for (Integer kd=0; kd<3; kd++)
    for (Integer ks=0; ks<2; ks++) 
    if (kd < numberOfDimensions) 
    {
      if (&y != this &&
          y.centerBoundaryNormal[kd][ks] &&
          y.centerBoundaryNormal[kd][ks]->elementCount()) 
      {
	if (centerBoundaryNormal[kd][ks] == 0)
	  centerBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	centerBoundaryNormal[kd][ks]->reference(*y.centerBoundaryNormal[kd][ks]);
	if (y.computedGeometry &      THEcenterBoundaryNormal) 
	{
	  computedGeometry   |=     THEcenterBoundaryNormal;
	} 
	else if (how         &      COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &=    ~THEcenterBoundaryNormal;
	  computeNeeded      |=     THEcenterBoundaryNormal;
	} // end if
      } 
      else if (isAllVertexCentered && &y != this &&
	       (centerBoundaryNormal[kd][ks] == NULL ||
		centerBoundaryNormal[kd][ks]->elementCount() == 0) &&
	       (y.vertexBoundaryNormal[kd][ks] &&
		y.vertexBoundaryNormal[kd][ks]->elementCount() != 0)) 
      {
	if (centerBoundaryNormal[kd][ks] == NULL)
	  centerBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	centerBoundaryNormal[kd][ks]->reference(*y.vertexBoundaryNormal[kd][ks]);
	if (y.computedGeometry &   THEvertexBoundaryNormal) 
	{
	  computedGeometry   |=  THEcenterBoundaryNormal;
	} 
	else if (how         &   COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &= ~THEcenterBoundaryNormal;
	  computeNeeded      |=  THEcenterBoundaryNormal;
	} // end if
      } 
      else if (isAllVertexCentered &&
	       (centerBoundaryNormal[kd][ks] == NULL ||
		centerBoundaryNormal[kd][ks]->elementCount() == 0) &&
	       (vertexBoundaryNormal[kd][ks] &&
		vertexBoundaryNormal[kd][ks]->elementCount() != 0)) 
      {
	if (centerBoundaryNormal[kd][ks] == NULL)
	  centerBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
	centerBoundaryNormal[kd][ks]->reference(*vertexBoundaryNormal[kd][ks]);
	if (computedGeometry   &   THEvertexBoundaryNormal) 
	{
	  computedGeometry   |=  THEcenterBoundaryNormal;
	} 
	else if (how         &   COMPUTEgeometryAsNeeded) 
	{
	  computedGeometry   &= ~THEcenterBoundaryNormal;
	  computeNeeded      |=  THEcenterBoundaryNormal;
	} // end if
      } 
      else if (centerBoundaryNormal[kd][ks] == NULL) 
      {
	centerBoundaryNormal[kd][ks] = MG_NEW RealMappedGridFunction;
      } // end if
      Range side[2];
      side[0] = Range(RealMappedGridFunction::startingGridIndex,
		      RealMappedGridFunction::startingGridIndex);
      side[1] = Range(RealMappedGridFunction::endingGridIndex,
		      RealMappedGridFunction::endingGridIndex);
      if (centerBoundaryNormal[kd][ks]->updateToMatchGrid(*this,
							  kd == 0 ? side[ks] : all, kd == 1 ? side[ks] : all,
							  kd == 2 ? side[ks] : all, d0) &
          RealMappedGridFunction::updateResized) 
      {
	*centerBoundaryNormal[kd][ks] = (Real)0.;
	if (how                &      COMPUTEgeometryAsNeeded)
	  computeNeeded        |=     THEcenterBoundaryNormal;
	computedGeometry       &=    ~THEcenterBoundaryNormal;
	upd                    |=     THEcenterBoundaryNormal;
      } // end if
      centerBoundaryNormal[kd][ks]->setIsCellCentered(LogicalFalse, kd);
    } 
    else if (centerBoundaryNormal[kd][ks]) 
    {
      delete centerBoundaryNormal[kd][ks];
      centerBoundaryNormal[kd][ks] = NULL;
    } // end if
  }  // end if centerBoundaryNormal
  

  if (what & THEcenterBoundaryTangent)
  {
    for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++)
    {
	
      if (numberOfDimensions > 1 && kd < numberOfDimensions) 
      {
        if (&y != this &&
	    y.centerBoundaryTangent[kd][ks] &&
	    y.centerBoundaryTangent[kd][ks]->elementCount()) 
	{
	  if (centerBoundaryTangent[kd][ks] == NULL)
	    centerBoundaryTangent[kd][ks] = MG_NEW RealMappedGridFunction;
	  centerBoundaryTangent[kd][ks]->reference(*y.centerBoundaryTangent[kd][ks]);
	  if (y.computedGeometry &      THEcenterBoundaryTangent) 
	  {
	    computedGeometry   |=     THEcenterBoundaryTangent;
	  } 
	  else if (how         &      COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry   &=    ~THEcenterBoundaryTangent;
	    computeNeeded      |=     THEcenterBoundaryTangent;
	  } // end if
        } 
	else if (centerBoundaryTangent[kd][ks] == NULL) 
	{
	  centerBoundaryTangent[kd][ks] = MG_NEW RealMappedGridFunction;
        } // end if
        Range side[2], d0m = numberOfDimensions - 1;
        side[0] = Range(RealMappedGridFunction::startingGridIndex,
                        RealMappedGridFunction::startingGridIndex);
        side[1] = Range(RealMappedGridFunction::endingGridIndex,
                        RealMappedGridFunction::endingGridIndex);
        if (centerBoundaryTangent[kd][ks]->updateToMatchGrid(*this,
							     kd == 0 ? side[ks] : all, kd == 1 ? side[ks] : all,
							     kd == 2 ? side[ks] : all, d0, d0m) &
	    RealMappedGridFunction::updateResized) 
	{
	  *centerBoundaryTangent[kd][ks] = (Real)0.;
	  if (how                &      COMPUTEgeometryAsNeeded)
	    computeNeeded        |=     THEcenterBoundaryTangent;
	  computedGeometry       &=    ~THEcenterBoundaryTangent;
	  upd                    |=     THEcenterBoundaryTangent;
        } // end if
        centerBoundaryTangent[kd][ks]->setIsCellCentered(LogicalFalse, kd);
      } 
      else if (centerBoundaryTangent[kd][ks]) 
      {
        delete centerBoundaryTangent[kd][ks];
        centerBoundaryTangent[kd][ks] = NULL;
      } // end if
    
    }
  
  } // end if centerBoundaryTangent

#else

  // **** parallel version ****
  if (what & THEvertexBoundaryNormal) 
  {

    // const int myid=max(0,Communication_Manager::My_Process_Number);
    // printf("MappedGrid::update2:myid=%i: computedGeometry & THEvertexBoundaryNormal=%i, "
    //        " &y==this = %i\n",
    //        myid,(int)((computedGeometry & THEvertexBoundaryNormal)!=0),
    // 	   int(&y==this));
    
    if( mask==NULL )
    {
      printf("MappedGrid::update2:ERROR: you should build the mask before building the vertexBoundaryNormal.\n");
      OV_ABORT("ERROR");
    }
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(*mask,maskLocal); 

    // dim(0:1,0:2) will hold the dimensions of the local vertexBoundaryNormalArray
    int pdim[6];
    #define dim(side,axis) pdim[(side)+2*(axis)]

    for (Integer kd=0; kd<3; kd++)for (Integer ks=0; ks<2; ks++)
    {
	
      if (kd < numberOfDimensions) 
      {
	bool thisProcessorHasElements=false;
	if( gridIndexRange(ks,kd)>=maskLocal.getBase(kd) && gridIndexRange(ks,kd)<=maskLocal.getBound(kd) )
	{
	  thisProcessorHasElements=true;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( dir!=kd )
	    {
	      dim(0,dir)=maskLocal.getBase(dir);
	      dim(1,dir)=maskLocal.getBound(dir);
	    }
	    else
	    {
	      dim(0,dir)=gridIndexRange(ks,kd);
	      dim(1,dir)=gridIndexRange(ks,kd);
	    }
	    // numElements*=(dim(1,dir)-dim(0,dir)+1);
	  }
	}

	// printf("MappedGrid::update2:myid=%i: (ks,kd)=(%i,%i) pVertexBoundaryNormal=%i elementCount=%i"
	//        " thisProcessorHasElements=%i\n",
	//        myid,ks,kd,(pVertexBoundaryNormal[kd][ks]==NULL ? 0 : 1),
	//        (pVertexBoundaryNormal[kd][ks]==NULL ? 0 : pVertexBoundaryNormal[kd][ks]->elementCount()),
	//        (int)thisProcessorHasElements);
	// fflush(0);

	if( &y != this &&
	    y.pVertexBoundaryNormal[kd][ks] &&
	    y.pVertexBoundaryNormal[kd][ks]->elementCount() ) 
	{
	  if( pVertexBoundaryNormal[kd][ks] == NULL )
	    pVertexBoundaryNormal[kd][ks] = MG_NEW realSerialArray; 
	  pVertexBoundaryNormal[kd][ks]->reference(*y.pVertexBoundaryNormal[kd][ks]);
	  if (y.computedGeometry & THEvertexBoundaryNormal) 
	  {
	    computedGeometry |= THEvertexBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEvertexBoundaryNormal;
	    computeNeeded    |=  THEvertexBoundaryNormal;
	  } // end if
	} 
	else if( isAllVertexCentered && &y != this &&
		 (pVertexBoundaryNormal[kd][ks] == NULL ||
		  pVertexBoundaryNormal[kd][ks]->elementCount() == 0) &&
		 (y.pCenterBoundaryNormal[kd][ks] &&
		  y.pCenterBoundaryNormal[kd][ks]->elementCount() != 0)) 
	{
	  if( pVertexBoundaryNormal[kd][ks] == NULL )
	    pVertexBoundaryNormal[kd][ks] = MG_NEW realSerialArray;
	  pVertexBoundaryNormal[kd][ks]->reference(*y.pCenterBoundaryNormal[kd][ks]);
	  if( y.computedGeometry & THEcenterBoundaryNormal ) 
	  {
	    computedGeometry |= THEvertexBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded ) 
	  {
	    computedGeometry &= ~THEvertexBoundaryNormal;
	    computeNeeded    |=  THEvertexBoundaryNormal;
	  } // end if
	} 
	else if( isAllVertexCentered &&
		 (pVertexBoundaryNormal[kd][ks] == NULL ||
		  pVertexBoundaryNormal[kd][ks]->elementCount() == 0) &&
		 (pCenterBoundaryNormal[kd][ks] &&
		  pCenterBoundaryNormal[kd][ks]->elementCount() != 0)) 
	{
	  if( pVertexBoundaryNormal[kd][ks] == NULL)
	    pVertexBoundaryNormal[kd][ks] = MG_NEW realSerialArray; 
	  pVertexBoundaryNormal[kd][ks]->reference(*pCenterBoundaryNormal[kd][ks]);
	  if( computedGeometry & THEcenterBoundaryNormal) 
	  {
	    computedGeometry |= THEvertexBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEvertexBoundaryNormal;
	    computeNeeded    |=  THEvertexBoundaryNormal;
	  } // end if
	} 
	else if( pVertexBoundaryNormal[kd][ks] == NULL ) 
	{
	  pVertexBoundaryNormal[kd][ks] = MG_NEW realSerialArray; 

	  if (how          &      COMPUTEgeometryAsNeeded)
	    computeNeeded  |=     THEvertexBoundaryNormal;
	  computedGeometry &=    ~THEvertexBoundaryNormal;
	} // end if
	else if( !( computedGeometry & THEvertexBoundaryNormal ) )
	{
	  if (how         &      COMPUTEgeometryAsNeeded)
	    computeNeeded |=     THEvertexBoundaryNormal;
	}
    
	if( !( computedGeometry & THEvertexBoundaryNormal ) && thisProcessorHasElements )
	{
	  // Allocate space for the array if it is not already the correct size.
	  Range R[3];
	  for( int dir=0; dir<3; dir++ )
	  {
	    R[dir]=Range(dim(0,dir),dim(1,dir));
	  }
	  
	  assert( pVertexBoundaryNormal[kd][ks]!=NULL );
	  realSerialArray & v = *pVertexBoundaryNormal[kd][ks];
	  bool resize=false;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( v.dimension(dir)!=R[dir] )
	    {
	      resize=true;
	      break;
	    }
	  }
	  if( resize )
	  { 
	    v.resize(R[0],R[1],R[2],d0); 
	    v=0.; 
	  } // end if( resize )
	}
	
      }
      else if( pVertexBoundaryNormal[kd][ks] ) // *wdh* 2011/08/21 -- this was pCenterBoundaryNormal ?!
      {
	delete pVertexBoundaryNormal[kd][ks];
	pVertexBoundaryNormal[kd][ks] = NULL;
      } // end if 
      
    } // end for ks,kd
    
    if( computeNeeded & THEvertexBoundaryNormal )
    {
      // THEvertexBoundaryNormal was updated:
      upd |= THEvertexBoundaryNormal;
    }


    // if( computeNeeded & THEvertexBoundaryNormal )
    // {
    //   printf("MappedGrid::update2:myid=%i: *** Compute the vertexBoundaryNormal ***\n",myid);
    // }
    // else
    // {
    //   printf("MappedGrid::update2:myid=%i: *** do NOT compute the vertexBoundaryNormal ***\n",myid);
    // }
    // fflush(0);

  } // end vertexBoundaryNormal
  
  if (what & THEcenterBoundaryNormal) 
  {
    if( mask==NULL )
    {
      printf("MappedGrid::update2:ERROR: you should build the mask before building the centerBoundaryNormal.\n");
      OV_ABORT("ERROR");
    }
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(*mask,maskLocal); 

    // dim(0:1,0:2) will hold the dimensions of the local centerBoundaryNormalArray
    int pdim[6];
    #define dim(side,axis) pdim[(side)+2*(axis)]

    for (Integer kd=0; kd<3; kd++)for (Integer ks=0; ks<2; ks++)
    {
      if (kd < numberOfDimensions) 
      {
	bool thisProcessorHasElements=false;
	if( gridIndexRange(ks,kd)>=maskLocal.getBase(kd) && gridIndexRange(ks,kd)<=maskLocal.getBound(kd) )
	{
	  thisProcessorHasElements=true;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( dir!=kd )
	    {
	      dim(0,dir)=maskLocal.getBase(dir);
	      dim(1,dir)=maskLocal.getBound(dir);
	    }
	    else
	    {
	      dim(0,dir)=gridIndexRange(ks,kd);
	      dim(1,dir)=gridIndexRange(ks,kd);
	    }
	    // numElements*=(dim(1,dir)-dim(0,dir)+1);
	  }
	}

	if( &y != this &&
	    y.pCenterBoundaryNormal[kd][ks] &&
	    y.pCenterBoundaryNormal[kd][ks]->elementCount() ) 
	{
	  if( pCenterBoundaryNormal[kd][ks] == 0)
	    pCenterBoundaryNormal[kd][ks] = MG_NEW realSerialArray;
	  pCenterBoundaryNormal[kd][ks]->reference(*y.pCenterBoundaryNormal[kd][ks]);
	  if( y.computedGeometry & THEcenterBoundaryNormal) 
	  {
	    computedGeometry |= THEcenterBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEcenterBoundaryNormal;
	    computeNeeded    |=  THEcenterBoundaryNormal;
	  } // end if
	} 
	else if( isAllVertexCentered && &y != this &&
		 (pCenterBoundaryNormal[kd][ks] == NULL ||
		  pCenterBoundaryNormal[kd][ks]->elementCount() == 0) &&
		 (y.pVertexBoundaryNormal[kd][ks] &&
		  y.pVertexBoundaryNormal[kd][ks]->elementCount() != 0)) 
	{
	  if( pCenterBoundaryNormal[kd][ks] == NULL )
	    pCenterBoundaryNormal[kd][ks] = MG_NEW realSerialArray;
	  pCenterBoundaryNormal[kd][ks]->reference(*y.pVertexBoundaryNormal[kd][ks]);
	  if( y.computedGeometry & THEvertexBoundaryNormal) 
	  {
	    computedGeometry |= THEcenterBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEcenterBoundaryNormal;
	    computeNeeded    |=  THEcenterBoundaryNormal;
	  } // end if
	} 
	else if( isAllVertexCentered &&
		 (pCenterBoundaryNormal[kd][ks] == NULL ||
		  pCenterBoundaryNormal[kd][ks]->elementCount() == 0) &&
		 (pVertexBoundaryNormal[kd][ks] &&
		  pVertexBoundaryNormal[kd][ks]->elementCount() != 0)) 
	{
	  if( pCenterBoundaryNormal[kd][ks] == NULL )
	    pCenterBoundaryNormal[kd][ks] = MG_NEW realSerialArray;
	  pCenterBoundaryNormal[kd][ks]->reference(*pVertexBoundaryNormal[kd][ks]);
	  if( computedGeometry & THEvertexBoundaryNormal) 
	  {
	    computedGeometry |= THEcenterBoundaryNormal;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEcenterBoundaryNormal;
	    computeNeeded    |=  THEcenterBoundaryNormal;
	  } // end if
	} 
	else if( pCenterBoundaryNormal[kd][ks] == NULL ) 
	{
	  pCenterBoundaryNormal[kd][ks] = MG_NEW realSerialArray;

	  if (how                &      COMPUTEgeometryAsNeeded)
	    computeNeeded        |=     THEcenterBoundaryNormal;
	  computedGeometry       &=    ~THEcenterBoundaryNormal;
	} // end if
	else if( !( computedGeometry & THEcenterBoundaryNormal ) )
	{
	  if (how                &      COMPUTEgeometryAsNeeded)
	    computeNeeded        |=     THEcenterBoundaryNormal;
	}
	
	if( !( computedGeometry & THEcenterBoundaryNormal ) && thisProcessorHasElements )
	{
	  // Allocate space for the array if it is not already the correct size.
	  Range R[3];
	  for( int dir=0; dir<3; dir++ )
	  {
	    R[dir]=Range(dim(0,dir),dim(1,dir));
	  }
	  
	  assert( pCenterBoundaryNormal[kd][ks]!=NULL );
	  realSerialArray & v = *pCenterBoundaryNormal[kd][ks];
	  bool resize=false;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( v.dimension(dir)!=R[dir] )
	    {
	      resize=true;
	      break;
	    }
	  }
	  if( resize )
	  { 
	    v.resize(R[0],R[1],R[2],d0); 
	    v=0.; 
	  } // end if( resize )
	}

      } 
      else if( pCenterBoundaryNormal[kd][ks] ) 
      {
	delete pCenterBoundaryNormal[kd][ks];
	pCenterBoundaryNormal[kd][ks] = NULL;
      } // end if

    } // end for ks,kd
    
    if( computeNeeded & THEcenterBoundaryNormal )
    {
      // THEcenterBoundaryNormal was updated:
      upd |= THEcenterBoundaryNormal;
    }


  }  // end if centerBoundaryNormal
  

  if (what & THEcenterBoundaryTangent)
  {
    if( mask==NULL )
    {
      printf("MappedGrid::update2:ERROR: you should build the mask before building the centerBoundaryTangent.\n");
      OV_ABORT("ERROR");
    }
    intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(*mask,maskLocal); 

    // dim(0:1,0:2) will hold the dimensions of the local vcenterBoundaryTangentArray
    int pdim[6];
    #define dim(side,axis) pdim[(side)+2*(axis)]

    for (Integer kd=0; kd<3; kd++)for (Integer ks=0; ks<2; ks++)
    {
      if (numberOfDimensions > 1 && kd < numberOfDimensions) 
      {

	bool thisProcessorHasElements=false;
	if( gridIndexRange(ks,kd)>=maskLocal.getBase(kd) && gridIndexRange(ks,kd)<=maskLocal.getBound(kd) )
	{
	  thisProcessorHasElements=true;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( dir!=kd )
	    {
	      dim(0,dir)=maskLocal.getBase(dir);
	      dim(1,dir)=maskLocal.getBound(dir);
	    }
	    else
	    {
	      dim(0,dir)=gridIndexRange(ks,kd);
	      dim(1,dir)=gridIndexRange(ks,kd);
	    }
	    // numElements*=(dim(1,dir)-dim(0,dir)+1);
	  }
	}

        if( &y != this &&
	    y.pCenterBoundaryTangent[kd][ks] &&
	    y.pCenterBoundaryTangent[kd][ks]->elementCount()) 
	{
	  if( pCenterBoundaryTangent[kd][ks] == NULL)
	    pCenterBoundaryTangent[kd][ks] = MG_NEW realSerialArray;
	  pCenterBoundaryTangent[kd][ks]->reference(*y.pCenterBoundaryTangent[kd][ks]);
	  if( y.computedGeometry & THEcenterBoundaryTangent ) 
	  {
	    computedGeometry |= THEcenterBoundaryTangent;
	  } 
	  else if( how & COMPUTEgeometryAsNeeded) 
	  {
	    computedGeometry &= ~THEcenterBoundaryTangent;
	    computeNeeded    |=  THEcenterBoundaryTangent;
	  } // end if
        } 
	else if( pCenterBoundaryTangent[kd][ks] == NULL ) 
	{
	  pCenterBoundaryTangent[kd][ks] = MG_NEW realSerialArray;
	  if (how          &      COMPUTEgeometryAsNeeded)
	    computeNeeded  |=     THEcenterBoundaryTangent;
	  computedGeometry &=    ~THEcenterBoundaryTangent;
	} // end if
	else if( !( computedGeometry & THEcenterBoundaryTangent ) )
	{
	  if (how          &      COMPUTEgeometryAsNeeded)
	    computeNeeded  |=     THEcenterBoundaryTangent;
	}

	if( !( computedGeometry & THEcenterBoundaryTangent ) && thisProcessorHasElements )
	{
	  // Allocate space for the array if it is not already the correct size.
	  Range R[3];
	  for( int dir=0; dir<3; dir++ )
	  {
	    R[dir]=Range(dim(0,dir),dim(1,dir));
	  }
	  
	  assert( pCenterBoundaryTangent[kd][ks]!=NULL );
	  realSerialArray & v = *pCenterBoundaryTangent[kd][ks];
	  bool resize=false;
	  for( int dir=0; dir<3; dir++ )
	  {
	    if( v.dimension(dir)!=R[dir] )
	    {
	      resize=true;
	      break;
	    }
	  }
	  if( resize )
	  { 
	    // to be consistent with the serial version which is a grid function with only 4 dimensions,
	    // we do the same thing here -- the last two dimensions are merged (*wdh* 061013)
	    // v.resize(R[0],R[1],R[2],d0,d0m); 
            Range d0m = numberOfDimensions - 1;
	    v.resize(R[0],R[1],R[2],d0.getLength()*d0m.getLength()); 
	    v=0.; 
	  } // end if
	}
	
      } 
      else if( pCenterBoundaryTangent[kd][ks] ) 
      {
	delete pCenterBoundaryTangent[kd][ks];
	pCenterBoundaryTangent[kd][ks] = NULL;
      } // end if
    } // end for ks,kd

    if( computeNeeded & THEcenterBoundaryTangent )
    {
      // THEcenterBoundaryTangent was updated:
      upd |= THEcenterBoundaryTangent;
    }
      
  } // end if centerBoundaryTangent
  
  // ***** end parallel version *****
#endif 

//   if (what & ~computedGeometry & y.computedGeometry & THEminMaxEdgeLength && !(how & COMPUTEgeometry)) 
//   {
//     minimumEdgeLength = y.minimumEdgeLength;
//     maximumEdgeLength = y.maximumEdgeLength;
//     computeNeeded    &= ~THEminMaxEdgeLength;
//     computedGeometry |=  THEminMaxEdgeLength;
//   } // end if

  if (what & ~computedGeometry & y.computedGeometry & THEboundingBox && !(how & COMPUTEgeometry)) 
  {
    boundingBox = y.boundingBox;
    localBoundingBox = y.localBoundingBox;  // Is this right ??
    computeNeeded    &= ~THEboundingBox;
    computedGeometry |=  THEboundingBox;
  } // end if

  return upd;
}

Integer MappedGridData::
updateUnstructuredGrid(MappedGridData& y,
		       const Integer&  what,
		       const Integer&  how,
		       Integer&        computeNeeded) 
// ================================================================================================
// /Description:
//    Allocate space for any geometry arrays on a unstructured grid. 
//    Mark the flag computeNeeded for any geometry arrays that should be filled in.
//  /y (input) : if &y==this then allocate space. If &y!=this then reference arrays to the one in y.
//  /what (input) : bit flag of things to update.
// ================================================================================================
{
  Integer upd = 0;
  const Range all, d0 = numberOfDimensions;

  if (what & THEmask)
  {
    // mask update stolen from update1, is this really correct here...?
    if (&y != this &&
	y.mask &&
	y.mask->elementCount()) 
    {
      if (mask == NULL)
      {
        // printf("MG_NEW the mask (2)\n");
        mask = MG_NEW IntegerMappedGridFunction;
      }
      mask->reference(*y.mask);
      if (y.computedGeometry &      THEmask) 
      {
	computedGeometry   |=     THEmask;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEmask;
	computeNeeded      |=     THEmask;
      } // end if
    } 
    else if (mask == NULL) 
    {
      // printf("new the mask (1)\n");
      mask = MG_NEW IntegerMappedGridFunction;
    } // end if
    if (mask                         ->updateToMatchGrid(*this) &
	IntegerMappedGridFunction::updateResized) 
    {
      *mask = ISghostPoint;
      (*mask)(Range(extendedRange(0,0),extendedRange(1,0)),
	      Range(extendedRange(0,1),extendedRange(1,1)),
	      Range(extendedRange(0,2),extendedRange(1,2)))
	= ISdiscretizationPoint;
      mask->periodicUpdate();
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEmask;
      computedGeometry       &=    ~THEmask;
      upd                    |=     THEmask;
    } // end if
  }
  if (what & THEvertex) 
  {
    if (&y != this &&
	y.vertex &&
	y.vertex->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.center &&
	     y.center->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.center);
      if (y.computedGeometry &      THEcenter) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllCellCentered && &y != this &&
	     y.corner &&
	     y.corner->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.corner);
      if (y.computedGeometry &      THEcorner) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     center && center->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*center);
      if (computedGeometry   &      THEcenter) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (isAllCellCentered &&
	     corner && corner->elementCount()) 
    {
      if (vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*corner);
      if (computedGeometry   &      THEcorner) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else if (vertex == NULL) 
    {
      vertex = MG_NEW RealMappedGridFunction;
    } // end if
    if( vertex->updateToMatchGrid(*this, all, all, all, d0) & RealMappedGridFunction::updateResized ) 
    {
      *vertex = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEvertex;
      computedGeometry       &=    ~THEvertex;
      upd                    |=     THEvertex;
    } // end if
    vertex                           ->setIsCellCentered(LogicalFalse);
  } // end if
  if (what & THEcenter) 
  {
    if (&y != this &&
	y.center && y.center->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*y.center);
      if (y.computedGeometry &      THEcenter) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (isAllVertexCentered && &y != this &&
	     y.vertex && y.vertex->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (isAllVertexCentered &&
	     vertex && vertex->elementCount()) 
    {
      if (center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*vertex);
      if (computedGeometry   &      THEvertex) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if (center == NULL) 
    {
      center = MG_NEW RealMappedGridFunction;
    } // end if
    if (center->updateToMatchGrid(*this, all, all, all, d0) & RealMappedGridFunction::updateResized) 
    {
      *center = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenter;
      computedGeometry       &=    ~THEcenter;
      upd                    |=     THEcenter;
    } // end if
  } // end if
  if (what & THEcorner) 
  {
    if (&y != this &&
	y.corner && y.corner->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*y.corner);
      if (y.computedGeometry &      THEcorner) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (isAllCellCentered && &y != this &&
	     y.vertex && y.vertex->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (isAllCellCentered &&
	     vertex && vertex->elementCount()) 
    {
      if (corner == NULL) corner = MG_NEW RealMappedGridFunction;
      corner->reference(*vertex);
      if (computedGeometry   &      THEvertex) 
      {
	computedGeometry   |=     THEcorner;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcorner;
	computeNeeded      |=     THEcorner;
      } // end if
    } 
    else if (corner == NULL) 
    {
      corner = MG_NEW RealMappedGridFunction;
    } // end if
    // The corner is a cell-centered grid function:
    if( corner->updateToMatchGrid(*this,GridFunctionParameters::cellCentered,d0) & 
                            RealMappedGridFunction::updateResized) 
    {
      *corner = (Real)0.;
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcorner;
      computedGeometry       &=    ~THEcorner;
      upd                    |=     THEcorner;
    } // end if
    //     for (Integer kd=0; kd<numberOfDimensions; kd++)
    //       corner->setIsCellCentered	(!isCellCentered(kd), kd);

  } // end if

/* ----
  if( what & THEvertex ) 
  {
    if (&y != this &&
	y.vertex &&
	y.vertex->elementCount()) 
    {
      // reference the array to the one in y
      if( vertex == NULL) vertex = MG_NEW RealMappedGridFunction;
      vertex->reference(*y.vertex);
      if (y.computedGeometry &      THEvertex) 
      {
	computedGeometry   |=     THEvertex;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEvertex;
	computeNeeded      |=     THEvertex;
      } // end if
    } 
    else
    {
      if (vertex == NULL) 
        vertex = MG_NEW RealMappedGridFunction;
      if( isAllVertexCentered && y.center && y.center.elementCount() )
      {
	vertex->reference(*y.center); // reference to the center array if it is there.
      }
      else if(vertex ->updateToMatchGrid(*this,all,d0) & RealMappedGridFunction::updateResized) 
      {
	if (how                &      COMPUTEgeometryAsNeeded)
	  computeNeeded        |=     THEvertex;
	computedGeometry       &=    ~THEvertex;
	upd                    |=     THEvertex;
      } // end if
    }
  } // end if
  if( what & THEcenter ) 
  {
    if (&y != this &&
	y.center &&
	y.center->elementCount()) 
    {
      // reference the array to the one in y
      if( center == NULL) center = MG_NEW RealMappedGridFunction;
      center->reference(*y.center);
      if (y.computedGeometry &      THEcenter) 
      {
	computedGeometry   |=     THEcenter;
      } 
      else if (how         &      COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &=    ~THEcenter;
	computeNeeded      |=     THEcenter;
      } // end if
    } 
    else if( vertex!=NULL ) 
    {
      center=vertex;
    }
    else if (center == NULL) 
    {
      center = MG_NEW RealMappedGridFunction;
    } // end if
    if (center ->updateToMatchGrid(*this,all,d0) & RealMappedGridFunction::updateResized) 
    {
      if (how                &      COMPUTEgeometryAsNeeded)
	computeNeeded        |=     THEcenter;
      computedGeometry       &=    ~THEcenter;
      upd                    |=     THEcenter;
    } // end if
  } // end if

---- */

  return upd;
}


void MappedGridData::destroy(const Integer what) 
{
#define MAPPED_GRID_DATA_DESTROY(x) if (x) { ::delete x; x = NULL; }  // *wdh* 001121 don't use A++ delete
  if (what & THEmask)
  {
//     if( mask )
//       printf("delete the mask\n");
    
    MAPPED_GRID_DATA_DESTROY(mask);
  }
  
    if (what & THEinverseVertexDerivative) 
    {
      MAPPED_GRID_DATA_DESTROY(inverseVertexDerivative);
//       MAPPED_GRID_DATA_DESTROY(inverseVertexDerivative2D);
//       MAPPED_GRID_DATA_DESTROY(inverseVertexDerivative1D);
    } // end if
    if (what & THEinverseCenterDerivative) 
    {
      MAPPED_GRID_DATA_DESTROY(inverseCenterDerivative);
//        MAPPED_GRID_DATA_DESTROY(inverseCenterDerivative2D);
//        MAPPED_GRID_DATA_DESTROY(inverseCenterDerivative1D);
    } // end if
    if (what & THEvertex) 
    {
      MAPPED_GRID_DATA_DESTROY(vertex);
//        MAPPED_GRID_DATA_DESTROY(vertex2D);
//        MAPPED_GRID_DATA_DESTROY(vertex1D);
    } // end if
    if (what & THEcenter) 
    {
      MAPPED_GRID_DATA_DESTROY(center);
//        MAPPED_GRID_DATA_DESTROY(center2D);
//        MAPPED_GRID_DATA_DESTROY(center1D);
    } // end if
    if (what & THEcorner) 
    {
      MAPPED_GRID_DATA_DESTROY(corner);
//        MAPPED_GRID_DATA_DESTROY(corner2D);
//        MAPPED_GRID_DATA_DESTROY(corner1D);
    } // end if
    if (what & THEvertexDerivative) 
    {
      MAPPED_GRID_DATA_DESTROY(vertexDerivative);
//        MAPPED_GRID_DATA_DESTROY(vertexDerivative2D);
//        MAPPED_GRID_DATA_DESTROY(vertexDerivative1D);
    } // end if
    if (what & THEcenterDerivative) 
    {
      MAPPED_GRID_DATA_DESTROY(centerDerivative);
//        MAPPED_GRID_DATA_DESTROY(centerDerivative2D);
//        MAPPED_GRID_DATA_DESTROY(centerDerivative1D);
    } // end if
    if (what & THEvertexJacobian) MAPPED_GRID_DATA_DESTROY(vertexJacobian);
    if (what & THEcenterJacobian) MAPPED_GRID_DATA_DESTROY(centerJacobian);
    if (what & THEcellVolume)     MAPPED_GRID_DATA_DESTROY(cellVolume);
    if (what & THEcenterNormal) 
    {
      MAPPED_GRID_DATA_DESTROY(centerNormal);
//        MAPPED_GRID_DATA_DESTROY(centerNormal2D);
//        MAPPED_GRID_DATA_DESTROY(centerNormal1D);
    } // end if
    if (what & THEcenterArea) 
    {
      MAPPED_GRID_DATA_DESTROY(centerArea);
//        MAPPED_GRID_DATA_DESTROY(centerArea2D);
//        MAPPED_GRID_DATA_DESTROY(centerArea1D);
    } // end if
    if (what & THEfaceNormal) 
    {
      MAPPED_GRID_DATA_DESTROY(faceNormal);
//        MAPPED_GRID_DATA_DESTROY(faceNormal2D);
//        MAPPED_GRID_DATA_DESTROY(faceNormal1D);
    } // end if
    if (what & THEfaceArea) 
    {
      MAPPED_GRID_DATA_DESTROY(faceArea);
//        MAPPED_GRID_DATA_DESTROY(faceArea2D);
//        MAPPED_GRID_DATA_DESTROY(faceArea1D);
    } // end if
    if (what & THEvertexBoundaryNormal)
      for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++)
      {
        MAPPED_GRID_DATA_DESTROY(vertexBoundaryNormal[kd][ks]);
        MAPPED_GRID_DATA_DESTROY(pVertexBoundaryNormal[kd][ks]);
      }
    if (what & THEcenterBoundaryNormal)
      for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++)
      {
        MAPPED_GRID_DATA_DESTROY(centerBoundaryNormal[kd][ks]);
        MAPPED_GRID_DATA_DESTROY(pCenterBoundaryNormal[kd][ks]);
      }
    if (what & THEcenterBoundaryTangent)
      for (Integer kd=0; kd<3; kd++) for (Integer ks=0; ks<2; ks++)
      {
        MAPPED_GRID_DATA_DESTROY(centerBoundaryTangent[kd][ks]);
        MAPPED_GRID_DATA_DESTROY(pCenterBoundaryTangent[kd][ks]);
      }
    
    for ( int i=0; i<4; i++ )
    {
      if ( unstructuredBoundaryConditionInfo[i] )
      {
	delete unstructuredBoundaryConditionInfo[i];
	unstructuredBoundaryConditionInfo[i] = NULL;
      }
      if (unstructuredPeriodicBoundaryInfo[i]) 
      {
	delete unstructuredPeriodicBoundaryInfo[i];
	unstructuredPeriodicBoundaryInfo[i] = NULL;
      }
    }
#undef MAPPED_GRID_DATA_DESTROY
    GenericGridData::destroy(what);
}

const IntegerArray *
MappedGridData::getUnstructuredBCInfo( int type )
{
  if ( gridType==MappedGrid::structuredGrid ) // should this be an assertion?
    return 0;

  assert( mapping.getMapping().getClassName()=="UnstructuredMapping" );
  assert( type>=UnstructuredMapping::Vertex && type<=UnstructuredMapping::Region );

  UnstructuredMapping &umap = (UnstructuredMapping &)mapping.getMapping();

  if (unstructuredBoundaryConditionInfo[type])
    return unstructuredBoundaryConditionInfo[type];

  unstructuredBoundaryConditionInfo[type] = new IntegerArray;

  IntegerArray &bci = *unstructuredBoundaryConditionInfo[type];

  std::string bcTag = std::string("__bcnum ")+UnstructuredMapping::EntityTypeStrings[type].c_str();

  UnstructuredMapping::tag_entity_iterator git, git_end;
  git =  umap.tag_entity_begin(bcTag);
  git_end = umap.tag_entity_end(bcTag);
  int nbc = distance(git,git_end);
  
  if ( !nbc )
    return unstructuredBoundaryConditionInfo[type];

  bci.resize(nbc,2);
  
  int b=0;
  for ( ; git!=git_end; git++, b++ )
    {
      bci(b,0) = git->e;
      bci(b,1) = (long int)umap.getTagData(UnstructuredMapping::EntityTypeEnum(type), git->e, bcTag);
    }

  return unstructuredBoundaryConditionInfo[type];
}

namespace{
  struct Cmp {
    Cmp(IntegerArray &A_) : A(A_) { }
    bool operator() ( int i, int j )
    { return A(i,0)<A(j,0); }
    IntegerArray &A;
  };
}

const IntegerArray *
MappedGridData::getUnstructuredPeriodicBC( int type )
{
  if ( gridType==MappedGrid::structuredGrid ) // should this be an assertion?
    return 0;

  assert( mapping.getMapping().getClassName()=="UnstructuredMapping" );
  assert( type>=UnstructuredMapping::Vertex && type<=UnstructuredMapping::Region );

  UnstructuredMapping &umap = (UnstructuredMapping &)mapping.getMapping();

  if ( unstructuredPeriodicBoundaryInfo[type] )
    return unstructuredPeriodicBoundaryInfo[type];

  unstructuredPeriodicBoundaryInfo[type] = new IntegerArray;

  IntegerArray &pbc = *unstructuredPeriodicBoundaryInfo[type];

  // periodic cells are marked in the unstructured mapping, we reconstruct
  //          periodicity info for other entities based on cell information

  UnstructuredMapping::EntityTypeEnum cellType = UnstructuredMapping::EntityTypeEnum(umap.getDomainDimension());
  UnstructuredMapping::EntityTypeEnum perType  = UnstructuredMapping::EntityTypeEnum(type);

  std::string perCellTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[cellType].c_str();
  std::string bdyCellTag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[cellType].c_str();
  std::string ghostCellTag = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[cellType].c_str();
  std::string perTag = std::string("periodic ") + UnstructuredMapping::EntityTypeStrings[perType].c_str();
  std::string bdyPerTag = std::string("boundary ") + UnstructuredMapping::EntityTypeStrings[perType].c_str();
  std::string ghostPerTag = std::string("Ghost ")+UnstructuredMapping::EntityTypeStrings[perType].c_str();

  int nPerCell = distance( umap.tag_entity_begin( perCellTag ), umap.tag_entity_end( perCellTag ) );

  if ( nPerCell==0 )
    return unstructuredPeriodicBoundaryInfo[type];

  UnstructuredMapping::tag_entity_iterator git, git_end;
  git =  umap.tag_entity_begin(perCellTag);
  git_end = umap.tag_entity_end(perCellTag);

  // cook up an upper bound for the number of periodic boundary entities
  int nPer = nPerCell;
  if ( cellType!=perType )
    { 
      nPer = 0;
      for ( ; git!=git_end; git++ )
	{
	  UnstructuredMappingAdjacencyIterator ai = umap.adjacency_begin(*git, perType);
	  nPer += ai.nAdjacent();
	}
    }
  
  pbc.redim(nPer,2);
  pbc = -1;

  nPer = 0;
  git =  umap.tag_entity_begin(perCellTag);

  ArraySimple<bool> found( umap.size(perType) );
  found = false;
  cout<<ghostPerTag<<endl;
  for ( ; git!=git_end; git++ )
    {
      int ep = (long int)umap.getTagData(cellType, git->e, perCellTag);

      if ( cellType == perType )
	{
	  pbc(nPer,0) = git->e;
	  pbc(nPer,1) = ep;
	  nPer++;
	}
      else
	{
	  UnstructuredMappingAdjacencyIterator ei, ei_end, epi;
	  epi = umap.adjacency_begin(cellType, ep, perType);
	  ei = umap.adjacency_begin(*git, perType);
	  ei_end = umap.adjacency_end(*git, perType);

	  assert(epi.nAdjacent()==ei.nAdjacent());

	  for ( ; ei!=ei_end; ei++, epi++ )
	    {
	      if ( umap.hasTag(perType, *ei, ghostPerTag) && !found(*ei) )
		{
		  // typically we don't like this but 
		  // should it be an assertion?
		  assert(!umap.hasTag(perType, *epi, ghostPerTag)); 
		  assert(!found(*ei));

		  pbc(nPer,0) = *ei;
		  pbc(nPer,1) = *epi;
		  found(*ei) = true;
		  nPer++;
		}
	    }
	}
    }

  pbc.resize(nPer,2);
  IntegerArray perm(nPer);
  perm.seqAdd(0,1);

  Cmp cmp(pbc);

  std::sort( &perm(0),&perm(0)+nPer, cmp );

  IntegerArray unsorted;
  unsorted = pbc;
  for ( int i=0; i<nPer; i++ )
    for ( int a=0; a<2; a++ )
      pbc( i, a) = unsorted( perm(i), a);

  //  unsorted.display("unsorted");
  //  pbc.display("sorted");
  return unstructuredPeriodicBoundaryInfo[type];

}
