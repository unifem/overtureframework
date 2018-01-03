// Problem report 0-04-04-08-34-31

#include <A++.h>

int
main( int argc, char *argv[] )
   {
#if 1
     Index::setBoundsCheck(on);
#endif
    
     int theNumberOfProcessors;
     Optimization_Manager::Initialize_Virtual_Machine("stringTest",theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

#ifndef USE_PPP
     AppString theString("{100,200}");
     intArray theArray( theString );
     theArray.display("theArray");
#endif
    
     Optimization_Manager::Exit_Virtual_Machine ();
  // ... includes MPI_Finalize();
     return 0;
   }

