

#include <math.h>
#include <limits.h>
#include "constants.h"
#include "machine.h"

extern int APP_DEBUG;

 












#define  INTARRAY
/*
   Bugfix (31/10/97) contributed from Kubo at skubo@totoro.rsn.hp.com
   Previous definition: define(MDI_floor,(int) i)
   define CNX_FLOOR(L) ((L)<0 && (L)-(int)(L)<0) ? (int)((L)-1) : (int)(L)
   Result [i1] = CNX_FLOOR(Lhs [i1]);
 */



#ifndef INTARRAY
/* Unary_Array_Operation(i_Floor,floor,int) */
void MDI_i_Floor_Array 
          ( int *Result , int *Lhs ,
            int *Mask_Array_Pointer ,
            array_domain *Result_Descriptor ,
            array_domain *Lhs_Descriptor ,
            array_domain *Mask_Descriptor )
   {
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

     int indr_loclo_Result       [MAXDIMS][MAXDIMS];
     int indr_locbound_Result    [MAXDIMS][MAXDIMS];
     int indr_locsize_Result     [MAXDIMS][MAXDIMS];
     int indr_locstride_Result   [MAXDIMS][MAXDIMS];
     int indr_locdim_Result      [MAXDIMS][MAXDIMS];
     int indr_compressible_Result[MAXDIMS][MAXDIMS];

     int indr_Offset_Result      [MAXDIMS];
     int indr_Offsetreset_Result [MAXDIMS][MAXDIMS];
     int indr_Sclstride_Result   [MAXDIMS][MAXDIMS];

     int Stride_Result_long      [MAXDIMS];

     int ii_0               [MAXDIMS];

     int Address_Result;
     int* index_data_ptr_Result  [MAXDIMS];
     int Result_Local_Map        [MAXDIMS];         
     int Base_Offset_Result      [MAXDIMS];
     
  /*--------------------------------------------------------*/
  /* ... new variables ...                                  */

     int Lo_Result_I;int Hi_Result_I;int Stride_Result_I;
     int Lo_Result_J;int Hi_Result_J;int Stride_Result_J;

     int Offset_Result;

     int compressible_Result[MAXDIMS];

     int locdim_Result      [MAXDIMS];
     int locsize_Result     [MAXDIMS];
     int loclo_Result       [MAXDIMS];
     int locbound_Result    [MAXDIMS];
     int locstride_Result   [MAXDIMS];

     int Sclstride_Result   [MAXDIMS];
     int Offsetreset_Result [MAXDIMS];

  /* Loop index variables */
     int i0;int j0;

  /* Used to store min base and max bound for Result */
     int Minimum_Base_For_Indirect_Addressing_Result;
     int Maximum_Bound_For_Indirect_Addressing_Result;


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


     /*--------------------------------------------------------*/

  /* indirect addressing variables */

     int indr_loclo_Mask       [MAXDIMS][MAXDIMS];
     int indr_locbound_Mask    [MAXDIMS][MAXDIMS];
     int indr_locsize_Mask     [MAXDIMS][MAXDIMS];
     int indr_locstride_Mask   [MAXDIMS][MAXDIMS];
     int indr_locdim_Mask      [MAXDIMS][MAXDIMS];
     int indr_compressible_Mask[MAXDIMS][MAXDIMS];

     int indr_Offset_Mask      [MAXDIMS];
     int indr_Offsetreset_Mask [MAXDIMS][MAXDIMS];
     int indr_Sclstride_Mask   [MAXDIMS][MAXDIMS];

     int Stride_Mask_long      [MAXDIMS];

     int ii_3               [MAXDIMS];

     int Address_Mask;
     int* index_data_ptr_Mask  [MAXDIMS];
     int Mask_Local_Map        [MAXDIMS];         
     int Base_Offset_Mask      [MAXDIMS];
     
  /*--------------------------------------------------------*/
  /* ... new variables ...                                  */

     int Lo_Mask_I;int Hi_Mask_I;int Stride_Mask_I;
     int Lo_Mask_J;int Hi_Mask_J;int Stride_Mask_J;

     int Offset_Mask;

     int compressible_Mask[MAXDIMS];

     int locdim_Mask      [MAXDIMS];
     int locsize_Mask     [MAXDIMS];
     int loclo_Mask       [MAXDIMS];
     int locbound_Mask    [MAXDIMS];
     int locstride_Mask   [MAXDIMS];

     int Sclstride_Mask   [MAXDIMS];
     int Offsetreset_Mask [MAXDIMS];

  /* Loop index variables */
     int i3;int j3;

  /* Used to store min base and max bound for Mask */
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;



     /*--------------------------------------------------------*/

#if MDI_DEBUG

     Binary_Conformable = -1;

  /* We might decide to store a single value if the strides are constant along each axis.
  // We should also isolate the cases where the strides are one!
  */
     Lhs_Rhs_Strides_Equal    = -1;
     Result_Lhs_Strides_Equal = -1;
     Strides_Equal            = -1;
     Unit_Stride              = -1;
     Array_Size               = -1;

  /* We might decide to store a single value if the bases are constant along each axis. */
     Lhs_Rhs_Bases_Equal    = -1;
     Result_Lhs_Bases_Equal = -1;
     Bases_Equal            = -1;

  /* We need to test if the block sizes are the same too! */
     Lhs_Rhs_Dimensions_Equal    = -1;
     Result_Lhs_Dimensions_Equal = -1;
     Dimensions_Equal            = -1;
  /*--------------------------------------------------------*/

  /* ... old variables still used ...                       */
     Indirect_Addressing_Used              = -1;
     Minimum_Base_For_Indirect_Addressing  = -1;
     Maximum_Bound_For_Indirect_Addressing = -1;
  /*--------------------------------------------------------*/

  /* ... new variables ...                                  */
     nd  = -1;
     nd2 = -1;
     cnt = -1;

     tempdim    = -1;
     nextdim    = -1;
     ordtempdim = -1;
     locndim    = -1;
     length     = -1;
     longest    = -1; 
     second     = -1;
     longlen    = -1;
     secondlen  = -1;

     numcompress = -1;
     notcompress = -1;
     offsetrange = -1;

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          Dimorder       [i_debug] = -1;
          ICounter       [i_debug] = -1;
          max_dim_length [i_debug] = -1;
        }

     ICounter [MAXDIMS] = -1;

     locndimtemp = -1;
     indirdim    = -1;

  /* Variables used for initialization and debugging mode */
     i_debug = -1;
     j_debug = -1;

#endif

     Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing = INT_MIN;


     /*--------------------------------------------------------*/

#if MDI_DEBUG

  /* Initialize local storage */
     Address_Result  = 0;
     Lo_Result_I     = 0;
     Hi_Result_I     = 0;
     Stride_Result_I = 0;
     Lo_Result_J     = 0;
     Hi_Result_J     = 0;
     Stride_Result_J = 0;
     Offset_Result   = 0;

  /* initialize indirect addressing variables */

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          for (j_debug=0; j_debug < MAXDIMS; j_debug++)
             {
               indr_loclo_Result        [i_debug][j_debug] = -1;
               indr_locbound_Result     [i_debug][j_debug] = -1;
               indr_locsize_Result      [i_debug][j_debug] = -1;
               indr_locstride_Result    [i_debug][j_debug] = -1;
               indr_locdim_Result       [i_debug][j_debug] = -1;
               indr_compressible_Result [i_debug][j_debug] = -1;

               indr_Offsetreset_Result  [i_debug][j_debug] = -1;
               indr_Sclstride_Result    [i_debug][j_debug] = -1;
             }

          indr_Offset_Result    [i_debug] = -1;
          Stride_Result_long    [i_debug] = -1;
          ii_1              [i_debug] = -1;

          index_data_ptr_Result [i_debug] = NULL;
          Result_Local_Map      [i_debug] = -1;         
          Base_Offset_Result    [i_debug] = -1;
          compressible_Result   [i_debug] = -1;
          locdim_Result         [i_debug] = -1;
          locsize_Result        [i_debug] = -1;
          loclo_Result          [i_debug] = -1;
          locbound_Result       [i_debug] = -1;
          locstride_Result      [i_debug] = -1;

          Sclstride_Result      [i_debug] = -1;
          Offsetreset_Result    [i_debug] = -1;
        }

#endif

  /* Used to store min base and max bound for Result */
     Minimum_Base_For_Indirect_Addressing_Result  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Result = INT_MIN;


     /*--------------------------------------------------------*/

#if MDI_DEBUG

  /* Initialize local storage */
     Address_Lhs  = 0;
     Lo_Lhs_I     = 0;
     Hi_Lhs_I     = 0;
     Stride_Lhs_I = 0;
     Lo_Lhs_J     = 0;
     Hi_Lhs_J     = 0;
     Stride_Lhs_J = 0;
     Offset_Lhs   = 0;

  /* initialize indirect addressing variables */

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          for (j_debug=0; j_debug < MAXDIMS; j_debug++)
             {
               indr_loclo_Lhs        [i_debug][j_debug] = -1;
               indr_locbound_Lhs     [i_debug][j_debug] = -1;
               indr_locsize_Lhs      [i_debug][j_debug] = -1;
               indr_locstride_Lhs    [i_debug][j_debug] = -1;
               indr_locdim_Lhs       [i_debug][j_debug] = -1;
               indr_compressible_Lhs [i_debug][j_debug] = -1;

               indr_Offsetreset_Lhs  [i_debug][j_debug] = -1;
               indr_Sclstride_Lhs    [i_debug][j_debug] = -1;
             }

          indr_Offset_Lhs    [i_debug] = -1;
          Stride_Lhs_long    [i_debug] = -1;
          ii_1              [i_debug] = -1;

          index_data_ptr_Lhs [i_debug] = NULL;
          Lhs_Local_Map      [i_debug] = -1;         
          Base_Offset_Lhs    [i_debug] = -1;
          compressible_Lhs   [i_debug] = -1;
          locdim_Lhs         [i_debug] = -1;
          locsize_Lhs        [i_debug] = -1;
          loclo_Lhs          [i_debug] = -1;
          locbound_Lhs       [i_debug] = -1;
          locstride_Lhs      [i_debug] = -1;

          Sclstride_Lhs      [i_debug] = -1;
          Offsetreset_Lhs    [i_debug] = -1;
        }

#endif

  /* Used to store min base and max bound for Lhs */
     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;


     /*--------------------------------------------------------*/

#if MDI_DEBUG

  /* Initialize local storage */
     Address_Mask  = 0;
     Lo_Mask_I     = 0;
     Hi_Mask_I     = 0;
     Stride_Mask_I = 0;
     Lo_Mask_J     = 0;
     Hi_Mask_J     = 0;
     Stride_Mask_J = 0;
     Offset_Mask   = 0;

  /* initialize indirect addressing variables */

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          for (j_debug=0; j_debug < MAXDIMS; j_debug++)
             {
               indr_loclo_Mask        [i_debug][j_debug] = -1;
               indr_locbound_Mask     [i_debug][j_debug] = -1;
               indr_locsize_Mask      [i_debug][j_debug] = -1;
               indr_locstride_Mask    [i_debug][j_debug] = -1;
               indr_locdim_Mask       [i_debug][j_debug] = -1;
               indr_compressible_Mask [i_debug][j_debug] = -1;

               indr_Offsetreset_Mask  [i_debug][j_debug] = -1;
               indr_Sclstride_Mask    [i_debug][j_debug] = -1;
             }

          indr_Offset_Mask    [i_debug] = -1;
          Stride_Mask_long    [i_debug] = -1;
          ii_1              [i_debug] = -1;

          index_data_ptr_Mask [i_debug] = NULL;
          Mask_Local_Map      [i_debug] = -1;         
          Base_Offset_Mask    [i_debug] = -1;
          compressible_Mask   [i_debug] = -1;
          locdim_Mask         [i_debug] = -1;
          locsize_Mask        [i_debug] = -1;
          loclo_Mask          [i_debug] = -1;
          locbound_Mask       [i_debug] = -1;
          locstride_Mask      [i_debug] = -1;

          Sclstride_Mask      [i_debug] = -1;
          Offsetreset_Mask    [i_debug] = -1;
        }

#endif

  /* Used to store min base and max bound for Mask */
     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;



#if MDI_DEBUG
     if (APP_DEBUG > 0)
          printf ("Inside of MDI_i_Floor_Array \n");
#endif

     if (Mask_Array_Pointer == NULL)
        {
          MDI_ASSERT (Result_Descriptor != NULL);
     MDI_ASSERT (Lhs_Descriptor != NULL);
     Indirect_Addressing_Used = Result_Descriptor ->Uses_Indirect_Addressing 
 			        || Lhs_Descriptor ->Uses_Indirect_Addressing;



          if (Indirect_Addressing_Used)
             {
/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
            /* Strides NOT Equal and Bases NOT Equal */
               /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

          Minimum_Base_For_Indirect_Addressing  = INT_MAX;
          Maximum_Bound_For_Indirect_Addressing = INT_MIN;

          MDI_Indirect_Setup_Lhs_Result (Lhs_Descriptor,indr_loclo_Lhs,indr_locbound_Lhs,
                                     indr_locstride_Lhs, indr_locsize_Lhs,indr_locdim_Lhs,
                                     indr_compressible_Lhs,indr_Offset_Lhs,indr_Sclstride_Lhs,
                                     indr_Offsetreset_Lhs,index_data_ptr_Lhs,Lhs_Local_Map,Base_Offset_Lhs,
                                     Result_Descriptor,indr_loclo_Result,indr_locbound_Result,
                                     indr_locstride_Result, indr_locsize_Result,indr_locdim_Result,
                                     indr_compressible_Result,indr_Offset_Result,indr_Sclstride_Result,
                                     indr_Offsetreset_Result,index_data_ptr_Result,Result_Local_Map,Base_Offset_Result,
                                     Dimorder,ICounter,&longest,&second,&locndim,&indirdim);


          offsetrange = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest];
          if (longest >0)
               offsetrange *= indr_locsize_Lhs[indirdim][longest-1];

          do {
               Lo_Lhs_J = indr_loclo_Lhs[indirdim][second];
               Hi_Lhs_J = indr_locbound_Lhs[indirdim][second];
               Stride_Lhs_J = indr_locstride_Lhs[indirdim][second];

               for (j1  = Lo_Lhs_J; j1 <= Hi_Lhs_J; j1 += Stride_Lhs_J)
                  {

                    Lo_Lhs_I = indr_Offset_Lhs[indirdim];
                    Hi_Lhs_I = indr_Offset_Lhs[indirdim] + offsetrange;
                    Stride_Lhs_I = indr_Sclstride_Lhs[indirdim][longest];

                 /* Trap out the case where the stride could be zero in the loop (but allow 
                    indr_Sclstride_Lhs[indirdim][longest] to remain zero! */
                    if (Stride_Lhs_I == 0)
                         Stride_Lhs_I = 1;

                 /* for (nd=1;nd<MAXDIMS;nd++) */
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         ii_1[nd] = indr_Offset_Lhs[nd];
                         Stride_Lhs_long[nd]  = indr_Sclstride_Lhs[nd][longest];

                         ii_0[nd] = indr_Offset_Result[nd];
                         Stride_Result_long[nd]  = indr_Sclstride_Result[nd][longest];
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


                         MDI_ASSERT (Result_Descriptor != NULL);

#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                       {
                         for (nd=0; nd < MAXDIMS; nd++)
                            {
                              printf ("index_data_ptr_Result[%d][ii_0[%d]] = %d ii_0[%d] = %d \n",
                                   nd,nd,index_data_ptr_Result[nd][ii_0[nd]],nd,ii_0[nd]);
                              printf ("Base_Offset_Result[%d]        = %d \n",nd,Base_Offset_Result[nd]);
                              printf ("Result_Descriptor->Stride[%d] = %d \n",nd,Result_Descriptor->Stride[nd]);
                              printf ("Result_Descriptor->Base[%d]   = %d \n",nd,Result_Descriptor->Base[nd]);
                            }
	               }
#endif

                 /* We hope the compiler will lift these out of the loop since they are loop invariant */
                 /* take this out here because doesn't work for indexes */
                    Address_Result = (index_data_ptr_Result[0][ii_0[0]] + Base_Offset_Result[0]) * Result_Descriptor->Stride[0] + Result_Descriptor->Base[0];
#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                         printf ("After %d iterations: Address_Result = %d \n",0,Address_Result);
#endif

                    for (nd=1;nd<MAXDIMS;nd++)
                       {
                         Address_Result += ((index_data_ptr_Result[nd][ii_0[nd]] + Base_Offset_Result[nd]) * Result_Descriptor->Stride[nd] + Result_Descriptor->Base[nd]) * Result_Descriptor->Size[nd-1];

#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                              printf ("After %d iterations: Address_Result = %d \n",nd,Address_Result);
#endif
                       }


                         Result[Address_Result] = (Lhs[Address_Lhs] < 0 && Lhs[Address_Lhs] - (int)Lhs[Address_Lhs] < 0) ? (int)(Lhs[Address_Lhs]-1) : (int)Lhs[Address_Lhs];
                      /* for (nd=1;nd<MAXDIMS;nd++)*/
                         for (nd=0;nd<MAXDIMS;nd++)
                            {
#if MDI_DEBUG
                              if (APP_DEBUG > 10)
                                 {
                                   printf ("ii_1[%d] = %d Stride_Lhs_long[%d] = %d \n",nd,ii_1[nd],nd,Stride_Lhs_long[nd]);
                                   printf ("ii_0[%d] = %d Stride_Result_long[%d] = %d \n",nd,ii_0[nd],nd,Stride_Result_long[nd]);
                                 }
#endif
                              ii_1[nd] += Stride_Lhs_long[nd];
                              ii_0[nd] += Stride_Result_long[nd];
                            }
                       }
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                            {
                              printf ("indr_Offset_Lhs[%d] = %d second = %d indr_Sclstride_Lhs[%d][%d] = %d \n",
                                   nd,indr_Offset_Lhs[nd],second,nd,second,indr_Sclstride_Lhs[nd][second]);
                              printf ("indr_Offset_Result[%d] = %d second = %d indr_Sclstride_Result[%d][%d] = %d \n",
                                   nd,indr_Offset_Result[nd],second,nd,second,indr_Sclstride_Result[nd][second]);
                            }
#endif
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][second];
                         indr_Offset_Result[nd] += indr_Sclstride_Result[nd][second];
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

                         indr_Offset_Result[nd] -= indr_Sclstride_Result[nd][second];
                         indr_Offset_Result[nd] -= indr_Offsetreset_Result[nd][second];
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
	            if (ICounter[ordtempdim] < indr_locbound_Lhs[indirdim][ordtempdim])
                       break;
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         indr_Offset_Lhs[nd] -= indr_Offsetreset_Lhs[nd][ordtempdim];
                         indr_Offset_Result[nd] -= indr_Offsetreset_Result[nd][ordtempdim];
                       }
                    ICounter[ordtempdim] = indr_loclo_Lhs[0][ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=indr_locstride_Lhs[0][ordtempdim];
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][ordtempdim];
                         indr_Offset_Result[nd] += indr_Sclstride_Result[nd][ordtempdim];
                       }
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



/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
             }
            else
             {
               /*--------------------------------------------------------*/

     MDI_ASSERT (Lhs_Descriptor != NULL);
     MDI_ASSERT (Result_Descriptor != NULL);
     Binary_Conformable = Result_Descriptor->Is_Contiguous_Data && 
                          Lhs_Descriptor->Is_Contiguous_Data;

#if MDI_DEBUG
     if (APP_DEBUG)
        {
          for (nd=0;nd<MAXDIMS;nd++)
               printf ("Result_Descriptor->Size[nd] = %d \n",Result_Descriptor->Size[nd]);

          for (nd=0;nd<MAXDIMS;nd++)
               printf ("Lhs_Descriptor->Size[nd] = %d \n",Lhs_Descriptor->Size[nd]);
        }
#endif

               if (Binary_Conformable)
                  {
                    /*--------------------------------------------------------*/

     MDI_ASSERT (Lhs_Descriptor != NULL);

  /* Array_Size is the length of the array access! */
     Array_Size = Lhs_Descriptor->Size[MAXDIMS-1];

#if MDI_DEBUG
     if (APP_DEBUG)
          printf ("Array_Size = %d \n",Array_Size);
#endif

  /* Mostly this is the case for unindexed arrays! */
     for (i1=0; i1 < Array_Size; i1++)
        {

#if MDI_DEBUG
          if (APP_DEBUG) printf ("i1 = %d \n",i1);
#endif

          Result [i1] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];
        }


                  }
                 else
                  {
                    /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

  /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed(quick bug fix 1/23/95, call
       macro below last for Lhs so Lhs value of locdim is used
       later, fix better later) ... */

  /* Macro_Local_Array_Bound_Set_Up(Lhs) */
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



  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

#ifndef NODIMCOMPRESS
  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

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

     numcompress = MDI_Compress_Lhs_Result (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                        loclo_Result,locbound_Result,locsize_Result,locdim_Result,compressible_Result,locndim,longest);

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          numcompress = MDI_Compress_Lhs_Result (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                             loclo_Result,locbound_Result,locsize_Result,locdim_Result,compressible_Result,locndim,second);

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;

          loclo_Lhs[second]     = 0;
          locbound_Lhs[second]  = 0;
          locstride_Lhs[second] = 1;

          Sclstride_Lhs[second]   = 1;
          Offsetreset_Lhs[second] = 0;

          Sclstride_Result[second]   = 1;
          Offsetreset_Result[second] = 0;
        }
#else

     /*------------------------------------------------------------------*/

  /* ... set up Dimorder array when compression and ordering
      are turned off ... */

     for (nd=0;nd<locndim;nd++)
          Dimorder[nd]=nd;
  
     longest = 0;
     second = 1;



     if (locndim == 1)
        {
          loclo_Lhs[second]     = 0;
          locbound_Lhs[second]  = 0;
          locstride_Lhs[second] = 1;

          Sclstride_Lhs[second]   = 1;
          Offsetreset_Lhs[second] = 0;

          Sclstride_Result[second]   = 1;
          Offsetreset_Result[second] = 0;
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


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     Sclstride_Result[0] = locstride_Result[0];
  /* ... check stride because it may have been set to 0 for sum along an axis ... */
     if (locstride_Result[0] !=0)
          Offsetreset_Result[0] = ((locbound_Result[0]-loclo_Result[0])/locstride_Result[0]) * Sclstride_Result[0];
       else
          Offsetreset_Result[0] = 0;

  /*
     Offset_Result = 0;
     for (nd=locndim;nd<MAXDIMS;nd++) 
        {
          Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
        }
  */

  /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */

  /*Offset_Result += loclo_Result[0];*/

  /* ... offsetreset is computed by dividing the range by strides
     and then multiplying by scalestrd instead of just multiplying
     by scale incase the range isn't evenly divisible ... */

  /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

     for (nd=1;nd<locndim;nd++)
        {
          Sclstride_Result[nd] = locsize_Result[nd-1]*locstride_Result[nd];

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (locstride_Result[nd] !=0)
               Offsetreset_Result[nd] = ((locbound_Result[nd]-loclo_Result[nd])/locstride_Result[nd]) * Sclstride_Result[nd];
            else
               Offsetreset_Result[nd] = 0;

       /* Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; */
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

               i0          = Offset_Result;
               Stride_Result_I  = Sclstride_Result[longest];

               for (i1  = Lo_Lhs_I; i1 <= Hi_Lhs_I; i1 += Stride_Lhs_I)
                  {

#if MDI_DEBUG
                    if (APP_DEBUG) printf ("i1 = %d i0 = %d \n",i1,i0);
#endif

                    Result [i0] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];
                    i0 += Stride_Result_I; 
                  }
               Offset_Lhs += Sclstride_Lhs[second];
               Offset_Result += Sclstride_Result[second];
             }

       /* ... if arrays are more than 1d fix offsets, otherwise
          offsets aren't used again so they don't need to be reset ...  */

          if (locndim>1)
             {
            /* ... one extra Sclstride was added at end of j loop so
	       remove that first then reset by the whole amount
	       incremented by going through j loop ... */
               Offset_Lhs -= Sclstride_Lhs[second];
               Offset_Result -= Sclstride_Result[second];

               Offset_Lhs -= Offsetreset_Lhs[second];
               Offset_Result -= Offsetreset_Result[second];
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
                    Offset_Result -= Offsetreset_Result[ordtempdim];
                    ICounter[ordtempdim] = loclo_Lhs[ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=locstride_Lhs[ordtempdim];
                    Offset_Lhs += Sclstride_Lhs[ordtempdim];
                    Offset_Result += Sclstride_Result[ordtempdim];
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

/********************************************************************/


                  }
             }
        }
       else
        {
       /* Using the Mask (Where statement support)! */
          MDI_ASSERT (Mask_Descriptor != NULL);
     MDI_ASSERT (Result_Descriptor != NULL);
     MDI_ASSERT (Lhs_Descriptor != NULL);
     Indirect_Addressing_Used = Mask_Descriptor->Uses_Indirect_Addressing || 
                                Result_Descriptor->Uses_Indirect_Addressing || 
                                Lhs_Descriptor->Uses_Indirect_Addressing;



          if (Indirect_Addressing_Used)
             {
/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
            /* Strides NOT Equal and Bases NOT Equal */
               /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */


          MDI_Indirect_Setup_Lhs_Result_Mask (Lhs_Descriptor,indr_loclo_Lhs,indr_locbound_Lhs,
                                       indr_locstride_Lhs, indr_locsize_Lhs,indr_locdim_Lhs,
                                       indr_compressible_Lhs,indr_Offset_Lhs,indr_Sclstride_Lhs,
                                       indr_Offsetreset_Lhs,index_data_ptr_Lhs,Lhs_Local_Map,Base_Offset_Lhs,
                                       Result_Descriptor,indr_loclo_Result,indr_locbound_Result,
                                       indr_locstride_Result, indr_locsize_Result,indr_locdim_Result,
                                       indr_compressible_Result,indr_Offset_Result,indr_Sclstride_Result,
                                       indr_Offsetreset_Result,index_data_ptr_Result,Result_Local_Map,Base_Offset_Result,
                                       Mask_Descriptor,indr_loclo_Mask,indr_locbound_Mask,
                                       indr_locstride_Mask, indr_locsize_Mask,indr_locdim_Mask,
                                       indr_compressible_Mask,indr_Offset_Mask,indr_Sclstride_Mask,
                                       indr_Offsetreset_Mask,index_data_ptr_Mask,Mask_Local_Map,Base_Offset_Mask,
                                       Dimorder,ICounter,&longest,&second,&locndim,&indirdim);


          offsetrange = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest];
          if (longest >0)
               offsetrange *= indr_locsize_Lhs[indirdim][longest-1];

          do {
               Lo_Lhs_J = indr_loclo_Lhs[indirdim][second];
               Hi_Lhs_J = indr_locbound_Lhs[indirdim][second];
               Stride_Lhs_J = indr_locstride_Lhs[indirdim][second];

               for (j1  = Lo_Lhs_J; j1 <= Hi_Lhs_J; j1 += Stride_Lhs_J)
                  {
                    Lo_Lhs_I = indr_Offset_Lhs[indirdim];
                    Hi_Lhs_I = indr_Offset_Lhs[indirdim] + offsetrange;
                    Stride_Lhs_I = indr_Sclstride_Lhs[indirdim][longest];

                 /* Trap out the case where the stride could be zero in the loop (but allow 
                    indr_Sclstride_Lhs[indirdim][longest] to remain zero! */
                    if (Stride_Lhs_I == 0)
                         Stride_Lhs_I = 1;

                 /* for (nd=1;nd<MAXDIMS;nd++) */
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         ii_1[nd] = indr_Offset_Lhs[nd];
                         Stride_Lhs_long[nd]  = indr_Sclstride_Lhs[nd][longest];

                         ii_0[nd] = indr_Offset_Result[nd];
                         Stride_Result_long[nd]  = indr_Sclstride_Result[nd][longest];

                         ii_3[nd] = indr_Offset_Mask[nd];
                         Stride_Mask_long[nd]  = indr_Sclstride_Mask[nd][longest];
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


                         MDI_ASSERT (Result_Descriptor != NULL);

#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                       {
                         for (nd=0; nd < MAXDIMS; nd++)
                            {
                              printf ("index_data_ptr_Result[%d][ii_0[%d]] = %d ii_0[%d] = %d \n",
                                   nd,nd,index_data_ptr_Result[nd][ii_0[nd]],nd,ii_0[nd]);
                              printf ("Base_Offset_Result[%d]        = %d \n",nd,Base_Offset_Result[nd]);
                              printf ("Result_Descriptor->Stride[%d] = %d \n",nd,Result_Descriptor->Stride[nd]);
                              printf ("Result_Descriptor->Base[%d]   = %d \n",nd,Result_Descriptor->Base[nd]);
                            }
	               }
#endif

                 /* We hope the compiler will lift these out of the loop since they are loop invariant */
                 /* take this out here because doesn't work for indexes */
                    Address_Result = (index_data_ptr_Result[0][ii_0[0]] + Base_Offset_Result[0]) * Result_Descriptor->Stride[0] + Result_Descriptor->Base[0];
#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                         printf ("After %d iterations: Address_Result = %d \n",0,Address_Result);
#endif

                    for (nd=1;nd<MAXDIMS;nd++)
                       {
                         Address_Result += ((index_data_ptr_Result[nd][ii_0[nd]] + Base_Offset_Result[nd]) * Result_Descriptor->Stride[nd] + Result_Descriptor->Base[nd]) * Result_Descriptor->Size[nd-1];

#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                              printf ("After %d iterations: Address_Result = %d \n",nd,Address_Result);
#endif
                       }


                         MDI_ASSERT (Mask_Descriptor != NULL);

#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                       {
                         for (nd=0; nd < MAXDIMS; nd++)
                            {
                              printf ("index_data_ptr_Mask[%d][ii_3[%d]] = %d ii_3[%d] = %d \n",
                                   nd,nd,index_data_ptr_Mask[nd][ii_3[nd]],nd,ii_3[nd]);
                              printf ("Base_Offset_Mask[%d]        = %d \n",nd,Base_Offset_Mask[nd]);
                              printf ("Mask_Descriptor->Stride[%d] = %d \n",nd,Mask_Descriptor->Stride[nd]);
                              printf ("Mask_Descriptor->Base[%d]   = %d \n",nd,Mask_Descriptor->Base[nd]);
                            }
	               }
#endif

                 /* We hope the compiler will lift these out of the loop since they are loop invariant */
                 /* take this out here because doesn't work for indexes */
                    Address_Mask = (index_data_ptr_Mask[0][ii_3[0]] + Base_Offset_Mask[0]) * Mask_Descriptor->Stride[0] + Mask_Descriptor->Base[0];
#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                         printf ("After %d iterations: Address_Mask = %d \n",0,Address_Mask);
#endif

                    for (nd=1;nd<MAXDIMS;nd++)
                       {
                         Address_Mask += ((index_data_ptr_Mask[nd][ii_3[nd]] + Base_Offset_Mask[nd]) * Mask_Descriptor->Stride[nd] + Mask_Descriptor->Base[nd]) * Mask_Descriptor->Size[nd-1];

#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                              printf ("After %d iterations: Address_Mask = %d \n",nd,Address_Mask);
#endif
                       }



                         if (Mask_Array_Pointer[Address_Mask]) Result[Address_Result] = (Lhs[Address_Lhs] < 0 && Lhs[Address_Lhs] - (int)Lhs[Address_Lhs] < 0) ? (int)(Lhs[Address_Lhs]-1) : (int)Lhs[Address_Lhs];

                      /* for (nd=1;nd<MAXDIMS;nd++) */
                         for (nd=0;nd<MAXDIMS;nd++)
                            {
#if MDI_DEBUG
                              if (APP_DEBUG > 10)
                                 {
                                   printf ("ii_1[%d] = %d Stride_Lhs_long[%d] = %d \n",
                                        nd,ii_1[nd],nd,Stride_Lhs_long[nd]);
                                   printf ("ii_0[%d] = %d Stride_Result_long[%d] = %d \n",
                                        nd,ii_0[nd],nd,Stride_Result_long[nd]);
                                   printf ("ii_3[%d] = %d Stride_Mask_long[%d] = %d \n",
                                        nd,ii_3[nd],nd,Stride_Mask_long[nd]);
                                 }
#endif
                              ii_1[nd] += Stride_Lhs_long[nd]; 
                              ii_0[nd] += Stride_Result_long[nd]; 
                              ii_3[nd] += Stride_Mask_long[nd]; 
                            }
                       }
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                            {
                              printf ("indr_Offset_Lhs[%d] = %d second = %d indr_Sclstride_Lhs[%d][%d] = %d \n",
                                   nd,indr_Offset_Lhs[nd],second,nd,second,indr_Sclstride_Lhs[nd][second]);
                              printf ("indr_Offset_Result[%d] = %d second = %d indr_Sclstride_Result[%d][%d] = %d \n",
                                   nd,indr_Offset_Result[nd],second,nd,second,indr_Sclstride_Result[nd][second]);
                              printf ("indr_Offset_Mask[%d] = %d second = %d indr_Sclstride_Mask[%d][%d] = %d \n",
                                   nd,indr_Offset_Mask[nd],second,nd,second,indr_Sclstride_Mask[nd][second]);
                            }
#endif
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][second];
                         indr_Offset_Result[nd] += indr_Sclstride_Result[nd][second];
                         indr_Offset_Mask[nd] += indr_Sclstride_Mask[nd][second];
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

                         indr_Offset_Result[nd] -= indr_Sclstride_Result[nd][second];
                         indr_Offset_Result[nd] -= indr_Offsetreset_Result[nd][second];

                         indr_Offset_Mask[nd] -= indr_Sclstride_Mask[nd][second];
                         indr_Offset_Mask[nd] -= indr_Offsetreset_Mask[nd][second];
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
                            {
                              indr_Offset_Lhs[nd] -= indr_Offsetreset_Lhs[nd][ordtempdim];
                              indr_Offset_Result[nd] -= indr_Offsetreset_Result[nd][ordtempdim];
                              indr_Offset_Mask[nd] -= indr_Offsetreset_Mask[nd][ordtempdim];
                            }
                         ICounter[ordtempdim] = indr_loclo_Lhs[indirdim][ordtempdim];
                       }

                    if (tempdim<locndim)
                       {
                      /* ... dimension ordtempdim isn't at its max ... */
                         ordtempdim = Dimorder[tempdim];
                         ICounter[ordtempdim]+=indr_locstride_Lhs[indirdim][ordtempdim];
                         for (nd=0;nd<MAXDIMS;nd++)
                            {
                              indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][ordtempdim];
                              indr_Offset_Result[nd] += indr_Sclstride_Result[nd][ordtempdim];
                              indr_Offset_Mask[nd] += indr_Sclstride_Mask[nd][ordtempdim];
                            }
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



/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
             }
            else
             {
               /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

  /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

  /* ... store local copy of array bounds so they can be reordered and compressed ... */

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



#ifndef NODIMCOMPRESS

  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

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

     numcompress = MDI_Compress_Lhs_Result_Mask (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                             loclo_Result,locbound_Result,locsize_Result,locdim_Result,compressible_Result,
                                             loclo_Mask,locbound_Mask,locsize_Mask,locdim_Mask,compressible_Mask,locndim,longest);

     locndim -= numcompress;
     if (second >longest)
        second-= numcompress;

     if (second>=0)
        {
          numcompress = MDI_Compress_Lhs_Result_Mask (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                                  loclo_Result,locbound_Result,locsize_Result,locdim_Result,compressible_Result,
                                                  loclo_Mask,locbound_Mask,locsize_Mask,locdim_Mask,compressible_Mask,locndim,second);

          locndim -= numcompress;
          if (longest>second ) longest-= numcompress;
        }
       else
        {
          second = 1;

          loclo_Lhs[second]=0;
          locbound_Lhs[second]=0;
          locstride_Lhs[second]=1;

          Sclstride_Lhs[second]=1;
          Offsetreset_Lhs[second]=0;

          Sclstride_Result[second]=1;
          Offsetreset_Result[second]=0;

          Sclstride_Mask[second]=1;
          Offsetreset_Mask[second]=0;
        }

#else
     /*------------------------------------------------------------------*/

  /* ... set up Dimorder array when compression and ordering
      are turned off ... */

     for (nd=0;nd<locndim;nd++)
          Dimorder[nd]=nd;
  
     longest = 0;
     second = 1;



     if (locndim == 1)
        {
          loclo_Lhs[second]=0;
          locbound_Lhs[second]=0;
          locstride_Lhs[second]=1;

          Sclstride_Lhs[second]=1;
          Offsetreset_Lhs[second]=0;

          Sclstride_Result[second]=1;
          Offsetreset_Result[second]=0;

          Sclstride_Mask[second]=1;
          Offsetreset_Mask[second]=0;
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


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     Sclstride_Result[0] = locstride_Result[0];
  /* ... check stride because it may have been set to 0 for sum along an axis ... */
     if (locstride_Result[0] !=0)
          Offsetreset_Result[0] = ((locbound_Result[0]-loclo_Result[0])/locstride_Result[0]) * Sclstride_Result[0];
       else
          Offsetreset_Result[0] = 0;

  /*
     Offset_Result = 0;
     for (nd=locndim;nd<MAXDIMS;nd++) 
        {
          Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; 
        }
  */

  /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */

  /*Offset_Result += loclo_Result[0];*/

  /* ... offsetreset is computed by dividing the range by strides
     and then multiplying by scalestrd instead of just multiplying
     by scale incase the range isn't evenly divisible ... */

  /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

     for (nd=1;nd<locndim;nd++)
        {
          Sclstride_Result[nd] = locsize_Result[nd-1]*locstride_Result[nd];

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (locstride_Result[nd] !=0)
               Offsetreset_Result[nd] = ((locbound_Result[nd]-loclo_Result[nd])/locstride_Result[nd]) * Sclstride_Result[nd];
            else
               Offsetreset_Result[nd] = 0;

       /* Offset_Result += loclo_Result[nd]*locsize_Result[nd-1]; */
        }


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     Sclstride_Mask[0] = locstride_Mask[0];
  /* ... check stride because it may have been set to 0 for sum along an axis ... */
     if (locstride_Mask[0] !=0)
          Offsetreset_Mask[0] = ((locbound_Mask[0]-loclo_Mask[0])/locstride_Mask[0]) * Sclstride_Mask[0];
       else
          Offsetreset_Mask[0] = 0;

  /*
     Offset_Mask = 0;
     for (nd=locndim;nd<MAXDIMS;nd++) 
        {
          Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
        }
  */

  /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */

  /*Offset_Mask += loclo_Mask[0];*/

  /* ... offsetreset is computed by dividing the range by strides
     and then multiplying by scalestrd instead of just multiplying
     by scale incase the range isn't evenly divisible ... */

  /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

     for (nd=1;nd<locndim;nd++)
        {
          Sclstride_Mask[nd] = locsize_Mask[nd-1]*locstride_Mask[nd];

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (locstride_Mask[nd] !=0)
               Offsetreset_Mask[nd] = ((locbound_Mask[nd]-loclo_Mask[nd])/locstride_Mask[nd]) * Sclstride_Mask[nd];
            else
               Offsetreset_Mask[nd] = 0;

       /* Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; */
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

               i0          = Offset_Result;
               Stride_Result_I  = Sclstride_Result[longest];

               i3          = Offset_Mask;
               Stride_Mask_I  = Sclstride_Mask[longest];

               for (i1  = Lo_Lhs_I; i1 <= Hi_Lhs_I; i1 += Stride_Lhs_I)
                  {

#if MDI_DEBUG
                    if (APP_DEBUG) printf ("i1 = %d i0 = %d i3 = %d \n",i1,i0,i3);
#endif

                    if (Mask_Array_Pointer [i3]) Result [i0] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];

                    i0 += Stride_Result_I; 
                    i3 += Stride_Mask_I; 
                  }
               Offset_Lhs += Sclstride_Lhs[second];
               Offset_Result += Sclstride_Result[second];
               Offset_Mask += Sclstride_Mask[second];
             }


       /* ... if arrays are more than 1d fix offsets, otherwise
          offsets aren't used again so they don't need to be reset ...  */
          if (locndim>1)
             {
            /* ... one extra Sclstride was added at end of j loop so
	       remove that first then reset by the whole amount
	       incremented by going through j loop ... */
               Offset_Lhs -= Sclstride_Lhs[second];
               Offset_Result -= Sclstride_Result[second];
               Offset_Mask -= Sclstride_Mask[second];

               Offset_Lhs -= Offsetreset_Lhs[second];
               Offset_Result -= Offsetreset_Result[second];
               Offset_Mask -= Offsetreset_Mask[second];
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
                    Offset_Result -= Offsetreset_Result[ordtempdim];
                    Offset_Mask -= Offsetreset_Mask[ordtempdim];
                    ICounter[ordtempdim] = loclo_Lhs[ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=locstride_Lhs[ordtempdim];
                    Offset_Lhs += Sclstride_Lhs[ordtempdim];
                    Offset_Result += Sclstride_Result[ordtempdim];
                    Offset_Mask += Sclstride_Mask[ordtempdim];
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

/********************************************************************/


             }
        }
   }
 
/*-------------------------------------------------------------------*/

void MDI_i_Floor_Array_Accumulate_To_Operand
          ( int *Lhs ,
            int *Mask_Array_Pointer ,
            array_domain *Lhs_Descriptor ,
            array_domain *Mask_Descriptor )
   {
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


     /*--------------------------------------------------------*/

  /* indirect addressing variables */

     int indr_loclo_Mask       [MAXDIMS][MAXDIMS];
     int indr_locbound_Mask    [MAXDIMS][MAXDIMS];
     int indr_locsize_Mask     [MAXDIMS][MAXDIMS];
     int indr_locstride_Mask   [MAXDIMS][MAXDIMS];
     int indr_locdim_Mask      [MAXDIMS][MAXDIMS];
     int indr_compressible_Mask[MAXDIMS][MAXDIMS];

     int indr_Offset_Mask      [MAXDIMS];
     int indr_Offsetreset_Mask [MAXDIMS][MAXDIMS];
     int indr_Sclstride_Mask   [MAXDIMS][MAXDIMS];

     int Stride_Mask_long      [MAXDIMS];

     int ii_3               [MAXDIMS];

     int Address_Mask;
     int* index_data_ptr_Mask  [MAXDIMS];
     int Mask_Local_Map        [MAXDIMS];         
     int Base_Offset_Mask      [MAXDIMS];
     
  /*--------------------------------------------------------*/
  /* ... new variables ...                                  */

     int Lo_Mask_I;int Hi_Mask_I;int Stride_Mask_I;
     int Lo_Mask_J;int Hi_Mask_J;int Stride_Mask_J;

     int Offset_Mask;

     int compressible_Mask[MAXDIMS];

     int locdim_Mask      [MAXDIMS];
     int locsize_Mask     [MAXDIMS];
     int loclo_Mask       [MAXDIMS];
     int locbound_Mask    [MAXDIMS];
     int locstride_Mask   [MAXDIMS];

     int Sclstride_Mask   [MAXDIMS];
     int Offsetreset_Mask [MAXDIMS];

  /* Loop index variables */
     int i3;int j3;

  /* Used to store min base and max bound for Mask */
     int Minimum_Base_For_Indirect_Addressing_Mask;
     int Maximum_Bound_For_Indirect_Addressing_Mask;


 
     /*--------------------------------------------------------*/

#if MDI_DEBUG

     Binary_Conformable = -1;

  /* We might decide to store a single value if the strides are constant along each axis.
  // We should also isolate the cases where the strides are one!
  */
     Lhs_Rhs_Strides_Equal    = -1;
     Result_Lhs_Strides_Equal = -1;
     Strides_Equal            = -1;
     Unit_Stride              = -1;
     Array_Size               = -1;

  /* We might decide to store a single value if the bases are constant along each axis. */
     Lhs_Rhs_Bases_Equal    = -1;
     Result_Lhs_Bases_Equal = -1;
     Bases_Equal            = -1;

  /* We need to test if the block sizes are the same too! */
     Lhs_Rhs_Dimensions_Equal    = -1;
     Result_Lhs_Dimensions_Equal = -1;
     Dimensions_Equal            = -1;
  /*--------------------------------------------------------*/

  /* ... old variables still used ...                       */
     Indirect_Addressing_Used              = -1;
     Minimum_Base_For_Indirect_Addressing  = -1;
     Maximum_Bound_For_Indirect_Addressing = -1;
  /*--------------------------------------------------------*/

  /* ... new variables ...                                  */
     nd  = -1;
     nd2 = -1;
     cnt = -1;

     tempdim    = -1;
     nextdim    = -1;
     ordtempdim = -1;
     locndim    = -1;
     length     = -1;
     longest    = -1; 
     second     = -1;
     longlen    = -1;
     secondlen  = -1;

     numcompress = -1;
     notcompress = -1;
     offsetrange = -1;

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          Dimorder       [i_debug] = -1;
          ICounter       [i_debug] = -1;
          max_dim_length [i_debug] = -1;
        }

     ICounter [MAXDIMS] = -1;

     locndimtemp = -1;
     indirdim    = -1;

  /* Variables used for initialization and debugging mode */
     i_debug = -1;
     j_debug = -1;

#endif

     Minimum_Base_For_Indirect_Addressing  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing = INT_MIN;


     /*--------------------------------------------------------*/

#if MDI_DEBUG

  /* Initialize local storage */
     Address_Lhs  = 0;
     Lo_Lhs_I     = 0;
     Hi_Lhs_I     = 0;
     Stride_Lhs_I = 0;
     Lo_Lhs_J     = 0;
     Hi_Lhs_J     = 0;
     Stride_Lhs_J = 0;
     Offset_Lhs   = 0;

  /* initialize indirect addressing variables */

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          for (j_debug=0; j_debug < MAXDIMS; j_debug++)
             {
               indr_loclo_Lhs        [i_debug][j_debug] = -1;
               indr_locbound_Lhs     [i_debug][j_debug] = -1;
               indr_locsize_Lhs      [i_debug][j_debug] = -1;
               indr_locstride_Lhs    [i_debug][j_debug] = -1;
               indr_locdim_Lhs       [i_debug][j_debug] = -1;
               indr_compressible_Lhs [i_debug][j_debug] = -1;

               indr_Offsetreset_Lhs  [i_debug][j_debug] = -1;
               indr_Sclstride_Lhs    [i_debug][j_debug] = -1;
             }

          indr_Offset_Lhs    [i_debug] = -1;
          Stride_Lhs_long    [i_debug] = -1;
          ii_1              [i_debug] = -1;

          index_data_ptr_Lhs [i_debug] = NULL;
          Lhs_Local_Map      [i_debug] = -1;         
          Base_Offset_Lhs    [i_debug] = -1;
          compressible_Lhs   [i_debug] = -1;
          locdim_Lhs         [i_debug] = -1;
          locsize_Lhs        [i_debug] = -1;
          loclo_Lhs          [i_debug] = -1;
          locbound_Lhs       [i_debug] = -1;
          locstride_Lhs      [i_debug] = -1;

          Sclstride_Lhs      [i_debug] = -1;
          Offsetreset_Lhs    [i_debug] = -1;
        }

#endif

  /* Used to store min base and max bound for Lhs */
     Minimum_Base_For_Indirect_Addressing_Lhs  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Lhs = INT_MIN;


     /*--------------------------------------------------------*/

#if MDI_DEBUG

  /* Initialize local storage */
     Address_Mask  = 0;
     Lo_Mask_I     = 0;
     Hi_Mask_I     = 0;
     Stride_Mask_I = 0;
     Lo_Mask_J     = 0;
     Hi_Mask_J     = 0;
     Stride_Mask_J = 0;
     Offset_Mask   = 0;

  /* initialize indirect addressing variables */

     for (i_debug=0; i_debug < MAXDIMS; i_debug++)
        {
          for (j_debug=0; j_debug < MAXDIMS; j_debug++)
             {
               indr_loclo_Mask        [i_debug][j_debug] = -1;
               indr_locbound_Mask     [i_debug][j_debug] = -1;
               indr_locsize_Mask      [i_debug][j_debug] = -1;
               indr_locstride_Mask    [i_debug][j_debug] = -1;
               indr_locdim_Mask       [i_debug][j_debug] = -1;
               indr_compressible_Mask [i_debug][j_debug] = -1;

               indr_Offsetreset_Mask  [i_debug][j_debug] = -1;
               indr_Sclstride_Mask    [i_debug][j_debug] = -1;
             }

          indr_Offset_Mask    [i_debug] = -1;
          Stride_Mask_long    [i_debug] = -1;
          ii_1              [i_debug] = -1;

          index_data_ptr_Mask [i_debug] = NULL;
          Mask_Local_Map      [i_debug] = -1;         
          Base_Offset_Mask    [i_debug] = -1;
          compressible_Mask   [i_debug] = -1;
          locdim_Mask         [i_debug] = -1;
          locsize_Mask        [i_debug] = -1;
          loclo_Mask          [i_debug] = -1;
          locbound_Mask       [i_debug] = -1;
          locstride_Mask      [i_debug] = -1;

          Sclstride_Mask      [i_debug] = -1;
          Offsetreset_Mask    [i_debug] = -1;
        }

#endif

  /* Used to store min base and max bound for Mask */
     Minimum_Base_For_Indirect_Addressing_Mask  = INT_MAX;
     Maximum_Bound_For_Indirect_Addressing_Mask = INT_MIN;



#if MDI_DEBUG
     if (APP_DEBUG > 0)
          printf ("Inside of MDI_i_Floor_Array_Accumulate_To_Operand \n");
#endif

     if (Mask_Array_Pointer == NULL)
        {
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
/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
            /* Strides NOT Equal and Bases NOT Equal */
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


                         Lhs[Address_Lhs] = (Lhs[Address_Lhs] < 0 && Lhs[Address_Lhs] - (int)Lhs[Address_Lhs] < 0) ? (int)(Lhs[Address_Lhs]-1) : (int)Lhs[Address_Lhs];
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


/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
             }
            else
             {
               /*--------------------------------------------------------*/

     MDI_ASSERT (Lhs_Descriptor != NULL);

  /* This data is available in the descriptor without computing it here! */
     Binary_Conformable = Lhs_Descriptor->Is_Contiguous_Data; 

  /* In this case we have to explicitly check each Stride to see if it is a Unit stride! */
     Unit_Stride = 1;
     for (nd=0;nd<MAXDIMS;nd++)
          if ( Lhs_Descriptor->Stride[nd] != 1) 
             {
               Unit_Stride = 0;
               break;
             }

#if MDI_DEBUG
     if (APP_DEBUG)
        {
          for (nd=0;nd<MAXDIMS;nd++)
               printf ("Lhs_Descriptor->Size[nd] = %d \n",Lhs_Descriptor->Size[nd]);
        }
#endif

               if (Binary_Conformable)
                  {
                    /*--------------------------------------------------------*/

     MDI_ASSERT (Lhs_Descriptor != NULL);

  /* Array_Size is the length of the array access! */
     Array_Size = Lhs_Descriptor->Size[MAXDIMS-1];

#if MDI_DEBUG
     if (APP_DEBUG)
          printf ("Array_Size = %d \n",Array_Size);
#endif

  /* Mostly this is the case for unindexed arrays! */
     for (i1=0; i1 < Array_Size; i1++)
        {

#if MDI_DEBUG
          if (APP_DEBUG) printf ("i1 = %d \n",i1);
#endif

          Lhs [i1] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];
        }


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

                    Lhs [i1] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];
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
        }
       else
        {
       /* Using the Mask (Where statement support)! */
          MDI_ASSERT (Mask_Descriptor != NULL);
     MDI_ASSERT (Lhs_Descriptor != NULL);
     Indirect_Addressing_Used = Mask_Descriptor ->Uses_Indirect_Addressing 
 			        || Lhs_Descriptor ->Uses_Indirect_Addressing;



          if (Indirect_Addressing_Used)
             {
/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
            /* Strides NOT Equal and Bases NOT Equal */
               /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

          Minimum_Base_For_Indirect_Addressing  = INT_MAX;
          Maximum_Bound_For_Indirect_Addressing = INT_MIN;

          MDI_Indirect_Setup_Lhs_Mask (Lhs_Descriptor,indr_loclo_Lhs,indr_locbound_Lhs,
                                     indr_locstride_Lhs, indr_locsize_Lhs,indr_locdim_Lhs,
                                     indr_compressible_Lhs,indr_Offset_Lhs,indr_Sclstride_Lhs,
                                     indr_Offsetreset_Lhs,index_data_ptr_Lhs,Lhs_Local_Map,Base_Offset_Lhs,
                                     Mask_Descriptor,indr_loclo_Mask,indr_locbound_Mask,
                                     indr_locstride_Mask, indr_locsize_Mask,indr_locdim_Mask,
                                     indr_compressible_Mask,indr_Offset_Mask,indr_Sclstride_Mask,
                                     indr_Offsetreset_Mask,index_data_ptr_Mask,Mask_Local_Map,Base_Offset_Mask,
                                     Dimorder,ICounter,&longest,&second,&locndim,&indirdim);


          offsetrange = indr_locbound_Lhs[indirdim][longest] - indr_loclo_Lhs[indirdim][longest];
          if (longest >0)
               offsetrange *= indr_locsize_Lhs[indirdim][longest-1];

          do {
               Lo_Lhs_J = indr_loclo_Lhs[indirdim][second];
               Hi_Lhs_J = indr_locbound_Lhs[indirdim][second];
               Stride_Lhs_J = indr_locstride_Lhs[indirdim][second];

               for (j1  = Lo_Lhs_J; j1 <= Hi_Lhs_J; j1 += Stride_Lhs_J)
                  {

                    Lo_Lhs_I = indr_Offset_Lhs[indirdim];
                    Hi_Lhs_I = indr_Offset_Lhs[indirdim] + offsetrange;
                    Stride_Lhs_I = indr_Sclstride_Lhs[indirdim][longest];

                 /* Trap out the case where the stride could be zero in the loop (but allow 
                    indr_Sclstride_Lhs[indirdim][longest] to remain zero! */
                    if (Stride_Lhs_I == 0)
                         Stride_Lhs_I = 1;

                 /* for (nd=1;nd<MAXDIMS;nd++) */
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         ii_1[nd] = indr_Offset_Lhs[nd];
                         Stride_Lhs_long[nd]  = indr_Sclstride_Lhs[nd][longest];

                         ii_3[nd] = indr_Offset_Mask[nd];
                         Stride_Mask_long[nd]  = indr_Sclstride_Mask[nd][longest];
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


                         MDI_ASSERT (Mask_Descriptor != NULL);

#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                       {
                         for (nd=0; nd < MAXDIMS; nd++)
                            {
                              printf ("index_data_ptr_Mask[%d][ii_3[%d]] = %d ii_3[%d] = %d \n",
                                   nd,nd,index_data_ptr_Mask[nd][ii_3[nd]],nd,ii_3[nd]);
                              printf ("Base_Offset_Mask[%d]        = %d \n",nd,Base_Offset_Mask[nd]);
                              printf ("Mask_Descriptor->Stride[%d] = %d \n",nd,Mask_Descriptor->Stride[nd]);
                              printf ("Mask_Descriptor->Base[%d]   = %d \n",nd,Mask_Descriptor->Base[nd]);
                            }
	               }
#endif

                 /* We hope the compiler will lift these out of the loop since they are loop invariant */
                 /* take this out here because doesn't work for indexes */
                    Address_Mask = (index_data_ptr_Mask[0][ii_3[0]] + Base_Offset_Mask[0]) * Mask_Descriptor->Stride[0] + Mask_Descriptor->Base[0];
#if MDI_DEBUG
                    if (APP_DEBUG > 10)
                         printf ("After %d iterations: Address_Mask = %d \n",0,Address_Mask);
#endif

                    for (nd=1;nd<MAXDIMS;nd++)
                       {
                         Address_Mask += ((index_data_ptr_Mask[nd][ii_3[nd]] + Base_Offset_Mask[nd]) * Mask_Descriptor->Stride[nd] + Mask_Descriptor->Base[nd]) * Mask_Descriptor->Size[nd-1];

#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                              printf ("After %d iterations: Address_Mask = %d \n",nd,Address_Mask);
#endif
                       }


                         if (Mask_Array_Pointer[Address_Mask]) Lhs[Address_Lhs] = (Lhs[Address_Lhs] < 0 && Lhs[Address_Lhs] - (int)Lhs[Address_Lhs] < 0) ? (int)(Lhs[Address_Lhs]-1) : (int)Lhs[Address_Lhs];
                      /* for (nd=1;nd<MAXDIMS;nd++)*/
                         for (nd=0;nd<MAXDIMS;nd++)
                            {
#if MDI_DEBUG
                              if (APP_DEBUG > 10)
                                 {
                                   printf ("ii_1[%d] = %d Stride_Lhs_long[%d] = %d \n",nd,ii_1[nd],nd,Stride_Lhs_long[nd]);
                                   printf ("ii_3[%d] = %d Stride_Mask_long[%d] = %d \n",nd,ii_3[nd],nd,Stride_Mask_long[nd]);
                                 }
#endif
                              ii_1[nd] += Stride_Lhs_long[nd];
                              ii_3[nd] += Stride_Mask_long[nd];
                            }
                       }
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
#if MDI_DEBUG
                         if (APP_DEBUG > 10)
                            {
                              printf ("indr_Offset_Lhs[%d] = %d second = %d indr_Sclstride_Lhs[%d][%d] = %d \n",
                                   nd,indr_Offset_Lhs[nd],second,nd,second,indr_Sclstride_Lhs[nd][second]);
                              printf ("indr_Offset_Mask[%d] = %d second = %d indr_Sclstride_Mask[%d][%d] = %d \n",
                                   nd,indr_Offset_Mask[nd],second,nd,second,indr_Sclstride_Mask[nd][second]);
                            }
#endif
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][second];
                         indr_Offset_Mask[nd] += indr_Sclstride_Mask[nd][second];
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

                         indr_Offset_Mask[nd] -= indr_Sclstride_Mask[nd][second];
                         indr_Offset_Mask[nd] -= indr_Offsetreset_Mask[nd][second];
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
	            if (ICounter[ordtempdim] < indr_locbound_Lhs[indirdim][ordtempdim])
                       break;
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         indr_Offset_Lhs[nd] -= indr_Offsetreset_Lhs[nd][ordtempdim];
                         indr_Offset_Mask[nd] -= indr_Offsetreset_Mask[nd][ordtempdim];
                       }
                    ICounter[ordtempdim] = indr_loclo_Lhs[0][ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=indr_locstride_Lhs[0][ordtempdim];
                    for (nd=0;nd<MAXDIMS;nd++)
                       {
                         indr_Offset_Lhs[nd] += indr_Sclstride_Lhs[nd][ordtempdim];
                         indr_Offset_Mask[nd] += indr_Sclstride_Mask[nd][ordtempdim];
                       }
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



/* iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii*/
             }
            else
             {
               /*------------------------------------------------------------------*/

  /* ... Some examples of parameter definitions are: 
       $ 1 <==> Lhs[i1] = Rhs[i1], $ 2 <==> Rhs, $ 3 <==> 2,
       $ 4 <==> ' ' ...  */

  /*xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx*/

  /* ... store local copy of array bounds so they can be 
       reordered and compressed(quick bug fix 1/23/95, call
       macro below last for Lhs so Lhs value of locdim is used
       later, fix better later) ... */

  /* Macro_Local_Array_Bound_Set_Up(Lhs) */
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



  /* ... possible fix compressibility and stride for operations such as sum along an axis ... */
     

#ifndef NODIMCOMPRESS
  /* ... turn off compressibility if both arrays aren't compressible in the same dimension ... */

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

     numcompress = MDI_Compress_Lhs_Mask (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                        loclo_Mask,locbound_Mask,locsize_Mask,locdim_Mask,compressible_Mask,locndim,longest);

     locndim -= numcompress;
     if (second >longest)
          second-= numcompress;

     if (second>=0)
        {
          numcompress = MDI_Compress_Lhs_Mask (loclo_Lhs,locbound_Lhs,locsize_Lhs,locdim_Lhs,compressible_Lhs,
                                             loclo_Mask,locbound_Mask,locsize_Mask,locdim_Mask,compressible_Mask,locndim,second);

          locndim -= numcompress;
          if (longest>second )
               longest-= numcompress;
        }
       else
        {
          second = 1;

          loclo_Lhs[second]     = 0;
          locbound_Lhs[second]  = 0;
          locstride_Lhs[second] = 1;

          Sclstride_Lhs[second]   = 1;
          Offsetreset_Lhs[second] = 0;

          Sclstride_Mask[second]   = 1;
          Offsetreset_Mask[second] = 0;
        }
#else

     /*------------------------------------------------------------------*/

  /* ... set up Dimorder array when compression and ordering
      are turned off ... */

     for (nd=0;nd<locndim;nd++)
          Dimorder[nd]=nd;
  
     longest = 0;
     second = 1;



     if (locndim == 1)
        {
          loclo_Lhs[second]     = 0;
          locbound_Lhs[second]  = 0;
          locstride_Lhs[second] = 1;

          Sclstride_Lhs[second]   = 1;
          Offsetreset_Lhs[second] = 0;

          Sclstride_Mask[second]   = 1;
          Offsetreset_Mask[second] = 0;
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


     /*------------------------------------------------------------------*/

  /* ... set up some arrays for computing offsets ... */

     Sclstride_Mask[0] = locstride_Mask[0];
  /* ... check stride because it may have been set to 0 for sum along an axis ... */
     if (locstride_Mask[0] !=0)
          Offsetreset_Mask[0] = ((locbound_Mask[0]-loclo_Mask[0])/locstride_Mask[0]) * Sclstride_Mask[0];
       else
          Offsetreset_Mask[0] = 0;

  /*
     Offset_Mask = 0;
     for (nd=locndim;nd<MAXDIMS;nd++) 
        {
          Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; 
        }
  */

  /* ... this must account for dimensions that are length 1 and above the highest dimension used ... */

  /*Offset_Mask += loclo_Mask[0];*/

  /* ... offsetreset is computed by dividing the range by strides
     and then multiplying by scalestrd instead of just multiplying
     by scale incase the range isn't evenly divisible ... */

  /* vectorization slows this loop down ... */
#if CRAY
#  pragma _CRI novector
#endif

     for (nd=1;nd<locndim;nd++)
        {
          Sclstride_Mask[nd] = locsize_Mask[nd-1]*locstride_Mask[nd];

       /* ... check stride because it may have been set to 0 for sum along an axis ... */
          if (locstride_Mask[nd] !=0)
               Offsetreset_Mask[nd] = ((locbound_Mask[nd]-loclo_Mask[nd])/locstride_Mask[nd]) * Sclstride_Mask[nd];
            else
               Offsetreset_Mask[nd] = 0;

       /* Offset_Mask += loclo_Mask[nd]*locsize_Mask[nd-1]; */
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

               i3          = Offset_Mask;
               Stride_Mask_I  = Sclstride_Mask[longest];

               for (i1  = Lo_Lhs_I; i1 <= Hi_Lhs_I; i1 += Stride_Lhs_I)
                  {

#if MDI_DEBUG
                    if (APP_DEBUG) printf ("i1 = %d i3 = %d \n",i1,i3);
#endif

                    if (Mask_Array_Pointer[i3]) Lhs [i1] = (Lhs [i1] < 0 && Lhs [i1] - (int)Lhs [i1] < 0) ? (int)(Lhs [i1]-1) : (int)Lhs [i1];
                    i3 += Stride_Mask_I; 
                  }
               Offset_Lhs += Sclstride_Lhs[second];
               Offset_Mask += Sclstride_Mask[second];
             }

       /* ... if arrays are more than 1d fix offsets, otherwise
          offsets aren't used again so they don't need to be reset ...  */

          if (locndim>1)
             {
            /* ... one extra Sclstride was added at end of j loop so
	       remove that first then reset by the whole amount
	       incremented by going through j loop ... */
               Offset_Lhs -= Sclstride_Lhs[second];
               Offset_Mask -= Sclstride_Mask[second];

               Offset_Lhs -= Offsetreset_Lhs[second];
               Offset_Mask -= Offsetreset_Mask[second];
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
                    Offset_Mask -= Offsetreset_Mask[ordtempdim];
                    ICounter[ordtempdim] = loclo_Lhs[ordtempdim];
                  }

               if (tempdim<locndim)
                  {
                 /* ... dimension ordtempdim isn't at its max ... */
                    ordtempdim = Dimorder[tempdim];
                    ICounter[ordtempdim]+=locstride_Lhs[ordtempdim];
                    Offset_Lhs += Sclstride_Lhs[ordtempdim];
                    Offset_Mask += Sclstride_Mask[ordtempdim];
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

/********************************************************************/


             }
        }
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




