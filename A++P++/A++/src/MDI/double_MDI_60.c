

#include <math.h>
#include <limits.h>
#include "constants.h"
#include "machine.h"

extern int APP_DEBUG;

 













#define  DOUBLEARRAY
#ifdef INTARRAY

int MDI_Compress_Lhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }


int MDI_Compress_Lhs_Rhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Rhs,int* locbound_Rhs,int* locsize_Rhs,
   int* locdim_Rhs,int* compressible_Rhs, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs and Rhs as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Rhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Rhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Rhs[nd];

          locsize_Rhs[compressdim] *= length * locdim_Rhs[notcompress]; 

          length *= locdim_Rhs[compressdim];

          locdim_Rhs  [compressdim] = length * locdim_Rhs   [notcompress];
          loclo_Rhs   [compressdim] = length * loclo_Rhs    [notcompress];
          locbound_Rhs[compressdim] = length * (locbound_Rhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Rhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Rhs      [nd-numcompress] = locdim_Rhs      [nd];
               locsize_Rhs     [nd-numcompress] = locsize_Rhs     [nd];
               loclo_Rhs       [nd-numcompress] = loclo_Rhs       [nd];
               locbound_Rhs    [nd-numcompress] = locbound_Rhs    [nd];
               compressible_Rhs[nd-numcompress] = compressible_Rhs[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }

int MDI_Compress_Lhs_Result
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Result,int* locbound_Result,int* locsize_Result,
   int* locdim_Result,int* compressible_Result, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs and Result as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Result[compressdim])
        {
          nd = compressdim;
          while (compressible_Result[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Result[nd];

          locsize_Result[compressdim] *= length * locdim_Result[notcompress]; 

          length *= locdim_Result[compressdim];

          locdim_Result  [compressdim] = length * locdim_Result   [notcompress];
          loclo_Result   [compressdim] = length * loclo_Result    [notcompress];
          locbound_Result[compressdim] = length * (locbound_Result[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Result[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Result      [nd-numcompress] = locdim_Result      [nd];
               locsize_Result     [nd-numcompress] = locsize_Result     [nd];
               loclo_Result       [nd-numcompress] = loclo_Result       [nd];
               locbound_Result    [nd-numcompress] = locbound_Result    [nd];
               compressible_Result[nd-numcompress] = compressible_Result[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }

int MDI_Compress_Lhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Mask,int* locbound_Mask,int* locsize_Mask,
   int* locdim_Mask,int* compressible_Mask, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs and Mask as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Mask[compressdim])
        {
          nd = compressdim;
          while (compressible_Mask[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Mask[nd];

          locsize_Mask[compressdim] *= length * locdim_Mask[notcompress]; 

          length *= locdim_Mask[compressdim];

          locdim_Mask  [compressdim] = length * locdim_Mask   [notcompress];
          loclo_Mask   [compressdim] = length * loclo_Mask    [notcompress];
          locbound_Mask[compressdim] = length * (locbound_Mask[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Mask[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Mask      [nd-numcompress] = locdim_Mask      [nd];
               locsize_Mask     [nd-numcompress] = locsize_Mask     [nd];
               loclo_Mask       [nd-numcompress] = loclo_Mask       [nd];
               locbound_Mask    [nd-numcompress] = locbound_Mask    [nd];
               compressible_Mask[nd-numcompress] = compressible_Mask[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }


int MDI_Compress_Lhs_Result_Rhs
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Result,int* locbound_Result,int* locsize_Result,
   int* locdim_Result,int* compressible_Result, 
   int* loclo_Rhs,int* locbound_Rhs,int* locsize_Rhs,
   int* locdim_Rhs,int* compressible_Rhs, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs, Result and Rhs as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Result[compressdim])
        {
          nd = compressdim;
          while (compressible_Result[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Result[nd];

          locsize_Result[compressdim] *= length * locdim_Result[notcompress]; 

          length *= locdim_Result[compressdim];

          locdim_Result  [compressdim] = length * locdim_Result   [notcompress];
          loclo_Result   [compressdim] = length * loclo_Result    [notcompress];
          locbound_Result[compressdim] = length * (locbound_Result[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Result[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Result      [nd-numcompress] = locdim_Result      [nd];
               locsize_Result     [nd-numcompress] = locsize_Result     [nd];
               loclo_Result       [nd-numcompress] = loclo_Result       [nd];
               locbound_Result    [nd-numcompress] = locbound_Result    [nd];
               compressible_Result[nd-numcompress] = compressible_Result[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Rhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Rhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Rhs[nd];

          locsize_Rhs[compressdim] *= length * locdim_Rhs[notcompress]; 

          length *= locdim_Rhs[compressdim];

          locdim_Rhs  [compressdim] = length * locdim_Rhs   [notcompress];
          loclo_Rhs   [compressdim] = length * loclo_Rhs    [notcompress];
          locbound_Rhs[compressdim] = length * (locbound_Rhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Rhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Rhs      [nd-numcompress] = locdim_Rhs      [nd];
               locsize_Rhs     [nd-numcompress] = locsize_Rhs     [nd];
               loclo_Rhs       [nd-numcompress] = loclo_Rhs       [nd];
               locbound_Rhs    [nd-numcompress] = locbound_Rhs    [nd];
               compressible_Rhs[nd-numcompress] = compressible_Rhs[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }

int MDI_Compress_Lhs_Rhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Rhs,int* locbound_Rhs,int* locsize_Rhs,
   int* locdim_Rhs,int* compressible_Rhs, 
   int* loclo_Mask,int* locbound_Mask,int* locsize_Mask,
   int* locdim_Mask,int* compressible_Mask, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs, Rhs and Mask as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Rhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Rhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Rhs[nd];

          locsize_Rhs[compressdim] *= length * locdim_Rhs[notcompress]; 

          length *= locdim_Rhs[compressdim];

          locdim_Rhs  [compressdim] = length * locdim_Rhs   [notcompress];
          loclo_Rhs   [compressdim] = length * loclo_Rhs    [notcompress];
          locbound_Rhs[compressdim] = length * (locbound_Rhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Rhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Rhs      [nd-numcompress] = locdim_Rhs      [nd];
               locsize_Rhs     [nd-numcompress] = locsize_Rhs     [nd];
               loclo_Rhs       [nd-numcompress] = loclo_Rhs       [nd];
               locbound_Rhs    [nd-numcompress] = locbound_Rhs    [nd];
               compressible_Rhs[nd-numcompress] = compressible_Rhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Mask[compressdim])
        {
          nd = compressdim;
          while (compressible_Mask[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Mask[nd];

          locsize_Mask[compressdim] *= length * locdim_Mask[notcompress]; 

          length *= locdim_Mask[compressdim];

          locdim_Mask  [compressdim] = length * locdim_Mask   [notcompress];
          loclo_Mask   [compressdim] = length * loclo_Mask    [notcompress];
          locbound_Mask[compressdim] = length * (locbound_Mask[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Mask[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Mask      [nd-numcompress] = locdim_Mask      [nd];
               locsize_Mask     [nd-numcompress] = locsize_Mask     [nd];
               loclo_Mask       [nd-numcompress] = loclo_Mask       [nd];
               locbound_Mask    [nd-numcompress] = locbound_Mask    [nd];
               compressible_Mask[nd-numcompress] = compressible_Mask[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }

int MDI_Compress_Lhs_Result_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Result,int* locbound_Result,int* locsize_Result,
   int* locdim_Result,int* compressible_Result, 
   int* loclo_Mask,int* locbound_Mask,int* locsize_Mask,
   int* locdim_Mask,int* compressible_Mask, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs, Result and Mask as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Result[compressdim])
        {
          nd = compressdim;
          while (compressible_Result[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Result[nd];

          locsize_Result[compressdim] *= length * locdim_Result[notcompress]; 

          length *= locdim_Result[compressdim];

          locdim_Result  [compressdim] = length * locdim_Result   [notcompress];
          loclo_Result   [compressdim] = length * loclo_Result    [notcompress];
          locbound_Result[compressdim] = length * (locbound_Result[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Result[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Result      [nd-numcompress] = locdim_Result      [nd];
               locsize_Result     [nd-numcompress] = locsize_Result     [nd];
               loclo_Result       [nd-numcompress] = loclo_Result       [nd];
               locbound_Result    [nd-numcompress] = locbound_Result    [nd];
               compressible_Result[nd-numcompress] = compressible_Result[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Mask[compressdim])
        {
          nd = compressdim;
          while (compressible_Mask[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Mask[nd];

          locsize_Mask[compressdim] *= length * locdim_Mask[notcompress]; 

          length *= locdim_Mask[compressdim];

          locdim_Mask  [compressdim] = length * locdim_Mask   [notcompress];
          loclo_Mask   [compressdim] = length * loclo_Mask    [notcompress];
          locbound_Mask[compressdim] = length * (locbound_Mask[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Mask[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Mask      [nd-numcompress] = locdim_Mask      [nd];
               locsize_Mask     [nd-numcompress] = locsize_Mask     [nd];
               loclo_Mask       [nd-numcompress] = loclo_Mask       [nd];
               locbound_Mask    [nd-numcompress] = locbound_Mask    [nd];
               compressible_Mask[nd-numcompress] = compressible_Mask[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
   }


int MDI_Compress_Lhs_Result_Rhs_Mask
  (int* loclo_Lhs,int* locbound_Lhs,int* locsize_Lhs,
   int* locdim_Lhs,int* compressible_Lhs,  
   int* loclo_Result,int* locbound_Result,int* locsize_Result,
   int* locdim_Result,int* compressible_Result, 
   int* loclo_Rhs,int* locbound_Rhs,int* locsize_Rhs,
   int* locdim_Rhs,int* compressible_Rhs, 
   int* loclo_Mask,int* locbound_Mask,int* locsize_Mask,
   int* locdim_Mask,int* compressible_Mask, 
   int locndim, int compressdim) 
   {
     int nd; int length; int numcompress; int notcompress;

  /* compress as many dimensions of Lhs, Result, Rhs and Mask as possible into compressdim */

     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Lhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Lhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Lhs[nd];

          locsize_Lhs[compressdim] *= length * locdim_Lhs[notcompress]; 

          length *= locdim_Lhs[compressdim];

          locdim_Lhs  [compressdim] = length * locdim_Lhs   [notcompress];
          loclo_Lhs   [compressdim] = length * loclo_Lhs    [notcompress];
          locbound_Lhs[compressdim] = length * (locbound_Lhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Lhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Lhs      [nd-numcompress] = locdim_Lhs      [nd];
               locsize_Lhs     [nd-numcompress] = locsize_Lhs     [nd];
               loclo_Lhs       [nd-numcompress] = loclo_Lhs       [nd];
               locbound_Lhs    [nd-numcompress] = locbound_Lhs    [nd];
               compressible_Lhs[nd-numcompress] = compressible_Lhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Result[compressdim])
        {
          nd = compressdim;
          while (compressible_Result[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Result[nd];

          locsize_Result[compressdim] *= length * locdim_Result[notcompress]; 

          length *= locdim_Result[compressdim];

          locdim_Result  [compressdim] = length * locdim_Result   [notcompress];
          loclo_Result   [compressdim] = length * loclo_Result    [notcompress];
          locbound_Result[compressdim] = length * (locbound_Result[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Result[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Result      [nd-numcompress] = locdim_Result      [nd];
               locsize_Result     [nd-numcompress] = locsize_Result     [nd];
               loclo_Result       [nd-numcompress] = loclo_Result       [nd];
               locbound_Result    [nd-numcompress] = locbound_Result    [nd];
               compressible_Result[nd-numcompress] = compressible_Result[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Rhs[compressdim])
        {
          nd = compressdim;
          while (compressible_Rhs[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Rhs[nd];

          locsize_Rhs[compressdim] *= length * locdim_Rhs[notcompress]; 

          length *= locdim_Rhs[compressdim];

          locdim_Rhs  [compressdim] = length * locdim_Rhs   [notcompress];
          loclo_Rhs   [compressdim] = length * loclo_Rhs    [notcompress];
          locbound_Rhs[compressdim] = length * (locbound_Rhs[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Rhs[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Rhs      [nd-numcompress] = locdim_Rhs      [nd];
               locsize_Rhs     [nd-numcompress] = locsize_Rhs     [nd];
               loclo_Rhs       [nd-numcompress] = loclo_Rhs       [nd];
               locbound_Rhs    [nd-numcompress] = locbound_Rhs    [nd];
               compressible_Rhs[nd-numcompress] = compressible_Rhs[nd];
             }
       /* locndim -= numcompress;*/
        }


     /*------------------------------------------------------------------*/
     numcompress = 0;
     if (compressible_Mask[compressdim])
        {
          nd = compressdim;
          while (compressible_Mask[nd])
               nd++;
          notcompress = nd;

          length = 1;
          for (nd=compressdim+1;nd<notcompress;nd++)
               length *= locdim_Mask[nd];

          locsize_Mask[compressdim] *= length * locdim_Mask[notcompress]; 

          length *= locdim_Mask[compressdim];

          locdim_Mask  [compressdim] = length * locdim_Mask   [notcompress];
          loclo_Mask   [compressdim] = length * loclo_Mask    [notcompress];
          locbound_Mask[compressdim] = length * (locbound_Mask[notcompress]+1)-1;

       /* ... this dim has already been compressed as much as possible now ... */
          compressible_Mask[compressdim] = 0;

          numcompress = notcompress-compressdim;

       /* ... effectively remove dimensions that were compressed into compressdim ...  */

       /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd=notcompress+1;nd<locndim;nd++)
             {
               locdim_Mask      [nd-numcompress] = locdim_Mask      [nd];
               locsize_Mask     [nd-numcompress] = locsize_Mask     [nd];
               loclo_Mask       [nd-numcompress] = loclo_Mask       [nd];
               locbound_Mask    [nd-numcompress] = locbound_Mask    [nd];
               compressible_Mask[nd-numcompress] = compressible_Mask[nd];
             }
       /* locndim -= numcompress;*/
        }



     return numcompress;
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




