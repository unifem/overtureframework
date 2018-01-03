// Problem Report 99-10-07-16-51-32

// This is "Kyle's bug" a test code that demonstrates a previous bug with
// the A++ indirect addressing.  The problem happens when scalar indexing is
// combined with intArrays in the indexing.  In particular the problem is when
// there is leading scalar indexing before the intArray parameters.  The
// problem was previously fixed but the previous solution caused several
// other bugs to appear.  The latest fix seems to work better and is
// a modification of the previous fix attempt.

#define ALT_TEST 0

#include "A++.h"
int
main(void)
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

     intArray templ(2,4);
     templ(0, 0) = 10;
     templ(0, 1) = 11;
     templ(0, 2) = 12;
     templ(0, 3) = 13;
     templ(1, 0) = 14;
     templ(1, 1) = 15;
     templ(1, 2) = 16;
     templ(1, 3) = 17;

     templ.display("templ");

     intArray emslice(2);

     intArray res;

     emslice(0) = 0;
     emslice(1) = 1;
     emslice.display("emslice");

  // templ(0,emslice).display("templ(0,emslice)");

     APP_DEBUG = 0;

  // Test that operands on the RHS can mix scalar and intArray parameters 
  // (with the scalar parameters leading the intArray parameters)
     res = templ(0,emslice);

     APP_DEBUG = 0;

     res.display("res");
     cout<<"templ(0,0), templ(0,1) "<<templ(0,0)<<" , "<<templ(0,1)<<endl;

     intArray answer(2);
     answer(0) = 10;
     answer(1) = 11;

  // Check and see if we are getting the correct answer!
  // The != operator will return a 1 for each element not equal between the two arrays
  // the sum of the entries is 0 if all elements are the same.
     if (sum(res != answer) != 0)
        {
	  printf ("ERROR: incorrect answer! \n");
	  APP_ABORT();
        }
       else
        {
	  printf ("PASSED: leading scalar parameters in indirect addressing works on RHS! \n");
        }

  // Test that operands on the LHS can include scalar indexing mixed with
  // indirect addressing (where the scalar parameters preceeds the intArray parameters)
     templ(0,emslice) = 7;
     answer = 7;

  // Check and see if we are getting the correct answer!
  // The != operator will return a 1 for each element not equal between the two arrays
  // the sum of the entries is 0 if all elements are the same.
     if (sum(templ(0,emslice) != answer) != 0)
        {
	  printf ("ERROR: incorrect answer! \n");
	  APP_ABORT();
        }
       else
        {
	  printf ("PASSED: leading scalar parameters in indirect addressing works on LHS! \n");
        }

     printf ("Program Terminated Normally! \n");
   }


