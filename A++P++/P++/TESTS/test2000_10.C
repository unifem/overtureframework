#if 0
Hi again,

If I change the defaultGhostCellWidth to (1,1), this does not work with
Partitioning_Type. See the attached example file.

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

  Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(1,1);

  Index::setBoundsCheck(on);

  Partitioning_Type DD;

  DD.display();
  
  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}

