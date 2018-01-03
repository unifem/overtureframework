#define BOUNDS_CHECK

// Force the use of a special version of the parallel printf
// define USE_PARALLEL_PRINTF
#include "A++.h"

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int Number_of_Processors = 0;

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     printf ("Test printf! \n");

#if 1
     printf ("Test new parallel printf! \n");
#else
     parallelPrintf ("Test new parallel printf! \n");
#endif

     APP_DEBUG = 1;

     intArray X(10);
     intArray Y(10);
     X = 1;
     Y = X;

     APP_DEBUG = 0;

     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }

