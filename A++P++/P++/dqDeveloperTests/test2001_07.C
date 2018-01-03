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
     int theNumberOfProcessors;

     Optimization_Manager::Initialize_Virtual_Machine ("",theNumberOfProcessors,argc,argv);

     Communication_Manager::setOutputProcessor(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Partitioning_Type* thePartition = new Partitioning_Type(Range(0,0));
     intArray A(100);
  // A.displayReferenceCounts();

#if 1
     Array_Domain_Type & theADomain = A.Array_Descriptor.Array_Domain;

#define GENERATE_ERROR_2 TRUE
#if GENERATE_ERROR_2
     Indirect_Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type theIndexPointer;
     for(i=0;i<MAX_ARRAY_DIMENSION; i++)
        {
          theIndexPointer[i] = new Internal_Indirect_Addressing_Index;
          APP_ASSERT(theIndexPointer[i] != NULL);
       // intArray *theIndexData = new intArray(10,*thePartition);
          intArray *theIndexData = new intArray(10);
          APP_ASSERT(theIndexData != NULL);
       // theIndexData->incrementReferenceCount();
       // printf ("Axis = %d (pointer = %p) theIndexData->getReferenceCount() = %d \n",
       //      i,theIndexData,theIndexData->getReferenceCount());
          theIndexPointer[i]->setIntArray( theIndexData );
       // printf ("theIndexPointer[%d]->getIntArrayPointer()->getReferenceCount() = %d \n",
       //      i,theIndexPointer[i]->getIntArrayPointer()->getReferenceCount());
       // theIndexPointer[i]->getIntArrayPointer()->displayReferenceCounts("theIndexPointer[i]->getIntArrayPointer()");

       // The theIndexPointer[i]->setIntArray() function increments the 
       // reference count of the theIndexData so we have to delete it.
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
  // theADomain->incrementReferenceCount();
     Array_Domain_Type *theArrayDomain = new Array_Domain_Type(theADomain , theIndexPointer );
     APP_ASSERT(theArrayDomain != NULL);

     printf ("AFTER CONSTRUCTION: theArrayDomain->getReferenceCount() = %d \n",theArrayDomain->getReferenceCount());

  // printf ("Exiting after construction of new Array_Domain_Type! \n");
  // APP_ABORT();
#endif

  // A.displayReferenceCounts();
     printf ("Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if GENERATE_ERROR_2
     printf ("MAX_ARRAY_DIMENSION = %d \n",MAX_ARRAY_DIMENSION);
     APP_DEBUG = 0;
  // theArrayDomain->display("theArrayDomain");
     for(i=0;i<MAX_ARRAY_DIMENSION; i++)
        {
       // printf ("Calling DELETE on Axis = %d (pointer = %p) theIndexData->getReferenceCount() = %d \n",
       //      i,theIndexPointer[i]->getIntArrayPointer(),
       //      theIndexPointer[i]->getIntArrayPointer()->getReferenceCount());
       // theIndexPointer[i]->decrementReferenceCount();
       // if (theIndexPointer[i]->getReferenceCount() < intArray::getReferenceCountBase())
       // This handles the reference counted delete of the intarray used internally (if it exists)
       // theIndexPointer[i]->getIntArrayPointer()->displayReferenceCounts();
          theIndexPointer[i]->setIntArray(NULL);
          delete theIndexPointer[i];
          theIndexPointer[i] = NULL;
        }
     APP_DEBUG = 0;
#endif

     printf ("Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

#if GENERATE_ERROR
     theArrayDomain->decrementReferenceCount();
     printf ("theArrayDomain->getReferenceCount() = %d \n",theArrayDomain->getReferenceCount());
     if (theArrayDomain->getReferenceCount() < intArray::getReferenceCountBase())
        {
          if ( (theArrayDomain->isView() == FALSE) && (theArrayDomain->isTemporary() == FALSE) )
             {
               if (theArrayDomain->Partitioning_Object_Pointer == NULL)
                  {
                 // Internal_Partitioning_Type::DeleteArrayToPartitioning(*this);
                    printf ("theArrayDomain->Partitioning_Object_Pointer == NULL \n");
                  }
                 else
                  {
                 // Internal_Partitioning_Type::DeleteArrayToPartitioning (*(theArrayDomain->Partitioning_Object_Pointer),*this);
                    printf ("theArrayDomain->Partitioning_Object_Pointer != NULL \n");
                  }
             }

          printf ("XXXXXX: reference counted delete for theArrayDomain \n");
          delete theArrayDomain;
          printf ("DONE: XXXXXX: reference counted delete for theArrayDomain \n");
        }
       else
        {
          printf ("XXXXXX: force the call to delete for theArrayDomain \n");
       // theArrayDomain->decrementReferenceCount();
          delete theArrayDomain;
        }

     theArrayDomain = NULL;
#endif

#endif

     delete thePartition;
     thePartition = NULL;


     printf ("AT BASE: Number of arrays is use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

  // A.displayReferenceCounts();

  // Call the diagnostics mechanism to display memory usage
  // Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

  // APP_DEBUG = 1;

#if 0
  // We have to turn this off so that the partitioning object constructor (built in local scope) will be called)

  // Turn on the mechanism to remove all internal memory in use after the last array is deleted
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

  // Exit from within the global release mechanism
     Diagnostic_Manager::setExitFromGlobalMemoryRelease();
#endif

     return 0;
   }











