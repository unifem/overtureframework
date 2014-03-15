#define COMPILE_PPP

bool automaticCommunication = true; //  *wdh* turn off auto communication

// kkc 061031 include mpi here to avoid mpich/bsparti error
#include <mpi.h>
#include "A++.h"

#if defined(USE_PARALLEL_INDIRECT_ADDRESSING_SUPPORT)
// low level support for indirect addressing
#include "CommunicationScheduler.h"
#endif

#define MAX(A,B) ( (A) > (B) ) ? (A) : (B);
#define MIN(A,B) ( (A) < (B) ) ? (A) : (B);


// This file contains the Parallel Conformability Enforcement (PCE) 
// functions.  The point of these functions is to take a number of 
// parallel array objects and return an equal number of serial array 
// objects and a history of what message passing is required later (if 
// any). 
//    The serial arrays returned are to be deleted within the parallel 
// operators that called the PCE function.  So the reference counts must 
// be manipulated to make this work.
//    1) If the PCE function returns a view then the raw data reference 
//       count is incremented in the process of taking the view.
//    2) If the PCE function returns the original array or another array 
//       then the reference count for that array object (not the raw data
//       in the array object) is incremented so as to permit that the 
//       delete in the parallel operator that called the PCE function 
//       either destroys the array (if it was newly created within the 
//       PCE function) or that it decrements its reference count (when 
//       it is deleted).  
// The point is that the interface is simple.  what complicates the 
// situation is that the serial arrays are used in more complicated ways 
// within the serial binary operation (because A++ is highly optimized
// to reuse temporaries etc.).  
//
//     The parallel abstract binary operators are next in line (called 
// from the parallel operator functions) They receive the parallel arrays
// and the result of the serial binary operation.  They never quite now 
// what they get since the A++ operations are so highly optimized and 
// this has been a problem in tracing memory leaks.


#define SIMULATE_MULTIPROCESSOR TRUE

// Here we can force the use of the VSG update which forces all binary 
// operations to pass messages (if required by the way they are 
// referenced (i.e. Indexed)).  Making this FALSE allows for the 
// alternate and more efficient communication model to be used.
// VSG update: The VSG update assumes the Lhs is the owner and then 
//   performs an owner computes rule for each binary operation.
// Overlap update: The overlap update is more efficient than the VSG 
//   update and records the how arrays are referenced relative to one 
//   another so that it can store and accumulate the information about 
//   how message passing should be used update the 
// The more efficient communication model stores the references to the 
// internal ghost boundaries and defers the update of the ghost boundary 
// until the equals operator is called (or similar statement terminating 
// operation).  This model will only do message passing on the Lhs 
// operand for the statement.  Where as the VSG update will force the 
// update of the ghost boundaries within each temporary created (or 
// reused) in every binary operator.  The VSG update is robust yet slower
// -- while the overlap update is faster but only works within the limits
// of the ghost boundaries provided (thus in general only with arrays 
// that are aligned with one another).

#define FORCE_VSG_UPDATE (Optimization_Manager::ForceVSG_Update == TRUE)

#if 0
// This will fail for certain cases but I want to get the cases for which
// it will work working before I worry about the side effects issue.  
// This used to work fine so it is a matter of fixing it again.
// the view of a temporary issue which was fixed in A++ has a 
// repercussion on how this model works so we have to recover from that 
// and other fixes to A++/P++ during the last year.
// define USING_OLDER_SIDE_EFFECT_METHOD TRUE
#endif



/* EXPAND THE MACROS HERE! */







//======================================================================
Array_Conformability_Info_Type*
Array_Domain_Type::getArray_Conformability_Info ()
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #1 Array_Domain_Type::getArray_Conformability_Info () \n");
#endif

     Array_Conformability_Info_Type* Return_Array_Set = NULL;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 2)
          printf ("Building a new Array_Conformability_Info_Type \n");
#endif

  // We have to be able to reuse this Array_Conformability_Info_Type object!
     Return_Array_Set = new Array_Conformability_Info_Type();
     APP_ASSERT(Return_Array_Set != NULL);

  // Bugfix (12/5/2000) this is redundent since the default constructor calls the same Initialize member function
  // ... (bug fix, 6/6/96, kdb) this is necessary because
  // the object may come from a list and not be initialized ...
  // printf ("In Array_Domain_Type::getArray_Conformability_Info(): Reinitializing the Array_Conformability_Info_Type object! \n");
  // Return_Array_Set->Initialize();

#if 0
     if (Return_Array_Set->getReferenceCount() != Array_Conformability_Info_Type::getReferenceCountBase())
        {
          printf ("Return_Array_Set->getReferenceCount() = %d \n",Return_Array_Set->getReferenceCount());
          printf ("Array_Conformability_Info_Type::getReferenceCountBase() = %d \n",
               Array_Conformability_Info_Type::getReferenceCountBase());
        }
#endif

     APP_ASSERT(Return_Array_Set->getReferenceCount() == Array_Conformability_Info_Type::getReferenceCountBase());

     return Return_Array_Set;
   }

//======================================================================
Array_Conformability_Info_Type*
Array_Domain_Type::getArray_Conformability_Info (const Array_Domain_Type & X_Descriptor )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #2 Array_Domain_Type::getArray_Conformability_Info () \n");
     X_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
#endif

     Array_Conformability_Info_Type* Return_Array_Set = NULL;

  // We have to be able to reuse this Array_Conformability_Info_Type object!
     if ( X_Descriptor.Array_Conformability_Info != NULL )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
               printf ("Able to use the Array_Conformability_Info_Type object from the X_Descriptor \n");
#endif

          Return_Array_Set = X_Descriptor.Array_Conformability_Info;
          Return_Array_Set->incrementReferenceCount();
        }
       else
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
               printf ("Building a new Array_Conformability_Info_Type \n");
#endif

          Return_Array_Set = new Array_Conformability_Info_Type(X_Descriptor);
          APP_ASSERT(Return_Array_Set != NULL);
          APP_ASSERT(Return_Array_Set->getReferenceCount() == Array_Conformability_Info_Type::getReferenceCountBase());

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
               printf ("Built a new Array_Conformability_Info_Type (Return_Array_Set = %p) \n",Return_Array_Set);
#endif
        }

     APP_ASSERT( Return_Array_Set != NULL );

#if COMPILE_DEBUG_STATEMENTS
     Return_Array_Set->Test_Consistency ("Test Array_Set at BASE of getArray_Conformability_Info()");
#endif

     return Return_Array_Set;
   }

Array_Conformability_Info_Type*
Array_Domain_Type::getArray_Conformability_Info ( const Array_Domain_Type & Lhs_Descriptor , const Array_Domain_Type & Rhs_Descriptor )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #3 Array_Domain_Type::getArray_Conformability_Info () \n");
     Lhs_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
     Rhs_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
#endif

     Array_Conformability_Info_Type* Return_Array_Set = NULL;

     if ( Lhs_Descriptor.Array_Conformability_Info != NULL )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
               printf ("Able to use the Array_Conformability_Info_Type object from the Lhs_ParallelArray \n");
#endif

          Return_Array_Set = Lhs_Descriptor.Array_Conformability_Info;
          APP_ASSERT( Return_Array_Set != NULL );
          Return_Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
          Return_Array_Set->Test_Consistency ("Test Array_Set at BASE of getArray_Conformability_Info()");
#endif
        }
       else
        {
          if ( Rhs_Descriptor.Array_Conformability_Info != NULL )
             {
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 2)
                    printf ("Able to use the Array_Conformability_Info_Type object from the Rhs_ParallelArray \n");
#endif

               Return_Array_Set = Rhs_Descriptor.Array_Conformability_Info;
               APP_ASSERT( Return_Array_Set != NULL );
               Return_Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
               Return_Array_Set->Test_Consistency ("Test Array_Set at BASE of getArray_Conformability_Info()");
#endif
             }
            else
             {
            // If neither have acumulated an Array_Set then we build a new 
	    // one using the Lhs_Descriptor

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 2)
                    printf ("Building a new Array_Conformability_Info_Type \n");
#endif

               Return_Array_Set = new Array_Conformability_Info_Type(Lhs_Descriptor);

               APP_ASSERT(Return_Array_Set != NULL);
               APP_ASSERT(Return_Array_Set->getReferenceCount() == Array_Conformability_Info_Type::getReferenceCountBase());

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 2)
                    printf ("Built a new Array_Conformability_Info_Type (Return_Array_Set = %p) \n",Return_Array_Set);
#endif
             }
        }

     return Return_Array_Set;
   }


Array_Conformability_Info_Type*
Array_Domain_Type::getArray_Conformability_Info (
   const Array_Domain_Type & This_Descriptor , 
   const Array_Domain_Type & Lhs_Descriptor , 
   const Array_Domain_Type & Rhs_Descriptor )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #4 Array_Domain_Type::getArray_Conformability_Info () \n");
     This_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
     Lhs_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
     Rhs_Descriptor.Test_Consistency ("In getArray_Conformability_Info");
#endif

     Array_Conformability_Info_Type* Return_Array_Set = NULL;

  // The THIS descriptor has a higher priority over the Lhs or Rhs
     if ( This_Descriptor.Array_Conformability_Info != NULL )
        {
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
               printf ("Able to use the Array_Conformability_Info_Type object from the This_ParallelArray \n");
#endif

          Return_Array_Set = This_Descriptor.Array_Conformability_Info;
          APP_ASSERT( Return_Array_Set != NULL );
          Return_Array_Set->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
          Return_Array_Set->Test_Consistency ("Test Array_Set at BASE of getArray_Conformability_Info()");
#endif
        }
       else
        {
       // Call the existing function to retrive the object from either the Lhs or Rhs
          Return_Array_Set = getArray_Conformability_Info ( Lhs_Descriptor , Rhs_Descriptor );
        }

     return Return_Array_Set;
   }

//================================================================

int
Array_Domain_Type::Compute_Overlap_Update (
     int Processor_Position,
     Array_Conformability_Info_Type & Return_Array_Set,
     const Array_Domain_Type & Lhs_Descriptor,
     const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
     const Array_Domain_Type & Rhs_Descriptor,
     const SerialArray_Domain_Type & Rhs_Serial_Descriptor,
     Internal_Index* Lhs_Partition_Index,
     Internal_Index* Rhs_Partition_Index,
     int Axis)
   {
  // Error checking -- Test the processor space
     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <=  MAX_PROCESSORS);

     int VSG_Model = UNDEFINED_VSG_MODEL;
     switch (Processor_Position)
        {
          case SINGLE_PROCESSOR:
            // This case could also be trapped out before as a serial environment
               Return_Array_Set.Update_Left_Ghost_Boundary_Width [Axis] = FALSE;  
               Return_Array_Set.Update_Right_Ghost_Boundary_Width [Axis] = FALSE;  
            // Return_Array_Set.Left_Number_Of_Points_Truncated[Axis] = 0;   
            // Return_Array_Set.Right_Number_Of_Points_Truncated[Axis] = 0;   
               break;

          case LEFT_PROCESSOR  :
               VSG_Model = Array_Domain_Type::Overlap_Update_Case_Left_Processor 
                            ( Return_Array_Set,
                              Lhs_Descriptor, Lhs_Serial_Descriptor,
                              Rhs_Descriptor, Rhs_Serial_Descriptor,
                              Lhs_Partition_Index, Rhs_Partition_Index, Axis );
               break;

          case MIDDLE_PROCESSOR:
               VSG_Model = Array_Domain_Type::Overlap_Update_Case_Middle_Processor 
                            ( Return_Array_Set,
                              Lhs_Descriptor, Lhs_Serial_Descriptor,
                              Rhs_Descriptor, Rhs_Serial_Descriptor,
                              Lhs_Partition_Index, Rhs_Partition_Index, Axis );
               break;

          case RIGHT_PROCESSOR :
               VSG_Model = Array_Domain_Type::Overlap_Update_Case_Right_Processor 
                            ( Return_Array_Set,
                              Lhs_Descriptor, Lhs_Serial_Descriptor,
                              Rhs_Descriptor, Rhs_Serial_Descriptor,
                              Lhs_Partition_Index, Rhs_Partition_Index, Axis );
               break;

          case NOT_PRESENT_ON_PROCESSOR :
               VSG_Model = UNDEFINED_VSG_MODEL;
            // ... (1/16/97, kdb) test this ...
            // Return_Array_Set.Update_Left_Ghost_Boundary_Width [Axis] = FALSE;  
            // Return_Array_Set.Update_Right_Ghost_Boundary_Width [Axis] = FALSE;  
            // Return_Array_Set.Left_Number_Of_Points_Truncated[Axis] = 0;   
            // Return_Array_Set.Right_Number_Of_Points_Truncated[Axis] = 0;   
               break;

          default: 
               printf ("ERROR default reached in switch of Array_Domain_Type::Compute_Overlap_Update! \n");
               APP_ABORT();
               break;
        }

     return VSG_Model;
   }

//================================================================

int
Array_Domain_Type::Check_For_Ghost_Region_Only (
   int Processor_Position,
   Array_Conformability_Info_Type & Return_Array_Set,
   const Array_Domain_Type & Descriptor,
   const SerialArray_Domain_Type & Serial_Descriptor,
   Internal_Index* Partition_Index,
   int Axis )
   {
  // Error checking -- Test the processor space
     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

     int VSG_Model = UNDEFINED_VSG_MODEL;
     switch (Processor_Position)
        {
          case SINGLE_PROCESSOR:
            // This case could also be trapped out before as a serial environment
               Return_Array_Set.Update_Left_Ghost_Boundary_Width [Axis] = FALSE;  
               Return_Array_Set.Update_Right_Ghost_Boundary_Width [Axis] = FALSE;  
            // Return_Array_Set.Left_Number_Of_Points_Truncated[Axis] = 0;   
            // Return_Array_Set.Right_Number_Of_Points_Truncated[Axis] = 0;   
               break;

          case LEFT_PROCESSOR  :
               VSG_Model = Array_Domain_Type::Check_Ghost_Case_Left_Processor 
                            ( Return_Array_Set, Descriptor, Serial_Descriptor, Partition_Index, Axis );
               break;

          case MIDDLE_PROCESSOR:
               VSG_Model = Array_Domain_Type::Check_Ghost_Case_Middle_Processor 
                            ( Return_Array_Set, Descriptor, Serial_Descriptor, Partition_Index, Axis );
               break;

          case RIGHT_PROCESSOR :
               VSG_Model = Array_Domain_Type::Check_Ghost_Case_Right_Processor 
                            ( Return_Array_Set, Descriptor, Serial_Descriptor, Partition_Index, Axis );
               break;

          case NOT_PRESENT_ON_PROCESSOR :
               VSG_Model = UNDEFINED_VSG_MODEL;
               break;

          default: 
               printf ("ERROR default reached in switch of Array_Domain_Type::Check_For_Ghost_Region_Only! \n");
               APP_ABORT();
               break;
        }
     return VSG_Model;
   }

// **********************************************************************
// We require at least width one ghost boundaries to support the overlap 
// update.
// **********************************************************************
void
Array_Domain_Type::Test_For_Sufficient_Ghost_Boundary_Width (
   const Array_Domain_Type & Lhs_Parallel_Descriptor, 
   const SerialArray_Domain_Type & Lhs_Serial_Descriptor, 
   const Array_Domain_Type & Rhs_Parallel_Descriptor,
   const SerialArray_Domain_Type & Rhs_Serial_Descriptor, int Axis )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of Array_Domain_Type::Test_For_Sufficient_Ghost_Boundary_Width \n");
#endif

     if (APP_DEBUG > 0)
          printf ("Test_For_Sufficient_Ghost_Boundary_Width disabled (NO ERROR CHECKING DONE!) \n");
   }

// **********************************************************************
// Message passing only occures along an axis that is partitioned (so we 
// need to test each axis)
// **********************************************************************
bool
Array_Domain_Type::isPartitioned (
   const Array_Domain_Type & Parallel_Descriptor,
   const SerialArray_Domain_Type & Serial_Descriptor, int Axis )
   {
  // Figure out if a P++ array is partitioned along a specific axis
  // This correctly handles the case of the serial array being a 
  // Null Array too. But not the case where a view is entirely on a 
  // single processor.

     int Global_Size_Array [MAX_ARRAY_DIMENSION];
     Parallel_Descriptor.getRawDataSize ( Global_Size_Array );

     bool Return_Value = Global_Size_Array[Axis] != 
                             ( Serial_Descriptor.getLength(Axis) - (2 * Parallel_Descriptor.InternalGhostCellWidth[Axis]) );

     return Return_Value;
   }

// **********************************************************************
// Display function used for debugging only
// **********************************************************************
void
Display_Index_Array ( Internal_Index* Lhs , Internal_Index* Rhs )
   {
     int i = 0;
     char buffer[256];
     for (i = 0; i < MAX_ARRAY_DIMENSION; i++)
        {
          sprintf (buffer,"LHS Axis %d",i);
          Lhs[i].display(buffer);
        }
     for (i = 0; i < MAX_ARRAY_DIMENSION; i++)
        {
          sprintf (buffer,"RHS Axis %d",i);
          Rhs[i].display(buffer);
        }
   }

// **********************************************************************
// This function is used to find out how the current processor (the one 
// where this code executes) is positioned relative to the array which 
// is partitioned on a collection of processors.
// **********************************************************************
bool
Is_A_Small_Problem ( const SerialArray_Domain_Type & X , int Axis )
   {
  // This function might get more complex later so we break it out now
  // in the development of the message passing interpretation.

     bool Return_Value = FALSE;

     int Lower_Bound = 0;
     int Dimension   = X.getLength(Axis);

     APP_ASSERT(Dimension >= 0);

     if ( (Dimension <= Lower_Bound) && (X.Is_A_Null_Array == FALSE) )
          Return_Value = TRUE;

     return Return_Value;
   }

// **********************************************************************
// Here we see what message passing will be done and we record it into the
// Array_Set which is passed along though the evaluation of the P++ 
// expression.
// **********************************************************************
int
Array_Domain_Type::Interpret_Message_Passing (
   Array_Conformability_Info_Type & Array_Set, 
   const Array_Domain_Type & Lhs_Parallel_Descriptor,
   const SerialArray_Domain_Type & Lhs_Serial_Descriptor,
   const Array_Domain_Type & Rhs_Parallel_Descriptor,
   const SerialArray_Domain_Type & Rhs_Serial_Descriptor,
   Internal_Index* Lhs_Partition_Index, 
   Internal_Index* Rhs_Partition_Index )
   {

  // Must intialize to the default value to avoid later error
  // i.e. even no message passing is a trivial case of an 
  // OVERLAPPING_BOUNDARY_VSG_MODEL

     int Final_VSG_Model = OVERLAPPING_BOUNDARY_VSG_MODEL;

  // A VSG model is computed for each axis (based on the indexing)
  // then a single model is chosen for execution based on the worst case model.
  // For example - if one axis requires a the more expensive VSG model then
  // the whole grid is treated using the VSG execution model.
     int VSG_Model[MAX_ARRAY_DIMENSION];

  // Initialize the array which reports which model to use!
     int temp;
     for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
          VSG_Model[temp] = UNDEFINED_VSG_MODEL;

  // Initialize Index for modification which will form a 
  // conformable operation between the A++ serial array objects
  // Note that the local mask includes the ghost boundaries!
     for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
        {
          Lhs_Partition_Index [temp] = Lhs_Parallel_Descriptor.Local_Mask_Index[temp]; 
          Rhs_Partition_Index [temp] = Rhs_Parallel_Descriptor.Local_Mask_Index[temp]; 
        }

  // ... NOTE: Might not need this anymore ...

  // Here we change the Lhs_Partition_Index and Rhs_Partition_Index to 
  // reflect the history of what used to be carried in the Local_Mask 
  // (but because we can't modify the parallel descriptors directly we 
  // can't use the same simple technique the example which demonstrates 
  // the side effect problem is A = A +B(I+1) --- 
  // in this example A is would be modified and then the operator= would 
  // screw up because A is not properly represented in it's descriptor's 
  // view of the Local_Mask).
  // So we have to construct the equivalet Local_Mask seperatly before 
  // calling the Compute_Overlap_Update function. To do this we use the 
  // local view of the array object and the info stored from the previous
  // binary operation(s).

     for (temp=0; temp < MAX_ARRAY_DIMENSION; temp++)
        {
       // Now modify the Lhs_Partition_Index and Rhs_Partition_Index based 
       // on the history stored in the Array_Set from the last operation 
       // (if there was no previous operation then the Array_Set
       // is present but initialized to zero (default) values (meaning no history 
       // has been carried over)).
 
          if (Lhs_Parallel_Descriptor.Array_Conformability_Info != NULL)
             {
               int Lhs_Base_Adjustment  = Lhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Left_Ghost_Boundary_Width[temp];
               int Lhs_Bound_Adjustment = Lhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Right_Ghost_Boundary_Width[temp];

            // Lhs_Partition_Index [temp].display("Lhs_Partition_Index BEFORE ajustment in Array_Domain_Type::Interpret_Message_Passing");

               Lhs_Partition_Index [temp].adjustBase (Lhs_Base_Adjustment);
               Lhs_Partition_Index [temp].adjustBound (Lhs_Bound_Adjustment);

            // Lhs_Partition_Index [temp].display("Lhs_Partition_Index AFTER ajustment in Array_Domain_Type::Interpret_Message_Passing");

            // ... (10/8/96,kdb) fix up update of ghost cells because
	    // the update flag may not be set correctly here because info
	    // is already lost unless this is done ...
               if (Lhs_Parallel_Descriptor.Array_Conformability_Info->Update_Left_Ghost_Boundary_Width [temp]) 
                    Array_Set.Update_Left_Ghost_Boundary_Width [temp] = TRUE;
               if (Lhs_Parallel_Descriptor.Array_Conformability_Info->Update_Right_Ghost_Boundary_Width [temp])
                    Array_Set.Update_Right_Ghost_Boundary_Width [temp] = TRUE;

            // ... (10/10/96, kdb) need to save Truncate vals too but only
            // for Lhs because Array_Set only reflects Lhs changes ...
               Array_Set.Truncate_Left_Ghost_Boundary_Width [temp] =
                    MAX(Lhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Left_Ghost_Boundary_Width [temp],
                        Array_Set.Truncate_Left_Ghost_Boundary_Width [temp]);
               Array_Set.Truncate_Right_Ghost_Boundary_Width [temp] =
                    MIN(Lhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Right_Ghost_Boundary_Width [temp],
                        Array_Set.Truncate_Right_Ghost_Boundary_Width [temp]);

               Array_Set.Left_Number_Of_Points_Truncated[temp] = 
                    Lhs_Parallel_Descriptor.Array_Conformability_Info->Left_Number_Of_Points_Truncated [temp];
               Array_Set.Right_Number_Of_Points_Truncated[temp] = 
                    Lhs_Parallel_Descriptor.Array_Conformability_Info->Right_Number_Of_Points_Truncated [temp];
             }
            else
             {
            // printf ("##### Lhs_Parallel_Descriptor.Array_Conformability_Info == NULL \n");
             }
	  

          if (Rhs_Parallel_Descriptor.Array_Conformability_Info != NULL)
             {
               int Rhs_Base_Adjustment  = Rhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Left_Ghost_Boundary_Width[temp];
               int Rhs_Bound_Adjustment = Rhs_Parallel_Descriptor.Array_Conformability_Info->Truncate_Right_Ghost_Boundary_Width[temp];

            // Rhs_Partition_Index [temp].display("Rhs_Partition_Index BEFORE ajustment in Array_Domain_Type::Interpret_Message_Passing");
               Rhs_Partition_Index [temp].adjustBase (Rhs_Base_Adjustment);
               Rhs_Partition_Index [temp].adjustBound (Rhs_Bound_Adjustment);

            // Rhs_Partition_Index [temp].display("Rhs_Partition_Index AFTER ajustment in Array_Domain_Type::Interpret_Message_Passing");

            // ... (10/8/96,kdb) fix up update of ghost cells because
	    // the update flag may not be set correctly here because info
	    // is already lost unless this is done ...
               if (Rhs_Parallel_Descriptor.Array_Conformability_Info->Update_Left_Ghost_Boundary_Width [temp])
                    Array_Set.Update_Left_Ghost_Boundary_Width [temp] = TRUE;  
               if (Rhs_Parallel_Descriptor.Array_Conformability_Info->Update_Right_Ghost_Boundary_Width [temp])
                    Array_Set.Update_Right_Ghost_Boundary_Width [temp] = TRUE;

            // ... change the sight because this is the Rhs array ...
               Array_Set.Left_Number_Of_Points_Truncated[temp] =
                    -Rhs_Parallel_Descriptor.Array_Conformability_Info->Left_Number_Of_Points_Truncated [temp];
               Array_Set.Right_Number_Of_Points_Truncated[temp] =
                    -Rhs_Parallel_Descriptor.Array_Conformability_Info->Right_Number_Of_Points_Truncated [temp];
             }
            else
             {
            // printf ("##### Rhs_Parallel_Descriptor.Array_Conformability_Info == NULL \n");
             }
        }

     APP_ASSERT ( !( Lhs_Parallel_Descriptor.Is_A_Null_Array || Rhs_Parallel_Descriptor.Is_A_Null_Array ) );

     if ( Lhs_Parallel_Descriptor.Is_A_Null_Array || Rhs_Parallel_Descriptor.Is_A_Null_Array )
        {
       // Then return a NULL array and skip the overhead of figuring out 
       // that we will eventually do nothing! So this should be a simple 
       // thing to handle it is just that for now we want to know that
       // in our test problems we don't have this cases cropping up 
       // unexpectedly.

          printf ("ERROR -- Case of NULL operands: ");
          printf ("should not have been possible to reach this statement! \n");
          APP_ABORT();
        }
       else
        {
          int Axis;
          for (Axis=0; Axis < MAX_ARRAY_DIMENSION; Axis++)
             {
            // ... Check global values to see if any processor will need to do a
            // full vsg update.  If so make this processor do a full vsg also. ...
               VSG_Model[Axis] = Array_Domain_Type::Check_Global_Values ( 
                                      Array_Set, Lhs_Parallel_Descriptor, Rhs_Parallel_Descriptor, Axis);
               if (VSG_Model[Axis] == FULL_VSG_MODEL)
                    Final_VSG_Model = FULL_VSG_MODEL;
             }

          for (Axis=0; Axis < MAX_ARRAY_DIMENSION; Axis++)
             {
            // if (VSG_Model[Axis] != FULL_VSG_MODEL)
               if (Final_VSG_Model != FULL_VSG_MODEL)
	          {
                 // We only want to worry about partitioned dimensions (i.e. 
	         // only distributed Axis)
                 // We have to account for the ghost boundaries in the 
	         // SerialArray when we check if an array is distributed along 
	         // each axis.

                    if (!Array_Domain_Type::isPartitioned (Lhs_Parallel_Descriptor,Lhs_Serial_Descriptor,Axis) &&
                        !Array_Domain_Type::isPartitioned(Rhs_Parallel_Descriptor,Rhs_Serial_Descriptor,Axis) )
                       {
                         VSG_Model[Axis] = NON_DISTRIBUTED_AXIS;
                         Array_Set.Update_Left_Ghost_Boundary_Width  [Axis] = 0;
                         Array_Set.Update_Right_Ghost_Boundary_Width [Axis] = 0;
                       }
                      else
                       {
                         int Lhs_Processor_Position = Lhs_Parallel_Descriptor.Get_Processor_Position ( Axis );
                         int Rhs_Processor_Position = Rhs_Parallel_Descriptor.Get_Processor_Position ( Axis );

                         if (Lhs_Processor_Position != Rhs_Processor_Position)
                            {
                           // The small problem case is a hard one to solve so we 
	                   // seperate it out. I hope that the use of the Maryland 
	                   // PARTI library will simplify operations on seemingly 
                           // trivial small problems but we will see.

                              VSG_Model[Axis] = FULL_VSG_MODEL;

                              if (Lhs_Processor_Position == NOT_PRESENT_ON_PROCESSOR)
                                 {
                                   VSG_Model[Axis] = Array_Domain_Type::Check_For_Ghost_Region_Only
                                                        ( Rhs_Processor_Position, Array_Set,
                                                          Rhs_Parallel_Descriptor, Rhs_Serial_Descriptor,
                                                          Rhs_Partition_Index, Axis );
                                 }
                                else
                                 {
                                   if (Rhs_Processor_Position == NOT_PRESENT_ON_PROCESSOR)
                                      {
                                        VSG_Model[Axis] = Array_Domain_Type::Check_For_Ghost_Region_Only
                                                             ( Lhs_Processor_Position, Array_Set,
                                                               Lhs_Parallel_Descriptor, Lhs_Serial_Descriptor,
                                                               Lhs_Partition_Index, Axis );
	                              }
                                     else
                                      {
		                     // ... positions are different but neither is null ...
                                        VSG_Model[Axis] = FULL_VSG_MODEL;
	                              }
                                 }
                            }
                           else
                            {
                           // ... check to make sure the Partition_Index isn't null
                           // which causes problems in Compute_Overlap_Update.  If
                           // so this cases is similar to above.  ...
                              if ((Lhs_Partition_Index[Axis].Index_Mode == Null_Index) && (Rhs_Partition_Index[Axis].Index_Mode != Null_Index))
                                 {
                                   VSG_Model[Axis] = Array_Domain_Type::Check_For_Ghost_Region_Only
                                                        ( Rhs_Processor_Position, Array_Set,
                                                          Rhs_Parallel_Descriptor, Rhs_Serial_Descriptor,
                                                          Rhs_Partition_Index, Axis );
	                         }
                                else
                                 {
                                   if ((Rhs_Partition_Index[Axis].Index_Mode == Null_Index) && (Lhs_Partition_Index[Axis].Index_Mode != Null_Index))
                                      {
                                        VSG_Model[Axis] = Array_Domain_Type::Check_For_Ghost_Region_Only
                                                             ( Lhs_Processor_Position, Array_Set,
                                                               Lhs_Parallel_Descriptor, Lhs_Serial_Descriptor,
                                                               Lhs_Partition_Index, Axis );
	                              }
                                // ... handeled properly through Compute_Overlap_Update ...
#if 0
                                     else
                                        if ((Rhs_Partition_Index[Axis].Index_Mode == Null_Index) && (Lhs_Partition_Index[Axis].Index_Mode == Null_Index))
                                           {
                                             VSG_Model[Axis] = FULL_VSG_MODEL;
                                           }
#endif
                                     else
                                      {
                                     // For now we restrict P++ to this more simple case (I'm 
	                             // not certain if the more general case is a simple or hard problem).
                                        int Processor_Position = Lhs_Processor_Position;

                                        VSG_Model[Axis] = Array_Domain_Type::Compute_Overlap_Update 
                                                             ( Processor_Position, Array_Set,
                                                               Lhs_Parallel_Descriptor, Lhs_Serial_Descriptor,
                                                               Rhs_Parallel_Descriptor, Rhs_Serial_Descriptor,
                                                               Lhs_Partition_Index, Rhs_Partition_Index, Axis);
                                      }
                                 }
                            }
                       }
                  }
                 else
	          {
                    APP_ASSERT(Final_VSG_Model == FULL_VSG_MODEL);

                 // Initialize this properly
                    VSG_Model[Axis] = FULL_VSG_MODEL;
                  }
            // end of for loop
             }

       // If any axis requires the more general model then we have to use the more general model
          for (Axis=0; Axis < MAX_ARRAY_DIMENSION; Axis++)
             {
               if (VSG_Model[Axis] == FULL_VSG_MODEL)
                  {
                    Final_VSG_Model = FULL_VSG_MODEL;
                 // ... (1/16/96,kdb) don't need to update ghost boundaries because 
	         // this will happen anyways ...
                    Array_Set.Update_Left_Ghost_Boundary_Width  [Axis] = 0;
                    Array_Set.Update_Right_Ghost_Boundary_Width [Axis] = 0;
	          }
             }

       // If we require a FULL_VSG_MODEL update then we record this in 
       // the Return_Array_Set structure and assume that further 
       // operations can only use the interior of the partitioned array 
       // (i.e. ghost boundaries are assumed to be invalid)

          Array_Set.Full_VSG_Update_Required = (Final_VSG_Model == FULL_VSG_MODEL) ? TRUE : FALSE;

          if (FORCE_VSG_UPDATE)
             {
            // Force the value of the return value to be consistant with that in the Array_Set
               Final_VSG_Model = FULL_VSG_MODEL;
               Array_Set.Full_VSG_Update_Required = TRUE;
             }
	  
       // end of FALSE block of conditional test (Lhs and Rhs arrays not null)
        }

     return Final_VSG_Model;
   }


#define DOUBLEARRAY
#if !defined(PPP)
#error (conform_enforce.C) This is only code for P++
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type*
doubleArray::Parallel_Conformability_Enforcement (const doubleArray & X_ParallelArray,  doubleSerialArray* & X_SerialArray )
   {
  // This is a trivial case for the Parallel_Conformability_Enforcement since there is only a single array is use
  // and thus nothing to be forced to be aligned (and thus make a serial conformable operation)

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
          printf ("Inside of #1 doubleArray::Parallel_Conformability_Enforcement (doubleArray.Array_ID = %d, doubleSerialArray* = %p) \n",
              X_ParallelArray.Array_ID(),X_SerialArray);
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #1 doubleArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(X_SerialArray == NULL);
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_ParallelArray.displayReferenceCounts("X_ParallelArray in #1 doubleArray::Parallel_Conformability_Enforcement (doubleArray,doubleSerialArray*)");
        }
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ( *(X_ParallelArray.Array_Descriptor) );
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
     APP_ASSERT( Return_Array_Set != NULL );


  // X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray;
  // We must increment the reference count so that when the operator 
  // deletes the X_SerialArray pointer the SerialArray of the 
  // X_ParallelArray will not be deleted.
  // X_SerialArray->incrementReferenceCount();

#if 0
  // I think that if the input is a temporary then this approach will be a problem
     APP_ASSERT(X_ParallelArray.Array_Descriptor.SerialArray->isTemporary() == FALSE);
     X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray->getLocalArrayWithGhostBoundariesPointer();
#else
  // This set of Index objects will define the Lhs view we will use to do the serial operation
     Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
     Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

     int nd;
     for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
        {
          Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);
          int tempBase   = X_ParallelArray.Array_Descriptor.SerialArray->getBase(nd);
          int tempBound  = X_ParallelArray.Array_Descriptor.SerialArray->getBound(nd);
          int tempStride = X_ParallelArray.Array_Descriptor.SerialArray->getStride(nd);
          *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

          APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
       // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
        }

     X_SerialArray = doubleSerialArray::Build_Pointer_To_View_Of_Array 
                          (*X_ParallelArray.Array_Descriptor.SerialArray, Matching_Index_Pointer_List );
#endif

     APP_ASSERT( X_SerialArray != NULL );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray->displayReferenceCounts("X_SerialArray in #1 doubleArray::Parallel_Conformability_Enforcement (doubleArray,doubleSerialArray*)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Leaving #1 doubleArray::Parallel_Conformability_Enforcement (Return_Array_Set = %p) \n",Return_Array_Set); 
#endif

     APP_ASSERT (X_SerialArray->getReferenceCount() >= getReferenceCountBase());
     return Return_Array_Set;
   }

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
doubleArray::Parallel_Conformability_Enforcement (
       const doubleArray & Lhs_ParallelArray,  
       doubleSerialArray* & Lhs_SerialArray,
       const doubleArray & Rhs_ParallelArray,  
       doubleSerialArray* & Rhs_SerialArray )
   {
  // This function returns a serial array for the Lhs and Rhs for which a conformable 
  // serial array operation can be defined.  For the Lhs we assume an owner compute model
  // (so no communication is required).  But for the Rhs we do what ever communication is 
  // required (unless the overlap update is used in which case we reduce the size of the Lhs
  // serial array to make the operation conformable).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #2 doubleArray::Parallel_Conformability_Enforcement (doubleArray.Array_ID = %d, doubleSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #2 doubleArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #2 doubleArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

  // Two operand message passing interpretation support
     int num_arrays = 1;
     const doubleArray** List_Of_ParallelArrays = new const doubleArray* [num_arrays];
     APP_ASSERT (List_Of_ParallelArrays != NULL);
     doubleSerialArray** List_Of_SerialArrays   = new doubleSerialArray* [num_arrays];
     APP_ASSERT (List_Of_SerialArrays != NULL);

     intArray* Mask_ParallelArray          = NULL;
     intSerialArray* Mask_SerialArray      = NULL;
     doubleSerialArray* Matching_SerialArray   = NULL;

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_SerialArrays[i] = NULL;
        }

     const doubleArray* Matching_ParallelArray = &Lhs_ParallelArray;
     List_Of_ParallelArrays[0]             = &Rhs_ParallelArray;

  // Call macro which isolates details acorss multiple functions
     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) ) // *wdh* || !automaticCommunication )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const doubleArray* Other_doubleParallelArray = NULL;
               doubleSerialArray* Other_doubleSerialArray   = NULL;

               Other_doubleParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         doubleSerialArray *Destination_SerialArray = NULL;
                         doubleSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_doubleSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build doubleSerialArray Temp \n");

                              doubleSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling doubleSerialArray::Build_Pointer_To_View_Of_Array Other_doubleSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_doubleSerialArray = 
                                   doubleSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_doubleSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_doubleSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("VSG UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_doubleSerialArray->getBase(0),
                                        Other_doubleSerialArray->getBound(0),
                                        Other_doubleSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_doubleSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_doubleSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_doubleParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_doubleSerialArray \n");

                                  Other_doubleSerialArray = new doubleSerialArray ();
                                  APP_ASSERT (Other_doubleSerialArray != NULL);

                                  Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                       if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_doubleParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                               if (Other_doubleParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_doubleParallelArray->isView() == TRUE);
                                  }

                               Other_doubleSerialArray =
                                    doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_doubleParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_doubleSerialArray != NULL);

                               APP_ASSERT (Other_doubleSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_doubleParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_doubleSerialArray->displayReferenceCounts("Other_doubleSerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("OVERLAP UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_doubleSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = List_Of_SerialArrays[0];
     Lhs_SerialArray = Matching_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 doubleArray::Parallel_Conformability_Enforcement delete the lists of parallel and serial arrays \n");
#endif

#if 1
  // Is this really good enough (since it fails to modify the reference counts!)
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

  // Should these assertions include "-1"?
     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
// Used for the replace operator (scalar and array)
Array_Conformability_Info_Type*
doubleArray::Parallel_Conformability_Enforcement
      (const doubleArray  & Lhs_ParallelArray,
       doubleSerialArray*  & Lhs_SerialArray,
       const intArray & Rhs_ParallelArray,
       intSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #3 doubleArray::Parallel_Conformability_Enforcement (doubleArray.Array_ID = %d, doubleSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #3 doubleArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #3 doubleArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif
//================ END DEBUG SECTION ============================
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Two operand message passing interpretation support

     int num_arrays = 0;
     const doubleArray** List_Of_ParallelArrays = NULL;
     doubleSerialArray** List_Of_SerialArrays   = NULL;

     const intArray* Mask_ParallelArray = &Rhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     const doubleArray* Matching_ParallelArray = &Lhs_ParallelArray;
     doubleSerialArray* Matching_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const doubleArray* Other_doubleParallelArray = NULL;
               doubleSerialArray* Other_doubleSerialArray   = NULL;

               Other_doubleParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         doubleSerialArray *Destination_SerialArray = NULL;
                         doubleSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_doubleSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build doubleSerialArray Temp \n");

                              doubleSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling doubleSerialArray::Build_Pointer_To_View_Of_Array Other_doubleSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_doubleSerialArray = 
                                   doubleSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_doubleSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_doubleSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("VSG UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_doubleSerialArray->getBase(0),
                                        Other_doubleSerialArray->getBound(0),
                                        Other_doubleSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_doubleSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_doubleSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_doubleParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_doubleSerialArray \n");

                                  Other_doubleSerialArray = new doubleSerialArray ();
                                  APP_ASSERT (Other_doubleSerialArray != NULL);

                                  Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_doubleParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                               if (Other_doubleParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_doubleParallelArray->isView() == TRUE);
                                  }

                               Other_doubleSerialArray =
                                    doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_doubleParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_doubleSerialArray != NULL);

                               APP_ASSERT (Other_doubleSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_doubleParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_doubleSerialArray->displayReferenceCounts("Other_doubleSerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("OVERLAP UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_doubleSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  if( automaticCommunication ) Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = Mask_SerialArray;
     Lhs_SerialArray = Matching_SerialArray;

     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase());
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase());

     return Return_Array_Set;
   }
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
doubleArray::Parallel_Conformability_Enforcement (
       const doubleArray & This_ParallelArray, doubleSerialArray* & This_SerialArray,
       const intArray & Lhs_ParallelArray, intSerialArray* & Lhs_SerialArray,
       const doubleArray & Rhs_ParallelArray,  doubleSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #4 doubleArray::Parallel_Conformability_Enforcement (doubleArray.Array_ID = %d, doubleSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     This_ParallelArray.Test_Consistency ("Test This_ParallelArray in #4 doubleArray::Parallel_Conformability_Enforcement");
     Lhs_ParallelArray.Test_Consistency  ("Test Lhs_ParallelArray in #4 doubleArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency  ("Test Rhs_ParallelArray in #4 doubleArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(This_SerialArray == NULL);
     APP_ASSERT(Lhs_SerialArray  == NULL);
     APP_ASSERT(Rhs_SerialArray  == NULL);
#endif
//================ END DEBUG SECTION ============================

  // printf ("WARNING: Inside of #4 doubleArray::Parallel_Conformability_Enforcement -- Case of 3 array operations (typically where statements) not properly handled for general distributions \n");

  // ... Case where This_ParallelArray is null doesn't work yet because
  // Return_Array_Set will be associated with the wrong array if
  // another array than This_ParallelArray is set to be the 
  // Matching_ParallelArray ...

   /*
   if (This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
   {
     printf("Sorry, case where This_ParallelArray is Null not");
     printf(" implemented yet\n");
     APP_ABORT();
   }
   */

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info 
  //      (*(This_ParallelArray.Array_Descriptor), *(Lhs_ParallelArray.Array_Descriptor), *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     int num_arrays = 1;
     const doubleArray** List_Of_ParallelArrays = new const doubleArray* [num_arrays];  
     APP_ASSERT(List_Of_ParallelArrays != NULL);

     doubleSerialArray** List_Of_SerialArrays   = new doubleSerialArray* [num_arrays];
     APP_ASSERT(List_Of_SerialArrays   != NULL);

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_ParallelArrays[i] = NULL;
          List_Of_SerialArrays  [i] = NULL;
        }

     List_Of_ParallelArrays[0] = &Rhs_ParallelArray;

 // const doubleArray* Matching_ParallelArray = &This_ParallelArray;
    const doubleArray* Matching_ParallelArray;
    if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
       {
         Matching_ParallelArray    = &This_ParallelArray;
         List_Of_ParallelArrays[0] = &Rhs_ParallelArray;
       }
      else
       {
         Matching_ParallelArray    = &Rhs_ParallelArray;
         List_Of_ParallelArrays[0] = &This_ParallelArray;
       }

  // Matching_ParallelArray = &This_ParallelArray;
     doubleSerialArray* Matching_SerialArray = NULL;

     const intArray* Mask_ParallelArray = &Lhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (after doubleSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const doubleArray* Other_doubleParallelArray = NULL;
               doubleSerialArray* Other_doubleSerialArray   = NULL;

               Other_doubleParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         doubleSerialArray *Destination_SerialArray = NULL;
                         doubleSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_doubleSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build doubleSerialArray Temp \n");

                              doubleSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling doubleSerialArray::Build_Pointer_To_View_Of_Array Other_doubleSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_doubleSerialArray = 
                                   doubleSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_doubleSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_doubleSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("VSG UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_doubleSerialArray->getBase(0),
                                        Other_doubleSerialArray->getBound(0),
                                        Other_doubleSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after doubleSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_doubleSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_doubleSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_doubleParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_doubleParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_doubleSerialArray \n");

                                  Other_doubleSerialArray = new doubleSerialArray ();
                                  APP_ASSERT (Other_doubleSerialArray != NULL);

                                  Source_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_doubleParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_doubleParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_doubleParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                               if (Other_doubleParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_doubleParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_doubleParallelArray->isView() == TRUE);
                                  }

                               Other_doubleSerialArray =
                                    doubleSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_doubleParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_doubleSerialArray != NULL);

                               APP_ASSERT (Other_doubleSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_doubleParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_doubleSerialArray->displayReferenceCounts("Other_doubleSerialArray in PCE (base of Overlap Update after doubleSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_doubleSerialArray->view("OVERLAP UPDATE: Other_doubleSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_doubleSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_doubleSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
        {
          Rhs_SerialArray = List_Of_SerialArrays[0];
          This_SerialArray = Matching_SerialArray;
        }
       else
        {
          This_SerialArray = List_Of_SerialArrays[0];
          Rhs_SerialArray = Matching_SerialArray;
        }

     Lhs_SerialArray = Mask_SerialArray;

#if 1
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

     return Return_Array_Set;
   }


// *********************************************************************
//                PARALLEL INDIRECT CONFORMABILITY ENFORCEMENT
// *********************************************************************
// ... get rid of const on Lhs until a better soln is found ...

Array_Conformability_Info_Type*
doubleArray::Parallel_Indirect_Conformability_Enforcement (
       const doubleArray & Lhs,  
       doubleSerialArray* & Lhs_SerialArray,
       const doubleArray & Rhs,  
       doubleSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
doubleArray::Parallel_Indirect_Conformability_Enforcement (
       const doubleArray & Lhs,  
       doubleSerialArray* & Lhs_SerialArray,
       const intArray & Rhs,  
       intSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }
#endif

// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
doubleArray::Parallel_Indirect_Conformability_Enforcement (
      const doubleArray & Lhs,
       doubleSerialArray* & Lhs_SerialArray)
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // This section handles the details of parallel support for indirect addressing
// I think that Brian M. has to separate out some functionality in the lower 
// level support so this can be supported.

// New code for indirect addressing support
   printf ("Need to have the indirect addressing support broken out to compute A(I) separately. \n");
   APP_ABORT();

;

     return Return_Array_Set;
   }

#undef DOUBLEARRAY

#define FLOATARRAY
#if !defined(PPP)
#error (conform_enforce.C) This is only code for P++
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type*
floatArray::Parallel_Conformability_Enforcement (const floatArray & X_ParallelArray,  floatSerialArray* & X_SerialArray )
   {
  // This is a trivial case for the Parallel_Conformability_Enforcement since there is only a single array is use
  // and thus nothing to be forced to be aligned (and thus make a serial conformable operation)

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
          printf ("Inside of #1 floatArray::Parallel_Conformability_Enforcement (floatArray.Array_ID = %d, floatSerialArray* = %p) \n",
              X_ParallelArray.Array_ID(),X_SerialArray);
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #1 floatArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(X_SerialArray == NULL);
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_ParallelArray.displayReferenceCounts("X_ParallelArray in #1 floatArray::Parallel_Conformability_Enforcement (floatArray,floatSerialArray*)");
        }
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ( *(X_ParallelArray.Array_Descriptor) );
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
     APP_ASSERT( Return_Array_Set != NULL );


  // X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray;
  // We must increment the reference count so that when the operator 
  // deletes the X_SerialArray pointer the SerialArray of the 
  // X_ParallelArray will not be deleted.
  // X_SerialArray->incrementReferenceCount();

#if 0
  // I think that if the input is a temporary then this approach will be a problem
     APP_ASSERT(X_ParallelArray.Array_Descriptor.SerialArray->isTemporary() == FALSE);
     X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray->getLocalArrayWithGhostBoundariesPointer();
#else
  // This set of Index objects will define the Lhs view we will use to do the serial operation
     Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
     Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

     int nd;
     for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
        {
          Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);
          int tempBase   = X_ParallelArray.Array_Descriptor.SerialArray->getBase(nd);
          int tempBound  = X_ParallelArray.Array_Descriptor.SerialArray->getBound(nd);
          int tempStride = X_ParallelArray.Array_Descriptor.SerialArray->getStride(nd);
          *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

          APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
       // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
        }

     X_SerialArray = floatSerialArray::Build_Pointer_To_View_Of_Array 
                          (*X_ParallelArray.Array_Descriptor.SerialArray, Matching_Index_Pointer_List );
#endif

     APP_ASSERT( X_SerialArray != NULL );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray->displayReferenceCounts("X_SerialArray in #1 floatArray::Parallel_Conformability_Enforcement (floatArray,floatSerialArray*)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Leaving #1 floatArray::Parallel_Conformability_Enforcement (Return_Array_Set = %p) \n",Return_Array_Set); 
#endif

     APP_ASSERT (X_SerialArray->getReferenceCount() >= getReferenceCountBase());
     return Return_Array_Set;
   }

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
floatArray::Parallel_Conformability_Enforcement (
       const floatArray & Lhs_ParallelArray,  
       floatSerialArray* & Lhs_SerialArray,
       const floatArray & Rhs_ParallelArray,  
       floatSerialArray* & Rhs_SerialArray )
   {
  // This function returns a serial array for the Lhs and Rhs for which a conformable 
  // serial array operation can be defined.  For the Lhs we assume an owner compute model
  // (so no communication is required).  But for the Rhs we do what ever communication is 
  // required (unless the overlap update is used in which case we reduce the size of the Lhs
  // serial array to make the operation conformable).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #2 floatArray::Parallel_Conformability_Enforcement (floatArray.Array_ID = %d, floatSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #2 floatArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #2 floatArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

  // Two operand message passing interpretation support
     int num_arrays = 1;
     const floatArray** List_Of_ParallelArrays = new const floatArray* [num_arrays];
     APP_ASSERT (List_Of_ParallelArrays != NULL);
     floatSerialArray** List_Of_SerialArrays   = new floatSerialArray* [num_arrays];
     APP_ASSERT (List_Of_SerialArrays != NULL);

     intArray* Mask_ParallelArray          = NULL;
     intSerialArray* Mask_SerialArray      = NULL;
     floatSerialArray* Matching_SerialArray   = NULL;

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_SerialArrays[i] = NULL;
        }

     const floatArray* Matching_ParallelArray = &Lhs_ParallelArray;
     List_Of_ParallelArrays[0]             = &Rhs_ParallelArray;

  // Call macro which isolates details acorss multiple functions
     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = floatSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const floatArray* Other_floatParallelArray = NULL;
               floatSerialArray* Other_floatSerialArray   = NULL;

               Other_floatParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         floatSerialArray *Destination_SerialArray = NULL;
                         floatSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_floatSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build floatSerialArray Temp \n");

                              floatSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling floatSerialArray::Build_Pointer_To_View_Of_Array Other_floatSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_floatSerialArray = 
                                   floatSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_floatSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_floatSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("VSG UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_floatSerialArray->getBase(0),
                                        Other_floatSerialArray->getBound(0),
                                        Other_floatSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_floatSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_floatSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_floatParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_floatSerialArray \n");

                                  Other_floatSerialArray = new floatSerialArray ();
                                  APP_ASSERT (Other_floatSerialArray != NULL);

                                  Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_floatParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                               if (Other_floatParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_floatParallelArray->isView() == TRUE);
                                  }

                               Other_floatSerialArray =
                                    floatSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_floatParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_floatSerialArray != NULL);

                               APP_ASSERT (Other_floatSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_floatParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_floatSerialArray->displayReferenceCounts("Other_floatSerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("OVERLAP UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_floatSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = List_Of_SerialArrays[0];
     Lhs_SerialArray = Matching_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 floatArray::Parallel_Conformability_Enforcement delete the lists of parallel and serial arrays \n");
#endif

#if 1
  // Is this really good enough (since it fails to modify the reference counts!)
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

  // Should these assertions include "-1"?
     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
// Used for the replace operator (scalar and array)
Array_Conformability_Info_Type*
floatArray::Parallel_Conformability_Enforcement
      (const floatArray  & Lhs_ParallelArray,
       floatSerialArray*  & Lhs_SerialArray,
       const intArray & Rhs_ParallelArray,
       intSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #3 floatArray::Parallel_Conformability_Enforcement (floatArray.Array_ID = %d, floatSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #3 floatArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #3 floatArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif
//================ END DEBUG SECTION ============================
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Two operand message passing interpretation support

     int num_arrays = 0;
     const floatArray** List_Of_ParallelArrays = NULL;
     floatSerialArray** List_Of_SerialArrays   = NULL;

     const intArray* Mask_ParallelArray = &Rhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     const floatArray* Matching_ParallelArray = &Lhs_ParallelArray;
     floatSerialArray* Matching_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = floatSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const floatArray* Other_floatParallelArray = NULL;
               floatSerialArray* Other_floatSerialArray   = NULL;

               Other_floatParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         floatSerialArray *Destination_SerialArray = NULL;
                         floatSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_floatSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build floatSerialArray Temp \n");

                              floatSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling floatSerialArray::Build_Pointer_To_View_Of_Array Other_floatSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_floatSerialArray = 
                                   floatSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_floatSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_floatSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("VSG UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_floatSerialArray->getBase(0),
                                        Other_floatSerialArray->getBound(0),
                                        Other_floatSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_floatSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_floatSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_floatParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_floatSerialArray \n");

                                  Other_floatSerialArray = new floatSerialArray ();
                                  APP_ASSERT (Other_floatSerialArray != NULL);

                                  Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_floatParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                               if (Other_floatParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_floatParallelArray->isView() == TRUE);
                                  }

                               Other_floatSerialArray =
                                    floatSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_floatParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_floatSerialArray != NULL);

                               APP_ASSERT (Other_floatSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_floatParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_floatSerialArray->displayReferenceCounts("Other_floatSerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("OVERLAP UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_floatSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = Mask_SerialArray;
     Lhs_SerialArray = Matching_SerialArray;

     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase());
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase());

     return Return_Array_Set;
   }
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
floatArray::Parallel_Conformability_Enforcement (
       const floatArray & This_ParallelArray, floatSerialArray* & This_SerialArray,
       const intArray & Lhs_ParallelArray, intSerialArray* & Lhs_SerialArray,
       const floatArray & Rhs_ParallelArray,  floatSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #4 floatArray::Parallel_Conformability_Enforcement (floatArray.Array_ID = %d, floatSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     This_ParallelArray.Test_Consistency ("Test This_ParallelArray in #4 floatArray::Parallel_Conformability_Enforcement");
     Lhs_ParallelArray.Test_Consistency  ("Test Lhs_ParallelArray in #4 floatArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency  ("Test Rhs_ParallelArray in #4 floatArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(This_SerialArray == NULL);
     APP_ASSERT(Lhs_SerialArray  == NULL);
     APP_ASSERT(Rhs_SerialArray  == NULL);
#endif
//================ END DEBUG SECTION ============================

  // printf ("WARNING: Inside of #4 floatArray::Parallel_Conformability_Enforcement -- Case of 3 array operations (typically where statements) not properly handled for general distributions \n");

  // ... Case where This_ParallelArray is null doesn't work yet because
  // Return_Array_Set will be associated with the wrong array if
  // another array than This_ParallelArray is set to be the 
  // Matching_ParallelArray ...

   /*
   if (This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
   {
     printf("Sorry, case where This_ParallelArray is Null not");
     printf(" implemented yet\n");
     APP_ABORT();
   }
   */

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info 
  //      (*(This_ParallelArray.Array_Descriptor), *(Lhs_ParallelArray.Array_Descriptor), *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     int num_arrays = 1;
     const floatArray** List_Of_ParallelArrays = new const floatArray* [num_arrays];  
     APP_ASSERT(List_Of_ParallelArrays != NULL);

     floatSerialArray** List_Of_SerialArrays   = new floatSerialArray* [num_arrays];
     APP_ASSERT(List_Of_SerialArrays   != NULL);

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_ParallelArrays[i] = NULL;
          List_Of_SerialArrays  [i] = NULL;
        }

     List_Of_ParallelArrays[0] = &Rhs_ParallelArray;

 // const floatArray* Matching_ParallelArray = &This_ParallelArray;
    const floatArray* Matching_ParallelArray;
    if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
       {
         Matching_ParallelArray    = &This_ParallelArray;
         List_Of_ParallelArrays[0] = &Rhs_ParallelArray;
       }
      else
       {
         Matching_ParallelArray    = &Rhs_ParallelArray;
         List_Of_ParallelArrays[0] = &This_ParallelArray;
       }

  // Matching_ParallelArray = &This_ParallelArray;
     floatSerialArray* Matching_SerialArray = NULL;

     const intArray* Mask_ParallelArray = &Lhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = floatSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (after floatSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const floatArray* Other_floatParallelArray = NULL;
               floatSerialArray* Other_floatSerialArray   = NULL;

               Other_floatParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         floatSerialArray *Destination_SerialArray = NULL;
                         floatSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_floatSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build floatSerialArray Temp \n");

                              floatSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling floatSerialArray::Build_Pointer_To_View_Of_Array Other_floatSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_floatSerialArray = 
                                   floatSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_floatSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_floatSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("VSG UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_floatSerialArray->getBase(0),
                                        Other_floatSerialArray->getBound(0),
                                        Other_floatSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after floatSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_floatSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_floatSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_floatParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_floatParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_floatSerialArray \n");

                                  Other_floatSerialArray = new floatSerialArray ();
                                  APP_ASSERT (Other_floatSerialArray != NULL);

                                  Source_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_floatParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_floatParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_floatParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                               if (Other_floatParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_floatParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_floatParallelArray->isView() == TRUE);
                                  }

                               Other_floatSerialArray =
                                    floatSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_floatParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_floatSerialArray != NULL);

                               APP_ASSERT (Other_floatSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_floatParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_floatSerialArray->displayReferenceCounts("Other_floatSerialArray in PCE (base of Overlap Update after floatSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_floatSerialArray->view("OVERLAP UPDATE: Other_floatSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_floatSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_floatSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
        {
          Rhs_SerialArray = List_Of_SerialArrays[0];
          This_SerialArray = Matching_SerialArray;
        }
       else
        {
          This_SerialArray = List_Of_SerialArrays[0];
          Rhs_SerialArray = Matching_SerialArray;
        }

     Lhs_SerialArray = Mask_SerialArray;

#if 1
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

     return Return_Array_Set;
   }


// *********************************************************************
//                PARALLEL INDIRECT CONFORMABILITY ENFORCEMENT
// *********************************************************************
// ... get rid of const on Lhs until a better soln is found ...

Array_Conformability_Info_Type*
floatArray::Parallel_Indirect_Conformability_Enforcement (
       const floatArray & Lhs,  
       floatSerialArray* & Lhs_SerialArray,
       const floatArray & Rhs,  
       floatSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
floatArray::Parallel_Indirect_Conformability_Enforcement (
       const floatArray & Lhs,  
       floatSerialArray* & Lhs_SerialArray,
       const intArray & Rhs,  
       intSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }
#endif

// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
floatArray::Parallel_Indirect_Conformability_Enforcement (
      const floatArray & Lhs,
       floatSerialArray* & Lhs_SerialArray)
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // This section handles the details of parallel support for indirect addressing
// I think that Brian M. has to separate out some functionality in the lower 
// level support so this can be supported.

// New code for indirect addressing support
   printf ("Need to have the indirect addressing support broken out to compute A(I) separately. \n");
   APP_ABORT();

;

     return Return_Array_Set;
   }

#undef FLOATARRAY

#define INTARRAY
#if !defined(PPP)
#error (conform_enforce.C) This is only code for P++
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type*
intArray::Parallel_Conformability_Enforcement (const intArray & X_ParallelArray,  intSerialArray* & X_SerialArray )
   {
  // This is a trivial case for the Parallel_Conformability_Enforcement since there is only a single array is use
  // and thus nothing to be forced to be aligned (and thus make a serial conformable operation)

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 3)
          printf ("Inside of #1 intArray::Parallel_Conformability_Enforcement (intArray.Array_ID = %d, intSerialArray* = %p) \n",
              X_ParallelArray.Array_ID(),X_SerialArray);
     X_ParallelArray.Test_Consistency ("Test X_ParallelArray in #1 intArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(X_SerialArray == NULL);
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_ParallelArray.displayReferenceCounts("X_ParallelArray in #1 intArray::Parallel_Conformability_Enforcement (intArray,intSerialArray*)");
        }
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ( *(X_ParallelArray.Array_Descriptor) );
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
     APP_ASSERT( Return_Array_Set != NULL );


  // X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray;
  // We must increment the reference count so that when the operator 
  // deletes the X_SerialArray pointer the SerialArray of the 
  // X_ParallelArray will not be deleted.
  // X_SerialArray->incrementReferenceCount();

#if 0
  // I think that if the input is a temporary then this approach will be a problem
     APP_ASSERT(X_ParallelArray.Array_Descriptor.SerialArray->isTemporary() == FALSE);
     X_SerialArray = X_ParallelArray.Array_Descriptor.SerialArray->getLocalArrayWithGhostBoundariesPointer();
#else
  // This set of Index objects will define the Lhs view we will use to do the serial operation
     Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
     Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

     int nd;
     for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
        {
          Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);
          int tempBase   = X_ParallelArray.Array_Descriptor.SerialArray->getBase(nd);
          int tempBound  = X_ParallelArray.Array_Descriptor.SerialArray->getBound(nd);
          int tempStride = X_ParallelArray.Array_Descriptor.SerialArray->getStride(nd);
          *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

          APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
       // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
        }

     X_SerialArray = intSerialArray::Build_Pointer_To_View_Of_Array 
                          (*X_ParallelArray.Array_Descriptor.SerialArray, Matching_Index_Pointer_List );
#endif

     APP_ASSERT( X_SerialArray != NULL );

#if COMPILE_DEBUG_STATEMENTS
     if (Diagnostic_Manager::getReferenceCountingReport() > 0)
        {
       // This mechanism outputs reports which allow us to trace the reference counts
          X_SerialArray->displayReferenceCounts("X_SerialArray in #1 intArray::Parallel_Conformability_Enforcement (intArray,intSerialArray*)");
        }
#endif

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Leaving #1 intArray::Parallel_Conformability_Enforcement (Return_Array_Set = %p) \n",Return_Array_Set); 
#endif

     APP_ASSERT (X_SerialArray->getReferenceCount() >= getReferenceCountBase());
     return Return_Array_Set;
   }

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
intArray::Parallel_Conformability_Enforcement (
       const intArray & Lhs_ParallelArray,  
       intSerialArray* & Lhs_SerialArray,
       const intArray & Rhs_ParallelArray,  
       intSerialArray* & Rhs_SerialArray )
   {
  // This function returns a serial array for the Lhs and Rhs for which a conformable 
  // serial array operation can be defined.  For the Lhs we assume an owner compute model
  // (so no communication is required).  But for the Rhs we do what ever communication is 
  // required (unless the overlap update is used in which case we reduce the size of the Lhs
  // serial array to make the operation conformable).

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Inside of #2 intArray::Parallel_Conformability_Enforcement (intArray.Array_ID = %d, intSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #2 intArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #2 intArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info ();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

  // Two operand message passing interpretation support
     int num_arrays = 1;
     const intArray** List_Of_ParallelArrays = new const intArray* [num_arrays];
     APP_ASSERT (List_Of_ParallelArrays != NULL);
     intSerialArray** List_Of_SerialArrays   = new intSerialArray* [num_arrays];
     APP_ASSERT (List_Of_SerialArrays != NULL);

     intArray* Mask_ParallelArray          = NULL;
     intSerialArray* Mask_SerialArray      = NULL;
     intSerialArray* Matching_SerialArray   = NULL;

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_SerialArrays[i] = NULL;
        }

     const intArray* Matching_ParallelArray = &Lhs_ParallelArray;
     List_Of_ParallelArrays[0]             = &Rhs_ParallelArray;

  // Call macro which isolates details acorss multiple functions
     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling intSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = intSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = List_Of_SerialArrays[0];
     Lhs_SerialArray = Matching_SerialArray;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #2 intArray::Parallel_Conformability_Enforcement delete the lists of parallel and serial arrays \n");
#endif

#if 1
  // Is this really good enough (since it fails to modify the reference counts!)
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

  // Should these assertions include "-1"?
     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase()-1);

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
// Used for the replace operator (scalar and array)
Array_Conformability_Info_Type*
intArray::Parallel_Conformability_Enforcement
      (const intArray  & Lhs_ParallelArray,
       intSerialArray*  & Lhs_SerialArray,
       const intArray & Rhs_ParallelArray,
       intSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #3 intArray::Parallel_Conformability_Enforcement (intArray.Array_ID = %d, intSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     Lhs_ParallelArray.Test_Consistency ("Test Lhs_ParallelArray in #3 intArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency ("Test Rhs_ParallelArray in #3 intArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(Lhs_SerialArray == NULL);
     APP_ASSERT(Rhs_SerialArray == NULL);
#endif
//================ END DEBUG SECTION ============================
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info
  //      (*(Lhs_ParallelArray.Array_Descriptor) , *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     APP_ASSERT (Communication_Manager::Number_Of_Processors > 0);
     APP_ASSERT (Communication_Manager::Number_Of_Processors <= MAX_PROCESSORS);

  // Two operand message passing interpretation support

     int num_arrays = 0;
     const intArray** List_Of_ParallelArrays = NULL;
     intSerialArray** List_Of_SerialArrays   = NULL;

     const intArray* Mask_ParallelArray = &Rhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     const intArray* Matching_ParallelArray = &Lhs_ParallelArray;
     intSerialArray* Matching_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling intSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = intSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     Rhs_SerialArray = Mask_SerialArray;
     Lhs_SerialArray = Matching_SerialArray;

     APP_ASSERT (Lhs_SerialArray->getReferenceCount() >= getReferenceCountBase());
     APP_ASSERT (Rhs_SerialArray->getReferenceCount() >= getReferenceCountBase());

     return Return_Array_Set;
   }
#endif

// *********************************************************************
//                     PARALLEL CONFORMABILITY ENFORCEMENT
// *********************************************************************
Array_Conformability_Info_Type* 
intArray::Parallel_Conformability_Enforcement (
       const intArray & This_ParallelArray, intSerialArray* & This_SerialArray,
       const intArray & Lhs_ParallelArray, intSerialArray* & Lhs_SerialArray,
       const intArray & Rhs_ParallelArray,  intSerialArray* & Rhs_SerialArray )
   {
//================ DEBUG SECTION ============================
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Inside of #4 intArray::Parallel_Conformability_Enforcement (intArray.Array_ID = %d, intSerialArray* = %p) \n",
              Lhs_ParallelArray.Array_ID(),Lhs_SerialArray);
     This_ParallelArray.Test_Consistency ("Test This_ParallelArray in #4 intArray::Parallel_Conformability_Enforcement");
     Lhs_ParallelArray.Test_Consistency  ("Test Lhs_ParallelArray in #4 intArray::Parallel_Conformability_Enforcement");
     Rhs_ParallelArray.Test_Consistency  ("Test Rhs_ParallelArray in #4 intArray::Parallel_Conformability_Enforcement");
     APP_ASSERT(This_SerialArray == NULL);
     APP_ASSERT(Lhs_SerialArray  == NULL);
     APP_ASSERT(Rhs_SerialArray  == NULL);
#endif
//================ END DEBUG SECTION ============================

  // printf ("WARNING: Inside of #4 intArray::Parallel_Conformability_Enforcement -- Case of 3 array operations (typically where statements) not properly handled for general distributions \n");

  // ... Case where This_ParallelArray is null doesn't work yet because
  // Return_Array_Set will be associated with the wrong array if
  // another array than This_ParallelArray is set to be the 
  // Matching_ParallelArray ...

   /*
   if (This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
   {
     printf("Sorry, case where This_ParallelArray is Null not");
     printf(" implemented yet\n");
     APP_ABORT();
   }
   */

  // The Array_Conformability_Info is passed from array object to 
  // array object through the evaluation of the array expression
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();
  // Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info 
  //      (*(This_ParallelArray.Array_Descriptor), *(Lhs_ParallelArray.Array_Descriptor), *(Rhs_ParallelArray.Array_Descriptor) );
     APP_ASSERT( Return_Array_Set != NULL );

     int num_arrays = 1;
     const intArray** List_Of_ParallelArrays = new const intArray* [num_arrays];  
     APP_ASSERT(List_Of_ParallelArrays != NULL);

     intSerialArray** List_Of_SerialArrays   = new intSerialArray* [num_arrays];
     APP_ASSERT(List_Of_SerialArrays   != NULL);

  // Initialize the list of serial array pointers to NULL (avoid purify UMR errors)
     int i = 0;
     for (i=0; i < num_arrays; i++)
        {
          List_Of_ParallelArrays[i] = NULL;
          List_Of_SerialArrays  [i] = NULL;
        }

     List_Of_ParallelArrays[0] = &Rhs_ParallelArray;

 // const intArray* Matching_ParallelArray = &This_ParallelArray;
    const intArray* Matching_ParallelArray;
    if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
       {
         Matching_ParallelArray    = &This_ParallelArray;
         List_Of_ParallelArrays[0] = &Rhs_ParallelArray;
       }
      else
       {
         Matching_ParallelArray    = &Rhs_ParallelArray;
         List_Of_ParallelArrays[0] = &This_ParallelArray;
       }

  // Matching_ParallelArray = &This_ParallelArray;
     intSerialArray* Matching_SerialArray = NULL;

     const intArray* Mask_ParallelArray = &Lhs_ParallelArray;
     intSerialArray* Mask_SerialArray   = NULL;

     // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
     if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
        {
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$  PARALLEL CONFORMABILITY ENFORCEMENT MACRO  $$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");
          printf ("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ \n");

          printf ("Matching_ParallelArray: global range (%d,%d,%d) local range (%d,%d,%d) \n",
               Matching_ParallelArray->getGlobalMaskIndex(0).getBase(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getBound(),
               Matching_ParallelArray->getGlobalMaskIndex(0).getStride(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBase(),
               Matching_ParallelArray->getLocalMaskIndex(0).getBound(),
               Matching_ParallelArray->getLocalMaskIndex(0).getStride());
          int na;
          for (na=0; na < num_arrays; na++)
               printf ("List_Of_ParallelArrays[0]: global range (%d,%d,%d) local range (%d,%d,%d) \n",
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getGlobalMaskIndex(0).getStride(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBase(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getBound(),
                    List_Of_ParallelArrays[na]->getLocalMaskIndex(0).getStride());
        }

  // *************** START OF INTERPRETATION MACRO *************
  // We want to seperate out the case where the ghost boundary widths 
  // are the same since then the local A++ operations are simple (in 
  // the case of no or simple indexing).  This also allows these case 
  // to be seperated for simplified debugging
     bool Has_Same_Ghost_Boundary_Widths = TRUE;
     int na;
     int nd;
     for (na=0; na < num_arrays; na++)
        {
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
	            (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
	             List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
        }

     if (Mask_ParallelArray != NULL) 
          Has_Same_Ghost_Boundary_Widths = Has_Same_Ghost_Boundary_Widths &&
               Partitioning_Type::Has_Same_Ghost_Boundary_Widths
                    (Matching_ParallelArray->Array_Descriptor.Array_Domain, 
                     Mask_ParallelArray->Array_Descriptor.Array_Domain);

     if (Has_Same_Ghost_Boundary_Widths == FALSE)
        {
          printf ("SORRY NOT IMPLEMENTED: ghost boundaries of all pairs \n");
          printf (" of operands must be the same width (this is a Block-PARTI constraint). \n");
          APP_ABORT();
        }
     APP_ASSERT (Has_Same_Ghost_Boundary_Widths == TRUE);

  // ... Find out how many arrays are non-null.  Matching_ParallelArray 
  // must be non-null unless all arrays are null arrays also ...

     int num_non_null = 0;
     for (na=0;na<num_arrays;na++)
          if (!List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;
     if (Mask_ParallelArray != NULL) 
          if (!Mask_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
               num_non_null++;

     if (Matching_ParallelArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
          APP_ASSERT(num_non_null==0);
       else
          num_non_null++;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf("Number of Non-NullArrays = %d \n",num_non_null);
#endif

#if COMPILE_DEBUG_STATEMENTS
  // Check that these are all NULL
     APP_ASSERT (Matching_SerialArray == NULL);
     for (na=0; na < num_arrays; na++)
        {
          APP_ASSERT (List_Of_SerialArrays[na] == NULL);
        }
     APP_ASSERT (Mask_SerialArray == NULL);
#endif

     if ( ( (Communication_Manager::Number_Of_Processors == 1)
            && !SIMULATE_MULTIPROCESSOR && Has_Same_Ghost_Boundary_Widths )
          || (num_non_null <= 1) )
        {
       // This is a shortcut through the PCE for the special cases (see the conditional test expression)
          Matching_SerialArray = Matching_ParallelArray->getSerialArrayPointer();
          Matching_SerialArray->incrementReferenceCount();

          for (na=0; na < num_arrays; na++)
             {
               List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
               List_Of_SerialArrays[na]->incrementReferenceCount();
             }
          if (Mask_ParallelArray != NULL) 
             {
               Mask_SerialArray = Mask_ParallelArray->getSerialArrayPointer();
               Mask_SerialArray->incrementReferenceCount();
             }
        }
       else
        {
          const Array_Domain_Type* Matching_Parallel_Descriptor =
               &Matching_ParallelArray->Array_Descriptor.Array_Domain;
          const SerialArray_Domain_Type* Matching_Serial_Descriptor =
               &((Matching_ParallelArray->Array_Descriptor.SerialArray)->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
       // Matching_Parallel_Descriptor->display("In PCE: Matching_Parallel_Descriptor");
#endif

          Internal_Index Matching_Partition_Index [MAX_ARRAY_DIMENSION];
          Internal_Index Other_Partition_Index    [MAX_ARRAY_DIMENSION];
          Internal_Index Mask_Partition_Index     [MAX_ARRAY_DIMENSION];

       // This is where the communication model is chosen.  In the case 
       // of the VSG model we will have aditional work to do -- but if 
       // the OVERLAP model is used then we will have spend only a minor 
       // amount of time saving a simple state and doing no message
       // passing.  But we can only use the OVERLAP model for alligned 
       // array operations.

       // ... Loop over all arrays to find intersections if the 
       // OVERLAP model works for the pair of Matching and each other
       // array.  For pairs where the VSG model is used the
       // constructed temporary array for communication must be
       // conformable with the intersection found for all of the
       // OVERLAP model pairs. ...

       // ... The Partition_Index is not adjusted here because the
       // adjustment amount isn't known until the final intersection is computed. ...

       // This is not particularly efficient
          int* Final_VSG_Model = new int[num_arrays+1];
          APP_ASSERT (Final_VSG_Model != NULL);

          for (na=0; na < num_arrays; na++)
             {
               const Array_Domain_Type* Other_Parallel_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

#if COMPILE_DEBUG_STATEMENTS
            // Other_Parallel_Descriptor->display("In PCE: Other_Parallel_Descriptor");
#endif

               Final_VSG_Model[na] =
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[na] == FULL_VSG_MODEL);
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Matching_Partition_Index for axis = %d \n",nd);
                    Matching_Partition_Index[nd].display("Matching_Partition_Index (after Interpret_Message_Passing) BEFORE MODIFICATION!");
                  }
             }
       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               Return_Array_Set->display("Return_Array_Set from Lhs and Rhs");
             }
#endif

       // ... also make Mask array match if there is one ...
          if (Mask_ParallelArray != NULL) 
             {
            // printf ("Valid Mask: initialize Final_VSG_Model[num_arrays] \n");
               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor =
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               Final_VSG_Model[num_arrays] = 
                    Array_Domain_Type::Interpret_Message_Passing 
                         (*Return_Array_Set,
                          *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                          *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                          Matching_Partition_Index, Other_Partition_Index);

            // added assert (7/26/2000)
               if (FORCE_VSG_UPDATE)
                    APP_ASSERT (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL);

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 3)
                  {
                    Return_Array_Set->display("Return_Array_Set from Lhs/Rhs and Where Mask");
                  }
#endif
             }
            else
             {
            // initialize this to zero just so it is not left uninitialized
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays] \n");
               Final_VSG_Model[num_arrays] = UNDEFINED_VSG_MODEL;  // OVERLAPPING_BOUNDARY_VSG_MODEL;
            // printf ("Invalid Mask: initialize Final_VSG_Model[num_arrays= %d] = %d \n",num_arrays,Final_VSG_Model[num_arrays]);
             }
#if 1
#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 0)
             {
               for (na=0; na < num_arrays; na++)
                  {
                    printf ("(Lhs,Rhs[%d]) pair: Final_VSG_Model[%d] = %s Update. \n",
                         na,na,(Final_VSG_Model[na] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
                  }
               printf ("(Lhs,Mask) pair: Final_VSG_Model[%d] = %s Update. \n",
                    num_arrays,(Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) ? "FULL_VSG_MODEL" : "NON VSG");
             }
#endif
#endif

       // ... Build view of Lhs that is conformable with all intersected
       // arrays that can use Overlapping Boundary Model to 
       // communucate with Lhs. If all communication is done
       // through full Vsg this will just be the original SerialArray. ...  

       // ... bug fix (10/11/96,kdb) reset Matching_Partition_Index to the
       // Local_Mask_Index because Return_Array_Set now contains the changes 
       // in Matching_Parallel_Descriptor->Array_Conformability_Info as well as 
       // those just computed and so Matching_Partition_Index will be 
       // adjusted too much if not reset ...

       // added assert (7/26/2000)
          if (FORCE_VSG_UPDATE == FALSE)
             {
               for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    Matching_Partition_Index[nd] = Matching_Parallel_Descriptor->Local_Mask_Index[nd];
                    Matching_Partition_Index[nd].adjustBase (Return_Array_Set->Truncate_Left_Ghost_Boundary_Width[nd]);
                    Matching_Partition_Index[nd].adjustBound (Return_Array_Set->Truncate_Right_Ghost_Boundary_Width[nd]);
                  }
             }

       // This set of Index objects will define the Lhs view we will use to do the serial operation
          Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Matching_Index_Pointer_List;

          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Matching_Index_Pointer_List [nd] = &(Matching_Partition_Index[nd]);

            // Matching_Index_Pointer_List [nd]->display("BEFORE ADJUSTMENT");
            // Now adjust the stride (should have a index member function for this!)
            // We need a unit stride range when we index even a strided array object
            // int tempBase   = Matching_Index_Pointer_List[nd]->getBase();
            // int tempBound  = Matching_Index_Pointer_List[nd]->getBound();
            // int tempStride = Matching_Index_Pointer_List[nd]->getStride();
               int tempBase   = Matching_ParallelArray->Array_Descriptor.SerialArray->getBase(nd);
               int tempBound  = Matching_ParallelArray->Array_Descriptor.SerialArray->getBound(nd);
               int tempStride = Matching_ParallelArray->Array_Descriptor.SerialArray->getStride(nd);
            // *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBase+((tempBound-tempBase)/tempStride),tempStride);
               *Matching_Index_Pointer_List [nd] = Range (tempBase,tempBound,tempStride);

               APP_ASSERT (Matching_Index_Pointer_List[nd]->getStride() == 1);
            // Matching_Index_Pointer_List [nd]->display("AFTER ADJUSTMENT");
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("Calling intSerialArray::Build_Pointer_To_View_Of_Array() \n");
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("Index for axis = %d \n",nd);
                    Matching_Index_Pointer_List [nd]->display("Matching_Partition_Index AFTER MODIFICATION!");
                  }
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
            // We are most interested in 1D progroams currently
               printf ("Matching_Index_Pointer_List[0] range (%d,%d,%d) \n",
               Matching_Index_Pointer_List [0]->getBase(),
               Matching_Index_Pointer_List [0]->getBound(),
               Matching_Index_Pointer_List [0]->getStride());
             }
#endif

          APP_ASSERT(Matching_ParallelArray != NULL);
          APP_ASSERT(Matching_ParallelArray->Array_Descriptor.SerialArray != NULL);
	  
       // Matching_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain.display("Matching Array");
       // Matching_ParallelArray->view("Matching Array");

       // ... CHECK TO SEE IF THIS BUILDS NULL ARRAY WHEN IT SHOULD ...
#if 0
       // APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
          APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
          if (Matching_ParallelArray->Array_Descriptor.SerialArray->isNullArray() == TRUE)
             {
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->getDataPointer() != NULL);
               APP_ASSERT (Matching_ParallelArray->Array_Descriptor.SerialArray->isView() == TRUE);
             }
#endif

       // This allows us to build a view of the array which include ghost boundaries so that they
       // are updated as well (????) within the communication.
          Matching_SerialArray = intSerialArray::Build_Pointer_To_View_Of_Array 
                                      (*Matching_ParallelArray->Array_Descriptor.SerialArray, 
                                      Matching_Index_Pointer_List );
          APP_ASSERT (Matching_SerialArray != NULL);

       // The new view should not be a temporary (so that it will not be absorbed into the serial array operation)
       // This can be a temporary if the input parallel array was a temporary
       // APP_ASSERT (Matching_SerialArray->isTemporary() == FALSE);
          APP_ASSERT ( (Matching_SerialArray->isTemporary() == FALSE) ||
                       ( (Matching_SerialArray->isTemporary() == TRUE) &&
                         (Matching_ParallelArray->isTemporary() == TRUE) ) );

#if 0
       // To permit the PCE functions to have a consistant interface
       // and allow the returned references to serial arrays to have
       // their reference counts decremented indpendent of if the VSG
       // or OVERLAP update is used we have to increment the reference
       // count of this array.
          Matching_SerialArray->incrementReferenceCount();
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_ParallelArray->displayReferenceCounts("Matching_ParallelArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (after intSerialArray::Build_Pointer_To_View_Of_Array)");
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               Matching_SerialArray->view("In PCE: Matching_SerialArray");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
               printf ("############################################################## \n");
             }

       // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
          if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
             {
               printf ("Matching_SerialArray: local range (%d,%d,%d) \n",
                    Matching_SerialArray->getBase(0),
                    Matching_SerialArray->getBound(0),
                    Matching_SerialArray->getStride(0));

            // printf ("Exiting after construction of Matching_SerialArray \n");
            // APP_ABORT();
             }
#endif
       // ... This is only needed for the full Vsg model but is pulled
       // out here to avoid repeating each time in the loop ...

          int Matching_Dimensions [MAX_ARRAY_DIMENSION];
          Integer_Array_MAX_ARRAY_DIMENSION_Type Integer_List;

       // Initialize this to a default value (which should cause an error if used!)
          for (nd=0;nd<MAX_ARRAY_DIMENSION;nd++)
             {
               Integer_List[nd] = -1;
             }

       // We want the RAW dimensions of the data because PARTI
       // needs the whole partition and then will fill in the
       // correct parts!

          Matching_SerialArray->Array_Descriptor.Array_Domain.getRawDataSize(Matching_Dimensions);

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
            // ... this assert causes problem in case where Matching_Serial_Descriptor
            // is not null but the Matching_SerialArray is ...
            // APP_ASSERT (Matching_Dimensions[nd] > 0);
               Integer_List[nd] = Matching_Dimensions[nd];
             }

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 2)
             {
               for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                  {
                    printf ("In macro: Integer_List[%d] = %d \n",nd,Integer_List[nd]);
                    APP_ASSERT(Integer_List[nd] >= 0);
                  }
             }

          for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
             {
               APP_ASSERT(Integer_List[nd] >= 0);
             }
#endif

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (before communication)");
             }
#endif

       // Now do the communication (if required) to support the binary operation!
          for (na=0; na < num_arrays; na++)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = List_Of_ParallelArrays[na];

               const Array_Domain_Type* Other_Parallel_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
                    &(List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
                 // present on the local processor (the trivial case since there is nothing to do)

                 // Initialize the Matching_SerialArray and Other_SerialArray 
                 // to the existing (NULL) serial arrays

                    List_Of_SerialArrays[na] = List_Of_ParallelArrays[na]->Array_Descriptor.SerialArray;
                    APP_ASSERT(List_Of_SerialArrays[na] != NULL);
                    List_Of_SerialArrays[na]->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (case of null arrays)");
                       }
#endif

                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[na] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         if (List_Of_SerialArrays[na]->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*List_Of_SerialArrays[na]); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                       {
                         printf ("In PCE: Runtime selected communication model is %s UPDATE \n",
                              ((Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE) ? "VSG" : "OVERLAP");
	               }
#endif

                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                    if ( (Final_VSG_Model[na]==FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                      // Force communication to provide correct parallel operation 
	              // of operator on unaligned arrays. The use of the PARTI 
	              // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
	              // Lhs_Partition_Index and Rhs_Partition_Index or the 
	              // Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[na] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[na] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model == UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }

                 // store pointer to the Other SerialArray in list
                    List_Of_SerialArrays[na] = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         List_Of_SerialArrays[na]->displayReferenceCounts("List_Of_SerialArrays[na] in PCE (after VSG/Overlap Update)");
                       }
#endif
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Other_Serial_Descriptor->Is_A_Null_Array )

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 1)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (in communication loop)");
                  }
#endif
             } // end of na loop

       // ... also make Mask_SerialArray conformable if it exists ...
          if (Mask_ParallelArray != NULL)
             {
               const intArray* Other_intParallelArray = NULL;
               intSerialArray* Other_intSerialArray   = NULL;

               Other_intParallelArray = Mask_ParallelArray;
               APP_ASSERT (Other_intParallelArray != NULL);

               const Array_Domain_Type* Other_Parallel_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.Array_Domain);
               const SerialArray_Domain_Type* Other_Serial_Descriptor = 
	            &(Mask_ParallelArray->Array_Descriptor.SerialArray->Array_Descriptor.Array_Domain);

               if ( Matching_Serial_Descriptor->Is_A_Null_Array && Other_Serial_Descriptor->Is_A_Null_Array )
                  {
                 // This is the case where neither the Lhs nor the Rhs are 
   	         // present on the local processor

                 // Initialize the Matching_SerialArray and Mask_SerialArray 
	         // to the existing (NULL) serial arrays
                 // Matching_SerialArray->incrementReferenceCount();

                    Mask_SerialArray = Mask_ParallelArray->Array_Descriptor.SerialArray;
                    APP_ASSERT (Mask_SerialArray != NULL);
                    Mask_SerialArray->incrementReferenceCount();

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (case of null arrays)");
                       }
#endif
                 // This code is copied (and modified) from the case above.
                 // ... updateGhostBoundaries will be called from other
                 // processors which will expect a send and receive from 
	         // this processor even if it's meaningless ...
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE)
                       {
                         printf ("WARNING: Not sure we we have to handle null array ghost boundary update for null array case in VSG update in PCE \n");
                         if (Mask_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                            {
                              Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Mask_SerialArray); 
                            }
                           else 
                            {
                              if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                 {
                                   Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray); 
                                 }
                            }
	               }
                  }
                 else
                  {
                 // OVERLAPPING_BOUNDARY_VSG_MODEL or FULL_VSG_MODEL
                 // The Mask array sets at the end of the list at position "num_arrays"
                    if ( (Final_VSG_Model[num_arrays] == FULL_VSG_MODEL) || FORCE_VSG_UPDATE )
                       {
                      // Force communication to provide correct parallel operation
                      // of operator on unaligned arrays. The use of the PARTI
                      // library from Maryland should simplify this step.

                      // Note: The FULL_VSG_MODEL does not use the
                      // Matching_Partition_Index and Other_Partition_Index or the Return_Array_Set.

                         // Post receive for incoming message from the processors owning 
                      // the Other if this processor owns part of Matching.  If this processor 
                      // owns part of the Other then send outgoing messages to the 
                      // processors owning the Matching.

                         bool Matching_Owned_By_This_Processor = 
                              !Matching_Serial_Descriptor->Is_A_Null_Array;
                         bool Other_Owned_By_This_Processor = 
                              !Other_Serial_Descriptor->Is_A_Null_Array;

#if 0
			 printf ("VSG: Matching_Owned_By_This_Processor = %s \n",(Matching_Owned_By_This_Processor) ? "TRUE" : "FALSE");
			 printf ("VSG: Other_Owned_By_This_Processor ==== %s \n",(Other_Owned_By_This_Processor) ? "TRUE" : "FALSE");
#endif

                      // We only have to set one of these but then we would not 
                      // want to leave the other one uninitialized so we set both 
                      // of them here.

                         intSerialArray *Destination_SerialArray = NULL;
                         intSerialArray *Source_SerialArray      = NULL;

                         if (Matching_Owned_By_This_Processor)
                            {
                           // Prepare to receive the data into the Other_intSerialArray 
                           // from the processors owning the Other_ParallelArray 
                           // The amount ot data receive is determined by the size 
                           // of the Matching

                              APP_ASSERT( Matching_SerialArray != NULL );
                           // APP_ASSERT( Matching_SerialArray ->Array_Descriptor != NULL );

                           // Build the temporary storage for the data that will 
                           // operated upon with the Matching for what ever operation is 
                           // to be done!

                           // printf ("In PCE: Build intSerialArray Temp \n");

                              intSerialArray Temp ( Integer_List );

                           // We have to set the base of the Temp array from which 
                           // we build the view we can optimize this whole setup of 
                           // the Destination_SerialArray later.
                           // This should and can be optimized a lot more!

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   Temp.setBase(Matching_SerialArray->Array_Descriptor.Array_Domain.Data_Base[nd], nd);

                           // Now build array of Index objects so we can use the 
                           // Build_Pointer_To_View_Of_Array function
                           // We need an array of pointers to Internal_Index objects 
                           // so we have to first build an array of Internal_Index 
                           // objects and then use these to initialize the array of 
                           // pointers.
                              Index_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Array;
                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // Matching_SerialArray->view("In VSG MACRO Matching_SerialArray");

                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                 {
                                // Other_Index_Array [nd] = Internal_Index 
                                //      (Matching_ParallelArray->getLocalBase(nd),
                                //       Matching_ParallelArray->getLocalLength(nd),
                                //       Matching_ParallelArray->getLocalStride(nd) );
                                /* ... bug fix (10/9/96,kdb) these need to be raw functions */
	                        /*
                                   Other_Index_Array [nd] = Internal_Index 
                                        (Matching_SerialArray->getBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getStride(nd) );
                                 */
                                   Other_Index_Array [nd] = Internal_Index
                                        (Matching_SerialArray->getRawBase(nd),
                                         Matching_SerialArray->getLength(nd),
                                         Matching_SerialArray->getRawStride(nd) );

                                   Other_Index_Pointer_List [nd] = &(Other_Index_Array [nd]);

                                // (7/27/2000) This assert can fail if the indexing is 
                                // X(I + -1) = Y(I +  0) + Y(I +  1) + Y(I + -1); 
                                // on 4 processors with 1 ghost boundary. So comment it out!
                                // APP_ASSERT ( Other_Index_Array [nd].length() > 0 );

#if COMPILE_DEBUG_STATEMENTS
                                // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                                   if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                      {
                                        printf ("Axis = %d Build a view of Temp using index: local range (%d,%d,%d) \n",
                                             nd,
                                             Other_Index_Pointer_List [nd]->getBase(),
                                             Other_Index_Pointer_List [nd]->getBound(),
                                             Other_Index_Pointer_List [nd]->getStride());
                                      }
#endif

                                 }

                           // APP_ASSERT (Temp.isNullArray() == FALSE);
                              APP_ASSERT (Temp.getDataPointer() != NULL);
                              if (Temp.isNullArray() == TRUE)
                                 {
                                   APP_ASSERT (Temp.getDataPointer() != NULL);
                                   APP_ASSERT (Temp.isView() == TRUE);
                                 }

                           // printf ("In PCE: Calling intSerialArray::Build_Pointer_To_View_Of_Array Other_intSerialArray \n");

                           // We have to have a view of the restricted domain here
                           // to avoid a nonconformable operation
                              Other_intSerialArray = 
                                   intSerialArray::Build_Pointer_To_View_Of_Array (Temp, Other_Index_Pointer_List );
#if 0
                           // To permit the PCE functions to have a consistant interface
                           // and allow the returned references to serial arrays to have
                           // their reference counts decremented indpendent of if the VSG
                           // or OVERLAP update is used we have to increment the reference
                           // count of this array.
                              Other_intSerialArray->incrementReferenceCount();
#endif

                           // The new view should not be a temporary (so that it will not be 
                           // absorbed into the serial array operation)
                              APP_ASSERT (Other_intSerialArray->isTemporary() == FALSE);

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 2)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Temp.view("VSG UPDATE: Temp (serial array)");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("VSG UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
                           // Helpful mechanism for debugging communication models (VSG and Overlap updates models)
                              if (Diagnostic_Manager::getMessagePassingInterpretationReport() > 0)
                                 {
                                   printf ("Temp (destination for all recieved messages): local range (%d,%d,%d) \n",
                                        Temp.getBase(0),
                                        Temp.getBound(0),
                                        Temp.getStride(0));
                                   printf ("Other_SerialArray (relavant view of temp): local range (%d,%d,%d) \n",
                                        Other_intSerialArray->getBase(0),
                                        Other_intSerialArray->getBound(0),
                                        Other_intSerialArray->getStride(0));

                                // printf ("Exiting in VSG PCE after intSerialArray::Build_Pointer_To_View_Of_Array() \n");
                                // APP_ABORT();
                                 }
#endif

                           // After we use this array we will delete it in the 
                           // operator code!  There we want it to be deleted so 
                           // don't increment the reference count.

#if COMPILE_DEBUG_STATEMENTS
                           // Quick test of conformability
                              for (nd=0; nd < MAX_ARRAY_DIMENSION; nd++)
                                   APP_ASSERT ( Matching_SerialArray->getLength(nd) == Other_intSerialArray->getLength(nd) );
#endif

                              Destination_SerialArray = Other_intSerialArray;

                           // If the Other is not present on the local processor then 
                           // this is a NullArray but in this case no messages are 
                           // sent from this array -- so this is OK.
                           // if the Other is present then the source must come from 
                           // the Other and so we set the Source_SerialArray to point 
                           // to the local part on of the Other_ParallelArray.

                              Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                           }
                          else
                           {
                             if (Other_Owned_By_This_Processor)
                                {
                               // And this case implies that the Matching is not owned by this processor!

                                  APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray != NULL );
                               // APP_ASSERT( Other_intParallelArray->Array_Descriptor.SerialArray->
                               //             Array_Descriptor!=NULL);

                               // Send data from processors owning Other to processors owning 
                               // Matching but there is no operation that takes place if the Matching 
                               // is not owned.  Set Matching and Other SerialArrays to NullArrays 
                               // since the Matching is not owned by this processor

#if 0
                                  APP_ASSERT(Matching_Serial_Descriptor->Is_A_Null_Array == TRUE);
                                  Matching_SerialArray = Matching_ParallelArray->Array_Descriptor.SerialArray;

                               // After we use this array we will delete it in the operator 
                               // code! But since it is a reference to the 
                               // Other_intParallelArray->SerialArray we don't want it to be deleted!

                               // Bugfix (12/15/2000) don't modify the reference count of the Matching_SerialArray
                               // Matching_SerialArray->incrementReferenceCount();
#endif

                               // printf ("In PCE: Allocate an array object for Other_intSerialArray \n");

                                  Other_intSerialArray = new intSerialArray ();
                                  APP_ASSERT (Other_intSerialArray != NULL);

                                  Source_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                  APP_ASSERT (Source_SerialArray != NULL);

                               // We just need to set this to something (not NULL)
                                  Destination_SerialArray = Other_intParallelArray->Array_Descriptor.SerialArray;
                                }
                               else
                                {
                               // Neither the Matching nor the Other is present on this processor!
                                  printf ("ERROR: Neither the Matching nor the Other is present on this processor! \n");
                                  printf ("ERROR: This case should not be possible ... \n");
                                  APP_ABORT();
                                }
                           }

                     // The Other is the source and the Matching is the destination
                        if( automaticCommunication ) Partitioning_Type::regularSectionTransfer
                             ( *Matching_ParallelArray , *Destination_SerialArray , 
                               *Other_intParallelArray  , *Source_SerialArray );

                     // ... (11/25/96,kdb) operator= updates the ghost boundaries but 
                     // they need to be updated before that (might merge below
                     // into above sometime later) ...

                     // ... (1/9/97,kdb) update ghost boundaries will update the
                     // ghost boundaries even if they aren't in the view so
                     // Destination_SerialArray can't be used here because if
                     // the Matching_SerialArray is null the Destination_SerialArray
                     // will be set to the Other_SerialArray which might be a
                     // different size causing out of bounds reads and writes.

                     // Internal_Partitioning_Type::updateGhostBoundaries
                     //      (*Matching_ParallelArray,*Destination_SerialArray);

                        if (Matching_Owned_By_This_Processor)
                           {
                             Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Destination_SerialArray);
                           }
                          else
                           {
                          // Wouldn't it be better to call isNullArray() ?
                             if (Matching_SerialArray->Array_Descriptor.Array_Domain.Size[0] > 0)
                                {
                                  Internal_Partitioning_Type::updateGhostBoundaries (*Matching_ParallelArray,*Matching_SerialArray);
                                }
                           }

;
                       }
                      else
                       {
                         if (Final_VSG_Model[num_arrays] == OVERLAPPING_BOUNDARY_VSG_MODEL)
                            {
                           // Defer the communication for the operation by truncating
	                   // the view that the A++ (serial) operation uses locally 
                           // on this processor and record that the ghost boundary of
                           // the Lhs (which calls operator=) will require updating.

                              // Defer the communication for the operation by truncating the view 
                           // that the A++ (serial) operation uses locally on this processor 
                           // and record that the ghost boundary of the Lhs (which calls 
                           // operator=) will require updating.

                           // Build the views which the binary operators will perform operations 
                           // on.  These views are guarenteed to be conformable (which is part 
                           // of the purpose of this function).  By if the views are not of the 
                           // whole array then ghost boundaries will have to be updated by  
                           // message passing.

                              Index_Pointer_Array_MAX_ARRAY_DIMENSION_Type Other_Index_Pointer_List;

                           // initialize Other_Partition_Index one dimension at a time
                           // then adjust this so it's conformable with Lhs_Partition_Index[nd] 

                              if (!Matching_SerialArray->Array_Descriptor.Array_Domain.Is_A_Null_Array)
                                 {
                                   for (nd = 0; nd<MAX_ARRAY_DIMENSION;nd++)
                                      {
                                        Other_Partition_Index [nd] = Other_Parallel_Descriptor->Local_Mask_Index[nd];

                                        Array_Domain_Type::Overlap_Update_Fix_Rhs
                                             (*Return_Array_Set,
                                              *Matching_Parallel_Descriptor, *Matching_Serial_Descriptor,
                                              *Other_Parallel_Descriptor, *Other_Serial_Descriptor,
                                               Matching_Partition_Index, Other_Partition_Index, nd );

                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }
                                else
                                 {
                                   for (nd = 0; nd < MAX_ARRAY_DIMENSION; nd++)
                                      {
                                        Other_Partition_Index [nd]    = Internal_Index(0,0,1); 
                                        Other_Index_Pointer_List [nd] = &(Other_Partition_Index[nd]);
                                      }
                                 }

                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray != NULL);
                               APP_ASSERT (Other_intParallelArray->Array_Descriptor.SerialArray->isNullArray() == FALSE);
                               APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                               if (Other_intParallelArray->isNullArray() == TRUE)
                                  {
                                    APP_ASSERT (Other_intParallelArray->getDataPointer() != NULL);
                                    APP_ASSERT (Other_intParallelArray->isView() == TRUE);
                                  }

                               Other_intSerialArray =
                                    intSerialArray::Build_Pointer_To_View_Of_Array 
                                         (*(Other_intParallelArray->Array_Descriptor.SerialArray),
                                          Other_Index_Pointer_List );
                               APP_ASSERT (Other_intSerialArray != NULL);

                               APP_ASSERT (Other_intSerialArray->Array_Descriptor.Array_Domain.Is_A_Temporary == 
                                           Other_intParallelArray->Array_Descriptor.SerialArray->
                                                Array_Descriptor.Array_Domain.Is_A_Temporary);

#if COMPILE_DEBUG_STATEMENTS
                    if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                       {
                      // This mechanism outputs reports which allow us to trace the reference counts
                         Matching_SerialArray->displayReferenceCounts("Matching_SerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                         Other_intSerialArray->displayReferenceCounts("Other_intSerialArray in PCE (base of Overlap Update after intSerialArray::Build_Pointer_To_View_Of_Array(Other))");
                       }
#endif

#if COMPILE_DEBUG_STATEMENTS
                              if (APP_DEBUG > 3)
                                 {
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   Other_intSerialArray->view("OVERLAP UPDATE: Other_intSerialArray");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                   printf ("############################################################## \n");
                                 }
#endif

;
                            }
                           else
                            {
                              APP_ASSERT(Final_VSG_Model[num_arrays] == UNDEFINED_VSG_MODEL);
                              printf ("ERROR - Final_VSG_Model==UNDEFINED_VSG_MODEL \n");
                              APP_ABORT();
                            }
                       }
                  } // end of (!Matching_Serial_Descriptor->Is_A_Null_Array ||
                    //         !Mask_Serial_Descriptor.Is_A_Null_Array )

            // store pointer to the Other SerialArray in list
               Mask_SerialArray = Other_intSerialArray;

#if COMPILE_DEBUG_STATEMENTS
               if (Diagnostic_Manager::getReferenceCountingReport() > 0)
                  {
                 // This mechanism outputs reports which allow us to trace the reference counts
                    Mask_SerialArray->displayReferenceCounts("Mask_SerialArray in PCE (after VSG/Overlap Update)");
                  }
#endif
             } // end of Mask_ParallelArray != NULL

#if COMPILE_DEBUG_STATEMENTS
          if (Diagnostic_Manager::getReferenceCountingReport() > 1)
             {
            // This mechanism outputs reports which allow us to trace the reference counts
               Matching_SerialArray->displayReferenceCounts("Matching_intSerialArray in PCE (at BASE)");
             }
#endif

       // Now delete this storage
          delete [] Final_VSG_Model;

        } // end of more than 1 processor case 
   // *************** END OF INTERPRETATION MACRO *************



     if (!This_ParallelArray.Array_Descriptor.Array_Domain.Is_A_Null_Array)
        {
          Rhs_SerialArray = List_Of_SerialArrays[0];
          This_SerialArray = Matching_SerialArray;
        }
       else
        {
          This_SerialArray = List_Of_SerialArrays[0];
          Rhs_SerialArray = Matching_SerialArray;
        }

     Lhs_SerialArray = Mask_SerialArray;

#if 1
     delete [] List_Of_ParallelArrays;
     delete [] List_Of_SerialArrays;
#else
     for (i=0; i < num_arrays; i++)
        {
          if (List_Of_ParallelArrays [i] != NULL)
             {
               List_Of_ParallelArrays [i]->decrementReferenceCount();
               if (List_Of_ParallelArrays [i]->getReferenceCount() < List_Of_ParallelArrays [i]->getReferenceCountBase())
                    delete List_Of_ParallelArrays [i];
             }
          List_Of_ParallelArrays [i] = NULL;

          if (List_Of_SerialArrays [i] != NULL)
             {
               List_Of_SerialArrays [i]->decrementReferenceCount();
               if (List_Of_SerialArrays [i]->getReferenceCount() < List_Of_SerialArrays [i]->getReferenceCountBase())
                    delete List_Of_SerialArrays [i];
             }
          List_Of_SerialArrays [i] = NULL;
        }

     delete List_Of_ParallelArrays;
     delete List_Of_SerialArrays;
     List_Of_ParallelArrays = NULL;
     List_Of_SerialArrays   = NULL;
#endif

     return Return_Array_Set;
   }


// *********************************************************************
//                PARALLEL INDIRECT CONFORMABILITY ENFORCEMENT
// *********************************************************************
// ... get rid of const on Lhs until a better soln is found ...

Array_Conformability_Info_Type*
intArray::Parallel_Indirect_Conformability_Enforcement (
       const intArray & Lhs,  
       intSerialArray* & Lhs_SerialArray,
       const intArray & Rhs,  
       intSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }

#if !defined(INTARRAY)
// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
intArray::Parallel_Indirect_Conformability_Enforcement (
       const intArray & Lhs,  
       intSerialArray* & Lhs_SerialArray,
       const intArray & Rhs,  
       intSerialArray* & Rhs_SerialArray )
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // New code to support parallel indirect addressing 
  // (using Brian Miller's lower level support)

  // One of the 2 input arrays must be using indirect addressing
     APP_ASSERT ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) ||
                  (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) );

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) )
        {
       // Case of A(I) = T
          printf ("Case of A(I) = T in Macro for Indirect_Addressing_Conformable_Setup \n");

#if 0
       // build the CommunicationScheduler object (contianing the lower level support)
          CommunicationScheduler indirectAdressingSchedule ();

       // Get the indirection array out of the indexed Lhs array object
	  intArray* indirectionArrayPointer = Lhs.getDomain().Index_Array[0];
	  APP_ASSERT (indirectionArrayPointer != NULL);

       // The Lhs contains the index arrays used to index it if it uses indirect addressing
	  indirectAdressingSchedule.computeScheduleForAofI(Lhs,Rhs);

       // What is the input parameter required to execute a communication schedule?
	  indirectAdressingSchedule.executeSchedule();
#endif
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == FALSE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of T = B(J)
          printf ("Case of T = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     if ( (Lhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) &&
          (Rhs.Array_Descriptor.Array_Domain.Uses_Indirect_Addressing == TRUE) )
        {
       // Case of A(I) = B(J)
          printf ("Case of A(I) = B(J) in Macro for Indirect_Addressing_Conformable_Setup \n");

          printf ("Sorry, not implemented in Macro for Indirect_Addressing_Conformable_Setup \n");
          APP_ABORT();
        }

     printf ("Exiting in Macro for Indirect_Addressing_Conformable_Setup \n");
     APP_ABORT();
;

     return Return_Array_Set;
   }
#endif

// ... get rid of const on Lhs until a better soln is found ...
Array_Conformability_Info_Type*
intArray::Parallel_Indirect_Conformability_Enforcement (
      const intArray & Lhs,
       intSerialArray* & Lhs_SerialArray)
   {
     Array_Conformability_Info_Type *Return_Array_Set = NULL;

  // ... make new Array_Conformability_Info_Type object instead of reusing ...
     Return_Array_Set = Array_Domain_Type::getArray_Conformability_Info();

     // This section handles the details of parallel support for indirect addressing
// I think that Brian M. has to separate out some functionality in the lower 
// level support so this can be supported.

// New code for indirect addressing support
   printf ("Need to have the indirect addressing support broken out to compute A(I) separately. \n");
   APP_ABORT();

;

     return Return_Array_Set;
   }

#undef INTARRAY

int Array_Conformability_Info_Type::CLASS_ALLOCATION_POOL_SIZE = DEFAULT_CLASS_ALLOCATION_POOL_SIZE;
/* Static variable */

Array_Conformability_Info_Type* Array_Conformability_Info_Type::Current_Link                      = NULL;

int Array_Conformability_Info_Type::Memory_Block_Index                = 0;

const int Array_Conformability_Info_Type::Max_Number_Of_Memory_Blocks = MAX_NUMBER_OF_MEMORY_BLOCKS;

unsigned char *Array_Conformability_Info_Type::Memory_Block_List [MAX_NUMBER_OF_MEMORY_BLOCKS];

#define USE_CPP_NEW_DELETE_OPERATORS FALSE

#ifndef INLINE_FUNCTIONS

void *Array_Conformability_Info_Type::operator new ( size_t Size )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Call Array_Conformability_Info_Type::operator new! Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     return malloc(Size);
#else
  // Because of the way the size of the memory blocks doubles in size
  // for each proceeding memory block 100 is a good limit for the size of
  // the memory block list!

  // These were taken out to allow the new operator to be inlined!
  // const int Max_Number_Of_Memory_Blocks = 1000;
  // static unsigned char *Memory_Block_List [Max_Number_Of_Memory_Blocks];
  // static int Memory_Block_Index = 0;

     if (Size != sizeof(Array_Conformability_Info_Type))
        {
       // Bugfix (5/22/95) this case must be supported and was commented out by mistake
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from Array_Conformability_Info_Type
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

       // printf ("In Array_Conformability_Info_Type::operator new: Calling malloc because Size(%d) != sizeof(Array_Conformability_Info_Type)(%d) \n",Size,sizeof(Array_Conformability_Info_Type));

          return malloc(Size);
        }
       else
        {
       // printf ("In Array_Conformability_Info_Type::operator new: Using the pool mechanism Size(%d) == sizeof(Array_Conformability_Info_Type)(%d) \n",Size,sizeof(Array_Conformability_Info_Type));

          if (Current_Link == NULL)
             {
            // CLASS_ALLOCATION_POOL_SIZE *= 2;
#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Call malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

            // Use new operator instead of malloc to avoid Purify FMM warning
#if 1
               Current_Link = (Array_Conformability_Info_Type*) APP_MALLOC ( CLASS_ALLOCATION_POOL_SIZE * sizeof(Array_Conformability_Info_Type) );
#else
               Current_Link = (Array_Conformability_Info_Type*) new char [ CLASS_ALLOCATION_POOL_SIZE * sizeof(Array_Conformability_Info_Type) ];
#endif

#if COMPILE_DEBUG_STATEMENTS
               if (APP_DEBUG > 1)
                    printf ("Called malloc for Array Memory_Block_Index = %d \n",Memory_Block_Index);
#endif

#if EXTRA_ERROR_CHECKING
               if (Current_Link == NULL) 
                  { 
                    printf ("ERROR: malloc == NULL in Array::operator new! \n"); 
                    APP_ABORT();
                  } 

            // Initialize the Memory_Block_List to NULL
            // This is used to delete the Memory pool blocks to free memory in use
            // and thus prevent memory-in-use errors from Purify
               if (Memory_Block_Index == 0)
                  {
                    for (int i=0; i < Max_Number_Of_Memory_Blocks-1; i++)
                       Memory_Block_List [i] = NULL;
                  }
#endif

               Memory_Block_List [Memory_Block_Index++] = (unsigned char *) Current_Link;

#if EXTRA_ERROR_CHECKING
            // Bounds checking!
               if (Memory_Block_Index >= Max_Number_Of_Memory_Blocks)
                  {
                    printf ("ERROR: Memory_Block_Index (%d) >= Max_Number_Of_Memory_Blocks (%d) \n",Memory_Block_Index,Max_Number_Of_Memory_Blocks);
                    APP_ABORT();
                  }
#endif

            // Initialize the free list of pointers!
               for (int i=0; i < CLASS_ALLOCATION_POOL_SIZE-1; i++)
                  {
                    Current_Link [i].freepointer = &(Current_Link[i+1]);
                  }

            // Set the pointer of the last one to NULL!
               Current_Link [CLASS_ALLOCATION_POOL_SIZE-1].freepointer = NULL;
             }
        }

  // Save the start of the list and remove the first link and return that
  // first link as the new object!

     Array_Conformability_Info_Type* Forward_Link = Current_Link;
     Current_Link = Current_Link->freepointer;

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 0)
          printf ("Returning from Array_Conformability_Info_Type::operator new! (with address of %p) \n",Forward_Link);
#endif

     return Forward_Link;

  // case of USE_CPP_NEW_DELETE_OPERATORS
#endif
   }
#endif

void Array_Conformability_Info_Type::operator delete ( void *Pointer, size_t sizeOfObject )
   {
#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
        {
          printf ("In Array_Conformability_Info_Type::operator delete: Size(%d)  sizeof(Array_Conformability_Info_Type)(%d) \n",sizeOfObject,sizeof(Array_Conformability_Info_Type));
        }
#endif

#if USE_CPP_NEW_DELETE_OPERATORS
     free (Pointer);
#else
     if (sizeOfObject != sizeof(Array_Conformability_Info_Type))
        {
       // Overture's Grid Function class derives from A++/P++ array objects
       // and so must be able to return a valid pointer to memory when using 
       // even the A++ or P++ new operator.

       // If this is an object derived from Array_Conformability_Info_Type
       // then we can't do anything with memory pools from here!
       // It would have to be done within the context of the derived objects
       // operator new!  So we just return the following!

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In Array_Conformability_Info_Type::operator delete: Calling global delete (free) because Size(%d) != sizeof(Array_Conformability_Info_Type)(%d) \n",sizeOfObject,sizeof(Array_Conformability_Info_Type));
             }
#endif

          free(Pointer);
        }
       else
        {
          Array_Conformability_Info_Type *New_Link = (Array_Conformability_Info_Type*) Pointer;

#if COMPILE_DEBUG_STATEMENTS
          if (APP_DEBUG > 1)
             {
               printf ("In Array_Conformability_Info_Type::operator delete (%p): Using the pool mechanism Size(%d) == sizeof(Array_Conformability_Info_Type)(%d) \n",Pointer,sizeOfObject,sizeof(Array_Conformability_Info_Type));
             }
#endif
          if (New_Link != NULL)
             {
            // purify error checking
               APP_ASSERT ( (New_Link->freepointer != NULL) || (New_Link->freepointer == NULL) );
               APP_ASSERT ( (Current_Link != NULL) || (Current_Link == NULL) );
               APP_ASSERT ( (New_Link != NULL) || (New_Link == NULL) );

            // Put deleted object (New_Link) at front of linked list (Current_Link)!
               New_Link->freepointer = Current_Link;
               Current_Link = New_Link;
             }
#if EXTRA_ERROR_CHECKING
            else
             {
               printf ("ERROR: In Array_Conformability_Info_Type::operator delete - attempt made to delete a NULL pointer! \n");
               APP_ABORT();
             }
#endif
        }

#if COMPILE_DEBUG_STATEMENTS
     if (APP_DEBUG > 1)
          printf ("Leaving Array_Conformability_Info_Type::operator delete! \n");
#endif

  // case of USE_CPP_NEW_DELETE_OPERATORS FALSE
#endif
   }








//----------------------------------------------------------------------



//--------------------------------------------------------------------



//---------------------------------------------------------------








//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//NNNN NEW LIST VERSION OF TYPED MESSAGE PASSING INTERPRITATION NNNN

// This new version  of this macro have several changes.  First 
// the Rhs Parallel and Serial arrays have been replaced by 
// List_Of_ParallelArrays and List_Of_SerialArrays.  The 
// code for the VSG and Overlapping Boundary Models have been
// placed inside macros.  Inside these two macros and this macro
// the items in the lists are refered to as Other_ParallelArray
// and Other_SerialArray.  These would have been Rhs_ParallelArray
// and Rhs_SerialArray previuosly.  The macros for the VSG and the
// Overlapping boundary models still pairwise.  The Lhs_ParallelArray
// and Lhs_SerialArray have been replaced by Matching_ParallelArray
// and Matching_SerialArray.  These name changes were made to avoid
// confusion when more than 2 objects are made conformable and also
// because the above explaination about what arrays correspond to
// what new names is not entirely true which will be explained later.
// A Mask_ParallelArray and Mask_SerialArray is also optionally
// added as long as pointers to these objects are not NULL.  
// The function Interpret_Message_Passing is called pairwise with
// the Matching arrays and the arrays in the list.  This function
// has been modified so that instead of modifying the
// Partition_Indexes it stores the changes to the Matching_Partition_Index
// in Return_Array_Set and leaves Matching_Partition_Index unchanged.
// Since the required changes to Matching_Partition_Index might 
// not be the final changes when all of the changes for the whole
// list are intersected the Other_Partition_Index is not changed
// nor are the required changes stored.  This must be recomputed
// later and so the changes are made then.  If there is a Mask array
// it is treated just like another array on the list with it's
// Partition_Index contributing to the intersection also.  Once the
// communication type is found for each pair and the intersection
// computed the SerialArrays are formed as before for each array on
// the list and the Mask_SerialArray if needed.  If the VSG model
// is required the temporary array it creates will be conformable
// with the intersection required for the Overlapping Boundary
// Model.  The Partition_Index for each of these arrays is
// initialized based on the Local_Mask_Index.  If an 
// Array_Conformability_Info_Type object is attached the Partition_Index
// is adjusted.  Because of this a new Array_Conformability_Info_Type
// is created for Return_Array_Set even if an old one is attached to
// one of the arrays.  The Local_Mask_Index is not modified at the
// end so there are no side effects but the changes to the
// Matching_Index_Partition are stored in Return_Array_Set which
// will replace the Array_Conformability_Info_Type already attached.
// (WARNING: NOT DONE YET JUST REALIZED THIS IS ALWAYS ATTACHED TO
// LHS)
//    The reason why the names correspondence described above
// is not quite true is because of the following example. Given 
// the statement where (Mask) A = B; suppose A is a Null_Array and
// Mask and B are not.  Mask and B still must have conformable
// SerialArrays.  In this case instead of making A the Matching Array
// B is the Matching Array and A is put on the list. (This
// isn't done in this macro.) The Mask could have been made the
// Matching Array but this is more complicated for other examples.
// Later (with expression templates) all arrays in 
// A = B+C+D; must have conformable SerialArrays.  This can't be
// tested yet but this should be handeled correctly by this macro.
// If A is a Null_Array then another array is made the Matching Array.
// This macro assumes that either there is one Null_Array or that
// all arrays are Null_Arrays.


// Two operand message passing interpretation support


// NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
// NNNNNN NEW FULL VSG MACRO NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN



//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
//NNNNNNNNNN NEW OVERLAPING BOUNDARY MACRO NNNNNNNNNNNNNNNNNNNNNNNNN



//NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN























