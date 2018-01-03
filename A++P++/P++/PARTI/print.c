#include "bsparti.h"
#include "port.h"
#include "print.h"

void print_schedData(schedData)
   SchedData *schedData;
{
   int             i, ndims, startPosn, *step, *nelems;

   printf("----------------------------\n");
   ndims = schedData->numDims;
   startPosn = schedData->startPosn;
   nelems = schedData->numelem;
   step = schedData->str;
   printf("Communicating %d dimensional array, starting at position %d \n",
	  ndims, startPosn);
   for (i = 0; i < ndims; i++) {
     printf("Dim. %d: %d total elements at stride %d apart \n", i,
	    nelems[i], step[i]);
   }
}

void print_sched(schedPtr)
   SCHED   *schedPtr;
{
   int             i, j, numprocs, proc;

   proc = PARTI_myproc();
   numprocs = PARTI_numprocs();

   if ( schedPtr == NULL ) {
     printf("Node %d has empty schedule \n", proc);
     return;
   }

   for (j = 0; j < numprocs; j++) {
     PARTI_gsync();
     if (j == proc) {
       for (i = 0; i < numprocs; i++) {
	 if (schedPtr->sMsgSz[i] > 0) {
	   
	   printf("NODE %d : send schedule to proc %d of %d elements \n", 
		  proc, i, schedPtr->sMsgSz[i]);
	   
	   print_schedData(schedPtr->sData[i]);  
	 }
       }
       for (i = 0; i < numprocs; i++) {
	 if (schedPtr->rMsgSz[i] > 0) {
	   
	   printf("NODE %d : receive schedule from proc %d of %d elements \n", 
		  proc, i, schedPtr->rMsgSz[i]);
	   
	   print_schedData(schedPtr->rData[i]); 
	   
	 }
       }
       printf("\n");
     }
   }
}

void print_darray(darray)
DARRAY *darray;

{
  int i;
  int mp;

  mp = PARTI_myproc();
  if (darray == NULL) {
    printf("%d : Empty darray \n", mp);
    return;
  }

  printf("%d : NumDims = %d \n", mp, darray->nDims);
  for ( i=0; i < darray->nDims; i++) {
    printf("%d : Dim=%d : Ghost Cells=%d, Global Len=%d, ",
	   mp, i, darray->ghostCells[i], darray->dimVecG[i]);
    if ( darray->decompPosn[darray->decompDim[i]] == 0)
      printf("Local Len %d \n", darray->dimVecL_L[i]);
    else if ( darray->decompPosn[darray->decompDim[i]] + 1 ==
	   (darray->decomp)->dimProc[darray->decompDim[i]])
      printf("Local Len %d \n", darray->dimVecL_R[i]);
    else if ( darray->decompPosn[darray->decompDim[i]] != -1)
      printf("Local Len %d \n", darray->dimVecL[i]);
    else
      printf("Local Len 0\n");
  }

}

void print_msgBuf(msgBuf, len)
   int            *msgBuf, len;
{
   int             i;

   printf("\nNODE %d: msgBuf = ",PARTI_myproc());
   for (i = 0; i < len; i++) {
if (i % 10 == 0) printf("\n");
      printf("%d ", msgBuf[i]);
   }
   printf("\n");
}


/*
void print_data(start, step, size)
   int             start, step, size;
{

   int             i;

   for (i = 0; i < size; i++)
      printf("%d ", start + i * step);
}
*/
