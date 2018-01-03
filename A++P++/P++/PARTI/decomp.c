#include <stdio.h>
#include "utils.h"
#include "hash.h"
#include "bsparti.h"
#include "port.h"


/* DISABLED THE BODY OF THIS FUNCTION - als and gupta - 2/7/92 */
int getDecompDim(dArray, dim)
   DARRAY *dArray;
   int             dim;
{
   int             i, tmp;

/*
   if (dArray->dimDist[dim] == '*') {
      error("getDecompDim called from an undistributed dimension");
      return (0);
   } else {
      tmp = dim;
      for (i = 0; i < dim; i++) {
	 if (dArray->dimDist[i] == '*')
	    tmp--;
      }
      return (tmp);
   }
*/
   return(dim);
}


/*
isActive(procVec, minVec, maxVec, ndims)
   int            *procVec, *minVec, *maxVec, ndims;
{
   int             i;

   i = 0;
   while (((minVec[i] <= procVec[i]) && (procVec[i] <= maxVec[i])) &&
	  (i < ndims))
      i++;

   return (i == ndims);
}
*/
/*
DARRAY *create_dArray VA_DCL(int,n)
{
   DARRAY *dArray;
   va_list args;
   int i;

   dArray = NEW(DARRAY);
   dArray->referenceCount = 0;
   dArray->nDims = VA_START(args,n,int);
   dArray->dimVecG = NEWN(int,dArray->nDims);
   dArray->dimVecL = NEWN(int,dArray->nDims);
   dArray->dimDist = NEWN(char,dArray->nDims);
   dArray->ghostCells = NEWN(int,dArray->nDims);

   for (i=0; i<dArray->nDims; i++) {
      dArray->dimVecG[i] = va_arg(args, int);
      dArray->ghostCells[i] = va_arg(args, int);
   }
   va_end(args);
   return(dArray);
}
*/

/*****************************************************************************
 *
 *  create_decomp(numDims, sizes)
 * 
 *  creates the a decomposition with "numDim" dimensions whose sizes are
 *  given by the array sizes
 * 
 * Inputs: numDims - the dimension of dArray whose overlap/ghost cells are 
 *                   being updated
 *         sizes -  the sizes of the decomposition in each dimension
 *                 
 * 
 * Returns: pointer to the newly created decomposition
 * 
 *****************************************************************************/
DECOMP *create_decomp (numDims, sizes)
  int numDims, *sizes;
{
   DECOMP  *decomp;
   int i;

/* folowing two lines added by Dan Quinlan to debug memory leak */
/* static int count = 0;
   printf ("Inside of DECOMP *create_decomp (numDims, sizes) (count = %d) \n",count++);
*/
   decomp = NEW(DECOMP);
   decomp->referenceCount = 0;  /* Code added by Dan Quinlan */
   decomp->nDims    = numDims;
   decomp->dimVec   = NEWN(int, decomp->nDims);
   decomp->dimProc  = NEWN(int, decomp->nDims);
   decomp->dimDist  = NEWN(char, decomp->nDims);
   decomp->baseProc = 0;
   decomp->nProcs   = PARTI_numprocs();

   for (i=0; i<numDims; i++) {
      decomp->dimVec[i] = sizes[i];
   }

   return(decomp);
}


/******************************************************************************
 *  distribute(decomp, dist)
 * 
 *  set the type of distribution for each dimension of the decomposition
 * 
 * Inputs:   decomp    pointer to the decomposition
 *           dist      string of N character where (N = numDim).
 *                     Each character can have one of the following 3 values
 *                      'B' Blocked distribution
 *                      'C' Cyclic distribution
 *                      '*' No distribution
 *                 
 * 
 * Returns: pointer to the newly created decomposition
 * 
 ***************************************************************************/
void distribute (decomp, dist)
   DECOMP  *decomp;
   char    *dist;
{
    int    i, used, bigRatioDim, nProcs, factor;
    float  ratio, nRatio;

 
    for (i=0; i<decomp->nDims; i++) 
       decomp->dimDist[i] = dist[i];

   /*
    * decide the number of processors allocated to each dimension
    * of the decomposition.  
    */
    for (i=0; i<decomp->nDims; i++)
       decomp->dimProc[i] = 1;
    
    /* the number of physical processors used by the decomposition */
    nProcs = decomp->nProcs;

    /* don't know if this shaping of the decomposition (from a linear
       processor array into the number of dimensions of the decomposition)
       always produced the properly shaped decomposition

       changed to use greatest_prime_factor() 2/6/92 - als
    */

    for (used=1; used < nProcs; ) {
      bigRatioDim = 0;
      ratio = 1;
      if (decomp->dimDist[0] != '*')
	ratio = ((float) decomp->dimVec[0]) / ((float) decomp->dimProc[0]);
      for (i=1; i<decomp->nDims; i++) {
	if (decomp->dimDist[i] != '*')
	  nRatio = ((float) decomp->dimVec[i]) / ((float) decomp->dimProc[i]);
	else
	  nRatio = 1;
	if (nRatio > ratio ||
	    (nRatio == ratio && nRatio > 1 &&
	     decomp->dimProc[i] < decomp->dimProc[bigRatioDim])) {
	  /* if the ratios are the same, this favors the dimension that */
	  /* already is assigned fewer processors */
	  ratio = nRatio;
	  bigRatioDim = i;
	}
      }
      factor = greatest_prime_factor(nProcs/used);
      decomp->dimProc[bigRatioDim] *= factor;
      used *= factor;
    }
/*..........................................................................*/    
/* This portion has been commented to allow ghost cells even if a dimension
   is not distributed or has only one processor assigned to it. */
/*..........................................................................*/    
/*    for (i=0; i<decomp->nDims; i++) {
      if (decomp->dimProc[i] ==  1) {
	decomp->dimDist[i] = '*';
      }
    }
*/    

}



/***************************************************************************
 *
 * embed(decomp, vproc, startPosn, endPosn) 
 * 
 *  embed a decomposition in the virtual processor space.  This involves
 *  deciding what processors are allocated to the decomposition.
 *  
 * 
 * Inputs: decomp  - pointer to the decomposition
 *         vproc   - pointer to the virtual processor space
 *         startPosn - start position in the virtual processor space
 *         endPosn   - end position in the virtual processor sapce
 * 
 *
 ***************************************************************************/
void embed(decomp, vp, startPosn, endPosn)
   DECOMP *decomp;
   VPROC  *vp;
   int    startPosn, endPosn;
{
   int    firstProc, lastProc, baseProc;
   int    i, used, bigRatioDim, nProcs;
   float  ratio, nRatio;
   float  temp;

   /*
    * decide what physical processors are allocated to a decomposition.  
    */
   /* temp is the number of virtual processors per physical processor */
   temp = ((float) vp->dimVec[0])/vp->nProcs;
   /* firstProc = vp->usedProcs; */ /* why is this here? */
   firstProc = (int) startPosn/temp;
   /* als and gupta - 2/13/92 */
   /* changed to f.p. arithmetic to deal with temp not an integer
      als - 6/94 */
/*
   if (((endPosn +1) % temp) == 0)
      lastProc  = endPosn/temp;
   else
*/
   lastProc  = fMAX((int) (endPosn/temp), 0);

   /* if both physical and virtual processors are numbered starting at 0,
      then this works */
   /* this doesn't work if number of v.p.'s is not evenly divisible by the */
   /* number of physical processors */
   /* lastProc  = endPosn/temp; */

   if ((firstProc < 0) || (lastProc >= vp->nProcs)) {
      error("function embed() is allocating an illegal processor");
      return;
   }
   decomp->nProcs   = lastProc - firstProc + 1;
   decomp->baseProc = firstProc;
   vp->usedProcs += decomp->nProcs;

}


/* Rewritten as per the new definition  of Sched : 
   Gagan   07/03/93   */ 

void init_Sched(sched)
   SCHED  *sched;
{
   static ID_Counter = 1;
   int             i;

   if ( sched == NULL )
     return;

/* printf ("Inside of init_Sched (ID_Counter = %d) \n",ID_Counter); */

   for (i = 0; i < PARTI_numprocs(); i++) {
      sched->sMsgSz[i]  = 0;
      sched->rMsgSz[i]  = 0;
      /* als - 2/12/92 */
      sched->rData[i]   = NULL;
      sched->sData[i]   = NULL;
   }

   sched->type = 0;
   sched->hash = -1;
   sched->referenceCount = 0;  /* Code added by Dan Quinlan */
   sched->ID_Number = ID_Counter++;  /* Code added by Dan Quinlan */
}




