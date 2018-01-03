// bug with indexMap of a 2D view

#define BOUNDS_CHECK
#include <A++.h>

int
main()
   {
     Index::setBoundsCheck(on);
  // ...Synchronize C++ and C I/O Subsystems
     ios::sync_with_stdio();

     int size = 2;

#if 0
  // Bill's original test
     intArray tag(10,10);
     tag=0.;
     tag(4,4)=1.;
  
     Range I1(3,5),I2(4,5);
  
     tag.indexMap().display("tag.indexMap");

     APP_DEBUG = 0;
     tag(I1,I2).indexMap();
     APP_DEBUG = 0;
  
     tag(I1,I2).indexMap().display("tag(I1,I2).indexMap **should be 4,4**");
     tag(I1,I2).display("tag(I1,I2)");
#else

#if 1
  // More complex multidimensional test
     int base[4] = { 100, 200, 300, 400 };
#else
  // simple ZERO base array test
     int base[4] = { 0, 0, 0, 0 };
#endif

  // Range I1(3,5),I2(4,5);
  // Range I0(3,5), I1(3,5), I2(3,5), I3(3,5);
     Range I0(1,size/2), I1(1,size/2), I2(1,size/2), I3(1,size/2);

  // Adjust the base
     I0 += base[0];
     I1 += base[1];
     I2 += base[2];
     I3 += base[3];

#if 1
     intArray tag1D(size);
     tag1D.setBase(base[0],0);

     tag1D=0.;
     tag1D(1+base[0])=1.;
  
  // Built the answer that we want to see
     intArray correctResult1D (1);
     correctResult1D(0) = 1 + base[0];

     intArray result1DView,result1DNonView;

     result1DNonView = tag1D.indexMap();

  // tag1D(I0).indexMap().display("tag1D(I1).indexMap **should be 4**");
  // result1DNonView.view("result1DNonView");

     if ( sum (correctResult1D != result1DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 1D indexMap test for non views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 1D indexMap test for non views! \n");
        }

     APP_DEBUG = 0;
     result1DView = tag1D(I0).indexMap();
     APP_DEBUG = 0;

     if ( sum (result1DView != result1DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 1D indexMap test for views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 1D indexMap test for views! \n");
        }
#endif

#if 1
     intArray tag2D(size,size);
     tag2D.setBase(base[0],0);
     tag2D.setBase(base[1],1);

     tag2D = 0.0;
     tag2D(1+base[0],1+base[1]) = 1.0;

  // Built the answer that we want to see
     intArray correctResult2D (1,2);
     correctResult2D(0,0) = 1 + base[0];
     correctResult2D(0,1) = 1 + base[1];

     intArray result2DView,result2DNonView;

  // tag2D.indexMap().display("tag.indexMap");

     result2DNonView = tag2D.indexMap();

  // tag2D(I0,I1).indexMap().display("tag2D(I0,I1).indexMap **should be 4,4**");
  // tag2D(I0,I1).display("tag2D(I0,I1)");
     result2DNonView.display("result2DNonView");

     if ( sum (correctResult2D != result2DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 2D indexMap test for non views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 2D indexMap test for non views! \n");
        }

     APP_DEBUG = 0;
     result2DView    = tag2D(I0,I1).indexMap();
     APP_DEBUG = 0;

     result2DView.display("result2DView");

     if ( sum (result2DView != result2DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 2D indexMap test for views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 2D indexMap test for views! \n");
        }
#endif

#if 1
     intArray tag3D(size,size,size);
     tag3D.setBase(base[0],0);
     tag3D.setBase(base[1],1);
     tag3D.setBase(base[2],2);

     tag3D = 0;

     tag3D(1+base[0],1+base[1],1+base[2]) = 1;

  // Built the answer that we want to see
     intArray correctResult3D (1,3);
     correctResult3D(0,0) = 1 + base[0];
     correctResult3D(0,1) = 1 + base[1];
     correctResult3D(0,2) = 1 + base[2];

     intArray result3DView,result3DNonView;

  // tag3D.indexMap().display("tag.indexMap");

     result3DNonView = tag3D.indexMap();

#if 0
     result3DNonView.display("result3DNonView");
#endif

     if ( sum (correctResult3D != result3DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 3D indexMap test for non views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 3D indexMap test for non views! \n");
        }

     APP_DEBUG = 0;
     result3DView    = tag3D(I0,I1,I2).indexMap();
     APP_DEBUG = 0;

     if ( sum (result3DView != result3DNonView) != 0 )
        {
       // failed test
          printf ("FAILED: 3D indexMap test for views! \n");
          APP_ABORT();
        }
       else
        {
       // passed test
          printf ("PASSED: 3D indexMap test for views! \n");
        }
#endif

#endif

     return 0;
   }
