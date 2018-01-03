#define BOUNDS_CHECK
#include <A++.h>

int
main(int argc,char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int num_of_process=4;
     Optimization_Manager::Initialize_Virtual_Machine(" ",num_of_process,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Range r1(0,8,2);      // Non-unit stride.
     intArray Ia0(r1);
     Ia0 = 1;
     int si = sum(Ia0);    // This causes the error asserting that stride == 1.

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();
   }
