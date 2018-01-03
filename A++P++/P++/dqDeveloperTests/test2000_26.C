#define BOUNDS_CHECK
#include "A++.h"

int
main( int argc, char *argv[])
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int theNumberOfProcessors;
     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int thisProcessorNumber = Communication_Manager::My_Process_Number;

#if 0
  // Brian's example which worked once he did a cvs checkout of a fresh version!
     intArray J(10);
     J=3;
     Partitioning_Type thePartition(Range(1,2));  
     intArray T(J,DEEPALIGNEDCOPY);
     T.partition( thePartition );
     T.display("T after construction.");
     intArray Texact(J,DEEPALIGNEDCOPY);
     Texact=5;
     Texact.partition(thePartition);
     intArray err = T - Texact;
     err.display("T - Texact");
#endif

     intArray A(10);
     A = 1;

     A.reshape(1,10);
  
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

     return 0;
   }
