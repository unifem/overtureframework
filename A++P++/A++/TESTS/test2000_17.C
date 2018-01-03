
//================================================================
// A++ library test code for multiple dimensions.  Version 1.2.7 
//================================================================

#define BOUNDS_CHECK

#include<A++.h>
#include<iostream>

#define TYPE float
#define TYPEArray floatArray

#define COUNT_OUTSTANDING_ARRAYS FALSE

using namespace std;
int
main(int argc, char** argv)
   {
  Range Ispan(2,12);
  Range Jspan(-2,8);
  Range Kspan(-6,-1);
  Range Lspan(100,105);

  TYPEArray A(Ispan,Jspan,Kspan,Lspan);
  TYPEArray B(Ispan,Jspan,Kspan);
  TYPEArray D(Ispan,Jspan,Kspan);

  Index Ispan1(4,6);
  Index Jspan1(0,6);
  Index Kspan1(-5,4);

  Index all;

  A = 1;
  B = 0;
  D = 0;

#if 0
  TYPE m1;
  intArray I(3),J(3);

  I(0) = 4;
  I(1) = 7;
  I(2) = 11;

  J(0) = -1;
  J(1) = 2;
  J(2) = 6;

  TYPEArray FF(3);
  FF(0) = 5.;
  FF(1) = 6.;
  FF(2) = 7.;

   where (FF<(TYPE)7.) m1 = max(A(I,J,-3,103)+(TYPE)2.);
#endif

#if 1
  TYPEArray N1(Ispan,11);
  TYPEArray N2(N1.getDataPointer(),N1.getLength(0),N1.getLength(1),N1.getLength(2),N1.getLength(3)); // N2 is -5

#if COUNT_OUTSTANDING_ARRAYS
// If we want to have A++/P++ properly remove the array from
// internal use (delete the memory and return the array_ID)
// then we have to decrement the reference count explicitly.
  N2.decrementRawDataReferenceCount();
#endif
#endif

#if 0              
   TYPEArray H1;
// H1.reference(TYPEArray(B-D)(all,all,-6)); // H1 has elements -8,-6,...,10,12
// H1.reference(TYPEArray(B-D));
   H1.reference(B-1);
// H1.reference(B);

// When the temporary is deleted the reference count is not decremented on the data so it
// stays in the reference count array.
// B-1;
#endif

// Turn on the mechanism to remove all internal memory in use after the last array is deleted
   Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

// Force an error if the GlobalMemoryRelease() function is not called (how do we do that?)
// We could allocate a global scope class and force it in the destructor to check if 
// GlobalMemoryRelease() had been called (such a destructor would be the only think that
// we could be sure would be called last (though static object destructors would not work
// with this mechanism).
   Diagnostic_Manager::setExitFromGlobalMemoryRelease(TRUE);

  return (1);

}

