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
     Partitioning_Type Partition_A (Range(0,0));
     Partitioning_Type Partition_B (Range(0,0));
  // intArray A(Size,2*Size,4*Size,Partition_A);
     intArray A(Size,2*Size,4*Size,Partition_A);
     intArray B(Size,2*Size,4*Size,Partition_B);

#if 0
     printf ("A.Array_Descriptor.Array_Domain.Partitioning_Object_Pointer->DefaultintArrayList.getLength() = %d \n",
          A.Array_Descriptor.Array_Domain.Partitioning_Object_Pointer->DefaultintArrayList.getLength());
     printf ("A.Array_Descriptor.Array_Domain.Partitioning_Object_Pointer->intArrayList.getLength() = %d \n",
          A.Array_Descriptor.Array_Domain.Partitioning_Object_Pointer->intArrayList.getLength());
#endif

     A.seqAdd(0);
     B.seqAdd(0);

#if 0
     int i;
     for (i=0; i< Number_Of_Processors; i++)
        {
	  Partition_A.SpecifyProcessorRange (Range(0,i));
       // A.displayPartitioning("AAA");
        }

     for (i=1; i< Number_Of_Processors; i++)
        {
	  Partition_A.SpecifyProcessorRange (Range(i,Number_Of_Processors-1));
       // A.displayPartitioning("AAA");
        }
#endif

  // Check to see if the arrays have the same values
     if (sum(A != B) != 0)
        {
          printf ("FAILED: data changed as a result of redistribution! \n");
	  A.display("A");
	  B.display("B");
          APP_ABORT();
        }
       else
        {
          printf ("PASSED: data preserved properly after redistribution! \n");
        }

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }

