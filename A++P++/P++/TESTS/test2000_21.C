
// Test program for 

#define BOUNDS_CHECK
#include <A++.h>

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on M++ array bounds checking!

     int Number_Of_Processors = 0;  // get number for default value from command line
     Optimization_Manager::Initialize_Virtual_Machine("",Number_Of_Processors,argc,argv);
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

     printf ("Number_Of_Processors = %d \n",Number_Of_Processors);

     Index Ispan2(6,2,2);
  // Index Ispan2(6,20,2);

     floatArray WW(Ispan2);

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }

