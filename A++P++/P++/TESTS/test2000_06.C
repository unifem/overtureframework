// Problem Report 99-10-07-13-12-38

// include "main.h"
#include "A++.h"

#define MAX_NO_PROC 4

void
test (int n)
   {
     const int Size = 258;
     Index I (1,Size-2,1);
     Partitioning_Type Partitioning(Range(0,n-1));
     doubleArray U(Size,Partitioning);
     U.fill(0.0);
     U(I) =  U(I-1);
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

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }

