// John Lyon's code which fails in P++

#include <stdio>
#include <iostream>
#include <A++.h>
//#include "MHD.h"
class MHD {
 public:
// constructors just initialize the arrays
  MHD(int ni,int nj,int nk,int,Range) {
        X.redim(ni+1,nj+1,nk+1);
  }


     private:
// grid data
  floatArray X;
//  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//      more arrays for second time level
// cell centered variables
};

int dump_step(int, MHD*);

main(int argc, char** argv) {
int ndump=1, nstart=1, nstop =1;
int lstep;
float Dt;

 int num_proc = 6;
   int nmhd     = 4;
   int nion     = 1;
 int N_I=50; int N_J=24; int N_K=32; int N_ORDER=8;

 Optimization_Manager::Initialize_Virtual_Machine("",num_proc,argc,argv);

#ifdef _AIX
  // This call is made so that the Blue Pacific compiler will see the use of MPI directly
  // and then link to the appropriate library (libmpi).  mpCC determines the 
  // correct libraries to link and needs this to trigger linking to libmpi.
     MPI_Barrier(MPI_COMM_WORLD);
#endif

 int my_pe = Communication_Manager::My_Process_Number; 

// constructor for the MHD class
  Range mhd_procs(1,nmhd);
  MHD mhd = MHD(N_I,N_J,N_K,N_ORDER,mhd_procs);
   
// ifdef PARA_PPP
 Communication_Manager::Sync();
 Optimization_Manager::Exit_Virtual_Machine();
// endif
}
