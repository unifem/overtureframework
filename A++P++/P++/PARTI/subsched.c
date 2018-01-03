/***************************************************************************
 *
 * I found the bug, and now my (simple) tests work for moving one element.
 * I'm including file subsched.c again, so try it out.  I only had to add one
 * line of code, but it makes a big difference.  Let me know if you have any
 * more problems.
 * 
 * Alan
 * 
 ***************************************************************************/

#include <stdio.h>
#include "utils.h"
#include "hash.h"
#include "bsparti.h"
#include "helper.h"
#include "port.h"

void gen_send_sched();
void gen_recv_sched();
void compute_schedule();
void init_Sched();


/***************************************************************************
 *
 * subArraySched(srcDA, destDA, numDims, srcDims, sLos, sHis, sStrides,
 *                                       destDims, dLos, dHis, dStrides)
 * 
 *  creates the schedule describing the data motion neccessary to realign a
 *  sub-portion of a multi-dimensional source array to a sub-portion of
 *  multi-dimensional destination array.
 * 
 *  only works for up to four dimensional subarrays (because of the sizes
 *  of arrays in some of the data structures)
 *
 * Inputs: srcDA -   distributed array description for the src array being
 *                   moved
 *         destDA -  distributed array description for the array to which 
 *                   srcDA is being moved
 *         numDims - the number of dimensions of array 
 *         srcDims  - dimensions of the src array to be aligned 
 *         sLos,sHis,sStrides - specify src ranges and strides
 *         destDims - dimensions of dest to which src is being aligned 
 *         dLos,dHis,dStrides - specifies dest ranges and strides
 * 
 * Returns: schedule describing necessary data motion
 * 
 ***************************************************************************/
/* strides in source and dest added - als 12/92 */
SCHED  *subArraySched(srcDA, destDA, numDims,
		      srcDims, sLos, sHis, sStrides,
		      destDims, dLos, dHis, dStrides)
  DARRAY *srcDA, *destDA;
  int    numDims;
  int   *srcDims, *sLos, *sHis, *sStrides, *destDims, *dLos, *dHis, *dStrides;
{
   int             *tmp;
   SCHED   *schedPtr = NULL;
   int             i;
   int             dim;
   int             srcDim, sLo, sHi, sStr, destDim, dLo, dHi, dStr, Lo, Hi;
   int             local_lo, local_hi, global_lo, global_hi, stride;
   int             lMin, lMax, num_elems, numSrc, numDest;
   int             tmp7, localDest = 1,
                   localSrc = 1;
   int             srcIndexVals[MAX_DIM], destIndexVals[MAX_DIM];
   struct range_rec src4LDest, dest4LSrc;
   struct exch_rec src, dest;
   int             procOffset, srcProcPosnVec[MAX_DIM], destProcPosnVec[MAX_DIM];
   char err_str[150];

   if (!active(PARTI_myproc(), srcDA->decomp)  && !active(PARTI_myproc(), destDA->decomp))
      return(schedPtr);

   /* check that the source and dest have the same number  
      of elements to be moved in each dimension */
   for (i = 0; i < numDims; i++) {
     if(srcDims[i] >= 0 && destDims[i] >= 0) {
       numSrc = abs(sHis[i] - sLos[i]) / sStrides[i] + 1;
       numDest = abs(dHis[i] - dLos[i]) / dStrides[i] + 1;
       if (numSrc != numDest) {
	 /* add back in 1 to dim numbers if called  
	    from Fortran 	 */
	 sprintf(err_str, "in subArraySched(), number of source data items,\n dimension %d=%d, does not match number in destination, dimension %d=%d",
		 srcDims[i]+FFlag, numSrc, destDims[i]+FFlag, numDest);
	 error(err_str);
       }     
     }     
     else  { 
       /* either src or dest has a fixed dimension */
       if (srcDims[i] < 0) {
	 /* set sLo, sHi, sStride to default value -1 */
	 sLos[i] = sHis[i] = sStrides[i] = -1;
	 numSrc = 0;
	 numDest = abs(dHis[i] - dLos[i]) / dStrides[i] + 1;
	 if (numDest != 1) {
	   /* add back in 1 to dim numbers if called from Fortran */
	   sprintf(err_str, "in subArraySched(), number of destination data items in fixed dimension %d = %d, not 1 \n",
		   destDims[i]+FFlag, numDest);
	   error(err_str);
	 }
       }
       else { /* destDims[i] == -1 */
	 /* set dLo, dHi, dStride to default value -1 */
	 dLos[i] = dHis[i] = dStrides[i] = -1;
	 numSrc = abs(sHis[i] - sLos[i]) / sStrides[i] + 1;
	 numDest = 0;
	 if (numSrc != 1) {
	   /* add back in 1 to dim numbers if called from Fortran */
	   sprintf(err_str, "in subArraySched(), number of source data items in fixed dimension %d = %d, not 1 \n",
		   srcDims[i]+FFlag, numSrc);
	   error(err_str);
	 }
       }
     }
   }


   /* check to see if we've already computed  
      and saved the schedule	 */
   schedPtr = lookup_sub(srcDA, destDA, numDims,
			 srcDims, sLos, sHis, sStrides,
			 destDims, dLos, dHis, dStrides);
   if (schedPtr != NULL) {
  /* printf("Node %d: reusing saved subarray_exch schedule\n", PARTI_myproc()); */
     schedPtr->referenceCount++;  /* Code added by Dan Quinlan */
     return schedPtr;
   }

   /* allocate space for and initialize sched */
   schedPtr = NEW(SCHED);
   if (schedPtr == NULL) {
     fatal_error("can`t allocate space for schedule");
   }
   init_Sched(schedPtr);
   schedPtr->type = 2;
   
   /* sendData.nProcs = recvData.nProcs = 0;   */ 
   
   src.numRanges = src.nFixedIndices   = 0; 
   dest.numRanges = dest.nFixedIndices = 0;
   /* coordinates of processor in src decomposition */
   gray(srcDA->decomp, PARTI_myproc(), srcProcPosnVec);
   /* coordinates of processor in dest decomposition */
   gray(destDA->decomp, PARTI_myproc(), destProcPosnVec);
   
   
   /* localSrc is whether the processor 
      will send data in this schedule */
   /* localDest is whether the processor 
      will receive data in this schedule */
   
   localSrc  &= active(PARTI_myproc(), srcDA->decomp);
   localDest &= active(PARTI_myproc(), destDA->decomp);
   
   for (i = 0; i < numDims; i++) {
     srcDim = srcDims[i];
     sLo = sLos[i];
     sHi = sHis[i];
     sStr = sStrides[i];
     destDim = destDims[i];
     dLo = dLos[i];
     dHi = dHis[i];
     dStr = dStrides[i];
     numSrc  = (srcDim >= 0)  ? abs(sHi - sLo) / sStr + 1 : 0;
     numDest = (destDim >=0) ? abs(dHi - dLo) / dStr + 1 : 0;


     /* first decide if any of either ranges 
	in the current dimension,  source  
	srcDim and dest destDim, is on this processor */
     
     /* in this case, there is more than 
	one element in this dimension in 
	the source and destination  the  
	position vector containing -1 means 
	that the processor is  not in the 
	decomposition */
     
     if (numSrc > 1 && numDest > 1) {
       if ((srcDA->dimDist[srcDim] != '*') &&
	   localSrc && (srcProcPosnVec[0] != -1)) {
	 
	 /* clip to the range of data  
	    stored on this processor */
	 local_lo = srcDA->g_index_low[srcDim];
	 local_hi = srcDA->g_index_hi[srcDim];
	 global_lo = sLo;
	 global_hi = sHi;
	 stride = sStr;
	 /* the local start index is computed  
	    relative to the global start  
	    index, using the stride the local 
	    end index is just clipped, 
	    relying on the stride to end at the  
	    right place */
	 if (global_lo <= global_hi) {
	   if (global_lo < local_lo) {
	     lMin = local_lo + stride - 1 -
	       (local_lo - global_lo -  
		1) % stride;
	   }
	   else {
	     lMin = global_lo;
	   }
	   lMax = fMIN(local_hi, global_hi);
	   num_elems = (lMin <= lMax) ?  
	     ((lMax - lMin)/stride + 1) : 0;
	 }
	 else {
	   if (global_lo > local_hi) {
	     lMin = local_hi - stride + 1 +
	       (global_lo - local_hi - 1) % 
		 stride;
	   }
	   else {
	     lMin = global_lo;
	   }
	   lMax = fMAX(local_lo, global_hi);
	   num_elems = (lMax <= lMin) ? ((lMin - lMax)/ 
					 stride + 1) : 0;
	 }
	 
	 localSrc &= (num_elems > 0);
       }

       tmp7 = src.numRanges;
       src.rangeDim[tmp7] = srcDim;  /* the src dimension */
       src.ranges[tmp7].min = sLo;  /* and the global index range */
       src.ranges[tmp7].max = sHi;
       src.ranges[tmp7].stride = sStr;  /* and stride */
       src.numRanges++;
       if ((destDA->dimDist[destDim] != '*') &&
	   localDest  && (destProcPosnVec[0] != -1)) {
	 
	 /* clip to the range of data stored  
	    on this processor */
	 local_lo = destDA->g_index_low[destDim];
	 local_hi = destDA->g_index_hi[destDim];
	 global_lo = dLo;
	 global_hi = dHi;
	 stride = dStr;
	 
	 /* the local start index is computed 
	    relative to the global  start index, 
	    using the stride the local end index  
	    is just clipped, relying on the  
	    stride to end at the right place */
	 
	 if (global_lo <= global_hi) {
	   if (global_lo < local_lo) {
	     lMin = local_lo + stride - 1 -
	       (local_lo-global_lo-1) 
		 %stride;
	   }
	   else {
	     lMin = global_lo;
	   }
	   lMax = fMIN(local_hi, global_hi);
	   num_elems = (lMin <= lMax) ? (lMax - lMin)/ 
	     stride + 1 : 0;
	 }
	 else {
	   if (global_lo > local_hi) {
	     lMin = local_hi - stride + 1 +
	       (global_lo - local_hi - 1) % stride;
	   }
	   else {
	     lMin = global_lo;
	   }
	   lMax = fMAX(local_lo, global_hi);
	   num_elems = (lMax <= lMin) ? 
	     ((lMin - lMax)/stride + 1) : 0;
	 }
	 
	 localDest &= (num_elems > 0);
       }
       tmp7 = dest.numRanges;
       dest.rangeDim[tmp7] = destDim;  /* dest dimension */
       dest.ranges[tmp7].min = dLo; /* and global index range */
       dest.ranges[tmp7].max = dHi;
       dest.ranges[tmp7].stride = dStr;  /* and stride */
       dest.numRanges++;
     }  

     else {
       /* there is only one element in this dimension in */
       /* the source and/or the destination - a fixed index */
       if (numSrc == 1) {
	 /* the src has a fixed index */
	 if ((srcDA->dimDist[srcDim] != '*') &&
	     localSrc && (srcProcPosnVec[0] != -1)) {
	   local_lo = srcDA->g_index_low[srcDim];
	   local_hi = srcDA->g_index_hi[srcDim];
	   localSrc &= ((sLo >= local_lo) && (sLo <= local_hi));
	 }
	 tmp7 = src.nFixedIndices;
	 src.indexDim[tmp7] = srcDim;  /* src dimension */
	 src.indexVals[tmp7] = sLo; /* and global index */
	 src.nFixedIndices++;
       }
       if (numDest == 1) {
	 /* the dest has a fixed index */
	 if ((destDA->dimDist[destDim] != '*') &&
	     localDest && (destProcPosnVec[0] != -1)) {
	   local_lo = destDA->g_index_low[destDim];
	   local_hi = destDA->g_index_hi[destDim];
	   localDest &= ((dLo >= local_lo) && (dLo <= local_hi));
	 }
	 tmp7 = dest.nFixedIndices;
	 dest.indexDim[tmp7] = destDim;  /* dest dimension */
	 dest.indexVals[tmp7] = dLo; /* and global index */
	 dest.nFixedIndices++;
       }
       
     }  
   }


   /* if I (i.e p) needs to either send or receive then 
      compute the index * values for both the src and 
      dest.  The index value for a dimension  corres. 
      to a range is set equal to the first element 
      of the range  residing on me.  These values 
      are stored in "local" index terms since they are 
      used to compute startPosn and step 
      for the local dest/src */
   
   /* srcIndexVals stores the 
      coordinates, in local terms, about 
      where the sent data will come from  */
   
   /* src4LDest stores info about  
      the source processor(s) that 
      will  send data to this processor,  
      in global coordinates */
   /* dest4LSrc stores info about the destination  
      processor(s) that this processor will send to, 
      in global coordinates */
   /* destIndexVals stores the coordinates, in local terms, about */
   /* where the received data will be stored */
   

   if (localDest || localSrc) {
     for (i = 0; i < src.nFixedIndices; i++) {
       dim = src.indexDim[i];
       src4LDest.indexVals[dim] = src.indexVals[i];
       srcIndexVals[dim] = src.indexVals[i];
       if (srcDA->dimDist[dim] != '*') {
	 srcIndexVals[dim] -= srcDA->g_index_low[dim] ;
       }
     }
     for (i = 0; i < dest.nFixedIndices; i++) {
       dim = dest.indexDim[i];
       dest4LSrc.indexVals[dim] = dest.indexVals[i];
       destIndexVals[dim] = dest.indexVals[i];
       if (destDA->dimDist[dim] != '*') {
	 procOffset = destProcPosnVec 
	   [getDecompDim(destDA, dim)];
	 destIndexVals[dim] -= destDA->g_index_low[dim];
       }
     }
     
     /* why not both src and dest ranges, and 
	why count through the src  ranges 
	and then use the dest ranges info?  */
     
     for (i = 0; i < dest.numRanges; i++) { 
       dim = dest.rangeDim[i];
       if (destDA->dimDist[dim] != '*') {
	 procOffset = destProcPosnVec 
	   [getDecompDim(destDA, dim)];
	 destIndexVals[dim] = fMAX(destDA->g_index_low[dim],
				   dest.ranges[i].min);
	 destIndexVals[dim] -= destDA->g_index_low[dim];
       } else
	 destIndexVals[dim] = dest.ranges[i].min;
     }
     
     /*
      *  if part of src resides on my processor then compute the 
      *  corresponding part of dest and determine what I need to 
      *  send and to whom 
      */
     if (localSrc) {
       gen_send_sched(schedPtr, src.numRanges, src, dest, srcDA, destDA, srcIndexVals, &dest4LSrc, srcProcPosnVec);
     }
     
     /* 
      *  if part of dest resides on my processor then compute the 
      *  corresponding part of src and determine what I need to 
      *  receive and from whom 
      */
     if (localDest) {
       gen_recv_sched(schedPtr, dest.numRanges, src, dest, srcDA, destDA, destIndexVals, &src4LDest, destProcPosnVec);
     }

   }

   /* save the generated schedule in the hash table */
   /*
     printf("Node %d: saving subarray_exch schedule\n", PARTI_myproc());
     */
/* printf ("BEFORE insert_sub_table schedPtr->referenceCount = %d \n",schedPtr->referenceCount); */
   insert_sub_table(schedPtr, srcDA, destDA, numDims,
		    srcDims, sLos, sHis, sStrides,
		    destDims, dLos, dHis, dStrides);
/* printf ("AFTER insert_sub_table schedPtr->referenceCount = %d \n",schedPtr->referenceCount); */
     
   return (schedPtr);

}



void gen_send_sched(sched, numRanges, src, dest, srcDA, destDA, srcIndexVals, dest4LSrc, srcProcPosnVec)
   int             numRanges;
   SCHED  *sched;
   struct exch_rec src, dest;
   DARRAY *srcDA, *destDA;
   struct range_rec *dest4LSrc;
   int            *srcIndexVals, *srcProcPosnVec;
{
   int             sDim, dDim, lSrcMin, lSrcMax, procOffset;
   int             i, proc, srcVal, step, msgNum;
   int             local_lo, local_hi, global_lo, global_hi;
   int             stride, d_stride, num_elems[MAX_DIM], dist_from_start;
   struct dest     lSrcInfo;
   int do_send  ; 

   assert (numRanges >= 0);
   for(i = 0; i < numRanges; i++)  {
     sDim = src.rangeDim[i];
     dDim = dest.rangeDim[i];
     dest4LSrc->rangeDims[i] = dDim;
     /* if the dimension is distributed, 
	to transform to local coordinates */
     if (srcDA->dimDist[sDim] == '*')
       procOffset = 0;
     else
       procOffset = srcProcPosnVec[getDecompDim(srcDA, sDim)];
     
     /* clip to the range of data 
	stored on this processor */
     global_lo = src.ranges[i].min;
     global_hi = src.ranges[i].max;
     local_lo = srcDA->g_index_low[sDim];
     local_hi = srcDA->g_index_hi[sDim];
     stride = src.ranges[i].stride;
     d_stride = dest.ranges[i].stride;
     if (dest.ranges[i].min > dest.ranges[i].max)
       d_stride = -d_stride;
     /* the destination counts backwards */
     
     
     if (global_lo <= global_hi) {
       if (global_lo < local_lo) {
	 lSrcMin = local_lo + stride - 1 -
	   (local_lo - global_lo - 1) % stride;
       }
       else {
	 lSrcMin = global_lo;
       }
       lSrcMax = fMIN(local_hi, global_hi);
       num_elems[i] = (lSrcMax - lSrcMin)/stride + 1;
     }
     else {
       if (global_lo > local_hi) {
	 lSrcMin = local_hi - stride + 1 +
	   (global_lo - local_hi - 1) % stride;
       }
       else {
	 lSrcMin = global_lo;
       }
       lSrcMax = fMAX(local_lo, global_hi);
       num_elems[i] = (lSrcMin - lSrcMax)/stride + 1;
     }
     
     /* fill in the final index value for 
	the line of data to be sent */
     srcIndexVals[sDim] = lSrcMin - srcDA->g_index_low[sDim];
     
     /* fill in the structure that describes 
	the destination line (in the 
	processor(s) to which this processor  
	will send */
     dist_from_start = abs(lSrcMin - global_lo)/stride;
     dest4LSrc->min[i]  = dest.ranges[i].min + dist_from_start * d_stride;
     dest4LSrc->max[i]  = dest4LSrc->min[i] + (num_elems[i] - 1) * d_stride;
     dest4LSrc->step[i] = d_stride;
     
     if (lSrcMin <= lSrcMax)
       lSrcInfo.step[i] = compute_step(srcDA, stride, sDim);
     else
       lSrcInfo.step[i] = compute_step(srcDA, -stride, sDim);
     
   }

/* Code added by Dan Quinlan */
/* Avoid Uninitialized Memory Reads in PURIFY! */
   if (numRanges == 0)
      {
        sDim         = 0;
     /* srcDA        = NULL; */
     /* srcIndexVals = NULL; */
      }

   lSrcInfo.startPosn = compute_startPosn(srcDA, sDim, srcIndexVals);
   do_send = 1 ; 
   for( i = 0; i < numRanges; i++) 
     if(num_elems[i] < 1)  do_send = 0 ; 
   if (do_send > 0) {
     compute_schedule(sched, destDA, dest4LSrc, lSrcInfo,numRanges,1);
   }
	
}


/***************************************************************************
 * 
 * a recursive routine that systemically goes through all ranges computes a
 * mini-schedule for each vector of data and builds a large schedule 
 *
 **************************************************************************/
void gen_recv_sched(sched, numRanges, src, dest, srcDA, destDA, destIndexVals, src4LDest, destProcPosnVec)
   int             numRanges;
   SCHED  *sched;
   struct exch_rec src, dest;
   DARRAY *srcDA, *destDA;
   struct range_rec *src4LDest;
   int            *destIndexVals, *destProcPosnVec;
{
   int              sDim, dDim, lDestMin, lDestMax, procOffset;
   int              i, proc, destVal, step, msgNum;
   int             local_lo, local_hi, global_lo, global_hi;
   int              stride, s_stride, num_elems[MAX_DIM], dist_from_start;
   int             do_recv ; 
   struct dest      lDestInfo;

   for(i = 0; i < numRanges; i++) {
     sDim = src.rangeDim[i];
     dDim = dest.rangeDim[i];
     
     src4LDest->rangeDims[i] = sDim;
     if (destDA->dimDist[dDim] == '*')
       procOffset = 0;
     else
       procOffset = destProcPosnVec[getDecompDim(destDA, dDim)];
     
     global_lo = dest.ranges[i].min;
     global_hi = dest.ranges[i].max;
     local_lo = destDA->g_index_low[dDim];
     local_hi = destDA->g_index_hi[dDim];
     stride = dest.ranges[i].stride;
     s_stride = src.ranges[i].stride;
     if (src.ranges[i].min > src.ranges[i].max)
       /* the source counts backwards */
       s_stride = -s_stride;
     /* the local start index is computed 
	relative to the global start  index, 
	using the stride to find the right start */
     /* the local end index is just clipped, 
	relying on the stride to end 
	at the right place */

     if (global_lo <= global_hi) {
       if (global_lo < local_lo) {
	 lDestMin = local_lo + stride - 1 -
	   (local_lo - global_lo - 1) % stride;
       }
       else {
	 lDestMin = global_lo;
       }
       lDestMax = fMIN(local_hi, global_hi);
       num_elems[i] = (lDestMax - lDestMin)/stride + 1;
     }
     else {
       if (global_lo > local_hi) {
	 lDestMin = local_hi - stride + 1 +
	   (global_lo - local_hi - 1) % stride;
       }
       else {
	 lDestMin = global_lo;
       }
       lDestMax = fMAX(local_lo, global_hi);
       num_elems[i] = (lDestMin - lDestMax)/stride + 1;
     }
     
     destIndexVals[dDim] = lDestMin - destDA->g_index_low[dDim];
     dist_from_start = abs(lDestMin - global_lo)/stride;
     src4LDest->min[i] = src.ranges[i].min + dist_from_start * s_stride;
     src4LDest->max[i] = src4LDest->min[i] + (num_elems[i] - 1) * s_stride;
     src4LDest->step[i] = s_stride;
     
     if (lDestMin <= lDestMax)
       lDestInfo.step[i] = compute_step(destDA, stride, dDim);
     else
       lDestInfo.step[i] = compute_step(destDA, -stride, dDim);
     
   } 

/* Code added by Dan Quinlan */
/* Avoid Uninitialized Memory Reads in PURIFY! */
   if (numRanges == 0)
      {
        dDim         = 0;
     /* srcDA        = NULL; */
     /* srcIndexVals = NULL; */
      }

   lDestInfo.startPosn = compute_startPosn(destDA, dDim, destIndexVals);
   do_recv = 1 ; 
   for(i= 0 ; i < numRanges; i++)
     if(num_elems[i] < 1) do_recv = 0 ;  
   if (do_recv > 0) {
     compute_schedule(sched, srcDA,src4LDest,lDestInfo,numRanges,2);
   }

}

#if 0

/*****************************************************************************
 * 
 * compute_schedule (dArray, range, dest,numRanges)
 * 
 *  creates the schedule describing the data motion neccessary to either gather
 *  or scatter a 1-dimensional range of indices for a multi-dimensional array
 *  whose distribution and physical charachteristics are described by dArray.
 * 
 * Inputs: 
 *  dArray -  distributed array description for the array being
 *            gathered/scattered 
 *  range   - specifies the range-dimension, the range bounds, and the 
 *            index values for all other dimension of the array being 
 *            gathered/scattered 
 *  dest    - specifies the on-processor part of the communication
 * 
 *  numRanges - No. of dimensions having more than one element 
 * Returns: schedule describing necessary data motion
 * 
 ****************************************************************************/
void  ORIG_compute_schedule(sched , dArray, range, dest,numRanges, type)
   SCHED *sched ;  
   DARRAY *dArray;
   struct range_rec  *range;
   struct dest       dest;
   int   numRanges       ; 
   int type ;  /* 1 is send-schedule, 2 is receive schedule  */ 
{
   int             i, j, j1, j2, j3, j4 ;
   int             recvProc, sendProc;
   int             nCommProcs[MAX_DIM], procOffsets[MAX_DIM];
   int             startPosn;
   int             startPosn1, startPosn2, startPosn3, startPosn4 ; 
   int             max[MAX_DIM] , min[MAX_DIM], step[MAX_DIM];
   int             lenR[MAX_DIM], lenL[MAX_DIM];
   int             size[MAX_DIM][MAX_NODES] ;  
   int             minDB[MAX_DIM], minMB[MAX_DIM]; 
   int             maxDB[MAX_DIM], maxMB[MAX_DIM];
/*   SchedData schedData[32]; */
   int             baseProcVec[MAX_DIM], sPosnVec[MAX_DIM], rPosnVec[MAX_DIM];
   int             rPosnVec1[MAX_DIM], rPosnVec2[MAX_DIM] ; 
   int             rPosnVec3[MAX_DIM] ;  

   int            *indexVals,  numProcs = 1;
   int            rangeDims[MAX_DIM] ; 
   int             blockSz[MAX_DIM];
   DECOMP         *vMach;
   int            blockSz_L[MAX_DIM], blockSz_R[MAX_DIM];
   int            diff[MAX_DIM], last_proc[MAX_DIM];
   int            min_blk_size[MAX_DIM], max_blk_size[MAX_DIM];

   int            local_lo, local_hi, global_lo, global_hi, stride;
   int            rMin, rMax;
   SchedData      *scheddata ;  
   int            procno ; 

   /*
    *  some initialization stuff 
    */
   vMach = dArray->decomp;
   gray(vMach, vMach->baseProc, baseProcVec);

   /*
    * compute communication pattern for current processor 
    */
   for(i = 0 ; i < numRanges ; i++)  {
     indexVals = range->indexVals;
     rangeDims[i] = range->rangeDims[i];
     blockSz[i] = dArray->dimVecL[rangeDims[i]];
     blockSz_L[i] = dArray->dimVecL_L[rangeDims[i]];
     blockSz_R[i] = dArray->dimVecL_R[rangeDims[i]];
     last_proc[i] = dArray->decomp->dimProc[dArray->decompDim[rangeDims[i]]] -1;
     diff[i] = blockSz_L[i] - blockSz[i];
     min[i]  = range->min[i];
     max[i]  = range->max[i];
     step[i] = range->step[i];
     indexVals[rangeDims[i]] = 0; /*min;*/
   

     /* 
       if the dimension for which the range is specified is  
       distributed across
       the processors then compute processors (in terms of offsets  
       from  myself) with whom I must communicate and message  
       sizes for the end
       processors (ie left & right). 
       */

     minDB[i] = (min[i] - diff[i]) / blockSz[i];
     /* if the min index is on the leftmost processor */
     if ( minDB[i] < 1){
       minDB[i] = 0;
       minMB[i] = min[i];
       min_blk_size[i] = blockSz_L[i];
     }
     /* if the min index is on the rightmost processor */
     else if ( minDB[i] >= last_proc[i]){
       minDB[i] = last_proc[i];
       minMB[i] = min[i] - (last_proc[i])*blockSz[i]-diff[i];
       min_blk_size[i] = blockSz_R[i];
     }
     /* if min is on any other processor */
     else{
       minMB[i] = min[i] - (minDB[i])*blockSz[i]-diff[i];
       min_blk_size[i] = blockSz[i];
     }

     /* same as min */
     maxDB[i] = (max[i] -diff[i]) / blockSz[i];
     if ( maxDB[i] < 1){
       maxDB[i] = 0;
       maxMB[i] = max[i];
       max_blk_size[i] = blockSz_L[i];
     }
     else if ( maxDB[i] >= last_proc[i]){
       maxDB[i] = last_proc[i];
       maxMB[i] = max[i] - (last_proc[i])*blockSz[i]-diff[i];
       max_blk_size[i] = blockSz_R[i];
     }
     else {
       maxMB[i] = max[i] - (maxDB[i])*blockSz[i]-diff[i];
       max_blk_size[i] = blockSz[i];
     }

     if (dArray->dimDist[rangeDims[i]] == '*') {
       nCommProcs[i] = 1;
       lenL[i] = lenR[i] = abs(max[i] - min[i]) + 1;
     } 
     else {
       nCommProcs[i] = abs(maxDB[i] - minDB[i]) + 1;
       if (maxDB[i] == minDB[i]) {
	 lenL[i] = abs(max[i] - min[i]) + 1;
       }  
       else {
	 if (maxDB[i] > minDB[i]) {
	   lenL[i] = min_blk_size[i] - minMB[i];
	   lenR[i] = maxMB[i] + 1;
	 }
	 else {
	   lenL[i] = minMB[i] + 1;
	   lenR[i] = max_blk_size[i] - maxMB[i];
	 }
       }
     }
   }

   for ( i = 0; i < numRanges ; i++)  {
     for (j = minDB[i]; (step[i] > 0 && j <= maxDB[i]) || 
	  (step[i] < 0 && j >= maxDB[i]); j += SIGN(step[i])) {

       global_lo    = min[i];  /* parameters of range */
       global_hi = max[i];
       stride    = step[i];
       
       if (stride >= 0) {
	 if (j == minDB[i]) 
	   local_lo   = global_lo;
	 else  
	   local_lo   = global_lo + lenL[i] + 
	     (j - minDB[i] - 1) * blockSz[i];
	 
	 if (j == minDB[i]) 
	   local_hi = local_lo + lenL[i] - 1;
	 else if (j == maxDB[i]) 
	   local_hi = local_lo + lenR[i] - 1;
	 else 
	   local_hi = local_lo + blockSz[i] - 1;
       }
       else {
	 if (j == minDB[i]) 
	   local_hi = global_lo;
	 else		  
	   local_hi = global_lo - lenL[i] - 
	     (minDB[i] - j - 1) * blockSz[i];
	 
	 if (j == minDB[i]) 
	   local_lo = local_hi - lenL[i] + 1;
	 else if (j == maxDB[i]) 
	   local_lo = local_hi - lenR[i] + 1;
	 else 
	   local_lo = local_hi - blockSz[i] + 1;
       }

       /* give the local and global start and 
	  end indices and the stride, */
       /* find the number of elements for remote processor j */
       /* the local start index is computed  
	  relative to the global start */
       /* index, using the stride to find the right start */
       /* the local end index is just clipped,  
	  relying on the stride to end */
       /* at the right place */
       

       if (stride >= 0) {
	 if (global_lo < local_lo) {
	   rMin = local_lo + stride - 1 -
	     (local_lo - global_lo - 1) % stride;
	 }
	 else {
	   rMin = global_lo;
	 }
	 
	 rMax = fMIN(local_hi, global_hi);
	 size[i][j] = (rMax >= rMin) ? ((rMax - rMin)/stride + 1) : 0;
       }
       else {
	 if (global_lo > local_hi) {
	   rMin = local_hi + stride + 1 +
	     (global_lo - local_hi - 1) % -stride;
	 }
	 else {
	   rMin = global_lo;
	 }
	 rMax = fMAX(local_lo, global_hi);
	 size[i][j]  = (rMax <= rMin) ? ((rMin - rMax)/-stride + 1) : 0;
       }
       
       
     }
   }
   indexVals = range->indexVals;
   compute_proc_offsets(dArray, indexVals, procOffsets);
   startPosn = dest.startPosn;


   if(numRanges == 0) {
     /* this corresponds to only having a single data item in the schedule, */
     /* so set numDims = 1, and the number of elements to 1, so the */
     /* datamoves will work correctly */

     add_posVec(dArray, baseProcVec, procOffsets, 
		rPosnVec, vMach->nDims, 0, 0);
     procno = invGray(vMach,rPosnVec);
     
     scheddata = NEW(SchedData) ; 
     scheddata->proc = procno ; 
     scheddata->numDims = 1; 
     scheddata->startPosn = startPosn ; 
     scheddata->numelem[0] = 1;
     scheddata->str[0]     = 1; 
     
     if(type == 1)  { /* send schedule */ 
       sched->sData[procno] = scheddata;
       sched->sMsgSz[procno] = 1; 
     }
     else if(type ==2) {  
       sched->rData[procno] = scheddata;
       sched->rMsgSz[procno] = 1; 
     }
   }
   else if(numRanges ==1)  {
     
     for (j1 = minDB[0]; (step[0] > 0 && j1 <= maxDB[0]) || 
	  (step[0] < 0 && j1 >= maxDB[0]); j1 += SIGN(step[0])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[0], j1);
       
       procno = invGray(vMach,rPosnVec) ; 
       
       if(size[0][j1] > 0 )  {
	 scheddata = NEW(SchedData) ; 
	 scheddata->proc = procno ; 
	 scheddata->numDims = numRanges ; 
	 scheddata->startPosn = startPosn ; 
	 scheddata->numelem[0] = size[0][j1] ; 
	 scheddata->str[0]     = dest.step[0] ; 
       }  /* End of conditional on size */ 
       else 
	 scheddata = NULL ; 
       
       if(type == 1)  { /* send schedule */ 
	 sched->sData[procno] = scheddata ; 
	 sched->sMsgSz[procno] = size[0][j1] ; 
       }
       else if(type ==2) {  
	 sched->rData[procno] = scheddata ; 
	 sched->rMsgSz[procno] = size[0][j1] ; 
       }
       
       
       
       startPosn += size[0][j1] * dest.step[0];
     }
     
   }
   else if(numRanges ==2)  {
     
     for (j2 = minDB[1]; (step[1] > 0 && j2 <= maxDB[1]) || 
	  (step[1] < 0 && j2 >= maxDB[1]); j2 += SIGN(step[1])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[1], j2);
       startPosn1 = startPosn ; 
       for (j1 = minDB[0]; (step[0] > 0 && j1 <= maxDB[0]) || 
	    (step[0] < 0 && j1 >= maxDB[0]); j1 += SIGN(step[0])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1, vMach->nDims, rangeDims[0], j1);
	 
	 procno = invGray(vMach,rPosnVec1) ; 
	 
	 
	 
	 if ( (size[0][j1]>0) && (size[1][j2] > 0) ) { 
	   scheddata = NEW(SchedData) ; 
	   scheddata->proc = procno ; 
	   scheddata->numDims = numRanges ; 
	   scheddata->startPosn = startPosn1 ; 
	   scheddata->numelem[0] = size[0][j1] ; 
	   scheddata->numelem[1] = size[1][j2] ; 
	   scheddata->str[0]     = dest.step[0] ; 
	   scheddata->str[1]     = dest.step[1] ; 
	 }
	 else scheddata = NULL ; 
	 
	 
	 if(type == 1)  { /* send schedule */ 
	   sched->sData[procno] = scheddata ; 
	   sched->sMsgSz[procno] = size[0][j1] *
	     size[1][j2]  ; 
	 }
	 else if(type ==2) {  
	   sched->rData[procno] = scheddata ; 
	   sched->rMsgSz[procno] = size[0][j1] *
	     size[1][j2]  ; 
	 }
	 
	 
	 startPosn1 += size[0][j1]* dest.step[0];
       }
       
       startPosn += size[1][j2] * dest.step[1];
     }
   }
   else if(numRanges ==3)  {
     
     for (j3 = minDB[2]; (step[2] > 0 && j3 <= maxDB[2]) || 
	  (step[2] < 0 && j3 >= maxDB[2]); j3 += SIGN(step[2])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[2], j3); 
       startPosn1 = startPosn ; 
       for (j2 = minDB[1]; (step[1] > 0 && j2 <= maxDB[1]) || 
	    (step[1] < 0 && j2 >= maxDB[1]); j2 += SIGN(step[1])) {
	 add_posVec1(dArray, rPosnVec,rPosnVec1, vMach->nDims, rangeDims[1], j2);
	 startPosn2 = startPosn1 ; 
	 for (j1 = minDB[0]; (step[0] > 0 && j1 <= maxDB[0]) || 
	      (step[0] < 0 && j1 >= maxDB[0]); j1 += SIGN(step[0])) {
	   add_posVec1(dArray, rPosnVec1, rPosnVec2, vMach->nDims, rangeDims[0], j1);
	   
	   procno = invGray(vMach,rPosnVec2) ; 
	   
	   if ( (size[0][j1] > 0) && (size[1][j2] > 0) && (size[2][j3] > 0) ) { 
	     scheddata = NEW(SchedData) ; 
	     scheddata->proc = procno ; 
	     scheddata->numDims = numRanges ; 
	     scheddata->startPosn = startPosn2 ; 
	     scheddata->numelem[0] = size[0][j1] ; 
	     scheddata->numelem[1] = size[1][j2] ; 
	     scheddata->numelem[2] = size[2][j3] ; 
	     scheddata->str[0]     = dest.step[0] ; 
	     scheddata->str[1]     = dest.step[1] ; 
	     scheddata->str[2]     = dest.step[2] ; 
	   } 
	   else scheddata = NULL ; 
	   
	   
	   if(type == 1)  { /* send schedule */ 
	     sched->sData[procno] = scheddata ; 
	     sched->sMsgSz[procno] = size[0][j1] *
	       size[1][j2]*
		 size[2][j3]  ; 
	   }
	   else if(type ==2) {  
	     sched->rData[procno] = scheddata ; 
	     sched->rMsgSz[procno] = size[0][j1] *
	       size[1][j2] *
		 size[2][j3] ; 
	   }
	   
	   startPosn2 += size[0][j1] * dest.step[0];
	 }
	 
	 startPosn1 += size[1][j2] * dest.step[1];
       }
       startPosn += size[2][j3] * dest.step[2];
     }
   }
   else if(numRanges ==4)  {
     
     for (j4 = minDB[3]; (step[3] > 0 && j4 <= maxDB[3]) || 
	  (step[3] < 0 && j4 >= maxDB[3]); j4 += SIGN(step[3])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[3], j4); 
       startPosn1 = startPosn ; 
       for (j3 = minDB[2]; (step[2] > 0 && j3 <= maxDB[2]) || 
	    (step[2] < 0 && j3 >= maxDB[2]); j3 += SIGN(step[2])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1,  vMach->nDims, rangeDims[2], j3); 
	 startPosn2 = startPosn1 ; 
	 for (j2 = minDB[1]; (step[1] > 0 && j2 <= maxDB[1]) || 
	      (step[1] < 0 && j2 >= maxDB[1]); j2 += SIGN(step[1])) {
	   add_posVec1(dArray, rPosnVec1,rPosnVec2, vMach->nDims, rangeDims[1], j2);
	   startPosn3 = startPosn2 ; 
	   for (j1 = minDB[0]; (step[0] > 0 && j1 <= maxDB[0]) || 
		(step[0] < 0 && j1 >= maxDB[0]); j1 += SIGN(step[0])) {
	     add_posVec1(dArray, rPosnVec2, rPosnVec3, vMach->nDims, rangeDims[0], j1);
	     
	     procno = invGray(vMach,rPosnVec3) ; 
	     
	     
	     if ( (size[0][j1] > 0) && (size[1][j2] > 0) && (size[2][j3] > 0) && (size[3][j4] > 0) ) { 
	       scheddata = NEW(SchedData) ; 
	       scheddata->proc = procno ; 
	       scheddata->numDims = numRanges ; 
	       scheddata->startPosn = startPosn3 ; 
	       scheddata->numelem[0] = size[0][j1] ; 
	       scheddata->numelem[1] = size[1][j2] ; 
	       scheddata->numelem[2] = size[2][j3] ; 
	       scheddata->numelem[3] = size[3][j4] ; 
	       scheddata->str[0]     = dest.step[0] ; 
	       scheddata->str[1]     = dest.step[1] ; 
	       scheddata->str[2]     = dest.step[2] ; 
	       scheddata->str[3]     = dest.step[3] ; 
	     }
	     else scheddata = NULL ;  
	     
	     if(type == 1)  { /* send schedule */ 
	       sched->sData[procno] = scheddata ; 
	       sched->sMsgSz[procno] = size[0][j1]*
		 size[1][j2]*
		   size[2][j3]*
		     size[3][j4] ; 
	     }
	     else if(type ==2) {  
	       sched->rData[procno] = scheddata ; 
	       sched->rMsgSz[procno] = size[0][j1] *
		 size[1][j2] *
		   size[2][j3] *  
		     size[3][j4] ; 
	     }
	     
	     
	     startPosn3 += size[0][j1] * dest.step[0];
	   }
	   
	   startPosn2 += size[1][j2] * dest.step[1];
	 }
	 startPosn1 += size[2][j3] * dest.step[2];
       }
       startPosn += size[3][j4] * dest.step[3];
     }
   }
   else {
     fatal_error("Subarrray_sched with more than 4D arrays not implemented");
   }

}

#endif


/*****************************************************************************
 * 
 * compute_schedule (dArray, range, dest,numRanges)
 * 
 *  creates the schedule describing the data motion neccessary to either gather
 *  or scatter a 1-dimensional range of indices for a multi-dimensional array
 *  whose distribution and physical charachteristics are described by dArray.
 * 
 * Inputs: 
 *  dArray -  distributed array description for the array being
 *            gathered/scattered 
 *  range   - specifies the range-dimension, the range bounds, and the 
 *            index values for all other dimension of the array being 
 *            gathered/scattered 
 *  dest    - specifies the on-processor part of the communication
 * 
 *  numRanges - No. of dimensions having more than one element 
 * Returns: schedule describing necessary data motion
 * 
 ****************************************************************************/
void  compute_schedule(sched , dArray, range, dest,numRanges, type)
   SCHED *sched ;  
   DARRAY *dArray;
   struct range_rec  *range;
   struct dest       dest;
   int   numRanges       ; 
   int type ;  /* 1 is send-schedule, 2 is receive schedule  */ 
   {
     int             i, j;
     int             recvProc, sendProc;
     int             nCommProcs[MAX_DIM], procOffsets[MAX_DIM];
     int             startPosn;
     int             startPosns[MAX_DIM];	/* Generalized by BTNG. */
     int             max[MAX_DIM] , min[MAX_DIM], step[MAX_DIM];
     int             lenR[MAX_DIM], lenL[MAX_DIM];
     int             size[MAX_DIM][MAX_NODES] ;  
     int             DB[MAX_DIM];	/* Added by BTNG to generalize. */
     int             minDB[MAX_DIM], minMB[MAX_DIM]; 
     int             maxDB[MAX_DIM], maxMB[MAX_DIM];
  /* SchedData schedData[32]; */
     int             baseProcVec[MAX_DIM], sPosnVec[MAX_DIM], rPosnVec[MAX_DIM];
     int             rPosnVec1[MAX_DIM], rPosnVec2[MAX_DIM] ; 
     int             rPosnVec3[MAX_DIM] ;  
     int             rPosnVec4[MAX_DIM] ;	/* Added by BTNG for 6D. */
     int             rPosnVec5[MAX_DIM] ;	/* Added by BTNG for 6D. */

     int            *indexVals,  numProcs = 1;
     int            rangeDims[MAX_DIM] ; 
     int             blockSz[MAX_DIM];
     DECOMP         *vMach;
     int            blockSz_L[MAX_DIM], blockSz_R[MAX_DIM];
     int            diff[MAX_DIM], last_proc[MAX_DIM];
     int            min_blk_size[MAX_DIM], max_blk_size[MAX_DIM];

     int            local_lo, local_hi, global_lo, global_hi, stride;
     int            rMin, rMax;
     SchedData      *scheddata ;  
     int            procno ; 

  /*
   * some initialization stuff 
   */
     vMach = dArray->decomp;
     gray(vMach, vMach->baseProc, baseProcVec);

  /*
   * compute communication pattern for current processor 
   */

     j = 0;  /* initialize j to avoid purify UMR */

     for(i = 0 ; i < numRanges ; i++)
        {
          indexVals = range->indexVals;
          rangeDims[i] = range->rangeDims[i];
          blockSz[i] = dArray->dimVecL[rangeDims[i]];
          blockSz_L[i] = dArray->dimVecL_L[rangeDims[i]];
          blockSz_R[i] = dArray->dimVecL_R[rangeDims[i]];
          last_proc[i] = dArray->decomp->dimProc[dArray->decompDim[rangeDims[i]]] -1;
          diff[i] = blockSz_L[i] - blockSz[i];
          min[i]  = range->min[i];
          max[i]  = range->max[i];
          step[i] = range->step[i];
          indexVals[rangeDims[i]] = 0; /*min;*/

       /*
          if the dimension for which the range is specified is distributed across
          the processors then compute processors (in terms of offsets from  myself)
          with whom I must communicate and message sizes for the end processors (ie left & right). 
        */

          minDB[i] = (min[i] - diff[i]) / blockSz[i];
       /* if the min index is on the leftmost processor */
          if ( minDB[i] < 1)
             {
               minDB[i] = 0;
               minMB[i] = min[i];
               min_blk_size[i] = blockSz_L[i];
             }
       /* if the min index is on the rightmost processor */
            else
             {
               if ( minDB[i] >= last_proc[i])
                  {
                    minDB[i] = last_proc[i];
                    minMB[i] = min[i] - (last_proc[i])*blockSz[i]-diff[i];
                    min_blk_size[i] = blockSz_R[i];
                  }
            /* if min is on any other processor */
                 else
                  {
                    minMB[i] = min[i] - (minDB[i])*blockSz[i]-diff[i];
                    min_blk_size[i] = blockSz[i];
                  }
             }

       /* same as min */
          maxDB[i] = (max[i] -diff[i]) / blockSz[i];
          if ( maxDB[i] < 1)
             {
               maxDB[i] = 0;
               maxMB[i] = max[i];
               max_blk_size[i] = blockSz_L[i];
             }
            else
             {
               if ( maxDB[i] >= last_proc[i])
                  {
                    maxDB[i] = last_proc[i];
                    maxMB[i] = max[i] - (last_proc[i])*blockSz[i]-diff[i];
                    max_blk_size[i] = blockSz_R[i];
                  }
                 else
                  {
                    maxMB[i] = max[i] - (maxDB[i])*blockSz[i]-diff[i];
                    max_blk_size[i] = blockSz[i];
                  }
             }

          if (dArray->dimDist[rangeDims[i]] == '*')
             {
               nCommProcs[i] = 1;
               lenL[i] = lenR[i] = abs(max[i] - min[i]) + 1;
             }
            else
             {
               nCommProcs[i] = abs(maxDB[i] - minDB[i]) + 1;
               if (maxDB[i] == minDB[i])
                  {
                    lenL[i] = abs(max[i] - min[i]) + 1;
                  }
                 else
                  {
                    if (maxDB[i] > minDB[i])
                       {
                         lenL[i] = min_blk_size[i] - minMB[i];
                         lenR[i] = maxMB[i] + 1;
                       }
                      else
                       {
                         lenL[i] = minMB[i] + 1;
                         lenR[i] = max_blk_size[i] - maxMB[i];
                       }
                  }
             }
#if 0
       /* Print out comment until this problem is fixed */
	  printf ("numRanges = %d \n",numRanges);
	  printf ("step[i] = %d \n",step[i]);
	  printf ("i = %d \n",i);
	  printf ("j = %d \n",j);
	  printf ("maxDB[i] = %d \n",maxDB[i]);
	  
          printf ("Expensive initialization of %d x %d array (initialize only %d x %d) \n",MAX_DIM,MAX_NODES,
               numRanges,(step[i] > 0 && j <= maxDB[i]) || (step[i] < 0 && j >= maxDB[i]));
          printf ("Lower bound on j is: minDB[%d] = %d  Upper bound on j is: step[%d] = %d  maxDB[%d] = %d \n",
               i,minDB[i],i,step[i],i,maxDB[i]);
#endif
        }


  /* Initialize just the part of the array that we use (to avoid purify UMRs) */
     for ( i = 0; i < numRanges; i++)
        {
          for (j = 0; j < maxDB[i]; j++)
             {
               size[i][j] = 0;
             }
        }

     for ( i = 0; i < numRanges ; i++)
        {
          for (j = minDB[i]; (step[i] > 0 && j <= maxDB[i]) || (step[i] < 0 && j >= maxDB[i]); j += SIGN(step[i]))
             {
               global_lo = min[i];  /* parameters of range */
               global_hi = max[i];
               stride    = step[i];

               if (stride >= 0)
                  {
                    if (j == minDB[i]) 
                         local_lo   = global_lo;
                      else
                         local_lo   = global_lo + lenL[i] + (j - minDB[i] - 1) * blockSz[i];

                    if (j == minDB[i]) 
                         local_hi = local_lo + lenL[i] - 1;
                      else
                       {
                         if (j == maxDB[i])
                              local_hi = local_lo + lenR[i] - 1;
                           else
                              local_hi = local_lo + blockSz[i] - 1;
                       }
                  }
                 else
                  {
                    if (j == minDB[i])
                         local_hi = global_lo;
                      else
                         local_hi = global_lo - lenL[i] - (minDB[i] - j - 1) * blockSz[i];

                    if (j == minDB[i])
                         local_lo = local_hi - lenL[i] + 1;
                      else
                       {
                         if (j == maxDB[i]) 
                              local_lo = local_hi - lenR[i] + 1;
                           else
                              local_lo = local_hi - blockSz[i] + 1;
                       }
                  }

            /* give the local and global start and end indices and the stride, */
            /* find the number of elements for remote processor j */
            /* the local start index is computed relative to the global start */
            /* index, using the stride to find the right start */
            /* the local end index is just clipped, relying on the stride to end */
            /* at the right place */

               if (stride >= 0)
                  {
                    if (global_lo < local_lo)
                       {
                         rMin = local_lo + stride - 1 - (local_lo - global_lo - 1) % stride;
                       }
                      else
                       {
                         rMin = global_lo;
                       }
	 
                    rMax = fMIN(local_hi, global_hi);

                    size[i][j] = (rMax >= rMin) ? ((rMax - rMin)/stride + 1) : 0;
                  }
                 else
                  {
                    if (global_lo > local_hi)
                       {
                         rMin = local_hi + stride + 1 + (global_lo - local_hi - 1) % -stride;
                       }
                      else
                       {
                         rMin = global_lo;
                       }
                    rMax = fMAX(local_lo, global_hi);
                    size[i][j]  = (rMax <= rMin) ? ((rMin - rMax)/-stride + 1) : 0;
                  }
#if 0
               printf ("In PARTI (subsched.c): i = %d  j= %d \n",i,j);
#endif
             }
        }

   indexVals = range->indexVals;
   compute_proc_offsets(dArray, indexVals, procOffsets);
   startPosn = dest.startPosn;

   if(numRanges == 0) {
     /* this corresponds to only having a single data item in the schedule, */
     /* so set numDims = 1, and the number of elements to 1, so the */
     /* datamoves will work correctly */

     add_posVec(dArray, baseProcVec, procOffsets, 
		rPosnVec, vMach->nDims, 0, 0);
     procno = invGray(vMach,rPosnVec);
     
     scheddata = NEW(SchedData) ; 
     scheddata->proc = procno ; 
     scheddata->numDims = 1; 
     scheddata->startPosn = startPosn ; 
     scheddata->numelem[0] = 1;
     scheddata->str[0]     = 1; 
     
     if(type == 1)  { /* send schedule */ 
       sched->sData[procno] = scheddata;
       sched->sMsgSz[procno] = 1; 
     }
     else if(type ==2) {  
       sched->rData[procno] = scheddata;
       sched->rMsgSz[procno] = 1; 
     }
   }
   else if(numRanges ==1)  {
     
     for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
	  (step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[0], DB[0]);
       
       procno = invGray(vMach,rPosnVec) ; 
       
       if(size[0][DB[0]] > 0 )  {
	 scheddata = NEW(SchedData) ; 
	 scheddata->proc = procno ; 
	 scheddata->numDims = numRanges ; 
	 scheddata->startPosn = startPosn ; 
	 scheddata->numelem[0] = size[0][DB[0]] ; 
	 scheddata->str[0]     = dest.step[0] ; 
       }  /* End of conditional on size */ 
       else 
	 scheddata = NULL ; 
       
       if(type == 1)  { /* send schedule */ 
	 sched->sData[procno] = scheddata ; 
	 sched->sMsgSz[procno] = size[0][DB[0]] ; 
       }
       else if(type ==2) {  
	 sched->rData[procno] = scheddata ; 
	 sched->rMsgSz[procno] = size[0][DB[0]] ; 
       }
       
       
       
       startPosn += size[0][DB[0]] * dest.step[0];
     }
     
   }
   else if(numRanges ==2)  {
     
     for (DB[1] = minDB[1]; (step[1] > 0 && DB[1] <= maxDB[1]) || 
	  (step[1] < 0 && DB[1] >= maxDB[1]); DB[1] += SIGN(step[1])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[1], DB[1]);
       startPosns[0] = startPosn ; 
       for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
	    (step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1, vMach->nDims, rangeDims[0], DB[0]);
	 
	 procno = invGray(vMach,rPosnVec1) ; 
	 
	 
	 
	 if ( (size[0][DB[0]]>0) && (size[1][DB[1]] > 0) ) { 
	   scheddata = NEW(SchedData) ; 
	   scheddata->proc = procno ; 
	   scheddata->numDims = numRanges ; 
	   scheddata->startPosn = startPosns[0] ; 
	   scheddata->numelem[0] = size[0][DB[0]] ; 
	   scheddata->numelem[1] = size[1][DB[1]] ; 
	   scheddata->str[0]     = dest.step[0] ; 
	   scheddata->str[1]     = dest.step[1] ; 
	 }
	 else scheddata = NULL ; 
	 
	 
	 if(type == 1)  { /* send schedule */ 
	   sched->sData[procno] = scheddata ; 
	   sched->sMsgSz[procno] = size[0][DB[0]] *
	     size[1][DB[1]]  ; 
	 }
	 else if(type ==2) {  
	   sched->rData[procno] = scheddata ; 
	   sched->rMsgSz[procno] = size[0][DB[0]] *
	     size[1][DB[1]]  ; 
	 }
	 
	 
	 startPosns[0] += size[0][DB[0]]* dest.step[0];
       }
       
       startPosn += size[1][DB[1]] * dest.step[1];
     }
   }
   else if(numRanges ==3)  {
     
     for (DB[2] = minDB[2]; (step[2] > 0 && DB[2] <= maxDB[2]) || 
	  (step[2] < 0 && DB[2] >= maxDB[2]); DB[2] += SIGN(step[2])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[2], DB[2]); 
       startPosns[0] = startPosn ; 
       for (DB[1] = minDB[1]; (step[1] > 0 && DB[1] <= maxDB[1]) || 
	    (step[1] < 0 && DB[1] >= maxDB[1]); DB[1] += SIGN(step[1])) {
	 add_posVec1(dArray, rPosnVec,rPosnVec1, vMach->nDims, rangeDims[1], DB[1]);
	 startPosns[1] = startPosns[0] ; 
	 for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
	      (step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
	   add_posVec1(dArray, rPosnVec1, rPosnVec2, vMach->nDims, rangeDims[0], DB[0]);
	   
	   procno = invGray(vMach,rPosnVec2) ; 
	   
	   if ( (size[0][DB[0]] > 0) && (size[1][DB[1]] > 0) && (size[2][DB[2]] > 0) ) { 
	     scheddata = NEW(SchedData) ; 
	     scheddata->proc = procno ; 
	     scheddata->numDims = numRanges ; 
	     scheddata->startPosn = startPosns[1] ; 
	     scheddata->numelem[0] = size[0][DB[0]] ; 
	     scheddata->numelem[1] = size[1][DB[1]] ; 
	     scheddata->numelem[2] = size[2][DB[2]] ; 
	     scheddata->str[0]     = dest.step[0] ; 
	     scheddata->str[1]     = dest.step[1] ; 
	     scheddata->str[2]     = dest.step[2] ; 
	   } 
	   else scheddata = NULL ; 
	   
	   
	   if(type == 1)  { /* send schedule */ 
	     sched->sData[procno] = scheddata ; 
	     sched->sMsgSz[procno] = size[0][DB[0]] *
	       size[1][DB[1]]*
		 size[2][DB[2]]  ; 
	   }
	   else if(type ==2) {  
	     sched->rData[procno] = scheddata ; 
	     sched->rMsgSz[procno] = size[0][DB[0]] *
	       size[1][DB[1]] *
		 size[2][DB[2]] ; 
	   }
	   
	   startPosns[1] += size[0][DB[0]] * dest.step[0];
	 }
	 
	 startPosns[0] += size[1][DB[1]] * dest.step[1];
       }
       startPosn += size[2][DB[2]] * dest.step[2];
     }
   }
   else if(numRanges ==4)  {
     
     for (DB[3] = minDB[3]; (step[3] > 0 && DB[3] <= maxDB[3]) || 
	  (step[3] < 0 && DB[3] >= maxDB[3]); DB[3] += SIGN(step[3])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[3], DB[3]); 
       startPosns[0] = startPosn ; 
       for (DB[2] = minDB[2]; (step[2] > 0 && DB[2] <= maxDB[2]) || 
	    (step[2] < 0 && DB[2] >= maxDB[2]); DB[2] += SIGN(step[2])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1,  vMach->nDims, rangeDims[2], DB[2]); 
	 startPosns[1] = startPosns[0] ; 
	 for (DB[1] = minDB[1]; (step[1] > 0 && DB[1] <= maxDB[1]) || 
	      (step[1] < 0 && DB[1] >= maxDB[1]); DB[1] += SIGN(step[1])) {
	   add_posVec1(dArray, rPosnVec1,rPosnVec2, vMach->nDims, rangeDims[1], DB[1]);
	   startPosns[2] = startPosns[1] ; 
	   for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
		(step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
	     add_posVec1(dArray, rPosnVec2, rPosnVec3, vMach->nDims, rangeDims[0], DB[0]);
	     
	     procno = invGray(vMach,rPosnVec3) ; 
	     
	     
	     if ( (size[0][DB[0]] > 0)
	       && (size[1][DB[1]] > 0)
	       && (size[2][DB[2]] > 0)
	       && (size[3][DB[3]] > 0)
	       ) { 
	       scheddata = NEW(SchedData) ; 
	       scheddata->proc = procno ; 
	       scheddata->numDims = numRanges ; 
	       scheddata->startPosn = startPosns[2] ; 
	       scheddata->numelem[0] = size[0][DB[0]] ; 
	       scheddata->numelem[1] = size[1][DB[1]] ; 
	       scheddata->numelem[2] = size[2][DB[2]] ; 
	       scheddata->numelem[3] = size[3][DB[3]] ; 
	       scheddata->str[0]     = dest.step[0] ; 
	       scheddata->str[1]     = dest.step[1] ; 
	       scheddata->str[2]     = dest.step[2] ; 
	       scheddata->str[3]     = dest.step[3] ; 
	     }
	     else scheddata = NULL ;  
	     
	     if(type == 1)  { /* send schedule */ 
	       sched->sData[procno] = scheddata ; 
	       sched->sMsgSz[procno] = size[0][DB[0]]*
		 size[1][DB[1]]*
		   size[2][DB[2]]*
		     size[3][DB[3]] ; 
	     }
	     else if(type ==2) {  
	       sched->rData[procno] = scheddata ; 
	       sched->rMsgSz[procno] = size[0][DB[0]] *
		 size[1][DB[1]] *
		   size[2][DB[2]] *  
		     size[3][DB[3]] ; 
	     }

	     startPosns[2] += size[0][DB[0]] * dest.step[0];
	   }
	   startPosns[1] += size[1][DB[1]] * dest.step[1];
	 }
	 startPosns[0] += size[2][DB[2]] * dest.step[2];
       }
       startPosn += size[3][DB[3]] * dest.step[3];
     }
   }
   else if(numRanges ==5)  {
     
     for (DB[4] = minDB[4]; (step[4] > 0 && DB[4] <= maxDB[4]) || 
	  (step[4] < 0 && DB[4] >= maxDB[4]); DB[4] += SIGN(step[4])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[4], DB[4]); 
       startPosns[0] = startPosn ; 
       for (DB[3] = minDB[3]; (step[3] > 0 && DB[3] <= maxDB[3]) || 
	    (step[3] < 0 && DB[3] >= maxDB[3]); DB[3] += SIGN(step[3])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1,  vMach->nDims, rangeDims[3], DB[3]); 
         startPosns[1] = startPosns[0] ; 
         for (DB[2] = minDB[2]; (step[2] > 0 && DB[2] <= maxDB[2]) || 
	      (step[2] < 0 && DB[2] >= maxDB[2]); DB[2] += SIGN(step[2])) {
	   add_posVec1(dArray, rPosnVec1, rPosnVec2,  vMach->nDims, rangeDims[2], DB[2]); 
	   startPosns[2] = startPosns[1] ; 
	   for (DB[1] = minDB[1]; (step[1] > 0 && DB[1] <= maxDB[1]) || 
	        (step[1] < 0 && DB[1] >= maxDB[1]); DB[1] += SIGN(step[1])) {
	     add_posVec1(dArray, rPosnVec2,rPosnVec3, vMach->nDims, rangeDims[1], DB[1]);
	     startPosns[3] = startPosns[2] ; 
	     for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
		  (step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
	       add_posVec1(dArray, rPosnVec3, rPosnVec4, vMach->nDims, rangeDims[0], DB[0]);
	     
	       procno = invGray(vMach,rPosnVec4) ; 
	     
	     
	       if ( (size[0][DB[0]] > 0)
		 && (size[1][DB[1]] > 0)
		 && (size[2][DB[2]] > 0)
		 && (size[3][DB[3]] > 0)
		 && (size[4][DB[4]] > 0) ) { 
	         scheddata = NEW(SchedData) ; 
	         scheddata->proc = procno ; 
	         scheddata->numDims = numRanges ; 
	         scheddata->startPosn = startPosns[3] ; 
	         scheddata->numelem[0] = size[0][DB[0]] ; 
	         scheddata->numelem[1] = size[1][DB[1]] ; 
	         scheddata->numelem[2] = size[2][DB[2]] ; 
	         scheddata->numelem[3] = size[3][DB[3]] ; 
	         scheddata->numelem[4] = size[4][DB[4]] ; 
	         scheddata->str[0]     = dest.step[0] ; 
	         scheddata->str[1]     = dest.step[1] ; 
	         scheddata->str[2]     = dest.step[2] ; 
	         scheddata->str[3]     = dest.step[3] ; 
	         scheddata->str[4]     = dest.step[4] ; 
	       }
	       else scheddata = NULL ;  
	     
	       if(type == 1)  { /* send schedule */ 
	         sched->sData[procno] = scheddata ; 
	         sched->sMsgSz[procno] = size[0][DB[0]]*
		   size[1][DB[1]]*
		     size[2][DB[2]]*
		       size[3][DB[3]]*
		         size[4][DB[4]] ; 
	       }
	       else if(type ==2) {  
	         sched->rData[procno] = scheddata ; 
	         sched->rMsgSz[procno] = size[0][DB[0]] *
		   size[1][DB[1]] *
		     size[2][DB[2]] *  
		       size[3][DB[3]] * 
		         size[4][DB[4]] ; 
	       }

	       startPosns[3] += size[0][DB[0]] * dest.step[0];
	     }
	     startPosns[2] += size[1][DB[1]] * dest.step[1];
	   }
	   startPosns[1] += size[2][DB[2]] * dest.step[2];
	 }
	 startPosns[0] += size[3][DB[3]] * dest.step[3];
       }
       startPosn += size[4][DB[4]] * dest.step[4];
     }
   }
   else if(numRanges ==6)  {
     
     for (DB[5] = minDB[5]; (step[5] > 0 && DB[5] <= maxDB[5]) || 
	  (step[5] < 0 && DB[5] >= maxDB[5]); DB[5] += SIGN(step[5])) {
       add_posVec(dArray, baseProcVec, procOffsets, 
		  rPosnVec, vMach->nDims, rangeDims[5], DB[5]); 
       startPosns[0] = startPosn ; 
       for (DB[4] = minDB[4]; (step[4] > 0 && DB[4] <= maxDB[4]) || 
	    (step[4] < 0 && DB[4] >= maxDB[4]); DB[4] += SIGN(step[4])) {
	 add_posVec1(dArray, rPosnVec, rPosnVec1,  vMach->nDims, rangeDims[4], DB[4]); 
         startPosns[1] = startPosns[0] ; 
         for (DB[3] = minDB[3]; (step[3] > 0 && DB[3] <= maxDB[3]) || 
	      (step[3] < 0 && DB[3] >= maxDB[3]); DB[3] += SIGN(step[3])) {
	   add_posVec1(dArray, rPosnVec1, rPosnVec2,  vMach->nDims, rangeDims[3], DB[3]); 
           startPosns[2] = startPosns[1] ; 
           for (DB[2] = minDB[2]; (step[2] > 0 && DB[2] <= maxDB[2]) || 
	        (step[2] < 0 && DB[2] >= maxDB[2]); DB[2] += SIGN(step[2])) {
	     add_posVec1(dArray, rPosnVec2, rPosnVec3,  vMach->nDims, rangeDims[2], DB[2]); 
	     startPosns[3] = startPosns[2] ; 
	     for (DB[1] = minDB[1]; (step[1] > 0 && DB[1] <= maxDB[1]) || 
	          (step[1] < 0 && DB[1] >= maxDB[1]); DB[1] += SIGN(step[1])) {
	       add_posVec1(dArray, rPosnVec3,rPosnVec4, vMach->nDims, rangeDims[1], DB[1]);
	       startPosns[4] = startPosns[3] ; 
	       for (DB[0] = minDB[0]; (step[0] > 0 && DB[0] <= maxDB[0]) || 
		    (step[0] < 0 && DB[0] >= maxDB[0]); DB[0] += SIGN(step[0])) {
	         add_posVec1(dArray, rPosnVec4, rPosnVec5, vMach->nDims, rangeDims[0], DB[0]);
	     
	         procno = invGray(vMach,rPosnVec5) ; 
	     
	     
	         if ( (size[0][DB[0]] > 0)
		   && (size[1][DB[1]] > 0)
		   && (size[2][DB[2]] > 0)
		   && (size[3][DB[3]] > 0)
		   && (size[4][DB[4]] > 0)
		   && (size[5][DB[5]] > 0)
		   ) { 
	           scheddata = NEW(SchedData) ; 
	           scheddata->proc = procno ; 
	           scheddata->numDims = numRanges ; 
	           scheddata->startPosn = startPosns[3] ; 
	           scheddata->numelem[0] = size[0][DB[0]] ; 
	           scheddata->numelem[1] = size[1][DB[1]] ; 
	           scheddata->numelem[2] = size[2][DB[2]] ; 
	           scheddata->numelem[3] = size[3][DB[3]] ; 
	           scheddata->numelem[4] = size[4][DB[4]] ; 
	           scheddata->numelem[5] = size[5][DB[5]] ; 
	           scheddata->str[0]     = dest.step[0] ; 
	           scheddata->str[1]     = dest.step[1] ; 
	           scheddata->str[2]     = dest.step[2] ; 
	           scheddata->str[3]     = dest.step[3] ; 
	           scheddata->str[4]     = dest.step[4] ; 
	           scheddata->str[5]     = dest.step[5] ; 
	         }
	         else scheddata = NULL ;  
	     
	         if(type == 1)  { /* send schedule */ 
	           sched->sData[procno] = scheddata ; 
	           sched->sMsgSz[procno] = size[0][DB[0]]*
		     size[1][DB[1]]*
		       size[2][DB[2]]*
		         size[3][DB[3]]*
		           size[4][DB[4]] * 
		             size[5][DB[5]] ; 
	         }
	         else if(type ==2) {  
	           sched->rData[procno] = scheddata ; 
	           sched->rMsgSz[procno] = size[0][DB[0]] *
		     size[1][DB[1]] *
		       size[2][DB[2]] *  
		         size[3][DB[3]] * 
		           size[4][DB[4]] * 
		             size[5][DB[5]] ; 
	         }

	         startPosns[4] += size[0][DB[0]] * dest.step[0];
	       }
	       startPosns[3] += size[1][DB[1]] * dest.step[1];
	     }
#if 0
          /* Looking for a purify UMR!!! */
             printf ("startPosns[2]  = %d \n",startPosns[2]);
             printf ("DB[2]          = %d \n",DB[2]);
             printf ("size[1][DB[2]] = %d \n",size[1][DB[2]]);
             printf ("dest.step[2]   = %d \n",dest.step[2]);
#endif
	     startPosns[2] += size[1][DB[2]] * dest.step[2];
	   }
	   startPosns[1] += size[2][DB[3]] * dest.step[3];
         }
         startPosns[0] += size[3][DB[4]] * dest.step[4];
       }
       startPosn += size[4][DB[5]] * dest.step[5];
     }
   }
   else {
     fatal_error("Subarrray_sched with more than 6D arrays not implemented");
   }

}



