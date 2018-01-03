// Problem report 0-04-11-10-47-38

#include <A++.h>

int
main()
   {
     intArray ia(10), ib(10);
     ia.seqAdd(0,1);
     ib.seqAdd(9,-1);

  // ia.display("ia before");
  // printf ("before indexing: Reference count = %d \n",ia.getRawDataReferenceCount());
  // ia.view("ia before");
  // ia(ib).view("ia(ib)");
  // printf ("after ia(ib).view(): indexing Reference count = %d \n",ia.getReferenceCount());

  // ia.displayReferenceCounts("BEFORE COPY: ia");
     APP_DEBUG = 0;

  // ib.displayReferenceCounts("BEFORE REFERENCE: ib");
#if 1
#if 0
     ia = ia(ib);
#else
  // This is an error to the GNU compiler
     const intArray & ia_reference = ia(ib);
  // ib.displayReferenceCounts("AFTER REFERENCE: ib");
     ia = ia_reference;
  // intArray X = ia_reference;
  // ia_reference.displayReferenceCounts("AFTER COPY: ia_reference");
#endif
#else
     ia=ia(ib)*1;  // *this works
#endif
  // ib.displayReferenceCounts("AFTER COPY: ib");

     APP_DEBUG = 0;

  // ia.displayReferenceCounts("AFTER COPY: ia");
  // ib.displayReferenceCounts("AFTER COPY: ib");
  // X.displayReferenceCounts("AFTER COPY: X");

#if 0
  // This is the correct value to obtain at this point 
#if 0
     if (ib.getReferenceCount() != 1)
#else
     if (ib.getReferenceCount() != 2)
#endif
        {
          printf ("ERROR: reference count (%d != 3) of arrays using copy constructor and indirect addressing not decremented! \n",ib.getReferenceCount());
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: reference count of arrays using copy constructor and indirect addressing ARE correctly incremented! \n");
        }
#else
     printf ("WARNING: Reference count test of indirect addressing vector commented out (ref count = %d) \n",ib.getReferenceCount());
#endif

#if 1
  // Different compilers will call the destructors at different places 
  // So on the sun this is an increasing value (up to 8), while on the Dec this value is always 2!
  // To fix this we associate each view with a different array so that 
  // the results are more consistant between compilers.

     intArray X1 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());
     intArray X2 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());
     intArray X3 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());
     intArray X4 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());
     intArray X5 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());
     intArray X6 = ia(ib);
     printf ("after ia(ib): Reference count = %d \n",ia.getRawDataReferenceCount());

#if 0
     if (ia.getRawDataReferenceCount() != 7)
#else
     if (ia.getRawDataReferenceCount() != 8)
#endif
        {
          printf ("ERROR: RAW reference count of arrays using indirect addressing not incremented! \n");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: RAW reference count of arrays using indirect addressing ARE correctly incremented! \n");
        }
#endif

     APP_DEBUG = 0;
     printf ("Program Terminated Normally! \n");

     Array_Domain_Type::smartReleaseOfInternalMemory = TRUE;

     return 0;
   }

