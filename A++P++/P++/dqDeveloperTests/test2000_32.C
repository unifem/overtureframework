// This test code was used to test the diagnostics array counting mechansim (total memory use)
// it represented a bug where in the VSG update the diagnostics saved a pointer to an arrya domain
// which went out of scope. The diagnostic mechanism was changed to avoid using references to 
// existing array domains (because there is not particular order in which they will be deleted in general).

#define BOUNDS_CHECK
#include<A++.h>

int
main(int argc, char** argv)
   {
     MemoryManagerType  Memory_Manager;

     Index::setBoundsCheck(on);

     int Number_Of_Processors = 1;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

     Communication_Manager::setOutputProcessor(1);

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

#if 0
     Range I(0,1);
     intArray B(I,I);

     B = 0;

     B(I,0) = B(I,0) + B(I,1);  //original statement for testing
#else
     Range I(0,1);
     Range J(0,0);
     intArray B(I);
     intArray C(J);

     B = 0;
     C = 1;

     printf ("\n\n\nCalling C(J) = B(J) + B(J+1); \n\n\n\n");

  // B(J) = B(J) + B(J+1); // generates memory leak on node 1 of 2 processors
  // B(J) = B(J) + B(J);
  // B(J) = B(J) + 1;  // case that failed on 1 processor

  // C(J) = B(J) + B(J+1);

  // Diagnostic_Manager::setReferenceCountingReport(1);

     C(J) = B(J) + B(J+1);

  // Diagnostic_Manager::setReferenceCountingReport(0);
#endif

     printf ("Program Terminated Normally! \n");

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

     return (0);
   }



