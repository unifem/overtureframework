#include "MappingRC.h"
#include "GenericDataBase.h"
#include "ReferenceCountingList.h"

//===============================================================================
// Reference Counted Class to hold a pointer to a mapping
//
//  Use this class to hold a pointer to a mapping
//
//
//  Notes:
//   o This class is necessary since the Mapping Class is NOT reference counted,
//     (We don't want to force Joe user to write a refernce counted class)
//   o This means that if some one gives us a Mapping to put into this container
//     class then this Mapping could possibly go out of scope and get deleted
//     before the destructor for this class is called. Therefore we keep the 
//     variable uncountedReferencesMayExist to tell us whether we are using
//     a Mapping that someone gave that may not be reference counted. We know
//     this situation exists by setting 
//        uncountedReferencesMayExist= mapping.uncountedReferencesMayExist()
//     If "mapping" is reference counted (i.e. the reference count is not zero) 
//     then uncountedReferencesMayExist==0 
//   o 
//====================================================================================

MappingRC::DataBaseModeEnum MappingRC::dataBaseMode=MappingRC::doNotLinkMappings;
// MappingRC::DataBaseModeEnum MappingRC::dataBaseMode=MappingRC::linkMappings;

//-----------------------------------------------------------------------------
// construct a mapping with the given Class name, standard Mapping by default
//-----------------------------------------------------------------------------
MappingRC::
MappingRC( const aString & mappingClassName )
{
  initialize( mappingClassName );
}         

//-----------------------------------------------------------------------------
// initialization routine :
//     construct a mapping with the given Class name, standard Mapping by default
//-----------------------------------------------------------------------------
void MappingRC::
initialize( const aString & mappingClassName )
{
//  cout << "Entering MappingRC:: initialize" << endl;
  
  if( mappingClassName==nullString ) // make a default mapping
  {
    mapPointer = new Mapping();
    assert(mapPointer!=0);
  }
  else // make a making of the specified type
  {
    mapPointer=Mapping::makeMapping( mappingClassName );
    if( mapPointer==NULL )
      mapPointer = new Mapping();  // make a default Mapping if we can't make the specified one
    assert(mapPointer!=0);
  }
  // *** wdh if( !mapPointer->uncountedReferencesMayExist() ) 
  mapPointer->incrementReferenceCount();   //
}  

//-----------------------------------------------------------------------------
// constructor, assign pointer to the given mapping
//-----------------------------------------------------------------------------
MappingRC::
MappingRC( Mapping & map ) 
{
  if( Mapping::debug & 4 )
    cout << " MappingRC::Constructor called" << endl;
  mapPointer=&map;
  if( !mapPointer->uncountedReferencesMayExist() ) 
    mapPointer->incrementReferenceCount();
}
  
//-----------------------------------------------------------------------------
// destructor
//-----------------------------------------------------------------------------
MappingRC::
~MappingRC()
{ 
  if( Mapping::debug & 4 )
    cout << " MappingRC::Destructor called" << endl;
  // Only delete the Mapping if we are sure that it is reference counted
  if( !mapPointer->uncountedReferencesMayExist() && mapPointer->decrementReferenceCount()==0 )
  {
     if( Mapping::debug & 4 )
       cout << " MappingRC::Destructor: mapPointer deleted. name=" << mapPointer->getName(Mapping::mappingName) << endl;
     delete mapPointer;
   }
//    cout << " MappingRC::Exiting destructor" << endl;
}

//-----------------------------------------------------------------------------
// ----- copy constructor, deep copy by default----
//-----------------------------------------------------------------------------
MappingRC::
MappingRC( const MappingRC & maprc, const CopyType copyType )
{
  if( copyType==DEEP )
  {
    // ** initialize( maprc.getClassName() );
    mapPointer= (Mapping*)
       ((ReferenceCounting*)maprc.mapPointer)->virtualConstructor(copyType);  // create a new Mapping, and copy
    mapPointer->incrementReferenceCount();
  }
  else
  {
    mapPointer=maprc.mapPointer;  
    if( !mapPointer->uncountedReferencesMayExist() )
      mapPointer->incrementReferenceCount();
  }
  
}
  
//-----------------------------------------------------------------------------
// ---Assignment operator : deep copy ---
//-----------------------------------------------------------------------------
MappingRC & MappingRC::
operator=( const Mapping & map )
{
  MappingRC maprc((Mapping&)map);
  return operator=(maprc);   
}

//-----------------------------------------------------------------------------
// ---Assignment operator : deep copy ---
//-----------------------------------------------------------------------------
MappingRC & MappingRC::
operator=( const MappingRC & maprc )
{
  if( this==&maprc ) // *wdh 961204
    return *this;

  assert(mapPointer!=0);
  if( mapPointer->getClassName() != maprc.mapPointer->getClassName() )
  {
    MappingRC newMaprc(maprc);  // make a new copy (this is a deep copy through the copy constructor)
    reference( newMaprc );      // reference to this new copy
  }
  else
  {
    (ReferenceCounting&)*mapPointer=(ReferenceCounting&)*(maprc.mapPointer); // use virtual = for a deep copy
  }
  return *this;
}

  
//-----------------------------------------------------------------------------
//  reference to another MappingRC
//-----------------------------------------------------------------------------
void MappingRC::
reference( const MappingRC & maprc )
{
  if( this != &maprc )
  {
    if( !mapPointer->uncountedReferencesMayExist() && mapPointer->decrementReferenceCount()==0 )
    {
      if( Mapping::debug & 4 )
        cout << " MappingRC::reference: mapPointer deleted " << endl;
      delete mapPointer;
    }
    mapPointer= maprc.mapPointer;
    if( !mapPointer->uncountedReferencesMayExist() )
      mapPointer->incrementReferenceCount(); 

    if( Mapping::debug & 4 )
      cout << " MappingRC::reference(mapRC): mappingName=" << getName(Mapping::mappingName)
	   << ", reference count = " << mapPointer->getReferenceCount() << endl;
  }
}

//-----------------------------------------------------------------------------
//  reference to a Mapping
//-----------------------------------------------------------------------------
void MappingRC::
reference( const Mapping & map )
{
  // Note that if uncountedReferencesMayExist!=0 then we shouldn't assume that the
  // mapPointer is valid (The Mapping may already be deleted)
  if( mapPointer != &map )
  {
    if( !mapPointer->uncountedReferencesMayExist() && mapPointer->decrementReferenceCount()==0 )
    {
      if( Mapping::debug & 4 )
        cout << " MappingRC::reference: mapPointer deleted" << endl;
      delete mapPointer;
    }
    mapPointer= (Mapping *)&map;  // cast away const !

    if( !mapPointer->uncountedReferencesMayExist() )
      mapPointer->incrementReferenceCount(); 

    if( Mapping::debug & 4 )
      cout << " MappingRC::reference(map): mappingName=" << getName(Mapping::mappingName)
	   << ", reference count = " << mapPointer->getReferenceCount() << endl;
  }
}

//-----------------------------------------------------------------------------------
// break reference
//---------------------------------------------------------------------------------
void MappingRC::
breakReference()
{
  // if there is only one reference, no need to make a new copy
  if( mapPointer->getReferenceCount() != 1 )
  {
    MappingRC maprc = *this;  // make a copy
    reference( maprc );       // reference to this new copy
  }
}  


//-----------------------------------------------------------------------------
// This function is used to create a new member of the Class provided the
// mappingClassName is equal to the name of the class
//-----------------------------------------------------------------------------
Mapping *MappingRC::
make( const aString & mappingClassName )
{
  return mapPointer->make( mappingClassName );
}
  
//-----------------------------------------------------------------------------------
// Map the domain r to the range x
//-----------------------------------------------------------------------------------
void MappingRC::
map( const realArray & r, realArray & x, realArray &xr, MappingParameters & params  )
{
  mapPointer->map( r,x,xr,params);
}
  
void MappingRC::
mapGrid( const realArray & r, realArray & x, realArray &xr, MappingParameters & params  )
{
  mapPointer->mapGrid( r,x,xr,params);
}
  

//-----------------------------------------------------------------------------------
// Map the range x back to the domain r
//-----------------------------------------------------------------------------------
void MappingRC::
inverseMap( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  mapPointer->inverseMap( x,r,rx,params );
}

void MappingRC::
basicInverse( const realArray & x, realArray & r, realArray & rx, MappingParameters & params )
{
  mapPointer->basicInverse( x,r,rx,params );
}



void MappingRC::
mapC( const realArray & r, const realArray & x, const realArray &xr, MappingParameters & params  )
{
  mapPointer->map( r,(realArray&)x,(realArray&)xr,params);  // cast away const
}
  

void MappingRC::
inverseMapC( const realArray & x, const realArray & r, const realArray & rx, MappingParameters & params )
{
  mapPointer->inverseMap( x,(realArray&)r,(realArray&)rx,params );  // cast away const
}

        
real MappingRC::
sizeOf(FILE *file /* = NULL */ ) const
{
  return mapPointer->sizeOf(file);
}


// Map the domain r to the range x
void MappingRC::
mapS( const RealArray & r, RealArray & x, RealArray &xr, MappingParameters & params)
{
  mapPointer->mapS(r,x,xr,params);
}


  // Map the range x back to the domain r
void MappingRC::
inverseMapS( const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params)
{
  mapPointer->inverseMapS(x,r,rx,params);
  
}

void MappingRC::
basicInverseS(const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params)
{
  mapPointer->basicInverseS(x,r,rx,params);
}

void MappingRC::
mapCS( const RealArray & r, const RealArray & x, const RealArray &xr, MappingParameters & params)
{
  mapPointer->mapS( r,(RealArray&)x,(RealArray&)xr,params);  // cast away const
}

void MappingRC::
inverseMapCS( const RealArray & x, const RealArray & r, const RealArray & rx,
	      MappingParameters & params)
{
  mapPointer->inverseMapS( x,(RealArray&)r,(RealArray&)rx,params );  // cast away const
}


void MappingRC::
mapGridS(const RealArray & r, RealArray & x, RealArray & xr, MappingParameters & params )
{
  mapPointer->mapGridS(r,x,xr,params);
}


void MappingRC::
inverseMapGridS(const RealArray & x, RealArray & r, RealArray & rx, MappingParameters & params )
{
  mapPointer->inverseMapGridS(x,r,rx,params);
}




//-----------------------------------------------------------------------------------
// Update mapping, change parameters interactively
//-----------------------------------------------------------------------------------
void MappingRC::
update( MappingInformation & mapInfo )
{
  mapPointer->update( mapInfo );
}

//-----------------------------------------------------------------------------------
// display
//-----------------------------------------------------------------------------------
void MappingRC::
display( const aString & label ) const
{
  mapPointer->display( label );
}
  

int MappingRC::
checkMapping()
{
  return mapPointer->checkMapping();
}


//--------------Access Functions----------------------------- 

aString MappingRC::
getClassName() const
{
// ****  return className;
  return mapPointer->getClassName();
}
  
int MappingRC::
getCoordinateEvaluationType( const Mapping::coordinateSystem type ) const
{
  return mapPointer->getCoordinateEvaluationType( type );
}

int MappingRC::
getDomainDimension() const
{
  return mapPointer->getDomainDimension();
}
int MappingRC::
getRangeDimension() const
{
  return mapPointer->getRangeDimension();
}

Mapping::
basicInverseOptions MappingRC::getBasicInverseOption() const
{
  return mapPointer->getBasicInverseOption();
}

int MappingRC::
getBoundaryCondition( const int side, const int axis ) const
{
  return mapPointer->getBoundaryCondition( side,axis );
}
int MappingRC::
getShare( const int side, const int axis ) const
{
  return mapPointer->getShare( side,axis );
}
real MappingRC::
getSignForJacobian() const
{
  return mapPointer->getSignForJacobian();
}
Mapping::periodicType MappingRC::
getIsPeriodic( const int axis ) const
{
  return mapPointer->getIsPeriodic( axis );
}
int MappingRC::
getGridDimensions( const int axis ) const
{
  return mapPointer->getGridDimensions( axis );
}
const realArray& MappingRC::
getGrid(MappingParameters & params )
{
  return mapPointer->getGrid(params);
}

int MappingRC::
getID() const
{
  return mapPointer->getID();
}


int MappingRC::
getInvertible() const
{
  return mapPointer->getInvertible();
}
real MappingRC::
getPeriodVector( const int axis, const int direction ) const
{
  return mapPointer->getPeriodVector( axis,direction );
}
Mapping::topologyEnum MappingRC::
getTopology( const int side, const int axis ) const
{
  return mapPointer->getTopology(side,axis);
}
Mapping::coordinateSingularity MappingRC::
getTypeOfCoordinateSingularity( const int side, const int axis ) const
{
  return mapPointer->getTypeOfCoordinateSingularity( side,axis );
}
Mapping::mappingSpace MappingRC::
getDomainSpace() const
{
  return mapPointer->getDomainSpace();
}
Mapping::mappingSpace MappingRC::
getRangeSpace() const
{
  return mapPointer->getRangeSpace();
}
Mapping::coordinateSystem MappingRC::
getDomainCoordinateSystem() const
{
  return mapPointer->getDomainCoordinateSystem();
}
Mapping::coordinateSystem MappingRC::
getRangeCoordinateSystem() const
{
  return mapPointer->getRangeCoordinateSystem();
}
Bound MappingRC::
getDomainBound( const int side, const int axis ) const
{
  return mapPointer->getDomainBound( side,axis );
}
Bound MappingRC::
getRangeBound( const int side, const int axis ) const
{
  return mapPointer->getRangeBound( side,axis );
}
Bound MappingRC::
getDomainCoordinateSystemBound( const int side, const int axis ) const
{
  return mapPointer->getDomainCoordinateSystemBound( side,axis );
}
Bound MappingRC::
getRangeCoordinateSystemBound( const int side, const int axis ) const
{
  return mapPointer->getRangeCoordinateSystemBound( side,axis );
}


// --------------set functions-------------------------

// Set or get names such as "mappingName", "domainAxis1Name", etc
void MappingRC::
setName( const Mapping::mappingItemName item, const aString & name )
{
  mapPointer->setName( item,name );
}

aString MappingRC::
getName( const Mapping::mappingItemName item ) const
{
  return mapPointer->getName( item );
}
  
void MappingRC::
setDomainDimension( const int domainDimension )
{
  mapPointer->setDomainDimension( domainDimension );
}
void MappingRC::
setRangeDimension( const int rangeDimension )
{
  mapPointer->setRangeDimension( rangeDimension );
}
void MappingRC::
setBasicInverseOption( const basicInverseOptions option )
{
  mapPointer->setBasicInverseOption( option );
}
void MappingRC::
setBoundaryCondition( const int side, const int axis, const int bc )
{
  mapPointer->setBoundaryCondition( side,axis,bc );
}
void MappingRC::
setSignForJacobian( const real signForJac )
{
  mapPointer->setSignForJacobian(signForJac);
}
void MappingRC::
setShare( const int side, const int axis, const int share )
{
  mapPointer->setShare( side,axis,share );
}
void MappingRC::
setIsPeriodic( const int axis, const Mapping::periodicType isPeriodic )
{
  mapPointer->setIsPeriodic( axis,isPeriodic );
}
void MappingRC::
setGridDimensions( const int axis, const int dim )
{
  mapPointer->setGridDimensions( axis,dim );
}
void MappingRC::
setGrid(realArray & grid, IntegerArray & gridIndexRange)
{
  mapPointer->setGrid(grid,gridIndexRange);
}

void MappingRC::
setInvertible( const int invertible )
{
  mapPointer->setInvertible( invertible );
}
void MappingRC::
setID() 
{
  mapPointer->setID();
}

void MappingRC::
setPeriodVector( const int axis, const int direction, const real periodVectorComponent )
{
  mapPointer->setPeriodVector( axis,direction,periodVectorComponent );
}
void MappingRC::
setDomainSpace( const Mapping::mappingSpace domainSpace )
{
  mapPointer->setDomainSpace( domainSpace );
}
void MappingRC::
setRangeSpace( const Mapping::mappingSpace rangeSpace ) 
{
  mapPointer->setRangeSpace( rangeSpace );
}
void MappingRC::
setDomainCoordinateSystem( const Mapping::coordinateSystem domainCoordinateSystem )
{
  mapPointer->setDomainCoordinateSystem( domainCoordinateSystem );
}
void MappingRC::
setRangeCoordinateSystem( const Mapping::coordinateSystem rangeCoordinateSystem )
{
  mapPointer->setRangeCoordinateSystem( rangeCoordinateSystem );
}
void MappingRC::
setDomainBound( const int side, const int axis, const Bound domainBound )
{
  mapPointer->setDomainBound( side,axis,domainBound );
}
void MappingRC::
setRangeBound( const int side, const int axis, const Bound rangeBound )
{
  mapPointer->setRangeBound( side,axis,rangeBound );
}
void MappingRC::
setDomainCoordinateSystemBound( const int side, const int axis,
    const Bound domainCoordinateSystemBound )
{
  mapPointer->setDomainCoordinateSystemBound( side,axis,domainCoordinateSystemBound );
}
void MappingRC::
setRangeCoordinateSystemBound( const int side, const int axis,
    const Bound rangeCoordinateSystemBound )
{
  mapPointer->setRangeCoordinateSystemBound( side,axis,rangeCoordinateSystemBound );
}
void MappingRC::
setCoordinateEvaluationType( const Mapping::coordinateSystem type, const int trueOrFalse )
{
  mapPointer->setCoordinateEvaluationType(type,trueOrFalse);
}

void MappingRC::
setTopology( const int side, const int axis, const Mapping::topologyEnum topo )
{
  mapPointer->setTopology(side,axis,topo);
}


void MappingRC::
setTypeOfCoordinateSingularity( const int side, const int axis,
                                                const Mapping::coordinateSingularity type )
{
  mapPointer->setTypeOfCoordinateSingularity( side,axis,type);
}

void MappingRC::
useRobustInverse(const bool trueOrFalse /* =TRUE */ )
{
  mapPointer->useRobustInverse(trueOrFalse);
}

bool MappingRC::
usesDistributedInverse() const
{
  return mapPointer->usesDistributedInverse();
}

int MappingRC::
get( const GenericDataBase & dir, const aString & name, Partitioning_Type *partition /* =NULL */ )
{
//  cout << "entering MappingRC:: get" << endl;
  
  GenericDataBase & subDir = *dir.virtualConstructor();
  dir.find(subDir,name,"Mapping");


  subDir.setMode(GenericDataBase::streamInputMode);

  // Look for the className of the Mapping:
  aString mappingClassName;
  subDir.get( mappingClassName,"classNameRC" );

  bool found=false;
  if( false )
  {
    // *** use the new link function 

    // first check to see if the Mapping has already been saved
    // somewhere else on the database.
    int dataBaseID=0;
    subDir.get( dataBaseID,"dataBaseID" );
    if( dataBaseMode==linkMappings && dataBaseID!=0 )
    {
      assert( dir.getList()!=NULL );
      ReferenceCountingList & rcList = *dir.getList();
      const ReferenceCounting *rc = rcList.find(dataBaseID);
      if( rc!=NULL )
      {
	found=true;
	if( Mapping::debug & 2 )
	  printf("MappingRC::get:INFO: found the Mapping with dataBaseID=%i already exists\n",dataBaseID);
	mapPointer=(Mapping*)rc;
	mapPointer->incrementReferenceCount();
      }
      else
      {
	if( Mapping::debug & 4 )
	  printf("MappingRC::get:INFO: unable to find the Mapping with dataBaseID=%i...build it..\n",dataBaseID);
	((GenericDataBase &)dir).build(subDir,dataBaseID);
	// throw "error";
      }
    }
  }
  
  if( !found )
  {

    // Try to make an instance of the appropriate derived Mapping class
    if( mapPointer==NULL || mapPointer->getClassName()!=mappingClassName )
    {
      Mapping *newMapPointer = Mapping::makeMapping( mappingClassName );
      if( newMapPointer!=NULL )
      { // a new mapping was made
	if(mapPointer && !mapPointer->uncountedReferencesMayExist() && 
	   mapPointer->decrementReferenceCount()==0 )
	{
	  if( Mapping::debug & 4 )
	    cout << " MappingRC::get: mapPointer deleted" << endl;
	  delete mapPointer;
	}
	mapPointer=newMapPointer;
	mapPointer->incrementReferenceCount(); 
      }
      else
      {
	cout << "MappingRC:get:Error: unable to make the mapping, name=" << (const char *) name
	     << ", className =" << (const char *) mappingClassName << endl;
	return 1;  // unable to make the Mapping, return an error code
      }
    }
    // We optionally set the partition for the mapping. *wdh* 2011/08/20
    if( partition!=NULL )
    {
      if( false )
        printF("MappingRC::get: Setting a partition\n");
      if( false )
      {
	const intSerialArray & processorSet = partition->getProcessorSet();
	printF("MappingRC::get: partition -> processors=[%i,%i]\n",processorSet.getBase(0),processorSet.getBound(0));
      }

      mapPointer->setPartition(*partition);

    }
    
    mapPointer->get( subDir,"." );   // get the mapping
    
  }
  delete &subDir;
  return 0; 
}

int MappingRC::
put( GenericDataBase & dir, const aString & name) const
// ==============================================================================
// /Description:
//    Save a MappingRC to a database file. 
//  If the Mapping has already been saved by someone else then it will exist
// in the ReferenceCountingList that is a member of the GenericDataBase. In this
// case we only need to save the identification number for the Mapping.
// =============================================================================
{
  if( mapPointer==NULL )
    return 1;

  GenericDataBase & subDir = *dir.virtualConstructor();      // create a derived data-base object
  dir.create(subDir,name,"Mapping");                      // create a sub-directory 
  // save the mapping as a stream of data by default, this is more efficient
  subDir.setMode(GenericDataBase::streamOutputMode);

  subDir.put( mapPointer->getClassName(),"classNameRC" );  // save a special class name (for stream mode)

  if( false )
  {
    // this is obsolete : use the new link function.
    assert( dir.getList()!=NULL );
    ReferenceCountingList & rcList = *dir.getList();

    if( dataBaseMode==linkMappings && rcList.find(mapPointer->getID())!=NULL )
    {
      // mapping already exists on the data base
      if( Mapping::debug & 2 )
	printf("MappingRC::put: Mapping already has been put, id=%i\n",mapPointer->getID());
    
      subDir.put(mapPointer->getID(),"dataBaseID");
      // we should also save enough info so we could find the existing Mapping on the database.
      // this would be needed if we didn't read the Mapping's in the same order.
    }
    else
    {
      // Mapping does not yet exist
      int dataBaseID=subDir.getID(); // this will be the data base ID for the Mapping.
      subDir.put(dataBaseID,"dataBaseID");
      if( Mapping::debug & 2 ) printf("MappingRC::put: dataBaseID=%i\n",dataBaseID);

      mapPointer->put( subDir,"." );
    }

    if( Mapping::debug & 2 )
      printf("MappingRC::put:INFO id=%i, dataBaseMode=%i (1==link mode)\n",mapPointer->getID(),dataBaseMode);
  }
  else
  {
    mapPointer->put( subDir,"." );
  }
  
  delete &subDir;
  return 0;
}


  // return a reference to the Mapping, an error occurs if there is no Mapping
Mapping & MappingRC::
getMapping() const
{
  if( mapPointer==NULL )
  {
    cout << "MappingRC::getMapping: ERROR: there is no mapping in this MappingRC \n";
    {throw "error";}
  }
  return *mapPointer;
}

int MappingRC::
setDataBaseMode(DataBaseModeEnum mode)
// =========================================================================
// /Description:
//    Set the mode for the data base. This option indicates whether we should
// attempt to save space in the database.
// \begin{description}
//    \item[doNotLinkMappings] : default mode, do not attempt to save space.
//    \item[linkMappings] : attempt to save space. Do not save duplictate copies
//      of the same Mapping (which might occur if Mapping's are composed etc.)
// ========================================================================
{
  dataBaseMode=mode;
  return 0;
}

MappingRC::DataBaseModeEnum MappingRC::
getDataBaseMode() 
// =========================================================================
// /Description:
//    Get the mode for the data base. 
// ===========================================================================
{
  return dataBaseMode;
}

