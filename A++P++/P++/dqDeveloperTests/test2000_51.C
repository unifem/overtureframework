/*
DATE:               22:18:04 11/19/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Configuration bug
SEVERITY:           non-critical
SUMMARY:           
A++ defines Boolean which conflicts with X
ENVIRONMENT:        
/usr/casc/overture/A++P++/A++P++-DATE-00-10-10-TIME-13-33/SUN_CC/NODEBUG/A++P++/A++/lib/solaris_cc_CC
DESCRIPTION:        
A++ defines Boolean which conflicts with X, suggest you
change to bool.
HOW TO REPEAT:      
Add the line
#define Boolean int;
before including A++.h
TEST CODE:          
*/

#include<A++.h>

// It is not possible to test if "Boolean" has been already typedef'd to be an int
// so this test is largely useless.

#if BOOL_IS_BROKEN
// error "We can't define the word Boolean as a macro since it conflicts with the X windowing software"
#endif

int
main ()
   {
  // Force a runtime error (but "Boolean" is typedef'd not defined using the cpp)
#if defined(Boolean)
     return 1;
#else
     return 0;
#endif
   }


