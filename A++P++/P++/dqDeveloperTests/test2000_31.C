#include <A++.h>

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

#if 1
     Optimization_Manager::setForceVSG_Update(On);
#endif

     int size = 10;
     float y = 5.0;

     floatArray X(size);
     X = 0;
     floatArray Z(X);

     Diagnostic_Manager::setReferenceCountingReport(1);
     
#if 1
  // min(X,Z).view("min(X,Z)");
  // min(y,min(X,Z).view("min(X,Z)")).view("min(y,min(X,Z).view( min(X,Z)))");
  // where (X > 0)
  //      min(X,X).view("min(X,X)");
  //      Z = min(y,min(X,X.displayReferenceCounts("X")).displayReferenceCounts("min(X,X)")).displayReferenceCounts("min(y,min(X,Z).displayReferenceCounts( min(X,Z)))");

#if 0
     where(X > (float)0.0)
          Z = min(y, min(X,X));
#else
  // Z = min(y, min(X,X));
  // Z = min(y,X);
     Z = min(y,X+1);
#endif

  // X.displayReferenceCounts("X");

#else
     where (X > 0)
        {
       // Z = min(X,y,X); // fails
       // Z = min(X,y,y); // fails

          APP_DEBUG = 1;
       // Z = min(X,y,y).view("min(X,y,y)");
       // min(X,y,y).view("min(X,y,y)");
          min(X,y,X).view("min(X,y,X)");
          APP_DEBUG = 0;
        }
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     printf ("Program Terminated Normally! \n");

     Optimization_Manager::Exit_Virtual_Machine();
     return 0;
   }
