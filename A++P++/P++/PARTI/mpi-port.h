#ifndef _mpi_port_h
#define _mpi_port_h

#ifdef __cplusplus
#define PR_BEGIN_EXTERN_C extern "C" {
#define PR_END_EXTERN_C }
#else
#define PR_BEGIN_EXTERN_C
#define PR_END_EXTERN_C
#endif

PR_BEGIN_EXTERN_C

/* Specified as part of P++ diagnostics */
extern int PARTI_numberOfMessagesSent;
extern int PARTI_numberOfMessagesRecieved;
extern int PARTI_messagePassingInterpretationReport;

int PARTI_MPI_numprocs();
int PARTI_MPI_gsync();

int PARTI_MPI_myproc();

int PARTI_MPI_csend(int tag, char* buf, int len, int node, MPI_Request* pid);
int PARTI_MPI_crecv(int tag, char* buf, int len, int node, MPI_Request* pid);

int PARTI_MPI_isend(int tag, char* buf, int len, int node, MPI_Request* pid);
int PARTI_MPI_irecv(int tag, char* buf, int len, int node, MPI_Request* pid);
int PARTI_MPI_msgwait(MPI_Request* gid);
int PARTI_MPI_msgdone(MPI_Request* gid);

PR_END_EXTERN_C

#endif
