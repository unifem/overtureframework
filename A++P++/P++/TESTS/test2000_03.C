// Problem report 0-03-27-16-51-22

// Building a Partitioning_Type with no arguments, then assigning to it does 
// not update the left hand side's data.  Subsequent null constructors do 
// affect the data in the Partitioning_Type created first!?!

#include <iostream>
#include <A++.h>
using namespace std;
#define SHOWS_BUG  1
#define WORKAROUND_ONE 0
int
main( int argc, char *argv[], char *env[] )
   {
     int this_processor;
     int Max_Number_Of_Processors = 4;
// *wdh* 100924     Optimization_Manager::Initialize_Virtual_Machine ("", Max_Number_Of_Processors,argc,argv);
     Optimization_Manager::Initialize_Virtual_Machine ("", Max_Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 1
#if SHOWS_BUG
     Partitioning_Type thePartition;
     thePartition = Partitioning_Type(Range(0,0));
#else
#if WORKAROUND_ONE
     Partitioning_Type thePartition(Range(0,0)); // doesn't exhibit bug
#else
     Partitioning_Type thePartition;
     thePartition.SpecifyProcessorRange(Range(0,0)); // doesn't exhibit bug
#endif
#endif
#endif

#if 1    
     thePartition.display("first thePartition");
#endif
     Partitioning_Type theDefaultPartition;    
     theDefaultPartition.display("theDefaultPartition");

#if 1
     thePartition.display("second thePartition");
#endif

     Optimization_Manager::Exit_Virtual_Machine ();

  // ... includes MPI_Finalize();
     return 0;
   }

