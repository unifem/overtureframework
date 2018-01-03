#define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
  // MemoryManagerType  Memory_Manager;

     Index::setBoundsCheck(on);

     int Number_Of_Processors = 1;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

#if 1
     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);
#endif

     Diagnostic_Manager::setReferenceCountingReport(1);

     Range I(0,0);
     Range J(0,-1);
     intArray B(I);

     B = 0;

  // TYPEArray B(I,I);
  // B(I,0) = B(I,0) + B(I,1);  //original statement for testing
  // B(J) = B(J) + B(J+1);
     B(J) = B(J) + B(J);

     printf ("Program Terminated Normally! \n");

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

     return (0);
   }

