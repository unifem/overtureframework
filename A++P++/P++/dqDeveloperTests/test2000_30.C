/*
DATE:               20:09:49 08/02/0
VISITOR:            Bill Henshaw
EMAIL:              henshaw@llnl.gov
CATEGORY:           A++/P++
CLASS:              Software bug
SEVERITY:           critical
SUMMARY:           
Setting partition object fails
ENVIRONMENT:        
Sun, /usr/casc/overture/A++P++/A++P++-DATE-00-07-17-TIME-17-17/SUN_CC/NODEBUG/A++P++/P++/lib/solaris_cc_CC
DESCRIPTION:        
Setting the partition object after calling SpecifyProcessorRange aborts with an assert
HOW TO REPEAT:      
See test code below, ~henshaw/res/P++/bug1.C
TEST CODE:          
*/

#include <A++.h>
typedef doubleArray realArray;

int
main( int argc, char *argv[])
   {
     Index::setBoundsCheck(on);
     ios::sync_with_stdio();     // Synchronize C++ and C I/O subsystems

     int Number_Of_Processors = 0;
     Optimization_Manager::Initialize_Virtual_Machine ("", Number_Of_Processors, argc, argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     realArray x1;
     Partitioning_Type  partition;

     Range P(0,0); 
#if 1
     APP_DEBUG = 2;
     partition.SpecifyProcessorRange(P); // this causes an error.
     APP_DEBUG = 0;
#endif

     x1.partition(partition);
     x1.redim(4,4);


  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }
