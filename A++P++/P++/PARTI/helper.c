#include <stdio.h>
#include "helper.h"
#include "bsparti.h"
#include "utils.h"
#include "hash.h"

/*
 * compute memory distance between consecutive elements in the range. 
 */
int compute_step(dArray, step, rangeDim)
   DARRAY *dArray;
   int             step, rangeDim;
{
   int             i, numDims, *dimSz, *ghostCells;

   numDims = dArray->nDims;
   dimSz = dArray->local_size;
   ghostCells = dArray->ghostCells;

   if (FFlag) {
      for (i = 0; i < rangeDim; i++)
         step *= (dimSz[i] + 2*ghostCells[i]);
   }
   else {
      for (i = numDims - 1; i > rangeDim; i--)
         step *= (dimSz[i] + 2*ghostCells[i]);
   }
   return (step);
}

/* 
 * compute memory distance between consecutive elements in the range. 
 */
int compute_startPosn(dArray, rangeDim, indexVals)
   DARRAY *dArray;
   int             rangeDim, *indexVals;
{
   int             i, startPosn = 0, tmp = 1, tmp2;
   int             numDim, indexVal, *dimSz, *ghostCells;

   dimSz = dArray->local_size;
   numDim = dArray->nDims;
   ghostCells = dArray->ghostCells;

   if (FFlag) {
      for (i = 0; i < numDim; i++) {
         indexVal = indexVals[i] + ghostCells[i];

	 /* try this to  to see what it does! */
         if (i != rangeDim) {
	    /* assumes that (-x % y) == -(x % y) for x,y>0 */
	    tmp2 = indexVal % (dimSz[i] + 2*ghostCells[i]);
            indexVal = tmp2 >= 0 ? tmp2 : dimSz[i] + ghostCells[i] + tmp2;
         }
         startPosn += indexVal * tmp;
         tmp *= (dimSz[i] + 2*ghostCells[i]);
      }
      return (startPosn);
   }
   else {
     for (i = 1; i <= numDim; i++) {
         indexVal = indexVals[numDim-i] + ghostCells[numDim - i];
         if ((numDim - i) != rangeDim) {
	    /* assumes that (-x % y) == -(x % y) for x,y>0 */
	    tmp2 = indexVal % (dimSz[numDim - i] + 2*ghostCells[numDim - i]);
            indexVal = tmp2 >= 0 ? tmp2 : dimSz[numDim - i] + ghostCells[numDim-i] + tmp2;
         }
         startPosn += indexVal * tmp;
         tmp *= (dimSz[numDim - i] + 2*ghostCells[numDim - i]);
      }
      return (startPosn);
   }
}

/* changes for FORTRAN: compute_startPosn() for(i=numDim-1; i>rangeDim; i--)
 * ->  (i=0; i<rangeDim; i++) ... += indexVals[i]*tmp          ->   ... +=
 * (indexVals[i]-1)*tmp compute_step() for(i=numDim-1; i>rangeDim; i--) ->
 * (i=0; i<rangeDim; i++) */


/******************************************************************************
 *
 * this procedure examines the range of data that is be transfered/received
 * and determines the processors with which I must communicate. Processors
 * are defined in terms of offsets from myself. 
 *
 ****************************************************************************/
void compute_proc_offsets(dArray, indexVals, procOffsets)
   DARRAY *dArray;
   int            *indexVals, *procOffsets;
{
   int             i, tmp = 0, numDims, *dimSize;
   char           *dimDist;
   int             block_size;
   int             block_size_L;
   int             last_proc;
   int             diff;

   numDims = dArray->nDims;
   dimDist = dArray->dimDist;
   dimSize = dArray->local_size;

   for (i = 0; i < numDims; i++) {
      procOffsets[i] = 0;
   }

   for (i = 0; i < numDims; i++) {
      if (dimDist[i] != '*') {
	block_size = dArray->dimVecL[i];
	block_size_L = dArray->dimVecL_L[i];
	last_proc = dArray->decomp->dimProc[dArray->decompDim[i]] - 1;
	diff = block_size_L - block_size;
	procOffsets[i] = ( indexVals[i] - diff)/block_size;
	if ( procOffsets[i] <= 0 ) {
	  procOffsets[i] = 0;
	}
	else if (procOffsets[i] >= last_proc) {
	  procOffsets[i] = last_proc;
	}
/*************************************************************************
	procOffsets[i] = indexVals[i] < 0 ? 
	  (indexVals[i] + 1) / dimSize[i] - 1 : indexVals[i] / dimSize[i];
*************************************************************************/	  
      }
   }
}


/* gray should be called from distribute(), not align(), since it works
   on a decomposition, not a darray */
/*
  Gray computes the coordinates of physical processor p in decomposition
  vMach, returning them in posVec - gray assumes that the processors are
  linearly numbered, with the decomposition embedded starting at processor
  vMach -> baseProc
*/
void gray(vMach, p, posVec)
   DECOMP          *vMach;
   int             p, *posVec;
{
   int             ndims, *szVec;
   int             i, tmp = 1;

   if ((p < vMach->baseProc) || (p >= vMach->baseProc + vMach->nProcs)) {
      for (i = 0; i < vMach->nDims; i++)
         posVec[i] = -1;
      return;
   }

   p -= vMach->baseProc;
   /* tmp becomes the total number of physical processors used by
      the decomposition */
   for (i = 1; i < vMach->nDims; i++)
      tmp *= vMach->dimProc[i];

   assert (tmp >= 0);

   for (i = 0; i < vMach->nDims - 1; i++) {
      posVec[i] = fMAX(0, p / tmp);
      p -= posVec[i] * tmp;
      tmp /= vMach->dimProc[i + 1];
   }
   posVec[i] = fMAX(0, p);
}


int invGray(vMach, posVec)
   DECOMP         *vMach;
   int            *posVec;
{
   int             ndims, *szVec, i, p = 0, tmp = 1;

   ndims = vMach->nDims;
   szVec = vMach->dimProc;
   for (i = 1; i <= ndims; i++) {
   /* assert( (posVec[ndims - i] >= 0) && (posVec[ndims - i] < PARTI_numprocs()) ); */
      if ((posVec[ndims - i] < 0) || (posVec[ndims - i] >= szVec[ndims - i]))
        {
	 return (-1);
        }
      p += posVec[ndims - i] * tmp;
      tmp *= szVec[ndims - i];
   }
   p += vMach->baseProc;
   if ((p < vMach->baseProc) || (p >= vMach->baseProc + vMach->nProcs))
     {
      return (-1);
     }
   else
     {
      return (p);
     }
}


void add_posVec(dArray, posVec1, posVec2, sumVec, ndims, rdim, scaler)
   DARRAY *dArray;
   int            *posVec1, *posVec2, *sumVec, ndims, rdim, scaler;
{
   int             i, tmp;

   for (i = 0; i < ndims; i++) {
      sumVec[i] = posVec1[i] + posVec2[i];
   }
   tmp = rdim;
   for (i = 0; i < rdim; i++) {
      if (dArray->dimDist[i] == '*')
	 tmp--;
   }
   sumVec[rdim /* tmp */] += scaler;  /* als - 2/10/92 */
}




void add_posVec1(dArray, oldVec,  sumVec, ndims, rdim, scaler)
   DARRAY *dArray;
   int            *oldVec, *sumVec, ndims, rdim, scaler;
{
   int             i, tmp;

   for (i = 0; i < ndims; i++) {
      sumVec[i] = oldVec[i] ;
   }
   tmp = rdim;
   sumVec[rdim /* tmp */] += scaler;  /* als - 2/10/92 */
}

void minus_posVec(dArray, posVec1, posVec2, diffVec, ndims, rdim, scaler)
   DARRAY *dArray;
   int            *posVec1, *posVec2, *diffVec, ndims, rdim, scaler;
{
   int             i, tmp;

   for (i = 0; i < ndims; i++) {
      diffVec[i] = posVec1[i] - posVec2[i];
   }
   tmp = rdim;
   for (i = 0; i < rdim; i++) {
      if (dArray->dimDist[i] == '*')
	 tmp--;
   }
   diffVec[rdim /* tmp */] -= scaler;  /* als - 2/10/92 */
}

