// Problem Report <unreported bug>

//
// Test out indirect addressing
//
#include <A++.h>
extern int APP_DEBUG;

#define MACRO_PRINT_INFO(X) \
     printf ("%s Base[0] = %d Data_Base[0] = %d User_Base[0] = %d Stride[0] = %d \n",str, \
          X.Array_Descriptor.Array_Domain.Base[0],                         \
          X.Array_Descriptor.Array_Domain.Data_Base[0],                    \
          X.Array_Descriptor.Array_Domain.User_Base[0],                    \
          X.Array_Descriptor.Array_Domain.Stride[0]);

// Note that GNU g++ requires the input parameters to be defined as const & instead of just &
void print ( char* str, const intArray & X )
   {
     MACRO_PRINT_INFO(X)
   }

void print ( char* str, const floatArray & X )
   {
     MACRO_PRINT_INFO(X)
   }

void print ( char* str, const doubleArray & X )
   {
     MACRO_PRINT_INFO(X)
   }


int
main()
   {
     APP_DEBUG = 0;
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

#if 0
     int base=1,bound=1;
     Index I=Range(base,bound);
     intArray ia(I);     // indirect addressing array
     ia.seqAdd(base,1);  // ia = 0,1,2,3,4,5
  // ia.display("ia after initialization");
  // print("ia(I)",ia);
#endif

     intArray A (Range(10,20));
     print("A(Range(10,20))",A);
     print("A(Range(12,18))",A(Range(12,18)));
     print("A(Range(12,18)(Range(13,18)))",A(Range(12,18))(Range(13,18)));
     print("A(Range(12,18)(Range(13,18))(Range(14,18)))",A(Range(12,18))(Range(13,18))(Range(14,18)));

     printf ("Range I(10,14) \n");
     intArray I(Range(10,14));
     I.seqAdd(12,1);
     print("A(I)",A(I));

     floatArray ZZ(Range(1,20));
     ZZ = -15.0;

     Index I1(4,5,2);
     Index I2(6,2,2);

     print("ZZ(I1)",ZZ(I1));
     print("ZZ(I1)(I2)",ZZ(I1)(I2));

     ZZ(I1)(I2)(6) = -1.;
  // ZZ(I1)(I2)(7) = -1.;

  // Case not implemented in A++/P++
  // print("A(I)(Range(10,12))",A(I)(Range(10,12)));

     printf ("Range J(12,12) \n");
     intArray J(Range(12,13));
     J.seqAdd(12,1);
  // Case not implemented in A++/P++
  // print("A(I)(Range(11,12)(J))",A(I)(Range(11,12))(J));

     return 0;
   }














