

#include <math.h>
#include <limits.h>
#include "constants.h"
#include "machine.h"

extern int APP_DEBUG;

 













#define  FLOATARRAY
#ifdef INTARRAY

int MDI_Local_Array_Set_Up_Lhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor)
   {
     int nd;
     int Offset_Lhs;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }



     *Offsetptr_Lhs = Offset_Lhs;

     return locndim;

   }


int MDI_Local_Array_Set_Up_Lhs_Rhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Rhs,int* locbound_Rhs,int* locstride_Rhs,
   int* locsize_Rhs,int* locdim_Rhs,int* compressible_Rhs,
   int* Offsetptr_Rhs, array_domain* Rhs_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Rhs;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Rhs    [nd] = Rhs_Descriptor->Base   [nd];
          locbound_Rhs [nd] = Rhs_Descriptor->Bound  [nd];
          locsize_Rhs  [nd] = Rhs_Descriptor->Size   [nd];
          locstride_Rhs[nd] = Rhs_Descriptor->Stride [nd];
        }

     locdim_Rhs[0] = locsize_Rhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Rhs[nd-1]>0)
               locdim_Rhs[nd] = locsize_Rhs[nd]/locsize_Rhs[nd-1];
            else
               locdim_Rhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Rhs[nd]-loclo_Rhs[nd--]) == 0) */
     while ((locbound_Rhs[nd]-loclo_Rhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Rhs = 0;
     Offset_Rhs += loclo_Rhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Rhs += loclo_Rhs[nd]*locsize_Rhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Rhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Rhs[nd] = FALSE;
          length = locbound_Rhs[nd] - loclo_Rhs[nd] +1;
          if (locstride_Rhs[nd+1]==1) 
               compressible_Rhs[nd] = (locdim_Rhs[nd] == length && length%locstride_Rhs[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... array Lhs and Rhs will be conformable after collapsing
       dimensions only if they are both compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Rhs[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Rhs[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Rhs = Offset_Rhs;
     return locndim;
   }

int MDI_Local_Array_Set_Up_Lhs_Result
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Result,int* locbound_Result,int* locstride_Result,
   int* locsize_Result,int* locdim_Result,int* compressible_Result,
   int* Offsetptr_Result, array_domain* Result_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Result;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Result    [nd] = Result_Descriptor->Base   [nd];
          locbound_Result [nd] = Result_Descriptor->Bound  [nd];
          locsize_Result  [nd] = Result_Descriptor->Size   [nd];
          locstride_Result[nd] = Result_Descriptor->Stride [nd];
        }

     locdim_Result[0] = locsize_Result[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Result[nd-1]>0)
               locdim_Result[nd] = locsize_Result[nd]/locsize_Result[nd-1];
            else
               locdim_Result[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Result[nd]-loclo_Result[nd--]) == 0) */
     while ((locbound_Result[nd]-loclo_Result[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Result = 0;
     Offset_Result += loclo_Result[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Result[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Result[nd] = FALSE;
          length = locbound_Result[nd] - loclo_Result[nd] +1;
          if (locstride_Result[nd+1]==1) 
               compressible_Result[nd] = (locdim_Result[nd] == length && length%locstride_Result[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... array Lhs and Result will be conformable after collapsing
       dimensions only if they are both compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Result[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Result[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Result = Offset_Result;
     return locndim;
   }

int MDI_Local_Array_Set_Up_Lhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Mask,int* locbound_Mask,int* locstride_Mask,
   int* locsize_Mask,int* locdim_Mask,int* compressible_Mask,
   int* Offsetptr_Mask, array_domain* Mask_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Mask;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Mask    [nd] = Mask_Descriptor->Base   [nd];
          locbound_Mask [nd] = Mask_Descriptor->Bound  [nd];
          locsize_Mask  [nd] = Mask_Descriptor->Size   [nd];
          locstride_Mask[nd] = Mask_Descriptor->Stride [nd];
        }

     locdim_Mask[0] = locsize_Mask[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Mask[nd-1]>0)
               locdim_Mask[nd] = locsize_Mask[nd]/locsize_Mask[nd-1];
            else
               locdim_Mask[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Mask[nd]-loclo_Mask[nd--]) == 0) */
     while ((locbound_Mask[nd]-loclo_Mask[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Mask = 0;
     Offset_Mask += loclo_Mask[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Mask[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Mask[nd] = FALSE;
          length = locbound_Mask[nd] - loclo_Mask[nd] +1;
          if (locstride_Mask[nd+1]==1) 
               compressible_Mask[nd] = (locdim_Mask[nd] == length && length%locstride_Mask[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... array Lhs and Mask will be conformable after collapsing
       dimensions only if they are both compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Mask[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Mask[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Mask = Offset_Mask;
     return locndim;
   }


int MDI_Local_Array_Set_Up_Lhs_Result_Rhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Result,int* locbound_Result,int* locstride_Result,
   int* locsize_Result,int* locdim_Result,int* compressible_Result,
   int* Offsetptr_Result, array_domain* Result_Descriptor,
   int* loclo_Rhs,int* locbound_Rhs,int* locstride_Rhs,
   int* locsize_Rhs,int* locdim_Rhs,int* compressible_Rhs,
   int* Offsetptr_Rhs, array_domain* Rhs_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Result; int Offset_Rhs;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Result    [nd] = Result_Descriptor->Base   [nd];
          locbound_Result [nd] = Result_Descriptor->Bound  [nd];
          locsize_Result  [nd] = Result_Descriptor->Size   [nd];
          locstride_Result[nd] = Result_Descriptor->Stride [nd];
        }

     locdim_Result[0] = locsize_Result[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Result[nd-1]>0)
               locdim_Result[nd] = locsize_Result[nd]/locsize_Result[nd-1];
            else
               locdim_Result[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Result[nd]-loclo_Result[nd--]) == 0) */
     while ((locbound_Result[nd]-loclo_Result[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Result = 0;
     Offset_Result += loclo_Result[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Result[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Result[nd] = FALSE;
          length = locbound_Result[nd] - loclo_Result[nd] +1;
          if (locstride_Result[nd+1]==1) 
               compressible_Result[nd] = (locdim_Result[nd] == length && length%locstride_Result[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Rhs    [nd] = Rhs_Descriptor->Base   [nd];
          locbound_Rhs [nd] = Rhs_Descriptor->Bound  [nd];
          locsize_Rhs  [nd] = Rhs_Descriptor->Size   [nd];
          locstride_Rhs[nd] = Rhs_Descriptor->Stride [nd];
        }

     locdim_Rhs[0] = locsize_Rhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Rhs[nd-1]>0)
               locdim_Rhs[nd] = locsize_Rhs[nd]/locsize_Rhs[nd-1];
            else
               locdim_Rhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Rhs[nd]-loclo_Rhs[nd--]) == 0) */
     while ((locbound_Rhs[nd]-loclo_Rhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Rhs = 0;
     Offset_Rhs += loclo_Rhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Rhs += loclo_Rhs[nd]*locsize_Rhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Rhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Rhs[nd] = FALSE;
          length = locbound_Rhs[nd] - loclo_Rhs[nd] +1;
          if (locstride_Rhs[nd+1]==1) 
               compressible_Rhs[nd] = (locdim_Rhs[nd] == length && length%locstride_Rhs[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... arrays Lhs,Result and Rhs will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Result[nd] || !compressible_Rhs[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Result[nd] = 0;
               compressible_Rhs[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Result = Offset_Result;
     *Offsetptr_Rhs = Offset_Rhs;
     return locndim;
   }

int MDI_Local_Array_Set_Up_Lhs_Rhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Rhs,int* locbound_Rhs,int* locstride_Rhs,
   int* locsize_Rhs,int* locdim_Rhs,int* compressible_Rhs,
   int* Offsetptr_Rhs, array_domain* Rhs_Descriptor,
   int* loclo_Mask,int* locbound_Mask,int* locstride_Mask,
   int* locsize_Mask,int* locdim_Mask,int* compressible_Mask,
   int* Offsetptr_Mask, array_domain* Mask_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Rhs; int Offset_Mask;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Rhs    [nd] = Rhs_Descriptor->Base   [nd];
          locbound_Rhs [nd] = Rhs_Descriptor->Bound  [nd];
          locsize_Rhs  [nd] = Rhs_Descriptor->Size   [nd];
          locstride_Rhs[nd] = Rhs_Descriptor->Stride [nd];
        }

     locdim_Rhs[0] = locsize_Rhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Rhs[nd-1]>0)
               locdim_Rhs[nd] = locsize_Rhs[nd]/locsize_Rhs[nd-1];
            else
               locdim_Rhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Rhs[nd]-loclo_Rhs[nd--]) == 0) */
     while ((locbound_Rhs[nd]-loclo_Rhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Rhs = 0;
     Offset_Rhs += loclo_Rhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Rhs += loclo_Rhs[nd]*locsize_Rhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Rhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Rhs[nd] = FALSE;
          length = locbound_Rhs[nd] - loclo_Rhs[nd] +1;
          if (locstride_Rhs[nd+1]==1) 
               compressible_Rhs[nd] = (locdim_Rhs[nd] == length && length%locstride_Rhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Mask    [nd] = Mask_Descriptor->Base   [nd];
          locbound_Mask [nd] = Mask_Descriptor->Bound  [nd];
          locsize_Mask  [nd] = Mask_Descriptor->Size   [nd];
          locstride_Mask[nd] = Mask_Descriptor->Stride [nd];
        }

     locdim_Mask[0] = locsize_Mask[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Mask[nd-1]>0)
               locdim_Mask[nd] = locsize_Mask[nd]/locsize_Mask[nd-1];
            else
               locdim_Mask[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Mask[nd]-loclo_Mask[nd--]) == 0) */
     while ((locbound_Mask[nd]-loclo_Mask[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Mask = 0;
     Offset_Mask += loclo_Mask[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Mask[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Mask[nd] = FALSE;
          length = locbound_Mask[nd] - loclo_Mask[nd] +1;
          if (locstride_Mask[nd+1]==1) 
               compressible_Mask[nd] = (locdim_Mask[nd] == length && length%locstride_Mask[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... arrays Lhs,Rhs and Mask will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Rhs[nd] || !compressible_Mask[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Rhs[nd] = 0;
               compressible_Mask[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Rhs = Offset_Rhs;
     *Offsetptr_Mask = Offset_Mask;
     return locndim;
   }

int MDI_Local_Array_Set_Up_Lhs_Result_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Result,int* locbound_Result,int* locstride_Result,
   int* locsize_Result,int* locdim_Result,int* compressible_Result,
   int* Offsetptr_Result, array_domain* Result_Descriptor,
   int* loclo_Mask,int* locbound_Mask,int* locstride_Mask,
   int* locsize_Mask,int* locdim_Mask,int* compressible_Mask,
   int* Offsetptr_Mask, array_domain* Mask_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Result; int Offset_Mask;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Result    [nd] = Result_Descriptor->Base   [nd];
          locbound_Result [nd] = Result_Descriptor->Bound  [nd];
          locsize_Result  [nd] = Result_Descriptor->Size   [nd];
          locstride_Result[nd] = Result_Descriptor->Stride [nd];
        }

     locdim_Result[0] = locsize_Result[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Result[nd-1]>0)
               locdim_Result[nd] = locsize_Result[nd]/locsize_Result[nd-1];
            else
               locdim_Result[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Result[nd]-loclo_Result[nd--]) == 0) */
     while ((locbound_Result[nd]-loclo_Result[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Result = 0;
     Offset_Result += loclo_Result[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Result[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Result[nd] = FALSE;
          length = locbound_Result[nd] - loclo_Result[nd] +1;
          if (locstride_Result[nd+1]==1) 
               compressible_Result[nd] = (locdim_Result[nd] == length && length%locstride_Result[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Mask    [nd] = Mask_Descriptor->Base   [nd];
          locbound_Mask [nd] = Mask_Descriptor->Bound  [nd];
          locsize_Mask  [nd] = Mask_Descriptor->Size   [nd];
          locstride_Mask[nd] = Mask_Descriptor->Stride [nd];
        }

     locdim_Mask[0] = locsize_Mask[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Mask[nd-1]>0)
               locdim_Mask[nd] = locsize_Mask[nd]/locsize_Mask[nd-1];
            else
               locdim_Mask[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Mask[nd]-loclo_Mask[nd--]) == 0) */
     while ((locbound_Mask[nd]-loclo_Mask[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Mask = 0;
     Offset_Mask += loclo_Mask[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Mask[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Mask[nd] = FALSE;
          length = locbound_Mask[nd] - loclo_Mask[nd] +1;
          if (locstride_Mask[nd+1]==1) 
               compressible_Mask[nd] = (locdim_Mask[nd] == length && length%locstride_Mask[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... arrays Lhs,Result and Mask will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Result[nd] || !compressible_Mask[nd])
             {
               compressible_Lhs[nd] = 0;
               compressible_Result[nd] = 0;
               compressible_Mask[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Result = Offset_Result;
     *Offsetptr_Mask = Offset_Mask;
     return locndim;
   }


int MDI_Local_Array_Set_Up_Lhs_Result_Rhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locstride_Lhs,
   int* locsize_Lhs,int* locdim_Lhs,int* compressible_Lhs,
   int* Offsetptr_Lhs, array_domain* Lhs_Descriptor,
   int* loclo_Result,int* locbound_Result,int* locstride_Result,
   int* locsize_Result,int* locdim_Result,int* compressible_Result,
   int* Offsetptr_Result, array_domain* Result_Descriptor,
   int* loclo_Rhs,int* locbound_Rhs,int* locstride_Rhs,
   int* locsize_Rhs,int* locdim_Rhs,int* compressible_Rhs,
   int* Offsetptr_Rhs, array_domain* Rhs_Descriptor,
   int* loclo_Mask,int* locbound_Mask,int* locstride_Mask,
   int* locsize_Mask,int* locdim_Mask,int* compressible_Mask,
   int* Offsetptr_Mask, array_domain* Mask_Descriptor)
   {
     int nd;
     int Offset_Lhs; int Offset_Result; int Offset_Rhs; int Offset_Mask;
     int locndim; int length;

     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Lhs    [nd] = Lhs_Descriptor->Base   [nd];
          locbound_Lhs [nd] = Lhs_Descriptor->Bound  [nd];
          locsize_Lhs  [nd] = Lhs_Descriptor->Size   [nd];
          locstride_Lhs[nd] = Lhs_Descriptor->Stride [nd];
        }

     locdim_Lhs[0] = locsize_Lhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Lhs[nd-1]>0)
               locdim_Lhs[nd] = locsize_Lhs[nd]/locsize_Lhs[nd-1];
            else
               locdim_Lhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Lhs[nd]-loclo_Lhs[nd--]) == 0) */
     while ((locbound_Lhs[nd]-loclo_Lhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Lhs = 0;
     Offset_Lhs += loclo_Lhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Lhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Lhs[nd] = FALSE;
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          if (locstride_Lhs[nd+1]==1) 
               compressible_Lhs[nd] = (locdim_Lhs[nd] == length && length%locstride_Lhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Result    [nd] = Result_Descriptor->Base   [nd];
          locbound_Result [nd] = Result_Descriptor->Bound  [nd];
          locsize_Result  [nd] = Result_Descriptor->Size   [nd];
          locstride_Result[nd] = Result_Descriptor->Stride [nd];
        }

     locdim_Result[0] = locsize_Result[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Result[nd-1]>0)
               locdim_Result[nd] = locsize_Result[nd]/locsize_Result[nd-1];
            else
               locdim_Result[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Result[nd]-loclo_Result[nd--]) == 0) */
     while ((locbound_Result[nd]-loclo_Result[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Result = 0;
     Offset_Result += loclo_Result[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Result[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Result[nd] = FALSE;
          length = locbound_Result[nd] - loclo_Result[nd] +1;
          if (locstride_Result[nd+1]==1) 
               compressible_Result[nd] = (locdim_Result[nd] == length && length%locstride_Result[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Rhs    [nd] = Rhs_Descriptor->Base   [nd];
          locbound_Rhs [nd] = Rhs_Descriptor->Bound  [nd];
          locsize_Rhs  [nd] = Rhs_Descriptor->Size   [nd];
          locstride_Rhs[nd] = Rhs_Descriptor->Stride [nd];
        }

     locdim_Rhs[0] = locsize_Rhs[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Rhs[nd-1]>0)
               locdim_Rhs[nd] = locsize_Rhs[nd]/locsize_Rhs[nd-1];
            else
               locdim_Rhs[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Rhs[nd]-loclo_Rhs[nd--]) == 0) */
     while ((locbound_Rhs[nd]-loclo_Rhs[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Rhs = 0;
     Offset_Rhs += loclo_Rhs[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Rhs += loclo_Rhs[nd]*locsize_Rhs[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Rhs[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Rhs[nd] = FALSE;
          length = locbound_Rhs[nd] - loclo_Rhs[nd] +1;
          if (locstride_Rhs[nd+1]==1) 
               compressible_Rhs[nd] = (locdim_Rhs[nd] == length && length%locstride_Rhs[nd]==0);
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides using current
      descriptor set up for now, FIX THIS LATER when descriptor
      knows about arrays with more than 4 dimensions ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          loclo_Mask    [nd] = Mask_Descriptor->Base   [nd];
          locbound_Mask [nd] = Mask_Descriptor->Bound  [nd];
          locsize_Mask  [nd] = Mask_Descriptor->Size   [nd];
          locstride_Mask[nd] = Mask_Descriptor->Stride [nd];
        }

     locdim_Mask[0] = locsize_Mask[0];
     for (nd=1;nd<MAXDIMS;nd++)
        {
          if (locsize_Mask[nd-1]>0)
               locdim_Mask[nd] = locsize_Mask[nd]/locsize_Mask[nd-1];
            else
               locdim_Mask[nd]=0;
        }

     locndim = MAXDIMS;
     nd = MAXDIMS-1;
  /* while ((locbound_Mask[nd]-loclo_Mask[nd--]) == 0) */
     while ((locbound_Mask[nd]-loclo_Mask[nd]) == 0) 
        {
          nd--;
          if (nd==-1) break;
          locndim--;
        }

     Offset_Mask = 0;
     Offset_Mask += loclo_Mask[0];
  /* for (nd=locndim;nd<MAXDIMS;nd++) */
     for (nd=1;nd<MAXDIMS;nd++) 
       {
         Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
       }

  /* ... NOTE: the following could be stored in the descriptor ... */

     compressible_Mask[locndim-1] = FALSE;

     for(nd=locndim-2;nd>=0;nd--)
        {
          compressible_Mask[nd] = FALSE;
          length = locbound_Mask[nd] - loclo_Mask[nd] +1;
          if (locstride_Mask[nd+1]==1) 
               compressible_Mask[nd] = (locdim_Mask[nd] == length && length%locstride_Mask[nd]==0);
        }



     /*------------------------------------------------------------------*/

  /* ... arrays Lhs,Result,Rhs and Mask will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd=locndim-2;nd>=0;nd--)
        {
          if (!compressible_Lhs[nd] || !compressible_Result[nd] || !compressible_Rhs[nd] || !compressible_Mask[nd] )
             {
               compressible_Lhs[nd] = 0;
               compressible_Result[nd] = 0;
               compressible_Rhs[nd] = 0;
               compressible_Mask[nd] = 0;
             }
        }



     *Offsetptr_Lhs = Offset_Lhs;
     *Offsetptr_Result = Offset_Result;
     *Offsetptr_Rhs = Offset_Rhs;
     *Offsetptr_Mask = Offset_Mask;
     return locndim;
   }


#endif








/*****************************************************************/


/*****************************************************************/



/*****************************************************************/


/*****************************************************************/


/*****************************************************************/







 

/*****************************************************************/









/*****************************************************************/



/*****************************************************************/



/*****************************************************************/

/*****************************************************************/


/*****************************************************************/


/*****************************************************************/


/*****************************************************************/


/*****************************************************************/


/*****************************************************************/


/*****************************************************************/



/*==================================================================*/


/********************************************************************/


/********************************************************************/
/**loc array bound set up********************************************/


/**find compress dims************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/







/*==========================================================*/


/*==========================================================*/



/* Macro for the Lhs single array operation using the where mask! */


/********************************************************************/







/*==========================================================*/

/********************************************************************/








/*bases equal strides equal******************************************/


/********************************************************************/



/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/



/********************************************************************/



/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/



/********************************************************************/



/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/



/********************************************************************/



/*------------------------------------------------------------------*/



/*------------------------------------------------------------------*/



/********************************************************************/

/* **************************************************************** */
/* *********  Macros to support indirect addressing  ************** */
/* **************************************************************** */

/********************************************************************/


/* ---------------------------------------------------------------- */

/* Macro used to build subsequent macros */


/********************************************************************/



/* ---------------------------------------------------------------- */


/* ---------------------------------------------------------------- */


/********************************************************************/
/* Macro used to build subsequent macros */


/********************************************************************/



/* ---------------------------------------------------------------- */


/* ---------------------------------------------------------------- */


/********************************************************************/

/* Macro used to build subsequent macros */


/********************************************************************/



/********************************************************************/



/********************************************************************/






/********************************************************************/



/********************************************************************/
/**loc array bound set up********************************************/


/*zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*/



/*zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz*/
/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/






/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/


/********************************************************************/

/********************************************************************/



/********************************************************************/



/********************************************************************/









/********************************************************************/



/********************************************************************/




/*define(Macro_Indirect_Addressing_Where_Loop_Structure_Bases_NOT_Equal_Strides_NOT_Equal_Lhs_Rhs,
     Macro_Indirect_Addressing_Loop_Structure_Bases_NOT_Equal_Strides_NOT_Equal_Var0_Lhs_Var2(if (Mask_Array_Pointer[Address_Mask]) $1,Mask,3,Rhs,2)
)*/


/*define(Macro_Indirect_Addressing_Where_Loop_Structure_Bases_NOT_Equal_Strides_NOT_Equal_Result_Lhs,
     Macro_Indirect_Addressing_Loop_Structure_Bases_NOT_Equal_Strides_NOT_Equal_Var0_Lhs_Var2(if (Mask_Array_Pointer[Address_Mask]) $1,Mask,3,Result,0)
)*/


/********************************************************************/






/* **************************************************************** 
// **************************************************************** 
// Define the macros we will use for all the operator's expansions!
// **************************************************************** 
// **************************************************************** 
//
// Define the marcos used by the operator= and operators +,-,*, and /
// 
// Use the d i v e r t  m4 function to avoid output of macro definition!
*/


/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/*
//
// D e f i n e  macro for use by operators +,-,*, and /
*/

/*-------------------------------------------------------------------*/
/*
//
// D e f i n e  macro for use by min, max, pow, sign, fmod, etc.
*/

/*-------------------------------------------------------------------*/
/*
//
// D e f i n e  macro for use by replace.
*/

/*-------------------------------------------------------------------*/
/* 
// 
// Macro for Unary operations! (like unary minus, cos, sin, arc_sin, tan, etc.)
*/ 
 
/*-------------------------------------------------------------------*/
/* 
// 
// Macro for Unary operations returning intArray! (like $1Array::operator! )
*/ 
 
/*-------------------------------------------------------------------*/
/* 
// 
// Macro for Unary operations returning scalars! (like min, max, sum)
*/ 


/*-------------------------------------------------------------------*/
/* 
// 
// Macro for Unary operations! (like unary sum along axis)
*/ 
 
/*-------------------------------------------------------------------*/
/* 
// 
// Macro for Unary operation returning intArray! (like intArray::Build_Index()! )
*/ 

/*-------------------------------------------------------------------*/



/* ----------------------------------------------------------*/



/* ----------------------------------------------------------*/



/* ----------------------------------------------------------*/



/* ----------------------------------------------------------*/



/*-------------------------------------------------------------------*/



/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/



/*================================================================*/









/*================================================================*/




/*================================================================*/






/*================================================================*/




/*================================================================*/




