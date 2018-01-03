#define BOUNDS_CHECK
#include<A++.h>
#include<iostream>

#define TYPE float
#define TYPEArray floatArray
#define TYPESerialArray floatSerialArray

int
main(int argc, char** argv)
   {
     MemoryManagerType  Memory_Manager;
     int Number_Of_Processors = 1;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 0
     Diagnostic_Manager::setTrackArrayData();
     APP_ASSERT (Diagnostic_Manager::getTrackArrayData() == TRUE);
#endif

  // Diagnostic_Manager::setMessagePassingInterpretationReport(1);
     Diagnostic_Manager::setReferenceCountingReport(1);

     TYPEArray A(10);

     A = 1.0;
     TYPE maxValue = 0;
#if 0
     maxValue = max(A);
#endif

#if 0
     where (A>(TYPE)1. && A<(TYPE)9.)
          maxValue = max(A);
#endif

#if 0
     intArray B(10);
     intArray C(10);
     intArray D(10);
     B = 1;
     C = 1;
     D = 1;

  // B = (A>(TYPE)1. && A<(TYPE)9.);
  // B = (B && (A < (TYPE)9.0));
     D = (B && C);
#endif

#if 0
     intArray X(10);
     intArray Y(10);
     intArray Z(10);
     X = 1;
     Y = 1;
     Z = 1;

     X = -(Y+Z);
#endif

#if 0
  // Could not get this code to generate error
     intArray A1(10,10);
     intArray A2(10,10);
     intArray A3(10,10);
     Index all;

     A1 = 1;
     A2 = 2;
     A3 = 3;

     intArray H1,H2;

     H1.reference((A1-A2)(all,1));
     H2 = H1(all,1);
#endif

#if 1
     intArray B1 (10,5);
     B1.redim(5,10);
     B1.resize(10,5);
#endif

     printf ("Program Terminated Normally! \n");

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Virtual Machine exited! \n");

  //===========================================================

  // use purify exit mechanism
  // purify_exit(0);
     return 0;
   }



