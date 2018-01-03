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

     printf("Run P++ code(Number Of Processors=%d) \n",Number_Of_Processors);

     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);

  // ===========================================================

  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
  
     int ghost_cell_width = 0;
  // int ghost_cell_width = 1;
     Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths (ghost_cell_width);

     TYPEArray xx(Range(0,1),Range(0,1),Range(0,1),Range(2,3));
     Partitioning_Type* partition = new Partitioning_Type();
     int ghost_boundary_width = 0;
     partition->partitionAlongAxis(0,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(1,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(2,FALSE,ghost_boundary_width);
     partition->partitionAlongAxis(3,TRUE,1);
     xx.partition(*partition);
     xx=3.;

     xx.reshape(Range(0,3),Range(0,0),Range(0,1),Range(0,1));

     printf ("partition->getReferenceCount() = %d \n",partition->getReferenceCount());
     APP_ASSERT (partition->getReferenceCount() >= Internal_Partitioning_Type::getReferenceCountBase());
     partition->decrementReferenceCount();
     if (partition->getReferenceCount() < Internal_Partitioning_Type::getReferenceCountBase())
        {
          printf ("calling delete partition \n");
          delete partition;
        }
     partition = NULL;

  // ================= P++ finish ==============================

     printf ("Program Terminated Normally! \n");

  // ... leave these 2 lines in for A++ also for debugging ...

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

  // ===========================================================

     return 0;
   }



