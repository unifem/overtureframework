#define COMPILE_PPP

#include "A++.h"

#define FILE_LEVEL_DEBUG 0

// Use the new code specific for the new version of P++
#define USE_NEW_CODE TRUE

// min and max macros used in code below
#define MAX(A,B) ( (A) > (B) ) ? (A) : (B);
#define MIN(A,B) ( (A) < (B) ) ? (A) : (B);
 
/*
// Code Changes: The middle processor code has been drastically
// changed.  It now uses Left_Number_Of_Points and Right_Number_Of_Points
// like the left and right processors did previously.  The
// Overlap_Update functions no longer modify the Partition_Indexes.
// Instead the required change to the Lhs_Partition_Index is stored
// in Return_Array_Set.  If the required change to Lhs_Partition_Index
// is not as great as a previous required change, then Return_Array_Set
// isn't modified.  The Rhs_Partition_Index isn't modified because
// the modification determined here might not produce a conformable
// SerialArray if there are more than 2 SerialArrays that need to be
// made conformable.  Instead, Overlap_Update_Fix_Rhs has been added
// to this file to modify the Rhs_Partition_Indexes after the minimal
// overlapping region for all arrays has been determined.  Another
// change is that the names of the Support_For_Overlap_Update functions
// have been chaanged to just Overlap_Update functions.  This is
// because the previous functions with those names have been removed.
// They no longer had any purpose except to call the Support functions
// just adding another function call layer.
*/




//##########################################################################
//             Cases for within Overlap Update
//##########################################################################

int Array_Domain_Type::Overlap_Update_Case_Left_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Lhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
          const Array_Domain_Type       & Rhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Rhs_Serial_Descriptor ,
          Internal_Index *Lhs_Partition_Index,
          Internal_Index *Rhs_Partition_Index,
          int Axis )
{
   // This macro forces the setup to be the same for each of the Left 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   if ((Lhs_Partition_Index[Axis].getMode()!=Null_Index)&&
       (Rhs_Partition_Index[Axis].getMode()!=Null_Index))
   {
   //int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int Lhs_gbase  = Lhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Lhs_gbound = Lhs_Parallel_Descriptor.Global_Index[Axis].getBound();
   int Rhs_gbase  = Rhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Rhs_gbound = Rhs_Parallel_Descriptor.Global_Index[Axis].getBound();

   int Lhs_ghost_cell_width = Lhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int Rhs_ghost_cell_width = Rhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];

   // ... offset of local view from global view ...
   int Lhs_Left_Number_Of_Points  =  
      Lhs_Partition_Index [Axis].getBase() - Lhs_gbase;
   int Lhs_Right_Number_Of_Points =  
      Lhs_gbound - Lhs_Partition_Index [Axis].getBound();
   int Rhs_Left_Number_Of_Points  = 
      Rhs_Partition_Index [Axis].getBase() - Rhs_gbase;
   int Rhs_Right_Number_Of_Points = 
      Rhs_gbound - Rhs_Partition_Index [Axis].getBound();

   // ... store unscaled versions of these ...
   int Lhs_Left_Number_Of_Points_unscaled  = Lhs_Left_Number_Of_Points;
   int Lhs_Right_Number_Of_Points_unscaled = Lhs_Right_Number_Of_Points;
   int Rhs_Left_Number_Of_Points_unscaled  = Rhs_Left_Number_Of_Points;
   int Rhs_Right_Number_Of_Points_unscaled = Rhs_Right_Number_Of_Points;

   // ... bug fix (10/15/96,kdb) dividing by both Stride_Factor and
   //  by stride causes problems and is confusing.  Simplify by 
   //  rescaling by stride immediately ...

   int Lhs_stride = Lhs_Partition_Index [Axis].getStride();
   int Rhs_stride = Rhs_Partition_Index [Axis].getStride();

   Lhs_Left_Number_Of_Points = (Lhs_Left_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Lhs_Right_Number_Of_Points = (Lhs_Right_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Rhs_Left_Number_Of_Points = (Rhs_Left_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;
   Rhs_Right_Number_Of_Points = (Rhs_Right_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;

   APP_ASSERT(Lhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Lhs_Right_Number_Of_Points >= 0);
   APP_ASSERT(Rhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Rhs_Right_Number_Of_Points >= 0);


   // *wdh* 090307 
   DOMAIN_SIZE_Type Rhs_Size = Rhs_Serial_Descriptor.Size[Axis]; 
   DOMAIN_SIZE_Type Lhs_Size = Lhs_Serial_Descriptor.Size[Axis]; 
   if (Axis>0)
   {
      Rhs_Size /= Rhs_Serial_Descriptor.Size[Axis-1]; 
      Lhs_Size /= Lhs_Serial_Descriptor.Size[Axis-1]; 
   }

   int Lhs_start_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size
       - 2 * Lhs_ghost_cell_width;
   int Lhs_end_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size -1;

   int Rhs_start_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size
       - 2 * Rhs_ghost_cell_width;
   int Rhs_end_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size -1;


   int Lhs_start_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis];
   int Lhs_end_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Lhs_ghost_cell_width;

   int Rhs_start_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis];
   int Rhs_end_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Rhs_ghost_cell_width;


   // ... WARNING: stride > 1 needs to be tested ...

   // ... note (10/15/96,kdb) don't need this anymore ...

   int Stride_Factor = abs ( Lhs_Parallel_Descriptor.Stride[Axis] - 
      Rhs_Parallel_Descriptor.Stride[Axis] ) + 1;

   /*
   if (Lhs_Parallel_Descriptor.Stride[Axis] != 
       Rhs_Parallel_Descriptor.Stride[Axis])
   {
      // We need to use the Maximum stride to interpret the Lhs and Rhs 
      // consistantly

      int Max_Stride = (Lhs_Parallel_Descriptor.Stride[Axis] > 
	 Rhs_Parallel_Descriptor.Stride[Axis]) ?
         Lhs_Parallel_Descriptor.Stride[Axis] : 
	 Rhs_Parallel_Descriptor.Stride[Axis];


      // Temporary code for debugging cases of stride 1 and 2 before 
      // being more general

      APP_ASSERT(Max_Stride >= 0);
      APP_ASSERT(Max_Stride <= 2);

      if (Lhs_Parallel_Descriptor.Stride[Axis] > 
	  Rhs_Parallel_Descriptor.Stride[Axis])
      {
         Rhs_Left_Number_Of_Points  *= Max_Stride;
         Rhs_Right_Number_Of_Points *= Max_Stride;
      }
      else
      {
         Lhs_Left_Number_Of_Points  *= Max_Stride;
         Lhs_Right_Number_Of_Points *= Max_Stride;
      }
   }
   */

   // ... (12/13/96,kdb) it might be impossible to check to
   //  see if other processors other than the neighboring
   //  processor need an update and right now block parti requires
   //  an update for both the high and low end ghost cells if
   //  it does any update so turn on all ghost cell updates
   //  when this situation could occur.  Remove this later when 
   //  block parti is more flexible ...

   int update_all = FALSE;
   int nd;
   for (nd=0; nd<MAX_ARRAY_DIMENSION; nd++)
   {
     if (Lhs_Parallel_Descriptor.Size[nd] != 
         Rhs_Parallel_Descriptor.Size[nd] ) 
     update_all = TRUE;
   }
   if (Lhs_Parallel_Descriptor.Partitioning_Object_Pointer != 
       Rhs_Parallel_Descriptor.Partitioning_Object_Pointer) 
	  update_all = TRUE;



   int Truncation_Width = Lhs_Right_Number_Of_Points - 
      	                  Rhs_Right_Number_Of_Points;

   int factor = 1;
   if ( abs (Truncation_Width) > (factor * Lhs_ghost_cell_width) )
   {
      Return_Communication_Model = FULL_VSG_MODEL;
   }
   else
   {
      // ... bug fix (10/17/96,kdb) if the base of Lhs doesn't extend
      //  beyond the right ghost cell overlap region while Rhs ends
      //  before the ghost cell region, the processor to the right 
      //  will do a full vsg update so this processor must also.  The situation
      //  is the same if Rhs and Lhs are reversed ...
     
// ... (12/26/95,kdb) don't need this anymore because of global check ...
#if 0
      int Lhs_num_to_left = MAX((Lhs_start_right_ghost - Lhs_gbase + Lhs_stride - 1)/
	 Lhs_stride,0);
      int Rhs_num_to_left = MAX((Rhs_start_right_ghost - Rhs_gbase + Rhs_stride - 1)/
	 Rhs_stride,0);

      if(abs(Lhs_num_to_left-Rhs_num_to_left) > factor* Lhs_ghost_cell_width)
      {
         Return_Communication_Model = FULL_VSG_MODEL;
      }
      else if (((Lhs_gbase >= Lhs_start_right_ghost) &&
               (Rhs_gbound < Rhs_start_right_ghost)) ||

              ((Rhs_gbase >= Rhs_start_right_ghost) &&
               (Lhs_gbound < Lhs_start_right_ghost)))
      {
         Return_Communication_Model = FULL_VSG_MODEL;

	 // ... make sure neighboring processor won't convert from 
	 //  vsg to overlapping boundary ...

         if ((Lhs_gbase >= Lhs_start_right_ghost) &&
             (Rhs_gbound < Rhs_start_right_ghost)) 
         {
	    if ((Rhs_gbase >= Rhs_start_left_ghost + Rhs_ghost_cell_width)&&
	        (Lhs_gbase > Lhs_end_left_ghost))
	      // ... Rhs only lives on ghost cell of processor to the left ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
	 else if ((Rhs_gbase >= Rhs_start_right_ghost) &&
                  (Lhs_gbound < Lhs_start_right_ghost)) 
	 {
	    if ((Lhs_gbase >= Lhs_start_left_ghost + Lhs_ghost_cell_width)&&
	        (Rhs_gbase > Rhs_end_left_ghost))
	      // ... Lhs only lives on ghost cell of processor to the left ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
      }
#endif

      if (Return_Communication_Model != FULL_VSG_MODEL)
      {
         Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

         int Adjustment = 0; 
         if (Truncation_Width < 0)
	    // ... (10/15/96,kdb) already scaled properly ...
            //Adjustment = (Truncation_Width / Stride_Factor) / 
	    //   Lhs_Partition_Index[Axis].getStride();
            Adjustment = Truncation_Width; 

         Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis] = 
           MIN (Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis],
	        Adjustment);
         if ( !Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis])
            Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = 
	       (Truncation_Width != 0) ? TRUE : FALSE;

         // ... Make sure this is turned off because global checking code
	 //  might turn it on ...
         Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = FALSE;

         // Temp code to force message passing in case of stride > 1 (a 
         // difficult case) The correct way to handle this is to compute 
         // what the adjacent processor would compute and thus we can be 
         // certain that the sending and receiving of message is the same 
         // on both sides of the partition boundary.

         // ... (11/4/96,kdb) this causes problems when ghost cell width
	 //  is 0 so remove and fix better later ...
         //if ((Stride_Factor > 1) && 
	 //    (!Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis]))
         //   Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;

	 // ... (11/6/96,kdb) try to detect case where update right isn't
	 //  indicated but processor to the right will need an update and
	 //  so this processor must also to avoid messing up communication ...

         if ( !Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis])
	 {
            if (update_all)
	    {
	       // ... see comment above about temporary fix due to block parti
	       //  problem ...
               Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;
	    }
	    else
	    {

	    // ... if Lhs has more points in the ghost cell region and Lhs
	    //  and Rhs have the same Right_Number_Of_Points before
	    //  dividing by stride then the processor to the right will
	    //  need to do a left update so this processor must do a 
	    //  right update ...

            if (((Lhs_gbound >= Lhs_start_right_ghost) ||
                 (Rhs_gbound >= Rhs_start_right_ghost)) &&
                Lhs_ghost_cell_width>0)
            {
	       // ... There is at least 1 point in the Lhs or Rhs ghost cell 
	       //   region ...

	       //if (Lhs_Right_Number_Of_Points_unscaled ==
	       //    Rhs_Right_Number_Of_Points_unscaled)
	       if (Lhs_Right_Number_Of_Points==
	           Rhs_Right_Number_Of_Points)
               {

	          // ... MAX doesn't seem to work in macro ...

	          int Lhs_num = 0; 
                  if (Lhs_gbound >= Lhs_start_right_ghost) 
		  {
	             Lhs_num = 
                        Lhs_gbound 
		        - (Lhs_gbase > Lhs_start_right_ghost?
                        Lhs_gbase : Lhs_start_right_ghost);
                        //Lhs_gbound - MAX (Lhs_gbase, Lhs_start_right_ghost);
                     Lhs_num -= Lhs_Right_Number_Of_Points_unscaled;  
                     Lhs_num = (Lhs_num/
                       Lhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
		  }
	      
	          int Rhs_num = 0; 
                  if (Rhs_gbound >= Rhs_start_right_ghost) 
		  {
	             Rhs_num = 
                        Rhs_gbound - (Rhs_gbase > Rhs_start_right_ghost?
                        Rhs_gbase : Rhs_start_right_ghost);
                        //Rhs_gbound - MAX (Rhs_gbase, Rhs_start_right_ghost);
                     Rhs_num -= Rhs_Right_Number_Of_Points_unscaled;  
                     Rhs_num = (Rhs_num/
                       Rhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
		  }

	          if (Rhs_num != Lhs_num) 
                     Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis]
	                = TRUE;
               }
	    }
	    }

	 }
      }
   }

   APP_ASSERT ( (Return_Communication_Model == FULL_VSG_MODEL) || 
	(Return_Communication_Model == OVERLAPPING_BOUNDARY_VSG_MODEL) );

   // ... this code should no longer be called for full vsg so add this test ...
   APP_ASSERT ( Return_Communication_Model != FULL_VSG_MODEL);  
   }

   return Return_Communication_Model;
}

//=================================================================

int Array_Domain_Type::Overlap_Update_Case_Middle_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Lhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
          const Array_Domain_Type       & Rhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Rhs_Serial_Descriptor ,
          Internal_Index *Lhs_Partition_Index,
          Internal_Index *Rhs_Partition_Index,
          int Axis)
{
   // This macro forces the setup to be the same for each of the Left 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   if ((Lhs_Partition_Index[Axis].getMode()!=Null_Index)&&
       (Rhs_Partition_Index[Axis].getMode()!=Null_Index))
   {
   //int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int Lhs_gbase  = Lhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Lhs_gbound = Lhs_Parallel_Descriptor.Global_Index[Axis].getBound();
   int Rhs_gbase  = Rhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Rhs_gbound = Rhs_Parallel_Descriptor.Global_Index[Axis].getBound();

   int Lhs_ghost_cell_width = Lhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int Rhs_ghost_cell_width = Rhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];

   // ... offset of local view from global view ...
   int Lhs_Left_Number_Of_Points  =  
      Lhs_Partition_Index [Axis].getBase() - Lhs_gbase;
   int Lhs_Right_Number_Of_Points =  
      Lhs_gbound - Lhs_Partition_Index [Axis].getBound();
   int Rhs_Left_Number_Of_Points  = 
      Rhs_Partition_Index [Axis].getBase() - Rhs_gbase;
   int Rhs_Right_Number_Of_Points = 
      Rhs_gbound - Rhs_Partition_Index [Axis].getBound();

   // ... store unscaled versions of these ...
   int Lhs_Left_Number_Of_Points_unscaled  = Lhs_Left_Number_Of_Points;
   int Lhs_Right_Number_Of_Points_unscaled = Lhs_Right_Number_Of_Points;
   int Rhs_Left_Number_Of_Points_unscaled  = Rhs_Left_Number_Of_Points;
   int Rhs_Right_Number_Of_Points_unscaled = Rhs_Right_Number_Of_Points;

   // ... bug fix (10/15/96,kdb) dividing by both Stride_Factor and
   //  by stride causes problems and is confusing.  Simplify by 
   //  rescaling by stride immediately ...

   int Lhs_stride = Lhs_Partition_Index [Axis].getStride();
   int Rhs_stride = Rhs_Partition_Index [Axis].getStride();

   Lhs_Left_Number_Of_Points = (Lhs_Left_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Lhs_Right_Number_Of_Points = (Lhs_Right_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Rhs_Left_Number_Of_Points = (Rhs_Left_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;
   Rhs_Right_Number_Of_Points = (Rhs_Right_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;

   APP_ASSERT(Lhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Lhs_Right_Number_Of_Points >= 0);
   APP_ASSERT(Rhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Rhs_Right_Number_Of_Points >= 0);


   DOMAIN_SIZE_Type Rhs_Size = Rhs_Serial_Descriptor.Size[Axis]; 
   DOMAIN_SIZE_Type Lhs_Size = Lhs_Serial_Descriptor.Size[Axis]; 
   if (Axis>0)
   {
      Rhs_Size /= Rhs_Serial_Descriptor.Size[Axis-1]; 
      Lhs_Size /= Lhs_Serial_Descriptor.Size[Axis-1]; 
   }

   int Lhs_start_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size
       - 2 * Lhs_ghost_cell_width;
   int Lhs_end_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size -1;

   int Rhs_start_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size
       - 2 * Rhs_ghost_cell_width;
   int Rhs_end_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size -1;


   int Lhs_start_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis];
   int Lhs_end_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Lhs_ghost_cell_width;

   int Rhs_start_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis];
   int Rhs_end_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Rhs_ghost_cell_width;


   // ... WARNING: stride > 1 needs to be tested ...

   // ... note (10/15/96,kdb) don't need this anymore ...

   int Stride_Factor = abs ( Lhs_Parallel_Descriptor.Stride[Axis] - 
      Rhs_Parallel_Descriptor.Stride[Axis] ) + 1;

   /*
   if (Lhs_Parallel_Descriptor.Stride[Axis] != 
       Rhs_Parallel_Descriptor.Stride[Axis])
   {
      // We need to use the Maximum stride to interpret the Lhs and Rhs 
      // consistantly

      int Max_Stride = (Lhs_Parallel_Descriptor.Stride[Axis] > 
	 Rhs_Parallel_Descriptor.Stride[Axis]) ?
         Lhs_Parallel_Descriptor.Stride[Axis] : 
	 Rhs_Parallel_Descriptor.Stride[Axis];


      // Temporary code for debugging cases of stride 1 and 2 before 
      // being more general

      APP_ASSERT(Max_Stride >= 0);
      APP_ASSERT(Max_Stride <= 2);

      if (Lhs_Parallel_Descriptor.Stride[Axis] > 
	  Rhs_Parallel_Descriptor.Stride[Axis])
      {
         Rhs_Left_Number_Of_Points  *= Max_Stride;
         Rhs_Right_Number_Of_Points *= Max_Stride;
      }
      else
      {
         Lhs_Left_Number_Of_Points  *= Max_Stride;
         Lhs_Right_Number_Of_Points *= Max_Stride;
      }
   }
   */

   // ... (12/13/96,kdb) it might be impossible to check to
   //  see if other processors other than the neighboring
   //  processor need an update and right now block parti requires
   //  an update for both the high and low end ghost cells if
   //  it does any update so turn on all ghost cell updates
   //  when this situation could occur.  Remove this later when 
   //  block parti is more flexible ...

   int update_all = FALSE;
   int nd;
   for (nd=0; nd<MAX_ARRAY_DIMENSION; nd++)
   {
     if (Lhs_Parallel_Descriptor.Size[nd] != 
         Rhs_Parallel_Descriptor.Size[nd] ) 
     update_all = TRUE;
   }
   if (Lhs_Parallel_Descriptor.Partitioning_Object_Pointer != 
       Rhs_Parallel_Descriptor.Partitioning_Object_Pointer) 
	  update_all = TRUE;



   int Truncation_Width_Base = Lhs_Left_Number_Of_Points - 
      	                       Rhs_Left_Number_Of_Points;

   int Truncation_Width_Bound = Lhs_Right_Number_Of_Points - 
      	                        Rhs_Right_Number_Of_Points;

   int factor = 1;

   // QUESTION: Should this check be done for both the base and 
   //           bound?  This probably doesn't matter until 
   //           strides an greater than 1.
   if ( (abs (Truncation_Width_Base) > factor * Lhs_ghost_cell_width) ||
        (abs (Truncation_Width_Bound) > factor * Lhs_ghost_cell_width) )
   {
      Return_Communication_Model = FULL_VSG_MODEL;
   }
   else
   {
      // ... bug fix (10/17/96,kdb) if the base of Lhs doesn't extend
      //  beyond the right ghost cell overlap region while Rhs ends
      //  before the ghost cell region, the processor to the right 
      //  will do a full vsg update so this processor must also.  The situation
      //  is the same if Rhs and Lhs are reversed.  Also check other side. ...
     
// ... (12/26/95,kdb) don't need this anymore because of global check ...
#if 0
      int Lhs_num_to_left = MAX((Lhs_start_right_ghost - Lhs_gbase + Lhs_stride - 1)/
	 Lhs_stride,0);
      int Rhs_num_to_left = MAX((Rhs_start_right_ghost - Rhs_gbase + Rhs_stride - 1)/
	 Rhs_stride,0);

      int Lhs_num_to_right = MAX((Lhs_gbound - Lhs_end_left_ghost + Lhs_stride - 1)/
	 Lhs_stride,0);
      int Rhs_num_to_right = MAX((Rhs_gbound - Rhs_end_left_ghost + Rhs_stride - 1)/
	 Rhs_stride,0);

      if((abs(Lhs_num_to_right-Rhs_num_to_right) > factor* Lhs_ghost_cell_width) ||
         (abs(Lhs_num_to_left -Rhs_num_to_left)  > factor* Lhs_ghost_cell_width) )
      {
         Return_Communication_Model = FULL_VSG_MODEL;
      }
      else if (((Lhs_gbase >= Lhs_start_right_ghost) &&
               (Rhs_gbound < Rhs_start_right_ghost)) ||

              ((Rhs_gbase >= Rhs_start_right_ghost) &&
               (Lhs_gbound < Lhs_start_right_ghost)) ||

              ((Lhs_gbound <= Lhs_end_left_ghost) &&
               (Rhs_gbase > Rhs_end_left_ghost)) ||

              ((Rhs_gbound <= Rhs_end_left_ghost) &&
               (Lhs_gbase > Lhs_end_left_ghost)))
      {
         Return_Communication_Model = FULL_VSG_MODEL;

	 // ... make sure neighboring processor won't convert from 
	 //  vsg to overlapping boundary ...

         if ((Lhs_gbase >= Lhs_start_right_ghost) &&
             (Rhs_gbound < Rhs_start_right_ghost)) 
         {
	    if ((Rhs_gbase >= Rhs_start_left_ghost + Rhs_ghost_cell_width)&&
	        (Lhs_gbase > Lhs_end_left_ghost))
	      // ... Rhs only lives on ghost cell of processor to the left ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
	 else if ((Rhs_gbase >= Rhs_start_right_ghost) &&
                  (Lhs_gbound < Lhs_start_right_ghost)) 
	 {
	    if ((Lhs_gbase >= Lhs_start_left_ghost + Lhs_ghost_cell_width)&&
	        (Rhs_gbase > Rhs_end_left_ghost))
	      // ... Lhs only lives on ghost cell of processor to the left ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
         else if ((Lhs_gbound <= Lhs_end_left_ghost) &&
                  (Rhs_gbase > Rhs_end_left_ghost)) 
	 {
	    if ((Rhs_gbound <= Rhs_start_right_ghost + Rhs_ghost_cell_width)&&
	        (Lhs_gbound < Lhs_start_right_ghost))
	      // ... Lhs only lives on ghost cell of processor to the right ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
         else if ((Rhs_gbound <= Rhs_end_left_ghost) &&
                  (Lhs_gbase > Lhs_end_left_ghost)) 
	 {
	    if ((Lhs_gbound <= Lhs_start_right_ghost + Lhs_ghost_cell_width)&&
	        (Rhs_gbound < Rhs_start_right_ghost))
	      // ... Rhs only lives on ghost cell of processor to the right ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
      }
#endif

      if (Return_Communication_Model != FULL_VSG_MODEL)
      {

         Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

         int Adjustment = 0;
         // ... (10/15/96,kdb) already scaled properly ...
         if (Truncation_Width_Base < 0)
            //Adjustment = (-Truncation_Width_Base / Stride_Factor) / 
	    //   Lhs_Partition_Index[Axis].getStride();
            Adjustment = -Truncation_Width_Base;

         Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis] = 
            MAX (Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis], 
	         Adjustment);
         if (!Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis])
            Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = 
	       (Truncation_Width_Base != 0) ? TRUE : FALSE;


         Adjustment = 0;
         // ... (10/15/96,kdb) already scaled properly ...
         if (Truncation_Width_Bound < 0)
            //Adjustment = (Truncation_Width_Bound / Stride_Factor) / 
	    //   Lhs_Partition_Index[Axis].getStride();
            Adjustment = Truncation_Width_Bound;

         Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis] = 
            MIN (Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis],
	         Adjustment);
         if (!Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis])
            Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = 
	       (Truncation_Width_Bound != 0) ? TRUE : FALSE;


         // Temp code to force message passing in case of stride > 1 (a 
         // difficult case) The correct way to handle this is to compute 
         // what the adjacent processor would compute and thus we can be 
         // certain that the sending and receiving of message is the same 
         // on both sides of the partition boundary.

         // ... (11/4/96,kdb) this causes problems when ghost cell width
	 //  is 0 so remove and fix better later ...
         //if ((Stride_Factor > 1) && 
	 //    (!Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis]))
         //   Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;

	 // ... (11/6/96,kdb) try to detect case where update right isn't
	 //  indicated but processor to the right will need an update and
	 //  so this processor must also to avoid messing up communication ...

         if ( !Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis])
	 {
            if (update_all)
	    {
	       // ... see comment above about temporary fix due to block parti
	       //  problem ...
               Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;
	    }
	    else
	    {

	    // ... if Lhs has more points in the ghost cell region and Lhs
	    //  and Rhs have the same Right_Number_Of_Points before
	    //  dividing by stride then the processor to the right will
	    //  need to do a left update so this processor must do a 
	    //  right update ...

            if (((Lhs_gbound >= Lhs_start_right_ghost) ||
                 (Rhs_gbound >= Rhs_start_right_ghost)) &&
                Lhs_ghost_cell_width>0)
            {
	       // ... There is at least 1 point in the Lhs or Rhs ghost cell 
	       //   region ...

	       //if (Lhs_Right_Number_Of_Points_unscaled ==
	       //    Rhs_Right_Number_Of_Points_unscaled)
	       if (Lhs_Right_Number_Of_Points==
	           Rhs_Right_Number_Of_Points)
               {

	          // ... MAX doesn't seem to work in macro ...

	          int Lhs_num = 0; 
                  if (Lhs_gbound >= Lhs_start_right_ghost) 
		  {
	             Lhs_num = 
                        Lhs_gbound 
		        - (Lhs_gbase > Lhs_start_right_ghost?
                        Lhs_gbase : Lhs_start_right_ghost);
                        //Lhs_gbound - MAX (Lhs_gbase, Lhs_start_right_ghost);
                     Lhs_num -= Lhs_Right_Number_Of_Points_unscaled;  
                     Lhs_num = (Lhs_num/
                       Lhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
		  }
	      
	          int Rhs_num = 0; 
                  if (Rhs_gbound >= Rhs_start_right_ghost) 
		  {
	             Rhs_num = 
                        Rhs_gbound - (Rhs_gbase > Rhs_start_right_ghost?
                        Rhs_gbase : Rhs_start_right_ghost);
                        //Rhs_gbound - MAX (Rhs_gbase, Rhs_start_right_ghost);
                     Rhs_num -= Rhs_Right_Number_Of_Points_unscaled;  
                     Rhs_num = (Rhs_num/
                       Rhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
		  }

	          if (Rhs_num != Lhs_num) 
                     Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis]
	                = TRUE;
               }
	    }
	    }

	 }

	 // ... (11/6/96,kdb) try to detect case where update left isn't
	 //  indicated but processor to the left will need an update and
	 //  so this processor must also to avoid messing up communication ...

         if ( !Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis])
	 {
            if (update_all)
	    {
	       // ... see comment above about temporary fix due to block parti
	       //  problem ...
               Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = TRUE;
	    }
	    else
	    {

	    // ... if Lhs has more points in the ghost cell region and Lhs
	    //  and Rhs have the same Left_Number_Of_Points before
	    //  dividing by stride then the processor to the left will
	    //  need to do a right update so this processor must do a 
	    //  left update ...

            if (((Lhs_gbase <= Lhs_end_left_ghost) ||
                 (Rhs_gbase <= Rhs_end_left_ghost)) &&
                Lhs_ghost_cell_width>0)
            {
	       // ... There is at least 1 point in the Lhs or Rhs ghost cell 
	       //   region ...

	       //if (Lhs_Left_Number_Of_Points_unscaled ==
	       //    Rhs_Left_Number_Of_Points_unscaled)
	       if (Lhs_Left_Number_Of_Points==
	           Rhs_Left_Number_Of_Points)
               {
	       // ... MIN doesn't seem to work in macro ...

	       int Lhs_num = 0; 
               if (Rhs_gbase <= Rhs_end_left_ghost) 
	       {
	          Lhs_num = 
		     (Lhs_gbound < Lhs_end_left_ghost ? Lhs_gbound:Lhs_end_left_ghost)
		     - Lhs_gbase;
		     //MIN (Lhs_gbound, Lhs_end_left_ghost) - Lhs_gbase;
                  Lhs_num = (Lhs_num/
                    Lhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
	       }
	      
	       int Rhs_num = 0; 
               if (Rhs_gbase <= Rhs_end_left_ghost) 
	       {
	          Rhs_num = 
		     (Rhs_gbound < Rhs_end_left_ghost ? Rhs_gbound:Rhs_end_left_ghost)
		        - Rhs_gbase;
		     //MIN (Rhs_gbound, Rhs_end_left_ghost) - Rhs_gbase;
                  Rhs_num = (Rhs_num/
                    Rhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
	       }

	       //if (Rhs_num < Lhs_num) 
	       if (Rhs_num != Lhs_num) 
                  Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis]
	             = TRUE;
               }
	    }
	    }

	 }
	      
	      
      }
   }

   APP_ASSERT ( (Return_Communication_Model == FULL_VSG_MODEL) || 
	(Return_Communication_Model == OVERLAPPING_BOUNDARY_VSG_MODEL) );

   // ... this code should no longer be called for full vsg so add this test ...
   APP_ASSERT ( Return_Communication_Model != FULL_VSG_MODEL);  
   }

   return Return_Communication_Model;
}

//=================================================================

int Array_Domain_Type::Overlap_Update_Case_Right_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Lhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
          const Array_Domain_Type       & Rhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Rhs_Serial_Descriptor ,
          Internal_Index *Lhs_Partition_Index,
          Internal_Index *Rhs_Partition_Index,
          int Axis)
{
   // This macro forces the setup to be the same for each of the Left, 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   if ((Lhs_Partition_Index[Axis].getMode()!=Null_Index)&&
       (Rhs_Partition_Index[Axis].getMode()!=Null_Index))
   {
   //int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int Lhs_gbase  = Lhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Lhs_gbound = Lhs_Parallel_Descriptor.Global_Index[Axis].getBound();
   int Rhs_gbase  = Rhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Rhs_gbound = Rhs_Parallel_Descriptor.Global_Index[Axis].getBound();

   int Lhs_ghost_cell_width = Lhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int Rhs_ghost_cell_width = Rhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];

   // ... offset of local view from global view ...
   int Lhs_Left_Number_Of_Points  =  
      Lhs_Partition_Index [Axis].getBase() - Lhs_gbase;
   int Lhs_Right_Number_Of_Points =  
      Lhs_gbound - Lhs_Partition_Index [Axis].getBound();
   int Rhs_Left_Number_Of_Points  = 
      Rhs_Partition_Index [Axis].getBase() - Rhs_gbase;
   int Rhs_Right_Number_Of_Points = 
      Rhs_gbound - Rhs_Partition_Index [Axis].getBound();

   // ... store unscaled versions of these ...
   int Lhs_Left_Number_Of_Points_unscaled  = Lhs_Left_Number_Of_Points;
   int Lhs_Right_Number_Of_Points_unscaled = Lhs_Right_Number_Of_Points;
   int Rhs_Left_Number_Of_Points_unscaled  = Rhs_Left_Number_Of_Points;
   int Rhs_Right_Number_Of_Points_unscaled = Rhs_Right_Number_Of_Points;

   // ... bug fix (10/15/96,kdb) dividing by both Stride_Factor and
   //  by stride causes problems and is confusing.  Simplify by 
   //  rescaling by stride immediately ...

   int Lhs_stride = Lhs_Partition_Index [Axis].getStride();
   int Rhs_stride = Rhs_Partition_Index [Axis].getStride();

   Lhs_Left_Number_Of_Points = (Lhs_Left_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Lhs_Right_Number_Of_Points = (Lhs_Right_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Rhs_Left_Number_Of_Points = (Rhs_Left_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;
   Rhs_Right_Number_Of_Points = (Rhs_Right_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;

   APP_ASSERT(Lhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Lhs_Right_Number_Of_Points >= 0);
   APP_ASSERT(Rhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Rhs_Right_Number_Of_Points >= 0);


   DOMAIN_SIZE_Type Rhs_Size = Rhs_Serial_Descriptor.Size[Axis]; 
   DOMAIN_SIZE_Type Lhs_Size = Lhs_Serial_Descriptor.Size[Axis]; 
   if (Axis>0)
   {
      Rhs_Size /= Rhs_Serial_Descriptor.Size[Axis-1]; 
      Lhs_Size /= Lhs_Serial_Descriptor.Size[Axis-1]; 
   }

   int Lhs_start_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size
       - 2 * Lhs_ghost_cell_width;
   int Lhs_end_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size -1;

   int Rhs_start_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size
       - 2 * Rhs_ghost_cell_width;
   int Rhs_end_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size -1;


   int Lhs_start_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis];
   int Lhs_end_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Lhs_ghost_cell_width;

   int Rhs_start_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis];
   int Rhs_end_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Rhs_ghost_cell_width;


   // ... WARNING: stride > 1 needs to be tested ...

   // ... note (10/15/96,kdb) don't need this anymore ...

   int Stride_Factor = abs ( Lhs_Parallel_Descriptor.Stride[Axis] - 
      Rhs_Parallel_Descriptor.Stride[Axis] ) + 1;

   /*
   if (Lhs_Parallel_Descriptor.Stride[Axis] != 
       Rhs_Parallel_Descriptor.Stride[Axis])
   {
      // We need to use the Maximum stride to interpret the Lhs and Rhs 
      // consistantly

      int Max_Stride = (Lhs_Parallel_Descriptor.Stride[Axis] > 
	 Rhs_Parallel_Descriptor.Stride[Axis]) ?
         Lhs_Parallel_Descriptor.Stride[Axis] : 
	 Rhs_Parallel_Descriptor.Stride[Axis];


      // Temporary code for debugging cases of stride 1 and 2 before 
      // being more general

      APP_ASSERT(Max_Stride >= 0);
      APP_ASSERT(Max_Stride <= 2);

      if (Lhs_Parallel_Descriptor.Stride[Axis] > 
	  Rhs_Parallel_Descriptor.Stride[Axis])
      {
         Rhs_Left_Number_Of_Points  *= Max_Stride;
         Rhs_Right_Number_Of_Points *= Max_Stride;
      }
      else
      {
         Lhs_Left_Number_Of_Points  *= Max_Stride;
         Lhs_Right_Number_Of_Points *= Max_Stride;
      }
   }
   */

   // ... (12/13/96,kdb) it might be impossible to check to
   //  see if other processors other than the neighboring
   //  processor need an update and right now block parti requires
   //  an update for both the high and low end ghost cells if
   //  it does any update so turn on all ghost cell updates
   //  when this situation could occur.  Remove this later when 
   //  block parti is more flexible ...

   int update_all = FALSE;
   int nd;
   for (nd=0; nd<MAX_ARRAY_DIMENSION; nd++)
   {
     if (Lhs_Parallel_Descriptor.Size[nd] != 
         Rhs_Parallel_Descriptor.Size[nd] ) 
     update_all = TRUE;
   }
   if (Lhs_Parallel_Descriptor.Partitioning_Object_Pointer != 
       Rhs_Parallel_Descriptor.Partitioning_Object_Pointer) 
	  update_all = TRUE;




   int Truncation_Width = Lhs_Left_Number_Of_Points - 
          	          Rhs_Left_Number_Of_Points;

   int factor = 1;
   if ( abs (Truncation_Width) > ( factor * Lhs_ghost_cell_width) )
   {
      Return_Communication_Model = FULL_VSG_MODEL;
   }
   else
   {
      // ... bug fix (10/17/96,kdb) if the boundary of Lhs doesn't extend
      //  beyond the left ghost cell overlap region while Rhs doesn't
      //  begin inside the ghost cell region, the processor to the left
      //  will do a full vsg update so this processor must also.  The situation
      //  is the same if Rhs and Lhs are reversed ...

// ... (12/26/95,kdb) don't need this anymore because of global check ...
#if 0
      int Lhs_num_to_right = MAX((Lhs_gbound - Lhs_end_left_ghost + Lhs_stride - 1)/
	 Lhs_stride,0);
      int Rhs_num_to_right = MAX((Rhs_gbound - Rhs_end_left_ghost + Rhs_stride - 1)/
	 Rhs_stride,0);

      if (abs(Lhs_num_to_right-Rhs_num_to_right) > factor* Lhs_ghost_cell_width)
      {
         Return_Communication_Model = FULL_VSG_MODEL;
      }
      else if (((Lhs_gbound <= Lhs_end_left_ghost) &&
               (Rhs_gbase > Rhs_end_left_ghost)) ||

              ((Rhs_gbound <= Rhs_end_left_ghost) &&
               (Lhs_gbase > Lhs_end_left_ghost)))
      {
         Return_Communication_Model = FULL_VSG_MODEL;

	 // ... make sure neighboring processor won't convert from 
	 //  vsg to overlapping boundary ...

         if ((Lhs_gbound <= Lhs_end_left_ghost) &&
             (Rhs_gbase > Rhs_end_left_ghost)) 
	 {
	    if ((Rhs_gbound <= Rhs_start_right_ghost + Rhs_ghost_cell_width)&&
	        (Lhs_gbound < Lhs_start_right_ghost))
	      // ... Lhs only lives on ghost cell of processor to the right ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
         else if ((Rhs_gbound <= Rhs_end_left_ghost) &&
                  (Lhs_gbase > Lhs_end_left_ghost)) 
	 {
	    if ((Lhs_gbound <= Lhs_start_right_ghost + Lhs_ghost_cell_width)&&
	        (Rhs_gbound < Rhs_start_right_ghost))
	      // ... Rhs only lives on ghost cell of processor to the right ...
              Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;
	 }
      }

#endif

      if (Return_Communication_Model != FULL_VSG_MODEL)
      {

         Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

         int Adjustment = 0;
         if (Truncation_Width < 0)
            // Truncation_Width is negative
	    // ... (10/15/96,kdb) already scaled properly ...
            //Adjustment = (-Truncation_Width / Stride_Factor) / 
  	    //   Lhs_Partition_Index[Axis].getStride();
            Adjustment = -Truncation_Width;

         Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis] = 
            MAX(Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis],
  	        Adjustment);
         if ( Return_Array_Set.Update_Left_Ghost_Boundary_Width  [Axis] == 0 )
            Return_Array_Set.Update_Left_Ghost_Boundary_Width  [Axis] = 
	       (Truncation_Width != 0) ? TRUE : FALSE;
 
         // ... Make sure this is turned off because global checking code
	 //  might turn it on ...
         Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = FALSE;

         // Temp code to force message passing in case of stride > 1 (a 
         // difficult case)
         // The correct way to handle this is to compute what the adjacent 
         // processor would compute and thus we can be certain that the 
         // sending an receiving of message is the same on both sides of
         // the partition boundary.

         // ... (11/4/96,kdb) this causes problems when ghost cell width
	 //  is 0 so remove and fix better later ...
         //if ((Stride_Factor > 1) && 
 	 //   (!Return_Array_Set.Update_Left_Ghost_Boundary_Width [Axis]))
         //   Return_Array_Set.Update_Left_Ghost_Boundary_Width  [Axis] = TRUE;

	 // ... (11/6/96,kdb) try to detect case where update left isn't
	 //  indicated but processor to the left will need an update and
	 //  so this processor must also to avoid messing up communication ...

         if ( !Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis])
	 {
            if (update_all)
	    {
	       // ... see comment above about temporary fix due to block parti
	       //  problem ...
               Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = TRUE;
	    }
	    else
	    {

	    // ... if Lhs has more points in the ghost cell region and Lhs
	    //  and Rhs have the same Left_Number_Of_Points before
	    //  dividing by stride then the processor to the left will
	    //  need to do a right update so this processor must do a 
	    //  left update ...

            if (((Lhs_gbase <= Lhs_end_left_ghost) ||
                 (Rhs_gbase <= Rhs_end_left_ghost)) &&
                Lhs_ghost_cell_width>0)
            {
	       // ... There is at least 1 point in the Lhs or Rhs ghost cell 
	       //   region ...

	       //if (Lhs_Left_Number_Of_Points_unscaled ==
	       //    Rhs_Left_Number_Of_Points_unscaled)
	       if (Lhs_Left_Number_Of_Points==
	           Rhs_Left_Number_Of_Points)
               {
	       // ... MIN doesn't seem to work in macro ...

	       int Lhs_num = 0; 
               if (Rhs_gbase <= Rhs_end_left_ghost) 
	       {
	          Lhs_num = 
		     (Lhs_gbound < Lhs_end_left_ghost ? Lhs_gbound:Lhs_end_left_ghost)
		     - Lhs_gbase;
		     //MIN (Lhs_gbound, Lhs_end_left_ghost) - Lhs_gbase;
                  Lhs_num = (Lhs_num/
                    Lhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
	       }
	      
	       int Rhs_num = 0; 
               if (Rhs_gbase <= Rhs_end_left_ghost) 
	       {
	          Rhs_num = 
		     (Rhs_gbound < Rhs_end_left_ghost ? Rhs_gbound:Rhs_end_left_ghost)
		        - Rhs_gbase;
		     //MIN (Rhs_gbound, Rhs_end_left_ghost) - Rhs_gbase;
                  Rhs_num = (Rhs_num/
                    Rhs_Parallel_Descriptor.Global_Index[Axis].getStride()) + 1;
	       }

	       //if (Rhs_num < Lhs_num) 
	       if (Rhs_num != Lhs_num) 
                  Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis]
	             = TRUE;
               }
	    }
	    }

	 }
      }
   }


   APP_ASSERT ( (Return_Communication_Model == FULL_VSG_MODEL) || 
      (Return_Communication_Model == OVERLAPPING_BOUNDARY_VSG_MODEL) );

   // ... this code should no longer be called for full vsg so add this test ...
   APP_ASSERT ( Return_Communication_Model != FULL_VSG_MODEL);  
   }

   return Return_Communication_Model;
}

//=================================================================

void Array_Domain_Type::Overlap_Update_Fix_Rhs (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Lhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
          const Array_Domain_Type       & Rhs_Parallel_Descriptor,
          const SerialArray_Domain_Type & Rhs_Serial_Descriptor ,
          Internal_Index *Lhs_Partition_Index,
          Internal_Index *Rhs_Partition_Index,
          int Axis )
{
   // ... NOTE: probably don't need Return_Array_Set or
   //  Lhs_Partition_Index here ... 

   // This macro forces the setup to be the same for each of the Left 
   // Right and Middle processor cases

   // Temp code
// Lhs_Parallel_Descriptor.display("Lhs_Parallel_Descriptor");
// Lhs_Serial_Descriptor.display("Lhs_Serial_Descriptor");
// Rhs_Parallel_Descriptor.display("Rhs_Parallel_Descriptor");
// Rhs_Serial_Descriptor.display("Rhs_Serial_Descriptor");

   //int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int Lhs_gbase  = Lhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Lhs_gbound = Lhs_Parallel_Descriptor.Global_Index[Axis].getBound();
   int Rhs_gbase  = Rhs_Parallel_Descriptor.Global_Index[Axis].getBase();
   int Rhs_gbound = Rhs_Parallel_Descriptor.Global_Index[Axis].getBound();

   int Lhs_ghost_cell_width = Lhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int Rhs_ghost_cell_width = Rhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];

   // ... offset of local view from global view ...
   int Lhs_Left_Number_Of_Points  =  
      Lhs_Partition_Index [Axis].getBase() - Lhs_gbase;
   int Lhs_Right_Number_Of_Points =  
      Lhs_gbound - Lhs_Partition_Index [Axis].getBound();
   int Rhs_Left_Number_Of_Points  = 
      Rhs_Partition_Index [Axis].getBase() - Rhs_gbase;
   int Rhs_Right_Number_Of_Points = 
      Rhs_gbound - Rhs_Partition_Index [Axis].getBound();

   // ... store unscaled versions of these ...
   int Lhs_Left_Number_Of_Points_unscaled  = Lhs_Left_Number_Of_Points;
   int Lhs_Right_Number_Of_Points_unscaled = Lhs_Right_Number_Of_Points;
   int Rhs_Left_Number_Of_Points_unscaled  = Rhs_Left_Number_Of_Points;
   int Rhs_Right_Number_Of_Points_unscaled = Rhs_Right_Number_Of_Points;

   // ... bug fix (10/15/96,kdb) dividing by both Stride_Factor and
   //  by stride causes problems and is confusing.  Simplify by 
   //  rescaling by stride immediately ...

   int Lhs_stride = Lhs_Partition_Index [Axis].getStride();
   int Rhs_stride = Rhs_Partition_Index [Axis].getStride();

   Lhs_Left_Number_Of_Points = (Lhs_Left_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Lhs_Right_Number_Of_Points = (Lhs_Right_Number_Of_Points + Lhs_stride - 1) /
      Lhs_stride;
   Rhs_Left_Number_Of_Points = (Rhs_Left_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;
   Rhs_Right_Number_Of_Points = (Rhs_Right_Number_Of_Points + Rhs_stride - 1) /
      Rhs_stride;

   APP_ASSERT(Lhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Lhs_Right_Number_Of_Points >= 0);
   APP_ASSERT(Rhs_Left_Number_Of_Points  >= 0);
   APP_ASSERT(Rhs_Right_Number_Of_Points >= 0);


   DOMAIN_SIZE_Type Rhs_Size = Rhs_Serial_Descriptor.Size[Axis]; 
   DOMAIN_SIZE_Type Lhs_Size = Lhs_Serial_Descriptor.Size[Axis]; 
   if (Axis>0)
   {
      Rhs_Size /= Rhs_Serial_Descriptor.Size[Axis-1]; 
      Lhs_Size /= Lhs_Serial_Descriptor.Size[Axis-1]; 
   }

   int Lhs_start_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size
       - 2 * Lhs_ghost_cell_width;
   int Lhs_end_right_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] + Lhs_Size -1;

   int Rhs_start_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size
       - 2 * Rhs_ghost_cell_width;
   int Rhs_end_right_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] + Rhs_Size -1;


   int Lhs_start_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis];
   int Lhs_end_left_ghost = 
       Lhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Lhs_ghost_cell_width;

   int Rhs_start_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis];
   int Rhs_end_left_ghost = 
       Rhs_Serial_Descriptor.Data_Base[Axis] - 1
       + 2 * Rhs_ghost_cell_width;


   // ... WARNING: stride > 1 needs to be tested ...

   // ... note (10/15/96,kdb) don't need this anymore ...

   int Stride_Factor = abs ( Lhs_Parallel_Descriptor.Stride[Axis] - 
      Rhs_Parallel_Descriptor.Stride[Axis] ) + 1;

   /*
   if (Lhs_Parallel_Descriptor.Stride[Axis] != 
       Rhs_Parallel_Descriptor.Stride[Axis])
   {
      // We need to use the Maximum stride to interpret the Lhs and Rhs 
      // consistantly

      int Max_Stride = (Lhs_Parallel_Descriptor.Stride[Axis] > 
	 Rhs_Parallel_Descriptor.Stride[Axis]) ?
         Lhs_Parallel_Descriptor.Stride[Axis] : 
	 Rhs_Parallel_Descriptor.Stride[Axis];


      // Temporary code for debugging cases of stride 1 and 2 before 
      // being more general

      APP_ASSERT(Max_Stride >= 0);
      APP_ASSERT(Max_Stride <= 2);

      if (Lhs_Parallel_Descriptor.Stride[Axis] > 
	  Rhs_Parallel_Descriptor.Stride[Axis])
      {
         Rhs_Left_Number_Of_Points  *= Max_Stride;
         Rhs_Right_Number_Of_Points *= Max_Stride;
      }
      else
      {
         Lhs_Left_Number_Of_Points  *= Max_Stride;
         Lhs_Right_Number_Of_Points *= Max_Stride;
      }
   }
   */

   // ... (12/13/96,kdb) it might be impossible to check to
   //  see if other processors other than the neighboring
   //  processor need an update and right now block parti requires
   //  an update for both the high and low end ghost cells if
   //  it does any update so turn on all ghost cell updates
   //  when this situation could occur.  Remove this later when 
   //  block parti is more flexible ...

   int update_all = FALSE;
   int nd;
   for (nd=0; nd<MAX_ARRAY_DIMENSION; nd++)
   {
     if (Lhs_Parallel_Descriptor.Size[nd] != 
         Rhs_Parallel_Descriptor.Size[nd] ) 
     update_all = TRUE;
   }
   if (Lhs_Parallel_Descriptor.Partitioning_Object_Pointer != 
       Rhs_Parallel_Descriptor.Partitioning_Object_Pointer) 
	  update_all = TRUE;



   int Truncation_Width_Base = Lhs_Left_Number_Of_Points - 
      	                       Rhs_Left_Number_Of_Points;

   int Truncation_Width_Bound = Lhs_Right_Number_Of_Points - 
      	                        Rhs_Right_Number_Of_Points;

   // QUESTION: Should this check be done for both the base and 
   //           bound?  This probably doesn't matter until 
   //           strides an greater than 1.

   if (Truncation_Width_Base >= 0)
   {
      // ... (10/15/96,kdb) already scaled properly ...
      //int Adjustment = (Truncation_Width_Base / Stride_Factor) / 
      //  Rhs_Partition_Index[Axis].getStride();
      int Adjustment = Truncation_Width_Base;
      Rhs_Partition_Index[Axis].adjustBase ( Adjustment );
   }
   else
   {
      printf("Error: Lhs base shouldn't need to be adjusted\n");
   }

   if (Truncation_Width_Bound >= 0)
   {
      // ... (10/15/96,kdb) already scaled properly ...
      //int Adjustment = (-Truncation_Width_Bound / Stride_Factor) / 
      //  Rhs_Partition_Index[Axis].getStride();
      int Adjustment = -Truncation_Width_Bound;
      Rhs_Partition_Index[Axis].adjustBound ( Adjustment );
   }
   else
   {
      printf("Error: Lhs bound shouldn't need to be adjusted\n");
   }
}

//=================================================================

void Array_Domain_Type::Fix_Array_Conformability (
          Array_Conformability_Info_Type    & Array_Set,
          const Array_Domain_Type       & Parallel_Descriptor,
          const SerialArray_Domain_Type & Serial_Descriptor,
	  bool reverse_offset)
{
   // ... (10/11/96,kdb) Fix Array_Set so that it is compatible 
   //  with the Local_Mask_Index and the SerialArray ...

   // ... (11/24/96,kdb) only do this when not full vsg ...

   if (!Array_Set.Full_VSG_Update_Required)
   {
      int nd;
      for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
      {
	if (!Serial_Descriptor.Is_A_Null_Array)
	{
           int serial_base  = Serial_Descriptor.getRawBase(nd); 
           int serial_bound = Serial_Descriptor.getRawBound(nd); 
           int local_mask_base  = Parallel_Descriptor.Local_Mask_Index[nd].getBase();
           int local_mask_bound = Parallel_Descriptor.Local_Mask_Index[nd].getBound();
           int local_mask_stride = 
	      Parallel_Descriptor.Local_Mask_Index[nd].getStride();

           // ... bug fix (10/15/96,kdb) the truncation width needs to be
           //  divided by stride ...
           Array_Set.Truncate_Left_Ghost_Boundary_Width[nd]=
              (serial_base - local_mask_base)/local_mask_stride;
	      //serial_base - local_mask_base;
           Array_Set.Truncate_Right_Ghost_Boundary_Width[nd]=
      	      (serial_bound - local_mask_bound)/local_mask_stride;  
	      //serial_bound - local_mask_bound;  

           if (!Array_Set.Update_Left_Ghost_Boundary_Width[nd])
              Array_Set.Update_Left_Ghost_Boundary_Width[nd]
	      = (Array_Set.Truncate_Left_Ghost_Boundary_Width[nd] != 0);
           if (!Array_Set.Update_Right_Ghost_Boundary_Width[nd])
              Array_Set.Update_Right_Ghost_Boundary_Width[nd]
	      = (Array_Set.Truncate_Right_Ghost_Boundary_Width[nd] != 0);
	}

     // Why we have the "reverse_offset" feature:
     // The Array_Set is usually associated with the offset of the Lhs from
     // the Rhs.  If somehow a pointer to Array_Set is set for the Rhs instead
     // of the Lhs the offsets need to be reversed.  I know this is a bit vague
     // and I will comment it better later but I will have to work through
     // how it is set to give a real detailed explaination.
     //                  X X X X X | X X X      Lhs
     //                    X X X X | X X X X    Rhs
     // The offset of Lhs from Rhs might be -1 while the offset of Rhs from
     // Lhs is 1.  This function resets the appropriate values.  I hope this
     // makes some sense at least.

        if (reverse_offset)
	{
           Array_Set.Left_Number_Of_Points_Truncated[nd] =
             -Array_Set.Left_Number_Of_Points_Truncated[nd];
           Array_Set.Right_Number_Of_Points_Truncated[nd] =
             -Array_Set.Right_Number_Of_Points_Truncated[nd];
	}
      }
   }
}

//##########################################################################
//             Cases for Gost Boundary Check
//##########################################################################

int Array_Domain_Type::Check_Ghost_Case_Left_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Parallel_Descriptor,
          const SerialArray_Domain_Type & Serial_Descriptor,
          Internal_Index *Partition_Index,
          int Axis )
{
   // This macro forces the setup to be the same for each of the Left 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int gbase  = Parallel_Descriptor.Global_Index[Axis].getBase();
   int gbound = Parallel_Descriptor.Global_Index[Axis].getBound();

   DOMAIN_SIZE_Type Dim_Size = Serial_Descriptor.Size[Axis];
   if (Axis>0)
       Dim_Size /= Serial_Descriptor.Size[Axis-1];

   // ... these are the actual ghost cells and not the ghost cell overlap
   //   region like above so the ghost cell width isn't multiplied
   //   by 2 in this case ...

   int start_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size
      - Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int end_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size -1;

   int start_left_ghost = 
       Serial_Descriptor.Data_Base[Axis];
   int end_left_ghost = 
       Serial_Descriptor.Data_Base[Axis] - 1
       + Parallel_Descriptor.InternalGhostCellWidth[Axis];



   // ... assume that this function wouldn't even be called if not overlapping
   //  boundary in some dimension and so only look for cells in ghost region
   //  in this dimension ...

   Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

   if (gbase >= start_right_ghost) 
   {
      // ... only intersects ghost cell region so use overlapping
      //  boundary region and make a null array ...


      int Adjustment = -Partition_Index[Axis].getCount();
      Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis] = 
         Adjustment;

      Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;

   }
   // ... Make sure this is turned off because global checking code
   //  might turn it on ...
   Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = FALSE;


   return Return_Communication_Model;
}

//=================================================================

int Array_Domain_Type::Check_Ghost_Case_Middle_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Parallel_Descriptor,
          const SerialArray_Domain_Type & Serial_Descriptor,
          Internal_Index *Partition_Index,
          int Axis )
{
   // This macro forces the setup to be the same for each of the Left 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int gbase  = Parallel_Descriptor.Global_Index[Axis].getBase();
   int gbound = Parallel_Descriptor.Global_Index[Axis].getBound();

   DOMAIN_SIZE_Type Dim_Size = Serial_Descriptor.Size[Axis];
   if (Axis>0)
       Dim_Size /= Serial_Descriptor.Size[Axis-1];

   // ... these are the actual ghost cells and not the ghost cell overlap
   //   region like above so the ghost cell width isn't multiplied
   //   by 2 in this case ...

   int start_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size
      - Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int end_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size -1;

   int start_left_ghost = 
       Serial_Descriptor.Data_Base[Axis];
   int end_left_ghost = 
       Serial_Descriptor.Data_Base[Axis] - 1
       + Parallel_Descriptor.InternalGhostCellWidth[Axis];



   // ... assume that this function wouldn't even be called if not overlapping
   //  boundary in some dimension and so only look for cells in ghost region
   //  in this dimension ...

   Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

   if (gbase >= start_right_ghost)
   {
      // ... only intersects ghost cell region so use overlapping
      //  boundary region and make a null array ...

      int Adjustment = -Partition_Index[Axis].getCount();
      Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[Axis] = 
         Adjustment;

      Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;
   }
   else if (gbound < start_left_ghost +
            Parallel_Descriptor.InternalGhostCellWidth[Axis])
   {
      // ... only intersects ghost cell region so use overlapping
      //  boundary region and make a null array ...

      int Adjustment = Partition_Index[Axis].getCount();
      Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis] = 
         Adjustment;

      Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = TRUE;

   }

   return Return_Communication_Model;
}
//=================================================================

int
Array_Domain_Type::Check_Ghost_Case_Right_Processor (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Parallel_Descriptor,
          const SerialArray_Domain_Type & Serial_Descriptor,
          Internal_Index *Partition_Index, int Axis )
{
   // This macro forces the setup to be the same for each of the Left, 
   // Right and Middle processor cases

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   int gbase  = Parallel_Descriptor.Global_Index[Axis].getBase();
   int gbound = Parallel_Descriptor.Global_Index[Axis].getBound();

   DOMAIN_SIZE_Type Dim_Size = Serial_Descriptor.Size[Axis];
   if (Axis>0)
       Dim_Size /= Serial_Descriptor.Size[Axis-1];

   // ... these are the actual ghost cells and not the ghost cell overlap
   //   region like above so the ghost cell width isn't multiplied
   //   by 2 in this case ...

   int start_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size
      - Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int end_right_ghost = 
      Serial_Descriptor.Data_Base[Axis] + Dim_Size -1;

   int start_left_ghost = 
       Serial_Descriptor.Data_Base[Axis];
   int end_left_ghost = 
       Serial_Descriptor.Data_Base[Axis] - 1
       + Parallel_Descriptor.InternalGhostCellWidth[Axis];



   // ... assume that this function wouldn't even be called if not overlapping
   //  boundary in some dimension and so only look for cells in ghost region
   //  in this dimension ...

   Return_Communication_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

   if (gbound < start_left_ghost +
       Parallel_Descriptor.InternalGhostCellWidth[Axis])
   {
      // ... only intersects ghost cell region so use overlapping
      //  boundary region and make a null array ...


      int Adjustment = Partition_Index[Axis].getCount();
      Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[Axis] = 
         Adjustment;

      Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = TRUE;
   }

   // ... Make sure this is turned off because global checking code
   //  might turn it on ...
   Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = FALSE;


   return Return_Communication_Model;
}

//##########################################################################
//            Check Global Values 
//##########################################################################

int
Array_Domain_Type::Check_Global_Values (
          Array_Conformability_Info_Type  & Return_Array_Set,
          const Array_Domain_Type       & Lhs_Parallel_Descriptor,
          const Array_Domain_Type       & Rhs_Parallel_Descriptor,
          int Axis )
{
// This function figure out how to align the global accesses and
// if what that takes is within the limits of the ghost boundary widths
// then the OVERLAP_UPDATE can be used (cheaper) else the
// FULL_VSG_UPDATE is required.

   int Return_Communication_Model = UNDEFINED_VSG_MODEL;

   DOMAIN_SIZE_Type Rhs_Size = Rhs_Parallel_Descriptor.Size[Axis]; 
   DOMAIN_SIZE_Type Lhs_Size = Lhs_Parallel_Descriptor.Size[Axis]; 
   if (Axis>0)
   {
      Rhs_Size /= Rhs_Parallel_Descriptor.Size[Axis-1]; 
      Lhs_Size /= Lhs_Parallel_Descriptor.Size[Axis-1]; 
   }

   int Lhs_Left_Offset  = Lhs_Parallel_Descriptor.Base[Axis];
   int Lhs_Right_Offset = Lhs_Size - Lhs_Parallel_Descriptor.Base[Axis] - 1;
   int Rhs_Left_Offset  = Rhs_Parallel_Descriptor.Base[Axis];
   int Rhs_Right_Offset = Rhs_Size - Rhs_Parallel_Descriptor.Base[Axis] - 1;


   int Lhs_stride = Lhs_Parallel_Descriptor.Stride[Axis];
   int Rhs_stride = Rhs_Parallel_Descriptor.Stride[Axis];

// ... wrong scaling ...
#if 0
   Lhs_Left_Offset = (Lhs_Left_Offset + Lhs_stride - 1) /
      Lhs_stride;
   Lhs_Right_Offset = (Lhs_Right_Offset + Lhs_stride - 1) /
      Lhs_stride;
   Rhs_Left_Offset = (Rhs_Left_Offset + Rhs_stride - 1) /
      Rhs_stride;
   Rhs_Right_Offset = (Rhs_Right_Offset + Rhs_stride - 1) /
      Rhs_stride;
#endif

   APP_ASSERT(Lhs_Left_Offset  >= 0);
   APP_ASSERT(Lhs_Right_Offset >= 0);
   APP_ASSERT(Rhs_Left_Offset  >= 0);
   APP_ASSERT(Rhs_Right_Offset >= 0);

   int Lhs_ghost_cell_width = Lhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];
   int Rhs_ghost_cell_width = Rhs_Parallel_Descriptor.InternalGhostCellWidth[Axis];

   int factor = 1;
   int Truncation_Width_Base  = Rhs_Left_Offset  - Lhs_Left_Offset;
   int Truncation_Width_Bound = Rhs_Right_Offset - Lhs_Right_Offset;

   if (Lhs_Parallel_Descriptor.Partitioning_Object_Pointer != 
       Rhs_Parallel_Descriptor.Partitioning_Object_Pointer) 
   {
      Return_Communication_Model = FULL_VSG_MODEL;
   }
   else if (Lhs_Size != Rhs_Size)
   {
     // ... for now if arrays have a different distribution force a 
     //  full vsg because it's difficult to determine globally if 
     //  a full vsg needs to be done ...

      Return_Communication_Model = FULL_VSG_MODEL;

   }
   else if ((abs (Truncation_Width_Base) > (factor * Lhs_ghost_cell_width)) ||
           ( abs (Truncation_Width_Bound) > (factor * Lhs_ghost_cell_width)))
   {
      Return_Communication_Model = FULL_VSG_MODEL;
   }
   else
   {
     // ... Find distance of Rhs from any of the other arrays used in this
     //  expression ...

     if (Return_Array_Set.Left_Number_Of_Points_Truncated[Axis] >=0)
     {
	if (Truncation_Width_Base >= 0) 
	{
	   Truncation_Width_Base =
	      MAX(Truncation_Width_Base, 
                  Return_Array_Set.Left_Number_Of_Points_Truncated[Axis]);
	}
        else
	{
	   Truncation_Width_Base =
	      Truncation_Width_Base - 
                  Return_Array_Set.Left_Number_Of_Points_Truncated[Axis];
	}
     }
     else
     {
	if (Truncation_Width_Base <= 0) 
	{
	   Truncation_Width_Base =
	      MIN(Truncation_Width_Base, 
                  Return_Array_Set.Left_Number_Of_Points_Truncated[Axis]);
	}
        else
	{
	   Truncation_Width_Base =
	      Truncation_Width_Base - 
                  Return_Array_Set.Left_Number_Of_Points_Truncated[Axis];
	}
     }

     if (Return_Array_Set.Right_Number_Of_Points_Truncated[Axis] >=0)
     {
	if (Truncation_Width_Bound >= 0) 
	{
	   Truncation_Width_Bound =
	      MAX(Truncation_Width_Bound, 
                 Return_Array_Set.Right_Number_Of_Points_Truncated[Axis]);
	}
	else
	{
	   Truncation_Width_Bound =
	      Truncation_Width_Bound - 
                  Return_Array_Set.Right_Number_Of_Points_Truncated[Axis];
	}
     }
     else
     {
	if (Truncation_Width_Bound <= 0) 
	{
	   Truncation_Width_Bound =
	      MIN(Truncation_Width_Bound, 
                  Return_Array_Set.Right_Number_Of_Points_Truncated[Axis]);
	}
	else
	{
	   Truncation_Width_Bound =
	      Truncation_Width_Bound - 
                  Return_Array_Set.Right_Number_Of_Points_Truncated[Axis];
	}
     }

     // ... check to see if current Rhs array is too far off from a previous
     //  array ...

     if ((abs (Truncation_Width_Base) > (factor * Lhs_ghost_cell_width)) ||
        ( abs (Truncation_Width_Bound) > (factor * Lhs_ghost_cell_width)))
     {
        Return_Communication_Model = FULL_VSG_MODEL;
     }
     else
     {
        Return_Array_Set.Left_Number_Of_Points_Truncated[Axis]  = Truncation_Width_Base; 
        Return_Array_Set.Right_Number_Of_Points_Truncated[Axis] = Truncation_Width_Bound; 

        if ((Return_Array_Set.Left_Number_Of_Points_Truncated[Axis]!= 0) ||
            (Return_Array_Set.Right_Number_Of_Points_Truncated[Axis]!= 0))
        {
           Return_Array_Set.Update_Left_Ghost_Boundary_Width[Axis] = TRUE;
           Return_Array_Set.Update_Right_Ghost_Boundary_Width[Axis] = TRUE;
        }
     }
   }

   /*
   if (Return_Communication_Model == FULL_VSG_MODEL)
   {
      int nd;
      for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
      {
        Return_Array_Set.Left_Number_Of_Points_Truncated[nd] = 0;
        Return_Array_Set.Right_Number_Of_Points_Truncated[nd] = 0;
        Return_Array_Set.Update_Left_Ghost_Boundary_Width[nd] = FALSE;
        Return_Array_Set.Update_Right_Ghost_Boundary_Width[nd] = FALSE;
        Return_Array_Set.Truncate_Left_Ghost_Boundary_Width[nd] = 0;
        Return_Array_Set.Truncate_Right_Ghost_Boundary_Width[nd] = 0;
      }
   }
   */
	   
   return Return_Communication_Model;
}

//=================================================================







//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------

