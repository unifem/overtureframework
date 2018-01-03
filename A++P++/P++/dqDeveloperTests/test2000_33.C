// Test code for P++ Array::displayArraySizesPerProcessor (const char* Label)

#define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
     Index::setBoundsCheck(on);  //  Turn on A++ array bounds checking

     int Number_Of_Processors = 0;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

     Communication_Manager::setOutputProcessor(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 1
     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);
#endif

  // Diagnostic_Manager::setReferenceCountingReport(1);

     Range R(0,99);
     intArray X(R);
     intArray X1(1);
     intArray X2(10);
     intArray X3(100);
     intArray X4(1000);
     X  = 0;
     X1 = 0;
     X2 = 0;
     X3 = 0;
     X4 = 0;

     printf ("Calling: X.displayArraySizesPerProcessor() \n");

     X.displayArraySizesPerProcessor();

     printf ("DONE: X.displayArraySizesPerProcessor() \n");

     printf ("Diagnostic_Manager::getNumberOfArraysInUse()   = %d \n",Diagnostic_Manager::getNumberOfArraysInUse());
     printf ("Diagnostic_Manager::getTotalArrayMemoryInUse() = %d \n",Diagnostic_Manager::getTotalArrayMemoryInUse());

     printf ("Program Terminated Normally! \n");
  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

     return 0;
   }



