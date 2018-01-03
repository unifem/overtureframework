/*
Dan, I think I found a 4D dependence in the PARTI code.  Try running this:
You should get an output containing:
*** Fatal error: Node 0 : Subarrray_sched with more than 4D arrays not implemented ***
[0] MPI Abort by user Aborting program!
which is issued by PARTI/subsched.c (near tht bottom of the file).
*/

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

     intArray A0(5,4,4,4,4,4);
     intArray A1(5,4,4,4,4,4);
     A0 = 1;
     A1 = 0;
     Index all;
     Range i0(1,3,1);

     A1(i0,all,all,all,all,all) = A0(i0+1,all,all,all,all,all);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();
   }
