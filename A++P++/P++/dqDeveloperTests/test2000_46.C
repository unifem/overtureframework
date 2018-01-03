
/*
DATE:               11:22:49 11/01/0
VISITOR:            Stefan Nilsson
EMAIL:              nilsson2@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           critical
SUMMARY:           
P++ leaks memory 
ENVIRONMENT:        
Sun,DEC version 0.7.5b (also a newer version on Sun) of P++ both with and without PADRE
DESCRIPTION:        
When evaluating P++ expressions involving more than two terms on the RHS P++ leaks memory. 
HOW TO REPEAT:      
Run the submitted code and check memory use with 'top'. Purify does not report any leaks, but it shows clearly using top.
TEST CODE:          
*/

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

  // Turn on diagnostics
     Diagnostic_Manager::setTrackArrayData(TRUE);

  // Turn on internal release of memory
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

  // Exit from GlobalMemoryRelease
     Diagnostic_Manager::setExitFromGlobalMemoryRelease(TRUE);

  // Turn on Reference Counting information
     Diagnostic_Manager::setReferenceCountingReport(FALSE);

     Index::setBoundsCheck(on);

     Range I(0,9);

     int i = 0;

#if 0
     doubleArray x(10);
     x = 1.0;
     doubleArray y(10);
     y = 0.0;

  // The trinary expression in this loop is where P++ leaks memory
     for (i=0; i < 1; i++)
        {
       // Count at TOP of loop
       // printf ("TOP OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());

       // U += x - x + x;
          y = x + 1;

       // Count at BOTTOM of loop
          printf ("BOTTOM OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());
#if 0
          x.displayReferenceCounts("loop: x");
          y.displayReferenceCounts("loop: y");

          if (i >= 0)
             {
               printf ("i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());
               Diagnostic_Manager::report();
             }
#endif
        }
#endif

#if 0
     doubleSerialArray A(10);
     A = 1.0;
     doubleSerialArray B(10);
     B = 0.0;

  // Turn on Reference Counting information
     Diagnostic_Manager::setReferenceCountingReport(FALSE);

     for (i=0; i < 5; i++)
        {
       // Count at TOP of loop
          printf ("TOP OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());

       // This line demonstrates an error
          A = B(I) + 1;

       // Count at BOTTOM of loop
          printf ("BOTTOM OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());
        }
#endif

     intArray C(10);
     C = 1;
  // intArray D(10);
  // D = 0;

     for (i=0; i < 1; i++)
        {
       // Count at TOP of loop
          printf ("TOP OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());

       // This line demonstrates an error
       // A = B(I) + 1;
       // int m1 = 0;
       // where (C > 1 && C < 9) m1 = 0; // max(C);
       // where (A>(TYPE)1. && A<(TYPE)9.) m1 = max(A);  // only value besides 1 
       // C = (C > 1 && C < 9);
       // C = (C + 1) && (C + 1);
       // C = (C+1) && (C+1);
       // C = (C+1) + C;
#if 0
       // Previous problem statement!
          C = (C+1) && (C+1);

          int m1 = 0;
       // where (C<(TYPE)9.) m1 = max(C+C);
          m1 = max(C+C);
          C = -(C+C);

          m1 = sum(C+1);

          where ( C > 1 && C < 9 ) m1 = max(C);
#endif

          intArray P1;
          P1 = C;

       // Count at BOTTOM of loop
          printf ("BOTTOM OF LOOP: i = %d #ofArraysInUse = %d \n",i,Diagnostic_Manager::getNumberOfArraysInUse());
        }

     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }

















