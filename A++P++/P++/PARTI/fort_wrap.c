#include <stdio.h>
#include "bsparti.h"
#include "hash.h"
#include "print.h"

/*****************************************************************
 * returns in info the size in each dimension of the local array
 * This routine is used for memory allocation
******************************************************************/
void flasizes_(ptr, info)
   int  *ptr, *info;
{
   DARRAY *dArray;
   int    i;

   dArray = lookupda(*ptr);
   FFlag  = 1;
   laSizes(dArray, info);
   FFlag  = 0;
}

/*****************************************************************
 * for the array pointed to by ptr determine in GLOBAL terms the
 * index of the first value in dimension "dim" stored in the
 * current processor
******************************************************************/
int ifglbnd_(ptr, dim)
   int  *ptr, *dim;
{
   DARRAY *dArray;
   int    arrayDim, decompDim;

   dArray = lookupda(*ptr);

   arrayDim  = *dim - 1;

   return(dArray->g_index_low[arrayDim]+1);

}

/*****************************************************************
 * for the array pointed to by ptr determine in GLOBAL terms the
 * index of the last value in dimension "dim" stored in the
 * current processor
******************************************************************/
int ifgubnd_(ptr, dim)
   int  *ptr, *dim;
{
   DARRAY *dArray;
   int    arrayDim, decompDim;

   dArray = lookupda(*ptr);

   arrayDim  = *dim - 1;

   return(dArray->g_index_hi[arrayDim]+1);

}

/*****************************************************************
 * for the array pointed to by ptr and the GLOBAL index "index" in
 * dimension "dim", returns the corresponding LOCAL index value
 *
******************************************************************/
int ifglobal_to_local_(ptr, index, dim)
   int  *ptr, *dim, *index;
{
   DARRAY *dArray;
   int    arrayDim, gindex;

   dArray = lookupda(*ptr);

   gindex = *index - 1;
   arrayDim  = *dim - 1;

   if (gindex < dArray -> g_index_low[arrayDim] ||
       gindex > dArray -> g_index_hi[arrayDim]) {
     return (-1);
   }

   return(gindex - dArray->g_index_low[arrayDim] +
	           dArray->ghostCells[arrayDim] + 1);
}

/*****************************************************************
 * for the array pointed to by ptr and the GLOBAL index "index" in
 * dimension "dim", returns the corresponding LOCAL index value,
 * including internal ghost cells
 *
******************************************************************/
int ifglobal_to_local_with_ghost_(ptr, index, dim)
   int  *ptr, *dim, *index;
{
   DARRAY *dArray;
   int    arrayDim, gindex;
   int num_ghost;


   dArray = lookupda(*ptr);

   gindex = *index - 1;
   arrayDim  = *dim - 1;
   num_ghost = dArray -> ghostCells[arrayDim];

   if (gindex < dArray -> g_index_low[arrayDim] - num_ghost ||
       gindex > dArray -> g_index_hi[arrayDim] + num_ghost) {
     return (-1);
   }

   return(gindex - dArray->g_index_low[arrayDim] + num_ghost + 1);
}

/*****************************************************************
 * for the array pointed to by ptr and the LOCAL index "index" in
 * dimension "dim", returns the corresponding GLOBAL "index" value
 *
******************************************************************/
int iflocal_to_global_(ptr, index, dim)
   int  *ptr, *dim, *index;
{
   DARRAY *dArray;
   int    arrayDim, decompDim, gMinIndex;
   int lindex = *index;

   dArray = lookupda(*ptr);

   arrayDim  = *dim - 1;

   /* adjust for internal ghost cells */
   lindex -= dArray->ghostCells[arrayDim];
   if (lindex < 1 || lindex > dArray->local_size[arrayDim]) {
     return (-1);
   }

   /* adjust lindex by -1 to turn into a C index, then +1 to turn whole */
   /* thing back into a Fortran index - same as just adding lindex */
   return(dArray->g_index_low[arrayDim] + lindex);
}

/*****************************************************************
 * for the array pointed to by ptr and the LOCAL index "index" in
 * dimension "dim", returns the corresponding GLOBAL "index" value,
 * including internal ghost cells
 *
******************************************************************/
int iflocal_to_global_with_ghost_(ptr, index, dim)
   int  *ptr, *dim, *index;
{
   DARRAY *dArray;
   int    arrayDim, decompDim, gMinIndex;
   int lindex = *index;
   int num_ghost;
   int return_val;

   dArray = lookupda(*ptr);

   arrayDim  = *dim - 1;

   num_ghost = dArray -> ghostCells[arrayDim];

   /* adjust for internal ghost cells */
   lindex -= num_ghost;
   if (lindex < -num_ghost+1 ||
       lindex > dArray->local_size[arrayDim] + num_ghost) {
     return (-1);
   }

   /* adjust lindex by -1 to turn into a C index, then +1 to turn whole */
   /* thing back into a Fortran index - same as just adding lindex */
   return_val = dArray->g_index_low[arrayDim] + lindex;
   if (return_val < 1 || return_val > dArray->dimVecG[arrayDim]) {
     return_val = -1;
   }
   return(return_val);
}

/*****************************************************************
 * Creates Schedule to fill in Ghost Cells and moves the data
 * Only works for Double Precision Data
 * ptr1 - points to data being move
 * ptr2 - used to access schedule
******************************************************************/
fedgefill_(ptr1, ptr2, dim, fill)
   int  *ptr1, *ptr2, *dim, *fill;
{
   DARRAY *dArray;
   struct nsched *sched;

   dArray = lookupda(*ptr2);
   FFlag  = 1;
   sched  = exchSched(dArray, *dim-1, *fill);
   FFlag  = 0;
   dDataMove((double *)ptr1,sched,(double *)ptr1);

}

/*****************************************************************
 * Creates Schedule to move subarray and moves the data
 * Only works for Double Precision Data
 * ptr1 - points to data being move
 * ptr2 - used to access schedule
******************************************************************/
/*
fsubarrayfill_(srcPtr, destPtr, ndim, info1, info2)
   int  *srcPtr, *destPtr, *ndim, *info1, *info2;
{
   SCHED          *schedPtr;
   DARRAY         *srcDAPtr, *destDAPtr;
   int            srcDims[7], destDims[7];
   int            sLos[7],    dLos[7];
   int            sHis[7],    dHis[7];
   int            i, j;
   
   FFlag = 1;

   srcDAPtr  = lookupda(*srcPtr);
   destDAPtr = lookupda(*destPtr);

   j = 0;
   for ( i = 0; i < *ndim; i++){
     srcDims[i] = info1[j]-1;
     destDims[i] = info2[j]-1;
     j++;
     sLos[i] = info1[j]-1;
     dLos[i] = info2[j]-1;
     j++;
     sHis[i] = info1[j]-1;
     dHis[i] = info2[j]-1;
     j++;
   }

   schedPtr = subArraySched(srcDAPtr, destDAPtr, *ndim,
			    srcDims,  sLos, sHis,
			    destDims, dLos, dHis);
   FFlag = 0;
   dDataMove(srcPtr,schedPtr,destPtr);
}
*/

/*****************************************************************
 * Parameters :
 *   ptr      : Reference to the Darray descriptor
 *   dim      : Fill Dimension
 *   amtFill  : Amount of Fill in.
 *              (+) At the High End
 *              (-) At the Low end.
 * Returns    : An Integer describing pointer to schedule
 * Creates Schedule to Fill in Ghost Cells.
*****************************************************************/
int ifexch_sched_(ptr, dim, amtFill)
   int  *ptr, *dim, *amtFill;
{
   struct nsched  *schedPtr, *exchSched();
   DARRAY         *dArrayPtr;

   dArrayPtr = lookupda(*ptr);

   FFlag = 1;

   schedPtr = exchSched(dArrayPtr, *dim-1, *amtFill);

   FFlag = 0;
   return((int)schedPtr);
}

/*****************************************************************
 * Parameters :
 *   ptr      : Reference to the Darray descriptor
 *   ndims    : number of Dimensions in Darray
 *   fillVec  : offset vector for ghost cells
 *              (+) At the High End
 *              (-) At the Low end.
 * Returns    : An Integer describing pointer to schedule
 * Creates Schedule to Fill in Ghost Cells.
*****************************************************************/
int ifghostfill_sched_(ptr, ndims, fillVec)
   int  *ptr, *ndims, *fillVec;
{
   SCHED  *schedPtr;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);

   FFlag = 1;

   schedPtr = ghostFillSched(dArrayPtr, *ndims, fillVec);

   FFlag = 0;
   return((int)schedPtr);
}

/*****************************************************************
 * Parameters :
 *   ptr      : Reference to the Darray descriptor
 *   ndims    : number of Dimensions in Darray
 *   fillVec  : offset vector for ghost cells
 *              (+) At the High End
 *              (-) At the Low end.
 * Returns    : An Integer describing pointer to schedule
 * Creates Schedule to Fill in all Ghost Cells related to the fill vector
*****************************************************************/
int ifghostfillspan_sched_(ptr, ndims, fillVec)
   int  *ptr, *ndims, *fillVec;
{
   SCHED  *schedPtr;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);

   FFlag = 1;

   schedPtr = ghostFillSpanSched(dArrayPtr, *ndims, fillVec);

   FFlag = 0;
   return((int)schedPtr);
}

/*****************************************************************
 * Parameters :
 *   ptr      : Reference to the Darray descriptor
 *
 * Returns    : An Integer describing pointer to schedule
 * Creates Schedule to Fill in all Ghost Cells for the darray pointed to by ptr
*****************************************************************/
int ifghostfillall_sched_(ptr)
   int  *ptr;
{
   SCHED  *schedPtr;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);

   FFlag = 1;

   schedPtr = ghostFillAllSched(dArrayPtr);

   FFlag = 0;
   return((int)schedPtr);
}

/*******************************************************************/
/* Parameters :                                                    */
/*   scrPtr   : Src dArray descriptor                              */
/*   destPtr  : Dest dArray descriptor                             */
/*   ndim     : Number of dimensions                               */
/*   s_dim    :    Source Dimensions				   */
/*   s_lo     :    Source Low Indices				   */
/*   s_hi     :    Source High Indices				   */
/*   s_stride :    Source Strides				   */
/*   d_dim    :    Dest Dimensions				   */
/*   d_lo     :    Dest Low Indices				   */
/*   d_hi     :    Dest High Indices				   */
/*   d_stride :    Dest Strides					   */
/*   Returns : an integer describing pointer to schedule	   */ 
/*   Function : Constructs a schedule describing data motion for   */
/*		copying a subarray from source into a subarray of  */
/*		destination					   */
/*******************************************************************/
int ifsubarray_sched_(srcPtr, destPtr, ndim, s_dim, s_lo, s_hi, s_stride,
		                             d_dim, d_lo, d_hi, d_stride )
   int  *srcPtr, *destPtr, *ndim ;
   int  *s_dim, *s_lo, *s_hi, *s_stride, *d_dim, *d_lo, *d_hi, *d_stride;
{
   SCHED          *schedPtr;
   DARRAY         *srcDAPtr, *destDAPtr;
   int            srcDims[7], destDims[7];
   int            sLos[7],    dLos[7];
   int            sHis[7],    dHis[7];
   int            i;
   
   FFlag = 1;

   srcDAPtr  = lookupda(*srcPtr);
   destDAPtr = lookupda(*destPtr);

   for ( i = 0; i < *ndim; i++){
     srcDims[i] = s_dim[i]-1;
     destDims[i] = d_dim[i]-1;
     sLos[i] = s_lo[i]-1;
     dLos[i] = d_lo[i]-1;
     sHis[i] = s_hi[i]-1;
     dHis[i] = d_hi[i]-1;
   }

   schedPtr = subArraySched(srcDAPtr, destDAPtr, *ndim,
			    srcDims,  sLos, sHis, s_stride,
			    destDims, dLos, dHis, d_stride);
   FFlag = 0;

   return((int)schedPtr);

}


/***************************************************************
  * Parameters :
  *   decomp : return value of decomposition ( Integer )
  *   ndim   : Number of Dimensions in Decomposition
  *   dimInfo: Sizes of dimensions
  *
  * Creates a decomposition
****************************************************************/
void fdecomp_(decomp, ndim, dimInfo)
   int *decomp, *ndim, *dimInfo;
{
   FFlag  = 1;
   *decomp = (int) create_decomp(*ndim, dimInfo);
   FFlag  = 0;
}



/***************************************************************
  * Parameters :
  *   decomp   : integer describing pointer to decomposition
  *   distInfo : Array describing distribution for each dimension
  *              (0) : Undistributed
  *              (1) : Block
  *              (2) : Cyclic ( Not Implemented )
  * Distributes a decomposition
*****************************************************************/
void fdistribute_(decomp, distInfo)
   int *decomp, *distInfo;
{
   DECOMP  *decomp1;
   char    dists[10];
   int     i;

   decomp1 = (DECOMP *) *decomp;

   for (i=0; i<decomp1->nDims; i++) {
      switch (distInfo[i]) {
      case 0: dists[i] = '*';
              break;
      case 1: dists[i] = 'B';
              break;
      case 2: dists[i] = 'C';
              break;
      }
   }
   dists[i] = '\0';

   FFlag  = 1;
   distribute(decomp1,dists);
   FFlag  = 0;

}


/***************************************************************
 * Parameters :
 *   decomp   : decomposition (Integer)
 *   vp       : Virtual processor set
 *   startPosn: Starting position in VP set
 *   endPosn  : End position in VP set
 *
 * Embeds a decomposition in subset of virtual processor space
****************************************************************/ 
void fembed_(decomp, vp, startPosn, endPosn)
   int *decomp, *vp, *startPosn, *endPosn;
{
   DECOMP  *decomp1;
   VPROC   *vproc;
   int     i;

   decomp1 = (DECOMP *) *decomp;
   vproc   = (VPROC *) *vp;

   FFlag  = 1;
   embed(decomp1, vproc, *startPosn-1, *endPosn-1);
   FFlag  = 0;

}

/*****************************************************************
  * Parameters :
  *   vp       : Integer describing virtual processor set
  *   ndim     : Number of dimensions in VP set
  *            : (Current Limitation : must be 1 dim)
  *   sizes    : VP set size in each dimension
  * creates virtual processor space and initializes hash table for darrays
  *
  * Virtual Processor number in each dimension start at 1 (for FORTRAN)
*******************************************************************/
void fvproc_(vp, ndim, sizes)
   int *vp, *ndim, *sizes;
{
   FFlag  = 1;
   *vp = (int) vProc(*ndim, sizes);
   FFlag  = 0;

   init_da_table();

}

/*****************************************************************
 * Parameters :                                                  *
 *   vMach : Decomposition                                       *
 *   ndim  : Number of dimensions in Fortran array               *
 *   info0 : darray dimensions ( Not a parameter )               *
 *   info1 : dimension sizes                                     *
 *   info2 : No. of internal GhostCells                          *
 *   ext_gCells_l - number of external ghost cells on left in each dim
 *   ext_gCells_r - number of external ghost cells on right in each dim
 *   extra_flag : Where to put extra cells if lenght of dim is not    *
 *           exactly divisible by number of processors           *
 *           0. Default, which is option 4                       *
 *           1. on the left most processor                       *
 *           2. on the right most processor                      *
 *           3. Split equally if odd the extra one on left       *
 *           4. Split equally if odd the extra one on right      *
 *   info3 : decomposition dimensions to which darray            *
 *           dimension is aligned                                *
 * Returns : an index into the table holding the darray          *
 *           descriptors                                         *
 * Aligns the given array to given decomposition                 *
*****************************************************************/
int ifalign_(vMach, ndim, info1, info2, ext_gCells_l, ext_gCells_r,
	     extra_flag, info3)
   int *vMach, *ndim, *info1, *info2, *ext_gCells_l, *ext_gCells_r,
       *extra_flag, *info3; 
{
   DECOMP *decomp;
   DARRAY *dArray;
   int    i, info0[7], info4[7];

   decomp = (DECOMP *) *vMach;

   /* Info0 contains darray dimension numbers */
   /* Info3 contains decomposition dimension numbers, and -1 means the */
   /* darray dimension is not aligned to any decomp dimension */
   /* since we need to change the dimension numbers, copy them into info4 */
   /* and change the new array */
   for (i=0; i<*ndim; i++) {
      info0[i] = i;
      info4[i] = info3[i];
      if (info4[i] > 0)
	info4[i]--;
   }
   
   FFlag  = 1;
   dArray = align(decomp,*ndim,info0,info1,info2,ext_gCells_l,ext_gCells_r,extra_flag,info4);
   FFlag  = 0;

   return insert_da_table(dArray);
}

/*********************************************************************
  * The data move functions move data for various data types
  * Parameters :
  *   src      : Starting address of Source Darray
  *   sched    : schedule describing data movement
  *   dest     : Starting address of Destination Darray
*********************************************************************/

/* Integer data move */
void fidata_move_(src, sched, dest)
   int *src, *sched, *dest;
{
   SCHED *schedPtr;

   schedPtr = (struct nsched *) *sched;
   FFlag  = 1;
   iDataMove(src, schedPtr, dest);
   FFlag  = 0;
}

/* Single precision float data move */
ffdata_move_(src, sched, dest)
   int *src, *sched, *dest;
{
   struct nsched *schedPtr;

   schedPtr = (struct nsched *) *sched;
   FFlag  = 1;
   fDataMove((float*)src, schedPtr, (float*)dest);
   FFlag  = 0;
}

/* Double precision float data move */
fddata_move_(src, sched, dest)
   int *src, *sched, *dest;
{
   struct nsched *schedPtr;

   schedPtr = (struct nsched *) *sched;
   FFlag  = 1;
   dDataMove((double *)src, schedPtr, (double *)dest);
   FFlag  = 0;
}

/* Character data move */
fcdata_move_(src, sched, dest)
   int *src, *sched, *dest;
{
   struct nsched *schedPtr;

   schedPtr = (struct nsched *) *sched;
   FFlag  = 1;
   cDataMove((char*)src, schedPtr, (char*)dest);
   FFlag  = 0;
}



/******************************************************************
  * Parameters :
  *   ptr      : Starting address of the darray
  *   dim      : Dimension in question
  *   start    : Start index in the dimension
  *              ( Fortran Indexing starts at 1 )
  *   stride   : distance between elements
  *            : positive - increasing indexes
  *            : negative - decreasing indexes
  * Returns    : The local start index
*******************************************************************/
int iflalbnd_(ptr, dim, start, stride)
   int *ptr, *dim, *start, *stride;
{
   int index;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);
   FFlag  = 1;
   index = lalbnd(dArrayPtr, *dim-1, *start-1, *stride);
   FFlag  = 0;

   return(index+1);
}

/******************************************************************
  * Parameters :
  *   ptr      : Starting address of the darray
  *   dim      : Dimension in question
  *   stop     : End index in the dimension
  *              ( Fortran Indexing starts at 1 )
  *   stride   : distance between elements
  *            : positive - increasing indexes
  *            : negative - decreasing indexes
  * Returns    : The local end index
*******************************************************************/
int iflaubnd_(ptr, dim, stop, stride)
   int *ptr, *dim, *stop, *stride;
{
   int index;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);
   FFlag  = 1;
   index = laubnd(dArrayPtr, *dim-1, *stop-1, *stride);
   FFlag  = 0;

   return(index+1);
}

/****************************************************************
*
*  remove all stored exchange schedules
*
*****************************************************************/
void remove_exch_scheds_()
{
  remove_exch_scheds();
}

/****************************************************************
*
*  remove all stored subarray_exch schedules
*
*****************************************************************/
void remove_subarray_scheds_()
{
  remove_subarray_scheds();
}

/*********************************
*
* Frees the schedule
*
**********************************/
void free_sched_(sched)
   int *sched;
{
   SCHED *schedPtr;

   schedPtr = (SCHED *) *sched;

   free_sched(schedPtr);

}

/**********************************
  * Print schedule
**********************************/  
void fprint_sched_(sched)
     int *sched;
{
   SCHED *schedPtr;

   schedPtr = (struct nsched *) *sched;
   print_sched(schedPtr);
}

/**********************************
  * Print darray
**********************************/  

fprint_darray_(ptr)
   int *ptr;
{
   int index;
   DARRAY *dArrayPtr;

   dArrayPtr = lookupda(*ptr);

   print_darray(dArrayPtr);
   
}





