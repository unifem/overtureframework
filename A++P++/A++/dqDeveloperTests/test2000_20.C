/*
DATE:               08:14:42 09/29/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           critical
SUMMARY:           
Error in indirect addressing of views
ENVIRONMENT:        
/usr/casc/overture/A++P++/A++P++-0.7.6a/A++/lib/solaris_cc_CC
DESCRIPTION:        
There seems to be an error when using indirect addressing
of a view. The incorrect base seems to be chosen.
HOW TO REPEAT:      
See code below (~henshaw/res/A++/bug46.C)
TEST CODE:          
 */

#include <A++.h>

int 
main()
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

     floatArray a(4,2),b;

     a.seqAdd(0,1.);

     a.display("a");

     intArray ia(2);
     ia(0)=2;
     ia(1)=3;

     Range all;
     floatArray & a0 = a(all,0);
     floatArray & a1 = a(all,1);

     a1.display("a1 or a(all,1)");  // 4 5 6 7

     b=a0(ia);
     b.display("b=a0(ia)");

     b=a1(ia);
  // a1(ia).view("a1(ia)");
     b.display("b=a1(ia) *** this is wrong **");  // *** this is wrong **

     b=a(ia,1);
     b.display("b=a(ia,1)");

     return 0;
   }


