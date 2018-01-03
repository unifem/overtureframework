// Demonstrate problem and test copy constructor with 
// new option (DEEPALIGNEDCOPY) to preserve the distribution

#define BOUNDS_CHECK
#include <A++.h>

void
testCopyConstructor ( int size, const intArray & X, const intArray & Y, const intArray & Z )
   {
  // This function builds multiple array objects using the option to allow then to be 
  // distributed aligned with the array object that they will copy.

#if 1
     intArray A(X,DEEPALIGNEDCOPY);

     if (A.isSameDistribution(X) == TRUE)
        {
       // passed
          printf ("PASSED: A has same distribution as X \n");
        }
       else
        {
       // failed
          printf ("FAILED: A has DIFFERENT distribution than X \n");
	  APP_ABORT();
        }
#endif

#if 1 
  // Case of index view (no stride)
     Range Bi(1,size-2);
     intArray B(X(Bi),DEEPALIGNEDCOPY);

     if (B.isSameDistribution(X) == TRUE)
        {
       // passed
          printf ("PASSED: B has same distribution as X \n");
        }
       else
        {
       // failed
          printf ("FAILED: B has DIFFERENT distribution than X \n");
	  APP_ABORT();
        }
#endif

#if 1
  // Case of strided index view (with stride)

     printf ("\n\n\n");
     printf ("Case of index view (with stride) \n");
     printf ("\n\n");

     X.display("X");
#if 1
  // This causes an error with PADRE!!!
     Range Ci(0,size-2,2);
#else
     Range Ci(0,size-2,1);
#endif
     Ci.display("Ci");

  // X(Ci).view("X(Ci)");

     APP_DEBUG = 0;
     intArray C(X(Ci),DEEPALIGNEDCOPY);
     APP_DEBUG = 0;

     if (C.isSameDistribution(X) == TRUE)
        {
       // passed
          printf ("PASSED: C has same distribution as X \n");
        }
       else
        {
       // failed
          printf ("FAILED: C has DIFFERENT distribution than X \n");
	  APP_ABORT();
        }
#endif

#if 1
  // Case of strided index view (with stride)

     printf ("\n\n\n");
     printf ("Case of index view (with stride) \n");
     printf ("\n\n");

     Range Di(1,size-1,2);
     Di.display("Di");

     APP_DEBUG = 0;
     intArray D(X(Di),DEEPALIGNEDCOPY);
     APP_DEBUG = 0;

     if (D.isSameDistribution(X) == TRUE)
        {
       // passed
          printf ("PASSED: D has same distribution as X \n");
        }
       else
        {
       // failed
          printf ("FAILED: D has DIFFERENT distribution than X \n");
	  APP_ABORT();
        }
#endif

#if 1
  // Case of 2D strided index view (with stride)

     printf ("\n\n\n");
     printf ("Case of 2D index view (with stride) \n");
     printf ("\n\n");

     Range Ei(0,size-1,2);
     Range Ej(1,size-1,2);
     Ei.display("Ei");
     Ej.display("Ej");

     intArray E(Y(Ei,Ej),DEEPALIGNEDCOPY);

     if (E.isSameDistribution(Y) == TRUE)
        {
       // passed
          printf ("PASSED: E has same distribution as Y \n");
        }
       else
        {
       // failed
          printf ("FAILED: E has DIFFERENT distribution than Y \n");
	  APP_ABORT();
        }
#endif

#if 1
  // Case of 3D strided index view (with stride)

     printf ("\n\n\n");
     printf ("Case of 3D index view (with stride) \n");
     printf ("\n\n");

     Range Fi(0,size-1,2);
     Range Fj(1,size-1,2);
     Range Fk(3,size,2);
     Fi.display("Fi");
     Fj.display("Fj");
     Fk.display("Fk");

     intArray F(Z(Fi,Fj,Fk),DEEPALIGNEDCOPY);

     if (F.isSameDistribution(Z) == TRUE)
        {
       // passed
          printf ("PASSED: F has same distribution as Z \n");
        }
       else
        {
       // failed
          printf ("FAILED: F has DIFFERENT distribution than Z \n");
	  APP_ABORT();
        }
#endif
   }


int
main(int argc, char** argv)
   {
     ios::sync_with_stdio();     // Syncs C++ and C I/O subsystems!
     Index::setBoundsCheck (On); // Turns on A++/P++ array bounds checking!

     int numberOfProcessors = 0;  // get number for default value from command line
     Optimization_Manager::Initialize_Virtual_Machine("",numberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#if 0
  // Restrict output to processor 1 only
     Communication_Manager::setOutputProcessor(0);
#endif

     Optimization_Manager::setForceVSG_Update(On);

  // Set reporting of message passing interpretation (for debugging)
     Diagnostic_Manager::setMessagePassingInterpretationReport(0);

     int size = 10;

#if 1
  // Partition initialll on one one processor and then redistribute to all other partitioning.
     Partitioning_Type Partition(Range(0,numberOfProcessors-1));

  // Build an array with the new partition
     intArray X(size,Partition);
     intArray Y(size,size,Partition);
     intArray Z(size,size,size,Partition);

  // initialize arrays
     X = 1;
     Y = 2;
     Z = 3;

     testCopyConstructor(size,X,Y,Z);
#else
  // Note that this case hanges for some reasons which I will invistigate later

  // Partition initialll on one one processor and then redistribute to all other partitioning.
     Partitioning_Type Partition(Range(0,0));

  // Build an array with the new partition
     intArray X(size,Partition);
     intArray Y(size,size,Partition);
     intArray Z(size,size,size,Partition);

  // initialize arrays
     X = 1;
     Y = 2;
     Z = 3;

  // different copy constructor options
  // intArray Y(X,SHALLOWCOPY);
  // intArray Y(X,DEEPCOPY);         // DEEPCOPY is same as DEEPCOLAPSEDCOPY
  // intArray Y(X,DEEPCOLAPSEDCOPY); // DEEPCOPY is same as DEEPCOLAPSEDCOPY
  // intArray Y(X,DEEPCOLAPSEDALIGNEDCOPY);
  // intArray Y(X,DEEPALIGNEDCOPY);

     int i;
     for (i=0; i < numberOfProcessors; i++)
        {
          printf ("Partition from processor 0 to %d \n",i);
          Partition.SpecifyProcessorRange (Range(0,i));
          testCopyConstructor(size,X,Y,Z);
        }

     printf ("Now Partition the other direction! \n");

     for (i=1; i < numberOfProcessors; i++)
        {
          printf ("Partition (in the other direction) from processor %d to %d \n",i,numberOfProcessors-1);
          Partition.SpecifyProcessorRange (Range(i,numberOfProcessors-1));
          testCopyConstructor(size,X,Y,Z);
        }
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine ();
     printf ("Program Terminated Normally! \n");
     return 0;
   }







