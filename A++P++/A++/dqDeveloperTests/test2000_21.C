/*
DATE:               08:56:06 10/03/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           critical
SUMMARY:           
Error in indexMap
ENVIRONMENT:        
/usr/casc/overture/A++P++/A++P++-0.7.6a/A++/lib/solaris_cc_CC/
DESCRIPTION:        
indexMap operation generates a seg fault through attempt
to derefernce a nil pointer
HOW TO REPEAT:      
See code below (~henshaw/res/A++/bug47.C)
TEST CODE:
*/

#include <A++.h>

int 
main()
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

     intArray ia;
     intArray mask1D(5);
     intArray mask2D(5,5);
     intArray mask3D(5,5,5);
     intArray mask4D(5,5,5,5);

     mask1D = 0;
     mask2D = 0;
     mask3D = 0;
     mask4D = 0;

  // Test case where no points are in the mask
     ia = (mask1D!=0).indexMap();
     ia.display("ia (should be a NULL array!)");
     ia.redim(0);
     ia = (mask2D!=0).indexMap();
     ia.display("ia (should be a NULL array!)");
     ia.redim(0);
     ia = (mask3D!=0).indexMap();
     ia.display("ia (should be a NULL array!)");
     ia.redim(0);
     ia = (mask4D!=0).indexMap();
     ia.display("ia (should be a NULL array!)");

  // Now setup a non-zero mask
     mask1D(2)       = 1;
     mask2D(2,2)     = 1;
     mask3D(2,2,2)   = 1;
     mask4D(2,2,2,2) = 1;

 // (mask!=0).view("(mask!=0)");

     ia=(mask1D!=0).indexMap();
     ia.display("ia (should be a valid array: (2))");
     ia.redim(0);
     ia=(mask2D!=0).indexMap();
     ia.display("ia (should be a valid array: (2,2))");
     ia.redim(0);
     ia=(mask3D!=0).indexMap();
     ia.display("ia (should be a valid array: (2,2,2))");
     ia.redim(0);
     ia=(mask4D!=0).indexMap();
     ia.display("ia (should be a valid array: (2,2,2,2))");

#if 0
  // This should be it's own test program later

  // Test assignement of null array to a valid array (should be an error)
     intArray ib,ic;
     ib=ic;
     Range R(0,-1);
     ib(R) = ic(R);
     intArray X(10),Y(10),Z(10);

     X(R) = Y(R);
     X(R) = Y(Z(R));
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     return 0;
   }

