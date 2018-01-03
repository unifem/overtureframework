#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/param.h>

#include <assert.h>

// **************************************************************************
//                        MAIN PROGRAM FUNCTION
// **************************************************************************

#include <stdarg.h>
// *wdh* 100924 -- add const
void MPI_Printf(const char *fmt, ...)
{
#define BUFSIZE 128
    int rank;
    int tag=10, p,np;
    va_list args;
    char str[BUFSIZE];
    MPI_Status status;
    va_start(args,fmt);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &np);
    if (rank==0) {
        fprintf(stdout,"PE%3.3d: ",rank);
        vfprintf(stdout,fmt,args);
        for (p=1; p<np; p++) {
            MPI_Recv(str,BUFSIZE,MPI_CHAR,p,tag, MPI_COMM_WORLD, &status);
            fprintf(stdout,"PE%3.3d: ",p);
            fprintf(stdout,"%s",str);
        }
    } else {
        vsprintf(str,fmt,args);
        MPI_Send(str,BUFSIZE,MPI_CHAR,0,tag,MPI_COMM_WORLD);
    }
    va_end(args);
    fflush(stdout);

}

int main(int argc, char** argv)
   {
     int return_status = 0;
     int myProcessorNumber   = -1;
     int numberOfProcessors =  0;

     return_status = MPI_Init (&argc, &argv);
     assert(return_status == MPI_SUCCESS);

     return_status = MPI_Comm_rank(MPI_COMM_WORLD, &myProcessorNumber);
     assert(return_status == MPI_SUCCESS);

     MPI_Printf ("MPI Initialized (processor = %d) \n",myProcessorNumber);

     return_status = MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcessors);
     assert(return_status == MPI_SUCCESS);

     MPI_Printf ("MPI Initialized (processor = %d of %d processors) \n",myProcessorNumber,numberOfProcessors);

     return_status = MPI_Finalize();
     assert(return_status == MPI_SUCCESS);

  // printf ("Program Terminated Normally! \n");
     return 0;
   }
