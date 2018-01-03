#if 0
Hi,

the version of A++P++ I ve been using so far (for all bugreports) is
0.7.2c. Anyway, here is yet another bugreport.

When I run the attached code linked with purify. I get an UMR when I
call

pr.updateGhostBoundaries()

The message from purify is

      UMR: Uninitialized memory read
      This is occurring while in:
            MPI_Testall    [libmpich.a]
            MPI_Test       [libmpich.a]
            PARTI_MPI_msgdone [libPpp.a]
            dDataMove      [libPpp.a]
            void Internal_Partitioning_Type::updateGhostBoundaries(const
doubleArray&,const doubleSerialArray&) [libPpp.a]
            main           [Ptest4.C:19]
      Reading 4 bytes from 0xffbeea88 on the stack.
      Address 0xffbeea88 is       16 bytes below frame pointer in
function PARTI_MPI_msgdone.

It only occurs when using more than 1 process (naturally I guess), and
there are lots of other UMRs from the internal MPI functions, so I
dont know if it is an MPI error or an P++ error.

When running on the COMPASS with my real code this (at least I think so)
causes an floating-point exception after some iterations.

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

  doubleArray pr(10);
  pr = 1.;
  pr.updateGhostBoundaries();
  
  Optimization_Manager::Exit_Virtual_Machine();

  return 0;
}

