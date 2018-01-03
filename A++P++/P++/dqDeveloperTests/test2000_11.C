#if 0
Hi, here is another one,
the attached code gets indexing out of bounds when run on more than one
process. The error goes away if I do not call
Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1)
in the beginning.

/Stefan
#endif



#include "A++.h"
// #include "mpi.h"


int 
main(int argc, char** argv)
   {
     ios::sync_with_stdio();
     Index::setBoundsCheck(on);

     int Number_of_Processors=0;

  // This causes a purify FMW: Free memory write: and FMR: Free memory read:
  // It is a current bug in P++ (I think)
     Diagnostic_Manager::setSmartReleaseOfInternalMemory(ON);

  // Diagnostic_Manager::setReferenceCountingReport(1);

     Optimization_Manager::Initialize_Virtual_Machine("",Number_of_Processors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

//   Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

#if 0
  // Note from DQ: I thought this would fix the problem but it does not!
     Optimization_Manager::setForceVSG_Update(On);
#endif

#if 1
     doubleArray pr(100,100);
  // doubleArray X(100,100);
#else
     doubleSerialArray pr(100,100);
#endif

     Index middle_x(1,98), middle_y(1,98);
     pr = 1.;

#if 0
  // pr.view("pr");

     pr(middle_x,middle_y) = pr(middle_x+1,middle_y) + pr(middle_x,middle_y-1) + 
          pr(middle_x-1,middle_y) + pr(middle_x,middle_y+1) +
          pr(middle_x+1,middle_y+1) + pr(middle_x-1,middle_y-1) + 
          pr(middle_x-1,middle_y+1) + pr(middle_x+1,middle_y-1) ;
#else
  // pr(middle_x,middle_y) = pr(middle_x+1,middle_y) + pr(middle_x,middle_y-1) +
  //      pr(middle_x-1,middle_y) + pr(middle_x,middle_y+1);
  // pr(middle_x,middle_y) = pr(middle_x,middle_y) + pr(middle_x,middle_y) + pr(middle_x,middle_y);
  // pr = pr + (pr + pr);
  // pr = (pr + pr) + pr;
  // pr = (pr + pr) + 1.0;
#endif

  // Call the diagnostics mechanism to display memory usage
     Diagnostic_Manager::report();

     Optimization_Manager::Exit_Virtual_Machine();

     return 0;
   }



