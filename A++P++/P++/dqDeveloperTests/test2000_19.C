// Demonstrate use of MPI_Intercomm_create()
// to build MPI communicators dynamically without 
// it being a collective operation.

#define BOUNDS_CHECK
#include <A++.h>

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on M++ array bounds checking!

     int Number_Of_Processors = 0;  // get number for default value from command line
     Optimization_Manager::Initialize_Virtual_Machine("",Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     printf ("Number_Of_Processors = %d \n",Number_Of_Processors);

  // MPI_Comm P++Communicator & Communication_Manager::getMPI_Communicator();

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");
     Optimization_Manager::Exit_Virtual_Machine ();
     return 0;
   }


