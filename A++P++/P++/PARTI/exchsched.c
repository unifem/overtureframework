#include <stdio.h>
#include "bsparti.h"
#include "helper.h"
#include "port.h"
#include "utils.h"
#include "hash.h"
#include "List.h"

void compute_sr_sched();
void init_Sched();
SCHED *ghostFillSched();

/*************************************************************************
 *
 * exchSched( dArrayPtr, dim, fill )
 * 
 *  creates the schedule describing the data motion neccessary to fill in
 *  a set of overlap/ghost cells along dimension "dim" of an array with
 *  the same characteristics as dArray. The parameter "fill" controls
 *  the dept of the overlap cells.
 * 
 * Inputs: dArrayPtr -   distributed array description for the src array 
 *                   requiring its overlap/ghost cells to be filled in.
 *         dim     - the dimension of dArray whose overlap/ghost cells are 
 *                   being updated
 *         fill -    the dept of cells along "dim" being filled in. 
 *                   A positive value implies High end fill while a
 *                   negative value implies Low end fill.
 *                 
 * 
 * Returns: pointer to the schedule describing necessary data motion
 * 
 ************************************************************************/
SCHED *exchSched(dArrayPtr, dim, fill)
   DARRAY *dArrayPtr;
   int    dim, fill;
{
   SCHED    *schedPtr = NULL;
   int      fillVec[MAX_DIM], ndims, i;

   ndims = dArrayPtr -> nDims;
   /* generate the vector for the call to the general ghost cell fill */
   /* routine - it's all 0's except in the fill dimension */
   for (i = 0; i < ndims; i++) {
     if (i == dim) {
       fillVec[i] = fill;
     }
     else {
       fillVec[i] = 0;
     }
   }

   schedPtr = ghostFillSched(dArrayPtr, ndims, fillVec);

   assert(schedPtr != NULL);
   return(schedPtr);

}


/*************************************************************************
 *
 * ghostFillSched( dArrayPtr, ndims, fillVec )
 * 
 *  creates the schedule describing the data motion neccessary to fill in
 *  a set of overlap/ghost cells for a darray with "ndims" dimensions.
 *  The parameter "fillVec", of length "ndims", describes the overlap cells
 *  to be filled, and represents the offsets in each dimension of a reference
 *  to the darray.  For example, an array reference A[i+1][j-1][k+2] would
 *  require a fillVec of [1 -1 2] to fill in the "corner" ghost cells.
 *  Note that this routine only fills in exactly the cells asked for, not
 *  all the possible permutations (e.g. in the example, only the corner is
 *  filled, not the corresponding faces and edges).  For this routine,
 *  given a block distributed darray, a processor sends to (at most)
 *  exactly one other processor and receives from (at most) exactly one
 *  other processor.
 * 
 * Inputs: dArrayPtr - distributed array description for the src array 
 *                   requiring its overlap/ghost cells to be filled in.
 *         ndims     - the number of dimensions of the dArray, and the
 *                   length of fillVec
 *         fillVec   - The offset vector describing the ghost cells to be
 *                   filled.
 *                   A positive value in a dimension implies high end
 *                   fill while a negative value implies low end fill.
 *                 
 * 
 * Returns: pointer to the schedule describing necessary data motion
 * 
 ************************************************************************/
SCHED *ghostFillSched(dArrayPtr, ndims, fillVec)
   DARRAY *dArrayPtr;
   int    ndims, *fillVec;
{
   SCHED    *schedPtr = NULL;
   struct range_rec range;
   int      i, local_fillVec[MAX_DIM], fill, decompDim;

   if (ndims != dArrayPtr -> nDims) {
     error("number of darray dimensions does not match length of fill vector for requested ghost cell fill");
     return(schedPtr);
   }

   for (i = 0; i < ndims; i++) {
     decompDim = dArrayPtr->decompDim[i];
     if (dArrayPtr->dimDist[i] != '*' &&
	 dArrayPtr->decomp->dimProc[decompDim] > 1) {
       local_fillVec[i] = fillVec[i];
     }
     else {
       local_fillVec[i] = 0;
     }

     if (abs(local_fillVec[i]) > dArrayPtr->ghostCells[i]){
       error("not enough allocated ghost cells for requested fill");
       return(schedPtr);
     }
   }

   if (!active(PARTI_myproc(), dArrayPtr->decomp)) {
      return(schedPtr);
   }

   /* check to see if we've already computed and saved the schedule */
   /* TEMPORARILY TURN OFF */
   schedPtr = lookup_exch(dArrayPtr, ndims, local_fillVec);
   /*schedPtr = NULL;*/
   if (schedPtr != NULL) {
  /* printf ("Reuse an existing SCHED in ghostFillSched \n"); */
  /* We don't want to increment the reference count of something we reuse */
  /* schedPtr->referenceCount++; */
     return schedPtr;
   }


   schedPtr = NEW(SCHED);
/* printf ("Allocated a new SCHED structure schedPtr = %p \n",schedPtr); */

   if (schedPtr == NULL) {
       fatal_error("can`t allocate space for schedule");
   }

   init_Sched(schedPtr);
   schedPtr -> type = 1;

   for (i = 0; i<ndims; i++) {
     fill = local_fillVec[i];
     if ( fill != 0) {
       /* in this dimension, only need ghost cells */
       if ( fill < 0 ){
	 range.min[i] = fill;
       }
       else {
	 range.min[i] = dArrayPtr->local_size[i];
       }
       range.max[i] = range.min[i] + abs(fill) - 1;
     }
     else {  /* need entire range */
       range.min[i] = 0;
       range.max[i] = dArrayPtr->local_size[i] - 1;
     }
     range.indexVals[i] = range.min[i];
     range.rangeDims[i] = i;
     range.step[i]     = compute_step(dArrayPtr,1,i);
   }

   assert(ndims == dArrayPtr->nDims);
   compute_sr_sched(schedPtr,&range,dArrayPtr, ndims);

   /* save the generated schedule in the hash table */
  
   insert_exch_table(schedPtr, dArrayPtr, ndims, local_fillVec);

   return(schedPtr);
}

/***********************************************************************
 *
 * compute_sr_sched ( schedPtr, rangePtr, dArray, ndims )
 * 
 *  a procedure used by ghostFillSched() to compute the communication 
 *  schedule it needs.
 * 
 * Inputs: schedPtr - schedule being generated
 *                   
 *         rangePtr - range descriptor for the ghost cells to be filled
 *                   
 *         dArray - darray descriptor whose ghost cells are filled in
 *                     by the schedule
 *
 *         ndims - number of dimensions in the dArray, and length of fillVec
 *
 * 
 * Returns: schedule describing necessary data motion
 * 
 ************************************************************************/
void compute_sr_sched(schedPtr, rangePtr, dArray, ndims)
   SCHED  *schedPtr;
   struct range_rec  *rangePtr;
   DARRAY *dArray;
   int    ndims;
{
   int             i, j;
   int             sStartPosn[MAX_DIM], rStartPosn[MAX_DIM];
   int             *step, size[MAX_DIM], total_size;
   int             sPosnVec[MAX_DIM], rPosnVec[MAX_DIM];
   struct dest       dest;
   int             *indexVals, *rangeDims;
   int             blockSz[MAX_DIM], proc, decompDim;
   int             offProcVal[MAX_DIM], offProcValMB[MAX_DIM];
   DECOMP          *vMach;
   SchedData *recvData = NULL, *sendData = NULL;

   /*
    * some initialization stuff
    */
   vMach     = dArray->decomp;
   
   indexVals = rangePtr->indexVals;
   rangeDims  = rangePtr->rangeDims;
   step      = rangePtr->step;
   dest.startPosn = compute_startPosn(dArray, rangeDims[0], indexVals );
   for (i = 0; i < ndims; i++) {
     dest.step[i] = step[i];
   }

   for (i = 0; i < ndims; i++) {
     blockSz[i]   = dArray->local_size[i];
     offProcVal[i]   = indexVals[i];
     /* the distance from the edge of the local data block w/o ghost cells */
     /* assumes that (-x % y) == -(x % y) for x,y>0 */
     offProcValMB[i] = offProcVal[i] % blockSz[i];
   }

   for (i=0; i<vMach->nDims; i++) 
      {
        rPosnVec[i] = sPosnVec[i] = dArray->decompPosn[i];
      }

   /* adjust processor coordinates for distributed dimensions */
   for (i=0; i<ndims; i++) {
     decompDim = dArray->decompDim[i];
     if (decompDim >= 0 && offProcVal[i] != 0) {
       /* the coordinate of the processor the receive data comes from */
       rPosnVec[decompDim] =  dArray->decompPosn[decompDim] +
	                      (offProcVal[i] > 0 ? 1 : -1);
       /* the coordinate of the processor the send data goes to */
#if 1
       sPosnVec[decompDim] =  dArray->decompPosn[decompDim] -
	                      (offProcVal[i] > 0 ? 1 : -1);
#else
    /* New code by Dan Quinlan */
       sPosnVec[decompDim] =  dArray->decompPosn[decompDim] +
	                      (offProcVal[i] > 0 ? 1 : -1);
#endif
     }
   }

   for (i = 0; i < ndims; i++) {
     sStartPosn[i] = offProcValMB[i] >= 0 ?
                     offProcValMB[i] : blockSz[i] + offProcValMB[i];
   }

   total_size = 1;
   for (i = 0; i < dArray->nDims; i++) {
     size[i] = abs(rangePtr->max[i] - rangePtr->min[i]) + 1;
     total_size *= size[i];
   }

   /*
    * only receive from "active" processors
    */
   /* proc becomes the processor the ghost cells come from */
   proc = invGray(vMach, rPosnVec);
   if (active(proc, vMach)) {
     /* the receives the processor must perform to fill in its ghost cells */
     recvData = NEW(SchedData);
     assert(recvData != NULL);

     recvData -> proc = proc;
     recvData -> numDims = dArray -> nDims;
     recvData -> startPosn = dest.startPosn;
     for (i = 0; i < dArray->nDims; i++) {
       recvData -> numelem[i] = size[i];
       recvData -> str[i] = dest.step[i];
     }
     schedPtr -> rData[proc] = recvData;
     schedPtr -> rMsgSz[proc] = total_size;
   }
   /*
    * only send to "active" processors
    */
   /* proc becomes the processor the ghost cells are sent to */
   proc = invGray(vMach, sPosnVec);
   if (active(proc, vMach)) {
     /* the sends the processor must do to fill in its neighbor's */
     /* ghost cells */ 
     sendData = NEW(SchedData);
     assert(sendData != NULL);
 
     sendData -> proc = proc;
     sendData -> numDims = dArray -> nDims;
     sendData -> startPosn = compute_startPosn(dArray,rangeDims[0],sStartPosn);
     for (i = 0; i < dArray->nDims; i++) {
       sendData -> numelem[i] = size[i];
       sendData -> str[i] = step[i];
     }
     schedPtr -> sData[proc] = sendData;
     schedPtr -> sMsgSz[proc] = total_size;
   }

}


/*************************************************************************
 *
 * merge_sched(total_sched, new_sched)
 *
 *  merges two schedules, putting the result into the first one.  Since in
 *  a schedule there can be only one send and one receive SchedData for
 *  each processor, the new schedule will overwrite a previous entry in
 *  total_sched for the same processor
 *
 * Inputs: total_sched - the schedule to be merged into
 *         new_sched   - the schedule that is being merged in
 *
 ************************************************************************/
void merge_sched(total_sched, new_sched)
  SCHED *total_sched, *new_sched;
{
  int proc, i;
  SchedData *data, *new_data;

  if (new_sched == NULL) return;

  for ( proc = 0; proc < PARTI_numprocs(); proc++) {
    if (new_sched->rMsgSz[proc] > 0) {
      /* copy the receive schedule data */
      total_sched->rMsgSz[proc] = new_sched->rMsgSz[proc];
      total_sched->rData[proc] = NEW(SchedData);
      data = total_sched->rData[proc];
      new_data = new_sched->rData[proc];
      data->proc = new_data->proc;
      data->numDims = new_data->numDims;
      data->startPosn = new_data->startPosn;
      for (i = 0; i < data->numDims; i++) {
	data->numelem[i] = new_data->numelem[i];
	data->str[i] = new_data->str[i];
      }
    }

    if (new_sched->sMsgSz[proc] > 0) {
      /* copy the send schedule data */
      total_sched->sMsgSz[proc] = new_sched->sMsgSz[proc];
      total_sched->sData[proc] = NEW(SchedData);
      data = total_sched->sData[proc];
      new_data = new_sched->sData[proc];
      data->proc = new_data->proc;
      data->numDims = new_data->numDims;
      data->startPosn = new_data->startPosn;
      for (i = 0; i < data->numDims; i++) {
	data->numelem[i] = new_data->numelem[i];
	data->str[i] = new_data->str[i];
      }
    }
  }
}

/* 
   compute all the combinations of n choose p items, recursively

   Assumes n > 0, p > 0 and n >= p

   The representation of the combination is a vector of 0's and 1's of
   length n, where 1 means the elements is in the set

   partial: the vector of elements selected so far
   place  : the position in the vector currently being looked at
   comb_list: the list of all generated combinations
*/
void comb(n, p, partial, place, comb_list)
int n, p, *partial, place;
List comb_list;
{
  int *new_partial, i;

  if (n - place <= p) {
    /* must add all remaining elements */
    for (i = place; i < n; i++) {
      partial[i] = 1;
    }
    insert_List(partial, comb_list);
    return;
  }

  if (p == 0) {
    /* have all the elements needed */
    for (i = place; i < n; i++) {
      partial[i] = 0;
    }
    insert_List(partial, comb_list);
    return;
  }

  /* recurse with and without the element in position "place" */
  new_partial = NEWN(int, n);
  for (i = 0; i < place; i++) {
    new_partial[i] = partial[i];
  }

  partial[place]     = 0;    /* not in partial */
  new_partial[place] = 1;    /* in new_partial */

  comb(n, p, partial, place+1, comb_list);

  comb(n, p-1, new_partial, place+1, comb_list);
}

/*************************************************************************
 *
 * ghostFillSpanSched( dArrayPtr, ndims, fillVec )
 * 
 *  creates the schedule describing the data motion neccessary to fill in
 *  a set of overlap/ghost cells for a darray with "ndims" dimensions.
 *  The parameter "fillVec", of length "ndims", describes the overlap cells
 *  to be filled, and represents the offsets in each dimension of a reference
 *  to the darray.  For example, an array reference A[i+1][j-1][k+2] would
 *  require a fillVec of [1 -1 2] to fill in the "corner" ghost cells.
 *  This routine fills in all the possible permutations of ghost cells
 *  asked for (e.g. in the example, all faces, edges and corners are
 *  filled, corresponding to fill vectors [1 -1 0], [1 0 2] and [0 -1 2]
 *  for the faces, [1 0 0], [0 -1 0] and [0 0 2] for the edges, and the
 *  corner [1 -1 2].  For this routine, a processor can send to and receive
 *  from multiple other processors.
 * 
 * Inputs: dArrayPtr - distributed array description for the src array 
 *                   requiring its overlap/ghost cells to be filled in.
 *         ndims     - the number of dimensions of the dArray, and the
 *                   length of fillVec
 *         fillVec   - The offset vector describing the ghost cells to be
 *                   filled.
 *                   A positive value in a dimension implies high end
 *                   fill while a negative value implies low end fill.
 *                 
 * 
 * Returns: pointer to the schedule describing necessary data motion
 * 
 ************************************************************************/
SCHED *ghostFillSpanSched(dArrayPtr, ndims, fillVec)
   DARRAY *dArrayPtr;
   int    ndims, *fillVec;
{
   SCHED  *schedPtr = NULL, *total_sched = NULL;
   int    i, j, p, decompDim, found, num_non_zeros;
   int    local_fillVec[MAX_DIM], tempVec[MAX_DIM], *combVec;
   List   comb_list;
   int    *partial;

   if (ndims != dArrayPtr -> nDims) {
     error("number of darray dimensions does not match length of fill vector for requested ghost cell fill");
     return(schedPtr);
   }

   for (i = 0; i < ndims; i++) {
     decompDim = dArrayPtr->decompDim[i];
     if (dArrayPtr->dimDist[i] != '*' &&
	 dArrayPtr->decomp->dimProc[decompDim] > 1) {
       local_fillVec[i] = fillVec[i];
     }
     else {
       local_fillVec[i] = 0;
     }

     if (abs(local_fillVec[i]) > dArrayPtr->ghostCells[i]){
       error("not enough allocated ghost cells for requested fill");
       return(schedPtr);
     }
   }

   if (!active(PARTI_myproc(), dArrayPtr->decomp)) {
      return(schedPtr);
   }

   total_sched = NEW(SCHED);

   if (total_sched == NULL) {
     fatal_error("can`t allocate space for schedule");
   }

   init_Sched(total_sched);
   /* don't set the type (exch or sub), since it's not going into */
   /* a hash table */

   comb_list = create_List();

   num_non_zeros = 0;
   /* ... (12/12/96,kdb) looping range doesn't appear to be correct
       because local_fillVec[0] isn't counted if set to 1 and
       isn't defined for local_fillVec[ndims] ... 
   */
   /*for (p = 1; p <= ndims; p++) {*/
   for (p = 0; p < ndims; p++) {
     if (local_fillVec[p] != 0) {
       num_non_zeros++;
     }
   }
   /* generate all combinations of n choose 1, n choose 2, ... , */
   /* n choose num_non_zeros elements */
   for (p = 1; p <= num_non_zeros; p++) {
     /* generate all combinations of ndims choose p elements */
     /* into comb_list */
     partial = NEWN(int, ndims);

     comb(ndims, p, partial, 0, comb_list);
   }

   /* generate the schedule for each combination vector */
   FOREACH(combVec, comb_list) {
     found = 0;
     for (j = 0; j < ndims; j++) {
       if (combVec[j] == 1) {
	 tempVec[j] =  local_fillVec[j];
       }
       else {
	 tempVec[j] = 0;
       }
       if (tempVec[j] != 0) {
	 found = 1;
       }
     }

     if (found) {
       schedPtr = ghostFillSched(dArrayPtr, ndims, tempVec);
       
       /* merge the new schedule into the rest */
       merge_sched(total_sched, schedPtr);
     }

     free(combVec);
   }
   
   destroy_List(comb_list);
   
   return(total_sched);

}

/* 
   compute all the permutations of n items, recursively, ignoring positions
   where the vector "vec" is 0, meaning there are no ghost cells in that
   dimension

   Assumes n > 0

   The representation of the permutation is a vector of 0's and non-zeros of
   length n, where a non-zero is a fill of that amount in that dimension
   of the darray

   vec: the vector with the number of ghost cells in each dimension, with 0
        meaning no ghost cells
   partial: the vector of elements selected so far
   place  : the position in the vector currently being looked at
   perm_list: the list of all generated permutations
*/
void perm(n, vec, partial, place, perm_list)
int n, *vec, *partial, place;
List perm_list;
{
  int *new_partial1, *new_partial2, i;

  for (; place < n && vec[place] == 0; place++) {
    /* in dimensions without ghost cells, the fill vector has a 0 */
    partial[place] = 0;
  }

  if (place >= n) {
    /* have a complete fill vector */
    insert_List(partial, perm_list);
    return;
  }

  /* recurse with all choices for the ghost cells in position "place" */
  new_partial1 = NEWN(int, n);
  new_partial2 = NEWN(int, n);
  for (i = 0; i < place; i++) {
    new_partial1[i] = partial[i];
    new_partial2[i] = partial[i];
  }

  partial[place]     = 0;
  new_partial1[place] = vec[place];
  new_partial2[place] = -vec[place];

  perm(n, vec, partial, place+1, perm_list);
  perm(n, vec, new_partial1, place+1, perm_list);
  perm(n, vec, new_partial2, place+1, perm_list);

}

/*************************************************************************
 *
 * ghostFillAllSched( dArrayPtr )
 * 
 *  creates the schedule describing the data motion neccessary to fill in
 *  all of the overlap/ghost cells for the darray pointed to by dArrayPtr.
 *  This routine fills in all the possible permutations of ghost cells
 *  (e.g. for a 3D darray, all faces, edges and corners are
 *  filled).  For example, for a 2D darray, this would correspond to
 *  building a merged schedule from calls to ghostFillSched with fill
 *  vectors [1 1], [1 0], [0 1], [-1 -1], [-1 0], [0 -1], [1 -1] and [-1 1].
 *  For this routine, a processor can send to and receive
 *  from multiple other processors.
 * 
 * Inputs: dArrayPtr - distributed array description for the src array 
 *                   requiring its overlap/ghost cells to be filled in.
 * 
 * Returns: pointer to the schedule describing necessary data motion
 * 
 ************************************************************************/
SCHED *ghostFillAllSched(dArrayPtr)
   DARRAY *dArrayPtr;
{
   SCHED  *schedPtr = NULL, *total_sched = NULL;
   int    i, j, p, decompDim, found;
   int    local_fillVec[MAX_DIM], tempVec[MAX_DIM], *permVec;
   List   perm_list;
   int    *partial;
   int    ndims = dArrayPtr->nDims;

   for (i = 0; i < ndims; i++) {
     decompDim = dArrayPtr->decompDim[i];
     if (dArrayPtr->dimDist[i] != '*' &&
	 dArrayPtr->decomp->dimProc[decompDim] > 1) {
       local_fillVec[i] = dArrayPtr->ghostCells[i];
     }
     else {
       local_fillVec[i] = 0;
     }
   }

   if (!active(PARTI_myproc(), dArrayPtr->decomp)) {
      return(schedPtr);
   }

   total_sched = NEW(SCHED);

   if (total_sched == NULL) {
     fatal_error("can`t allocate space for schedule");
   }

   init_Sched(total_sched);
   /* don't set the type (exch or sub), since it's not going into */
   /* a hash table */

   perm_list = create_List();

   /* generate all combinations of fill vectors into perm_list */
   partial = NEWN(int, ndims);

   perm(ndims, local_fillVec, partial, 0, perm_list);

   free(partial); /* Code added by Dan Quinlan */

   /* remove the first permutation - should be the one with all 0's, which */
   /* doesn't require filling ghost cells */
   remove_List(NULL, perm_list);

   /* generate the schedule for each combination vector */
   FOREACH(permVec, perm_list) {
     schedPtr = ghostFillSched(dArrayPtr, ndims, permVec);
       
     /* merge the new schedule into the rest */
     merge_sched(total_sched, schedPtr);

     free(permVec);
   }
   
   destroy_List(perm_list);
   
   return(total_sched);

}
