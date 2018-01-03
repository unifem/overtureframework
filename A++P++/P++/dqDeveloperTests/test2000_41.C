#define BOUNDS_CHECK
#include "A++.h"

int
main( int argc, char *argv[])
   {
     Index::setBoundsCheck(on);

     int theNumberOfProcessors = 0;

     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

     Communication_Manager::setOutputProcessor(0);

     Diagnostic_Manager::setTrackArrayData(TRUE);
     Diagnostic_Manager::setMessagePassingInterpretationReport(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     Partitioning_Type P(Range(0,1));

     int ghostBoundaryWidth = 1;
     int distributeAxis     = TRUE;
     P.partitionAlongAxis(0,distributeAxis,ghostBoundaryWidth);

     int size = 10;
     int indirectionVectorSize = 1; // size/2;

     intArray A(size,P);
     intArray B(3,3,3,P);
     intArray I1(indirectionVectorSize,P);
     intArray I2(indirectionVectorSize,P);
     intArray I3(indirectionVectorSize,P);

     I1 = 0;
     I2 = 1;
     I3 = 2;

     Range I(1,size-1);

     A = 1;
     A(I) = A(I) + A(I);

  // B(I1,I2,I3) = 1;

  // A.view("Null Array: A");
  // int sumOfA = sum(A);
  // printf ("sum(A) = %d \n",sumOfA);

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

     return 0;
   }













