
#include <stdio.h>
#include "utils.h"
#include "hash.h"
#include "bsparti.h"
#include "helper.h"
#include "port.h"


/*****************************************************************************
 * (added by kdb, 1/18/97)
 * 
 * get_proc_num (dArray, indexVals )
 * 
 *  returns the processor number where indexVals exists
 *
 * Inputs: 
 *  dArray    - distributed array description for the array being
 *              gathered/scattered 
 *  indexVals - a multidimensional index
 * Returns: the number of the processor where indexVals lives
 * 
 ****************************************************************************/
int get_proc_num (dArray, indexVals)
   DARRAY *dArray;
   int *indexVals;
{
   int            baseProcVec[MAX_DIM], PosnVec[MAX_DIM];
   int            procOffsets[MAX_DIM];
   DECOMP         *vMach;
   int            procno ; 

   /* some initialization stuff */
   vMach = dArray->decomp;
   gray(vMach, vMach->baseProc, baseProcVec);

   /* compute processor offsets from base processor */
   compute_proc_offsets(dArray, indexVals, procOffsets);

   /* add base processor offsets */

   add_posVec(dArray, baseProcVec, procOffsets, 
	      PosnVec, vMach->nDims, 0, 0);

   /* convert PosnVec to a linear processor number */

   procno = invGray(vMach,PosnVec);
     
   return procno;
}



