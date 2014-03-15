#include "ReferenceCounting.h"
#include "ReferenceCountingList.h"


ReferenceCountingList::
ReferenceCountingList()
{ 
  start=NULL; 
  end=NULL; 
}

ReferenceCountingList:: 
~ReferenceCountingList()
{ // delete Items allocated by add
  ReferenceCountingItem *ptr1, *ptr2;
  ptr1=start;
  while( ptr1 )
  { 
    ptr2=ptr1;
    ptr1=ptr1->next;
    delete ptr2;
  }
}

void ReferenceCountingList:: 
add( ReferenceCounting *val, const int & identifier /* =-1 */ )
// ==========================================================================================
// /Description:
//   Add an item to the list, ie. add a pointer to the ReferenceCounting object.
// ==========================================================================================
{ 
  ReferenceCountingItem *ptr = new ReferenceCountingItem( val,identifier );
  if( start==NULL )
  { start=ptr; end=ptr;
  }
  else
  { end->next=ptr;
    end=ptr;
  }
}

int ReferenceCountingList:: 
remove( ReferenceCounting *val )
// ==========================================================================================
// /Description:
//   Remove an item from the list (ie. remove the pointer to the ReferenceCounting object)
// return 1 if deleted, 0 if not found
// ==========================================================================================
{ 
  if( start==NULL )
    return 0;
  // Since this is a singly linked list we must keep track of the
  // previous entry so we can delete an entry
  ReferenceCountingItem *ptr,*ptr1;
  ptr=start;
  // see if the first item in the list points to the same ReferenceCounting object
  if( ptr->val==val )
  {
    start=ptr->next;
    delete ptr;
    return 1;
  }    
  // Now check the rest of the list
  bool found=FALSE;
  while( ptr )
  {
    if( ptr->next->val==val )
    { // found! remove from the list and delete
      ptr1=ptr->next;      
      ptr->next=ptr->next->next;
      delete ptr1;
      found=TRUE;
      break;
    }
    ptr=ptr->next;
  }
  if( found )
    return 1;
  else
    return 0;
}

ReferenceCounting* ReferenceCountingList:: 
find( int identifier ) const
// ===============================================================================================
//  /Description:
//     Find an item with the given identifier. Return a NULL pointer if no item was found.
// ===============================================================================================
{
  ReferenceCounting *returnValue=NULL;
  if( start==NULL )
    return returnValue;
    
  ReferenceCountingItem *ptr;
  ptr=start;
  while( ptr )
  {
    if( ptr->id==identifier )
    {
      return ptr->val;
    }
    ptr=ptr->next;
  }
  return returnValue;
}
