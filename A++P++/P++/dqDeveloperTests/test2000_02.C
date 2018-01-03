// Problem report 0-03-17-10-01-05

#include <A++.h>

int
main( int argc, char *argv[], char *env[] )
   {
     int Max_Number_Of_Processors = 4;
  // Optimization_Manager::Initialize_Virtual_Machine("IndirectP",Max_Number_Of_Processors,argc,argv);
     Optimization_Manager::Initialize_Virtual_Machine("",Max_Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     intArray J(Range(1,40));

     Range theSubRange(2,38,2);

     J(theSubRange).getFullRange(0).display("getFullRange of a view");

  // generate an error if the stride is not equal to 1!
     if (J(theSubRange).getFullRange(0).getStride() != 1)
        {
          printf ("ERROR: getFullRange member function not reporting the correct stride (should be 1) \n");
	  APP_ABORT();
        }
       else
        {
          printf ("PASSED: getFullRange member function reports stride = 1 (correct) \n");
        }
     
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");
    
  // ... includes MPI_Finalize();
     Optimization_Manager::Exit_Virtual_Machine ();
     return 0;
   }

