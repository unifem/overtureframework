/*
  This program replicates the bug wherein an assertion fails when
  assigning to a null array that carries its own distribution.
  Brian Gunney
 */

#define BOUNDS_CHECK
#include <A++.h>

int
main(int argc,char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     int num_of_process=4;
     Optimization_Manager::Initialize_Virtual_Machine(" ",num_of_process,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int ncex=4, ncey=4;
     int nptx=ncex+1, npty=ncey+1;
     doubleArray Solution(1,1,1,1,nptx,npty);

     Solution = 0;
     Partitioning_Type P(Range(0,0));      // Partition exclusively on processor 0.

     doubleArray psol(0,P);        // This method for defining a null array will cause an error on asisgnment below.
  // doubleArray psol(1,1,1,1,nptx,npty,P);     // This works fine.

#if 0
  // Using this will fix the bug (but it should work without this work around)
     psol.redim (Solution);
#endif
     psol = Solution;

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf("program terminated properly\n");

     Optimization_Manager::Exit_Virtual_Machine();
   }
