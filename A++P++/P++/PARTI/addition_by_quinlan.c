#include<stdio.h>
#include "utils.h"
#include "hash.h"
#include "bsparti.h"
#include"port.h"
#include"List.h"

/* Copied from datamove.c */
#define GRPSIZE                 64
#define NTYPE1                  100
#define NODE_PID                0
/* wdh 100924 #define MAX_NODES               128 */


#ifdef  SP1
int     PARTI_source;           /* source node of incoming message */
int     PARTI_type;             /* type of incoming message */
int     PARTI_nbytes;           /* number of bytes received */
#endif

/* everybody but PVM uses nonblocking sends/receives */
extern char *RmsgBuf[GRPSIZE],*SmsgBuf[GRPSIZE],       /* receive _and_ send buffers */
        *local_msgBuf;
extern int  RmsgBuf_size[GRPSIZE],SmsgBuf_size[GRPSIZE],
        local_msgBuf_size;

void cleanup_da_table();
void cleanup_subarray_table();
void cleanup_exch_table();

void cleanup_after_PARTI ()
   {
     int i = 0;
#if 1
  /* printf ("Inside of cleanup_after_PARTI! \n"); */
  /* printf ("local_msgBuf = %p \n",local_msgBuf); */

     if (local_msgBuf != NULL) 
        {
          free (local_msgBuf);
          local_msgBuf = NULL;
        }

     for (i=0; i < GRPSIZE; i++)
        {
       /* printf ("RmsgBuf[%d] = %p  SmsgBuf[%d] = %p \n",i,RmsgBuf[i],i,SmsgBuf[i]); */
          if (RmsgBuf[i] != NULL) 
             {
               free (RmsgBuf[i]);
               RmsgBuf[i] = NULL;
             }
          if (SmsgBuf[i] != NULL) 
             {
               free (SmsgBuf[i]);
               SmsgBuf[i] = NULL;
             }
        }
#endif

#if 1
     cleanup_da_table();
     cleanup_subarray_table();
     cleanup_exch_table();
#endif
   }

void delete_DARRAY ( DARRAY* X )
   {
  /* printf ("In delete_DARRAY: X->referenceCount = %d \n",X->referenceCount);                 */
  /* printf ("In delete_DARRAY: X->decomp->referenceCount = %d \n",X->decomp->referenceCount); */
     assert (X->referenceCount >= 0);
     if (X->referenceCount-- == 0)
        {
          if (X->ghostCells != NULL) free(X->ghostCells);
          X->ghostCells = NULL;
          if (X->dimVecG != NULL) free(X->dimVecG);
          X->dimVecG = NULL;
          if (X->dimVecL != NULL) free(X->dimVecL);
          X->dimVecL = NULL;
          if (X->dimVecL_L != NULL) free(X->dimVecL_L);
          X->dimVecL_L = NULL;
          if (X->dimVecL_R != NULL) free(X->dimVecL_R);
          X->dimVecL_R = NULL;
          if (X->g_index_low != NULL) free(X->g_index_low);
          X->g_index_low = NULL;
          if (X->g_index_hi != NULL) free(X->g_index_hi);
          X->g_index_hi = NULL;
          if (X->local_size != NULL) free(X->local_size);
          X->local_size = NULL;
          if (X->decompDim != NULL) free(X->decompDim);
          X->decompDim = NULL;
          if (X->decompPosn != NULL) free(X->decompPosn);
          X->decompPosn = NULL;
          if (X->dimDist != NULL) free(X->dimDist);
          X->dimDist = NULL;
          delete_DECOMP (X->decomp);
          X->decomp = NULL;

          free(X);
          X = NULL;
        }
   }

void delete_SCHED ( SCHED* X )
   {
#if 1
  /* We want to turn this functionality off since the deletion of
     any SCHED conflicts with it's being saved in the hash tables.
   */
     int i = 0;

     assert (X != NULL);
  /* printf ("Inside of delete_SCHED -- ID_Number = %d \n",X->ID_Number); */
     assert (X->ID_Number >= 1);

     assert (X->referenceCount >= 0);
     if (X->referenceCount-- == 0)
        {
          for (i = 0; i < PARTI_numprocs(); i++) 
             {
               X->sMsgSz[i]  = 0;
               X->rMsgSz[i]  = 0;
               if (X->rData[i] != NULL)
                  {
                    free(X->rData[i]);
                    X->rData[i]   = NULL;
                  }
               if (X->sData[i] != NULL)
                  {
                    free(X->sData[i]);
                    X->sData[i]   = NULL;
                  }
             }

       /* Anything but the default values */
          X->type = -10000;
          X->hash = -10000;
       /* X->ID_Number = -10000;  We want to identify which SCHED structures are a problem so comment this out */

          free(X);
          X = NULL;
        }

     X = NULL;
#endif
   }

void delete_DECOMP ( DECOMP* X )
   {
  /* printf ("In delete_DECOMP: X->referenceCount = %d \n",X->referenceCount); */
     assert (X->referenceCount >= 0);
     if (X->referenceCount-- == 0)
        {
          free(X->dimVec);
          X->dimVec = NULL;
          free(X->dimProc);
          X->dimProc = NULL;
          free(X->dimDist);
          X->dimDist = NULL;

          free(X);
          X = NULL;
        }
   }

void delete_VPROC ( VPROC* X )
   {
     free(X->dimVec);
     X->dimVec = NULL;
     free(X);
     X = NULL;
   }

void delete_Cell ( Cell* X )
   {
     assert (X->referenceCount >= 0);
     if (X->referenceCount-- == 0)
        {
          if (X->item != NULL)
             {
               free (X->item);
               X->item = NULL;
             }

          free(X);
          X = NULL;
        }
   }


