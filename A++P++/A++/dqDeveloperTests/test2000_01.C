// Problem Report 0-02-01-09-24-05

#include <A++.h>

int main()
   {
     intArray ia(10), ib(10);
     ia=1;
     ib=2;

#if 0
     Range R(0,1);     // fails with unit length range object (e.g. R(0,0))
#else
     Range R(0,0);     // works with R(0,1)
#endif

     intArray iaa(R);

  // ib.display("ib");

     printf ("now execute  iaa(R)=ia(ib(R)); \n");

     iaa(R)=ia(ib(R));   // *** bug here when R is length 1

     iaa.display("iaa");

     if (iaa(0) != 1)
        {
          printf ("ERROR: incorect results! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: indirect addressing with length 1 Range object \n");
        }

     printf ("Program Terminated Normally! \n");

     return 0;
   }
