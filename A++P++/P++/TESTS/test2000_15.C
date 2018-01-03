// Code from Stefan which he thinks shows the use of ghost boundaries in computations.
// If this is true it could be a result of the incorrect fix to the overlap boundary update
// (the fix which largely disables that mechanism!).

// I think it works fine, but it shows (again) an interesting point about the use of the 
// C++ copy constructor and its use with a function as input.

#define BOUNDS_CHECK

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

     int gb = 0;
     if ( Internal_Partitioning_Type::isGhostBoundarySupported() ) gb = 1;
     Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(gb,gb);

     Index::setBoundsCheck(on);

     doubleArray pr(3,3);
     pr = 1.;

     pr.display("Here's pr : ");

     pr.updateGhostBoundaries();

#if 0
  // The getLocalArrayWithGhostBoundaries() function returns a shallow copy by definition
  // (or at least by the implementation).  When used with the copy constructor the 
  // copy constructor is optimized away.  This is because it appears as the second
  // in a sequence of constructor calles (the first one of which comes from return
  // of the constructor in the implementation of the getLocalArrayWithGhostBoundaries()
  // function.  This is a C++ detail which is a good reason to expect strange behavior
  // when a copy constructor is called using a function as it's input.
     doubleSerialArray lpr = pr.getLocalArrayWithGhostBoundaries();
#else
  // Better to call and assign the value separately
  // (forcing a deep copy which is what you wanted).
     doubleSerialArray lpr;
     lpr = pr.getLocalArrayWithGhostBoundaries();
#endif
     lpr = 5;
     lpr(lpr.getBase(0),lpr.getBase(1))=2.;

     lpr.display("Here's lpr : ");

     lpr = lpr*lpr;
     pr = pr*pr;

     lpr.display("Here's lpr : ");
     pr.display("Here's pr : ");

     pr.getLocalArrayWithGhostBoundaries() = 1.0;
     pr = 2.0;

     pr.getLocalArrayWithGhostBoundaries().display("Here is pr with the expanded ghost boundaries!");

     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }


