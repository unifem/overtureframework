/*
Redim tests
*/

#define BOUNDS_CHECK
#include "A++.h"

int
main( int argc, char *argv[] )
   {
     Index::setBoundsCheck(on);

     int theNumberOfProcessors;
     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int size = 100;
     int ghostBoundaryWidth = 8;

     Partitioning_Type::SpecifyDefaultInternalGhostBoundaryWidths(ghostBoundaryWidth);
     Partitioning_Type P(Range(0,theNumberOfProcessors-1));

     intArray A(size,P);

     Range I1 (-ghostBoundaryWidth,0);
     Range I2 ((size-ghostBoundaryWidth)-1,size-1);

     if (ghostBoundaryWidth == 0)
        {
       // This will not work for ghostBoundaryWidth > 0 because it will access a region outside of A
       // (the hidden ghost boundaries).  See the next example for how to access the ghost boundaries.
          A.seqAdd(0);

          A.display ("A Before");

          A(I1) = A(I2);

          A.display ("A: After A(I1) = A(I2) WITHOUT getLocalArrayWithGhostBoundaries()");
        }

  // Initialize the whole domain including ghost boundaries between processors
  // and the external ghost boundaries at the physical boundaries of the array
     intSerialArray & A_withGhostBoundaries = A.getLocalArrayWithGhostBoundaries();

     A.getLocalArrayWithGhostBoundaries().seqAdd(0);

     A.getLocalArrayWithGhostBoundaries().display ("A Before");
     A_withGhostBoundaries.display ("A_withGhostBoundaries Before");

     if (theNumberOfProcessors == 1)
        {
       // This does not work in parallel
          A.getLocalArrayWithGhostBoundaries()(I1) = A.getLocalArrayWithGhostBoundaries()(I2);

          A.getLocalArrayWithGhostBoundaries().display ("A.getLocalArrayWithGhostBoundaries(): After A(I1) = A(I2) WITH getLocalArrayWithGhostBoundaries()");
        }
       else
        {
          printf ("Skipping parallel assignment of left ghost boundaries to right ghost boundaries \n");
        }


     Optimization_Manager::Exit_Virtual_Machine ();

     return 0;
   }

