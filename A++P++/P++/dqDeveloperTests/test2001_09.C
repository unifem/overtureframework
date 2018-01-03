#include <A++.h>

int
main( int argc, char *argv[])
   {
#if 1
     Index::setBoundsCheck(on);
#endif

     int Number_Of_Processors = 0;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     intArray A(2,2);
     intArray B(2,2);
     A = 1;
     B = 2;

     Range I(1,1);

     A(I) = B(I);

#if 0
     B = 7;
     intArray & X = A(I);
     X = B;
#endif

     A.display();
     B.display();

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }




