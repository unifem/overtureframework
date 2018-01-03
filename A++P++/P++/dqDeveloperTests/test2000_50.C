/*
   Testing Overture problem of repartitioning a Null array with a partitioning object
   containing nonzero ghost boundary widths.
*/

#define BOUNDS_CHECK

#include<A++.h>

int
main(int argc, char** argv)
   {
  // MemoryManagerType  Memory_Manager;

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

  // Diagnostic_Manager::setSmartReleaseOfInternalMemory( ON );

     intArray x(0);
     x = 1;

     Partitioning_Type P;
     P.SpecifyInternalGhostBoundaryWidths(1,1);

     printf ("Now partition the null array! \n");

     x.partition(P);

  // Use the new interface
     APP_ASSERT (P.getGhostBoundaryWidth(0) == 1);
     APP_ASSERT (P.getGhostBoundaryWidth(1) == 1);
     APP_ASSERT (P.getGhostBoundaryWidth(2) == 0);
     APP_ASSERT (P.getGhostBoundaryWidth(3) == 0);
     APP_ASSERT (P.getGhostBoundaryWidth(4) == 0);
     APP_ASSERT (P.getGhostBoundaryWidth(5) == 0);

     int i=0;
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf ("x.getGhostBoundaryWidth(%d) = %d \n",i,x.getGhostBoundaryWidth(i));

#if 1
     printf ("Now call redim after the association of the array with a new partitioning object! \n");

     x.redim(2,2);

     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf ("AFTER redim: x.getGhostBoundaryWidth(%d) = %d \n",i,x.getGhostBoundaryWidth(i));

     APP_ASSERT (x.getGhostBoundaryWidth(0) == 1);
     APP_ASSERT (x.getGhostBoundaryWidth(1) == 1);
     APP_ASSERT (x.getGhostBoundaryWidth(2) == 0);
     APP_ASSERT (x.getGhostBoundaryWidth(3) == 0);
     APP_ASSERT (x.getGhostBoundaryWidth(4) == 0);
     APP_ASSERT (x.getGhostBoundaryWidth(5) == 0);
#endif

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

     return 0;
   }

