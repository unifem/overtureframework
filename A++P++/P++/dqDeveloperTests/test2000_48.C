// USE_PPP is set in config.h depending upon if we are using P++ or not!
// define USE_PPP
//#define TEST_INDIRECT

//define TEST_ARRAY_OVERFLOW
//================================================================
// A++ library test P++ code.  Version 1.2.9 
//================================================================

//#define BOUNDS_CHECK

#include<A++.h>
#include<iostream>

#define TYPE float
#define TYPEArray floatArray
#define TYPESerialArray floatSerialArray


int
main(int argc, char** argv)
{
  MemoryManagerType  Memory_Manager;

  Index::setBoundsCheck(on);

  // ================= P++ startup =============================

  printf("This is the new testppp, with 2 processors. \n");

  // ... leave this in with A++ also for debugging ...

  int Number_Of_Processors = 1;

  Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  printf("Run P++ code(Number Of Processors=%d) \n",
    Number_Of_Processors);

  Diagnostic_Manager::setTrackArrayData();
  APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);

  //===========================================================

// This call is made so that the Blue Pacific compiler will see the use of MPI directly
// and then link to the appropriate library (libmpi).  mpCC determines the 
// correct libraries to link and needs this to trigger linking to libmpi.
  MPI_Barrier(MPI_COMM_WORLD);
  
  int ghost_cell_width = 0;
  //int ghost_cell_width = 1;
  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths (ghost_cell_width);
  
  Range Ispan(2,12);
  Range Jspan(-2,8);
  Range Kspan(-6,-1);
  Range Lspan(100,105);

  TYPEArray A(Ispan,Jspan,Kspan,Lspan);
  TYPEArray B(Ispan,Jspan,Kspan);
  TYPEArray C(Ispan,Jspan);

  A = 1.0;
  B = 1.0;

  Optimization_Manager::Optimize_Scalar_Indexing = FALSE;

  Partitioning_Type *Single_Processor_Partition = new Partitioning_Type(Range(0,0));

  printf ("$$$ 1: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 1: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

  APP_ASSERT (Single_Processor_Partition->referenceCount > 0);
  APP_ASSERT (Single_Processor_Partition->referenceCount == 1);

  TYPEArray T1; // ,T2,T3,T4,T5;
  TYPEArray locT1; // ,locT2,locT3,locT4,locT5;

#if 0
  printf ("#1 Default intArrayList.getLength()    = %d \n",Internal_Partitioning_Type::DefaultintArrayList.getLength());
  printf ("#1 Default floatArrayList.getLength()  = %d \n",Internal_Partitioning_Type::DefaultfloatArrayList.getLength());
  printf ("#1 Default doubleArrayList.getLength() = %d \n",Internal_Partitioning_Type::DefaultdoubleArrayList.getLength());
  printf ("#1 Single_Processor_Partition->intArrayList.getLength()    = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->intArrayList.getLength());
  printf ("#1 Single_Processor_Partition->floatArrayList.getLength()  = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->floatArrayList.getLength());
  printf ("#1 Single_Processor_Partition->doubleArrayList.getLength() = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->doubleArrayList.getLength());
#endif

  locT1.partition(*Single_Processor_Partition);

#if 0
  printf ("#2 Default intArrayList.getLength()    = %d \n",Internal_Partitioning_Type::DefaultintArrayList.getLength());
  printf ("#2 Default floatArrayList.getLength()  = %d \n",Internal_Partitioning_Type::DefaultfloatArrayList.getLength());
  printf ("#2 Default doubleArrayList.getLength() = %d \n",Internal_Partitioning_Type::DefaultdoubleArrayList.getLength());
  printf ("#2 Single_Processor_Partition->intArrayList.getLength()    = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->intArrayList.getLength());
  printf ("#2 Single_Processor_Partition->floatArrayList.getLength()  = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->floatArrayList.getLength());
  printf ("#2 Single_Processor_Partition->doubleArrayList.getLength() = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->doubleArrayList.getLength());

  APP_ASSERT (Internal_Partitioning_Type::DefaultfloatArrayList.getLength() == 4);
  APP_ASSERT (Single_Processor_Partition->getInternalPartitioningObject()->floatArrayList.getLength() == 1);
#endif

  printf ("$$$ 2: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 2: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

#if 0
  locT2.partition(*Single_Processor_Partition);
  locT3.partition(*Single_Processor_Partition);
  locT4.partition(*Single_Processor_Partition);
  locT5.partition(*Single_Processor_Partition);
#endif

  T1 = B;

#if 0
  printf ("#3 Default intArrayList.getLength()    = %d \n",Internal_Partitioning_Type::DefaultintArrayList.getLength());
  printf ("#3 Default floatArrayList.getLength()  = %d \n",Internal_Partitioning_Type::DefaultfloatArrayList.getLength());
  printf ("#3 Default doubleArrayList.getLength() = %d \n",Internal_Partitioning_Type::DefaultdoubleArrayList.getLength());
  printf ("#3 Single_Processor_Partition->intArrayList.getLength()    = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->intArrayList.getLength());
  printf ("#3 Single_Processor_Partition->floatArrayList.getLength()  = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->floatArrayList.getLength());
  printf ("#3 Single_Processor_Partition->doubleArrayList.getLength() = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->doubleArrayList.getLength());
#endif

  printf ("$$$ 2.5: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 2.5: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

#if 1
  int numberOfReferencesBeforeRedim = Single_Processor_Partition->getInternalPartitioningObject()->referenceCount;
  locT1.redim(T1);
  int numberOfReferencesAfterRedim = Single_Processor_Partition->getInternalPartitioningObject()->referenceCount;

  printf ("numberOfReferencesBeforeRedim = %d \n",numberOfReferencesBeforeRedim);
  printf ("numberOfReferencesAfterRedim  = %d \n",numberOfReferencesAfterRedim);

  APP_ASSERT (numberOfReferencesBeforeRedim == numberOfReferencesAfterRedim);
#endif

#if 0
  printf ("#4 Default intArrayList.getLength()    = %d \n",Internal_Partitioning_Type::DefaultintArrayList.getLength());
  printf ("#4 Default floatArrayList.getLength()  = %d \n",Internal_Partitioning_Type::DefaultfloatArrayList.getLength());
  printf ("#4 Default doubleArrayList.getLength() = %d \n",Internal_Partitioning_Type::DefaultdoubleArrayList.getLength());
  printf ("#4 Single_Processor_Partition->intArrayList.getLength()    = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->intArrayList.getLength());
  printf ("#4 Single_Processor_Partition->floatArrayList.getLength()  = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->floatArrayList.getLength());
  printf ("#4 Single_Processor_Partition->doubleArrayList.getLength() = %d \n",Single_Processor_Partition->getInternalPartitioningObject()->doubleArrayList.getLength());
#endif

  printf ("$$$ 3: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 3: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

#if 1
  TYPEArray T6(A);

  printf ("$$$ 4: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 4: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

  TYPEArray locT6;

  printf ("$$$ 5: Single_Processor_Partition->referenceCount = %d \n",Single_Processor_Partition->referenceCount);
  printf ("$$$ 5: Single_Processor_Partition->getInternalPartitioningObject()->referenceCount = %d \n",
       Single_Processor_Partition->getInternalPartitioningObject()->referenceCount);

  locT6.partition(*Single_Processor_Partition);
#endif

// delete Single_Processor_Partition;
   printf ("Single_Processor_Partition->getReferenceCount() = %d \n",Single_Processor_Partition->getReferenceCount());
   APP_ASSERT (Single_Processor_Partition->getReferenceCount() >= Internal_Partitioning_Type::getReferenceCountBase());
   Single_Processor_Partition->decrementReferenceCount();
   if (Single_Processor_Partition->getReferenceCount() < Internal_Partitioning_Type::getReferenceCountBase())
      {
        printf ("calling delete Single_Processor_Partition \n");
        delete Single_Processor_Partition;
      }
   Single_Processor_Partition = NULL;

  //================= P++ finish ==============================

  printf ("Program Terminated Normally! \n");

  // ... leave these 2 lines in for A++ also for debugging ...

// Call the diagnostics mechanism to display memory usage
  Diagnostic_Manager::report();

  Optimization_Manager::Exit_Virtual_Machine ();
  printf ("Virtual Machine exited! \n");

  //===========================================================

  return 0;
}



