// Test Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON); with purify
// This seems to be a bug in P++.

#include "A++.h"

int 
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int Number_of_Processors = 0;

#if 1
  // This causes a purify FMW: Free memory write: and FMR: Free memory read:
  // It is a current bug in P++ (I think)
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);
#endif

  // Diagnostic_Manager::setReferenceCountingReport(1);

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  // Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

  // Allocate an array object
//   doubleArray X(100,100);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

#if 0
  // This causes a purify FMW: Free memory write: and FMR: Free memory read:
  // It is a current bug in P++ (I think)
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);
#endif

     Optimization_Manager::Exit_Virtual_Machine();

     printf ("BASE of MAIN: Number of arrays in use = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());

     GlobalMemoryRelease();

     return 0;
   }



