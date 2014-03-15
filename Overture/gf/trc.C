#include "A++.h"
#include "ListOfReferenceCountedObjects.h"
#include "mathutil.h"
#include "PointerTypes.h"

typedef floatArray RealArray;

//-------------------------------------------------------------------
// Add a member to the list that is created on the stack
//-------------------------------------------------------------------
void func( ListOfReferenceCountedObjects<RealArray> & list )
{
  RealArray a(3,3);
  a=123.;
  list.addElement( a,1 );
}


//================================================================================
//  Test the ListOfReferenceCountedObjects Class
//
//================================================================================


void main()
{
  MemoryManagerType memoryManager;  // This will delete allocated memory at the end

  cout << "Test of the list class " << endl;
  
  int n=2;
  intP nP;
  nP.reference(n);
  nP=0;
  nP = nP==1 ? 5 : int(nP) ;

  ListOfReferenceCountedObjects<RealArray> list,list2;

  RealArray a(1),b(2),c(3),d(4),e(5),f;
  a=1.; b=2.; c=3.; d=4.; e=5.;
  
  
  list.addElement();
  list[0].reference(a);
  a.display("here is a");
  list[0].display("here is list[0]");
  list.addElement();
  list[1]=b;
  list[1]=6.;
  b.display("here is b");
  list[1].display("here is list[1]");
  list.deleteElement( 0 );
  list[0].display("here is list[0] after deleting element 0");

  list.addElement(f);  // add an undimensioned A++ array
    
  int i;
  
  cout << "add 150 items" << endl;
  for( i=0; i<150; i++ )
    list.addElement();

  cout << "remove 150 items " << endl;
  for( i=0; i<150; i++ )
    list.deleteElement();

  for( i=0; i<10; i++ )
    list.addElement();
  
  list[0].reference(a);
  list[1].reference(b);
  list[2]=c;
  list[3]=d;
  list[4].reference(e);
  
  list.swapElements( 1,3);
  list[1].display(" list[1] after swap(1,3)");
  list[3].display(" list[3] after swap(1,3)");
  
  list2.reference(list);
  cout << "after list2.reference(list): " << endl;
  cout << " list2.rcData->getReferenceCount = "
       <<   list2.rcData->getReferenceCount() << endl;
  cout << " list.rcData->getReferenceCount = "
       <<   list.rcData->getReferenceCount() << endl;
  
  list2[1].display("list2[1] after list2.reference(list)");
  if( max(list2[1]-list[1])>0. )
    cout << "ERROR max(list2[1]-list[1]) = " << max(list2[1]-list[1]) << endl;

  list.breakReference();
  cout << "after list.breakReference(): " << endl;
  cout << " list2.rcData->getReferenceCount = "
       <<   list2.rcData->getReferenceCount() << endl;
  cout << " list.rcData->getReferenceCount = "
       <<   list.rcData->getReferenceCount() << endl;


  list[1]=10.;
  list[1].display("list[1] after breakReference and =10");
  list2[1].display("list2[1] after breakReference");
  
  func( list );
  list[1].display("list[1] after func( list )");
  
  
}
