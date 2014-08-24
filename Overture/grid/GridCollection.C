//
// Who to blame:  Geoff Chesshire, Bill Henshaw.
//

#include "GridCollection.h"
#include "display.h"
#include "ParallelUtility.h"
#include "ParentChildSiblingInfo.h"
#include "LoadBalancer.h"

#ifdef USE_STL
RCVector_STATIC_MEMBER_DATA(GridCollection)
#endif // USE_STL

//
// class GridCollection:
//
// Public member functions:
//
// Default constructor.
//
// If numberOfDimensions_==0 (e.g., by default) then create a null
// GridCollection.  Otherwise create a GridCollection with the given
// number of dimensions and number of component grids.
//
GridCollection::GridCollection(
  const Integer numberOfDimensions_,
  const Integer numberOfGrids_):
  GenericGridCollection() {
    className = "GridCollection";
    master=this;
    rcData = new
      GridCollectionData(numberOfDimensions_, numberOfGrids_);
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    updateReferences();
    refinementLevelInfo = NULL;
}
//
// Copy constructor.  (Does a deep copy by default.)
//
GridCollection::GridCollection(
  const GridCollection& x,
  const CopyType        ct):
  GenericGridCollection() {
    className = "GridCollection";
    master=this;
    switch (ct) {
      case DEEP:
      case NOCOPY:
        rcData = (GridCollectionData*)
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
    refinementLevelInfo = x.refinementLevelInfo;
}
//
// Destructor.
//
GridCollection::~GridCollection()
  { if (isCounted && rcData->decrementReferenceCount() == 0) delete rcData; }

void GridCollection::
specifyProcesses(const Range& range)
// =============================================================================
// /Description:
//    Specify the range of processors to use for the partioning of arrays
//    This will update the GridDistributionList and 
//    will change the range of processors for all MappedGrid's too.
// =============================================================================
{ 
  assert( rcData!=NULL );
  GridDistributionList & gridDistributionList = rcData->gridDistributionList;
  gridDistributionList.resize(numberOfGrids(),GridDistribution());
  for( int g=0; g<numberOfGrids(); g++ )
  {
    gridDistributionList[g].setProcessors(range.getBase(),range.getBound());
    (*this)[g].specifyProcesses(range);
  }
}

GridCollection& GridCollection::
operator=(const GridCollection& x) 
// =================================================================================================
// /Description:
//    Assignment operator.  (Does a deep copy.)
//
// /Note: To copy a GridCollection to a different set of processors, build
//        a gridDistributionList, set keepGridDistributionOnCopy(true), and then perform the copy.
// =================================================================================================
{
//  GenericGridCollection::operator=(x);
  if (rcData != x.rcData) 
  {
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
      GridCollection& y =
	*(GridCollection*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
    master=x.master;
  } // end if
  refinementLevelInfo = x.refinementLevelInfo;

  // *wdh* 020413: Do this because not all of  ParentChildSiblingInfo is ref counted?
  updateParentChildSiblingInfo();

  return *this;
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{reference(GridCollection)}} 
void GridCollection::
reference(const GridCollection& x)
// ===========================================================
// /Description:
//    Make a reference.  (Does a shallow copy.)
//\end{GridCollectionInclude.tex}
// ===========================================================
{
  GenericGridCollection::reference(x);
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
  refinementLevelInfo = x.refinementLevelInfo;
}


void GridCollection::
reference(GridCollectionData& x) 
{
  GenericGridCollection::reference(x);
  if (rcData != &x) 
  {
    if (rcData->decrementReferenceCount() == 0) delete rcData;
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
void GridCollection::breakReference() {
//  GenericGridCollection::breakReference();
    if (!isCounted || rcData->decrementReferenceCount() != 1) {
        GridCollection x = *this; // Uses the (deep) copy constructor.
        reference(x);
    } // end if
}
//
// Change the grid to be all vertex-centered.
//
void GridCollection::changeToAllVertexCentered() {
    for (Integer i=0; i<numberOfGrids(); i++)
      grid[i].changeToAllVertexCentered();
}
//
// Change the grid to be all cell-centered.
//
void GridCollection::changeToAllCellCentered() {
    for (Integer i=0; i<numberOfGrids(); i++)
      grid[i].changeToAllCellCentered();
}
//
// Check that the data structure is self-consistent.
//
void GridCollection::consistencyCheck() const {
    GenericGridCollection::consistencyCheck();
    if (rcData != GenericGridCollection::rcData) {
        cerr << className << "::consistencyCheck():  "
             << "rcData != GenericGridCollection::rcData for "
             << getClassName() << " " << getGlobalID() << "." << endl;
        assert(rcData == GenericGridCollection::rcData);
    } // end if
    refinementFactor         .Test_Consistency();
    multigridCoarseningFactor.Test_Consistency();
    grid                     .consistencyCheck();
    baseGrid                 .consistencyCheck();
    refinementLevel          .consistencyCheck();
    componentGrid            .consistencyCheck();
    multigridLevel           .consistencyCheck();
    domain                   .consistencyCheck();
}


//\begin{>>GridCollectionInclude.tex}{\subsubsection{reference(numberOfGridPoints)}} 
int GridCollection::
numberOfGridPoints() const
// =============================================================================
// /Description:
//    Return the number of grid points (based on numberOfComponentGrids)
//\end{GridCollectionInclude.tex}
// =============================================================================
{
  int num=0;
  for( int grid=0; grid<numberOfComponentGrids(); grid++ )
  {
    const MappedGrid & mg = (*this)[grid];
    const IntegerArray & d = mg.dimension();

    num+=(d(1,0)-d(0,0)+1)*(d(1,1)-d(0,1)+1)*(d(1,2)-d(0,2)+1);
  }
  return num;
}


// Here is the master grid.
GridCollection & GridCollection::
masterGridCollection()
{
  assert( master!=0 );
  return *master;
}

//
// "Get" and "put" database operations.
//
Integer GridCollection::
get( const GenericDataBase& db,
     const aString&         name) 
{
  Integer returnValue = rcData->get(db, name);
  updateReferences();

  return returnValue;
}

Integer GridCollection::
put( GenericDataBase& db,
     const aString&   name,
     int geometryToPut /* = -1  */
  ) const
// geometryToPut : specify which geometry to put, by default put computedGeometry
{ 
  return rcData->put(db, name, geometryToPut); 
}
//
// Set references to reference-counted data.
//
void GridCollection::
updateReferences(const Integer what)
{
  GenericGridCollection::reference(*rcData);
#define REFERENCE(Type, x) ((Type&)x).reference(rcData->x)
#define REF_ARRAY(Type, x) \
  if (x.getDataPointer() != rcData->x.getDataPointer()) REFERENCE(Type, x)
							  REF_ARRAY(RealArray,                boundingBox);
  REF_ARRAY(IntegerArray,             refinementFactor);
  REF_ARRAY(IntegerArray,             multigridCoarseningFactor);
#ifdef USE_STL
  REFERENCE(RCVector<MappedGrid>,     grid);
  REFERENCE(RCVector<GridCollection>, baseGrid);
  REFERENCE(RCVector<GridCollection>, refinementLevel);
  REFERENCE(RCVector<GridCollection>, componentGrid);
  REFERENCE(RCVector<GridCollection>, multigridLevel);
  REFERENCE(RCVector<GridCollection>, domain);
#else
  REFERENCE(ListOfMappedGrid,         grid);
  REFERENCE(ListOfGridCollection,     baseGrid);
  REFERENCE(ListOfGridCollection,     refinementLevel);
  REFERENCE(ListOfGridCollection,     componentGrid);
  REFERENCE(ListOfGridCollection,     multigridLevel);
  REFERENCE(ListOfGridCollection,     domain);
#endif // USE_STL
#undef REFERENCE
#undef REF_ARRAY

  int i;
  for (i=0; i<numberOfGrids(); i++) {
    grid[i].updateReferences(what);
    GenericGridCollection::grid[i].reference(grid[i]);
  }
  GenericGridCollection::updateReferences(what);

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
  FOR_COLLECTION(baseGrid);
  FOR_COLLECTION(refinementLevel);
  FOR_COLLECTION(componentGrid);
  FOR_COLLECTION(multigridLevel);
  FOR_COLLECTION(domain);
#undef FOR_COLLECTION
}
//
// Update the grid, sharing the data of another grid.
//
Integer GridCollection::update(
  GenericGridCollection& x,
  const Integer          what,
  const Integer          how) 
{
  Integer upd = rcData->update(*((GridCollection&)x).rcData, what, how);
  updateReferences(what);
  // We need to assign the mappedGrid in each geometry array *wdh*
  GridCollection & cg = (GridCollection&)x;
  for( int k=0; k<cg.numberOfGrids(); k++ )
    cg[k].updateMappedGridPointers(what);
  return upd;
}

//! return the ListOfParentChildSiblingInfo.
ListOfParentChildSiblingInfo* GridCollection:: 
getParentChildSiblingInfo() const
{
  return rcData->parentChildSiblingInfoList;
}


//! Update the ListOfParentChildSiblingInfo.
void GridCollection::
updateParentChildSiblingInfo()
{
  rcData->updateParentChildSiblingInfo(*this);
  
}

//! Call this function when the GridCollection has changed and the parent child sibling info
//! needs to be updated.
void GridCollection::
parentChildSiblingInfoNeedsUpdate()
{
  rcData->parentChildSiblingInfoNeedsUpdate=true;
}


//! Update the parent child sibling info
void GridCollectionData::
updateParentChildSiblingInfo(GridCollection & gc)
{
  if( parentChildSiblingInfoNeedsUpdate && numberOfRefinementLevels>1 )
  {
    if( parentChildSiblingInfoList==NULL )
      parentChildSiblingInfoList = new ListOfParentChildSiblingInfo;
    else
    {
      parentChildSiblingInfoList->destroy(); // *wdh* 020412
    }
  
    ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( gc, *parentChildSiblingInfoList );
    parentChildSiblingInfoNeedsUpdate=false;
  }
}


//
// Destroy optional grid data.
//
void GridCollection::destroy(const Integer what) {
    rcData->destroy(what);
    updateReferences();
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{add(MappedGrid)}}
int GridCollection::
add(MappedGrid & g)
// ==========================================================================
// /Description: 
//    Add a new grid. The grid collection will keep a reference to g.
//\end{GridCollectionInclude.tex}
//==========================================================================
{
// ------------------

  assert( rcData!=NULL );
  
  const Integer n = rcData->numberOfGrids; 
  rcData->numberOfComponentGrids=n+1; // this order is important
  if( n>0)
    rcData->setNumberOfGrids(n+1);
  else
    rcData->setNumberOfDimensionsAndGrids(g.numberOfDimensions(),n+1);  // *wdh* 010530
  // if there are refinement levels then we will need to update the
  // various lists.

//     numberOfRefinementLevels = max0(numberOfRefinementLevels, level+1);
  rcData->baseGridNumber(n)        = n;
  rcData->refinementLevelNumber(n) = 0;
  rcData->componentGridNumber(n)   = n;
  rcData->multigridLevelNumber(n)  = 0;
  rcData->domainNumber(n)          = 0;
//     computedGeometry &= ~(GenericGrid::EVERYTHING | THElists);
//     return n;

  rcData->numberOfBaseGrids        = max(rcData->baseGridNumber)        + 1;
  rcData->numberOfRefinementLevels = max(rcData->refinementLevelNumber) + 1;
  rcData->numberOfMultigridLevels  = max(rcData->multigridLevelNumber)  + 1;
  rcData->numberOfDomains          = max(rcData->domainNumber)          + 1;

  // printf("GridCollection::add: numberOfGrids=%i \n",numberOfGrids());
  

  (*this)[n].reference(g);

  updateReferences();

  rcData->parentChildSiblingInfoNeedsUpdate=true;
  return 0;
}


//\begin{>>GridCollectionInclude.tex}{\subsubsection{add(Mapping)}}
int GridCollection::
add(Mapping & map)
// ==========================================================================
// /Description: 
//   Add a new grid, built from a Mapping
//\end{GridCollectionInclude.tex}
//==========================================================================
{
  MappedGrid g(map);

  if( numberOfGrids()>0  && g.getGridType()!=GenericGrid::unstructuredGrid )
  {
    // set number of ghost points equal to that from the previous grid.
    MappedGrid & mg = (*this)[numberOfGrids()-1];
    for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      for( int side=Start; side<=End; side++ )
      {
	g.setNumberOfGhostPoints(side,axis,mg.numberOfGhostPoints(side,axis));
        // printf("GridCollection::add: set ghsot points to %i\n",mg.numberOfGhostPoints(side,axis));
      }
  }
  // g.update(MappedGrid::THEmask); // **** is this needed
  
  return add(g);
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{deleteGrid}}
int GridCollection::
deleteGrid(Integer k )
// ================================================================================
// /Description:
//    Delete a grid.
//\end{GridCollectionInclude.tex}
// ================================================================================
{
  IntegerArray gridsToDelete(1);
  gridsToDelete=k;

  return deleteGrid(gridsToDelete);
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{displayDistribution}}
void GridCollection::
displayDistribution(const aString & label, FILE *file /* =stdout */ )
// =====================================================================
// /Description:
//    Display the distribution (grids and processors)
//\end{GridCollectionInclude.tex}
// =====================================================================
{
  const int myid = max(0,Communication_Manager::My_Process_Number);
  if( myid==0 )
  {
    const int np=max(1,Communication_Manager::Number_Of_Processors);
    GridCollection & gc = *this;
    fprintf(file,
	    " ======== Parallel Distribution for %s (np=%i)============\n",(const char*)label,np);
    fprintf(file," numberOfGrids=%i, gridDistributionList.size()=%ul \n",gc.numberOfGrids(),
	    gc->gridDistributionList.size());
    for( int grid=0; grid<gc.numberOfComponentGrids(); grid++ )
    {
      MappedGrid & mg = gc[grid];
      Partitioning_Type & partition = (Partitioning_Type &)mg.getPartition();
      const intSerialArray & processorSet = partition.getProcessorSet();

      // if( gc.numberOfGrids()==gc->gridDistributionList.size() )
      if( grid<gc->gridDistributionList.size() ) // *wdh* 101004
      {
        int dimProc[3]={1,1,1}; // hold number of processors per array dimension 
        gc->gridDistributionList[grid].computeParallelArrayDistribution(dimProc);
        const IntegerArray & d = gc[grid].dimension();
	int dims[3]={1,1,1};//
        int gridPts=1, gridPtsPerProc=1;
	for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	{
	  dims[axis]=(d(1,axis)-d(0,axis)+1);
          gridPts*=dims[axis];
	  gridPtsPerProc*=dims[axis]/dimProc[axis];
	}
	
	int minPts=INT_MAX;
	int maxPts=0;
	for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
	{
	  int pointsPerProc=dims[axis]/dimProc[axis];
	  minPts = min(minPts,pointsPerProc);
	  maxPts = max(maxPts,pointsPerProc);
	}
	real ratio=real(maxPts)/real(minPts);
	int pStart=processorSet(processorSet.getBase(0)), pEnd=processorSet(processorSet.getBound(0));
	fprintf(file,"  grid=%i: procs=%i [%i,%i], proc-decomp=[%i]x[%i]x[%i], pts=%i=[%i]x[%i]x[%i], "
		"pts/proc=%i=[%i]x[%i]x[%i], ratio=%i/%i=%3.1g, ghost=[%i,%i,%i]\n",
		grid,pEnd-pStart+1,pStart,pEnd,
		dimProc[0],dimProc[1],dimProc[2],
                gridPts,
		(d(1,0)-d(0,0)+1),
		(d(1,1)-d(0,1)+1),
		(d(1,2)-d(0,2)+1),
                gridPtsPerProc,
		(d(1,0)-d(0,0)+1)/dimProc[0],
		(d(1,1)-d(0,1)+1)/dimProc[1],
		(d(1,2)-d(0,2)+1)/dimProc[2],maxPts,minPts,ratio,
		partition.getGhostBoundaryWidth(0), 
		partition.getGhostBoundaryWidth(1), 
		partition.getGhostBoundaryWidth(2) 
                );
      }
      else
      {
	fprintf(file,"  grid=%i: actual-processors=[%i,%i]\n",
		grid,processorSet(processorSet.getBase(0)),processorSet(processorSet.getBound(0)));
      }
    }
    // fprintf(file,"\n");
    fflush(file);
  }
  // -- now display multigrid levels (if any ) --
  if( numberOfMultigridLevels()>1 )
  {
    for( int l=0; l<numberOfMultigridLevels(); l++ )
    {
      aString labelmg = label + sPrintF(" (level %i)",l);
      multigridLevel[l].displayDistribution(labelmg,file);
    }
  }
  

}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{deleteGrid}}
int GridCollection::
deleteGrid(const IntegerArray & gridsToDelete )
// ================================================================================
// /Description:
//    Delete a list of grids.
//\end{GridCollectionInclude.tex}
// ================================================================================
{
  // see GenericGridCollection::deleteMultigridLevels
  int numberToDelete=gridsToDelete.getLength(0);
  if( numberToDelete==0 )
  {
    printf("GridCollection::deleteGrid:WARNING: no grids were specified to be deleted\n");
    return -1;
  }
  int newNumberOfGrids=numberOfGrids()-numberToDelete;
  if( newNumberOfGrids<0 )
  {
    printf("GridCollection::deleteGrid:ERROR:trying to delete %i grids, but the collection only has %i grids\n",
	   numberToDelete,numberOfGrids());
    return 1;
  }
  if( min(gridsToDelete)<0 || max(gridsToDelete)>=numberOfGrids() )
  {
    printf("GridCollection::deleteGrid:ERROR:trying to delete an invalid grid. There are %i grids\n",numberOfGrids());
    gridsToDelete.display("gridsToDelete");
    Overture::abort("error");
  }

  display(gridsToDelete,"GridCollection::deleteGrids: gridsToDelete");

  // make a list of grids that remain:
  IntegerArray save(numberOfGrids()),ia,gridsToSave(numberOfGrids()-numberToDelete);
  save.seqAdd(0,1);        // all grids: 0,1,2,3,...
  save(gridsToDelete)=-1;  // mark deleted grids
  ia=(save>=0).indexMap(); // list of remaining grids
  gridsToSave=save(ia);    // compressed list of remaining grids

  display(gridsToSave,"gridsToSave");

  return deleteGrid(gridsToDelete,gridsToSave);
  

}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{deleteGrid}}
int GridCollection::
deleteGrid(const IntegerArray & gridsToDelete, const IntegerArray & gridsToSave)
// ================================================================================
// /Description:
//    Delete a list of grids.
//\end{GridCollectionInclude.tex}
// ================================================================================
{
  int numberToDelete=gridsToDelete.getLength(0);
  int newNumberOfGrids=numberOfGrids()-numberToDelete;

  assert( newNumberOfGrids==gridsToSave.getLength(0) );

  // display(baseGridNumber,"baseGridNumber (before)");
  // display(gridsToDelete,"gridsToDelete");
  // display(gridsToSave,"gridsToSave");
  
  Range R=newNumberOfGrids;
  if( true )
  { // do this until A++ is fixed.
    IntegerArray temp;
    // base grid numbers get shifted
    
    // Note: base grids and refinement grids are componentGrid's

     // make a list of base grids that were deleted
    int numberOfBaseGridsDeleted=0;
    int i;
    IntegerArray baseGridsDeleted(numberToDelete);
    for( i=0; i<numberToDelete; i++ )
    {
      if( baseGridNumber(gridsToDelete(i))== gridsToDelete(i) ) // a base grid has itself as the baseGridNumber
      {
	baseGridsDeleted(numberOfBaseGridsDeleted)=baseGridNumber(gridsToDelete(i));
	numberOfBaseGridsDeleted++;
      }
    }
    baseGridsDeleted.resize(numberOfBaseGridsDeleted);
    
    baseGridsDeleted.display("baseGridsDeleted");
    
    // ** temp= baseGridNumber(gridsToSave); baseGridNumber(R)=temp;   // ********** this is wrong *****
    temp=refinementLevelNumber(gridsToSave); refinementLevelNumber(R)=temp;    // this is ok

    // baseGridsDeleted=baseGridNumber(gridsToDelete);

    for( i=0; i<newNumberOfGrids; i++ )
    {
      // we need to shift component grid numbers if a grid "in the middle" is deleted.
      // before : componentGridNumber : 0 1 2 3 4
      //   now delete grids 1 and 3
      // after (before shift) : 0 2 4
      // after (after shift)  : 0 1 2
      componentGridNumber(i)=componentGridNumber(gridsToSave(i));
      int shift = sum( (gridsToDelete-componentGridNumber(i)) < 0 );
      componentGridNumber(i)-=shift;
      // printf("GridCollection::deleteGrid: after shift: componentGridNumber(%i)=%i\n",i,componentGridNumber(i));
      

      // we need to shift base grid numbers
      // before : baseGridNumber : 0 1 2 0 2 
      //   now delete grid 1 
      // after (before shift) : 0 2 0 2 
      // after (after shift)  : 0 1 0 1
      baseGridNumber(i)=baseGridNumber(gridsToSave(i));
      if( numberOfBaseGridsDeleted>0 )
      {
	shift = sum( (baseGridsDeleted-baseGridNumber(i)) < 0 );
	baseGridNumber(i)-=shift;
        // printf("GridCollection::deleteGrid: shift=%i, baseGridNumber(%i)=%i, baseGridsDeleted=%i\n",
        //    shift,i,baseGridNumber(i));
      }
    }
    
    temp=multigridLevelNumber(gridsToSave); multigridLevelNumber(R)=temp;   // this is ok
    temp=domainNumber(gridsToSave); domainNumber(R)=temp;  
  }
  else
  {
    baseGridNumber(R)        = baseGridNumber(gridsToSave);
    refinementLevelNumber(R) = refinementLevelNumber(gridsToSave);
    componentGridNumber(R)   = componentGridNumber(gridsToSave);
    multigridLevelNumber(R)  = multigridLevelNumber(gridsToSave);
    domainNumber(R)          = domainNumber(gridsToSave);
  }
  
  // display(baseGridNumber(R),"baseGridNumber (after)");
  // display(componentGridNumber,"GridCollection::deleteGrids: componentGridNumber (after)");

  for( int i=0; i<numberToDelete; i++ )
  {
    int k=gridsToDelete(i);
#ifdef USE_STL
    grid.erase(grid.begin() + k);
#else
    grid.deleteElement(k);
#endif // USE_STL
   } 

  int & numberOfBaseGrids = rcData->numberOfBaseGrids;
  int & numberOfRefinementLevels = rcData->numberOfRefinementLevels;
  int & numberOfComponentGrids = rcData->numberOfComponentGrids;
  int & numberOfMultigridLevels = rcData->numberOfMultigridLevels;
  int & numberOfDomains         = rcData->numberOfDomains;
  int & computedGeometry = rcData->computedGeometry;

  setNumberOfGrids(newNumberOfGrids);

  // display(componentGridNumber,"GridCollection::deleteGrids: componentGridNumber");
  // display(rcData->componentGridNumber,"GridCollection::deleteGrids: rcData->componentGridNumber");
  

  numberOfBaseGrids        = max(rcData->baseGridNumber)        + 1;
  numberOfRefinementLevels = max(rcData->refinementLevelNumber) + 1;
  numberOfComponentGrids   = max(rcData->componentGridNumber)   + 1;
  numberOfMultigridLevels  = max(rcData->multigridLevelNumber)  + 1;
  numberOfDomains          = max(rcData->domainNumber)          + 1;
  computedGeometry &= ~THElists;

  rcData->parentChildSiblingInfoNeedsUpdate=true;
  return 0;
}

//! Add a refinement grid to the collection.
Integer GridCollection::
addRefinement(
  const IntegerArray& range,  // The indexRange of the refinement grid.
  const IntegerArray& factor, // The refinement factor w.r.t. level-1.
  const Integer&      level,  // The refinement level number.
  const Integer       k)      // The index of an ancestor of the refinement.
{
    
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
Integer GridCollection::
replaceRefinementLevels(int level0, int numberOfRefinementLevels0, IntegerArray **gridInfo )
{
  int returnValue=rcData->replaceRefinementLevels(level0,numberOfRefinementLevels0,gridInfo );
  updateReferences();
  return returnValue;
}



//\begin{>>GridCollectionInclude.tex}{\subsubsection{deleteRefinement}}
void GridCollection::
deleteRefinement(const Integer& k)
// ===========================================================
// /Description:
//   Delete refinement grid k (and all multigrid levels of refinement grid k,
//  if they exist).
//\end{GridCollectionInclude.tex}
// ===========================================================
{
  rcData->deleteRefinement(k);
  updateReferences();
}

//
// Delete all grids with refinement level greater than the given level.
//
void GridCollection::deleteRefinementLevels(const Integer level) {
    rcData->deleteRefinementLevels(level);
    updateReferences();
}
//
// Reference x[i] for refinementLevelNumber(i) <= level.
// Delete all other grids.
//
void GridCollection::referenceRefinementLevels(
  GenericGridCollection& x,
  const Integer          level) {
    rcData->referenceRefinementLevels(*((GridCollection&)x).rcData, level);
    updateReferences();
}
//
// Add a multigrid coarsening of grid k.
//
Integer GridCollection::addMultigridCoarsening(
  const IntegerArray& factor, // The coarsening factor w.r.t level-1
  const Integer&      level,  // The multigrid level number.
  const Integer       k) {    // The index of the corresponding grid
                              // at any finer multigrid level.
    Integer n = rcData->addMultigridCoarsening(factor, level, k);
    updateReferences();
    return n;
}
//
// Delete grid k, a multigrid coarsening, and all of its multigrid coarsenings.
//
void GridCollection::deleteMultigridCoarsening(const Integer& k) {
    rcData->deleteMultigridCoarsening(k);
    updateReferences();
}
//
// Delete all of the grids with multigrid level greater than the given level.
//
void GridCollection::deleteMultigridLevels(const Integer level) {
    rcData->deleteMultigridLevels(level);
    updateReferences();
}

void GridCollection::addToDomain( int d, const IntegerArray & grids )
// ===================================================================================
// /Description:
//    Specify a list of grids to add to a domain d. Note that a grid can only belong
// to one domain at a time so by adding a grid to domain d, it will be removed from 
// the domain that it was in.
// 
// /d (input) : domain number (starting from 0)
// /grids (input) : a list of grids to add to the domain
// ===================================================================================
{
  for( int i=grids.getBase(0); i<=grids.getBound(0); i++ )
  {
    int g=grids(i);
    if( g>=0 && g<numberOfGrids() )
    {
      domainNumber(g)=d;
    }
    else
    {
      printF("GridCollection::addToDomain:ERROR: grid number %i found in grids(%i) is not valid\n"
             "   The grid number should be in the range [%i,%i]. Ignoring this value.\n",
	     g,i,0,numberOfGrids()-1);
    }
  }
}



//
// Set the number of grids.
//
void GridCollection::setNumberOfGrids(const Integer& numberOfGrids_) {
    rcData->setNumberOfGrids(numberOfGrids_);
    updateReferences();
}
//
// Set the number of dimensions.
//
void GridCollection::setNumberOfDimensions(
  const Integer& numberOfDimensions_) {
    rcData->setNumberOfDimensions(numberOfDimensions_);
    updateReferences();
}
//
// Set the number of dimensions and grids.
//
void GridCollection::setNumberOfDimensionsAndGrids(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) {
    rcData->setNumberOfDimensionsAndGrids(numberOfDimensions_, numberOfGrids_);
    updateReferences();
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{sizeOf}}
real GridCollection::
sizeOf(FILE *file /* = NULL */, bool returnSizeOfReference /* = false */ ) const
// ==========================================================================
// /Description: 
//   Return number of bytes allocated by this grid; optionally print detailed info to a file
//
// /file (input) : optinally supply a file to write detailed info to. Choose file=stdout to
// /returnSizeOfReference (input): if true only count the items that would not be referenced if this
//   GridCollection were referenced to another.
// write to standard output.
// /Return value: the number of bytes.
//\end{MappedGridInclude.tex}
//==========================================================================
{
  assert( rcData!=NULL );
  real size=sizeof(*this)+sizeof(*rcData);
  if( numberOfGrids()>0 && !returnSizeOfReference )
  {
    for(int g=0; g<numberOfGrids(); g++ )
    {
      size+=(*this)[g].sizeOf();
    }
  }
  size+=boundingBox.elementCount()*sizeof(real);
  size+=refinementFactor.elementCount()*sizeof(real);
  size+=multigridCoarseningFactor.elementCount()*sizeof(real);
  
  // what about the lists of MappedGrid's and GridCollections ??

  return size;
}

//
//  Initialize the GridCollection with the given number of dimensions and grids.
//  These grids have their gridNumbers, baseGridNumbers and componentGridNumbers
//  set to [0, ..., numberOfGrids_-1], and their refinementLevelNumbers and
//  multigridLevelNumbers set to zero.
//
void GridCollection::initialize(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) {
    GenericGridCollection::initialize(numberOfGrids_);
    rcData->initialize(numberOfDimensions_, numberOfGrids_);
}
//
// Stream output operator.
//
ostream& operator<<(ostream& s, const GridCollection& g) {
    Integer i;
    s <<   (GenericGridCollection&)g << endl
      << "  numberOfDimensions()              =  "
      <<  g.numberOfDimensions() << endl
      << "  boundingBox                       = ["
      <<  g.boundingBox(0,0) << ":"
      <<  g.boundingBox(1,0) << ","
      <<  g.boundingBox(0,1) << ":"
      <<  g.boundingBox(1,1) << ","
      <<  g.boundingBox(0,2) << ":"
      <<  g.boundingBox(1,2) << "]" << endl
      << "  refinementFactor                  = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "")
        << g.refinementFactor(0,i) << ":"
        << g.refinementFactor(1,i) << ":"
        << g.refinementFactor(2,i);
    s << "]" << endl
      << "  multigridCoarseningFactor         = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "")
        << g.multigridCoarseningFactor(0,i) << ":"
        << g.multigridCoarseningFactor(1,i) << ":"
        << g.multigridCoarseningFactor(2,i);
    return s
      << "]";
}

//
// class GridCollectionData:
//
GridCollectionData::GridCollectionData(
  const Integer numberOfDimensions_,
  const Integer numberOfGrids_):
  GenericGridCollectionData(numberOfGrids_) 
{
  className = "GridCollectionData";
  pLoadBalancer=NULL;
  interpolant=NULL;
  initialize(numberOfDimensions_, numberOfGrids_);
}

GridCollectionData::GridCollectionData(
  const GridCollectionData& x,
  const CopyType            ct):
  GenericGridCollectionData() 
{
  className = "GridCollectionData";
  pLoadBalancer=NULL;
  interpolant=NULL;
  initialize(numberOfDimensions, numberOfGrids);
  if (ct != NOCOPY) *this = x;
}

GridCollectionData::
~GridCollectionData() 
{ 
  delete parentChildSiblingInfoList;
  // *wdh* 081022
  if( interpolant!=NULL && !interpolant->uncountedReferencesMayExist() && interpolant->decrementReferenceCount()==0 )
  {
    printF("GridCollectionData::destructor: delete the Interpolant.\n");
    delete interpolant;
  }
}

GridCollectionData& GridCollectionData::
operator=(const GridCollectionData& x) 
{
//    if( true )
//    {
//      for( int g=0; g<min(numberOfGrids,x.numberOfGrids); g++ )
//      {
//        printf("GridCollectionData operator=: BEFORE GenericGCData::operator=(x) : grid=%i name=%s x::name=%s\n",g,
//                (const char*)grid[g].getName(),(const char*)x.grid[g].getName());
//      }
//    }

  numberOfComponentGrids=x.numberOfComponentGrids; // *wdh*
  setNumberOfDimensionsAndGrids(x.numberOfDimensions, x.numberOfGrids);

//    if( true )
//    {
//      for( int g=0; g<min(numberOfGrids,x.numberOfGrids); g++ )
//      {
//        printf("GridCollectionData operator=: BEFORE GenericGCData::operator=(x) : grid=%i name=%s x::name=%s\n",g,
//                (const char*)grid[g].getName(),(const char*)x.grid[g].getName());
//      }
//    }

  GenericGridCollectionData::operator=(x);

//    if( true )
//    {
//      for( int g=0; g<numberOfGrids; g++ )
//      {
//        printf("GridCollectionData operator=: after GenericGCData::operator=(x) : grid=%i name=%s x::name=%s\n",g,
//                (const char*)grid[g].getName(),(const char*)x.grid[g].getName());
//      }
//    }

  boundingBox               = x.boundingBox;
  refinementFactor          = x.refinementFactor;
  multigridCoarseningFactor = x.multigridCoarseningFactor;
  interpolant               = x.interpolant;
  // *wdh* 081022
  if( interpolant!=NULL && !interpolant->uncountedReferencesMayExist())
    interpolant->incrementReferenceCount();

//  grid                      = x.grid; // Optimized below;
//    much of this is redundant, since some grid[i] already
//    have been copied by GenericGridCollectionData::operator=().
//  for (Integer i=0; i<numberOfGrids; i++)
//    GenericGridCollectionData::grid[i].reference(grid[i]);
#ifdef USE_STL
    Integer i = GenericGridCollectionData::grid.size() - grid.size();
    if (i < 0) grid.erase(
      grid.begin() + GenericGridCollectionData::grid.size(), grid.end());
    else for (Integer j=0; j<i; j++) grid.push_back();
    for (i=0; i<grid.size(); i++)
#else
    while (grid.getLength() > GenericGridCollectionData::grid.getLength())
      grid.deleteElement();
    while (grid.getLength() < GenericGridCollectionData::grid.getLength())
      grid.addElement();
    for (Integer i=0; i<grid.getLength(); i++)
#endif // USE_STL
     {
      // *wdh* The parentChildSiblingInfo is is not being copied since it
      // is in the envelop of the MappedGrid class -- should probably move it to the letter.
      grid[i].parentChildSiblingInfo=x.grid[i].parentChildSiblingInfo;

      if ((GenericGrid&)grid[i] != GenericGridCollectionData::grid[i])
      {
        // Normally this loop is not needed since the list "grid" is copied through the base class
        // Question: When is this loop needed???
	GenericGridCollectionData::grid[i].reference(grid[i] = x.grid[i]);

	
      }
    }

    // Now fixup AMR refinement level grids
    for( Integer i=0; i<grid.getLength(); i++)
    {
      // check refinement level number
      if( x.refinementLevelNumber(i)>0 )
      {
	// For AMR grids we need to assign the Mapping to point to the base grid

	const int base = x.baseGridNumber(i);
	// printf("GridCollection: fixup Mapping for AMR grid %i (base=%i)\n",i,base);

	// Make a ReparameterizationTransform that uses the base grid Mapping and a copy
	// of the ReparameterizationTransform from x.grid[i]...
	MappingRC & baseMap = grid[base].rcData->mapping;
	Mapping & xMap = x.grid[i].mapping().getMapping();
	assert( xMap.getClassName()=="ReparameterizationTransform" );
	ReparameterizationTransform & xTransform = (ReparameterizationTransform &)xMap;
          
	// Make a copy of the ReparameterizationTransform -- but use baseMap instead of the one there.
	ReparameterizationTransform & transform = *new ReparameterizationTransform(xTransform,baseMap);
	transform.incrementReferenceCount();

	grid[i].rcData->mapping.reference(transform);
	GenericGridCollectionData::grid[i].reference(grid[i]);

//            if( restrict.decrementReferenceCount()==0 )
//  	    delete &restrict;
	if( transform.decrementReferenceCount()==0 )
	  delete &transform;

      }
    }

    // *wdh* 020413: Don't do this because not all of  ParentChildSiblingInfo is ref counted?
    //     instead we update the ParentChildSiblingInfo in GridCollection::operator=
//     if( x.parentChildSiblingInfoList!=NULL )
//     {
//       if( parentChildSiblingInfoList==NULL )
// 	parentChildSiblingInfoList = new ListOfParentChildSiblingInfo;

//       *parentChildSiblingInfoList=*x.parentChildSiblingInfoList;
//     }
//     else
//     {
//       delete parentChildSiblingInfoList;
//       parentChildSiblingInfoList=NULL;
//     }
    
    return *this;
}
void GridCollectionData::reference(const GridCollectionData& x) {
    cerr << "GridCollectionData::reference(const GridCollectionData&) "
         << "was called!" << endl;
    GenericGridCollectionData::reference(x);
}
void GridCollectionData::breakReference() {
    cerr << "GridCollectionData::breakReference() was called!" << endl;
    GenericGridCollectionData::breakReference();
}
void GridCollectionData::consistencyCheck() const {
    GenericGridCollectionData::consistencyCheck();
    boundingBox               .Test_Consistency();
    refinementFactor          .Test_Consistency();
    multigridCoarseningFactor .Test_Consistency();
    grid                      .consistencyCheck();
    baseGrid                  .consistencyCheck();
    refinementLevel           .consistencyCheck();
    componentGrid             .consistencyCheck();
    multigridLevel            .consistencyCheck();
    domain                    .consistencyCheck();
}
Integer GridCollectionData::
get( const GenericDataBase& db,
     const aString&         name) 
{
    Integer returnValue = 0;
    GenericDataBase& dir = *db.virtualConstructor();
    db.find(dir, name, getClassName());
    GenericGridCollectionData& g0 = *this;

//  Make sure GenericGridCollectionData::grid contains at least one MappedGrid.
#ifdef USE_STL
    if (g0.grid.size() == 0) {
        g0.grid.push_back(MappedGrid());
        GenericGridCollectionData::grid.reference(g0.grid);
    } // end if
#else
    if (g0.grid.getLength() == 0) {
        g0.grid.addElement(MappedGrid());
        GenericGridCollectionData::grid.reference(g0.grid);
    } // end if
#endif // USE_STL


    // Get the parallel distribution info here I think 

    // Allow the user to supply a LoadBalancer, otherwise build one here 

    bool found = gridDistributionList.get(dir,"GridDistributionList");
    if( found )
    {
      int mgLevels=0;
      for( int grid=0; grid<gridDistributionList.size(); grid++ )
	mgLevels=max(mgLevels,gridDistributionList[grid].getMultigridLevel());
      assert( mgLevels>=0 );
      
      if( false )
      {
	printF(" GridCollectionData:get:INFO: gridDistributionList read in. gridDistributionList.size()=%i\n",
	       gridDistributionList.size());
        printF(" numberOfMultigridLevels=%i, mgLevels=%i (from gridDistributionList)\n",numberOfMultigridLevels,mgLevels);

	gridDistributionList.display("GridCollectionData:get: gridDistributionList read in.");
      }
      
      // Load-balance this grid (use the user supplied LoadBalancer if it has been assigned)
      LoadBalancer defaultLoadBalancer;
      LoadBalancer & loadBalancer = pLoadBalancer !=NULL ? *pLoadBalancer : defaultLoadBalancer;

      // work-loads per grid are based on the number of grid points by default:
      // loadBalancer.assignWorkLoads( *this,gridDistributionList );

      if( mgLevels==0 )
      { // there are no multigrid levels -- load balance all grids
        loadBalancer.determineLoadBalance( gridDistributionList );
      }
      else
      { // there are multigrid levels -- load balance level by level
	int refinementLevel=0;
	for( int level=0; level<=mgLevels; level++ )
	{
 	  loadBalancer.determineLoadBalance( gridDistributionList,refinementLevel,level,level );
	  if( false )
	    gridDistributionList.display(sPrintF("\n ** GridCollectionData:get: AFTER load balance level=%i",level));
	}
	
      }
      


      if( false )
      {
	gridDistributionList.display("GridCollectionData:get: gridDistributionList AFTER load balance.");
      }


    }
    else
    {
      printF("INFO: GridCollection::get: parallel grid distribution not found. Will use default.\n");
    }
    
    returnValue |= GenericGridCollectionData::get(dir, "GenericGridCollectionData");


    returnValue |= dir.get(numberOfDimensions, "numberOfDimensions");

    returnValue |= dir.get(boundingBox,        "boundingBox");

//  Make sure that *grid[i].rcData is of the same class as *grid[i-1].rcData.
//  This is done by using the MappedGrid deep copy constructor, which uses
//  MappedGridData::virtualConstructor() to construct *grid[i].rcData.
    Integer i;
#ifdef USE_STL
    for (i=grid.size(); i<numberOfGrids; i++)
      if (i) grid.push_back(MappedGrid(grid[i-1]));
      else grid.push_back(MappedGrid());
#else
    for (i=grid.getLength(); i<numberOfGrids; i++)
      if (i) grid.addElement(MappedGrid(grid[i-1]));
      else grid.addElement();
#endif // USE_STL
    for (i=0; i<numberOfGrids; i++) {
        if (g0.grid[i]->getClassName() != grid[i]->getClassName()) {
            cerr << "GridCollectionData::get(const GenericDataBase&, "
                 << "const aString&):  "
                 <<     "g0.grid[" << i << "]->getClassName() = "
                 <<      g0.grid[     i     ]->getClassName()
                 << " while grid[" << i << "]->getClassName() = "
                 <<         grid[     i     ]->getClassName()
                 << "." << endl;
            assert(g0.grid[i]->getClassName() == grid[i]->getClassName());
        } // end if
        grid[i].reference((MappedGridData&)*g0.grid[i]);
    } // end for

    // for backward compatibility look for this next flag:
   
    int mappingsCompressedForAMR=false;
    
    if( numberOfRefinementLevels>1 )
    {
      if( dir.get(mappingsCompressedForAMR,"mappingsCompressedForAMR")!=0 )
        mappingsCompressedForAMR=false;
    }
    if( mappingsCompressedForAMR && numberOfRefinementLevels>1 )
    {
      char name[32];
      for (Integer i=0; i<numberOfGrids; i++) 
      {
	if( refinementLevelNumber(i)>0 )
	{
	  // this is a refinement grid. Build a mapping.
	  MappedGrid & baseg = (MappedGrid&) grid[baseGridNumber(i)];
      
	  ReparameterizationTransform & newMapping = * new 
            ReparameterizationTransform(*baseg.mapping().mapPointer, ReparameterizationTransform::restriction);
	  newMapping.incrementReferenceCount();
	  
	  real parameterBounds[6];
	  sprintf(name, "parameterBounds[%d]", i);
	  returnValue |= dir.get((real*)parameterBounds,name,6);
//        printf(" GET: grid=%i parameterBounds=%f,%f,%f,%f,%f,%f\n",i,parameterBounds[0],
// 			       parameterBounds[1],
// 			       parameterBounds[2],
// 			       parameterBounds[3],
// 			       parameterBounds[4],
// 			       parameterBounds[5]);

	  newMapping.setBounds(parameterBounds[0], 
			       parameterBounds[1],
			       parameterBounds[2],
			       parameterBounds[3],
			       parameterBounds[4],
			       parameterBounds[5]);

	  MappedGrid & mg = (MappedGrid&) grid[i];
	  mg.mapping().reference(newMapping);

	}
      }
    }


    const Integer computedGeometry0 = computedGeometry;
    initialize(numberOfDimensions, numberOfGrids);

    if (numberOfGrids > 0) {
        returnValue |= dir.get(refinementFactor,
                              "refinementFactor");
        returnValue |= dir.get(multigridCoarseningFactor,
                              "multigridCoarseningFactor");
    } // end if

    GridCollectionData::update(g0,
      computedGeometry0 & EVERYTHING, COMPUTEnothing);
    computedGeometry = computedGeometry0;

    delete &dir;
    return returnValue;
}

Integer GridCollectionData::
put(GenericDataBase& db,
    const aString&   name,
    int geometryToPut /* = -1  */
    ) const 
// geometryToPut : specify which geometry to put, by default put computedGeometry
{
  Integer returnValue = 0;
  const int geometryToPut0 = geometryToPut;
  if( geometryToPut==-1 ) 
    geometryToPut=computedGeometry;
//   else
//   {
//     // NOTE: We must always save some things:
//     geometryToPut |= (THEbaseGrid |
//  		      THErefinementLevel |
//  		      THEcomponentGrid |
//  		      THEmultigridLevel);
//   }

  GenericDataBase& dir = *db.virtualConstructor();
  db.create(dir, name, getClassName());

  // save GridDistributionInfo so we can LoadBalance when we read in the grid
  if( gridDistributionList.size()!=numberOfGrids )
  {
    // Assign default work loads if the gridDistributionList has not been assigned.

    LoadBalancer defaultLoadBalancer;
    LoadBalancer & loadBalancer = pLoadBalancer !=NULL ? *pLoadBalancer : defaultLoadBalancer;

    // work-loads per grid are based on the number of grid points by default:
    loadBalancer.assignWorkLoads( (GridCollectionData&)(*this),(GridDistributionList&)gridDistributionList );
    //  loadBalancer.determineLoadBalance( gridDistributionList );
  }
  if( false )
    printF(" GridCollectionData:put:INFO: put the gridDistributionList. gridDistributionList.size()=%i\n",
	   gridDistributionList.size());

  gridDistributionList.put(dir,"GridDistributionList");

  returnValue |= GenericGridCollectionData::put(dir, "GenericGridCollectionData",geometryToPut0 );

  returnValue |= dir.put(numberOfDimensions, "numberOfDimensions");
  returnValue |= dir.put(boundingBox,        "boundingBox");


  // for backward compatibility:
  if( numberOfRefinementLevels>1 )
  {
    int mappingsCompressedForAMR=true;
    dir.put(mappingsCompressedForAMR,"mappingsCompressedForAMR");

    char name[32];
    for (Integer i=0; i<numberOfGrids; i++) 
    {
      if( refinementLevelNumber(i)>0 )
      {
	// this is a refinement grid. Save the bounds.
	real parameterBounds[6];
	Mapping & map = grid[i].mapping().getMapping();
	assert( map.getClassName()=="ReparameterizationTransform" );

	ReparameterizationTransform & transform = (ReparameterizationTransform &)map;

	transform.getBounds(parameterBounds[0],
			    parameterBounds[1],
			    parameterBounds[2],
			    parameterBounds[3],
			    parameterBounds[4],
			    parameterBounds[5] );
//	 printf(" PUT: grid=%i parameterBounds=%f,%f,%f,%f,%f,%f\n",i,parameterBounds[0],
// 	       parameterBounds[1],
// 	       parameterBounds[2],
// 	       parameterBounds[3],
// 	       parameterBounds[4],
// 	       parameterBounds[5]);

      
	sprintf(name, "parameterBounds[%d]", i);
	dir.put((real*)parameterBounds,name,6);
      }
    }
  }

  if (numberOfGrids > 0) {
    returnValue |= dir.put(refinementFactor,
			   "refinementFactor");
    returnValue |= dir.put(multigridCoarseningFactor,
			   "multigridCoarseningFactor");
  } // end if

  delete &dir;
  return returnValue;
}

Integer GridCollectionData::
update(
  GenericGridCollectionData& x,
  const Integer              what,
  const Integer              how) 
{
//  The GenericGridCollectionData lists are updated in GridCollectionData
//  for the purpose of optimization.
    Integer upd = GenericGridCollectionData::update(x, what & ~(
      THEbaseGrid      | THErefinementLevel |
      THEcomponentGrid | THEmultigridLevel | THEdomain  ), how);
//  GridCollectionData& y = (GridCollectionData&)x;
//  The following statement is redundant, since grid[i] and
//  GenericGridCollectionData::grid[i] refer to the same component grid data.
//  for (Integer i=0; i<numberOfGrids; i++)
//    upd |= grid[i].update(y[i], what, how);
    for (Integer i=0; i<numberOfGrids; i++) grid[i].updateReferences(what);
    Integer computeNeeded =
      how & COMPUTEgeometry         ? what :
      how & COMPUTEgeometryAsNeeded ? what & ~computedGeometry :
                                      NOTHING;
    if (computeNeeded & THEboundingBox) {
        for (Integer k=0; k<numberOfGrids; k++) {
            MappedGrid& g = grid[k];
            for (Integer kd=0; kd<3; kd++) if (k == 0) {
                boundingBox(0,kd) = g.boundingBox(0,kd);
                boundingBox(1,kd) = g.boundingBox(1,kd);
            } else {
                boundingBox(0,kd) =
                  amin1(boundingBox(0,kd), g.boundingBox(0,kd));
                boundingBox(1,kd) =
                  amax1(boundingBox(1,kd), g.boundingBox(1,kd));
            } // end if, end for
        } // end for
        computedGeometry |= THEboundingBox;
    } // end if
    if (what &                THEbaseGrid)
      upd |= updateCollection(THEbaseGrid        | (what & ~THElists),
        numberOfBaseGrids,         baseGrid,
        GenericGridCollectionData::baseGrid,        baseGridNumber);
    if (what &                THErefinementLevel)
      upd |= updateCollection(THErefinementLevel | (what & ~THElists),
        numberOfRefinementLevels,  refinementLevel,
        GenericGridCollectionData::refinementLevel, refinementLevelNumber);
    if (what &                THEcomponentGrid)
      upd |= updateCollection(THEcomponentGrid   | (what & ~THElists),
        numberOfComponentGrids,    componentGrid,
        GenericGridCollectionData::componentGrid,   componentGridNumber);
    if (what &                THEmultigridLevel)
      upd |= updateCollection(THEmultigridLevel  | (what & ~THElists),
        numberOfMultigridLevels,   multigridLevel,
        GenericGridCollectionData::multigridLevel,  multigridLevelNumber);
    if (what &                THEdomain)
      upd |= updateCollection(THEdomain  | (what & ~THElists),
        numberOfDomains,           domain,
        GenericGridCollectionData::domain,          domainNumber);

    return upd;
}


void GridCollectionData::destroy(const Integer what) {
#ifdef USE_STL
    if (what & THEbaseGrid)
      baseGrid.erase       (baseGrid.begin(),        baseGrid.end());
    if (what & THErefinementLevel)
      refinementLevel.erase(refinementLevel.begin(), refinementLevel.end());
    if (what & THEcomponentGrid)
      componentGrid.erase  (componentGrid.begin(),   componentGrid.end());
    if (what & THEmultigridLevel)
      multigridLevel.erase (multigridLevel.begin(),  multigridLevel.end());
    if (what & THEdomain)
      domain.erase (domain.begin(),  domain.end());
#else
    if (what & THEbaseGrid)
      baseGrid.reference(ListOfGridCollection());
    if (what & THErefinementLevel)
      refinementLevel.reference(ListOfGridCollection());
    if (what & THEcomponentGrid)
      componentGrid.reference(ListOfGridCollection());
    if (what & THEmultigridLevel)
      multigridLevel.reference(ListOfGridCollection());
    if (what & THEdomain)
      domain.reference(ListOfGridCollection());
#endif // USE_STL
//  The following statement is redundant, since grid[i] and
//  GenericGridCollectionData::grid[i] refer to the same component grid data.
//  for (Integer i=0; i<numberOfGrids; i++) grid[i].destroy(what);
    GenericGridCollectionData::destroy(what);
}


//! Replace refinement levels "level0" and higher
/*!

 \param level0,numberOfRefinementLevels0 : replace and/or add levels level0,..,numberOfRefinementLevels0-1
 \param gridInfo[bg][l](0:ni-1,0:ng-1) : info defining a new refinement grid on base grid bg 
       and refinement level=level0+l
 */
Integer GridCollectionData::
replaceRefinementLevels(int level0, int numberOfRefinementLevels0, IntegerArray **gridInfo )
{
  int debug=0;

  parentChildSiblingInfoNeedsUpdate=true;

  if( level0<1 || level0>numberOfRefinementLevels )
  {
    printf("GridCollectionData::replaceRefinementLevels: invalid value for level0=%i\n",level0);
    printf("   level0 should be greater than 0 and <=  numberOfRefinementLevels=%i\n",numberOfRefinementLevels);
    Overture::abort("error");
  }

  // We want to reuse any existing refinement grids since it is expensive to create them.


  // newNumberOfRefinementGrids[bg] = number of refinement grids on base grid bg on *new* grid

  int *newNumberOfRefinementGrids = new int[numberOfBaseGrids];
  int bg;
  for( bg=0; bg<numberOfBaseGrids; bg++ ) newNumberOfRefinementGrids[bg]=0;
  
  int level;
  for( level=level0; level<numberOfRefinementLevels0; level++ )
  {
    for( bg=0; bg<numberOfBaseGrids; bg++ )
    {
      newNumberOfRefinementGrids[bg]+=gridInfo[bg][level-level0].getLength(1);
    }
  }
  // rgrid[i][bg] = reference to the new refinement grid -- either a reference to an existing
  //                grid (that we re-use) or a new grid (if there were not enough old ones to reuse)
  real time=getCPU();
  int numberOfNewGrids=0;
  MappedGridData ***rgrid = new MappedGridData ** [numberOfBaseGrids];
  for( bg=0; bg<numberOfBaseGrids; bg++ )
  {
    numberOfNewGrids+=newNumberOfRefinementGrids[bg];
     // I think this should be fast -- otherwise will have to keep pointers
    rgrid[bg] = new MappedGridData * [newNumberOfRefinementGrids[bg]];  // what if 0 are allocated?
  }
  time=getCPU()-time;
  if( debug & 1 ) printf(" time to new %i MappedGridData's = %8.1e\n",numberOfNewGrids,time);
  time=getCPU();
  
  // num[bg] : counts refinement grids on base grid bg
  int *num = new int [numberOfBaseGrids];
  for( bg=0; bg<numberOfBaseGrids; bg++ ) num[bg]=0;

  // Reuse any existing grids -- for a given refinement grid we can only re-use grids from
  // the same base grid (but the level number does not need to be the same).
  // *wdh* 030818 int g0=numberOfBaseGrids; //  will hold first grid in "grid[]" to be replaced
  int g0=-1; //  will hold first grid in "grid[]" to be replaced

  int g;
  for( g=0; g<numberOfGrids; g++ )
  {
    if( refinementLevelNumber(g)>=level0 )
    {
      if( g0<0 ) g0=g;    // all grids >=g0 will be replaced.
      int bg=baseGridNumber(g);
      if( num[bg]<newNumberOfRefinementGrids[bg] )
      {
	// rgrid[bg][num[bg]].reference(grid[g]);
	rgrid[bg][num[bg]]= grid[g].rcData;
        rgrid[bg][num[bg]]->incrementReferenceCount();
	
	num[bg]++;
      }
    }
  }
  if( g0<0 ) g0=numberOfGrids;  // there are no grids to reuse. *wdh* 030818
  
  numberOfNewGrids+=g0;  // total number of grids in the new collection.

  time=getCPU()-time;
  if( debug & 1 ) printf(" time to reference old = %8.1e\n",time);
  time=getCPU();

  // create any new refinement grids if needed
  for( bg=0; bg<numberOfBaseGrids; bg++ ) 
  {
    assert( baseGridNumber(bg)==bg );
    
    MappedGrid &g_b = grid[bg];  // base grid

    for( int rg=num[bg]; rg<newNumberOfRefinementGrids[bg]; rg++ )
    {
      ReparameterizationTransform &newMapping = *new ReparameterizationTransform
	(*g_b.mapping().mapPointer, ReparameterizationTransform::restriction);
      newMapping.incrementReferenceCount();

      MappedGrid mg(newMapping);
      
      rgrid[bg][rg]=mg.rcData;
      rgrid[bg][rg]->incrementReferenceCount();

      if (newMapping.decrementReferenceCount() == 0) delete &newMapping;
    }
  }
  time=getCPU()-time;
  if( debug & 1 ) printf(" time to create extra new = %8.1e\n",time);
  time=getCPU();
  
  //  -- make the grid collection the correct size ---
  setNumberOfGrids( numberOfNewGrids );
  numberOfComponentGrids=numberOfNewGrids;
  
  time=getCPU()-time;
  if( debug & 1 ) printf(" time to setNumberOfGrids = %8.1e\n",time);
  time=getCPU();

  numberOfRefinementLevels=numberOfRefinementLevels0;
  
  // parent[bg][level] : holds a grid that can be the "parent" of refinement grids (the parent
  //     can be *any* grid at the previous level and same base grid.
  int **parent = new int * [numberOfBaseGrids];
  for( bg=0; bg<numberOfBaseGrids; bg++ ) 
  {
    parent[bg] = new int [numberOfRefinementLevels0];
    for( level=0; level<numberOfRefinementLevels0; level++ )
    {
      if( level==0 )
      {
	assert( baseGridNumber(bg)==bg );  // is this ok to assume?
	parent[bg][level]=bg; 
      }
      else
	parent[bg][level]=-1;
    }
  }
  
  for( bg=0; bg<numberOfBaseGrids; bg++ ) num[bg]=0;

  // Now reference grid[g] to the correct grid:  
  IntegerArray range(2,3), factor(3);
  int gNew=g0;
  for( level=level0; level<numberOfRefinementLevels0; level++ )
  {
    // add in new grids for this level
    for( bg=0; bg<numberOfBaseGrids; bg++ )
    {
      int b=bg, p;
	//  Find a parent grid (on the previous refinement level and at the same multigrid level):

      const IntegerArray & info = gridInfo[bg][level-level0];
      int ngrl=info.getLength(1);  // number of refinement grids for this base grid and level
      
      if( ngrl>0 ) 
        parent[bg][level]=gNew;   // this grid can be used as a parent grid
      
      for( int rg=0; rg<ngrl; rg++ )
      {
      
        grid[gNew].reference( *(rgrid[bg][ num[bg] ]) );  
        rgrid[bg][ num[bg] ]->decrementReferenceCount();
	
        // set properties of grid[gNew]
	baseGridNumber(gNew)=bg;
	refinementLevelNumber(gNew)=level;
	componentGridNumber(gNew)=gNew;
	multigridLevelNumber(gNew)=0;
	domainNumber(gNew)=domainNumber(bg);

	p=parent[bg][level-1];
	if( p<0 )
	{
	  for (p=0;
	       refinementLevelNumber(p) != level - 1         ||
		 baseGridNumber(p)      != baseGridNumber(gNew) ||
		 multigridLevelNumber(p)!= multigridLevelNumber(gNew);
	       p++)
	  {
	  }
	  
          parent[bg][level-1]=p;
	}
	assert( p<numberOfComponentGrids && p>=0 );

        range(0,0)=info(0,rg);
        range(1,0)=info(1,rg);
        range(0,1)=info(2,rg);
        range(1,1)=info(3,rg);
        range(0,2)=info(4,rg);
        range(1,2)=info(5,rg);
	factor(0)=info(6,rg);
	factor(1)=info(7,rg);
	factor(2)=info(8,rg);
	
        assert( factor(0)>0 && factor(1)>0 && factor(2)>0 );
	
        updateRefinementGrid( gNew, b, p, range,factor,level );

        // increment counts
        gNew++;
        num[bg]++;
      }
    }
  }

  // Assign parallel distribution (if the info is there)
  if( gridDistributionList.size()==numberOfGrids )
  {
    for( int g=numberOfBaseGrids; g<numberOfGrids; g++ )
    {
      if( refinementLevelNumber(g)>=level0 )
      {
	int pStart=-1,pEnd=0;
	gridDistributionList[g].getProcessorRange(pStart,pEnd);
	// printF("GC::replaceRefinementLevels: assign grid %i to processors=[%i,%i]\n",g,pStart,pEnd);
	grid[g].specifyProcesses(Range(pStart,pEnd));
      }
    }
  }
  else if( gridDistributionList.size()>0 )
  {
    printF("GC::replaceRefinementLevels:WARNING: gridDistributionList is there but it is has the wrong size,\n"
           "  numberOfGrids=%i but gridDistributionList.size()=%i .\n",numberOfGrids,gridDistributionList.size());
  }

  time=getCPU()-time; 
  if( debug & 1 ) printf(" time to build and update grid list = %8.1e\n",time);
  time=getCPU();
  
  delete [] newNumberOfRefinementGrids;
  delete [] num;
  for( bg=0; bg<numberOfBaseGrids; bg++ )
    delete [] rgrid[bg];
  delete [] rgrid;
  for( bg=0; bg<numberOfBaseGrids; bg++ )
    delete [] parent[bg];
  delete [] parent;
     
  if( computedGeometry & GridCollection::THEmultigridLevel )
    printf("***** replaceRefinementLevels END THEmultigridLevel!\n");
  return 0;
}


//! Protected routine to update parameters in a new grid.
int GridCollectionData::
updateRefinementGrid( int n, int b, int p,  
                      const IntegerArray& range,  
                      const IntegerArray& factor, 
                      const Integer& level )
{
  MappedGrid &g_n = grid[n], &g_b = grid[b];

  // *wdh* print a warning if the range is outside the base grid dimensions
  // we do not warn if the grid is periodic since a refinement patch may cross a periodic branch cut
  bool ok=true;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    int ra=range(Start,axis)/refinementFactor(axis,p);
    int rb=range(End,axis)/refinementFactor(axis,p);

    if( (ra < g_b.dimension(Start,axis) || rb>g_b.dimension(End,axis)) && !g_b.isPeriodic(axis) )
    {
      ok=false;
      break;
    }
  }
  if( !ok )
  {
    printf("GridCollection::addRefinement:WARNING: range is not contained in the dimension of the base grid\n");
    printf("range=[%i,%i]x[%i,%i]x[%i,%i], range/refinementFactor=[%i,%i]x[%i,%i]x[%i,%i] (relative to base grid)\n"
	   " should normally be contained in (base grid).dimension=[%i,%i]x[%i,%i]x[%i,%i], base=%i\n",
	   range(0,0),range(1,0),range(0,1),range(1,1),range(0,2),range(1,2),
	   range(0,0)/refinementFactor(0,p),range(1,0)/refinementFactor(0,p),
	   range(0,1)/refinementFactor(1,p),range(1,1)/refinementFactor(1,p),
	   range(0,2)/refinementFactor(2,p),range(1,2)/refinementFactor(2,p),
	   g_b.dimension(0,0),g_b.dimension(1,0),g_b.dimension(0,1),g_b.dimension(1,1),
	   g_b.dimension(0,2),g_b.dimension(1,2),b);
  }


  g_n.isRefinementGrid()=true;

  g_n->numberOfDimensions          = g_b.numberOfDimensions();
  g_n->boundaryCondition           = g_b.boundaryCondition();
  g_n->boundaryDiscretizationWidth = g_b.boundaryDiscretizationWidth();
  g_n->isCellCentered              = g_b.isCellCentered();
  g_n->discretizationWidth         = g_b.discretizationWidth();
  //  g_n->gridIndexRange              = to be determined;
  g_n->numberOfGhostPoints         = g_b.numberOfGhostPoints();
  g_n->useGhostPoints              = LogicalTrue;
  g_n->isPeriodic                  = g_b.isPeriodic();
  g_n->sharedBoundaryFlag          = g_b.sharedBoundaryFlag();
  g_n->sharedBoundaryTolerance     = g_b.sharedBoundaryTolerance();

  RealArray parameterBounds(2,3); IntegerArray baseGridIndexRange(2);
  for (Integer j=0; j<3; j++) 
  {
    if (j < numberOfDimensions)
    {
      refinementFactor(j,n) = refinementFactor(j,p) * factor(j);
      multigridCoarseningFactor(j,n) = multigridCoarseningFactor(j,p);
      Integer i;
      for (i=0; i<2; i++) 
      {
	g_n->gridIndexRange(i,j) = factor(j) * range(i,j);
	baseGridIndexRange(i) = refinementFactor(j,n) * g_b.gridIndexRange(i,j);
      } // end for
      if (g_n.isCellCentered(j))
         g_n->gridIndexRange(1,j) += factor(j);
      else if (g_n.isPeriodic(j) && g_n.gridIndexRange(1,j) == baseGridIndexRange(1) - factor(j))
	g_n->gridIndexRange(1,j) = baseGridIndexRange(1);
      for (i=0; i<2; i++) 
      {
	parameterBounds(i,j)=g_b.gridSpacing(j)/refinementFactor(j,n)*(g_n.gridIndexRange(i,j)-baseGridIndexRange(0));
	if (g_n.gridIndexRange(i,j) != baseGridIndexRange(i))
	  g_n->boundaryCondition(i,j) = g_n->sharedBoundaryFlag(i,j) = 0;
      } // end for
      if (g_n.isPeriodic(j)) 
      {
	if (g_n.gridIndexRange(0,j) != baseGridIndexRange(0) || g_n.gridIndexRange(1,j) != baseGridIndexRange(1)) 
	{
	  g_n->isPeriodic(j) = LogicalFalse;
	  g_n->boundaryCondition(0,j) = g_n->boundaryCondition(1,j) = 0;
	  g_n->numberOfGhostPoints(0,j) = max(g_n.numberOfGhostPoints(0,j),(g_n.discretizationWidth(j) - 1) / 2);
	  g_n->numberOfGhostPoints(1,j) = max(g_n.numberOfGhostPoints(1,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
      } 
      else 
      {
	if (g_n.gridIndexRange(0,j) != baseGridIndexRange(0))
	{
	  g_n->boundaryCondition(0,j) = 0;
	  g_n->numberOfGhostPoints(0,j) = max(g_n.numberOfGhostPoints(0,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
	if (g_n.gridIndexRange(1,j) != baseGridIndexRange(1))
	{
	  g_n->boundaryCondition(1,j) = 0;
	  g_n->numberOfGhostPoints(1,j) = max(g_n.numberOfGhostPoints(1,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
      } // end if
    } 
    else
    {
      refinementFactor(j,n) = multigridCoarseningFactor(j,n) = 1;
      for (Integer i=0; i<2; i++)
      {
	parameterBounds(i,j)     = i;
	g_n->gridIndexRange(i,j) = g_b.gridIndexRange(i,j);
      } // end for
    } // end if
  }
  
  //  We have messed with the dimensions, etc.
  g_n.destroy(~NOTHING);  // destroy everything.
  g_n.update(NOTHING);

  ReparameterizationTransform &newMapping = (ReparameterizationTransform&)(g_n.mapping().getMapping());
  newMapping.setBounds(parameterBounds(0,0), parameterBounds(1,0),
		       parameterBounds(0,1), parameterBounds(1,1),
		       parameterBounds(0,2), parameterBounds(1,2));

  return 0;
}

Integer GridCollectionData::
addRefinement( const IntegerArray& range,
	       const IntegerArray& factor,
	       const Integer&      level,
	       const Integer       k)
{
  parentChildSiblingInfoNeedsUpdate=true;

//  Find the new refinement grid.
  Integer n = GenericGridCollectionData::addRefinement(level, k), b, p;
//  Find the corresponding base grid at the same multigrid level.
  for (b=0;
       refinementLevelNumber(b) != 0                 ||
	 baseGridNumber(b)        != baseGridNumber(n) ||
	 multigridLevelNumber(b)  != multigridLevelNumber(n);
       b++);
//  Find a parent grid at the same multigrid level.
  for (p=0;
       refinementLevelNumber(p) != level - 1         ||
	 baseGridNumber(p)        != baseGridNumber(n) ||
	 multigridLevelNumber(p)  != multigridLevelNumber(n);
       p++);

  MappedGrid &g_n = grid[n], &g_b = grid[b];

  // *wdh* print a warning if the range is outside the base grid dimensions
  // we do not warn if the grid is periodic since a refinement patch may cross a periodic branch cut
  bool ok=true;
  for( int axis=0; axis<numberOfDimensions; axis++ )
  {
    int ra=range(Start,axis)/refinementFactor(axis,p);
    int rb=range(End,axis)/refinementFactor(axis,p);

    if( (ra < g_b.dimension(Start,axis) || rb>g_b.dimension(End,axis)) && !g_b.isPeriodic(axis) )
    {
      ok=false;
      break;
    }
  }
  if( !ok )
  {
    printf("GridCollection::addRefinement:WARNING: range is not contained in the dimension of the base grid\n");
    printf("range=[%i,%i]x[%i,%i]x[%i,%i], range/refinementFactor=[%i,%i]x[%i,%i]x[%i,%i] (relative to base grid)\n"
	   " should normally be contained in (base grid).dimension=[%i,%i]x[%i,%i]x[%i,%i], base=%i\n",
	   range(0,0),range(1,0),range(0,1),range(1,1),range(0,2),range(1,2),
	   range(0,0)/refinementFactor(0,p),range(1,0)/refinementFactor(0,p),
	   range(0,1)/refinementFactor(1,p),range(1,1)/refinementFactor(1,p),
	   range(0,2)/refinementFactor(2,p),range(1,2)/refinementFactor(2,p),
	   g_b.dimension(0,0),g_b.dimension(1,0),g_b.dimension(0,1),g_b.dimension(1,1),
	   g_b.dimension(0,2),g_b.dimension(1,2),b);
  }
	
  ReparameterizationTransform &newMapping = *new ReparameterizationTransform
    (*g_b.mapping().mapPointer, ReparameterizationTransform::restriction);
  newMapping.incrementReferenceCount();
  g_n.reference(newMapping);

  //  --- Fix up the MappedGrid data. ----
  g_n.isRefinementGrid()=true;

  g_n->numberOfDimensions          = g_b.numberOfDimensions();
  g_n->boundaryCondition           = g_b.boundaryCondition();
  g_n->boundaryDiscretizationWidth = g_b.boundaryDiscretizationWidth();
  g_n->isCellCentered              = g_b.isCellCentered();
  g_n->discretizationWidth         = g_b.discretizationWidth();
  //  g_n->gridIndexRange              = to be determined;
  g_n->numberOfGhostPoints         = g_b.numberOfGhostPoints();
  g_n->useGhostPoints              = LogicalTrue;
  g_n->isPeriodic                  = g_b.isPeriodic();
  g_n->sharedBoundaryFlag          = g_b.sharedBoundaryFlag();
  g_n->sharedBoundaryTolerance     = g_b.sharedBoundaryTolerance();

  RealArray parameterBounds(2,3); IntegerArray baseGridIndexRange(2);
  for (Integer j=0; j<3; j++) 
  {
    if (j < numberOfDimensions)
    {
      refinementFactor(j,n) = refinementFactor(j,p) * factor(j);
      multigridCoarseningFactor(j,n) = multigridCoarseningFactor(j,p);
      Integer i;
      for (i=0; i<2; i++) 
      {
	g_n->gridIndexRange(i,j) = factor(j) * range(i,j);
	baseGridIndexRange(i) = refinementFactor(j,n) * g_b.gridIndexRange(i,j);
      } // end for
      if (g_n.isCellCentered(j))
         g_n->gridIndexRange(1,j) += factor(j);
      else if (g_n.isPeriodic(j) && g_n.gridIndexRange(1,j) == baseGridIndexRange(1) - factor(j))
	g_n->gridIndexRange(1,j) = baseGridIndexRange(1);
      for (i=0; i<2; i++) 
      {
	parameterBounds(i,j)=g_b.gridSpacing(j)/refinementFactor(j,n)*(g_n.gridIndexRange(i,j)-baseGridIndexRange(0));
	if (g_n.gridIndexRange(i,j) != baseGridIndexRange(i))
	  g_n->boundaryCondition(i,j) = g_n->sharedBoundaryFlag(i,j) = 0;
      } // end for
      if (g_n.isPeriodic(j)) 
      {
	if (g_n.gridIndexRange(0,j) != baseGridIndexRange(0) || g_n.gridIndexRange(1,j) != baseGridIndexRange(1)) 
	{
	  g_n->isPeriodic(j) = LogicalFalse;
	  g_n->boundaryCondition(0,j) = g_n->boundaryCondition(1,j) = 0;
	  g_n->numberOfGhostPoints(0,j) = max(g_n.numberOfGhostPoints(0,j),(g_n.discretizationWidth(j) - 1) / 2);
	  g_n->numberOfGhostPoints(1,j) = max(g_n.numberOfGhostPoints(1,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
      } 
      else 
      {
	if (g_n.gridIndexRange(0,j) != baseGridIndexRange(0))
	{
	  g_n->boundaryCondition(0,j) = 0;
	  g_n->numberOfGhostPoints(0,j) = max(g_n.numberOfGhostPoints(0,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
	if (g_n.gridIndexRange(1,j) != baseGridIndexRange(1))
	{
	  g_n->boundaryCondition(1,j) = 0;
	  g_n->numberOfGhostPoints(1,j) = max(g_n.numberOfGhostPoints(1,j),(g_n.discretizationWidth(j) - 1) / 2);
	} // end if
      } // end if
    } 
    else
    {
      refinementFactor(j,n) = multigridCoarseningFactor(j,n) = 1;
      for (Integer i=0; i<2; i++)
      {
	parameterBounds(i,j)     = i;
	g_n->gridIndexRange(i,j) = g_b.gridIndexRange(i,j);
      } // end for
    } // end if
  }
  
  //  We have messed with the dimensions, etc.
  g_n.destroy(~NOTHING);  // destroy everything.
  g_n.update(NOTHING);

  newMapping.setBounds(parameterBounds(0,0), parameterBounds(1,0),
		       parameterBounds(0,1), parameterBounds(1,1),
		       parameterBounds(0,2), parameterBounds(1,2));
  if (newMapping.decrementReferenceCount() == 0) delete &newMapping;
  return n;
}

void GridCollectionData::
deleteRefinement(const Integer& k)
// ===================================================================================
// /Description:
//   Delete refinement grid k.
// ===================================================================================
{
  parentChildSiblingInfoNeedsUpdate=true;
  if (k < 0 || k >= numberOfGrids) 
  {
    cout << "GridCollectionData::deleteRefinement(k = "
	 << k << "):  Grid " << k << " does not exist." << endl;
    assert(k >= 0); assert(k < numberOfGrids);
  } 
  else if (refinementLevelNumber(k) == 0)
  {
    cout << "GridCollectionData::deleteRefinement(k = "
	 << k << "):  Grid k = " << k << " is not a refinement." << endl;
    assert(refinementLevelNumber(k) != 0);
  } // end if
  GridCollectionData::deleteMultigridCoarsening(k);
}


void GridCollectionData::
deleteRefinementLevels(const Integer level)
{
  Integer i = numberOfGrids, j = i - 1;
  while (i--) 
    if (refinementLevelNumber(i) > level) 
    {
      if (i < j--) 
      {
	Range three = 3, r1(i, j), r2 = r1 + 1;
	refinementFactor(three,r1) = refinementFactor(three,r2);
	multigridCoarseningFactor(three,r1) = multigridCoarseningFactor(three,r2);
      } // end if
#ifdef USE_STL
      grid.erase(grid.begin() + i);
#else
      grid.deleteElement(i);
#endif // USE_STL
    } // end if, end while
  GenericGridCollectionData::deleteRefinementLevels(level);
}

void GridCollectionData::referenceRefinementLevels(
  GenericGridCollectionData& x,
  const Integer              level) {
    GridCollectionData& y = (GridCollectionData&)x;
    setNumberOfDimensionsAndGrids
      (y.numberOfDimensions, y.numberOfGrids);
    GenericGridCollectionData::referenceRefinementLevels(x, level);
    for (Integer i=0, j=0; i<y.numberOfGrids; i++)
      if (y.refinementLevelNumber(i) <= level) {
        const Range three = 3;
        refinementFactor(three,j) = y.refinementFactor(three,i);
        multigridCoarseningFactor(three,j) =
          y.multigridCoarseningFactor(three,i);
        grid[j].reference(y.grid[i]); j++;
    } // end if, end for
}
Integer GridCollectionData::
addMultigridCoarsening(
  const IntegerArray& factor,
  const Integer&      level,
  const Integer       k) 
{
//  Find the new multigrid coarsening.
    Integer n =
      GenericGridCollectionData::addMultigridCoarsening(level, k), p;
//  Find its next finer multigrid level.
    for (p=0;
//    baseGridNumber(p)        != baseGridNumber(n)        || // (redundant)
//    refinementLevelNumber(p) != refinementLevelNumber(n) || // (redundant)
      componentGridNumber(p)   != componentGridNumber(n)   ||
      multigridLevelNumber(p)  != level - 1;
      p++);
    MappedGrid &g_n = grid[n], &g_p = grid[p];

    g_n.reference(g_p.mapping());

//  Fix up the MappedGrid data.
    g_n->numberOfDimensions          = g_p.numberOfDimensions();
    g_n->boundaryCondition           = g_p.boundaryCondition();
    g_n->boundaryDiscretizationWidth = g_p.boundaryDiscretizationWidth();
    g_n->isCellCentered              = g_p.isCellCentered();
    g_n->discretizationWidth         = g_p.discretizationWidth();
//  g_n->gridIndexRange              = to be determined;
    g_n->numberOfGhostPoints         = g_p.numberOfGhostPoints();
    g_n->useGhostPoints              = g_p.useGhostPoints();
    g_n->isPeriodic                  = g_p.isPeriodic();
    g_n->sharedBoundaryFlag          = g_p.sharedBoundaryFlag();
    g_n->sharedBoundaryTolerance     = g_p.sharedBoundaryTolerance();

    for (Integer j=0; j<3; j++) if (j < numberOfDimensions) {
        refinementFactor(j,n) = refinementFactor(j,p);
        multigridCoarseningFactor(j,n) = multigridCoarseningFactor(j,p) *
          factor(j);
        for (Integer i=0; i<2; i++)
          g_n->gridIndexRange(i,j) = g_p.gridIndexRange(i,j) / factor(j);
    } else {
        refinementFactor(j,n) = multigridCoarseningFactor(j,n) = 1;
        for (Integer i=0; i<2; i++)
          g_n->gridIndexRange(i,j) = g_p.gridIndexRange(i,j);
    } // end if, end for
//  We have messed with the dimensions, etc.
    g_n.destroy(~NOTHING); g_n.update(NOTHING);
    return n;
}

void GridCollectionData::
deleteMultigridCoarsening(const Integer& k) 
// =================================================================================
//  Delete a refinement level grid (and all multigrid coarsenings of it).
// =================================================================================
{
  // printf("** GridCollectionData::deleteMultigridCoarsening k=%i\n",k);

  if (k < 0 || k >= numberOfGrids) 
  {
    cout << "GridCollectionData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid " << k << " does not exist." << endl;
    assert(k >= 0); assert(k < numberOfGrids);
  } 
  else if (multigridLevelNumber(k) == 0 && refinementLevelNumber(k) == 0)
  {
    cout << "GridCollectionData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid k = " << k << " is not a multigrid coarsening."
	 << endl;
    assert(multigridLevelNumber(k) != 0 || refinementLevelNumber(k) != 0);
  } // end if

  Integer i = numberOfGrids, j = i - 1;
  while (i--) 
  {
    if (componentGridNumber(i) == componentGridNumber(k) &&  multigridLevelNumber(i) >= multigridLevelNumber(k)) 
    {
      if (i < j--)
      {
	Range three = 3, r1(i, j), r2 = r1 + 1;
	refinementFactor(three,r1) = refinementFactor(three,r2);
	multigridCoarseningFactor(three,r1) = multigridCoarseningFactor(three,r2);
      } // end if
#ifdef USE_STL
      grid.erase(grid.begin() + i);
#else
      grid.deleteElement(i);
#endif // USE_STL
    } // end if
  }
  GenericGridCollectionData::deleteMultigridCoarsening(k);
}

void GridCollectionData::deleteMultigridLevels(const Integer level) {
    if (level < 0) {
        cout << "GridCollectionData::deleteMultigridLevel(level = "
             << level << "):  Multigrid level " << level << " does not exist."
             << endl;
        assert(level >= 0);
    } else if (level < numberOfMultigridLevels-1) {
        Integer i = numberOfGrids, j = i - 1;
        while (i--) if (multigridLevelNumber(i) > level) {
            if (i < j--) {
                Range three = 3, r1(i, j), r2 = r1 + 1;
                refinementFactor(three,r1) = refinementFactor(three,r2);
                multigridCoarseningFactor(three,r1) =
                  multigridCoarseningFactor(three,r2);
            } // end if
#ifdef USE_STL
            grid.erase(grid.begin() + i);
#else
            grid.deleteElement(i);
#endif // USE_STL
        } // end if, end while
    } // end if
    GenericGridCollectionData::deleteMultigridLevels(level);
}
void GridCollectionData::setNumberOfGrids(const Integer& numberOfGrids_)
  { setNumberOfDimensionsAndGrids(numberOfDimensions, numberOfGrids_); }
void GridCollectionData::setNumberOfDimensions(
  const Integer& numberOfDimensions_)
  { setNumberOfDimensionsAndGrids(numberOfDimensions_, numberOfGrids); }

void GridCollectionData::
setNumberOfDimensionsAndGrids(
  const Integer& numberOfDimensions_,
  const Integer& numberOfGrids_) 
{
  if( numberOfMultigridLevels==0 && numberOfGrids_>0 ) // *wdh* 010530
    numberOfMultigridLevels  = 1;

  if( numberOfDomains==0 && numberOfGrids_>0 ) 
    numberOfDomains  = 1;

  numberOfDimensions = numberOfDimensions_;
  Integer n =
    numberOfGrids_ - refinementFactor.elementCount() / 3;
  if (n) 
  {
    refinementFactor         .resize(3, numberOfGrids_);
    multigridCoarseningFactor.resize(3, numberOfGrids_);
    if (n > 0) 
    {
      const Range three = 3,
	newGrids(numberOfGrids_ - n, numberOfGrids_ - 1);
      refinementFactor(three, newGrids)          = 1;
      multigridCoarseningFactor(three, newGrids) = 1;
    } // end if
  } // end if
#ifdef USE_STL
  n = numberOfGrids_ - grid.size();
  if (n < 0) grid.erase(grid.begin() + numberOfGrids_, grid.end());
  else for (Integer i=0; i<n; i++) grid.push_back();
  for (Integer i=0; i<numberOfGrids_; i++)
    if (GenericGridCollectionData::grid.size() > i)
      GenericGridCollectionData::grid[i].reference(grid[i]);
    else GenericGridCollectionData::grid.push_back(grid[i]);
  if (GenericGridCollectionData::grid.size() > numberOfGrids_)
    GenericGridCollectionData::grid.erase(
      GenericGridCollectionData::grid.begin() + numberOfGrids_,
      GenericGridCollectionData::grid.end());
#else
  while (grid.getLength() < numberOfGrids_)
    grid.addElement(MappedGrid(numberOfDimensions));
  while (grid.getLength() > numberOfGrids_) grid.deleteElement();
  for (Integer i=0; i<numberOfGrids_; i++)
    if (GenericGridCollectionData::grid.getLength() > i)
      GenericGridCollectionData::grid[i].reference(grid[i]);
    else GenericGridCollectionData::grid.addElement(grid[i]);
  while (GenericGridCollectionData::grid.getLength() > numberOfGrids_)
    GenericGridCollectionData::grid.deleteElement();
#endif // USE_STL
  GenericGridCollectionData::setNumberOfGrids(numberOfGrids_);
}


// =========================================================================================
/// \brief update "collections" such as THEbaseGrid, THErefinementLevel, THEcomponentGrid, 
///    THEmultigridLevel, and THEdomain. 
/// \details The collections are lists that consist of sub-sets of the total set of grids. 
//     For example, cg.refinementLevel[l] is a GridCollection that holds all grids on a
//     given refinement level.
// =========================================================================================
Integer GridCollectionData::
updateCollection(
  const Integer&                   what,
  Integer&                         numberOfCollections,
#ifdef USE_STL
  RCVector<GridCollection>&        list,
  RCVector<GenericGridCollection>& genericList,
#else
  ListOfGridCollection&            list,
  ListOfGenericGridCollection&     genericList,
#endif // USE_STL
  IntegerArray&                    number) 
{
  //  Fix up the length of list.
  numberOfCollections = numberOfGrids > 0 ? max(number) + 1 : 0;
#ifdef USE_STL
  if (list.size() > numberOfCollections) list.erase(
    list.begin() + numberOfCollections, list.end());
  if (genericList.size() > numberOfCollections) genericList.erase(
    genericList.begin() + numberOfCollections, genericList.end());
#else
  while (list.getLength() > numberOfCollections) list.deleteElement();
  while (genericList.getLength() > numberOfCollections)
    genericList.deleteElement();
#endif // USE_STL
  if (numberOfCollections) {
//      Fill lists with appropriately-constructed GridCollections.
    IntegerArray nG(numberOfCollections); nG = 0; Integer k, i;
    for (k=0; k<numberOfGrids; k++) nG(number(k))++;
    for (i=0; i<numberOfCollections; i++) 
    {
#ifdef USE_STL
      if (i < list.size())
	list[i].setNumberOfDimensionsAndGrids(numberOfDimensions, nG(i));
      else list.push_back(GridCollection(numberOfDimensions, nG(i)));
      if (i < genericList.size()) genericList[i].reference(list[i]);
      else genericList.push_back(list[i]);
#else
      if (i < list.getLength())
	list[i].setNumberOfDimensionsAndGrids(numberOfDimensions, nG(i));
      else list.addElement(GridCollection(numberOfDimensions, nG(i)));
      if (i < genericList.getLength()) genericList[i].reference(list[i]);
      else genericList.addElement(list[i]);
#endif // USE_STL
    } // end for
    GenericGridCollectionData::updateCollection(what, numberOfCollections, genericList, number);
    for (nG=0, k=0; k<numberOfGrids; k++) 
    {
      const Integer j = nG(i = number(k))++; 
      list[i][j].reference(grid[k]);
      list[i].refinementFactor(0,j) = refinementFactor(0,k);
      list[i].refinementFactor(1,j) = refinementFactor(1,k);
      list[i].refinementFactor(2,j) = refinementFactor(2,k);
      list[i].multigridCoarseningFactor(0,j) = multigridCoarseningFactor(0,k);
      list[i].multigridCoarseningFactor(1,j) = multigridCoarseningFactor(1,k);
      list[i].multigridCoarseningFactor(2,j) = multigridCoarseningFactor(2,k);
    } // end for
  } else {
    GenericGridCollectionData::updateCollection(
      what, numberOfCollections, genericList, number);
  } // end if
  return what & THElists;
}

void GridCollectionData::
initialize(const Integer& numberOfDimensions_,
	   const Integer& numberOfGrids_) 
{
  GridCollectionData::setNumberOfDimensionsAndGrids
    (numberOfDimensions_, numberOfGrids);

  // *wdh* 081022
  if( interpolant!=NULL )
  {
    if( !interpolant->uncountedReferencesMayExist() && interpolant->decrementReferenceCount()==0 )
    {
      if( Mapping::debug & 4 ) printF("GridCollectionData::initialize: delete an old Interpolant\n");
      delete interpolant;
    }
  }
  
  interpolant        = NULL;
  parentChildSiblingInfoList=NULL;
  parentChildSiblingInfoNeedsUpdate=true;
  
  boundingBox.redim(2,3); boundingBox = (Real)0.;
  destroy(~NOTHING & ~GenericGridCollectionData::EVERYTHING
	  & ~MappedGrid::EVERYTHING);
}



#include "BoxLib.H"
#include "Box.H"
#include "BoxList.H"

static Box 
cellCenteredBox( MappedGrid & mg, int ratio =1 )
// ================================================================================================
// /Description:
//   Build a cell centered box from a MappedGrid.
//\end{RegridInclude.tex} 
// ===============================================================================================
{
  Box box = mg.box();      // we could keep a list for below
  int axis;
  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
  {
    if( mg.isPeriodic(axis) )
    {
      box.setSmall(axis,mg.gridIndexRange(0,axis)-1);
      box.setBig(axis,  mg.gridIndexRange(1,axis)+1);
    }
//      else
//      { // include interpolation points on interpolation boundaries.
//        if( mg.boundaryCondition(0,axis)==0 )
//  //        box.setSmall(axis,mg.dimension(0,axis)); 
//          box.setSmall(axis,mg.gridIndexRange(0,axis)-1); // should be include more ghost points if they exist?
//        if( mg.boundaryCondition(1,axis)==0 )
//  //        box.setSmall(axis,mg.dimension(1,axis)); 
//          box.setBig(axis,  mg.gridIndexRange(1,axis)+1);
//      }
    
  }


  box.convert(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));  // build a cell centered box

  if( ratio>1 )
    box.refine( ratio );
  else if( ratio<-1 )
    box.coarsen( -ratio );

  for( axis=mg.numberOfDimensions(); axis<3; axis++ )
  {
    box.setSmall(axis,0); // +widthOfProperNesting);
    box.setBig(axis,0);   // +widthOfProperNesting);
  }
  return box;
}



static int
buildInteriorDomain(GridCollection & gc, 
		    int baseGrid,
		    int refinementLevel,
		    BoxList & interior  )
// ============================================================================================
// /Description:
//   Build a list of boxes that covers the domain interior (one grid-line inside) the the boxes on a level.
//  This list is used to mark points on the mask hidden by refinement.
//  
//
// /baseGrid (input): boxes for this base grid.
// /refinementLevel (input) : build a list for this level (>1)
// /interior : a list of boxes defining the interior region.
//
//\end{RegridInclude.tex} 
// ==========================================================================================
{
  int debug=0;
  if( gc.numberOfRefinementLevels()<=1 || refinementLevel<1 )
    return 0;
  
  GridCollection & rl = gc.refinementLevel[refinementLevel];
  IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));

  // ** this doesn't work: intersects requires all directions to be the same centering
  // IndexType centering (D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL));
  // if( numberOfDimensions==2 )
  //  centering =IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::NODE));

  BoxList boxList1(centering), boxList2(centering);

  // rl.refinementFactor is the refinement to the base grid
  int grid=0;
  int ratio=rl.refinementFactor(0,grid)/gc.refinementLevel[refinementLevel-1].refinementFactor(0,grid); 
  if( debug & 1 )
    printf(" **** ratio=%i \n",ratio);
  
  assert( ratio>1 );
  int relativeRatio=int(pow(ratio,refinementLevel)+.5);  // total ratio from this level to base level

  MappedGrid & bg = gc[baseGrid];
  // find all refinement grids on this level that have the same base grid
  for( int gr=0; gr<rl.numberOfComponentGrids(); gr++ )
  {
    if( rl.baseGridNumber(gr)==baseGrid )
    {
      // build a fine box
      MappedGrid & mg = gc[rl.gridNumber(gr)];
      Box box= cellCenteredBox(mg);
      for( int axis=0; axis<mg.numberOfDimensions(); axis++ )
      {
	if( bg.isPeriodic(axis) )
	{
// 020927
	  if( mg.gridIndexRange(Start,axis)==bg.gridIndexRange(Start,axis) )
    	    box.setSmall(axis,mg.gridIndexRange(0,axis)-ratio);
	  if( mg.gridIndexRange(End,axis)==bg.gridIndexRange(End,axis)*relativeRatio )
  	    box.setBig(axis,  mg.gridIndexRange(1,axis)+ratio);
	}
        else 
	{
//          If there is a refinement grid that on the ghost line of an interpolation
//          boundary we extend the box outward to  avoid having a single
//          valid line on the coarser grid (020927)
//                 

          if( bg.boundaryCondition(Start,axis)==0 && 
              mg.gridIndexRange(0,axis)==(bg.gridIndexRange(0,axis)-1)*relativeRatio )
  	    box.setSmall(axis,mg.gridIndexRange(0,axis)-ratio);
          if( bg.boundaryCondition(End,axis)==0 && 
              mg.gridIndexRange(1,axis)==(bg.gridIndexRange(1,axis)+1)*relativeRatio   )
	    box.setBig(axis,  mg.gridIndexRange(1,axis)+ratio);

          // *wdh* 040726: Expand the box at physical boundaries so that the boundary
          // itself will be marked as hidden
          if( mg.boundaryCondition(Start,axis)>0 )
  	    box.setSmall(axis,mg.gridIndexRange(0,axis)-ratio);
          if( mg.boundaryCondition(End,axis)>0 )
	    box.setBig(axis,  mg.gridIndexRange(1,axis)+ratio);

	}
      }
      
      if( debug & 2 ) 
      {
	printf(" Add grid %i to to boxList1\n",rl.gridNumber(gr));
	cout << "---> box = " << box << endl;
      }
	
      boxList1.add(box); 
    }
  }
  // Method: first build the complement of the boxList1, then expand the complement.
  // Build the proper nesting domain as the complement of the complement.

  // could be a box that covers all flagged points:
  // on the base level we may have to treat periodic boxes 
  // int relativeRatio=int(pow(ratio,refinementLevel)+.5);
  Box baseBox= cellCenteredBox(bg,relativeRatio);
  // expand the base box since we need to include ghost points on interpolation boundaries.
  for( int axis=0; axis<gc.numberOfDimensions(); axis++ )
    baseBox.grow(axis,relativeRatio);  
// *wdh* 020927    baseBox.grow(axis,1);  

  if( debug & 2 )
    cout << "baseBox :" << baseBox << endl;

  boxList2=complementIn(baseBox,boxList1);

  if( debug & 2 )
  {
    cout << " boxList1: " << boxList1 << endl;
    cout << " complement of boxList1, boxList2: " << boxList2 << endl;
  }
  int widthToShrink=1;
  BoxList complement;
  complement=accrete(boxList2,widthToShrink);
  complement.intersect(baseBox);
  complement.simplify();
  interior=complementIn(baseBox,complement);
  interior.simplify();
  boxList1.clear();
  boxList2.clear();


  return 0;
}

//\begin{>>GridCollectionInclude.tex}{\subsubsection{setMaskAtRefinements}}
int GridCollection::
setMaskAtRefinements()
// ===========================================================
// /Description:
//    Assign values in the mask arrays to reflect points that are
// hidden by refinement grids. This information is used when plotting
// grids, for example.
//
//\end{GridCollectionInclude.tex}
// ===========================================================
{
  int debug=0;
  
  GridCollection & gc = *this;
  if( gc.numberOfRefinementLevels()<=1 )
    return 0;
  
  gc.update(MappedGrid::THEmask); // make sure the mask is updated.

  // we need to unmark hidden mask values. -- could do better here --
  bool newWay=true;
  int g;
  for( g=0; g<gc.numberOfComponentGrids(); g++ )
  {
    if( newWay )
    {
      #ifdef USE_PPP
        intArray & mask = gc[g].mask();
        intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
      #else
        intSerialArray& maskLocal=gc[g].mask();
      #endif
      maskLocal &= ~MappedGrid::IShiddenByRefinement;
    }
    else
    {
      gc[g].mask()&= ~MappedGrid::IShiddenByRefinement;
    }
    
    gc[g]->computedGeometry |=  MappedGrid::THEmask; // needed as mask is saved to db file.
  }

  Index Iv[3], &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];

  BoxList interior;
  for( int gb=0; gb<gc.numberOfBaseGrids(); gb++ )
  {
    int baseg=gc.baseGridNumber(gb);
    MappedGrid & bg = gc[baseg];  // ***wdh
    
    for( int level=1; level<gc.numberOfRefinementLevels(); level++ )
    {
      assert( gc.refinementLevel[level].numberOfComponentGrids()>0 );
      
      buildInteriorDomain(gc,baseg,level,interior);

      GridCollection & rl = gc.refinementLevel[level-1]; // mark points on this lower level
      
      g=0;
      const int ratio=gc.refinementLevel[level].refinementFactor(0,g)/rl.refinementFactor(0,g);
      int axis;
      for( g=0; g<rl.numberOfComponentGrids(); g++ )
      {
	if( rl.baseGridNumber(g)==baseg )
	{
	  // intersect this grid with the interior box list
	  Box box= cellCenteredBox(rl[g],ratio);  // refined box

          int grid0=rl.gridNumber(g);
          MappedGrid & mg = gc[grid0];
	  const intArray & mask = mg.mask();
          #ifdef USE_PPP
           intSerialArray maskLocal; getLocalArrayWithGhostBoundaries(mask,maskLocal);
          #else
           const intSerialArray & maskLocal=mask; 
          #endif

// ------------
	  for( axis=0; axis<mg.numberOfDimensions(); axis++ )
	  {
	    if( bg.boundaryCondition(Start,axis)==0 )
	      box.setSmall(axis,box.smallEnd(axis)-ratio);
	    if( bg.boundaryCondition(End,axis)==0 )
	      box.setBig(axis,box.bigEnd(axis)+ratio);
	  }
// ------------- 
          // cout << "Box for grid " << rl.gridNumber(g) << " is" << box << endl;
	  
	  BoxList intersectionList(IndexType(D_DECL(IndexType::CELL,IndexType::CELL,IndexType::CELL)));
	  intersectionList=intersect(interior,box);
	  intersectionList.simplify();
	  for( BoxListIterator bli(intersectionList); bli; ++bli)
	  {
	    Box box = intersectionList[bli];
            // cout << "intersection: box=" << box << endl;
	    for( axis=0; axis<gc.numberOfDimensions(); axis++ )
	    {
              int base = floorDiv(box.smallEnd(axis)+ratio-1,ratio); // round up
	      int bound= floorDiv(box.bigEnd(axis)+1        ,ratio); // add one since we create node centered grids
	      Iv[axis]=Range(base,bound);
	    }
	    for( axis=gc.numberOfDimensions(); axis<3; axis++ )
	      Iv[axis]=mg.gridIndexRange(0,axis);  // default values

            if( debug & 1 )
              printf("Mark hidden by refinement: grid=%i (baseGrid=%i) on level %i points: [%i,%i]x[%i,%i] ratio=%i\n",
                     grid0,baseg,level-1,
		     I1.getBase(),I1.getBound(),I2.getBase(),I2.getBound(),ratio);
                   
	    if( newWay )
	    {
              int includeGhost=true;
              bool ok=ParallelUtility::getLocalArrayBounds(mask,maskLocal,I1,I2,I3,includeGhost);
              if( !ok ) continue;
	      where( maskLocal(I1,I2,I3)!=0 )
		maskLocal(I1,I2,I3) |= MappedGrid::IShiddenByRefinement; 
	    }
	    else
	    {
	      where( mask(I1,I2,I3)!=0 )
		mask(I1,I2,I3) |= MappedGrid::IShiddenByRefinement; 
	    }
	    
	  }
          
	}
      }
      interior.clear();
    }
    gc[baseg].mask().periodicUpdate();
  }
  return 0;
}

// =============  old way
/* ---

//\begin{>>GridCollectionInclude.tex}{\subsubsection{setMaskAtRefinements}}
int GridCollection::
setMaskAtRefinements()
// ===========================================================
// /Description:
//    Assign values in the mask arrays to reflect points that are
// hidden by refinement grids. This information is used when plotting
// grids, for example. On a vertex grid, vertices on the coarse grid
// that lie under the 
//\end{GridCollectionInclude.tex}
// ===========================================================
{

  GridCollection & gc = *this;
  gc.update(MappedGrid::THEmask); // make sure the mask is updated.

  if( gc.numberOfRefinementLevels()<=1 )
    return 0;

  int debug=0; // 3;
  

  const int & numberOfDimensions = gc.numberOfDimensions();
  
  Index Iv[3]; // , &I1=Iv[0], &I2=Iv[1], &I3=Iv[2];
  Index Jv[3], &J1=Jv[0], &J2=Jv[1], &J3=Jv[2];
  Index Kv[3], &K1=Kv[0], &K2=Kv[1], &K3=Kv[2];
  Range all;
  
  int rf[3]={1,1,1};  // refinement factors to the base grid
  int rrf[3]={1,1,1};  // relative refinement factors to a grid on a lower level.
  
  // we need to unmark hidden mask values. -- could do better here --
  int g;
  for( g=0; g<gc.numberOfComponentGrids(); g++ )
  {
    gc[g].mask()&= ~MappedGrid::IShiddenByRefinement;
    gc[g]->computedGeometry |=  MappedGrid::THEmask; // needed ao mask is asaved to db file.
  }


  int axis;
  for( int l=gc.numberOfRefinementLevels()-1; l>0; l-- )
  {
    
    GridCollection & rl   = gc.refinementLevel[l];
    GridCollection & rlm1 = gc.refinementLevel[l-1];

    for( g=0; g<rl.numberOfComponentGrids(); g++ )
    {
      assert( rl[g].isAllVertexCentered() );
      
      const int g0 =rl.gridNumber(g);        // index into gc
      const int base = gc.baseGridNumber(g0);  // base grid for this refinement

      MappedGrid & cr = rl[g];              // refined grid
      MappedGrid & cb = gc[base];             // base grid
      // const IntegerArray & indexRange = cr.indexRange();
      const IntegerArray & indexRange = cr.extendedIndexRange();
      const intArray & mask = cr.mask();
       
      // rl.refinementFactor is the refinement to the base grid
      rf[0]=rl.refinementFactor(0,g); 
      rf[1]=rl.refinementFactor(1,g);
      rf[2]=rl.refinementFactor(2,g);
      if( debug & 1 )
        printf(" refinement factors l=%i are (%i,%i,%i)\n",l,rf[0],rf[1],rf[2]);
      assert( rf[0]>0 && rf[1]>0 && rf[2]>0 );

      const bool isAllVertexCentered = !gc[0].isAllCellCentered();
      // const real ccOffset= isAllVertexCentered ? 0. : .5;

      // Mark points that lie underneath this grid on level l-1.  *** should we do all levels below??
      bool onlyOneBaseGrid=false;
      for( int g2=0; g2<rlm1.numberOfComponentGrids() && !onlyOneBaseGrid; g2++ )
      {
        if( rlm1.baseGridNumber(g2)!=base )
          continue;
	for( axis=0; axis<numberOfDimensions; axis++ )
	{
	  rrf[axis]=rf[axis]/rlm1.refinementFactor(axis,g2); // refinement factor to coarser grid
	  assert( rrf[axis]>0 );

	  Iv[axis]=Range(cr.indexRange(Start,axis),
                         cr.indexRange(End  ,axis),rrf[axis]);

	  // Jv : coarse grid index values corresponding to fine grid values in Iv
	  Jv[axis]=Range(floorDiv(Iv[axis].getBase(),rrf[axis]),floorDiv(Iv[axis].getBound(),rrf[axis]));
          // *wdh* 000815 do not mark boundary points as being covered.
	  // Jv[axis]=Range(floorDiv(Iv[axis].getBase(),rrf[axis])+1,floorDiv(Iv[axis].getBound(),rrf[axis])-1);
	}
	// determine K[dir]=intersection of Jv with c2.indexRange();

	const IntegerArray & extended2 = extendedGridIndexRange(rlm1[g2]);
        // --> if the grid g2 is periodic, we may have 2 sub-patches to assign
        bool done=false;
        for( int periodicPatch=0; periodicPatch<2 && !done; periodicPatch++ )
	{
          done=true;
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    if( rlm1[g2].isPeriodic(axis)==Mapping::functionPeriodic && 
		(Jv[axis].getBase() < extended2(Start,axis) || Jv[axis].getBound() > extended2(Start,axis)) )
	    {
              //  printf("*** refinement patch covers a branch cut ****\n");
	      
              if( periodicPatch==0 )
	      {
		done=false;  // 
	      }
	      else 
	      { // shift by the period
                if( Jv[axis].getBase() < extended2(Start,axis) )
  		  Jv[axis]+=rlm1[g2].gridIndexRange(End,axis)-rlm1[g2].gridIndexRange(Start,axis);
                else
  		  Jv[axis]-=rlm1[g2].gridIndexRange(End,axis)-rlm1[g2].gridIndexRange(Start,axis);
	      }
	    }
	  }

	  bool intersects=TRUE;
	  Kv[axis3]=Range(extended2(Start,axis3),extended2(Start,axis3));
	  for( axis=0; axis<numberOfDimensions; axis++ )
	  {
	    int base =max(Jv[axis].getBase(), extended2(Start,axis));
	    int bound=min(Jv[axis].getBound(),extended2(End  ,axis));
	    if( base>bound )
	    {
	      intersects=FALSE;
	      break;
	    }
	    Kv[axis]=Range(base,bound);
	  }
	  if( intersects )
	  {
	    const intArray & mask2 = rlm1[g2].mask();
	  

	    where( mask2(K1,K2,K3)!=0 )
	    {
	      mask2(K1,K2,K3)|= MappedGrid::IShiddenByRefinement;  
	    }
	  
            rlm1[g2].mask().periodicUpdate();

	    // if the coarsening of the fine grid patch is entirly in the parne grid then there
	    // must only be one parent:
	    onlyOneBaseGrid= K1==J1 && K2==J2 && K3==J3;
	  
	  }  // if intersects
	}
      }
    }
  }
  return 0;
}

--- */
