/*
DATE:               18:58:13 12/02/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           non-critical
SUMMARY:           
Compile failure for declaration floatArray x(int,Range)
ENVIRONMENT:        
Any.
DESCRIPTION:        
Compile failure for declaration floatArray x(int,Range)
HOW TO REPEAT:      
See test code, ~henshaw/res/bug52.C
TEST CODE:          
*/

#include <A++.h>

int 
main()
   {
     Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

  // Be explicit to avoid ambiguity
#if 0
     Range I=50;
     floatArray r(2,I);
#else
     Range I=50;
     floatArray r(Range(2),I);
#endif

  return 0;
}

