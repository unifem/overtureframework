// Test program for the displayPartitioning() array member function.

#define BOUNDS_CHECK
#include <A++.h>

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on M++ array bounds checking!

     int Number_Of_Processors = 0;  // get number for default value from command line
     Optimization_Manager::Initialize_Virtual_Machine("",Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     printf ("Number_Of_Processors = %d \n",Number_Of_Processors);

     int Size = 10;
     Partitioning_Type Partition (Range(0,0));
     intArray AAA(Size,2*Size,4*Size,Partition);

     int i;
     for (i=0; i< Number_Of_Processors; i++)
        {
	  Partition.SpecifyProcessorRange (Range(0,i));
          AAA.displayPartitioning("AAA");
        }

     for (i=1; i< Number_Of_Processors; i++)
        {
	  Partition.SpecifyProcessorRange (Range(i,Number_Of_Processors-1));
          AAA.displayPartitioning("AAA");
        }

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }

