#define COMPILE_PPP

#include "A++.h"

#if !defined(PPP)
#error (conform_enforce.C) This is only code for P++
#endif

 // Data required for the "new" and "delete" operators!
static Array_Conformability_Info_Type *Current_Link                    = NULL;
static const int Max_Number_Of_Memory_Blocks                           = MAX_NUMBER_OF_MEMORY_BLOCKS;
static unsigned char *Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];
static int Memory_Block_Index                                          = 0;

// *********************************************************
// This function is useful in conjuction with the Purify (from Pure Software Inc.)
// it frees memory allocated for use internally in A++ Array_Conformability_Info_Type
//  objects.  This memory is used internally and is reported as "in use" by Purify
// if it is not freed up using this function.  This function works with
// similar functions for each A++ object to free up all of the A++ memory in
// use internally.
// *********************************************************
void
Array_Conformability_Info_Type::freeMemoryInUse()
   {
     for (int i=0; i < Max_Number_Of_Memory_Blocks-1; i++)
          if (Memory_Block_List [i] != NULL)
               free ((char*) (Memory_Block_List[i]));
   }


void
Array_Conformability_Info_Type::Initialize ()
   {
  // Last_Update_Was_Overlap_Update = FALSE;
  // Full_VSG_Update_Required       = FALSE;
     Full_VSG_Update_Required       = (Optimization_Manager::ForceVSG_Update == TRUE) ? TRUE : FALSE;
     
     referenceCount                 = getReferenceCountBase();
     for (int i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
#if 0
          Left_Boundary_Accessed_Flag    [i] = FALSE;
          Left_Overlap_Accessed_Flag     [i] = FALSE;
          Right_Boundary_Accessed_Flag   [i] = FALSE;
          Right_Overlap_Accessed_Flag    [i] = FALSE;
          Lhs_Aggregate_Start_Truncation [i] = 0;
          Lhs_Aggregate_End_Truncation   [i] = 0;
          Aggregate_Lhs_Truncate_Start_In_Middle_Processor [i] = 0;
          Aggregate_Lhs_Truncate_End_In_Middle_Processor   [i] = 0;
#endif
          Truncate_Left_Ghost_Boundary_Width  [i] = 0;
          Truncate_Right_Ghost_Boundary_Width [i] = 0;
          Update_Left_Ghost_Boundary_Width    [i] = 0;
          Update_Right_Ghost_Boundary_Width   [i] = 0;
          Offset_To_Centered_Data_Storage     [i] = 0;
          Left_Number_Of_Points_Truncated     [i] = 0;
          Right_Number_Of_Points_Truncated    [i] = 0;
          Data_Storage_Offset_In_Temporary    [i] = 0;
        }
   }

Array_Conformability_Info_Type::~Array_Conformability_Info_Type ()
   {
  // Initialize ();
  // if (referenceCount < 0)
  //    {
          Full_VSG_Update_Required       = -9999;
       // Leave this value unset (it should be -1)
       // referenceCount                 = 0;
          for (int i=0; i < MAX_ARRAY_DIMENSION; i++)
             {
               Truncate_Left_Ghost_Boundary_Width  [i] = -9999;
               Truncate_Right_Ghost_Boundary_Width [i] = -9999;
               Update_Left_Ghost_Boundary_Width    [i] = -9999;
               Update_Right_Ghost_Boundary_Width   [i] = -9999;
               Offset_To_Centered_Data_Storage     [i] = -9999;
               Left_Number_Of_Points_Truncated     [i] = -9999;
               Right_Number_Of_Points_Truncated    [i] = -9999;
               Data_Storage_Offset_In_Temporary    [i] = -9999;
             }
  //    }

     referenceCount                 = -99999;
   }

Array_Conformability_Info_Type::Array_Conformability_Info_Type ()
   {
  // ... allow constructor that just sets default values ...
     Initialize ();
 //  printf ("ERROR: Should be calling constructor Array_Conformability_Info_Type ( const Array_Descriptor_Type & X ) \n");
 //  APP_ABORT();
   }

//Array_Conformability_Info_Type::Array_Conformability_Info_Type ( const Array_Descriptor_Type & X )
Array_Conformability_Info_Type::Array_Conformability_Info_Type ( const Array_Domain_Type & X )
   {
     Initialize ();
  // APP_ASSERT (X.Is_A_Null_Array == FALSE);
     for (int i=0; i < MAX_ARRAY_DIMENSION; i++)
        {
       // Offset_To_Centered_Data_Storage = X.Base[i];
          Data_Storage_Offset_In_Temporary [i] = X.Base[i];
        }
#if 0
     Lhs_Lvalue_Assignment_Index[0] = X.Local_Mask_Index[0];
     Lhs_Lvalue_Assignment_Index[1] = X.Local_Mask_Index[1];
     Lhs_Lvalue_Assignment_Index[2] = X.Local_Mask_Index[2];
     Lhs_Lvalue_Assignment_Index[3] = X.Local_Mask_Index[3];
#endif
   }

Array_Conformability_Info_Type & Array_Conformability_Info_Type::operator= ( const Array_Conformability_Info_Type & X )
   {
     printf ("ERROR: Array_Conformability_Info_Type::operator= not implemented yet! \n");
     APP_ABORT();

     return *this;
   }

Array_Conformability_Info_Type::Array_Conformability_Info_Type ( const Array_Conformability_Info_Type & X )
   {
     printf ("ERROR: Array_Conformability_Info_Type::Array_Conformability_Info_Type ( const Array_Conformability_Info_Type & X ) not implemented yet! \n");
     APP_ABORT();
   }

void
Array_Conformability_Info_Type::display( const char* Label ) const
   {
  // save the current state and turn off the output of node number prefix to all strings
     bool printfState = Communication_Manager::getPrefixParallelPrintf();
     Communication_Manager::setPrefixParallelPrintf(FALSE);

     int i = 0;
     printf ("Array_Conformability_Info_Type::display() -- %s (address = %p) \n",Label,this);

     printf ("referenceCount       ================ %d \n",referenceCount);
     printf ("Full_VSG_Update_Required       ====== %s \n",(Full_VSG_Update_Required)       ? "TRUE" : "FALSE");

     printf ("Truncate_Left_Ghost_Boundary_Width == ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Truncate_Left_Ghost_Boundary_Width[i]);
     printf ("\n");

     printf ("Truncate_Right_Ghost_Boundary_Width = ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Truncate_Right_Ghost_Boundary_Width[i]);
     printf ("\n");

     printf ("Update_Left_Ghost_Boundary_Width ==== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Update_Left_Ghost_Boundary_Width[i]);
     printf ("\n");

     printf ("Update_Right_Ghost_Boundary_Width === ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Update_Right_Ghost_Boundary_Width[i]);
     printf ("\n");

     printf ("Offset_To_Centered_Data_Storage ===== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Offset_To_Centered_Data_Storage[i]);
     printf ("\n");

     printf ("Left_Number_Of_Points_Truncated ===== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Left_Number_Of_Points_Truncated[i]);
     printf ("\n");

     printf ("Right_Number_Of_Points_Truncated ==== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Right_Number_Of_Points_Truncated[i]);
     printf ("\n");

     printf ("Data_Storage_Offset_In_Temporary ===== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Data_Storage_Offset_In_Temporary[i]);
     printf ("\n");
#if 0
  // printf ("Last_Update_Was_Overlap_Update =================== %s \n",(Last_Update_Was_Overlap_Update) ? "TRUE" : "FALSE");
     printf ("Lhs_Lvalue_Assignment_Index ====================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          Lhs_Lvalue_Assignment_Index[i].display("Lhs_Lvalue_Assignment_Index");

     printf ("Left_Boundary_Accessed_Flag ====================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Left_Boundary_Accessed_Flag[i]);
     printf ("\n");

     printf ("Right_Boundary_Accessed_Flag ===================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Right_Boundary_Accessed_Flag[i]);
     printf ("\n");

     printf ("Left_Overlap_Accessed_Flag ======================= ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Left_Overlap_Accessed_Flag[i]);
     printf ("\n");

     printf ("Right_Overlap_Accessed_Flag ====================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Right_Overlap_Accessed_Flag[i]);
     printf ("\n");

     printf ("Lhs_Aggregate_Start_Truncation =================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Lhs_Aggregate_Start_Truncation[i]);
     printf ("\n");

     printf ("Lhs_Aggregate_End_Truncation ===================== ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Lhs_Aggregate_End_Truncation[i]);
     printf ("\n");

     printf ("Aggregate_Lhs_Truncate_Start_In_Middle_Processor = ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Aggregate_Lhs_Truncate_Start_In_Middle_Processor[i]);
     printf ("\n");

     printf ("Aggregate_Lhs_Truncate_End_In_Middle_Processor === ");
     for (i=0; i < MAX_ARRAY_DIMENSION; i++)
          printf (" %d ",Aggregate_Lhs_Truncate_End_In_Middle_Processor[i]);
     printf ("\n");
#endif

  // reset output of node number prefix to all strings
     Communication_Manager::setPrefixParallelPrintf(printfState);

     printf ("\n");
   }

void
Array_Conformability_Info_Type::Test_Consistency( const char *Label ) const
   {
  // Only used when EXTRA_ERROR_CHECKING is TRUE
#if (EXTRA_ERROR_CHECKING == FALSE)
     printf ("A++ version (EXTRA_ERROR_CHECKING) was incorrectly built since Array_Conformability_Info_Type::Test_Consistency (%s) was called! \n",Label);
     APP_ABORT();
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 5)
          printf ("Inside of Array_Conformability_Info_Type::Test_Consistency! (Label = %s) \n",Label);
#endif
     APP_ASSERT (referenceCount >= getReferenceCountBase());

     if (Update_Left_Ghost_Boundary_Width  [0] < 0)
          display(Label);

     APP_ASSERT (Update_Left_Ghost_Boundary_Width  [0] >= 0);
     APP_ASSERT (Update_Left_Ghost_Boundary_Width  [1] >= 0);
     APP_ASSERT (Update_Left_Ghost_Boundary_Width  [2] >= 0);
     APP_ASSERT (Update_Left_Ghost_Boundary_Width  [3] >= 0);
     APP_ASSERT (Update_Right_Ghost_Boundary_Width [0] >= 0);
     APP_ASSERT (Update_Right_Ghost_Boundary_Width [1] >= 0);
     APP_ASSERT (Update_Right_Ghost_Boundary_Width [2] >= 0);
     APP_ASSERT (Update_Right_Ghost_Boundary_Width [3] >= 0);

// ... this isn't required now because these are used for global test ...
#if 0
     APP_ASSERT (Left_Number_Of_Points_Truncated  [0] >= 0);
     APP_ASSERT (Left_Number_Of_Points_Truncated  [1] >= 0);
     APP_ASSERT (Left_Number_Of_Points_Truncated  [2] >= 0);
     APP_ASSERT (Left_Number_Of_Points_Truncated  [3] >= 0);
     APP_ASSERT (Right_Number_Of_Points_Truncated [0] >= 0);
     APP_ASSERT (Right_Number_Of_Points_Truncated [1] >= 0);
     APP_ASSERT (Right_Number_Of_Points_Truncated [2] >= 0);
     APP_ASSERT (Right_Number_Of_Points_Truncated [3] >= 0);

     APP_ASSERT (Data_Storage_Offset_In_Temporary [0] >= 0);
     APP_ASSERT (Data_Storage_Offset_In_Temporary [1] >= 0);
     APP_ASSERT (Data_Storage_Offset_In_Temporary [2] >= 0);
     APP_ASSERT (Data_Storage_Offset_In_Temporary [3] >= 0);
#endif

#if 0
  // Mode could be Null_Index so just make sure it is not and All_Index
     APP_ASSERT (Lhs_Lvalue_Assignment_Index[0].getMode() != All_Index);
     APP_ASSERT (Lhs_Lvalue_Assignment_Index[1].getMode() != All_Index);
     APP_ASSERT (Lhs_Lvalue_Assignment_Index[2].getMode() != All_Index);
     APP_ASSERT (Lhs_Lvalue_Assignment_Index[3].getMode() != All_Index);

#if 1
  // This test is only meaningful if the test problem is small enough
     int Arbitrary_Bound = 50;
     APP_ASSERT (abs(Lhs_Aggregate_Start_Truncation[0]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_Start_Truncation[1]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_Start_Truncation[2]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_Start_Truncation[3]) <= Arbitrary_Bound);

     APP_ASSERT (abs(Lhs_Aggregate_End_Truncation[0]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_End_Truncation[1]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_End_Truncation[2]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Lhs_Aggregate_End_Truncation[3]) <= Arbitrary_Bound);

     APP_ASSERT (abs(Aggregate_Lhs_Truncate_Start_In_Middle_Processor[0]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_Start_In_Middle_Processor[1]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_Start_In_Middle_Processor[2]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_Start_In_Middle_Processor[3]) <= Arbitrary_Bound);

     APP_ASSERT (abs(Aggregate_Lhs_Truncate_End_In_Middle_Processor[0]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_End_In_Middle_Processor[1]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_End_In_Middle_Processor[2]) <= Arbitrary_Bound);
     APP_ASSERT (abs(Aggregate_Lhs_Truncate_End_In_Middle_Processor[3]) <= Arbitrary_Bound);
#endif
#endif
   }



