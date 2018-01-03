// Problem Report 99-10-01-15-07-45

//
// Test out the A++ Class Library
//


#include <A++.h>

int main()
   {
     cout << "====== Test of A++ =====" << endl;

     intArray mask;

#if 0
     floatArray a(Range(-1,5));
     a=0.;
     a(2)=1.;
  
     Range R(1,4);
  
     mask = a(R) > 0.;
     a(R).display("a(R)");
     mask.display("Here is mask for a(R)>0.");

     mask.display("mask");
     printf(" mask.getBase(0) = %i \n",mask.getBase(0));

     intArray indirect;
     indirect = mask.indexMap();
     indirect.display("Here is indirect");

     cout << "a(indirect(0)) = " << a(indirect(0)) << endl;
#endif

     floatArray b(Range(-1,5),Range(-1,3));
     b=0.;
     b(2,1)=1.;
     b(3,1)=1.;
  
     Range R1(1,4), R2(-1,2);
     mask.redim(0);

     mask=b(R1,R2)>0.;

     mask.display("Here is mask for b(R1,R2)>0.");

//   indirect.redim(0);
     intArray indirect;
     indirect = mask.indexMap();
     indirect.display("Here is indirect");

     int i = 0;
     for( i=0; i<=indirect.getBound(0); i++ )
          printf("b(indirect(%i,0),indirect(%i,1)) = %e\n",i,i,b(indirect(i,0),indirect(i,1)));

     int numberOfArrays=0;
     for( i=0; i<500; i++ )
        {
          indirect = mask.indexMap();
       // mask.indexMap();

          if( Array_Domain_Type::getNumberOfArraysInUse() > numberOfArrays )
             {
               numberOfArrays=Array_Domain_Type::getNumberOfArraysInUse();
               printf("**** WARNING: number of A++ arrays has increased to = %i \n",numberOfArrays);
             }
        }

     if (numberOfArrays > 4)
        {
          printf ("ERROR: Memory leak in array object indexMap() member function! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: No memory leak in array object indexMap() member function! \n");
        }

     printf ("Program Terminated Normally! \n");
     return 0;
   }




