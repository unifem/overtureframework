#include "MultigridCompositeGrid.h"

//========================================================================================================
/// MultigridCompositeGrid : holds the CompositeGrid with multigrid hierarchy so
///   that multiple solvers can share the same multigrid hierarchy.
//========================================================================================================


//========================================================================================================
// /\brief Default constructor for the MultigridCompositeGrid class which holds the CompositeGrid 
///   multigrid hierarchy so that multiple solvers can share the same multigrid hierarchy.
//========================================================================================================
MultigridCompositeGrid::
MultigridCompositeGrid ()
{
  initialize();
}

//========================================================================================================
// Copy constructor, deep copy by default
//========================================================================================================
MultigridCompositeGrid::
MultigridCompositeGrid( const MultigridCompositeGrid & rcc, 
                        const CopyType copyType  /*= DEEP */ )
{
  if( copyType==DEEP )  
  {
    initialize();            // put "this" object into a valid state
    (*this)=rcc;             // this is a deep copy
  }
  else
  {
    rcData=rcc.rcData;                   // set pointer to reference counted data
    rcData->incrementReferenceCount();   
    reference(rcc);                      // reference this object to rcc
  }
}

//========================================================================================================
/// \brief Destructor.
//========================================================================================================
MultigridCompositeGrid::
~MultigridCompositeGrid ()
{
  if( rcData->decrementReferenceCount() == 0 )    // if there are no references, then
    delete rcData;                                // delete the reference counted data
}

// ========================================================================================================
/// \brief Allocate space for the CompositeGrid
// ========================================================================================================
int MultigridCompositeGrid::
allocate()
{
  assert( rcData!=NULL );
  if( rcData->pcg==NULL )
  {
    rcData->pcg = new CompositeGrid;
    rcData->pcg->incrementReferenceCount();
  }
  
  return 0;
}


//========================================================================================================
/// \brief Specify whether the multigrid hierarchy is up to date. This is used as a mechanism
/// to know when the multigrid hierachy needs to be regenerated. 
//========================================================================================================
void MultigridCompositeGrid::
setGridIsUpToDate( bool trueOrFalse /* =true  */ )
{ 
   rcData->isUpToDate=trueOrFalse; 
}


//========================================================================================================
/// \brief Update the grid to match a new CompositeGrid.
//========================================================================================================
void MultigridCompositeGrid::
updateToMatchGrid( CompositeGrid & cg )
{ 
  // allocate the CompositeGrid if necessary
  allocate(); 

  rcData->pcg->reference(cg);
  rcData->isUpToDate=false;
}


//========================================================================================================
/// \brief Assign initial values to variables.
//========================================================================================================
void MultigridCompositeGrid::
initialize()  
{
  rcData = new MultigridCompositeGrid::MultigridCompositeGridData;       // create a reference counted data object
  rcData->isUpToDate=false;
  rcData->incrementReferenceCount();
}

//========================================================================================================
/// \brief Reference this object to another.
//========================================================================================================
void MultigridCompositeGrid::
reference( const MultigridCompositeGrid & rcc )
{
  if( this==&rcc ) // no need to do anything in this case
    return;
  if( rcData->decrementReferenceCount() == 0 )
    ::delete rcData;   
  rcData=rcc.rcData;
  rcData->incrementReferenceCount();
}

//========================================================================================================
/// \brief Break the reference that this object has with any other objects
///  after calling this function the object will have a separate copy of all member data.
//========================================================================================================
void MultigridCompositeGrid::
breakReference()
{
  // If there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    MultigridCompositeGrid rcc = *this;  // makes a deep copy
    reference(rcc);                      // make a reference to this new copy
  }
}

//========================================================================================================
// Assignment with = is a deep copy
//========================================================================================================
MultigridCompositeGrid & MultigridCompositeGrid::
operator= ( const MultigridCompositeGrid & rcc )
{
  *rcData=*rcc.rcData;     // deep copy of reference counted data
  return *this;
}


//========================================================================================================
// Default constructor for the reference counted data
//========================================================================================================
MultigridCompositeGrid::MultigridCompositeGridData::
MultigridCompositeGridData()
{
  pcg=NULL;
}

//========================================================================================================
// Default destructor for the reference counted data
//========================================================================================================
MultigridCompositeGrid::MultigridCompositeGridData::
~MultigridCompositeGridData()
{
  if( pcg!=NULL && pcg->decrementReferenceCount()==0 )
    delete pcg;
  
}

//========================================================================================================
// Assignment with = is a semi-deep copy (CompositeGrid is shared)
//========================================================================================================
MultigridCompositeGrid::MultigridCompositeGridData& 
MultigridCompositeGrid::MultigridCompositeGridData::operator=(const MultigridCompositeGrid::MultigridCompositeGridData & rcc )
{
  if( pcg!=NULL && pcg->decrementReferenceCount()==0 )
    delete pcg;

  pcg=rcc.pcg;
  if( pcg!=NULL ) pcg->incrementReferenceCount();

  isUpToDate=rcc.isUpToDate;

  return *this;
  
}
