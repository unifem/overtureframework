#if 0
Hi, here is another one,
the attached code gets indexing out of bounds when run on more than one
process. The error goes away if I do not call
Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1)
in the beginning.

/Stefan
#endif



#include "A++.h"
#include "mpi.h"


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

  doubleArray pr(100,100);
  Index middle_x(1,100-2), middle_y(1,100-2);
  pr = 1.;
  
    pr(middle_x,middle_y) = pr(middle_x+1,middle_y) + pr(middle_x,middle_y-1) + 
      pr(middle_x-1,middle_y) + pr(middle_x,middle_y+1) +
      pr(middle_x+1,middle_y+1) + pr(middle_x-1,middle_y-1) + 
      pr(middle_x-1,middle_y+1) + pr(middle_x+1,middle_y-1) ;

  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}

