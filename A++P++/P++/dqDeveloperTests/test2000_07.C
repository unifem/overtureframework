// Problem Report 99-10-27-16-18-24
//      different internal ghost boundaries make arrays non-conforming

// CAUSE OF THE PROBLEM:
// DQ Note: This is not implemented in P++, so it is an error to mix
// arrays with different ghost boundary widths in a simple expression.
// This limitation stems from the inability to define the communication schecdule for
// the ghost boundary updates (or any other communication) within Block-PARTI.

#include "A++.h"

int
main(int argc, char **argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on P++ array bounds checking!

     int Number_Of_Processors = 1;
     int checkpoint=0;

     Optimization_Manager::Initialize_Virtual_Machine ("",Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

  // single process partition for output 
     Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
     Partitioning_Type outputPartition(Range(0,0)), thePartition();

     printf ("Test 2 \n");

     int i,j,nx=32,ny=32;

     printf ("Test 3 argc = %d \n",argc);

     if(argc >= 2)
          nx = atoi( argv[1] );

     printf ("Test 4 \n");

     if(argc >= 3)
          ny = atoi( argv[2] );

     printf ("Test 5 \n");

     printf ("nx = %d  ny = %d \n",nx,ny);

     Range I(0,nx),J(0,ny);

     doubleArray u(I,J),uOutput(I,J);

  //
  // set partitions explicitly since below we rely on f, u, and 
  // uExact having the same partitions
  //
     uOutput.partition(outputPartition);

     u = 0.0;

  // here we copy a partitioned u into a single partition uOutput
  // so we can write uOutput to file
     uOutput = u;

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");
     Optimization_Manager::Exit_Virtual_Machine();
     printf ("Virtual Machine exited! \n");

     return 0;
   }
















