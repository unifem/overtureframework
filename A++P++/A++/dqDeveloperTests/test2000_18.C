// Example code to test the construction of array objects with 
// non unit strides.

#define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     Range I(0,7,1);
     intArray A(I);
     A.seqAdd();

// Only the memory is laid out funny (with stride) this can't be seen by the array object itself
     printf ("A: Base = %d Bound = %d Stride = %d \n",A.getBase(0),A.getBound(0),A.getStride(0));
     printf ("A: Raw Base = %d Raw Bound = %d Raw Stride = %d \n",A.getRawBase(0),A.getRawBound(0),A.getRawStride(0));

  // However an array built in this way should be a view
     printf ("A.isView = %s \n",(A.isView()) ? "TRUE" : "FALSE");

     A.display("A with unit stride");

     Range J(0,6,2);
     intArray B(A.getDataPointer(),J);

     printf ("B.isView = %s \n",(B.isView()) ? "TRUE" : "FALSE");
     printf ("B: Base = %d Bound = %d Stride = %d \n",B.getBase(0),B.getBound(0),B.getStride(0));
     printf ("B: Raw Base = %d Raw Bound = %d Raw Stride = %d \n",B.getRawBase(0),B.getRawBound(0),B.getRawStride(0));

     B.seqAdd(100);
  // A = 4;
  // B = 7;

     B.display("B with non unit stride");
     A.display("A with unit stride");

     Range K(1,2,2);
     B(K) = B(K-1) + B(K+1);

     B.display("B after B(K) = B(K-1); with non unit stride");
     A.display("A after B(K) = B(K-1); with unit stride");

// Need to decrement the reference count on B's data so that the number of arrays will be counted properly!!!
#if 0
  // #####################################################################################
  // Turn on the mechanism to remove all internal memory in use after the last array is deleted
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

  // Force an error if the GlobalMemoryRelease() function is not called (how do we do that?)
  // We could allocate a global scope class and force it in the destructor to check if 
  // GlobalMemoryRelease() had been called (such a destructor would be the only think that
  // we could be sure would be called last (though static object destructors would not work
  // with this mechanism).
     Diagnostic_Manager::setExitFromGlobalMemoryRelease(TRUE);

     if ( Diagnostic_Manager::getExitFromGlobalMemoryRelease() == TRUE )
        {
          printf ("Should not be exiting using this mechanism! \n");
          return (1);
        }
      else
        {
          return (0);
        }
#else
         return (0);
#endif
   }

