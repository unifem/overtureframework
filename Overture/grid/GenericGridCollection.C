//
// Who to blame:  Geoff Chesshire and WDH
//

#include "GenericGridCollection.h"
#include "MappingRC.h"
#include "LoadBalancer.h"

#ifdef USE_STL
RCVector_STATIC_MEMBER_DATA(GenericGridCollection)
#endif // USE_STL

//
// class GenericGridCollection:
//
// Public member functions:
//
// Default constructor.
//
// If numberOfGrids_==0 (e.g., by default) then create a null
// GenericGridCollection.  Otherwise create a GenericGridCollection
// with the given number of dimensions and number of component grids.
//
GenericGridCollection::GenericGridCollection(
  const Integer numberOfGrids_):
  ReferenceCounting() {
    className = "GenericGridCollection";
    master=this;
    rcData = new GenericGridCollectionData(numberOfGrids_);
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    updateReferences();
}
//
// Copy constructor.  (Does a deep copy by default.)
//
GenericGridCollection::GenericGridCollection(
  const GenericGridCollection& x,
  const CopyType               ct):
  ReferenceCounting(x, ct) {
    className = "GenericGridCollection";
    master=this;
    switch (ct) {
      case DEEP:
      case NOCOPY:
        rcData = (GenericGridCollectionData*)
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
GenericGridCollection::~GenericGridCollection()
  { if (isCounted && rcData->decrementReferenceCount() == 0) delete rcData; }
//
// Assignment operator.  (Does a deep copy.)
//
GenericGridCollection& GenericGridCollection::
operator=(const GenericGridCollection& x) 
{
//  ReferenceCounting::operator=(x);
  if (rcData != x.rcData) 
  {
    if (rcData->getClassName() == x.rcData->getClassName()) 
    {
      (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
      updateReferences();
    } else {
      GenericGridCollection& y =
	*(GenericGridCollection*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
    master=x.master;
  } // end if
  return *this;
}

//\begin{>>GenericGridCollectionInclude.tex}{\subsubsection{reference(GenericGridCollection)}} 
void GenericGridCollection::
reference(const GenericGridCollection& x) 
// ===========================================================
// /Description:
//    Make a reference.  (Does a shallow copy.)
//\end{GenericGridCollectionInclude.tex}
// ===========================================================
{
  ReferenceCounting::reference(x);
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

void GenericGridCollection::
reference(GenericGridCollectionData& x)
{
  if (rcData != &x) 
  {
    if (isCounted && rcData->decrementReferenceCount() == 0)
      delete rcData;
    rcData = &x;
    isCounted = !x.uncountedReferencesMayExist();
    rcData->incrementReferenceCount();
    // *wdh* updateReferences();
  } // end if
  updateReferences();   // *wdh* 000322 -- we must always do this since the number of grids etc. may have changed.
}
//
// Break a reference.  (Replaces with a deep copy.)
//
void GenericGridCollection::breakReference() {
//  ReferenceCounting::breakReference();
    if (!isCounted || rcData->getReferenceCount() != 1) {
        GenericGridCollection x = *this; // Uses the (deep) copy constructor.
        reference(x);
    } // end if
}
//
// Check that the data structure is self-consistent.
//
void GenericGridCollection::consistencyCheck() const {
    ReferenceCounting::consistencyCheck();
    if (rcData == NULL) {
        cerr << className << "::consistencyCheck():  "
             << "rcData == NULL for "
             << getClassName() << " " << getGlobalID() << "." << endl;
        assert(rcData != NULL);
    }
    rcData->                 consistencyCheck();
    grid                    .consistencyCheck();
    gridNumber              .Test_Consistency();
    baseGrid                .consistencyCheck();
    baseGridNumber          .Test_Consistency();
    refinementLevel         .consistencyCheck();
    refinementLevelNumber   .Test_Consistency();
    componentGrid           .consistencyCheck();
    componentGridNumber     .Test_Consistency();
    multigridLevel          .consistencyCheck();
    multigridLevelNumber    .Test_Consistency();
    domain                  .consistencyCheck();
    domainNumber            .Test_Consistency();
}

// Here is the master grid.
GenericGridCollection & GenericGridCollection::
masterGridCollection()
{
  assert( master!=0 );
  return *master;
}


const aString & GenericGridCollection::getDomainName(int domainID ) const
// ========================================================================
// /Description:
//   Get the name of a domain
// /domain (input) : domain number
// ========================================================================
{
  return rcData->getDomainName(domainID );
}


void GenericGridCollection::setDomainName(int domainID, const aString & name )
// ========================================================================
// /Description:
//   Set the name of a domain
// /domain (input) : domain number
// /name (input) : name for the domain
// ========================================================================
{
  rcData->setDomainName(domainID,name);
}

void GenericGridCollection::
setLoadBalancer( LoadBalancer & loadBalancer ) 
// ========================================================================
// /Description:
//    Suppy a LoadBalancer to use when reading in grids
// ========================================================================
{
  rcData->pLoadBalancer = &loadBalancer;
}



//
// "Get" and "put" database operations.
//
Integer GenericGridCollection::get(
  const GenericDataBase& db,
  const aString&         name) {
    Integer returnValue = rcData->get(db, name);
    updateReferences();
    return returnValue;
}
Integer GenericGridCollection::put(
  GenericDataBase& db,
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
void GenericGridCollection::
updateReferences(const Integer what) 
{
#define REFERENCE(Type, x) ((Type&)x).reference(rcData->x)
#define REF_ARRAY(Type, x) \
  if (x.getDataPointer() != rcData->x.getDataPointer()) REFERENCE(Type, x)
#ifdef USE_STL
							  REFERENCE(RCVector<GenericGrid>,           grid);
  REF_ARRAY(IntegerArray,                    gridNumber);
  REFERENCE(RCVector<GenericGridCollection>, baseGrid);              
  REF_ARRAY(IntegerArray,                    baseGridNumber);
  REFERENCE(RCVector<GenericGridCollection>, refinementLevel);       
  REF_ARRAY(IntegerArray,                    refinementLevelNumber);
  REFERENCE(RCVector<GenericGridCollection>, componentGrid);        
  REF_ARRAY(IntegerArray,                    componentGridNumber);
  REFERENCE(RCVector<GenericGridCollection>, multigridLevel);       
  REF_ARRAY(IntegerArray,                    multigridLevelNumber);
  REFERENCE(RCVector<GenericGridCollection>, domain);       
  REF_ARRAY(IntegerArray,                    domainNumber);
#else
  REFERENCE(ListOfGenericGrid,               grid);
  REF_ARRAY(IntegerArray,                    gridNumber);
  REFERENCE(ListOfGenericGridCollection,     baseGrid);
  REF_ARRAY(IntegerArray,                    baseGridNumber);
  REFERENCE(ListOfGenericGridCollection,     refinementLevel);
  REF_ARRAY(IntegerArray,                    refinementLevelNumber);
  REFERENCE(ListOfGenericGridCollection,     componentGrid);
  REF_ARRAY(IntegerArray,                    componentGridNumber);
  REFERENCE(ListOfGenericGridCollection,     multigridLevel);
  REF_ARRAY(IntegerArray,                    multigridLevelNumber);
  REFERENCE(ListOfGenericGridCollection,     domain);
  REF_ARRAY(IntegerArray,                    domainNumber);
#endif // USE_STL
#undef REFERENCE
#undef REF_ARRAY

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
  FOR_COLLECTION(baseGrid);
  FOR_COLLECTION(refinementLevel);
  FOR_COLLECTION(componentGrid);
  FOR_COLLECTION(multigridLevel);
  FOR_COLLECTION(domain);
#undef FOR_COLLECTION
  
  for( i=0; i<numberOfGrids(); i++) grid[i].updateReferences(what);
}
//
// Update the grid, sharing the data of another grid.
//
Integer GenericGridCollection::update(
  GenericGridCollection& x,
  const Integer          what,
  const Integer          how) {
    Integer upd =
      rcData->update(*((GenericGridCollection&)x).rcData, what, how);
    updateReferences(what);
    return upd;
}
//
// Destroy optional grid data.
//
void GenericGridCollection::destroy(const Integer what) {
    rcData->destroy(what);
    updateReferences();
}
//
// Add a refinement grid to the collection.
//
Integer GenericGridCollection::addRefinement(
  const Integer& level, // The refinement level number.
  const Integer  k) {   // The index of an ancestor of the refinement grid.
    Integer n = rcData->addRefinement(level, k);
    updateReferences();
    return n;
}
//
// Delete all multigrid levels of refinement grid k.
//
void GenericGridCollection::deleteRefinement(const Integer& k) {
    rcData->deleteRefinement(k);
    updateReferences();
}
//
// Delete all grids with refinement level greater than the given level.
//
void GenericGridCollection::deleteRefinementLevels(const Integer level) {
    rcData->deleteRefinementLevels(level);
    updateReferences();
}
//
// Reference x[i] for refinementLevelNumber(i) <= level.
// Delete all other grids.
//
void GenericGridCollection::referenceRefinementLevels(
  GenericGridCollection& x,
  const Integer          level) {
    rcData->referenceRefinementLevels(*x, level);
    updateReferences();
}
//
// Add a multigrid coarsening of grid k.
//
Integer GenericGridCollection::addMultigridCoarsening(
  const Integer& level, // The multigrid level number.
  const Integer  k) {   // The index of the corresponding grid
                        // at any finer multigrid level.
    Integer n = rcData->addMultigridCoarsening(level, k);
    updateReferences();
    return n;
}
//
// Delete grid k, a multigrid coarsening, and all of its multigrid coarsenings.
//
void GenericGridCollection::deleteMultigridCoarsening(const Integer& k) {
    rcData->deleteMultigridCoarsening(k);
    updateReferences();
}
//
// Delete all of the grids with multigrid level greater than the given level.
//
void GenericGridCollection::deleteMultigridLevels(const Integer level) {
    rcData->deleteMultigridLevels(level);
    updateReferences();
}
//
// Set the number of grids.
//
void GenericGridCollection::setNumberOfGrids(const Integer& numberOfGrids_) {
    rcData->setNumberOfGrids(numberOfGrids_);
    updateReferences();
}
//
//  Initialize the GenericGridCollection with the given number of grids.
//  These grids have their gridNumbers, baseGridNumbers and componentGridNumbers
//  set to [0, ..., numberOfGrids_-1], and their refinementLevelNumbers and
//  multigridLevelNumbers set to zero.
//
void GenericGridCollection::initialize(const Integer& numberOfGrids_)
  { rcData->initialize(numberOfGrids_); }
//
// Stream output operator.
//
ostream& operator<<(ostream& s, const GenericGridCollection& g) {
    s << (ReferenceCounting&)g << endl;
    char _[80]; Integer i;
    sprintf(_, "  computedGeometry()                =  %o (octal)",
      (unsigned)g.computedGeometry());
    s << _ << endl
      << "  numberOfGrids()                   =  "
      <<  g.numberOfGrids() << endl
      << "  gridNumber                        = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.gridNumber(i);
    s << "]" << endl
      << "  numberOfBaseGrids()               =  "
      <<  g.numberOfBaseGrids() << endl
      << "  baseGridNumber                    = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.baseGridNumber(i);
    s << "]" << endl
      << "  numberOfRefinementLevels()        =  "
      <<  g.numberOfRefinementLevels() << endl
      << "  refinementLevelNumber             = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.refinementLevelNumber(i);
    s << "]" << endl
      << "  numberOfComponentGrids()          =  "
      <<  g.numberOfComponentGrids() << endl
      << "  componentGridNumber               = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.componentGridNumber(i);
    s << "]" << endl
      << "  numberOfMultigridLevels()         =  "
      <<  g.numberOfMultigridLevels() << endl
      << "  multigridLevelNumber              = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.multigridLevelNumber(i);
    s << "]" << endl
      << "  numberOfDomains()         =  "
      <<  g.numberOfDomains() << endl
      << "  domainNumber              = [";
    for (i=0; i<g.numberOfGrids(); i++)
      s << (i ? "," : "") << g.domainNumber(i);
    return s
      << "]";
}

// ====================================================================================
// \Description:
//  Specify whether to keep the parallel distribution when setting this GridCollection
//  equal to another. This option can be used to copy a GridCollection to a different
//  set of processors. 
//  
//  When this option is true, and this GridCollection is set equal to another (with
//  the "=" operator) then the distribution is not copied but rather the current
//  distribution is used. 
// ====================================================================================
void GenericGridCollection::
keepGridDistributionOnCopy(const bool trueOrFalse /* =true */ )
{
  rcData->keepGridDistributionOnCopy = trueOrFalse;
}





// ********************************************************************************************
// ******************** class GenericGridCollectionData: **************************************
// ********************************************************************************************

// Constructor:
GenericGridCollectionData::GenericGridCollectionData(
  const Integer numberOfGrids_):
  ReferenceCounting() {
    className = "GenericGridCollectionData";
    initialize(numberOfGrids_);
}

// Copy constructor: 
GenericGridCollectionData::GenericGridCollectionData(
  const GenericGridCollectionData& x,
  const CopyType                   ct):
  ReferenceCounting() {
    className = "GenericGridCollectionData";
    initialize(0);
    if (ct != NOCOPY) *this = x;
}

// Destructor:
GenericGridCollectionData::~GenericGridCollectionData() { }

// *******************************************************************************
// #include "MappedGrid.h"
// *******************************************************************************

GenericGridCollectionData& GenericGridCollectionData::
operator=(const GenericGridCollectionData& x) 
{
  ReferenceCounting::operator=(x);
  numberOfComponentGrids   = x.numberOfComponentGrids;  // *wdh*
  setNumberOfGrids(x.numberOfGrids);

  // If keepGridDistributionOnCopy is true then we use the current parallel distribution rather
  // than the one in x. 
  // This will allow one to copy a GridCollection to a different set of processors by initially building
  // a gridDistributionList before copying
  if( keepGridDistributionOnCopy && gridDistributionList.size()!=numberOfGrids )
  {
    printF("GridCollection:operator= ERROR: keepGridDistributionOnCopy==true but the gridDistributionList\n"
	   " does not have the correct number of grids! gridDistributionList.size()=%i, numberOfGrids=%i\n",
	   gridDistributionList.size(),numberOfGrids);
    OV_ABORT("error");
  }

  if( !keepGridDistributionOnCopy || gridDistributionList.size()!=numberOfGrids )
  {
    gridDistributionList.clear();
    if( x.gridDistributionList.size()==numberOfGrids )
      gridDistributionList=x.gridDistributionList;
  }
  
  // Assign parallel distribution 
  if( gridDistributionList.size()==numberOfGrids )
  {
    for( int i=0; i<numberOfGrids; i++ )
    {
      int pStart=-1,pEnd=0;
      gridDistributionList[i].getProcessorRange(pStart,pEnd);
      if( false )
        printF("GenericGridCollection::operator=:assign grid %i to processors=[%i,%i]\n",i,pStart,pEnd);
      grid[i].specifyProcesses(Range(pStart,pEnd));
    }
  }
  

//    printf(">>>GenericGridCollectionData:operator= : grid.getLength()=%i, \n",grid.getLength());
//    for( int g=0; g<grid.getLength(); g++ )
//    {
     
//      printf("BEFORE g=%i grid[g].getClassName()=%s, rcData->getClassName()=%s\n",g,
//  	   (const char*)grid[g].getClassName(),(const char*)grid[g].rcData->getClassName());
//      MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//      MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//      printf("GenericGridCollectionData: rc.name=%s. x.rc.name=%s\n",
//  	   (const char*)rc->mapping.getName(Mapping::mappingName),
//  	   (const char*)xrc->mapping.getName(Mapping::mappingName));
//    }
    
  // This next statement will copy the list of grids, grid[g] g=0,1,...
  //     grid[g].rcData will point to the reference counted data of the derived class (usually MappedGridData)

  if( false )
  {
    // old way    
    grid = x.grid;
  }
  else
  { // *wdh* 040316
    assert( grid.getLength()==x.grid.getLength() );
    
    for( int g=0; g<grid.getLength(); g++ )
    {
      // check refinement level number
      if( x.refinementLevelNumber(g)==0 )
      {
//  	printf("GenericGridCollection: copy grid %i (deep copy)\n",g);
//  	if( true )
//  	{
//  	MappedGridData *rc1= (MappedGridData*)grid[1].rcData;
//  	MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//  	MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//  	printf("Before copy MappedGridData: rc.name=%s, x.rc.name=%s rc1.name=%s\n",
//  	       (const char*)rc->mapping.getName(Mapping::mappingName),
//  	       (const char*)xrc->mapping.getName(Mapping::mappingName),
//                 (const char*)rc1->mapping.getName(Mapping::mappingName));
//  	}

	grid[g] = x.grid[g];

//  	if( true )
//  	{
//  	MappedGridData *rc1= (MappedGridData*)grid[1].rcData;
//  	MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//  	MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//  	printf("After copy MappedGridData: rc.name=%s, x.rc.name=%s rc1.name=%s\n",
//  	       (const char*)rc->mapping.getName(Mapping::mappingName),
//  	       (const char*)xrc->mapping.getName(Mapping::mappingName),
//                 (const char*)rc1->mapping.getName(Mapping::mappingName));
//  	}
	
      }
      else
      {
	// For AMR grids we deep copy all but the Mapping

	const int base = x.baseGridNumber(g);
//  	printf("GenericGridCollection: copy AMR grid %i (base=%i) (do not copy mapping)\n",g,base);
//  	if( true )
//  	{
//  	MappedGridData *rc1= (MappedGridData*)grid[1].rcData;
//  	MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//  	MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//  	printf("Before copy MappedGridData: rc.name=%s, x.rc.name=%s rc1.name=%s\n",
//  	       (const char*)rc->mapping.getName(Mapping::mappingName),
//  	       (const char*)xrc->mapping.getName(Mapping::mappingName),
//                 (const char*)rc1->mapping.getName(Mapping::mappingName));
//  	}

        int option=1;  // This means do not copy the Mapping
        grid[g].equals(x.grid[g],option);

//  	if( true )
//  	{
//  	MappedGridData *rc1= (MappedGridData*)grid[1].rcData;
//  	MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//  	MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//  	printf("After copy MappedGridData: rc.name=%s, x.rc.name=%s rc1.name=%s\n",
//  	       (const char*)rc->mapping.getName(Mapping::mappingName),
//  	       (const char*)xrc->mapping.getName(Mapping::mappingName),
//                 (const char*)rc1->mapping.getName(Mapping::mappingName));
//  	}
	
      }
	
    }
  }
//    for( int g=0; g<grid.getLength(); g++ )
//    {
//      MappedGridData *rc = (MappedGridData*)grid[g].rcData;
//      MappedGridData *xrc = (MappedGridData*)x.grid[g].rcData;
//      printf("GenericGridCollectionData:AFTER: rc.name=%s. x.rc.name=%s\n",
//  	   (const char*)rc->mapping.getName(Mapping::mappingName),
//  	   (const char*)xrc->mapping.getName(Mapping::mappingName));
//    }
//    for( int g=0; g<grid.getLength(); g++ )
//    {
//      printf("AFTER: g=%i grid[g].getClassName()=%s\n",g,(const char*)(&(grid[g]))->getClassName());
//    }

  gridNumber               = x.gridNumber;
  numberOfBaseGrids        = x.numberOfBaseGrids;
  baseGridNumber           = x.baseGridNumber;
  numberOfRefinementLevels = x.numberOfRefinementLevels;
  refinementLevelNumber    = x.refinementLevelNumber;
  numberOfComponentGrids   = x.numberOfComponentGrids;
  componentGridNumber      = x.componentGridNumber;
  numberOfMultigridLevels  = x.numberOfMultigridLevels;
  multigridLevelNumber     = x.multigridLevelNumber;
  numberOfDomains          = x.numberOfDomains;
  domainNumber             = x.domainNumber;

// *wdh* 981014    computedGeometry         = NOTHING;
  computedGeometry         = x.computedGeometry;
  gridCollectionType       = x.gridCollectionType;
  return *this;
}

void GenericGridCollectionData::reference(const GenericGridCollectionData& x) {
  cerr << "GenericGridCollectionData::reference(const GenericGridCollectionData&) "
       << "was called!" << endl;
  ReferenceCounting::reference(x);
}

void GenericGridCollectionData::breakReference() {
  cerr << "GenericGridCollectionData::breakReference() was called!" << endl;
  ReferenceCounting::breakReference();
}

void GenericGridCollectionData::consistencyCheck() const {
  ReferenceCounting::      consistencyCheck();
  grid                    .consistencyCheck();
  gridNumber              .Test_Consistency();
  baseGrid                .consistencyCheck();
  baseGridNumber          .Test_Consistency();
  refinementLevel         .consistencyCheck();
  refinementLevelNumber   .Test_Consistency();
  componentGrid           .consistencyCheck();
  componentGridNumber     .Test_Consistency();
  multigridLevel          .consistencyCheck();
  multigridLevelNumber    .Test_Consistency();
  domain                  .consistencyCheck();
  domainNumber            .Test_Consistency();
#ifdef USE_STL
  if (grid.size() != numberOfGrids) {
    cerr << className << "::consistencyCheck():  "
	 << "grid.size() != numberOfGrids for "
	 << getClassName() << " " << getGlobalID() << "." << endl;
    assert(grid.size() == numberOfGrids);
  }
#else
  if (grid.getLength() != numberOfGrids) {
    cerr << className << "::consistencyCheck():  "
	 << "grid.getLength() != numberOfGrids for "
	 << getClassName() << " " << getGlobalID() << "." << endl;
    assert(grid.getLength() == numberOfGrids);
  }
#endif // USE_STL
  for (Integer i=0; i<numberOfGrids; i++) grid[i].consistencyCheck();
}


const aString & GenericGridCollectionData::getDomainName(int domainID ) const
// ========================================================================
// /Description:
//   Get the name of a domain
// /domain (input) : domain number
// ========================================================================
{
  if( domainID<0 || domainID>=numberOfDomains )
  {
    printF("GenericGridCollection::getDomainName: invalid value for domainID=%i. Using domainID=0.\n",
           domainID);
    domainID=0;
  }
  if( domainName.size() < numberOfDomains )
  {
    // allocate space for domain names and assign default names
    std::vector<aString> & domainNameNC = (std::vector<aString>&)domainName; // cast away const
    for( int d=domainName.size(); d<numberOfDomains; d++ )
    {
      domainNameNC.push_back(sPrintF("domain%i",d));
    }
  }
  return domainName[domainID];
}


void GenericGridCollectionData::setDomainName(int domainID, const aString & name )
// ========================================================================
// /Description:
//   Set the name of a domain
// /domain (input) : domain number
// /name (input) : name for the domain
// ========================================================================
{
  if( domainID<0 || domainID>=numberOfDomains )
  {
    printF("GenericGridCollection::setDomainName: invalid value for domainID=%i. Using domainID=0.\n",
            domainID);
    domainID=0;
  }
  if( domainName.size() < numberOfDomains )
  {
    // allocate space for domain names and assign default names
    for( int d=domainName.size(); d<numberOfDomains; d++ )
    {
      domainName.push_back(sPrintF("domain%i",d));
    }
  }
  domainName[domainID]=name;
}



Integer GenericGridCollectionData::
get(const GenericDataBase& db,
    const aString&         name) 
{
  Integer returnValue = 0, i;
  GenericDataBase& dir = *db.virtualConstructor();
  db.find(dir, name, getClassName());

  returnValue |= dir.get(numberOfGrids, "numberOfGrids");

//  Make sure that *grid[i].rcData is of the same class as *grid[i-1].rcData.
//  This is done by using the GenericGrid deep copy constructor, which uses
//  GenericGridData::virtualConstructor() to construct *grid[i].rcData.
#ifdef USE_STL
  for (i=grid.size(); i<numberOfGrids; i++)
    if (i) grid.push_back(GenericGrid(grid[i-1]));
    else grid.push_back(GenericGrid());
#else
  for (i=grid.getLength(); i<numberOfGrids; i++)
    if (i) grid.addElement(GenericGrid(grid[i-1]));
    else grid.addElement();
#endif // USE_STL
  initialize(numberOfGrids);

  returnValue |= dir.get(computedGeometry, "computedGeometry");

  if (numberOfGrids > 0) 
  {
    returnValue |= dir.get(gridNumber,
			   "gridNumber");
    returnValue |= dir.get(numberOfBaseGrids,
			   "numberOfBaseGrids");
    returnValue |= dir.get(baseGridNumber,
			   "baseGridNumber");
    returnValue |= dir.get(numberOfRefinementLevels,
			   "numberOfRefinementLevels");
    returnValue |= dir.get(refinementLevelNumber,
			   "refinementLevelNumber");
    returnValue |= dir.get(numberOfComponentGrids,
			   "numberOfComponentGrids");
    returnValue |= dir.get(componentGridNumber,
			   "componentGridNumber");
    returnValue |= dir.get(numberOfMultigridLevels,
			   "numberOfMultigridLevels");
    returnValue |= dir.get(multigridLevelNumber,
			   "multigridLevelNumber");
    returnValue |= dir.get(numberOfDomains,
			   "numberOfDomains");
    returnValue |= dir.get(domainNumber,
			   "domainNumber");
    if( numberOfDomains>1 )
    { // get the domain names if there is more than one domain
      aString *name = new aString [numberOfDomains];
      returnValue |= dir.get(name,"domainName",numberOfDomains);
      for( int d=0; d<numberOfDomains; d++ )
        setDomainName(d,name[d]);
      delete [] name;
    }
  } // end if

  MappingRC::DataBaseModeEnum dbMode=MappingRC::getDataBaseMode();
  // MappingRC::setDataBaseMode(MappingRC::linkMappings);  // do not put multiple copies of Mapping's


  // for backward compatibility look for this next flag:
  int mappingsCompressedForAMR=false;
  if( numberOfRefinementLevels>1 )
  {
    if( dir.get(mappingsCompressedForAMR,"mappingsCompressedForAMR")!=0 )
      mappingsCompressedForAMR=false;
  }
  
  // Assign parallel distribution (if the info is there)
  if( gridDistributionList.size()==numberOfGrids )
  {
    for( i=0; i<numberOfGrids; i++ )
    {
      int pStart=-1,pEnd=0;
      gridDistributionList[i].getProcessorRange(pStart,pEnd);
      if( false )
        printF("GenericGridCollection::get:assignLoadBalance: assign grid %i to processors=[%i,%i]\n",i,pStart,pEnd);
      grid[i].specifyProcesses(Range(pStart,pEnd));
    }
  }
  else if( gridDistributionList.size()>0 )
  {
    printF("GenericGridCollection::get:WARNING: gridDistributionList is there but it is has the wrong size,\n"
           "  numberOfGrids=%i but gridDistributionList.size()=%i .\n",numberOfGrids,gridDistributionList.size());
  }

  for (i=0; i<numberOfGrids; i++) 
  {
    char name[40]; sprintf(name, "grid[%d]", i);
//     if( numberOfRefinementLevels>1 )
//     {
//       printf("GenericGridCollection::get:grid=%i, baseGridNumber=%i, refinementLevelNumber=%i\n",i,
// 	     baseGridNumber(i), refinementLevelNumber(i));
//     }
    
    bool getMapping= !mappingsCompressedForAMR ||  !( numberOfRefinementLevels>1 && refinementLevelNumber(i)>0 );
    // If this is a refinement grid -- we treat this as a special case --
    returnValue |= grid[i].get(dir, name,getMapping);
    
  } // end for

  MappingRC::setDataBaseMode(dbMode); // reset

  const Integer computedGeometry0 = computedGeometry;
  GenericGridCollectionData::update(*this,
				    computedGeometry0 & EVERYTHING, COMPUTEnothing);
  computedGeometry = computedGeometry0;

  delete &dir;
  return returnValue;
}

Integer GenericGridCollectionData::
put(GenericDataBase& db,
    const aString&   name,
    int geometryToPut /* = -1  */
    ) const 
// geometryToPut : by default put computedGeometry
{
  Integer returnValue = 0;
  const int geometryToPut0 = geometryToPut;  // use this for saving the Mapping 
  if( geometryToPut==-1 ) 
    geometryToPut=computedGeometry;
  else
  {
    // NOTE: We must always save some things if they have been computed:
    geometryToPut |= (THEbaseGrid |
		      THErefinementLevel |
		      THEcomponentGrid |
		      THEmultigridLevel |
		      THEdomain );

    geometryToPut = geometryToPut & computedGeometry;
  }

  GenericDataBase& dir = *db.virtualConstructor();
  db.create(dir, name, getClassName());

  returnValue |= dir.put(numberOfGrids,    "numberOfGrids");
  returnValue |= dir.put(geometryToPut, "computedGeometry");

  if (numberOfGrids > 0) 
  {
    returnValue |= dir.put(gridNumber,
			   "gridNumber");
    returnValue |= dir.put(numberOfBaseGrids,
			   "numberOfBaseGrids");
    returnValue |= dir.put(baseGridNumber,
			   "baseGridNumber");
    returnValue |= dir.put(numberOfRefinementLevels,
			   "numberOfRefinementLevels");
    returnValue |= dir.put(refinementLevelNumber,
			   "refinementLevelNumber");
    returnValue |= dir.put(numberOfComponentGrids,
			   "numberOfComponentGrids");
    returnValue |= dir.put(componentGridNumber,
			   "componentGridNumber");
    returnValue |= dir.put(numberOfMultigridLevels,
			   "numberOfMultigridLevels");
    returnValue |= dir.put(multigridLevelNumber,
			   "multigridLevelNumber");
    returnValue |= dir.put(numberOfDomains,
			   "numberOfDomains");
    returnValue |= dir.put(domainNumber,
			   "domainNumber");
    if( numberOfDomains>1 )
    { // save the domain names if there is more than one domain
      aString *name = new aString [numberOfDomains];
      for( int d=0; d<numberOfDomains; d++ )
        name[d]=getDomainName(d);
      returnValue |= dir.put(name,"domainName",numberOfDomains);
      delete [] name;
    }
    
  } // end if

  MappingRC::DataBaseModeEnum dbMode=MappingRC::getDataBaseMode();
  //    MappingRC::setDataBaseMode(MappingRC::linkMappings);  // do not put multiple copies of Mapping's

  // for backward compatibility:
  if( numberOfRefinementLevels>1 )
  {
    int mappingsCompressedForAMR=true;
    dir.put(mappingsCompressedForAMR,"mappingsCompressedForAMR");
  }
  
  for (Integer i=0; i<numberOfGrids; i++) 
  {
    char name[32]; sprintf(name, "grid[%d]", i);
    // todo: if this is a refinement grid then just save enough info to be able to rebuild it.
//     if( numberOfRefinementLevels>1 )
//     {
//       printf("GenericGridCollection::put:grid=%i, baseGridNumber=%i, refinementLevelNumber=%i\n",i,
// 	     baseGridNumber(i), refinementLevelNumber(i));
//     }
    // If this is a refinement grid: do not save the Mapping.
    bool putMapping= !(numberOfRefinementLevels>1 && refinementLevelNumber(i)>0);
    returnValue |= grid[i].put(dir, name, putMapping, geometryToPut0);
    
  } // end for

  MappingRC::setDataBaseMode(dbMode); // reset

  delete &dir;
  return returnValue;
}

Integer GenericGridCollectionData::update(
  GenericGridCollectionData& x,
  const Integer              what,
  const Integer              how) {
// *wdh*    if (what & INT_MIN) cerr << "Warning:  update() was called with default arguments.  This is not recommended." << endl;
    Integer upd = NOTHING;
    Integer i;
    for (i=0; i<numberOfGrids; i++)
      upd |= grid[i].update(x[i], what & ~INT_MIN, how);
    for (i=0; i<numberOfGrids; i++) grid[i].updateReferences(what);
    if (what &                 THEbaseGrid)
      upd |= updateCollection( THEbaseGrid        | (what & ~THElists),
        numberOfBaseGrids,        baseGrid,         baseGridNumber);
    if (what &                 THErefinementLevel)
      upd |= updateCollection( THErefinementLevel | (what & ~THElists),
        numberOfRefinementLevels, refinementLevel, refinementLevelNumber);
    if (what &                 THEcomponentGrid)
      upd |= updateCollection( THEcomponentGrid   | (what & ~THElists),
        numberOfComponentGrids,   componentGrid,   componentGridNumber);
    if (what &                 THEmultigridLevel)
      upd |= updateCollection( THEmultigridLevel  | (what & ~THElists),
        numberOfMultigridLevels,  multigridLevel,  multigridLevelNumber);
    if (what &                 THEdomain)
      upd |= updateCollection( THEdomain  | (what & ~THElists),
        numberOfDomains,  domain,  domainNumber);
    return upd;
}
void GenericGridCollectionData::destroy(const Integer what) {
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
      baseGrid.reference(ListOfGenericGridCollection());
    if (what & THErefinementLevel)
      refinementLevel.reference(ListOfGenericGridCollection());
    if (what & THEcomponentGrid)
      componentGrid.reference(ListOfGenericGridCollection());
    if (what & THEmultigridLevel)
      multigridLevel.reference(ListOfGenericGridCollection());
    if (what & THEdomain)
      domain.reference(ListOfGenericGridCollection());
#endif // USE_STL
    for (Integer i=0; i<numberOfGrids; i++) grid[i].destroy(what);
    computedGeometry &= ~what;
}
void GenericGridCollectionData::geometryHasChanged(const Integer what) {
    computedGeometry &= ~what;
    for (Integer i=0; i<numberOfGrids; i++)
      grid[i].geometryHasChanged(what);
}

Integer GenericGridCollectionData::
addRefinement( const Integer& level,
	       const Integer  k )
{
  return addRefinements(level,k,1);
}


Integer GenericGridCollectionData::
addRefinements(const Integer level,
	       const Integer  k, 
               const Integer numberToAdd ) 
{
  if (k < 0 || k >= numberOfGrids)
  {
    cout << "GenericGridCollectionData::addRefinement(level = " << level
	 << ", k = " << k << "):  Grid " << k << " does not exist." << endl;
    assert(k >= 0); assert(k < numberOfGrids);
  } // end if
  Logical noParent = LogicalTrue;
  for (Integer i=0; noParent && i<numberOfGrids; i++)
    noParent = baseGridNumber(i) != baseGridNumber(k) ||
      refinementLevelNumber(i) != level-1 ||
      multigridLevelNumber(i) != 0;
  if (noParent) 
  {
    cout << "GenericGridCollectionData::addRefinement(level = "
	 << level << ", k = " << k << "):  "
	 << "A parent grid at refinement level " << level-1
	 << " and multigrid level 0 does not exist." << endl;
    assert(!noParent);
  } // end if

  const Integer n = numberOfGrids; // first new grid 

  setNumberOfGrids(n+numberToAdd);
  numberOfRefinementLevels = max0(numberOfRefinementLevels, level+1);
  for( int nn=n; nn<n+numberToAdd; nn++ )
  {
    baseGridNumber(nn)        = baseGridNumber(k);
    refinementLevelNumber(nn) = level;
    componentGridNumber(nn)   = numberOfComponentGrids++;  // *wdh* note increment
    multigridLevelNumber(nn)  = 0;
    domainNumber(nn)          = domainNumber(k);
  }
  
  computedGeometry &= ~(GenericGrid::EVERYTHING | THElists);
  return n;
}

void GenericGridCollectionData::deleteRefinement(const Integer& k) {
    if (k < 0 || k >= numberOfGrids) {
        cout << "GenericGridCollectionData::deleteRefinement(k = "
             << k << "):  Grid " << k << " does not exist." << endl;
        assert(k >= 0); assert(k < numberOfGrids);
    } else if (refinementLevelNumber(k) == 0) {
        cout << "GenericGridCollectionData::deleteRefinement(k = "
             << k << "):  Grid k = " << k << " is not a refinement." << endl;
        assert(refinementLevelNumber(k) != 0);
    } // end if
    GenericGridCollectionData::deleteMultigridCoarsening(k);
}
void GenericGridCollectionData::deleteRefinementLevels(const Integer level) {
    if (level < 0) {
        cout << "GenericGridCollectionData::deleteRefinementLevel(level = "
             << level << "):  Refinement level " << level << " does not exist."
             << endl;
        assert(level >= 0);
    } else if (level < numberOfRefinementLevels-1) {
        Integer i = numberOfGrids, j = i - 1;
        while (i--) if (refinementLevelNumber(i) > level) {
            if (i < j--) {
                Range r1(i, j), r2 = r1 + 1;
                baseGridNumber(r1)                 = baseGridNumber(r2);
                refinementLevelNumber(r1)          = refinementLevelNumber(r2);
                componentGridNumber(r1)            = componentGridNumber(r2);
                multigridLevelNumber(r1)           = multigridLevelNumber(r2);
                domainNumber(r1)                   = domainNumber(r2);
            } // end if
#ifdef USE_STL
            grid.erase(grid.begin() + i);
#else
            grid.deleteElement(i);
#endif // USE_STL
        } // end if, end while
        setNumberOfGrids(j+1);
        numberOfBaseGrids             = max(baseGridNumber)        + 1;
        numberOfRefinementLevels      = max(refinementLevelNumber) + 1;
        numberOfComponentGrids        = max(componentGridNumber)   + 1;
        numberOfMultigridLevels       = max(multigridLevelNumber)  + 1;
        numberOfDomains               = max(domainNumber)          + 1;
        computedGeometry &= ~THElists;
    } // end if
}
void GenericGridCollectionData::referenceRefinementLevels(
  GenericGridCollectionData& x,
  const Integer              level) {
    if (level < 0) {
        cout << "GenericGridCollectionData::referenceRefinementLevels(x, level = "
             << level << "):  Refinement level " << level << " does not exist."
             << endl;
        assert(level >= 0);
    } // end if
    Integer i, j;
    for (i=0, j=0; i<x.numberOfGrids; i++)
      if (x.refinementLevelNumber(i) <= level) j++;
    setNumberOfGrids(j);
    for (i=0, j=0; i<x.numberOfGrids; i++)
      if (x.refinementLevelNumber(i) <= level) {
        baseGridNumber(j)        = x.baseGridNumber(i);
        refinementLevelNumber(j) = x.refinementLevelNumber(i);
        componentGridNumber(j)   = x.componentGridNumber(i);
        multigridLevelNumber(j)  = x.multigridLevelNumber(i);
        domainNumber(j)          = x.domainNumber(i);
        grid[j].reference(x.grid[i]); j++;
    } // end if, end for
    numberOfBaseGrids        = max(baseGridNumber)        + 1;
    numberOfRefinementLevels = max(refinementLevelNumber) + 1;
    numberOfComponentGrids   = max(componentGridNumber)   + 1;
    numberOfMultigridLevels  = max(multigridLevelNumber)  + 1;
    numberOfDomains          = max(domainNumber)          + 1;
    computedGeometry = NOTHING;
}
Integer GenericGridCollectionData::addMultigridCoarsening(
  const Integer& level,
  const Integer  k) {
    if (k < 0 || k >= numberOfGrids) {
        cout << "GenericGridCollectionData::addMultigridCoarsening(level = "
             << level << ", k = " << k << "):  There is no grid k = " << k
             << "." << endl;
        assert(k >= 0); assert(k < numberOfGrids);
    } // end if
    Logical noParent = LogicalTrue; Integer i;
    for (i=0; noParent && i<numberOfGrids; i++) noParent =
      componentGridNumber(i)  != componentGridNumber(k) ||
      multigridLevelNumber(i) != level - 1;
    if (noParent) {
        cout << "GenericGridCollectionData::addMultigridCoarsening(level = "
             << level << ", k = " << k << "):  "
             << "A parent grid at multigrid level " << level-1
             << " does not exist." << endl;
        assert(!noParent);
    } // end if
    for (i=0; i<numberOfGrids; i++)
      if (componentGridNumber(i)  == componentGridNumber(k) &&
          multigridLevelNumber(i) == level) {
        cout << "GenericGridCollectionData::addMultigridCoarsening(level = "
             << level << ", k = " << k << "):  Multigrid level " << level
             << " of grid " << k << " already exists." << endl;
        assert(componentGridNumber(i)  != componentGridNumber(k) ||
               multigridLevelNumber(i) != level);
    } // end if, end for
    const Integer n = numberOfGrids; setNumberOfGrids(n+1);
    baseGridNumber(n)        = baseGridNumber(k);
    refinementLevelNumber(n) = refinementLevelNumber(k);
    componentGridNumber(n)   = componentGridNumber(k);
    multigridLevelNumber(n)  = level;
    domainNumber(n)          = domainNumber(k);
    numberOfMultigridLevels  = max0(numberOfMultigridLevels, level+1);
    computedGeometry &= ~(GenericGrid::EVERYTHING | THElists);
    return n;
}

void GenericGridCollectionData::
deleteMultigridCoarsening(const Integer& k)
{
  if (k < 0 || k >= numberOfGrids) 
  {
    cout << "GenericGridCollectionData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid " << k << " does not exist." << endl;
    assert(k >= 0); assert(k < numberOfGrids);
  }
  else if (multigridLevelNumber(k) == 0 && refinementLevelNumber(k) == 0)
  {
    cout << "GenericGridCollectionData::deleteMultigridCoarsening(k = "
	 << k << "):  Grid k = " << k << " is not a multigrid coarsening."
	 << endl;
    assert(multigridLevelNumber(k) != 0 || refinementLevelNumber(k) != 0);
  } // end if
  const Integer k0 = componentGridNumber(k),  l  = multigridLevelNumber(k);
  Integer i = numberOfGrids, j = i - 1;
  while (i--) 
  {
    if (componentGridNumber(i) == k0 &&	multigridLevelNumber(i) >= l) 
    {
      if (i < j--) 
      {
	Range r1(i, j), r2 = r1 + 1;
	baseGridNumber(r1)        = baseGridNumber(r2);
	refinementLevelNumber(r1) = refinementLevelNumber(r2);
	componentGridNumber(r1)   = componentGridNumber(r2);
	multigridLevelNumber(r1)  = multigridLevelNumber(r2);
	domainNumber(r1)          = domainNumber(r2);
      } // end if
#ifdef USE_STL
      grid.erase(grid.begin() + i);
#else
      grid.deleteElement(i);
#endif // USE_STL
    } // end while
  }
  
  setNumberOfGrids(j+1);
  numberOfBaseGrids        = max(baseGridNumber)        + 1;
  numberOfRefinementLevels = max(refinementLevelNumber) + 1;
  // *wdh*    numberOfComponentGrids   = max(componentGridNumber)   + 1;
  numberOfComponentGrids=0;
  for( i=0; i<numberOfGrids; i++ )
  {
    if( componentGridNumber(i) > k0 )  // *wdh* 000625
      componentGridNumber(i)-=1;
    
    if( multigridLevelNumber(i)==0 )
      numberOfComponentGrids++;
  }
    
  numberOfMultigridLevels  = max(multigridLevelNumber)  + 1;
  numberOfDomains          = max(domainNumber)  + 1;
  computedGeometry &= ~THElists;
}


void GenericGridCollectionData::deleteMultigridLevels(const Integer level) {
    if (level < 0) {
        cout << "GenericGridCollectionData::deleteMultigridLevel(level = "
             << level << "):  Multigrid level " << level << " does not exist."
             << endl;
        assert(level >= 0);
    } else if (level < numberOfMultigridLevels-1) {
        Integer i = numberOfGrids, j = i - 1;
        while (i--) if (multigridLevelNumber(i) > level) {
            if (i < j--) {
                Range r1(i, j), r2 = r1 + 1;
                baseGridNumber(r1)        = baseGridNumber(r2);
                refinementLevelNumber(r1) = refinementLevelNumber(r2);
                componentGridNumber(r1)   = componentGridNumber(r2);
                multigridLevelNumber(r1)  = multigridLevelNumber(r2);
                domainNumber(r1)          = domainNumber(r2);
            } // end if
#ifdef USE_STL
            grid.erase(grid.begin() + i);
#else
            grid.deleteElement(i);
#endif // USE_STL
        } // end if, end while
        setNumberOfGrids(j+1);
        numberOfBaseGrids        = max(baseGridNumber)        + 1;
        numberOfRefinementLevels = max(refinementLevelNumber) + 1;
        numberOfComponentGrids   = max(componentGridNumber)   + 1;
        numberOfMultigridLevels  = max(multigridLevelNumber)  + 1;
        numberOfDomains          = max(domainNumber)          + 1;
        computedGeometry &= ~THElists;
    } // end if
}


//! Set the number of grids.
/*!
    This function will set the number of grids in the grid collection.
    It will resize the gridNumber, baseGridNumber, refinementLevelNumber etc. arrays
    and add/delete elements to the "grid" list .
 */
void GenericGridCollectionData::
setNumberOfGrids( const Integer& numberOfGrids_) 
{
  Integer n = numberOfGrids_ - gridNumber.elementCount();
  if (n) 
  {
    gridNumber           .resize(numberOfGrids_);
    baseGridNumber       .resize(numberOfGrids_);
    refinementLevelNumber.resize(numberOfGrids_);
    componentGridNumber  .resize(numberOfGrids_);
    multigridLevelNumber .resize(numberOfGrids_);
    domainNumber         .resize(numberOfGrids_);
    if (n > 0)
    {
      const Range newGrids(numberOfGrids_ - n, numberOfGrids_ - 1);
      for (Integer i=numberOfGrids_ - n; i<numberOfGrids_; i++)
	gridNumber(i)                 = i;
      baseGridNumber(newGrids)        =
	refinementLevelNumber(newGrids) =
	componentGridNumber(newGrids)   =
	multigridLevelNumber(newGrids)  =
	domainNumber(newGrids)  = 0;
    } // end if
  } // end if
#ifdef STL
  n = numberOfGrids_ - grid.size();
  if (n < 0) grid.erase(grid.begin() + numberOfGrids_, grid.end());
  else for (Integer i=0; i<n; i++) grid.push_back();
#else
  while (grid.getLength() < numberOfGrids_) grid.addElement();
  while (grid.getLength() > numberOfGrids_) grid.deleteElement();
#endif // STL
  numberOfGrids = numberOfGrids_;
}


// ======================================================================================
// /Description:
//     update/build the sub-GridCollections such as refinementLevel, multigridLevel, domain
//
// /number (input) : grid k should be placed in collection number(k) 
// ======================================================================================
Integer GenericGridCollectionData::
updateCollection(const Integer&                   what,
		 Integer&                         numberOfCollections,
#ifdef USE_STL
		 RCVector<GenericGridCollection>& list,
#else
		 ListOfGenericGridCollection&     list,
#endif // USE_STL
		 IntegerArray&                    number) 
{

  //  Fix up the length of list.
  numberOfCollections = numberOfGrids > 0 ? max(number) + 1 : 0;
#ifdef USE_STL
  if (list.size() > numberOfCollections)
    list.erase(list.begin() + numberOfCollections, list.end());
#else
  while (list.getLength() > numberOfCollections) list.deleteElement();
#endif // USE_STL
  if (numberOfCollections) 
  {
    // Fill list with appropriately-constructed GenericGridCollections.
    // nG(j) : number of grids in collection i 
    IntegerArray nG(numberOfCollections); nG = 0; Integer k, i;
    for (k=0; k<numberOfGrids; k++) 
      nG(number(k))++;

    for (i=0; i<numberOfCollections; i++) 
    {
      #ifdef USE_STL
        if (i < list.size()) list[i].setNumberOfGrids(nG(i));
        else list.push_back(GenericGridCollection(nG(i)));
      #else
       if (i < list.getLength()) list[i].setNumberOfGrids(nG(i));
       else list.addElement(GenericGridCollection(nG(i)));
      #endif // USE_STL
      // *wdh      list[i]->numberOfBaseGrids = what & THEbaseGrid        ? 1 : numberOfBaseGrids;
      list[i]->numberOfBaseGrids = what & THEbaseGrid        ? 1 : 0;
      // *wdh      list[i]->numberOfRefinementLevels = what & THErefinementLevel ? 1 : numberOfRefinementLevels;
      list[i]->numberOfRefinementLevels = 1;
      // *wdh      list[i]->numberOfComponentGrids = what & THEcomponentGrid   ? 1 : numberOfComponentGrids;
      list[i]->numberOfComponentGrids = 0;
      list[i]->numberOfMultigridLevels = what & THEmultigridLevel  ? 1 : numberOfMultigridLevels;
      list[i]->numberOfDomains         = what & THEdomain          ? 1 : numberOfDomains;
      list[i]->computedGeometry = NOTHING;
    } // end for

    for (nG=k=0; k<numberOfGrids; k++) 
    {
      const Integer j = nG(i = number(k))++;
      list[i][j].reference(grid[k]);
      list[i].gridNumber(j)            = gridNumber(k);
      list[i].baseGridNumber(j)        = baseGridNumber(k);
      list[i].refinementLevelNumber(j) = refinementLevelNumber(k);
      list[i].componentGridNumber(j)   = componentGridNumber(k);
      list[i].multigridLevelNumber(j)  = multigridLevelNumber(k);
      list[i].domainNumber(j)          = domainNumber(k);
      const int mgLevel =  what & THEmultigridLevel ? i : 0;
      if( list[i].multigridLevelNumber(j)==mgLevel ) // *wdh*
        list[i]->numberOfComponentGrids++;
    } // end for

    if( what & THEdomain ) // *wdh* 071208 
    { // determine gridNumber, baseGridNumber, componentGridNumber for each domain 

      // domainGridNumber(k) : grid "k" in the entire collection is this grid-number in the domain 
      //                       gc[k] <-> gc.domain[domainGridNumber(k)]
      IntegerArray domainGridNumber(numberOfGrids); domainGridNumber=-1;
      nG=0;
      for( int k=0; k<numberOfGrids; k++ ) 
      {
	int d = number(k);             // domain number 
        domainGridNumber(k) = nG(d);   //  gc[k] <-> gc.domain[domainGridNumber(k)]

        int j = domainGridNumber(k);
        list[d].gridNumber(j) = j;

        assert( baseGridNumber(k)<=k );
        assert( number(baseGridNumber(k))==d );
        assert( number(componentGridNumber(k))==d );
        list[d].baseGridNumber(j) = domainGridNumber(baseGridNumber(k));
        list[d].componentGridNumber(j) = domainGridNumber(componentGridNumber(k));
	nG(d)++;

      }
    }

    // *wdh* :
    list[i]->numberOfBaseGrids=0;
    for (i=0; i<numberOfCollections; i++) 
    {
      if( !(what & THEmultigridLevel) )
	list[i]->numberOfMultigridLevels =max(list[i].multigridLevelNumber)+1;
      if( !(what & THEdomain) )
	list[i]->numberOfDomains =max(list[i].domainNumber)+1;
      if( !(what & THErefinementLevel) )
        list[i]->numberOfRefinementLevels =max(list[i].refinementLevelNumber)+1;
      if( !(what & THEbaseGrid) )
      {
        // count the number of distinct baseGrid's
        for( k=0; k<list[i]->numberOfGrids; k++ )
	{
	  bool newBaseGrid=TRUE;
	  for( int j=0; j<k; j++ )
	  {
	    if( list[i].baseGridNumber(k)==list[i].baseGridNumber(j) )
	    {
	      newBaseGrid=FALSE;
	      break;
	    }
	  }
	  if( newBaseGrid )
	    list[i]->numberOfBaseGrids++;
	}
      }
    }
  } // end if
  computedGeometry |= what & THElists;
  return what & THElists;
}



// =============================================================================
//  Initialize the GenericGridCollectionData class.
// =============================================================================
void GenericGridCollectionData::initialize(
  const Integer& numberOfGrids_) {

    GenericGridCollectionData::setNumberOfGrids(numberOfGrids_);
    for (Integer i=0; i<numberOfGrids_; i++) {
        gridNumber(i)            = i;
        baseGridNumber(i)        = i;
        refinementLevelNumber(i) = 0;
        componentGridNumber(i)   = i;
        multigridLevelNumber(i)  = 0;
        domainNumber(i)          = 0;
    } // end if
    numberOfBaseGrids        = numberOfGrids_;
    numberOfRefinementLevels = numberOfGrids_ ? 1 : 0;
    numberOfMultigridLevels  = numberOfGrids_ ? 1 : 0;
    numberOfDomains          = numberOfGrids_ ? 1 : 0;
    numberOfComponentGrids   = numberOfGrids_;
    gridCollectionType       = masterGridCollection;
    
    pLoadBalancer=NULL;
    keepGridDistributionOnCopy=false;  // used to copy a grid collection to a different parallel distrubution.
    
    destroy(~NOTHING & ~GenericGrid::EVERYTHING);
}
