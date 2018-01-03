/*
DATE:               21:43:07 11/19/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           serious
SUMMARY:           
Smart release of memory doesn't work
ENVIRONMENT:        
/usr/casc/overture/A++P++/A++P++-DATE-00-10-10-TIME-13-33/SUN_CC/NODEBUG/A++P++/A++/lib/solaris_cc_CC/lib
DESCRIPTION:        
Smart release of memory does not work for
a rather simple code. It does work for a
very simple code.
HOW TO REPEAT:      
~henshaw/res/A++/bug50.C
TEST CODE:          
*/

#include <A++.h>

int 
main()
   {
     Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

     floatArray x(10);
     x = 3.0;
     x = sin(x);

     printf ("Program Terminated Normally! \n");

     return 0;
   }
