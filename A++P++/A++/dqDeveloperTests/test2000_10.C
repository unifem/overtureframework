// A++/P++ Problem Report 0-04-11-10-51-13: Indirect addressing in where statement fails

#define BOUNDS_CHECK

#include <A++.h>

int
main()
   {
#if 1
  Index::setBoundsCheck(on);
#endif

#if 1
     intArray X(1);
     intArray Y(1);
     intArray mask(1);
     intArray I(1);
     X    = 3;
     Y    = 10;
     mask = 2;
     I    = 0;

     Range K = 1;
     intArray A(1);
     intArray B(1);
     A    = -1;
     B    = -1;

#if 1
     where( mask )
        {
//        X(K)=(X(K)-Y(I(K))*1.);  // this works, NOTE *1.
//        X(K)=(X(K)-Y(I(K)));
//        A=(X(K)-Y(I(K)));
//        B=(X(K)-Y(I(K))*1.);  // this works, NOTE *1.
//        A=(X-Y(I(K)));
//        B=(X-Y(I(K))*1.);  // this works, NOTE *1.
//        A=(X-Y(I));
//        B=(X-Y(I)*1.);  // this works, NOTE *1.

//        A=(X-Y(I)).display("A");
//        B=(X-Y(I)*1.).display("B");  // this works, NOTE *1.

//        (X-Y(I)).display("X-Y(I)");
//        (X-Y(I)*1.0).display("X-Y(I)*1.0");  // this works, NOTE *1.

//        (X-Y(I(K))).display("X+Y(I)");
//        (Y(I(K))-X).display("X+Y(I)");
//        (X-Y(I(K))*1.0).display("X+Y(I)*1.0");  // this works, NOTE *1.
//        (Y(I(K))*1.0-X).display("Y(I)*1.0+X");  // this works, NOTE *1.

//        (X+Y(I(K))).display("X+Y(I)");
//        (X+Y(I(K))*1.0).display("X+Y(I)*1.0");  // this works, NOTE *1.

// Simplest example which fails
//        (X+Y(I)).display("X+Y(I)");
//        (X+Y(I)*1.0).display("X+Y(I)*1.0");  // this works, NOTE *1.

          (Y(I)+X).display("Y(I)+X");
          (X+Y(I)).display("X+Y(I)");
          (X+Y(I)*1.0).display("X+Y(I)*1.0");  // this works, NOTE *1.
        }
#else
     A=(X-Y(I));
     B=(X-Y(I)*1.);  // this works, NOTE *1.
#endif

//   A.display("A");
//   B.display("B");

     float err = A(0) - B(0);

     if (err != 0)
        {
          printf ("ERROR: indirect addressing problem with where statement! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: indirect addressing problem with where statement working! \n");
        }


#else
     const int n=10;
     floatArray u(n), up(n), up2(n), wk(n);

     up=0;
     up2=0;

     u=5.;
     wk.seqAdd(3.,.5);
     int m=5;
     intArray mask2(m), ik(m);

     mask2=0;
     mask2(Range(2,3))=1;

     int nn = mask2.getLength(0);
     Range K=nn;
  // ik(K).seqAdd(2,1);
     ik.seqAdd(0,1);
     floatArray X(K);
     X = 10;
     u = 1;

     K.display("K");

  // ****** first compute the correct answer with scalar indexing *****
     int jj;
     for (jj=0; jj<nn; jj++)
        {
          if (mask2(jj)==0)
             {
//             up(ik(jj))=(wk(jj)-u(ik(jj)))*2.;
//             up(jj)=(wk(jj)-u(ik(jj)))*2.;
//             up(jj)=(1-u(ik(jj)))*2.;
//             up(jj)=(wk(jj)-u(ik(jj)));
               up(jj)=(X(jj)-u(ik(jj)));
             }
        }

     mask2.display("mask2");
  // **** this next statement fails *****
     where( mask2==0 )
        {
#if 1
//        up2(ik(K))=(wk(K)-u(ik(K))*1.)*2.;  // this works, NOTE *1.
//        up2(K)=(wk(K)-u(ik(K))*1.)*2.;  // this works, NOTE *1.
//        up2(K)=(1-u(ik(K))*1.)*2.;  // this works, NOTE *1.
//        up2(K)=(wk(K)-u(ik(K))*1.);  // this works, NOTE *1.
//        X = (wk(K)-u(ik(K))*1.);  // this works, NOTE *1.
//        X = (X(K)-u(ik(K))*1.);  // this works, NOTE *1.
          up2(K)=(wk(K)-u(ik(K))*1.);  // this works, NOTE *1.
#else
//        up2(ik(K))=(wk(K)-u(ik(K)))*2.;
//        up2(K)=(wk(K)-u(ik(K)))*2.;
//        up2(K)=(1-u(ik(K)))*2.;     not an error
//        up2(K)=(wk(K)-u(ik(K)));
//        X = (wk(K)-u(ik(K)));
//        X = (X-u(ik(K)));
          up2(K)=(wk(K)-u(ik(K)));
#endif
        }
     float err = max(fabs(up-up2));
     printf("error = %e\n",err);
     up.display("up");
     up2.display("up2");
     X.display("X");
     
  // (up-up2).display("up-up2");
#endif

     printf ("Program Terminated Normally! \n");
     return 0;
   }

