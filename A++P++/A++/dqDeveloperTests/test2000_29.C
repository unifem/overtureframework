/*
DATE:               13:48:50 12/06/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           critical
SUMMARY:           
Error giving Partition object to an array with references
ENVIRONMENT:        
/home/dquinlan/A++P++/A++P++Source/A++P++-0.7.6c/A++/lib/solaris_cc_CC
DESCRIPTION:        
The test code below fails with an assert.
HOW TO REPEAT:      
See below, ~henshaw/bug53.C
TEST CODE:          
*/

#include <A++.h>

int main()
   {
     Partitioning_Type p1;

     floatArray a(10), b;

     b.reference(a);

     a.partition(p1);

     return 0;
   }

