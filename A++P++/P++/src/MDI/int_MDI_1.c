

#define NODIMCOMPRESS

#include <math.h>
#include <limits.h>
#include "constants.h"
#include "machine.h"

extern int APP_DEBUG;

 











/*================================================================*/


#define  INTARRAY
int* MDI_int_Allocate ( array_domain* Descriptor )
{
  int* Array_Data_Pointer = NULL;

  /* We don't have to worry about stride here because the we only 
     allocate memory for actual arrays (not views or references) and 
     these are always stride 1!
  */
  DOMAIN_SIZE_Type Size = Descriptor->Size[MAXDIMS-1];

#if MDI_DEBUG
  if (APP_DEBUG > 0) printf ("Inside of MDI_int_Allocate \n");
#endif

  if (Size > 0)
  {
    Array_Data_Pointer = (int*) MDI_MALLOC ( Size * sizeof(int) );

    if (Array_Data_Pointer == NULL)
    {
      printf ("HEAP ERROR: Array_Data_Pointer == NULL in int* MDI_int_Allocate (Size = %ld)! \n", Size);
      exit (1);
    }
  }
  else
  {
#if MDI_DEBUG
    if (Size < 0)
    {
      printf ("ERROR: Size < 0 Size = %d in MDI_int_Allocate! \n",Size);
      exit (1);
    }
#endif

    Array_Data_Pointer = NULL;
  }

  return Array_Data_Pointer;
}

/*================================================================*/

void MDI_int_Deallocate ( int* Array_Data_Pointer , array_domain* Descriptor )
{
#if MDI_DEBUG
  if (APP_DEBUG > 0)
    printf ("Inside of MDI_int_Deallocate Memory pointer = %p \n",
      Array_Data_Pointer);
   
  if (Array_Data_Pointer == NULL)
  {
    printf ("ERROR: Array_Data_Pointer == NULL in int* MDI_int_Deallocate! \n");
    exit (1);
  }
#endif

  free (Array_Data_Pointer);
}

/*================================================================*/

void MDI_int_Print_Array ( int* Lhs , array_domain *Lhs_Descriptor , 
			  int Display_Format )
{
  int i; int j; int k; int l;
  int Offset_I; int Offset_J; int Offset_K; int Offset_L;
  int print_index;

  /*--------------------------------------------------------*/

     int Binary_Conformable;

  /* We might decide to store a single value if the strides are constant along each axis.
  // We should also isolate the cases where the strides are one!
  */
     int Lhs_Rhs_Strides_Equal;
     int Result_Lhs_Strides_Equal;
     int Strides_Equal;
     int Unit_Stride;
     int Array_Size;

  /* We might decide to store a single value if the bases are constant along each axis.
  */
     int Lhs_Rhs_Bases_Equal;
     int Result_Lhs_Bases_Equal;
     int Bases_Equal;

  /* We need to test if the block sizes are the same too! */
     int Lhs_Rhs_Dimensions_Equal;
     int Result_Lhs_Dimensions_Equal;
     int Dimensions_Equal;
  /*--------------------------------------------------------*/
  /* ... old variables still used ...                       */
     int Indirect_Addressing_Used;
     int Minimum_Base_For_Indirect_Addressing;
     int Maximum_Bound_For_Indirect_Addressing;
  /*--------------------------------------------------------*/
  /* ... new variables ...                                  */

     int nd; int nd2; int cnt;
     int tempdim; int nextdim; int ordtempdim;
     int locndim; int length; int longest; 
     int second; int longlen; int secondlen;
     int numcompress; int notcompress;
     int offsetrange;
     int Dimorder       [MAXDIMS];
     int ICounter       [MAXDIMS+1];
     int max_dim_length [MAXDIMS];
     int locndimtemp;
     int indirdim;

/* Variables used for initialization and debugging mode */
     int i_debug;
     int j_debug;


  /*--------------------------------------------------------*/

  /* indirect addressing variables */

     int indr_loclo_Lhs       [MAXDIMS][MAXDIMS];
     int indr_locbound_Lhs    [MAXDIMS][MAXDIMS];
     int indr_locsize_Lhs     [MAXDIMS][MAXDIMS];
     int indr_locstride_Lhs   [MAXDIMS][MAXDIMS];
     int indr_locdim_Lhs      [MAXDIMS][MAXDIMS];
     int indr_compressible_Lhs[MAXDIMS][MAXDIMS];

     int indr_Offset_Lhs      [MAXDIMS];
     int indr_Offsetreset_Lhs [MAXDIMS][MAXDIMS];
     int indr_Sclstride_Lhs   [MAXDIMS][MAXDIMS];

     int Stride_Lhs_long      [MAXDIMS];

     int ii_1               [MAXDIMS];

     int Address_Lhs;
     int* index_data_ptr_Lhs  [MAXDIMS];
     int Lhs_Local_Map        [MAXDIMS];         
     int Base_Offset_Lhs      [MAXDIMS];
     
  /*--------------------------------------------------------*/
  /* ... new variables ...                                  */

     int Lo_Lhs_I;int Hi_Lhs_I;int Stride_Lhs_I;
     int Lo_Lhs_J;int Hi_Lhs_J;int Stride_Lhs_J;

     int Offset_Lhs;

     int compressible_Lhs[MAXDIMS];

     int locdim_Lhs      [MAXDIMS];
     int locsize_Lhs     [MAXDIMS];
     int loclo_Lhs       [MAXDIMS];
     int locbound_Lhs    [MAXDIMS];
     int locstride_Lhs   [MAXDIMS];

     int Sclstride_Lhs   [MAXDIMS];
     int Offsetreset_Lhs [MAXDIMS];

  /* Loop index variables */
     int i1;int j1;

  /* Used to store min base and max bound for Lhs */
     int Minimum_Base_For_Indirect_Addressing_Lhs;
     int Maximum_Bound_For_Indirect_Addressing_Lhs;



  /* Assume Strides NOT Equal and Bases NOT Equal */

  /* ---------------------------------------------------------------- */

  /* We should avoid an implementation that mixes constant and non 
     constant Indexing!  Since all dimensions are index by indirect 
     addressing if any dimensions are we need only check a single 
     dimension.
  */
     MDI_ASSERT (Lhs_Descriptor != NULL);
     Indirect_Addressing_Used = Lhs_Descriptor ->Uses_Indirect_Addressing;



  if (Indirect_Addressing_Used)
  {

#if 1
/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii */
    /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

          Minimum_Base_For_Indirect_Addressing  = INT_MAX;
          Maximum_Bound_For_Indirect_Addressing = INT_MIN;

          MDI_Indirect_Setup_Lhs (Lhs_Descriptor,indr_loclo_Lhs,indr_locbound_Lhs,
                                  indr_locstride_Lhs, indr_locsize_Lhs,indr_locdim_Lhs,
                                  indr_compressible_Lhs,indr_Offset_Lhs,indr_Sclstride_Lhs,
                                  indr_Offsetreset_Lhs,index_data_ptr_Lhs,Lhs_Local_Map,
                                  Base_Offset_Lhs,Dimorder,ICounter,&longest,&second,&locndim,&indirdim);


          offsetrange = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest];
          if (longest >0)
               offsetrange *= indr_locsize_Lhs[indirdim][longest-1];

          do {
               Lo_Lhs_J = indr_loclo_Lhs[indirdim][second];
               Hi_Lhs_J = indr_locbound_Lhs[indirdim][second];
               Stride_Lhs_J = indr_locstride_Lhs[indirdim][second];

               for (j1  = Lo_Lhs_J; j1 <= Hi_Lhs_J; j1 += Stride_Lhs_J)
                  {

                    Lo_Lhs_I     = indr_Offset_Lhs[indirdim];
                    Hi_Lhs_I     = indr_Offset_Lhs[indirdim] + offsetrange;
                    Stride_Lhs_I = indr_Sclstride_Lhs[indirdim][longest];

                 /* Trap out the case where the stride could be zero in the loop (but allow 
                    indr_Sclstride_Lhs[indirdim][longest] to remain zero! */
                    if (Stride_Lhs_I == 0)
                         Stride_Lhs_I = 1;

                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         ii_1[nd] = indr_Offset_Lhs[nd];
                         Stride_Lhs_long[nd] = indr_Sclstride_Lhs[nd][longest];
                       }

                    MDI_ASSERT (Stride_Lhs_I > 0);

                    for (i1  = Lo_Lhs_I; i1 <= Hi_Lhs_I; i1 += Stride_Lhs_I)
                       {
                         MDI_ASSERT (Lhs_Descriptor != NULL);

#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                       {
                         for (nd=0; nd < MAXDIMS; nd++)
                            {
                              printf ("index_data_ptr_Lhs[%d][ii_1[%d]] = %d ii_1[%d] = %d \n",
                                   nd,nd,index_data_ptr_Lhs[nd][ii_1[nd]],nd,ii_1[nd]);
                              printf ("Base_Offset_Lhs[%d]        = %d \n",nd,Base_Offset_Lhs[nd]);
                              printf ("Lhs_Descriptor->Stride[%d] = %d \n",nd,Lhs_Descriptor->Stride[nd]);
                              printf ("Lhs_Descriptor->Base[%d]   = %d \n",nd,Lhs_Descriptor->Base[nd]);
                            }
	               }
#endif

                 /* We hope the compiler will lift these out of the loop since they are loop invariant */
                 /* take this out here because doesn't work for indexes */
                    Address_Lhs = (index_data_ptr_Lhs[0][ii_1[0]] + Base_Offset_Lhs[0]) * Lhs_Descriptor->Stride[0] + Lhs_Descriptor->Base[0];
#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                         printf ("After %d iterations: Address_Lhs = %d \n",0,Address_Lhs);
#endif

                    for (nd=1;nd<MAXDIMS;nd++)
                       {
                         Address_Lhs += ((index_data_ptr_Lhs[nd][ii_1[nd]] + Base_Offset_Lhs[nd]) * Lhs_Descriptor->Stride[nd] + Lhs_Descriptor->Base[nd]) * Lhs_Descriptor->Size[nd-1];

#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                              printf ("After %d iterations: Address_Lhs = %d \n",nd,Address_Lhs);
#endif
                       }


                         if (i1 == Lo_Lhs_I ) 
           printf ("AXIS 1 (%3d) ",j1+Lhs_Descriptor->Data_Base[1]);

#ifndef INTARRAY
         if ( Display_Format == MDI_DECIMAL_DISPLAY_FORMAT )
              printf ("%3.4f ",(double)(Lhs[Address_Lhs]));
         else
              printf ("%3.4e ",(double)(Lhs[Address_Lhs]));
#else
         if ( Display_Format == MDI_DECIMAL_DISPLAY_FORMAT )
              printf ("%3d ",(int)(Lhs[Address_Lhs]));
         else
              printf ("%3.4e ",(double)(Lhs[Address_Lhs]));
#endif
         if (i1 == Hi_Lhs_I) printf ("\n");
      ;
                         for (nd=0;nd<MAXDIMS;nd++)
                            {
#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                            {
                              printf ("ii_1[%d] = %d Stride_Lhs_long[%d] = %d \n",nd,ii_1[nd],nd,Stride_Lhs_long[nd]);
                            }
#endif
                              ii_1[nd] += Stride_Lhs_long[nd]; 
                            }
                       }
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                            {
                              printf ("indr_Offset_Lhs[%d] = %d second = %d indr_Sclstride_Lhs[%d][%d] = %d \n",
                                   nd,indr_Offset_Lhs[nd],second,nd,second,indr_Sclstride_Lhs[nd][second]);
                            }
#endif
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][second];
                       }
                  }

            /* ... if arrays are more than 1d fix offsets, otherwise
               offsets aren't used again so they don't need to be reset ...  */
               if (locndim>1)
                  {
                 /* ... one extra Sclstride was added at end of j loop so
	            remove that first then reset by the whole amount
                    incremented by going through j loop ... */
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         indr_Offset_Lhs[nd] -= indr_Sclstride_Lhs[nd][second];
                         indr_Offset_Lhs[nd] -= indr_Offsetreset_Lhs[nd][second];
                       }
                  }

            /* ... compute offsets due to higher dimensions iterating ...  */
               if (locndim > 2)
                  {
                 /* ... this loop vectorizes but runs slower with vectorization than without ... */
#if CRAY
#  pragma _CRI novector
#endif
                    for (tempdim=2;tempdim<locndim;tempdim++)
                       {
                      /* ... loop until a dimension is found that isn't at its 
                         max or all dimensions have been checked, correcting the offset ... */
                         ordtempdim = Dimorder[tempdim];
                         if (ICounter[ordtempdim] < indr_locbound_Lhs[0][ordtempdim])
                              break;
                         for (nd=0;nd<MAXDIMS;nd++)
	                      indr_Offset_Lhs[nd] -= indr_Offsetreset_Lhs[nd][ordtempdim];
                         ICounter[ordtempdim] = indr_loclo_Lhs[indirdim][ordtempdim];
                       }

                    if (tempdim<locndim)
                       {
                      /* ... dimension ordtempdim isn't at its max ... */
                         ordtempdim = Dimorder[tempdim];
                         ICounter[ordtempdim]+=indr_locstride_Lhs[0][ordtempdim];
                         for (nd=0;nd<MAXDIMS;nd++)
                              indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][ordtempdim];
                         tempdim = 0;
                       }
                 /* ... tempdim is either 0 or locndim here ... */
                    ICounter[locndim] = tempdim;
                  }
                 else
                  {
                 /* ... arrays are only 2d or less ... */
                    ICounter[locndim] = locndim;
                  }
             } while (ICounter[locndim] != locndim);


/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii */
#endif
  }
 else
  {
    /*------------------------------------------------------------------*/
  /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed ... */

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



#ifndef NODIMCOMPRESS

  /* ... find how to order the dimensions so the longest two after
      collapsing come first (only need to do this for Lhs because both
      have the same size since they are conformable and so will be 
      ordered the same, ordering is done before compressing because
      dimensions are compressed only if they will end up being
      the longest or second longest) ... */

     /*------------------------------------------------------------------*/

     longest = locndim-1;
     while(compressible_Lhs[longest-1]&&longest>0)
          longest--;

     length = locbound_Lhs[longest] - loclo_Lhs[longest] +1;
     length /=locstride_Lhs[longest];
     if (length<1)
          length=1;
     nd=longest;
     while (compressible_Lhs[nd++]) 
          length *= (locbound_Lhs[nd+1]-loclo_Lhs[nd+1]+1);
     longlen = length;

     second = -1;
     secondlen = 0;

     for(nd=longest-1;nd>=0;nd--)
        {
          length = locbound_Lhs[nd] - loclo_Lhs[nd] +1;
          length /=locstride_Lhs[nd];
          if (length<1) length=1;

          if (!compressible_Lhs[nd-1] || nd==0)
             {
               int nd2 = nd;
               while (compressible_Lhs[nd2++]) 
               length *= (locbound_Lhs[nd2+1]-loclo_Lhs[nd2+1]+1);
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
             {
               if (nd != longest && nd != second) Dimorder[cnt++]=nd;
            /* if (nd != longest && nd != second && (!compressible_Lhs[nd-1] || nd==0)) Dimorder[cnt++]=nd; */
             }
        }



  /* ... collapse local dimensions for longest loop and second
      longest loop if these dimensions are compressible ... */

     numcompress = MDI_Compress_Lhs (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,locndim,longest);

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          numcompress = MDI_Compress_Lhs (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,locndim,longest);

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;

          loclo_Lhs[second]=0;
          locbound_Lhs[second]=0;
          locstride_Lhs[second]=1;

          Sclstride_Lhs[second]=1;
          Offsetreset_Lhs[second]=0;
        }

#else
     /*------------------------------------------------------------------*/

  /* ... set up Dimorder array when compression and ordering
      are turned off ... */

     for (nd=0;nd<locndim;nd++)
          Dimorder[nd]=nd;
  
     longest = 0;
     second = 1;



     if (locndim == 1 )
        {
          loclo_Lhs[second]=0;
          locbound_Lhs[second]=0;
          locstride_Lhs[second]=1;

          Sclstride_Lhs[second]=1;
          Offsetreset_Lhs[second]=0;
        }

#endif

  /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

  /* ... set up ICounter array to control looping for higher
      dimensions, vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif
     for (nd=0;nd<locndim;nd++)
          ICounter[nd] = loclo_Lhs[nd];
     ICounter[locndim] = locndim;

  /* ... set up loop control arrays ... */
     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     Sclstride_Lhs[0] = locstride_Lhs[0];
  /* ... check stride because it may have been set to 0 for sum along an axis ... */
     if (locstride_Lhs[0] !=0)
          Offsetreset_Lhs[0] = ((locbound_Lhs[0]-loclo_Lhs[0])/locstride_Lhs[0]) * Sclstride_Lhs[0];
       else
          Offsetreset_Lhs[0] = 0;

  /*
     Offset_Lhs = 0;
     for (nd=locndim;nd<MAXDIMS;nd++) 
        {
          Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; 
        }
  */

  /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */

  /*Offset_Lhs += loclo_Lhs[0];*/

  /* ... offsetreset is computed by dividing the range by strides
     and then multiplying by scalestrd instead of just multiplying
     by scale incase the range isn't evenly divisible ... */

  /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

     for (nd=1;nd<locndim;nd++)
        {
          Sclstride_Lhs[nd] = locsize_Lhs[nd-1]*locstride_Lhs[nd];

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (locstride_Lhs[nd] !=0)
               Offsetreset_Lhs[nd] = ((locbound_Lhs[nd]-loclo_Lhs[nd])/locstride_Lhs[nd]) * Sclstride_Lhs[nd];
            else
               Offsetreset_Lhs[nd] = 0;

       /* Offset_Lhs += loclo_Lhs[nd]*locsize_Lhs[nd-1]; */
        }



  /* offsetrange = locbound_Lhs[longest]; */
     offsetrange = locbound_Lhs[longest]-loclo_Lhs[longest];
     if (longest >0)
          offsetrange *= locsize_Lhs[longest-1];

     do {
          Lo_Lhs_J = loclo_Lhs[second];
          Hi_Lhs_J = locbound_Lhs[second];
          Stride_Lhs_J = locstride_Lhs[second];

          for (j1  = Lo_Lhs_J; j1 <= Hi_Lhs_J; j1 += Stride_Lhs_J)
             {

#if MDI_DEBUG
               if (APP_DEBUG) printf ("j1 = %d \n",j1);
#endif

               Lo_Lhs_I = Offset_Lhs;
               Hi_Lhs_I = Offset_Lhs + offsetrange;
               Stride_Lhs_I = Sclstride_Lhs[longest];

               for (i1  = Lo_Lhs_I; i1 <= Hi_Lhs_I; i1 += Stride_Lhs_I)
                  {

#if MDI_DEBUG
                    if (APP_DEBUG) printf ("i1 = %d \n",i1);
#endif

                    if (j1 == Lo_Lhs_J && i1 == Lo_Lhs_I) 
	 {
	   for (nd=locndim-2;nd>1;nd--)
	   {
	     nd2 = nd;
	     while (ICounter[nd2] == loclo_Lhs[nd2] && nd2>1) nd2--;
	     if (nd2==1)
	     {
               printf ("***** AXIS %d (%3d) ***** \n", nd+1,
	          ICounter[nd+1]+Lhs_Descriptor->Data_Base[nd+1]);
	     }
	   }
	 }

         if (j1 == Lo_Lhs_J && i1 == Lo_Lhs_I) 
         {
	   if (locndim>2) printf ("***** AXIS 2 (%3d) ***** \n",
	     ICounter[2]+Lhs_Descriptor->Data_Base[2]);
           printf ("AXIS 0 --->: ");
           for (print_index  = loclo_Lhs[0];
                print_index <= locbound_Lhs[0];
                print_index += locstride_Lhs[0])
             printf ("(%4d) ",(print_index)+Lhs_Descriptor->Data_Base[0]);
           printf ("\n");
         }

         if (i1 == Lo_Lhs_I ) 
           printf ("AXIS 1 (%3d) ",j1+Lhs_Descriptor->Data_Base[1]);

#ifndef INTARRAY
         if ( Display_Format == MDI_DECIMAL_DISPLAY_FORMAT )
            printf ("%3.4f ",(double)(Lhs[i1]));
         else
            printf ("%3.4e ",(double)(Lhs[i1]));
#else
         if ( Display_Format == MDI_DECIMAL_DISPLAY_FORMAT )
            printf ("%4d ",(int)(Lhs[i1]));
         else
            printf ("%3.4e ",(double)(Lhs[i1]));
#endif
         if (i1 == Hi_Lhs_I) printf ("\n");
      ;
                  }
               Offset_Lhs += Sclstride_Lhs[second];
             }

       /* ... if arrays are more than 1d fix offsets, otherwise
          offsets aren't used again so they don't need to be reset ...  */
          if (locndim>1)
             {
            /* ... one extra Sclstride was added at end of j loop so
	       remove that first then reset by the whole amount
	       incremented by going through j loop ... */
               Offset_Lhs -= Sclstride_Lhs[second];
               Offset_Lhs -= Offsetreset_Lhs[second];
             }

       /* ... compute offsets due to higher dimensions iterating ...  */
          if (locndim > 2)
             {
            /* ... this loop vectorizes but runs slower with vectorization than without ... */
#if CRAY
#  pragma _CRI novector
#endif
               for (tempdim=2;tempdim<locndim;tempdim++)
                  {
                 /* ... loop until a dimension is found that isn't at its 
	            max or all dimensions have been checked, correcting the offset ... */
                    ordtempdim = Dimorder[tempdim];
	            if (ICounter[ordtempdim] < locbound_Lhs[ordtempdim])
                         break;
                    Offset_Lhs -= Offsetreset_Lhs[ordtempdim];
                    ICounter[ordtempdim] = loclo_Lhs[ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=locstride_Lhs[ordtempdim];
                    Offset_Lhs += Sclstride_Lhs[ordtempdim];
                    tempdim = 0;
                  }
            /* ... tempdim is either 0 or locndim here ... */
               ICounter[locndim] = tempdim;
             }
            else
             {
            /* ... arrays are only 2d or less ... */
               ICounter[locndim] = locndim;
             }
        }
     while (ICounter[locndim] != locndim);


  }
}









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




