#include "GenericDataBase.h"
#include "OvertureTypes.h"

#define SECOND EXTERN_C_NAME(second)
extern "C" { 
  void SECOND( real & time );
}

//=============================================================
//  Return the current value of the amount of CPU time used
//============================================================
real getCPU()
{
  real time;
  #ifndef USE_PPP
    SECOND( time );
  #else
    time= MPI_Wtime(); 
  #endif
  return time;
}    
