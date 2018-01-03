#include "port.h"

#ifdef PVM
#define DOUBLE_TYPE double
#define INT_TYPE int
#define GLOBAL_SUM_MSG	501

/* Correction by Dan Quinlan to allow use with P++ which has a MAX_PROCESSORS macro defined */
/* #define MAX_PROCESSORS 1024 */
/* use new include file to specify the number of processors. wdh and jwb 100924. */
#include "../include/maxProcessors.h"

/* int ar[NUMNODES]; */
int ar[MAX_PROCESSORS];   /* Used in pvm_mcast on line 69 below (and tested against MAX_PROCESSORS bound */

void PARTI_killall()
{
       if(pvm_lvgroup(EXECNAME) < 0)
                pvm_perror(EXECNAME);
	pvm_exit();
}


int jimsend(tag,buf,len,node,pid)
int tag, len, node, pid;
char *buf;
{
#if 0
        int i = 0;
        double* Print_Buffer = (double*) buf;
        printf ("Message buffer at %p (%d) TO SEND: ",buf,buf);
        for (i=0; i < len / sizeof(double); i++)
             printf (" %f",Print_Buffer[i]);
        printf ("\n");
#endif
        pvm_initsend( PvmDataDefault );
        pvm_pkbyte( buf, len, 1);
        pvm_send( pvm_gettid(EXECNAME,node), tag );
   return 0;
}

int jimrecv(tag,buf,len,node,pid)
int tag, len, node, pid;
char *buf;
{
#if 0
        int i = 0;
        double* Print_Buffer = (double*) buf;
#endif
        pvm_recv( pvm_gettid(EXECNAME,node), tag );
        pvm_upkbyte( buf, len, 1);
#if 0
        printf ("Message buffer at %p (%d) RECEIVED: ",buf,buf);
        for (i=0; i < len / sizeof(double); i++)
             printf (" %f",Print_Buffer[i]);
        printf ("\n");
#endif
   return 0;
}


/*
 * PARTI_gdsum
 * Perform a global sum (double) on a (virtual) hypercube structured network.
 * Limitations : Works only networks of size 2^n, n>=0.
 * Mustafa Uysal - Nov 27, 92
 * 3.x-ified:  Jim Humphries July 23, 93
 */
void PARTI_gdsum(x, n, work)
   double *x, *work;
   int n;
{
  int iter_count;
  int i,j;


   if (PARTI_myproc() == 0)
     {
       iter_count = PARTI_numprocs();
       for (i=0; i<iter_count-1;i++)
         {
           pvm_recv(-1,GLOBAL_SUM_MSG);
           pvm_upkbyte((char*)work, n*sizeof(DOUBLE_TYPE),1);
           /* compute the local sum */
           for (j=0; j<n; j++)
             x[j] += work[j];
         }

       pvm_initsend( PvmDataDefault );
       pvm_pkbyte((char*)x, n*sizeof(DOUBLE_TYPE),1);
       pvm_mcast(ar, PARTI_numprocs(), GLOBAL_SUM_MSG);
     }
   else
     {
       pvm_initsend( PvmDataDefault);
       pvm_pkbyte((char*)x, n*sizeof(DOUBLE_TYPE),1);
       pvm_send(pvm_gettid(EXECNAME,0), GLOBAL_SUM_MSG);
       pvm_recv(-1,GLOBAL_SUM_MSG);
       pvm_upkbyte((char*)x, n*sizeof(DOUBLE_TYPE),1);
     }
}

int myproc(node)
int node;
{
	static int MYGID;
	if(node>=0)
		MYGID = node;

	return MYGID;
}
#endif


/* for SP1 */

#ifdef SP1


int     PARTI_source;           /* source node of incoming message */
int     PARTI_type;             /* type of incoming message */
int     PARTI_nbytes;           /* number of bytes received */

int
mpc_numnodes()

{
    int   numnodes, mynode;

    mpc_environ (&numnodes, &mynode);
    return (numnodes);
}

int
mpc_numnodes_()

{
    return (mpc_numnodes());
}

/* mynode functions */
int
mpc_mynode()

{
    int   numnodes, mynode;

    mpc_environ (&numnodes, &mynode);
    return (mynode);
}

int
mpc_mynode_()
{
    return (mpc_mynode());
}

/* global synchrouization function */
mpc_gsync()

{
    mpc_sync (ALLGRP);
}

mpc_gsync_()

{
    mpc_sync (ALLGRP);
}

/* blocking send function */
mpc_csend (buffer, nbyte, node, type)
char  *buffer;
int   nbyte, node, type;

{
    mpc_bsend (buffer, nbyte, node, type);
}

/* wrapper for blocking send */
mpc_csend_ (buffer, nbyte, node, type)
char  *buffer;
int   *nbyte, *node, *type;
{
    mpc_bsend (buffer, *nbyte, *node, *type);
}

/* wrapper for blocking send */
mpc_bsend_ (buffer, nbyte, node, type)
char  *buffer;
int   *nbyte, *node, *type;

{
    mpc_bsend (buffer, *nbyte, *node, *type);
}

/* blocking receive function */
mpc_crecv (type, buffer, length, node)
int   type;                     /* type of the incoming message */
char  *buffer;                  /* buffer */
int   length;                   /* length of incoming message */
int   node;                     /* source of incoming message */

{
    PARTI_source = node;
    PARTI_type   = type;

    /* call SP-1 EUI blocking receive function */
    mpc_brecv (buffer, length, &PARTI_source, &PARTI_type, &PARTI_nbytes);
}

/* wrapper for blocking receive */
mpc_crecv_ (type, buffer, nbyte, node)
char  *buffer;
int   *nbyte, *node, *type;
{
    mpc_crecv (*type, buffer, *nbyte, *node);
}

/* nonblocking receive function */
int
mpc_irecv (type, buffer, length, node)
int   type;                     /* type of the incoming message */
char  *buffer;                  /* buffer */
int   length;                   /* length of incoming message */
int   node;                     /* source of incoming message */

{
    int   msgid;                /* message id of nonblocking receive */

    PARTI_source = node;
    PARTI_type   = type;

    /* call SP-1 EUI blocking receive function */
    mpc_recv (buffer, length, &PARTI_source, &PARTI_type, &msgid);

    /* return the message id */
    return (msgid);
}
/* nonblocking send function */
int
mpc_isend (type, buffer, length, node)
int   type;                     /* type of the incoming message */
char  *buffer;                  /* buffer */
int   length;                   /* length of incoming message */
int   node;                     /* source of incoming message */

{
    int   msgid;                /* message id of nonblocking receive */

    /* call SP-1 EUI blocking send function */
    mpc_send (buffer, length, node, type, &msgid);

    /* return the message id */
    return (msgid);
}

/* global integer combine function */
mpc_gisum (data, n, work)
int   *data;
int   n;
int   *work;

{
    int    i;
    /* call global combine function */
    mpc_combine (data, work, n*sizeof(int), i_vadd, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}

mpc_gisum_ (data, n, work)
int   *data;
int   *n;
int   *work;

{
    mpc_gisum (data, *n, work);
}

/* global integer combine function */
mpc_gihigh (data, n, work)
int   *data;
int   n;
int   *work;

{
    int    i;

    /* call global combine function */
    mpc_combine (data, work, n*sizeof(int), i_vmax, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}

mpc_gihigh_ (data, n, work)
int   *data;
int   *n;
int   *work;

{
    mpc_gihigh (data, *n, work);
}

/* global integer combine function */
mpc_gilow (data, n, work)
int   *data;
int   n;
int   *work;

{
    int    i;

    /* call global combine function */
    mpc_combine (data, work, n*sizeof(int), i_vmin, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}

mpc_gilow_ (data, n, work)
int   *data;
int   *n;
int   *work;

{
    mpc_gilow (data, *n, work);
}

/* global double combine function */
mpc_gdsum (data, n, work)
double  *data;
int     n;
double  *work;

{
    int    i;

    /* call global combine function */
    mpc_combine (data, work, n*sizeof(double), d_vadd, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}
mpc_gdsum_ (data, n, work)
double  *data;
int     *n;
double  *work;

{
    mpc_gdsum (data, *n, work);
}
/* global real combine function */
mpc_gdhigh (data, n, work)
double *data;
int    n;
double *work;

{
    int    i;

    /* call global combine function */
    mpc_combine (data, work, n*sizeof(double), d_vmax, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}

mpc_gdhigh_ (data, n, work)
double *data;
int    *n;
double *work;

{
    mpc_gdhigh (data, *n, work);
}

/* global integer combine function */
mpc_gdlow (data, n, work)
double *data;
int    n;
double *work;

{
    int    i;

    /* call global combine function */
    mpc_combine (data, work, n*sizeof(double), d_vmin, ALLGRP);

    /* copy the result from work to data */
    for (i = 0; i < n; i++)
        data[i] = work[i];
}

mpc_gdlow_ (data, n, work)
double *data;
int    *n;
double *work;

{
    mpc_gdlow (data, *n, work);
}

/*
 * interface of FORTRAN eui functions
 */
void
mp_bsend_(outmsg, msglen, dest, type)
int  *outmsg, *msglen, *dest, *type;

{
    mp_bsend (outmsg, msglen, dest, type);
}

void
mp_brecv_(inmsg, msglen, source, type, nbyte)
int  *inmsg, *msglen, *source, *type, *nbyte;

{
    mp_brecv (inmsg, msglen, source, type, nbyte);
}

void
mp_environ_(numtask, taskid)
int  *numtask, *taskid;

{
    mp_environ (numtask, taskid);
}

void
mp_combine_(outmsg, inmsg, msglen, func, gid)
int  *outmsg, *inmsg, *msglen, *gid;
void  (*func)();

{
    mp_combine (outmsg, inmsg, msglen, func, gid);
}

/*
void
tb0time_(time)
double  *time;

{
    tb0time (time);
}

*/
void
mp_sync_(gid)
int  *gid;

{
    mp_sync (gid);
}

void
mp_task_query_(buf, nelem, qtype)
int  *buf, *nelem, *qtype;

{
    mp_task_query (buf, nelem, qtype);
}


#endif
