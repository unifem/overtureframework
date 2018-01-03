
// This example demonstrates a problem that Brian is having in using Array domain objects to
// build communication schedules.

#define BOUNDS_CHECK
#include "A++.h"

MemoryManagerType  Memory_Manager;

int
main( int argc, char *argv[])
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     int i;
     int theNumberOfProcessors = 0;

     Optimization_Manager::Initialize_Virtual_Machine ("",theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     intArray A(100);
     A=90;
     
     SerialArray_Domain_Type theASerialDomain = (A.Array_Descriptor.getSerialArray()).Array_Descriptor.Array_Domain;

     intSerialArray theIndexArray(5);
     theIndexArray.setBase(0) = 0;
     theIndexArray(0) = 1; theIndexArray(1) = 3; theIndexArray(2) = 5; theIndexArray(3) = 7; theIndexArray(4) = 9;
     
#define GENERATE_ERROR_2 TRUE
#if GENERATE_ERROR_2
     Indirect_Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type theIndexPointer;
     for(i=0;i<MAX_ARRAY_DIMENSION; i++)
        {
          theIndexPointer[i] = new Internal_Indirect_Addressing_Index;
          APP_ASSERT(theIndexPointer[i] != NULL);
          intSerialArray *theIndexData = new intSerialArray(10);
          APP_ASSERT(theIndexData != NULL);
          printf ("Axis = %d theIndexData->getReferenceCount() = %d \n",
                  i,theIndexData->getReferenceCount());
          theIndexPointer[i]->setIntSerialArray( theIndexData );
          printf ("theIndexPointer[%d]->getIntSerialArrayPointer()->getReferenceCount() = %d \n",
                  i,theIndexPointer[i]->getIntSerialArrayPointer()->getReferenceCount());
#if 0
       // We have to decrement the refernece count and delete the array if its reference count is less than the base
          APP_ASSERT (theIndexPointer[i]->getReferenceCount() >= theIndexPointer[i]->getReferenceCountBase());
          theIndexPointer[i]->decrementReferenceCount();
          if (theIndexPointer[i]->getReferenceCount() < theIndexPointer[i]->getReferenceCountBase())
             {
               delete theIndexPointer[i];
             }
#endif
          APP_ASSERT (theIndexData->getReferenceCount() >= theIndexData->getReferenceCountBase());
          theIndexData->decrementReferenceCount();
          if (theIndexData->getReferenceCount() < theIndexData->getReferenceCountBase())
             {
               delete theIndexData;
             }
        }
#endif

#define GENERATE_ERROR TRUE
#if GENERATE_ERROR
     //
     // abort hit here
     //
     SerialArray_Domain_Type *theSerialArrayDomain = new SerialArray_Domain_Type(theASerialDomain , theIndexPointer );
     APP_ASSERT(theSerialArrayDomain != NULL);

     printf ("AFTER CONSTRUCTION: theSerialArrayDomain->getReferenceCount() = %d \n",
             theSerialArrayDomain->getReferenceCount());
#endif

  // A.displayReferenceCounts();
     printf ("Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if GENERATE_ERROR_2
     printf ("MAX_ARRAY_DIMENSION = %d \n",MAX_ARRAY_DIMENSION);
     APP_DEBUG = 1;
  // theArrayDomain->display("theArrayDomain");
     for(i=0;i<MAX_ARRAY_DIMENSION; i++)
        {

       // This handles the reference counted delete of the intarray used internally (if it exists)
          theIndexPointer[i]->setIntSerialArray(NULL);
          delete theIndexPointer[i];
          theIndexPointer[i] = NULL;
        }
     APP_DEBUG = 0;
#endif

     printf ("Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if GENERATE_ERROR
     theSerialArrayDomain->decrementReferenceCount();
     printf ("theSerialArrayDomain->getReferenceCount() = %d \n",theSerialArrayDomain->getReferenceCount());
     if (theSerialArrayDomain->getReferenceCount() < intSerialArray::getReferenceCountBase())
        {
          printf ("XXXXXX: reference counted delete for theSerialArrayDomain \n");
          delete theSerialArrayDomain;
        }

     theSerialArrayDomain = NULL;
#endif


     printf ("AT BASE: Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

  // A.displayReferenceCounts();

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

  // APP_DEBUG = 1;

     return 0;
   }







