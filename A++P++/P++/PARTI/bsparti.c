#include <stdio.h>
#define MAIN_PARTI_SOURCE_FILE
#include "bsparti.h"
#include "helper.h"
#include "port.h"
#include "utils.h"
#include "hash.h"

/* globals */
int FFlag;

/* this assumes a block decomposition */
/****************************************************************************
 *
 * lalbnd(dArray, dim, start, stride ) 
 * 
 *  adjusts the bound of the array being traversed
 *  
 * 
 * Inputs: dArray  - distributed array 
 *         dim     - the dimension of dArray being traversed
 *         start   - start of range being traversed
 *         stride  - distance between elements, positive means counting up,
 *                   negative means counting down
 *                   (strides added 2/93 - als)
 * 
 * LIMITATIONS: 
 *        - in C loops must use either "<=" or ">=" to specify loop termination
 *          (i.e.  "for (i=start; i<=stop; ..)")
 *        
 ***************************************************************************/
int lalbnd(dArray, dim, start, stride ) 
  DARRAY *dArray;
  int     dim, start, stride;
{
  int     local_hi, local_lo, temp, decompDim, local_start;
  char err_str[100];

  if (dim >= dArray->nDims) {
     if (FFlag) dim++; /* to print the right Fortran dimension */
     sprintf(err_str, "lalbnd() called with illegal parameter (dim=%d)", dim);
     error(err_str);
     return(-1);
  }
  if (stride == 0) {
     error("lalbnd() called with illegal parameter (stride=0)");
     return(-1);
  }

  if ( !active(PARTI_myproc(), dArray->decomp) ) 
     if (stride > 0)
        return(dArray->local_size[dim] + dArray->ghostCells[dim]);
     else 
        return(-1);
  
  /* changing on 5/26/92 Gupta */
  local_lo = dArray->g_index_low[dim];
  local_hi = dArray->g_index_hi[dim];

/*
  if (stride > 0) {
     temp = fMAX(start, local_lo);
     return(temp - local_lo + dArray->ghostCells[dim]);
  }
  else {
     temp = fMIN(start, local_hi);
     return(temp - local_lo + dArray->ghostCells[dim]);
  }
*/

  /* the local start index is computed relative to the global */
  /* start index, using the stride */
  if (stride > 0) {
    if (start < local_lo) {
      local_start = local_lo + stride - 1 -
	(local_lo - start - 1) % stride;
    }
    else {
      local_start = start;
    }
  }
  else {
    if (start > local_hi) {
      local_start = local_hi + stride + 1 +
	(start - local_hi - 1) % -stride;
    }
    else {
      local_start = start;
    }
  }
  /* return the index in local terms */
  return(local_start - local_lo + dArray->ghostCells[dim]);

}


/* this assumes a block decomposition too, like lalbnd */
/****************************************************************************
 *
 * laubnd(dArray, dim, start, stride ) 
 * 
 *  adjusts the bound of the array being traversed
 *  
 * 
 * Inputs: dArray  - distributed array 
 *         dim     - the dimension of dArray being traversed
 *         start   - start of range being traversed
 *         stride  - distance between elements, positive means counting up,
 *                   negative means counting down
 *                   (strides added 2/93 - als)
 * 
 * LIMITATIONS: 
 *        - in C loops must use either "<=" or ">=" to specify loop termination
 *          (i.e.  "for (i=start; i<=stop; ..)")
 *        
 ***************************************************************************/
int laubnd(dArray, dim, stop, stride ) 
  DARRAY *dArray;
  int     dim, stop, stride;
{
  int     local_hi, local_lo, decompDim, local_stop;
  char err_str[100];

  if (dim >= dArray->nDims) {
     if (FFlag) dim++; /* to print the right Fortran dimension */
     sprintf(err_str, "laubnd() called with illegal parameter (dim=%d)", dim);
     error(err_str);
     return(-1);
  }
  if (stride == 0) {
     error("laubnd() called with illegal parameter (stride=0)");
     return(-1);
  }

  if ( !active(PARTI_myproc(), dArray->decomp) )
     if (stride > 0)
        return(-1);
     else
        return(dArray->local_size[dim] + dArray->ghostCells[dim]);

/* killed 1/93 - als
  if (dArray->dimDist[dim] == '*') {
     return(stop);
  }
*/

  /* changing on 5/26/92 Gupta */
  local_lo = dArray->g_index_low[dim];
  local_hi = dArray->g_index_hi[dim];

  if (stride > 0) {
     local_stop = fMIN(stop, local_hi);
  }
  else {
     local_stop = fMAX(stop, local_lo);
  }
  /* return the index in local terms */
  return(local_stop - local_lo + dArray->ghostCells[dim]);

}


/************************************************************************
 *
 * align(decomp, numDims, arrayDims, sizes, int_gCells, ext_gCells_l,
 *       ext_gCells_r,  extra_flag, decompDims) 
 * 
 * 
 * Inputs: decomp    - decomposition to which the array is being aligned
 *         numDims   - the number of dimensions of array 
 *         arrayDims  - dimension of the src array to be aligned 
 *         sizes      - size of array
 *         int_gCells - number of internal ghost cells on dimension array
 *         ext_gCells_l - number of external ghost cells on left in each dim
 *         ext_gCells_r - number of external ghost cells on right in each dim
 *         extra_flag   -Where to put extra cells if length of dim is not    
 *                 exactly divisible by number of processors
 *                 0. Default which is same as option 4.
 *                 1. on the left most processor                       
 *                 2. on the right most processor                      
 *                 3. Split equally if odd the extra one on left       
 *                 4. Split equally if odd the extra one on right      
 *         decompDims - the decomp dimension to which the array dim is 
 *                      being aligned
 * 
 * Returns: a distributed array
 * 
 ***********************************************************************/
DARRAY   *align(decomp, numDims, arrayDims, sizes, int_gCells,
		ext_gCells_l, ext_gCells_r, extra_flag, decompDims)
  DECOMP   *decomp;
  int      numDims;
  int      *arrayDims, *sizes, *int_gCells, *ext_gCells_l,
           *ext_gCells_r, *extra_flag, *decompDims;
{
   DARRAY         *dArray;
   int            i, arrayDim, decompDim;
   int            temp_L, temp_G, temp_P, t_extra;

   dArray = NEW(DARRAY);
   dArray->nDims = numDims;
   dArray->dimVecG = NEWN(int,dArray->nDims);
   dArray->dimVecL = NEWN(int,dArray->nDims);
   dArray->dimVecL_L = NEWN(int,dArray->nDims);
   dArray->dimVecL_R = NEWN(int,dArray->nDims);
   dArray->g_index_low = NEWN(int,dArray->nDims);
   dArray->g_index_hi  = NEWN(int,dArray->nDims);
   dArray->local_size = NEWN(int,dArray->nDims);
   dArray->dimDist = NEWN(char,dArray->nDims);
   dArray->ghostCells = NEWN(int,dArray->nDims);
   dArray->decompDim  = NEWN(int,dArray->nDims);
   dArray->decompPosn = NEWN(int,dArray->nDims);
   dArray->referenceCount = 0;  /* Code added by Dan Quinlan */

   gray(decomp, PARTI_myproc(), dArray->decompPosn);
   dArray->decomp = decomp;
   decomp->referenceCount++;    /* Code added by Dan Quinlan */
   
   for (i = 0; i < numDims; i++) {
      arrayDim                     = arrayDims[i];
      dArray->dimVecG[arrayDim]    = sizes[i];
      dArray->ghostCells[arrayDim] = int_gCells[i];
      decompDim                    = decompDims[i];
      dArray->decompDim[arrayDim]  = decompDim;
      if ((decompDim == -1) || (decomp->dimDist[decompDim] == '*') ||
	  decomp->dimProc[decompDim] == 1) {
         dArray->dimDist[arrayDim]    = '*';
         /* dArray->ghostCells[arrayDim] = 0;          */
         dArray->dimVecL[arrayDim] = dArray->dimVecG[arrayDim];
         dArray->dimVecL_L[arrayDim] = dArray->dimVecG[arrayDim];
         dArray->dimVecL_R[arrayDim] = dArray->dimVecG[arrayDim];
      }
      else {
	temp_G = dArray->dimVecG[arrayDim]-ext_gCells_l[arrayDim] - ext_gCells_r[arrayDim];
	temp_P = decomp->dimProc[decompDim];
	temp_L = temp_G/temp_P;
	
	if ( temp_L * temp_P < temp_G ) {
	  if ( abs(temp_L*temp_P - temp_G) > abs((temp_L+1)*temp_P - temp_G))
	    temp_L++;
	   
	  t_extra = temp_G - temp_L*temp_P;
	  dArray->dimVecL[arrayDim] = temp_L;
	  dArray->dimVecL_L[arrayDim] = temp_L;
	  dArray->dimVecL_R[arrayDim] = temp_L;
	  
	  switch (extra_flag[i]) {
	  case 0:
	  case 4:
	    dArray->dimVecL_L[arrayDim] = temp_L + t_extra/2;
	    dArray->dimVecL_R[arrayDim] = temp_L + t_extra - t_extra/2;
	    break;
	  case 1:
	    dArray->dimVecL_L[arrayDim] = temp_L + t_extra;
	    break;
	  case 2:
	    dArray->dimVecL_R[arrayDim] = temp_L + t_extra;
	    break;
	  case 3:
	    dArray->dimVecL_R[arrayDim] = temp_L + t_extra/2;
	    dArray->dimVecL_L[arrayDim] = temp_L + t_extra - t_extra/2;
	    break;
	  default:
	    error("Out of range value for extra_flag in align");
	    break;
	  }

	  if (( dArray->dimVecL_R[arrayDim] <= 0) ||
	      ( dArray->dimVecL_L[arrayDim] <= 0)) {
	    /* shouldn't have added 1 to temp_L in the first place */
	    temp_L--;

	    t_extra = temp_G - temp_L*temp_P;
	    dArray->dimVecL[arrayDim] = temp_L;
	    dArray->dimVecL_L[arrayDim] = temp_L;
	    dArray->dimVecL_R[arrayDim] = temp_L;
	    
	    switch (extra_flag[i]) {
	    case 0:
	    case 4:
	      dArray->dimVecL_L[arrayDim] = temp_L + t_extra/2;
	      dArray->dimVecL_R[arrayDim] = temp_L + t_extra - t_extra/2;
	      break;
	    case 1:
	      dArray->dimVecL_L[arrayDim] = temp_L + t_extra;
	      break;
	    case 2:
	      dArray->dimVecL_R[arrayDim] = temp_L + t_extra;
	      break;
	    case 3:
	      dArray->dimVecL_R[arrayDim] = temp_L + t_extra/2;
	      dArray->dimVecL_L[arrayDim] = temp_L + t_extra - t_extra/2;
	      break;
	    default:
	      break;
	    }
	  }
	}
	else{
	  dArray->dimVecL[arrayDim] = temp_L;
	  dArray->dimVecL_R[arrayDim] = temp_L;
	  dArray->dimVecL_L[arrayDim] = temp_L;
	}

	dArray->dimVecL_R[arrayDim] += ext_gCells_r[arrayDim];
	dArray->dimVecL_L[arrayDim] += ext_gCells_l[arrayDim];
      }

	dArray->dimDist[arrayDim]    = decomp->dimDist[decompDim];

      if (dArray->ghostCells[arrayDim] > dArray->dimVecL[arrayDim] &&
	  dArray->dimDist[arrayDim] != '*') {
	error("more internal ghost cells requested than real cells available on immediate neighbor");
      }

      /*....Set the global bounds for my processor */
      if (active(PARTI_myproc(), decomp)) {
	/*....Set the lower bound */
	if (decompDim < 0 || dArray->decompPosn[decompDim] == 0)
	  /* My processor is the first processor */
	  dArray->g_index_low[arrayDim] = 0;
	else 
	  dArray->g_index_low[arrayDim] = dArray->dimVecL_L[arrayDim] +
	    (dArray->decompPosn[decompDim]-1)*dArray->dimVecL[arrayDim];

	/*....Set the upper bound */
	if (decompDim < 0 || decomp->dimProc[decompDim] == 1) {
	  /* only one processor is assigned to this decomp dimension, so */
	  /* this processor is both the first and last */
	  dArray->g_index_hi[arrayDim] = dArray->dimVecL_L[arrayDim] -1;
	}
	else if (dArray->decompPosn[decompDim] == decomp->dimProc[decompDim]-1 )
	  /* My processor is the last processor */
	  dArray->g_index_hi[arrayDim] = dArray->dimVecL_L[arrayDim] +
	    dArray->dimVecL_R[arrayDim] + 
	      (dArray->decompPosn[decompDim]-1)*dArray->dimVecL[arrayDim] -1;
	else
	  dArray->g_index_hi[arrayDim] = dArray->dimVecL_L[arrayDim] +
	    (dArray->decompPosn[decompDim])*dArray->dimVecL[arrayDim]-1;

	dArray->local_size[arrayDim] = dArray->g_index_hi[arrayDim] -
	  dArray->g_index_low[arrayDim] + 1;
      }
      else {
	/* this processor is not in the decomposition, so owns none of the */
	/* darray */
	dArray->g_index_low[arrayDim] = -1;
	dArray->g_index_hi[arrayDim] = -1;
	dArray->local_size[arrayDim] = 0;
      }
    }
   return(dArray);
 }


/***************************************************************************
 *
 * vProc( numDims, sizes )
 * 
 * Inputs: vProcPtr  - 
 *         numDims   - the number of dimensions in virtual processor space
 *                     LIMITATION : Only 1 DIM
 *         sizes  - array of the number of virtual processors in
 *                  each dimension
 *
 * Creates virtual processor space
 *
 * Virtual processor numbers start at 0 in each dimension (For C)
 **************************************************************************/
VPROC *vProc(numDims, sizes)
  int numDims, *sizes;
{
   int            i, arrayDim, nprocs;
   VPROC          *vProcPtr;
   char err_str[200];

   nprocs = PARTI_numprocs();
   /* since this is the first call a user should make to the library, do */
   /* the check here */
   if (nprocs > MAX_NODES && PARTI_myproc() == 0) {
     sprintf(err_str, "Multiblock PARTI library called with number processors = %d, and compiled for MAX_NODES = %d", nprocs, MAX_NODES);
     fatal_error(err_str);
   }

   vProcPtr = NEW(VPROC);
   vProcPtr->nProcs = nprocs;
   vProcPtr->usedProcs = 0;

  /*
   * for now assume vproc has only 1 dimension
   */
   vProcPtr->nDims  = numDims;
   vProcPtr->dimVec = NEWN(int,numDims);
   for(i=0; i<numDims; i++) {
      vProcPtr->dimVec[i]    =  sizes[i];
   }

   return(vProcPtr);
}



/**************************************************************************
 *
 * if node is one of the processors assigned to the decomposition decomp 
 * then return TRUE otherwise FALSE
 *
 **************************************************************************/
int active(node, decomp)
   int    node;
   DECOMP *decomp;
{
   int baseProc, maxProc;

   baseProc = decomp->baseProc;
   maxProc  = baseProc + decomp->nProcs;

   return((node >= baseProc) && (node < maxProc));
}


/* 
 *       ---------------COMMENTS-----------------
 *
 *  - add a field in dArray for each distributed dimension depicting
 *    which processor in the virtual machine (decomposition) this
 *    partition of data is bound to.
 *    This field should be initialized in align() (ie when a dArray is
 *    bound to a decomposition -- bind_vMach_to_dArray() -- ) using gray().  
 *    This reduces computation in other procedures.
 *
 */





/* assumes ghost cells always symmetric (size is local length plus
   2 times number of ghost cells) */
/*************************************************************************
 *
 * void laSizes (dArray, sizes)
 * 
 *  This procedure computes the local size in each dimension for an 
 *  array whose physical and distribution characteristic
 *  are described in the input argument  dArray.
 *  This routine is used for memory allocation.
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         sizes  -  the local size for each dimension of array
 *                 
 * Returns: nothing
 * 
 ************************************************************************/
void laSizes(dArray, info)
   DARRAY *dArray;
   int  *info;
{
   int    i;

/* changing on 05/26/92 Gupta */
   for (i=0; i<dArray->nDims; i++) {
/*     info[i] = dArray->dimVecL[i] + (2 * dArray->ghostCells[i]); */
     info[i] = dArray->local_size[i];
     if (info[i] > 0) {
       /* total local size includes internal ghost cells, if this processor */
       /* owns any of the darray */
       info[i] +=  2 * dArray->ghostCells[i];
     }
   }
}


/*************************************************************************
 *
 *  int gLBnd (dArray, dim)
 * 
 *  This procedure computes and returns the lower bound of the range of
 *  locally stored global indices.  A description of the arrays physical 
 *  and distribution characteristics is passed as an input parameter 
 *  along with the dimension being queried.
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         dim    -  the dimension of array being queried
 *                 
 * Returns: an integer corresponding to the lower bound of the
 *          range of global indices stored locally.
 * 
 ************************************************************************/
int gLBnd(dArray, dim)
   int    dim;
   DARRAY *dArray;
{
/* changing on 05/26/92 Gupta */

   int    arrayDim, decompDim;

   arrayDim  = dim;
   decompDim = dArray->decompDim[arrayDim]; 
/*   return(dArray->dimVecL[arrayDim] * dArray->decompPosn[decompDim]); */
   return(dArray->g_index_low[arrayDim]);

}



/*************************************************************************
 *
 *  int gUBnd (dArray, dim)
 * 
 *  This procedure computes and returns the upper bound of the range of
 *  locally stored global indices.  A description of the arrays physical 
 *  and distribution characteristics is passed as an input parameter 
 *  along with the dimension being queried.
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         dim    -  the dimension of array being queried
 *                 
 * Returns: an integer corresponding to the upper bound of the
 *          range of global indices stored locally.
 * 
 ************************************************************************/
int gUBnd(dArray, dim)
   DARRAY *dArray;
   int  dim;
{
/* changing on 05/26/92 Gupta */

   int    arrayDim, decompDim;

   arrayDim  = dim;
   decompDim = dArray->decompDim[arrayDim]; 
/*   return(dArray->dimVecL[arrayDim] * (dArray->decompPosn[decompDim] + 1) - 1); */
   return(dArray->g_index_hi[arrayDim]);
   
}


/* assumes a block decomposition */
/*************************************************************************
 *
 *  int globalToLocal (dArray, index, dim)
 * 
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         index  -  the global index value whose local
 *                   value is being computed.
 *         dim    -  the array dimension of interest
 *                 
 * Returns: an integer representing the local index value, or -1 if the
 *          processor doesn't own that part of the darray 
 * 
 ************************************************************************/
int globalToLocal(dArray, index, dim)
   DARRAY *dArray;
   int   dim, index;
{
  if (index < dArray -> g_index_low[dim] ||
      index > dArray -> g_index_hi[dim]) {
    return(-1);
  }

  return(index - dArray->g_index_low[dim] + dArray->ghostCells[dim]);
}

/* assumes a block decomposition */
/*************************************************************************
 *
 *  int globalToLocalWithGhost (dArray, index, dim)
 * 
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         index  -  the global index value whose local
 *                   value is being computed.
 *         dim    -  the array dimension of interest
 *                 
 * Returns: an integer representing the local index value, or -1 if the
 *          processor doesn't own that part of the darray and doesn't have
 *          a copy of that global index value as an internal ghost cell
 * 
 ************************************************************************/
int globalToLocalWithGhost(dArray, index, dim)
   DARRAY *dArray;
   int   dim, index;
{
  int num_ghost = dArray -> ghostCells[dim];

  if (index < dArray -> g_index_low[dim] - num_ghost ||
      index > dArray -> g_index_hi[dim] + num_ghost) {
    return(-1);
  }

  return(index - dArray->g_index_low[dim] + num_ghost);
}

/* assumes a block decomposition */
/*************************************************************************
 *
 *  int localToGlobal (dArray, index, dim)
 * 
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         index  -  the local index value whose global 
 *                   value is being computed.
 *         dim    -  the array dimension of interest
 *                 
 * Returns: an integer representing the global index value, or -1 if the
 *          index is not a real darray element (e.g. a ghost cell, or an
 *          out of bounds index)
 * 
 ************************************************************************/
int localToGlobal(dArray, index, dim)
   DARRAY *dArray;
   int   dim, index;
{
  /* changed on 05/26/92 Gupta */

  /* adjust for internal ghost cells */
  index -= dArray->ghostCells[dim];
  if (index < 0 || index >= dArray->local_size[dim]) {
    return (-1);
  }
  return(dArray->g_index_low[dim] + index);
}

/* assumes a block decomposition */
/*************************************************************************
 *
 *  int localToGlobalWithGhost (dArray, index, dim)
 * 
 *
 * 
 * Inputs: dArray -  pointer to the distributed array
 *         index  -  the local index value whose global 
 *                   value is being computed.
 *         dim    -  the array dimension of interest
 *                 
 * Returns: an integer representing the global index value, or < 0 if the
 *          index is not a real darray element or an internal ghost cell
 *          (e.g. an out of bounds index or an internal ghost cell that
 *           doesn't correspond to a real index)
 * 
 ************************************************************************/
int localToGlobalWithGhost(dArray, index, dim)
   DARRAY *dArray;
   int   dim, index;
{
  int num_ghost = dArray -> ghostCells[dim];
  int return_val;

  /* adjust for internal ghost cells */
  index -= num_ghost;
  if (index < -num_ghost || index >= dArray->local_size[dim] + num_ghost) {
    return (-1);
  }
  return_val = dArray->g_index_low[dim] + index;
  if (return_val < 0 || return_val >= dArray->dimVecG[dim]) {
    return_val = -1;
  }
  return(return_val);
}

/****************************************************************
*
*  remove all stored exchange schedules
*
*****************************************************************/
void remove_exch_scheds()
{
  destroy_exch_table();
}

/****************************************************************
*
*  remove all stored subarray_exch schedules
*
*****************************************************************/
void remove_subarray_scheds()
{
  destroy_sub_table();
}

/****************************************************************
*
* Frees the space allocated for the schedule
*
*****************************************************************/
void free_sched_space(sched)
   SCHED  *sched;
{
  int  proc, maxp;
  SchedData *sch;

  if ( sched == NULL )
    return;

/* printf ("Inside of free_sched_space (ID_Number = %d) \n",sched->ID_Number); */
  
  assert (sched != NULL);
/* printf ("sched->referenceCount = %d \n",sched->referenceCount); */
  assert (sched->referenceCount >= 0);
  if (sched->referenceCount-- == 0)
     {
       maxp = PARTI_numprocs();
       for ( proc = 0; proc < maxp; proc++){
        /* printf ("In free_sched_space: proc = %d \n",proc); */
           sch = sched->rData[proc];
           if (sch != NULL) free(sch);
           sch = sched->sData[proc]; 
           if (sch != NULL) free(sch);
       }	
          free(sched);
     }
}

/****************************************************************
*
*  Frees the schedule, and removes it from the hash table where
* it is saved
*
*****************************************************************/
void free_sched(sched)
   SCHED  *sched;
{
  if ( sched == NULL )
    return;
  
  /* first remove from the right hash table */
  if (sched -> type == 1) {
    /* exchange schedule */
    delete_exch_table(sched);
  }
  else if (sched -> type == 2) {
    /* subarray exchange schedule */
    delete_sub_table(sched);
  }

  free_sched_space(sched);
}
