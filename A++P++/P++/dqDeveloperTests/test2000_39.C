#include "A++.h"

int
main( int argc, char *argv[])
   {
     int theNumberOfProcessors = 0;

     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Communication_Manager::setOutputProcessor(0);

     Diagnostic_Manager::setTrackArrayData(TRUE);
     Diagnostic_Manager::setMessagePassingInterpretationReport(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Partitioning_Type P(Range(0,0));

     intArray A(0,P);
     A = 0;
     A.view("Null Array: A");

     A.setBase(10,0);
     A.view("Null Array with base 10: A");

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

     return 0;
   }













