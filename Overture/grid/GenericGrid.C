//
// Who to blame:  Geoff Chesshire and WDH
//

#include "GenericGrid.h"

#ifdef USE_STL
#include "RCVector.h"
RCVector_STATIC_MEMBER_DATA(GenericGrid)
#endif // USE_STL

//
// class GenericGrid:
//
// Public member functions:
//
// Default constructor.
//
// Create a GenericGrid.
//
GenericGrid::GenericGrid():
  ReferenceCounting() {
    className = "GenericGrid";
    rcData = new GenericGridData;
    isCounted = LogicalTrue;
    rcData->incrementReferenceCount();
    updateReferences();
}
//
// Copy constructor.  (Does a deep copy by default.)
//
GenericGrid::GenericGrid(
  const GenericGrid& x,
  const CopyType     ct):
  ReferenceCounting(x, ct) {
    className = "GenericGrid";
    switch (ct) {
      case DEEP:
      case NOCOPY:
        rcData = (GenericGridData*)
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
GenericGrid::~GenericGrid()
  { if (isCounted && rcData->decrementReferenceCount() == 0) delete rcData; }
//
// Assignment operator.  (Does a deep copy.)
//
GenericGrid& GenericGrid::
operator=(const GenericGrid& x) 
{
  //  ReferenceCounting::operator=(x);
  if (rcData != x.rcData) 
  {
    if (rcData->getClassName() == x.rcData->getClassName())
    {
      (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;
      updateReferences();
    } 
    else
    {
      GenericGrid& y =
	*(GenericGrid*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
  } // end if
  return *this;
}


// ====================================================================================
// /Description:
//    Equals operator plus options. This version is used when copying a GridCollection
// that has AMR grids -- in which case we do not want to make a depp copy of the Mapping.
//
//   /options (input): (options & 2)==1 : do NOT copy the mapping 
// ====================================================================================
GenericGrid& GenericGrid::
equals(const GenericGrid& x, int option /* =0 */ ) 
{
  //  ReferenceCounting::operator=(x);
  if (rcData != x.rcData) 
  {
    if (rcData->getClassName() == x.rcData->getClassName())
    {
      // (ReferenceCounting&)*rcData = (ReferenceCounting&)*x.rcData;

      rcData->equals(*x.rcData,option);  // call the virtual function equals
      
      updateReferences();
    } 
    else
    {
      GenericGrid& y = *(GenericGrid*)x.virtualConstructor();
      reference(y); delete &y;
    } // end if
  } // end if
  return *this;
}

//
// Make a reference.  (Does a shallow copy.)
//
void GenericGrid::
reference(const GenericGrid& x) {
    ReferenceCounting::reference(x);
    if (rcData != x.rcData) {
        if (isCounted && rcData->decrementReferenceCount() == 0)
          delete rcData;
        rcData = x.rcData;
        isCounted = x.isCounted;
        if (isCounted) rcData->incrementReferenceCount();
        updateReferences();
    } // end if
}
void GenericGrid::reference(GenericGridData& x) {
    if (rcData != &x) {
        if (rcData->decrementReferenceCount() == 0) delete rcData;
        rcData = &x;
        isCounted = !x.uncountedReferencesMayExist();
        if (isCounted) rcData->incrementReferenceCount();
        updateReferences();
    } // end if
}
//
// Break a reference.  (Replaces with a deep copy.)
//
void GenericGrid::breakReference() {
//  ReferenceCounting::breakReference();
    if (!isCounted || rcData->getReferenceCount() != 1) {
        GenericGrid x = *this; // Uses the (deep) copy constructor.
        reference(x);
    } // end if
}
//
// Check that the data structure is self-consistent.
//
void GenericGrid::consistencyCheck() const {
    ReferenceCounting::consistencyCheck();
    if (rcData == NULL) {
        cerr << className << "::consistencyCheck():  "
             << "rcData == NULL for "
             << getClassName() << " " << getGlobalID() << "." << endl;
        assert(rcData != NULL);
    }
    rcData->consistencyCheck();
}
//
//  "Get" and "put" database operations.
//
Integer GenericGrid::
get(const GenericDataBase& db,
    const aString&         name,
    bool getMapping /* =true */)
{
    Integer returnValue = rcData->get(db, name,getMapping);
    updateReferences();
    return returnValue;
}
Integer GenericGrid::
put( GenericDataBase& db,
     const aString&   name,
     bool putMapping /* = true */,
     int geometryToPut /* = -1  */ 
   ) const
// geometryToPut : specify which geometry to put, by default put computedGeometry
{ 
  return rcData->put(db, name,putMapping,geometryToPut); 
}
//
//  Specify the set of processes over which GridFunctions are distributed.
//  We now support only the specification of a contiguous range of process IDs.
//
void GenericGrid::specifyProcesses(const Range& range)
  { rcData->specifyProcesses(range); }


//
// Set references to reference-counted data.
//
void GenericGrid::updateReferences(const Integer /* what */) { }
//
// Update the grid, sharing the data of another grid.
//
Integer GenericGrid::update(
  GenericGrid&  x,
  const Integer what,
  const Integer how) {
    Integer upd = rcData->update(*((GenericGrid&)x).rcData, what, how);
    updateReferences(what);
    return upd;
}
//
// Destroy optional grid data.
//
void GenericGrid::destroy(const Integer what) {
    rcData->destroy(what);
    updateReferences();
}
//
//  Initialize the GenericGrid.
//
void GenericGrid::initialize() { rcData->initialize(); }
//
// Stream output operator.
//
ostream& operator<<(ostream& s, const GenericGrid& g) {
    s << (ReferenceCounting&)g << endl;
    char _[80];
    sprintf(_, "  computedGeometry()                =  %o (octal)",
      (unsigned)g.computedGeometry());
    return s << _;
}

//
// class GenericGridData:
//
GenericGridData::GenericGridData():
  ReferenceCounting() {
    className = "GenericGridData";
    initialize();
}
GenericGridData::GenericGridData(
  const GenericGridData& x,
  const CopyType         ct):
  ReferenceCounting() {
    className = "GenericGridData";
    initialize();
    if (ct != NOCOPY) *this = x;
}

GenericGridData::
~GenericGridData() 
{ }

GenericGridData& GenericGridData::
operator=(const GenericGridData& x) 
{
  ReferenceCounting::operator=(x);
  computedGeometry = NOTHING;
  return *this;
}

GenericGridData& GenericGridData::
equals(const GenericGridData& x, const int option /* = 0 */) 
{
  ReferenceCounting::operator=(x);
  computedGeometry = NOTHING;
  return *this;
}

void GenericGridData::reference(const GenericGridData& x)
  { ReferenceCounting::reference(x); }
void GenericGridData::breakReference()
  { ReferenceCounting::breakReference(); }
void GenericGridData::consistencyCheck() const
  { ReferenceCounting::consistencyCheck(); }

Integer GenericGridData::
get(const GenericDataBase& db,
    const aString&         name,
    bool getMapping /* =true */ )   // for AMR grids we may not get the mapping.
{
  Integer returnValue = 0;
  GenericDataBase& dir = *db.virtualConstructor();
  db.find(dir, name, getClassName());
  initialize();
  returnValue |= dir.get(computedGeometry, "computedGeometry");
  const Integer computedGeometry0 = computedGeometry;
  GenericGridData::update(*this,
			  computedGeometry & EVERYTHING, COMPUTEnothing);
  computedGeometry = computedGeometry0;
  delete &dir;
  return returnValue;
}
Integer GenericGridData::
put(GenericDataBase& db,
    const aString&   name,
    bool putMapping /* = true */,
    int geometryToPut /* = -1  */ 
  ) const 
// geometryToPut : by default put computedGeometry
{
    Integer returnValue = 0;
    if( geometryToPut==-1 ) geometryToPut=computedGeometry;

    GenericDataBase& dir = *db.virtualConstructor();
    db.create(dir, name, getClassName());
    returnValue |= dir.put(geometryToPut, "computedGeometry");
    delete &dir;
    return returnValue;
}

void GenericGridData::
specifyProcesses(const Range& range)
{ 
  printf("GenericGridData::specifyProcesses:ERROR: base class function called!\n");
}


Integer GenericGridData::update(
  GenericGridData& /*x*/,
  const Integer    what,
  const Integer    /*how*/) {
// *wdh    if (what & INT_MIN) cerr << "Warning:  update() was called with default arguments.  This is not recommended." << endl;
    return NOTHING;
}
void GenericGridData::destroy(const Integer what)
  { computedGeometry &= ~what; }
void GenericGridData::geometryHasChanged(const Integer what)
  { computedGeometry &= ~what; }
void GenericGridData::initialize()
  { computedGeometry = NOTHING; destroy(~NOTHING); }
