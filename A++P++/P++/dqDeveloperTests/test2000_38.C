// P++ tests for scalar indexing

#include "A++.h"

int 
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int Number_of_Processors = 0;

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int size = 10;
     int i,j;

  // Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

  // Build a 1D array
     intArray A(size*size);
     intArray B(size,size);

     A = -1;
     for (i=0; i < size; i++)
          A(i) = i;

     A.display("A");

     B = -1;
     for (j=0; j < size; j++)
          for (i=0; i < size; i++)
               B(i,j) = j*size+i;

     B.display("B");

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}

