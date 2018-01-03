// Problem Report 99-10-07-13-12-38

// include "main.h"
#include "A++.h"

#define MAX_NO_PROC 4

void
test (int n)
   {
  // printf ("Top of test function n = %d \n",n);
     const int Size = 258;
     Index I (1,Size-2,1);
  // printf ("Build a Partitioning_Type n = %d \n",n);
     Partitioning_Type Partitioning(Range(0,n-1));
  // printf ("Build an array object n = %d \n",n);
     doubleArray U(Size,Partitioning);
  // printf ("Call fill member function n = %d \n",n);
     U.fill(0.0);
  // printf ("Call U(I) =  U(I-1) n = %d \n",n);
  // Communication_Manager::Sync();
     if (n > 1)
        {
       // APP_DEBUG = 5;
       // Communication_Manager::setOutputProcessor(1);
        }
  // U(I) =  U(I-1);
  // Communication_Manager::Sync();
  // printf ("Exiting test function n = %d \n",n);
   }

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

     for( int processors = 1; processors <= Number_Of_Processors; processors++)
          test(processors);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }

