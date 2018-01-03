

#include <math.h>
#include <limits.h>
#include "constants.h"
#include "machine.h"

extern int APP_DEBUG;

 













#define  INTARRAY
#ifdef INTARRAY

void MDI_Indirect_Setup_Lhs
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS], 
   int* index_data_ptr_Lhs[MAXDIMS], int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be reordered and compressed ... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
               if (!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) 
                    dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++)
                    indr_compressible_Lhs[nd][nd2] = 0;
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */



     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;


     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;
             }
        }

#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++) ICounter[nd] = indr_loclo_Lhs[0][nd];
          ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

/*------------------------------------------------------------------*/


   }



void MDI_Indirect_Setup_Lhs_Rhs
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Rhs_Descriptor,
   int indr_loclo_Rhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Rhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Rhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Rhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Rhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Rhs[MAXDIMS],
   int indr_Sclstride_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Rhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Rhs[MAXDIMS], 
   int Rhs_Local_Map[MAXDIMS],
   int Base_Offset_Rhs[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Rhs;
     int Maximum_Bound_For_Indirect_Addressing_Rhs;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Rhs
   */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     if (Rhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Rhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Rhs = INT_MIN;

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Rhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Rhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Rhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Rhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Rhs[nd][0] = indr_locsize_Rhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2] / indr_locsize_Rhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Rhs[nd] = (-Rhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] =  0;
                         indr_locbound_Rhs [nd][nd2] = -1;
                         indr_locsize_Rhs  [nd][nd2] =  0;
                         indr_locstride_Rhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Rhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Rhs[nd][nd2] = 0;
                    Base_Offset_Rhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Rhs_Descriptor->Base[nd] == Rhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Rhs[nd] = &(Rhs_Local_Map[nd]);
	            Rhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2]     = 0;
                         indr_locbound_Rhs[nd][nd2]  = 0;
                         indr_locsize_Rhs[nd][nd2]   = 1;
                         indr_locstride_Rhs[nd][nd2] = 0;
                         indr_locdim_Rhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Rhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd] - Rhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Rhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Rhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Rhs > Rhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Rhs < Rhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Rhs[nd] = NULL;

                    indr_loclo_Rhs[nd][0] = Rhs_Descriptor->Base[nd];
                    indr_locbound_Rhs[nd][0] = Rhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Rhs_Descriptor->Stride[nd];
                    indr_locsize_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];
                    indr_locstride_Rhs[nd][0] = Rhs_Descriptor->Stride[nd];
                    indr_locdim_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2] = 0;
                         indr_locbound_Rhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Rhs[nd][nd2] = 1;
                         indr_locdim_Rhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Rhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
 */
                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Rhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Rhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Rhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Rhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Rhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Rhs[nd][0]<indr_loclo_Rhs[nd][0]) && indr_locstride_Rhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Rhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Rhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Rhs[nd][nd2] = FALSE;
               length = indr_locbound_Rhs[nd][nd2] - indr_loclo_Rhs[nd][nd2] + 1;
               if (indr_locstride_Rhs[nd][nd2+1] == 1)
                    indr_compressible_Rhs[nd][nd2] = (indr_locdim_Rhs[nd][nd2] == length && length % indr_locstride_Rhs[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Rhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Rhs <= Maximum_Bound_For_Indirect_Addressing_Rhs)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Rhs,Maximum_Bound_For_Indirect_Addressing_Rhs);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional
      arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Rhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Rhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Rhs[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Rhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Rhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Rhs[nd][0]  = indr_locdim_Rhs[nd][0];
                    indr_locbound_Rhs[nd][0] = max_dim_length[0] * indr_locstride_Rhs[nd][0]+indr_loclo_Rhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Rhs[nd][nd2]        = indr_locdim_Rhs[nd][nd2] *indr_locsize_Rhs[nd][nd2-1];
                         indr_loclo_Rhs[nd][nd2]          = 0;
                         indr_locbound_Rhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Rhs[nd][nd2]      = 1;
                         indr_compressible_Rhs[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */



  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
               if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || (!indr_compressible_Rhs[nd][nd2] && indr_locdim_Rhs[nd][locndim-1]>1) ) 
                    dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Rhs[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][longest] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][longest];
  
               indr_locdim_Rhs[nd][longest] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][longest] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][longest] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][second] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][second];
  
               indr_locdim_Rhs[nd][second] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][second] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][second] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second ) longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                  indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }

#else
  
  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Rhs[nd][0] = indr_locstride_Rhs[nd][0];
#if 1
          if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
	       ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Rhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Rhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Rhs[nd][0] !=0)
               indr_Offsetreset_Rhs[nd][0] = ((indr_locbound_Rhs[nd][0] -indr_loclo_Rhs[nd][0])/indr_locstride_Rhs[nd][0]) * indr_Sclstride_Rhs[nd][0];
            else
               indr_Offsetreset_Rhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_locstride_Rhs[nd][nd2];

#if 1
               if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
                    ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Rhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Rhs[nd][nd2] !=0)
                    indr_Offsetreset_Rhs[nd][nd2] = ((indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) / indr_locstride_Rhs[nd][nd2]) * indr_Sclstride_Rhs[nd][nd2];
                 else
                    indr_Offsetreset_Rhs[nd][nd2] = 0;

               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Rhs[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Rhs[i_debug]);
          printf ("\n");

          printf ("Rhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Rhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Rhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

/*------------------------------------------------------------------*/

   }


void MDI_Indirect_Setup_Lhs_Result
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Result_Descriptor,
   int indr_loclo_Result[MAXDIMS][MAXDIMS],
   int indr_locbound_Result[MAXDIMS][MAXDIMS],
   int indr_locstride_Result[MAXDIMS][MAXDIMS], 
   int indr_locsize_Result[MAXDIMS][MAXDIMS],
   int indr_locdim_Result[MAXDIMS][MAXDIMS],
   int indr_compressible_Result[MAXDIMS][MAXDIMS],
   int indr_Offset_Result[MAXDIMS],
   int indr_Sclstride_Result[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Result[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Result[MAXDIMS], 
   int Result_Local_Map[MAXDIMS],
   int Base_Offset_Result[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Result
   */

     MDI_ASSERT (Result_Descriptor != NULL);

     if (Result_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Result[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Result [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Result  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2] * Result_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Result[nd][0] = indr_locsize_Result[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Result[nd][nd2] = indr_locsize_Result[nd][nd2] / indr_locsize_Result[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Result[nd] = (-Result_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] =  0;
                         indr_locbound_Result [nd][nd2] = -1;
                         indr_locsize_Result  [nd][nd2] =  0;
                         indr_locstride_Result[nd][nd2] =  1;
                       }
 
                    indr_locdim_Result[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Result[nd][nd2] = 0;
                    Base_Offset_Result[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Result_Descriptor->Base[nd] == Result_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Result[nd] = &(Result_Local_Map[nd]);
	            Result_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2]     = 0;
                         indr_locbound_Result[nd][nd2]  = 0;
                         indr_locsize_Result[nd][nd2]   = 1;
                         indr_locstride_Result[nd][nd2] = 0;
                         indr_locdim_Result[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Result_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd] - Result_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Result[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Result[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Result > Result_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Result = Result_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Result < Result_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Result = Result_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Result[nd] = NULL;

                    indr_loclo_Result[nd][0] = Result_Descriptor->Base[nd];
                    indr_locbound_Result[nd][0] = Result_Descriptor->Base[nd] + (indr_dimension[0]-1) * Result_Descriptor->Stride[nd];
                    indr_locsize_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];
                    indr_locstride_Result[nd][0] = Result_Descriptor->Stride[nd];
                    indr_locdim_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2] = 0;
                         indr_locbound_Result[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Result[nd][nd2] = 1;
                         indr_locdim_Result[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Result[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
 */
                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Result)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Result;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Result)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Result;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Result pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Result[nd][0]<indr_loclo_Result[nd][0]) && indr_locstride_Result[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Result[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Result[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Result[nd][nd2] = FALSE;
               length = indr_locbound_Result[nd][nd2] - indr_loclo_Result[nd][nd2] + 1;
               if (indr_locstride_Result[nd][nd2+1] == 1)
                    indr_compressible_Result[nd][nd2] = (indr_locdim_Result[nd][nd2] == length && length % indr_locstride_Result[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Result pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Result <= Maximum_Bound_For_Indirect_Addressing_Result)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Result,Maximum_Bound_For_Indirect_Addressing_Result);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional
      arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Result[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Result[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Result[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Result[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Result[nd][0]   = max_dim_length[0];
                    indr_locsize_Result[nd][0]  = indr_locdim_Result[nd][0];
                    indr_locbound_Result[nd][0] = max_dim_length[0] * indr_locstride_Result[nd][0]+indr_loclo_Result[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Result[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Result[nd][nd2]        = indr_locdim_Result[nd][nd2] *indr_locsize_Result[nd][nd2-1];
                         indr_loclo_Result[nd][nd2]          = 0;
                         indr_locbound_Result[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Result[nd][nd2]      = 1;
                         indr_compressible_Result[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */



  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
               if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || (!indr_compressible_Result[nd][nd2] && indr_locdim_Result[nd][locndim-1]>1) ) 
                    dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Result[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][longest] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][longest];
  
               indr_locdim_Result[nd][longest] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][longest] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][longest] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][second] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][second];
  
               indr_locdim_Result[nd][second] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][second] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][second] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second ) longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                  indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;
             }
        }

#else
  
  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Result[nd][0] = indr_locstride_Result[nd][0];
#if 1
          if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
	       ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Result[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Result[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Result[nd][0] !=0)
               indr_Offsetreset_Result[nd][0] = ((indr_locbound_Result[nd][0] -indr_loclo_Result[nd][0])/indr_locstride_Result[nd][0]) * indr_Sclstride_Result[nd][0];
            else
               indr_Offsetreset_Result[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Result[nd] += indr_loclo_Result[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_locstride_Result[nd][nd2];

#if 1
               if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
                    ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Result[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Result[nd][nd2] !=0)
                    indr_Offsetreset_Result[nd][nd2] = ((indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) / indr_locstride_Result[nd][nd2]) * indr_Sclstride_Result[nd][nd2];
                 else
                    indr_Offsetreset_Result[nd][nd2] = 0;

               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Result[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Result[i_debug]);
          printf ("\n");

          printf ("Result_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Result_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Result[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

/*------------------------------------------------------------------*/

   }


void MDI_Indirect_Setup_Lhs_Mask
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional
      arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */



  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
               if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) ) 
                    dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second ) longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                  indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }

#else
  
  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

/*------------------------------------------------------------------*/

   }



void MDI_Indirect_Setup_Lhs_Result_Rhs
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Result_Descriptor,
   int indr_loclo_Result[MAXDIMS][MAXDIMS],
   int indr_locbound_Result[MAXDIMS][MAXDIMS],
   int indr_locstride_Result[MAXDIMS][MAXDIMS], 
   int indr_locsize_Result[MAXDIMS][MAXDIMS],
   int indr_locdim_Result[MAXDIMS][MAXDIMS],
   int indr_compressible_Result[MAXDIMS][MAXDIMS],
   int indr_Offset_Result[MAXDIMS],
   int indr_Sclstride_Result[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Result[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Result[MAXDIMS], 
   int Result_Local_Map[MAXDIMS],
   int Base_Offset_Result[MAXDIMS],
   array_domain* Rhs_Descriptor,
   int indr_loclo_Rhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Rhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Rhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Rhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Rhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Rhs[MAXDIMS][MAXDIMS],
/* BUGFIX (20/2/98) this should be an int not an int* 
   int* indr_Offset_Rhs[MAXDIMS], */
   int indr_Offset_Rhs[MAXDIMS],
   int indr_Sclstride_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Rhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Rhs[MAXDIMS], 
   int Rhs_Local_Map[MAXDIMS],
   int Base_Offset_Rhs[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;
     int Minimum_Base_For_Indirect_Addressing_Rhs;
     int Maximum_Bound_For_Indirect_Addressing_Rhs;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Result
   */

     MDI_ASSERT (Result_Descriptor != NULL);

     if (Result_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Rhs
   */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     if (Rhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Result[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Result [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Result  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2] * Result_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Result[nd][0] = indr_locsize_Result[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Result[nd][nd2] = indr_locsize_Result[nd][nd2] / indr_locsize_Result[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Result[nd] = (-Result_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] =  0;
                         indr_locbound_Result [nd][nd2] = -1;
                         indr_locsize_Result  [nd][nd2] =  0;
                         indr_locstride_Result[nd][nd2] =  1;
                       }
 
                    indr_locdim_Result[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Result[nd][nd2] = 0;
                    Base_Offset_Result[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Result_Descriptor->Base[nd] == Result_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Result[nd] = &(Result_Local_Map[nd]);
	            Result_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2]     = 0;
                         indr_locbound_Result[nd][nd2]  = 0;
                         indr_locsize_Result[nd][nd2]   = 1;
                         indr_locstride_Result[nd][nd2] = 0;
                         indr_locdim_Result[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Result_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd] - Result_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Result[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Result[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Result > Result_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Result = Result_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Result < Result_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Result = Result_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Result[nd] = NULL;

                    indr_loclo_Result[nd][0] = Result_Descriptor->Base[nd];
                    indr_locbound_Result[nd][0] = Result_Descriptor->Base[nd] + (indr_dimension[0]-1) * Result_Descriptor->Stride[nd];
                    indr_locsize_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];
                    indr_locstride_Result[nd][0] = Result_Descriptor->Stride[nd];
                    indr_locdim_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2] = 0;
                         indr_locbound_Result[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Result[nd][nd2] = 1;
                         indr_locdim_Result[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Result[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
 */
                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Result)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Result;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Result)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Result;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Result pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Result[nd][0]<indr_loclo_Result[nd][0]) && indr_locstride_Result[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Result[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Result[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Result[nd][nd2] = FALSE;
               length = indr_locbound_Result[nd][nd2] - indr_loclo_Result[nd][nd2] + 1;
               if (indr_locstride_Result[nd][nd2+1] == 1)
                    indr_compressible_Result[nd][nd2] = (indr_locdim_Result[nd][nd2] == length && length % indr_locstride_Result[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Rhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Rhs = INT_MIN;

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Rhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Rhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Rhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Rhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Rhs[nd][0] = indr_locsize_Rhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2] / indr_locsize_Rhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Rhs[nd] = (-Rhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] =  0;
                         indr_locbound_Rhs [nd][nd2] = -1;
                         indr_locsize_Rhs  [nd][nd2] =  0;
                         indr_locstride_Rhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Rhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Rhs[nd][nd2] = 0;
                    Base_Offset_Rhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Rhs_Descriptor->Base[nd] == Rhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Rhs[nd] = &(Rhs_Local_Map[nd]);
	            Rhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2]     = 0;
                         indr_locbound_Rhs[nd][nd2]  = 0;
                         indr_locsize_Rhs[nd][nd2]   = 1;
                         indr_locstride_Rhs[nd][nd2] = 0;
                         indr_locdim_Rhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Rhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd] - Rhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Rhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Rhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Rhs > Rhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Rhs < Rhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Rhs[nd] = NULL;

                    indr_loclo_Rhs[nd][0] = Rhs_Descriptor->Base[nd];
                    indr_locbound_Rhs[nd][0] = Rhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Rhs_Descriptor->Stride[nd];
                    indr_locsize_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];
                    indr_locstride_Rhs[nd][0] = Rhs_Descriptor->Stride[nd];
                    indr_locdim_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2] = 0;
                         indr_locbound_Rhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Rhs[nd][nd2] = 1;
                         indr_locdim_Rhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Rhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
 */
                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Rhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Rhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Rhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Rhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Rhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Rhs[nd][0]<indr_loclo_Rhs[nd][0]) && indr_locstride_Rhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Rhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Rhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Rhs[nd][nd2] = FALSE;
               length = indr_locbound_Rhs[nd][nd2] - indr_loclo_Rhs[nd][nd2] + 1;
               if (indr_locstride_Rhs[nd][nd2+1] == 1)
                    indr_compressible_Rhs[nd][nd2] = (indr_locdim_Rhs[nd][nd2] == length && length % indr_locstride_Rhs[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Result pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Result <= Maximum_Bound_For_Indirect_Addressing_Result)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Result,Maximum_Bound_For_Indirect_Addressing_Result);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Rhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Rhs <= Maximum_Bound_For_Indirect_Addressing_Rhs)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Rhs,Maximum_Bound_For_Indirect_Addressing_Rhs);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Result[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Result[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Result[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Rhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Rhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Rhs[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Result[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Result[nd][0]   = max_dim_length[0];
                    indr_locsize_Result[nd][0]  = indr_locdim_Result[nd][0];
                    indr_locbound_Result[nd][0] = max_dim_length[0] * indr_locstride_Result[nd][0]+indr_loclo_Result[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Result[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Result[nd][nd2]        = indr_locdim_Result[nd][nd2] *indr_locsize_Result[nd][nd2-1];
                         indr_loclo_Result[nd][nd2]          = 0;
                         indr_locbound_Result[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Result[nd][nd2]      = 1;
                         indr_compressible_Result[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Rhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Rhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Rhs[nd][0]  = indr_locdim_Rhs[nd][0];
                    indr_locbound_Rhs[nd][0] = max_dim_length[0] * indr_locstride_Rhs[nd][0]+indr_loclo_Rhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Rhs[nd][nd2]        = indr_locdim_Rhs[nd][nd2] *indr_locsize_Rhs[nd][nd2-1];
                         indr_loclo_Rhs[nd][nd2]          = 0;
                         indr_locbound_Rhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Rhs[nd][nd2]      = 1;
                         indr_compressible_Rhs[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
             if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Result[nd][nd2] && indr_locdim_Result[nd][locndim-1]>1) || 
                 (!indr_compressible_Rhs[nd][nd2] && indr_locdim_Rhs[nd][locndim-1]>1) ) 
                  dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Result[nd][nd2] = 0;
                    indr_compressible_Rhs[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][longest] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][longest];
  
               indr_locdim_Result[nd][longest] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][longest] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][longest] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][longest] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][longest];
  
               indr_locdim_Rhs[nd][longest] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][longest] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][longest] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][second] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][second];
  
               indr_locdim_Result[nd][second] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][second] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][second] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][second] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][second];
  
               indr_locdim_Rhs[nd][second] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][second] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][second] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Result[nd][0] = indr_locstride_Result[nd][0];
#if 1
          if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
	       ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Result[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Result[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Result[nd][0] !=0)
               indr_Offsetreset_Result[nd][0] = ((indr_locbound_Result[nd][0] -indr_loclo_Result[nd][0])/indr_locstride_Result[nd][0]) * indr_Sclstride_Result[nd][0];
            else
               indr_Offsetreset_Result[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Result[nd] += indr_loclo_Result[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_locstride_Result[nd][nd2];

#if 1
               if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
                    ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Result[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Result[nd][nd2] !=0)
                    indr_Offsetreset_Result[nd][nd2] = ((indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) / indr_locstride_Result[nd][nd2]) * indr_Sclstride_Result[nd][nd2];
                 else
                    indr_Offsetreset_Result[nd][nd2] = 0;

               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Rhs[nd][0] = indr_locstride_Rhs[nd][0];
#if 1
          if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
	       ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Rhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Rhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Rhs[nd][0] !=0)
               indr_Offsetreset_Rhs[nd][0] = ((indr_locbound_Rhs[nd][0] -indr_loclo_Rhs[nd][0])/indr_locstride_Rhs[nd][0]) * indr_Sclstride_Rhs[nd][0];
            else
               indr_Offsetreset_Rhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_locstride_Rhs[nd][nd2];

#if 1
               if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
                    ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Rhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Rhs[nd][nd2] !=0)
                    indr_Offsetreset_Rhs[nd][nd2] = ((indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) / indr_locstride_Rhs[nd][nd2]) * indr_Sclstride_Rhs[nd][nd2];
                 else
                    indr_Offsetreset_Rhs[nd][nd2] = 0;

               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Result[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Rhs[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Result[i_debug]);
          printf ("\n");

          printf ("Result_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Result_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Result[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Rhs[i_debug]);
          printf ("\n");

          printf ("Rhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Rhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Rhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

   }


void MDI_Indirect_Setup_Lhs_Rhs_Mask
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Rhs_Descriptor,
   int indr_loclo_Rhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Rhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Rhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Rhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Rhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Rhs[MAXDIMS],
   int indr_Sclstride_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Rhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Rhs[MAXDIMS], 
   int Rhs_Local_Map[MAXDIMS],
   int Base_Offset_Rhs[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
/* BUGFIX (20/2/98) this should be an int not an int* 
   int* indr_Offset_Mask[MAXDIMS], */
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Rhs;
     int Maximum_Bound_For_Indirect_Addressing_Rhs;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Rhs
   */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     if (Rhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Rhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Rhs = INT_MIN;

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Rhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Rhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Rhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Rhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Rhs[nd][0] = indr_locsize_Rhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2] / indr_locsize_Rhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Rhs[nd] = (-Rhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] =  0;
                         indr_locbound_Rhs [nd][nd2] = -1;
                         indr_locsize_Rhs  [nd][nd2] =  0;
                         indr_locstride_Rhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Rhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Rhs[nd][nd2] = 0;
                    Base_Offset_Rhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Rhs_Descriptor->Base[nd] == Rhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Rhs[nd] = &(Rhs_Local_Map[nd]);
	            Rhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2]     = 0;
                         indr_locbound_Rhs[nd][nd2]  = 0;
                         indr_locsize_Rhs[nd][nd2]   = 1;
                         indr_locstride_Rhs[nd][nd2] = 0;
                         indr_locdim_Rhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Rhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd] - Rhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Rhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Rhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Rhs > Rhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Rhs < Rhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Rhs[nd] = NULL;

                    indr_loclo_Rhs[nd][0] = Rhs_Descriptor->Base[nd];
                    indr_locbound_Rhs[nd][0] = Rhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Rhs_Descriptor->Stride[nd];
                    indr_locsize_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];
                    indr_locstride_Rhs[nd][0] = Rhs_Descriptor->Stride[nd];
                    indr_locdim_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2] = 0;
                         indr_locbound_Rhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Rhs[nd][nd2] = 1;
                         indr_locdim_Rhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Rhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
 */
                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Rhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Rhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Rhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Rhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Rhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Rhs[nd][0]<indr_loclo_Rhs[nd][0]) && indr_locstride_Rhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Rhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Rhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Rhs[nd][nd2] = FALSE;
               length = indr_locbound_Rhs[nd][nd2] - indr_loclo_Rhs[nd][nd2] + 1;
               if (indr_locstride_Rhs[nd][nd2+1] == 1)
                    indr_compressible_Rhs[nd][nd2] = (indr_locdim_Rhs[nd][nd2] == length && length % indr_locstride_Rhs[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Rhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Rhs <= Maximum_Bound_For_Indirect_Addressing_Rhs)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Rhs,Maximum_Bound_For_Indirect_Addressing_Rhs);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Rhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Rhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Rhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Rhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Rhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Rhs[nd][0]  = indr_locdim_Rhs[nd][0];
                    indr_locbound_Rhs[nd][0] = max_dim_length[0] * indr_locstride_Rhs[nd][0]+indr_loclo_Rhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Rhs[nd][nd2]        = indr_locdim_Rhs[nd][nd2] *indr_locsize_Rhs[nd][nd2-1];
                         indr_loclo_Rhs[nd][nd2]          = 0;
                         indr_locbound_Rhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Rhs[nd][nd2]      = 1;
                         indr_compressible_Rhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
             if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Rhs[nd][nd2] && indr_locdim_Rhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) ) 
                  dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Rhs[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][longest] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][longest];
  
               indr_locdim_Rhs[nd][longest] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][longest] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][longest] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][second] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][second];
  
               indr_locdim_Rhs[nd][second] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][second] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][second] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Rhs[nd][0] = indr_locstride_Rhs[nd][0];
#if 1
          if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
	       ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Rhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Rhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Rhs[nd][0] !=0)
               indr_Offsetreset_Rhs[nd][0] = ((indr_locbound_Rhs[nd][0] -indr_loclo_Rhs[nd][0])/indr_locstride_Rhs[nd][0]) * indr_Sclstride_Rhs[nd][0];
            else
               indr_Offsetreset_Rhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_locstride_Rhs[nd][nd2];

#if 1
               if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
                    ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Rhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Rhs[nd][nd2] !=0)
                    indr_Offsetreset_Rhs[nd][nd2] = ((indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) / indr_locstride_Rhs[nd][nd2]) * indr_Sclstride_Rhs[nd][nd2];
                 else
                    indr_Offsetreset_Rhs[nd][nd2] = 0;

               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Rhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Rhs[i_debug]);
          printf ("\n");

          printf ("Rhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Rhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Rhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

   }


void MDI_Indirect_Setup_Lhs_Result_Mask
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Result_Descriptor,
   int indr_loclo_Result[MAXDIMS][MAXDIMS],
   int indr_locbound_Result[MAXDIMS][MAXDIMS],
   int indr_locstride_Result[MAXDIMS][MAXDIMS], 
   int indr_locsize_Result[MAXDIMS][MAXDIMS],
   int indr_locdim_Result[MAXDIMS][MAXDIMS],
   int indr_compressible_Result[MAXDIMS][MAXDIMS],
   int indr_Offset_Result[MAXDIMS],
   int indr_Sclstride_Result[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Result[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Result[MAXDIMS], 
   int Result_Local_Map[MAXDIMS],
   int Base_Offset_Result[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
/* BUGFIX (20/2/98) this should be an int not an int* 
   int* indr_Offset_Mask[MAXDIMS], */
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Result
   */

     MDI_ASSERT (Result_Descriptor != NULL);

     if (Result_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Result[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Result [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Result  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2] * Result_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Result[nd][0] = indr_locsize_Result[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Result[nd][nd2] = indr_locsize_Result[nd][nd2] / indr_locsize_Result[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Result[nd] = (-Result_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] =  0;
                         indr_locbound_Result [nd][nd2] = -1;
                         indr_locsize_Result  [nd][nd2] =  0;
                         indr_locstride_Result[nd][nd2] =  1;
                       }
 
                    indr_locdim_Result[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Result[nd][nd2] = 0;
                    Base_Offset_Result[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Result_Descriptor->Base[nd] == Result_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Result[nd] = &(Result_Local_Map[nd]);
	            Result_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2]     = 0;
                         indr_locbound_Result[nd][nd2]  = 0;
                         indr_locsize_Result[nd][nd2]   = 1;
                         indr_locstride_Result[nd][nd2] = 0;
                         indr_locdim_Result[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Result_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd] - Result_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Result[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Result[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Result > Result_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Result = Result_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Result < Result_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Result = Result_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Result[nd] = NULL;

                    indr_loclo_Result[nd][0] = Result_Descriptor->Base[nd];
                    indr_locbound_Result[nd][0] = Result_Descriptor->Base[nd] + (indr_dimension[0]-1) * Result_Descriptor->Stride[nd];
                    indr_locsize_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];
                    indr_locstride_Result[nd][0] = Result_Descriptor->Stride[nd];
                    indr_locdim_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2] = 0;
                         indr_locbound_Result[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Result[nd][nd2] = 1;
                         indr_locdim_Result[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Result[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
 */
                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Result)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Result;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Result)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Result;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Result pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Result[nd][0]<indr_loclo_Result[nd][0]) && indr_locstride_Result[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Result[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Result[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Result[nd][nd2] = FALSE;
               length = indr_locbound_Result[nd][nd2] - indr_loclo_Result[nd][nd2] + 1;
               if (indr_locstride_Result[nd][nd2+1] == 1)
                    indr_compressible_Result[nd][nd2] = (indr_locdim_Result[nd][nd2] == length && length % indr_locstride_Result[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Result pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Result <= Maximum_Bound_For_Indirect_Addressing_Result)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Result,Maximum_Bound_For_Indirect_Addressing_Result);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Result[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Result[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Result[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Result[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Result[nd][0]   = max_dim_length[0];
                    indr_locsize_Result[nd][0]  = indr_locdim_Result[nd][0];
                    indr_locbound_Result[nd][0] = max_dim_length[0] * indr_locstride_Result[nd][0]+indr_loclo_Result[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Result[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Result[nd][nd2]        = indr_locdim_Result[nd][nd2] *indr_locsize_Result[nd][nd2-1];
                         indr_loclo_Result[nd][nd2]          = 0;
                         indr_locbound_Result[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Result[nd][nd2]      = 1;
                         indr_compressible_Result[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
             if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Result[nd][nd2] && indr_locdim_Result[nd][locndim-1]>1) || 
                 (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) ) 
                  dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Result[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][longest] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][longest];
  
               indr_locdim_Result[nd][longest] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][longest] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][longest] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][second] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][second];
  
               indr_locdim_Result[nd][second] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][second] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][second] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Result[nd][0] = indr_locstride_Result[nd][0];
#if 1
          if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
	       ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Result[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Result[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Result[nd][0] !=0)
               indr_Offsetreset_Result[nd][0] = ((indr_locbound_Result[nd][0] -indr_loclo_Result[nd][0])/indr_locstride_Result[nd][0]) * indr_Sclstride_Result[nd][0];
            else
               indr_Offsetreset_Result[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Result[nd] += indr_loclo_Result[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_locstride_Result[nd][nd2];

#if 1
               if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
                    ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Result[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Result[nd][nd2] !=0)
                    indr_Offsetreset_Result[nd][nd2] = ((indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) / indr_locstride_Result[nd][nd2]) * indr_Sclstride_Result[nd][nd2];
                 else
                    indr_Offsetreset_Result[nd][nd2] = 0;

               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Result[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Result[i_debug]);
          printf ("\n");

          printf ("Result_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Result_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Result[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

   }


void MDI_Indirect_Setup_Lhs_Mask_Rhs
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   array_domain* Rhs_Descriptor,
   int indr_loclo_Rhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Rhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Rhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Rhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Rhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Rhs[MAXDIMS][MAXDIMS],
/* BUGFIX (20/2/98) this should be an int not an int* 
   int* indr_Offset_Rhs[MAXDIMS], */
   int indr_Offset_Rhs[MAXDIMS],
   int indr_Sclstride_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Rhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Rhs[MAXDIMS], 
   int Rhs_Local_Map[MAXDIMS],
   int Base_Offset_Rhs[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Rhs;
     int Maximum_Bound_For_Indirect_Addressing_Rhs;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Rhs
   */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     if (Rhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Rhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Rhs = INT_MIN;

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Rhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Rhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Rhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Rhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Rhs[nd][0] = indr_locsize_Rhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2] / indr_locsize_Rhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Rhs[nd] = (-Rhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] =  0;
                         indr_locbound_Rhs [nd][nd2] = -1;
                         indr_locsize_Rhs  [nd][nd2] =  0;
                         indr_locstride_Rhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Rhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Rhs[nd][nd2] = 0;
                    Base_Offset_Rhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Rhs_Descriptor->Base[nd] == Rhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Rhs[nd] = &(Rhs_Local_Map[nd]);
	            Rhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2]     = 0;
                         indr_locbound_Rhs[nd][nd2]  = 0;
                         indr_locsize_Rhs[nd][nd2]   = 1;
                         indr_locstride_Rhs[nd][nd2] = 0;
                         indr_locdim_Rhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Rhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd] - Rhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Rhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Rhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Rhs > Rhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Rhs < Rhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Rhs[nd] = NULL;

                    indr_loclo_Rhs[nd][0] = Rhs_Descriptor->Base[nd];
                    indr_locbound_Rhs[nd][0] = Rhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Rhs_Descriptor->Stride[nd];
                    indr_locsize_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];
                    indr_locstride_Rhs[nd][0] = Rhs_Descriptor->Stride[nd];
                    indr_locdim_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2] = 0;
                         indr_locbound_Rhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Rhs[nd][nd2] = 1;
                         indr_locdim_Rhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Rhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
 */
                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Rhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Rhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Rhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Rhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Rhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Rhs[nd][0]<indr_loclo_Rhs[nd][0]) && indr_locstride_Rhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Rhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Rhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Rhs[nd][nd2] = FALSE;
               length = indr_locbound_Rhs[nd][nd2] - indr_loclo_Rhs[nd][nd2] + 1;
               if (indr_locstride_Rhs[nd][nd2+1] == 1)
                    indr_compressible_Rhs[nd][nd2] = (indr_locdim_Rhs[nd][nd2] == length && length % indr_locstride_Rhs[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Rhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Rhs <= Maximum_Bound_For_Indirect_Addressing_Rhs)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Rhs,Maximum_Bound_For_Indirect_Addressing_Rhs);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Rhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Rhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Rhs[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Rhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Rhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Rhs[nd][0]  = indr_locdim_Rhs[nd][0];
                    indr_locbound_Rhs[nd][0] = max_dim_length[0] * indr_locstride_Rhs[nd][0]+indr_loclo_Rhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Rhs[nd][nd2]        = indr_locdim_Rhs[nd][nd2] *indr_locsize_Rhs[nd][nd2-1];
                         indr_loclo_Rhs[nd][nd2]          = 0;
                         indr_locbound_Rhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Rhs[nd][nd2]      = 1;
                         indr_compressible_Rhs[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
             if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) || 
                 (!indr_compressible_Rhs[nd][nd2] && indr_locdim_Rhs[nd][locndim-1]>1) ) 
                  dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                    indr_compressible_Rhs[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][longest] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][longest];
  
               indr_locdim_Rhs[nd][longest] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][longest] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][longest] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][second] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][second];
  
               indr_locdim_Rhs[nd][second] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][second] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][second] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Rhs[nd][0] = indr_locstride_Rhs[nd][0];
#if 1
          if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
	       ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Rhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Rhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Rhs[nd][0] !=0)
               indr_Offsetreset_Rhs[nd][0] = ((indr_locbound_Rhs[nd][0] -indr_loclo_Rhs[nd][0])/indr_locstride_Rhs[nd][0]) * indr_Sclstride_Rhs[nd][0];
            else
               indr_Offsetreset_Rhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_locstride_Rhs[nd][nd2];

#if 1
               if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
                    ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Rhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Rhs[nd][nd2] !=0)
                    indr_Offsetreset_Rhs[nd][nd2] = ((indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) / indr_locstride_Rhs[nd][nd2]) * indr_Sclstride_Rhs[nd][nd2];
                 else
                    indr_Offsetreset_Rhs[nd][nd2] = 0;

               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Rhs[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Rhs[i_debug]);
          printf ("\n");

          printf ("Rhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Rhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Rhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

   }


void MDI_Indirect_Setup_Lhs_Mask_Result
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   array_domain* Result_Descriptor,
   int indr_loclo_Result[MAXDIMS][MAXDIMS],
   int indr_locbound_Result[MAXDIMS][MAXDIMS],
   int indr_locstride_Result[MAXDIMS][MAXDIMS], 
   int indr_locsize_Result[MAXDIMS][MAXDIMS],
   int indr_locdim_Result[MAXDIMS][MAXDIMS],
   int indr_compressible_Result[MAXDIMS][MAXDIMS],
/* BUGFIX (20/2/98) this should be an int not an int* 
   int* indr_Offset_Result[MAXDIMS], */
   int indr_Offset_Result[MAXDIMS],
   int indr_Sclstride_Result[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Result[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Result[MAXDIMS], 
   int Result_Local_Map[MAXDIMS],
   int Base_Offset_Result[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */

     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Result
   */

     MDI_ASSERT (Result_Descriptor != NULL);

     if (Result_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Result[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Result [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Result  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2] * Result_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Result[nd][0] = indr_locsize_Result[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Result[nd][nd2] = indr_locsize_Result[nd][nd2] / indr_locsize_Result[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Result[nd] = (-Result_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] =  0;
                         indr_locbound_Result [nd][nd2] = -1;
                         indr_locsize_Result  [nd][nd2] =  0;
                         indr_locstride_Result[nd][nd2] =  1;
                       }
 
                    indr_locdim_Result[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Result[nd][nd2] = 0;
                    Base_Offset_Result[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Result_Descriptor->Base[nd] == Result_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Result[nd] = &(Result_Local_Map[nd]);
	            Result_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2]     = 0;
                         indr_locbound_Result[nd][nd2]  = 0;
                         indr_locsize_Result[nd][nd2]   = 1;
                         indr_locstride_Result[nd][nd2] = 0;
                         indr_locdim_Result[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Result_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd] - Result_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Result[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Result[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Result > Result_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Result = Result_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Result < Result_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Result = Result_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Result[nd] = NULL;

                    indr_loclo_Result[nd][0] = Result_Descriptor->Base[nd];
                    indr_locbound_Result[nd][0] = Result_Descriptor->Base[nd] + (indr_dimension[0]-1) * Result_Descriptor->Stride[nd];
                    indr_locsize_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];
                    indr_locstride_Result[nd][0] = Result_Descriptor->Stride[nd];
                    indr_locdim_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2] = 0;
                         indr_locbound_Result[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Result[nd][nd2] = 1;
                         indr_locdim_Result[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Result[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
 */
                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Result)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Result;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Result)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Result;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Result pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Result[nd][0]<indr_loclo_Result[nd][0]) && indr_locstride_Result[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Result[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Result[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Result[nd][nd2] = FALSE;
               length = indr_locbound_Result[nd][nd2] - indr_loclo_Result[nd][nd2] + 1;
               if (indr_locstride_Result[nd][nd2+1] == 1)
                    indr_compressible_Result[nd][nd2] = (indr_locdim_Result[nd][nd2] == length && length % indr_locstride_Result[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Result pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Result <= Maximum_Bound_For_Indirect_Addressing_Result)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Result,Maximum_Bound_For_Indirect_Addressing_Result);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Result[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Result[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Result[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Result[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Result[nd][0]   = max_dim_length[0];
                    indr_locsize_Result[nd][0]  = indr_locdim_Result[nd][0];
                    indr_locbound_Result[nd][0] = max_dim_length[0] * indr_locstride_Result[nd][0]+indr_loclo_Result[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Result[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Result[nd][nd2]        = indr_locdim_Result[nd][nd2] *indr_locsize_Result[nd][nd2-1];
                         indr_loclo_Result[nd][nd2]          = 0;
                         indr_locbound_Result[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Result[nd][nd2]      = 1;
                         indr_compressible_Result[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2;nd2>=0;nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0;nd<MAXDIMS;nd++)
             if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                 (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) || 
                 (!indr_compressible_Result[nd][nd2] && indr_locdim_Result[nd][locndim-1]>1) ) 
                  dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                    indr_compressible_Result[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][longest] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][longest];
  
               indr_locdim_Result[nd][longest] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][longest] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][longest] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][second] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][second];
  
               indr_locdim_Result[nd][second] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][second] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][second] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;

               indr_Sclstride_Result[nd][1]=0;
               indr_Offsetreset_Result[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Result[nd][0] = indr_locstride_Result[nd][0];
#if 1
          if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
	       ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Result[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Result[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Result[nd][0] !=0)
               indr_Offsetreset_Result[nd][0] = ((indr_locbound_Result[nd][0] -indr_loclo_Result[nd][0])/indr_locstride_Result[nd][0]) * indr_Sclstride_Result[nd][0];
            else
               indr_Offsetreset_Result[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Result[nd] += indr_loclo_Result[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_locstride_Result[nd][nd2];

#if 1
               if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
                    ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Result[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Result[nd][nd2] !=0)
                    indr_Offsetreset_Result[nd][nd2] = ((indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) / indr_locstride_Result[nd][nd2]) * indr_Sclstride_Result[nd][nd2];
                 else
                    indr_Offsetreset_Result[nd][nd2] = 0;

               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Result[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Result[i_debug]);
          printf ("\n");

          printf ("Result_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Result_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Result[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

   }



void MDI_Indirect_Setup_Lhs_Result_Rhs_Mask
  (array_domain* Lhs_Descriptor,
   int indr_loclo_Lhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Lhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Lhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Lhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Lhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Lhs[MAXDIMS],
   int indr_Sclstride_Lhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Lhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Lhs[MAXDIMS], 
   int Lhs_Local_Map[MAXDIMS],
   int Base_Offset_Lhs[MAXDIMS],
   array_domain* Result_Descriptor,
   int indr_loclo_Result[MAXDIMS][MAXDIMS],
   int indr_locbound_Result[MAXDIMS][MAXDIMS],
   int indr_locstride_Result[MAXDIMS][MAXDIMS], 
   int indr_locsize_Result[MAXDIMS][MAXDIMS],
   int indr_locdim_Result[MAXDIMS][MAXDIMS],
   int indr_compressible_Result[MAXDIMS][MAXDIMS],
   int indr_Offset_Result[MAXDIMS],
   int indr_Sclstride_Result[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Result[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Result[MAXDIMS], 
   int Result_Local_Map[MAXDIMS],
   int Base_Offset_Result[MAXDIMS],
   array_domain* Rhs_Descriptor,
   int indr_loclo_Rhs[MAXDIMS][MAXDIMS],
   int indr_locbound_Rhs[MAXDIMS][MAXDIMS],
   int indr_locstride_Rhs[MAXDIMS][MAXDIMS], 
   int indr_locsize_Rhs[MAXDIMS][MAXDIMS],
   int indr_locdim_Rhs[MAXDIMS][MAXDIMS],
   int indr_compressible_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offset_Rhs[MAXDIMS],
   int indr_Sclstride_Rhs[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Rhs[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Rhs[MAXDIMS], 
   int Rhs_Local_Map[MAXDIMS],
   int Base_Offset_Rhs[MAXDIMS],
   array_domain* Mask_Descriptor,
   int indr_loclo_Mask[MAXDIMS][MAXDIMS],
   int indr_locbound_Mask[MAXDIMS][MAXDIMS],
   int indr_locstride_Mask[MAXDIMS][MAXDIMS], 
   int indr_locsize_Mask[MAXDIMS][MAXDIMS],
   int indr_locdim_Mask[MAXDIMS][MAXDIMS],
   int indr_compressible_Mask[MAXDIMS][MAXDIMS],
   int indr_Offset_Mask[MAXDIMS],
   int indr_Sclstride_Mask[MAXDIMS][MAXDIMS],
   int indr_Offsetreset_Mask[MAXDIMS][MAXDIMS],
   int* index_data_ptr_Mask[MAXDIMS], 
   int Mask_Local_Map[MAXDIMS],
   int Base_Offset_Mask[MAXDIMS],
   int* Dimorder, int* ICounter,
   int* longestptr, int* secondptr, int* locndimptr, int* indirdimptr)
   {
/*------------------------------------------------------------------*/
     int* desc_ptr;
     int nd; int nd2;
     int length;
     int dim_not_compressible;
     int longlen; int secondlen;
     int numcompress; int notcompress;
     int longest; int second; int locndim;
     int_array* index_ptr;
     array_domain* index_desc_ptr;
     int Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     int Maximum_Bound_For_Indirect_Addressing = INT_MIN;
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;
     int Minimum_Base_For_Indirect_Addressing_Rhs;
     int Maximum_Bound_For_Indirect_Addressing_Rhs;
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;
     int Size_Index[MAXDIMS];
     int locndimtemp;
     int indirdim;
     int max_dim_length[MAXDIMS];
     int indr_dimension[MAXDIMS]; int indr_dim;

     int i_debug; int j_debug;

/*------------------------------------------------------------------*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed 
       (call Lhs version last because some variables are reset and 
       the Lhs version should be used)... */
     indr_dim = 0;
     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Result
   */

     MDI_ASSERT (Result_Descriptor != NULL);

     if (Result_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Rhs
   */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     if (Rhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Mask
   */

     MDI_ASSERT (Mask_Descriptor != NULL);

     if (Mask_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }

     /*------------------------------------------------------------------*/
  /* ... set up indirect dimension sizes so an Index can be set up to
      look like a multidimensional intArray for conformability in indirect addressing ... 
      Here we search for the highest dimention in which indirect addressing is used for Lhs
   */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     if (Lhs_Descriptor->Uses_Indirect_Addressing)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

               if (index_ptr)
                  {
                 /* only need this pointer right below */
                    index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

                    if (index_desc_ptr->Descriptor_Dimension > indr_dim)
                       {
                      /* this assumes that all intArrays of equal dimensions
                         are conformable so the sizes only need to be set
                         for the highest dimensional intArray */

                         indr_dim = index_desc_ptr->Descriptor_Dimension;

                      /* The Index array should be 1 dimensional (this could still mean */
                      /* that it is a 1D view of a higher dimensional array object)!    */
                      /* This assertion disabled something that used to work (see test2000_14.C) */
                      /* MDI_ASSERT (indr_dim == 1); */

                         for (nd2=0;nd2<MAXDIMS;nd2++)
                            {
                              indr_dimension[nd2] = ((index_desc_ptr->Bound[nd2] - index_desc_ptr->Base[nd2]) / index_desc_ptr->Stride[nd2]) + 1;
                            }
                       }
                      else
                       {
                      /* Case for Null Array as index object (silly case but must be handled) */
                         if (index_desc_ptr->Descriptor_Dimension == 0)
                            {
	                   /* local null array case */
                              for (nd2=0;nd2<MAXDIMS;nd2++)
                                   indr_dimension[nd2] = 1;
                            }
                       }
                  }
             }
        }



     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Result_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Result[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Result [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Result  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Result[nd][nd2] = index_desc_ptr->Stride[nd2] * Result_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Result[nd][0] = indr_locsize_Result[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Result[nd][nd2] = indr_locsize_Result[nd][nd2] / indr_locsize_Result[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Result[nd] = (-Result_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result    [nd][nd2] =  0;
                         indr_locbound_Result [nd][nd2] = -1;
                         indr_locsize_Result  [nd][nd2] =  0;
                         indr_locstride_Result[nd][nd2] =  1;
                       }
 
                    indr_locdim_Result[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Result[nd][nd2] = 0;
                    Base_Offset_Result[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Result_Descriptor->Base[nd] == Result_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Result[nd] = &(Result_Local_Map[nd]);
	            Result_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2]     = 0;
                         indr_locbound_Result[nd][nd2]  = 0;
                         indr_locsize_Result[nd][nd2]   = 1;
                         indr_locstride_Result[nd][nd2] = 0;
                         indr_locdim_Result[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Result_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd] - Result_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Result[nd] = Result_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Result[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Result[nd] = -Result_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Result[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Result > Result_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Result = Result_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Result < Result_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Result = Result_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Result[nd] = NULL;

                    indr_loclo_Result[nd][0] = Result_Descriptor->Base[nd];
                    indr_locbound_Result[nd][0] = Result_Descriptor->Base[nd] + (indr_dimension[0]-1) * Result_Descriptor->Stride[nd];
                    indr_locsize_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];
                    indr_locstride_Result[nd][0] = Result_Descriptor->Stride[nd];
                    indr_locdim_Result[nd][0] = indr_dimension[0] * Result_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Result[nd][nd2] = 0;
                         indr_locbound_Result[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Result[nd][nd2] = 1;
                         indr_locdim_Result[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Result[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
 */
                    Base_Offset_Result[nd] = -Result_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Result)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Result;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Result)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Result;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Result pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Result[nd][0]<indr_loclo_Result[nd][0]) && indr_locstride_Result[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Result[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Result[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Result[nd][nd2] = FALSE;
               length = indr_locbound_Result[nd][nd2] - indr_loclo_Result[nd][nd2] + 1;
               if (indr_locstride_Result[nd][nd2+1] == 1)
                    indr_compressible_Result[nd][nd2] = (indr_locdim_Result[nd][nd2] == length && length % indr_locstride_Result[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Rhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Rhs = INT_MIN;

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Rhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Rhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Rhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Rhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Rhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Rhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Rhs[nd][0] = indr_locsize_Rhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2] / indr_locsize_Rhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Rhs[nd] = (-Rhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs    [nd][nd2] =  0;
                         indr_locbound_Rhs [nd][nd2] = -1;
                         indr_locsize_Rhs  [nd][nd2] =  0;
                         indr_locstride_Rhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Rhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Rhs[nd][nd2] = 0;
                    Base_Offset_Rhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Rhs_Descriptor->Base[nd] == Rhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Rhs[nd] = &(Rhs_Local_Map[nd]);
	            Rhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2]     = 0;
                         indr_locbound_Rhs[nd][nd2]  = 0;
                         indr_locsize_Rhs[nd][nd2]   = 1;
                         indr_locstride_Rhs[nd][nd2] = 0;
                         indr_locdim_Rhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Rhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd] - Rhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Rhs[nd] = Rhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Rhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Rhs[nd] = -Rhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Rhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Rhs > Rhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Rhs < Rhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Rhs = Rhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Rhs[nd] = NULL;

                    indr_loclo_Rhs[nd][0] = Rhs_Descriptor->Base[nd];
                    indr_locbound_Rhs[nd][0] = Rhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Rhs_Descriptor->Stride[nd];
                    indr_locsize_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];
                    indr_locstride_Rhs[nd][0] = Rhs_Descriptor->Stride[nd];
                    indr_locdim_Rhs[nd][0] = indr_dimension[0] * Rhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Rhs[nd][nd2] = 0;
                         indr_locbound_Rhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Rhs[nd][nd2] = 1;
                         indr_locdim_Rhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Rhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
 */
                    Base_Offset_Rhs[nd] = -Rhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Rhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Rhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Rhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Rhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Rhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Rhs[nd][0]<indr_loclo_Rhs[nd][0]) && indr_locstride_Rhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Rhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Rhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Rhs[nd][nd2] = FALSE;
               length = indr_locbound_Rhs[nd][nd2] - indr_loclo_Rhs[nd][nd2] + 1;
               if (indr_locstride_Rhs[nd][nd2+1] == 1)
                    indr_compressible_Rhs[nd][nd2] = (indr_locdim_Rhs[nd][nd2] == length && length % indr_locstride_Rhs[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Mask_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Mask[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Mask [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Mask  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Mask[nd][nd2] = index_desc_ptr->Stride[nd2] * Mask_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Mask[nd][0] = indr_locsize_Mask[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2] / indr_locsize_Mask[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Mask[nd] = (-Mask_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask    [nd][nd2] =  0;
                         indr_locbound_Mask [nd][nd2] = -1;
                         indr_locsize_Mask  [nd][nd2] =  0;
                         indr_locstride_Mask[nd][nd2] =  1;
                       }
 
                    indr_locdim_Mask[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Mask[nd][nd2] = 0;
                    Base_Offset_Mask[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Mask_Descriptor->Base[nd] == Mask_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Mask[nd] = &(Mask_Local_Map[nd]);
	            Mask_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2]     = 0;
                         indr_locbound_Mask[nd][nd2]  = 0;
                         indr_locsize_Mask[nd][nd2]   = 1;
                         indr_locstride_Mask[nd][nd2] = 0;
                         indr_locdim_Mask[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Mask_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd] - Mask_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Mask[nd] = Mask_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Mask[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Mask[nd] = -Mask_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Mask[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Mask > Mask_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Mask = Mask_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Mask < Mask_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Mask = Mask_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Mask[nd] = NULL;

                    indr_loclo_Mask[nd][0] = Mask_Descriptor->Base[nd];
                    indr_locbound_Mask[nd][0] = Mask_Descriptor->Base[nd] + (indr_dimension[0]-1) * Mask_Descriptor->Stride[nd];
                    indr_locsize_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];
                    indr_locstride_Mask[nd][0] = Mask_Descriptor->Stride[nd];
                    indr_locdim_Mask[nd][0] = indr_dimension[0] * Mask_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Mask[nd][nd2] = 0;
                         indr_locbound_Mask[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Mask[nd][nd2] = 1;
                         indr_locdim_Mask[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Mask[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
 */
                    Base_Offset_Mask[nd] = -Mask_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Mask)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Mask;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Mask)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Mask;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Mask pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Mask[nd][0]<indr_loclo_Mask[nd][0]) && indr_locstride_Mask[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Mask[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Mask[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Mask[nd][nd2] = FALSE;
               length = indr_locbound_Mask[nd][nd2] - indr_loclo_Mask[nd][nd2] + 1;
               if (indr_locstride_Mask[nd][nd2+1] == 1)
                    indr_compressible_Mask[nd][nd2] = (indr_locdim_Mask[nd][nd2] == length && length % indr_locstride_Mask[nd][nd2] == 0);
             }
        }


     /*------------------------------------------------------------------*/
  /* ... set up local array bounds and strides for indirect addressing ... */

     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          index_ptr = (int_array*)Lhs_Descriptor->Index_Array[nd];

          if (index_ptr)
             {
            /* need to store this for each dimension to use later */
               index_data_ptr_Lhs[nd] = index_ptr->Array_Descriptor.Array_Data;

            /* only need this pointer right below */
               index_desc_ptr = &(index_ptr->Array_Descriptor.Array_Domain);

               if (!index_desc_ptr->Is_A_Null_Array)
                  {
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] = index_desc_ptr->Base[nd2];
                         indr_locbound_Lhs [nd][nd2] = index_desc_ptr->Bound[nd2];
                         indr_locsize_Lhs  [nd][nd2] = index_desc_ptr->Size[nd2];
                         indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2];
                      /* indr_locstride_Lhs[nd][nd2] = index_desc_ptr->Stride[nd2] * Lhs_Descriptor->Stride[nd]; */
                       }

                    indr_locdim_Lhs[nd][0] = indr_locsize_Lhs[nd][0];
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2] / indr_locsize_Lhs[nd][nd2-1];
                       }
                 /* ... NOTE: this assumes that base is set to 0 in dimension with intArray ... */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
                 /* Build the offset to reference the zero position along each axis.
                    Then the value of the i n d e x intArray elements are added to
                    provide the correct i n d e x into the array object.
                  */
                 /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                 /* Include the data base to move the pointer to the zero reference point.
                    Then the value of the intArray elements are used to moe the i n d e x to the correct position.
                  */
                    Base_Offset_Lhs[nd] = (-Lhs_Descriptor->Data_Base[nd]);
                  }
                 else
                  {
                 /* local i n d e x array is null even though parallel i n d e x array isn't null  */
                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs    [nd][nd2] =  0;
                         indr_locbound_Lhs [nd][nd2] = -1;
                         indr_locsize_Lhs  [nd][nd2] =  0;
                         indr_locstride_Lhs[nd][nd2] =  1;
                       }
 
                    indr_locdim_Lhs[nd][0] = 0;
                    for (nd2=1;nd2<MAXDIMS;nd2++)
                         indr_locdim_Lhs[nd][nd2] = 0;
                    Base_Offset_Lhs[nd] = 0; 
                  }
             }
            else
             {
            /* ... fixed constant case */
            /* ...(bug fix, 7/22/96, kdb) since this is indirect addressing, 
	       dimension 0 can't be a fixed constant but could have length 1 
	       so force into section below ...
             */
               if ( (Lhs_Descriptor->Base[nd] == Lhs_Descriptor->Bound[nd]) && (nd > 0) )
                  {
                 /* index_data_ptr is pointer to the index data array, in
	            this case the data array will be length 1 and have it's only value be 0 
	          */
                    index_data_ptr_Lhs[nd] = &(Lhs_Local_Map[nd]);
	            Lhs_Local_Map[nd] = 0;

                    for (nd2=0;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2]     = 0;
                         indr_locbound_Lhs[nd][nd2]  = 0;
                         indr_locsize_Lhs[nd][nd2]   = 1;
                         indr_locstride_Lhs[nd][nd2] = 0;
                         indr_locdim_Lhs[nd][nd2]    = 1;
                       }
#if 0
                 /* ... currently the base is set differently for an array
                    with indirect addressing than without, this is a temporary
                    fix to account for that ... */
          
                 /* ... temporary fix no longer needed */
                 /* if (Lhs_Descriptor->Uses_Indirect_Addressing)
                       {
                  */
                      /* ... base is set to constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd] - Lhs_Descriptor->Data_Base[nd];
                       }
                      else
                       {
                       */
                      /* ... base already has data_base subtracted from constant value ... */
                      /* Base_Offset_Lhs[nd] = Lhs_Descriptor->Base[nd]; */
                      /* ... (3/24/98,kdb) i n d e x_data_ptr was just set to 0
                         above so don't need to subtract anything ... */
                         Base_Offset_Lhs[nd] = 0;
                      /* ... (3/13/98,kdb) this is fixed in address calculation now ... */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->User_Base[nd]; */
                      /* Base_Offset_Lhs[nd] = -Lhs_Descriptor->Data_Base[nd]; */
	              /*
	               }
                       */
#else
                    Base_Offset_Lhs[nd] = 0;
#endif
                  }
                 else
                  {
                 /* ... index case ... */
                    if (Minimum_Base_For_Indirect_Addressing_Lhs > Lhs_Descriptor->Base[nd])
	                 Minimum_Base_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Base[nd];

                    if (Maximum_Bound_For_Indirect_Addressing_Lhs < Lhs_Descriptor->Bound[nd])
	                 Maximum_Bound_For_Indirect_Addressing_Lhs = Lhs_Descriptor->Bound[nd];

                 /* ... set this up to look like the highest dimensioned intArray used for indirect addressing ... */

                    index_data_ptr_Lhs[nd] = NULL;

                    indr_loclo_Lhs[nd][0] = Lhs_Descriptor->Base[nd];
                    indr_locbound_Lhs[nd][0] = Lhs_Descriptor->Base[nd] + (indr_dimension[0]-1) * Lhs_Descriptor->Stride[nd];
                    indr_locsize_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];
                    indr_locstride_Lhs[nd][0] = Lhs_Descriptor->Stride[nd];
                    indr_locdim_Lhs[nd][0] = indr_dimension[0] * Lhs_Descriptor->Stride[nd];

                    for (nd2=1;nd2<MAXDIMS;nd2++)
                       {
                         indr_loclo_Lhs[nd][nd2] = 0;
                         indr_locbound_Lhs[nd][nd2] = indr_dimension[nd2]-1;
	              /* this may need to be fixed for vectorization later */
                         indr_locsize_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_dimension[nd2];
                         indr_locstride_Lhs[nd][nd2] = 1;
                         indr_locdim_Lhs[nd][nd2] = indr_dimension[nd2];
                       }

                 /* ... index_data_ptr will already be correctly offset ...  */
                 /* Base_Offset_Lhs[nd] = 0; */
                 /* ... code has changed and now Base is added so it must be subtracted here ... */
/*                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
 */
                    Base_Offset_Lhs[nd] = -Lhs_Descriptor->Base[nd];
                  }
             }
        }

  /* First find the maximum of the different min bases and max bounds so that we can call
     the MDI_Get_Default_Index_Map_Data with the min and max over all operands first.
     This will prevent the the reallocation of memory in the MDI_Get_Default_Index_Map_Data
     which could force previously used space to be used (causing a purify Free Memory Read (FMR)).
   */

     if (Minimum_Base_For_Indirect_Addressing > Minimum_Base_For_Indirect_Addressing_Lhs)
          Minimum_Base_For_Indirect_Addressing = Minimum_Base_For_Indirect_Addressing_Lhs;

     if (Maximum_Bound_For_Indirect_Addressing < Maximum_Bound_For_Indirect_Addressing_Lhs)
          Maximum_Bound_For_Indirect_Addressing = Maximum_Bound_For_Indirect_Addressing_Lhs;

/*
     Call redundently for each operand so that the max version will be 
     defined and called before initialization of the index_data_ptr_Lhs pointers.
 */

  /* Bugfix (9/22/2000)
     Handle case of Min base == MAX_INT and Max bound == -MAX_INT
     which are problem inputs for MDI_Get_Default_Index_Map_Data */
     if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
          MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);

/*
     Part of bugfix (8/16/2000) to avoid calling MDI_Get_Default_Index_Map_Data 
     in a way that would force it to free memory only the first time it is called.

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
               Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data
               if (Minimum_Base_For_Indirect_Addressing <= Maximum_Bound_For_Indirect_Addressing)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing,Maximum_Bound_For_Indirect_Addressing);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }
*/

  /* ... all of the index arrays should have the same number of dimensions ... */
  /* temp fix ... */

     locndim = 0;
     indirdim = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          locndimtemp = MAXDIMS;
          nd2 = MAXDIMS-1;
          while (nd2>=0 && (indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) == 0) 
             {
               nd2--;
               locndimtemp--;
             }
          if (locndimtemp > locndim) 
             {
               locndim = locndimtemp;
               indirdim = nd;
             }
        }
  /* a 1 x 1 x ... x 1 array will incorrectly produce locndim = 0 ...  */
     if (locndim == 0)
          locndim = 1;

  /* i n d e x array might be null but domain not reset */
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if ((indr_locbound_Lhs[nd][0]<indr_loclo_Lhs[nd][0]) && indr_locstride_Lhs[nd][0] == 1)
             locndim = 1;
        }

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Offset_Lhs[nd] = 0;
          for (nd2=locndim;nd2<MAXDIMS;nd2++) 
             {
               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1]; 
             }
        }

  /* ... NOTE: the following could be stored in the descriptor ... */

     for (nd=0;nd<MAXDIMS;nd++)
        {
       /* ... bug fix because higher dims not set (10/27/95) ... */
          for(nd2=locndim-1;nd2<MAXDIMS;nd2++)
               indr_compressible_Lhs[nd][nd2] = FALSE;

          for(nd2=locndim-2;nd2>=0;nd2--)
             {
               indr_compressible_Lhs[nd][nd2] = FALSE;
               length = indr_locbound_Lhs[nd][nd2] - indr_loclo_Lhs[nd][nd2] + 1;
               if (indr_locstride_Lhs[nd][nd2+1] == 1)
                    indr_compressible_Lhs[nd][nd2] = (indr_locdim_Lhs[nd][nd2] == length && length % indr_locstride_Lhs[nd][nd2] == 0);
             }
        }



  /* bugfix (8/16/2000) avoid calling realloc which frees memory already pointed to! */
     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Result pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Result[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Result <= Maximum_Bound_For_Indirect_Addressing_Result)
                    index_data_ptr_Result[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Result,Maximum_Bound_For_Indirect_Addressing_Result);
                 else
                    index_data_ptr_Result[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Rhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Rhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Rhs <= Maximum_Bound_For_Indirect_Addressing_Rhs)
                    index_data_ptr_Rhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Rhs,Maximum_Bound_For_Indirect_Addressing_Rhs);
                 else
                    index_data_ptr_Rhs[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Mask pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Mask[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Mask <= Maximum_Bound_For_Indirect_Addressing_Mask)
                    index_data_ptr_Mask[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Mask,Maximum_Bound_For_Indirect_Addressing_Mask);
                 else
                    index_data_ptr_Mask[nd] = NULL;
             }
        }

     /* ---------------------------------------------- */
  /* Setup the index_data_ptr_Lhs pointer */

  /* Then call the MDI_Get_Default_Index_Map_Data() function only once with a min max pair
     so that there is only one opportunity for the allocated space to be redimentioned 
   */

     for (nd=0; nd < MAXDIMS; nd++)
        {
          if (index_data_ptr_Lhs[nd] == NULL)
             {
            /* Bugfix (9/22/2000)
               Handle case of Min base == MAX_INT and Max bound == -MAX_INT
               which are problem inputs for MDI_Get_Default_Index_Map_Data */
               if (Minimum_Base_For_Indirect_Addressing_Lhs <= Maximum_Bound_For_Indirect_Addressing_Lhs)
                    index_data_ptr_Lhs[nd] = MDI_Get_Default_Index_Map_Data (Minimum_Base_For_Indirect_Addressing_Lhs,Maximum_Bound_For_Indirect_Addressing_Lhs);
                 else
                    index_data_ptr_Lhs[nd] = NULL;
             }
        }


#ifndef NODIMCOMPRESS

  /* ... convert 1d arrays to be conformable with multidimensional
      arrays if any left after compression ... */

     /*------------------------------------------------------------------*/

  /* ... if some intArray is multidimensional, turn all
      1d intArrays or indices into multi dimensions by
      modifying local bases and bounds ... */

     if (locndim >1)
        {
          for(nd=0;nd<MAXDIMS;nd++) max_dim_length[nd] = 1;

       /* find max dimension lengths */ 
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if (indr_locdim_Lhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Lhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Lhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Result[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Result[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Result[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Rhs[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Rhs[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Rhs[nd][nd2];
                    break;
                  }
	       

               /*------------------------------------------------------------------*/

               if (indr_locdim_Mask[nd][locndim-1] >1)
                  {
                    max_dim_length[locndim-1] = indr_locdim_Mask[nd][locndim-1];
                    for(nd2=0;nd2<locndim-1;nd2++) 
	                 max_dim_length[nd2] = indr_locdim_Mask[nd][nd2];
                    break;
                  }
	       

             }

       /* ... now modify 1d arrays ... */
          for(nd=0;nd<MAXDIMS;nd++) 
             {
               /*------------------------------------------------------------------*/

               if(indr_locdim_Lhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Lhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Lhs[nd][0]  = indr_locdim_Lhs[nd][0];
                    indr_locbound_Lhs[nd][0] = max_dim_length[0] * indr_locstride_Lhs[nd][0]+indr_loclo_Lhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Lhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Lhs[nd][nd2]        = indr_locdim_Lhs[nd][nd2] *indr_locsize_Lhs[nd][nd2-1];
                         indr_loclo_Lhs[nd][nd2]          = 0;
                         indr_locbound_Lhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Lhs[nd][nd2]      = 1;
                         indr_compressible_Lhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Result[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Result[nd][0]   = max_dim_length[0];
                    indr_locsize_Result[nd][0]  = indr_locdim_Result[nd][0];
                    indr_locbound_Result[nd][0] = max_dim_length[0] * indr_locstride_Result[nd][0]+indr_loclo_Result[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Result[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Result[nd][nd2]        = indr_locdim_Result[nd][nd2] *indr_locsize_Result[nd][nd2-1];
                         indr_loclo_Result[nd][nd2]          = 0;
                         indr_locbound_Result[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Result[nd][nd2]      = 1;
                         indr_compressible_Result[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Rhs[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Rhs[nd][0]   = max_dim_length[0];
                    indr_locsize_Rhs[nd][0]  = indr_locdim_Rhs[nd][0];
                    indr_locbound_Rhs[nd][0] = max_dim_length[0] * indr_locstride_Rhs[nd][0]+indr_loclo_Rhs[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Rhs[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Rhs[nd][nd2]        = indr_locdim_Rhs[nd][nd2] *indr_locsize_Rhs[nd][nd2-1];
                         indr_loclo_Rhs[nd][nd2]          = 0;
                         indr_locbound_Rhs[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Rhs[nd][nd2]      = 1;
                         indr_compressible_Rhs[nd][nd2-1] = 1;
                       }
                  }


               /*------------------------------------------------------------------*/

               if(indr_locdim_Mask[nd][0]>max_dim_length[0])
                  {
                 /* ... base and stride stay the same for dim 0 ... */
                    indr_locdim_Mask[nd][0]   = max_dim_length[0];
                    indr_locsize_Mask[nd][0]  = indr_locdim_Mask[nd][0];
                    indr_locbound_Mask[nd][0] = max_dim_length[0] * indr_locstride_Mask[nd][0]+indr_loclo_Mask[nd][0]-1;

                    for(nd2=1;nd2<locndim;nd2++)
                       {
                         indr_locdim_Mask[nd][nd2]         = max_dim_length[nd2];
                         indr_locsize_Mask[nd][nd2]        = indr_locdim_Mask[nd][nd2] *indr_locsize_Mask[nd][nd2-1];
                         indr_loclo_Mask[nd][nd2]          = 0;
                         indr_locbound_Mask[nd][nd2]       = max_dim_length[nd2]-1;
                         indr_locstride_Mask[nd][nd2]      = 1;
                         indr_compressible_Mask[nd][nd2-1] = 1;
                       }
                  }


             } /* end nd loop */

        } /* end locndim>1 test */




  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

     /*------------------------------------------------------------------*/

  /* ... arrays 0 to MAXDIMS will be conformable after collapsing
       dimensions only if they are all compressible in the 
       same dimension, if not turn off the compressible flag
       so the arrays won't have that dimension collapsed (last dim 
       is never compressible so don't check) ... */

     for(nd2=locndim-2; nd2>=0; nd2--)
        {
          dim_not_compressible = FALSE;

          for(nd=0; nd<MAXDIMS; nd++)
               if ((!indr_compressible_Lhs[nd][nd2] && indr_locdim_Lhs[nd][locndim-1]>1) || 
                   (!indr_compressible_Result[nd][nd2] && indr_locdim_Result[nd][locndim-1]>1) || 
                   (!indr_compressible_Rhs[nd][nd2] && indr_locdim_Rhs[nd][locndim-1]>1) || 
                   (!indr_compressible_Mask[nd][nd2] && indr_locdim_Mask[nd][locndim-1]>1) )
                    dim_not_compressible = TRUE;

          if (dim_not_compressible) 
               for(nd=0;nd<MAXDIMS;nd++) 
                  {
                    indr_compressible_Lhs[nd][nd2] = 0;
                    indr_compressible_Result[nd][nd2] = 0;
                    indr_compressible_Rhs[nd][nd2] = 0;
                    indr_compressible_Mask[nd][nd2] = 0;
                  }
        }




  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     


  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

  /* ... ordering for all indirect addressing arrays should be the same so arbitrarily pick the first ... */
     longest = locndim-1;
     while(indr_compressible_Lhs[indirdim][longest-1]&&longest>0)
          longest--;

     length  = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest] +1;
     length /= indr_locstride_Lhs[indirdim][longest];
     if (length<1)
          length = 1;
     nd = longest;
     while (indr_compressible_Lhs[indirdim][nd++]) 
          length *= (indr_locbound_Lhs[indirdim][nd+1]- indr_loclo_Lhs[indirdim][nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = indr_locbound_Lhs[indirdim][nd] - indr_loclo_Lhs[indirdim][nd] +1;
          length /=indr_locstride_Lhs[indirdim][nd];
          if (length<1)
             length = 1;
          if (!indr_compressible_Lhs[indirdim][nd-1] || nd==0)
             {
               int nd2=nd;
               while (indr_compressible_Lhs[indirdim][nd2++]) 
                    length *= (indr_locbound_Lhs[indirdim][nd2+1] - indr_loclo_Lhs[indirdim][nd2+1]+1);
               if (length>longlen)
                  {
                    second = longest;
                    secondlen = longlen;
                    longlen = length;
                    longest = nd;
                  }
                 else
                    if (length>secondlen)
                       {
                         second = nd;
                         secondlen = length;
                       }
             }
        }

     Dimorder[0] = longest;
     Dimorder[1] = second;

     if (locndim>2)
        {
          int cnt=2;
          for (nd=0;nd<locndim;nd++)
               if (nd != longest && nd != second)
                    Dimorder[cnt++]=nd;
        }




  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][longest] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][longest];
  
               indr_locdim_Lhs[nd][longest] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][longest] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][longest] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][longest] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][longest];
  
               indr_locdim_Result[nd][longest] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][longest] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][longest] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][longest] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][longest];
  
               indr_locdim_Rhs[nd][longest] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][longest] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][longest] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;
     /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][longest])
             {
               nd2 = longest;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=longest+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][longest] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][longest];
  
               indr_locdim_Mask[nd][longest] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][longest] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][longest] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][longest] = 0;

               numcompress = notcompress-longest;

            /* ... effectively remove dimensions that were compressed into longest ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

     locndim -= numcompress;
     if (second >longest)
        second-= numcompress;

     if (second>=0)
        {
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Lhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Lhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Lhs[nd][nd2];
  
               indr_locsize_Lhs[nd][second] *= length * indr_locdim_Lhs[nd][notcompress]; 
  
               length *= indr_locdim_Lhs[nd][second];
  
               indr_locdim_Lhs[nd][second] = length * indr_locdim_Lhs[nd][notcompress];
               indr_loclo_Lhs[nd][second] = length * indr_loclo_Lhs[nd][notcompress];
               indr_locbound_Lhs[nd][second] = length * (indr_locbound_Lhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Lhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Lhs[nd][nd2-numcompress]       = indr_locdim_Lhs[nd][nd2];
                    indr_locsize_Lhs[nd][nd2-numcompress]      = indr_locsize_Lhs[nd][nd2];
                    indr_loclo_Lhs[nd][nd2-numcompress]        = indr_loclo_Lhs[nd][nd2];
                    indr_locbound_Lhs[nd][nd2-numcompress]     = indr_locbound_Lhs[nd][nd2];
                    indr_compressible_Lhs[nd][nd2-numcompress] = indr_compressible_Lhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Result[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Result[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Result[nd][nd2];
  
               indr_locsize_Result[nd][second] *= length * indr_locdim_Result[nd][notcompress]; 
  
               length *= indr_locdim_Result[nd][second];
  
               indr_locdim_Result[nd][second] = length * indr_locdim_Result[nd][notcompress];
               indr_loclo_Result[nd][second] = length * indr_loclo_Result[nd][notcompress];
               indr_locbound_Result[nd][second] = length * (indr_locbound_Result[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Result[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Result[nd][nd2-numcompress]       = indr_locdim_Result[nd][nd2];
                    indr_locsize_Result[nd][nd2-numcompress]      = indr_locsize_Result[nd][nd2];
                    indr_loclo_Result[nd][nd2-numcompress]        = indr_loclo_Result[nd][nd2];
                    indr_locbound_Result[nd][nd2-numcompress]     = indr_locbound_Result[nd][nd2];
                    indr_compressible_Result[nd][nd2-numcompress] = indr_compressible_Result[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Rhs[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Rhs[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Rhs[nd][nd2];
  
               indr_locsize_Rhs[nd][second] *= length * indr_locdim_Rhs[nd][notcompress]; 
  
               length *= indr_locdim_Rhs[nd][second];
  
               indr_locdim_Rhs[nd][second] = length * indr_locdim_Rhs[nd][notcompress];
               indr_loclo_Rhs[nd][second] = length * indr_loclo_Rhs[nd][notcompress];
               indr_locbound_Rhs[nd][second] = length * (indr_locbound_Rhs[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Rhs[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Rhs[nd][nd2-numcompress]       = indr_locdim_Rhs[nd][nd2];
                    indr_locsize_Rhs[nd][nd2-numcompress]      = indr_locsize_Rhs[nd][nd2];
                    indr_loclo_Rhs[nd][nd2-numcompress]        = indr_loclo_Rhs[nd][nd2];
                    indr_locbound_Rhs[nd][nd2-numcompress]     = indr_locbound_Rhs[nd][nd2];
                    indr_compressible_Rhs[nd][nd2-numcompress] = indr_compressible_Rhs[nd][nd2];
                  }
             }
        }

;
          /*------------------------------------------------------------------*/

     numcompress = 0;
     for (nd=0;nd<MAXDIMS;nd++)
        {
          if (indr_compressible_Mask[nd][second])
             {
               nd2 = second;
               while (indr_compressible_Mask[nd][nd2])
                  nd2++;
               notcompress = nd2;
  
               length = 1;
               for (nd2=second+1;nd2<notcompress;nd2++) 
	            length *= indr_locdim_Mask[nd][nd2];
  
               indr_locsize_Mask[nd][second] *= length * indr_locdim_Mask[nd][notcompress]; 
  
               length *= indr_locdim_Mask[nd][second];
  
               indr_locdim_Mask[nd][second] = length * indr_locdim_Mask[nd][notcompress];
               indr_loclo_Mask[nd][second] = length * indr_loclo_Mask[nd][notcompress];
               indr_locbound_Mask[nd][second] = length * (indr_locbound_Mask[nd][notcompress]+1)-1;
  
            /* ... this dim has already been compressed as much as possible now ... */
               indr_compressible_Mask[nd][second] = 0;

               numcompress = notcompress-second;

            /* ... effectively remove dimensions that were compressed into second ...  */
 
            /* ... make sure this loop doesn't vectorize because of the possible recurrences ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (nd2=notcompress+1;nd2<locndim;nd2++)
                  {
                    indr_locdim_Mask[nd][nd2-numcompress]       = indr_locdim_Mask[nd][nd2];
                    indr_locsize_Mask[nd][nd2-numcompress]      = indr_locsize_Mask[nd][nd2];
                    indr_loclo_Mask[nd][nd2-numcompress]        = indr_loclo_Mask[nd][nd2];
                    indr_locbound_Mask[nd][nd2-numcompress]     = indr_locbound_Mask[nd][nd2];
                    indr_compressible_Mask[nd][nd2-numcompress] = indr_compressible_Mask[nd][nd2];
                  }
             }
        }

;

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }

#else

  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

     longest = 0;
     second = 1;
     Dimorder[0] = 0;
     Dimorder[1] = 1;

     if (locndim == 1)
        {
          for (nd=0;nd<MAXDIMS;nd++)
             {
               indr_loclo_Lhs[nd][second]=0;
               indr_locbound_Lhs[nd][second]=0;
               if (indr_locdim_Lhs[nd][locndim]>1)
                    indr_locstride_Lhs[nd][second]=1;

               indr_Sclstride_Lhs[nd][1]=0;
               indr_Offsetreset_Lhs[nd][1]=0;

               indr_Sclstride_Rhs[nd][1]=0;
               indr_Offsetreset_Rhs[nd][1]=0;

               indr_Sclstride_Mask[nd][1]=0;
               indr_Offsetreset_Mask[nd][1]=0;
             }
        }
#endif

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down
      (since all dimensions of index arrays must have the
      same length, pick the first since it will always exist) ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = indr_loclo_Lhs[0][nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Lhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Lhs[nd][0] = indr_locstride_Lhs[nd][0];
#if 1
          if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
	       ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Lhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Lhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Lhs[nd][0] !=0)
               indr_Offsetreset_Lhs[nd][0] = ((indr_locbound_Lhs[nd][0] -indr_loclo_Lhs[nd][0])/indr_locstride_Lhs[nd][0]) * indr_Sclstride_Lhs[nd][0];
            else
               indr_Offsetreset_Lhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Lhs[nd][nd2] = indr_locsize_Lhs[nd][nd2-1] * indr_locstride_Lhs[nd][nd2];

#if 1
               if ( ( !(Lhs_Descriptor->Uses_Indirect_Addressing) && Lhs_Descriptor->Bound[nd]==Lhs_Descriptor->Base[nd] ) ||
                    ( Lhs_Descriptor->Uses_Indirect_Addressing && Lhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Lhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Lhs[nd][nd2] !=0)
                    indr_Offsetreset_Lhs[nd][nd2] = ((indr_locbound_Lhs[nd][nd2]-indr_loclo_Lhs[nd][nd2]) / indr_locstride_Lhs[nd][nd2]) * indr_Sclstride_Lhs[nd][nd2];
                 else
                    indr_Offsetreset_Lhs[nd][nd2] = 0;

               indr_Offset_Lhs[nd] += indr_loclo_Lhs[nd][nd2] * indr_locsize_Lhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Result_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Result[nd][0] = indr_locstride_Result[nd][0];
#if 1
          if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
	       ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Result[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Result[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Result[nd][0] !=0)
               indr_Offsetreset_Result[nd][0] = ((indr_locbound_Result[nd][0] -indr_loclo_Result[nd][0])/indr_locstride_Result[nd][0]) * indr_Sclstride_Result[nd][0];
            else
               indr_Offsetreset_Result[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Result[nd] += indr_loclo_Result[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Result[nd][nd2] = indr_locsize_Result[nd][nd2-1] * indr_locstride_Result[nd][nd2];

#if 1
               if ( ( !(Result_Descriptor->Uses_Indirect_Addressing) && Result_Descriptor->Bound[nd]==Result_Descriptor->Base[nd] ) ||
                    ( Result_Descriptor->Uses_Indirect_Addressing && Result_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Result[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Result[nd][nd2] !=0)
                    indr_Offsetreset_Result[nd][nd2] = ((indr_locbound_Result[nd][nd2]-indr_loclo_Result[nd][nd2]) / indr_locstride_Result[nd][nd2]) * indr_Sclstride_Result[nd][nd2];
                 else
                    indr_Offsetreset_Result[nd][nd2] = 0;

               indr_Offset_Result[nd] += indr_loclo_Result[nd][nd2] * indr_locsize_Result[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Rhs_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Rhs[nd][0] = indr_locstride_Rhs[nd][0];
#if 1
          if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
	       ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Rhs[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Rhs[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Rhs[nd][0] !=0)
               indr_Offsetreset_Rhs[nd][0] = ((indr_locbound_Rhs[nd][0] -indr_loclo_Rhs[nd][0])/indr_locstride_Rhs[nd][0]) * indr_Sclstride_Rhs[nd][0];
            else
               indr_Offsetreset_Rhs[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Rhs[nd][nd2] = indr_locsize_Rhs[nd][nd2-1] * indr_locstride_Rhs[nd][nd2];

#if 1
               if ( ( !(Rhs_Descriptor->Uses_Indirect_Addressing) && Rhs_Descriptor->Bound[nd]==Rhs_Descriptor->Base[nd] ) ||
                    ( Rhs_Descriptor->Uses_Indirect_Addressing && Rhs_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Rhs[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Rhs[nd][nd2] !=0)
                    indr_Offsetreset_Rhs[nd][nd2] = ((indr_locbound_Rhs[nd][nd2]-indr_loclo_Rhs[nd][nd2]) / indr_locstride_Rhs[nd][nd2]) * indr_Sclstride_Rhs[nd][nd2];
                 else
                    indr_Offsetreset_Rhs[nd][nd2] = 0;

               indr_Offset_Rhs[nd] += indr_loclo_Rhs[nd][nd2] * indr_locsize_Rhs[nd][nd2-1];
             }
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     MDI_ASSERT (Mask_Descriptor != NULL);

     for (nd=0;nd<MAXDIMS;nd++)
        {
          indr_Sclstride_Mask[nd][0] = indr_locstride_Mask[nd][0];
#if 1
          if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
	       ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
            /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
               indr_Sclstride_Mask[nd][0] = 0;
#endif

      /*  MDI_ASSERT(indr_Sclstride_Mask[nd][0] != 0); */

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (indr_locstride_Mask[nd][0] !=0)
               indr_Offsetreset_Mask[nd][0] = ((indr_locbound_Mask[nd][0] -indr_loclo_Mask[nd][0])/indr_locstride_Mask[nd][0]) * indr_Sclstride_Mask[nd][0];
            else
               indr_Offsetreset_Mask[nd][0] = 0;

       /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */
          indr_Offset_Mask[nd] += indr_loclo_Mask[nd][0];
  
       /* ... offsetreset is computed by dividing the range by strides
          and then multiplying by scalestrd instead of just multiplying
          by scale in case the range isn't evenly divisible ... */

       /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

          for (nd2=1;nd2<locndim;nd2++)
             {
               indr_Sclstride_Mask[nd][nd2] = indr_locsize_Mask[nd][nd2-1] * indr_locstride_Mask[nd][nd2];

#if 1
               if ( ( !(Mask_Descriptor->Uses_Indirect_Addressing) && Mask_Descriptor->Bound[nd]==Mask_Descriptor->Base[nd] ) ||
                    ( Mask_Descriptor->Uses_Indirect_Addressing && Mask_Descriptor->Index_Array[nd]==NULL ) )
                 /* if just a scalar in this dimension, Sclstride=0 is needed for index computations */
                    indr_Sclstride_Mask[nd][0] = 0;
#endif

            /* ... check stride because it may have been set to 0 for sum along an axis ... */
               if (indr_locstride_Mask[nd][nd2] !=0)
                    indr_Offsetreset_Mask[nd][nd2] = ((indr_locbound_Mask[nd][nd2]-indr_loclo_Mask[nd][nd2]) / indr_locstride_Mask[nd][nd2]) * indr_Sclstride_Mask[nd][nd2];
                 else
                    indr_Offsetreset_Mask[nd][nd2] = 0;

               indr_Offset_Mask[nd] += indr_loclo_Mask[nd][nd2] * indr_locsize_Mask[nd][nd2-1];
             }
        }



     *longestptr = longest;
     *secondptr = second;
     *locndimptr = locndim;
     *indirdimptr = indirdim;

  /* MDI_ASSERT(indr_Sclstride_Lhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Result[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Rhs[indirdim][longest] != 0); */
  /* MDI_ASSERT(indr_Sclstride_Mask[indirdim][longest] != 0); */

#if MDI_DEBUG
     if (APP_DEBUG > 10)
        {
          printf ("Dimorder[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Dimorder[i_debug]);
          printf ("\n");

          printf ("ICounter[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",ICounter[i_debug]);
          printf ("\n");


          printf ("Base_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Lhs[i_debug]);
          printf ("\n");

          printf ("Lhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Lhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Lhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Lhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Lhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Lhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Result[i_debug]);
          printf ("\n");

          printf ("Result_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Result_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Result[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Result[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Result[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Result[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Rhs[i_debug]);
          printf ("\n");

          printf ("Rhs_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Rhs_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Rhs[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Rhs[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Rhs[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Rhs[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");


          printf ("Base_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Base_Offset_Mask[i_debug]);
          printf ("\n");

          printf ("Mask_Local_Map[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",Mask_Local_Map[i_debug]);
          printf ("\n");

          printf ("indr_Offset_Mask[0-MAXDIMS] = ");
          for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
               printf ("%d ",indr_Offset_Mask[i_debug]);
          printf ("\n");


          printf ("indr_loclo_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_loclo_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locbound_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locbound_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locsize_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locsize_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_locdim_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_locdim_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_compressible_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_compressible_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Sclstride_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Sclstride_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");

          printf ("indr_Offsetreset_Mask[0-MAXDIMS][0-MAXDIMS] = ");
          for (j_debug = 0; j_debug < MAXDIMS; j_debug++)
             {
               for (i_debug = 0; i_debug < MAXDIMS; i_debug++)
                    printf ("%d ",indr_Offsetreset_Mask[j_debug][i_debug]);
               printf ("\n");
             }
          printf ("\n");



          printf ("*longestptr  = %d \n",*longestptr);
          printf ("*secondptr   = %d \n",*secondptr);
          printf ("*locndimptr  = %d \n",*locndimptr);
          printf ("*indirdimptr = %d \n",*indirdimptr);
	}
#endif

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




