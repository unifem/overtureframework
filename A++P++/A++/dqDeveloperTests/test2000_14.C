#include <A++.h>
// indirect addressing bug.
int 
main()
   {
     Range R = 10;
     intArray ia1(R),ia(R,2);
     ia1 = 2;
     ia  = 2;
  
     floatArray x(R), y(R);
     x = 0.0;

     y(R)=x(ia1(R));               // ** this works **** 
     y.display("y(R) = x(ia1(R))");
     y(R) = x(ia(R,0));            // ** generates an assertion **** 
     y.display("y(R) = x(ia)");

     return 0;
   }
