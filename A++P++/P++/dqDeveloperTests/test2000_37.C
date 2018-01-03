#include "A++.h"

int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();

     int Number_of_Processors=0;

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Communication_Manager::setOutputProcessor(0);

#if 1
     Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);
#endif

     Index::setBoundsCheck(on);

     Partitioning_Type P;

  // Specify that P should have zero ghost boundary widths along the first axis and non be partitioned
     P.partitionAlongAxis(0,FALSE,0);

  // P.display();

     intArray A(16);
     intArray B(16);
     intArray C(1,16,P);

     A = 1;
     B = 1;

  // A.view("A");

     A.seqAdd(10);
     B.seqAdd(10);
     C.seqAdd(10);

#if 0
     int i;
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf ("A.getInternalGhostCellWidth(%d) = %d \n",i,A.getInternalGhostCellWidth(i));

     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf ("A.getGhostBoundaryWidth(%d) = %d \n",i,A.getGhostBoundaryWidth(i));
#endif

  // A.view("A before reshape(1,16)");

  // printf ("Scalar indexing tests before reshape(16)! \n");
  // A(7) = 50;

     printf ("Now call reshape! \n");

     C.reshape(16);

#if 0
     A.reshape(1,16);

  // printf ("Scalar indexing tests after reshape(1,16)! \n");
  // A(0,0) = 2;

  // A.view("A after reshape(2,8)");
  // B.view("B(1,16)");

     if ( sum(A != C) != 0 )
        {
          printf ("ERROR: FAILED test: (A.reshape(1,16) != C(1,16))! \n");
	  APP_ABORT();
        }
       else
        {
          printf ("A.reshape(1,16) = C(1,16) : PASSED test! \n");
        }
#endif

#if 0
     A.reshape(16);

  // A.view("A after reshape(16)");

  // printf ("Scalar indexing tests after reshape(16)! \n");
  // A(10) = 3;

     if ( sum(A != B) != 0 )
        {
          printf ("ERROR: FAILED A.reshape(16) == B(16) test! \n");
	  APP_ABORT();
        }
       else
        {
          printf ("A.reshape(16) = B(16) : PASSED test! \n");
        }

  // Destructive test of array object (tests scalar indexing a many other things)
     Diagnostic_Manager::test(A);
     Diagnostic_Manager::test(B);
     Diagnostic_Manager::test(C);
#endif

#if 0
  // Now generate an error to make sure that errors are reported properly
  // This works but we need to comment it out from the tests so that this test code will pass properly
     A.reshape(2,8);
     Diagnostic_Manager::test(A);
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();

     printf ("Program Terminated Normally! \n");
     return 0;
   }











