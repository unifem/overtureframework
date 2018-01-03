// Problem Report 99-10-07-10-48-46

//
// Test out indirect addressing
//
#include <A++.h>
extern int APP_DEBUG;

#define MACRO_PRINT_INFO(X) \
     printf ("%s Base[0] = %d Data_Base[0] = %d User_Base[0] = %d Bound[0] = %d Stride[0] = %d \n",str, \
          X.Array_Descriptor.Array_Domain.Base[0],      \
          X.Array_Descriptor.Array_Domain.Data_Base[0], \
          X.Array_Descriptor.Array_Domain.User_Base[0], \
          X.Array_Descriptor.Array_Domain.Bound[0],     \
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
     int base=1,bound=3;
     Index I=Range(base,bound);
     intArray ia(I);     // indirect addressing array
     ia.seqAdd(base,1);  // ia = 0,1,2,3,4,5
     ia.display("ia after initialization");

     doubleArray yy(I);
     yy.seqAdd(100,1.);    // yy = 0.,1.,2.,3.,4.,5.

     doubleArray x3(Range(base-2,bound+2));
     x3.seqAdd(10,1);

  // This is a error on the GNU g++ compiler v2.95
#ifdef __GNUG__
     doubleArray x4;
     x4.reference(x3(I)); // view
#else
     const doubleArray & x4 = x3(I); // view
#endif
     yy.display("yy before assignment");
     x3.display("x3 before assignment");
     x4.display("x4 before assignment");
     x3(I).display("x3(I)");

  // **this next line has a bug -- yy(i) is being assigned to x4(ia(i)-1) instead of x4(ia(i))
  // APP_DEBUG = 8;
  // x4(ia(I))=yy(I);

  // x3       Base[0] = 0 Data_Base[0] = -1 User_Base[0] = -1 Stride[0] = 1 
  // x3(I)    Base[0] = 2 Data_Base[0] = -1 User_Base[0] = 1 Stride[0] = 1 
  // x3(I)(I) Base[0] = 2 Data_Base[0] = -1 User_Base[0] = 1 Stride[0] = 1 
  // x3(ia)   Base[0] = 0 Data_Base[0] = -1 User_Base[0] = -1 Stride[0] = 1 
  // x3(ia)   Base[0] = 0 Data_Base[0] = -1 User_Base[0] = -1 Stride[0] = 1 
  // x4       Base[0] = 2 Data_Base[0] = -1 User_Base[0] = 1 Stride[0] = 1
  // x4(ia)   Base[0] = 0 Data_Base[0] = -1 User_Base[0] = 1 Stride[0] = 1 

#if 1
     print ("x3      ",x3);
     print ("x3(I)   ",x3(I));
     print ("x3(I)(I)",x3(I)(I));
     print ("x3(ia)  ",x3(ia));
     print ("x3(ia)  ",x3(ia));
     print ("x4      ",x4);
     print ("x4(ia)  ",x4(ia));
#endif

     APP_DEBUG = 0;
#if 1
     x4(ia(I))=yy;
  // x4(ia)=200;
#else
  // x3(ia)=yy;
     x3(ia)=200;
#endif
     APP_DEBUG = 0;

  // x3(I)(ia)=yy;

  // APP_DEBUG = 0;
     ia.display("ia");

     x3.display("x3");
     x4.display("x4");

     int failed = FALSE;
     for( int i=base; i<=bound; i++ )
          if (x4(ia(i)) != yy(i))
               failed = TRUE;

     if (failed == TRUE)
        {
          printf ("ERROR: incorrect results \n");
          APP_ABORT();
        }

     printf ("Program Terminated Normally! \n");

     return 0;
   }
