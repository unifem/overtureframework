//
// Who to blame:  Geoff Chesshire and WDH
//
//#define OV_DEBUG
#include "CompositeGrid.h"
#include "UnstructuredMapping.h" // who to blame for this: Kyle Chand
#include "display.h"
#include "HDF_DataBase.h"
#include "App.h"
#include "ParallelUtility.h"
#include "SparseArray.h"

#ifdef USE_STL
RCVector_STATIC_MEMBER_DATA(CompositeGrid)
#endif // USE_STL

//  Define a triple for loop.  The macro is defined only within this file.
#define COMPOSITE_GRID_FOR_3(range,i,j,k)                              \
        for (k=((Integer*)(range))[4]; k<=((Integer*)(range))[5]; k++) \
        for (j=((Integer*)(range))[2]; j<=((Integer*)(range))[3]; j++) \
        for (i=((Integer*)(range))[0]; i<=((Integer*)(range))[1]; i++)

//
// class CompositeGrid:
//
// Public member functions:
//
// Default constructor.
//
// If numberOfDimensions_==0 (e.g., by default) then create a null
// CompositeGrid.  Otherwise create a CompositeGrid
// with the given number of dimensions and number of component grids.
//
CompositeGrid::CompositeGrid(
  const Integer numberOfDimensions_,
  const Integer numberOfComponentGrids_): 
  GridCollection() {
    className = "CompositeGrid";
    master=this;
    rcData = new
      CompositeGridData(numberOfDimensions_, numberOfComponentGrids_);
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    updateReferences();
}
//
// Copy constructor.  (Does a deep copy by default.)
//
CompositeGrid::CompositeGrid(
  const CompositeGrid& x,
  const CopyType       ct): 
  GridCollection() {
    className = "CompositeGrid";
    master=this;
    switch (ct) {
      case DEEP:
      case NOCOPY:
        rcData = (CompositeGridData*)
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
}
//
// Destructor.
//
CompositeGrid::
~CompositeGrid()
{ 
  // printF("~CompositeGrid() : rcData->getRefCount=%i\n",rcData->getReferenceCount());
  if (isCounted && rcData->decrementReferenceCount() == 0) 
    delete rcData; 
}


CompositeGrid& CompositeGrid::
operator=(const CompositeGrid& x) 
// =================================================================================================
// /Description:
//    Assignment operator.  (Does a deep copy.)
//
// /Note: To copy a CompositeGrid to a different set of processors, build
//        a gridDistributionList, set keepGridDistributionOnCopy(true), and then perform the copy.
// =================================================================================================
{
//  GridCollection::operator=(x);
  if (rcData != x.rcData) {
    if (rcData->getClassName() == x.rcData->getClassName())
    {
      (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
      updateReferences();

      // *wdh* 000612 : to get refinementLevels etc.
      if( rcData->computedGeometry & THEbaseGrid )
        rcData->update(THEbaseGrid); 
      if( rcData->computedGeometry & THEcomponentGrid )
        rcData->update(THEcomponentGrid); 
      if( rcData->computedGeometry & THErefinementLevel )
        rcData->update(THErefinementLevel); 
      if( rcData->computedGeometry & THEmultigridLevel)
        rcData->update(THEmultigridLevel);
      if( rcData->computedGeometry & THEdomain)
        rcData->update(THEdomain);
    } 
    else
    {
      CompositeGrid& y =
	*(CompositeGrid*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
    master=x.master;
  } // end if
  return *this;
}

//\begin{>>CompositeGridInclude.tex}{\subsubsection{reference(CompositeGrid)}} 
void CompositeGrid::
reference(const CompositeGrid& x) 
// ===========================================================
// /Description:
//    Make a reference.  (Does a shallow copy.)
//\end{CompositeGridInclude.tex}
// ===========================================================
{
  GridCollection::reference(x);
  if (rcData != x.rcData) 
  {
    if (isCounted && rcData->decrementReferenceCount() == 0)
      delete rcData;
    rcData = x.rcData;
    isCounted = x.isCounted;
    if (isCounted) rcData->incrementReferenceCount();
    // *wdh* updateReferences();
    master=x.master;
  } // end if
  updateReferences();   // *wdh* 000322 -- we must always do this since the number of grids etc. may have changed.

}

void CompositeGrid::
reference(CompositeGridData& x) 
{
  GridCollection::reference(x);
  if (rcData != &x) 
  {
    if (isCounted && rcData->decrementReferenceCount() == 0)
      delete rcData;
    rcData = &x;
    isCounted = !x.uncountedReferencesMayExist();
    if (isCounted) rcData->incrementReferenceCount();
    // *wdh* updateReferences();
  } // end if
  updateReferences();   // *wdh* 000322 -- we must always do this since the number of grids etc. may have changed.
}

//
// Break a reference.  (Replaces with a deep copy.)
//
void CompositeGrid::breakReference() {
//  GridCollection::breakReference();
    if (!isCounted || rcData->getReferenceCount() != 1) {
        CompositeGrid x = *this; // Uses the (deep) copy constructor.
        reference(x);
    } // end if
}
//
// Change the grid to be all vertex-centered.
//
void CompositeGrid::changeToAllVertexCentered() {
    Logical isAllVertexCentered = LogicalTrue;
    for (Integer i=0; i<numberOfGrids(); i++)
      isAllVertexCentered =
      isAllVertexCentered && grid[i].isAllVertexCentered();
    if (!isAllVertexCentered)
      destroy(EVERYTHING & ~GridCollection::EVERYTHING);
    GridCollection::changeToAllVertexCentered();
}
//
// Change the grid to be all cell-centered.
//
void CompositeGrid::changeToAllCellCentered() {
    Logical isAllCellCentered = LogicalTrue;
    for (Integer i=0; i<numberOfGrids(); i++)
      isAllCellCentered =
      isAllCellCentered && grid[i].isAllCellCentered();
    if (!isAllCellCentered)
      destroy(EVERYTHING & ~GridCollection::EVERYTHING);
    GridCollection::changeToAllCellCentered();
}

//\begin{>>CompositeGridInclude.tex}{\subsubsection{changeInterpolationWidth}} 
int CompositeGrid::
changeInterpolationWidth( int width )
// ===========================================================
// /Description:
//   Reduce interpolation width of an already computed overlapping grid.
// For example you may want to use 2-point interpolation instead of 3-point
// interpolation. This routine will adjust the interpoleeLocation array
// and interpolationWidth array. It is currently not possible to reset the
// grid to it's original width (although this could be supported)
//.
// /width (input) : a positive integer. The interpolation with can only be
// decreased.
//\end{CompositeGridInclude.tex}
// ===========================================================
{
  assert( width>=1 );
  
  const IntegerArray & iw0 = interpolationWidth();
  IntegerArray & iw = (IntegerArray &) iw0;
  
  int g;
  for( g=0; g<numberOfComponentGrids(); g++ )
  {
    if( numberOfInterpolationPoints(g)>0 )
    {
      // *wdh* 061213 -- use local arrays ---
     #ifdef USE_PPP
      intSerialArray vWidth; getLocalArrayWithGhostBoundaries(variableInterpolationWidth[g],vWidth);
      intSerialArray il;     getLocalArrayWithGhostBoundaries(interpoleeLocation[g],il);
      intSerialArray ig;     getLocalArrayWithGhostBoundaries(interpoleeGrid[g],ig);
      realSerialArray ci;    getLocalArrayWithGhostBoundaries(interpolationCoordinates[g],ci);
     #else
      intSerialArray & vWidth = variableInterpolationWidth[g];
      intSerialArray & il = interpoleeLocation[g];
      intSerialArray & ig = interpoleeGrid[g];
      const realSerialArray & ci = interpolationCoordinates[g];
     #endif

      int indexPosition;
      real relativeOffset,px;
      const int iBase=il.getBase(0), iBound=il.getBound(0);
      for( int i=iBase; i<=iBound; i++ )
      {
	int oldWidth=vWidth(i);
	if( width < oldWidth )
	{
	  vWidth(i)=width;  // new width

	  int gridi = ig(i);   // **** could vectorize this loop since list is sorted by interpolee
	  MappedGrid & cgridi = (*this)[gridi];

	  for( int axis=axis1; axis<numberOfDimensions(); axis++ ) 
	  {
	    indexPosition=il(i,axis);
	    relativeOffset=ci(i,axis)/cgridi.gridSpacing(axis)+cgridi.indexRange(Start,axis);
	    // for 3-pt interpolation : 0<= px <=3 and normally .5<= px <= 2.5 if centred.
	    px= cgridi.isCellCentered(axis)  ? relativeOffset-indexPosition-.5  : relativeOffset-indexPosition;

	    //......interpolation width less than maximum allowed
	    if( px > width/2. )
	    {
	      // we need to increase the interpoleeLocation
	      int ipx=min(int(px-(width-2)/2.),oldWidth-width);
	      il(i,axis)+=ipx;

	      // printf("grid=%i, i=%i gridi=%i, axis=%i px=%8.2e shift=%i\n",g,i,gridi,axis,px,ipx);
	    }
	  }
	}
      }
    }
  } // end for g
  iw=min(iw,width);
  
  return 0;
}



//
// Check that the data structure is self-consistent.
//
void CompositeGrid::consistencyCheck() const {
    GridCollection::consistencyCheck();
    if (rcData != GridCollection::rcData) {
        cerr << className << "::consistencyCheck():  "
             << "rcData != GridCollection::rcData for "
             << getClassName() << " " << getGlobalID() << "." << endl;
        assert(rcData == GridCollection::rcData);
    } // end if
    numberOfInterpolationPoints      .Test_Consistency();
    numberOfImplicitInterpolationPoints.Test_Consistency();
    interpolationStartEndIndex.Test_Consistency();

//    numberOfInterpoleePoints         .Test_Consistency();
    interpolationIsImplicit          .Test_Consistency();
//    backupInterpolationIsImplicit    .Test_Consistency();
    interpolationWidth               .Test_Consistency();
//    backupInterpolationWidth         .Test_Consistency();
    interpolationOverlap             .Test_Consistency();
    maximumHoleCuttingDistance       .Test_Consistency();
//    backupInterpolationOverlap       .Test_Consistency();
//    interpolationConditionLimit      .Test_Consistency();
//    backupInterpolationConditionLimit.Test_Consistency();
    interpolationPreference          .Test_Consistency();
    mayInterpolate                   .Test_Consistency();
//    mayBackupInterpolate             .Test_Consistency();
    mayCutHoles                      .Test_Consistency();
    sharedSidesMayCutHoles           .Test_Consistency();
    multigridCoarseningRatio         .Test_Consistency();
    multigridProlongationWidth       .Test_Consistency();
    multigridRestrictionWidth        .Test_Consistency();
//    interpoleeGridRange              .Test_Consistency();
    interpolationCoordinates         .consistencyCheck();
    interpoleeGrid                   .consistencyCheck();
    variableInterpolationWidth       .consistencyCheck();
//    interpoleePoint                  .consistencyCheck();
    interpoleeLocation               .consistencyCheck();
    interpolationPoint               .consistencyCheck();
//    interpolationCondition           .consistencyCheck();
    multigridLevel                   .consistencyCheck();
    domain                           .consistencyCheck();
//    inverseCondition                 .consistencyCheck();
    inverseCoordinates               .consistencyCheck();
    inverseGrid                      .consistencyCheck();
}
// Here is the master grid.
CompositeGrid & CompositeGrid::
masterGridCollection()
{
  assert( master!=0 );
  return *master;
}


//
// "Get" and "put" database operations.
//
Integer CompositeGrid::
get( const GenericDataBase& db,
     const aString&         name) 
{
  Integer returnValue = rcData->get(db, name);
  updateReferences();

  // printF("CompositeGrid::get: numberOfMultigridLevels=%i\n",numberOfMultigridLevels());
  if( numberOfMultigridLevels()>1 )
  {
    // -- Fill in the gridDistributions on all MG levels --
    // printF("INFO: CompositeGrid::get: assign gridDistributions on MG levels\n");
    GridDistributionList & gridDistributionList = rcData->gridDistributionList;
    for( int l=0; l<numberOfMultigridLevels(); l++ )
    {
      GridCollection & cgl = multigridLevel[l];
      GridDistributionList & gdl = cgl->gridDistributionList;
      gdl.resize(cgl.numberOfComponentGrids());

      // printF("INFO: CompositeGrid::get: level=%i numberOfComponentGrids=%i\n",l,cgl.numberOfComponentGrids());
      for( int grid=0; grid<cgl.numberOfComponentGrids(); grid++ )
      {
  	const int masterGridNumber = cgl.gridNumber(grid); // here is the grid number in the master list of grids
  	gdl[grid]=gridDistributionList[masterGridNumber];
      }
    }
  }

  if( false ) 
    displayDistribution("CompositeGrid:get");

  return returnValue;
}

Integer CompositeGrid::
put( GenericDataBase& db,
     const aString&   name,
     int geometryToPut /* = -1  */    ) const
// =========================================================================================
// /Description:
//     Save a CompositeGrid to a data base file
//
// /db (input) : save to this data base
// /name (input) : name to save as in the data base
// /geometryToPut (input) : specify which geometry to put, by default put computedGeometry
// =========================================================================================
{ 
  // --- First convert serial array (local) interpolation data to parallel array form ---
  // printF("++++ CG:put: numberOfMultigridLevels=%i\n",numberOfMultigridLevels());

  CompositeGrid & cg = (CompositeGrid&)(*this); // (cast away const)
  cg->convertLocalInterpolationData();

  for( int l=1; l<numberOfMultigridLevels(); l++ )
  {
    // -- also convert interp. data on all MG levels *wdh* 2013/08/31 --
    CompositeGrid & cgl = cg.multigridLevel[l];
    
    // printF("CG:put: level=%i, localInterpolationDataState=%i (localInterpolationDataForAll=%i)\n",
    //       l,(int)cg->localInterpolationDataState,
    //       (int)CompositeGridData::localInterpolationDataForAll);

    cgl->convertLocalInterpolationData();

    // Now reference parallel interp data to to master lists
    for( int grid=0; grid<cg.numberOfComponentGrids(); grid++ )
    {
      const int masterGrid = cgl.gridNumber(grid); // here is the grid number in the master list of grids
      // printF("CG:put: level=%i reference grid=%i to masterGrid=%i\n",l,grid,masterGrid);
      
      interpoleeGrid[masterGrid].reference(cgl.interpoleeGrid[grid]);
      interpoleeLocation[masterGrid].reference(cgl.interpoleeLocation[grid]);
      interpolationPoint[masterGrid].reference(cgl.interpolationPoint[grid]);
      interpolationCoordinates[masterGrid].reference(cgl.interpolationCoordinates[grid]);
      variableInterpolationWidth[masterGrid].reference(cgl.variableInterpolationWidth[grid]);
      
    }
    

  }


  return rcData->put(db, name, geometryToPut); 
}

//
// Set references to reference-counted data.
//
void CompositeGrid::updateReferences(const Integer what) {
    GridCollection::reference(*rcData);
#define REFERENCE(x) x.reference(rcData->x)
#define REF_ARRAY(x) \
    if (x.getDataPointer() != rcData->x.getDataPointer()) REFERENCE(x)
    REF_ARRAY(numberOfInterpolationPoints);
    REF_ARRAY(numberOfImplicitInterpolationPoints);
    REF_ARRAY(interpolationStartEndIndex);

//    REF_ARRAY(numberOfInterpoleePoints);
    REF_ARRAY(interpolationIsImplicit);
//    REF_ARRAY(backupInterpolationIsImplicit);
    REF_ARRAY(interpolationWidth);
//    REF_ARRAY(backupInterpolationWidth);
    REF_ARRAY(interpolationOverlap);
    REF_ARRAY(maximumHoleCuttingDistance);
   
//    REF_ARRAY(backupInterpolationOverlap);
//    REF_ARRAY(interpolationConditionLimit);
//    REF_ARRAY(backupInterpolationConditionLimit);
    REF_ARRAY(interpolationPreference);
    REF_ARRAY(mayInterpolate);
//    REF_ARRAY(mayBackupInterpolate);
    REF_ARRAY(mayCutHoles);
    REF_ARRAY(sharedSidesMayCutHoles);
    REF_ARRAY(multigridCoarseningRatio);
    REF_ARRAY(multigridProlongationWidth);
    REF_ARRAY(multigridRestrictionWidth);
//    REF_ARRAY(interpoleeGridRange);
    REFERENCE(interpolationCoordinates);
    REFERENCE(interpoleeGrid);
    REFERENCE(variableInterpolationWidth);
//    REFERENCE(interpoleePoint);
    REFERENCE(interpoleeLocation);
    REFERENCE(interpolationPoint);
//    REFERENCE(interpolationCondition);
    REFERENCE(multigridLevel);
    REFERENCE(domain);
//    REFERENCE(inverseCondition);
    REFERENCE(inverseCoordinates);
    REFERENCE(inverseGrid);
#undef REFERENCE
#undef REF_ARRAY
#define SET_GRID(x) if (x.gridCollectionData == rcData) x.gridCollection = this
//    SET_GRID(inverseCondition);
    SET_GRID(inverseCoordinates);
    SET_GRID(inverseGrid);
#undef SET_GRID
    GridCollection::updateReferences(what);

    int i;
#ifdef USE_STL
/* is this correct? */
#define FOR_COLLECTION(X) \
  for( i=list.begin(); i<=list.begin(); i++ ) \
     X[i].master=this;
#else
#define FOR_COLLECTION(X) \
  for( i=0; i<X.getLength(); i++ ) \
    X[i].master=this;
#endif
  FOR_COLLECTION(multigridLevel);
  FOR_COLLECTION(domain);
#undef FOR_COLLECTION
}
//
// Update the grid, sharing the data of another grid.
//
Integer CompositeGrid::update(
  GenericGridCollection& x,
  const Integer          what,
  const Integer          how)
{
  Integer upd = rcData->update(*((CompositeGrid&)x).rcData, what, how);
  updateReferences(what);

  // We need to assign the mappedGrid in each geometry array *wdh*
  CompositeGrid & cg = (CompositeGrid&)x;
  for( int k=0; k<cg.numberOfGrids(); k++ )
    cg[k].updateMappedGridPointers(what);
    
  return upd;
}
//
// Destroy optional grid data.
//
void CompositeGrid::destroy(const Integer what) {
    rcData->destroy(what);
    updateReferences();
}


//\begin{>>CompositeGridInclude.tex}{\subsubsection{add(Mapping)}}
int CompositeGrid::
add(Mapping & map)
// ==========================================================================
// /Description: 
//   Add a new grid, built from a Mapping
//\end{CompositeGridInclude.tex}
//==========================================================================
{
  MappedGrid g(map);
  if( numberOfGrids()>0 && g.getGridType()!=GenericGrid::unstructuredGrid )
  {
    // set number of ghost points equal to that from the previous grid.
    MappedGrid & mg = (*this)[numberOfGrids()-1];
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( int side=Start; side<=End; side++ )
      {
	g.setNumberOfGhostPoints(side,axis,mg.numberOfGhostPoints(side,axis));
        // printf("CompositeGrid::add: set ghsot points to %i\n",mg.numberOfGhostPoints(side,axis));
      }
    
  }
  return add(g);
}
    
//\begin{>>CompositeGridInclude.tex}{\subsubsection{add(MappedGrid)}}
int CompositeGrid::
add(MappedGrid & g)
// ==========================================================================
// /Description: 
//    Add a new grid. The grid collection will keep a reference to g.
//\end{CompositeGridInclude.tex}
//==========================================================================
{
  int returnValue;
  const int numberOfOldGrids=numberOfGrids();
  
  returnValue = GridCollection::add(g);
  updateReferences();

  // update some of the parameter arrays:
  // interpolationWidth

  const int l = 0; // mg level
  const int n=numberOfOldGrids;
  Range G=numberOfOldGrids, all;
  
  numberOfInterpolationPoints(n) = 0;
  if( numberOfOldGrids>0 )
  {
    const int n0=n-1; // assign new values from this grid, choose the last one
    rcData->interpolationIsImplicit(G,n,l)= rcData->interpolationIsImplicit(G,n0,l);
    rcData->interpolationIsImplicit(n,G,l)= rcData->interpolationIsImplicit(n0,G,l);
    rcData->interpolationIsImplicit(n,n,l)= rcData->interpolationIsImplicit(n0,n0,l);

    for (int axis=0; axis<3; axis++) 
    {
      rcData->interpolationWidth(axis,G,n,l) = rcData->interpolationWidth(axis,G,n0,l);
      rcData->interpolationWidth(axis,n,G,l) = rcData->interpolationWidth(axis,n0,G,l);
      rcData->interpolationWidth(axis,n,n,l) = rcData->interpolationWidth(axis,n0,n0,l);


      rcData->interpolationOverlap(axis,G,n,l) = rcData->interpolationOverlap(axis,G,n0,l);
      rcData->interpolationOverlap(axis,n,G,l) = rcData->interpolationOverlap(axis,n0,G,l);
      rcData->interpolationOverlap(axis,n,n,l) = rcData->interpolationOverlap(axis,n0,n0,l);
    
      rcData->multigridCoarseningRatio(axis,n,l) = rcData->multigridCoarseningRatio(axis,n0,l);
      rcData->multigridProlongationWidth(axis,n,l) = rcData->multigridProlongationWidth(axis,n0,l);
      rcData->multigridRestrictionWidth(axis,n,l) = rcData->multigridRestrictionWidth(axis,n0,l);

    } 
    rcData->maximumHoleCuttingDistance(all,all,n)=rcData->maximumHoleCuttingDistance(all,all,n0);
  }
  else
  {
    // this is the first grid in the collection, use default values:
    rcData->interpolationIsImplicit(G,n,l)= true;
    rcData->interpolationIsImplicit(n,G,l)= true;
    rcData->interpolationIsImplicit(n,n,l)= true;

    for (int axis=0; axis<3; axis++) 
    {
      rcData->interpolationWidth(axis,G,n,l) = 3;
      rcData->interpolationWidth(axis,n,G,l) = 3;
      rcData->interpolationWidth(axis,n,n,l) = 3;


      rcData->interpolationOverlap(axis,G,n,l) = 0.;
      rcData->interpolationOverlap(axis,n,G,l) = 0.;
      rcData->interpolationOverlap(axis,n,n,l) = 0.;
    
      rcData->multigridCoarseningRatio(axis,n,l) = 2;
      rcData->multigridProlongationWidth(axis,n,l) = 3;
      rcData->multigridRestrictionWidth(axis,n,l) = 3;

    } 
    rcData->maximumHoleCuttingDistance(all,all,n)=SQRT(.1*REAL_MAX);
  }

  rcData->mayInterpolate(n,G,l) = true;
  rcData->mayInterpolate(G,n,l) = true;
  rcData->mayInterpolate(n,n,l) = true;

  rcData->interpolationPreference(n,G,l) = -1; // what should this be ?
  rcData->interpolationPreference(G,n,l) = -1;
  rcData->interpolationPreference(n,n,l) = -1;

  rcData->mayCutHoles(n,G)=true;
  rcData->mayCutHoles(G,n)=true;

  rcData->sharedSidesMayCutHoles(n,G)=false;
  rcData->sharedSidesMayCutHoles(G,n)=false;
  

  return returnValue;
}


    // delete a grid:
int CompositeGrid::
deleteGrid(Integer k)
{
  IntegerArray gridsToDelete(1);
  gridsToDelete=k;

  return deleteGrid(gridsToDelete);

}

// delete a list of grids:
int CompositeGrid::
deleteGrid(const IntegerArray & gridsToDelete )
{
  // see GenericGridCollection::deleteMultigridLevels
  int numberToDelete=gridsToDelete.getLength(0);
  if( numberToDelete==0 )
  {
    printf("CompositeGrid::deleteGrid:WARNING: no grids were specified to be deleted\n");
    return -1;
  }
  int newNumberOfGrids=numberOfGrids()-numberToDelete;
  if( newNumberOfGrids<0 )
  {
    printf("CompositeGrid::deleteGrid:ERROR:trying to delete %i grids, but the collection only has %i grids\n",
	   numberToDelete,numberOfGrids());
    return 1;
  }
  if( min(gridsToDelete)<0 || max(gridsToDelete)>=numberOfGrids() )
  {
    printf("CompositeGrid::deleteGrid:ERROR:trying to delete an invalid grid. There are %i grids\n",numberOfGrids());
    gridsToDelete.display("gridsToDelete");
    Overture::abort("error");
  }

  display(gridsToDelete,"CompositeGrid::deleteGrid: gridsToDelete");

  // make a list of grids that remain:
  IntegerArray save(numberOfGrids()),ia,gridsToSave(numberOfGrids()-numberToDelete);
  save.seqAdd(0,1);        // all grids: 0,1,2,3,...
  save(gridsToDelete)=-1;  // mark deleted grids
  ia=(save>=0).indexMap(); // list of remaining grids
  gridsToSave=save(ia);    // compressed list of remaining grids

  display(gridsToSave,"gridsToSave");

  int returnValue=deleteGrid(gridsToDelete,gridsToSave);

  return returnValue;
}

// delete a list of grids (use this when you also know the list of grids to save).
int CompositeGrid::
deleteGrid(const IntegerArray & gridsToDelete, const IntegerArray & gridsToSave )
// should this function be protected?
{
  int returnValue=0;
  // shift values before deleting the grids.

  // we need to re-assign values in the arrays found in setNumberOfDimensionsAndGrids
  // before they are reshaped to be a smaller size. 
  // Note that a(3,4) and b= a.reshape(2,3) will satisfy a(0:1,0:2)==b(0:1,0:2)
  //
  int numberToDelete=gridsToDelete.getLength(0);
  int newNumberOfGrids=gridsToSave.getLength(0);

  if( min(gridsToDelete) > max(gridsToSave) )
  {
    // not need to shift parameters values since grids to be deleted are on
    // the end of the list.
  }
  else
  { 
    // shift parameters around in preparation for grids to be deleted.
    // These arrays will be resized below.

    Range R=newNumberOfGrids;

//     // do this until A++ is fixed.
//     IntegerArray temp(R,R);
//     for( int k=0; k<newNumberOfGrids; k++ )
//       temp(R,k)=mayCutHoles(gridsToSave,gridsToSave(k));
//     mayCutHoles(R,R)=temp;


    numberOfInterpolationPoints(R)=numberOfInterpolationPoints(gridsToSave)*1;
    numberOfImplicitInterpolationPoints(R)=numberOfImplicitInterpolationPoints(gridsToSave)*1;
    int i,j,k;
    for( i=0; i<4; i++ )
      for( j=0; j<newNumberOfGrids; j++ )
      for( k=0; k<newNumberOfGrids; k++ )
        interpolationStartEndIndex(i,j,k)=interpolationStartEndIndex(i,gridsToSave(j),gridsToSave(k));

    for( i=0; i<3; i++ )
      for( j=0; j<2; j++ )
      for( k=0; k<newNumberOfGrids; k++ )
	maximumHoleCuttingDistance(i,j,k)=maximumHoleCuttingDistance(i,j,gridsToSave(k));       


    for( j=0; j<newNumberOfGrids; j++ )
      for( k=0; k<newNumberOfGrids; k++ )
      {
	mayCutHoles(j,k)=mayCutHoles(gridsToSave(j),gridsToSave(k));
	sharedSidesMayCutHoles(j,k)=sharedSidesMayCutHoles(gridsToSave(j),gridsToSave(k));
      }
    
    for( int l=0; l<numberOfMultigridLevels(); l++ )
    {
      for( j=0; j<newNumberOfGrids; j++ )
      for( k=0; k<newNumberOfGrids; k++ )
        interpolationIsImplicit(j,k,l)=interpolationIsImplicit(gridsToSave(j),gridsToSave(k),l);
      for( i=0; i<3; i++ )
      {
        for( j=0; j<newNumberOfGrids; j++ )
        for( k=0; k<newNumberOfGrids; k++ )
	{
	  interpolationWidth(i,j,k,l)=interpolationWidth(i,gridsToSave(j),gridsToSave(k),l);
	  interpolationOverlap(i,j,k,l)=interpolationOverlap(i,gridsToSave(j),gridsToSave(k),l);
	}
	for( j=0; j<newNumberOfGrids; j++ )
	{
	  multigridCoarseningRatio(i,j,l)=multigridCoarseningRatio(i,gridsToSave(j),l);
	  multigridProlongationWidth(i,j,l)=multigridProlongationWidth(i,gridsToSave(j),l);
	  multigridRestrictionWidth (i,j,l)=multigridRestrictionWidth(i,gridsToSave(j),l);
	}
      }
      for( j=0; j<newNumberOfGrids; j++ )
      for( k=0; k<newNumberOfGrids; k++ )
      {
	interpolationPreference(j,k,l)=interpolationPreference(gridsToSave(j),gridsToSave(k),l);
	mayInterpolate(j,k,l)=mayInterpolate(gridsToSave(j),gridsToSave(k),l);
      }
      
    }

    // printf("CompositeGrid::deleteGrid:WARNING: This case is not finished\n");
  }

  returnValue=GridCollection::deleteGrid(gridsToDelete,gridsToSave); // this will resize parameter arrays.
  updateReferences();

  printf("CompositeGrid::deleteGrids: newNumberOfGrids=%i, numberOfGrids=%i, numberOfComponentGrids=%i \n",
        newNumberOfGrids,numberOfGrids(),numberOfComponentGrids());
  

  return returnValue;
}


//
// Add a refinement grid to the collection.
//
Integer CompositeGrid::
addRefinement(const IntegerArray& range,  // The indexRange of the refinement grid.
	      const IntegerArray& factor, // The refinement factor w.r.t. level-1.
	      const Integer&      level,  // The refinement level number.
	      const Integer       k)     // The index of an ancestor of the refinement.
{
  Range G=numberOfComponentGrids();  // original number of grids
  
  Integer n = rcData->addRefinement(range, factor, level, k);

  updateReferences();
  return n;
}


//! Replace refinement levels "level0" and higher
/*!
  This function is used by the AMR Regrid class in order to efficiently replace a collection
  of refinement grids. This function avoids the overhead of calling addRefinement and deleteRefinement
  many times.

 \param level0,numberOfRefinementLevels0 : replace and/or add levels level0,..,numberOfRefinementLevels0-1
 \param gridInfo[bg][lev](0:ni-1,0:ng-1) : info defining a new refinement grid on base grid bg 
       and refinement level=level0+lev, lev=0,1,.... If we let 
             IntegerArray & info = gridInfo[bg][lev]
       then the number of new refinement grids is given by info.getLength(1).
       The first 6 entries in info define the range(0:1,0:2) of the refinement grid and the
       next three entries define the refinement factors along each axis,
          info(0,g) = range(0,0)
          info(1,g) = range(1,0)
          info(2,g) = range(0,1)
          info(3,g) = range(1,1)
          info(4,g) = range(0,2)
          info(5,g) = range(1,2)
          info(6,g) = factor(0)    
          info(7,g) = factor(1) 
          info(8,g) = factor(2) 

 */
Integer CompositeGrid::
replaceRefinementLevels(int level0, int numberOfRefinementLevels0, IntegerArray **gridInfo )
{
  int returnValue=rcData->replaceRefinementLevels(level0,numberOfRefinementLevels0,gridInfo );
  updateReferences();
  return returnValue;
}


//
// Delete all multigrid levels of refinement grid k.
//
void CompositeGrid::
deleteRefinement(const Integer& k)
{
   rcData->deleteRefinement(k);
   updateReferences();
}

//
// Delete all grids with refinement level greater than the given level.
//
void CompositeGrid::deleteRefinementLevels(const Integer level) {
    rcData->deleteRefinementLevels(level);
    updateReferences();
}
//
// Reference x[i] for refinementLevelNumber(i) <= level.
// Delete all other grids.
//
void CompositeGrid::referenceRefinementLevels(
  GenericGridCollection& x,
  const Integer          level) {
    rcData->referenceRefinementLevels(*((CompositeGrid&)x).rcData, level);
    updateReferences();
}

// ===============================================================================================
/// \brief Return the number of possible multigrid levels supported by this grid. 
/// \details Return the number of possible multigrid levels supported by the current numbers of grid lines
///   on the different grids. We assume that the grids are coarsened by a factor of 2. Any grid
///  has at least one multigrid level (the finest level).
// *wdh* 2011/08/20
// ===============================================================================================
Integer CompositeGrid::
numberOfPossibleMultigridLevels() const
{
  const CompositeGrid & cg = *this;
  int maxLevels=INT_MAX;
  const int minimumNumberOfCellsOnCoarseGrid=2; // what should this be ??
  for( int grid=0; grid<numberOfComponentGrids(); grid++ )
  {
    const IntegerArray & gridIndexRange = cg[grid].gridIndexRange();
    for( int axis=0; axis<numberOfDimensions(); axis++ )
    {
      // Determine how factors of 2 appear in the number of grid cells.
      int n = gridIndexRange(End,axis)-gridIndexRange(Start,axis);  // number of grid cells
      int pow2=0;
      while( ( n % 2 )==0 && n>minimumNumberOfCellsOnCoarseGrid )
      {
	pow2++;
	n/=2;
      }
      maxLevels=min(maxLevels,pow2);
    }
    if( maxLevels==0 ) break;
  }
  // We count the finest level as a MG level so add one:
  maxLevels=maxLevels+1;
  return maxLevels;
}



//
// Add a multigrid coarsening of grid k.
//
Integer CompositeGrid::addMultigridCoarsening(
  const IntegerArray& factor, // The coarsening factor w.r.t level-1
  const Integer&      level,  // The multigrid level number.
  const Integer       k) {    // The index of the corresponding grid
                              // at any finer multigrid level.
    Integer n = rcData->addMultigridCoarsening(factor, level, k);
    updateReferences();
    return n;
}
//
// Add multigrid coarsenings of grids in order to complete the multigrid levels.
//
void CompositeGrid::makeCompleteMultigridLevels() {
    rcData->makeCompleteMultigridLevels();
    updateReferences();
}
//
// Delete grid k, a multigrid coarsening, and all of its multigrid coarsenings.
//
void CompositeGrid::deleteMultigridCoarsening(const Integer& k) {
    rcData->deleteMultigridCoarsening(k);
    updateReferences();
}
//
// Delete all of the grids with multigrid level greater than the given level.
//
void CompositeGrid::deleteMultigridLevels(const Integer level) {
    rcData->deleteMultigridLevels(level);
    updateReferences();
}
//
// Set the number of grids.
//
void CompositeGrid::setNumberOfGrids(const Integer& numberOfGrids_) {
    rcData->setNumberOfGrids(numberOfGrids_);
    updateReferences();
}
//
// Set the number of dimensions.
//
void CompositeGrid::setNumberOfDimensions(
  const Integer& numberOfDimensions_) {
    rcData->setNumberOfGrids(numberOfDimensions_);
    updateReferences();
}
//
// Set the number of dimensions and grids.
//
void CompositeGrid::setNumberOfDimensionsAndGrids(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) {
    rcData->setNumberOfDimensionsAndGrids(numberOfDimensions_, numberOfGrids_);
    updateReferences();
}

//\begin{>>CompositeGridInclude.tex}{\subsubsection{sizeOf}}
real CompositeGrid::
sizeOf(FILE *file /* = NULL */, bool returnSizeOfReference /* = false */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this grid; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// write to standard output.
// /returnSizeOfReference (input): if true only count the items that would not be referenced if this
//   CompositeGrid were referenced to another.
// /Return value: the number of bytes.
//\end{CompositeGridInclude.tex}
//==========================================================================
{
  real size=sizeof(*this);
  size+=GridCollection::sizeOf(file,returnSizeOfReference)-sizeof(GridCollection);
  
  size+=numberOfInterpolationPoints.elementCount()*sizeof(int);
  size+=numberOfImplicitInterpolationPoints.elementCount()*sizeof(int);
  size+=interpolationStartEndIndex.elementCount()*sizeof(int);
  size+=interpolationIsImplicit.elementCount()*sizeof(int);
  size+=interpolationWidth.elementCount()*sizeof(int);
  size+=interpolationPreference.elementCount()*sizeof(int);
  size+=mayInterpolate.elementCount()*sizeof(int);
  size+=mayCutHoles.elementCount()*sizeof(int);
  size+=sharedSidesMayCutHoles.elementCount()*sizeof(int);
  size+=multigridCoarseningRatio.elementCount()*sizeof(int);
  size+=multigridProlongationWidth.elementCount()*sizeof(int);
  size+=multigridRestrictionWidth.elementCount()*sizeof(int);

  size+=interpolationOverlap.elementCount()*sizeof(real);
  size+=maximumHoleCuttingDistance.elementCount()*sizeof(real);

  if( numberOfGrids()>0 )
  {
    for( int g=0; g<numberOfGrids(); g++ )
    {
      if( g<interpolationCoordinates.getLength() )
        size+=interpolationCoordinates[g].elementCount()*sizeof(real);
      if( g<interpoleeGrid.getLength() )
        size+=interpoleeGrid[g].elementCount()*sizeof(int);
      if( g<variableInterpolationWidth.getLength() )
        size+=variableInterpolationWidth[g].elementCount()*sizeof(int);
//      if( g<interpoleePoint.getLength() )
//	size+=interpoleePoint[g].elementCount()*sizeof(int);
      if( g<interpoleeLocation.getLength() )
	size+=interpoleeLocation[g].elementCount()*sizeof(int);
      if( g<interpolationPoint.getLength() )
	size+=interpolationPoint[g].elementCount()*sizeof(int);
//       if( g<interpolationCondition.getLength() )
//         size+=interpolationCondition[g].elementCount()*sizeof(real);
    }
  }

  // what about multigridLevel ?? what arrays are not shared?

  size+=inverseCoordinates.sizeOf();
  size+=inverseGrid.sizeOf();
  if( rcData!=NULL )
  {
    size+=rcData->hybridConnectivity.sizeOf();

    typedef TrivialArray<BoundaryAdjustment,Range>     BoundaryAdjustmentArray;
    typedef TrivialArray<BoundaryAdjustmentArray,Range>BoundaryAdjustmentArray2;
    BoundaryAdjustmentArray2 & boundaryAdjustment = rcData->boundaryAdjustment;
    if( boundaryAdjustment.getNumberOfElements() ) 
    {
      for( int k1=0; k1<numberOfBaseGrids(); k1++ )  // switched to base grids 040316
	for( int k2=0; k2<numberOfBaseGrids(); k2++ )
	{
	  BoundaryAdjustmentArray& bA12 = boundaryAdjustment(k1,k2);
	  if( bA12.getNumberOfElements()>0 ) 
	  {
	    for( int axis=0; axis<numberOfDimensions(); axis++ )
	    for( int side=0; side<=1; side++ )
	    {
	      BoundaryAdjustment& bA = bA12(side,axis);
              size+=bA.sizeOf();
	    }
	  }
	}
    }
  }
  
  if( multigridLevel.getLength()>0 )
  {
    for( int l=0; l<numberOfMultigridLevels(); l++ )
    {
      size+=multigridLevel[l].sizeOf(file,true);  // count up data that is not referenced.
    }
  }
  

  return size;
}

//\begin{>>CompositeGridInclude.tex}{\subsubsection{saveGridToAFile}}
int CompositeGrid::
saveGridToAFile(const aString & gridFileName, const aString & gridName ) 
// =======================================================================================
// /Description:
//   This function will output a CompositeGrid to a file.
//
// /cg (input) : grid to save
// /gridFileName : name of the file to save such as "myGrid.hdf"
// /gridName : save the grid under this name in the data base file.
//
//\end{CompositeGridInclude.tex}
// =========================================================================================
{
  printF("CompositeGrid::Saving the CompositeGrid in %s\n",(const char*)gridFileName);

  HDF_DataBase dataFile;
  dataFile.mount(gridFileName,"I");

  int streamMode=1; // save in compressed form.
  dataFile.put(streamMode,"streamMode");
  if( !streamMode )
    dataFile.setMode(GenericDataBase::noStreamMode); // this is now the default
  else
  {
    dataFile.setMode(GenericDataBase::normalMode); // need to reset if in noStreamMode
  }
           
  destroy(MappedGrid::EVERYTHING & ~MappedGrid::THEmask );
  put(dataFile,gridName);
    
  dataFile.unmount();


  return 0;
}


//\begin{>>CompositeGridInclude.tex}{\subsubsection{setOverlapParameters}}
int CompositeGrid::
setOverlapParameters()
// ==========================================================================
// /Description: 
//    Assign default values to the overlap parameters such as interpolationWidth etc.
//
//\end{CompositeGridInclude.tex}
//==========================================================================
{
  Range all;
  int k1;
  for( k1=0; k1<numberOfGrids(); k1++) 
  {
    maximumHoleCuttingDistance(all,all,k1)=SQRT(.1*REAL_MAX); // this will be squared
  }
    

  int l;
  for( l=0; l<numberOfMultigridLevels(); l++ )
  {
    for( k1=0; k1<numberOfGrids(); k1++) 
    {
      for (int k2=0; k2<numberOfGrids(); k2++) 
      {
	interpolationIsImplicit(k1,k2,l) = true;

        int axis;
	for( axis=0; axis<numberOfDimensions(); axis++ )
	{
	  interpolationWidth(axis,k1,k2,l)              =3;
	  interpolationOverlap(axis,k1,k2,l)            = .5;
	  multigridCoarseningRatio(axis,k1,l)           = 2;
	  multigridProlongationWidth(axis,k1,l)         = 3;
	  multigridRestrictionWidth(axis,k1,l)          = 3;
	}
	// note: may cut holes does not have multigrid levels.
	mayCutHoles(k1,k2)=TRUE;   
	sharedSidesMayCutHoles(k1,k2)=FALSE;
      
	for( axis=numberOfDimensions(); axis<3; axis++ )
	{
	  interpolationWidth(axis,k1,k2,l)              =1;
	  interpolationOverlap(axis,k1,k2,l)            = -.5;
	  multigridCoarseningRatio(axis,k1,l)           = 1;
	  multigridProlongationWidth(axis,k1,l)         = 1;
	  multigridRestrictionWidth(axis,k1,l)          = 1;
	}
      
	interpolationPreference(k1,k2,l)           = k1;
	mayInterpolate(k1,k2,l)                    = true;
	  
      } // for k2
    }
  }
  return 0;
}

//\begin{>>CompositeGridInclude.tex}{\subsubsection{setOverlapParameters}}
int CompositeGrid::
setOverlapParameters(CompositeGrid & cg)
// ==========================================================================
// /Description: 
//    Assign values to the overlap parameters such as interpolationWidth etc.
//  based on the values in the grid cg.
//
//\end{CompositeGridInclude.tex}
//==========================================================================
{
  if( cg.numberOfGrids()!=numberOfGrids() )
    setOverlapParameters();
  
  int numberOfGridsToSet=min(cg.numberOfGrids(),numberOfGrids());

  Range all;
  int k1;
  for( k1=0; k1<numberOfGridsToSet; k1++) 
  {
    maximumHoleCuttingDistance(all,all,k1)=cg.maximumHoleCuttingDistance(all,all,k1);
  }
    

  int l;
  for( l=0; l<numberOfMultigridLevels(); l++ )
  {
    for( k1=0; k1<numberOfGridsToSet; k1++) 
    {
      for (int k2=0; k2<numberOfGridsToSet; k2++) 
      {
	interpolationIsImplicit(k1,k2,l) = cg.interpolationIsImplicit(k1,k2,l);

        int axis;
	for( axis=0; axis<numberOfDimensions(); axis++ )
	{
	  interpolationWidth(axis,k1,k2,l)              =cg.interpolationWidth(axis,k1,k2,l);
	  interpolationOverlap(axis,k1,k2,l)            =cg.interpolationOverlap(axis,k1,k2,l);
	  multigridCoarseningRatio(axis,k1,l)           =cg.multigridCoarseningRatio(axis,k1,l);
	  multigridProlongationWidth(axis,k1,l)         =cg.multigridProlongationWidth(axis,k1,l);
	  multigridRestrictionWidth(axis,k1,l)          =cg.multigridRestrictionWidth(axis,k1,l);
	}
	// note: may cut holes does not have multigrid levels.
	mayCutHoles(k1,k2)=cg.mayCutHoles(k1,k2);
	sharedSidesMayCutHoles(k1,k2)=cg.sharedSidesMayCutHoles(k1,k2);
      
	for( axis=numberOfDimensions(); axis<3; axis++ )
	{
	  interpolationWidth(axis,k1,k2,l)              =1;
	  interpolationOverlap(axis,k1,k2,l)            = -.5;
	  multigridCoarseningRatio(axis,k1,l)           = 1;
	  multigridProlongationWidth(axis,k1,l)         = 1;
	  multigridRestrictionWidth(axis,k1,l)          = 1;
	}
      
	interpolationPreference(k1,k2,l)           = cg.interpolationPreference(k1,k2,l);
	mayInterpolate(k1,k2,l)                    = cg.mayInterpolate(k1,k2,l);
	  
      } // for k2
    }
  }
  return 0;
}



// Initialize the CompositeGrid with the given number of dimensions and grids.
// These grids have their gridNumbers, baseGridNumbers and componentGridNumbers
// set to [0, ..., numberOfGrids_-1], and their refinementLevelNumbers and
// multigridLevelNumbers set to zero.
//
void CompositeGrid::initialize(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) {
    GridCollection::initialize(numberOfDimensions_, numberOfGrids_);
    rcData->initialize(numberOfDimensions_, numberOfGrids_);
}
//
// Stream output operator.
//
ostream& operator<<(ostream& s, const CompositeGrid& g) {
    Integer i, k, k1, k2, l;
    s << (GridCollection&)g << endl
      << "  numberOfCompleteMultigridLevels() =  "
      <<  g.numberOfCompleteMultigridLevels() << endl
      << "  epsilon()                         =  "
      <<  g.epsilon() << endl
      << "  numberOfInterpolationPoints       = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "")
        << g.numberOfInterpolationPoints(i);
    s << "]" << endl
//      << "  numberOfInterpoleePoints          = [";
//    for (i=0; i<g.numberOfGrids(); i++)
//      s << (i ? "," : "")
//        << g.numberOfInterpoleePoints(i);
//    s << "]" << endl
      << "  interpolationIsAllExplicit()      =  "
      <<  (g.interpolationIsAllExplicit() ? 'T' : 'F') << endl
      << "  interpolationIsAllImplicit()      =  "
      <<  (g.interpolationIsAllImplicit() ? 'T' : 'F') << endl
      << "  interpolationIsImplicit           = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
            if (k2) s << ";";
            for (k1=0; k1<g.numberOfComponentGrids(); k1++)
              s << (k1 ? "," : "")
                << (g.interpolationIsImplicit(k1,k2,l) ? 'T' : 'F');
        } // end for
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
//       << "  backupInterpolationIsImplicit     = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << (g.backupInterpolationIsImplicit(k1,k2,l) ? 'T' : 'F');
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//    s << "]" << endl
      << "  interpolationWidth                = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
            if (k2) s << ";";
            for (k1=0; k1<g.numberOfComponentGrids(); k1++)
              s << (k1 ? "," : "")
                << g.interpolationWidth(0,k1,k2,l) << ":"
                << g.interpolationWidth(1,k1,k2,l) << ":"
                << g.interpolationWidth(2,k1,k2,l);
        } // end for
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
//       << "  backupInterpolationWidth          = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << g.backupInterpolationWidth(0,k1,k2,l) << ":"
//                 << g.backupInterpolationWidth(1,k1,k2,l) << ":"
//                 << g.backupInterpolationWidth(2,k1,k2,l);
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//    s << "]" << endl
      << "  interpolationOverlap              = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
            if (k2) s << ";";
            for (k1=0; k1<g.numberOfComponentGrids(); k1++)
              s << (k1 ? "," : "")
                << g.interpolationOverlap(0,k1,k2,l) << ":"
                << g.interpolationOverlap(1,k1,k2,l) << ":"
                << g.interpolationOverlap(2,k1,k2,l);
        } // end for
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
//       << "  backupInterpolationOverlap        = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << g.backupInterpolationOverlap(0,k1,k2,l) << ":"
//                 << g.backupInterpolationOverlap(1,k1,k2,l) << ":"
//                 << g.backupInterpolationOverlap(2,k1,k2,l);
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//     s << "]" << endl
//       << "  interpolationConditionLimit       = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << g.interpolationConditionLimit(k1,k2,l);
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//     s << "]" << endl
//       << "  backupInterpolationConditionLimit = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << g.backupInterpolationConditionLimit(k1,k2,l);
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//     s << "]" << endl
      << "  interpolationPreference           = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
            if (k2) s << ";";
            for (k1=0; k1<g.numberOfComponentGrids(); k1++)
              s << (k1 ? "," : "")
                << g.interpolationPreference(k1,k2,l);
        } // end for
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
      << "  mayInterpolate                    = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
            if (k2) s << ";";
            for (k1=0; k1<g.numberOfComponentGrids(); k1++)
              s << (k1 ? "," : "")
                << (g.mayInterpolate(k1,k2,l) ? 'T' : 'F');
        } // end for
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
//       << "  mayBackupInterpolate              = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << (g.mayBackupInterpolate(k1,k2,l) ? 'T' : 'F');
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
//     s << "]" << endl
      << "  mayCutHoles                       = [";
    for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
        if (k2) s << ";";
        for (k1=0; k1<g.numberOfComponentGrids(); k1++)
          s << (k1 ? "," : "")
            << (g.mayCutHoles(k1,k2) ? 'T' : 'F');
    } // end for
    s << "]" << endl
      << "  multigridCoarseningRatio          = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k=0; k<g.numberOfComponentGrids(); k++)
          s << (k ? "," : "")
            << g.multigridCoarseningRatio(0,k,l) << ":"
            << g.multigridCoarseningRatio(1,k,l) << ":"
            << g.multigridCoarseningRatio(2,k,l);
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
      << "  multigridProlongationWidth        = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k=0; k<g.numberOfComponentGrids(); k++)
          s << (k ? "," : "")
            << g.multigridProlongationWidth(0,k,l) << ":"
            << g.multigridProlongationWidth(1,k,l) << ":"
            << g.multigridProlongationWidth(2,k,l);
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
    s << "]" << endl
      << "  multigridRestrictionWidth         = [";
    for (l=0; l<g.numberOfMultigridLevels(); l++) {
        if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
        for (k=0; k<g.numberOfComponentGrids(); k++)
          s << (k ? "," : "")
            << g.multigridRestrictionWidth(0,k,l) << ":"
            << g.multigridRestrictionWidth(1,k,l) << ":"
            << g.multigridRestrictionWidth(2,k,l);
        if (g.numberOfMultigridLevels() > 1) s << ")";
    } // end for
//    s << "]" << endl
//       << "  interpoleeGridRange               = [";
//     for (l=0; l<g.numberOfMultigridLevels(); l++) {
//         if (g.numberOfMultigridLevels() > 1) s << (l ? ",(" : "(");
//         for (k2=0; k2<g.numberOfComponentGrids(); k2++) {
//             if (k2) s << ";";
//             for (k1=0; k1<=g.numberOfComponentGrids(); k1++)
//               s << (k1 ? "," : "")
//                 << g.interpoleeGridRange(k1,k2,l);
//         } // end for
//         if (g.numberOfMultigridLevels() > 1) s << ")";
//     } // end for
   return s
      << "]";
}

//
// class CompositeGridData:
//
CompositeGridData::
CompositeGridData(const Integer numberOfDimensions_,
		  const Integer numberOfComponentGrids_):
  GridCollectionData(numberOfDimensions_, numberOfComponentGrids_) 
{
  className = "CompositeGridData";
  surfaceStitching=NULL;
  partitionInitialized=false;
  initialize(numberOfDimensions_, numberOfComponentGrids_);
}

CompositeGridData::
CompositeGridData( const CompositeGridData& x,
		   const CopyType           ct):
  GridCollectionData() 
{
  className = "CompositeGridData";
  surfaceStitching=NULL;
  partitionInitialized=false;
  initialize(numberOfDimensions, numberOfGrids);
  if (ct != NOCOPY) *this = x;
}

CompositeGridData::
~CompositeGridData() 
{ 
  // kkc 030224 
  // get rid of the surface stitching 
  if ( surfaceStitching && surfaceStitching->decrementReferenceCount()==0 )
    delete surfaceStitching;

}

void 
CompositeGridData::initializePartition()
// =======================================================================================
// /Description:
//   Initialize the partition object.
// =======================================================================================
{
  if( !partitionInitialized )
  {
    // printf("***** CompositeGridData:: initialize the partition with address %d ***** \n",&partition);
    partitionInitialized=true;

    // For the interpolation arrays such as interpolationPoint or interpoleeLocation we only
    // partition the first dimension -- no ghost boundary points are needed
    int numberOfDimensionsToPartition=1;
    partition.SpecifyDecompositionAxes(numberOfDimensionsToPartition);
    const int numGhost=0;  
    for( int kd=0; kd<numberOfDimensionsToPartition; kd++ )
      partition.partitionAlongAxis(kd, true, numGhost );
    for( int kd=numberOfDimensionsToPartition; kd<MAX_ARRAY_DIMENSION; kd++ )
      partition.partitionAlongAxis(kd, false, 0);  }
}

void CompositeGrid::
specifyProcesses(const Range& range)
// =============================================================================
// /Description:
//    Specify the range of processors to use for the partioning of arrays
//    This will change the range of processors for all MappedGrid's too
// =============================================================================
{ 
  GridCollection::specifyProcesses(range);
  // rcData->initializePartition();
  #ifdef USE_PPP
  // rcData->partition.SpecifyProcessorRange(range); 
    rcData->partition.Internal_Partitioning_Object->Starting_Processor=range.getBase();
    rcData->partition.Internal_Partitioning_Object->Ending_Processor=range.getBound();
  #endif

}

CompositeGridData& CompositeGridData::
operator=(const CompositeGridData& x) 
{
  // printf("********CompositeGridData::operator= *************\n");

  GridCollectionData::operator=(x);
//    if( true )
//    {
//      for( int g=0; g<numberOfGrids; g++ )
//      {
//        printf("CompositeGridData operator=: after GCData::operator=(x) : grid=%i name=%s x::name=%s\n",g,
//                (const char*)grid[g].getName(),(const char*)x.grid[g].getName());
//      }
//    }
  
  numberOfCompleteMultigridLevels   = x.numberOfCompleteMultigridLevels;
  epsilon                           = x.epsilon;
  localInterpolationDataState       = x.localInterpolationDataState;
  
  numberOfInterpolationPoints.redim(0);
  numberOfInterpolationPoints       = x.numberOfInterpolationPoints;

  numberOfImplicitInterpolationPoints.redim(0); 
  numberOfImplicitInterpolationPoints=x.numberOfImplicitInterpolationPoints;
  
  interpolationStartEndIndex.redim(0);
  interpolationStartEndIndex=x.interpolationStartEndIndex;
  
//  numberOfInterpoleePoints.redim(0);
//  numberOfInterpoleePoints          = x.numberOfInterpoleePoints;
  interpolationIsAllExplicit        = x.interpolationIsAllExplicit;
  interpolationIsAllImplicit        = x.interpolationIsAllImplicit;
  interpolationIsImplicit.redim(0);
  interpolationIsImplicit           = x.interpolationIsImplicit;
//  backupInterpolationIsImplicit.redim(0);
//  backupInterpolationIsImplicit     = x.backupInterpolationIsImplicit;
  interpolationWidth.redim(0);
  interpolationWidth                = x.interpolationWidth;
//  backupInterpolationWidth.redim(0);
//  backupInterpolationWidth          = x.backupInterpolationWidth;
  interpolationOverlap.redim(0);
  interpolationOverlap              = x.interpolationOverlap;
  maximumHoleCuttingDistance.redim(0);
  maximumHoleCuttingDistance=x.maximumHoleCuttingDistance;
//  backupInterpolationOverlap.redim(0);
//  backupInterpolationOverlap        = x.backupInterpolationOverlap;
  interpolationPreference.redim(0);
  interpolationPreference           = x.interpolationPreference;
  mayInterpolate.redim(0);
  mayInterpolate                    = x.mayInterpolate;
//  mayBackupInterpolate.redim(0);
//  mayBackupInterpolate              = x.mayBackupInterpolate;
  mayCutHoles.redim(0);
  mayCutHoles                       = x.mayCutHoles;
  sharedSidesMayCutHoles.redim(0);
  sharedSidesMayCutHoles            = x.sharedSidesMayCutHoles;
  multigridCoarseningRatio.redim(0);
  multigridCoarseningRatio          = x.multigridCoarseningRatio;
  multigridProlongationWidth.redim(0);
  multigridProlongationWidth        = x.multigridProlongationWidth;
  multigridRestrictionWidth.redim(0);
  multigridRestrictionWidth         = x.multigridRestrictionWidth;
//  interpoleeGridRange.redim(0);
//  interpoleeGridRange               = x.interpoleeGridRange;
//  interpolationConditionLimit.redim(0);
//  interpolationConditionLimit       = x.interpolationConditionLimit;
//  backupInterpolationConditionLimit.redim(0);
//  backupInterpolationConditionLimit = x.backupInterpolationConditionLimit;

// ?? parition is not copied??   partitionInitialized        = x.partitionInitialized;

  Integer upd = NOTHING, des = NOTHING;
#ifdef USE_STL
  if (x.interpolationCoordinates.size())
    upd |= THEinterpolationCoordinates;
  else des |= THEinterpolationCoordinates;
  if (x.interpoleeGrid          .size())
    upd |= THEinterpoleeGrid;
  else des |= THEinterpoleeGrid;
//   if (x.variableInterpolationWidth.size())
//     upd |= THEvariableInterpolationWidth;
//   else des |= THEvariableInterpolationWidth;
  if (x.interpoleeLocation      .size())
    upd |= THEinterpoleeLocation;
  else des |= THEinterpoleeLocation;
  if (x.interpolationPoint      .size())
    upd |= THEinterpolationPoint;
  else des |= THEinterpolationPoint;
#else
  if (x.interpolationCoordinates.getLength())
    upd |= THEinterpolationCoordinates;
  else des |= THEinterpolationCoordinates;
  if (x.interpoleeGrid          .getLength())
    upd |= THEinterpoleeGrid;
  else des |= THEinterpoleeGrid;
//   if (x.variableInterpolationWidth.getLength())
//     upd |= THEvariableInterpolationWidth;
//   else des |= THEvariableInterpolationWidth;
  if (x.interpoleeLocation      .getLength())
    upd |= THEinterpoleeLocation;
  else des |= THEinterpoleeLocation;
  if (x.interpolationPoint      .getLength())
    upd |= THEinterpolationPoint;
  else des |= THEinterpolationPoint;
#endif // USE_STL
  if ( // x.inverseCondition  .gridCollectionData == (GridCollectionData *)(&x) &&
      x.inverseCoordinates.gridCollectionData == (GridCollectionData *)(&x) &&
      x.inverseGrid       .gridCollectionData == (GridCollectionData *)(&x))
    upd |= THEinverseMap; else des |= THEinverseMap;
  if (upd &= ~des) update(upd, COMPUTEnothing); if (des) destroy(des);
//
//****************************************************************
//***** Assume that the x data have the expected dimensions. *****
//*****          This is probably a bad assumption.          *****
//****************************************************************
//
//  #ifdef USE_PPP
//    Partitioning_Type & xPartition = (Partitioning_Type&)x.partition;
//    const intSerialArray & ps  = partition.getProcessorSet();
//    const intSerialArray & xps = xPartition.getProcessorSet();
//    // display(ps,"partition.getProcessorSet()");
//    // display(xps,"xPartition.getProcessorSet()");
   
//    const bool sameParallelDistribution = ps.getLength(0)==xps.getLength(0) && max(abs(ps-xps))==0;
//  #else
//    const bool sameParallelDistribution = true;
//  #endif
 const bool sameParallelDistribution = hasSameDistribution(partition,x.partition);

  if( x.localInterpolationDataState!=localInterpolationDataForAll )
  {
#ifdef USE_STL
    if (x.interpolationCoordinates.size())
      interpolationCoordinates = x.interpolationCoordinates;
    if (x.interpoleeGrid.size()) {
      interpoleeGrid         = x.interpoleeGrid;
      variableInterpolationWidth = x.variableInterpolationWidth;
    } // end if
    if (x.interpoleeLocation.size())
      interpoleeLocation       = x.interpoleeLocation;
    if (x.interpolationPoint.size())
      interpolationPoint       = x.interpolationPoint;
#else
    if( x.interpolationCoordinates.getLength() )
    {
      if( sameParallelDistribution && x.interpolationCoordinates.getLength()==interpolationCoordinates.getLength() )
      {
	for( int g=0; g<interpolationCoordinates.getLength(); g++ )
	  assign(interpolationCoordinates[g],x.interpolationCoordinates[g]);
      }
      else
	interpolationCoordinates = x.interpolationCoordinates;
    }
    if( x.interpoleeGrid.getLength() ) 
    {
      if( sameParallelDistribution && x.interpoleeGrid.getLength()==interpoleeGrid.getLength())
      {
	for( int g=0; g<interpoleeGrid.getLength(); g++ )
	  assign(interpoleeGrid[g],x.interpoleeGrid[g]);
      }
      else
      {
	interpoleeGrid         = x.interpoleeGrid;
      }
    } // end if
    if( x.variableInterpolationWidth.getLength() ) 
    {
      if( sameParallelDistribution && x.variableInterpolationWidth.getLength()==variableInterpolationWidth.getLength()) 
      {
	for( int g=0; g<variableInterpolationWidth.getLength(); g++ )
	  assign(variableInterpolationWidth[g],x.variableInterpolationWidth[g]);
      }
      else
      {
	variableInterpolationWidth=x.variableInterpolationWidth;
      }
    } // end if
    if( x.interpoleeLocation.getLength() )
    {
      if( sameParallelDistribution && x.interpoleeLocation.getLength()==interpoleeLocation.getLength() )
      {
	for( int g=0; g<interpoleeLocation.getLength(); g++ )
	  assign(interpoleeLocation[g],x.interpoleeLocation[g]);
      }
      else
	interpoleeLocation       = x.interpoleeLocation;
    }
    if( x.interpolationPoint.getLength() )
    {
      if( sameParallelDistribution && x.interpolationPoint.getLength()==interpolationPoint.getLength() )
      {
	for( int g=0; g<interpolationPoint.getLength(); g++ )
	  assign(interpolationPoint[g],x.interpolationPoint[g]);
      }
      else
	interpolationPoint       = x.interpolationPoint;
    }
#endif // USE_STL
  }
  
  if( x.interpolationPointLocal.getLength() )
  { // Here are the new interpolation data arrays (for parallel too)
    numberOfInterpolationPointsLocal.redim(x.numberOfInterpolationPointsLocal);
    numberOfInterpolationPointsLocal=x.numberOfInterpolationPointsLocal;

    interpolationPointLocal.destroy();
    interpolationPointLocal=x.interpolationPointLocal;

    interpoleeGridLocal.destroy();
    interpoleeGridLocal=x.interpoleeGridLocal;

    variableInterpolationWidthLocal.destroy();
    variableInterpolationWidthLocal=x.variableInterpolationWidthLocal;

    interpoleeLocationLocal.destroy();
    interpoleeLocationLocal=x.interpoleeLocationLocal;

    interpolationCoordinatesLocal.destroy();
    interpolationCoordinatesLocal=x.interpolationCoordinatesLocal;

    // If we are copying to a grid on 1 processor, merge the interpolation data
    // associated with refinement grids (so that we can plot these interpolation points)
    #ifdef USE_PPP
    const intSerialArray & ps  = partition.getProcessorSet();

    // printF("CompositeGrid:operator=: copy local interp data ?\n");
    
    if( ps.getLength(0)==1 && x.localInterpolationDataState!=noLocalInterpolationData )
    {
      int gStart = x.localInterpolationDataState==localInterpolationDataForAll ? 0 : numberOfBaseGrids;

      // printF("CompositeGrid:operator=: copy local interp data, gStart=%i\n",gStart);
      

      localInterpolationDataState=noLocalInterpolationData;  // there is no local data now
      
      for( int g=gStart; g<numberOfComponentGrids; g++ )
      {
        // total number of interpolation points on grid g:
	int ni = ParallelUtility::getSum(x.numberOfInterpolationPointsLocal(g)); 
	// const int ni = ParallelUtility::getSum(interpolationPointLocal[g].getLength(0)); 

	// printf("CGD=: grid=%i niLocal=%i ni=%i\n",g,x.numberOfInterpolationPointsLocal(g),ni);

	numberOfInterpolationPoints(g)=ni;

        interpolationPoint[g].partition(partition);
	interpolationPoint[g].redim(ni,numberOfDimensions);

        interpoleeLocation[g].partition(partition);
	interpoleeLocation[g].redim(ni,numberOfDimensions);

        interpoleeGrid[g].partition(partition);
	interpoleeGrid[g].redim(ni);

        variableInterpolationWidth[g].partition(partition);
	variableInterpolationWidth[g].redim(ni);

	interpolationCoordinates[g].partition(partition);
	interpolationCoordinates[g].redim(ni,numberOfDimensions);

 	intSerialArray ip;  getLocalArrayWithGhostBoundaries(interpolationPoint[g],ip);
 	intSerialArray il;  getLocalArrayWithGhostBoundaries(interpoleeLocation[g],il);
 	intSerialArray ig;  getLocalArrayWithGhostBoundaries(interpoleeGrid[g],ig);
 	intSerialArray vw;  getLocalArrayWithGhostBoundaries(variableInterpolationWidth[g],vw);
        realSerialArray ci; getLocalArrayWithGhostBoundaries(interpolationCoordinates[g],ci);

	Index Iv[2];
	// Iv[0]=Range(interpolationPointLocal[g].getBase(0),interpolationPointLocal[g].getBound(0));
	Iv[0]=Range(0,x.numberOfInterpolationPointsLocal(g)-1);
	Iv[1]=Range(numberOfDimensions);
	const int p0=ps(0);  // copy results to this processor
	CopyArray::getAggregateArray( interpolationPointLocal[g], Iv, ip, p0);  // results go into ip
	CopyArray::getAggregateArray( interpoleeLocationLocal[g], Iv, il, p0);  // results go into il

	// ::display(interpolationPointLocal[g],"interpolationPointLocal[g]");
	
	// ::display(ip," ip after copy from all processors");
	// ::display(interpolationPoint[g].getLocalArray()," interpolationPoint[g]");
	
	CopyArray::getAggregateArray( interpolationCoordinatesLocal[g], Iv, ci, p0);

        Iv[1]=0;
	CopyArray::getAggregateArray( interpoleeGridLocal[g], Iv, ig, p0);
	CopyArray::getAggregateArray( variableInterpolationWidthLocal[g], Iv, vw, p0);  // results go into vw

        // ::display(ig," ig after copy from all processors");

	// ::display(interpoleeLocation[g]," interpoleeLocation[g] after copy from all processors");

      }
    }
    #endif
  }

  if (x.inverseCoordinates.gridCollectionData == this) {
    inverseCoordinates = x.inverseCoordinates;
    if( !inverseCoordinates.isNull() ) // *wdh* 011126
      inverseCoordinates.updateToMatchGrid(*this);
  }
  if (x.inverseGrid.gridCollectionData == this) {
    inverseGrid = x.inverseGrid;
    if( !inverseGrid.isNull() ) // *wdh* 011126
      inverseGrid.updateToMatchGrid(*this);
  }
  computedGeometry |= upd & x.computedGeometry;

  hybridConnectivity = x.hybridConnectivity;

  // kkc
  surfaceStitching = x.surfaceStitching;
  if ( surfaceStitching )
    ((Mapping *)surfaceStitching)->incrementReferenceCount();

  return *this;
}
void CompositeGridData::reference(const CompositeGridData& x) {
    cerr << "CompositeGridData::reference(const CompositeGridData&) "
         << "was called!" << endl;
    GridCollectionData::reference(x);
}
void CompositeGridData::breakReference() {
    cerr << "CompositeGridData::breakReference() was called!" << endl;
    GridCollectionData::breakReference();
}
void CompositeGridData::consistencyCheck() const {
    GridCollectionData::              consistencyCheck();
    numberOfInterpolationPoints      .Test_Consistency();
    numberOfImplicitInterpolationPoints.Test_Consistency(); 
    interpolationStartEndIndex.Test_Consistency();          

//    numberOfInterpoleePoints         .Test_Consistency();
    interpolationIsImplicit          .Test_Consistency();
//    backupInterpolationIsImplicit    .Test_Consistency();
    interpolationWidth               .Test_Consistency();
//    backupInterpolationWidth         .Test_Consistency();
    interpolationOverlap             .Test_Consistency();
    maximumHoleCuttingDistance.Test_Consistency();
//    backupInterpolationOverlap       .Test_Consistency();
//    interpolationConditionLimit      .Test_Consistency();
//    backupInterpolationConditionLimit.Test_Consistency();
    interpolationPreference          .Test_Consistency();
    mayInterpolate                   .Test_Consistency();
//    mayBackupInterpolate             .Test_Consistency();
    mayCutHoles                      .Test_Consistency();
    sharedSidesMayCutHoles           .Test_Consistency();
    multigridCoarseningRatio         .Test_Consistency();
    multigridProlongationWidth       .Test_Consistency();
    multigridRestrictionWidth        .Test_Consistency();
//    interpoleeGridRange              .Test_Consistency();
    interpolationCoordinates         .consistencyCheck();
    interpoleeGrid                   .consistencyCheck();
    variableInterpolationWidth       .consistencyCheck();
//    interpoleePoint                  .consistencyCheck();
    interpoleeLocation               .consistencyCheck();
    interpolationPoint               .consistencyCheck();
//    interpolationCondition           .consistencyCheck();
    multigridLevel                   .consistencyCheck();
    domain                           .consistencyCheck();
//    inverseCondition                 .consistencyCheck();
    inverseCoordinates               .consistencyCheck();
    inverseGrid                      .consistencyCheck();
}
Integer CompositeGridData::get(
  const GenericDataBase& db,
  const aString&         name) {
    Integer returnValue = 0;
    GenericDataBase& dir = *db.virtualConstructor();
    db.find(dir, name, getClassName());
    dir.setMode(GenericDataBase::streamInputMode);

    returnValue |= GridCollectionData::get(dir, "GridCollectionData");

    const Integer computedGeometry0 = computedGeometry;
    initialize(numberOfDimensions, numberOfGrids);

    returnValue |= dir.get(numberOfCompleteMultigridLevels,
                          "numberOfCompleteMultigridLevels");
    returnValue |= dir.get(epsilon,
                          "epsilon");
#if defined GNU || defined __PHOTON || defined __DECCXX
    {
    Integer foo;
    returnValue |= dir.get(foo,
                          "interpolationIsAllExplicit");
    interpolationIsAllExplicit = foo;
    returnValue |= dir.get(foo,
                          "interpolationIsAllImplicit");
    interpolationIsAllImplicit = foo;
    }
#else
    returnValue |= dir.get(interpolationIsAllExplicit,
                          "interpolationIsAllExplicit");
    returnValue |= dir.get(interpolationIsAllImplicit,
                          "interpolationIsAllImplicit");
#endif // defined GNU || defined __PHOTON || defined __DECCXX

    if( numberOfGrids > 0 ) 
    {
        returnValue |= dir.get(numberOfInterpolationPoints,
                              "numberOfInterpolationPoints");
        returnValue |= dir.get(numberOfImplicitInterpolationPoints,
                              "numberOfImplicitInterpolationPoints");
        returnValue |= dir.get(interpolationStartEndIndex,
                              "interpolationStartEndIndex");
    } // end if

    if( numberOfComponentGrids > 0 ) 
    {
        returnValue |= dir.get(interpolationIsImplicit,
                              "interpolationIsImplicit");
        returnValue |= dir.get(interpolationWidth,
                              "interpolationWidth");
        returnValue |= dir.get(interpolationOverlap,
                              "interpolationOverlap");
        returnValue |= dir.get(maximumHoleCuttingDistance,
                              "maximumHoleCuttingDistance");
        returnValue |= dir.get(interpolationPreference,
                              "interpolationPreference");
        returnValue |= dir.get(mayInterpolate,
                              "mayInterpolate");
        returnValue |= dir.get(mayCutHoles,
                              "mayCutHoles");
        returnValue |= dir.get(sharedSidesMayCutHoles,
                              "sharedSidesMayCutHoles");
        returnValue |= dir.get(multigridCoarseningRatio,
                              "multigridCoarseningRatio");
        returnValue |= dir.get(multigridProlongationWidth,
                              "multigridProlongationWidth");
        returnValue |= dir.get(multigridRestrictionWidth,
                              "multigridRestrictionWidth");
    } // end if

    // ** todo: interpolationPointLocal etc. ***

    CompositeGridData::update((GenericGridCollectionData&)*this,
      computedGeometry0 & (EVERYTHING & ~THElists), COMPUTEnothing);
    computedGeometry = computedGeometry0 & ~THElists;

    for (Integer i=0; i<numberOfGrids; i++) 
      if (numberOfInterpolationPoints(i)) 
      {
        char thing_i[32];
        if (computedGeometry & THEinterpolationCoordinates) 
        {
            sprintf(thing_i,      "interpolationCoordinates[%d]", i);
            returnValue |=
              dir.getDistributed(interpolationCoordinates[i], thing_i);
        } // end if
        if (computedGeometry & THEinterpoleeGrid)
        {
            sprintf(thing_i,      "interpoleeGrid[%d]", i);
            returnValue |= dir.getDistributed(interpoleeGrid[i], thing_i);
            sprintf(thing_i,      "variableInterpolationWidth[%d]", i);
            int rt = dir.getDistributed(variableInterpolationWidth[i], thing_i);
            returnValue |= rt;
            if( rt!=0 )
	    {
              printf("Giving default values for variableInterpolationWidth : %i\n",max(interpolationWidth));
              variableInterpolationWidth[i].redim(numberOfInterpolationPoints(i));
              variableInterpolationWidth[i]=max(interpolationWidth);
	    }
        } // end if
        if (computedGeometry & THEinterpoleeLocation) {
            sprintf(thing_i,      "interpoleeLocation[%d]", i);
            returnValue |= dir.getDistributed(interpoleeLocation[i], thing_i);
        } // end if
        if (computedGeometry & THEinterpolationPoint) {
            sprintf(thing_i,      "interpolationPoint[%d]", i);
            returnValue |= dir.getDistributed(interpolationPoint[i], thing_i);
        } // end if
    } // end if, end for

    if (computedGeometry & THEinverseMap) returnValue |=
      inverseCoordinates.get(dir, "inverseCoordinates") |
      inverseGrid       .get(dir, "inverseGrid");

    computedGeometry = computedGeometry0; // *wdh* 061123 put here so MG lists interp data gets updated properly 

    // *wdh* 061123
    // printf(" CG:get: computedGeometry & THEmultigridLevel=%i\n",int((computedGeometry & THEmultigridLevel)!=0));
    
    CompositeGridData::update((GenericGridCollectionData&)*this,
      computedGeometry & THElists, COMPUTEnothing);

    // *wdh* 061123computedGeometry = computedGeometry0;

    // // //
    // kkc 5/23/01 added io of hybrid connectivity
    //
    int hybridConnectivitySaved=0;
    dir.get(hybridConnectivitySaved,"hybridConnectivitySaved");   // *wdh* do this way for streaming mode
    if ( hybridConnectivitySaved ) 
    {
      int unstructuredGridIndex=-1;                    
      dir.get(unstructuredGridIndex,"unstructuredGridIndex");
      intArray ugi;
      dir.getDistributed(ugi,"hybridUVertex2GridIndex");
      intArray bfacem;
      dir.getDistributed(bfacem,"hybridBoundaryFaceMapping");
	
      intArray *gi2uvptr = new intArray[numberOfGrids-1];
      intArray *gv2uvptr = new intArray[numberOfGrids-1];
      aString buff;
      for ( int g=0; g<numberOfGrids-1; g++ ) 
      {
	buff ="";
	sPrintF(buff,"hybridGridIndex2UVertex[%d]",g);
	dir.getDistributed(gi2uvptr[g],buff);
	buff ="";
	sPrintF(buff,"hybridGridVertex2UVertex[%d]",g);
	dir.getDistributed(gv2uvptr[g],buff);
      }

      hybridConnectivity.setCompositeGridHybridConnectivity(unstructuredGridIndex,
							    gi2uvptr,
							    ugi,
							    gv2uvptr,
							    bfacem);
    }

    // // //

    // // // 
    // kkc 2/24/03 added io of surface stitching
    //
    int surfaceStitchingSaved = 0;
    dir.get(surfaceStitchingSaved,"surfaceStitchingSaved");
    if ( surfaceStitchingSaved )
      {
	surfaceStitching = new UnstructuredMapping;
	surfaceStitching->get(dir, "surfaceStitching");
      }
    // // // 
      
    delete &dir;
    return returnValue;
}

int CompositeGridData::
convertLocalInterpolationData()
// ====================================================================================================
// /Description:
//    Convert serial array (local) interpolation data to parallel array form
//
// In some cases, the interpolation data is stored in "local" serial" arrays. 
// Parallel AMR interpolation data, for example, is normally saved in serial arrays, 
//         interpolationPointLocal, interpoleeGridLocal, ...
// These serial arrays hold the interpolation data that exist on a given processor. 
// Before saving this serial interp-data to a data-base file we first build parallel arrays 
// that hold the information. This makes it easier to read in on a different number of processors.
//   
// /NOTE: Parallel amr grids use local arrays (except when they are read from a file)
//        The parallel grid generator will also generate local arrays.
// ====================================================================================================
{
  #ifdef USE_PPP
  int numberOfGrids0=numberOfComponentGrids; // *wdh* 2013/08/31 - includes AMR but NOT MG

  if( numberOfGrids0<=1 ||       // no interpolation points in this case 
      numberOfBaseGrids<=1 ||   // no interpolation points in this case either
      localInterpolationDataState==noLocalInterpolationData ) 
    return 0;
  
  if( localInterpolationDataState==localInterpolationDataForAll ||
      (localInterpolationDataState==localInterpolationDataForAMR && numberOfRefinementLevels>1) )
  {
    real time=getCPU();
  
    const MPI_Comm & OV_COMM = Overture::OV_COMM;
    const int myid=max(0,Communication_Manager::My_Process_Number);
    const int np= max(1,Communication_Manager::numberOfProcessors());

    int debug=0;  // 3 
    // sum up the local number of interp points on grids with local interpolation data. 
    const int baseGrid=localInterpolationDataState==localInterpolationDataForAll ? 0 : numberOfBaseGrids;
    const int numberOfLocalGrids=numberOfGrids0-baseGrid;

    if( debug & 1 )
    {
      printf("myid=%i numberOfGrids0=%i, numberOfInterpolationPointsLocal.getLength(0)=%i\n",myid,numberOfGrids0,
	     numberOfInterpolationPointsLocal.getLength(0));
    
      printf("myid=%i numberOfGrids0=%i, numberOfInterpolationPoints.getLength(0)=%i\n",myid,numberOfGrids0,
	     numberOfInterpolationPoints.getLength(0));
    
      // assert( numberOfInterpolationPointsLocal.getLength(0)==numberOfGrids0 );

      ::display(numberOfInterpolationPointsLocal,"numberOfInterpolationPointsLocal");
      ::display(numberOfInterpolationPoints,"numberOfInterpolationPoints");
    }
    
    ParallelUtility::getSums(&numberOfInterpolationPointsLocal(baseGrid),
			     &numberOfInterpolationPoints(baseGrid),numberOfLocalGrids);

    
    initializePartition();

    // **************************************************************************************
    // *********** First dimension the parallel arrays interpolationPoint,... ***************
    // **************************************************************************************
    for( int g=baseGrid; g<numberOfGrids0; g++ )
    {
      // printF(" *** numberOfInterpolationPoints(g=%i)=%i\n",g,numberOfInterpolationPoints(g));
      
      if( localInterpolationDataState==localInterpolationDataForAll || refinementLevelNumber(g)>0 )
      {
        // total number of interpolation points on this grid:
	// int ni = ParallelUtility::getSum(numberOfInterpolationPointsLocal(g)); // ** use getSums
        int ni = numberOfInterpolationPoints(g);
        int niMax = ParallelUtility::getMaxValue(ni);
	assert( ni==niMax );
        assert( numberOfDimensions>0 && numberOfDimensions<4 );
        if( ni>0 )
	{
          interpolationPoint[g].partition(partition);
          interpoleeLocation[g].partition(partition);
          interpolationCoordinates[g].partition(partition);
          variableInterpolationWidth[g].partition(partition);
          interpoleeGrid[g].partition(partition);

	  interpolationPoint[g].redim(ni,numberOfDimensions);
	  interpoleeLocation[g].redim(ni,numberOfDimensions);
          interpolationCoordinates[g].redim(ni,numberOfDimensions);
	  variableInterpolationWidth[g].redim(ni);
	  interpoleeGrid[g].redim(ni);

	  if( false && np==1 )
	  { // on one processor we could just do this :
	    interpolationPoint[g].getLocalArray()=interpolationPointLocal[g];
	    interpoleeLocation[g].getLocalArray()=interpoleeLocationLocal[g];
	    interpolationCoordinates[g].getLocalArray()=interpolationCoordinatesLocal[g];
	    variableInterpolationWidth[g].getLocalArray()=variableInterpolationWidthLocal[g];
	    interpoleeGrid[g].getLocalArray()=interpoleeGridLocal[g];
	  }
	}
        else
	{
	  interpolationPoint[g].redim(0);
	  interpoleeLocation[g].redim(0);
          interpolationCoordinates[g].redim(0);
	  variableInterpolationWidth[g].redim(0);
	  interpoleeGrid[g].redim(0);
	  
	}
	
      }

      if( debug & 1  ) // **TEMP
      {
	printF("CG:convertLocal g=%i interpoleeGrid[g]=[%i,%i][%i,%i]\n",g,
	       interpoleeGrid[g].getBase(0),interpoleeGrid[g].getBound(0),
	       interpoleeGrid[g].getBase(1),interpoleeGrid[g].getBound(1));
      }


    } // end for g


    // --------------------------------------------------------------------------------------
    // The interp arrays are sorted by donor grid. 
    // We need to know the number of interpolation points between grids (g1,g2) on proc. p
    // We then can determine where to send the interpolation data on this processor
    // --------------------------------------------------------------------------------------

    // --------------------------------------------------------------------------------------
    // numInterpPerProc(g1,g2,p) : the number of interpolation points between grids (g1,g2) on proc. p
    // --------------------------------------------------------------------------------------

    SparseArray<int> numInterpPerProc(numberOfComponentGrids,numberOfComponentGrids,np);
    // IntegerArray numInterpPerProc(numberOfComponentGrids,numberOfComponentGrids,np);
    // numInterpPerProc=0;

    // NOTE: interpolationStartEndIndex is used for both local and parallel interpolation data
    // NOTE: ise(.,g1,g2) : these values refer to interpolationPointLocal for refinement grids 
    const IntegerArray & ise = interpolationStartEndIndex; 
    int *numberOfNonZeroEntries= new int[np];  
    for( int p=0; p<np; p++ )
      numberOfNonZeroEntries[p]=0;
    
    for( int g1=0; g1<numberOfComponentGrids; g1++ )
    {
      for( int g2=0; g2<numberOfComponentGrids; g2++ )
      {
	int num = ise(0,g1,g2)>=0 ? ise(1,g1,g2)-ise(0,g1,g2)+1 : 0;
		
	// numInterpPerProc(g1,g2,myid)=num;
        if( num>0 )
	{
	  numInterpPerProc.get(g1,g2,myid)=num;
	  numberOfNonZeroEntries[myid]++;
	}
// 	if( debug & 1 && num>0 )
// 	  printf("CG:put: numInterpPerProc(g1=%i,g2=%i,myid=%i)=%i\n",g1,g2,myid,num);
      }
    }

    int tag=123;
    MPI_Status status;

    // *** old way: send all data ***
//     if( true )
//     {
//       int ngSq=numberOfComponentGrids*numberOfComponentGrids;
//       for( int p=0; p<np; p++ )
//       {
// 	MPI_Sendrecv(&numInterpPerProc(0,0,myid), ngSq, MPI_INT, p, tag, 
// 		     &numInterpPerProc(0,0,p   ), ngSq, MPI_INT, p, tag, OV_COMM, &status ); 
//       }
//     }
    
    // **** new way -- send compressed version of the data ****
    // -- 1. send the number of nonzero entries on each processor
    const int tag0=386631;
    for( int p=0; p<np; p++ )
    {
      int tags=tag0+p, tagr=tag0+myid;
      if( myid!=p )
      {
	MPI_Sendrecv(&numberOfNonZeroEntries[myid], 1, MPI_INT, p, tags, 
		     &numberOfNonZeroEntries[p]   , 1, MPI_INT, p, tagr, OV_COMM, &status ); 
      }
    }
    if( debug & 4 )
    {
      for( int p=0; p<np; p++ )
      {
        printf("myid=%i numberOfNonZeroEntries[p=%i]=%i\n",myid,p,numberOfNonZeroEntries[p]);
      }
      fflush(0);
      Communication_Manager::Sync();
    }
    
    // --- 2. allocate space for the sparse data
    int ** numInterpPerProcSparseIndex = new int *[np]; 
    int ** numInterpPerProcSparseData = new int *[np];  
    for( int p=0; p<np; p++ )
    {
      numInterpPerProcSparseIndex[p] = new int [ max(1,numberOfNonZeroEntries[p]) ];
      numInterpPerProcSparseData[p] = new int [ max(1,numberOfNonZeroEntries[p]) ];
    }
    // --- 3. fill in the sparse data on this processor
    int k=0;
    for( int g1=0; g1<numberOfComponentGrids; g1++ )
    {
      for( int g2=0; g2<numberOfComponentGrids; g2++ )
      {
	int num = ise(0,g1,g2)>=0 ? ise(1,g1,g2)-ise(0,g1,g2)+1 : 0;
        if( num>0 )
	{
          assert( k<numberOfNonZeroEntries[myid] );
	  
	  numInterpPerProcSparseIndex[myid][k]=(g1)+numberOfComponentGrids*(g2);
	  numInterpPerProcSparseData[myid][k]=num;
          k++;
	}
      }
    }
    assert( k==numberOfNonZeroEntries[myid] );

    // --- 4. Send sparse data
    const int tag1=219372;
    const int tag2=270114;
    for( int p=0; p<np; p++ )
    {
      if( p!=myid )
      {
        int tags=tag1+p, tagr=tag1+myid;
	MPI_Sendrecv(numInterpPerProcSparseIndex[myid],numberOfNonZeroEntries[myid],MPI_INT, p, tags, 
		     numInterpPerProcSparseIndex[p]   ,numberOfNonZeroEntries[p]   ,MPI_INT, p, tagr, OV_COMM, &status ); 
	int num=0;
	MPI_Get_count( &status, MPI_INT, &num );
	assert( num==numberOfNonZeroEntries[p] );

        tags=tag2+p, tagr=tag2+myid;
	MPI_Sendrecv(numInterpPerProcSparseData[myid],numberOfNonZeroEntries[myid],MPI_INT, p, tags, 
		     numInterpPerProcSparseData[p]   ,numberOfNonZeroEntries[p]   ,MPI_INT, p, tagr, OV_COMM, &status ); 	MPI_Get_count( &status, MPI_INT, &num );
	assert( num==numberOfNonZeroEntries[p] );
      }
      
    }
    if( debug & 4 )
    {
      for( int p=0; p<np; p++ )
      {
	for( int k=0; k<numberOfNonZeroEntries[p]; k++ )
	{
	  printf("myid=%i received data from p=%i: k=%i index=%i data=%i\n",myid,p,k,numInterpPerProcSparseIndex[p][k],
		 numInterpPerProcSparseData[p][k]);
	}
      }
      if( debug & 1 )
      {
	fflush(0);
	Communication_Manager::Sync();
      }
    }

    // --- 5. Fill in the sparse data from other processors
    for( int p=0; p<np; p++ )
    {
      if( p!=myid )
      {
        // printf(" myid=%i: receive numberOfNonZeroEntries[p=%i]=%i\n",myid,p,numberOfNonZeroEntries[p]);
	for( int k=0; k<numberOfNonZeroEntries[p]; k++ )
	{
	  int index = numInterpPerProcSparseIndex[p][k];
	  int g2 = index/numberOfComponentGrids;
	  int g1 = index-numberOfComponentGrids*(g2);
//           printf(" myid=%i: p=%i, k=%i index=%i (g1,g2)=(%i,%i) num=%i\n",myid,p,k,index,g1,g2,
//                    numInterpPerProcSparseData[p][k]);
	  
	  assert( g1>=0 && g1<numberOfComponentGrids );
	  assert( g2>=0 && g2<numberOfComponentGrids );
	  assert( numInterpPerProcSparseData[p][k]>0 );
	  
          // assert( numInterpPerProc(g1,g2,p)==numInterpPerProcSparseData[p][k] ); // compare to old way
	  
	  // numInterpPerProc(g1,g2,p)=numInterpPerProcSparseData[p][k];
	  numInterpPerProc.get(g1,g2,p)=numInterpPerProcSparseData[p][k];
	}
      }
    }
    if( debug & 1 )
    {
      fflush(0);
      Communication_Manager::Sync();
    }

    delete [] numberOfNonZeroEntries;
    for( int p=0; p<np; p++ )
    {
      delete [] numInterpPerProcSparseIndex[p];
      delete [] numInterpPerProcSparseData[p];
    }
    delete [] numInterpPerProcSparseIndex;
    delete [] numInterpPerProcSparseData;
    
    // Send numInterpPerProc(numberOfComponentGrids,numberOfComponentGrids,myid) to p=0,1,...,np-1

    if( debug & 4 )
    {
      for( int g1=0; g1<numberOfComponentGrids; g1++ )
      {
	for( int g2=0; g2<numberOfComponentGrids; g2++ )
	{
	  for( int p=0; p<np; p++ )
	  {
	    int num=numInterpPerProc(g1,g2,p);
	    if( num>0 )
	      printf("CG:put:After: myid=%i numInterpPerProc(g1=%i,g2=%i,p=%i)=%i\n",myid,g1,g2,p,num);
	  }
	}
      }
      fflush(0);
      Communication_Manager::Sync();
    }
    
    
    // ********** determine where to send the interp data and how much to receive *************
    int *numToSendp = new int [np*np];
    #define numToSend(p1,p2) numToSendp[(p1)+np*(p2)]
    for( int p1=0; p1<np; p1++ )
    {
      for( int p2=0; p2<np; p2++ ) 
        numToSend(p1,p2)=0;
    }
    
    IndexBox *box = new IndexBox[np];
    
    // =========== Send/receive data for interpolationPoint[g1]: ===========================
    for( int g1=baseGrid; g1<numberOfComponentGrids; g1++ )   
    {
      int numInterpCumulative =0;  // accumulate number of interp points for grid g1 
      if( numberOfInterpolationPoints(g1)==0 ) continue;
      
      // Get boxes that define the parallel distribution of the interpolation arrays:
      for( int p=0; p<np; p++ )
        CopyArray::getLocalArrayBox( p, interpoleeGrid[g1], box[p] );

      // loop over donor grids g2:
      for( int g2=0; g2<numberOfComponentGrids; g2++ )
      {
        // int i0=ise(0,g1,g2),  i1=ise(1,g1,g2); 

        int numDonorCumulative=0;  // accumulate number of interp points on this donor (for current g1)
	for( int p=0; p<np; p++ )
	{
	  if( numInterpPerProc(g1,g2,p)>0 ) // processor p has values to send
	  {
	    // local:  i=i0,i0+1,...,i1
	    // global: j=j0,j0+1,...,j1
	    //   i0=ise(0,g1,g2),  i1=ise(1,g1,g2)
	    //   j0 = sum_{g=0,..,g1-1} numberOfInterpolationPoints(g)  + sum_{g<g2,p<myid} numInterpPerProc(g1,g,p)
          
	    // local data i=i0,...,i1 should be copied to global data j0,...,j1
	    //  interpolationPoint[g1](j,d) = interpolationPointLocal[g1](i,d)

            int i0 = 0, i1=0;
	    int j0 = numInterpCumulative+numDonorCumulative; // global index 
            int j1 = j0+numInterpPerProc(g1,g2,p)-1;
	    // printF(" g1=%i, g2=%i p=%i, local: [i0,i1]=[%i,%i], global: [j0,j1]=[%i,%i]\n",g1,g2,p,i0,i1,j0,j1);
	    
	    for( int p2=0; p2<np; p2++ )  // determine the processors that receieve the data
	    {
	      // ia,..,ib : data to send from proc. p to processor p2
              // IndexBox box(0,1);
              // trouble here:
              intArray & ig =interpoleeGrid[g1];
	      // printF(" ig : [%i,%i]\n",ig.getBase(0),ig.getBound(0));
	      // CopyArray::getLocalArrayBox( p2, interpoleeGrid[g1], box ); // these could be pre-computed
	      // CopyArray::getLocalArrayBox( p2, grid[g1].mask(), box ); // these could be pre-computed

              // printF(" interpolationPoint[g1=%i]: p2=%i box=[%i,%i]\n",g1,p2,box.base(0),box.bound(0));
	      
 	      int ia=max(j0,box[p2].base(0)), ib=min(j1,box[p2].bound(0));
 	      if( ib>=ia )
 	      {
		// printF(" g1=%i, g2=%i send %i values from processor p=%i to p2=%i\n",g1,g2,ib-ia+1,p,p2);

 		numToSend(p,p2)+=ib-ia+1;
	      }
	    }
	  
	  }
          numDonorCumulative+=numInterpPerProc(g1,g2,p);
	} // end for p

        numInterpCumulative+=numDonorCumulative;
      } // end for g2
      
    } // end for g1
    if( debug & 1 )
    {
      fflush(0);
      Communication_Manager::Sync();
    }


    // Define the data type for sending the interpolation data 

    struct InterpolateData
    {
    real ci[3];           // interpolationCoordinates
    int globalIndex;      // global index
    int ip[3];            // interpolationPoint
    int il[3];            // interpoleeLocation
    int viw;              // variable interpolation width
    int grid,donor;
    };

    // Define the MPI DataType corresponding to the above struct
    MPI_Datatype InterpolateDataType, oldTypes[2];
    MPI_Aint offsets[2], extent;
    int blockCounts[2];
  
    offsets[0]    = 0;
    oldTypes[0]   = MPI_Real;  // NOTE: Use MPI_Real  (not MPI_REAL == MPI_FLOAT)
    blockCounts[0]= 3;  // there are 3 reals

    MPI_Type_extent(oldTypes[0], &extent);

    offsets[1]    = blockCounts[0]*extent;
    oldTypes[1]   = MPI_INT;
    blockCounts[1]= 10;      // there are 10 int's in InterpolateData

    MPI_Type_struct(2, blockCounts, offsets, oldTypes, &InterpolateDataType);
    MPI_Type_commit(&InterpolateDataType);


    //  ------- Allocate the MPI buffers for sending and receiving ---------

    int *numSent = new int[np];  // running count of number sent to processor p
    InterpolateData **sd = new InterpolateData* [np];
    InterpolateData **rd = new InterpolateData* [np];
    for( int p=0; p<np; p++ )
    {
      sd[p] = new InterpolateData[max(1,numToSend(myid,p))];
      rd[p] = new InterpolateData[max(1,numToSend(p,myid))];   // numToReceive(p) = numToSend(p,myid)
      numSent[p]=0;
    }
    
    // ***********  post receives from processor p:  *******************
    MPI_Request *receiveRequest = new MPI_Request[np];  
    const int tag3=245510;
    for( int p=0; p<np; p++ )
    {  
      tag=tag3+myid;
      MPI_Irecv( rd[p], numToSend(p,myid), InterpolateDataType, p, tag, OV_COMM, &receiveRequest[p] );
    }


    // ***********************************************
    // ***** Fill in the data for sending ************
    // ***********************************************
    
    for( int g1=baseGrid; g1<numberOfComponentGrids; g1++ )   
    {
      int numInterpCumulative =0;  // accumulate number of interp points for grid g1 
      if( numberOfInterpolationPoints(g1)==0 ) continue;
      
      intSerialArray & ip  = interpolationPointLocal[g1];
      intSerialArray & ig  = interpoleeGridLocal[g1];
      intSerialArray & il  = interpoleeLocationLocal[g1];
      intSerialArray & viw = variableInterpolationWidthLocal[g1];
      realSerialArray & ci  = interpolationCoordinatesLocal[g1];
      
      // Get boxes that define the parallel distribution of the interpolation arrays:
      for( int p=0; p<np; p++ )
        CopyArray::getLocalArrayBox( p, interpoleeGrid[g1], box[p] );

      // loop over donor grids g2:
      int iLocal=0;  // index into local interpolation arrays
      for( int g2=0; g2<numberOfComponentGrids; g2++ )
      {
        // int i0=ise(0,g1,g2),  i1=ise(1,g1,g2); 

        int numDonorCumulative=0;  // accumulate number of interp points on this donor (for current g1)
	for( int p=0; p<np; p++ )
	{
	  if( numInterpPerProc(g1,g2,p)>0 ) // processor p has values to send
	  {
	    int j0 = numInterpCumulative+numDonorCumulative; // global index 
            int j1 = j0+numInterpPerProc(g1,g2,p)-1;
	    // printF(" g1=%i, g2=%i p=%i, local: [i0,i1]=[%i,%i], global: [j0,j1]=[%i,%i]\n",g1,g2,p,i0,i1,j0,j1);
	    
	    for( int p2=0; p2<np; p2++ )  // determine the processors that receieve the data
	    {
	      // ia,..,ib : data to send from proc. p to processor p2
 	      int ia=max(j0,box[p2].base(0)), ib=min(j1,box[p2].bound(0));
 	      if( ia<=ib && myid==p )
 	      {
		// printF(" g1=%i, g2=%i send %i values from processor p=%i to p2=%i\n",g1,g2,ib-ia+1,p,p2);

 		 // numToSend(p,p2)+=ib-ia+1;

                int & k = numSent[p2]; 
                assert( k<numToSend(myid,p2) );
		assert( iLocal<=ip.getBound(0) );
		
                for( int j=ia; j<=ib; j++ )
		{
                  InterpolateData & data = sd[p2][k];
		  for( int dir=0; dir<numberOfDimensions; dir++ )
		  {
		    data.ip[dir]=ip(iLocal,dir);
		    data.il[dir]=il(iLocal,dir);
		    data.ci[dir]=ci(iLocal,dir);
		  }
                  data.viw=viw(iLocal);
                  data.globalIndex=j;
                  data.grid=g1;
		  data.donor=g2;
                  iLocal++;
  		  k++;
		}
		
	      }
	    }
	  
	  }
          numDonorCumulative+=numInterpPerProc(g1,g2,p);
	} // end for p

        numInterpCumulative+=numDonorCumulative;
      } // end for g2
      
    } // end for g1
    if( debug & 1 )
    {
      fflush(0);
      Communication_Manager::Sync();
    }

    delete [] numSent;
    delete [] box;


    // **************************************************
    // *************** send data ************************
    // **************************************************
    MPI_Request *sendRequest = new MPI_Request[np];  
    for( int p=0; p<np; p++ )
    {
      tag=tag3+p;
      MPI_Isend( sd[p], numToSend(myid,p), InterpolateDataType, p, tag, OV_COMM, &sendRequest[p] ); 
    }

    // --- wait for all the receives to finish ---
    MPI_Status *receiveStatus= new MPI_Status[np];  
    MPI_Waitall(np,receiveRequest,receiveStatus);

    if( true ) // *wdh* 061215
    { // double check that the number of values received equals what we expected
      for( int p=0; p<np; p++ )
      {
	int num=0;
	MPI_Get_count( &receiveStatus[p], InterpolateDataType, &num );
        assert( num==numToSend(p,myid) );
      }
    }

    // *************************************
    // ********* unpack the data ***********
    // *************************************
    intSerialArray *ipa = new intSerialArray [numberOfComponentGrids];
    intSerialArray *ila = new intSerialArray [numberOfComponentGrids];
    intSerialArray *viwa= new intSerialArray [numberOfComponentGrids];
    intSerialArray *iga = new intSerialArray [numberOfComponentGrids];
    realSerialArray *cia= new realSerialArray [numberOfComponentGrids];
    
    for( int grid=0; grid<numberOfComponentGrids; grid++ )
    {
      getLocalArrayWithGhostBoundaries(interpolationPoint[grid],ipa[grid]);
      getLocalArrayWithGhostBoundaries(interpoleeLocation[grid],ila[grid]);
      getLocalArrayWithGhostBoundaries(variableInterpolationWidth[grid],viwa[grid]);
      getLocalArrayWithGhostBoundaries(interpoleeGrid[grid],iga[grid]);
      getLocalArrayWithGhostBoundaries(interpolationCoordinates[grid],cia[grid]);
    }
      

    for( int p=0; p<np; p++ )
    {
      for( int k=0; k<numToSend(p,myid); k++ )
      {
	InterpolateData & data = rd[p][k];

	int i = data.globalIndex;
	int grid=data.grid;
	int donor=data.donor;
	assert( grid>=0 && grid<numberOfComponentGrids );
	assert( donor>=0 && donor<numberOfComponentGrids );
	
// 	const IntegerArray & ip  = interpolationPoint[grid].getLocalArray();
// 	const IntegerArray & il  = interpoleeLocation[grid].getLocalArray();
// 	const IntegerArray & viw = variableInterpolationWidth[grid].getLocalArray();
// 	const IntegerArray & ig  = interpoleeGrid[grid].getLocalArray();
// 	const RealArray & ci  = interpolationCoordinates[grid].getLocalArray();

 	const IntegerArray & ip  = ipa[grid];
 	const IntegerArray & il  = ila[grid];
 	const IntegerArray & viw = viwa[grid];
 	const IntegerArray & ig  = iga[grid];
 	const RealArray    & ci  = cia[grid];

	// assert( i>=ip.getBase(0) && i<=ip.getBound(0) );
        if( i<ip.getBase(0) || i>ip.getBound(0) || i>=numberOfInterpolationPoints(grid) )
	{
	  printf("CompositeGrid::put:ERROR unpacking interp data: i=%i is out of bounds! This should not happen!\n"
                 " i should be between ip.getBase(0)=%i and ip.getBound(0)=%i AND less than ni=%i\n"
                 " il.getBase(0)=%i and il.getBound(0)=%i\n"
                 " myid=%i, receiving data from p=%i, grid=%i, donor=%i, k=%i of a total of %i values to receive\n",
                 i,ip.getBase(0),ip.getBound(0),numberOfInterpolationPoints(grid),
                 il.getBase(0),il.getBound(0),myid,p,grid,donor,k,numToSend(p,myid));
	  Overture::abort("error");
	}
	
	if( debug & 2 )
	{
	  printf("myid=%i, p=%i: receive pt i=%i (global index) (grid,donor)=(%i,%i) ip=(%i,%i)\n",myid,p,
		 i,grid,donor,data.ip[0],data.ip[1]);
	}
	ig(i)=donor;
	viw(i)=data.viw;

        if( ig.getBase(0)!=ip.getBase(0) || ig.getBound(0)!=ip.getBound(0) )
	{
	  printf("CompositeGrid::put:ERROR array ig dos not match array ip!! This should not happen!\n"
                 " ip=[%i,%i][%i,%i] il=[%i,%i][%i,%i] ig=[%i,%i] \n"
           " myid=%i, receiving data from p=%i, i=%i grid=%i, donor=%i, k=%i of a total of %i values to receive\n",
                 ip.getBase(0),ip.getBound(0),ip.getBase(1),ip.getBound(1),
                 il.getBase(0),il.getBound(0),il.getBase(1),il.getBound(1),
                 ig.getBase(0),ig.getBound(0),
                 myid,p,i,grid,donor,k,numToSend(p,myid));
	  Overture::abort("error");

	}
	
        assert( ig.getBase(0)==ip.getBase(0) && ig.getBound(0)==ip.getBound(0) );
        assert( ip.getBase(1)==0 && ip.getBound(1)==(numberOfDimensions-1) );
	
	for( int dir=0; dir<numberOfDimensions; dir++ )
	{
	  ip(i,dir)=data.ip[dir];
	  il(i,dir)=data.il[dir];
	  ci(i,dir)=data.ci[dir];
	  
	}
      }
    }
    if( debug & 1 )
    {
      fflush(0);
      Communication_Manager::Sync();
    }
    
    // Communication_Manager::Sync();

    delete [] ipa;
    delete [] ila;
    delete [] viwa;
    delete [] iga;
    delete [] cia;

    // -------------------------------------------------------------------------------------
    // -- we need to sum up interpolationStartEndIndex values from local grids as well: ----
    // .. do this here since the interpolationStartEndIndex array is used above as local values ...
    // -------------------------------------------------------------------------------------
    if( true && numberOfLocalGrids>0 )
    {
      
      const IntegerArray & ise = interpolationStartEndIndex; 
      int *pNumInterpLocal = new int [numberOfLocalGrids*numberOfLocalGrids];
#define numInterpLocal(g,g2) pNumInterpLocal[(g)+numberOfLocalGrids*(g2)]
      int *pNumInterp = new int [numberOfLocalGrids*numberOfLocalGrids];
#define numInterp(g,g2) pNumInterp[(g)+numberOfLocalGrids*(g2)]
      for( int g=baseGrid; g<numberOfGrids0; g++ )
      {
	for( int g2=baseGrid; g2<numberOfGrids0; g2++ )
	{
	  if( ise(0,g,g2) >=0 )
	    numInterpLocal(g-baseGrid,g2-baseGrid) = ise(1,g,g2)-ise(0,g,g2)+1;
	  else
	    numInterpLocal(g-baseGrid,g2-baseGrid) = 0;
	}
      }
      // Sum up values from local arrays     
      ParallelUtility::getSums( pNumInterpLocal, pNumInterp, numberOfLocalGrids*numberOfLocalGrids);
      for( int g=baseGrid; g<numberOfGrids0; g++ )
      {
	int numSum=0; // running sum of interp. pts for grid g
	for( int g2=baseGrid; g2<numberOfGrids0; g2++ )
	{

          // printf(" convertLocalInterpolationData:Before ise(0:1,g=%i,g2=%i)=%i,%i\n",g,g2,ise(0,g,g2),ise(1,g,g2));

	  if( numInterp(g-baseGrid,g2-baseGrid) > 0 )
	  {
	    ise(0,g,g2)=numSum;     // interp pts on grid g that have donors on g2 start here...
	    numSum+=numInterp(g-baseGrid,g2-baseGrid);
	    ise(1,g,g2)=numSum-1;   // ... and end here 
            // When there are backup implicit interp pts on an explicit interp. grid, the implicit points are
            // located first in the list (to make it faster to interpolate by iteration).
            // In parallel we current do NOT do this. **FIX ME**
            // The implicit-by-iteration interpolator uses this next value so we need to set it.
	    ise(2,g,g2)=numSum-1;   // do this for now, all points assumed implicit.  *wdh* 2012/07/05 
	  }
	  else
	  {
            ise(0,g,g2)=-1;
	    ise(1,g,g2)=-1;
	    ise(2,g,g2)=-1;         // *wdh* 2012/07/05 
	  }

	  // printf(" convertLocalInterpolationData:After ise(0:1,g=%i,g2=%i)=%i,%i\n",g,g2,ise(0,g,g2),ise(1,g,g2));
	  
	}
	if( numSum!=numberOfInterpolationPoints(g) )
	{
          printf("ERROR: CompositeGridData::convertLocalInterpolationData:ERROR: numSum=%i but"
                 " numberOfInterpolationPoints(g=%i)=%i\n",numSum,g,numberOfInterpolationPoints(g));
	  OV_ABORT("ERROR");
	}
      }
      delete [] pNumInterpLocal;
      delete [] pNumInterp;
    }



    // wait for sends to finish on this processor before we can clean up
    MPI_Waitall(np,sendRequest,receiveStatus);

    // cleanup:
    MPI_Type_free( &InterpolateDataType );

    delete [] numToSendp;
    for( int p=0; p<np; p++ )
    {
      delete [] sd[p];
      delete [] rd[p];
    }
    delete [] sd;
    delete [] rd;
    
    delete [] receiveStatus;
    delete [] receiveRequest;
    delete [] sendRequest;

//    delete [] numToReceivep;

  
    // *wdh* 2013/08/31
    computedGeometry |=
      CompositeGrid::THEmask                     |
      CompositeGrid::THEinterpolationCoordinates |
      CompositeGrid::THEinterpolationPoint       |
      CompositeGrid::THEinterpoleeLocation       |
      CompositeGrid::THEinterpoleeGrid;

    if( debug & 1 )
    {
      time=getCPU()-time;
      time=ParallelUtility::getMaxValue(time);
      printF("CompositeGrid::put: time to move refinement grid interpolation data =%8.2e (s)\n",time);
      fflush(0);
      Communication_Manager::Sync();
    }

  } // end if numberOfRefinementLevels>1
  #endif
  return 0;
}

Integer CompositeGridData::
put(GenericDataBase& db,
    const aString&   name,
    int geometryToPut /* = -1  */ ) const 
// =========================================================================================
// /Description:
//     Save a CompositeGrid to a data base file
//
// /db (input) : save to this data base
// /name (input) : name to save as in the data base
// /geometryToPut (input) : specify which geometry to put, by default put computedGeometry
// =========================================================================================
{
  Integer returnValue = 0;
  if( geometryToPut==-1 ) 
    geometryToPut=computedGeometry;
  else
  {
    // NOTE: for now we always save the interpolation arrays if they have been computed
    geometryToPut |= computedGeometry & 
                    (THEinterpolationCoordinates | 
                      THEinterpoleeGrid | 
                      THEinterpoleeLocation |
		      THEinterpolationPoint);
    
   //     printf("\n ####### CG:put: computedGeometry & THEmask = %i\n\n",int(computedGeometry & THEmask));
   //     printf("\n ####### CG:put: geometryToPut & THEmask = %i\n\n",int(geometryToPut & THEmask));
  }

  GenericDataBase& dir = *db.virtualConstructor();
  db.create(dir, name, getClassName());

  dir.setMode(GenericDataBase::streamOutputMode); // turn on stream mode unless this is prevented

  returnValue |= GridCollectionData::put(dir, "GridCollectionData",geometryToPut );

  // we can only save what has been computed:  (N.B. Do this after the above call)
  geometryToPut= geometryToPut & computedGeometry;  

  returnValue |= dir.put(numberOfCompleteMultigridLevels,
			 "numberOfCompleteMultigridLevels");
  returnValue |= dir.put(epsilon, "epsilon");
  returnValue |= dir.put(interpolationIsAllExplicit,
			 "interpolationIsAllExplicit");
  returnValue |= dir.put(interpolationIsAllImplicit,
			 "interpolationIsAllImplicit");

  if( numberOfGrids > 0 )
  {
    returnValue |= dir.put(numberOfInterpolationPoints,
			   "numberOfInterpolationPoints");
    returnValue |= dir.put(numberOfImplicitInterpolationPoints,
			   "numberOfImplicitInterpolationPoints");
    returnValue |= dir.put(interpolationStartEndIndex,
			   "interpolationStartEndIndex");
  } // end if

  if( numberOfComponentGrids > 0 ) 
  {
    returnValue |= dir.put(interpolationIsImplicit,
			   "interpolationIsImplicit");
    returnValue |= dir.put(interpolationWidth,
			   "interpolationWidth");
    returnValue |= dir.put(interpolationOverlap,
			   "interpolationOverlap");
    returnValue |= dir.put(maximumHoleCuttingDistance,
			   "maximumHoleCuttingDistance");
    returnValue |= dir.put(interpolationPreference,
			   "interpolationPreference");
    returnValue |= dir.put(mayInterpolate,
			   "mayInterpolate");
    returnValue |= dir.put(mayCutHoles,
			   "mayCutHoles");
    returnValue |= dir.put(sharedSidesMayCutHoles,
			   "sharedSidesMayCutHoles");
    returnValue |= dir.put(multigridCoarseningRatio,
			   "multigridCoarseningRatio");
    returnValue |= dir.put(multigridProlongationWidth,
			   "multigridProlongationWidth");
    returnValue |= dir.put(multigridRestrictionWidth,
			   "multigridRestrictionWidth");
  } // end if

  for( Integer i=0; i<numberOfGrids; i++ )
  {
    if (numberOfInterpolationPoints(i)) 
    {
      char thing_i[32];
      if (geometryToPut &  THEinterpolationCoordinates) 
      {
	sprintf(thing_i,      "interpolationCoordinates[%d]", i);
	returnValue |= dir.putDistributed(interpolationCoordinates[i], thing_i);
      } // end if
      if (geometryToPut &  THEinterpoleeGrid) {
	sprintf(thing_i,      "interpoleeGrid[%d]", i);
	if( false )
	{
	    printF("CG:put g=%i interpoleeGrid[g]=[%i,%i][%i,%i]\n",i,
		   interpoleeGrid[i].getBase(0),interpoleeGrid[i].getBound(0),
		   interpoleeGrid[i].getBase(1),interpoleeGrid[i].getBound(1));
	  ::display(interpoleeGrid[i],sPrintF("put: interpoleeGrid for grid=%i",i));
          OV_GET_SERIAL_ARRAY(int,interpoleeGrid[i],igLocal);
	  // ::display(igLocal,sPrintF("put: interpoleeGridLocal for grid=%i",i));
	}
	returnValue |= dir.putDistributed(interpoleeGrid[i], thing_i);
	sprintf(thing_i,      "variableInterpolationWidth[%d]", i);
	returnValue |= dir.putDistributed(variableInterpolationWidth[i], thing_i);
      } // end if
      if (geometryToPut &  THEinterpoleeLocation) {
	sprintf(thing_i,      "interpoleeLocation[%d]", i);
	returnValue |= dir.putDistributed(interpoleeLocation[i], thing_i);
      } // end if
      if (geometryToPut &  THEinterpolationPoint) {
	sprintf(thing_i,      "interpolationPoint[%d]", i);
	returnValue |= dir.putDistributed(interpolationPoint[i], thing_i);
      } // end if
    } // end if, end for
  }

  if( numberOfMultigridLevels>1 )
  {
    for( int l=0; l<numberOfMultigridLevels; l++ )
    {
      CompositeGrid & cg = multigridLevel[l];
      
    }
    
  }
  

  
  if (geometryToPut & THEinverseMap) returnValue |=
					  inverseCoordinates.put(dir, "inverseCoordinates") |
					  inverseGrid       .put(dir, "inverseGrid");

  // // //
  // kkc 5/23/01 added output of hybrid connectivity
  //
  int hybridConnectivitySaved=hybridConnectivity.getUnstructuredGridIndex()>-1;
  returnValue |= dir.put(hybridConnectivitySaved,"hybridConnectivitySaved");  // *wdh* do this way for streaming mode
  if ( hybridConnectivity.getUnstructuredGridIndex()>-1 ) 
  {
    returnValue |= dir.put(hybridConnectivity.getUnstructuredGridIndex(),"unstructuredGridIndex");

    returnValue |= dir.putDistributed(hybridConnectivity.getBoundaryFaceMapping(),"hybridBoundaryFaceMapping");
    returnValue |= dir.putDistributed(hybridConnectivity.getUVertex2GridIndex(),"hybridUVertex2GridIndex");
	
    aString buff="";
    for ( int g=0; g<numberOfGrids-1; g++ ) 
    {
      buff ="";
      const intArray & gi2uv = hybridConnectivity.getGridIndex2UVertex(g);
      sPrintF(buff,"hybridGridIndex2UVertex[%d]",g);
      dir.putDistributed(gi2uv,buff);
      buff ="";
      const intArray & gv2uv = hybridConnectivity.getGridVertex2UVertex(g);
      sPrintF(buff,"hybridGridVertex2UVertex[%d]",g);
      dir.putDistributed(gv2uv,buff);
    }
  }

  // // //

  // // // 
  // kkc 2/24/03 added io of surface stitching
  //

  int surfaceStitchingSaved = surfaceStitching ? 1 : 0;
  returnValue |= dir.put(surfaceStitchingSaved,"surfaceStitchingSaved");
  if ( surfaceStitching )
    surfaceStitching->put(dir, "surfaceStitching");
  // // //

  delete &dir;
  return returnValue;
}


Integer CompositeGridData::
update( GenericGridCollectionData& x,
	const Integer              what,
	const Integer              how) 
{
  // update all lists but the multigrid level and domain (which are lists of CompositeGrid's and done below)
  Integer upd = GridCollectionData::update(x, what & (~THEmultigridLevel | ~THEdomain) , how);

  CompositeGridData& y = (CompositeGridData&)x;
  Integer computeNeeded =
    how & COMPUTEgeometry         ? what :
    how & COMPUTEgeometryAsNeeded ? what & ~computedGeometry :
                                      NOTHING;
  initializePartition();

//
//  Compute interpolationIsAllExplicit and interpolationIsAllImplicit from
//  values of interpolationIsImplicit, backupInterpolationIsImplicit,
//  mayInterpolate and mayBackupInterpolate.
//
  interpolationIsAllExplicit =
    interpolationIsAllImplicit = LogicalTrue;
  for (Integer l=0; l<numberOfMultigridLevels; l++)
    for (Integer k1=0; k1<numberOfComponentGrids; k1++)
      for (Integer k2=0; k2<numberOfComponentGrids; k2++)
	if (k1 != k2) 
        {
	  if ((mayInterpolate(k1,k2,l) &&
	       interpolationIsImplicit(k1,k2,l)) 
	      //  || ( mayBackupInterpolate(k1,k2,l) && backupInterpolationIsImplicit(k1,k2,l))
	    )
	    interpolationIsAllExplicit = LogicalFalse;
	  if ((mayInterpolate(k1,k2,l) &&
	       !interpolationIsImplicit(k1,k2,l)) 
	      //  || ( mayBackupInterpolate(k1,k2,l) && !backupInterpolationIsImplicit(k1,k2,l))
	    )
	    interpolationIsAllImplicit = LogicalFalse;
	} // end if, end for, end for, end for

#ifdef USE_STL
  if (what & THEinterpolationCoordinates) 
  {
    Integer i = numberOfGrids - interpolationCoordinates.size();
    if (i < 0) interpolationCoordinates.erase(
      interpolationCoordinates.begin() + numberOfGrids,
      interpolationCoordinates.end());
    else for (Integer j=0; j<i; j++) interpolationCoordinates.push_back();
  } // end if
  if (what & THEinterpoleeGrid) 
  {
    Integer i = numberOfGrids - interpoleeGrid.size();
    if (i < 0) 
      interpoleeGrid.erase(interpoleeGrid.begin() + numberOfGrids, interpoleeGrid.end());
    else 
      for (Integer j=0; j<i; j++) interpoleeGrid.push_back();
    i = numberOfGrids - variableInterpolationWidth.size();
    if (i < 0)
      variableInterpolationWidth.erase(variableInterpolationWidth.begin() + numberOfGrids,
				       variableInterpolationWidth.end());
    else
      for (Integer j=0; j<i; j++) variableInterpolationWidth.push_back();
  } // end if
  if (what & THEinterpoleeLocation) 
  {
    Integer i = numberOfGrids - interpoleeLocation.size();
    if (i < 0) interpoleeLocation.erase(
      interpoleeLocation.begin() + numberOfGrids,
      interpoleeLocation.end());
    else 
      for (Integer j=0; j<i; j++) interpoleeLocation.push_back();
  } // end if
  if (what & THEinterpolationPoint) 
  {
    Integer i = numberOfGrids - interpolationPoint.size();
    if (i < 0) interpolationPoint.erase(
      interpolationPoint.begin() + numberOfGrids,
      interpolationPoint.end());
    else 
      for (Integer j=0; j<i; j++) interpolationPoint.push_back();
  } // end if
#else
  if (what & THEinterpolationCoordinates) 
  {
    while (interpolationCoordinates.getLength() < numberOfGrids)
      interpolationCoordinates     .addElement();
    while (interpolationCoordinates.getLength() > numberOfGrids)
      interpolationCoordinates     .deleteElement();
  } // end if
  if (what & THEinterpoleeGrid) 
  {
    while (interpoleeGrid          .getLength() < numberOfGrids)
      interpoleeGrid               .addElement();
    while (interpoleeGrid          .getLength() > numberOfGrids)
      interpoleeGrid               .deleteElement();

    while (variableInterpolationWidth.getLength() < numberOfGrids)
      variableInterpolationWidth     .addElement();
    while (variableInterpolationWidth.getLength() > numberOfGrids)
      variableInterpolationWidth     .deleteElement();
  } // end if
  if (what & THEinterpoleeLocation) 
  {
    while (interpoleeLocation      .getLength() < numberOfGrids)
      interpoleeLocation           .addElement();
    while (interpoleeLocation      .getLength() > numberOfGrids)
      interpoleeLocation           .deleteElement();
  } // end if
  if (what & THEinterpolationPoint) 
  {
    while (interpolationPoint      .getLength() < numberOfGrids)
      interpolationPoint           .addElement();
    while (interpolationPoint      .getLength() > numberOfGrids)
      interpolationPoint           .deleteElement();
  } // end if
#endif // USE_STL

  for (Integer i=0; i<numberOfGrids; i++) 
  {
    if (numberOfInterpolationPoints(i)) 
    {
      if (what & THEinterpolationCoordinates) 
      {
	if (&y != this && i <
#ifdef USE_STL
	    y.interpolationCoordinates.size() &&
#else
	    y.interpolationCoordinates.getLength() &&
#endif // USE_STL
	    y.interpolationCoordinates[i].elementCount() == numberOfInterpolationPoints(i) * numberOfDimensions &&
	    y.interpolationCoordinates[i].getBase(0) == 0 &&
	    y.interpolationCoordinates[i].getBound(0) == numberOfInterpolationPoints(i) - 1 &&
	    y.interpolationCoordinates[i].getBase(1) == 0 &&
	    y.interpolationCoordinates[i].getBound(1) == numberOfDimensions - 1)
	  interpolationCoordinates[i].reference(y.interpolationCoordinates[i]);

	if (interpolationCoordinates[i].elementCount() != numberOfInterpolationPoints(i) * numberOfDimensions ||
	    interpolationCoordinates[i].getBase(0) != 0 ||
	    interpolationCoordinates[i].getBound(0) != numberOfInterpolationPoints(i) - 1 ||
	    interpolationCoordinates[i].getBase(1) != 0 ||
	    interpolationCoordinates[i].getBound(1) != numberOfDimensions - 1) 
	{
          interpolationCoordinates[i].partition(partition);
	  interpolationCoordinates[i].redim( numberOfInterpolationPoints(i), numberOfDimensions);
	  interpolationCoordinates[i] = (Real)0.;
	  if (how & COMPUTEgeometryAsNeeded)
	    computeNeeded  |=  THEinterpolationCoordinates;
	  computedGeometry &= ~THEinterpolationCoordinates;
	  upd              |=  THEinterpolationCoordinates;
	} // end if
      } // end if
      if (what & THEinterpoleeGrid)
      {
	if (&y != this && i <
#ifdef USE_STL
	    y.interpoleeGrid.size() &&
#else
	    y.interpoleeGrid.getLength() &&
#endif // USE_STL
	    y.interpoleeGrid[i].elementCount() == numberOfInterpolationPoints(i) &&
	    y.interpoleeGrid[i].getBase(0) == 0 && 
	    y.interpoleeGrid[i].getBound(0) == numberOfInterpolationPoints(i) - 1)
	{
	  interpoleeGrid[i].reference(y.interpoleeGrid[i]);
	  variableInterpolationWidth[i].reference(y.variableInterpolationWidth[i]);
	}
	if (interpoleeGrid[i].elementCount() !=numberOfInterpolationPoints(i) ||
	    interpoleeGrid[i].getBase(0) != 0 || 
	    interpoleeGrid[i].getBound(0) != numberOfInterpolationPoints(i) - 1)
	{
          interpoleeGrid[i].partition(partition);
	  interpoleeGrid[i].redim(numberOfInterpolationPoints(i));
	  interpoleeGrid[i] = 0;
	  variableInterpolationWidth[i].partition(partition); // *wdh* 050327
	  variableInterpolationWidth[i].redim(numberOfInterpolationPoints(i));
	  variableInterpolationWidth[i] = 0;
	  if (how & COMPUTEgeometryAsNeeded)
	    computeNeeded  |=  THEinterpoleeGrid;
	  computedGeometry &= ~THEinterpoleeGrid;
	  upd              |=  THEinterpoleeGrid;
	} // end if
      } // end if
      if (what & THEinterpoleeLocation) 
      {
	if (&y != this && i <
#ifdef USE_STL
	    y.interpoleeLocation.size() &&
#else
	    y.interpoleeLocation.getLength() &&
#endif // USE_STL
	    y.interpoleeLocation[i].elementCount() != numberOfInterpolationPoints(i) * numberOfDimensions &&
	    y.interpoleeLocation[i].getBase(0) == 0 &&
	    y.interpoleeLocation[i].getBound(0) == numberOfInterpolationPoints(i) - 1 &&
	    y.interpoleeLocation[i].getBase(1) == 0 &&
	    y.interpoleeLocation[i].getBound(1) ==  numberOfDimensions - 1)
	  interpoleeLocation[i].reference(y.interpoleeLocation[i]);

	if (interpoleeLocation[i].elementCount() != numberOfInterpolationPoints(i) * numberOfDimensions ||
	    interpoleeLocation[i].getBase(0) != 0 ||
	    interpoleeLocation[i].getBound(0) != numberOfInterpolationPoints(i) - 1 ||
	    interpoleeLocation[i].getBase(1) != 0 || 
	    interpoleeLocation[i].getBound(1) != numberOfDimensions - 1) 
	{
	  interpoleeLocation[i].partition(partition);
	  interpoleeLocation[i].redim( numberOfInterpolationPoints(i), numberOfDimensions);
	  interpoleeLocation[i] = 0;
	  if (how & COMPUTEgeometryAsNeeded)
	    computeNeeded  |=  THEinterpoleeLocation;
	  computedGeometry &= ~THEinterpoleeLocation;
	  upd              |=  THEinterpoleeLocation;
	} // end if
      } // end if
      if (what & THEinterpolationPoint) {
	if (&y != this && i <
#ifdef USE_STL
	    y.interpolationPoint.size() &&
#else
	    y.interpolationPoint.getLength() &&
#endif // USE_STL
	    y.interpolationPoint[i].elementCount() == numberOfInterpolationPoints(i) * numberOfDimensions &&
	    y.interpolationPoint[i].getBase(0) == 0 &&
	    y.interpolationPoint[i].getBound(0) == numberOfInterpolationPoints(i) - 1 &&
	    y.interpolationPoint[i].getBase(1) == 0 &&
	    y.interpolationPoint[i].getBound(1) ==  numberOfDimensions - 1)
	  interpolationPoint[i].reference(y.interpolationPoint[i]);
	if (interpolationPoint[i].elementCount() != numberOfInterpolationPoints(i) * numberOfDimensions ||
	    interpolationPoint[i].getBase(0) != 0 ||
	    interpolationPoint[i].getBound(0) != numberOfInterpolationPoints(i) - 1 ||
	    interpolationPoint[i].getBase(1) != 0 ||
	    interpolationPoint[i].getBound(1) != numberOfDimensions - 1) 
        {
          interpolationPoint[i].partition(partition);
	  interpolationPoint[i].redim(numberOfInterpolationPoints(i), numberOfDimensions);
	  interpolationPoint[i] = 0;
	  if (how & COMPUTEgeometryAsNeeded)
	    computeNeeded  |=  THEinterpolationPoint;
	  computedGeometry &= ~THEinterpolationPoint;
	  upd              |=  THEinterpolationPoint;
	} // end if
      } // end if
//             if (what & THEinterpolationCondition) {
//                 if (&y != this && i <
// #ifdef USE_STL
//                   y.interpolationCondition.size() &&
// #else
//                   y.interpolationCondition.getLength() &&
// #endif // USE_STL
//                   y.interpolationCondition[i].elementCount() ==
//                     numberOfInterpolationPoints(i) &&
//                   y.interpolationCondition[i].getBase(0) == 0 &&
//                   y.interpolationCondition[i].getBound(0) ==
//                     numberOfInterpolationPoints(i) - 1)
//                   interpolationCondition[i].
//                     reference(y.interpolationCondition[i]);
//                 if (interpolationCondition[i].elementCount() !=
//                     numberOfInterpolationPoints(i) ||
//                     interpolationCondition[i].getBase(0) != 0 ||
//                     interpolationCondition[i].getBound(0) !=
//                     numberOfInterpolationPoints(i) - 1) {
//                     interpolationCondition[i].redim(
//                       numberOfInterpolationPoints(i));
//                     interpolationCondition[i] = (Real)0.;
//                     if (how & COMPUTEgeometryAsNeeded)
//                       computeNeeded  |=  THEinterpolationCondition;
//                     computedGeometry &= ~THEinterpolationCondition;
//                     upd              |=  THEinterpolationCondition;
//                 } // end if
//             } // end if
    } 
    else 
    { // (numberOfInterpolationPoints == 0)
      if (what & THEinterpolationCoordinates)
	interpolationCoordinates[i].redim(0);
      if (what & THEinterpoleeGrid)
      {
	interpoleeGrid[i]            .redim(0);
	variableInterpolationWidth[i].redim(0);
      }
      if (what & THEinterpoleeLocation)
	interpoleeLocation[i]      .redim(0);
      if (what & THEinterpolationPoint)
	interpolationPoint[i]      .redim(0);
    } // end if
//         if (what & THEinterpoleeGrid) {
//             if (numberOfInterpoleePoints(i)) {
//                 if (&y != this && i <
// #ifdef USE_STL
//                   y.interpoleePoint.size() &&
// #else
//                   y.interpoleePoint.getLength() &&
// #endif // USE_STL
//                   y.interpoleePoint[i].elementCount() ==
//                     numberOfInterpoleePoints(i) &&
//                   y.interpoleePoint[i].getBase(0) == 0 &&
//                   y.interpoleePoint[i].getBound(0) ==
//                     numberOfInterpoleePoints(i) - 1)
//                   interpoleePoint[i].
//                     reference(y.interpoleePoint[i]);
//                 if (interpoleePoint[i].elementCount() !=
//                     numberOfInterpoleePoints(i) ||
//                     interpoleePoint[i].getBase(0) != 0 ||
//                     interpoleePoint[i].getBound(0) !=
//                     numberOfInterpoleePoints(i) - 1) {
//                     interpoleePoint[i].redim(
//                       numberOfInterpoleePoints(i));
//                     interpoleePoint[i] = 0;
//                     if (how & COMPUTEgeometryAsNeeded)
//                       computeNeeded  |=  THEinterpoleeGrid;
//                     computedGeometry &= ~THEinterpoleeGrid;
//                     upd              |=  THEinterpoleeGrid;
//                 } // end if
//             } else { // (numberOfInterpoleePoints == 0)
//                 interpoleePoint[i].redim(0);
//             } // end if
//         } // end if
    } // end for

  const Range all, nd1 = numberOfDimensions;
  if (what & THEinverseMap) 
  {
    if (&y != this) 
    {
//            inverseCondition  .reference(y.inverseCondition);
      inverseCoordinates.reference(y.inverseCoordinates);
      inverseGrid       .reference(y.inverseGrid);
      if (y.computedGeometry &   THEinverseMap) 
      {
	computedGeometry   |=  THEinverseMap;
      } 
      else if (how         &   COMPUTEgeometryAsNeeded) 
      {
	computedGeometry   &= ~THEinverseMap;
	computeNeeded      |=  THEinverseMap;
      } // end if
    } // end if
    if((  // inverseCondition  .updateToMatchGrid(*this, all, all, all) |
      // *wdh*     inverseCoordinates.updateToMatchGrid(*this, nd1, all, all, all) |
      inverseCoordinates.updateToMatchGrid(*this, all, all, all, nd1) |
      inverseGrid       .updateToMatchGrid(*this, all, all, all)) &
      RealCompositeGridFunction::updateResized) 
    {
      // inverseCondition       =   (Real)0.;
      inverseCoordinates     =   (Real)0.;
      inverseGrid            =   -1;
      if (how                &   COMPUTEgeometryAsNeeded)
	computeNeeded        |=  THEinverseMap;
      computedGeometry       &= ~THEinverseMap;
      upd                    |=  THEinverseMap;
    } // end if

// *wdh* 000202
//         boundaryAdjustment.redim(numberOfGrids,numberOfBaseGrids);
//         for (Integer k1=0; k1<numberOfGrids; k1++) {
//             MappedGrid& g1 = grid[k1];
//             for (Integer k2=0; k2<numberOfBaseGrids; k2++) {
//                 TrivialArray<BoundaryAdjustment,Range>&
//                   bA12 = boundaryAdjustment(k1,k2);
//                 assert(baseGridNumber(k2) == k2);
//                 if (k2 == baseGridNumber(k1)) bA12.redim(0); else {
//                     MappedGrid& g2 = grid[k2];
//                     bA12.redim(2,numberOfDimensions);
//                     Logical noSharedBoundaries = LogicalTrue;
//                     for (Integer kd1=0; kd1<numberOfDimensions; kd1++)
//                       for (Integer ks1=0; ks1<2; ks1++) {
//                         BoundaryAdjustment& bA = bA12(ks1,kd1);
//                         Logical needAdjustment = LogicalFalse;
//                         for (Integer kd2=0; kd2<numberOfDimensions; kd2++)
//                           for (Integer ks2=0; ks2<2; ks2++)
//                             if (g1.boundaryCondition(ks1,kd1) > 0 &&
//                                 g2.boundaryCondition(ks2,kd2) > 0 &&
//                                 g1.sharedBoundaryFlag(ks1,kd1) &&
//                                 g2.sharedBoundaryFlag(ks2,kd2) ==
//                                 g1.sharedBoundaryFlag(ks1,kd1))
//                               needAdjustment = LogicalTrue;
//                         if (needAdjustment) {
//                             noSharedBoundaries = LogicalFalse;
//                             const Integer side = ks1 ?
//                               RealMappedGridFunction::endingGridIndex :
//                               RealMappedGridFunction::startingGridIndex;
//                             const Range d0 = numberOfDimensions,
//                               d1 = kd1==0 ? Range(side,side) : Range(),
//                               d2 = kd1==1 ? Range(side,side) : Range(),
//                               d3 = kd1==2 ? Range(side,side) : Range();
//                             if (( bA.boundaryAdjustment
//                                     .updateToMatchGrid(g1, d1, d2, d3, d0) |
//                                   bA.acrossGrid
//                                     .updateToMatchGrid(g1, d1, d2, d3, d0) |
//                                   bA.oppositeBoundary
//                                     .updateToMatchGrid(g1, d1, d2, d3, d0) )
//                               & RealMappedGridFunction::updateResized) {
//                                 bA.computedGeometry &= ~THEinverseMap;
//                                 if (how          &  COMPUTEgeometryAsNeeded)
//                                   computeNeeded  |=  THEinverseMap;
//                                 computedGeometry &= ~THEinverseMap;
//                                 upd              |=  THEinverseMap;
//                             } // end if
//                         } else {
//                             bA.computedGeometry &= ~THEinverseMap;
//                             bA.boundaryAdjustment.destroy();
//                             bA.acrossGrid        .destroy();
//                             bA.oppositeBoundary  .destroy();
//                         } // end if
//                     } // end for, end for
//                     if (noSharedBoundaries) bA12.redim(0);
//                 } // end if
//             } // end for
//         } // end for
    } // end if what

    // Now update the lists of CompositeGrids

    if (what &                THEmultigridLevel)
      upd |= updateCollection(THEmultigridLevel | (what & ~THElists),
        numberOfMultigridLevels, multigridLevel,
        GridCollectionData::multigridLevel,
        GenericGridCollectionData::multigridLevel, multigridLevelNumber);

    if (what &                THEdomain)
    {
      upd |= updateCollection(THEdomain | (what & ~THElists),
        numberOfDomains, domain,
        GridCollectionData::domain,
        GenericGridCollectionData::domain, domainNumber);
    }
    
    upd |= computeGeometry(computeNeeded, how);

    return upd;
}
void CompositeGridData::destroy(const Integer what) {
#ifdef USE_STL
    if (what & THEinterpolationCoordinates) interpolationCoordinates.erase(
      interpolationCoordinates.begin(),     interpolationCoordinates.end());
    if (what & THEinterpoleeGrid)
    {
      interpoleeGrid.erase(interpoleeGrid.begin(),interpoleeGrid.end());
      variableInterpolationWidth.erase(variableInterpolationWidth.begin(),variableInterpolationWidth.end());
    }
    if (what & THEinterpoleeLocation)       interpoleeLocation.erase(
      interpoleeLocation.begin(),           interpoleeLocation.end());
    if (what & THEinterpolationPoint)       interpolationPoint.erase(
      interpolationPoint.begin(),           interpolationPoint.end());
    if (what & THEmultigridLevel)           multigridLevel.erase(
      multigridLevel.begin(),               multigridLevel.end());
    if (what & THEdomain)                   domain.erase(
      domain.begin(),                       domain.end());
#else
    if (what & THEinterpolationCoordinates)
      while (interpolationCoordinates.getLength())
        interpolationCoordinates.deleteElement();
    if (what & THEinterpoleeGrid)
    {
      while (interpoleeGrid.getLength())
        interpoleeGrid          .deleteElement();
      while (variableInterpolationWidth.getLength())
        variableInterpolationWidth.deleteElement();
    }
    if (what & THEinterpoleeLocation)
      while (interpoleeLocation.getLength())
        interpoleeLocation      .deleteElement();
    if (what & THEinterpolationPoint)
      while (interpolationPoint.getLength())
        interpolationPoint      .deleteElement();
    if (what & THEmultigridLevel)
      multigridLevel.reference(ListOfCompositeGrid());
    if (what & THEdomain)
      domain.reference(ListOfCompositeGrid());


#endif // USE_STL
    if (what & THEinverseMap) {
//         inverseCondition  .destroy();
         inverseCoordinates.destroy();
         inverseGrid       .destroy();
         boundaryAdjustment.redim(0);
    } // end if

    GridCollectionData::destroy(what);
}


//! Replace refinement level "level0" and higher
/*!

 \param level0,numberOfRefinementLevels0 : replace and/or add levels level0,..,numberOfRefinementLevels0-1
 \param gridInfo[bg][l](0:ni-1,0:ng-1) : info defining a new refinement grid on base grid bg 
       and refinement level=level0+l
 */
Integer CompositeGridData::
replaceRefinementLevels(int level0, int numberOfRefinementLevels0, IntegerArray **gridInfo )
{
  int returnValue=GridCollectionData::replaceRefinementLevels(level0,numberOfRefinementLevels0,gridInfo );
  if( returnValue!=0 ) return returnValue;
  
  // redimension arrays in the CompositeGrid.
  setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);

  // assign values to the composite grid arrays.


  for( Integer k10=0; k10<numberOfGrids; k10++)
  {
    if( refinementLevelNumber(k10) >= level0 ) 
    {
      // this must be a new grid

      Integer k1 = componentGridNumber(k10), k3 = baseGridNumber(k10);
      assert(k3 == componentGridNumber(k3));

      const Integer l = multigridLevelNumber(k1);
      numberOfInterpolationPoints(k1) = 0;

      for (Integer k20=0; k20<numberOfGrids; k20++)
      {
        // *wdh* if( multigridLevelNumber(k20)  == l &&
        // 	    (refinementLevelNumber(k20) == refinementLevelNumber(n) ||
        // 	     refinementLevelNumber(k20) == refinementLevelNumber(n) - 1)) 
	if( multigridLevelNumber(k20)  == l )
	{
	  MappedGrid& g_20 = grid[k20];
	  Integer k2 = componentGridNumber(k20), k4 = baseGridNumber(k20);
	  assert(k4 == componentGridNumber(k4));
	  if( k20 == k10 || k3 != k4 ) 
	  {
	    // Interpolation from self or from an unrelated grid.
	    interpolationIsImplicit(k1,k2,l) = interpolationIsImplicit(k3,k4,l);

	    // printf(" k1=%i k2=%i level(k10)=%i level(k20)=%i\n",k1,k2,refinementLevelNumber(k10),
	    // 	   refinementLevelNumber(k20));
	    
	    for (Integer kd=0; kd<3; kd++) 
	    {
              if( refinementLevelNumber(k10) > refinementLevelNumber(k20) )
	      {
		//  *wdh* 061102 Implicit interpolation is allowed from coarser grid on a different base grid since
                //  these values will have already been assigned. (i.e. pretend this is still explicit interp.)
		interpolationIsImplicit(k1,k2,l) = false;
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1.);
	      }
	      else
	      {
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = interpolationOverlap(kd,k3,k4,l);
	      }
	      
	    } // end for

	  } 
	  else //  if (k3 == k4)  baseGrid(k1) = baseGrid(k2)
	  {
	    //   Interpolation from a related grid.
	    interpolationIsImplicit(k1,k2,l) = LogicalFalse;
	    Integer kd;
	    for (kd=0; kd<numberOfDimensions; kd++) 
	    {
	      interpolationWidth(kd,k1,k2,l) = 1;
	      if (g_20.mapping().getGridDimensions(kd) == 1)
	      {
		// Interpolation on a surface grid.
		interpolationOverlap(kd,k1,k2,l)       = (Real)-.5;
	      } 
	      else if (refinementLevelNumber(k20) == refinementLevelNumber(k10))
	      {
		// Interpolation from a grid at the same refinement level.
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1. +
							 (Real).5 * (g_20.discretizationWidth(kd) - 1));
	      } 
	      else 
	      {
		//  Implicit interpolation from a parent (coarser) grid. (i.e. pretend this is still explicit interp.)
		// *wdh* 000821 interpolationIsImplicit(k1,k2,l) = LogicalTrue;
		interpolationIsImplicit(k1,k2,l) = false;
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1.);
	      } // end if

	      if (refinementLevelNumber(k20) == refinementLevelNumber(k10) - 1) 
	      {
		//  Coarsen for multigrid in the same way as for any parent.
		multigridCoarseningRatio(kd,k1,l) = multigridCoarseningRatio(kd,k3,l);
		multigridProlongationWidth(kd,k1,l) = multigridProlongationWidth(kd,k3,l);
		multigridRestrictionWidth(kd,k1,l) = multigridRestrictionWidth(kd,k3,l);
	      } // end if
	    } // end for
	    for (kd=numberOfDimensions; kd<3; kd++) 
	    {
	      interpolationWidth(kd,k1,k2,l) = 1;
	      interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
	      interpolationOverlap(kd,k1,k2,l) = interpolationOverlap(kd,k3,k4,l);
	    } // end for
	  } // end if
	  mayInterpolate(k1,k2,l) = k1 == k2 ? LogicalFalse : k3 == k4 ? LogicalTrue  :   mayInterpolate(k3,k4,l);
	  if (refinementLevelNumber(k20) == refinementLevelNumber(k10)) 
	  {
	    mayInterpolate(k2,k1,l)= k1 == k2 ? LogicalFalse : k3 == k4 ? LogicalTrue  : mayInterpolate(k4,k3,l);
	  } // end if
	} // end if
      } // end for k20

      //  Initially disallow interpolation to or from the new grid.
      //
      interpolationPreference(k1,Range(0,numberOfComponentGrids-1),l) = -1;
      interpolationPreference(Range(0,numberOfComponentGrids-1),k1,l) = -1;

    } // end if
  }

// ********************
//   printF("\n ***** CG:replaceRefinementLevels:\n");
//   for( int grid1=0; grid1<numberOfComponentGrids; grid1++ )
//     for( int grid2=0; grid2<numberOfComponentGrids; grid2++ )
//     {
//       printF(" grid1=%i grid2=%i interpolationOverlap=%5.2f\n",grid1,grid2,
// 	     interpolationOverlap(0,grid1,grid2,0));
//     }
// ********************


  // wdh: whay are these here?
//   Integer k1 = componentGridNumber(n), k3 = baseGridNumber(n);
//   assert(k3 == componentGridNumber(k3));
//   for (Integer kd=0; kd<3; kd++)
//   {
//     multigridCoarseningRatio(kd,k1,l) = multigridCoarseningRatio(kd,k3,l);
//     multigridProlongationWidth(kd,k1,l) = multigridProlongationWidth(kd,k3,l);
//     multigridRestrictionWidth(kd,k1,l) = multigridRestrictionWidth(kd,k3,l);
//   } // end for
// //

  if( computedGeometry & GridCollection::THEmultigridLevel )
    printf("***** CompositeGrid: replaceRefinementLevels START THEmultigridLevel!\n");
  return 0;
}


Integer CompositeGridData::
addRefinement(
  const IntegerArray& range,
  const IntegerArray& factor,
  const Integer&      level,
  const Integer       k) 
// ======================================================================================================
//   /Description:
//     Add a refinement.
//
// /Return value: grid number of the new grid added.
// ======================================================================================================
{
  Integer n = GridCollectionData::addRefinement(range, factor, level,  k);
  setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);
  const Integer l = multigridLevelNumber(n);
  numberOfInterpolationPoints(n) = 0;

  for( Integer k10=0; k10<numberOfGrids; k10++)
  {
    if( multigridLevelNumber(k10)  == l ) // *wdh* && (refinementLevelNumber(k10) == refinementLevelNumber(n))) 
    {
      Integer k1 = componentGridNumber(k10), k3 = baseGridNumber(k10);
      assert(k3 == componentGridNumber(k3));
      for (Integer k20=0; k20<numberOfGrids; k20++)
      {
        // *wdh* if( multigridLevelNumber(k20)  == l &&
        // 	    (refinementLevelNumber(k20) == refinementLevelNumber(n) ||
        // 	     refinementLevelNumber(k20) == refinementLevelNumber(n) - 1)) 
	if( multigridLevelNumber(k20)  == l )
	{
	  MappedGrid& g_20 = grid[k20];
	  Integer k2 = componentGridNumber(k20), k4 = baseGridNumber(k20);
	  assert(k4 == componentGridNumber(k4));
	  if (k20 == k10 || k3 != k4) 
	  {
	    // Interpolation from self or from an unrelated grid.
	    interpolationIsImplicit(k1,k2,l) = interpolationIsImplicit(k3,k4,l);

 	    for (Integer kd=0; kd<3; kd++) 
 	    {
// 	      interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
// 	      interpolationOverlap(kd,k1,k2,l) = interpolationOverlap(kd,k3,k4,l);

	      if( refinementLevelNumber(k10) > refinementLevelNumber(k20) )
	      {
		//  *wdh* 061102 Implicit interpolation is allowed from coarser grid on a different base grid since
		//  these values will have already been assigned. (i.e. pretend this is still explicit interp.)
		interpolationIsImplicit(k1,k2,l) = false;
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1.);
	      }
	      else
	      {
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = interpolationOverlap(kd,k3,k4,l);
	      }
 	    } // end for
	  } 
	  else //  if (k3 == k4) 
	  {
	    //   Interpolation from a related grid.
	    interpolationIsImplicit(k1,k2,l) = LogicalFalse;
	    Integer kd;
	    for (kd=0; kd<numberOfDimensions; kd++) 
	    {
	      interpolationWidth(kd,k1,k2,l) = 1;
	      if (g_20.mapping().getGridDimensions(kd) == 1)
	      {
		// Interpolation on a surface grid.
		interpolationOverlap(kd,k1,k2,l)       = (Real)-.5;
	      } 
	      else if (refinementLevelNumber(k20) == refinementLevelNumber(k10))
	      {
		// Interpolation from a grid at the same refinement level.
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1. +
							 (Real).5 * (g_20.discretizationWidth(kd) - 1));
	      } 
	      else 
	      {
		//  Implicit interpolation from a parent (coarser) grid.
		// *wdh* 000821 interpolationIsImplicit(k1,k2,l) = LogicalTrue;
		interpolationIsImplicit(k1,k2,l) = false;
		interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
		interpolationOverlap(kd,k1,k2,l) = amax1(epsilon,
							 (Real).5 * interpolationWidth(kd,k1,k2,l) - (Real)1.);
	      } // end if

	      if (refinementLevelNumber(k20) == refinementLevelNumber(k10) - 1) 
	      {
		//  Coarsen for multigrid in the same way as for any parent.
		multigridCoarseningRatio(kd,k1,l) = multigridCoarseningRatio(kd,k3,l);
		multigridProlongationWidth(kd,k1,l) = multigridProlongationWidth(kd,k3,l);
		multigridRestrictionWidth(kd,k1,l) = multigridRestrictionWidth(kd,k3,l);
	      } // end if
	    } // end for
	    for (kd=numberOfDimensions; kd<3; kd++) 
	    {
	      interpolationWidth(kd,k1,k2,l) = 1;
	      interpolationWidth(kd,k1,k2,l) = interpolationWidth(kd,k3,k4,l);
	      interpolationOverlap(kd,k1,k2,l) = interpolationOverlap(kd,k3,k4,l);
	    } // end for
	  } // end if
	  mayInterpolate(k1,k2,l) = k1 == k2 ? LogicalFalse : k3 == k4 ? LogicalTrue  :   mayInterpolate(k3,k4,l);
	  if (refinementLevelNumber(k20) == refinementLevelNumber(n)) 
	  {
	    mayInterpolate(k2,k1,l)= k1 == k2 ? LogicalFalse : k3 == k4 ? LogicalTrue  : mayInterpolate(k4,k3,l);
	  } // end if
	} // end if
      }
    } // end if
  }

  Integer k1 = componentGridNumber(n), k3 = baseGridNumber(n);
  assert(k3 == componentGridNumber(k3));
  for (Integer kd=0; kd<3; kd++)
  {
    multigridCoarseningRatio(kd,k1,l) = multigridCoarseningRatio(kd,k3,l);
    multigridProlongationWidth(kd,k1,l) = multigridProlongationWidth(kd,k3,l);
    multigridRestrictionWidth(kd,k1,l) = multigridRestrictionWidth(kd,k3,l);
  } // end for
//
//  Initially disallow interpolation to or from the new grid.
//
  interpolationPreference(k1,Range(0,numberOfComponentGrids-1),l) = -1;
  interpolationPreference(Range(0,numberOfComponentGrids-1),k1,l) = -1;
  return n;
}

void CompositeGridData::deleteRefinement(const Integer& k) {
    if (k < 0 || k >= numberOfGrids) {
        cout << "CompositeGridData::deleteRefinement(k = "
             << k << "):  Grid " << k << " does not exist." << endl;
        assert(k >= 0); assert(k < numberOfGrids);
    } else if (refinementLevelNumber(k) == 0) {
        cout << "CompositeGridData::deleteRefinement(k = "
             << k << "):  Grid k = " << k << " is not a refinement." << endl;
        assert(refinementLevelNumber(k) != 0);
    } // end if
    CompositeGridData::deleteMultigridCoarsening(k);
}
void CompositeGridData::deleteRefinementLevels(const Integer level) {
    Integer i = numberOfGrids, j = i - 1;
    while (i--) if (refinementLevelNumber(i) > level && i < j--) {
        Range r1(i, j), r2 = r1 + 1;
        numberOfInterpolationPoints(r1) = numberOfInterpolationPoints(r2);
//        numberOfInterpoleePoints(r1)    = numberOfInterpoleePoints(r2);
    } // end if, end while
    GridCollectionData::deleteRefinementLevels(level);
    setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);
}
void CompositeGridData::referenceRefinementLevels(
  GenericGridCollectionData& x,
  const Integer              level) {
    GridCollectionData::referenceRefinementLevels(x, level);
    setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);
    CompositeGridData& y = (CompositeGridData&)x;
    for (Integer i=0, j=0; i<y.numberOfGrids; i++)
      if (y.refinementLevelNumber(i) <= level) {
        numberOfInterpolationPoints(j) = y.numberOfInterpolationPoints(i);
//        numberOfInterpoleePoints(j)    = y.numberOfInterpoleePoints(i);
        j++;
    } // end if, end for
}
Integer CompositeGridData::
addMultigridCoarsening( const IntegerArray& factor,
			const Integer&      level,
			const Integer       k)
{
  Integer n = GridCollectionData::addMultigridCoarsening(factor, level,  k);
  setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);
  MappedGrid& g_n = grid[n];
  Integer k1 = componentGridNumber(n), k2, kd;

  assert( k1>=0 && k1<numberOfGrids);  // *wdh* 981118
    
  for (k2=0; k2<numberOfComponentGrids; k2++) 
  {
    interpolationIsImplicit(k1,k2,level) = interpolationIsImplicit(k1,k2,level-1);
    mayInterpolate(k1,k2,level) =  mayInterpolate(k1,k2,level-1);
    for (kd=0; kd<3; kd++) 
    {
      interpolationWidth(kd,k1,k2,level) =interpolationWidth(kd,k1,k2,level-1);
      interpolationOverlap(kd,k1,k2,level) = 	interpolationOverlap(kd,k1,k2,level-1);
    } // end for
  } // end for
  //
  //  Find the first component grid at the same multigrid level
  //  and refinement level.  If it is a different grid, we use its
  //  stencil widths.  Otherwise if level > 1, we use the stencil
  //  widths of the same component grid at the next-finer multigrid
  //  level.  Otherwise we use default stencil widths of two.
  //
  for (k2=0; k2<numberOfGrids; k2++) 
    if (k2 == n) 
      continue;
  else if (multigridLevelNumber(k2) == multigridLevelNumber(n) &&
	   baseGridNumber(k2)       == baseGridNumber(n)) 
    break;
  if (k2 != numberOfGrids) k2 = componentGridNumber(k2);

  for (kd=0; kd<numberOfDimensions; kd++)
  {
    multigridCoarseningRatio(kd,k1,level)   = factor(kd);
    multigridProlongationWidth(kd,k1,level) =
      k2 != numberOfGrids ? multigridProlongationWidth(kd,k2,level) :
      level > 1 ? multigridProlongationWidth(kd,k1,level-1) : 2;
    assert(multigridProlongationWidth(kd,k1,level) > 0);
    multigridRestrictionWidth(kd,k1,level)  =
      k2 != numberOfGrids ? multigridRestrictionWidth(kd,k2,level) :
      level > 1 ? multigridRestrictionWidth(kd,k1,level-1) : 2;
    assert(multigridRestrictionWidth(kd,k1,level) > 0);
    if (g_n.isCellCentered(kd)) 
    {
      //          multigridCoarseningRatio and multigridRestrictionWidth
      //          must be both odd or both even.
      if ((multigridRestrictionWidth(kd,k1,level)-multigridCoarseningRatio(kd,k1,level)) % 2)
	multigridRestrictionWidth(kd,k1,level)++;
    } 
    else 
    {
      //          multigridRestrictionWidth must be odd.
      if (multigridRestrictionWidth(kd,k1,level) % 2 == 0)
	multigridRestrictionWidth(kd,k1,level)++;
      //          multigridProlongationWidth must be even.
      if (multigridProlongationWidth(kd,k1,level) % 2)
	multigridProlongationWidth(kd,k1,level)++;
    } // end if
  } // end for
  for (kd=numberOfDimensions; kd<3; kd++) 
  {
    multigridCoarseningRatio(kd,k1,level)   = 1;
    multigridProlongationWidth(kd,k1,level) = 1;
    multigridRestrictionWidth(kd,k1,level)  = 1;
  } // end for
  //
  //  Initially disallow interpolation to or from the new grid.
  //
  interpolationPreference(k1,Range(0,numberOfComponentGrids-1),level) = -1;
  interpolationPreference(Range(0,numberOfComponentGrids-1),k1,level) = -1;
  return n;
}

void CompositeGridData::makeCompleteMultigridLevels() {
//
//  Find the coarsest multigrid level of each component grid.
//
    IntegerArray coarseLevel(numberOfComponentGrids); coarseLevel = -1;
    IntegerArray coarseGrid(numberOfComponentGrids);  coarseGrid  = -1;
    Integer g;
    for (g=0; g<numberOfGrids; g++) {
        const Integer k = componentGridNumber(g);
        if (coarseLevel(k) < multigridLevelNumber(g))
          { coarseLevel(k) = multigridLevelNumber(g); coarseGrid(k)  = g; }
    } // end for
//
//  Add multigrid coarsenings to the component grids that need more of them.
//  Use the same coarsening ratio as that of the corresponding base grid.
//  Note that if there is initially only one multigrid level of a base grid,
//  then all of the new multigrid levels will have coarsening factors of one.
//
    for (g=0; g<numberOfGrids; g++) {
        const Integer k = componentGridNumber(g), l = coarseLevel(k) + 1;
        if (l < numberOfCompleteMultigridLevels &&
          multigridLevelNumber(g) == l - 1) {
            const Integer b = baseGridNumber(g);
            assert(b == componentGridNumber(b));
            coarseLevel(k) = l; coarseGrid(k) = addMultigridCoarsening(
              multigridCoarseningRatio(Range(0,2),b,l), l, coarseGrid(k));
            assert(coarseGrid(k) > g);
        } // end if
    } // end for
}

void CompositeGridData::
deleteMultigridCoarsening(const Integer& k) 
{
  // printf("** CompositeGridData::deleteMultigridCoarsening k=%i\n",k);
  
  if (k < 0 || k >= numberOfGrids)
  {
    cout << "CompositeGridData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid " << k << " does not exist." << endl;
    assert(k >= 0); assert(k < numberOfGrids);
  } 
  else if (multigridLevelNumber(k) == 0 && refinementLevelNumber(k) == 0)
  {
    cout << "CompositeGridData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid k = " << k << " is not a multigrid coarsening."
	 << endl;
    assert(multigridLevelNumber(k) != 0 || refinementLevelNumber(k) != 0);
  } // end if
  Integer i, lastGrid=numberOfGrids-1;
  Range allGrids,allLevels;
  for( i=numberOfGrids-1; i>=0; i-- )
  {
    if (componentGridNumber(i) == componentGridNumber(k) && multigridLevelNumber(i) >= multigridLevelNumber(k) )
    {
      Range r1(i, lastGrid-1), r2=r1+1;
      numberOfInterpolationPoints(r1) = numberOfInterpolationPoints(r2);

      // *wdh* now update arrays
      interpolationIsImplicit(r1,allGrids,allLevels) = interpolationIsImplicit(r2,allGrids,allLevels);
      interpolationIsImplicit(allGrids,r1,allLevels) = interpolationIsImplicit(allGrids,r1,allLevels);
      for( int kd=0; kd<3; kd++ ) 
      {
        interpolationWidth(kd,r1,allGrids,allLevels)=interpolationWidth(kd,r2,allGrids,allLevels);
        interpolationWidth(kd,allGrids,r1,allLevels)=interpolationWidth(kd,allGrids,r2,allLevels);

        interpolationOverlap(kd,r1,allGrids,allLevels)=interpolationOverlap(kd,r2,allGrids,allLevels);
        interpolationOverlap(kd,allGrids,r1,allLevels)=interpolationOverlap(kd,allGrids,r2,allLevels);
      }
      
      lastGrid--;
    } // end if
  }
  
  GridCollectionData::deleteMultigridCoarsening(k);
  setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);

}

void CompositeGridData::deleteMultigridLevels(const Integer level) {
    if (level < 0) {
        cout << "CompositeGridData::deleteMultigridLevel(level = "
             << level << "):  Multigrid level " << level << " does not exist."
             << endl;
        assert(level >= 0);
    } else if (level < numberOfMultigridLevels-1) {
        Integer i = numberOfGrids, j = i - 1;
        while (i--) if (multigridLevelNumber(i) > level && i < j--) {
            Range r1(i, j), r2 = r1 + 1;
            numberOfInterpolationPoints(r1) = numberOfInterpolationPoints(r2);
//            numberOfInterpoleePoints(r1)    = numberOfInterpoleePoints(r2);
        } // end if, end while
    } // end if
    GridCollectionData::deleteMultigridLevels(level);
    setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids);
}
void CompositeGridData::setNumberOfGrids(const Integer& numberOfGrids_)
  { setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids_); }
void CompositeGridData::setNumberOfDimensions(
  const Integer& numberOfDimensions_)
  { setNumberOfDimensionsAndGrids(numberOfDimensions_, numberOfGrids); }

void CompositeGridData::
setNumberOfDimensionsAndGrids(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) 
{
  GridCollectionData::setNumberOfDimensionsAndGrids(numberOfDimensions_, numberOfGrids_);

  const Integer n = numberOfGrids_-numberOfInterpolationPoints.elementCount();
  if (n) 
  {
    numberOfInterpolationPoints.resize(numberOfGrids);
    numberOfImplicitInterpolationPoints.resize(numberOfGrids);
    interpolationStartEndIndex.resize(4,numberOfGrids,numberOfGrids);  
    if (n > 0) 
    {
      const Range newGrids(numberOfGrids_ - n, numberOfGrids_ - 1);
      numberOfInterpolationPoints(newGrids) = 0;
      Range all;
      for( int i=0; i<4; i++ )
      {
        interpolationStartEndIndex(i,all     ,newGrids)=-1;
        interpolationStartEndIndex(i,newGrids,all     )=-1;
      }
      
    } // end if
    computedGeometry &= ~(
      THEinterpolationCoordinates | THEinterpoleeGrid     |
      THEinterpoleeLocation       | THEinterpolationPoint |
      THEinverseMap         );
  } // end if

  const int numGrids=numberOfComponentGrids;
  // n1 : newNumberOfGrids - oldNumberOfGrids
  // n2 : newNumberOfMultigridLevels - oldNumber
  const Integer n1 = numGrids - (interpolationIsImplicit.getBound(0)-interpolationIsImplicit.getBase(0)+1),
    n2 = numberOfMultigridLevels -  (interpolationIsImplicit.getBound(2)-interpolationIsImplicit.getBase(2)+1);
  if (n1 || n2)
  {
    if (numGrids && numberOfMultigridLevels) 
    {
      interpolationIsImplicit          .resize(numGrids,numGrids,numberOfMultigridLevels);
      interpolationWidth               .resize(3, numGrids,numGrids,numberOfMultigridLevels);
      interpolationOverlap             .resize(3, numGrids,numGrids,numberOfMultigridLevels);
      maximumHoleCuttingDistance.resize(2,3,numGrids);
      interpolationPreference          .resize(numGrids,numGrids,numberOfMultigridLevels);
      mayInterpolate                   .resize(numGrids,numGrids,numberOfMultigridLevels);
      mayCutHoles                      .resize(numGrids,numGrids);
      sharedSidesMayCutHoles           .resize(numGrids,numGrids);
      multigridCoarseningRatio         .resize(3, numGrids,numberOfMultigridLevels);
      multigridProlongationWidth       .resize(3, numGrids,numberOfMultigridLevels);
      multigridRestrictionWidth        .resize(3, numGrids,numberOfMultigridLevels);
    } 
    else 
    {
      interpolationIsImplicit          .redim(0);
      interpolationWidth               .redim(0);
      interpolationOverlap             .redim(0);
      maximumHoleCuttingDistance.redim(0);
      interpolationPreference          .redim(0);
      mayInterpolate                   .redim(0);
      mayCutHoles                      .redim(0);
      sharedSidesMayCutHoles           .redim(0);
      multigridCoarseningRatio         .redim(0);
      multigridProlongationWidth       .redim(0);
      multigridRestrictionWidth        .redim(0);
    } // end if

    if (n1 > 0 && numGrids > 0) 
    {
      const Range three = 3, newComponentGrids(numGrids - n1, numGrids - 1), allComponentGrids = numGrids;
      mayCutHoles(newComponentGrids,allComponentGrids)= LogicalFalse;
      mayCutHoles(allComponentGrids,newComponentGrids)= LogicalFalse;
      sharedSidesMayCutHoles(newComponentGrids,allComponentGrids)= LogicalFalse;
      sharedSidesMayCutHoles(allComponentGrids,newComponentGrids)= LogicalFalse;
      maximumHoleCuttingDistance(nullRange,nullRange,newComponentGrids)=sqrt(.1*REAL_MAX);

      if (numberOfMultigridLevels > n2) 
      {
	const Range oldMultigridLevels = numberOfMultigridLevels - n2;
	interpolationIsImplicit(newComponentGrids,allComponentGrids,oldMultigridLevels) = LogicalFalse;
	interpolationIsImplicit(allComponentGrids,newComponentGrids,oldMultigridLevels) = LogicalFalse;

	interpolationWidth(three,newComponentGrids,allComponentGrids,oldMultigridLevels) = 0;
	interpolationWidth(three,allComponentGrids,newComponentGrids,oldMultigridLevels) = 0;

	interpolationOverlap(three,newComponentGrids,allComponentGrids,oldMultigridLevels) = 0.;
	interpolationOverlap(three,allComponentGrids,newComponentGrids,oldMultigridLevels) = 0.;
	interpolationPreference(newComponentGrids,allComponentGrids,oldMultigridLevels) = 0;
	interpolationPreference(allComponentGrids,newComponentGrids,oldMultigridLevels) = 0;
	mayInterpolate(allComponentGrids,newComponentGrids,oldMultigridLevels) = LogicalFalse;
	mayInterpolate(newComponentGrids,allComponentGrids,oldMultigridLevels) = LogicalFalse;
	multigridCoarseningRatio(three,newComponentGrids,oldMultigridLevels)=0;
	multigridProlongationWidth(three,newComponentGrids,oldMultigridLevels)=0;
	multigridRestrictionWidth(three,newComponentGrids,oldMultigridLevels)=0;
      } // end if
    } // end if
    if (n2 > 0 && numGrids > 0) 
    {
      const Range three = 3, newMultigridLevels(numberOfMultigridLevels - n2, numberOfMultigridLevels - 1),
	allComponentGrids = numGrids;
      interpolationIsImplicit(allComponentGrids,allComponentGrids,newMultigridLevels) = LogicalFalse;
      interpolationWidth(three,allComponentGrids,allComponentGrids,newMultigridLevels) = 0;
      interpolationOverlap(three,allComponentGrids,allComponentGrids,newMultigridLevels) = 0.;
      interpolationPreference(allComponentGrids,allComponentGrids,newMultigridLevels) = 0;
      mayInterpolate(allComponentGrids,allComponentGrids,newMultigridLevels) = LogicalFalse;
      multigridCoarseningRatio(three,allComponentGrids,newMultigridLevels)=0;
      multigridProlongationWidth(three,allComponentGrids,newMultigridLevels)=0;
      multigridRestrictionWidth(three,allComponentGrids,newMultigridLevels)=0;
    } // end if
  } // end if

}

void CompositeGridData::
initialize(const Integer& numberOfDimensions_,
	   const Integer& numberOfGrids_) 
{
  numberOfCompleteMultigridLevels=0; // *wdh* 000825
  CompositeGridData::setNumberOfDimensionsAndGrids
    (numberOfDimensions_, numberOfGrids_);
  destroy(~NOTHING & ~GridCollectionData::EVERYTHING);
//
//  Compute a default value for epsilon.
//
  epsilon = Mapping::epsilon();

  localInterpolationDataState=noLocalInterpolationData;
  
  // surfaceStitching=NULL; // *wdh* 030314 : moved to constructor since initialize is also called by get
}

void CompositeGridData::
getInterpolationStencil(const Integer&      k10,
			const Integer&      k20,
			const RealArray&    r,
			const IntegerArray& interpolationStencil,
			const intArray& useBackupRules) 
{
  MappedGrid& g = grid[k20];
  const Real a = -(Real)2. * epsilon, b = (Real)1. - a;
  const Integer base = r.getBase(0), bound = r.getBound(0),
    k1 = componentGridNumber(k10), k2 = componentGridNumber(k20),
    l  = multigridLevelNumber(k10);

#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define g_boundaryCondition(i,j)     g.boundaryCondition ((i),(j))
#define g_gridSpacing(i)             g.gridSpacing       ((i))
#define g_indexRange(i,j)            g.indexRange        ((i),(j))
#define g_extendedIndexRange(i,j)    g.extendedIndexRange((i),(j))
#define g_isCellCentered(i)          g.isCellCentered    ((i))
#define g_isPeriodic(i)              g.isPeriodic        ((i))
#define r_(i,j)                      r                   ((i),(j))
#define interpolationStencil_(i,j,k) interpolationStencil((i),(j),(k))
#define useBackupRules_(i)           useBackupRules      ((i))
#define iw0_(i)                      iw0                 ((i),k1,k2,l)
#else
#define g_boundaryCondition(i,j)     g_boundaryCondition_ [(i) + 2 * (j)]
#define g_gridSpacing(i)             g_gridSpacing_       [(i)]
#define g_indexRange(i,j)            g_indexRange_        [(i) + 2 * (j)]
#define g_extendedIndexRange(i,j)    g_extendedIndexRange_[(i) + 2 * (j)]
#define g_isCellCentered(i)          g_isCellCentered_    [(i)]
#define g_isPeriodic(i)              g_isPeriodic_        [(i)]
#define r_(i,j)                      r__                  [(i) + r_s * (j)]
#define interpolationStencil_(i,j,k) \
                     interpolationStencil__[(i) + iS_s1 * (j) + iS_s2 * (k)]
#define useBackupRules_(i)           useBackupRules__     [(i)]
#define iw0_(i)                      iw0__                [(i)]
    Integer *g_boundaryCondition_   = g.boundaryCondition() .getDataPointer(),
            *g_indexRange_          = g.indexRange()        .getDataPointer(),
            *g_extendedIndexRange_  = g.extendedIndexRange().getDataPointer(),
            *g_isCellCentered_      = g.isCellCentered()    .getDataPointer(),
            *g_isPeriodic_          = g.isPeriodic()        .getDataPointer(),
            *interpolationStencil__ = interpolationStencil  .getDataPointer(),
            *useBackupRules__       = useBackupRules        .getDataPointer(),
            *interpolationWidth__   = &interpolationWidth(0,k1,k2,l);
//        *backupInterpolationWidth__ = &backupInterpolationWidth(0,k1,k2,l);
    Real    *g_gridSpacing_         = g.gridSpacing()       .getDataPointer(),
            *r__                    = r                     .getDataPointer();
    const Integer r_s = &r(base,1) - &r(base,0),
      iS_s1 = &interpolationStencil(base,1,0) -
              &interpolationStencil(base,0,0),
      iS_s2 = &interpolationStencil(base,0,1) -
              &interpolationStencil(base,0,0);
    r__                    = &r_(-base,0);
    interpolationStencil__ = &interpolationStencil_(-base,0,0);
    useBackupRules__       = &useBackupRules_(-base);
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES

    // wdh: we can interpolate from extended index range
    real rBound[3][2];
    Integer kd;
    for (kd=0; kd<3; kd++) 
    {
      rBound[kd][0]=a+(g_extendedIndexRange(0,kd)-g_indexRange(0,kd))*g_gridSpacing(kd);   
      rBound[kd][1]=b+(g_extendedIndexRange(1,kd)-g_indexRange(1,kd))*g_gridSpacing(kd);
    }

    for (Integer i=base; i<=bound; i++) {
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        IntegerArray& iw0 = interpolationWidth;
//        IntegerArray& iw0 = useBackupRules_(i) ?
//          backupInterpolationWidth : interpolationWidth;
#else
        Integer* iw0__ = interpolationWidth__;
//        Integer* iw0__ = useBackupRules_(i) ?
//          backupInterpolationWidth__ : interpolationWidth__;
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        for ( kd=0; kd<3; kd++) if (kd < numberOfDimensions) {
// *wdh     if (r_(i,kd) < a || r_(i,kd) > b) {
            if (r_(i,kd) < rBound[kd][0] || r_(i,kd) > rBound[kd][1]) {
                interpolationStencil_(i,0,kd) =
                interpolationStencil_(i,1,kd) = INTEGER_MAX;
            } else {
                Real rr = r_(i,kd) / g_gridSpacing(kd) + g_indexRange(0,kd);
                interpolationStencil_(i,0,kd) =
                  Integer(floor(rr - (Real).5 * iw0_(kd) +
                  (g_isCellCentered(kd) ? (Real).5 : (Real)1.)));
                interpolationStencil_(i,1,kd) =
                  Integer(floor(rr + (Real).5 * iw0_(kd) -
                  (g_isCellCentered(kd) ? (Real).5 : (Real)0.)));
                if (!g_isPeriodic(kd)) {
                    if (interpolationStencil_(i,0,kd) < g_extendedIndexRange(0,kd) &&
                      g_boundaryCondition(0,kd)) {
//                      Point is close to a BC side.  One-sided interpolation used.
                        interpolationStencil_(i,0,kd) = g_extendedIndexRange(0,kd);
                        interpolationStencil_(i,1,kd) = interpolationStencil_(i,0,kd)
                          + (iw0_(kd) - 1);
                    } // end if
                    if (interpolationStencil_(i,1,kd) > g_extendedIndexRange(1,kd) &&
                      g_boundaryCondition(1,kd)) {
//                      Point is close to a BC side.  One-sided interpolation used.
                        interpolationStencil_(i,1,kd) = g_extendedIndexRange(1,kd);
                        interpolationStencil_(i,0,kd) = interpolationStencil_(i,1,kd)
                          - (iw0_(kd) - 1);
                    } // end if
                } // end if
            } // end if
        } else if (kd <= interpolationStencil.getBound(2)) {
            interpolationStencil_(i,0,kd) = g_extendedIndexRange(0,kd);
            interpolationStencil_(i,1,kd) = g_extendedIndexRange(1,kd);
        } // end if, end for
    } // end for
#undef g_boundaryCondition
#undef g_gridSpacing
#undef g_indexRange
#undef g_extendedIndexRange
#undef g_isCellCentered
#undef g_isPeriodic
#undef interpolationStencil_
#undef useBackupRules_
#undef iw0_
#undef r_
}
void CompositeGridData::getInterpolationStencil(
  const MappedGrid&   g, // The unrefined grid corresponding to grid[k20].
  const Integer&      k10,
  const Integer&      k20,
  const RealArray&    r,
  const IntegerArray& interpolationStencil,
  const intArray& useBackupRules) {
    MappedGrid& g2 = grid[k20];
    const Real a = -(Real)2. * epsilon, b = (Real)1. - a;
    const Integer base = r.getBase(0), bound = r.getBound(0),
      k1 = componentGridNumber(k10), k2 = componentGridNumber(k20),
      l  = multigridLevelNumber(k10);

#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define refinementFactor_(i,j)       refinementFactor     ((i),(j))
#define g_boundaryCondition(i,j)     g.boundaryCondition  ((i),(j))
#define g_discretizationWidth(i)     g.discretizationWidth((i))
#define g_indexRange(i,j)            g.indexRange         ((i),(j))
#define g_numberOfGhostPoints(i,j)   g.numberOfGhostPoints((i),(j))
#define g_isCellCentered(i)          g.isCellCentered     ((i))
#define g_isPeriodic(i)              g.isPeriodic         ((i))
#define g2_indexRange(i,j)           g2.indexRange        ((i),(j))
#define g2_extendedIndexRange(i,j)   g2.extendedIndexRange((i),(j))
#define g2_gridSpacing(i)            g2.gridSpacing       ((i))
#define g2_useGhostPoints            g2.useGhostPoints()
#define r_(i,j)                      r                    ((i),(j))
#define interpolationStencil_(i,j,k) interpolationStencil ((i),(j),(k))
#define useBackupRules_(i)           useBackupRules       ((i))
#define iw0_(i)                      iw0                  ((i),k1,k2,l)
#else
#define refinementFactor_(i,j)       refinementFactor__    [(i) + 3 * (j)]
#define g_boundaryCondition(i,j)     g_boundaryCondition_  [(i) + 2 * (j)]
#define g_discretizationWidth(i)     g_discretizationWidth_[(i)]
#define g_indexRange(i,j)            g_indexRange_         [(i) + 2 * (j)]
#define g_numberOfGhostPoints(i,j)   g_numberOfGhostPoints_[(i) + 2 * (j)]
#define g_isCellCentered(i)          g_isCellCentered_     [(i)]
#define g_isPeriodic(i)              g_isPeriodic_         [(i)]
#define g2_indexRange(i,j)           g2_indexRange_        [(i) + 2 * (j)]
#define g2_extendedIndexRange(i,j)   g2_extendedIndexRange_[(i) + 2 * (j)]
#define g2_gridSpacing(i)            g2_gridSpacing_       [(i)]
#define g2_useGhostPoints            g2_useGhostPoints_
#define r_(i,j)                      r__                   [(i) + r_s * (j)]
#define interpolationStencil_(i,j,k) \
                     interpolationStencil__[(i) + iS_s1 * (j) + iS_s2 * (k)]
#define useBackupRules_(i)           useBackupRules__      [(i)]
#define iw0_(i)                      iw0__                 [(i)]
    Integer *refinementFactor__     = refinementFactor       .getDataPointer(),
            *g_boundaryCondition_   = g.boundaryCondition()  .getDataPointer(),
            *g_discretizationWidth_ = g.discretizationWidth().getDataPointer(),
            *g_indexRange_          = g.indexRange()         .getDataPointer(),
            *g_numberOfGhostPoints_ = g.numberOfGhostPoints().getDataPointer(),
            *g_isCellCentered_      = g.isCellCentered()     .getDataPointer(),
            *g_isPeriodic_          = g.isPeriodic()         .getDataPointer(),
            *g2_indexRange_         = g2.indexRange()        .getDataPointer(),
            *g2_extendedIndexRange_ = g2.extendedIndexRange().getDataPointer(),
            *interpolationStencil__ = interpolationStencil   .getDataPointer(),
            *useBackupRules__       = useBackupRules         .getDataPointer(),
            *interpolationWidth__   = &interpolationWidth(0,k1,k2,l);
//        *backupInterpolationWidth__ = &backupInterpolationWidth(0,k1,k2,l);
    Logical g2_useGhostPoints_      = g2.useGhostPoints();
    Real    *g2_gridSpacing_        = g2.gridSpacing()       .getDataPointer(),
            *r__                    = r                      .getDataPointer();
    const Integer r_s = &r(base,1) - &r(base,0),
      iS_s1 = &interpolationStencil(base,1,0) -
              &interpolationStencil(base,0,0),
      iS_s2 = &interpolationStencil(base,0,1) -
              &interpolationStencil(base,0,0);
    r__                    = &r_(-base,0);
    interpolationStencil__ = &interpolationStencil_(-base,0,0);
    useBackupRules__       = &useBackupRules_(-base);
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES

#define g_extendedIndexRange(i,j) g_extendedIndexRange_[(j)][(i)]
    Integer g_extendedIndexRange_[3][2], kd, ks;
    for (kd=0; kd<3; kd++) {
        for (ks=0; ks<2; ks++) g_extendedIndexRange(ks,kd) =
          g_indexRange(ks,kd) * refinementFactor_(kd,k20);
        if (g_isCellCentered(kd) || g_isPeriodic(kd))
          g_extendedIndexRange(1,kd) += refinementFactor(kd,k20) - 1;
    } // end for
    for (kd=0; kd<numberOfDimensions; kd++) for (ks=0; ks<2; ks++)
      if (g_boundaryCondition(ks,kd) == 0 && g2_useGhostPoints)
        g_extendedIndexRange(ks,kd) =
          max0(g_extendedIndexRange(0,kd) - g_numberOfGhostPoints(0,kd),
          min0(g_extendedIndexRange(1,kd) + g_numberOfGhostPoints(1,kd),
          g_extendedIndexRange(ks,kd) +
          (2 * ks - 1) * (g_discretizationWidth(kd) - 1) / 2));


    // wdh: we can interpolate from extended index range
    real rBound[3][2];
    for (kd=0; kd<3; kd++) 
    {
      rBound[kd][0]=a+(g2_extendedIndexRange(0,kd)-g2_indexRange(0,kd))*g2_gridSpacing(kd);   
      rBound[kd][1]=b+(g2_extendedIndexRange(1,kd)-g2_indexRange(1,kd))*g2_gridSpacing(kd);
    }

    for (Integer i=base; i<=bound; i++) {
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        IntegerArray& iw0 = interpolationWidth;
//         IntegerArray& iw0 = useBackupRules_(i) ?
//           backupInterpolationWidth : interpolationWidth;
#else
        Integer* iw0__ = interpolationWidth__;
//         Integer* iw0__ = useBackupRules_(i) ?
//           backupInterpolationWidth__ : interpolationWidth__;
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        for (kd=0; kd<3; kd++) if (kd < numberOfDimensions) {
// *wdh     if (r_(i,kd) < a || r_(i,kd) > b) {
            if (r_(i,kd) < rBound[kd][0] || r_(i,kd) > rBound[kd][1]) {
                interpolationStencil_(i,0,kd) =
                interpolationStencil_(i,1,kd) = INTEGER_MAX;
            } else {
                Real rr = r_(i,kd) / g2_gridSpacing(kd) + g2_indexRange(0,kd);
                interpolationStencil_(i,0,kd) =
                  Integer(floor(rr - (Real).5 * iw0_(kd) +
                  (g_isCellCentered(kd) ? (Real).5 : (Real)1.)));
                interpolationStencil_(i,1,kd) =
                  Integer(floor(rr + (Real).5 * iw0_(kd) -
                  (g_isCellCentered(kd) ? (Real).5 : (Real)0.)));
                if (!g_isPeriodic(kd)) {
                    if (interpolationStencil_(i,0,kd) < g_extendedIndexRange(0,kd) &&
                      g_boundaryCondition(0,kd)) {
//                      Point is close to a BC side.  One-sided interpolation used.
                        interpolationStencil_(i,0,kd) = g_extendedIndexRange(0,kd);
                        interpolationStencil_(i,1,kd) = interpolationStencil_(i,0,kd)
                          + (iw0_(kd) - 1);
                    } // end if
                    if (interpolationStencil_(i,1,kd) > g_extendedIndexRange(1,kd) &&
                      g_boundaryCondition(1,kd)) {
//                      Point is close to a BC side.  One-sided interpolation used.
                        interpolationStencil_(i,1,kd) = g_extendedIndexRange(1,kd);
                        interpolationStencil_(i,0,kd) = interpolationStencil_(i,1,kd)
                          - (iw0_(kd) - 1);
                    } // end if
                } // end if
            } // end if
        } else if (kd <= interpolationStencil.getBound(2)) {
            interpolationStencil_(i,0,kd) = g_extendedIndexRange(0,kd);
            interpolationStencil_(i,1,kd) = g_extendedIndexRange(1,kd);
        } // end if, end for
    } // end for
#undef refinementFactor_
#undef g_boundaryCondition
#undef g_discretizationWidth
#undef g_indexRange
#undef g_extendedIndexRange
#undef g_numberOfGhostPoints
#undef g_isCellCentered
#undef g_isPeriodic
#undef g2_indexRange
#undef g2_gridSpacing
#undef g2_useGhostPoints
#undef interpolationStencil_
#undef useBackupRules_
#undef iw0_
#undef r_
}



Logical CompositeGridData::canInterpolate(
  const Integer&      k10,
  const Integer&      k20,
  const realArray&    r,
  const intArray& ok,
  const intArray& useBackupRules,
  const Logical       checkForOneSided) {
//
//  Determine whether points on grid k1 at r in the coordinates of grids k2
//  can be interpolated from grids k2.
//
    MappedGrid& g = grid[k20];
    Integer iv1[3], &i1=iv1[0], &i2=iv1[1], &i3=iv1[2], ks, kd, iab_[2*3];
    Logical isOneSided, oneSided[3][2], returnValue = LogicalTrue, invalid;
    IntegerArray iab2(1,2,3); RealArray rA(1,numberOfDimensions);
// *wdh* 980607    const Real a = -(Real)100. * epsilon, b = (Real)1. - a;
    const Real a = -(Real)2. * epsilon, b = (Real)1. - a;
    const Integer base = r.getBase(0), bound = r.getBound(0),
      k1 = componentGridNumber(k10), k2 = componentGridNumber(k20),
      l  = multigridLevelNumber(k10);

    assert( k10>=0 && k10<numberOfGrids);
    assert( k20>=0 && k20<numberOfGrids);

    assert( k1>=0 && k1<numberOfGrids);
    assert( k2>=0 && k2<numberOfGrids);
    

#define iab(i,j) iab_[(i) + 2 * (j)]
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define g_boundaryCondition(i,j)  g.boundaryCondition  ((i),(j))
#define g_dimension(i,j)          g.dimension          ((i),(j))
#define g_discretizationWidth(i)  g.discretizationWidth((i))
#define g_gridSpacing(i)          g.gridSpacing        ((i))
#define g_indexRange(i,j)         g.indexRange         ((i),(j))
#define g_extendedIndexRange(i,j) g.extendedIndexRange ((i),(j))
#define g_isCellCentered(i)       g.isCellCentered     ((i))
#define g_isPeriodic(i)           g.isPeriodic         ((i))
#define g_mask(i,j,k)             g.mask()             ((i),(j),(k))
#define r_(i,j)                   r                    ((i),(j))
#define useBackupRules_(i)        useBackupRules       ((i))
#define ok_(i)                    ok                   ((i))
#define iw0_(i)                   iw0                  ((i),k1,k2,l)
#define ov0_(i)                   ov0                  ((i),k1,k2,l)
#else
#define g_boundaryCondition(i,j)  g_boundaryCondition_  [(i) + 2 * (j)]
#define g_dimension(i,j)          g_dimension_          [(i) + 2 * (j)]
#define g_discretizationWidth(i)  g_discretizationWidth_[(i)]
#define g_gridSpacing(i)          g_gridSpacing_        [(i)]
#define g_indexRange(i,j)         g_indexRange_         [(i) + 2 * (j)]
#define g_extendedIndexRange(i,j) g_extendedIndexRange_ [(i) + 2 * (j)]
#define g_isCellCentered(i)       g_isCellCentered_     [(i)]
#define g_isPeriodic(i)           g_isPeriodic_         [(i)]
#define g_mask(i,j,k)             g_mask_               [(i)+i10*(j)+j10*(k)]
#define r_(i,j)                   r__                   [(i) + r_s * (j)]
#define useBackupRules_(i)        useBackupRules__      [(i)]
#define ok_(i)                    ok__                  [(i)]
#define iw0_(i)                   iw0__                 [(i)]
#define ov0_(i)                   ov0__                 [(i)]
    Integer *g_boundaryCondition_   = g.boundaryCondition()  .getDataPointer(),
            *g_dimension_           = g.dimension()          .getDataPointer(),
            *g_discretizationWidth_ = g.discretizationWidth().getDataPointer(),
            *g_indexRange_          = g.indexRange()         .getDataPointer(),
//            *g_extendedIndexRange_  = g.extendedIndexRange() .getDataPointer(),
            *g_isCellCentered_      = g.isCellCentered()     .getDataPointer(),
            *g_isPeriodic_          = g.isPeriodic()         .getDataPointer(),
            *g_mask_                = g.mask()               .getDataPointer(),
            *useBackupRules__       = useBackupRules         .getDataPointer(),
            *ok__                   = ok                     .getDataPointer(),
            *interpolationWidth__   = &interpolationWidth(0,k1,k2,l);
    
//        *backupInterpolationWidth__ = &backupInterpolationWidth(0,k1,k2,l);
    Real    *g_gridSpacing_         = g.gridSpacing()        .getDataPointer(),
            *r__                    = r                      .getDataPointer(),
            *interpolationOverlap__ = &interpolationOverlap(0,k1,k2,l);
//      *backupInterpolationOverlap__ = &backupInterpolationOverlap(0,k1,k2,l);
    const Integer i10 = g_dimension(1,0) - g_dimension(0,0) + 1,
           j10 = i10 * (g_dimension(1,1) - g_dimension(0,1) + 1),
      r_s = &r(base,1) - &r(base,0);
    g_mask_ = &g_mask(-g_dimension(0,0),-g_dimension(0,1),-g_dimension(0,2));
    r__               = &r_(-base,0);
    ok__              = &ok_(-base);
    useBackupRules__  = &useBackupRules_(-base);
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
    const Integer *g_I1 = g.I1(), *g_I2 = g.I2(), *g_I3 = g.I3();
    
    // wdh: we can interpolate from extended index range
    real rBound[3][2];
    for (kd=0; kd<3; kd++) 
    {
      rBound[kd][0]=a+(g.extendedRange(0,kd)-g_indexRange(0,kd))*g_gridSpacing(kd);   
      rBound[kd][1]=b+(g.extendedRange(1,kd)-g_indexRange(1,kd))*g_gridSpacing(kd);
    }

    for (Integer i=base; i<=bound; i++) if (ok_(i)) {
//
//      Determine the stencil of points to check.
//
#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        IntegerArray& iw0 = interpolationWidth;
        RealArray& ov0 = interpolationOverlap;
//         IntegerArray& iw0 = useBackupRules_(i) ?
//           backupInterpolationWidth : interpolationWidth;
//         RealArray& ov0 = useBackupRules_(i) ?
//           backupInterpolationOverlap : interpolationOverlap;
#else
        Integer* iw0__ = interpolationWidth__;
        Real* ov0__ = interpolationOverlap__;
//         Integer* iw0__ = useBackupRules_(i) ?
//           backupInterpolationWidth__ : interpolationWidth__;
//         Real* ov0__ = useBackupRules_(i) ?
//           backupInterpolationOverlap__ : interpolationOverlap__;
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
        invalid = isOneSided = LogicalFalse;
        for (kd=0; kd<3; kd++) 
        {
            oneSided[kd][0] = oneSided[kd][1] = LogicalFalse;
            if (kd < numberOfDimensions) 
            {
	      // *wdh if (invalid = r_(i,kd) < a || r_(i,kd) > b) break;
	      if( (invalid = (r_(i,kd) < rBound[kd][0] || r_(i,kd) > rBound[kd][1])) ) break;
                Real rr = r_(i,kd) / g_gridSpacing(kd) + g_indexRange(0,kd);

                // real overlap=ov0_(kd); // *wdh*
		
                iab(0,kd) = Integer(floor(rr - ov0_(kd) -
                  (g_isCellCentered(kd) ? (Real).5 : (Real)0.)));
                iab(1,kd) = Integer(floor(rr + ov0_(kd) +
                  (g_isCellCentered(kd) ? (Real).5 : (Real)1.)));
                if (!g_isPeriodic(kd)) {
                    if (iab(0,kd) < g.extendedRange(0,kd)) {
//                      Check if point is too close to an interpolated side.
		      if( (invalid = !g_boundaryCondition(0,kd)) ) break;
//                      One-sided interpolation is used close to a boundary.
                        isOneSided = oneSided[kd][0] = LogicalTrue;
                        iab(0,kd) = g.extendedRange(0,kd);
                        iab(1,kd) = iab(0,kd) +
                          Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
                    } // end if
                    if (iab(1,kd) > g.extendedRange(1,kd)) {
//                      Check if point is too close to an interpolated side.
		      if( (invalid = !g_boundaryCondition(1,kd)) ) break;
//                      One-sided interpolation is used close to a boundary.
                        isOneSided = oneSided[kd][1] = LogicalTrue;
                        iab(1,kd) = g.extendedRange(1,kd);
                        iab(0,kd) = iab(1,kd) -
                          Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
                    } // end if
                } // end if
            } else {
                iab(0,kd) = g.extendedRange(0,kd);
                iab(1,kd) = g.extendedRange(1,kd);
            } // end if
        } // end for
//
//      Check that all points in the stencil are either discretization points
//      or interpolation points.  Backup discretization points and backup
//      interpolation points are also allowed.
//
        if (!invalid) 
	{
	  COMPOSITE_GRID_FOR_3(iab_, i1, i2, i3)
	  {
	    if( (invalid = invalid ||
		 !(g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) & ISusedPoint)) ) break;
	  }
	  
	}
	
        if (!invalid && checkForOneSided && isOneSided) {
//
//          Check for one-sided interpolation from BC points
//          that interpolate from the interior of another grid.
//
//          Find the interpolation stencil.
            for (kd=0; kd<numberOfDimensions; kd++) rA(0,kd) = r_(i,kd);
            getInterpolationStencil(k10, k20, rA, iab2, useBackupRules);

#ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
#define     iab2_(i,j,k) iab2((i),(j),(k))
#else
#define     iab2_(i,j,k) iab2__[(j) + 2 * (k)]
#endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
            Integer* iab2__ = iab2.getDataPointer();
            for (kd=0; kd<3; kd++) {
                for (ks=0; ks<2; ks++) if (oneSided[kd][ks]) {
                    Integer iab21=iab2_(0,0,kd), iab22=iab2_(0,1,kd);
                    // *wdh* 021205: check added getInterpolationStencil will return this value for bogus pts
                    if( iab21==INT_MAX ) 
		    {
		      invalid=true;
		      break;
		    }
//                  Restrict the interpolation stencil to points that could be
//                  boundary discretization points of side (kd,ks) of the grid.
                    if (ks == 0) {
                      iab2_(0,0,kd) = g.extendedRange(0,kd);
                      iab2_(0,1,kd) = min0(
                        iab2_(0,1,kd),
                        iab2_(0,0,kd) +
                        (g_discretizationWidth(kd) - 1) / 2 - 1);
                    } else {
                      iab2_(0,1,kd) = g.extendedRange(1,kd);
                      iab2_(0,0,kd) = max0(iab2_(0,0,kd), iab2_(0,1,kd) -
                         (g_discretizationWidth(kd) - 1) / 2 + 1);
                    } // end if
//
//                  Check that all points in the stencil are either
//                  discretization points or interpolation points that are not
//                  interpolated one-sided from another grid.  Backup
//                  discretization points and backup interpolation points that
//                  are not interpolated one-sided from another grid are also
//                  allowed.
//
/* ------ *wdh* 980702
                    COMPOSITE_GRID_FOR_3(iab2__, i1, i2, i3)
                      if (invalid = invalid ||
                        g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) &
                        ISinteriorBoundaryPoint) break;
                    if (invalid) break;
------- */
                    COMPOSITE_GRID_FOR_3(iab2__, i1, i2, i3)
		    {
                      if( (invalid = invalid || g_mask(g_I1[i1],g_I2[i2],g_I3[i3]) & ISinteriorBoundaryPoint) )
		      {
                        // Make sure that we are not too close to an the interpolation point
			real rDist=0.;
                        real cellCenterederedOffset=g_isCellCentered(kd) ? .5 : 0.;
			for( int dir=0; dir<numberOfDimensions; dir++ )
			  rDist=max(rDist,fabs( r_(i,dir)/g_gridSpacing(dir)
						-(iv1[dir]+cellCenterederedOffset-g_indexRange(Start,dir))));
			if( rDist > ov0_(0) )  // use ov_(0) as the minimum overlap. Normally=.5
			{
			  // printf("CompositeGrid::canInterpolate: near an interior boundary point but rDist=%e"
                          //       ", ov=%6.2e, so this point is ok! \n",rDist,ov0_(0));
                          invalid=FALSE;  // this point is ok after all
			}
                        else
			  break;
		      }
                      if (invalid) break;
		    }
		    
//                  Restore the interpolation stencil;
                    iab2_(0,0,kd) = iab21;
                    iab2_(0,1,kd) = iab22;
                } // end if, end for
                if (invalid) break;
            } // end for
        } // end if

        if (invalid) ok_(i) = returnValue = LogicalFalse;

    } else {
        returnValue = LogicalFalse;
    } // end if, end for
    return returnValue;
#undef iab
#undef g_boundaryCondition
#undef g_dimension
#undef g_discretizationWidth
#undef g_gridSpacing
#undef g_indexRange
#undef g_extendedIndexRange
#undef g_isCellCentered
#undef g_isPeriodic
#undef g_mask
#undef r_
#undef useBackupRules_
#undef ok_
#undef iw0_
#undef ov0_
#undef iab2_
}


#if 0
Logical CompositeGridData::
canInterpolate(
  const Integer&      k10,
  const Integer&      k20,
  const realArray&    r,
  const intArray& ok,
  const intArray& useBackupRules,
  const Logical       checkForOneSided) 
{
  return canInterpolate(k10,k20,r.getLocalArray(),ok.getLocalArray(), useBackupRules.getLocalArray(), checkForOneSided );
}
#endif


// Logical CompositeGridData::canInterpolate(
//   const MappedGrid&   g, // The unrefined grid corresponding to grid[k20].
//   CompositeMask&      g_mask, // Masks on k2 (possibly) and its siblings.
//   const Integer&      k10,
//   const Integer&      k20,
//   const RealArray&    r,
//   const LogicalArray& ok,
//   const LogicalArray& useBackupRules,
//   const Logical       checkForOneSided) {
// //
// //  Determine whether points on grid k1 at r in the coordinates of grids k2
// //  can be interpolated from grids k2.
// //
//     MappedGrid& g2 = grid[k20];
//     Integer iv1[3], &i1=iv1[0], &i2=iv1[1], &i3=iv1[2], ks, kd, iab_[2*3];
//     Logical isOneSided, oneSided[3][2], returnValue = LogicalTrue, invalid;
//     IntegerArray iab2(1,2,3); RealArray rA(1,numberOfDimensions);
//     const Integer base = r.getBase(0), bound = r.getBound(0),
//       k1 = componentGridNumber(k10), k2 = componentGridNumber(k20),
//       l  = multigridLevelNumber(k10);

// #define iab(i,j) iab_[(i) + 2 * (j)]
// #ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
// #define refinementFactor_(i,j)     refinementFactor     ((i),(j))
// #define g_boundaryCondition(i,j)   g.boundaryCondition  ((i),(j))
// #define g_discretizationWidth(i)   g.discretizationWidth((i))
// #define g_indexRange(i,j)          g.indexRange         ((i),(j))
// #define g_numberOfGhostPoints(i,j) g.numberOfGhostPoints((i),(j))
// #define g_isCellCentered(i)        g.isCellCentered     ((i))
// #define g_isPeriodic(i)            g.isPeriodic         ((i))
// #define g2_indexRange(i,j)         g2.indexRange        ((i),(j))
// #define g2_gridSpacing(i)          g2.gridSpacing       ((i))
// #define g2_useGhostPoints          g2.useGhostPoints()
// #define r_(i,j)                    r                    ((i),(j))
// #define useBackupRules_(i)         useBackupRules       ((i))
// #define ok_(i)                     ok                   ((i))
// #define iw0_(i)                    iw0                  ((i),k1,k2,l)
// #define ov0_(i)                    ov0                  ((i),k1,k2,l)
// #else
// #define refinementFactor_(i,j)     refinementFactor__    [(i) + 3 * (j)]
// #define g_boundaryCondition(i,j)   g_boundaryCondition_  [(i) + 2 * (j)]
// #define g_discretizationWidth(i)   g_discretizationWidth_[(i)]
// #define g_indexRange(i,j)          g_indexRange_         [(i) + 2 * (j)]
// #define g_numberOfGhostPoints(i,j) g_numberOfGhostPoints_[(i) + 2 * (j)]
// #define g_isCellCentered(i)        g_isCellCentered_     [(i)]
// #define g_isPeriodic(i)            g_isPeriodic_         [(i)]
// #define g2_indexRange(i,j)         g2_indexRange_        [(i) + 2 * (j)]
// #define g2_gridSpacing(i)          g2_gridSpacing_       [(i)]
// #define g2_useGhostPoints          g2_useGhostPoints_
// #define r_(i,j)                    r__                   [(i) + r_s * (j)]
// #define useBackupRules_(i)         useBackupRules__      [(i)]
// #define ok_(i)                     ok__                  [(i)]
// #define iw0_(i)                    iw0__                 [(i)]
// #define ov0_(i)                    ov0__                 [(i)]
//     Integer *refinementFactor__     = refinementFactor       .getDataPointer(),
//             *g_boundaryCondition_   = g.boundaryCondition()  .getDataPointer(),
//             *g_discretizationWidth_ = g.discretizationWidth().getDataPointer(),
//             *g_indexRange_          = g.indexRange()         .getDataPointer(),
//             *g_numberOfGhostPoints_ = g.numberOfGhostPoints().getDataPointer(),
//             *g_isCellCentered_      = g.isCellCentered()     .getDataPointer(),
//             *g_isPeriodic_          = g.isPeriodic()         .getDataPointer(),
//             *g2_indexRange_         = g2.indexRange()        .getDataPointer(),
//             *useBackupRules__       = useBackupRules         .getDataPointer(),
//             *ok__                   = ok                     .getDataPointer(),
//             *interpolationWidth__   = &interpolationWidth(0,k1,k2,l),
//         *backupInterpolationWidth__ = &backupInterpolationWidth(0,k1,k2,l);
//     Logical g2_useGhostPoints_      = g2.useGhostPoints();
//     Real    *g2_gridSpacing_        = g2.gridSpacing()       .getDataPointer(),
//             *r__                    = r                      .getDataPointer(),
//             *interpolationOverlap__ = &interpolationOverlap(0,k1,k2,l),
//       *backupInterpolationOverlap__ = &backupInterpolationOverlap(0,k1,k2,l);
//     const Integer r_s = &r(base,1) - &r(base,0);
//     r__               = &r_(-base,0);
//     ok__              = &ok_(-base);
//     useBackupRules__  = &useBackupRules_(-base);
// #endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES

// #define g_extendedIndexRange(i,j) g_extendedIndexRange_[(j)][(i)]
//     Integer g_extendedIndexRange_[3][2];
//     for (kd=0; kd<3; kd++) {
//         for (ks=0; ks<2; ks++) g_extendedIndexRange(ks,kd) =
//           g_indexRange(ks,kd) * refinementFactor_(kd,k20);
//         if (g_isCellCentered(kd) || g_isPeriodic(kd))
//           g_extendedIndexRange(1,kd) += refinementFactor(kd,k20) - 1;
//     } // end for
//     for (kd=0; kd<numberOfDimensions; kd++) for (ks=0; ks<2; ks++)
//       if (g_boundaryCondition(ks,kd) == 0 && g2_useGhostPoints)
//         g_extendedIndexRange(ks,kd) =
//           max0(g_extendedIndexRange(0,kd) - g_numberOfGhostPoints(0,kd),
//           min0(g_extendedIndexRange(1,kd) + g_numberOfGhostPoints(1,kd),
//           g_extendedIndexRange(ks,kd) +
//           (2 * ks - 1) * (g_discretizationWidth(kd) - 1) / 2));

//     for (Integer i=base; i<=bound; i++) if (ok_(i)) {
// //
// //      Determine the stencil of points to check.
// //
// #ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
//         IntegerArray& iw0 = useBackupRules_(i) ?
//           backupInterpolationWidth : interpolationWidth;
//         RealArray& ov0 = useBackupRules_(i) ?
//           backupInterpolationOverlap : interpolationOverlap;
// #else
//         Integer* iw0__ = useBackupRules_(i) ?
//           backupInterpolationWidth__ : interpolationWidth__;
//         Real* ov0__ = useBackupRules_(i) ?
//           backupInterpolationOverlap__ : interpolationOverlap__;
// #endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
//         invalid = isOneSided = LogicalFalse;
//         for (kd=0; kd<3; kd++) {
//             oneSided[kd][0] = oneSided[kd][1] = LogicalFalse;
//             if (kd < numberOfDimensions) {
//                 Real rr = r_(i,kd) / g2_gridSpacing(kd) + g2_indexRange(0,kd);
//                 iab(0,kd) = Integer(floor(rr - ov0_(kd) -
//                   (g_isCellCentered(kd) ? (Real).5 : (Real)0.)));
//                 iab(1,kd) = Integer(floor(rr + ov0_(kd) +
//                   (g_isCellCentered(kd) ? (Real).5 : (Real)1.)));
//                 if (!g_isPeriodic(kd)) {
//                     if (iab(0,kd) < g_extendedIndexRange(0,kd)) {
// //                      Check if point is too close to an interpolated side.
//                         if (invalid = !g_boundaryCondition(0,kd)) break;
// //                      One-sided interpolation is used close to a boundary.
//                         isOneSided = oneSided[kd][0] = LogicalTrue;
//                         iab(0,kd) = g_extendedIndexRange(0,kd);
//                         iab(1,kd) = iab(0,kd) +
//                           Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
//                     } // end if
//                     if (iab(1,kd) > g_extendedIndexRange(1,kd)) {
// //                      Check if point is too close to an interpolated side.
//                         if (invalid = !g_boundaryCondition(1,kd)) break;
// //                      One-sided interpolation is used close to a boundary.
//                         isOneSided = oneSided[kd][1] = LogicalTrue;
//                         iab(1,kd) = g_extendedIndexRange(1,kd);
//                         iab(0,kd) = iab(1,kd) -
//                           Integer(floor((Real).5 * iw0_(kd) + ov0_(kd) + (Real).5));
//                     } // end if
//                 } // end if
//             } else {
//                 iab(0,kd) = g_extendedIndexRange(0,kd);
//                 iab(1,kd) = g_extendedIndexRange(1,kd);
//             } // end if
//         } // end for
// //
// //      Check that all points in the stencil are either discretization points
// //      or interpolation points.  Backup discretization points and backup
// //      interpolation points are also allowed.
// //
//         if (!invalid) COMPOSITE_GRID_FOR_3(iab_, i1, i2, i3)
//           if (invalid = invalid ||
//             !((const Integer&)g_mask(i1,i2,i3) & ISusedPoint)) break;

//         if (!invalid && checkForOneSided && isOneSided) {
// //
// //          Check for one-sided interpolation from BC points
// //          that interpolate from the interior of another grid.
// //
// //          Find the interpolation stencil.
//             for (kd=0; kd<numberOfDimensions; kd++) rA(0,kd) = r_(i,kd);
//             getInterpolationStencil(g, k10, k20, rA, iab2, useBackupRules);

// #ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
// #define     iab2_(i,j,k) iab2((i),(j),(k))
// #else
// #define     iab2_(i,j,k) iab2__[(j) + 2 * (k)]
// #endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
//             Integer* iab2__ = iab2.getDataPointer();
//             for (kd=0; kd<3; kd++) {
//                 for (ks=0; ks<2; ks++) if (oneSided[kd][ks]) {
//                     Integer iab21=iab2_(0,0,kd), iab22=iab2_(0,1,kd);
// //                  Restrict the interpolation stencil to points that could be
// //                  boundary discretization points of side (kd,ks) of the grid.
//                     if (ks == 0) {
//                       iab2_(0,0,kd) = g_extendedIndexRange(0,kd);
//                       iab2_(0,1,kd) = min0(
//                         iab2_(0,1,kd),
//                         iab2_(0,0,kd) +
//                         (g_discretizationWidth(kd) - 1) / 2 - 1);
//                     } else {
//                       iab2_(0,1,kd) = g_extendedIndexRange(1,kd);
//                       iab2_(0,0,kd) = max0(iab2_(0,0,kd), iab2_(0,1,kd) -
//                          (g_discretizationWidth(kd) - 1) / 2 + 1);
//                     } // end if
// //
// //                  Check that all points in the stencil are either
// //                  discretization points or interpolation points that are not
// //                  interpolated one-sided from another grid.  Backup
// //                  discretization points and backup interpolation points that
// //                  are not interpolated one-sided from another grid are also
// //                  allowed.
// //
//                     COMPOSITE_GRID_FOR_3(iab2__, i1, i2, i3)
//                       if (invalid = invalid || (const Integer&)
//                         g_mask(i1,i2,i3) & ISinteriorBoundaryPoint) break;
//                     if (invalid) break;

// //                  Restore the interpolation stencil;
//                     iab2_(0,0,kd) = iab21;
//                     iab2_(0,1,kd) = iab22;
//                 } // end if, end for
//                 if (invalid) break;
//             } // end for
//         } // end if

//         if (invalid) ok_(i) = returnValue = LogicalFalse;

//     } else {
//         returnValue = LogicalFalse;
//     } // end if, end for
//     return returnValue;
// #undef iab
// #undef refinementFactor_
// #undef g_boundaryCondition
// #undef g_discretizationWidth
// #undef g_indexRange
// #undef g_numberOfGhostPoints
// #undef g_extendedIndexRange
// #undef g_isCellCentered
// #undef g_isPeriodic
// #undef g2_indexRange
// #undef g2_gridSpacing
// #undef g2_useGhostPoints
// #undef r_
// #undef useBackupRules_
// #undef ok_
// #undef iw0_
// #undef ov0_
// #undef iab2_
// }
//
// Check if these boundary discretization points of this grid
// lie at least epsilon inside the parameter space of grid g2.
//
// void CompositeGridData::isInteriorBoundaryPoint(
//   const Integer&      k1,
//   const Integer&      k2,
//   const IntegerArray& i1,
//   const RealArray&    r2,
//   const LogicalArray& ok) {
//     MappedGrid &g1 = grid[k1], &g2 = grid[k2];
//     RealArray r, x; r.redim(r2); x.redim(r2); r = (Real).5;
//     IntegerArray numberOfSides; numberOfSides.redim(ok);
//     Logical areAllBoundaryPoints = LogicalTrue;
//     const Integer base = i1.getBase(0), bound = i1.getBound(0);
//     IntegerArray i1b = i1;
// #ifdef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
// #define g1_boundaryCondition(i,j) g1.boundaryCondition((i),(j))
// #define g1_discretizationWidth(i) g1.discretizationWidth((i))
// #define g1_gridIndexRange(i,j)    g1.gridIndexRange((i),(j))
// #define g1_gridSpacing(i)         g1.gridSpacing((i))
// #define g1_indexRange(i,j)        g1.indexRange((i),(j))
// #define g1_isCellCentered(i)      g1.isCellCentered((i))
// #define g2_boundaryCondition(i,j) g2.boundaryCondition((i),(j))
// #define i1b_(i,j)                 i1b((i),(j))
// #define r_(i,j)                   r((i),(j))
// #define ok_(i)                    ok((i))
// #define numberOfSides_(i) numberOfSides((i))
// #else
// #define g1_boundaryCondition(i,j) g1_boundaryCondition__  [(i) + 2     * (j)]
// #define g1_discretizationWidth(i) g1_discretizationWidth__[(i)              ]
// #define g1_gridIndexRange(i,j)    g1_gridIndexRange__     [(i) + 2     * (j)]
// #define g1_gridSpacing(i)         g1_gridSpacing__        [(i)              ]
// #define g1_indexRange(i,j)        g1_indexRange__         [(i) + 2     * (j)]
// #define g1_isCellCentered(i)      g1_isCellCentered__     [(i)              ]
// #define g2_boundaryCondition(i,j) g2_boundaryCondition_[(i) + 2     * (j)]
// #define i1b_(i,j)                 i1b__                [(i) + i1b_s * (j)]
// #define r_(i,j)                   r__                  [(i) + r_s   * (j)]
// #define ok_(i)                    ok__                 [(i)              ]
// #define numberOfSides_(i)         numberOfSides__      [(i)              ]
//     Integer   *g1_boundaryCondition__ = g1.boundaryCondition().getDataPointer(),
//           *g1_discretizationWidth__ = g1.discretizationWidth().getDataPointer(),
//               *g1_gridIndexRange__    = g1.gridIndexRange()   .getDataPointer(),
//               *g1_indexRange__        = g1.indexRange()       .getDataPointer(),
//               *g1_isCellCentered__    = g1.isCellCentered()   .getDataPointer(),
//               *g2_boundaryCondition_  = g2.boundaryCondition().getDataPointer(),
//               *i1b__                  = i1b                   .getDataPointer();
//     Real      *g1_gridSpacing__       = g1.gridSpacing()      .getDataPointer(),
//               *r__                    = r                     .getDataPointer();
//     LogicalAE *ok__                   = ok                    .getDataPointer(),
//               *numberOfSides__        = numberOfSides         .getDataPointer();
//     const Integer  i1b_s = &i1b(base,1) - &i1b(base,0),
//                    r_s   = &r(base,1)   - &r(base,0);
//     i1b__           = &i1b_(-base,0);
//     r__             = &r_  (-base,0);
//     ok__            = &ok_ (-base);
//     numberOfSides__ = &numberOfSides_(-base);
// #endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES

//     Integer i;
//     for (i=base; i<=bound; i++) if (ok_(i)) {
// //
// //      Find out how many sides of this grid the points lie on.
// //
//         numberOfSides_(i) = 0;
//         for (Integer kd=0; kd<numberOfDimensions; kd++) {
//             r_(i,kd) = (i1b_(i,kd) - g1_gridIndexRange(0,kd)) *
//               g1_gridSpacing(kd);
//             if (g1_isCellCentered(kd))
//               r_(i,kd) += (Real).5 * g1_gridSpacing(kd);
//             if (g1_boundaryCondition(0,kd) > 0 &&
//               i1b_(i,kd) < g1_indexRange(0,kd) +
//               (g1_discretizationWidth(kd) - 1) / 2) {
// //              The point is on the left side.
//                 numberOfSides_(i)++;
//                 if (g1_isCellCentered(kd) ||
//                   i1b_(i,kd) != g1_gridIndexRange(0,kd)) {
// //                  This is not a point on the boundary.
// //                  Compute the corresponding point on the boundary.
//                     i1b_(i,kd) = g1_gridIndexRange(0,kd);
//                     r_(i,kd) = (Real)0.;
//                     areAllBoundaryPoints = LogicalFalse;
//                 } // end if
//             } // end if
//             if (g1_boundaryCondition(1,kd) > 0 &&
//               i1b_(i,kd) > g1_indexRange(1,kd) -
//               (g1_discretizationWidth(kd) - 1) / 2) {
// //              The point is on the right side.
//                 numberOfSides_(i)++;
//                 if (g1_isCellCentered(kd) ||
//                   i1b_(i,kd) != g1_gridIndexRange(1,kd)) {
// //                  This is not a point on the boundary.
// //                  Compute the corresponding point on the boundary.
//                     i1b_(i,kd) = g1_gridIndexRange(1,kd);
//                     r_(i,kd) = (Real)1.;
//                     areAllBoundaryPoints = LogicalFalse;
//                 } // end if
//             } // end if
//         } // end for
//     } // end if, end for

//     if (areAllBoundaryPoints) {
// //
// //      All points lie on the boundary of this grid, and r2
// //      contains their coordinates in the parameter space of g2.
// //
//         r.reference(r2);

//     } else {
// //
// //      Some points do not lie on the boundary of this grid.
// //      Compute the corresponding boundary points and their
// //      coordinates in the parameter space of grid g2.
// //
//         g1.mapping().map(r, x);
//         adjustBoundary(k1, baseGridNumber(k2), i1b, x);
//         g2.mapping().inverseMap(x, r = r2);
//         const Range p(base,bound);
//         LogicalArray ok1(p); ok1 = r(p,0) != (Real)10.;
//         where (ok) ok(p) = ok1;
//     } // end if

// #ifndef DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
//     r__ = r.getDataPointer();
//     r__ = &r_(-base,0);
// #endif // DO_NOT_OPTIMIZE_SCALAR_ARRAY_REFERENCES
//     for (i=base; i<=bound; i++) if (ok_(i)) {
// //      Check if the points interpolate from the interior of grid g2.
// //***** Do not consider interior boundary conditions (e.g. polar axis).
//         for (Integer kd=0; kd<numberOfDimensions; kd++)
//           if ((g2_boundaryCondition(0,kd) > 0 &&
//                          r_(i,kd) < epsilon) ||
//               (g2_boundaryCondition(1,kd) > 0 &&
//               (Real)1. - r_(i,kd) < epsilon)) numberOfSides_(i)--;
// //
// //      The point lies in the interior of grid g2 only if numberOfSides > 0.
// //
//         if (numberOfSides_(i) <= 0) ok_(i) = LogicalFalse;
//     } // end if, end for
// #undef g1_boundaryCondition
// #undef g1_discretizationWidth
// #undef g1_gridIndexRange
// #undef g1_gridSpacing
// #undef g1_indexRange
// #undef g1_isCellCentered
// #undef g2_boundaryCondition
// #undef i1b_
// #undef r_
// #undef ok_
// #undef numberOfSides_
// }

// void CompositeGridData::adjustBoundary(
//   const Integer&      k1,
//   const Integer&      k2,
//   const IntegerArray& i1,
//   const RealArray&    x) {
// //
// //  Adjust the position x of points i1 of grid k1 interpolated from
// //  base grid k2 to take into account mismatch between shared boundaries.
// //
//     if (boundaryAdjustment.getNumberOfElements()) {
//         BoundaryAdjustmentArray& bA12 = boundaryAdjustment(k1,k2);
//         if (bA12.getNumberOfElements()) {
//             const Integer base = i1.getBase(0), bound = i1.getBound(0);
//             Integer jv[3], &j1=jv[0], &j2=jv[1], &j3=jv[2], kd1, kd2, ks1;
//             RealArray x0 = x;
//             for (kd1=numberOfDimensions; kd1<3; kd1++)
//               jv[kd1] = grid[k1].indexRange(0,kd1);
//             for (kd1=0; kd1<numberOfDimensions; kd1++)
//               for (ks1=0; ks1<2; ks1++) {
//                 BoundaryAdjustment& bA = bA12(ks1,kd1);
//                 if ((bA.computedGeometry & (THEinverseMap | THEmask)) ==
//                   (THEinverseMap | THEmask)) {
//                     jv[kd1] = bA.boundaryAdjustment.getBase(kd1);
//                     for (Integer i=base; i<=bound; i++) {
//                         for (kd2=0; kd2<numberOfDimensions; kd2++)
//                           if (kd2 != kd1) jv[kd2] = i1(i,kd2);
//                         Real dot = (Real)0.;
//                         for (kd2=0; kd2<numberOfDimensions; kd2++)
//                           dot += bA.acrossGrid(j1,j2,j3,kd2) *
//                             (bA.oppositeBoundary(j1,j2,j3,kd2) - x0(i,kd2));
//                         for (kd2=0; kd2<numberOfDimensions; kd2++)
//                           x(i,kd2) += dot * bA.boundaryAdjustment(j1,j2,j3,kd2);
//                     } // end for
//                 } // end if
//             } // end for, end for
//         } // end if
//     } // end if
// }

// =========================================================================================
/// \brief update "collections" such as THEbaseGrid, THErefinementLevel, THEcomponentGrid, 
///    THEmultigridLevel, and THEdomain. 
/// \details The collections are lists that consist of sub-sets of the total set of grids. 
//     For example, cg.refinementLevel[l] is a GridCollection that holds all grids on a
//     given refinement level.
// =========================================================================================
Integer CompositeGridData::updateCollection(
  const Integer&                   what,
  Integer&                         numberOfCollections,
#ifdef USE_STL
  RCVector<CompositeGrid>&         list,
  RCVector<GridCollection>&        gridCollectionList,
  RCVector<GenericGridCollection>& genericGridCollectionList,
#else
  ListOfCompositeGrid&             list,
  ListOfGridCollection&            gridCollectionList,
  ListOfGenericGridCollection&     genericGridCollectionList,
#endif // USE_STL
  IntegerArray&                    number) 
{
//  Fix up the length of list.
  numberOfCollections = numberOfGrids > 0 ? max(number) + 1 : 0;
#ifdef STL
  if (list.size() > numberOfCollections)
    list.erase(list.begin() + numberOfCollections, list.end());
  if (gridCollectionList.size() > numberOfCollections)
    gridCollectionList.erase(
      gridCollectionList.begin() + numberOfCollections,
      gridCollectionList.end());
  if (genericGridCollectionList.size() > numberOfCollections)
    genericGridCollectionList.erase(
      genericGridCollectionList.begin() + numberOfCollections,
      genericGridCollectionList.end());
#else
  while (list.getLength() > numberOfCollections) list.deleteElement();
  while (gridCollectionList.getLength() > numberOfCollections)
    gridCollectionList.deleteElement();
  while (genericGridCollectionList.getLength() > numberOfCollections)
    genericGridCollectionList.deleteElement();
#endif // STL
  if (numberOfCollections) 
  {
//      Fill lists with appropriately-constructed CompositeGrids.
    IntegerArray nG(numberOfCollections); nG = 0; Integer k, i;
    for (k=0; k<numberOfGrids; k++) nG(number(k))++;
    for (i=0; i<numberOfCollections; i++) 
     {
      if (i < list.getLength())
	list[i].setNumberOfDimensionsAndGrids(numberOfDimensions, nG(i));
#ifdef USE_STL
      else list.push_back(CompositeGrid(numberOfDimensions, nG(i)));
      if (i < gridCollectionList.size())
	gridCollectionList[i].reference(list[i]);
      else gridCollectionList.push_back(list[i]);
      if (i < genericGridCollectionList.size())
	genericGridCollectionList[i].reference(list[i]);
      else genericGridCollectionList.push_back(list[i]);
#else
      else list.addElement(CompositeGrid(numberOfDimensions, nG(i)));
      if (i < gridCollectionList.getLength())
	gridCollectionList[i].reference(list[i]);
      else gridCollectionList.addElement(list[i]);
      if (i < genericGridCollectionList.getLength())
	genericGridCollectionList[i].reference(list[i]);
      else genericGridCollectionList.addElement(list[i]);
#endif // USE_STL
    } // end for

    GridCollectionData::updateCollection(what, numberOfCollections, gridCollectionList,
                                         genericGridCollectionList, number);

    for (i=0; i<numberOfCollections; i++)
      list[i].setNumberOfDimensionsAndGrids(numberOfDimensions, nG(i));
    for (nG=0, k=0; k<numberOfGrids; k++) 
    {
      i = number(k);                // grid k belongs to list i 

      const Integer j = nG(i)++;    // j = current number of grids in list i 
      const Range three = 3, 
      // *wdh	991023        allGrids = numberOfComponentGrids,
      // *wdh 991023         allGridsPlusOne = numberOfComponentGrids + 1;
	allGrids = list[i].numberOfComponentGrids(),
	allGridsPlusOne = list[i].numberOfComponentGrids() + 1;

      // *wdh* 060815 -- copy interp data from appropriate MG level
      const int level = what & THEmultigridLevel ? number(k) : 0;
      
      list[i].numberOfCompleteMultigridLevels() = i < numberOfCompleteMultigridLevels ? 1 : 0;
      list[i].epsilon() = epsilon;
      list[i].numberOfInterpolationPoints(j) = 	numberOfInterpolationPoints(k);

      // These next values are over-written below for all but MG levels 
      list[i].interpolationIsAllExplicit() = interpolationIsAllExplicit;
      list[i].interpolationIsAllImplicit() = interpolationIsAllImplicit;
      list[i].interpolationIsImplicit(allGrids,allGrids,0) =interpolationIsImplicit(allGrids,allGrids,level);
      list[i].interpolationWidth(three,allGrids,allGrids,0) =interpolationWidth(three,allGrids,allGrids,level);
      list[i].interpolationOverlap(three,allGrids,allGrids,0) =	interpolationOverlap(three,allGrids,allGrids,level);
      list[i].maximumHoleCuttingDistance(nullRange,nullRange,allGrids)= maximumHoleCuttingDistance(nullRange,nullRange,allGrids);
      list[i].interpolationPreference(allGrids,allGrids,0) =interpolationPreference(allGrids,allGrids,level);
      list[i].mayInterpolate(allGrids,allGrids,0) =mayInterpolate(allGrids,allGrids,level);
      list[i].mayCutHoles(allGrids,allGrids) = mayCutHoles(allGrids,allGrids);
      list[i].sharedSidesMayCutHoles(allGrids,allGrids) = sharedSidesMayCutHoles(allGrids,allGrids);
      list[i].multigridCoarseningRatio(three,allGrids,0) = multigridCoarseningRatio(three,allGrids,level);
      list[i].multigridProlongationWidth(three,allGrids,0) =multigridProlongationWidth(three,allGrids,level);
      list[i].multigridRestrictionWidth(three,allGrids,0) = multigridRestrictionWidth(three,allGrids,level);
    } // end for

    if( !(what & THEmultigridLevel) )
    {
      // *wdh* 081024 -- properly assign the composite grid parameters
      //    g1,g2 : grid number in master list
      //    k1,k2 : corresponding numbers in the collection 
      const Range three = 3;
      IntegerArray num1(numberOfCollections), num2(numberOfCollections);
      num1=0;
      for( int g1=0; g1<numberOfGrids; g1++ )
      {
        const int d1 = number(g1);  // grid g1 is in collection d1 
        num1(d1)++;                 // g1 on the master list corresponds to k1=num1(d1)-1 on collection d1
        const int k1=num1(d1)-1;

	const int level = what & THEmultigridLevel ? number(g1) : 0;

        //  maximumHoleCuttingDistance(side,axis,grid)
	list[d1].maximumHoleCuttingDistance(nullRange,nullRange,k1)=maximumHoleCuttingDistance(nullRange,nullRange,g1);
	list[d1].multigridCoarseningRatio(three,k1,0) = multigridCoarseningRatio(three,g1,level);
	list[d1].multigridProlongationWidth(three,k1,0) =multigridProlongationWidth(three,g1,level);
	list[d1].multigridRestrictionWidth(three,k1,0) = multigridRestrictionWidth(three,g1,level);

        num2=0;
	for( int g2=0; g2<numberOfGrids; g2++ )
	{
	  int d2 = number(g2); // grid g2 is in collection d2
	  if( d1==d2 )
	  {
            num2(d2)++;  // g2 on the master list corresponds to k2=num2(d2)-1 on collection 2
	    const int k2=num2(d2)-1;

	    list[d1].interpolationIsImplicit(k1,k2,0) =interpolationIsImplicit(g1,g2,level);
	    list[d1].interpolationWidth(three,k1,k2,0) =interpolationWidth(three,g1,g2,level);
	    list[d1].interpolationOverlap(three,k1,k2,0) =	interpolationOverlap(three,g1,g2,level);
	    list[d1].interpolationPreference(k1,k2,0) =interpolationPreference(g1,g2,level);
	    list[d1].mayInterpolate(k1,k2,0) =mayInterpolate(g1,g2,level);
	    list[d1].mayCutHoles(k1,k2) = mayCutHoles(g1,g2);
	    list[d1].sharedSidesMayCutHoles(k1,k2) = sharedSidesMayCutHoles(g1,g2);
	    
	  }
	}
      }
      // For domains recompute interpolationIsAllExplicit and interpolationIsAllImplicit for each domain
      if( what & THEdomain )
      {
	for( i=0; i<numberOfCollections; i++ ) 
	{
	  Logical & interpolationIsAllExplicit = list[i]->interpolationIsAllExplicit;
	  Logical & interpolationIsAllImplicit = list[i]->interpolationIsAllImplicit;
	  const int numberOfComponentGrids = list[i].numberOfComponentGrids();
	  const IntegerArray & mayInterpolate = list[i].mayInterpolate;
	  const IntegerArray & interpolationIsImplicit = list[i].interpolationIsImplicit;
	  interpolationIsAllExplicit=true;
	  interpolationIsAllImplicit = true;
	  for (int k1=0; k1<numberOfComponentGrids; k1++)
	  {
	    for (int k2=0; k2<numberOfComponentGrids; k2++)
	    {
	      if (k1 != k2) 
	      {
		if( (mayInterpolate(k1,k2,0) && interpolationIsImplicit(k1,k2,0)) 
		    //  || ( mayBackupInterpolate(k1,k2,0) && backupInterpolationIsImplicit(k1,k2,0))
		  )
		  interpolationIsAllExplicit = false;
		if( (mayInterpolate(k1,k2,0) && !interpolationIsImplicit(k1,k2,0)) 
		    //  || ( mayBackupInterpolate(k1,k2,0) && !backupInterpolationIsImplicit(k1,k2,0))
		  )
		  interpolationIsAllImplicit = false;
	      } // end if, end for, end for, end for
	    }
	  }
	}
      }
    }

    for (i=0; i<numberOfCollections; i++) 
    {
      const Integer des = ~(computedGeometry | what) &      (
	THEinterpolationCoordinates | THEinterpoleeGrid     |
	THEinterpoleeLocation       | THEinterpolationPoint |
	THEinverseMap         );
      if (des) list[i].destroy(des);
      const Integer upd = (computedGeometry | what) &       (
	THEinterpolationCoordinates | THEinterpoleeGrid     |
	THEinterpoleeLocation       | THEinterpolationPoint |
	THEinverseMap         );
      if (upd) list[i].update(upd, COMPUTEnothing);
    } // end for


    // newGridNumber : maps a grid number from the master collection to the domain collection
    // newGridNumber[masterGridNumber]=domainGridNumber
    // 
    // master:         0  1  2  3  4  5  6  <- grid numbers
    // domainNumber:   0  0  1  1  0  2  2
    // newGridNumber:  0  1  0  1  2  0  1

    IntegerArray newGridNumber(numberOfGrids);
    nG=0;  // counts number of grids in each collection
    for( int g=0; g<numberOfGrids; g++ )
    {
      int d=number(g);   // this grid belongs to domain d 
      newGridNumber(g)=nG(d);
      nG(d)++;              // increase count of grids in domain d
    }

    // *wdh* 061123
//     printf(" CG:updateCollections: numberOfGrids=%i computedGeometry & THEinterpolationCoordinates=%i\n",
// 	   numberOfGrids,int((computedGeometry & THEinterpolationCoordinates)!=0));

    for( nG=0, k=0; k<numberOfGrids; k++ ) 
    {
      // i = MG-level or domain-number
      // nG(i) = counts the grids for this level or domain
      // j = current grid for this level or domain 

      i = number(k);               // grid k belongs to list i 
      const Integer j = nG(i)++;   // j = current number of grids in list i 
    
      const int ni=numberOfInterpolationPoints(k);

      if ((computedGeometry | what) & THEinterpolationCoordinates)
      {
	// printf("CG:updateCollections: update mg[i=%i].interpolationCoordinates[j=%i]="
	//        "interpolationCoordinates[k=%i], numberOfInterpolationPoints(k)=%i\n", i,j,k,
	//        numberOfInterpolationPoints(k));
	// interpolationCoordinates[k].getPartition().display("interpolationCoordinates[k].getPartition()");
	 
	if( ni>0 ) // this is needed for parallel, otherwise there is an error
	  list[i]->interpolationCoordinates[j].reference(interpolationCoordinates[k]);
	else
	  list[i]->interpolationCoordinates[j].redim(0);
      }
      
      if ((computedGeometry | what) & THEinterpoleeGrid)
      {
        // If we update the domain collection then we need to adjust the interpoleeGrid
        if( what & THEdomain )
	{
	  // we need to adjust the interpoleeGrid numbers to match the new grid
	  intArray & ig =list[i]->interpoleeGrid[j];
 	  ig=interpoleeGrid[k]; // deep copy -- since we need to change these values

          #ifdef USE_PPP
  	   intSerialArray igj; getLocalArrayWithGhostBoundaries(ig,igj);
  	   intSerialArray igk; getLocalArrayWithGhostBoundaries(interpoleeGrid[k],igk);
          #else
           intSerialArray & igj = ig;
	   intSerialArray & igk = interpoleeGrid[k];
	  #endif

	  for( int ii=igj.getBase(0); ii<=igj.getBound(0); ii++ )
	  {
	    // *** here we assume that the sub-collection is a valid independent CompositeGrid ***
	    int gn=newGridNumber(igj(ii));
	    assert( gn>=0 && gn<list[i]->numberOfComponentGrids );
	    igj(ii)=gn;
	  }
    
// 	  for( int ii=0; ii<ni; ii++ )
// 	  {
// 	    // *** here we assume that the sub-collection is a valid independent CompositeGrid ***
// 	    int gn=newGridNumber(ig(ii));
// 	    assert( gn>=0 && gn<list[i]->numberOfComponentGrids );
// 	    ig(ii)=gn;
//	  }

	}
	else
	{
	  if( ni>0 )
  	    list[i]->interpoleeGrid[j].reference(interpoleeGrid[k]);
          else
            list[i]->interpoleeGrid[j].redim(0);
	}

	if( ni>0 )
          list[i]->variableInterpolationWidth[j].reference(variableInterpolationWidth[k]);
        else
          list[i]->variableInterpolationWidth[j].redim(0);
      }
      if ((computedGeometry | what) & THEinterpoleeLocation)
      {
        if( ni>0 )
	  list[i]->interpoleeLocation[j].reference(interpoleeLocation[k]);
	else
	  list[i]->interpoleeLocation[j].redim(0);
      }
      if ((computedGeometry | what) & THEinterpolationPoint)
      {
        if( ni>0 )
	  list[i]->interpolationPoint[j].reference(interpolationPoint[k]);
        else
	  list[i]->interpolationPoint[j].redim(0);
      }
      
      if ((computedGeometry | what) & THEinverseMap) 
      {
	list[i]->inverseCoordinates[j].reference(inverseCoordinates[k]);
	list[i]->inverseGrid[j].reference(inverseGrid[k]);
	list[i]->inverseCoordinates.updateToMatchGrid(*list[i]);
	list[i]->inverseGrid       .updateToMatchGrid(*list[i]);

	for (Integer j2=0; j2<list[i].numberOfBaseGrids(); j2++) 
        {
	  BoundaryAdjustmentArray
	    &bA12j = list[i]->boundaryAdjustment(j,j2),
	    &bA12k = boundaryAdjustment(k,j2);
	  bA12j.redim(bA12k);
	  if (bA12j.getNumberOfElements()) 
          {
	    for (Integer kd=0; kd<numberOfDimensions; kd++)
	      for (Integer ks=0; ks<2; ks++) 
              {
		BoundaryAdjustment &bAj = bA12j(ks,kd), &bAk = bA12k(ks,kd);
		bAj.reference(bAk);
			    
//                             bAj.computedGeometry = bAk.computedGeometry;
//                             bAj.boundaryAdjustment.reference(
//                             bAk.boundaryAdjustment);
//                             bAj.acrossGrid        .reference(
//                             bAk.acrossGrid);
//                             bAj.oppositeBoundary  .reference(
//                             bAk.oppositeBoundary);
	      } // end for, end for
	  } // end if
	} // end for


      } // end if
      list[i]->computedGeometry |= (computedGeometry | what) & (
	THEinterpolationCoordinates | THEinterpoleeGrid     |
	THEinterpoleeLocation       | THEinterpolationPoint |
	THEinverseMap         );
      list[i].updateReferences(computedGeometry | what);
    
    } // end for( nG ..

    if( what & THEdomain )
    {
      // create the interpolationStartEndIndex array of sorted interp points
      // -- this could be done more efficiently since the pts have been sorted for
      //    the master grid -- BUT the grids in the domain may be in a different order --
      #ifdef USE_PPP
      // Parallel: for now we assume the interpolation pts in the domain are sorted.
       
      // fill in the interpolation start end index --- 
      const IntegerArray & ise = interpolationStartEndIndex;
      IntegerArray num1(numberOfDomains), num2(numberOfDomains), numi(numberOfDomains);
      num1=0;
      for( int g1=0; g1<numberOfGrids; g1++ )
      {
        int d1 = domainNumber(g1);
        num1(d1)++;         // g1 on the master list corresponds to k1=num1(d1)-1 on domain 1
        num2=0; numi=0;
	for( int g2=0; g2<numberOfGrids; g2++ )
	{
	  int d2 = domainNumber(g2); 
	  if( d1==d2 )
	  {
            num2(d2)++;  // g2 on the master list corresponds to k2=num2(d2)-1 on domain 2
	    int k1=num1(d1)-1, k2=num2(d2)-1;
	    // interpolationStartEndIndex(0,g1,g2) : start value 
	    // interpolationStartEndIndex(1,g1,g2) : end value 
	    // interpolationStartEndIndex(2,g1,g2) : end value for implicit pts
	    // interpolationStartEndIndex(3,g1,g2) : 
	    if( k1!=k2 && ise(0,g1,g2)>=0 )
	    {
              // printF(" (g1,g2)=(%i,%i) (k1,k2)=(%i,%i) domain=%i\n",g1,g2,k1,k2,d1);
	      
	      list[d1].interpolationStartEndIndex(0,k1,k2)=numi(d1);
	      list[d1].interpolationStartEndIndex(1,k1,k2)=numi(d1)+ise(1,g1,g2)-ise(0,g1,g2);
	      list[d1].interpolationStartEndIndex(2,k1,k2)=numi(d1)+ise(2,g1,g2)-ise(0,g1,g2);
	      numi(d1)+=ise(1,g1,g2)-ise(0,g1,g2)+1;  // *wdh* 070405 add +1 

	    }
	  }
	}
      }
      


      #else
      for (i=0; i<numberOfCollections; i++) 
      {
	list[i].sortInterpolationPoints();
      }
      #endif
    }
    if( what & THEmultigridLevel )
    {
      // fill in the interpolationStartEndIndex arrays in each MG level from the master copy
      for( int l=0; l<numberOfMultigridLevels; l++ )
      {
	for( int grid=0; grid<numberOfComponentGrids; grid++ )
	{
	  for( int grid2=0; grid2<numberOfComponentGrids; grid2++ )
	  {
	    int g=grid+l*numberOfComponentGrids;
	    int g2=grid2+l*numberOfComponentGrids;
	    list[l].interpolationStartEndIndex(Range(0,2),grid,grid2)=interpolationStartEndIndex(Range(0,2),g,g2);
	  }
	}
      
      }

    }
    
  } // end if

  return what & THElists;
}

namespace {
  // helper routing for setHybridConnectivity
  inline void addGhostAndAssignInterp( const bool &is2D, 
				       const int &ugrid, const int &grid, intArray &gid, 
				       realArray &gverts, 
				       const int i, const int j, const int k,
				       UnstructuredMapping &umap, MappedGrid &mg,
				       IntegerArray &numberOfInterpolationPoints,
				       intArray &uig, intArray &uil, intArray &uvw, intArray &uip, 
				       realArray &uic,
				       intArray &umask )
  {
    gid(i,j,k) = is2D ? 
      umap.addVertex( gverts(i,j,k,0),
		      gverts(i,j,k,1) ) :
      umap.addVertex( gverts(i,j,k,0),
		      gverts(i,j,k,1),
		      gverts(i,j,k,2) );
    if ( !umap.isGhost(UnstructuredMapping::Vertex, gid(i,j,k) ) )
      umap.setAsGhost(UnstructuredMapping::Vertex, gid(i,j,k));
    
    int ngid = gid(i,j,k);
    if ( ngid>=umask.getLength(0) )
      {
	int oldMV = umask.getLength(0);
	umask.resize(oldMV+100,1,1);
	Range R(oldMV, umask.getLength(0)-1);
	umask(R,0,0) = MappedGrid::ISdiscretizationPoint;
      }
    
    umask(ngid,0,0) = MappedGrid::ISinterpolationPoint;
    if ( uil.getLength(0)<=numberOfInterpolationPoints(ugrid) )
      {
	int oldLength = uil.getLength(0);
	uil.resize(oldLength+100, uil.getLength(1));
	uig.resize(oldLength+100);
	uvw.resize(oldLength+100, uvw.getLength(1));
	uip.resize(oldLength+100, uip.getLength(1));
	uic.resize(oldLength+100, uip.getLength(1));
      }

    uig(numberOfInterpolationPoints(ugrid)) = grid;
    uil(numberOfInterpolationPoints(ugrid),0) = i;
    uil(numberOfInterpolationPoints(ugrid),1) = j;
    if ( !is2D )
      uil(numberOfInterpolationPoints(ugrid),2) = k;

    Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
    ::getIndex(mg.gridIndexRange(),I1,I2,I3);

    uic(numberOfInterpolationPoints(ugrid),0) = real(i)/real(I1.getBound()-I1.getBase());
    uic(numberOfInterpolationPoints(ugrid),1) = real(j)/real(I2.getBound()-I2.getBase());
    if ( mg.numberOfDimensions()==3 )
      uic(numberOfInterpolationPoints(ugrid),2) = real(k)/real(I3.getBound()-I3.getBase());

    uvw(numberOfInterpolationPoints(ugrid),0) = 1;

    uip(numberOfInterpolationPoints(ugrid),0) = ngid;
    uip(numberOfInterpolationPoints(ugrid),1) = 0;
    if ( !is2D )
      uip(numberOfInterpolationPoints(ugrid),2) = 0;

    numberOfInterpolationPoints(ugrid)++;

  }
  
}


void 
CompositeGrid::
setHybridConnectivity(const int grid_,
		      intArray * gridIndex2UVertex_,
		      intArray & uVertex2GridIndex_,
		      intArray * gridVertex2UVertex_, // could be built from gridIndex2UVertex ?
		      intArray & boundaryFaceMapping_)
// ======================================================================================================
//   /Description:
//     This method sets the connectivity between an unstructured mesh and the structured
//     grids it joins together in a hybrid grid.  Ghost cells are added to the unstructured
//     mesh in sufficient quantity to allow interpolation/data exchange between the grids.
//     These ghost cells mirror the structured grid cells adjacent to the unstructured mesh.
//     The interpolation data arrays {\it interpoleeGrid}, {\it interpoleeLocation}, 
//     {\it interpoleeCoordinates}, and {\it variableInterpolationWidth} are altered to allow
//     the existing interpolation code to communicate hybrid grid data.
// 
//     The UnstructuredMapping is modified to include ghost cells/vertices from the structured grids.
//     Once this adjustment is made the boundaryFaceMapping is no longer valid. 
//
//     This currently (050120) only works for vertex centered grids.
//
//    Who to blame: Kyle Chand. 050118
// ======================================================================================================
{

  rcData->hybridConnectivity.setCompositeGridHybridConnectivity(grid_,
								gridIndex2UVertex_, uVertex2GridIndex_,
								gridVertex2UVertex_, boundaryFaceMapping_);

  assert( (*this)[grid_].mapping().getClassName()=="UnstructuredMapping" );

  this->update(THEinterpolationPoint | THEinterpoleeGrid | THEinterpoleeLocation 
	       | THEinterpolationCoordinates /*| THEvariableInterpolationWidth done coords update*/);

  UnstructuredMapping &umap = (UnstructuredMapping &)(*this)[grid_].mapping().getMapping();

  if ( umap.getDomainDimension()!=umap.getRangeDimension() )
    return; // surface grid

  UnstructuredMapping::EntityTypeEnum ztype = UnstructuredMapping::EntityTypeEnum(numberOfDimensions());

  (*this)[grid_].update(THEmask);
  intArray umask; umask.reference((*this)[grid_].mask());

  const CompositeGridHybridConnectivity & cghc = getHybridConnectivity();
  const intArray &bfm = cghc.getBoundaryFaceMapping();
  const intArray &ugi = cghc.getUVertex2GridIndex();

  int nGhostZoneGuess = bfm.getLength(0);
  int nGhostVertGuess = nGhostZoneGuess*int(rounder(pow(2,numberOfDimensions())));

  umap.reserve(UnstructuredMapping::Vertex, nGhostVertGuess);
  umap.reserve(ztype, nGhostZoneGuess);

  int oldMV = umask.getLength(0);
  umask.resize(nGhostVertGuess,1,1);
  Range R(oldMV, nGhostVertGuess-1);
  umask(R,0,0) = MappedGrid::ISdiscretizationPoint;

  // int ngv=0;
  // int ngz=0;

  int kp = numberOfDimensions()==2 ? 0 : 1;

  ArraySimple<int> newZone( umap.maxVerticesInEntity(ztype) );

  const bool is2D = numberOfDimensions()==2;

  ArraySimple<bool> *zmasks = new ArraySimple<bool>[ numberOfComponentGrids() ];

  for ( int i=int(ztype)-1; i>=int(UnstructuredMapping::Vertex); i-- )
    {
      umap.deleteConnectivity(ztype,UnstructuredMapping::EntityTypeEnum(i));
      if ( i!=int(UnstructuredMapping::Vertex) ) 
	{
	  umap.deleteConnectivity( UnstructuredMapping::EntityTypeEnum(i) );
	}
    }

  const IntegerArray & iw0 = interpolationWidth();
  IntegerArray & iw = (IntegerArray &) iw0;
  iw = 1;

  IntegerArray nper(numberOfComponentGrids());
  nper=0;
  for ( int v=0; v<ugi.getLength(0); v++ )
    {
      // determine how many uns vertices are on structured grid branch cuts
      //   this will be added as interpolation points on the structured grids

      int grid = ugi(v,0);
      int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
      int iip[3];
      iip[2] = i3=0;
      for ( int a=0; a<numberOfDimensions(); a++ )
	iip[a] = ii[a] = ugi(v,a+1);

      MappedGrid &mg = (MappedGrid &)(*this)[grid];
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      ::getIndex(mg.gridIndexRange(),I1,I2,I3);
      intArray &gid = (intArray &)cghc.getGridIndex2UVertex(grid);

      bool isp=false;
      for ( int a=0; a<mg.numberOfDimensions(); a++ )
	{
	  if ( mg.isPeriodic(a) && (ii[a]==Iv[a].getBase()) )
	    {
	      nper(grid)++;
	      iip[a] = Iv[a].getBound();
	      isp=true;
	      break;
	    }
	}
      if ( isp ) gid(iip[0],iip[1],iip[2]) = gid(i1,i2,i3);
      
    }

  for ( int g=0; g<numberOfComponentGrids(); g++ )
    {
      if ( g!= cghc.getUnstructuredGridIndex() )
	numberOfInterpolationPoints(g) = cghc.getNumberOfInterfaceVertices(g) + nper(g);
      else
	numberOfInterpolationPoints(g) = nGhostVertGuess; // this may need to be resized if we guess wrong
      //      cout<<"NINTERP GUESS IS "<<numberOfInterpolationPoints(g)<<endl;

      interpoleeGrid[g].resize(numberOfInterpolationPoints(g));
      interpoleeLocation[g].resize(numberOfInterpolationPoints(g),numberOfDimensions());

      interpolationCoordinates[g].resize(numberOfInterpolationPoints(g),numberOfDimensions());
      interpolationCoordinates[g] = 0.;
      variableInterpolationWidth[g].resize(numberOfInterpolationPoints(g));
      variableInterpolationWidth[g] = 1;
      interpolationPoint[g].resize(numberOfInterpolationPoints(g),numberOfDimensions());

      numberOfInterpolationPoints(g) = 0; // reset so we can use it as a counter
      
    }

  int ugrid = cghc.getUnstructuredGridIndex();
  
  intArray & uil = interpoleeLocation[ugrid];
  intArray & uig = interpoleeGrid[ugrid];
  intArray & uvw = variableInterpolationWidth[ugrid];
  intArray & uip = interpolationPoint[ugrid];
  realArray & uic = interpolationCoordinates[ugrid];

  for ( int v=0; v<ugi.getLength(0); v++ )
    {
      newZone = -1;

      int grid = ugi(v,0);

      intArray & il = interpoleeLocation[grid];
      intArray & ig = interpoleeGrid[grid];
      intArray & ip = interpolationPoint[grid];
      realArray &ic = interpolationCoordinates[grid];

      ig(numberOfInterpolationPoints(grid)) = ugrid;
      il(numberOfInterpolationPoints(grid),0) = v;

      for ( int i=1; i<numberOfDimensions(); i++ )
	il(numberOfInterpolationPoints(grid),i) = 0;
      
      int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
      i3=0;
      for ( int a=0; a<numberOfDimensions(); a++ )
	{
	  ii[a] = ugi(v,a+1);
	  ip(numberOfInterpolationPoints(grid),a) = ii[a];
	} 

      ic(numberOfInterpolationPoints(grid,0)) = real(v)/real(umap.size(UnstructuredMapping::Vertex)-1);

      numberOfInterpolationPoints(grid)++;

      MappedGrid &mg = (MappedGrid &)(*this)[grid];
      //      mg.update(MappedGrid::THEmask | MappedGrid::THEvertex);
      intArray & mask = mg.mask();
      realArray &gverts = mg.vertex();

      mask(i1,i2,i3) = MappedGrid::ISinterpolationPoint;

      ArraySimple<bool> &zmask = zmasks[grid];
      Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
      ::getIndex(mg.gridIndexRange(),I1,I2,I3);
      if ( !zmask.size() )
	{
	  zmask.resize(I1.getLength(), I2.getLength(), I3.getLength());
	  zmask = false;
	}

      intArray &gid = (intArray &)cghc.getGridIndex2UVertex(grid);

      // locate adjacent unmasked zones in the structured grid
      //   these will be added as ghosts to the unstructured mesh
      //
      //   note that ghost cells/vertices in the structured grids are NOT added to the 
      //       unstructured mesh
      for ( int ia=0; ia<2; ia++ )
	{
	  int iz = i1 + /*2*ia*/ ia - 1;
	  for ( int ja=0; ja<2 && ( iz>=I1.getBase() && iz<I1.getBound()) ; ja++ )
	    {
	      int jz = i2 + /*2*ja*/ ja - 1;
	      for ( int ka=0; ka<(kp+1) && ( jz>=I2.getBase() && jz<I2.getBound()); ka++ )
		{
		  int kz = kp ? ( i3 + /*2*ka*/ ka - 1 ) : 0;
		  if ( (kz>=I3.getBase() && kz<=(I3.getBound()-kp)) && 
		      ( mask(iz,jz,kz)        && mask(iz+1,jz,kz) && 
			mask(iz+1,jz+1,kz)    && mask(iz,jz+1,kz) &&
			mask(iz,jz,kz+kp)     && mask(iz+1,jz,kz+kp) && 
			mask(iz+1,jz+1,kz+kp) && mask(iz,jz+1,kz+kp) ) )
		  {
		      // then we need to add this zone to the umap

		    if ( !zmask(iz,jz,kz) )
		      {
			for ( int kk=0; kk<(kp+1); kk++ )
			  {
			    if ( gid(iz,jz,kz+kk)==-1 )
			      {
				addGhostAndAssignInterp( is2D, ugrid, grid, gid, gverts, 
							 iz,jz,kz+kk,
							 umap, mg,
							 numberOfInterpolationPoints,
							 uig,uil,uvw,uip,uic,umask );

			      }
			    
			    
			    if ( gid(iz+1,jz,kz+kk)==-1 )
			      {
				addGhostAndAssignInterp( is2D, ugrid, grid, gid, gverts, 
							 iz+1,jz,kz+kk,
							 umap, mg,
							 numberOfInterpolationPoints,
							 uig,uil,uvw,uip,uic,umask );

			      }
			    
			    
			    if ( gid(iz+1,jz+1,kz+kk)==-1 )
			      {
				addGhostAndAssignInterp( is2D, ugrid, grid, gid, gverts, 
							 iz+1,jz+1,kz+kk,
							 umap, mg,
							 numberOfInterpolationPoints,
							 uig,uil,uvw,uip,uic,umask );

			      }
			    
			    if ( gid(iz,jz+1,kz+kk)==-1 )
			      {
				addGhostAndAssignInterp( is2D, ugrid, grid, gid, gverts, 
							 iz,jz+1,kz+kk,
							 umap, mg,
							 numberOfInterpolationPoints,
							 uig,uil,uvw,uip,uic,umask );
			      }
			    
			    
			    assert(gid(iz,jz,kz+kk)<=umap.size(UnstructuredMapping::Vertex));
			    assert(gid(iz+1,jz,kz+kk)<=umap.size(UnstructuredMapping::Vertex));
			    assert(gid(iz+1,jz+1,kz+kk)<=umap.size(UnstructuredMapping::Vertex));
			    assert(gid(iz,jz+1,kz+kk)<=umap.size(UnstructuredMapping::Vertex));
			    
			  } // end kk
			
			if ( mg.mapping().getMapping().getSignForJacobian()>0 )
			  {
			    for ( int kk=0; kk<(kp+1); kk++ )
			      {
				newZone(0+4*kk) = gid(iz,jz,kz+kk);
				newZone(1+4*kk) = gid(iz+1,jz,kz+kk);
				newZone(2+4*kk) = gid(iz+1,jz+1,kz+kk);
				newZone(3+4*kk) = gid(iz,jz+1,kz+kk);
			      }				  
			  }
			else
			  {
			    for ( int kk=0; kk<(kp+1); kk++ )
			      {
				newZone(3+4*kk) = gid(iz,jz,kz+kk);
				newZone(2+4*kk) = gid(iz+1,jz,kz+kk);
				newZone(1+4*kk) = gid(iz+1,jz+1,kz+kk);
				newZone(0+4*kk) = gid(iz,jz+1,kz+kk);
			      }
			  }
			
			int zid = umap.addEntity( ztype, newZone );
			
			umap.setAsGhost( ztype, zid );
			
			zmask(iz,jz,kz) = true;
			
		      } //end if !zmask
		    
		  } // end if !mask
		  
		}// end ka
	    } // end ja
	}// end ia

      // add periodic interpolation points on the structured grid
      bool isper=false;
      for ( int a=0; a<mg.numberOfDimensions() && !isper; a++ )
	isper = mg.isPeriodic(a) && (ii[a]==Iv[a].getBase());
      
      if ( isper )
	{
	  ig(numberOfInterpolationPoints(grid)) = ugrid;
	  il(numberOfInterpolationPoints(grid),0) = v;
	  
	  for ( int i=1; i<numberOfDimensions(); i++ )
	    il(numberOfInterpolationPoints(grid),i) = 0;
	  
	  //	  int ii[3], &i1=ii[0], &i2=ii[1], &i3=ii[2];
	  //	  i3=0;
	  for ( int a=0; a<numberOfDimensions(); a++ )
	    {
	      if ( mg.isPeriodic(a) && ii[a]==Iv[a].getBase() )
		ii[a] = Iv[a].getBound();
	      //	      else
	      //		ii[a] = ugi(v,a+1);

	      ip(numberOfInterpolationPoints(grid),a) = ii[a];
	    } 
	  mask(ii[0],ii[1],ii[2]) = MappedGrid::ISinterpolationPoint;
	  //	  cout<<"PER POINT "<<ii[0]<<"  "<<ii[1]<<"  "<<ii[2]<<endl;
	  ic(numberOfInterpolationPoints(grid,0)) = real(v)/real(umap.size(UnstructuredMapping::Vertex)-1);
	  
	  numberOfInterpolationPoints(grid)++;
	}

    }// end v

  umask.resize(umap.size(UnstructuredMapping::Vertex),1,1);
  uil.resize(numberOfInterpolationPoints(ugrid),uil.getLength(1));
  uig.resize(numberOfInterpolationPoints(ugrid));
  uvw.resize(numberOfInterpolationPoints(ugrid));
  uip.resize(numberOfInterpolationPoints(ugrid),uip.getLength(1));

  //  numberOfInterpolationPoints.display("NINTERP IS ");
  //  cout<<"NEW UMAP SIZE W/GHOST IS "<<umap.size(ztype)<<"  "<<umap.size(UnstructuredMapping::Edge)<<endl;

  // now scan through the boundary bounding edges and tag any vertices there with bc=1
#if 1
  umap.buildEntity(UnstructuredMapping::EntityTypeEnum(numberOfDimensions()-1), true);
  string bdyEntTag = string("boundary ")+UnstructuredMapping::EntityTypeStrings[numberOfDimensions()-1];
  string bdyVertTag =string("boundary ")+UnstructuredMapping::EntityTypeStrings[UnstructuredMapping::Vertex];
  UnstructuredMapping::tag_entity_iterator bdy, bdy_end;
  UnstructuredMappingAdjacencyIterator vert, vert_end;

  bdy_end = umap.tag_entity_end( bdyEntTag );
  int nbv=0;
  for ( bdy=umap.tag_entity_begin(bdyEntTag); bdy!=bdy_end; bdy++ )
    {
      if ( umap.adjacency_begin(*bdy, UnstructuredMapping::EntityTypeEnum(numberOfDimensions())).nAdjacent()==1 )
	{
	  // {
// 	    vert_end = umap.adjacency_end(*bdy, UnstructuredMapping::EntityTypeEnum(numberOfDimensions()));
// 	    for (vert=umap.adjacency_begin(*bdy, UnstructuredMapping::EntityTypeEnum(numberOfDimensions()));
// 		 vert!=vert_end;
// 		 vert++ )
// 	      cout<<"ADJ Face "<<*vert<<endl;
// 	  }
//	  cout<<"Edge "<<bdy->e<<" is apparently on a boundary ?"<<endl;
	  vert_end = umap.adjacency_end(*bdy, UnstructuredMapping::Vertex);
	  for ( vert=umap.adjacency_begin(*bdy, UnstructuredMapping::Vertex);
		vert!=vert_end;
		vert++ )
	    {
	      if (!umap.hasTag(UnstructuredMapping::Vertex,*vert, bdyVertTag))
		cout<<"WARNING : Vertex "<<*vert<<" Is apparently NOT a boundary vertex"<<endl;;

		     //	      umap.addTag(UnstructuredMapping::Vertex, umap.tagPrefix(UnstructuredMapping::EntityTypeEnum(numberOfDimensions()-1),UnstructuredMapping::GhostEntity),(void *)0);
	      umap.setBC(UnstructuredMapping::Vertex, *vert, 1);
	      nbv++;
	    }
	}
    }
  
  //cout<<"NBV = "<<nbv<<endl;
#endif

  //  interpolationPoint[0].display("IP0");

  (*this)[ugrid].reference(umap,true);

  (*this)[ugrid].destroy(THEmask);
  (*this)[ugrid].update(THEmask);
  (*this)[ugrid].mask() = umask;

  sortInterpolationPoints();

  rcData->computedGeometry = 
    THEmask |
    THEinterpolationPoint | 
    THEinterpoleeGrid | 
    THEinterpoleeLocation | 
    THEinterpolationCoordinates;

  for ( int g=0; g<numberOfComponentGrids(); g++ )
    (*this)[g]->computedGeometry = THEmask;

  //  umap.getEntities(UnstructuredMapping::Face).display("NEW FACES");

  //    umap.getNodes().display("NODES ARE");
  // XXX this is a hack until we can sync the old and new connectivities (or get rid of the old in plotUns)
  //  intArray &elems = (intArray &)umap.getElements();
  //  elems.redim(0);
  //  elems = umap.getEntities(ztype);

  //umap.incrementReferenceCount(); // XXX is this needed ?
  // XXX this is a hack until we can tell a mapped grid that the mapping has changed
  //  deleteGrid(grid_);
  //  add(umap);

  // XXX I guess this does the same thing without destroying the interpolation arrays
    //  MappedGrid mgtmp(umap);
    //  (*this)[grid_] = mgtmp;
  
  //  umap.getNodes().display("AFTER ASSIGEN NODES ARE");

  //  (*this)[numberOfComponentGrids()-1].mask().reference(umask);

  //  if ( umap.decrementReferenceCount()==0 ) delete umap;

}

void CompositeGrid::
setSurfaceStitching( UnstructuredMapping *stitching )
{ // XXX kkc 050118 what if a stitching already exists? decrement ref count and delete???

  rcData->surfaceStitching = stitching;
  if ( stitching )
    stitching->incrementReferenceCount();
}

const CompositeGridHybridConnectivity & 
CompositeGrid::
getHybridConnectivity() const
{
  return rcData->hybridConnectivity;
}

void
CompositeGrid::
sortInterpolationPoints() 
// ============================================================================================
//  /Description:
//     Sort the interpolation points by donor grid and build the interpolationStartEndIndex
//
// ============================================================================================
{
  IntegerArray gridStart(numberOfComponentGrids()), ng(numberOfComponentGrids());
  interpolationStartEndIndex = -1;
  for( int grid=0; grid<numberOfComponentGrids(); grid++ )
  {
      
    if( numberOfInterpolationPoints(grid) > 0 )
    {
      intArray & interpoleeGrid1            = interpoleeGrid[grid];
      intArray & interpolationPoint1        = interpolationPoint[grid];
      intArray & interpoleeLocation1        = interpoleeLocation[grid];
      intArray & variableInterpolationWidth1= variableInterpolationWidth[grid];
      realArray    & interpolationCoordinates1  = interpolationCoordinates[grid];
	  
      intArray interpoleeGrid             = this->interpoleeGrid[grid];
      intArray interpolationPoint         = this->interpolationPoint[grid];
      intArray interpoleeLocation         = this->interpoleeLocation[grid];
      intArray variableInterpolationWidth = this->variableInterpolationWidth[grid];
      realArray interpolationCoordinates  = this->interpolationCoordinates[grid]; 

      const int *interpoleeLocationp = interpoleeLocation.Array_Descriptor.Array_View_Pointer1;
      const int interpoleeLocationDim0=interpoleeLocation.getRawDataSize(0);
#define INTERPOLEELOCATION(i0,i1) interpoleeLocationp[i0+interpoleeLocationDim0*(i1)]
      int *interpoleeLocation1p = interpoleeLocation1.Array_Descriptor.Array_View_Pointer1;
      const int interpoleeLocation1Dim0=interpoleeLocation1.getRawDataSize(0);
#define INTERPOLEELOCATION1(i0,i1) interpoleeLocation1p[i0+interpoleeLocation1Dim0*(i1)]
	  
      const int *interpolationPointp = interpolationPoint.Array_Descriptor.Array_View_Pointer1;
      const int interpolationPointDim0=interpolationPoint.getRawDataSize(0);
#define INTERPOLATIONPOINT(i0,i1) interpolationPointp[i0+interpolationPointDim0*(i1)]
      int *interpolationPoint1p = interpolationPoint1.Array_Descriptor.Array_View_Pointer1;
      const int interpolationPoint1Dim0=interpolationPoint1.getRawDataSize(0);
#define INTERPOLATIONPOINT1(i0,i1) interpolationPoint1p[i0+interpolationPoint1Dim0*(i1)]
	  
      const real *interpolationCoordinatesp = interpolationCoordinates.Array_Descriptor.Array_View_Pointer1;
      const int interpolationCoordinatesDim0=interpolationCoordinates.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES(i0,i1) interpolationCoordinatesp[i0+interpolationCoordinatesDim0*(i1)]
      real *interpolationCoordinates1p = interpolationCoordinates1.Array_Descriptor.Array_View_Pointer1;
      const int interpolationCoordinates1Dim0=interpolationCoordinates1.getRawDataSize(0);
#define INTERPOLATIONCOORDINATES1(i0,i1) interpolationCoordinates1p[i0+interpolationCoordinates1Dim0*(i1)]
	  
	  
      int * interpoleeGridp = interpoleeGrid.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID(i0) interpoleeGridp[i0]
      int * interpoleeGrid1p = interpoleeGrid1.Array_Descriptor.Array_View_Pointer0;
#define INTERPOLEEGRID1(i0) interpoleeGrid1p[i0]
      int * ngp = ng.Array_Descriptor.Array_View_Pointer0;
#define NG(i0) ngp[i0]
	  
      int * gridStartp = gridStart.Array_Descriptor.Array_View_Pointer0;
#define GRIDSTART(i0) gridStartp[i0]
      const int * variableInterpolationWidthp = variableInterpolationWidth.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH(i0) variableInterpolationWidthp[i0]
      int * variableInterpolationWidth1p = variableInterpolationWidth1.Array_Descriptor.Array_View_Pointer0;
#define VARIABLEINTERPOLATIONWIDTH1(i0) variableInterpolationWidth1p[i0]
	
      //	  interpoleeLocation1.display("IL before");
  
      // order the interpolation points by interpolee grid.
      ng=0;
      int i;
      const int nig=numberOfInterpolationPoints(grid);
      for( i=0; i<nig; i++ )
	NG(INTERPOLEEGRID(i))++;

      //	  ng.display("NG");

      GRIDSTART(0)=0;
      int grid2;
      for( grid2=1; grid2<numberOfComponentGrids(); grid2++ )
	GRIDSTART(grid2)=GRIDSTART(grid2-1)+NG(grid2-1);
	  
      //	  gridStart.display("GRIDSTART");
      // ***** we need to assign the interpolationStartEndIndex 
      // **** this needs to be set on multigridLevel[0] too ********
	  
      // for now we assume that the interpolation is implicit on coarser levels *** fix this ***
	  
      //kkc left over from ogmg	  cg1.interpolationIsAllExplicit()=false;
      //kkc left over from ogmg	  cg1.interpolationIsAllImplicit()=true;

      //kkc left over from ogmg :
      for( grid2=0; grid2<numberOfComponentGrids(); grid2++ )
      {
	      
	if( NG(grid2)>0 )
	{
	  interpolationStartEndIndex(0,grid,grid2)=GRIDSTART(grid2);              // start value
	  interpolationStartEndIndex(1,grid,grid2)=GRIDSTART(grid2)+NG(grid2)-1;  // end value
	  if( true || interpolationIsImplicit(grid,grid2,0) )
	    interpolationStartEndIndex(2,grid,grid2)= interpolationStartEndIndex(1,grid,grid2);
	  // fix this: put any implicit points first
	  // 	   else if( ngi(grid2)>0 )
	  // 	     interpolationStartEndIndex(2,grid,grid2)=GRIDSTART(grid2)+ngi(grid2)-1; // end value for implicit pts.
	}
      }
	  
      if( numberOfDimensions()==2 )
      {
	for( i=0; i<nig; i++ )
	{
	  grid2=INTERPOLEEGRID(i);
	  int j=GRIDSTART(grid2);
	  INTERPOLEEGRID1(j)=grid2;
	  INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
	  INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
	  INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
	  INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
	  INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
	  INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
	  variableInterpolationWidth1(j)=variableInterpolationWidth(i);
		  
	  GRIDSTART(grid2)++;
	}
      }
      else
      {
	for( i=0; i<nig; i++ )
	{
	  grid2=INTERPOLEEGRID(i);
	  int j=GRIDSTART(grid2);
	  interpoleeGrid1(j)=grid2;
	  INTERPOLATIONPOINT1(j,0)=INTERPOLATIONPOINT(i,0);
	  INTERPOLATIONPOINT1(j,1)=INTERPOLATIONPOINT(i,1);
	  INTERPOLATIONPOINT1(j,2)=INTERPOLATIONPOINT(i,2);
	  INTERPOLEELOCATION1(j,0)=INTERPOLEELOCATION(i,0);
	  INTERPOLEELOCATION1(j,1)=INTERPOLEELOCATION(i,1);
	  INTERPOLEELOCATION1(j,2)=INTERPOLEELOCATION(i,2);
	  INTERPOLATIONCOORDINATES1(j,0)=INTERPOLATIONCOORDINATES(i,0);
	  INTERPOLATIONCOORDINATES1(j,1)=INTERPOLATIONCOORDINATES(i,1);
	  INTERPOLATIONCOORDINATES1(j,2)=INTERPOLATIONCOORDINATES(i,2);
	  VARIABLEINTERPOLATIONWIDTH1(j)=VARIABLEINTERPOLATIONWIDTH(i);
		  
	  GRIDSTART(grid2)++;
	}
      }
	  
      //      interpoleeLocation1.display("IL after");


    }
    // cg.numberOfInterpolationPoints(grid)=numberOfInterpolationPoints(grid);
      
  }
  //  interpolationStartEndIndex.display("ISTARTEND");
	  
}

#undef COMPOSITE_GRID_FOR_3
