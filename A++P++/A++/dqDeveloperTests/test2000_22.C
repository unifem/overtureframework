/*
This code shows a bug in A++ that prevents the use of a Null array in the indirect addressing.
*/

#include <A++.h>

int 
main()
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  // Turn this off for now!
#if 0
  // This should be it's own test program later

  // Test assignement of null array to a valid array (should be an error)
     intArray ib,ic;
     ib=ic;
     Range R(0,-1);
     ib(R) = ic(R);
     intArray X(10),Y(10),Z(10);

     X(R) = Y(R);
     X(R) = Y(Z(R));
#endif

     return 0;
   }
