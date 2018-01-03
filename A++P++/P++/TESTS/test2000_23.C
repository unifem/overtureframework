// This example demonstrates and tests the use of the Diagnostic_Manager::getTotalMemoryInUse()
// function.



#define BOUNDS_CHECK
#include "A++.h"

MemoryManagerType  Memory_Manager;

int
main( int argc, char *argv[])
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     int i;
     int theNumberOfProcessors = -1;

     Optimization_Manager::Initialize_Virtual_Machine ("",theNumberOfProcessors,argc,argv);
     if ( !Internal_Partitioning_Type::isFloatTypeSupported() ) {
       cerr << "************* WARNING: not running this test! *************\n"
	    << "* This test requires support for float type which is not\n"
	    << "* supported under this PADRE configuration.\n"
	    << flush;
       Optimization_Manager::Exit_Virtual_Machine();
       exit(0);
     }

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);

  // Partitioning_Type thePartition(Range(0,0));
     intArray A(100);
     floatArray B(100);
     doubleArray C(100);

  // print out example of diagnostics report
     Diagnostic_Manager::report();
     
     Optimization_Manager::Exit_Virtual_Machine ();

     return 0;
   }







