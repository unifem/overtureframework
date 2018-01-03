// #define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
  // MemoryManagerType  Memory_Manager;

     int Number_Of_Processors = 1;

     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     printf("Run P++ code(Number Of Processors=%d) \n",Number_Of_Processors);

     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);

     Partitioning_Type *Single_Processor_Partition  = new Partitioning_Type(Range(0,0));

     intArray locT1,locT2,locT3,locT4,locT5;
     locT1.partition(*Single_Processor_Partition);
#if 1
     locT2.partition(*Single_Processor_Partition);
     locT3.partition(*Single_Processor_Partition);
     locT4.partition(*Single_Processor_Partition);
     locT5.partition(*Single_Processor_Partition);
#endif

     delete Single_Processor_Partition;

  // ================= P++ finish ==============================

     printf ("Program Terminated Normally! \n");

  // ... leave these 2 lines in for A++ also for debugging ...
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

     return 0;
   }



