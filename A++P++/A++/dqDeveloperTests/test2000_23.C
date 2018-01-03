/*
This code shows a bug in A++ that prevents the use of a Null array in the indirect addressing.
*/

#include <A++.h>

int
main()
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

  // Notice that the keyword "class" can be placed on teh declaration line and the code still compiles!!!
     class Range R(0,-1);
     R.grow(1).display("R.grow(1)");

     Range I(0,1);
     I.grow(1).display("I.grow(1)");

     Range K1(0,0);
     K1.display("K1");
     K1.grow(-1).display("K1.grow(-1)");

     Range K2(0,2);
     K2.display("K2");
     K2.grow(-1).display("K2.grow(-1)");

     Index K3(0);
     K3.display("K3");
     K3.grow(-1).display("K3.grow(-1)");

     Range K4(0);
     K4.display("K4");
     K4.grow(-1).display("K4.grow(-1)");

     Range K5(5);
     K5.display("K5");
     K5.grow(-1).display("K5.grow(-1)");

     Index K6(5);
     K6.display("K6");
     K6.grow(-1).display("K6.grow(-1)");

     Range L(0,20);
     L.grow(-1).display("L.grow(-1)");
     L.shrink(1).display("L.shrink(1)");
     L.shrink(-1).display("L.shrink(-1)");

  // Test All_Index mode Index and Range objects
     Range M;
     M.grow(-1).display("M.grow(-1)");
     M.grow(1).display("M.grow(1)");

     Index N;
     N.display("N");
     N.grow(-1).display("N.grow(-1)");
     N.grow(1).display("N.grow(1)");
     N.shrink(1).display("N.shrink(1)");

     int i = 0;
     Range O(0,50,2);
     for (i=0; i < 5; i++)
        {
          printf ("i = %d \n",i);
          O.grow(i).display("O.grow(i)");
          O.shrink(i).display("O.shrink(i)");
          O.grow(-i).display("O.grow(-i)");
          O.shrink(-i).display("O.shrink(-i)");
        }

     Range P(0,50);
     for (i=0; i < 5; i++)
        {
          printf ("i = %d \n",i);
          P.grow(i).display("P.grow(i)");
          P.shrink(i).display("P.shrink(i)");
          P.grow(-i).display("P.grow(-i)");
          P.shrink(-i).display("P.shrink(-i)");
        }

     return 0;
   }
