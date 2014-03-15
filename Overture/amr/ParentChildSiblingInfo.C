// created by Bobby Philip 05092001
#include "ParentChildSiblingInfo.h"
#include "ParentInfo.h"
#include "ChildInfo.h"
#include "SiblingInfo.h"
//
// class ParentChildSiblingInfo:
//

ParentChildSiblingInfo::ParentChildSiblingInfo(): ReferenceCounting()
{
  className = "ParentChildSiblingInfo";
  rcData = new ParentChildSiblingInfoData();

  if( rcData == NULL )
    {
      cerr << "ParentChildSiblingInfo::ParentChildSiblingInfo()"
	   << ": unable to allocate space for ParentChildSiblingInfoData object" << endl;
	assert( rcData != NULL );
    }

  rcData->incrementReferenceCount();
}

//! copy constructor that does a deep copy by default
ParentChildSiblingInfo::ParentChildSiblingInfo( const ParentChildSiblingInfo &x,
						const CopyType ct ): ReferenceCounting(x, ct)
  
{
  className = "ParentChildSiblingInfo";
  switch (ct) {
  case DEEP:
  case NOCOPY:
    rcData = (ParentChildSiblingInfoData*)((ReferenceCounting*)x.rcData)->virtualConstructor(ct);
    if( rcData == NULL )
      {
	cerr << "ParentChildSiblingInfo::ParentChildSiblingInfo( const ParentChildSiblingInfo &x, const CopyType ct )"
	     << ": call to ReferenceCounting::virtualConstructor failed!" << endl;
	assert( rcData !=NULL );
      }
    rcData->incrementReferenceCount();
    break;
  case SHALLOW:
    rcData = x.rcData;
    if( rcData != NULL ) rcData->incrementReferenceCount();
    break;
  default:
    cerr << "ParentChildSiblingInfo::ParentChildSiblingInfo( const ParentChildSiblingInfo &x, const CopyType ct )"
	 << ": invalid CopyType parameter value " << endl;
    abort();
    break;
  }
}

ParentChildSiblingInfo::~ParentChildSiblingInfo()
{
  if( ( rcData!=NULL )&&( rcData->decrementReferenceCount()==0 )) delete rcData;
}

ParentChildSiblingInfo &ParentChildSiblingInfo::operator=( const ParentChildSiblingInfo &x )
{
  if (rcData != x.rcData) 
    {
      if (rcData->getClassName() == x.rcData->getClassName()) 
	{
	  (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
	  // updateReferences();  
	}
      else 
	{
	  ParentChildSiblingInfo& y = *(ParentChildSiblingInfo*)x.virtualConstructor();
	  reference(y);
	  delete &y;
	} // end if
    } // end if
  return *this;
}

/*! sets the rcData pointer to point to the ParentChildSiblingInfoData that x->rcData
 *  points at and increments the reference count for the ParentChildSiblingInfoData object
 */
void ParentChildSiblingInfo::reference( const ParentChildSiblingInfo &x )
{
  ReferenceCounting::reference(x);
  if( rcData != x.rcData ) // if the letter isn't the same
    {
      if(( rcData!=NULL )&&( rcData->decrementReferenceCount()==0 )) 
	delete rcData;     // delete letter if there aren't any other references to it
      rcData = x.rcData;   // point to x's letter
      if( rcData != NULL ) // if it's not NULL increment the reference count
	rcData->incrementReferenceCount();
      //      updateReferences();
    }
}

/*! sets the rcData pointer to the ParentChildSiblingInfoData object x,
 *  incrementing its reference count */
void ParentChildSiblingInfo::reference( ParentChildSiblingInfoData &x )
{
  if (rcData != &x) 
    {
      if (( rcData!=NULL )&&( rcData->decrementReferenceCount() == 0 ))
	delete rcData;
      rcData = &x;
      if ( rcData != NULL ) 
	rcData->incrementReferenceCount();
      //      updateReferences();
    } // end if
}

/*! breaks a reference to the object rcData currently points at and creates
 *  a seperate copy */
void ParentChildSiblingInfo::breakReference()
{
  if(( rcData!=NULL )||( rcData->getReferenceCount()!=1 ))
    {
      ParentChildSiblingInfo x = *this;
      reference(x);
    }
}

void ParentChildSiblingInfo::consistencyCheck() const
{
  ReferenceCounting::consistencyCheck();
  if( rcData==NULL )
    {
      cerr << className << "::consistencyCheck(): rcData == NULL for "
	   << getClassName() << "" << getGlobalID() << "." << endl;
      assert( rcData != NULL );
    }

  rcData->consistencyCheck();
}

Integer ParentChildSiblingInfo::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;  
}

Integer ParentChildSiblingInfo::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;
  returnValue |= rcData->put( db, name );
  return returnValue;
}

/*! this routine is called to build the ParentChildSiblingInfo objects
 *  associated with each MappedGrid in a grid collection.
 \param gc : the GridCollection representing an adaptive grid (input)
 \param listOfPCSInfo : returns the reference counted list of ParentChildSiblingInfo objects
                        associated with each grid ordered by their grid number in gc.
  
 * it is assumed that we have access to the refinement level information
 * and that a call to gc.update(THERefinement) is not necessary
 */
void ParentChildSiblingInfo::buildParentChildSiblingInfoObjects( const GridCollection &gc, 
								 ListOfParentChildSiblingInfo &listOfPCSInfo )
{
  Range all;
  int gridIndex = 0;
  int refinementLevelIndex = 0;
  int numberOfRefinementLevels = gc.numberOfRefinementLevels();

#if 0   // BP : 01/22/2002
  int numberOfGrids = gc.numberOfGrids();
#else
  int numberOfGrids = gc.numberOfComponentGrids();
#endif

  // the next array is used to store information as to whether a grid has a parent grid which is periodic
  intSerialArray periodicFlag(numberOfRefinementLevels, numberOfGrids);
  periodicFlag=0;
  assert( listOfPCSInfo.listLength()==0 ); // initially assume that we have no associated PCSInfo objects at all
  assert( numberOfRefinementLevels > 0 );
  assert( gc.numberOfGrids() > 0 );

  // first initialize the list with PCSInfo objects
  for( gridIndex=0; gridIndex < numberOfGrids; gridIndex++ )
    listOfPCSInfo.addElement();

  assert( listOfPCSInfo.getLength()==numberOfGrids );

  // ------------------------------------------------------------------------------ //
  // create temporary array used for finding grid numbers in gc of a grid at
  // a refinement level

  intSerialArray gridNumbers(numberOfRefinementLevels, numberOfGrids );
  gridNumbers = 0;
  
  // we probably don't need to do the optimization in the next few lines now that
  // I know the gridNumber array exists on each refinement level but leaving it in
  // for now
  for( refinementLevelIndex = 0; 
       refinementLevelIndex < numberOfRefinementLevels; 
       refinementLevelIndex++ )
    {
      GridCollection &gcCurrentLevel = gc.refinementLevel[refinementLevelIndex];
      for( int gcCurrentLevelIndex = 0; 
#if 0   // BP : 01/22/2002, doing this so that no ref. level coarsenings are counted
	   gcCurrentLevelIndex < gcCurrentLevel.numberOfGrids();
#else
	   gcCurrentLevelIndex < gcCurrentLevel.numberOfComponentGrids();
#endif
	   gcCurrentLevelIndex++)
	gridNumbers(refinementLevelIndex, gcCurrentLevelIndex) = gcCurrentLevel.gridNumber(gcCurrentLevelIndex);
    }

  // ------------------------------------------------------------------------------ //

  for( refinementLevelIndex = 0; 
       refinementLevelIndex < numberOfRefinementLevels; 
       refinementLevelIndex++ )
    {
      GridCollection &gcCurrentLevel = gc.refinementLevel[refinementLevelIndex];
#if 0      // BP : 01/22/2002
      int currentLevelNumberOfGrids = gcCurrentLevel.numberOfGrids();
#else
      int currentLevelNumberOfGrids = gcCurrentLevel.numberOfComponentGrids();
#endif
      assert( currentLevelNumberOfGrids > 0 );
      // ------------------------------------------------------------------------------ //
      // first create parent and child info objects looking at adjacent refinement levels

      int indexIntoCurrentLevel = 0;
      for( indexIntoCurrentLevel = 0;
	   indexIntoCurrentLevel < currentLevelNumberOfGrids;
	   indexIntoCurrentLevel++ )
	{
	  MappedGrid &currentGrid = gcCurrentLevel[indexIntoCurrentLevel];
	  int numberOfDimensions = currentGrid.numberOfDimensions(); 
	  // get the index into gc
	  int gcCurrentGridIndex = gridNumbers(refinementLevelIndex, indexIntoCurrentLevel);
	  // get the base grid number from gc
	  int currentGridBase = gc.baseGridNumber(gcCurrentGridIndex);

	  IntegerArray refinementRatio(3);
	  refinementRatio = 1;
	  // if not the finest level
	  if( refinementLevelIndex < numberOfRefinementLevels-1 )
	    {
	      GridCollection &gcNextLevel = gc.refinementLevel[refinementLevelIndex+1];	      
	      int indexIntoNextLevel = 0;	      
	      for( indexIntoNextLevel = 0; 
#if 0    // BP : 01/22/2002
		   indexIntoNextLevel < gcNextLevel.numberOfGrids(); 
#else
		   indexIntoNextLevel < gcNextLevel.numberOfComponentGrids(); 
#endif
		   indexIntoNextLevel++ )
		{		  
		  MappedGrid &nextLevelGrid = gcNextLevel[indexIntoNextLevel];
		  // get the index of the grid in gc
		  int gcNextGridIndex = gridNumbers(refinementLevelIndex+1, indexIntoNextLevel);
		  // find out the base grid number
		  int nextGridBase = gc.baseGridNumber(gcNextGridIndex);
		  // figure out what the refinement ratio is
		  refinementRatio = gc.refinementFactor(all, gcNextGridIndex)/gc.refinementFactor(all, gcCurrentGridIndex);
		  Box intersectionBox;
		  
		  // if grids have the same base grid and intersect then create parent and child
		  if( ( currentGridBase == nextGridBase ) &&
		      ( ParentChildSiblingInfo::gridsOverlap( currentGrid, nextLevelGrid, refinementRatio, intersectionBox ) ) )
		    {
		      listOfPCSInfo[gcCurrentGridIndex].addChild( gcNextGridIndex,
								  intersectionBox );
		      
		      // coarsen the intersection box to get the parent box
		      const IntVect refRatioVect(D_DECL(refinementRatio(0),
							refinementRatio(1),
							refinementRatio(2)));
		      
		      Box coarseIntersectionBox = coarsen( intersectionBox, refRatioVect );
		      assert( coarseIntersectionBox.sameType( intersectionBox ) ); 
		      
		      listOfPCSInfo[gcNextGridIndex].addParent( gcCurrentGridIndex,
								coarseIntersectionBox );
		      // determine if the grid is periodic along any dimension
		      // we try to determine this information in advance so as to
		      // cut out computations for periodic grids which can be expensive
		      bool bGridIsPeriodic = FALSE;
		      for( int i = 0; i<numberOfDimensions; i++)
			bGridIsPeriodic = bGridIsPeriodic || currentGrid.isPeriodic(i);

		      if(bGridIsPeriodic)
			periodicFlag(refinementLevelIndex+1, indexIntoNextLevel) = 1;  // flag that the parent is periodic
		    }
		}
	    }
	  
	  // finished creating parent and child info objects
	  // ------------------------------------------------------------------------------ //
	  if(refinementLevelIndex>0)   //skip for the coarsest level
	    {
	      // check if the base grid is periodic
	      bool bIsPeriodic = FALSE;
	      for( int i=0; i<numberOfDimensions; i++)
		bIsPeriodic = bIsPeriodic || gc[currentGridBase].isPeriodic(i);

	      // now build sibling relationships for this grid
	      int siblingIndex = 0;
	      for( siblingIndex = indexIntoCurrentLevel;
		   siblingIndex < currentLevelNumberOfGrids;
		   siblingIndex++ )
		{
		  MappedGrid &siblingGrid = gcCurrentLevel[siblingIndex];
		  int gcSiblingGridIndex = gridNumbers(refinementLevelIndex, siblingIndex);
		  int siblingGridBase = gc.baseGridNumber(gcSiblingGridIndex);

		  if( currentGridBase == siblingGridBase ) 
		    {
		      // we could do a check to see if both grids have the same number 
		      // of ghost points later on
		      // only need to do this check for distinct grids at the same level
		      if( siblingIndex > indexIntoCurrentLevel ) 
			{
			  Box intersectionBox;

			  if( ParentChildSiblingInfo::isSibling( currentGrid, siblingGrid, intersectionBox ) )
			    listOfPCSInfo[gcCurrentGridIndex].addSibling( gcSiblingGridIndex,
									  intersectionBox,
									  intersectionBox );
			  
			  if( ParentChildSiblingInfo::isSibling( siblingGrid, currentGrid, intersectionBox ) )
			    listOfPCSInfo[gcSiblingGridIndex].addSibling( gcCurrentGridIndex, 
									  intersectionBox,
									  intersectionBox );
			}

		      // if both grids have a periodic parent check if they may still be siblings
		      // note that a grid may be a periodic sibling of itself
		      // and that two grids may be siblings in the usual sense as well as 
		      // periodic siblings
		      if(bIsPeriodic) // if the sibling grids have a common periodic base grid
			{
			  const MappedGrid &baseGrid = gc[currentGridBase];
			  const intSerialArray refinementFactor = gc.refinementFactor(all, gcCurrentGridIndex);
			  listOfPCSInfo[gcCurrentGridIndex].addIfPeriodicSibling(currentGrid, 
										 siblingGrid, 
										 baseGrid, 
										 refinementFactor,
										 gcSiblingGridIndex );
			  
			  listOfPCSInfo[gcSiblingGridIndex].addIfPeriodicSibling(siblingGrid,
										 currentGrid, 
										 baseGrid, 
										 refinementFactor,
										 gcCurrentGridIndex );
			}

		    }
		}
	      // finished creating sibling info objects
	      // ------------------------------------------------------------------------------ //
	    }
	}     
    }
}

/*! creates a new ChildInfo object and adds a pointer to the new object in the ChildList in 
    the letter ( the ParentChildSiblingInfoData object pointed to by rcData ).
    The routine assumes that it has already been established that the MappedGrid with index gridIndex
    is a child grid of the MappedGrid corresponding to this object
    \param gridIndex : index of the child MappedGrid into the adaptive grid (input)
    \param childBox  : the Box object (node centered) representing the index space on the child MappedGrid
                       that lies over the MappedGrid associated with this object (input)
*/
void ParentChildSiblingInfo::addChild( const int gridIndex, const Box &childBox )
{
  assert( childBox.ok() );
  assert( gridIndex >= 0);
  ChildInfo *childInfo = new ChildInfo( gridIndex, childBox );
  if( childInfo==NULL )
    {
      cerr << "ParentChildSiblingInfo::addChild(): failed to allocate space for ChildInfo object" << endl;
      assert( childInfo==NULL );
    }

  assert( this != NULL );
  assert( rcData != NULL );
  rcData->addChild( childInfo );
  // maybe we should consider returning a pointer to the object just created
}

/*! creates a new ParentInfo object and adds a pointer to the new object in the ParentList in 
    the letter ( the ParentChildSiblingInfoData object pointed to by rcData ).
    The routine assumes that it has already been established that the MappedGrid with index gridIndex
    is a parent grid of the MappedGrid corresponding to this object
    \param gridIndex : index of the parent MappedGrid into the adaptive grid (input)
    \param parentBox  : the Box object (node centered) representing the index space on the parent MappedGrid
                       that lies under the MappedGrid associated with this object (input)
*/
void ParentChildSiblingInfo::addParent( const int gridIndex, const Box &parentBox )
{
  assert( parentBox.ok() );
  assert( gridIndex >= 0);
  ParentInfo *parentInfo = new ParentInfo( gridIndex, parentBox );
  if( parentInfo==NULL )
    {
      cerr << "ParentChildSiblingInfo::addParent(): failed to allocate space for ParentInfo object" << endl;
      assert( parentInfo==NULL );
    }

  assert( this != NULL );
  assert( rcData != NULL );
  rcData->addParent( parentInfo );
  // maybe we should consider returning a pointer to the object just created
}

/*! creates a new SiblingInfo object and adds a pointer to the new object in the SiblingList in 
    the letter ( the ParentChildSiblingInfoData object pointed to by rcData ).
    The routine assumes that it has already been established that the MappedGrid with index gridIndex
    is a sibling grid of the MappedGrid corresponding to this object
    \param gridIndex : index of the sibling MappedGrid into the adaptive grid (input)
    \param ghostBox : the Box object (node centered) representing the index space of the ghost region of the MappedGrid associated 
    with this object that overlaps the sibling MappedGrid (input)
    \param siblingBox  : the Box object (node centered) representing the index space on the sibling MappedGrid
    that overlaps the ghost region of the MappedGrid associated with this object (input)
*/
void ParentChildSiblingInfo::addSibling( const int siblingGridIndex,
					 const Box &ghostBox,
					 const Box &siblingBox )
{
  SiblingInfo *siblingInfo = new SiblingInfo( siblingGridIndex, ghostBox, siblingBox );
  
  if( siblingInfo==NULL )
    {
      cerr << "ParentChildSiblingInfo::addSibling(): failed to allocate space for SiblingInfo object" << endl;
      assert( siblingInfo==NULL );
    }
  
  assert( this != NULL );
  assert( rcData != NULL );
  rcData->addSibling( siblingInfo );
  // maybe we should consider returning a pointer to the object just created
}

/*! This routine assumes that grids at different refinement levels are provided
    and determines whether they overlap spatially or not. A boolean value of TRUE is
    returned if the grids overlap spatially
    \param parentGrid : grid at coarser refinement level
    \param childGrid  : grid at finer refinement level
    \param refinementRatio : refinementRatio(i), i = 1,2,3 determines the factor of
    refinement in each direction to move from the coarser to the finer refinement level
    \param intersectionBox : returned reference to a node centered Box that contains the intersection in
    fine grid index space if the grids intersect. It is not initialized if there is no intersection
*/
bool ParentChildSiblingInfo::gridsOverlap( const MappedGrid &parentGrid,
					   const MappedGrid &childGrid,
					   const IntegerArray &refinementRatio,
					   Box   &intersectionBox )
{
  bool returnVal = TRUE;

  const int numberOfDimensions = parentGrid.numberOfDimensions();
  const Box &parentBox = parentGrid.box();
  const Box &childBox  = childGrid.box();
  int i;
  // make sure both grids are of the same type
  assert( parentBox.sameType( childBox ) );
  for( i = 0; i < numberOfDimensions; i++ )
    assert( refinementRatio(i)>=1 );

  const IntVect refRatioVect(D_DECL(refinementRatio(0),
				    refinementRatio(1),
				    refinementRatio(2)));

  Box refinedParentBox = refine( parentBox, refRatioVect );
  assert( refinedParentBox.sameType( parentBox ) ); 

  IntVect low( refinedParentBox.smallEnd() );
  IntVect hi( refinedParentBox.bigEnd() );
  low.max( childBox.smallEnd() );
  hi.min( childBox.bigEnd() ) ;

  // lower dimension intersections are not considered as
  // overlaps. this is based on the assumption that
  // every grid ( at a level finer than base ) has a parent
  // at the next coarser level. if a lower dimension
  // intersection takes place it implies that the
  // boundaries of the grids are aligning. but the
  // child grid still lies over another parent
  for( i = 0; i < numberOfDimensions; i++ )
    returnVal = returnVal && (low[i]<hi[i]);
  
  IndexType iType = IndexType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
  if(returnVal)   // initialize the box
    intersectionBox = Box( low, hi, iType );

  return returnVal;
}

/*!
   DEFINITION: Grid B is considered to be a sibling to Grid A
   if Grid A extended by its ghost boundary intersects grid B. If Grid A and B 
   have different ghost boundary widths then Grid B a sibling of Grid A does NOT
   imply Grid A a sibling of Grid B.
   This routine uses the above definition to determine whether siblingGrid
   can be considered to be a sibling of currentGrid and returns a boolean flag
   and an intersection box by reference.
   \param currentGrid : grid whose siblings have to be determined (input)
   \param siblingGrid : possible sibling grid to currentGrid (input)
   \param intersectionBox : node centered box representing the intersection in index space (output)
*/
bool ParentChildSiblingInfo::isSibling( const MappedGrid &currentGrid, 
					const MappedGrid &siblingGrid,
					Box &intersectionBox )
{
  const int numberOfDimensions = currentGrid.numberOfDimensions();
  const IntegerArray &ghostWidth = currentGrid.numberOfGhostPoints();
  const Box &currentBox = currentGrid.box();
  const Box &siblingBox = siblingGrid.box();
  assert( currentBox.sameType( siblingBox ) );

  return isSibling( currentBox, siblingBox, ghostWidth, numberOfDimensions, intersectionBox );
}

/*! 
  This routine is used internally to determine whether two MappedGrids are siblings using their Box objects.
  It returns a boolean value.
  \param currentBox : node centered Box object representing a grid whose siblings are to be determined (input)
  \param siblingBox : node centered Box object representing a grid which is a potential sibling (input)
  \param ghostWidth : ghost boundary width of currentBox (input)
  \param numberOfDimensions : number of valid dimensions (input)
  \param intersectionBox : node centered Box representing the intersection in index space if it exists (output)
 */
bool ParentChildSiblingInfo::isSibling( const Box &currentBox, 
					const Box &siblingBox, 
					const IntegerArray &ghostWidth,
					const int numberOfDimensions,
					Box &intersectionBox )
{
  bool returnVal = TRUE;

  Box extendedBox = currentBox;
  assert( currentBox.sameType( extendedBox ) ); 
  int i;
  for( i = 0; i < numberOfDimensions; i++ )
    {
      extendedBox.growLo(i, ghostWidth(0, i) );
      extendedBox.growHi(i, ghostWidth(1, i) );
    }

  IntVect low( extendedBox.smallEnd() );
  IntVect hi( extendedBox.bigEnd() );
  low.max( siblingBox.smallEnd() );
  hi.min( siblingBox.bigEnd() ); 

  for( i = 0; i < numberOfDimensions; i++ )
    returnVal = returnVal && (low[i]<=hi[i] );

  
  IndexType iType = IndexType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
  if(returnVal)
    intersectionBox = Box(low, hi, iType);

  return returnVal;
}


/*!
   This routine assumes that currentGrid and siblingGrid have a
   periodic base grid given by baseGrid.  

   Note 0. We say grid A is a "periodic sibling" of grid B if they do not
   overlap in index space but on translating by a fixed period overlap
   Note 1. We need to consider the base grid since two grids can have non
   periodic parents but can align along a branch cut of the base grid.
   Note 2. The case of a grid being a periodic sibling of itself is taken care of. 
   Note 3. If a grid is periodic in more than one direction and has a periodic
   sibling which intersects it in both directions a SiblingInfo object
   will be created for each direction of intersection

   \param currentGrid      : grid whose "periodic siblings" have to be determined (input)
   \param siblingGrid      : potential periodic sibling of currentGrid (input)
   \param baseGrid         : periodic common base grid to both currentGrid & siblingGrid (input)
   \param refinementFactor : refinementFactor(i), i=1,2, 3 gives the refinement factor along (input)
                             axes 0, 1, 2 of grids currentGrid & siblingGrid wrt baseGrid (input)
   \param siblingGridIndex : index into the adaptive grid collection of siblingGrid (input)
*/

bool ParentChildSiblingInfo::addIfPeriodicSibling( const MappedGrid &currentGrid, 
						   const MappedGrid &siblingGrid, 
						   const MappedGrid &baseGrid, 
						   const IntegerArray &refinementFactor, 
						   const int siblingGridIndex) 
{ 
  bool returnVal = FALSE;

  const int numberOfDimensions = currentGrid.numberOfDimensions();
  const IntegerArray &parentPeriodic = baseGrid.isPeriodic();
  const IntegerArray &ghostWidth = currentGrid.numberOfGhostPoints();
  const Box &currentBox = currentGrid.box();
  const Box &siblingBox = siblingGrid.box();

  for( int i=0; i<numberOfDimensions; i++ )
    {
      int shiftLen = 0;
      Box intersectionBox;
      if( parentPeriodic(i) ) // if the parent is periodic along this axis
	{
	  assert( baseGrid.gridIndexRange(0, i)==0 ); // if periodic the start index should be zero??
	  shiftLen = baseGrid.gridIndexRange(1, i)*refinementFactor(i);
	  if( currentBox.smallEnd(i) <= siblingBox.smallEnd(i) ) // currentBox lies to the left of siblingBox
	    shiftLen = -shiftLen;
	  Box shiftedSiblingBox = siblingBox;  // shift the sibling box
	  shiftedSiblingBox.shift( i, shiftLen );

	  if(isSibling( currentBox, (const Box &) shiftedSiblingBox, ghostWidth, numberOfDimensions, intersectionBox))
	    {
	      const Box ghostBox = intersectionBox;
	      const Box finalSiblingBox = intersectionBox.shift(i, -shiftLen );
	      addSibling( siblingGridIndex, ghostBox, finalSiblingBox );
	      returnVal = TRUE;
	    }
	}
    }

  return returnVal;
}

ostream& operator<<(ostream& s, const ParentChildSiblingInfo &pcsInfo)
{

  // looks like the copy constructor for the STL
  // list is being called, so we create a reference
  // to the list and then work with that
  s << "sizeof(ParentChildSiblingInfo) = " << sizeof(pcsInfo) << endl;
  s << "className = " << pcsInfo.className << endl;
  const list<ParentInfo*> &parentList = pcsInfo.getParents();
  s << "size of parent list: " << parentList.size() << endl;
  list<ParentInfo*>::const_iterator parentIterator;
  for( parentIterator = parentList.begin();
       parentIterator != parentList.end();
       parentIterator++ )
    {
      assert((*parentIterator)!=NULL );
      s << *(*parentIterator) << endl;
    }

  const list<ChildInfo*> &childList = pcsInfo.getChildren();
  s << "size of child list: " << childList.size() << endl;
  list<ChildInfo*>::const_iterator childIterator;

  for( childIterator =  childList.begin();
       childIterator != childList.end();
       childIterator++ )
    {
      assert((*childIterator)!=NULL );
      s << *(*childIterator) << endl;
    }

  const list<SiblingInfo*> &siblingList = pcsInfo.getSiblings();
  s << "size of sibling list: " << siblingList.size() << endl;
  list<SiblingInfo*>::const_iterator siblingIterator;
  for( siblingIterator = siblingList.begin();
       siblingIterator != siblingList.end();
       siblingIterator++ )
    {
      assert((*siblingIterator)!=NULL );
      s << *(*siblingIterator) << endl;
    }

  return s;
}

/*! 
  mainly internal use
 */
bool ParentChildSiblingInfo::isValidBoxForType( const Box &box, const IndexType &iType )
{
  bool retVal = (box.ixType()==iType);
  const IntVect &low = box.smallEnd();
  const IntVect &hi = box.bigEnd();
  for( int i=0; i<3; i++)
    retVal = retVal && (iType.ixType(i)==IndexType::CELL?(low[i]<hi[i]):(low[i]<=hi[i]));
  return retVal;
}

/*!
  returns the number of parent boxes and also their indices and intersection boxes by reference
  \param gridIndices : list of indices of parent MappedGrids in the adaptive GridCollection (output)
  \param parentBoxes : list of Boxes representing index space on parents overlapped by MappedGrid corresponding to this object (output)
  \param iType : desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)
  \param problemDimension : number of valid dimensions (input)

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions
 */
int ParentChildSiblingInfo::getParentBoxes( intSerialArray &gridIndices, 
					    BoxList &parentBoxes,
					    const IndexType &iType,
					    const int problemDimension )
{
  gridIndices.redim(0);
  parentBoxes.clear();

  const list<ParentInfo *> &ParentList = getParents();
  int numberOfBoxes = ParentList.size();   // will be O(N) when using STLport
  int i = 0;
  
  if( numberOfBoxes > 0 )
    {
      IndexType convType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
      for(int iDir = 0; iDir < problemDimension; iDir++)
	convType.setType(iDir, iType.ixType(iDir) );

      gridIndices.redim( numberOfBoxes );  // create a serial array of the correct size
      gridIndices = 0;
      parentBoxes.convert(convType);  // *wdh* 010124
      list<ParentInfo *>::const_iterator parentIterator;
      for( parentIterator = ParentList.begin(); parentIterator != ParentList.end(); parentIterator++)
	{
	  assert( (*parentIterator) != NULL );
	  Box parentBox = (*parentIterator)->getParentBox();
	  parentBox.convert( convType );

	  if(isValidBoxForType(parentBox, convType ))
	    {
	      parentBoxes.append( parentBox );
	      gridIndices(i++) = (*parentIterator)->getGridIndex();
	    }
	}

      gridIndices.resize(i);

    }
  
  return i;
}

/*!
  returns the number of child boxes and also their indices and intersection boxes by reference
  \param gridIndices : list of indices of child MappedGrids in the adaptive GridCollection (output)
  \param childBoxes : list of Boxes representing index space on children overlapped by MappedGrid corresponding to this object (output)
  \param iType : desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)
  \param problemDimension : number of valid dimensions (input)

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions
 */
int ParentChildSiblingInfo::getChildBoxes( intSerialArray &gridIndices, 
					   BoxList &childBoxes,
					   const IndexType &iType,
					   const int problemDimension)
{
  const list<ChildInfo *> &ChildList = getChildren();
  int numberOfBoxes = ChildList.size();   // will be O(N) when using STLport
  int i = 0;
  
  if( numberOfBoxes > 0 )
    {
      IndexType convType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
      for(int iDir = 0; iDir < problemDimension; iDir++)
	convType.setType(iDir, iType.ixType(iDir) );

      gridIndices.redim( numberOfBoxes );  // create a serial array of the correct size
      gridIndices = 0;
      list<ChildInfo *>::const_iterator childIterator;
      for( childIterator = ChildList.begin(); childIterator != ChildList.end(); childIterator++)
	{
	  assert( (*childIterator) != NULL );
	  Box childBox = (*childIterator)->getChildBox();
	  childBox.convert( convType );

	  if(isValidBoxForType(childBox, convType ))
	    {
	      childBoxes.append( childBox );
	      gridIndices(i++) = (*childIterator)->getGridIndex();
	    }
	}
      gridIndices.resize(i);
    }
  
  return i;
}

/*!
  returns the number of sibling boxes and also their indices and intersection boxes by reference
  \param gridIndices : list of indices of sibling MappedGrids in the adaptive GridCollection (output)
  \param siblingBoxes : list of Boxes representing index space on sibling overlapped by MappedGrid corresponding to this object (output)
  \param ghostBoxesOnCurrentGrid : list of Boxes representing index space in ghost region of current grid that corresponds to siblingBoxes (output)
  \param iType : desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)
  \param problemDimension : number of valid dimensions (input)

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions
 */

int ParentChildSiblingInfo::getSiblingBoxes( intSerialArray &gridIndices, 
					     BoxList &siblingBoxes,
					     BoxList &ghostBoxesOnCurrentGrid,
					     const IndexType &iType,
					     const int problemDimension )
{
  const list<SiblingInfo *> &SiblingList = getSiblings();
  int numberOfBoxes = SiblingList.size();   // will be O(N) when using STLport
  int i = 0;
  
  if( numberOfBoxes > 0 )
    {
      IndexType convType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
      for(int iDir = 0; iDir < problemDimension; iDir++)
	convType.setType(iDir, iType.ixType(iDir) );

      gridIndices.redim( numberOfBoxes );  // create a serial array of the correct size
      gridIndices = 0;
      list<SiblingInfo *>::const_iterator siblingIterator;
      for( siblingIterator = SiblingList.begin(); siblingIterator != SiblingList.end(); siblingIterator++)
	{
	  assert( (*siblingIterator) != NULL );
	  Box siblingBox = (*siblingIterator)->getSiblingBox();
	  siblingBox.convert( convType );
	  if( isValidBoxForType( siblingBox, convType ) )
	    {
	      siblingBoxes.append( siblingBox );
	      Box ghostBox = (*siblingIterator)->getGhostBox();
	      ghostBox.convert(convType);
	      assert( ghostBox.sameType( siblingBox ) );
	      ghostBoxesOnCurrentGrid.append( ghostBox );
	      gridIndices(i++) = (*siblingIterator)->getGridIndex();
	    }
	}

      gridIndices.resize(i);
    }
  return i;
}

/*!
  returns a list of boxes representing the ghost region of mg lying between ghostLines
  \param mg: MappedGrid whose ghost region is to be determined (input)
  \param ghostLines: Range object base = 0 corresponds to boundary, 1 to the first ghost line and so on
                     bound >= base represents last ghost line to consider (input)
  \param iType: desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)
  \param ghostRegionBoxes : list of boxes representing index space of ghost region of mg lying between ghostLines

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions
*/
void ParentChildSiblingInfo::getGhostRegionBoxes( const MappedGrid &mg,
						  const Range &ghostLines,
						  const IndexType &iType,
						  BoxList &ghostRegionBoxes,
                                                  bool excludePhysicalBoundaries /* = false */ )
{
  int iDir = 0;
  const int problemDimension = mg.numberOfDimensions();
  const int firstGhostLine = ghostLines.getBase();        
  const int lastGhostLine  = ghostLines.getBound();
  assert( (firstGhostLine>=0)&&(firstGhostLine<=lastGhostLine));
  Box gridBox = mg.box();
  // only convert the valid dimensions, else there can be quite unpredictable results
  for( iDir = 0; iDir < problemDimension; iDir++) 
    gridBox.convert(iDir, iType.ixType(iDir));

  // form the type to convert to
  const IndexType &convType = gridBox.ixType(); 
  // grow in specific dirs, an unfortunate consequence of setting Boxlib dimension to 3

  // If excludePhysicalBoundaries is true we also want to exclude regions next to physical boundaries
  // so we grow the inner box 
  //          +--+------------+
  //          |  |            |
  //          |  |            |
  //          |  |bc>0        |
  //          |  |            |
  //          |  |     bc>0   |
  //          |  +------------+
  //          |               |
  //          +---------------+

  Box innerComplementBox = gridBox;
  Box outerContainerBox  = gridBox;
  for( iDir = 0; iDir < problemDimension; iDir++)
  {
    if( excludePhysicalBoundaries )
    {
      if( mg.boundaryCondition(0,iDir)>0 )
        innerComplementBox.growLo(iDir, lastGhostLine);
      else
        innerComplementBox.growLo(iDir, firstGhostLine-1);

      if( mg.boundaryCondition(1,iDir)>0 )
        innerComplementBox.growHi(iDir, lastGhostLine);
      else
        innerComplementBox.growHi(iDir, firstGhostLine-1);
    }
    else
      innerComplementBox.grow(iDir, firstGhostLine-1);

    outerContainerBox.grow(iDir, lastGhostLine);
  }

  assert( outerContainerBox.contains(innerComplementBox) );
  assert( outerContainerBox.sameType(innerComplementBox) );
  assert( outerContainerBox.sameType(gridBox) );
  ghostRegionBoxes = boxDiff( outerContainerBox, innerComplementBox );
}

/*!  Given a range of bounding ghost lines (which can include the
  boundary) and a desired centering along each axis this routine
  returns a list of boxes that represent the index space on sibling
  MappedGrids that lie over the specified ghost region of the current
  grid indexed by currentGridIndex, a list of boxes representing the
  index space on the current grid lying under the sibling boxes, and
  an intSerialArray of grid indices of the respective siblings in gc.
  An integer return value specifies the number of boxes if any.

  \param gridIndices: list of indices of sibling MappedGrids in the adaptive GridCollection (output)
  \param siblingBoxes: list of Boxes representing index space on sibling overlapped by ghost region
  of MappedGrid corresponding to this object that lies between the ghost lines specified by the Range object ghostLines(output)
  \param ghostBoxesOnCurrentGrid list of Boxes representing index space in ghost region of current grid that corresponds to siblingBoxes (output)
  \param ghostLines: Range of ghost lines, 0 corresponding to the boundary, 1 being the first ghost line etc (input)
  \param gc : adaptive GridCollection(input)
  \param currentGridIndex: grid number of the current grid into gc (input)
  \param iType : desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions */
int ParentChildSiblingInfo::getSiblingGhostBoxes( intSerialArray &gridIndices, 
						  BoxList &siblingBoxes,
						  BoxList &ghostBoxesOnCurrentGrid,
						  const Range &ghostLines,
						  const GridCollection &gc,
						  const int currentGridIndex,
						  const IndexType &iType)
{
  gridIndices.redim(0);             // *wdh* 011024
  siblingBoxes.clear();
  ghostBoxesOnCurrentGrid.clear();

  int iDir = 0, i = 0;
  const list<SiblingInfo *> &SiblingList = getSiblings();
  if(!SiblingList.empty())
    {
      const int problemDimension = gc.numberOfDimensions();
      int numberOfBoxes = 17*SiblingList.size();  // max number of boxes

      IndexType convType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
      // only convert the valid dimensions, else there can be quite unpredictable results
      for( iDir = 0; iDir < problemDimension; iDir++) 
	convType.setType(iDir, iType.ixType(iDir) );

      siblingBoxes.convert(convType);
      ghostBoxesOnCurrentGrid.convert(convType);
      assert(siblingBoxes.length() ==0);
      assert(ghostBoxesOnCurrentGrid.length() ==0);

      gridIndices.redim(numberOfBoxes); gridIndices = 0;  // max number of siblings (it may be less)
  
      // create the list of ghost boxes that represent the index space around the given MappedGrid
      BoxList ghostRegionBoxes( convType );
      getGhostRegionBoxes( gc[currentGridIndex], ghostLines, iType, ghostRegionBoxes ); 

      list<SiblingInfo *>::const_iterator siblingIterator;
      for( siblingIterator = SiblingList.begin(); siblingIterator != SiblingList.end(); siblingIterator++)
	{
	  assert((*siblingIterator)!=NULL);
	  Box siblingBox = (*siblingIterator)->getSiblingBox();
	  siblingBox.convert(convType);  // convert it to the desired type
	  if(ParentChildSiblingInfo::isValidBoxForType(siblingBox, convType ))
	    {
	      Box ghostBox   = (*siblingIterator)->getGhostBox();
	      ghostBox.convert(convType);  // convert it to the desired type
	      assert(siblingBox.sameType(ghostBox));
	      assert(siblingBox.sameSize(ghostBox)); // make sure that they are just translates
	      bool bNotPeriodic = (siblingBox==ghostBox);

	      IntVect gLo, sLo;
	      if(!bNotPeriodic)
		{
		  gLo = ghostBox.smallEnd();
		  sLo = siblingBox.smallEnd();
		  gLo -= sLo;
		  for(iDir = 0; iDir<3; iDir++)
		    if( gLo[iDir] != 0 ) break;
		}

	      // generate a boxList that contains ghost points 
	      BoxList tmpList = intersect( ghostRegionBoxes, ghostBox );
	      tmpList.simplify();        // try to merge as many boxes as possible, may be expensive and unnecessary
	      if( tmpList.isNotEmpty())  // if we have a non empty intersection
		{
		  BoxListIterator bli(tmpList);
		  for(; bli; bli++)
		    {
		      Box fragmentBox = *bli;  // the different fragment boxes that used to be part of ghostBox :-)
		      assert( fragmentBox.sameType(*bli));
		      // add the boxes in the boxList to listOfGhostBoxes with the same sibling index
		      ghostBoxesOnCurrentGrid.append( fragmentBox );
		      // add the boxes to listOfSiblingBoxes if the grid is not periodic
		      if(bNotPeriodic)
			siblingBoxes.append( fragmentBox );
		      else  // we have a headache!, shift the box and append it
			{
			  fragmentBox.shift( iDir, -gLo[iDir] );
			  assert(siblingBox.contains( fragmentBox ));	      
			  siblingBoxes.append( fragmentBox );		  
			}
		      gridIndices(i++) = (*siblingIterator)->getGridIndex();
		    }
		}
	    }
	}

      assert(siblingBoxes.length()==ghostBoxesOnCurrentGrid.length()); 
      assert(siblingBoxes.length()==i);
      // resize the array of indices to the correct length preserving data
      gridIndices.resize( i );
    }
  return i;

}

/*!  Given a range of bounding ghost lines (which can include the
  boundary) and a desired centering along each axis this routine
  returns a list of boxes that represent the index space on parent
  MappedGrids that lie under the specified ghost region of the current
  grid indexed by currentGridIndex, and an intSerialArray of grid
  indices of the respective parents in gc. The boxes are returned in
  fine grid index space.  A flag controls whether ghost points that
  lie under sibling MappedGrids should be included or not.
  An integer return value specifies the number of boxes if any.

  \param gridIndices : list of indices of sibling MappedGrids in the adaptive GridCollection (output)
  \param parentGhostBoxes : list of Boxes representing index space on parents that lies under 
  the ghost region specified by the Range ghostLines (output). The boxes are specified in the 
  index space of the current grid and NOT that of the parents. i.e. they are fine grid boxes and NOT coarse boxes
  \param ghostLines : Range of ghost lines, 0 corresponding to the boundary, 1 being the first ghost line etc(input)
  \param gc : adaptive GridCollection (input)
  \param currentGridIndex : grid number of the current grid into gc (input)
  \param iType : desired index type along each direction, only valid dimensions are considered, rest assumed to be NODE centered (input)
  \param excludeSiblingPoints : whether ghost points that lie under sibling grids should be excluded. default is FALSE (input)

  NOTE: The boxes returned in the box lists have the index type specified by iType along 
  valid dimensions and are NODE centered for higher dimensions */
int ParentChildSiblingInfo::getParentGhostBoxes( intSerialArray &gridIndices, 
						 BoxList &parentGhostBoxes,
						 const Range &ghostLines,
						 const GridCollection &gc,
						 const int currentGridIndex,
						 const IndexType &iType,
						 const bool excludeSiblingPoints )
{
  gridIndices.redim(0);     // *wdh*
  parentGhostBoxes.clear(); // *wdh*

  int iDir = 0, i = 0;
  const list<ParentInfo *> &ParentList = getParents();
  if( !ParentList.empty() )
    {
      const int problemDimension = gc.numberOfDimensions();
      int numberOfBoxes = 26*ParentList.size();  // we multiply by 26 to account for max number of boxes
      // the number 26 comes by considering a 3D box totally nested in a parent and summing the number of
      // edges, corners, faces ..
      gridIndices.redim( numberOfBoxes ); gridIndices = 0;

      IndexType convType = IndexType(D_DECL(IndexType::NODE, IndexType::NODE, IndexType::NODE));
      // only convert the valid dimensions, else there can be quite unpredictable results
      for( iDir = 0; iDir < problemDimension; iDir++) 
	convType.setType(iDir, iType.ixType(iDir) );

      parentGhostBoxes.convert(convType);
      assert( parentGhostBoxes.ixType()==convType );
      assert(parentGhostBoxes.length()==0);

      BoxList ghostRegionBoxes(convType);
      BoxList finalGhostRegionBoxes(convType);

      // get the list of ghost boxes that surround the MappedGrid
      bool excludePhysicalBoundaries=true;
      getGhostRegionBoxes( gc[currentGridIndex], ghostLines, convType, ghostRegionBoxes, excludePhysicalBoundaries);

      // if we want to exclude the sibling ghost points, modify the BoxList to do so
      if(excludeSiblingPoints && ghostRegionBoxes.isNotEmpty() )
      {
	intSerialArray siblingGridIndices;	  
	BoxList siblingBoxes(convType), siblingGhostBoxes(convType);
	getSiblingGhostBoxes( siblingGridIndices, siblingBoxes, siblingGhostBoxes, ghostLines, gc, 
                              currentGridIndex, convType );

	siblingGhostBoxes.simplify();


	if( siblingGhostBoxes.isNotEmpty() )
	{
          if( true )
	  {
            // we really want finalGhostRegionBoxes = ghostRegionBoxes - siblingGhostBoxes;

	    //*wdh* (1) take the complement of sibling boxes wrt a large box (use boxList.minimalBox() )
	    //      (2) Intersect  ghostRegionBoxes with complement.        
	    Box parentBoundingBox = ghostRegionBoxes.minimalBox();
	    Box siblingBoundingBox = siblingGhostBoxes.minimalBox();
	    Box boundingBox=minBox(	parentBoundingBox,siblingBoundingBox );
	    BoxList siblingComplement = complementIn( boundingBox, siblingGhostBoxes);

            // cout << "ghostRegionBoxes=" << ghostRegionBoxes << endl;
	    // cout << "boundingBox=" << boundingBox << endl;
	    // cout << "siblingGhostBoxes = " << siblingGhostBoxes << endl;
	    // cout << "siblingComplement = " << siblingComplement << endl;

	    for(BoxListIterator blj( siblingComplement ); blj; blj++)
	    {
	      finalGhostRegionBoxes.join( intersect(ghostRegionBoxes, *blj ) );  
	    }

	    // cout << " finalGhostRegionBoxes= " << finalGhostRegionBoxes << endl;
	    
	  }
	  else
	  {
	    BoxListIterator bli( ghostRegionBoxes );
	    for(; bli; bli++)
	    {
	      BoxListIterator blj( siblingGhostBoxes );	      
	      for( ;blj; blj++)
	      {
		// cout << "Exclude: bli= " << *bli << " blj= " << *blj << endl;
		// *wdh* this is not right when there are multiple siblings -- bli may intersect one but not the
		//   the other in which case the result is all of bli!
		//  --> should subtract intersection from bli
		finalGhostRegionBoxes.join( boxDiff( *bli, *blj ) );  // wdh boxDif(b1,b2) = b1-b2
	      }
	    }
	  }
	}
        else
	{
	  finalGhostRegionBoxes = ghostRegionBoxes;   // *wdh* 011024
	}
      }
      else
	finalGhostRegionBoxes = ghostRegionBoxes;   // else do a copy

      finalGhostRegionBoxes.simplify(); // try to minimize the number of boxes

      if( finalGhostRegionBoxes.isNotEmpty())
	  {
	    Range all;
	    list<ParentInfo *>::const_iterator parentIterator = ParentList.begin();
	    assert( (*parentIterator) != NULL );
	    // the refinement ratio will be the same for all the grids so compute once
	    const intSerialArray refinementRatio = \
	      gc.refinementFactor(all, currentGridIndex)/gc.refinementFactor(all, (*parentIterator)->getGridIndex());
	    assert( refinementRatio(0) && refinementRatio(1) && refinementRatio(2) );
	    const IntVect refRatioVect(D_DECL(refinementRatio(0),
					      refinementRatio(1),
					      refinementRatio(2)));
	    
	    for( parentIterator = ParentList.begin(); parentIterator != ParentList.end(); parentIterator++)
	      {
		assert( (*parentIterator) != NULL );
		const int parentGridIndex = (*parentIterator)->getGridIndex();
		assert( parentGridIndex >=0 );
		Box parentGridBox = gc[parentGridIndex].box();
		parentGridBox.convert(convType);  // convert to the type of centering specified

		// *wdh* 011026 grow the parent box so we can interpolate from ghost points if needed.
                const int lastGhostLine  = ghostLines.getBound();
		for( iDir = 0; iDir < problemDimension; iDir++) 
		  parentGridBox.grow(iDir,lastGhostLine);


		if(ParentChildSiblingInfo::isValidBoxForType( parentGridBox, convType ))
		  {
		    // refine the parent box
		    const Box refinedParentBox = refine( parentGridBox, refRatioVect );
		    BoxList tmpList = intersect( finalGhostRegionBoxes, refinedParentBox );
		    tmpList.simplify();  // may be unnecessarily expensive
		    if(tmpList.isNotEmpty())
		      {
			BoxListIterator bli(tmpList);
			while(bli)
			  {
			    gridIndices(i++) = parentGridIndex;
			    parentGhostBoxes.append(*bli++);
			  }
		      }
		  }
	      }
	  }

      gridIndices.resize( i );
      assert( i==parentGhostBoxes.length() );
    }

  return i;
}

//
// class ParentChildSiblingInfoData:
//
ParentChildSiblingInfoData::ParentChildSiblingInfoData(): ReferenceCounting()
{
  className = "ParentChildSiblingInfoData";
}

ParentChildSiblingInfoData::ParentChildSiblingInfoData(const ParentChildSiblingInfoData &x,
						       const CopyType ct): ReferenceCounting()
{
  className = "ParentChildSiblingInfoData";
  if( ct != NOCOPY ) *this=x;  // operator=() called here
}

ParentChildSiblingInfoData::~ParentChildSiblingInfoData() 
{
  list<ParentInfo *>:: iterator parentIterator;
  for( parentIterator  = ParentList.begin();
       parentIterator != ParentList.end();
       parentIterator++ )
    {
#if 0
      if((*parentIterator!=NULL)&&((*parentIterator)->decrementReferenceCount()==0))
#else  // currently not reference counted, will use this code when it is
	if(*parentIterator!=NULL)
#endif
	  delete *parentIterator;
    }

  ParentList.clear();

  list<ChildInfo *>:: iterator childIterator;
  for( childIterator  = ChildList.begin();
       childIterator != ChildList.end();
       childIterator++ )
    {
#if 0
      if((*childIterator!=NULL)&&((*childIterator)->decrementReferenceCount()==0))
#else  // currently not reference counted, will use this code when it is
	if(*childIterator!=NULL)
#endif
	  // printf("~ParentChildSiblingInfoData: delete childIterator=%l\n",&(*childIterator));
      
	  delete *childIterator;
    }

  ChildList.clear();

  list<SiblingInfo *>:: iterator siblingIterator;
  for( siblingIterator  = SiblingList.begin();
       siblingIterator != SiblingList.end();
       siblingIterator++ )
    {
#if 0  // currently not reference counted, will use this code when it is
      if((*siblingIterator!=NULL)&&((*siblingIterator)->decrementReferenceCount()==0))
#else
	if(*siblingIterator!=NULL)
#endif
	  delete *siblingIterator;
    }

  SiblingList.clear();  
}

ParentChildSiblingInfoData &ParentChildSiblingInfoData::operator=( const ParentChildSiblingInfoData &x )
{
  ReferenceCounting::operator=(x);

  list<ParentInfo *>::const_iterator parentIterator;
  const list<ParentInfo *> &parentList = x.getParents();
  for( parentIterator  = parentList.begin();
       parentIterator != parentList.end();
       parentIterator++ )
    {
      assert( (*parentIterator)!=NULL );
      ParentList.push_back((*parentIterator));
    }
  
  list<ChildInfo *>:: const_iterator childIterator;
  const list<ChildInfo *> &childList = x.getChildren();
  for( childIterator  = childList.begin();
       childIterator != childList.end();
       childIterator++ )
    {
      assert( (*childIterator)!=NULL );
      ChildList.push_back((*childIterator));
    }
  
  list<SiblingInfo *>:: const_iterator siblingIterator;
  const list<SiblingInfo *> &siblingList = x.getSiblings();  
  for( siblingIterator  = siblingList.begin();
       siblingIterator != siblingList.end();
       siblingIterator++ )
    {
      assert( (*siblingIterator)!=NULL );
      SiblingList.push_back((*siblingIterator));
    }
  
  assert( ParentList  == x.getParents() );
  assert( ChildList   == x.getChildren() );
  assert( SiblingList == x.getSiblings() );
  return *this;
}

void ParentChildSiblingInfoData::reference( const ParentChildSiblingInfoData &x )
{
  cerr << "ParentChildSiblingInfoData::reference( const ParentChildSiblingInfoData &) was called!" << endl;
  ReferenceCounting::reference(x);
}

void ParentChildSiblingInfoData::breakReference( )
{
  cerr << "ParentChildSiblingInfoData::breakReference( ) was called!" << endl;
  ReferenceCounting::breakReference();
}

void ParentChildSiblingInfoData::consistencyCheck() const
{
  ReferenceCounting::consistencyCheck();

  // call consistencyCheck() for parents
  list<ParentInfo *>:: const_iterator parentIterator;

  for( parentIterator  = ParentList.begin();
       parentIterator != ParentList.end();
       parentIterator++ )
    {
      assert( (*parentIterator)!=NULL );
      (*parentIterator)->consistencyCheck();
    }

  // call consistencyCheck() for siblings
  list<ChildInfo *>:: const_iterator childIterator;
  for( childIterator  = ChildList.begin();
       childIterator != ChildList.end();
       childIterator++ )
    {
      assert( (*childIterator)!=NULL );
      (*childIterator)->consistencyCheck();
    }

  // call consistencyCheck() for siblings
  list<SiblingInfo *>:: const_iterator siblingIterator;
  for( siblingIterator  = SiblingList.begin();
       siblingIterator != SiblingList.end();
       siblingIterator++ )
    {
      assert( (*siblingIterator)!=NULL );
      (*siblingIterator)->consistencyCheck();
    }
}

Integer ParentChildSiblingInfoData::get( const GenericDataBase & db, const aString &name )
{
  Integer returnValue = 0;
  cerr << className << "::get( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

Integer ParentChildSiblingInfoData::put( const GenericDataBase & db, const aString &name ) const
{
  Integer returnValue = 0;

#if 0  // i'm not ready to test this code as yet
  GenericDataBase& dir = *db.virtualConstructor();
  db.create(dir, name, getClassName());
  returnValue |= dir.put( ParentList.size(), "sizeOfParentList");
  // iterate on parent list and store parent info objects
  char buf[80];
  list<ParentInfo*>::iterator parentIterator;
  int listIndex = 0;
  for( parentIterator = ParentList.begin();
       parentIterator != ParentList.end();
       parentIterator++ )
    {
      sprintf(buf, "ParentInfo%d", listIndex );
      returnValue |= (*parentIterator)->put( dir, buf );
      listIndex++;
    }

  // iterate on child list and store child info objects
  listIndex = 0;
  returnValue |= dir.put( ChildList.size(), "sizeOfChildList");
  list<ChildInfo*>::iterator childIterator;
  for( childIterator = ChildList.begin();
       childIterator != ChildList.end();
       childIterator++ )
    {
      sprintf(buf, "ChildInfo%d", listIndex );
      returnValue |= (*childIterator)->put( dir, buf );
      listIndex++;
    }

  // iterate on sibling list and store sibling info objects
  listIndex = 0;
  returnValue |= dir.put( SiblingList.size(), "sizeOfSiblingList");
  list<SiblingInfo*>::iterator siblingIterator;
  for( siblingIterator = SiblingList.begin();
       siblingIterator != SiblingList.end();
       siblingIterator++ )
    {
      sprintf(buf, "SiblingInfo%d", listIndex );
      returnValue |= (*siblingIterator)->put( dir, buf );
      listIndex++;
    }
  delete &dir;
#endif

  cerr << className << "::put( const GenericDataBase & db, const aString &name ) not implemented as yet!" << endl; 
  return returnValue;
}

void ParentChildSiblingInfoData::addChild( ChildInfo *childInfo )
{
  ChildList.push_back( childInfo );
}

void ParentChildSiblingInfoData::addParent( ParentInfo *parentInfo )
{
  ParentList.push_back( parentInfo );
}

void ParentChildSiblingInfoData::addSibling( SiblingInfo *siblingInfo )
{
  SiblingList.push_back( siblingInfo );
}

// we decide to have this operator in the Data class also
// more for debugging than for anything else
ostream& operator<<(ostream& s, const ParentChildSiblingInfoData &pcsInfoData )
{
  s << "className = " << pcsInfoData.className << endl;
  s << "size of parent list: " << pcsInfoData.getParents().size() << endl;

  list<ParentInfo*>::const_iterator parentIterator;
  for( parentIterator = pcsInfoData.getParents().begin();
       parentIterator != pcsInfoData.getParents().end();
       parentIterator++ )
    {
      assert((*parentIterator)!=NULL );
      s << *(*parentIterator) << endl;
    }

  s << "size of child list: " << pcsInfoData.getChildren().size() << endl;

  list<ChildInfo*>::const_iterator childIterator;

  for( childIterator = pcsInfoData.getChildren().begin();
       childIterator != pcsInfoData.getChildren().end();
       childIterator++ )
    {
      assert((*childIterator)!=NULL );
      s << *(*childIterator) << endl;
    }

  s << "size of sibling list: " << pcsInfoData.getSiblings().size() << endl;
  list<SiblingInfo*>::const_iterator siblingIterator;
  for( siblingIterator = pcsInfoData.getSiblings().begin();
       siblingIterator != pcsInfoData.getSiblings().end();
       siblingIterator++ )
    {
      assert((*siblingIterator)!=NULL );
      s << *(*siblingIterator) << endl;
    }

  return s;
}

