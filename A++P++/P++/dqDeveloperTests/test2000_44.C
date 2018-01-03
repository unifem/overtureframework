/*
Redim tests
*/

#include "A++.h"

class Container
   {
     public:
          floatArray  X;
          intArray    Y;
          doubleArray Z;

          Container ()
             {
               X.redim(100,100);
               Y.redim(100,100);
               Z.redim(100,100);
             }
   };



int
main( int argc, char *argv[] )
   {
     int theNumberOfProcessors;
     Optimization_Manager::Initialize_Virtual_Machine ("", theNumberOfProcessors,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

     int size = 10;

     floatArray A;
     intArray B;
     intArray C(size);
     floatArray D(size,size);
     intArray E(size,size,size);
     floatArray F(size,size);

     int i=0;
     for (i = 1; i <= size; i++)
        {
          A.redim (i,i,i);
          B.redim (i,i,i);
          C.redim (i,i,i);
          D.redim (i*2,i,i);
          E.redim (i,i*2,i);
          F.redim (i,i,i*2);
        }
     
     for (i = size; i >= 0; i--)
        {
          A.redim (i,i,i);
          B.redim (i,i,i);
          C.redim (i,i,i);
          D.redim (i*2,i,i);
          E.redim (i,i*2,i);
          F.redim (i,i,i*2);
        }

     Container foo;

     Optimization_Manager::Exit_Virtual_Machine ();

     return 0;
   }

