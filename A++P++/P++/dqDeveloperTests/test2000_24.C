#if 0
/*
DATE:               10:49:54 07/17/0
VISITOR:            Brian Miller
EMAIL:              miller125@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           serious
SUMMARY:           
DEEPALIGNEDCOPY of a strided view behaves funny
ENVIRONMENT:        
Solaris, CC v4.2, 'cvs status array.C_m4'
Working revision:    1.16    Mon Jul 10 17:01:17 2000
DESCRIPTION:        
After building an array as a deep aligned copy of a strided view, scalar indexing 
does not behave as I would assume.  I could very well be confused about
how this is supposed to work!
HOW TO REPEAT:      
With #define STRIDED_RANGE commented out, the code runs as I would
like it to.
Now uncomment the #define and run the code.  
Notice that prior to the assignment loop, T=3.  After the loop
it should (in my opinion) be 0, but it is still 3 and purify reports an ABW.
TEST CODE:          
*/
#endif

#include "A++.h"

#if 0
#   undef STRIDED_RANGE
#else
#   define STRIDED_RANGE
#endif

int
main( int argc, char *argv[])
   {
     int theNumberOfProcessors = 0;

     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

     Communication_Manager::setOutputProcessor(0);

     Diagnostic_Manager::setTrackArrayData(TRUE);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int thisProcessorNumber = Communication_Manager::localProcessNumber();

     intArray J(10);
     J = 3;
     J.seqAdd(10);

#if defined(STRIDED_RANGE)
     Range I(3,9,2);
#else
     Range I(3,9);
#endif

  // J(Range(3,9)).view("J(Range(3,9))");
     I.display("theSubRange");

  // J(I).view("J(I)");
     J.displayPartitioning("Partitioning of J");

#if 1
     intArray T(J(I),DEEPALIGNEDCOPY);
     T = 42;

     T.displayPartitioning("Partitioning of T");

     T.display("T after construction.");
  // J(I).view("J(I)");
  // T.view("T after construction.");

     printf ("T.getLocalBase(0) = %d  T.getLocalBound(0) = %d  T.getLocalStride(0) = %d \n",
          T.getLocalBase(0),T.getLocalBound(0),T.getLocalStride(0));
#endif

#if 1
     Optimization_Manager::setOptimizedScalarIndexing(On);
#if 1
     for ( int i = T.getLocalBase(0); i <= T.getLocalBound(0); i += T.getLocalStride(0) )
        {
          T(i) = i;
        }
#else
     (J(I))(3) = 1;
     (J(I))(4) = 1;
     (J(I))(5) = 1;
     (J(I))(6) = 1;

  // T(1) = 1;
  // T(2) = 1;
     T(3) = 1;
     T(4) = 1;
     T(5) = 1;
  // T(6) = 1;
#if 0
     for ( int i = T.getLocalBase(0); i <= T.getLocalBase(0); i += 1 )
        {
          T(i) = i;
        }
#endif
#endif
     Optimization_Manager::setOptimizedScalarIndexing(Off);

     T.display("T after assignment to 0");
  // T.view("view of T after assignment to 0");
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

     printf ("Program Terminated Normally! \n");

     return 0;
   }










