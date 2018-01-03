#include <A++.h>

int main()
   {
     Index::setBoundsCheck(on);

     intArray A(2,2);
     intArray B(2,2);
     A = 1;
     B = 2;

     Range I(1,1); 
     Range all;
     intArray J(2);
     J(0) = 0;
     J(1) = 1;

  // A(J) = B(I);
     A(I,all) = B(I,all);

#if 0
     B = 7;
     intArray & X = A(I);
     X = B;
#endif

     A.display();
     B.display();

     printf ("Program Terminated Normally! \n");

     return 0;
   }


