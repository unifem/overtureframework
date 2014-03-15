//========================================================================================================
//    This file defines the functions for the ReferenceCountedClass Class
//
//========================================================================================================

#include "ReferenceCountedClass.h"

//========================================================================================================
// Default constructor
//========================================================================================================
ReferenceCountedClass::
ReferenceCountedClass ()
{
}

//========================================================================================================
// Copy constructor, deep copy by default
//========================================================================================================
ReferenceCountedClass::
ReferenceCountedClass( const ReferenceCountedClass & rcc, const CopyType copyType  )
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
// Destructor
//========================================================================================================
ReferenceCountedClass::
~ReferenceCountedClass ()
{
  if( rcData->decrementReferenceCount() == 0 )    // if there are no references, then
    delete rcData;                                // delete the reference counted data
}

//========================================================================================================
// Assign initial values to variables
//========================================================================================================
void ReferenceCountedClass::
initialize()  
{
  rcData = new ReferenceCountedClass::RCData;       // create a reference counted data object
  rcData->incrementReferenceCount();

  i.reference(rcData->i);                // make reference counted i reference the "true" i
  x.reference(rcData->x);                // make reference counted x reference the "true" x
  array.redim(3);
}

//========================================================================================================
//  Reference this object to another
//========================================================================================================
void ReferenceCountedClass::
reference( const ReferenceCountedClass & rcc )
{
  if( this==&rcc ) // no need to do anything in this case
    return;
  if( rcData->decrementReferenceCount() == 0 )
    ::delete rcData;   
  rcData=rcc.rcData;
  rcData->incrementReferenceCount();
  i.reference(rcc.i);
  x.reference(rcc.x);
  array.reference(rcc.array);
}

//========================================================================================================
//  break the reference that this object has with any other objects
//  after calling this function the object will have a separate copy of all member data
//========================================================================================================
void ReferenceCountedClass::
breakReference()
{
  // If there is only 1 reference, no need to make a new copy
  if( rcData->getReferenceCount() != 1 )
  {
    ReferenceCountedClass rcc = *this;  // makes a deep copy
    reference(rcc);                     // make a reference to this new copy
  }
}

//========================================================================================================
// Assignment with = is a deep copy
//========================================================================================================
ReferenceCountedClass & ReferenceCountedClass::
operator= ( const ReferenceCountedClass & rcc )
{
  *rcData=*rcc.rcData;     // deep copy of reference counted data
  i    =rcc.i;             // deep copy of the member data
  x    =rcc.x;
  array=rcc.array;
  return *this;
}


//========================================================================================================
// Default constructor for the reference counted data
//========================================================================================================
ReferenceCountedClass::RCData::
RCData()
{
}

//========================================================================================================
// Default destructor for the reference counted data
//========================================================================================================
ReferenceCountedClass::RCData::
~RCData()
{
}

//========================================================================================================
// Assignment with = is a deep copy
//========================================================================================================
ReferenceCountedClass::RCData::RCData& ReferenceCountedClass::RCData::
operator=(const ReferenceCountedClass::RCData & rcc )
{
  i=rcc.i;
  x=rcc.x;
}

