// This example demonstrates a problem that Brian is having in using Array domain objects to
// build communication schedules.

#define BOUNDS_CHECK
#include "A++.h"

// This is no longer used and is replaced by the function:
//      Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);
// which better implements the memory release mechanism.
// MemoryManagerType  Memory_Manager;

int
main( int argc, char *argv[])
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     int i;
     int theNumberOfProcessors;

     Optimization_Manager::Initialize_Virtual_Machine ("",theNumberOfProcessors,argc,argv);

  // Communication_Manager::setOutputProcessor(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 1
     intArray *B = new intArray(10);
     printf ("value of B pointer = %p \n",B);

     APP_ASSERT (B != NULL);
  // B = NULL;
     printf ("In main(): calling delete for B = %p \n",B);
     delete B;
     printf ("In main(): DONE with call to delete for B = %p \n",B);
#endif

  // int *x = new int;

#if 0
     B->decrementReferenceCount();
     printf ("B->getReferenceCount() = %d \n",B->getReferenceCount());
     if (B->getReferenceCount() < intArray::getReferenceCountBase())
        {
          printf ("In main(): calling delete for B = %p \n",B);
       // intArray::operator delete (B,sizeof(intArray));
          APP_ASSERT (B != NULL);
          delete B;
          printf ("In main(): DONE with call to delete for B = %p \n",B);
        }
     B = NULL;
#endif

     printf ("In main(): Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if 0
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();
#endif

     Optimization_Manager::Exit_Virtual_Machine ();

#if 1
  // APP_DEBUG = 1;
  // Turn on the mechanism to remove all internal memory in use after the last array is deleted
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

  // Exit from within the global release mechanism
     Diagnostic_Manager::setExitFromGlobalMemoryRelease();
#endif

     return 0;
   }











