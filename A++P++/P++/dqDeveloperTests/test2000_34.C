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
     Diagnostic_Manager::setMessagePassingInterpretationReport(0);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 1
     intArray J(10);
  // J = 3;
     J.seqAdd(10);

#if defined(STRIDED_RANGE)
     Range I(3,9,2);
#else
     Range I(3,9);
#endif

  // J(Range(3,9)).view("J(Range(3,9))");
     I.display("theSubRange");

  // J(I).view("J(I)");
  // J.displayPartitioning("Partitioning of J");

     intArray & view = J(I);

  // view = 17.0;
  // view.view("view");

     printf ("CALL T(J(I),DEEPALIGNEDCOPY) \n");

     intArray T(J(I),DEEPALIGNEDCOPY);
#else
     intArray J(4);
     J.seqAdd(4);

#if defined(STRIDED_RANGE)
     Range I(3,3,2);
#else
     Range I(3,3);
#endif

     I.display("theSubRange");

  // printf ("J(Range(1)).getDomain().Left_Number_Of_Points(0) = %d \n",J(Range(1)).getDomain().Left_Number_Of_Points[0]);
  // printf ("J(Range(1)).getDomain().Right_Number_Of_Points(0) = %d \n",J(Range(1)).getDomain().Right_Number_Of_Points[0]);

  // J(I).view("J(I)");
  // J.displayPartitioning("Partitioning of J");

     intArray & view = J(I);

  // view = 17.0;
  // view.view("view");

     printf ("@@@@@@@@@@@@@@@@@   CALL T(J(I),DEEPALIGNEDCOPY)    @@@@@@@@@@@@@@@@@@@@ \n");

     intArray T(J(I),DEEPALIGNEDCOPY);
#endif

     J.displayLeftRightNumberOfPoints("J");
     J(I).displayLeftRightNumberOfPoints("J(I)");
     T.displayLeftRightNumberOfPoints("T from T(J(I),DEEPALIGNEDCOPY)");

  // APP_ABORT();

#if 0
  // view.displayPartitioning("Partitioning of view");
     printf ("Exiting as a test after view.view() \n");
     APP_ABORT();

  // T = 42;
     T.seqAdd(0);
     T.view("T after construction.");
#endif

  // Use array domain's operator==
     printf ("Testing the PARALLEL array domains \n");
     APP_ASSERT (view.Array_Descriptor.Array_Domain == T.Array_Descriptor.Array_Domain);

  // Now test the serial arrays in each parallel array object
     printf ("Testing the SERIAL array domains \n");
     APP_ASSERT (view.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain == T.Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);


  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();

     return 0;
   }










