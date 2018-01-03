// Problem Report 99-11-01-10-19-36

// The problem is that the Mod function does not work on the Tera Cluster.

#include "A++.h"
int
main()
   {
  // Test of C++ operators % and fmod scalar functions
     printf ("Test operator%():  3 % 2 = %d \n", 3%2 );
     printf ("Test fmod function: fmod(3,2) = %d \n", (int)fmod((double)3,(double)2) );

     intArray a(3);
     a=3;

     intArray b(3),c(3);

  // Tests without temporaries
     b= a % 2;
     for( int i=0; i<3; i++ )
          c(i)=a(i) % 2;

     a.display("a (should be 3)");
     b.display("b=a % 2 (should be 1)");
     c.display("c(i)=a(i) % 2 (should be 1)");

     if (sum ( c != 1 ) != 0)
        {
          printf ("ERROR: scalar test (no temporaries) incorrect result! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: scalar test (no temporaries) of mod operator (operator%)! \n");
        }

     if (sum ( b != 1 ) != 0)
        {
          printf ("ERROR: array test (no temporaries) incorrect result! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: array test (no temporaries) of mod operator (operator%)! \n");
        }

     intArray d(3);
     d=0;

  // Tests with temporaries
     b = (a+d) % 2;
     (a+d).display("a+d (should be 3)");
     b.display("b=(a+d) % 2 (should be 1)");

     if (sum ( b != 1 ) != 0)
        {
          printf ("ERROR: array test (with temporaries) incorrect result! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: array test (with temporaries) of mod operator (operator%)! \n");
        }

#if 0
     c=a+d;
     b=c % 2;
     c.display("c=a+d (should be 3)");
     b.display("b=c % 2 (should be 1)");

     if (sum ( b != 1 ) != 0)
        {
          printf ("ERROR: array test (with temporaries) incorrect result! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: array test (with temporaries) of mod operator (operator%)! \n");
        }
#endif

#if 0
     if (sum ( b != 1 ) != 0)
        {
          printf ("ERROR: incorrect result! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: test of mod function (operator%)! \n");
        }
#endif

     printf ("Program Terminated Normally! \n");
     return 0;
   }

#if 0

See test code. I get the following output
intArray::display() (CONST) (Array_ID = 110) -- a (should be 3) 
Array_Data is a VALID pointer = 140088ea0 (1074302624)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 3.0000e+00 3.0000e+00 3.0000e+00 
intArray::display() (CONST) (Array_ID = 111) -- b=a % 2 (should be 1) 
Array_Data is a VALID pointer = 140088e60 (1074302560)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 1.0000e+00 1.0000e+00 1.0000e+00 
intArray::display() (CONST) (Array_ID = 112) -- c(i)=a(i) % 2 (should be 1) 
Array_Data is a VALID pointer = 1400a94a0 (1074435232)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 1.0000e+00 1.0000e+00 1.0000e+00 
intArray::display() (CONST) (Array_ID = 114) -- a+d (should be 3) 
Array_Data is a VALID pointer = 1400a9480 (1074435200)! 
WARNING:!!!
 In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 3.0000e+00 3.0000e+00 3.0000e+00 
intArray::display() (CONST) (Array_ID = 111) -- b=(a+d) % 2 (should be 1) 
Array_Data is a VALID pointer = 1400a94e0 (1074435296)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 0.0000e+00 0.0000e+00 0.0000e+00 
intArray::display() (CONST) (Array_ID = 112) -- c=a+d (should be 3) 
Array_Data is a VALID pointer = 1400a9500 (1074435328)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 3.0000e+00 3.0000e+00 3.0000e+00 
intArray::display() (CONST) (Array_ID = 111) -- b=c % 2 (should be 1) 
Array_Data is a VALID pointer = 140088e60 (1074302560)! 
WARNING: In intArray::display() -- Smart display turned OFF! 
AXIS 0 --->: (   0) (   1) (   2) 
AXIS 1 (  0) 0.0000e+00 0.0000e+00 0.0000e+00 

#endif






