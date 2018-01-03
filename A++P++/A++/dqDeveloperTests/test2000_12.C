#include <A++.h>
int 
main()
{
  Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking
  int a[40];
  
  int oldNumber=0;
  int numberOfSteps=100;
  for (int i=0; i<numberOfSteps; i++) 
  {
    printf("step i=%i\n",i);

    intArray b;
    b.adopt(a,40);

    int newNumber=Array_Domain_Type::getNumberOfArraysInUse();
    if( newNumber>oldNumber )
    {
      printf("**** number of A++ arrays has increased to %i \n",newNumber);
      oldNumber=newNumber;
    }
    
  } // end for
  
  return 0;
}
