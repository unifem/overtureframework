// Problem report 0-03-28-14-22-59

// This bug was reported on the IBM, where P++ is not
// compiled with PADRE.  So it might be required to test
// this with a version of P++ not using PADRE.

#include <A++.h>

int
main( int argc, char *argv[], char *env[] )
   {
     int Max_Number_Of_Processors = 4;
     // *wdh* 100924
     Optimization_Manager::Initialize_Virtual_Machine("PartitionBug2",Max_Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Partitioning_Type thePartition(Range(0,0));
     intArray T(10,thePartition);

     T = 4.0;

  // APP_DEBUG = 2;

     intArray Tout = T;

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();
     return 0;
   }

